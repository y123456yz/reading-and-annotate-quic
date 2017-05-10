// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "net/tools/quic/quic_dispatcher.h"

#include <utility>

#include "base/debug/stack_trace.h"
#include "base/logging.h"
#include "base/stl_util.h"
#include "net/quic/quic_utils.h"
#include "net/tools/quic/quic_time_wait_list_manager.h"

namespace net {

namespace tools {

using base::StringPiece;

namespace {

// An alarm that informs the QuicDispatcher to delete old sessions.
class DeleteSessionsAlarm : public QuicAlarm::Delegate {
 public:
  explicit DeleteSessionsAlarm(QuicDispatcher* dispatcher)
      : dispatcher_(dispatcher) {
  }

  QuicTime OnAlarm() override {
    dispatcher_->DeleteSessions();
    // Let the dispatcher register the alarm at appropriate time.
    return QuicTime::Zero();
  }

 private:
  // Not owned.
  QuicDispatcher* dispatcher_;

  DISALLOW_COPY_AND_ASSIGN(DeleteSessionsAlarm);
};

}  // namespace

class QuicDispatcher::QuicFramerVisitor : public QuicFramerVisitorInterface {
 public:
  explicit QuicFramerVisitor(QuicDispatcher* dispatcher)
      : dispatcher_(dispatcher),
        connection_id_(0) {}

  // QuicFramerVisitorInterface implementation
  void OnPacket() override {}
  bool OnUnauthenticatedPublicHeader(
      const QuicPacketPublicHeader& header) override {
    connection_id_ = header.connection_id;
    return dispatcher_->OnUnauthenticatedPublicHeader(header);
  }
  bool OnUnauthenticatedHeader(const QuicPacketHeader& header) override {
    dispatcher_->OnUnauthenticatedHeader(header);
    return false;
  }
  void OnError(QuicFramer* framer) override {
    DVLOG(1) << QuicUtils::ErrorToString(framer->error());
  }

  bool OnProtocolVersionMismatch(QuicVersion /*received_version*/) override {
    if (dispatcher_->time_wait_list_manager()->IsConnectionIdInTimeWait(
            connection_id_)) {
      // Keep processing after protocol mismatch - this will be dealt with by
      // the TimeWaitListManager.
      return true;
    } else {
      DLOG(DFATAL) << "Version mismatch, connection ID (" << connection_id_
                   << ") not in time wait list.";
      return false;
    }
  }

  // The following methods should never get called because we always return
  // false from OnUnauthenticatedHeader().  As a result, we never process the
  // payload of the packet.
  void OnPublicResetPacket(const QuicPublicResetPacket& /*packet*/) override {
    DCHECK(false);
  }
  void OnVersionNegotiationPacket(
      const QuicVersionNegotiationPacket& /*packet*/) override {
    DCHECK(false);
  }
  void OnDecryptedPacket(EncryptionLevel level) override { DCHECK(false); }
  bool OnPacketHeader(const QuicPacketHeader& /*header*/) override {
    DCHECK(false);
    return false;
  }
  void OnRevivedPacket() override { DCHECK(false); }
  void OnFecProtectedPayload(StringPiece /*payload*/) override {
    DCHECK(false);
  }
 bool OnStreamFrame(const QuicStreamFrame& /*frame*/) override {
    DCHECK(false);
    return false;
  }
  bool OnAckFrame(const QuicAckFrame& /*frame*/) override {
    DCHECK(false);
    return false;
  }
  bool OnStopWaitingFrame(const QuicStopWaitingFrame& /*frame*/) override {
    DCHECK(false);
    return false;
  }
  bool OnPingFrame(const QuicPingFrame& /*frame*/) override {
    DCHECK(false);
    return false;
  }
  bool OnRstStreamFrame(const QuicRstStreamFrame& /*frame*/) override {
    DCHECK(false);
    return false;
  }
  bool OnConnectionCloseFrame(
      const QuicConnectionCloseFrame& /*frame*/) override {
    DCHECK(false);
    return false;
  }
  bool OnGoAwayFrame(const QuicGoAwayFrame& /*frame*/) override {
    DCHECK(false);
    return false;
  }
  bool OnWindowUpdateFrame(const QuicWindowUpdateFrame& /*frame*/) override {
    DCHECK(false);
    return false;
  }
  bool OnBlockedFrame(const QuicBlockedFrame& frame) override {
    DCHECK(false);
    return false;
  }
  void OnFecData(const QuicFecData& /*fec*/) override { DCHECK(false); }
  void OnPacketComplete() override { DCHECK(false); }

 private:
  QuicDispatcher* dispatcher_;

  // Latched in OnUnauthenticatedPublicHeader for use later.
  QuicConnectionId connection_id_;
};

QuicPacketWriter* QuicDispatcher::DefaultPacketWriterFactory::Create(
    QuicPacketWriter* writer,
    QuicConnection* connection) {
  return writer;
}

QuicDispatcher::PacketWriterFactoryAdapter::PacketWriterFactoryAdapter(
    QuicDispatcher* dispatcher)
    : dispatcher_(dispatcher) {}

QuicDispatcher::PacketWriterFactoryAdapter::~PacketWriterFactoryAdapter() {}

QuicPacketWriter* QuicDispatcher::PacketWriterFactoryAdapter::Create(
    QuicConnection* connection) const {
  return dispatcher_->packet_writer_factory_->Create(
      dispatcher_->writer_.get(),
      connection);
}

QuicDispatcher::QuicDispatcher(const QuicConfig& config,
                               const QuicCryptoServerConfig* crypto_config,
                               const QuicVersionVector& supported_versions,
                               PacketWriterFactory* packet_writer_factory,
                               QuicConnectionHelperInterface* helper)
    : config_(config),
      crypto_config_(crypto_config),
      helper_(helper),
      delete_sessions_alarm_(
          helper_->CreateAlarm(new DeleteSessionsAlarm(this))),
      packet_writer_factory_(packet_writer_factory),
      connection_writer_factory_(this),
      supported_versions_(supported_versions),
      current_packet_(nullptr),
      framer_(supported_versions,
              /*unused*/ QuicTime::Zero(),
              /*is_server*/ Perspective::IS_SERVER),
      framer_visitor_(new QuicFramerVisitor(this)) {
  framer_.set_visitor(framer_visitor_.get());
}

QuicDispatcher::~QuicDispatcher() {
  STLDeleteValues(&session_map_);
  STLDeleteElements(&closed_session_list_);
}

void QuicDispatcher::InitializeWithWriter(QuicPacketWriter* writer) {
  DCHECK(writer_ == nullptr);
  writer_.reset(writer);
  time_wait_list_manager_.reset(CreateQuicTimeWaitListManager());
}

void QuicDispatcher::ProcessPacket(const IPEndPoint& server_address,
                                   const IPEndPoint& client_address,
                                   const QuicEncryptedPacket& packet) {
  current_server_address_ = server_address;
  current_client_address_ = client_address;
  current_packet_ = &packet;
  // ProcessPacket will cause the packet to be dispatched in
  // OnUnauthenticatedPublicHeader, or sent to the time wait list manager
  // in OnAuthenticatedHeader.
  framer_.ProcessPacket(packet);
  // TODO(rjshade): Return a status describing if/why a packet was dropped,
  //                and log somehow.  Maybe expose as a varz.
}

bool QuicDispatcher::OnUnauthenticatedPublicHeader(
    const QuicPacketPublicHeader& header) {
  // Port zero is only allowed for unidirectional UDP, so is disallowed by QUIC.
  // Given that we can't even send a reply rejecting the packet, just black hole
  // it.
  if (current_client_address_.port() == 0) {
    return false;
  }

  // The session that we have identified as the one to which this packet
  // belongs.
  QuicServerSession* session = nullptr;
  QuicConnectionId connection_id = header.connection_id;
  SessionMap::iterator it = session_map_.find(connection_id);
  if (it == session_map_.end()) {
    if (time_wait_list_manager_->IsConnectionIdInTimeWait(connection_id)) {
      return HandlePacketForTimeWait(header);
    }

    // The packet has an unknown connection ID.
    // If the packet is a public reset, there is nothing we must do or can do.
    if (header.reset_flag) {
      return false;
    }

    // All packets within a connection sent by a client before receiving a
    // response from the server are required to have the version negotiation
    // flag set.  Since this may be a client continuing a connection we lost
    // track of via server restart, send a rejection to fast-fail the
    // connection.
    if (!header.version_flag) {
      DVLOG(1) << "Packet without version arrived for unknown connection ID "
               << connection_id;
      // Add this connection_id fo the time-wait state, to safely reject future
      // packets.
      QuicVersion version = supported_versions_.front();
      time_wait_list_manager_->AddConnectionIdToTimeWait(connection_id, version,
                                                         nullptr);
      DCHECK(time_wait_list_manager_->IsConnectionIdInTimeWait(connection_id));
      return HandlePacketForTimeWait(header);
    }

    session = AdditionalValidityChecksThenCreateSession(header, connection_id);
    if (session == nullptr) {
      return false;
    }
  } else {
    session = it->second;
  }

  session->connection()->ProcessUdpPacket(
      current_server_address_, current_client_address_, *current_packet_);

  // Do not parse the packet further.  The session methods called above have
  // processed it completely.
  return false;
}

QuicServerSession* QuicDispatcher::AdditionalValidityChecksThenCreateSession(
    const QuicPacketPublicHeader& header,
    QuicConnectionId connection_id) {
  QuicServerSession* session = CreateQuicSession(
      connection_id, current_server_address_, current_client_address_);

  if (session == nullptr) {
    DVLOG(1) << "Failed to create session for " << connection_id;

    if (!framer_.IsSupportedVersion(header.versions.front())) {
      // TODO(ianswett): Produce packet saying "no supported version".
      return nullptr;
    }

    // Add this connection_id to the time-wait state, to safely reject future
    // packets.
    QuicVersion version = header.versions.front();
    time_wait_list_manager_->AddConnectionIdToTimeWait(connection_id, version,
                                                       nullptr);
    DCHECK(time_wait_list_manager_->IsConnectionIdInTimeWait(connection_id));
    HandlePacketForTimeWait(header);

    return nullptr;
  }

  DVLOG(1) << "Created new session for connection ID " << connection_id;
  session_map_.insert(std::make_pair(connection_id, session));

  return session;
}

void QuicDispatcher::OnUnauthenticatedHeader(const QuicPacketHeader& header) {
  DCHECK(time_wait_list_manager_->IsConnectionIdInTimeWait(
      header.public_header.connection_id));
  time_wait_list_manager_->ProcessPacket(current_server_address_,
                                         current_client_address_,
                                         header.public_header.connection_id,
                                         header.packet_sequence_number,
                                         *current_packet_);
}

void QuicDispatcher::CleanUpSession(SessionMap::iterator it) {
  QuicConnection* connection = it->second->connection();
  QuicEncryptedPacket* connection_close_packet =
      connection->ReleaseConnectionClosePacket();
  write_blocked_list_.erase(connection);
  time_wait_list_manager_->AddConnectionIdToTimeWait(it->first,
                                                     connection->version(),
                                                     connection_close_packet);
  session_map_.erase(it);
}

void QuicDispatcher::DeleteSessions() {
  STLDeleteElements(&closed_session_list_);
}

void QuicDispatcher::OnCanWrite() {
  // The socket is now writable.
  writer_->SetWritable();

  // Give all the blocked writers one chance to write, until we're blocked again
  // or there's no work left.
  while (!write_blocked_list_.empty() && !writer_->IsWriteBlocked()) {
    QuicBlockedWriterInterface* blocked_writer =
        write_blocked_list_.begin()->first;
    write_blocked_list_.erase(write_blocked_list_.begin());
    blocked_writer->OnCanWrite();
  }
}

bool QuicDispatcher::HasPendingWrites() const {
  return !write_blocked_list_.empty();
}

void QuicDispatcher::Shutdown() {
  while (!session_map_.empty()) {
    QuicServerSession* session = session_map_.begin()->second;
    session->connection()->SendConnectionClose(QUIC_PEER_GOING_AWAY);
    // Validate that the session removes itself from the session map on close.
    DCHECK(session_map_.empty() || session_map_.begin()->second != session);
  }
  DeleteSessions();
}

void QuicDispatcher::OnConnectionClosed(QuicConnectionId connection_id,
                                        QuicErrorCode error) {
  SessionMap::iterator it = session_map_.find(connection_id);
  if (it == session_map_.end()) {
    LOG(DFATAL) << "ConnectionId " << connection_id
                << " does not exist in the session map.  "
                << "Error: " << QuicUtils::ErrorToString(error);
    LOG(DFATAL) << base::debug::StackTrace().ToString();
    return;
  }

  DVLOG_IF(1, error != QUIC_NO_ERROR) << "Closing connection ("
                                      << connection_id
                                      << ") due to error: "
                                      << QuicUtils::ErrorToString(error);

  if (closed_session_list_.empty()) {
    delete_sessions_alarm_->Cancel();
    delete_sessions_alarm_->Set(helper()->GetClock()->ApproximateNow());
  }
  closed_session_list_.push_back(it->second);
  CleanUpSession(it);
}

void QuicDispatcher::OnWriteBlocked(
    QuicBlockedWriterInterface* blocked_writer) {
  if (!writer_->IsWriteBlocked()) {
    LOG(DFATAL) <<
        "QuicDispatcher::OnWriteBlocked called when the writer is not blocked.";
    // Return without adding the connection to the blocked list, to avoid
    // infinite loops in OnCanWrite.
    return;
  }
  write_blocked_list_.insert(std::make_pair(blocked_writer, true));
}

void QuicDispatcher::OnConnectionAddedToTimeWaitList(
    QuicConnectionId connection_id) {
  DVLOG(1) << "Connection " << connection_id << " added to time wait list.";
}

void QuicDispatcher::OnConnectionRemovedFromTimeWaitList(
    QuicConnectionId connection_id) {
  DVLOG(1) << "Connection " << connection_id << " removed from time wait list.";
}

QuicServerSession* QuicDispatcher::CreateQuicSession(
    QuicConnectionId connection_id,
    const IPEndPoint& server_address,
    const IPEndPoint& client_address) {
  // The QuicServerSession takes ownership of |connection| below.
  QuicConnection* connection = new QuicConnection(
      connection_id, client_address, helper_.get(), connection_writer_factory_,
      /* owns_writer= */ false, /*is_server*/ Perspective::IS_SERVER,
      crypto_config_->HasProofSource(), supported_versions_);

  QuicServerSession* session = new QuicServerSession(config_, connection, this, helper_.get());
  session->InitializeSession(crypto_config_);
  return session;
}

QuicTimeWaitListManager* QuicDispatcher::CreateQuicTimeWaitListManager() {
  return new QuicTimeWaitListManager(
      writer_.get(), this, helper_.get(), supported_versions());
}

bool QuicDispatcher::HandlePacketForTimeWait(
    const QuicPacketPublicHeader& header) {
  if (header.reset_flag) {
    // Public reset packets do not have sequence numbers, so ignore the packet.
    return false;
  }

  // Switch the framer to the correct version, so that the sequence number can
  // be parsed correctly.
  framer_.set_version(time_wait_list_manager_->GetQuicVersionFromConnectionId(
      header.connection_id));

  // Continue parsing the packet to extract the sequence number.  Then
  // send it to the time wait manager in OnUnathenticatedHeader.
  return true;
}

}  // namespace tools
}  // namespace net

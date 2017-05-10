// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// A toy client, which connects to a specified port and sends QUIC
// request to that endpoint.

#ifndef NET_TOOLS_QUIC_QUIC_CLIENT_H_
#define NET_TOOLS_QUIC_QUIC_CLIENT_H_

#include <string>

#include "base/basictypes.h"
#include "base/command_line.h"
#include "base/memory/scoped_ptr.h"
#include "base/strings/string_piece.h"
#include "net/base/ip_endpoint.h"
#include "net/quic/crypto/crypto_handshake.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_framer.h"
#include "net/quic/quic_packet_creator.h"
#include "net/tools/epoll_server/epoll_server.h"
#include "net/tools/quic/quic_client_session.h"

namespace net {

class ProofVerifier;
class QuicServerId;

namespace tools {

class QuicEpollConnectionHelper;

class QuicClient : public EpollCallbackInterface,
                   public QuicDataStream::Visitor {
 public:
  // Create a quic client, which will have events managed by an externally owned
  // EpollServer.
  QuicClient(IPEndPoint server_address,
             const QuicServerId& server_id,
             const QuicVersionVector& supported_versions,
             EpollServer* epoll_server);

  ~QuicClient() override;

  // Initializes the client to create a connection. Should be called exactly
  // once before calling StartConnect or Connect. Returns true if the
  // initialization succeeds, false otherwise.
  bool Initialize();

  // "Connect" to the QUIC server, including performing synchronous crypto
  // handshake.
  bool Connect();

  QuicClientStream* CreateClientStream();

  // Disconnects from the QUIC server.
  void Disconnect();

  // From EpollCallbackInterface
  void OnRegistration(EpollServer* eps, int fd, int event_mask) override {}
  void OnModification(int fd, int event_mask) override {}
  void OnEvent(int fd, EpollEvent* event) override;
  // |fd_| can be unregistered without the client being disconnected. This
  // happens in b3m QuicProber where we unregister |fd_| to feed in events to
  // the client from the SelectServer.
  void OnUnregistration(int fd, bool replaced) override {}
  void OnShutdown(EpollServer* eps, int fd) override {}

  void WaitForEvents();

  void OnClose(QuicDataStream* stream) {}

  bool connected() const;

 private:
  EpollServer* epoll_server() { return epoll_server_; }

  // A packet writer factory that always returns the same writer
  class DummyPacketWriterFactory : public QuicConnection::PacketWriterFactory {
   public:
    explicit DummyPacketWriterFactory(QuicPacketWriter* writer);
    ~DummyPacketWriterFactory() override;

    QuicPacketWriter* Create(QuicConnection* connection) const override;

   private:
    QuicPacketWriter* writer_;
  };

  // Used during initialization: creates the UDP socket FD, sets socket options,
  // and binds the socket to our address.
  bool CreateUDPSocket();

  // Read a UDP packet and hand it to the framer.
  bool ReadAndProcessPacket();

  // Address of the server.
  const IPEndPoint server_address_;

  // |server_id_| is a tuple (hostname, port, is_https) of the server.
  QuicServerId server_id_;

  // config_ and crypto_config_ contain configuration and cached state about
  // servers.
  QuicConfig config_;
  QuicCryptoClientConfig crypto_config_;

  // Address of the client if the client is connected to the server.
  IPEndPoint client_address_;

  // If initialized, the address to bind to.
  IPAddressNumber bind_to_address_;
  // Local port to bind to. Initialize to 0.
  int local_port_;

  // Writer used to actually send packets to the wire. Needs to outlive
  // |session_|.
  scoped_ptr<QuicPacketWriter> writer_;

  // Session which manages streams.
  scoped_ptr<QuicClientSession> session_;
  // Listens for events on the client socket.
  EpollServer* epoll_server_;
  // UDP socket.
  int fd_;

  // Helper to be used by created connections.
  scoped_ptr<QuicEpollConnectionHelper> helper_;

  // Tracks if the client is initialized to connect.
  bool initialized_;

  // If overflow_supported_ is true, this will be the number of packets dropped
  // during the lifetime of the server.
  QuicPacketCount packets_dropped_;

  // True if the kernel supports SO_RXQ_OVFL, the number of packets dropped
  // because the socket would otherwise overflow.
  bool overflow_supported_;

  // This vector contains QUIC versions which we currently support.
  // This should be ordered such that the highest supported version is the first
  // element, with subsequent elements in descending order (versions can be
  // skipped as necessary). We will always pick supported_versions_[0] as the
  // initial version to use.
  QuicVersionVector supported_versions_;

  DISALLOW_COPY_AND_ASSIGN(QuicClient);
};

}  // namespace tools
}  // namespace net

#endif  // NET_TOOLS_QUIC_QUIC_CLIENT_H_

// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// A toy server, which listens on a specified address for QUIC traffic and
// handles incoming responses.

#ifndef NET_TOOLS_QUIC_QUIC_SERVER_H_
#define NET_TOOLS_QUIC_QUIC_SERVER_H_

#include "base/basictypes.h"
#include "base/memory/scoped_ptr.h"
#include "net/base/ip_endpoint.h"
#include "net/quic/crypto/quic_crypto_server_config.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_framer.h"
#include "net/tools/epoll_server/epoll_server.h"
#include "net/tools/quic/quic_default_packet_writer.h"

namespace net {
namespace tools {

class ProcessPacketInterface;
class QuicDispatcher;
class QuicPacketReader;

class QuicServer : public EpollCallbackInterface {
 public:
  QuicServer(const QuicConfig& config,
             const QuicVersionVector& supported_versions);

  ~QuicServer() override;

  // Start listening on the specified address.
  bool Listen(const IPEndPoint& address);

  // Wait up to 50ms, and handle any events which occur.
  void WaitForEvents();

  // Server deletion is imminent.  Start cleaning up the epoll server.
  void Shutdown();

  // From EpollCallbackInterface
  void OnRegistration(EpollServer* eps, int fd, int event_mask) override {}
  void OnModification(int fd, int event_mask) override {}
  void OnEvent(int fd, EpollEvent* event) override;
  void OnUnregistration(int fd, bool replaced) override {}

  // Reads a packet from the given fd, and then passes it off to
  // the QuicDispatcher.
  // If packets_dropped is non-null, the socket is configured to track
  // dropped packets, and some packets are read, it will be set to the number of
  // dropped packets.
  // Same as ReadAndDispatchPackets, only does one packet at a time.
  static bool ReadAndDispatchSinglePacket(int fd, int port,
                                          ProcessPacketInterface* processor,
                                          QuicPacketCount* packets_dropped);

  void OnShutdown(EpollServer* eps, int fd) override {}


  bool overflow_supported() { return overflow_supported_; }

  QuicPacketCount packets_dropped() { return packets_dropped_; }

  int port() { return port_; }

 protected:
  const QuicConfig& config() const { return config_; }
  const QuicCryptoServerConfig& crypto_config() const {
    return crypto_config_;
  }
  const QuicVersionVector& supported_versions() const {
    return supported_versions_;
  }
  EpollServer* epoll_server() { return &epoll_server_; }

  QuicDispatcher* dispatcher() { return dispatcher_.get(); }

 private:
  // Accepts data from the framer and demuxes clients to sessions.
  scoped_ptr<QuicDispatcher> dispatcher_;
  // Frames incoming packets and hands them to the dispatcher.
  EpollServer epoll_server_;

  // The port the server is listening on.
  int port_;

  // Listening connection.  Also used for outbound client communication.
  int fd_;

  // If overflow_supported_ is true this will be the number of packets dropped
  // during the lifetime of the server.  This may overflow if enough packets
  // are dropped.
  QuicPacketCount packets_dropped_;

  // True if the kernel supports SO_RXQ_OVFL, the number of packets dropped
  // because the socket would otherwise overflow.
  bool overflow_supported_;

  // config_ contains non-crypto parameters that are negotiated in the crypto
  // handshake.
  QuicConfig config_;
  // crypto_config_ contains crypto parameters for the handshake.
  QuicCryptoServerConfig crypto_config_;

  // This vector contains QUIC versions which we currently support.
  // This should be ordered such that the highest supported version is the first
  // element, with subsequent elements in descending order (versions can be
  // skipped as necessary).
  QuicVersionVector supported_versions_;

  DISALLOW_COPY_AND_ASSIGN(QuicServer);
};

}  // namespace tools
}  // namespace net

#endif  // NET_TOOLS_QUIC_QUIC_SERVER_H_

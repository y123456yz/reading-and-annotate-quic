// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// A client specific QuicSession subclass.

#ifndef NET_TOOLS_QUIC_QUIC_CLIENT_SESSION_H_
#define NET_TOOLS_QUIC_QUIC_CLIENT_SESSION_H_

#include <string>

#include "base/basictypes.h"
#include "net/quic/quic_client_session_base.h"
#include "net/quic/quic_crypto_client_stream.h"
#include "net/quic/quic_protocol.h"
#include "net/tools/quic/quic_client_stream.h"

namespace net {

class QuicConnection;
class QuicServerId;
class ReliableQuicStream;

namespace tools {

class QuicClientSession : public QuicSession {
 public:
  QuicClientSession(const QuicConfig& config, QuicConnection* connection);
  ~QuicClientSession() override;

  void InitializeSession(const QuicServerId& server_id,
                         QuicCryptoClientConfig* config);
  
  void CryptoConnect();

  QuicClientStream* CreateClientStream();

  QuicCryptoStream* GetCryptoStream() { return crypto_stream_.get(); };

  virtual ReliableQuicStream* CreateOutgoingDynamicStream() { return nullptr; };
  virtual ReliableQuicStream* CreateIncomingDynamicStream(QuicStreamId id) { return nullptr; };
  // has to be implemented for secure stream
  void OnProofValid(const QuicCryptoClientConfig::CachedState&) {};
  void OnProofVerifyDetailsAvailable(const ProofVerifyDetails&) {};

 private:
  scoped_ptr<QuicCryptoClientStream> crypto_stream_;

};

}  // namespace tools
}  // namespace net

#endif  // NET_TOOLS_QUIC_QUIC_CLIENT_SESSION_H_

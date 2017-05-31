// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef NET_QUIC_QUIC_CRYPTO_CLIENT_STREAM_H_
#define NET_QUIC_QUIC_CRYPTO_CLIENT_STREAM_H_

#include <string>

#include "net/quic/crypto/channel_id.h"
#include "net/quic/crypto/proof_verifier.h"
#include "net/quic/crypto/quic_crypto_client_config.h"
#include "net/quic/quic_config.h"
#include "net/quic/quic_crypto_stream.h"
#include "net/quic/quic_server_id.h"

namespace net {

class QuicClientSessionBase;

namespace test {
class CryptoTestUtils;
class QuicClientSessionPeer;
}  // namespace test

class NET_EXPORT_PRIVATE QuicCryptoClientStream : public QuicCryptoStream {
 public:
  QuicCryptoClientStream(const QuicServerId& server_id,
                        // QuicClientSessionBase* session,
                         QuicSession* session,
                         ProofVerifyContext* verify_context,
                         QuicCryptoClientConfig* crypto_config);
  ~QuicCryptoClientStream() override;

  // CryptoFramerVisitorInterface implementation
  void OnHandshakeMessage(const CryptoHandshakeMessage& message) override;

  // Performs a crypto handshake with the server.
  virtual void CryptoConnect();

  // num_sent_client_hellos returns the number of client hello messages that
  // have been sent. If the handshake has completed then this is one greater
  // than the number of round-trips needed for the handshake.
  int num_sent_client_hellos() const;

  // Returns true if a channel ID was sent on this connection.
  bool WasChannelIDSent() const;

  // Returns true if our ChannelIDSourceCallback was run, which implies the
  // ChannelIDSource operated asynchronously. Intended for testing.
  bool WasChannelIDSourceCallbackRun() const;

 private:
  // ChannelIDSourceCallbackImpl is passed as the callback method to
  // GetChannelIDKey. The ChannelIDSource calls this class with the result of
  // channel ID lookup when lookup is performed asynchronously.
  class ChannelIDSourceCallbackImpl : public ChannelIDSourceCallback {
   public:
    explicit ChannelIDSourceCallbackImpl(QuicCryptoClientStream* stream);
    ~ChannelIDSourceCallbackImpl() override;

    // ChannelIDSourceCallback interface.
    void Run(scoped_ptr<ChannelIDKey>* channel_id_key) override;

    // Cancel causes any future callbacks to be ignored. It must be called on
    // the same thread as the callback will be made on.
    void Cancel();

   private:
    QuicCryptoClientStream* stream_;
  };

  // ProofVerifierCallbackImpl is passed as the callback method to VerifyProof.
  // The ProofVerifier calls this class with the result of proof verification
  // when verification is performed asynchronously.
  class ProofVerifierCallbackImpl : public ProofVerifierCallback {
   public:
    explicit ProofVerifierCallbackImpl(QuicCryptoClientStream* stream);
    ~ProofVerifierCallbackImpl() override;

    // ProofVerifierCallback interface.
    void Run(bool ok,
             const std::string& error_details,
             scoped_ptr<ProofVerifyDetails>* details) override;

    // Cancel causes any future callbacks to be ignored. It must be called on
    // the same thread as the callback will be made on.
    void Cancel();

   private:
    QuicCryptoClientStream* stream_;
  };

  friend class test::CryptoTestUtils;
  friend class test::QuicClientSessionPeer;
  /*
  0-RTT握手过程
     QUIC握手的过程是需要一次数据交互，0-RTT时延即可完成握手过程中的密钥协商，比TLS相比效率提高了5倍，且具有更高的安全性。
     QUIC在握手过程中使用Diffie-Hellman算法协商初始密钥，初始密钥依赖于服务器存储的一组配置参数，该参数会周期性的更新。
     初始密钥协商成功后，服务器会提供一个临时随机数，双方根据这个数再生成会话密钥。
     具体握手过程如下：
     (1) 客户端判断本地是否已有服务器的全部配置参数，如果有则直接跳转到(5)，否则继续
     (2) 客户端向服务器发送inchoate client hello(CHLO)消息，请求服务器传输配置参数
     (3) 服务器收到CHLO，回复rejection(REJ)消息，其中包含服务器的部分配置参数
     (4) 客户端收到REJ，提取并存储服务器配置参数，跳回到(1) 
     (5) 客户端向服务器发送full client hello消息，开始正式握手，消息中包括客户端选择的公开数。此时客户
         端根据获取的服务器配置参数和自己选择的公开数，可以计算出初始密钥。
     (6) 服务器收到full client hello，如果不同意连接就回复REJ，同(3)；如果同意连接，根据客户端的公开数
         计算出初始密钥，回复server hello(SHLO)消息，SHLO用初始密钥加密，并且其中包含服务器选择的一个临时公开数。
     (7) 客户端收到服务器的回复，如果是REJ则情况同(4)；如果是SHLO，则尝试用初始密钥解密，提取出临时公开数
     (8) 客户端和服务器根据临时公开数和初始密钥，各自基于SHA-256算法推导出会话密钥
     (9) 双方更换为使用会话密钥通信，初始密钥此时已无用，QUIC握手过程完毕。之后会话密钥更新的流程与以上过程类似，
         只是数据包中的某些字段略有不同。
  */
  enum State {
    STATE_IDLE,
    STATE_INITIALIZE,  //1
    //客户端发送client hello
    STATE_SEND_CHLO,    
    //客户端发送client hello后等待接收服务端REJ
    STATE_RECV_REJ,
    STATE_VERIFY_PROOF,
    STATE_VERIFY_PROOF_COMPLETE,  //5
    STATE_GET_CHANNEL_ID,
    STATE_GET_CHANNEL_ID_COMPLETE,
    STATE_RECV_SHLO,    //8
    STATE_INITIALIZE_SCUP,
    STATE_NONE,
  };

  // Handles new server config and optional source-address token provided by the
  // server during a connection.
  void HandleServerConfigUpdateMessage(
      const CryptoHandshakeMessage& server_config_update);

  // DoHandshakeLoop performs a step of the handshake state machine. Note that
  // |in| may be nullptr if the call did not result from a received message.
  void DoHandshakeLoop(const CryptoHandshakeMessage* in);

  // Start the handshake process.
  void DoInitialize(QuicCryptoClientConfig::CachedState* cached);

  // Send either InchoateClientHello or ClientHello message to the server.
  void DoSendCHLO(const CryptoHandshakeMessage* in,
                  QuicCryptoClientConfig::CachedState* cached);

  // Process REJ message from the server.
  void DoReceiveREJ(const CryptoHandshakeMessage* in,
                    QuicCryptoClientConfig::CachedState* cached);

  // Start the proof verification process. Returns the QuicAsyncStatus returned
  // by the ProofVerifier's VerifyProof.
  QuicAsyncStatus DoVerifyProof(QuicCryptoClientConfig::CachedState* cached);

  // If proof is valid then it sets the proof as valid (which persists the
  // server config). If not, it closes the connection.
  void DoVerifyProofComplete(QuicCryptoClientConfig::CachedState* cached);

  // Start the look up of Channel ID process. Returns either QUIC_SUCCESS if
  // RequiresChannelID returns false or QuicAsyncStatus returned by
  // GetChannelIDKey.
  QuicAsyncStatus DoGetChannelID(QuicCryptoClientConfig::CachedState* cached);

  // If there is no channel ID, then close the connection otherwise transtion to
  // STATE_SEND_CHLO state.
  void DoGetChannelIDComplete();

  // Process SHLO message from the server.
  void DoReceiveSHLO(const CryptoHandshakeMessage* in,
                     QuicCryptoClientConfig::CachedState* cached);

  // Start the proof verification if |server_id_| is https and |cached| has
  // signature.
  void DoInitializeServerConfigUpdate(
      QuicCryptoClientConfig::CachedState* cached);

  // Called to set the proof of |cached| valid.  Also invokes the session's
  // OnProofValid() method.
  void SetCachedProofValid(QuicCryptoClientConfig::CachedState* cached);

  // Returns true if the server crypto config in |cached| requires a ChannelID
  // and the client config settings also allow sending a ChannelID.
  bool RequiresChannelID(QuicCryptoClientConfig::CachedState* cached);

  //QuicClientSessionBase* client_session();

  State next_state_;
  // num_client_hellos_ contains the number of client hello messages that this
  // connection has sent.
  //发送的client hello次数，DoSendCHLO函数发送client hello后自增
  int num_client_hellos_;

  QuicCryptoClientConfig* const crypto_config_;

  // Client's connection nonce (4-byte timestamp + 28 random bytes)
  std::string nonce_;
  // Server's (hostname, port, is_https, privacy_mode) tuple.
  const QuicServerId server_id_;

  // Generation counter from QuicCryptoClientConfig's CachedState.
  uint64 generation_counter_;

  // True if a channel ID was sent.
  bool channel_id_sent_;

  // True if channel_id_source_callback_ was run.
  bool channel_id_source_callback_run_;

  // channel_id_source_callback_ contains the callback object that we passed
  // to an asynchronous channel ID lookup. The ChannelIDSource owns this
  // object.
  ChannelIDSourceCallbackImpl* channel_id_source_callback_;

  // These members are used to store the result of an asynchronous channel ID
  // lookup. These members must not be used after
  // STATE_GET_CHANNEL_ID_COMPLETE.
  scoped_ptr<ChannelIDKey> channel_id_key_;

  // verify_context_ contains the context object that we pass to asynchronous
  // proof verifications.
  scoped_ptr<ProofVerifyContext> verify_context_;

  // proof_verify_callback_ contains the callback object that we passed to an
  // asynchronous proof verification. The ProofVerifier owns this object.
  ProofVerifierCallbackImpl* proof_verify_callback_;

  // These members are used to store the result of an asynchronous proof
  // verification. These members must not be used after
  // STATE_VERIFY_PROOF_COMPLETE.
  bool verify_ok_;
  std::string verify_error_details_;
  scoped_ptr<ProofVerifyDetails> verify_details_;

  // True if the server responded to a previous CHLO with a stateless
  // reject.  Used for book-keeping between the STATE_RECV_REJ,
  // STATE_VERIFY_PROOF*, and subsequent STATE_SEND_CHLO state.
  bool stateless_reject_received_;

  DISALLOW_COPY_AND_ASSIGN(QuicCryptoClientStream);
};

}  // namespace net

#endif  // NET_QUIC_QUIC_CRYPTO_CLIENT_STREAM_H_

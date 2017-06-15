// Copyright (c) 2012 The Chromium Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.
//
// The base class for client/server reliable streams.

#ifndef NET_QUIC_RELIABLE_QUIC_STREAM_H_
#define NET_QUIC_RELIABLE_QUIC_STREAM_H_

#include <sys/types.h>

#include <list>
#include <string>

#include "base/basictypes.h"
#include "base/memory/ref_counted.h"
#include "base/strings/string_piece.h"
#include "net/base/iovec.h"
#include "net/base/net_export.h"
#include "net/quic/quic_ack_notifier.h"
#include "net/quic/quic_flow_controller.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_stream_sequencer.h"
#include "net/quic/quic_types.h"

namespace net {

namespace test {
class ReliableQuicStreamPeer;
}  // namespace test

class QuicSession;


/*
QuicSession(QuicServerSession  QuicClientSession  QuicSpdySession)类和ReliableQuicStream(QuicClientStream  
QuicServerStream  QuicCryptoStream  QuicHeadersStream  QuicDataStream)类通过ReliableQuicStream.session_和
QuicServerSession.crypto_stream_成员联系起来

QuicSession类代表的是一个connection，ReliableQuicStream代表的是一条流信息
*/

//QuicClientStream  QuicServerStream  QuicCryptoStream  QuicHeadersStream  QuicDataStream继承该类
class NET_EXPORT_PRIVATE ReliableQuicStream {
 public:
  ReliableQuicStream(QuicStreamId id,
                     QuicSession* session);

  virtual ~ReliableQuicStream();

  // Called when a (potentially duplicate) stream frame has been received
  // for this stream.
  virtual void OnStreamFrame(const QuicStreamFrame& frame);

  // Called when the connection becomes writeable to allow the stream
  // to write any pending data.
  virtual void OnCanWrite();

  // Called by the session just before the stream is deleted.
  virtual void OnClose();

  // Called when we get a stream reset from the peer.
  virtual void OnStreamReset(const QuicRstStreamFrame& frame);

  // Called when we get or send a connection close, and should immediately
  // close the stream.  This is not passed through the sequencer,
  // but is handled immediately.
  virtual void OnConnectionClosed(QuicErrorCode error, bool from_peer);

  // Called when the final data has been read.
  virtual void OnFinRead();

  virtual uint32 ProcessRawData(const char* data, uint32 data_len) = 0;

  // Called to reset the stream from this end.
  virtual void Reset(QuicRstStreamErrorCode error);

  // Called to close the entire connection from this end.
  virtual void CloseConnection(QuicErrorCode error);
  virtual void CloseConnectionWithDetails(QuicErrorCode error,
                                          const std::string& details);

  // Returns the effective priority for the stream.  This value may change
  // during the life of the stream.
  virtual QuicPriority EffectivePriority() const = 0;

  QuicStreamId id() const { return id_; }

  QuicRstStreamErrorCode stream_error() const { return stream_error_; }
  QuicErrorCode connection_error() const { return connection_error_; }

  bool read_side_closed() const { return read_side_closed_; }
  bool write_side_closed() const { return write_side_closed_; }

  uint64 stream_bytes_read() const { return stream_bytes_read_; }
  uint64 stream_bytes_written() const { return stream_bytes_written_; }

  void set_fin_sent(bool fin_sent) { fin_sent_ = fin_sent; }
  void set_rst_sent(bool rst_sent) { rst_sent_ = rst_sent; }

  void set_fec_policy(FecPolicy fec_policy) { fec_policy_ = fec_policy; }
  FecPolicy fec_policy() const { return fec_policy_; }

  // Adjust our flow control windows according to new offset in |frame|.
  virtual void OnWindowUpdateFrame(const QuicWindowUpdateFrame& frame);

  // Used in Chrome.
  int num_frames_received() const;
  int num_early_frames_received() const;
  int num_duplicate_frames_received() const;

  QuicFlowController* flow_controller() { return &flow_controller_; }

  // Called when we see a frame which could increase the highest offset.
  // Returns true if the highest offset did increase.
  bool MaybeIncreaseHighestReceivedOffset(QuicStreamOffset new_offset);
  // Called when bytes are sent to the peer.
  void AddBytesSent(QuicByteCount bytes);
  // Called by the stream sequencer as bytes are consumed from the buffer.
  // If our receive window has dropped below the threshold, then send a
  // WINDOW_UPDATE frame.
  void AddBytesConsumed(QuicByteCount bytes);

  // Updates the flow controller's send window offset and calls OnCanWrite if
  // it was blocked before.
  void UpdateSendWindowOffset(QuicStreamOffset new_offset);

  // Returns true if we have received either a RST or a FIN - either of which
  // gives a definitive number of bytes which the peer has sent. If this is not
  // true on stream termination the session must keep track of the stream's byte
  // offset until a definitive final value arrives.
  bool HasFinalReceivedByteOffset() const {
    return fin_received_ || rst_received_;
  }

  // Returns true if the stream has queued data waiting to write.
  bool HasBufferedData() const;

  // Returns the version of QUIC being used for this stream.
  QuicVersion version() const;

 protected:
  // Sends as much of 'data' to the connection as the connection will consume,
  // and then buffers any remaining data in queued_data_.
  void WriteOrBufferData(
      base::StringPiece data,
      bool fin,
      QuicAckNotifier::DelegateInterface* ack_notifier_delegate);

  // Sends as many bytes in the first |count| buffers of |iov| to the connection
  // as the connection will consume.
  // If |ack_notifier_delegate| is provided, then it will be notified once all
  // the ACKs for this write have been received.
  // Returns the number of bytes consumed by the connection.
  QuicConsumedData WritevData(
      const struct iovec* iov,
      int iov_count,
      bool fin,
      QuicAckNotifier::DelegateInterface* ack_notifier_delegate);

  // Helper method that returns FecProtection to use for writes to the session.
  FecProtection GetFecProtection();

  // Close the read side of the socket.  Further frames will not be accepted.
  virtual void CloseReadSide();

  // Close the write side of the socket.  Further writes will fail.
  void CloseWriteSide();

  bool fin_buffered() const { return fin_buffered_; }

  const QuicSession* session() const { return session_; }
  QuicSession* session() { return session_; }

  const QuicStreamSequencer* sequencer() const { return &sequencer_; }
  QuicStreamSequencer* sequencer() { return &sequencer_; }

  void DisableConnectionFlowControlForThisStream() {
    stream_contributes_to_connection_flow_control_ = false;
  }

 private:
  friend class test::ReliableQuicStreamPeer;
  friend class QuicStreamUtils;
  class ProxyAckNotifierDelegate;

  struct PendingData {
    PendingData(std::string data_in,
                scoped_refptr<ProxyAckNotifierDelegate> delegate_in);
    ~PendingData();

    // Pending data to be written.
    std::string data; //待写入数据
    // Index of the first byte in data still to be written.
    size_t offset;  //偏移
    // Delegate that should be notified when the pending data is acked.
    // Can be nullptr.
    scoped_refptr<ProxyAckNotifierDelegate> delegate; //发送数据的ack委托处理相关
  };

  // Calls MaybeSendBlocked on our flow controller, and connection level flow
  // controller. If we are flow control blocked, marks this stream as write
  // blocked.
  void MaybeSendBlocked();

  //WriteOrBufferData中入队，OnCanWrite中出对
  std::list<PendingData> queued_data_;

  QuicStreamSequencer sequencer_;
  QuicStreamId id_;
  QuicSession* session_;
  // Bytes read and written refer to payload bytes only: they do not include
  // framing, encryption overhead etc.
  uint64 stream_bytes_read_;
  //默认为0，ReliableQuicStream::WritevData中赋值
  uint64 stream_bytes_written_;

  // Stream error code received from a RstStreamFrame or error code sent by the
  // visitor or sequencer in the RstStreamFrame.
  QuicRstStreamErrorCode stream_error_;
  // Connection error code due to which the stream was closed. |stream_error_|
  // is set to |QUIC_STREAM_CONNECTION_ERROR| when this happens and consumers
  // should check |connection_error_|.
  QuicErrorCode connection_error_;

  // True if the read side is closed and further frames should be rejected.
  bool read_side_closed_;
  // True if the write side is closed, and further writes should fail.
  bool write_side_closed_; //CloseWriteSide赋值，

  //WriteOrBufferData中赋值
  bool fin_buffered_;
  bool fin_sent_;

  // True if this stream has received (and the sequencer has accepted) a
  // StreamFrame with the FIN set.
  bool fin_received_;

  // In combination with fin_sent_, used to ensure that a FIN and/or a RST is
  // always sent before stream termination.
  bool rst_sent_;

  // True if this stream has received a RST stream frame.
  bool rst_received_;

  // FEC policy to be used for this stream.
  FecPolicy fec_policy_; //是否需要数据流支持前向纠错，构造函数中默认为FEC_PROTECT_OPTIONAL

  // Tracks if the session this stream is running under was created by a
  // server or a client.
  Perspective perspective_;

/*
flow_controller_               session级流量控制
connection_flow_controller_    connection级流量控制
*/
  QuicFlowController flow_controller_;

  // The connection level flow controller. Not owned.
  QuicFlowController* connection_flow_controller_;

  // Special streams, such as the crypto and headers streams, do not respect
  // connection level flow control limits (but are stream level flow control
  // limited). 
  //crypto and headers streams不应该收连接等级流量控制，而是受stream级流量控制
  bool stream_contributes_to_connection_flow_control_; 

  DISALLOW_COPY_AND_ASSIGN(ReliableQuicStream);
};

}  // namespace net

#endif  // NET_QUIC_RELIABLE_QUIC_STREAM_H_

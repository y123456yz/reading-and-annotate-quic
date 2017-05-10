#include "net/quic/quic_data_stream.h"

namespace net {
  namespace tools {

    class QuicClientStream: public ReliableQuicStream {
    public:
      QuicClientStream(QuicStreamId id, QuicSession* session);
      ~QuicClientStream();

      uint32 ProcessRawData(const char* data, uint32 data_len);

      QuicPriority EffectivePriority() const override;

      void WriteStringPiece(base::StringPiece data, bool fin);
    };
  }
}

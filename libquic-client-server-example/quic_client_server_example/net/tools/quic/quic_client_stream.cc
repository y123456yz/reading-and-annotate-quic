#include <iostream>

#include "net/tools/quic/quic_client_stream.h"

namespace net {
  namespace tools {

    QuicClientStream::QuicClientStream(QuicStreamId id, QuicSession* session)
      : ReliableQuicStream(id, session) {
    }

    QuicClientStream::~QuicClientStream() {}

    uint32 QuicClientStream::ProcessRawData(const char* data, uint32 data_len) {
      std::cout << "Received " << data_len << " bytes of data\n";
      return data_len;
    }

    QuicPriority QuicClientStream::EffectivePriority() const {
      return (QuicPriority) 0;
    }
    
    void QuicClientStream::WriteStringPiece(base::StringPiece data, bool fin) {
      this->WriteOrBufferData(data, fin, nullptr);
    }
  }
}

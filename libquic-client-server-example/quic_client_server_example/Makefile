CC=g++
CFLAGS=--std=c++11 -fpermissive -c -g
INC=-I . -I ../libquic/src -I ../libquic/src/third_party/protobuf/src
LDFLAGS=-L ../libquic/build -l quic -L ../libquic/build/boringssl/ssl -l ssl -L ../libquic/build/boringssl/crypto -L ../libquic/build/protobuf -l protobuf -l crypto -l pthread -l rt
SRCFILES=$(wildcard net/tools/quic/*.cc) $(wildcard net/tools/epoll_server/*.cc)
OBJFILES=$(SRCFILES:.cc=.o)

all: quic_perf_client quic_perf_server tcp_perf_server tcp_perf_client

quic_perf_client: $(OBJFILES) quic_perf_client.o
	$(CC) $(OBJFILES) $@.o -o $@ $(LDFLAGS)

quic_perf_server: $(OBJFILES) quic_perf_server.o
	$(CC) $(OBJFILES) $@.o -o $@ $(LDFLAGS)

tcp_perf_server: $(OBJFILES) tcp_perf_server.o
	$(CC) $(OBJFILES) $@.o -o $@ $(LDFLAGS)

tcp_perf_client: $(OBJFILES) tcp_perf_client.o
	$(CC) $(OBJFILES) $@.o -o $@ $(LDFLAGS)

.cc.o:
	$(CC) $(CFLAGS) $(INC) $< -o $@

print-%:
	@echo $* = $($*)

clean:
	rm $(OBJFILES) quic_perf_client.o quic_perf_server.o

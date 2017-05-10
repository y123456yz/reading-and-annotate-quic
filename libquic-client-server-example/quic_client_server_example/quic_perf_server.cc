// Small demo that reads form stdin and sents over a quic connection

#include <iostream>

#include "net/base/ip_endpoint.h"
#include "net/tools/quic/quic_server.h"

#include "base/at_exit.h"
#include "base/strings/string_split.h"
#include "base/strings/string_util.h"
#include "net/base/ip_endpoint.h"
#include "net/base/privacy_mode.h"
#include "net/quic/quic_protocol.h"
#include "net/quic/quic_server_id.h"

using namespace std;

int main(int argc, char *argv[]) {

  // Is needed for whatever reason
  base::AtExitManager exit_manager;

  net::IPAddressNumber ip_address = (net::IPAddressNumber) std::vector<unsigned char> { 0, 0, 0, 0 };
  net::IPEndPoint listen_address(ip_address, 1337);
  net::QuicConfig config;
  net::QuicVersionVector supported_versions = net::QuicSupportedVersions();
  net::EpollServer epoll_server;

  net::tools::QuicServer server(config, supported_versions);


  // Start listening on the specified address.
  if (!server.Listen(listen_address)) {
    cerr << "Could not listen on socket" << endl;
    return 1;
  }

  while (1) {
    server.WaitForEvents();
  }

  cout.flush();

}

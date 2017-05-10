// Small demo that reads form stdin and sents over a quic connection

#include <iostream>
//#include <time.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "base/command_line.h"
#include "base/strings/string_number_conversions.h"

using namespace std;

uint64 FLAGS_total_transfer = 10 * 1000 * 1000;
uint64 FLAGS_chunk_size = 1000;
uint64 FLAGS_duration = 0;

char* randomString(uint length) {
  char* result = new char[length];
  for (uint i = 0; i < length; i++) {
    result[i] = 'a' + rand()%26;
  }
  return result;
}

//使用方法./tcp_perf_client 127.0.0.1 -t 持续时间 -c 一次发送多少数据 -d 总大小
int main(int argc, char *argv[]) {
  int sock;
  struct sockaddr_in serv_addr;
  char buffer[256];

  base::CommandLine::Init(argc, argv);
  base::CommandLine* line = base::CommandLine::ForCurrentProcess();
  const base::CommandLine::StringVector& args = line->GetArgs();
  if (args.size() == 0) {
    cout << "No address to connect to was provided.\n";
    return 1;
  }
  std::string address = args[0];

  if (line->HasSwitch("t")) {
    if (!base::StringToUint64(line->GetSwitchValueASCII(base::StringPiece("t")), &FLAGS_total_transfer)) {
      cout << "-t must be an unsigned integer\n";
      return 1;
    }
  }
  if (line->HasSwitch("c")) {
    if (!base::StringToUint64(line->GetSwitchValueASCII("c"), &FLAGS_chunk_size)) {
      cout << "-c must be an unsigned integer\n";
      return 1;
    }
  }
  if (line->HasSwitch("d")) {
    if (!base::StringToUint64(line->GetSwitchValueASCII("d"), &FLAGS_duration)) {
      cout << "-d must be an unsigned integer\n";
      return 1;
    }
  }

  cout << "Run parameters are:\nchunk size: " << FLAGS_chunk_size
       << "\ntotal size: " << FLAGS_total_transfer
       << "\nduration: " << FLAGS_duration << "\n";

  sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock < 0) {
    cout << "Could not open socket\n";
    exit(1);
  }

  bzero((char *)  &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  if (!inet_aton(address.c_str(), &serv_addr.sin_addr)) {
    cout << "Server address is not a valid IP address";
    return 1;
  }
  serv_addr.sin_port = htons(4567);

  int rc = connect(sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (rc < 0) {
    cout << "Could not connect to server\n";
    exit(1);
  }

  if (FLAGS_duration == 0) {
    for (uint64 i = 0; i < FLAGS_total_transfer; i += FLAGS_chunk_size) {
      char* out = randomString(FLAGS_chunk_size);
      int n = write(sock, out, FLAGS_chunk_size);
      delete out;
      if (n < 0) {
        cout << "Could not write to socket\n";
        exit(1);
      }
    }
  } else {
    for (time_t dest = time(NULL) + FLAGS_duration; time(NULL) < dest; ) {
      char* out = randomString(FLAGS_chunk_size);
      int n = write(sock, out, FLAGS_chunk_size);
      delete out;
      if (n < 0) {
        cout << "Could not write to socket\n";
        exit(1);
      }
    }
  }

  close(sock);
  return 0;
}

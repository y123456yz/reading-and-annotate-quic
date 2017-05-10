#include <iostream>
#include <stdlib.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>
#include <sys/socket.h>
#include <netinet/in.h>

using namespace std;

uint BUFFER_SIZE = 1024;
uint64_t BYTES_RECEIVED = 0;

void alarm_handler(int sig) {
  cout << BYTES_RECEIVED << "\n";
  alarm(1);
}

int main(int argc, char *argv[]) {
  int listen_socket, accepted_socket;
  char buffer[BUFFER_SIZE];
  struct sockaddr_in serv_addr, clt_addr;
  socklen_t clt_addr_len = sizeof(clt_addr);

  listen_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (listen_socket < 0) {
    cout << "Could not open listening socket";
    exit(1);
  }

  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(4567);

  int rc = bind(listen_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
  if (rc < 0) {
    cout << "Could not bind socket to address: " << strerror(rc);
    exit(1);
  }

  listen(listen_socket, 1);

  accepted_socket = accept(listen_socket, (struct sockaddr *) &clt_addr, &clt_addr_len);
  if (accepted_socket < 0) {
    cout << "Could not accept incoming connection";
    exit(1);
  }

  signal(SIGALRM, alarm_handler);
  alarm(1);

  while (1) {
    bzero(buffer, BUFFER_SIZE);
    int n = read(accepted_socket, buffer, BUFFER_SIZE - 1);
    if (n < 0) {
      cout << "Failed reading from socket";
      exit(1);
    }
    if (n == 0) {
      //cout << "Connection closed\n";
      break;
    }
    BYTES_RECEIVED += n;
  }
  cout.flush();
  close(accepted_socket);
  close(listen_socket);
  return 0;
}

#pragma once

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#define PORT 8080

class Server {
 private:
  int server_fd, new_socket, valread;
  struct sockaddr_in address;
  int opt;
  int addrlen;
  char *hello;

 public:
  Server();
  void initialize();
  void bind();
  void listen();
  void accept();
  void send(std::string data, int size);
  std::string recv(int size);
};

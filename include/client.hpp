#pragma once

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <string>
#define PORT 8080

class Client {
 private:
  int sock, valread;
  struct sockaddr_in serv_addr;
  char *hello;

 public:
  Client();
  void initialize();
  void connect(const char *ip_addr);
  void send(std::string data, int size);
  std::string recv(int size);
};

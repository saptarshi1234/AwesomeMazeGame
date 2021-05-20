#pragma once

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include <exception>
#include <string>

#define PORT 8080

class InvalidIP : public std::exception {
 public:
  const char *what() const throw() { return "IP address is not valid"; }
};

class ConnectionError : public std::exception {
 public:
  const char *what() const throw() { return "Could not connect to address"; }
};

class CustomSocket {
 public:
  virtual void send(std::string data);
  virtual std::string recv();
};

class Client : public CustomSocket {
 private:
  int sock, valread;
  struct sockaddr_in serv_addr;
  char *hello;

 public:
  Client();
  void initialize();
  void connect(const char *ip_addr);
  void send(std::string data);
  std::string recv();
};

class Server : public CustomSocket {
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
  void send(std::string data);
  std::string recv();
};

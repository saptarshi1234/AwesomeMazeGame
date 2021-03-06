#include "networking.hpp"

#include <ifaddrs.h>
#include <netdb.h>
#include <sys/types.h>

#include <cstring>
#include <iomanip>
#include <iostream>
#include <sstream>
using std::cout;
using std::endl;

void CustomSocket::send(std::string data) { std::cout << "wrong" << std::endl; }
std::string CustomSocket::recv() {
  std::cout << "wrong" << std::endl;
  return "";
}

std::string CustomSocket::ip_details() {
  struct ifaddrs *ifAddrStruct = NULL;
  struct ifaddrs *ifa = NULL;
  void *tmpAddrPtr = NULL;
  std::stringstream ss;

  getifaddrs(&ifAddrStruct);

  for (ifa = ifAddrStruct; ifa != NULL; ifa = ifa->ifa_next) {
    if (!ifa->ifa_addr) {
      continue;
    }
    if (ifa->ifa_addr->sa_family == AF_INET) {  // check it is IP4
      // is a valid IP4 Address
      tmpAddrPtr = &((struct sockaddr_in *)ifa->ifa_addr)->sin_addr;
      char addressBuffer[INET_ADDRSTRLEN];
      inet_ntop(AF_INET, tmpAddrPtr, addressBuffer, INET_ADDRSTRLEN);
      if (strcmp(ifa->ifa_name, "lo") != 0) {
        ss << addressBuffer;
        break;
      }
    }
  }
  cout << ss.str() << endl;
  if (ifAddrStruct != NULL) freeifaddrs(ifAddrStruct);
  return ss.str();
}

Client::Client() {
  sock = 0;
  hello = "Hello from Client";
}

void Client::initialize() {
  if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
    printf("\n Socket creation error \n");
    exit(EXIT_FAILURE);
  }

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(PORT);
}

void Client::connect(const char *ip_addr) {
  if (inet_pton(AF_INET, ip_addr, &serv_addr.sin_addr) <= 0) {
    throw InvalidIP();
  }
  if (::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    throw ConnectionError();
  }
}

void Client::send(std::string data) {
  std::stringstream ss;
  ss << std::setw(8) << data.size();

  bool fail1 = ::send(sock, ss.str().c_str(), 8, MSG_NOSIGNAL) == -1;
  bool fail2 = ::send(sock, data.c_str(), data.size(), MSG_NOSIGNAL) == -1;

  if (fail1 || fail2) throw DisconnectError();
  // ::send(sock, ss.str().c_str(), 8, 0);
  // ::send(sock, data.c_str(), data.size(), 0);
}

std::string Client::recv() {
  char lenbuf[8];
  ::read(sock, lenbuf, 8);
  int msg_len = std::atoi(lenbuf);

  char buf[msg_len];
  ::read(sock, buf, msg_len);

  std::string ret(buf, msg_len);
  return ret;
}

Server::Server() {
  opt = 1;
  addrlen = sizeof(address);
  hello = "Hello from Server";
}

void Server::initialize() {
  if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
    perror("socket failed");
    exit(EXIT_FAILURE);
  }

  if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt,
                 sizeof(opt))) {
    perror("setsockopt");
    exit(EXIT_FAILURE);
  }
  address.sin_family = AF_INET;
  address.sin_addr.s_addr = INADDR_ANY;
  address.sin_port = htons(PORT);
}

void Server::bind() {
  if (::bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
    perror("bind failed");
    exit(EXIT_FAILURE);
  }
}

void Server::listen() {
  if (::listen(server_fd, 3) < 0) {
    perror("listen");
    exit(EXIT_FAILURE);
  }
}

void Server::accept() {
  if ((new_socket = ::accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0) {
    perror("accept");
    exit(EXIT_FAILURE);
  }
}

void Server::send(std::string data) {
  std::stringstream ss;
  ss << std::setw(8) << data.size();
  bool fail1 = ::send(new_socket, ss.str().c_str(), 8, MSG_NOSIGNAL) == -1;
  bool fail2 =
      ::send(new_socket, data.c_str(), data.size(), MSG_NOSIGNAL) == -1;

  if (fail1 || fail2) throw DisconnectError();
}

std::string Server::recv() {
  char lenbuf[8];
  ::read(new_socket, lenbuf, 8);
  int msg_len = std::atoi(lenbuf);

  char buf[msg_len];
  ::read(new_socket, buf, msg_len);

  std::string ret(buf, msg_len);
  return ret;
}
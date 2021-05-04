#include "server.hpp"

#include <iostream>

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
  std::string valread = recv(1024);
  std::cout << valread << std::endl;
  // ::send(new_socket, hello, strlen(hello), 0);
  // printf("Hello message sent\n");
}

void Server::send(std::string data, int size) {
  ::send(new_socket, data.c_str(), size, 0);
}

std::string Server::recv(int size) {
  char buf[1024];
  ::read(new_socket, buf, 1024);
  return buf;
}
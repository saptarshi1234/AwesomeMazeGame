#include "client.hpp"

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
  if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0) {
    printf("\nInvalid address/ Address not supported \n");
    exit(EXIT_FAILURE);
  }
  if (::connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    printf("\nConnection Failed \n");
    exit(EXIT_FAILURE);
  }
}

void Client::send(std::string data, int size) {
  ::send(sock, data.c_str(), size, 0);
}

std::string Client::recv(int size) {
  char buf[size];
  ::read(sock, buf, size);
  return buf;
}
#include "address.h"

#include <iostream>

Address::Address() {}

Address::Address(uint32_t ip, uint16_t port) {
  baseSetup(port);

  addr_.sin_addr.s_addr = htonl(ip);
}

Address::Address(in_addr ip, uint16_t port) {
  baseSetup(port);

  addr_.sin_addr = ip;
}

Address::Address(const char *ip, uint16_t port) {
  baseSetup(port);

  in_addr ipToNum_{};
  int erStat_ = inet_pton(AF_INET, ip, &ipToNum_);
  if (erStat_ <= 0) {
    std::cout << "Error in IP translation to special numeric format"
              << std::endl;
    throw std::runtime_error(
        "Error in IP translation to special numeric format");
  }

  addr_.sin_addr = ipToNum_;
}

const socklen_t Address::getSize() const { return sizeof(addr_); }

const uint32_t Address::getIP() const {
  return ntohl(addr_.sin_addr.s_addr);
}

const uint16_t Address::getPort() const { return ntohs(addr_.sin_port); }

bool Address::operator==(const Address &other) const {
  return getIP() == other.getIP() && getPort() == other.getPort();
}

const sockaddr_in &Address::getInfo() const { return addr_; }

void Address::baseSetup(uint16_t port) {
#if PLATFORM == PLATFORM_WINDOWS
  ZeroMemory(&addr_, sizeof(addr_));
#else
  memset(&addr_, 0, sizeof(addr_));
#endif

  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);
}

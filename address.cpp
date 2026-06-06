#include "address.h"

#include <iostream>

Address::Address(uint32_t ip, uint16_t port) {
  baseSetup(port);

  addr_.sin_addr.S_un.S_addr = ip;
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

socklen_t Address::getSize() { return addrSize_; }

sockaddr_in &Address::getInfo() { return addr_; }

void Address::baseSetup(uint16_t &port) {
#if PLATFORM == PLATFORM_WINDOWS
  ZeroMemory(&addr_, sizeof(addr_));
#else
  memset(&addr_, 0, sizeof(addr_));
#endif

  addr_.sin_family = AF_INET;
  addr_.sin_port = htons(port);

  addrSize_ = sizeof(addr_);
}

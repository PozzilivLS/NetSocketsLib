#ifndef ADDRESS_H
#define ADDRESS_H

#include "configuration.h"

class Address {
 public:
  Address(uint32_t ip, uint16_t port);
  Address(in_addr ip, uint16_t port);
  Address(const char *ip, uint16_t port);

  sockaddr_in &getInfo();
  socklen_t getSize();

 private:
  void baseSetup(uint16_t &port);

  sockaddr_in addr_;
  socklen_t addrSize_ = 0;
};

#endif
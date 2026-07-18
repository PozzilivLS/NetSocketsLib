#ifndef ADDRESS_H
#define ADDRESS_H

#include "configuration.h"

class Address {
 public:
  Address();
  Address(uint32_t ip, uint16_t port);
  Address(in_addr ip, uint16_t port);
  Address(const char *ip, uint16_t port);

  const sockaddr_in &getInfo() const;
  const socklen_t getSize() const;

  const uint32_t getIP() const;
  const uint16_t getPort() const;

  bool operator==(const Address &other) const;

 private:
  void baseSetup(uint16_t port);

  sockaddr_in addr_;
};

#endif
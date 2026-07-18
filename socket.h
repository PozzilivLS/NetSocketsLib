#ifndef SOCKET_H
#define SOCKET_H

#include "configuration.h"
#include "address.h"
#include "packet.h"

class Socket {
 public:
  Socket();
  ~Socket();

  bool create();
  void close();

  int sendTo(const Address &addr, const Packet &msg) const;
  int recvFrom(Address &addr, Packet &buffer);

  bool setNonBlockingMode();

  int bindTo(const Address &addr);
 private:
  int socket_ = 0;
};

#endif
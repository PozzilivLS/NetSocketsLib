#ifndef SOCKET_H
#define SOCKET_H

#include "configuration.h"
#include "address.h"
#include "packet.h"

class Socket {
 public:
  Socket();
  ~Socket();

  bool sendTo(Address &addr, Packet &msg);
  bool recvFrom(Address &addr, Packet &buffer);

  bool setNonBlockingMode();

 private:
  int socket_ = 0;
};

#endif
#include "socket.h"

#include <cstdio>

#include "packet.h"

Socket::Socket() {}

Socket::~Socket() { close(); }

bool Socket::create() {
  socket_ = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);

  if (socket_ <= 0) {
    printf("failed to create socket\n");
    return false;
  }

  return true;
}

void Socket::close() {
  if (socket_ > 0) {
#if PLATFORM == PLATFORM_WINDOWS
    closesocket(socket_);
#else
    ::close(socket_);
#endif
    socket_ = -1;
  }
}

int Socket::sendTo(const Address &addr, const Packet &msg) const {
  return sendto(socket_, msg.getData(), msg.getDataSize(), 0,
                (sockaddr *)&addr.getInfo(), addr.getSize());
}

int Socket::recvFrom(Address &addr, Packet &msg) {
  char buffer[1024];
  socklen_t addrSize = sizeof(sockaddr_in);

  int res = recvfrom(socket_, buffer, sizeof(buffer), 0,
                     (sockaddr *)&addr.getInfo(), &addrSize);
  if (res > 0) {
    msg.insertBack(buffer, res);
  }
  return res;
}

bool Socket::setNonBlockingMode() {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

  int nonBlocking = 1;
  if (fcntl(socket_, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
    printf("failed to set non-blocking socket\n");
    return false;
  }

#elif PLATFORM == PLATFORM_WINDOWS

  DWORD nonBlocking = 1;
  if (ioctlsocket(socket_, FIONBIO, &nonBlocking) != 0) {
    printf("failed to set non-blocking socket\n");
    return false;
  }

#endif
  return true;
}

int Socket::bindTo(const Address &addr) {
  return bind(socket_, (sockaddr *)&addr.getInfo(), addr.getSize());
}

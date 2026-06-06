#include "socket.h"

#include <cstdio>

#include "packet.h"

inline bool initializeSockets() {
#if PLATFORM == PLATFORM_WINDOWS
  WSADATA WsaData;
  return WSAStartup(MAKEWORD(2, 2), &WsaData) == NO_ERROR;
#else
  return true;
#endif
}

inline void shutdownSockets() {
#if PLATFORM == PLATFORM_WINDOWS
  WSACleanup();
#endif
}

Socket::Socket() {
  socket_ = socket(AF_INET, SOCK_DGRAM, 0);

  if (socket_ <= 0) {
    printf("failed to create socket\n");
  }
}

Socket::~Socket() {
#if PLATFORM == PLATFORM_WINDOWS
  closesocket(socket_);
#else
  close(socket_);
#endif
}

bool Socket::sendTo(Address &addr, Packet &msg) {
  if (sendto(socket_, msg.getData(), msg.getDataSize(), 0,
             (sockaddr *)&addr.getInfo(), addr.getSize()) <= 0) {
    return false;
  }
  return true;
}

bool Socket::recvFrom(Address &addr, Packet &msg) {
  char buffer[1024];
  int addrSize = sizeof(addr);
  if (recvfrom(socket_, buffer, sizeof(buffer), 0, (sockaddr *)&addr.getInfo(),
               &addrSize) <= 0) {
    return false;
  }
  msg.insertBack(buffer, sizeof(buffer));
  return true;
}

bool Socket::setNonBlockingMode() {
#if PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

  int nonBlocking = 1;
  if (fcntl(handle, F_SETFL, O_NONBLOCK, nonBlocking) == -1) {
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

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

// platform detection

#define PLATFORM_WINDOWS 1
#define PLATFORM_MAC 2
#define PLATFORM_UNIX 3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif

#if PLATFORM == PLATFORM_WINDOWS

#include <winsock2.h>
#include <WS2tcpip.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>

#endif
#include <cstdint>
#include <cstring>

#if PLATFORM == PLATFORM_WINDOWS
#pragma comment(lib, "wsock32.lib")
#endif

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

inline int getLastError() {
#if PLATFORM == PLATFORM_WINDOWS
  return WSAGetLastError();
#else
  return errno;
#endif
}

#endif  // !CONFIGURATION_H
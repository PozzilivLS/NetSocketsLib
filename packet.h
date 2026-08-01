#ifndef PACKET_H
#define PACKET_H

#include <iostream>
#include <stdexcept>
#include <vector>

#include "configuration.h"

class Packet {
 public:
  Packet(char *buffer, size_t bufferSize);
  Packet() = default;

  void clear();
  void insertBack(char *buffer, size_t bufferSize);

  const char *getData() const { return data_.data(); }
  size_t getDataSize() const { return data_.size(); }

  Packet &operator<<(const char *value);
  Packet &operator>>(char *value);

  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, Packet &>::type
  operator<<(T value);

  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, Packet &>::type
  operator>>(T &value);

  bool endOfPacket() const { return readPos_ >= data_.size(); }

 private:
  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, T>::type hostToNetwork(
      T value);

  template <typename T>
  typename std::enable_if<std::is_integral<T>::value, T>::type networkToHost(
      T value);

  std::vector<char> data_;
  size_t readPos_ = 0;
};

template <typename T>
inline typename std::enable_if<std::is_integral<T>::value, Packet &>::type
Packet::operator<<(T value) {
  T netValue = hostToNetwork<T>(value);

  const char *bytes = reinterpret_cast<const char *>(&netValue);
  data_.insert(data_.end(), bytes, bytes + sizeof(netValue));

  return *this;
}

template <typename T>
inline typename std::enable_if<std::is_integral<T>::value, Packet &>::type
Packet::operator>>(T &value) {
  T netValue;
  /*std::cout << readPos_ << " - " << data_.size()
            << " netSize: " << sizeof(netValue) << " valSize: " << sizeof(value)
            << std::endl;*/
  if (readPos_ + sizeof(netValue) > data_.size()) {
    throw std::runtime_error("Packet read overflow");
  }
  std::memcpy(&netValue, data_.data() + readPos_, sizeof(netValue));
  readPos_ += sizeof(netValue);

  value = networkToHost<T>(netValue);

  return *this;
}

// 16b
template <>
inline uint16_t Packet::hostToNetwork(uint16_t value) {
  return htons(value);
}

template <>
inline uint16_t Packet::networkToHost<uint16_t>(uint16_t value) {
  return ntohs(value);
}

// 32b
template <>
inline uint32_t Packet::hostToNetwork(uint32_t value) {
  return htonl(value);
}

template <>
inline uint32_t Packet::networkToHost<uint32_t>(uint32_t value) {
  return ntohl(value);
}

// 64b
template <>
inline uint64_t Packet::hostToNetwork<uint64_t>(uint64_t value) {
#if PLATFORM == PLATFORM_UNIX
  return htobe64(value);  // Linux
#elif PLATFORM == PLATFORM_MAC
  return htonll(value);  // macOS/BSD
#else
  return (static_cast<uint64_t>(htonl(value & 0xFFFFFFFF)) << 32) |
         htonl(value >> 32);
#endif
}

template <>
inline uint64_t Packet::networkToHost<uint64_t>(uint64_t value) {
#if PLATFORM == PLATFORM_UNIX
  return htobe64(value);  // Linux
#elif PLATFORM == PLATFORM_MAC
  return htonll(value);  // macOS/BSD
#else
  return (static_cast<uint64_t>(ntohl(value & 0xFFFFFFFF)) << 32) |
         ntohl(value >> 32);
#endif
}

// common
template <typename T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
Packet::hostToNetwork(T value) {
  if constexpr (sizeof(T) == 1) {
    return value;
  } else if constexpr (sizeof(T) == 2) {
    uint16_t temp = hostToNetwork(static_cast<uint16_t>(value));
    return static_cast<T>(temp);
  } else if constexpr (sizeof(T) == 4) {
    uint32_t temp = hostToNetwork(static_cast<uint32_t>(value));
    return static_cast<T>(temp);
  } else if constexpr (sizeof(T) == 8) {
    uint64_t temp = hostToNetwork(static_cast<uint64_t>(value));
    return static_cast<T>(temp);
  } else {
    static_assert(sizeof(T) == 0, "Unsupported integer size");
    return value;
  }
}

template <typename T>
inline typename std::enable_if<std::is_integral<T>::value, T>::type
Packet::networkToHost(T value) {
  if constexpr (sizeof(T) == 1) {
    return value;
  } else if constexpr (sizeof(T) == 2) {
    uint16_t temp = networkToHost(static_cast<uint16_t>(value));
    return static_cast<T>(temp);
  } else if constexpr (sizeof(T) == 4) {
    uint32_t temp = networkToHost(static_cast<uint32_t>(value));
    return static_cast<T>(temp);
  } else if constexpr (sizeof(T) == 8) {
    uint64_t temp = networkToHost(static_cast<uint64_t>(value));
    return static_cast<T>(temp);
  } else {
    static_assert(sizeof(T) == 0, "Unsupported integer size");
    return value;
  }
}

#endif
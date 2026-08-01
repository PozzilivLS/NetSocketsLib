#include "packet.h"

Packet::Packet(char *buffer, size_t bufferSize) {
  insertBack(buffer, bufferSize);
}

void Packet::clear() {
  data_.clear();
  readPos_ = 0;
}

void Packet::insertBack(char *buffer, size_t bufferSize) {
  data_.insert(data_.end(), buffer, buffer + bufferSize);
}

Packet &Packet::operator<<(const char *value) {
  uint32_t valueSize = static_cast<uint32_t>(std::strlen(value)) + 1;
  *this << valueSize;

  data_.insert(data_.end(), value, value + valueSize);

  return *this;
}

Packet &Packet::operator>>(char *value) {
  uint32_t valueSize = 0;
  *this >> valueSize;

  if (readPos_ + valueSize > data_.size()) {
    throw std::runtime_error("Packet read overflow");
  }

  std::memcpy(value, data_.data() + readPos_, valueSize);
  readPos_ += valueSize;

  return *this;
}

Packet &Packet::operator<<(const std::string &value) {
  uint32_t valueSize = static_cast<uint32_t>(value.size());
  *this << valueSize;
  data_.insert(data_.end(), value.begin(), value.end());
  return *this;
}

Packet &Packet::operator>>(std::string &value) {
  uint32_t valueSize = 0;
  *this >> valueSize;
  if (valueSize < 0 ||
      readPos_ + static_cast<size_t>(valueSize) > data_.size()) {
    throw std::runtime_error("Packet read overflow");
  }
  value.assign(data_.data() + readPos_, valueSize);
  readPos_ += valueSize;
  return *this;
}

//Packet &pck::Packet::operator<<(char value) {
//  data_.insert(data_.end(), value);
//  return *this;
//}
//
//Packet &pck::Packet::operator>>(char &value) {
//  if (readPos_ + sizeof(value) > data_.size()) {
//    throw std::runtime_error("Packet read overflow");
//  }
//  std::memcpy(&value, data_.data() + readPos_, sizeof(value));
//  readPos_ += sizeof(value);
//  return *this;
//}

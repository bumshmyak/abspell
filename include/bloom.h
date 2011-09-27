#pragma once

#include <stdint.h>
#include <using_std.h>

class bloom_filter {
 public:
  explicit bloom_filter(size_t size)
    : data_(size, 0) {
  }

  void add(const string& text) {
    const uint8_t mask[] = {0x01, 0x02, 0x04, 0x08,0x10,0x20,0x40,0x80};
    for (int id = 0; id < 8; ++id) {
      data_[get_hash(text, id) % data_.size()] |= mask[id];
    } 
  }

  uint32_t get_hash(const string& text, int id) const {
    const uint64_t base = 4012424397;
    const uint64_t factor[] = {1234324231,2534523787,2325532121,3666555667,3444333111,1234567123,1000111333,4000777333};
    uint64_t value = 0;
    for (size_t index = 0; index < text.size(); ++index) {
      value = (value * factor[id] + static_cast<uint8_t>(text[index])) % base;
    }
    return static_cast<uint32_t>(value);
  }

  bool find(const string& text) const {
    const int mask[] = {0x01, 0x02, 0x04, 0x08,0x10,0x20,0x40,0x80};
    for (int id = 0; id < 8; ++id) {
      if(0x00 == (data_[get_hash(text, id) % data_.size()] & mask[id])) {
        return false;
      }
    } 
    return true;
  }

 private:
  vector<uint8_t> data_;
};

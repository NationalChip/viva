// Copyright (c) 2018 Personal (Binbin Zhang)
// Created on 2018-04-09
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


#ifndef VARINT_H_
#define VARINT_H_

#include <stdint.h>

#include <iostream>

#include "utils.h"

namespace gxdecoder {

static const int kMaxVarint32Bytes = 5;

class Varint {
 public:
  static uint32_t Int32ToZigzag(const int32_t n) {
    return (static_cast<uint32_t>(n) << 1) ^ static_cast<uint32_t>(n >> 31);
  }

  static int32_t ZigzagToInt32(uint32_t n) {
    return (n >> 1) ^ -(n & 1);
  }

  static int WriteUint32(std::ostream& os, uint32_t value) {
    return WriteVarint32(os, value);
  }

  static uint32_t ReadUint32(std::istream& is) {
    return ReadVarint32(is);
  }

  static int WriteInt32(std::ostream&os, int32_t value) {
    uint32_t zigzag = Int32ToZigzag(value);
    return WriteVarint32(os, zigzag);
  }

  static int32_t ReadInt32(std::istream& is) {
    uint32_t zigzag = ReadVarint32(is);
    return ZigzagToInt32(zigzag);
  }

 private:
  static uint8_t* WriteVarint32ToArray(uint32_t value, uint8_t* target) {
    while (value >= 0x80) {
      *target = static_cast<uint8_t>(value | 0x80);
      value >>= 7;
      ++target;
    }
    *target = static_cast<uint8_t>(value);
    return target + 1;
  }

  static int WriteVarint32(std::ostream& os, int32_t value) {
    uint8_t bytes[kMaxVarint32Bytes];
    uint8_t* target = &bytes[0];
    uint8_t* end = WriteVarint32ToArray(value, target);
    int size = end - target;
    os.write(reinterpret_cast<char *>(bytes), sizeof(uint8_t) * size);
    return size;
  }

  static uint32_t ReadVarint32(std::istream& is) {
    uint32_t ret = 0;
    int offset = 0, size = 0;
    while (true) {
      uint8_t ch = static_cast<uint8_t>(is.get());
      ret |= static_cast<uint32_t>(ch & 0x7f) << offset;
      if (ch < 0x80) break;
      offset += 7;
      size += 1;
      CHECK(size <= kMaxVarint32Bytes);
    }
    return ret;
  }
};

}  // namespace gxdecoder

#endif  // VARINT_H_


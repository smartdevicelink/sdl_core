/*
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BITSTREAM_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BITSTREAM_H_

#include "stdint.h"

#include <climits>
#include <cstring>
#include <string>
#include <vector>

#include "utils/macro.h"

namespace utils {

// Big endian input bitstream class
// Tool to read and parse incoming data in a recursive way
// Has sublte similarity to standard stream classes in a way
// it handles stream parsing errors:
// If error while parsing stream occurs, whole stream is marked as
// "bad" and all subsequent parsing is stopped.
class BitStream {
 public:
  BitStream(uint8_t* bytes, size_t bytes_count);
  ~BitStream();

  // Mark stream as badly-formed.
  // Should be called by Extract* family of procedures if they decide
  // that stream is invalid
  void MarkBad()  { bad_ = true;  }
  // Predicates to check whether there were errors while parsing
  // Stream is good when it is created
  bool IsGood()   { return !bad_; }
  bool IsBad()    { return bad_;  }
  operator bool() { return IsGood(); }
 private:
  // These two functions are used for internal stream checks only
  // Stream parser procedures must not define their logic depending on
  // amount of data left in particular bit stream

  // Amount of full bytes left in stream
  size_t FullBytesLeft();
  // Amount of total bits left in stream
  size_t BitsLeft();

  // These stream extractors are helpers for direct friend extractors
  // of this class.

  // Extract single value, amount of bits read from stream depends on T size
  // If there is not enough data in the stream, stream is marked bad
  template<typename T>
  void Extract(T& val);

  // Read single value, amount of bits read from stream is signaled by |bits|
  // parameter. T must be wide enough to hold this amount of bits.
  // If there is not enough data in the stream, stream is marked bad
  template<typename T>
  void ExtractBits(T& val, size_t bits);

  // Extract |length| bytes from the stream. Stream read position
  // must be byte aligned when it is called, stream is marked bad otherwise.
  // If there is not enough data in the stream, it is marked bad.
  void ExtractBytes(void* buffer, size_t length);

 private:
  const uint8_t* bytes_;
  const size_t   bytes_count_;
  size_t byte_offset_;
  size_t bit_offset_;
  bool bad_;
 private:
  friend void Extract(BitStream*, uint8_t*);
  friend void Extract(BitStream*, uint8_t*, size_t);
  friend void Extract(BitStream*, uint32_t*);
  friend void Extract(BitStream*, uint32_t*, size_t);
  friend void Extract(BitStream*, std::string*, size_t);
  friend void Extract(BitStream*, std::vector<uint8_t>*, size_t);

};

// Extract single byte from stream
// If there is not enough data in the stream it is marked bad.
void Extract(BitStream* bs, uint8_t* val);

// Extract defined amount of |bits| from stream and store them
// in a byte size |val|. Hence |bits| must be less than 8.
void Extract(BitStream* bs, uint8_t* val, size_t bits);

// Extract 32 bit word from stream.
// If there is not enough data in the stream it is marked bad.
void Extract(BitStream* bs, uint32_t* val);

// Extract up to 32 |bits| from stream and store them in a |val|
// If there is not enough data in the stream it is marked bad.
void Extract(BitStream* bs, uint32_t* val, size_t bits);

// Extract |length| bytes from stream and store them to the
// string |str|. If stream is too short it is marked bad.
// String must not contain zero bytes.
void Extract(BitStream* bs, std::string* str, size_t length);

// Extract |length| bytes from stream and store them to the
// vector |data|. If stream is too short it is marked bad.
void Extract(BitStream* bs, std::vector<uint8_t>* data, size_t length);


// Template member definitions
template<typename T>
void BitStream::Extract(T& val) {
  // Slow but simple implementation
  // It's a space for bit stream reading optimization
  ExtractBits(val, sizeof(val) * CHAR_BIT);
}

template<typename T>
void BitStream::ExtractBits(T& val, size_t bits) {
  DCHECK(sizeof(val) * CHAR_BIT >= bits);
  if (IsGood()) {
    if (bits > BitsLeft()) {
      MarkBad();
      return;
    }
    val = T();  // Clear value
    for (size_t i = 0; i < bits; ++i) {
      size_t next_bit_number = CHAR_BIT - 1 - bit_offset_;
      uint8_t nextbit = (bytes_[byte_offset_] >> next_bit_number) & 1;
      val = (val << 1) | nextbit;
      ++bit_offset_;
      if (bit_offset_ == CHAR_BIT) {
        ++byte_offset_;
        bit_offset_ = 0;
      }
    }
  }
}

} // namespace utils


#endif // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_BITSTREAM_H_

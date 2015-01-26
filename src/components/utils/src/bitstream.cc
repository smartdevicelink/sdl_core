/**
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
#include "utils/bitstream.h"

namespace utils {

BitStream::BitStream(uint8_t* bytes, size_t bytes_count)
    : bytes_(bytes),
      bytes_count_(bytes_count),
      byte_offset_(0),
      bit_offset_(0),
      bad_(false) {
}

BitStream::~BitStream() {
}

size_t BitStream::FullBytesLeft() {
  size_t left = bytes_count_ - byte_offset_;
  if (bit_offset_ != 0)
    left -= 1;
  return left;
}

size_t BitStream::BitsLeft() {
  return (bytes_count_ - byte_offset_) * CHAR_BIT - bit_offset_;
}

void BitStream::ExtractBytes(void* buffer, size_t length) {
  DCHECK(length == 0 || buffer != NULL);
  if (IsGood()) {
    if (bit_offset_ != 0 ||          // bytes can be extracted only when
        FullBytesLeft() < length) {  // stream is byte-aligned
      MarkBad();
      return;
    }
  }
  memcpy(buffer, &bytes_[byte_offset_], length);
  byte_offset_ += length;
}

void Extract(BitStream* bs, uint8_t* val) {
  DCHECK(bs && val);
  if (*bs) {
    bs->Extract(*val);
  }
}

void Extract(BitStream* bs, uint8_t* val, size_t bits) {
  DCHECK(bs && val);
  if (*bs) {
    bs->ExtractBits(*val, bits);
  }
}

void Extract(BitStream* bs, uint32_t* val) {
  DCHECK(bs && val);
  if (*bs) {
    bs->Extract(*val);
  }
}

void Extract(BitStream* bs, uint32_t* val, size_t bits) {
  DCHECK(bs && val);
  if (*bs) {
    bs->ExtractBits(*val, bits);
  }
}

void Extract(BitStream* bs, std::string* str, size_t length) {
  DCHECK(bs && str);
  if (*bs) {
    // Prevent memory over-allocation
    if (bs->FullBytesLeft() < length) {
      bs->MarkBad();
      return;
    }
    str->resize(length+1);
    void* stringdata = &(*str)[0];
    bs->ExtractBytes(stringdata, length);
    str->resize(length);
  }
}

void Extract(BitStream* bs, std::vector<uint8_t>* data, size_t length) {
  DCHECK(bs && data);
  if (*bs) {
    // Prevent memory over-allocation
    if (bs->FullBytesLeft() < length) {
      bs->MarkBad();
      return;
    }
    data->resize(length);

    if (0 != length) {
      void* dataptr = &data->front();
      bs->ExtractBytes(dataptr, length);
    }
  }
}

}  // namespace utils


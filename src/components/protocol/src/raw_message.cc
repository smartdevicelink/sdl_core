/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "protocol/raw_message.h"

#include <memory.h>

namespace protocol_handler {

RawMessage::RawMessage(uint32_t connection_key,
                       uint32_t protocol_version,
                       const uint8_t* const data_param,
                       uint32_t data_sz,
                       uint8_t type,
                       uint32_t payload_size)
    : connection_key_(connection_key)
    , data_(NULL)
    , data_size_(data_sz)
    , protocol_version_(protocol_version)
    , service_type_(ServiceTypeFromByte(type))
    , payload_size_(payload_size)
    , waiting_(false) {
  if (data_param && data_sz > 0) {
    data_ = new uint8_t[data_sz];
    memcpy(data_, data_param, sizeof(*data_) * data_sz);
  }
}

RawMessage::~RawMessage() {
  delete[] data_;
}

uint32_t RawMessage::connection_key() const {
  return connection_key_;
}

void RawMessage::set_connection_key(uint32_t key) {
  connection_key_ = key;
}

uint8_t* RawMessage::data() const {
  return data_;
}

size_t RawMessage::payload_size() const {
  return payload_size_;
}

size_t RawMessage::data_size() const {
  return data_size_;
}

uint32_t RawMessage::protocol_version() const {
  return protocol_version_;
}

bool RawMessage::IsWaiting() const {
  return waiting_;
}

void RawMessage::set_waiting(bool v) {
  waiting_ = v;
}

}  // namespace protocol_handler

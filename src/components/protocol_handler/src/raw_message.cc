/**
 * \file SmartDeviceLinkRawMessage.cpp
 * \brief SmartDeviceLinkRawMessage class source file.
 *
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

#include "protocol_handler/raw_message.h"

namespace protocol_handler {

RawMessage::RawMessage(int connectionKey, unsigned int protocolVersion,
                       unsigned char* data, unsigned int data_size)
    : connection_key_(connectionKey),
      protocol_version_(protocolVersion),
      serial_number_(0),
      data_size_(data_size) {
  if (data_size > 0) {
    data_ = new unsigned char[data_size];
    for (int i = 0; i < data_size; ++i)
      data_[i] = data[i];
  } else
    data_ = 0;
}

RawMessage::RawMessage(int connectionKey, unsigned int protocolVersion,
                       int serialNumber, unsigned char* data,
                       unsigned int data_size)
    : connection_key_(connectionKey),
      protocol_version_(protocolVersion),
      serial_number_(serialNumber),
      data_size_(data_size) {
  if (data_size > 0) {
    data_ = new unsigned char[data_size];
    for (int i = 0; i < data_size; ++i)
      data_[i] = data[i];
  } else
    data_ = 0;
}

RawMessage::~RawMessage() {
  if (data_) {
    delete[] data_;
    data_ = 0;
  }
}

int RawMessage::connection_key() const {
  return connection_key_;
}

unsigned char* RawMessage::data() const {
  return data_;
}

unsigned int RawMessage::data_size() const {
  return data_size_;
}

unsigned int RawMessage::protocol_version() const {
  return protocol_version_;
}

unsigned int RawMessage::serial_number() const {
  return serial_number_;
}

bool RawMessage::operator ==(RawMessage &other) const {
  return (serial_number_ == other.serial_number_);
}

}  // namespace protocol_handler

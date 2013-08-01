/**
 * \file ProtocolPacket.cpp
 * \brief ProtocolPacket class source file.
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

#include <memory.h>
#include <iostream>  // TODO(AK): cpplink error - Streams are highly discouraged.
#include "protocol_handler/protocol_packet.h"

namespace protocol_handler {

ProtocolPacket::ProtocolPacket()
    : packet_(0),
      total_packet_size_(0),
      data_offset_(0),
      packet_id_(0) {
}

ProtocolPacket::ProtocolPacket(unsigned char version, bool compress,
                               unsigned char frameType,
                               unsigned char serviceType,
                               unsigned char frameData, unsigned char sessionID,
                               unsigned int dataSize, unsigned int messageID,
                               const unsigned char* data,
                               unsigned int packet_id)
    : packet_(0),
      total_packet_size_(0),
      data_offset_(0),
      packet_id_(packet_id) {
  serializePacket(version, compress, frameType, serviceType, frameData,
                  sessionID, dataSize, messageID, data);
}

ProtocolPacket::~ProtocolPacket() {
  // TODO(PV): where to clean?
  packet_ = 0;
  total_packet_size_ = 0;
  packet_id_ = 0;
}

// Serialization
RESULT_CODE ProtocolPacket::serializePacket(unsigned char version,
                                            bool compress,
                                            unsigned char frameType,
                                            unsigned char serviceType,
                                            unsigned char frameData,
                                            unsigned char sessionID,
                                            unsigned int dataSize,
                                            unsigned int messageID,
                                            const unsigned char* data) {
  if (packet_) {
    delete[] packet_;
    packet_ = 0;
    total_packet_size_ = 0;
  }

  unsigned char offset = 0;
  unsigned char compressF = 0x0;
  packet_ = new unsigned char[MAXIMUM_FRAME_DATA_SIZE];
  if (compress) {
    compressF = 0x1;
  }
  unsigned char firstByte = ((version << 4) & 0xF0) | ((compressF << 3) & 0x08)
      | (frameType & 0x07);

  packet_[offset++] = firstByte;
  packet_[offset++] = serviceType;
  packet_[offset++] = frameData;
  packet_[offset++] = sessionID;

  packet_[offset++] = dataSize >> 24;
  packet_[offset++] = dataSize >> 16;
  packet_[offset++] = dataSize >> 8;
  packet_[offset++] = dataSize;

  if (version == PROTOCOL_VERSION_2) {
    packet_[offset++] = messageID >> 24;
    packet_[offset++] = messageID >> 16;
    packet_[offset++] = messageID >> 8;
    packet_[offset++] = messageID;
  }

  total_packet_size_ = offset;

  if (data) {
    if ((offset + dataSize) <= MAXIMUM_FRAME_DATA_SIZE) {
      memcpy(packet_ + offset, data, dataSize);
      total_packet_size_ += dataSize;
    } else {
      delete[] packet_;
      packet_ = 0;
      total_packet_size_ = 0;
      return RESULT_FAIL;
    }
  }

  return RESULT_OK;
}

unsigned char * ProtocolPacket::packet() const {
  return packet_;
}

unsigned int ProtocolPacket::packet_size() const {
  return total_packet_size_;
}

unsigned int ProtocolPacket::packet_id() const {
  return packet_id_;
}

RESULT_CODE ProtocolPacket::appendData(unsigned char* chunkData,
                                       unsigned int chunkDataSize) {
  if (data_offset_ + chunkDataSize <= packet_data_.totalDataBytes) {
    memcpy(packet_data_.data + data_offset_, chunkData, chunkDataSize);
    data_offset_ += chunkDataSize;
    return RESULT_OK;
  }
  return RESULT_FAIL;
}
// End of Serialization

// Deserialization
RESULT_CODE ProtocolPacket::deserializePacket(const unsigned char* message,
                                              unsigned int messageSize) {
  unsigned char offset = 0;
  unsigned char firstByte = message[offset];
  offset++;

  packet_header_.version = firstByte >> 4u;

  if (firstByte & 0x08u) {
    packet_header_.compress = true;
  } else {
    packet_header_.compress = false;
  }

  packet_header_.frameType = firstByte & 0x07u;

  packet_header_.serviceType = message[offset++];
  packet_header_.frameData = message[offset++];
  packet_header_.sessionId = message[offset++];

  packet_header_.dataSize = message[offset++] << 24u;
  packet_header_.dataSize |= message[offset++] << 16u;
  packet_header_.dataSize |= message[offset++] << 8u;
  packet_header_.dataSize |= message[offset++];

  if (packet_header_.version == PROTOCOL_VERSION_2) {
    packet_header_.messageId = message[offset++] << 24u;
    packet_header_.messageId |= message[offset++] << 16u;
    packet_header_.messageId |= message[offset++] << 8u;
    packet_header_.messageId |= message[offset++];
  } else {
    packet_header_.messageId = 0u;
  }

  const unsigned int dataPayloadSize = messageSize - offset;

  if (dataPayloadSize != packet_header_.dataSize) {
    return RESULT_FAIL;
  }

  unsigned char * data = 0;
  if (dataPayloadSize != 0u) {
    data = new unsigned char[messageSize - offset];
    memcpy(data, message + offset, dataPayloadSize);
  }

  packet_data_.data = data;
  packet_data_.totalDataBytes = dataPayloadSize;
  return RESULT_OK;
}

unsigned char ProtocolPacket::version() const {
  return packet_header_.version;
}

bool ProtocolPacket::is_compress() const {
  return packet_header_.compress;
}

unsigned char ProtocolPacket::frame_type() const {
  return packet_header_.frameType;
}

unsigned char ProtocolPacket::service_type() const {
  return packet_header_.serviceType;
}

unsigned char ProtocolPacket::frame_data() const {
  return packet_header_.frameData;
}

unsigned char ProtocolPacket::session_id() const {
  return packet_header_.sessionId;
}

unsigned int ProtocolPacket::data_size() const {
  return packet_header_.dataSize;
}

unsigned int ProtocolPacket::message_id() const {
  return packet_header_.messageId;
}

unsigned char* ProtocolPacket::data() const {
  return packet_data_.data;
}

void ProtocolPacket::set_total_data_bytes(unsigned int dataBytes) {
  if (dataBytes) {
    if (packet_data_.data) {
      delete[] packet_data_.data;
      packet_data_.data = 0;
    }
    packet_data_.data = new unsigned char[dataBytes];
    packet_data_.totalDataBytes = dataBytes;
  }
}

unsigned int ProtocolPacket::total_data_bytes() const {
  return packet_data_.totalDataBytes;
}
// End of Deserialization
}  // namespace protocol_handler

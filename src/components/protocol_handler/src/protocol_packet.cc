/* Copyright (c) 2013, Ford Motor Company
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
/**
 * \file ProtocolPacket.cpp
 * \brief ProtocolPacket class source file.
 */

#include <stdint.h>
#include <memory.h>
#include "protocol_handler/protocol_packet.h"
#include "utils/macro.h"

namespace protocol_handler {

ProtocolPacket::ProtocolPacket()
    : packet_(0),
      total_packet_size_(0),
      data_offset_(0),
      packet_id_(0),
      connection_key_(0)  {
}

ProtocolPacket::ProtocolPacket(uint8_t connection_key,
                               uint8_t version, bool compress,
                               uint8_t frameType,
                               uint8_t serviceType,
                               uint8_t frameData, uint8_t sessionID,
                               uint32_t dataSize, uint32_t messageID,
                               const uint8_t* data,
                               uint32_t packet_id)
    : packet_(0),
      total_packet_size_(0),
      data_offset_(0),
      packet_id_(packet_id),
      connection_key_(connection_key) {
  RESULT_CODE result = serializePacket(version, compress, frameType, serviceType, frameData,
                  sessionID, dataSize, messageID, data);
  if (result != RESULT_OK) {
    NOTREACHED();
  }
}

ProtocolPacket::ProtocolPacket(uint8_t connection_key, uint8_t* data_param,
                               uint32_t data_size)
  : packet_(0),
    total_packet_size_(0),
    data_offset_(0),
    packet_id_(0),
    connection_key_(connection_key) {
    RESULT_CODE result = deserializePacket(data_param, data_size);
    if (result != RESULT_OK) {
      NOTREACHED();
    }
}

ProtocolPacket::~ProtocolPacket() {
  packet_ = 0;
  total_packet_size_ = 0;
  packet_id_ = 0;
  if (packet_data_.data) {
    delete[] packet_data_.data;
    packet_data_.data = 0;
  }
}

// Serialization
RESULT_CODE ProtocolPacket::serializePacket(uint8_t version,
                                            bool compress,
                                            uint8_t frameType,
                                            uint8_t serviceType,
                                            uint8_t frameData,
                                            uint8_t sessionID,
                                            uint32_t dataSize,
                                            uint32_t messageID,
                                            const uint8_t* data) {

  if (packet_) {
    delete[] packet_;
    packet_ = 0;
    total_packet_size_ = 0;
  }

  uint8_t offset = 0;
  uint8_t compressF = 0x0;
  packet_ = new uint8_t[MAXIMUM_FRAME_DATA_SIZE];
  if (compress) {
    compressF = 0x1;
  }
  uint8_t firstByte = ((version << 4) & 0xF0) | ((compressF << 3) & 0x08)
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

uint8_t * ProtocolPacket::packet() const {
  return packet_;
}

uint32_t ProtocolPacket::packet_size() const {
  return total_packet_size_;
}

uint32_t ProtocolPacket::packet_id() const {
  return packet_id_;
}

RESULT_CODE ProtocolPacket::appendData(uint8_t* chunkData,
                                       uint32_t chunkDataSize) {
  if (data_offset_ + chunkDataSize <= packet_data_.totalDataBytes) {
    if (chunkData) {
      if (packet_data_.data) {
        memcpy(packet_data_.data + data_offset_, chunkData, chunkDataSize);
        data_offset_ += chunkDataSize;
        return RESULT_OK;
      }
    }
  }

  return RESULT_FAIL;
}

RESULT_CODE ProtocolPacket::deserializePacket(const uint8_t* message,
                                              uint32_t messageSize) {
  uint8_t offset = 0;
  uint8_t firstByte = message[offset];
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

  packet_data_.totalDataBytes = packet_header_.dataSize;

  uint32_t dataPayloadSize = 0;
  if ((offset < messageSize) &&
      packet_header_.frameType != FRAME_TYPE_FIRST) {
    dataPayloadSize = messageSize - offset;
  }

  uint8_t * data = 0;
  if (dataPayloadSize) {
    data = new uint8_t[dataPayloadSize];
    if (data) {
      memcpy(data, message + offset, dataPayloadSize);
      data_offset_ = dataPayloadSize;
    } else {
      return RESULT_FAIL;
    }
  }

  if (packet_header_.frameType == FRAME_TYPE_FIRST) {
    data_offset_ = 0;
    const uint8_t* data = message + offset;
    uint32_t total_data_bytes = data[0] << 24;
    total_data_bytes |= data[1] << 16;
    total_data_bytes |= data[2] << 8;
    total_data_bytes |= data[3];
    set_total_data_bytes(total_data_bytes);
  } else {
    packet_data_.data = data;
  }

  return RESULT_OK;
}

uint8_t ProtocolPacket::protocol_version() const {
  return packet_header_.version;
}

bool ProtocolPacket::is_compress() const {
  return packet_header_.compress;
}

uint8_t ProtocolPacket::frame_type() const {
  return packet_header_.frameType;
}

uint8_t ProtocolPacket::service_type() const {
  return packet_header_.serviceType;
}

uint8_t ProtocolPacket::frame_data() const {
  return packet_header_.frameData;
}

uint8_t ProtocolPacket::session_id() const {
  return packet_header_.sessionId;
}

uint32_t ProtocolPacket::data_size() const {
  return packet_header_.dataSize;
}

uint32_t ProtocolPacket::message_id() const {
  return packet_header_.messageId;
}

uint8_t* ProtocolPacket::data() const {
  return packet_data_.data;
}

void ProtocolPacket::set_total_data_bytes(uint32_t dataBytes) {
  if (dataBytes) {
    if (packet_data_.data) {
      delete[] packet_data_.data;
      packet_data_.data = 0;
    }
    packet_data_.data = new uint8_t[dataBytes];
    packet_data_.totalDataBytes = dataBytes;
    }
}

void ProtocolPacket::set_data_bytes(
    const uint8_t * const data, const uint32_t dataBytes){
  if (dataBytes) {
    if (packet_data_.data) {
      delete[] packet_data_.data;
      packet_data_.data = 0;
    }
    packet_data_.totalDataBytes = dataBytes;
    packet_data_.data = new uint8_t[dataBytes];
    if(data)
      memcpy(packet_data_.data, data, packet_data_.totalDataBytes);
    }
}

uint32_t ProtocolPacket::total_data_bytes() const {
  return packet_data_.totalDataBytes;
}

uint8_t ProtocolPacket::connection_key() const {
  return connection_key_;
}

// End of Deserialization
}  // namespace protocol_handler

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

#include <stdint.h>
#include <memory.h>
#include <new>
#include "protocol_handler/protocol_packet.h"
#include "utils/macro.h"

namespace protocol_handler {

ProtocolPacket::ProtocolPacket()
    : data_offset_(0),
      packet_id_(0),
      connection_id_(0)  {
}

ProtocolPacket::ProtocolPacket(uint8_t connection_id,
                               uint8_t version, bool protection,
                               uint8_t frameType,
                               uint8_t serviceType,
                               uint8_t frameData, uint8_t sessionID,
                               uint32_t dataSize, uint32_t messageID,
                               const uint8_t *data,
                               uint32_t packet_id)
  : packet_header_(version, protection, frameType, serviceType,
                   frameData, sessionID, dataSize, messageID),
    data_offset_(0),
    packet_id_(packet_id),
    connection_id_(connection_id) {
  set_data(data, dataSize);
  DCHECK(MAXIMUM_FRAME_DATA_SIZE >= dataSize);
}

ProtocolPacket::ProtocolPacket(uint8_t connection_id, uint8_t *data_param,
                               uint32_t data_size)
  : data_offset_(0),
    packet_id_(0),
    connection_id_(connection_id) {
  RESULT_CODE result = deserializePacket(data_param, data_size);
  if (result != RESULT_OK) {
    //NOTREACHED();
  }
}

ProtocolPacket::~ProtocolPacket() {
  delete[] packet_data_.data;
}

// Serialization
RawMessagePtr ProtocolPacket::serializePacket() {
  uint8_t *packet = new (std::nothrow) uint8_t[MAXIMUM_FRAME_DATA_SIZE];
  if (!packet) {
    return RawMessagePtr();
  }
  // version is low byte
  const uint8_t version_byte = packet_header_.version << 4;
  // protection is first bit of second byte
  const uint8_t protection_byte = packet_header_.protection_flag ? (0x8) : 0x0;
  // frame type is last 3 bits of second byte
  const uint8_t frame_type_byte = packet_header_.frameType & 0x07;

  uint8_t offset = 0;
  packet[offset++] = version_byte | protection_byte | frame_type_byte;
  packet[offset++] = packet_header_.serviceType;
  packet[offset++] = packet_header_.frameData;
  packet[offset++] = packet_header_.sessionId;

  packet[offset++] = packet_header_.dataSize >> 24;
  packet[offset++] = packet_header_.dataSize >> 16;
  packet[offset++] = packet_header_.dataSize >> 8;
  packet[offset++] = packet_header_.dataSize;

  if (packet_header_.version != PROTOCOL_VERSION_1) {
    packet[offset++] = packet_header_.messageId >> 24;
    packet[offset++] = packet_header_.messageId >> 16;
    packet[offset++] = packet_header_.messageId >> 8;
    packet[offset++] = packet_header_.messageId;
  }

  DCHECK((offset + packet_data_.totalDataBytes) <= MAXIMUM_FRAME_DATA_SIZE);

  size_t total_packet_size = offset;
  if (packet_data_.data) {
    memcpy(packet + offset, packet_data_.data, packet_data_.totalDataBytes);
    total_packet_size += packet_data_.totalDataBytes;
  }

  const RawMessagePtr out_message(
        new RawMessage(
          connection_id(), packet_header_.version,
          packet, total_packet_size, packet_header_.serviceType) );

  delete[] packet;
  return out_message;
}

uint32_t ProtocolPacket::packet_id() const {
  return packet_id_;
}

RESULT_CODE ProtocolPacket::appendData(uint8_t *chunkData,
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

RESULT_CODE ProtocolPacket::deserializePacket(const uint8_t *message,
                                              uint32_t messageSize) {
  uint8_t offset = 0;
  uint8_t firstByte = message[offset];
  offset++;

  packet_header_.version = firstByte >> 4u;

  if (firstByte & 0x08u) {
    packet_header_.protection_flag = true;
  } else {
    packet_header_.protection_flag = false;
  }

  packet_header_.frameType = firstByte & 0x07u;

  packet_header_.serviceType = message[offset++];
  packet_header_.frameData = message[offset++];
  packet_header_.sessionId = message[offset++];

  packet_header_.dataSize = message[offset++] << 24u;
  packet_header_.dataSize |= message[offset++] << 16u;
  packet_header_.dataSize |= message[offset++] << 8u;
  packet_header_.dataSize |= message[offset++];

  if (packet_header_.version != PROTOCOL_VERSION_1) {
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

  uint8_t *data = 0;
  if (dataPayloadSize) {
    data = new (std::nothrow) uint8_t[dataPayloadSize];
    if (data) {
      memcpy(data, message + offset, dataPayloadSize);
      data_offset_ = dataPayloadSize;
    } else {
      return RESULT_FAIL;
    }
  }

  if (packet_header_.frameType == FRAME_TYPE_FIRST) {
    data_offset_ = 0;
    const uint8_t *data = message + offset;
    uint32_t total_data_bytes = data[0] << 24;
    total_data_bytes |= data[1] << 16;
    total_data_bytes |= data[2] << 8;
    total_data_bytes |= data[3];
    set_total_data_bytes(total_data_bytes);
    if (0 == packet_data_.data) {
      return RESULT_FAIL;
    }
  } else {
    if (packet_data_.data) {
      delete[] packet_data_.data;
    }
    packet_data_.data = data;
  }

  return RESULT_OK;
}

uint8_t ProtocolPacket::protocol_version() const {
  return packet_header_.version;
}

bool ProtocolPacket::protection_flag() const {
  return packet_header_.protection_flag;
}

void ProtocolPacket::set_protection_flag(const bool protection) {
  packet_header_.protection_flag = protection;
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

void ProtocolPacket::set_total_data_bytes(size_t dataBytes) {
  if (dataBytes) {
    delete[] packet_data_.data;
    packet_data_.data = new (std::nothrow) uint8_t[dataBytes];
    packet_data_.totalDataBytes =
        packet_data_.data ? dataBytes : 0;
  }
}

void ProtocolPacket::set_data(
    const uint8_t *const new_data, const size_t new_data_size) {
  if (new_data_size && new_data) {
    packet_header_.dataSize = packet_data_.totalDataBytes = new_data_size;
    delete[] packet_data_.data;
    packet_data_.data = new (std::nothrow) uint8_t[packet_data_.totalDataBytes];
    if (packet_data_.data) {
      memcpy(packet_data_.data, new_data, packet_data_.totalDataBytes);
    } else {
      // TODO(EZamakhov): add log info about memory problem
      packet_header_.dataSize = packet_data_.totalDataBytes = 0;
    }
  }
}

uint32_t ProtocolPacket::total_data_bytes() const {
  return packet_data_.totalDataBytes;
}

uint8_t ProtocolPacket::connection_id() const {
  return connection_id_;
}

// End of Deserialization
}  // namespace protocol_handler

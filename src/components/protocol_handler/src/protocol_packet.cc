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
#include <cstring>
#include <limits>

#include "protocol_handler/protocol_packet.h"
#include "utils/macro.h"
#include "utils/byte_order.h"

namespace protocol_handler {

ProtocolPacket::ProtocolData::ProtocolData()
  : data(NULL), totalDataBytes(0u) { }

ProtocolPacket::ProtocolData::~ProtocolData() {
  delete[] data;
}

ProtocolPacket::ProtocolHeader::ProtocolHeader()
  : version(0x00),
    protection_flag(PROTECTION_OFF),
    frameType(0x00),
    serviceType(0x00),
    frameData(0x00),
    sessionId(0x00),
    dataSize(0x00),
    messageId(0x00) {
}

ProtocolPacket::ProtocolHeader::ProtocolHeader(
    uint8_t version, bool protection, uint8_t frameType, uint8_t serviceType,
    uint8_t frameData, uint8_t sessionID, uint32_t dataSize, uint32_t messageID)
  : version(version),
    protection_flag(protection),
    frameType(frameType),
    serviceType(serviceType),
    frameData(frameData),
    sessionId(sessionID),
    dataSize(dataSize),
    messageId(messageID) {
}

void ProtocolPacket::ProtocolHeader::deserialize(
    const uint8_t* message, const size_t messageSize) {
  if (messageSize < PROTOCOL_HEADER_V1_SIZE) {
    return;
  }
  // first 4 bits
  version = message[0] >> 4u;
  // 5th bit
  protection_flag = message[0] & 0x08u;
  // 6-8 bits
  frameType = message[0] & 0x07u;

  serviceType = message[1];
  frameData   = message[2];
  sessionId   = message[3];

  // FIXME(EZamakhov): usage for FirstFrame message
  const uint32_t data_size_be = *(reinterpret_cast<const uint32_t*>(message + 4));
  dataSize = BE_TO_LE32(data_size_be);
  switch (version) {
    case PROTOCOL_VERSION_2:
    case PROTOCOL_VERSION_3: {
        if (messageSize < PROTOCOL_HEADER_V2_SIZE) {
          return;
        }
        const uint32_t message_id_be =
            *(reinterpret_cast<const  uint32_t*>(message + 8));
        messageId = BE_TO_LE32(message_id_be);
      }
      break;
    default:
      messageId = 0;
      break;
  }
}

ProtocolPacket::ProtocolHeaderValidator::ProtocolHeaderValidator()
  : max_payload_size_(std::numeric_limits<size_t>::max()) {}

void ProtocolPacket::ProtocolHeaderValidator::set_max_payload_size(
    const size_t max_payload_size) {
  max_payload_size_ = max_payload_size;
}

size_t ProtocolPacket::ProtocolHeaderValidator::max_payload_size() const {
  return max_payload_size_;
}

RESULT_CODE ProtocolPacket::ProtocolHeaderValidator::validate(const ProtocolHeader& header) const {
  // Protocol version shall be from 1 to 3
  switch (header.version) {
    case PROTOCOL_VERSION_1:
    case PROTOCOL_VERSION_2:
    case PROTOCOL_VERSION_3:
      break;
    default:
      return RESULT_FAIL;
  }
  // ServiceType shall be equal 0x0 (Control), 0x07 (RPC), 0x0A (PCM), 0x0B (Video), 0x0F (Bulk)
  if (ServiceTypeFromByte(header.serviceType) == kInvalidServiceType) {
    return RESULT_FAIL;
  }
  // Check frame info for each frame type
  // Frame type shall be 0x00 (Control), 0x01 (Single), 0x02 (First), 0x03 (Consecutive)
  // For Control frames Frame info value shall be from 0x00 to 0x06 or 0xFE(Data Ack), 0xFF(HB Ack)
  // For Single and First frames Frame info value shall be equal 0x00
  switch (header.frameType) {
    case FRAME_TYPE_CONTROL : {
        switch (header.frameData) {
          case FRAME_DATA_HEART_BEAT:
          case FRAME_DATA_START_SERVICE:
          case FRAME_DATA_START_SERVICE_ACK:
          case FRAME_DATA_START_SERVICE_NACK:
          case FRAME_DATA_END_SERVICE:
          case FRAME_DATA_END_SERVICE_ACK:
          case FRAME_DATA_END_SERVICE_NACK:
          case FRAME_DATA_SERVICE_DATA_ACK:
          case FRAME_DATA_HEART_BEAT_ACK:
            break;
          default:
            return RESULT_FAIL;
        }
        break;
      }
    case FRAME_TYPE_SINGLE:
      if (header.frameData != FRAME_DATA_SINGLE) {
        return RESULT_FAIL;
      }
      break;
    case FRAME_TYPE_FIRST:
      if (header.frameData != FRAME_DATA_FIRST) {
        return RESULT_FAIL;
      }
      break;
    case FRAME_TYPE_CONSECUTIVE:
      // Could have any FrameInfo value
      break;
    default:
      // All other Frame type is invalid
      return RESULT_FAIL;
  }
  // For Control frames Data Size value shall be less than MTU header
  // For Single and Consecutive Data Size value shall be greater than 0x00
  // and shall be less than N (this value will be defined in .ini file)
  if (header.dataSize > max_payload_size_) {
    return RESULT_FAIL;
  }
  switch (header.frameType) {
    case FRAME_TYPE_SINGLE:
    case FRAME_TYPE_CONSECUTIVE:
      if (header.dataSize <= 0) {
        return RESULT_FAIL;
      }
      break;
    default:
      break;
  }
  // Message ID be equal or greater than 0x01 (not actual for 1 protocol version and Control frames)
  if (FRAME_TYPE_CONTROL != header.frameType && PROTOCOL_VERSION_1 != header.version
     && header.messageId <= 0) {
    // Message ID shall be greater than 0x00, but not implemented in SPT
    // TODO(EZamakhov): return on fix on mobile side - APPLINK-9990
    return RESULT_FAIL;
  }
  return RESULT_OK;
}


ProtocolPacket::ProtocolPacket()
  : payload_size_(0), connection_id_(0)  {
}

ProtocolPacket::ProtocolPacket(ConnectionID connection_id,
                               uint8_t version, bool protection,
                               uint8_t frameType,
                               uint8_t serviceType,
                               uint8_t frameData, uint8_t sessionID,
                               uint32_t dataSize, uint32_t messageID,
                               const uint8_t *data)
  : packet_header_(version, protection, frameType, serviceType,
                   frameData, sessionID, dataSize, messageID),
    packet_data_(),
    payload_size_(0),
    connection_id_(connection_id) {
  set_data(data, dataSize);
}

ProtocolPacket::ProtocolPacket(ConnectionID connection_id)
  : packet_header_(),
    packet_data_(),
    payload_size_(0),
    connection_id_(connection_id) {
}

// Serialization
RawMessagePtr ProtocolPacket::serializePacket() const {
  // TODO(EZamakhov): Move header serialization to ProtocolHeader
  // version is low byte
  const uint8_t version_byte = packet_header_.version << 4;
  // protection is first bit of second byte
  const uint8_t protection_byte = packet_header_.protection_flag ? (0x8) : 0x0;
  // frame type is last 3 bits of second byte
  const uint8_t frame_type_byte = packet_header_.frameType & 0x07;

  uint8_t header[PROTOCOL_HEADER_V2_SIZE];
  uint8_t offset = 0;
  header[offset++] = version_byte | protection_byte | frame_type_byte;
  header[offset++] = packet_header_.serviceType;
  header[offset++] = packet_header_.frameData;
  header[offset++] = packet_header_.sessionId;

  header[offset++] = packet_header_.dataSize >> 24;
  header[offset++] = packet_header_.dataSize >> 16;
  header[offset++] = packet_header_.dataSize >> 8;
  header[offset++] = packet_header_.dataSize;

  if (packet_header_.version != PROTOCOL_VERSION_1) {
    header[offset++] = packet_header_.messageId >> 24;
    header[offset++] = packet_header_.messageId >> 16;
    header[offset++] = packet_header_.messageId >> 8;
    header[offset++] = packet_header_.messageId;
  };

  size_t total_packet_size = offset + (packet_data_.data ? packet_data_.totalDataBytes : 0);
#ifndef BUILD_TESTS // Remove DCHECK for PH tests
  DCHECK(total_packet_size <= MAXIMUM_FRAME_DATA_SIZE);
#endif

  uint8_t *packet = new (std::nothrow) uint8_t[total_packet_size];
  if (!packet) {
    return RawMessagePtr();
  }

  memcpy(packet, header, offset);
  if (packet_data_.data && packet_data_.totalDataBytes) {
    memcpy(packet + offset, packet_data_.data, packet_data_.totalDataBytes);
  }

  const RawMessagePtr out_message(
        new RawMessage(
          connection_id(), packet_header_.version,
          packet, total_packet_size, packet_header_.serviceType) );

  delete[] packet;
  return out_message;
}

RESULT_CODE ProtocolPacket::appendData(uint8_t *chunkData,
                                       uint32_t chunkDataSize) {
  if (payload_size_ + chunkDataSize <= packet_data_.totalDataBytes) {
    if (chunkData && chunkDataSize > 0) {
      if (packet_data_.data) {
        memcpy(packet_data_.data + payload_size_, chunkData, chunkDataSize);
        payload_size_ += chunkDataSize;
        return RESULT_OK;
      }
    }
  }

  return RESULT_FAIL;
}

size_t ProtocolPacket::packet_size() const {
  return packet_header_.dataSize;
}

bool ProtocolPacket::operator==(const ProtocolPacket& other) const {
  if (connection_id_ == other.connection_id_ &&
      packet_header_.version == other.packet_header_.version &&
      packet_header_.protection_flag == other.packet_header_.protection_flag &&
      packet_header_.frameType == other.packet_header_.frameType &&
      packet_header_.serviceType == other.packet_header_.serviceType &&
      packet_header_.frameData == other.packet_header_.frameData &&
      packet_header_.sessionId == other.packet_header_.sessionId &&
      packet_header_.dataSize == other.packet_header_.dataSize &&
      packet_header_.messageId ==  other.packet_header_.messageId &&
      packet_data_.totalDataBytes == other.packet_data_.totalDataBytes) {
    if (other.packet_data_.totalDataBytes == 0) {
      return true;
    }
    // Compare payload data
    if (packet_data_.data && other.packet_data_.data &&
        0 == memcmp(packet_data_.data, other.packet_data_.data,
                    packet_data_.totalDataBytes)) {
      return true;
    }
  }
  return false;
}

RESULT_CODE ProtocolPacket::deserializePacket(
    const uint8_t *message, const size_t messageSize) {
  packet_header_.deserialize(message, messageSize);
  const uint8_t offset =
      packet_header_.version == PROTOCOL_VERSION_1 ? PROTOCOL_HEADER_V1_SIZE
                                                   : PROTOCOL_HEADER_V2_SIZE;

  packet_data_.totalDataBytes = packet_header_.dataSize;

  uint32_t dataPayloadSize = 0;
  if ((offset < messageSize) &&
      packet_header_.frameType != FRAME_TYPE_FIRST) {
    dataPayloadSize = messageSize - offset;
  }

  uint8_t *data = NULL;
  if (dataPayloadSize) {
    data = new (std::nothrow) uint8_t[dataPayloadSize];
    if (!data) {
      return RESULT_FAIL;
    }
    memcpy(data, message + offset, dataPayloadSize);
    payload_size_ = dataPayloadSize;
  }

  if (packet_header_.frameType == FRAME_TYPE_FIRST) {
    payload_size_ = 0;
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
    delete[] packet_data_.data;
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

uint8_t *ProtocolPacket::data() const {
  return packet_data_.data;
}

void ProtocolPacket::set_total_data_bytes(size_t dataBytes) {
  if (dataBytes) {
    delete[] packet_data_.data;
    packet_data_.data = new (std::nothrow) uint8_t[dataBytes];
    packet_data_.totalDataBytes = packet_data_.data ? dataBytes : 0;
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

uint32_t ProtocolPacket::payload_size() const {
  return payload_size_;
}

// End of Deserialization
}  // namespace protocol_handler

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
#include "protocol_handler/incoming_data_handler.h"
#include "utils/logger.h"
#include "protocol/common.h"

namespace protocol_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ProtocolHandler")

IncomingDataHandler::IncomingDataHandler()
    : header_(), validator_(NULL), last_portion_of_data_was_malformed_(false) {}

void IncomingDataHandler::set_validator(
    const ProtocolPacket::ProtocolHeaderValidator* const validator) {
  validator_ = validator;
}

static const size_t MIN_HEADER_SIZE =
    std::min(PROTOCOL_HEADER_V1_SIZE, PROTOCOL_HEADER_V2_SIZE);

ProtocolFramePtrList IncomingDataHandler::ProcessData(
    const RawMessage& tm_message,
    RESULT_CODE* result,
    size_t* malformed_occurrence) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(result);
  DCHECK(malformed_occurrence);
  const transport_manager::ConnectionUID connection_id =
      tm_message.connection_key();
  const uint8_t* data = tm_message.data();
  const size_t tm_message_size = tm_message.data_size();
  if (tm_message_size == 0 || data == NULL) {
    LOG4CXX_WARN(logger_, "Wrong raw message " << tm_message_size << " bytes");
    *result = RESULT_FAIL;
    return ProtocolFramePtrList();
  }
  LOG4CXX_INFO(logger_,
               "Processing incoming data of size "
                   << tm_message_size << " for connection " << connection_id);
  ConnectionsDataMap::iterator it = connections_data_.find(connection_id);
  if (connections_data_.end() == it) {
    LOG4CXX_WARN(logger_, "ProcessData requested for unknown connection");
    *result = RESULT_FAIL;
    return ProtocolFramePtrList();
  }
  std::vector<uint8_t>& connection_data = it->second;
  connection_data.insert(connection_data.end(), data, data + tm_message_size);
  LOG4CXX_DEBUG(logger_,
                "Total data size for connection " << connection_id << " is "
                                                  << connection_data.size());
  ProtocolFramePtrList out_frames;
  *malformed_occurrence = 0;
  *result = CreateFrame(
      connection_data, out_frames, *malformed_occurrence, connection_id);
  LOG4CXX_DEBUG(logger_,
                "New data size for connection " << connection_id << " is "
                                                << connection_data.size());
  if (!out_frames.empty()) {
    LOG4CXX_INFO(logger_,
                 "Created and passed " << out_frames.size() << " packets");
  } else {
    if (RESULT_DEFERRED == *result) {
      LOG4CXX_DEBUG(
          logger_,
          "No packets have been created. Waiting next portion of data.");
    } else {
      LOG4CXX_WARN(logger_, "No packets have been created.");
    }
  }
  if (*malformed_occurrence > 0u || last_portion_of_data_was_malformed_) {
    *result = RESULT_MALFORMED_OCCURS;
  } else {
    *result = RESULT_OK;
  }
  return out_frames;
}

void IncomingDataHandler::AddConnection(
    const transport_manager::ConnectionUID connection_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  // Add empty list of session to new connection
  connections_data_[connection_id] = ConnectionsDataMap::mapped_type();
}

void IncomingDataHandler::RemoveConnection(
    const transport_manager::ConnectionUID connection_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  connections_data_.erase(connection_id);
}

uint32_t IncomingDataHandler::GetPacketSize(
    const ProtocolPacket::ProtocolHeader& header) {
  switch (header.version) {
    case PROTOCOL_VERSION_1:
      return header.dataSize + PROTOCOL_HEADER_V1_SIZE;
    case PROTOCOL_VERSION_2:
    case PROTOCOL_VERSION_3:
    case PROTOCOL_VERSION_4:
    case PROTOCOL_VERSION_5:
      return header.dataSize + PROTOCOL_HEADER_V2_SIZE;
    default:
      LOG4CXX_WARN(logger_,
                   "Unknown version: " << static_cast<int>(header.version));
      break;
  }
  return 0u;
}

RESULT_CODE IncomingDataHandler::CreateFrame(
    std::vector<uint8_t>& incoming_data,
    ProtocolFramePtrList& out_frames,
    size_t& malformed_occurrence,
    const transport_manager::ConnectionUID connection_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<uint8_t>::iterator data_it = incoming_data.begin();
  size_t data_size = incoming_data.size();

  while (data_size >= MIN_HEADER_SIZE) {
    header_.deserialize(&*data_it, data_size);
    const RESULT_CODE validate_result =
        validator_ ? validator_->validate(header_) : RESULT_OK;

    if (validate_result != RESULT_OK) {
      LOG4CXX_WARN(logger_, "Packet validation failed");
      if (!last_portion_of_data_was_malformed_) {
        ++malformed_occurrence;
        LOG4CXX_DEBUG(logger_,
                      "Malformed message found " << malformed_occurrence);
      }
      last_portion_of_data_was_malformed_ = true;
      ++data_it;
      --data_size;
      LOG4CXX_DEBUG(logger_,
                    "Moved to the next byte "
                        << std::hex << static_cast<const void*>(&*data_it));
      continue;
    }
    LOG4CXX_DEBUG(logger_, "Payload size " << header_.dataSize);
    const uint32_t packet_size = GetPacketSize(header_);
    if (packet_size <= 0) {
      LOG4CXX_WARN(logger_, "Null packet size");
      ++data_it;
      --data_size;
      LOG4CXX_DEBUG(logger_,
                    "Moved to the next byte "
                        << std::hex << static_cast<const void*>(&*data_it));
      continue;
    }
    if (data_size < packet_size) {
      LOG4CXX_DEBUG(logger_, "Packet data is not available yet");
      incoming_data.erase(incoming_data.begin(), data_it);
      return RESULT_DEFERRED;
    }
    ProtocolFramePtr frame(new protocol_handler::ProtocolPacket(connection_id));
    const RESULT_CODE deserialize_result =
        frame->deserializePacket(&*data_it, packet_size);
    LOG4CXX_DEBUG(logger_, "Deserialized frame " << frame);
    if (deserialize_result != RESULT_OK) {
      LOG4CXX_WARN(logger_, "Packet deserialization failed");
      incoming_data.erase(incoming_data.begin(), data_it);
      return RESULT_FAIL;
    }

    out_frames.push_back(frame);
    last_portion_of_data_was_malformed_ = false;
    LOG4CXX_DEBUG(logger_,
                  "Frame added. "
                      << "Connection ID " << connection_id);

    data_it += packet_size;
    data_size -= packet_size;
  }
  incoming_data.erase(incoming_data.begin(), data_it);
  return RESULT_OK;
}
}  // namespace protocol_handler

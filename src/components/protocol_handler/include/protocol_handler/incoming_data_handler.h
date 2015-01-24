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
#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_INCOMING_DATA_HANDLER_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_INCOMING_DATA_HANDLER_H_

#include <list>
#include <map>
#include <vector>
#include "utils/macro.h"
#include "protocol_packet.h"
#include "transport_manager/common.h"

namespace protocol_handler {

/**
 * \class IncomingDataHandler
 * \brief Class for contecat TM messages to ford frames and validate ford header data
 * IncomingDataHandler methods are reentrant and not thread-safe
 */
class IncomingDataHandler {
 public:
  IncomingDataHandler();
  /**
   * @brief Setting additional validator for checking malformed packets
   * \param validator pointer
   */
  void set_validator(const ProtocolPacket::ProtocolHeaderValidator* const validator);
  /**
   * @brief Contecat TM messages to ford frames and validate ford header data
   * \param TM messages for converting to frames
   * \param result of convertion
   *   - RESULT_FAIL - packet serialization or validation error occurs
   *   - RESULT_OK - no error ocures
   * \return list of complete, correct packets
   */
  std::list<ProtocolFramePtr> ProcessData(const RawMessage& tm_message, RESULT_CODE* result);
  /**
   * @brief Add connection for data handling and verification
   */
  void AddConnection(
    const transport_manager::ConnectionUID connection_id);
  /**
   * @brief Remove connection and all unhandled data
   */
  void RemoveConnection(
    const transport_manager::ConnectionUID connection_id);

 private:
  /**
   * @brief Returns size of frame to be formed from raw bytes.
   */
  static uint32_t GetPacketSize(const ProtocolPacket::ProtocolHeader& header);
  /**
   * @brief Try to create frame from incoming data
   * \param incommung_data raw stream
   * \param out_frames list for read frames
   *
   * \return operation RESULT_CODE
   *   - RESULT_DEFERRED - waiting for more data
   *   - RESULT_OK - one or more frames successfully created
   *   - RESULT_FAIL - packet serialization or validation error occurs
   */
  RESULT_CODE CreateFrame(std::vector<uint8_t>& incoming_data,
                          std::list<ProtocolFramePtr>& out_frames,
                          const transport_manager::ConnectionUID connection_id);

  typedef std::map<transport_manager::ConnectionUID, std::vector<uint8_t> > ConnectionsDataMap;
  ConnectionsDataMap connections_data_;
  ProtocolPacket::ProtocolHeader header_;
  const  ProtocolPacket::ProtocolHeaderValidator * validator_;
  DISALLOW_COPY_AND_ASSIGN(IncomingDataHandler);
};
}  // namespace protocol_handler
#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_INCOMING_DATA_HANDLER_H_

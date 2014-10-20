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
#ifndef SRC_COMPONENTS_INCLUDE_PROTOCOL_RAW_MESSAGE_H_
#define SRC_COMPONENTS_INCLUDE_PROTOCOL_RAW_MESSAGE_H_

#include "utils/macro.h"
#include "utils/shared_ptr.h"
#include "protocol/service_type.h"
#include "protocol/message_priority.h"

namespace protocol_handler {
/**
 * \class SmartDeviceLinkRawMessage
 * \brief Class-wrapper for information about message for interchanging
 * between components.
 */
class RawMessage {
 public:
  /**
   * \brief Constructor
   * \param connection_key Identifier of connection within which message
   * is transferred
   * \param protocolVersion Version of protocol of the message
   * \param data Message string
   * \param dataSize Message size
   * \param payload_size Received data size
   */
  RawMessage(uint32_t connection_key, uint32_t protocol_version,
             const uint8_t *const data_param, uint32_t data_size,
             uint8_t type = ServiceType::kRpc,
             uint32_t payload_size = 0);
  /**
   * \brief Destructor
   */
  ~RawMessage();
  /**
   * \brief Connection Identifier
   * Obtained from \s ConnectionHandler
   */
  uint32_t connection_key() const;
  /**
   * \brief Setter for connection identifier
   */
  void set_connection_key(uint32_t);
  /**
   * \brief Getter for message string data
   */
  uint8_t *data() const;
  /**
   * \brief Getter for message size
   */
  size_t data_size() const;
  /**
   * \brief Getter for actual data size
   */
  size_t payload_size() const;
  /**
   * \brief Version of SmartDeviceLink protocol
   * used for transferring message.
   */
  uint32_t protocol_version() const;
  /**
   * \brief Type of service message belongs to
   */
  ServiceType service_type() const {
    return service_type_;
  }
  /**
   * \brief Specifies current state of message in queue.
   * if false message is "ready to be processed"
   * otherwise it is "waiting for response"
   */
  bool IsWaiting() const;
  void set_waiting(bool v);

 private:
  uint32_t connection_key_;
  uint8_t *data_;
  size_t data_size_;
  uint32_t protocol_version_;
  ServiceType service_type_;
  size_t payload_size_;
  bool waiting_;
  DISALLOW_COPY_AND_ASSIGN(RawMessage);
};
typedef  utils::SharedPtr<RawMessage> RawMessagePtr;
}  // namespace protocol_handler
#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_RAW_MESSAGE_H_

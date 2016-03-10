/*
 * Copyright (c) 2016, Ford Motor Company
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
#ifndef SRC_COMPONENTS_INCLUDE_TEST_PROTOCOL_HANDLER_MOCK_PROTOCOL_HANDLER_SETTINGS_H_
#define SRC_COMPONENTS_INCLUDE_TEST_PROTOCOL_HANDLER_MOCK_PROTOCOL_HANDLER_SETTINGS_H_

#include "gmock/gmock.h"
#include "protocol_handler/protocol_handler_settings.h"

namespace test {
namespace components {
namespace protocol_handler_test {

class MockProtocolHandlerSettings : public protocol_handler::ProtocolHandlerSettings {
 public:
  MOCK_CONST_METHOD0(maximum_payload_size,
      size_t());
  MOCK_CONST_METHOD0(message_frequency_count,
      size_t());
  MOCK_CONST_METHOD0(message_frequency_time,
      size_t());
  MOCK_CONST_METHOD0(malformed_message_filtering,
      bool());
  MOCK_CONST_METHOD0(malformed_frequency_count,
      size_t());
  MOCK_CONST_METHOD0(malformed_frequency_time,
      size_t());
  MOCK_CONST_METHOD0(heart_beat_timeout,
      uint32_t());
  MOCK_CONST_METHOD0(max_supported_protocol_version,
      uint16_t());
  MOCK_CONST_METHOD0(enable_protocol_4,
      bool());
  MOCK_CONST_METHOD0(multiframe_waiting_timeout,
      uint32_t());
#ifdef ENABLE_SECURITY
  MOCK_CONST_METHOD0(force_protected_service,
      const std::vector<int>&());
  MOCK_CONST_METHOD0(force_unprotected_service,
      const std::vector<int>&());
#endif
};

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_PROTOCOL_HANDLER_MOCK_PROTOCOL_HANDLER_SETTINGS_H_

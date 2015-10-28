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

#include <gtest/gtest.h>
#include <vector>
#include <list>

#include "utils/macro.h"
#include "protocol_handler/protocol_packet.h"

namespace {
  const size_t MAXIMUM_FRAME_DATA_V3_SIZE = 131072;
}

namespace test {
namespace components {
namespace protocol_handler_test {
using namespace ::protocol_handler;

class ProtocolHeaderValidatorTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    some_message_id = 0xABCDEF0;
    some_session_id = 0xFEDCBA0;
  }
  ProtocolPacket::ProtocolHeaderValidator header_validator;
  uint32_t some_message_id;
  uint32_t some_session_id;
};

// Protocol version shall be from 1 to 3
TEST_F(ProtocolHeaderValidatorTest, MaxPayloadSizeSetGet) {
  EXPECT_EQ(std::numeric_limits<size_t>::max(),
            header_validator.max_payload_size());
  for (size_t value = 0; value < MAXIMUM_FRAME_DATA_V3_SIZE * 2; ++value) {
    header_validator.set_max_payload_size(value);
    EXPECT_EQ(value, header_validator.max_payload_size());
  }
}

// Protocol version shall be from 1 to 4
TEST_F(ProtocolHeaderValidatorTest, Malformed_Version) {
  std::vector<uint8_t> malformed_versions;
  malformed_versions.push_back(0);
  for (uint8_t version = PROTOCOL_VERSION_4 + 1; version <= PROTOCOL_VERSION_MAX; ++version) {
    malformed_versions.push_back(version);
  }

  for (size_t i = 0; i < malformed_versions.size(); ++i) {
    const ProtocolPacket::ProtocolHeader malformed_message_header(
          malformed_versions[i], PROTECTION_OFF, FRAME_TYPE_CONTROL, kControl,
          FRAME_DATA_HEART_BEAT, some_session_id, 0u, some_message_id);
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(malformed_message_header))
        << "Malformed version " << malformed_message_header.version;

  }
}

// ServiceType shall be equal 0x0 (Control), 0x07 (RPC), 0x0A (PCM), 0x0B (Video), 0x0F (Bulk)
TEST_F(ProtocolHeaderValidatorTest, Malformed_ServiceType) {
  std::vector<uint8_t> malformed_serv_types;
  for (uint8_t service_type = kControl + 1; service_type < kRpc; ++service_type) {
    malformed_serv_types.push_back(service_type);
  }
  malformed_serv_types.push_back(0x08);
  malformed_serv_types.push_back(0x09);
  malformed_serv_types.push_back(0x0C);
  malformed_serv_types.push_back(0x0D);
  malformed_serv_types.push_back(0x0E);

  for (size_t i = 0; i < malformed_serv_types.size(); ++i) {
    const ProtocolPacket::ProtocolHeader malformed_message_header(
          PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_CONTROL, malformed_serv_types[i],
          FRAME_DATA_HEART_BEAT, some_session_id, 0u, some_message_id);
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(malformed_message_header))
        << "Malformed service type " << malformed_message_header.serviceType;
  }
}

// Frame type shall be 0x00 (Control), 0x01 (Single), 0x02 (First), 0x03 (Consecutive)
TEST_F(ProtocolHeaderValidatorTest, Malformed_FrameType) {
  std::vector<uint8_t> malformed_frame_types;
  for (uint8_t frame_type = FRAME_TYPE_CONSECUTIVE + 1;
       frame_type <= FRAME_TYPE_MAX_VALUE; ++frame_type) {
    malformed_frame_types.push_back(frame_type);
  }
  for (size_t i = 0; i < malformed_frame_types.size(); ++i) {
    const ProtocolPacket::ProtocolHeader malformed_message_header(
          PROTOCOL_VERSION_3, PROTECTION_OFF, malformed_frame_types[i],
          kControl, FRAME_DATA_HEART_BEAT, some_session_id, 0u, some_message_id);
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(malformed_message_header))
        << "Malformed frame type " << malformed_message_header.frameType;
  }
}

// For Control frames Frame info value shall be from 0x00 to 0x06 or 0xFE(Data Ack), 0xFF(HB Ack)
TEST_F(ProtocolHeaderValidatorTest, Malformed_ControlFrame) {
  std::vector<uint8_t> malformed_frame_data;
  for (uint8_t frame_type = FRAME_DATA_END_SERVICE_NACK + 1;
       frame_type < FRAME_DATA_SERVICE_DATA_ACK; ++frame_type) {
    malformed_frame_data.push_back(frame_type);
  }
  for (size_t i = 0; i < malformed_frame_data.size(); ++i) {
    const ProtocolPacket::ProtocolHeader malformed_message_header(
          PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_CONTROL, kControl,
          malformed_frame_data[i], some_session_id, 0u, some_message_id);
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(malformed_message_header))
        << "Malformed Control frame with data " << malformed_message_header.frameData;
  }
}
// For Single and First frames Frame info value shall be equal 0x00
TEST_F(ProtocolHeaderValidatorTest, Malformed_SingleFrame) {
  std::vector<uint8_t> malformed_frame_data;
  for (uint8_t frame_type = FRAME_DATA_SINGLE + 1;
       frame_type < FRAME_DATA_MAX_VALUE; ++frame_type) {
    malformed_frame_data.push_back(frame_type);
  }
  malformed_frame_data.push_back(FRAME_DATA_MAX_VALUE);
  for (size_t i = 0; i < malformed_frame_data.size(); ++i) {
    const ProtocolPacket::ProtocolHeader malformed_message_header(
          PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_SINGLE, kControl,
          malformed_frame_data[i], some_session_id, 0u, some_message_id);
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(malformed_message_header))
        << "Malformed Single frame with data " << malformed_message_header.frameData;
    // All malformed messages shall be ignored
  }
}

// For Single and First frames Frame info value shall be equal 0x00
TEST_F(ProtocolHeaderValidatorTest, Malformed_FirstFrame) {
  std::vector<uint8_t> malformed_frame_data;
  for (uint8_t frame_type = FRAME_DATA_FIRST + 1;
       frame_type < FRAME_DATA_MAX_VALUE; ++frame_type) {
    malformed_frame_data.push_back(frame_type);
  }
  malformed_frame_data.push_back(FRAME_DATA_MAX_VALUE);
  for (size_t i = 0; i < malformed_frame_data.size(); ++i) {
    const ProtocolPacket::ProtocolHeader malformed_message_header(
          PROTOCOL_VERSION_3, PROTECTION_OFF, FRAME_TYPE_SINGLE, kControl,
          malformed_frame_data[i], some_session_id, 0u, some_message_id);
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(malformed_message_header))
        << "Malformed First frame with data " << malformed_message_header.frameData;
  }
}

TEST_F(ProtocolHeaderValidatorTest, Malformed_ControlFrame_EmptyPayload) {
  const size_t payload_size = 0u;
  const ProtocolPacket::ProtocolHeader control_message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_CONTROL, kControl,
        FRAME_DATA_HEART_BEAT, some_session_id, payload_size, some_message_id);
  const ProtocolPacket::ProtocolHeader single_message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_SINGLE, kControl,
        FRAME_DATA_SINGLE, some_session_id, payload_size, some_message_id);
  const ProtocolPacket::ProtocolHeader consecutive_message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_CONSECUTIVE, kControl,
        FRAME_DATA_LAST_CONSECUTIVE, some_session_id, payload_size, some_message_id);

  for (size_t max_payload_size = 0; max_payload_size < MAXIMUM_FRAME_DATA_V3_SIZE * 2;
       ++max_payload_size) {
    header_validator.set_max_payload_size(MAXIMUM_FRAME_DATA_V3_SIZE + max_payload_size);

    // For Control frames Data Size value could be zero
    EXPECT_EQ(RESULT_OK, header_validator.validate(control_message_header));
    // For Control frames Data Size value could be zero
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(single_message_header));
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(consecutive_message_header));
  }
}

// For Control frames Data Size value should be less than MTU header
TEST_F(ProtocolHeaderValidatorTest, Malformed_Payload_V2) {
  const size_t payload_size = MAXIMUM_FRAME_DATA_V2_SIZE;
  const ProtocolPacket::ProtocolHeader control_message_header(
        PROTOCOL_VERSION_2, PROTECTION_ON, FRAME_TYPE_CONTROL, kControl,
        FRAME_DATA_HEART_BEAT, some_session_id, payload_size, some_message_id);
  const ProtocolPacket::ProtocolHeader single_message_header(
        PROTOCOL_VERSION_2, PROTECTION_ON, FRAME_TYPE_SINGLE, kControl,
        FRAME_DATA_SINGLE, some_session_id, payload_size, some_message_id);
  const ProtocolPacket::ProtocolHeader consecutive_message_header(
        PROTOCOL_VERSION_2, PROTECTION_ON, FRAME_TYPE_CONSECUTIVE, kControl,
        FRAME_DATA_LAST_CONSECUTIVE, some_session_id, payload_size, some_message_id);

  for (size_t max_payload_size = 0;
       max_payload_size < MAXIMUM_FRAME_DATA_V3_SIZE * 2;
       ++max_payload_size) {
    header_validator.set_max_payload_size(max_payload_size);
    EXPECT_EQ(RESULT_OK, header_validator.validate(control_message_header));
    EXPECT_EQ(RESULT_OK, header_validator.validate(single_message_header));
    EXPECT_EQ(RESULT_OK, header_validator.validate(consecutive_message_header));
  }
}

TEST_F(ProtocolHeaderValidatorTest, Malformed_Payload_V3) {
  const size_t payload_size = MAXIMUM_FRAME_DATA_V3_SIZE;
  const ProtocolPacket::ProtocolHeader control_message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_CONTROL, kControl,
        FRAME_DATA_HEART_BEAT, some_session_id, payload_size, some_message_id);
  const ProtocolPacket::ProtocolHeader single_message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_SINGLE, kControl,
        FRAME_DATA_SINGLE, some_session_id, payload_size, some_message_id);
  const ProtocolPacket::ProtocolHeader consecutive_message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_CONSECUTIVE, kControl,
        FRAME_DATA_LAST_CONSECUTIVE, some_session_id, payload_size, some_message_id);

  for (size_t max_payload_size = 0;
       max_payload_size < MAXIMUM_FRAME_DATA_V3_SIZE;
       ++max_payload_size) {
    header_validator.set_max_payload_size(max_payload_size);
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(control_message_header));
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(single_message_header));
    EXPECT_EQ(RESULT_FAIL, header_validator.validate(consecutive_message_header));
  }

  for (size_t max_payload_size = MAXIMUM_FRAME_DATA_V3_SIZE;
       max_payload_size < MAXIMUM_FRAME_DATA_V3_SIZE * 2;
       ++max_payload_size) {
    header_validator.set_max_payload_size(max_payload_size);
    EXPECT_EQ(RESULT_OK, header_validator.validate(control_message_header));
    EXPECT_EQ(RESULT_OK, header_validator.validate(single_message_header));
    EXPECT_EQ(RESULT_OK, header_validator.validate(consecutive_message_header));
  }
}

TEST_F(ProtocolHeaderValidatorTest, Malformed_Payload_V3_with_V2_size) {
  const size_t payload_size = MAXIMUM_FRAME_DATA_V2_SIZE;
  const ProtocolPacket::ProtocolHeader control_message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_CONTROL, kControl,
        FRAME_DATA_HEART_BEAT, some_session_id, payload_size, some_message_id);
  const ProtocolPacket::ProtocolHeader single_message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_SINGLE, kControl,
        FRAME_DATA_SINGLE, some_session_id, payload_size, some_message_id);
  const ProtocolPacket::ProtocolHeader consecutive_message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_CONSECUTIVE, kControl,
        FRAME_DATA_LAST_CONSECUTIVE, some_session_id, payload_size, some_message_id);

  for (size_t max_payload_size = 0;
       max_payload_size < MAXIMUM_FRAME_DATA_V3_SIZE * 2;
       ++max_payload_size) {
    header_validator.set_max_payload_size(max_payload_size);
    EXPECT_EQ(RESULT_OK, header_validator.validate(control_message_header));
    EXPECT_EQ(RESULT_OK, header_validator.validate(single_message_header));
    EXPECT_EQ(RESULT_OK, header_validator.validate(consecutive_message_header));
  }
}

// Message ID be equal or greater than 0x01
TEST_F(ProtocolHeaderValidatorTest, Malformed_MessageID) {
  const uint32_t malformed_message_id = 0x0u;
  ProtocolPacket::ProtocolHeader message_header(
        PROTOCOL_VERSION_3, PROTECTION_ON, FRAME_TYPE_FIRST, kControl,
        FRAME_DATA_HEART_BEAT, some_session_id, 0u, malformed_message_id);

  message_header.frameType = FRAME_TYPE_FIRST;
  message_header.version = PROTOCOL_VERSION_1;
  EXPECT_EQ(RESULT_OK, header_validator.validate(message_header));

  message_header.version = PROTOCOL_VERSION_2;
  EXPECT_EQ(RESULT_FAIL, header_validator.validate(message_header));
  message_header.version = PROTOCOL_VERSION_3;
  EXPECT_EQ(RESULT_FAIL, header_validator.validate(message_header));

  message_header.frameType = FRAME_TYPE_CONTROL;
  EXPECT_EQ(RESULT_OK, header_validator.validate(message_header));
}

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test

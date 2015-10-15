/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "utils/macro.h"
#include "utils/bitstream.h"
#include "protocol_handler/protocol_payload.h"
#include "protocol/common.h"

namespace test {
namespace components {
namespace protocol_handler_test {
using namespace ::protocol_handler;
using ::utils::BitStream;

void prepare_data(uint8_t* data_for_sending, ProtocolPayloadV2& message) {
  uint8_t rpc_type_flag = message.header.rpc_type;
  uint8_t offset = 0;

  uint32_t function_id = message.header.rpc_function_id;
  data_for_sending[offset++] = ((rpc_type_flag << 4) & 0xF0) | (function_id >> 24);
  data_for_sending[offset++] = function_id >> 16;
  data_for_sending[offset++] = function_id >> 8;
  data_for_sending[offset++] = function_id;

  uint32_t correlationId = message.header.correlation_id;
  data_for_sending[offset++] = correlationId >> 24;
  data_for_sending[offset++] = correlationId >> 16;
  data_for_sending[offset++] = correlationId >> 8;
  data_for_sending[offset++] = correlationId;

  uint32_t jsonSize = message.header.json_size;
  data_for_sending[offset++] = jsonSize >> 24;
  data_for_sending[offset++] = jsonSize >> 16;
  data_for_sending[offset++] = jsonSize >> 8;
  data_for_sending[offset++] = jsonSize;

  if (message.json.length() != 0) {
    memcpy(data_for_sending + offset, message.json.c_str(),
           message.json.size());
  }

  if (message.data.size() != 0) {
    uint8_t* current_pointer = data_for_sending + offset + message.json.length();
    u_int32_t binarySize = message.data.size();
    for (uint32_t i = 0; i < binarySize; ++i) {
      current_pointer[i] = message.data[i];
    }
  }
}

TEST(ProtocolPayloadTest, ExtractProtocolWithOnlyHeader) {
  ProtocolPayloadV2 prot_payload_test;

  prot_payload_test.header.correlation_id = 1;
  prot_payload_test.header.rpc_function_id = 2;
  prot_payload_test.header.json_size = 0;
  prot_payload_test.header.rpc_type = kRpcTypeRequest;

  const size_t data_for_sending_size = PROTOCOL_HEADER_V2_SIZE;
  uint8_t* data_for_sending = new uint8_t[data_for_sending_size];

  prepare_data(data_for_sending, prot_payload_test);

  BitStream bs(data_for_sending, data_for_sending_size);

  ProtocolPayloadV2 prot_payload;
  Extract(&bs, &prot_payload, data_for_sending_size);

  EXPECT_TRUE(bs.IsGood());

  EXPECT_EQ(prot_payload_test.header.correlation_id,
            prot_payload.header.correlation_id);
  EXPECT_EQ(prot_payload_test.header.json_size, prot_payload.header.json_size);
  EXPECT_EQ(prot_payload_test.header.rpc_function_id,
            prot_payload.header.rpc_function_id);
  EXPECT_EQ(prot_payload_test.header.rpc_type, prot_payload.header.rpc_type);
  delete[] data_for_sending;
}

TEST(ProtocolPayloadTest, ExtractCorrectProtocolWithDataWithoutJSON) {
  ProtocolPayloadV2 prot_payload_test;

  prot_payload_test.header.correlation_id = 1;
  prot_payload_test.header.rpc_function_id = 2;
  prot_payload_test.header.json_size = 0;
  prot_payload_test.header.rpc_type = kRpcTypeNotification;
  prot_payload_test.data = {1, 2, 3};

  const size_t data_for_sending_size = PROTOCOL_HEADER_V2_SIZE +
                                       prot_payload_test.data.size() +
                                       prot_payload_test.json.length();
  uint8_t* data_for_sending = new uint8_t[data_for_sending_size];

  prepare_data(data_for_sending, prot_payload_test);

  BitStream bs(data_for_sending, data_for_sending_size);

  ProtocolPayloadV2 prot_payload;
  Extract(&bs, &prot_payload, data_for_sending_size);

  EXPECT_TRUE(bs.IsGood());

  EXPECT_EQ(prot_payload_test.header.correlation_id,
            prot_payload.header.correlation_id);
  EXPECT_EQ(prot_payload_test.header.json_size, prot_payload.header.json_size);
  EXPECT_EQ(prot_payload_test.header.rpc_function_id,
            prot_payload.header.rpc_function_id);
  EXPECT_EQ(prot_payload_test.header.rpc_type, prot_payload.header.rpc_type);
  EXPECT_EQ(prot_payload_test.data.size(), prot_payload.data.size());
  EXPECT_EQ(prot_payload_test.data[0], prot_payload.data[0]);
  EXPECT_EQ(prot_payload_test.data[1], prot_payload.data[1]);
  EXPECT_EQ(prot_payload_test.data[2], prot_payload.data[2]);

  delete[] data_for_sending;
}

TEST(ProtocolPayloadTest, ExtractCorrectProtocolWithoutDataWithJSON) {
  ProtocolPayloadV2 prot_payload_test;

  prot_payload_test.header.correlation_id = 1;
  prot_payload_test.header.rpc_function_id = 2;

  prot_payload_test.header.rpc_type = kRpcTypeResponse;

  std::string expect_output_json_string =
      "{\n \" : {\n \"name\" : \"\",\n\"parameters\" : \"\"\n}\n}\n";

  prot_payload_test.json = expect_output_json_string;
  prot_payload_test.header.json_size = prot_payload_test.json.length();

  const size_t data_for_sending_size = PROTOCOL_HEADER_V2_SIZE +
                                       prot_payload_test.data.size() +
                                       prot_payload_test.json.length();
  uint8_t *data_for_sending = new uint8_t[data_for_sending_size];
  prepare_data(data_for_sending, prot_payload_test);

  BitStream bs(data_for_sending, data_for_sending_size);
  ProtocolPayloadV2 prot_payload;
  Extract(&bs, &prot_payload, data_for_sending_size);

  EXPECT_TRUE(bs.IsGood());

  EXPECT_EQ(prot_payload_test.header.correlation_id,
            prot_payload.header.correlation_id);
  EXPECT_EQ(prot_payload_test.header.json_size, prot_payload.header.json_size);
  EXPECT_EQ(prot_payload_test.header.rpc_function_id,
            prot_payload.header.rpc_function_id);
  EXPECT_EQ(prot_payload_test.header.rpc_type, prot_payload.header.rpc_type);
  EXPECT_EQ(prot_payload_test.json.length(), prot_payload.json.length());
  EXPECT_EQ(prot_payload_test.json, prot_payload.json);
  delete[] data_for_sending;
}

TEST(ProtocolPayloadTest, ExtractCorrectProtocolWithDataWithJSON) {
  ProtocolPayloadV2 prot_payload_test;

  prot_payload_test.header.correlation_id = 1;
  prot_payload_test.header.rpc_function_id = 2;
  prot_payload_test.header.rpc_type = kRpcTypeRequest;
  prot_payload_test.data = {1, 2, 3};

  std::string expect_output_json_string =
      "{\n \" : {\n \"name\" : \"\",\n\"parameters\" : \"\"\n}\n}\n";

  prot_payload_test.json = expect_output_json_string;
  prot_payload_test.header.json_size = prot_payload_test.json.length();

  const size_t data_for_sending_size = PROTOCOL_HEADER_V2_SIZE +
                                       prot_payload_test.data.size() +
                                       prot_payload_test.json.length();
  uint8_t* data_for_sending = new uint8_t[data_for_sending_size];
  prepare_data(data_for_sending, prot_payload_test);

  BitStream bs(data_for_sending, data_for_sending_size);
  ProtocolPayloadV2 prot_payload;
  Extract(&bs, &prot_payload, data_for_sending_size);

  EXPECT_TRUE(bs.IsGood());

  EXPECT_EQ(prot_payload_test.header.correlation_id,
            prot_payload.header.correlation_id);
  EXPECT_EQ(prot_payload_test.header.json_size, prot_payload.header.json_size);
  EXPECT_EQ(prot_payload_test.header.rpc_function_id,
            prot_payload.header.rpc_function_id);
  EXPECT_EQ(prot_payload_test.header.rpc_type, prot_payload.header.rpc_type);
  EXPECT_EQ(prot_payload_test.json.length(), prot_payload.json.length());
  EXPECT_EQ(prot_payload_test.json, prot_payload.json);
  EXPECT_EQ(prot_payload_test.data.size(), prot_payload.data.size());
  EXPECT_EQ(prot_payload_test.data[0], prot_payload.data[0]);
  EXPECT_EQ(prot_payload_test.data[1], prot_payload.data[1]);
  EXPECT_EQ(prot_payload_test.data[2], prot_payload.data[2]);

  delete[] data_for_sending;
}

TEST(ProtocolPayloadTest, ExtractProtocolWithJSONWithDataWithWrongPayloadSize) {
  ProtocolPayloadV2 prot_payload_test;

  prot_payload_test.header.correlation_id = 1;
  prot_payload_test.header.rpc_function_id = 2;

  prot_payload_test.header.rpc_type = kRpcTypeResponse;
  prot_payload_test.data = {1, 2, 3};

  std::string expect_output_json_string =
      "{\n \" : {\n \"name\" : \"\",\n\"parameters\" : \"\"\n}\n}\n";

  prot_payload_test.json = expect_output_json_string;
  prot_payload_test.header.json_size = prot_payload_test.json.length();

  const size_t data_for_sending_size =
      PROTOCOL_HEADER_V2_SIZE + prot_payload_test.json.length();
  uint8_t* data_for_sending = new uint8_t[data_for_sending_size];
  prepare_data(data_for_sending, prot_payload_test);

  BitStream bs(data_for_sending, data_for_sending_size);
  ProtocolPayloadV2 prot_payload;

  // Try extract with payload size less than size of data
  Extract(&bs, &prot_payload, PROTOCOL_HEADER_V2_SIZE);

  EXPECT_TRUE(bs.IsBad());

  EXPECT_EQ(prot_payload_test.header.correlation_id,
            prot_payload.header.correlation_id);
  EXPECT_EQ(prot_payload_test.header.json_size, prot_payload.header.json_size);
  EXPECT_EQ(prot_payload_test.header.rpc_function_id,
            prot_payload.header.rpc_function_id);
  EXPECT_EQ(prot_payload_test.header.rpc_type, prot_payload.header.rpc_type);
  EXPECT_EQ(prot_payload_test.json.length(), prot_payload.json.length());
  EXPECT_EQ(prot_payload_test.json, prot_payload.json);
  EXPECT_EQ(0u, prot_payload.data.size());
  delete[] data_for_sending;
}

}  // namespace protocol_handler_test
}  // namespace components
}  // namespace test

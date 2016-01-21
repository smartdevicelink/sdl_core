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

#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "security_manager/security_query.h"
#include "protocol_handler/protocol_payload.h"
#include "utils/byte_order.h"
#include "security_manager/mock_security_manager.h"

// Test values for compare after serialization and byteorder conversion
#define SEQ_NUMBER 0x12345678u
#define CONNECTION_KEY 0xABCDEF0u

namespace test {
namespace components {
namespace security_manager_test {

using ::security_manager::SecurityQuery;

class SecurityQueryTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    // init_header used for SecurityQuery initialization
    init_header.query_type = SecurityQuery::NOTIFICATION;
    init_header.query_id = SecurityQuery::SEND_HANDSHAKE_DATA;
    init_header.seq_number = SEQ_NUMBER;
    init_header.json_size = 0u;

    // invalid_header is default (not initialized) QueryHeader
    invalid_header.query_type = SecurityQuery::INVALID_QUERY_TYPE;
    invalid_header.query_id = SecurityQuery::INVALID_QUERY_ID;
    invalid_header.seq_number = 0u;
    invalid_header.json_size = 0u;
  }
  /*
   * Wrapper for fast call SecurityQuery::SerializeQuery
   * Used for handling header and data array to byte array for serialization
   */
  std::vector<uint8_t> DeserializeData(SecurityQuery::QueryHeader header,
                                       const uint8_t* const binary_data,
                                       const size_t bin_data_size) const {
    // convert to Big-Endian (network) order
    const uint32_t query_id = header.query_id << 8;
    header.query_id = LE_TO_BE32(query_id);
    header.seq_number = LE_TO_BE32(header.seq_number);
    header.json_size = LE_TO_BE32(header.json_size);
    std::vector<uint8_t> vector;
    vector.reserve(sizeof(header) + bin_data_size);
    // copy header data
    const uint8_t* header_data = reinterpret_cast<uint8_t*>(&header);
    vector.insert(vector.end(), header_data, header_data + sizeof(header));
    // copy all data
    vector.insert(vector.end(), binary_data, binary_data + bin_data_size);
    return vector;
  }
  SecurityQuery::QueryHeader init_header, invalid_header;
};
/*
 * Security QueryHeader shall be the same size as RPC header
 * for correct working on Mobile side (3*8 byte)
 */
TEST_F(SecurityQueryTest, Equal_RPCHeader) {
  ASSERT_EQ(sizeof(SecurityQuery::QueryHeader) * 8,
            ::protocol_handler::ProtocolPayloadV2SizeBits());
}
/*
 * Security QueryHeader default construction
 */
TEST_F(SecurityQueryTest, QueryHeaderConstructor) {
  const SecurityQuery::QueryHeader new_header;

  EXPECT_PRED_FORMAT2(QueryHeader_EQ, new_header, invalid_header);
}
/*
 * Security QueryHeader shall construct with correct fields
 */
TEST_F(SecurityQueryTest, QueryHeaderConstructor2) {
  SecurityQuery::QueryHeader new_header(SecurityQuery::NOTIFICATION,
                                        SecurityQuery::SEND_HANDSHAKE_DATA,
                                        SEQ_NUMBER);
  ASSERT_EQ(new_header.query_type, SecurityQuery::NOTIFICATION);
  ASSERT_EQ(new_header.query_id, SecurityQuery::SEND_HANDSHAKE_DATA);
  ASSERT_EQ(new_header.seq_number, SEQ_NUMBER);
  ASSERT_EQ(new_header.json_size, 0u);

  SecurityQuery::QueryHeader new_header2(SecurityQuery::RESPONSE,
                                         SecurityQuery::SEND_INTERNAL_ERROR,
                                         SEQ_NUMBER + 1);
  ASSERT_EQ(new_header2.query_type, SecurityQuery::RESPONSE);
  ASSERT_EQ(new_header2.query_id, SecurityQuery::SEND_INTERNAL_ERROR);
  ASSERT_EQ(new_header2.seq_number, SEQ_NUMBER + 1);
  ASSERT_EQ(new_header2.json_size, 0u);
}
/*
 * Security QueryHeader shall copy of all filed on construction from copy
 */
TEST_F(SecurityQueryTest, QueryHeaderCopyConstructor) {
  SecurityQuery::QueryHeader new_header(init_header);

  EXPECT_PRED_FORMAT2(QueryHeader_EQ, new_header, init_header);
}
/*
 * Security QueryHeader shall construct with NULL fields
 */
TEST_F(SecurityQueryTest, QueryConstructor) {
  const SecurityQuery query;

  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_EQ(query.get_data_size(), 0u);
  ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  ASSERT_TRUE(query.get_json_message().empty());
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), invalid_header);

  // Deserialization shall return invalid header as vector
  const std::vector<uint8_t> vector = DeserializeData(invalid_header, NULL, 0);
  const std::vector<uint8_t> deserialize_vector = query.DeserializeQuery();
  ASSERT_EQ(deserialize_vector, vector);
}
/*
 * Security QueryHeader shall construct with specified fields
 */
TEST_F(SecurityQueryTest, QueryConstructor2) {
  const SecurityQuery query(init_header, CONNECTION_KEY);

  ASSERT_EQ(query.get_connection_key(), CONNECTION_KEY);
  ASSERT_EQ(query.get_data_size(), 0u);
  ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  ASSERT_TRUE(query.get_json_message().empty());
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), init_header);

  // Deserialization shall return init header as vector
  const std::vector<uint8_t> vector = DeserializeData(init_header, NULL, 0);
  const std::vector<uint8_t> deserialize_vector = query.DeserializeQuery();
  ASSERT_EQ(deserialize_vector, vector);
}
/*
 * Security QueryHeader shall construct with specified fields
 */
TEST_F(SecurityQueryTest, QueryConstructor3) {
  uint8_t raw_data[] = {0x0, 0x1, 0x2};
  const size_t raw_data_size = sizeof(raw_data) / sizeof(raw_data[0]);

  SecurityQuery query(init_header, CONNECTION_KEY, raw_data, raw_data_size);

  ASSERT_EQ(query.get_connection_key(), CONNECTION_KEY);
  ASSERT_EQ(query.get_data_size(), raw_data_size);
  // query shall handle own array of byte data
  ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  for (size_t i = 0; i < raw_data_size; ++i) {
    ASSERT_EQ(query.get_data()[i], raw_data[i]);
  }
  ASSERT_TRUE(query.get_json_message().empty());
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), init_header);

  // Deserialization shall return vector equal header + data array
  const std::vector<uint8_t> vector =
      DeserializeData(init_header, raw_data, raw_data_size);
  const std::vector<uint8_t> deserialize_vector = query.DeserializeQuery();
  ASSERT_EQ(deserialize_vector, vector);
}
/*
 * Security Query setters
 */
TEST_F(SecurityQueryTest, Setters) {
  const std::string str = "test example string";
  uint8_t raw_data[] = {0x6, 0x7, 0x8};
  const size_t raw_data_size = sizeof(raw_data) / sizeof(raw_data[0]);

  SecurityQuery query;
  query.set_connection_key(CONNECTION_KEY);
  query.set_data(raw_data, raw_data_size);
  query.set_json_message(str);
  query.set_header(init_header);

  ASSERT_EQ(query.get_connection_key(), CONNECTION_KEY);
  ASSERT_EQ(query.get_data_size(), raw_data_size);
  // query shall handle own array of byte data
  ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  for (size_t i = 0; i < raw_data_size; ++i) {
    ASSERT_EQ(query.get_data()[i], raw_data[i]);
  }
  ASSERT_EQ(query.get_json_message(), str);
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), init_header);
}
/*
 * SecurityQuery serializes NULL data
 */
TEST_F(SecurityQueryTest, Parse_NullData) {
  SecurityQuery query;
  const bool result = query.SerializeQuery(NULL, 0u);

  ASSERT_FALSE(result);
  // check side-effects
  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_EQ(query.get_data_size(), 0u);
  ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  ASSERT_TRUE(query.get_json_message().empty());
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), invalid_header);
}
/*
 * SecurityQuery serializes few (less header size) data
 */
TEST_F(SecurityQueryTest, Parse_LessHeaderData) {
  std::vector<uint8_t> vector(sizeof(init_header) - 1, 0);

  SecurityQuery query;
  const bool result = query.SerializeQuery(&vector[0], vector.size());

  ASSERT_FALSE(result);
  // check side-effects
  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_EQ(query.get_data_size(), 0u);
  ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  ASSERT_TRUE(query.get_json_message().empty());
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), invalid_header);
}
/*
 * SecurityQuery serializes data equal header size
 */
TEST_F(SecurityQueryTest, Parse_HeaderData) {
  const std::vector<uint8_t> vector = DeserializeData(init_header, NULL, 0u);

  SecurityQuery query;
  const bool result = query.SerializeQuery(&vector[0], vector.size());

  ASSERT_TRUE(result);
  // check side-effects
  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_EQ(query.get_data_size(), 0u);
  ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  ASSERT_TRUE(query.get_json_message().empty());
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), init_header);

  // Deserialization shall return vector equal serialization vector
  const std::vector<uint8_t> deserialize_vector = query.DeserializeQuery();
  ASSERT_EQ(deserialize_vector, vector);
}
/*
 * SecurityQuery serializes wrong header
 */
TEST_F(SecurityQueryTest, Parse_HeaderDataWrong) {
  // Wrong json size
  init_header.json_size = 0x0FFFFFFF;
  const std::vector<uint8_t> vector = DeserializeData(init_header, NULL, 0u);

  SecurityQuery query;
  const bool result = query.SerializeQuery(&vector[0], vector.size());

  ASSERT_FALSE(result);
  // check side-effects
  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_EQ(query.get_data_size(), 0u);
  ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  ASSERT_TRUE(query.get_json_message().empty());
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), init_header);

  // Deserialization shall return vector equal serialization vector
  const std::vector<uint8_t> deserialize_vector = query.DeserializeQuery();
  ASSERT_EQ(deserialize_vector, vector);
}
/*
 * SecurityQuery serializes data contains header and binary data
 * with INVALID_QUERY_TYPE
 */
TEST_F(SecurityQueryTest, Parse_InvalidQuery) {
  SecurityQuery::QueryHeader invalid_query_header(
      SecurityQuery::INVALID_QUERY_TYPE, SecurityQuery::INVALID_QUERY_ID,
      SEQ_NUMBER);

  // some sample data
  uint8_t raw_data[] = {0x6, 0x7, 0x8};
  const size_t raw_data_size = sizeof(raw_data) / sizeof(raw_data[0]);

  const std::vector<uint8_t> vector =
      DeserializeData(invalid_query_header, raw_data, raw_data_size);

  SecurityQuery query;
  const bool result = query.SerializeQuery(&vector[0], vector.size());

  ASSERT_TRUE(result);
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), invalid_query_header);
  ASSERT_EQ(query.get_data_size(), raw_data_size);
  // query shall handle own array of byte data
  ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  for (size_t i = 0; i < raw_data_size; ++i) {
    ASSERT_EQ(query.get_data()[i], raw_data[+i]);
  }
  // check side-effects
  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_TRUE(query.get_json_message().empty());

  // Deserialization shall return vector equal serialization vector
  const std::vector<uint8_t> deserialize_vector = query.DeserializeQuery();
  ASSERT_EQ(deserialize_vector, vector);
}
/*
 * SecurityQuery serializes data contains header and binary data
 * with unknown types and ids
 */
TEST_F(SecurityQueryTest, Parse_InvalidQuery_UnknownTypeId) {
  SecurityQuery::QueryHeader invalid_type_id_header(
      SecurityQuery::INVALID_QUERY_TYPE - 1,
      // Use not enum value for additional testing
      SecurityQuery::INVALID_QUERY_ID - 1, SEQ_NUMBER);

  const std::vector<uint8_t> vector =
      DeserializeData(invalid_type_id_header, NULL, 0u);

  SecurityQuery query;
  const bool result = query.SerializeQuery(&vector[0], vector.size());
  ASSERT_TRUE(result);
  // Parse set all unknown types and ids to INVALID_QUERY_ID
  invalid_type_id_header.query_type = SecurityQuery::INVALID_QUERY_TYPE;
  invalid_type_id_header.query_id = SecurityQuery::INVALID_QUERY_ID;
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(),
                      invalid_type_id_header);
  // check side-effects
  ASSERT_EQ(query.get_data_size(), 0u);
  ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_TRUE(query.get_json_message().empty());
}
/*
 * Security QueryH Parse data contains header and binary data
 * with unknown types and ids
 */
TEST_F(SecurityQueryTest, Parse_InvalidQuery_UnknownId_Response) {
  SecurityQuery::QueryHeader invalid_id_header(
      SecurityQuery::RESPONSE,
      // Use not enum value for additional testing
      SecurityQuery::INVALID_QUERY_ID - 2, SEQ_NUMBER);
  const std::vector<uint8_t> vector =
      DeserializeData(invalid_id_header, NULL, 0u);

  SecurityQuery query;
  const bool result = query.SerializeQuery(&vector[0], vector.size());
  ASSERT_TRUE(result);
  // Parse set all unknown types and ids to INVALID_QUERY_ID
  invalid_id_header.query_id = SecurityQuery::INVALID_QUERY_ID;
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), invalid_id_header);
  // check side-effects
  ASSERT_EQ(query.get_data_size(), 0u);
  ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_TRUE(query.get_json_message().empty());
}
/*
 * SecurityQuery serializes data contains header and binary data
 * with INVALID_QUERY_TYPE
 */
TEST_F(SecurityQueryTest, Parse_Handshake) {
  SecurityQuery::QueryHeader handshake_header(
      SecurityQuery::NOTIFICATION, SecurityQuery::SEND_HANDSHAKE_DATA,
      SEQ_NUMBER);
  // some sample data
  uint8_t raw_data[] = {0x6, 0x7, 0x8};
  const size_t raw_data_size = sizeof(raw_data) / sizeof(raw_data[0]);

  const std::vector<uint8_t> vector =
      DeserializeData(handshake_header, raw_data, raw_data_size);

  SecurityQuery query;
  const bool result = query.SerializeQuery(&vector[0], vector.size());
  ASSERT_TRUE(result);
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), handshake_header);
  ASSERT_EQ(query.get_data_size(), raw_data_size);
  // query shall handle own array of byte data
  ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  for (size_t i = 0; i < raw_data_size; ++i) {
    ASSERT_EQ(query.get_data()[i], raw_data[+i]);
  }
  // check side-effects
  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_TRUE(query.get_json_message().empty());

  // Deserialization shall return vector equal serialization vector
  const std::vector<uint8_t> deserialize_vector = query.DeserializeQuery();
  ASSERT_EQ(deserialize_vector, vector);
}
/*
 * SecurityQuery serializes data contains header and binary data
 * with SEND_HANDSHAKE_DATA
 */
TEST_F(SecurityQueryTest, Parse_InternalError) {
  std::string error_str = "{some error}";
  SecurityQuery::QueryHeader internal_error_header(
      SecurityQuery::REQUEST, SecurityQuery::SEND_INTERNAL_ERROR, SEQ_NUMBER);
  internal_error_header.json_size = error_str.size();

  const uint8_t* raw_data = reinterpret_cast<const uint8_t*>(error_str.c_str());

  const std::vector<uint8_t> vector =
      DeserializeData(internal_error_header, raw_data, error_str.size());

  SecurityQuery query;
  const bool result = query.SerializeQuery(&vector[0], vector.size());
  ASSERT_TRUE(result);
  EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(),
                      internal_error_header);
  // check side-effects
  ASSERT_EQ(query.get_data_size(), 0u);
  ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t*>(NULL));
  ASSERT_EQ(query.get_connection_key(), 0u);
  ASSERT_EQ(query.get_json_message(), error_str);

  // Deserialization shall return vector equal serialization vector
  const std::vector<uint8_t> deserialize_vector = query.DeserializeQuery();
  ASSERT_EQ(deserialize_vector, vector);
}

}  // namespace security_manager_test
}  // namespace components
}  // namespace test

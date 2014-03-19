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

#ifndef SECURITY_QUERY_TEST_H
#define SECURITY_QUERY_TEST_H

#include <gtest/gtest.h>
#include <vector>
#include "security_manager/security_query.h"
#include "protocol_handler/protocol_payload.h"
#include "utils/byte_order.h"
#include "security_manager/security_manager_mock.h"

namespace test  {
namespace components  {
namespace security_manager_test {

  using ::security_manager::SecurityQuery;

  class SecurityQueryTest: public ::testing::Test {
   protected:
    void SetUp() OVERRIDE {
      header_size = sizeof(SecurityQuery::QueryHeader);
      query_type = SecurityQuery::NOTIFICATION;
      query_id = SecurityQuery::SEND_HANDSHAKE_DATA;
      //Get any not NULL data for comfortable compare
      seq_number = 0x12345678;
      json_size = 0x1;
      connection_key = 0xABCDEF0;

      init_header.query_type = query_type;
      init_header.query_id = query_id;
      init_header.seq_number = seq_number;

      invalide_header = SecurityQuery::QueryHeader(
            SecurityQuery::INVALID_QUERY_TYPE,
            SecurityQuery::INVALID_QUERY_ID, 0);
    }
    /*
    * Wrapper for fast call SecurityQuery::ParseQuery
    */
    bool ParseQuery(SecurityQuery &query,
                    SecurityQuery::QueryHeader header,
                    const uint8_t * const binary_data,
                    const size_t bin_data_size) {
      //convert to Big-Endian (network) order
      header.query_id  = LE_TO_BE32(header.query_id << 8);
      header.json_size = LE_TO_BE32(header.json_size);
      std::vector<uint8_t> vector;
      vector.reserve(header_size + bin_data_size);
      const uint8_t* header_data = reinterpret_cast<uint8_t*>(&header);
      vector.insert(vector.end(), header_data, header_data + header_size);
      vector.insert(vector.end(), binary_data, binary_data + bin_data_size);

      return query.ParseQuery(&vector[0], vector.size());
    }
    size_t header_size;
    uint8_t query_type;
    uint32_t query_id, seq_number, json_size, connection_key;
    SecurityQuery::QueryHeader init_header, invalide_header;
  };
  /*
   * Security QueryHeader shall be the same size as RPC header
   * for correct working on Mobile side
   */
  TEST_F(SecurityQueryTest, Equal_RPCHeader) {
    ASSERT_EQ(sizeof(SecurityQuery::QueryHeader)*8,
              ::protocol_handler::ProtocolPayloadV2SizeBits());
  }
  /*
   * Security QueryHeader shall construct with Invalid fields
   */
  TEST_F(SecurityQueryTest, QueryHeaderConstructor) {
    SecurityQuery::QueryHeader new_header;

    EXPECT_PRED_FORMAT2(QueryHeader_EQ, new_header, invalide_header);
  }
  /*
  /*
   * Security QueryHeader shall construct with correct fields
   */
  TEST_F(SecurityQueryTest, QueryHeaderConstructor2) {
    SecurityQuery::QueryHeader new_header(query_type, query_id, seq_number);

    EXPECT_PRED_FORMAT2(QueryHeader_EQ, new_header, init_header);
  }
  /*
   * Security QueryHeader shall construct with NULL fields
   */
  TEST_F(SecurityQueryTest, QueryConstructor) {
    SecurityQuery query;

    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), 0);
    ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    ASSERT_TRUE(query.get_json_message().empty());
    EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), invalide_header);
  }
  /*
   * Security QueryHeader shall construct with specified fields
   */
  TEST_F(SecurityQueryTest, QueryConstructor2) {
    SecurityQuery query(init_header, connection_key);

    ASSERT_EQ(query.get_connection_key(), connection_key);
    ASSERT_EQ(query.get_data_size(), 0);
    ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    ASSERT_TRUE(query.get_json_message().empty());
    EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), init_header);
  }

  /*
   * Security QueryHeader setters
   */
  TEST_F(SecurityQueryTest, Setters) {
    const std::string str = "test example string";
    uint8_t raw_data[] = {0x6, 0x7, 0x8};
    const size_t raw_data_size =
        sizeof(raw_data)/sizeof(raw_data[0]);

    SecurityQuery query;
    query.set_connection_key(connection_key);
    query.set_data(raw_data, raw_data_size);
    query.set_json_message(str);
    query.set_header(init_header);

    ASSERT_EQ(query.get_connection_key(), connection_key);
    ASSERT_EQ(query.get_data_size(), raw_data_size);
    ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    for (int i = 0; i < raw_data_size; ++i) {
      ASSERT_EQ(query.get_data()[i], raw_data[i]);
      }
    ASSERT_EQ(query.get_json_message(), str);
    EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), init_header);
  }
  /*
   * Security QueryHeader Parse NULL data
   */
  TEST_F(SecurityQueryTest, Parse_NullData) {
    SecurityQuery query;
    const bool result_parse = query.ParseQuery(NULL, 0);

    ASSERT_FALSE(result_parse);
    //check side-effects
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), 0);
    ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    ASSERT_TRUE(query.get_json_message().empty());
    EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), invalide_header);
  }
  /*
   * Security QueryHeader Parse few (less header size) data
   */
  TEST_F(SecurityQueryTest, Parse_LessHeaderData) {
    std::vector<uint8_t> vector(header_size - 1, 0);

    SecurityQuery query;
    const bool result_parse = query.ParseQuery(&vector[0], vector.size());

    ASSERT_FALSE(result_parse);
    //check side-effects
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), 0);
    ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    ASSERT_TRUE(query.get_json_message().empty());
    EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), invalide_header);
  }
  /*
   * Security QueryHeader Parse few (equal header size) data
   */
  TEST_F(SecurityQueryTest, Parse_LessBinaryData) {
    SecurityQuery query;
    const bool result_parse =
        ParseQuery(query, init_header, NULL, 0);

    ASSERT_FALSE(result_parse);
    //check side-effects
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), 0);
    ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    ASSERT_TRUE(query.get_json_message().empty());
    EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), init_header);
  }
  /*
   * Security QueryHeader Parse data contains header and binary data
   * with INVALID_QUERY_TYPE
   */
  TEST_F(SecurityQueryTest, Parse_InvalideQuery) {
    SecurityQuery::QueryHeader invalide_query_header(
                SecurityQuery::INVALID_QUERY_TYPE,
                SecurityQuery::INVALID_QUERY_ID, seq_number);

    //some sample data
    uint8_t raw_data[] = {0x6, 0x7, 0x8};
    const size_t raw_data_size =
        sizeof(raw_data)/sizeof(raw_data[0]);

    SecurityQuery query;
    const bool result_parse =
        ParseQuery(query, invalide_query_header, raw_data, raw_data_size);
    ASSERT_TRUE(result_parse);
    EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), invalide_query_header);
    ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    for (int i = 0; i < raw_data_size; ++i) {
      ASSERT_EQ(query.get_data()[i], raw_data[+ i]);
      }
    //check side-effects
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), raw_data_size);
    ASSERT_TRUE(query.get_json_message().empty());
  }
  /*
   * Security QueryHeader Parse data contains header and binary data
   * with INVALID_QUERY_TYPE
   */
  TEST_F(SecurityQueryTest, Parse_Handshake) {
    SecurityQuery::QueryHeader handshake_header(
                SecurityQuery::NOTIFICATION,
                SecurityQuery::SEND_HANDSHAKE_DATA, seq_number);
    //some sample data
    uint8_t raw_data[] = {0x6, 0x7, 0x8};
    const size_t raw_data_size =
        sizeof(raw_data)/sizeof(raw_data[0]);

    SecurityQuery query;
    const bool result_parse =
        ParseQuery(query, handshake_header, raw_data, raw_data_size);
    ASSERT_TRUE(result_parse);
    EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), handshake_header);
    ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    for (int i = 0; i < raw_data_size; ++i) {
      ASSERT_EQ(query.get_data()[i], raw_data[+ i]);
      }
    //check side-effects
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), raw_data_size);
    ASSERT_TRUE(query.get_json_message().empty());
  }
  /*
   * Security QueryHeader Parse data contains header and binary data
   * with SEND_HANDSHAKE_DATA
   */
  TEST_F(SecurityQueryTest, Parse_InternalError) {
    std::string error_str = "{some error}";
    SecurityQuery::QueryHeader internal_error_header(
                SecurityQuery::REQUEST,
                SecurityQuery::SEND_INTERNAL_ERROR, seq_number);
    internal_error_header.json_size = error_str.size();

    const uint8_t* raw_data = reinterpret_cast<const uint8_t*>(error_str.c_str());

    SecurityQuery query;
    const bool result_parse =
        ParseQuery(query, internal_error_header, raw_data, error_str.size());
    ASSERT_TRUE(result_parse);
    EXPECT_PRED_FORMAT2(QueryHeader_EQ, query.get_header(), internal_error_header);
    //check side-effects
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), 0);
    ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    ASSERT_EQ(query.get_json_message(), error_str);
  }
} // security_manager_test
} // namespace components
} // namespace test
#endif // SECURITY_QUERY_TEST_H

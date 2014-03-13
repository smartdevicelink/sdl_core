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
#include "security_manager/security_query.h"
#include "protocol_handler/protocol_payload.h"
#include "utils/byte_order.h"

namespace test  {
namespace components  {
namespace security_manager_test {

  using ::security_manager::SecurityQuery;

  class SecurityQueryTest: public ::testing::Test {
   protected:
    void SetUp() OVERRIDE {
    }
    void TearDown() OVERRIDE {
    }
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
   * Security QueryHeader shall construct with NULL fields
   */
  TEST_F(SecurityQueryTest, QueryHeaderConstructor) {
    const  uint8_t query_type = 0xA;
    const  uint32_t query_id = 0xB;
    const  uint32_t seq_number = 0xC;
    SecurityQuery::QueryHeader header (query_type, query_id, seq_number);
    ASSERT_EQ(header.query_id, query_id);
    ASSERT_EQ(header.query_type, query_type);
    ASSERT_EQ(header.reserved, 0x0);
    ASSERT_EQ(header.seq_number, seq_number);
  }
  /*
   * Security QueryHeader shall construct with NULL fields
   */
  TEST_F(SecurityQueryTest, QueryConstructor) {
    SecurityQuery query;
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), 0);
    ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    const SecurityQuery::QueryHeader& header = query.get_header();
    ASSERT_EQ(header.query_type, SecurityQuery::INVALID_QUERY_TYPE);
    ASSERT_EQ(header.query_id, SecurityQuery::INVALID_QUERY_ID);
    ASSERT_EQ(header.seq_number, 0x0);
    ASSERT_EQ(header.reserved, 0x0);
  }
  /*
   * Security QueryHeader shall construct with specified fields
   */
  TEST_F(SecurityQueryTest, QueryConstructor2) {
    const  uint8_t query_type = 0xA;
    const  uint32_t query_id = 0xB;
    const  uint32_t seq_number = 0xC;
    const  uint32_t connection_key = 0xD;
    SecurityQuery::QueryHeader init_header (query_type, query_id, seq_number);

    SecurityQuery query(init_header, connection_key);

    ASSERT_EQ(query.get_connection_key(), connection_key);
    ASSERT_EQ(query.get_data_size(), 0);
    ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    const SecurityQuery::QueryHeader& header = query.get_header();
    ASSERT_EQ(header.query_type, query_type);
    ASSERT_EQ(header.query_id, query_id);
    ASSERT_EQ(header.seq_number, seq_number);
    ASSERT_EQ(header.reserved, 0x0);
  }

  /*
   * Security QueryHeader setters
   */
  TEST_F(SecurityQueryTest, Setters) {
    const  uint8_t query_type = 0xA;
    const  uint32_t query_id = 0xB;
    const  uint32_t seq_number = 0xC;
    const  uint32_t connection_key = 0xD;
    SecurityQuery::QueryHeader init_header (query_type, query_id, seq_number);
    const size_t data_size = sizeof(SecurityQuery::QueryHeader);
    const uint8_t* data = new uint8_t[data_size];

    SecurityQuery query;
    query.set_connection_key(connection_key);
    query.set_data(data, data_size);
    query.set_header(init_header);

    ASSERT_EQ(query.get_connection_key(), connection_key);
    ASSERT_EQ(query.get_data_size(), data_size);
    ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    for (int i = 0; i < data_size; ++i) {
      ASSERT_EQ(query.get_data()[i], data[i]);
      }
    const SecurityQuery::QueryHeader& header = query.get_header();
    ASSERT_EQ(header.query_type, query_type);
    ASSERT_EQ(header.query_id, query_id);
    ASSERT_EQ(header.seq_number, seq_number);
    ASSERT_EQ(header.reserved, 0x0);

    delete []data;
  }
  /*
   * Security QueryHeader Parse data contains only header
   * with PROTECT_SERVICE_REQUEST
   */
  TEST_F(SecurityQueryTest, Parse_NullBinDataRequest) {
    const uint8_t query_type = SecurityQuery::REQUEST;
    const uint32_t query_id = SecurityQuery::PROTECT_SERVICE_REQUEST;
    const uint32_t seq_number = 0x0A0B0C0D;
    const size_t data_size = sizeof(SecurityQuery::QueryHeader);
    uint8_t* data = new uint8_t[data_size];
    *reinterpret_cast<uint32_t*>(data) = LE_TO_BE32(query_id);
    data[0] = query_type;
    *reinterpret_cast<uint32_t*>(data+4)=seq_number;

    SecurityQuery query;
    const bool result_parse = query.Parse(data, data_size);

    ASSERT_TRUE(result_parse);
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), 0);
    ASSERT_EQ(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    const SecurityQuery::QueryHeader& header = query.get_header();
    ASSERT_EQ(header.query_type, query_type);
    ASSERT_EQ(header.query_id, query_id);
    ASSERT_EQ(header.seq_number, seq_number);
    ASSERT_EQ(header.reserved, 0x0);

    delete []data;
  }
  /*
   * Security QueryHeader Parse data contains header and binary data
   * with PROTECT_SERVICE_RESPONSE
   */
  TEST_F(SecurityQueryTest, Parse_Response) {
    const  uint8_t query_type = SecurityQuery::RESPONSE;
    const  uint32_t query_id = SecurityQuery::PROTECT_SERVICE_RESPONSE;
    const  uint32_t seq_number = 0x0A0B0C0D;
    const size_t add_size = 1;
    const size_t data_size = sizeof(SecurityQuery::QueryHeader) + add_size;
    uint8_t* data = new uint8_t[data_size];
    *reinterpret_cast<uint32_t*>(data) = LE_TO_BE32(query_id);
    data[0] = query_type;
    *reinterpret_cast<uint32_t*>(data+4)=seq_number;

    SecurityQuery query;
    const bool result_parse = query.Parse(data, data_size);

    ASSERT_TRUE(result_parse);
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), add_size);
    ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    const SecurityQuery::QueryHeader& header = query.get_header();
    ASSERT_EQ(header.query_type, query_type);
    ASSERT_EQ(header.query_id, query_id);
    ASSERT_EQ(header.seq_number, seq_number);
    ASSERT_EQ(header.reserved, 0x0);

    delete []data;
  }
  /*
   * Security QueryHeader Parse data contains header and binary data
   * with SEND_HANDSHAKE_DATA
   */
  TEST_F(SecurityQueryTest, Parse_Handshake) {
    const  uint8_t query_type = SecurityQuery::NOTIFICATION;
    const  uint32_t query_id = SecurityQuery::SEND_HANDSHAKE_DATA;
    const  uint32_t seq_number = 0x0A0B0C0D;
    const size_t header_size = sizeof(SecurityQuery::QueryHeader);
    const size_t add_size = 100;
    const size_t data_size = header_size + add_size;
    uint8_t* data = new uint8_t[data_size];
    *reinterpret_cast<uint32_t*>(data) = LE_TO_BE32(query_id);
    data[0] = query_type;
    *reinterpret_cast<uint32_t*>(data+4)=seq_number;

    SecurityQuery query;
    const bool result_parse = query.Parse(data, data_size);

    ASSERT_TRUE(result_parse);
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), add_size);
    ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    for (int i = 0; i < add_size; ++i) {
      ASSERT_EQ(query.get_data()[i], data[header_size + i]);
      }
    const SecurityQuery::QueryHeader& header = query.get_header();
    ASSERT_EQ(header.query_type, query_type);
    ASSERT_EQ(header.query_id, query_id);
    ASSERT_EQ(header.seq_number, seq_number);
    ASSERT_EQ(header.reserved, 0x0);

    delete []data;
  }
  /*
   * Security QueryHeader Parse data contains header and binary data
   * with SEND_HANDSHAKE_DATA
   */
  TEST_F(SecurityQueryTest, Parse_InvalideQuery) {
    const  uint8_t query_type = SecurityQuery::INVALID_QUERY_TYPE;
    const  uint32_t query_id = SecurityQuery::INVALID_QUERY_ID;
    const  uint32_t seq_number = 0x0A0B0C0D;
    const size_t header_size = sizeof(SecurityQuery::QueryHeader);
    const size_t add_size = 100;
    const size_t data_size = header_size + add_size;
    uint8_t* data = new uint8_t[data_size];
    *reinterpret_cast<uint32_t*>(data) = LE_TO_BE32(query_id);
    data[0] = query_type;
    *reinterpret_cast<uint32_t*>(data+4)=seq_number;

    SecurityQuery query;
    const bool result_parse = query.Parse(data, data_size);

    ASSERT_TRUE(result_parse);
    ASSERT_EQ(query.get_connection_key(), 0);
    ASSERT_EQ(query.get_data_size(), add_size);
    ASSERT_NE(query.get_data(), reinterpret_cast<uint8_t *>(NULL));
    for (int i = 0; i < add_size; ++i) {
      ASSERT_EQ(query.get_data()[i], data[header_size + i]);
      }
    const SecurityQuery::QueryHeader& header = query.get_header();
    ASSERT_EQ(header.query_type, query_type);
    ASSERT_EQ(header.query_id, query_id);
    ASSERT_EQ(header.seq_number, seq_number);
    ASSERT_EQ(header.reserved, 0x0);

    delete []data;
  }
} // security_manager_test
} // namespace components
} // namespace test
#endif // SECURITY_QUERY_TEST_H

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
    uint8_t query_type = 0xA;
    uint32_t query_id = 0xB;
    uint32_t seq_number = 0xC;
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
    ASSERT_EQ(query.getConnectionKey(), 0);
    ASSERT_EQ(query.getDataSize(), 0);
    ASSERT_EQ(query.getData(), reinterpret_cast<uint8_t *>(NULL));
    const SecurityQuery::QueryHeader& header = query.getHeader();
    ASSERT_EQ(header.query_id, SecurityQuery::INVALID_QUERY_ID);
    ASSERT_EQ(header.query_type, SecurityQuery::INVALID_QUERY_TYPE);
    ASSERT_EQ(header.reserved, 0x0);
    ASSERT_EQ(header.seq_number, 0x0);
  }
  /*
   * Security QueryHeader shall construct with specified fields
   */
  TEST_F(SecurityQueryTest, QueryConstructor2) {
    uint8_t query_type = 0xA;
    uint32_t query_id = 0xB;
    uint32_t seq_number = 0xC;
    uint32_t connection_key = 0xD;
    SecurityQuery::QueryHeader init_header (query_type, query_id, seq_number);
    SecurityQuery query(init_header, connection_key);
    ASSERT_EQ(query.getConnectionKey(), connection_key);
    ASSERT_EQ(query.getDataSize(), 0);
    ASSERT_EQ(query.getData(), reinterpret_cast<uint8_t *>(NULL));
    const SecurityQuery::QueryHeader& header = query.getHeader();
    ASSERT_EQ(header.query_id, query_id);
    ASSERT_EQ(header.query_type, query_type);
    ASSERT_EQ(header.reserved, 0x0);
    ASSERT_EQ(header.seq_number, seq_number);
  }

} // security_manager_test
} // namespace components
} // namespace test
#endif // SECURITY_QUERY_TEST_H

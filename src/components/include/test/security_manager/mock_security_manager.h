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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SECURITY_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SECURITY_MANAGER_H_

#include <string>
#include <list>
#include "gmock/gmock.h"
#include "utils/byte_order.h"
#include "security_manager/security_manager.h"
#include "security_manager/security_query.h"

namespace test {
namespace components {
namespace security_manager_test {

class MockSecurityManager : public ::security_manager::SecurityManager {
 public:
  MOCK_METHOD1(set_session_observer,
               void(::protocol_handler::SessionObserver*));
  MOCK_METHOD1(set_protocol_handler,
               void(::protocol_handler::ProtocolHandler*));
  MOCK_METHOD1(set_crypto_manager, void(::security_manager::CryptoManager*));
  MOCK_METHOD4(SendInternalError, void(const uint32_t, const uint8_t&,
                                       const std::string&, const uint32_t));
  MOCK_METHOD1(CreateSSLContext,
               ::security_manager::SSLContext*(const uint32_t&));
  MOCK_METHOD1(StartHandshake, void(uint32_t));
  MOCK_METHOD1(AddListener, void(::security_manager::SecurityManagerListener*));
  MOCK_METHOD1(RemoveListener,
               void(::security_manager::SecurityManagerListener*));
  // protocol_handler::ProtocolObserver part
  MOCK_METHOD1(OnMessageReceived,
               void(const ::protocol_handler::RawMessagePtr));
  MOCK_METHOD1(OnMobileMessageSent,
               void(const ::protocol_handler::RawMessagePtr));
};

/*
 * Matcher for RawMessages
 * Check binary data of RawMessages
 */
MATCHER_P(RawMessageEqSize, exp_data_size,
          std::string(negation ? "is not" : "is") + " RawMessages ") {
  const size_t arg_data_size = arg->data_size();
  if (arg_data_size != exp_data_size) {
    *result_listener << "Got " << arg_data_size << " bytes "
                     << " expected " << exp_data_size << " bytes";
    return false;
  }
  return true;
}
MATCHER_P2(RawMessageEq, exp_data, exp_data_size,
           std::string(negation ? "is not" : "is") + " RawMessages ") {
  const size_t arg_data_size = arg->data_size();
  if (arg_data_size != exp_data_size) {
    *result_listener << "Got " << arg_data_size << " bytes "
                     << " expected " << exp_data_size << " bytes";
    return false;
  }
  const uint8_t* arg_data = arg->data();
  for (uint32_t i = 0; i < arg_data_size; ++i) {
    if (arg_data[i] != exp_data[i]) {
      *result_listener << "Fail in " << i << " byte";
      return false;
    }
  }
  return true;
}

/*
 * Matcher for Handshake data
 */
MATCHER_P2(HandshakeStepEq, exp_data, exp_data_size,
           std::string(negation ? "is not" : "is") + " Handshake ") {
  const uint8_t* arg_data = arg;
  for (uint32_t i = 0; i < exp_data_size; ++i) {
    if (arg_data[i] != exp_data[i]) {
      *result_listener << "Fail in " << i << " byte";
      return false;
    }
  }
  return true;
}

/*
 * Matcher for checking RawMessage with InternalError Query
 * Check error id
 */
MATCHER_P(InternalErrorWithErrId, expectedErrorId,
          std::string(negation ? "is not" : "is") +
              " InternalError with selected error") {
  const size_t header_size =
      sizeof(security_manager::SecurityQuery::QueryHeader);
  if (arg->data_size() <= header_size) {
    *result_listener << "Size " << arg->data_size()
                     << " bytes less or equal sizeof(QueryHeader)="
                     << header_size;
    return false;
  }
  const uint8_t* data = arg->data();
  const uint8_t query_type = data[0];
  if (security_manager::SecurityQuery::NOTIFICATION != query_type) {
    *result_listener << "RawMessage is not notification, type=0x" << std::hex
                     << static_cast<int>(query_type);
    return false;
  }
  // Read Big-Endian number
  const uint32_t query_id = data[1] << 16 | data[2] << 8 | data[3];
  if (security_manager::SecurityQuery::SEND_INTERNAL_ERROR != query_id) {
    *result_listener << "Notification is not InternalError, id=0x" << std::hex
                     << query_id;
    return false;
  }
  const uint32_t json_size =
      data[8] << 24 | data[9] << 16 | data[10] << 8 | data[11];
  if (header_size + json_size >= arg->data_size()) {
    *result_listener << "InternalError contains only JSON data.";
    return false;
  }
  // Read err_id as bin data number
  const uint8_t* err_id =
      reinterpret_cast<const uint8_t*>(data + header_size + json_size);
  if (expectedErrorId != *err_id) {
    *result_listener << "InternalError id " << static_cast<int>(*err_id)
                     << " and not equal error " << expectedErrorId;
    return false;
  }
  return true;
}

}  // namespace security_manager_test
}  // namespace components
}  // namespace test
   /*
    * Matcher for checking QueryHeader equal in GTests
    */
::testing::AssertionResult QueryHeader_EQ(
    const char* m_expr, const char* n_expr,
    const ::security_manager::SecurityQuery::QueryHeader& q1,
    const ::security_manager::SecurityQuery::QueryHeader& q2);

#endif  // SRC_COMPONENTS_INCLUDE_TEST_SECURITY_MANAGER_MOCK_SECURITY_MANAGER_H_

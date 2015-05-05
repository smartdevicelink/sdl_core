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

#ifndef TEST_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_MOCK_H_
#define TEST_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_MOCK_H_
#include <gmock/gmock.h>
#include <string>
#include <list>
#include "utils/byte_order.h"
#include "security_manager/security_manager.h"
#include "security_manager/ssl_context.h"
#include "security_manager/security_query.h"

namespace test  {
namespace components  {
namespace security_manager_test {
  /*
   * MOCK implementation of protocol_handler::SessionObserver interface
   */
  class SessionObserverMock: public protocol_handler::SessionObserver {
   public:
    MOCK_METHOD2(SetSSLContext,
                 int (const uint32_t& key,
                      security_manager::SSLContext* context));
    MOCK_METHOD2(GetSSLContext,
                 security_manager::SSLContext* (
                   const uint32_t& key,
                   const protocol_handler::ServiceType& service_type));
    MOCK_METHOD2(SetProtectionFlag,
                 void(
                   const uint32_t& key,
                   const protocol_handler::ServiceType& service_type));
    MOCK_METHOD4(OnSessionStartedCallback,
                 uint32_t(
                   const transport_manager::ConnectionUID& connection_handle,
                   const uint8_t session_id,
                   const protocol_handler::ServiceType& service_type,
                   const bool is_protected, uint32_t* hash_id));
    MOCK_METHOD4(OnSessionEndedCallback,
                 uint32_t(
                   const transport_manager::ConnectionUID& connection_handle,
                   const uint8_t sessionId,
                   const uint32_t& hashCode,
                   const protocol_handler::ServiceType& service_type));
    MOCK_METHOD1(OnApplicationFloodCallBack,
                 void(const uint32_t&));
    MOCK_METHOD2(KeyFromPair,
                 uint32_t(
                   transport_manager::ConnectionUID connection_handle,
                   uint8_t sessionId));
    MOCK_METHOD3(PairFromKey,
                 void(
                   uint32_t key,
                   transport_manager::ConnectionUID* connection_handle,
                   uint8_t* sessionId));
    MOCK_METHOD4(GetDataOnSessionKey,
                 int32_t(uint32_t key,
                         uint32_t* app_id,
                         std::list<int32_t>* sessions_list,
                         uint32_t* device_id));
    MOCK_METHOD4(GetDataOnDeviceID,
                 int32_t(
                   uint32_t device_handle,
                   std::string* device_name,
                   std::list<uint32_t>* applications_list,
                   std::string* mac_address));
    MOCK_METHOD2(IsHeartBeatSupported,
                 bool( transport_manager::ConnectionUID connection_handle,
                       uint8_t session_id));
    MOCK_METHOD3(ProtocolVersionUsed,
                 bool(uint32_t connection_id, uint8_t session_id,
                      uint8_t& protocol_version));

  };

  /*
   * MOCK implementation of protocol_handler::ProtocolObserver interface
   */
  class ProtocoloObserverMock: public protocol_handler::ProtocolHandler {
  public:
    MOCK_METHOD2(SendMessageToMobileApp,
                 void(const protocol_handler::RawMessagePtr message,
                      bool final_message));
    MOCK_METHOD1(AddProtocolObserver,
                 void(protocol_handler::ProtocolObserver* observer));
    MOCK_METHOD1(RemoveProtocolObserver,
                 void(protocol_handler::ProtocolObserver* observer));
    MOCK_METHOD2(SendFramesNumber,
                 void(uint32_t connection_key, int32_t number_of_frames));
    MOCK_METHOD2(SendHeartBeat,
                 void(int32_t connection_id, uint8_t session_id));
    MOCK_METHOD2(SendEndSession,
                 void(int32_t connection_id, uint8_t session_id));
  };
  /*
   * MOCK implementation of security_manager::SSLContext interface
   */
  class CryptoManagerMock: public security_manager::CryptoManager {
  public:
    MOCK_METHOD6(Init,
                 bool (security_manager::Mode mode,
                       security_manager::Protocol protocol,
                       const std::string& cert_filename,
                       const std::string& key_filename,
                       const std::string& ciphers_list,
                       bool verify_peer));
    MOCK_METHOD0(Finish,
                 void ());
    MOCK_METHOD0(CreateSSLContext,
                 security_manager::SSLContext* ());
    MOCK_METHOD1(ReleaseSSLContext,
                 void(security_manager::SSLContext*));
    MOCK_CONST_METHOD0(LastError,
                       std::string());
  };
  /*
   * MOCK implementation of security_manager::SSLContext interface
   */
  class SSLContextMock: public security_manager::SSLContext {
  public:
    MOCK_CONST_METHOD0(mode, int ());
    MOCK_METHOD2(StartHandshake,
                 security_manager::SSLContext::HandshakeResult (
                   const uint8_t** const, size_t*));
    MOCK_METHOD4(DoHandshakeStep,
                 security_manager::SSLContext::HandshakeResult (
                   const uint8_t* const,  size_t,
                   const uint8_t** const, size_t*));
    MOCK_METHOD4(Encrypt,
                 bool (const uint8_t* const,  size_t,
                       const uint8_t** const, size_t*));
    MOCK_METHOD4(Decrypt,
                 bool (const uint8_t* const,  size_t,
                       const uint8_t** const, size_t*));
    MOCK_CONST_METHOD1(get_max_block_size, size_t (size_t));
    MOCK_CONST_METHOD0(IsInitCompleted, bool());
    MOCK_CONST_METHOD0(IsHandshakePending, bool());
    MOCK_CONST_METHOD0(LastError,
                       std::string());
  };
  /*
   * MOCK implementation of security_manager::SecurityManagerListener
   */
  class SMListenerMock: public security_manager::SecurityManagerListener {
  public:
    MOCK_METHOD2(OnHandshakeDone,
                       bool(uint32_t connection_key,
                            bool success));
  };

  /*
   * Matcher for RawMessages
   * Check binary data of RawMessages
   */
  MATCHER_P2(RawMessageEq, exp_data, exp_data_size,
             std::string(negation ? "is not" : "is") + " RawMessages ") {
    const size_t arg_data_size = arg->data_size();
    if (arg_data_size != exp_data_size) {
      *result_listener << "Got " << arg_data_size << " bytes "
                      << " expected " << exp_data_size << " bytes";
      return false;
    }
    const uint8_t *arg_data = arg->data();
    for (int i = 0; i < arg_data_size; ++i) {
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
            std::string(negation ? "is not" : "is")
            + " InternalError with selected error" ) {
    const size_t header_size = sizeof(security_manager::SecurityQuery::QueryHeader);
    if (arg->data_size() <= header_size) {
      *result_listener << "Size " << arg->data_size()
                       << " bytes less or equal sizeof(QueryHeader)="
                       << header_size;
      return false;
    }
    const uint8_t *data = arg->data();
    const uint8_t query_type = data[0];
    if (security_manager::SecurityQuery::NOTIFICATION != query_type) {
      *result_listener << "RawMessage is not notification, type=0x"
                       << std::hex << static_cast<int>(query_type);
      return false;
    }
    // Read Big-Endian number
    const uint32_t query_id = data[1] << 16 |
                              data[2] <<  8 |
                              data[3];
    if (security_manager::SecurityQuery::SEND_INTERNAL_ERROR != query_id) {
      *result_listener << "Notification is not InternalError, id=0x"
                       << std::hex << query_id;
      return false;
    }
    const uint32_t json_size = data[8]  << 24 |
                               data[9]  << 16 |
                               data[10] <<  8 |
                               data[11];
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

#endif  // TEST_COMPONENTS_SECURITY_MANAGER_INCLUDE_SECURITY_MANAGER_SECURITY_MANAGER_MOCK_H_

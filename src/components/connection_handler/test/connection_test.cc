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

#include <algorithm>

#include "gtest/gtest.h"

#include "protocol/common.h"
#include "connection_handler/connection.h"
#include "connection_handler/connection_handler_impl.h"
#include "protocol/service_type.h"
#include "connection_handler/mock_connection_handler_settings.h"
#include "transport_manager/mock_transport_manager.h"
#include "security_manager/mock_security_manager.h"
#include "security_manager/mock_ssl_context.h"

#define EXPECT_RETURN_TRUE true
#define EXPECT_RETURN_FALSE false
#define EXPECT_SERVICE_EXISTS true
#define EXPECT_SERVICE_NOT_EXISTS false

namespace test {
namespace components {
namespace connection_handler_test {
using namespace ::connection_handler;
using namespace ::protocol_handler;

class ConnectionTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    connection_handler_ = new ConnectionHandlerImpl(
        mock_connection_handler_settings, transport_manager_mock);
    const ConnectionHandle connectionHandle = 0;
    const DeviceHandle device_handle = 0u;
    const uint32_t heart_beat = 10000u;
    connection_ = new Connection(connectionHandle,
                                 device_handle,
                                 connection_handler_,
                                 heart_beat);
  }

  void TearDown() OVERRIDE {
    delete connection_;
    delete connection_handler_;
  }
  void StartSession() {
    session_id = connection_->AddNewSession();
    EXPECT_NE(session_id, 0u);
    const SessionMap sessionMap = connection_->session_map();
    EXPECT_FALSE(sessionMap.empty());
    const ServiceList serviceList = sessionMap.begin()->second.service_list;
    EXPECT_FALSE(serviceList.empty());
    const ServiceList::const_iterator it =
        std::find(serviceList.begin(), serviceList.end(), kRpc);
    const bool found_result = (it != serviceList.end());
    EXPECT_TRUE(found_result);
  }
  void AddNewService(const ServiceType service_type, const bool protection,
                     const bool expect_add_new_service_call_result,
                     const bool expect_exist_service) {
    const bool result =
        connection_->AddNewService(session_id, service_type, protection);
    EXPECT_EQ(result, expect_add_new_service_call_result);

#ifdef ENABLE_SECURITY
    if (protection) {
      connection_->SetProtectionFlag(session_id, service_type);
    }
#endif  // ENABLE_SECURITY
    const SessionMap session_map = connection_->session_map();
    EXPECT_FALSE(session_map.empty());
    const ServiceList newServiceList = session_map.begin()->second.service_list;
    EXPECT_FALSE(newServiceList.empty());
    const ServiceList::const_iterator it = std::find(newServiceList.begin(),
                                                     newServiceList.end(),
                                                     service_type);
    const bool found_result = it != newServiceList.end();
    EXPECT_EQ(expect_exist_service, found_result);
#ifdef ENABLE_SECURITY
    if (found_result) {
      const Service& service = *it;
      EXPECT_EQ(service.is_protected_, protection);
    }
#endif  // ENABLE_SECURITY
  }

  void RemoveService(const ServiceType service_type,
                     const bool expect_remove_service_result,
                     const bool expect_exist_service) {
    const bool result = connection_->RemoveService(session_id, service_type);
    EXPECT_EQ(result, expect_remove_service_result);

    const SessionMap newSessionMap = connection_->session_map();
    EXPECT_FALSE(newSessionMap.empty());
    const ServiceList newServiceList =
        newSessionMap.begin()->second.service_list;
    EXPECT_FALSE(newServiceList.empty());
    const ServiceList::const_iterator it =
        std::find(newServiceList.begin(), newServiceList.end(), service_type);
    const bool found_result = (it != newServiceList.end());
    EXPECT_EQ(expect_exist_service, found_result);
  }

  Connection* connection_;
  MockConnectionHandlerSettings mock_connection_handler_settings;
  testing::StrictMock<transport_manager_test::MockTransportManager>
      transport_manager_mock;
  ConnectionHandlerImpl* connection_handler_;
  uint32_t session_id;
};

TEST_F(ConnectionTest, Session_TryGetProtocolVersionWithoutSession) {
  uint8_t protocol_version;
  EXPECT_FALSE(connection_->ProtocolVersion(session_id, protocol_version));
}

TEST_F(ConnectionTest, Session_GetDefaultProtocolVersion) {
  StartSession();
  uint8_t protocol_version;
  EXPECT_TRUE(connection_->ProtocolVersion(session_id, protocol_version));
  EXPECT_EQ(static_cast<uint8_t>(PROTOCOL_VERSION_2), protocol_version);
}
TEST_F(ConnectionTest, Session_UpdateProtocolVersion) {
  StartSession();
  uint8_t protocol_version = static_cast<uint8_t>(PROTOCOL_VERSION_3);
  connection_->UpdateProtocolVersionSession(session_id, protocol_version);
  EXPECT_TRUE(connection_->ProtocolVersion(session_id, protocol_version));
  EXPECT_EQ(static_cast<uint8_t>(PROTOCOL_VERSION_3), protocol_version);
}

TEST_F(ConnectionTest, HeartBeat_NotSupported) {
  // Arrange
  StartSession();
  uint8_t protocol_version;
  EXPECT_TRUE(connection_->ProtocolVersion(session_id, protocol_version));
  EXPECT_EQ(static_cast<uint8_t>(PROTOCOL_VERSION_2), protocol_version);

  // Assert
  EXPECT_FALSE(connection_->SupportHeartBeat(session_id));
}

TEST_F(ConnectionTest, HeartBeat_Protocol3_Supported) {
  // Arrange
  StartSession();
  // Check execution if protocol version is 3
  const uint8_t protocol_version = static_cast<uint8_t>(PROTOCOL_VERSION_3);
  connection_->UpdateProtocolVersionSession(session_id, protocol_version);
  EXPECT_TRUE(connection_->SupportHeartBeat(session_id));
}

TEST_F(ConnectionTest, HeartBeat_Protocol4_PositiveHeartBeat_Supported) {
  // Arrange
  StartSession();
  // Check execution if protocol version is 4
  const uint8_t protocol_version = static_cast<uint8_t>(PROTOCOL_VERSION_4);
  connection_->UpdateProtocolVersionSession(session_id, protocol_version);
  EXPECT_TRUE(connection_->SupportHeartBeat(session_id));
}

TEST_F(ConnectionTest, HeartBeat_Protocol4_ZeroHeartBeat_NotSupported) {
  // Correctc of connection (need connection with heartbeat=0)
  delete connection_;
  connection_ = 0;

  const ConnectionHandle connectionHandle = 0;
  const DeviceHandle device_handle = 0u;
  const uint32_t heart_beat = 0u;
  connection_ = new Connection(connectionHandle,
                               device_handle,
                               connection_handler_,
                               heart_beat);
  StartSession();
  // Check execution if protocol version is 4
  const uint8_t protocol_version = static_cast<uint8_t>(PROTOCOL_VERSION_4);
  connection_->UpdateProtocolVersionSession(session_id, protocol_version);
  EXPECT_FALSE(connection_->SupportHeartBeat(session_id));
}

// Try to add service without session
TEST_F(ConnectionTest, Session_AddNewServiceWithoutSession) {
  EXPECT_EQ(connection_->AddNewService(session_id, kAudio, true),
            EXPECT_RETURN_FALSE);
  EXPECT_EQ(connection_->AddNewService(session_id, kAudio, false),
            EXPECT_RETURN_FALSE);
  EXPECT_EQ(connection_->AddNewService(session_id, kMobileNav, true),
            EXPECT_RETURN_FALSE);
  EXPECT_EQ(connection_->AddNewService(session_id, kMobileNav, false),
            EXPECT_RETURN_FALSE);
}

// Try to remove service without session
TEST_F(ConnectionTest, Session_RemoveServiceWithoutSession) {
  EXPECT_EQ(connection_->RemoveService(session_id, kAudio),
            EXPECT_RETURN_FALSE);
  EXPECT_EQ(connection_->RemoveService(session_id, kMobileNav),
            EXPECT_RETURN_FALSE);
  EXPECT_EQ(
      connection_->RemoveService(session_id, protocol_handler::kMobileNav),
      EXPECT_RETURN_FALSE);
}
// Try to remove RPC
TEST_F(ConnectionTest, Session_RemoveRPCBulk) {
  StartSession();
  EXPECT_EQ(connection_->RemoveService(session_id, kRpc), EXPECT_RETURN_FALSE);
  EXPECT_EQ(connection_->RemoveService(session_id, kBulk), EXPECT_RETURN_FALSE);
}
// Control Service could not be started anyway
TEST_F(ConnectionTest, Session_AddControlService) {
  StartSession();
  AddNewService(kControl, PROTECTION_OFF, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_NOT_EXISTS);
  AddNewService(kControl, PROTECTION_ON, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_NOT_EXISTS);
}

// Invalid Services couldnot be started anyway
TEST_F(ConnectionTest, Session_AddInvalidService) {
  StartSession();

  AddNewService(kInvalidServiceType, PROTECTION_OFF, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_NOT_EXISTS);
  AddNewService(kInvalidServiceType, PROTECTION_ON, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_NOT_EXISTS);
}

// RPC and Bulk Services could be only delay protected
TEST_F(ConnectionTest, Session_AddRPCBulkServices) {
  StartSession();
  AddNewService(kRpc, PROTECTION_OFF, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_EXISTS);

  //  Bulk shall not be added and shall be PROTECTION_OFF
  AddNewService(kBulk, PROTECTION_OFF, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_EXISTS);
#ifdef ENABLE_SECURITY
  AddNewService(kRpc, PROTECTION_ON, EXPECT_RETURN_TRUE, EXPECT_SERVICE_EXISTS);
#else
  AddNewService(kRpc, PROTECTION_ON, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_EXISTS);
#endif  // ENABLE_SECURITY

  //  Bulk shall not be added and shall be PROTECTION_ON
  AddNewService(kBulk, PROTECTION_ON, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_EXISTS);
}

TEST_F(ConnectionTest, Session_AddAllOtherService_Unprotected) {
  StartSession();
  AddNewService(kAudio, PROTECTION_OFF, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);
  AddNewService(kMobileNav, PROTECTION_OFF, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);
}

TEST_F(ConnectionTest, Session_AddAllOtherService_Protected) {
  StartSession();
  AddNewService(kAudio, PROTECTION_ON, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);
  AddNewService(kMobileNav, PROTECTION_ON, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);
}

TEST_F(ConnectionTest, FindAddedService) {
  StartSession();

  // Arrange
  SessionMap currentSessionMap = connection_->session_map();
  Service* sessionWithService =
      currentSessionMap.find(session_id)->second.FindService(kAudio);
  EXPECT_EQ(NULL, sessionWithService);

  // Act
  AddNewService(kAudio, PROTECTION_OFF, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);

  currentSessionMap = connection_->session_map();

  // Expect that service is existing
  sessionWithService =
      currentSessionMap.find(session_id)->second.FindService(kAudio);
  EXPECT_TRUE(sessionWithService != NULL);
}

TEST_F(ConnectionTest, Session_RemoveAddedService) {
  StartSession();
  AddNewService(kAudio, PROTECTION_OFF, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);

  EXPECT_EQ(connection_->RemoveService(session_id, kAudio), EXPECT_RETURN_TRUE);

  // Try delete nonexisting service
  EXPECT_EQ(connection_->RemoveService(session_id, kAudio),
            EXPECT_RETURN_FALSE);
}

TEST_F(ConnectionTest, Session_AddAllOtherService_DelayProtected1) {
  StartSession();

  AddNewService(kAudio, PROTECTION_OFF, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);

  AddNewService(kMobileNav, PROTECTION_OFF, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);

#ifdef ENABLE_SECURITY
  AddNewService(kAudio, PROTECTION_ON, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);

  AddNewService(kMobileNav, PROTECTION_ON, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);
#else
  AddNewService(kAudio, PROTECTION_ON, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_EXISTS);

  AddNewService(kMobileNav, PROTECTION_ON, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_EXISTS);
#endif  // ENABLE_SECURITY
}

//  Use other order
TEST_F(ConnectionTest, Session_AddAllOtherService_DelayProtected2) {
  StartSession();
  AddNewService(kAudio, PROTECTION_OFF, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);
#ifdef ENABLE_SECURITY
  AddNewService(kAudio, PROTECTION_ON, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);
#else
  AddNewService(kAudio, PROTECTION_ON, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_EXISTS);
#endif  // ENABLE_SECURITY

  AddNewService(kMobileNav, PROTECTION_OFF, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);
#ifdef ENABLE_SECURITY
  AddNewService(kMobileNav, PROTECTION_ON, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);
#else
  AddNewService(kMobileNav, PROTECTION_ON, EXPECT_RETURN_FALSE,
                EXPECT_SERVICE_EXISTS);
#endif  // ENABLE_SECURITY
}

TEST_F(ConnectionTest, RemoveSession) {
  StartSession();
  EXPECT_EQ(session_id, connection_->RemoveSession(session_id));
  EXPECT_EQ(0u, connection_->RemoveSession(session_id));
}

#ifdef ENABLE_SECURITY

TEST_F(ConnectionTest, SetSSLContextWithoutSession) {
  // random value. Session was not started
  uint8_t session_id = 10;
  security_manager_test::MockSSLContext mock_ssl_context;
  int setResult = connection_->SetSSLContext(session_id, &mock_ssl_context);
  EXPECT_EQ(security_manager::SecurityManager::ERROR_INTERNAL, setResult);
}

TEST_F(ConnectionTest, GetSSLContextWithoutSession) {
  // random value. Session was not started
  uint8_t session_id = 10;

  EXPECT_EQ(NULL, connection_->GetSSLContext(session_id, kMobileNav));
}

TEST_F(ConnectionTest, SetGetSSLContext) {
  StartSession();

  EXPECT_EQ(NULL, connection_->GetSSLContext(session_id, kMobileNav));
  AddNewService(kMobileNav, PROTECTION_ON, EXPECT_RETURN_TRUE,
                EXPECT_SERVICE_EXISTS);

  EXPECT_EQ(NULL, connection_->GetSSLContext(session_id, kMobileNav));

  security_manager_test::MockSSLContext mock_ssl_context;
  // Set SSLContext
  int setResult = connection_->SetSSLContext(session_id, &mock_ssl_context);
  EXPECT_EQ(security_manager::SecurityManager::ERROR_SUCCESS, setResult);

  security_manager::SSLContext* result =
      connection_->GetSSLContext(session_id, kMobileNav);
  EXPECT_EQ(result, &mock_ssl_context);
}

TEST_F(ConnectionTest, SetProtectionFlagForRPC) {
  StartSession();
  // Arrange
  SessionMap currentSessionMap = connection_->session_map();
  Service* service_rpc =
      currentSessionMap.find(session_id)->second.FindService(kRpc);

  EXPECT_FALSE(service_rpc->is_protected_);

  Service* service_bulk =
      currentSessionMap.find(session_id)->second.FindService(kBulk);
  EXPECT_FALSE(service_bulk->is_protected_);

  // Expect that service protection is enabled
  connection_->SetProtectionFlag(session_id, kRpc);

  currentSessionMap = connection_->session_map();
  service_bulk = currentSessionMap.find(session_id)->second.FindService(kBulk);
  EXPECT_TRUE(service_bulk->is_protected_);

  service_rpc = currentSessionMap.find(session_id)->second.FindService(kRpc);
  EXPECT_TRUE(service_rpc->is_protected_);
}

TEST_F(ConnectionTest, SetProtectionFlagForBulk) {
  StartSession();
  // Arrange
  SessionMap currentSessionMap = connection_->session_map();
  Service* service_rpc =
      currentSessionMap.find(session_id)->second.FindService(kRpc);

  EXPECT_FALSE(service_rpc->is_protected_);

  Service* service_bulk =
      currentSessionMap.find(session_id)->second.FindService(kBulk);
  EXPECT_FALSE(service_bulk->is_protected_);

  // Expect that service protection is enabled
  connection_->SetProtectionFlag(session_id, kBulk);

  currentSessionMap = connection_->session_map();
  service_bulk = currentSessionMap.find(session_id)->second.FindService(kBulk);
  EXPECT_TRUE(service_bulk->is_protected_);

  service_rpc = currentSessionMap.find(session_id)->second.FindService(kRpc);
  EXPECT_TRUE(service_rpc->is_protected_);
}

#endif  // ENABLE_SECURITY

}  // namespace connection_handle
}  // namespace components
}  // namespace test

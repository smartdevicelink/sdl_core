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
#ifndef TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_MOCK_H_
#define TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_MOCK_H_

#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_adapter/transport_adapter_event.h"
#include "transport_manager/transport_manager_listener.h"

#include "protocol_handler/session_observer.h"
#include "protocol_handler/protocol_packet.h"
#ifdef ENABLE_SECURITY
#include "security_manager/security_manager.h"
#include "security_manager/security_manager_mock.h"
#endif  // ENABLE_SECURITY

namespace test  {
namespace components  {
namespace protocol_handler_test {

using namespace protocol_handler;
using namespace transport_manager;

/*
 * MOCK implementation of transport_manager::TransportManager interface
 */
class TransportManagerMock: public TransportManager{
 public:
  MOCK_METHOD0(Init,
               int());
  MOCK_METHOD0(SearchDevices,
               int());
  MOCK_METHOD1(ConnectDevice,
               int(const DeviceHandle&));
  MOCK_METHOD1(DisconnectDevice,
               int(const DeviceHandle&));
  MOCK_METHOD1(Disconnect,
               int(const ConnectionUID &));
  MOCK_METHOD1(DisconnectForce,
               int(const ConnectionUID &));
  MOCK_METHOD1(SendMessageToDevice,
               int(const ::protocol_handler::RawMessagePtr));
  MOCK_METHOD1(ReceiveEventFromDevice,
               int(const TransportAdapterEvent&));
  MOCK_METHOD1(AddTransportAdapter,
               int(transport_adapter::TransportAdapter *));
  MOCK_METHOD1(AddEventListener,
               int(TransportManagerListener *));
  MOCK_METHOD0(Stop,
               int());
  MOCK_METHOD1(RemoveDevice,
               int(const DeviceHandle& ));
  MOCK_CONST_METHOD1(Visibility,
                     int(const bool &));
};

/*
 * MOCK implementation of protocol_handler::SessionObserver interface
 */
class SessionObserverMock: public protocol_handler::SessionObserver {
 public:
#ifdef ENABLE_SECURITY
  MOCK_METHOD2(SetSSLContext,
               int (const uint32_t& key,
                    security_manager::SSLContext* context));
  MOCK_METHOD2(GetSSLContext,
               security_manager::SSLContext* (
                 const uint32_t& key,
                 const protocol_handler::ServiceType& service_type));
#endif  // ENABLE_SECURITY
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

#ifdef ENABLE_SECURITY
using test::components::security_manager_test::SSLContextMock;
/*
 * MOCK implementation of security_manager::SecurityManager
 */
class SecurityManagerMock: public security_manager::SecurityManager {
 public:
  MOCK_METHOD1(AddListener,
               void(security_manager::SecurityManagerListener *));
  MOCK_METHOD1(CreateSSLContext,
               security_manager::SSLContext*(const uint32_t &));
  MOCK_METHOD1(StartHandshake,
               void(uint32_t));
  MOCK_METHOD4(SendInternalError,
               void(const uint32_t ,
                    const uint8_t&,
                    const std::string&,
                    const uint32_t ));
};
#endif  // ENABLE_SECURITY
}  // namespace test
}  // namespace components
}  // namespace protocol_handler_test
#endif  // TEST_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_MOCK_H_

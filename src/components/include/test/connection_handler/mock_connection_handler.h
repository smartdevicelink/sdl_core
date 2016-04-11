/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_TEST_CONNECTION_HANDLER_MOCK_CONNECTION_HANDLER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_CONNECTION_HANDLER_MOCK_CONNECTION_HANDLER_H_

#include <string>
#include <list>
#include <vector>

#include "gmock/gmock.h"
#include "connection_handler/connection_handler.h"
#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/devices_discovery_starter.h"
#include "connection_handler/connection_handler_settings.h"

namespace test {
namespace components {
namespace connection_handler_test {

using connection_handler::ConnectionHandle;
using connection_handler::DeviceHandle;
using connection_handler::CloseSessionReason;
using connection_handler::DevicesDiscoveryStarter;


class MockConnectionHandler : public connection_handler::ConnectionHandler {
 public:
  MOCK_METHOD1(set_connection_handler_observer,
      void(connection_handler::ConnectionHandlerObserver* observer));
  MOCK_METHOD0(StartTransportManager,
      void());
  MOCK_METHOD1(ConnectToDevice,
      void(connection_handler::DeviceHandle device_handle));
  MOCK_METHOD0(ConnectToAllDevices,
      void());
  MOCK_METHOD1(CloseRevokedConnection,
      void(uint32_t connection_key));
  MOCK_METHOD1(CloseConnection,
      void(ConnectionHandle connection_handle));
  MOCK_METHOD1(GetConnectionSessionsCount,
      uint32_t(uint32_t connection_key));
  MOCK_METHOD2(GetDeviceID,
      bool(const std::string& mac_address, DeviceHandle* device_handle));
  MOCK_METHOD2(CloseSession,
      void(uint32_t key, CloseSessionReason close_reason));
  MOCK_METHOD3(CloseSession,
      void(ConnectionHandle connection_handle,
           uint8_t session_id,
           CloseSessionReason close_reason));
  MOCK_METHOD2(SendEndService,
      void(uint32_t key, uint8_t service_type));
  MOCK_METHOD1(StartSessionHeartBeat,
      void(uint32_t connection_key));
  MOCK_METHOD2(SendHeartBeat,
      void(ConnectionHandle connection_handle, uint8_t session_id));
  MOCK_METHOD2(SetHeartBeatTimeout,
      void(uint32_t connection_key, uint32_t timeout));
  MOCK_METHOD2(KeepConnectionAlive,
      void(uint32_t connection_key, uint8_t session_id));
  MOCK_METHOD2(BindProtocolVersionWithSession,
      void(uint32_t connection_key, uint8_t protocol_version));
  MOCK_CONST_METHOD4(GetDataOnSessionKey,
      int32_t(uint32_t key,
              uint32_t* app_id,
              std::list<int32_t>* sessions_list,
              uint32_t* device_id));
  MOCK_CONST_METHOD0(get_settings,
      const connection_handler::ConnectionHandlerSettings&());
  MOCK_METHOD0(get_session_observer,
      const protocol_handler::SessionObserver&());
  MOCK_METHOD0(get_device_discovery_starter,
      DevicesDiscoveryStarter&());
  MOCK_CONST_METHOD1(GetConnectedDevicesMAC,
      void(std::vector<std::string>& macs));
};


}  // namespace connection_handler_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_CONNECTION_HANDLER_MOCK_CONNECTION_HANDLER_H_

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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_LISTENER_MOCK_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_LISTENER_MOCK_H_

#include <vector>
#include <gmock/gmock.h>
#include "transport_manager/transport_manager_listener.h"
#include "protocol/raw_message.h"

namespace test {
namespace components {
namespace transport_manager_test {

using namespace transport_manager;
using ::protocol_handler::RawMessage;
using ::protocol_handler::RawMessagePtr;

class TransportManagerListenerMock : public TransportManagerListener {
 public:
  MOCK_METHOD1(OnDeviceListUpdated,
      void(const std::vector<DeviceInfo>& device_info));
  MOCK_METHOD0(OnFindNewApplicationsRequest,
      void());
  MOCK_METHOD1(OnDeviceFound,
      void(const DeviceInfo& device_info));
  MOCK_METHOD1(OnDeviceAdded,
      void(const DeviceInfo& device_info));
  MOCK_METHOD1(OnDeviceRemoved,
      void(const DeviceInfo& device_info));
  MOCK_METHOD0(OnScanDevicesFinished,
      void());
  MOCK_METHOD1(OnScanDevicesFailed,
      void(const SearchDeviceError& error));
  MOCK_METHOD2(OnConnectionEstablished,
      void(const DeviceInfo& device_info, const ConnectionUID connection_id));
  MOCK_METHOD2(OnConnectionFailed,
      void(const DeviceInfo& device_info, const ConnectError& error));
  MOCK_METHOD1(OnConnectionClosed,
      void(const ConnectionUID connection_id));
  MOCK_METHOD2(OnUnexpectedDisconnect,
      void(const ConnectionUID connection_id, const CommunicationError& error));
  MOCK_METHOD2(OnConnectionClosedFailure,
      void(const ConnectionUID connection_id, const DisconnectError& error));
  MOCK_METHOD2(OnDeviceConnectionLost,
      void(const DeviceHandle& device, const DisconnectDeviceError& error));
  MOCK_METHOD2(OnDisconnectFailed,
      void(const DeviceHandle& device, const DisconnectDeviceError& error));
  MOCK_METHOD1(OnTMMessageReceived,
      void(const ::protocol_handler::RawMessagePtr message));
  MOCK_METHOD1(OnTMMessageReceiveFailed,
      void(const DataReceiveError& error));
  MOCK_METHOD1(OnTMMessageSend,
      void(const ::protocol_handler::RawMessagePtr message));
  MOCK_METHOD2(OnTMMessageSendFailed,
      void(const DataSendError& error, const ::protocol_handler::RawMessagePtr message));
};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_MANAGER_LISTENER_MOCK_H_

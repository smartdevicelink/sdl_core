/*
 * \file mock_transport_adapter_listener.h
 * \brief 
 *
 * Copyright (c) 2013, Ford Motor Company
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

#ifndef MOCK_TRANSPORT_MANAGER_LISTENER_H
#define MOCK_TRANSPORT_MANAGER_LISTENER_H

#include <gmock/gmock.h>

#include "transport_manager/common.h"
#include "transport_manager/info.h"
#include "transport_manager/transport_adapter/transport_adapter.h"
#include "transport_manager/transport_manager_listener.h"

using ::transport_manager::ApplicationList;
using ::transport_manager::ApplicationHandle;
using ::transport_manager::transport_adapter::TransportAdapter;
using ::transport_manager::SearchDeviceError;
using ::transport_manager::ConnectionUID;
using ::transport_manager::ConnectError;
using ::transport_manager::DisconnectError;
using ::transport_manager::DisconnectDeviceError;
using ::transport_manager::DataSendError;
using ::transport_manager::DataReceiveError;
using ::transport_manager::CommunicationError;
using ::transport_manager::DeviceInfo;
using ::transport_manager::DeviceHandle;

namespace test {
namespace components {
namespace transport_manager {

class MockTransportManagerListener :
    public ::transport_manager::TransportManagerListener {
 public:
  MOCK_METHOD1(OnDeviceListUpdated, void(const std::vector<DeviceInfo>&));
  MOCK_METHOD0(OnFindNewApplicationsRequest, void());
  MOCK_METHOD1(OnDeviceFound, void(const DeviceInfo &device_info));
  MOCK_METHOD1(OnDeviceAdded, void(const DeviceInfo &device_info));
  MOCK_METHOD1(OnDeviceRemoved, void(const DeviceInfo &device_info));
  MOCK_METHOD0(OnNoDeviceFound, void());
  MOCK_METHOD0(OnScanDevicesFinished, void());
  MOCK_METHOD1(OnScanDevicesFailed, void(const SearchDeviceError& error));

  MOCK_METHOD2(OnConnectionEstablished, void(const DeviceInfo& device_info,
          const ConnectionUID &connection_id));
  MOCK_METHOD2(OnConnectionFailed, void(const DeviceInfo& device_info,
          const ConnectError& error));

  MOCK_METHOD1(OnConnectionClosed, void(ConnectionUID connection_id));
  MOCK_METHOD2(OnConnectionClosedFailure, void (ConnectionUID connection_id,
          const DisconnectError& error));
  MOCK_METHOD2(OnUnexpectedDisconnect, void (ConnectionUID connection_id,
          const CommunicationError& error));
  MOCK_METHOD2(OnDeviceConnectionLost, void (const DeviceHandle& device,
          const DisconnectDeviceError& error));
  MOCK_METHOD2(OnDisconnectFailed, void (const DeviceHandle& device,
          const DisconnectDeviceError& error));

  MOCK_METHOD1(OnTMMessageReceived, void(const RawMessagePtr data_container));
  MOCK_METHOD2(OnTMMessageReceiveFailed, void(ConnectionUID connection_id,
          const DataReceiveError& error));
  MOCK_METHOD1(OnTMMessageSend, void(const RawMessagePtr message));
  MOCK_METHOD2(OnTMMessageSendFailed, void(const DataSendError& error,
          const RawMessagePtr message));
};

}  // namespace transport_manager
}  // namespace components
}  // namespace test

#endif /* MOCK_TRANSPORT_MANAGER_LISTENER_H */

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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_ADAPTER_LISTENER_MOCK_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_ADAPTER_LISTENER_MOCK_H_

#include "gmock/gmock.h"
#include "transport_manager/transport_adapter/transport_adapter_listener.h"

namespace test {
namespace components {
namespace transport_manager_test {

namespace tm = transport_manager;
namespace ta = tm::transport_adapter;
using ta::TransportAdapter;
using ta::TransportAdapterListener;
using tm::SearchDeviceError;
using tm::DeviceUID;
using tm::ApplicationHandle;
using tm::ConnectError;
using tm::CommunicationError;
using tm::DisconnectError;
using tm::DisconnectDeviceError;
using tm::DataSendError;
using tm::DataReceiveError;

class MockTransportAdapterListener : public TransportAdapterListener {
 public:
  MOCK_METHOD1(OnSearchDeviceDone,
               void(const TransportAdapter* transport_adapter));
  MOCK_METHOD2(OnSearchDeviceFailed,
               void(const TransportAdapter* transport_adapter,
                    const SearchDeviceError& error));
  MOCK_METHOD1(OnDeviceListUpdated,
               void(const TransportAdapter* transport_adapter));
  MOCK_METHOD1(OnFindNewApplicationsRequest,
               void(const TransportAdapter* transport_adapter));
  MOCK_METHOD3(OnConnectDone,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle));
  MOCK_METHOD4(OnConnectFailed,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ConnectError& error));
  MOCK_METHOD3(OnConnectRequested,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle));
  MOCK_METHOD4(OnUnexpectedDisconnect,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const CommunicationError& error));
  MOCK_METHOD3(OnDisconnectDone,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle));
  MOCK_METHOD4(OnDisconnectFailed,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const DisconnectError& error));
  MOCK_METHOD2(OnDisconnectDeviceDone,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle));
  MOCK_METHOD3(OnDisconnectDeviceFailed,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const DisconnectDeviceError& error));
  MOCK_METHOD4(OnDataSendDone,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ::protocol_handler::RawMessagePtr data_container));
  MOCK_METHOD5(OnDataSendFailed,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ::protocol_handler::RawMessagePtr data_container,
                    const DataSendError& error));
  MOCK_METHOD4(OnDataReceiveDone,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ::protocol_handler::RawMessagePtr data_container));
  MOCK_METHOD4(OnDataReceiveFailed,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const DataReceiveError& error));
  MOCK_METHOD3(OnCommunicationError,
               void(const TransportAdapter* transport_adapter,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle));
};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_ADAPTER_LISTENER_MOCK_H_

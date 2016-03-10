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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_ADAPTER_MOCK_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_ADAPTER_MOCK_H_

#include "gmock/gmock.h"
#include "transport_manager/transport_adapter/transport_adapter.h"

namespace test {
namespace components {
namespace transport_manager_test {

class TransportAdapterMock
    : public ::transport_manager::transport_adapter::TransportAdapter {
 public:
  MOCK_CONST_METHOD0(GetDeviceType,
                     ::transport_manager::transport_adapter::DeviceType());
  MOCK_CONST_METHOD0(GetConnectionType, ::transport_manager::ConnectionType());
  MOCK_CONST_METHOD0(IsInitialised, bool());
  MOCK_METHOD0(
      Init, ::transport_manager::transport_adapter::TransportAdapter::Error());
  MOCK_METHOD0(Terminate, void());
  MOCK_METHOD1(
      AddListener,
      void(::transport_manager::transport_adapter::TransportAdapterListener*
               listener));
  MOCK_CONST_METHOD0(IsSearchDevicesSupported, bool());
  MOCK_METHOD0(
      SearchDevices,
      ::transport_manager::transport_adapter::TransportAdapter::Error());
  MOCK_CONST_METHOD0(IsServerOriginatedConnectSupported, bool());
  MOCK_METHOD2(Connect,
               ::transport_manager::transport_adapter::TransportAdapter::Error(
                   const ::transport_manager::DeviceUID& device_handle,
                   const ::transport_manager::ApplicationHandle& app_handle));
  MOCK_METHOD1(ConnectDevice,
               ::transport_manager::transport_adapter::TransportAdapter::Error(
                   const ::transport_manager::DeviceUID& device_handle));
  MOCK_CONST_METHOD0(IsClientOriginatedConnectSupported, bool());
  MOCK_METHOD0(
      StartClientListening,
      ::transport_manager::transport_adapter::TransportAdapter::Error());
  MOCK_METHOD0(
      StopClientListening,
      ::transport_manager::transport_adapter::TransportAdapter::Error());
  MOCK_METHOD2(Disconnect,
               ::transport_manager::transport_adapter::TransportAdapter::Error(
                   const ::transport_manager::DeviceUID& device_handle,
                   const ::transport_manager::ApplicationHandle& app_handle));
  MOCK_METHOD1(DisconnectDevice,
               Error(const ::transport_manager::DeviceUID& device_handle));
  MOCK_METHOD3(SendData,
               ::transport_manager::transport_adapter::TransportAdapter::Error(
                   const ::transport_manager::DeviceUID& device_handle,
                   const ::transport_manager::ApplicationHandle& app_handle,
                   const protocol_handler::RawMessagePtr data));
  MOCK_CONST_METHOD0(GetDeviceList, ::transport_manager::DeviceList());
  MOCK_CONST_METHOD1(GetApplicationList,
                     ::transport_manager::ApplicationList(
                         const ::transport_manager::DeviceUID& device_handle));
  MOCK_CONST_METHOD1(
      DeviceName,
      std::string(const ::transport_manager::DeviceUID& device_handle));

#ifdef TELEMETRY_MONITOR
  MOCK_METHOD0(GetTelemetryObserver, ::transport_manager::TMTelemetryObserver*());
#endif  // TELEMETRY_MONITOR
};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_ADAPTER_MOCK_H_

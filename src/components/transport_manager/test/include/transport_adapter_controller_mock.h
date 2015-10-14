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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_ADAPTER_CONTROLLER_MOCK_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_ADAPTER_CONTROLLER_MOCK_H_

#include "gmock/gmock.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"

namespace test {
namespace components {
namespace transport_manager {

using namespace ::transport_manager::transport_adapter;

class TransportAdapterControllerMock : public TransportAdapterController {
 public:
  MOCK_METHOD1(AddDevice, DeviceSptr(DeviceSptr device));
  MOCK_METHOD1(SearchDeviceDone, void(DeviceVector device));
  MOCK_METHOD1(ApplicationListUpdated,
               ApplicationListUpdated(const DeviceUID& device_handle));
  MOCK_METHOD0(FindNewApplicationsRequest, void());
  MOCK_METHOD1(SearchDeviceFailed, void(const SearchDeviceError& error));
  MOCK_CONST_METHOD1(FindDevice, DeviceSptr(const DeviceUID& device_handle));
  MOCK_CONST_METHOD3(FindDevice, void(ConnectionSPtr connection,
                                      const DeviceUID& device_handle,
                                      const ApplicationHandle& app_handle));
  MOCK_METHOD2(ConnectDone, void(const DeviceUID& device_handle,
                                 const ApplicationHandle& app_handle));
  MOCK_METHOD3(ConnectFailed, void(const DeviceUID& device_handle,
                                   const ApplicationHandle& app_handle,
                                   const ConnectError& error));
  MOCK_METHOD2(ConnectionFinished, void(const DeviceUID& device_handle,
                                        const ApplicationHandle& app_handle));
  MOCK_METHOD3(ConnectionAborted, void(const DeviceUID& device_handle,
                                       const ApplicationHandle& app_handle,
                                       const CommunicationError& error));
  MOCK_METHOD2(DeviceDisconnected, void(const DeviceUID& device_handle,
                                        const DisconnectDeviceError& error));
  MOCK_METHOD2(DisconnectDone, void(const DeviceUID& device_handle,
                                    const ApplicationHandle& app_handle));
  MOCK_METHOD3(DataReceiveDone,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    ::protocol_handler::RawMessagePtr message));
  MOCK_METHOD3(DataReceiveFailed, void(const DeviceUID& device_handle,
                                       const ApplicationHandle& app_handle,
                                       const DataReceiveError& error));
  MOCK_METHOD3(DataSendDone, void(const DeviceUID& device_handle,
                                  const ApplicationHandle& app_handle,
                                  ::protocol_handler::RawMessagePtr message));
  MOCK_METHOD3(DataReceiveFailed,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    ::protocol_handler::RawMessagePtr message,
                    const DataSendError& error));
};

}  // namespace transport_manager
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_TEST_INCLUDE_TRANSPORT_ADAPTER_CONTROLLER_MOCK_H_

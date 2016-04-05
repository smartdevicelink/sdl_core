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

#include "gtest/gtest.h"
#include "include/transport_adapter_mock.h"
#include "transport_manager/tcp/tcp_client_listener.h"
#include "transport_manager/mock_transport_manager.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/transport_adapter/device.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::Return;
using namespace ::transport_manager;
using namespace ::transport_manager::transport_adapter;

class MockTransportAdapterController: public TransportAdapterController {
public:
 MOCK_METHOD1(AddDevice,DeviceSptr(DeviceSptr device));
 MOCK_METHOD0(AckDevices,void());
 MOCK_METHOD1(SearchDeviceDone, void(const DeviceVector& devices));
 MOCK_METHOD1(SearchDeviceFailed, void(const SearchDeviceError& error));
 MOCK_CONST_METHOD1(FindDevice, DeviceSptr(const DeviceUID& device_handle));
 MOCK_METHOD3(ConnectionCreated, void(ConnectionSPtr connection, const DeviceUID& device_handle, const ApplicationHandle& app_handle));
 MOCK_METHOD2(ConnectDone, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
 MOCK_METHOD3(ConnectFailed, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ConnectError& error));
 MOCK_METHOD2(ConnectionFinished, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
 MOCK_METHOD3(ConnectionAborted,void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const CommunicationError& error));
 MOCK_METHOD2(DisconnectDone, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle));
 MOCK_METHOD3(DataReceiveDone, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ::protocol_handler::RawMessagePtr message));
 MOCK_METHOD3(DataReceiveFailed, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const DataReceiveError& error));
 MOCK_METHOD3(DataSendDone, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ::protocol_handler::RawMessagePtr message));
 MOCK_METHOD4(DataSendFailed, void(const DeviceUID& device_handle, const ApplicationHandle& app_handle, const ::protocol_handler::RawMessagePtr message, const DataSendError& error));
 MOCK_METHOD0(FindNewApplicationsRequest, void());
 MOCK_METHOD1(ApplicationListUpdated, void(const DeviceUID& device_handle));
 MOCK_METHOD2(DeviceDisconnected, void (const DeviceUID& device_handle,const DisconnectDeviceError& error));
};

class TcpClientListenerTest : public ::testing::Test {
 public:
  TcpClientListenerTest()
      : port_(0),
        enable_keep_alive_(false),
        tcp_client_listener_(&adapter_controller_mock_, port_, enable_keep_alive_) {}

 protected:
  uint16_t port_;
  bool enable_keep_alive_;
  MockTransportAdapterController adapter_controller_mock_;
  TcpClientListener tcp_client_listener_;
};

TEST_F(TcpClientListenerTest, Ctor_test) {
  EXPECT_EQ(0, tcp_client_listener_.port());
  EXPECT_TRUE(NULL != tcp_client_listener_.thread());
  EXPECT_EQ(-1, tcp_client_listener_.get_socket());
}

TEST_F(TcpClientListenerTest, IsInitialised) {
 EXPECT_TRUE(tcp_client_listener_.IsInitialised());
}

TEST_F(TcpClientListenerTest, Init) {
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_.Init());
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test


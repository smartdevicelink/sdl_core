/*
 * Copyright (c) 2015, Ford Motor Company
 * All rights reserved.
 *
 * Copyright (c) 2018 Xevo Inc.
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
 * Neither the name of the copyright holders nor the names of its contributors
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

#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <time.h>

#include "gtest/gtest.h"
#include "transport_manager/transport_adapter/mock_transport_adapter.h"
#include "transport_manager/tcp/tcp_client_listener.h"
#include "transport_manager/tcp/network_interface_listener.h"
#include "transport_manager/mock_transport_manager.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/transport_adapter/mock_device.h"
#include "utils/make_shared.h"
#include "utils/test_async_waiter.h"
#include "utils/threads/thread.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using namespace ::transport_manager;
using namespace ::transport_manager::transport_adapter;

namespace {
const long kThreadStartWaitMsec = 10;
const uint32_t kConnectionCreatedTimeoutMsec = 200;
}

class MockTransportAdapterController : public TransportAdapterController {
 public:
  MOCK_METHOD1(AddDevice, DeviceSptr(DeviceSptr device));
  MOCK_METHOD0(AckDevices, void());
  MOCK_METHOD1(SearchDeviceDone, void(const DeviceVector& devices));
  MOCK_METHOD1(SearchDeviceFailed, void(const SearchDeviceError& error));
  MOCK_CONST_METHOD1(FindDevice, DeviceSptr(const DeviceUID& device_handle));
  MOCK_METHOD3(ConnectionCreated,
               void(ConnectionSPtr connection,
                    const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle));
  MOCK_METHOD2(ConnectDone,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle));
  MOCK_METHOD3(ConnectFailed,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ConnectError& error));
  MOCK_METHOD2(ConnectionFinished,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle));
  MOCK_METHOD3(ConnectionAborted,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const CommunicationError& error));
  MOCK_METHOD2(DisconnectDone,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle));
  MOCK_METHOD3(DataReceiveDone,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ::protocol_handler::RawMessagePtr message));
  MOCK_METHOD3(DataReceiveFailed,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const DataReceiveError& error));
  MOCK_METHOD3(DataSendDone,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ::protocol_handler::RawMessagePtr message));
  MOCK_METHOD4(DataSendFailed,
               void(const DeviceUID& device_handle,
                    const ApplicationHandle& app_handle,
                    const ::protocol_handler::RawMessagePtr message,
                    const DataSendError& error));
  MOCK_METHOD0(FindNewApplicationsRequest, void());
  MOCK_METHOD1(ApplicationListUpdated, void(const DeviceUID& device_handle));
  MOCK_METHOD2(DeviceDisconnected,
               void(const DeviceUID& device_handle,
                    const DisconnectDeviceError& error));
  MOCK_METHOD1(TransportConfigUpdated,
               void(const transport_manager::transport_adapter::TransportConfig&
                        new_config));
};

class MockNetworkInterfaceListener : public NetworkInterfaceListener {
 public:
  MOCK_METHOD0(Init, bool());
  MOCK_METHOD0(Deinit, void());
  MOCK_METHOD0(Start, bool());
  MOCK_METHOD0(Stop, bool());
};

class TcpClientListenerTest : public ::testing::TestWithParam<std::string> {
 public:
  TcpClientListenerTest()
      : port_(0) /* On Linux, binding to port 0 lets the system choose an
                    available port */
      , enable_keep_alive_(false)
      , interface_listener_mock_(NULL)
      , tcp_client_listener_(NULL) {}
  virtual ~TcpClientListenerTest() {
    delete tcp_client_listener_;
  }

 protected:
  void SetUp() OVERRIDE {
    tcp_client_listener_ = new TcpClientListener(
        &adapter_controller_mock_, port_, enable_keep_alive_, GetParam());
    interface_listener_mock_ = new MockNetworkInterfaceListener();
    tcp_client_listener_->set_testing(true);
    tcp_client_listener_->set_network_interface_listener(
        interface_listener_mock_);
  }

  bool InterfaceNameSpecified() const {
    return "" != GetParam();
  }

  void SleepFor(long msec) const {
    if (msec > 0) {
      struct timespec ts = {0, msec * 1000 * 1000};
      nanosleep(&ts, NULL);
    }
  }

  uint16_t port_;
  bool enable_keep_alive_;
  MockTransportAdapterController adapter_controller_mock_;
  MockNetworkInterfaceListener* interface_listener_mock_;
  TcpClientListener* tcp_client_listener_;
};

TEST_P(TcpClientListenerTest, Ctor_test) {
  EXPECT_EQ(0, tcp_client_listener_->port());
  EXPECT_TRUE(NULL != tcp_client_listener_->thread());
  EXPECT_EQ(-1, tcp_client_listener_->get_socket());
}

TEST_P(TcpClientListenerTest, IsInitialised) {
  // should return false until Init() is called
  EXPECT_FALSE(tcp_client_listener_->IsInitialised());
}

TEST_P(TcpClientListenerTest, Init) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));

  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());

  if (InterfaceNameSpecified()) {
    // TcpClientListener will create socket once IP address of the network is
    // notified.
    EXPECT_EQ(-1, tcp_client_listener_->get_socket());
  } else {
    // Interface name is not designated. In this case, TcpClientListener will
    // create socket with Init().
    EXPECT_TRUE(0 <= tcp_client_listener_->get_socket());
  }

  EXPECT_TRUE(tcp_client_listener_->IsInitialised());

  // Deinit() will be called during destructor
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, Terminate) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());

  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);

  tcp_client_listener_->Terminate();

  EXPECT_EQ(-1, tcp_client_listener_->get_socket());
}

TEST_P(TcpClientListenerTest, StartListening) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());

  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));

  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  // the "isThreadRunning_" flag of the thread will be update slightly later
  SleepFor(kThreadStartWaitMsec);

  if (InterfaceNameSpecified()) {
    EXPECT_FALSE(tcp_client_listener_->thread()->is_running());
  } else {
    EXPECT_TRUE(tcp_client_listener_->thread()->is_running());
  }

  // Stop() and Deinit() will be called during destructor
  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, StartListening_twice) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  // call again
  EXPECT_EQ(TransportAdapter::BAD_STATE,
            tcp_client_listener_->StartListening());

  // Stop() and Deinit() will be called during destructor
  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, StopListening) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));

  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StopListening());
  EXPECT_FALSE(tcp_client_listener_->thread()->is_running());

  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, StopListening_twice) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());
  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StopListening());

  // call again
  EXPECT_EQ(TransportAdapter::BAD_STATE, tcp_client_listener_->StopListening());

  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, ClientConnection) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  if (InterfaceNameSpecified()) {
    // set up a server socket by notifying a dummy IP address
    EXPECT_CALL(adapter_controller_mock_, TransportConfigUpdated(_)).Times(1);
    tcp_client_listener_->OnIPAddressUpdated(std::string("192.168.1.1"),
                                             std::string(""));
  }

  // get the port number (assigned by system) that the socket is listening on
  struct sockaddr_in server_addr;
  socklen_t server_addr_len = sizeof(server_addr);
  EXPECT_EQ(0,
            getsockname(tcp_client_listener_->get_socket(),
                        reinterpret_cast<struct sockaddr*>(&server_addr),
                        &server_addr_len));

  // try connecting to the socket
  struct sockaddr_in client_addr;
  client_addr.sin_family = AF_INET;
  client_addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
  client_addr.sin_port = server_addr.sin_port;
  socklen_t client_addr_len = sizeof(client_addr);

  int s = socket(AF_INET, SOCK_STREAM, 0);
  EXPECT_TRUE(0 <= s);

  TestAsyncWaiter waiter;

  // controller should be notified of AddDevice event
  DeviceSptr mock_device = utils::MakeShared<MockTCPDevice>(
      htonl(INADDR_LOOPBACK), "dummy_tcp_device");
  EXPECT_CALL(adapter_controller_mock_, AddDevice(_))
      .WillOnce(Return(mock_device));
  EXPECT_CALL(adapter_controller_mock_, ConnectionCreated(_, _, _))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));

  // adapter_controller_mock_ may also receive ConnectDone() and
  // ConnectionFinished() from ThreadedSocketConnection. Ignore them as hey are
  // not part ly client listener's tests.
  EXPECT_CALL(adapter_controller_mock_, ConnectDone(_, _)).Times(AtLeast(0));
  EXPECT_CALL(adapter_controller_mock_, ConnectionFinished(_, _))
      .Times(AtLeast(0));

  EXPECT_EQ(0,
            connect(s,
                    reinterpret_cast<struct sockaddr*>(&client_addr),
                    client_addr_len));

  // since the connection is handled on another thread, wait for some time
  EXPECT_TRUE(waiter.WaitFor(1, kConnectionCreatedTimeoutMsec));

  close(s);

  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, OnIPAddressUpdated_ValidIPv4Address) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  const std::string test_ipv4_addr = "192.168.1.1";
  const std::string test_ipv6_addr = "";
  char buf[16];
  snprintf(buf, sizeof(buf), "%u", port_);
  const std::string test_port(buf);

  TransportConfig expected_config;
  expected_config.insert(std::make_pair(std::string(TC_ENABLED), "true"));
  expected_config.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr));
  expected_config.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  EXPECT_CALL(adapter_controller_mock_, TransportConfigUpdated(expected_config))
      .Times(1);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr, test_ipv6_addr);

  if (InterfaceNameSpecified()) {
    // when the client listener runs with designated interface name, it should
    // start the thread here
    EXPECT_TRUE(0 <= tcp_client_listener_->get_socket());

    SleepFor(kThreadStartWaitMsec);

    EXPECT_TRUE(tcp_client_listener_->thread()->is_running());
  }

  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, OnIPAddressUpdated_IPv4Address_changed) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  const std::string test_ipv4_addr_1 = "192.168.1.1";
  const std::string test_ipv6_addr = "";
  char buf[16];
  snprintf(buf, sizeof(buf), "%u", port_);
  const std::string test_port(buf);

  TransportConfig expected_config_1;
  expected_config_1.insert(std::make_pair(std::string(TC_ENABLED), "true"));
  expected_config_1.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr_1));
  expected_config_1.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  EXPECT_CALL(adapter_controller_mock_,
              TransportConfigUpdated(expected_config_1)).Times(1);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr_1, test_ipv6_addr);

  const std::string test_ipv4_addr_2 = "172.16.2.3";
  TransportConfig expected_config_2;
  expected_config_2.insert(std::make_pair(std::string(TC_ENABLED), "true"));
  expected_config_2.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr_2));
  expected_config_2.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  EXPECT_CALL(adapter_controller_mock_,
              TransportConfigUpdated(expected_config_2)).Times(1);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr_2, test_ipv6_addr);

  if (InterfaceNameSpecified()) {
    EXPECT_TRUE(0 <= tcp_client_listener_->get_socket());

    SleepFor(kThreadStartWaitMsec);

    EXPECT_TRUE(tcp_client_listener_->thread()->is_running());
  }

  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, OnIPAddressUpdated_IPv4Address_same) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  const std::string test_ipv4_addr_1 = "192.168.1.1";
  const std::string test_ipv6_addr = "";
  char buf[16];
  snprintf(buf, sizeof(buf), "%u", port_);
  const std::string test_port(buf);

  TransportConfig expected_config_1;
  expected_config_1.insert(std::make_pair(std::string(TC_ENABLED), "true"));
  expected_config_1.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr_1));
  expected_config_1.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  EXPECT_CALL(adapter_controller_mock_,
              TransportConfigUpdated(expected_config_1)).Times(1);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr_1, test_ipv6_addr);

  const std::string test_ipv4_addr_2 = "192.168.1.1";  // same as before
  TransportConfig expected_config_2;
  expected_config_2.insert(std::make_pair(std::string(TC_ENABLED), "true"));
  expected_config_2.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr_2));
  expected_config_2.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  // client listener should not generate TransportConfigUpdated event
  EXPECT_CALL(adapter_controller_mock_, TransportConfigUpdated(_)).Times(0);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr_2, test_ipv6_addr);

  if (InterfaceNameSpecified()) {
    EXPECT_TRUE(0 <= tcp_client_listener_->get_socket());

    SleepFor(kThreadStartWaitMsec);

    EXPECT_TRUE(tcp_client_listener_->thread()->is_running());
  }

  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, OnIPAddressUpdated_IPv4Address_disabled) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  const std::string test_ipv4_addr_1 = "192.168.1.1";
  const std::string test_ipv6_addr = "";
  char buf[16];
  snprintf(buf, sizeof(buf), "%u", port_);
  const std::string test_port(buf);

  TransportConfig expected_config_1;
  expected_config_1.insert(std::make_pair(std::string(TC_ENABLED), "true"));
  expected_config_1.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr_1));
  expected_config_1.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  EXPECT_CALL(adapter_controller_mock_,
              TransportConfigUpdated(expected_config_1)).Times(1);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr_1, test_ipv6_addr);

  const std::string test_ipv4_addr_2 = "";
  TransportConfig expected_config_2;
  expected_config_2.insert(std::make_pair(std::string(TC_ENABLED), "false"));
  expected_config_2.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr_2));
  expected_config_2.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  EXPECT_CALL(adapter_controller_mock_,
              TransportConfigUpdated(expected_config_2)).Times(1);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr_2, test_ipv6_addr);

  if (InterfaceNameSpecified()) {
    EXPECT_EQ(-1, tcp_client_listener_->get_socket());

    SleepFor(kThreadStartWaitMsec);

    EXPECT_FALSE(tcp_client_listener_->thread()->is_running());
  }

  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, OnIPAddressUpdated_IPv4Address_reenabled) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  const std::string test_ipv4_addr_1 = "192.168.1.1";
  const std::string test_ipv6_addr = "";
  char buf[16];
  snprintf(buf, sizeof(buf), "%u", port_);
  const std::string test_port(buf);

  TransportConfig expected_config_1;
  expected_config_1.insert(std::make_pair(std::string(TC_ENABLED), "true"));
  expected_config_1.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr_1));
  expected_config_1.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  EXPECT_CALL(adapter_controller_mock_,
              TransportConfigUpdated(expected_config_1)).Times(1);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr_1, test_ipv6_addr);

  const std::string test_ipv4_addr_2 = "";
  TransportConfig expected_config_2;
  expected_config_2.insert(std::make_pair(std::string(TC_ENABLED), "false"));
  expected_config_2.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr_2));
  expected_config_2.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  EXPECT_CALL(adapter_controller_mock_,
              TransportConfigUpdated(expected_config_2)).Times(1);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr_2, test_ipv6_addr);

  const std::string test_ipv4_addr_3 = "192.168.1.1";
  TransportConfig expected_config_3;
  expected_config_3.insert(std::make_pair(std::string(TC_ENABLED), "true"));
  expected_config_3.insert(
      std::make_pair(std::string(TC_TCP_IP_ADDRESS), test_ipv4_addr_3));
  expected_config_3.insert(std::make_pair(std::string(TC_TCP_PORT), test_port));

  EXPECT_CALL(adapter_controller_mock_,
              TransportConfigUpdated(expected_config_3)).Times(1);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr_3, test_ipv6_addr);

  if (InterfaceNameSpecified()) {
    EXPECT_TRUE(0 <= tcp_client_listener_->get_socket());

    SleepFor(kThreadStartWaitMsec);

    EXPECT_TRUE(tcp_client_listener_->thread()->is_running());
  }

  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

TEST_P(TcpClientListenerTest, OnIPAddressUpdated_EmptyIPv4Address) {
  EXPECT_CALL(*interface_listener_mock_, Init()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->Init());
  EXPECT_CALL(*interface_listener_mock_, Start()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, tcp_client_listener_->StartListening());

  const std::string test_ipv4_addr = "";
  const std::string test_ipv6_addr = "";
  char buf[16];
  snprintf(buf, sizeof(buf), "%u", port_);
  const std::string test_port(buf);

  // if the client listener receives an empty IP address after started, it
  // should ignore it
  EXPECT_CALL(adapter_controller_mock_, TransportConfigUpdated(_)).Times(0);

  tcp_client_listener_->OnIPAddressUpdated(test_ipv4_addr, test_ipv6_addr);

  if (InterfaceNameSpecified()) {
    EXPECT_EQ(-1, tcp_client_listener_->get_socket());

    SleepFor(kThreadStartWaitMsec);

    EXPECT_FALSE(tcp_client_listener_->thread()->is_running());
  }

  EXPECT_CALL(*interface_listener_mock_, Stop()).WillOnce(Return(true));
  EXPECT_CALL(*interface_listener_mock_, Deinit()).Times(1);
}

INSTANTIATE_TEST_CASE_P(NetworkInterfaceName,
                        TcpClientListenerTest,
                        ::testing::Values(std::string(""),
                                          std::string("dummy_interface0")));

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

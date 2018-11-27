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
#include "transport_manager/transport_adapter/mock_device_scanner.h"
#include "transport_manager/transport_adapter/mock_client_connection_listener.h"
#include "transport_manager/transport_adapter/mock_server_connection_factory.h"
#include "transport_manager/transport_adapter/mock_device.h"
#include "transport_manager/transport_adapter/mock_connection.h"
#include "transport_manager/transport_adapter/mock_transport_adapter_listener.h"
#include "transport_manager/transport_adapter/mock_transport_adapter_impl.h"
#include "transport_manager/mock_transport_manager_settings.h"

#include "transport_manager/transport_adapter/transport_adapter_impl.h"
#include "transport_manager/transport_adapter/transport_adapter_listener.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/transport_adapter/connection.h"
#include "protocol/raw_message.h"

#include "resumption/last_state_impl.h"
#include "config_profile/profile.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::_;
using ::testing::NiceMock;
using namespace ::transport_manager;
using namespace ::protocol_handler;

class TransportAdapterTest : public ::testing::Test {
 protected:
  TransportAdapterTest()
      : last_state_("app_storage_folder", "app_info_storage") {}

  void SetUp() OVERRIDE {
    dev_id = "device_id";
    uniq_id = "unique_device_id";
    app_handle = 1;
    ON_CALL(transport_manager_settings, use_last_state())
        .WillByDefault(Return(true));
  }

  NiceMock<MockTransportManagerSettings> transport_manager_settings;
  resumption::LastStateImpl last_state_;
  std::string dev_id;
  std::string uniq_id;
  int app_handle;
};

TEST_F(TransportAdapterTest, Init) {
  MockDeviceScanner* dev_mock = new MockDeviceScanner();
  MockClientConnectionListener* clientMock = new MockClientConnectionListener();
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(dev_mock,
                                             serverMock,
                                             clientMock,
                                             last_state_,
                                             transport_manager_settings);
  EXPECT_CALL(*dev_mock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*clientMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, transport_adapter.Init());

  // Expect terminate because at the end of test transport_adapter will be
  // destroyed. That will call Terminate() for connections and device scanner.
  EXPECT_CALL(*dev_mock, Terminate());
  EXPECT_CALL(*clientMock, Terminate());
  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, SearchDevices_WithoutScanner) {
  MockClientConnectionListener* clientMock = new MockClientConnectionListener();
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, clientMock, last_state_, transport_manager_settings);

  EXPECT_CALL(*clientMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  EXPECT_EQ(TransportAdapter::OK, transport_adapter.Init());

  EXPECT_EQ(TransportAdapter::NOT_SUPPORTED, transport_adapter.SearchDevices());

  EXPECT_CALL(*clientMock, Terminate());
  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, SearchDevices_DeviceNotInitialized) {
  MockDeviceScanner* dev_mock = new MockDeviceScanner();
  MockTransportAdapterImpl transport_adapter(
      dev_mock, NULL, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*dev_mock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*dev_mock, IsInitialised()).WillRepeatedly(Return(false));
  EXPECT_CALL(*dev_mock, Scan()).Times(0);
  EXPECT_EQ(TransportAdapter::BAD_STATE, transport_adapter.SearchDevices());
  EXPECT_CALL(*dev_mock, Terminate());
}

TEST_F(TransportAdapterTest, SearchDevices_DeviceInitialized) {
  MockDeviceScanner* dev_mock = new MockDeviceScanner();
  MockTransportAdapterImpl transport_adapter(
      dev_mock, NULL, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*dev_mock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*dev_mock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*dev_mock, Scan()).WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_EQ(TransportAdapter::OK, transport_adapter.SearchDevices());

  EXPECT_CALL(*dev_mock, Terminate());
}

TEST_F(TransportAdapterTest, SearchDeviceDone_DeviceExisting) {
  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  auto mockdev = std::make_shared<MockDevice>(dev_id, uniq_id);
  transport_adapter.AddDevice(mockdev);

  std::vector<std::shared_ptr<Device> > devList;
  devList.push_back(mockdev);

  EXPECT_CALL(*mockdev, IsSameAs(_)).WillOnce(Return(true));
  transport_adapter.SearchDeviceDone(devList);
}

TEST_F(TransportAdapterTest, SearchDeviceFailed) {
  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  MockTransportAdapterListener mock_listener;
  transport_adapter.AddListener(&mock_listener);

  SearchDeviceError er;
  EXPECT_CALL(mock_listener, OnSearchDeviceFailed(_, _));
  transport_adapter.SearchDeviceFailed(er);
}

TEST_F(TransportAdapterTest, AddDevice) {
  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  MockTransportAdapterListener mock_listener;
  transport_adapter.AddListener(&mock_listener);

  std::shared_ptr<MockDevice> mockdev =
      std::make_shared<MockDevice>(dev_id, uniq_id);

  EXPECT_CALL(mock_listener, OnDeviceListUpdated(&transport_adapter));
  transport_adapter.AddDevice(mockdev);
}

TEST_F(TransportAdapterTest, Connect_ServerNotSupported) {
  MockClientConnectionListener* clientMock = new MockClientConnectionListener();
  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, clientMock, last_state_, transport_manager_settings);

  EXPECT_CALL(*clientMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  int app_handle = 1;

  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::NOT_SUPPORTED, res);

  EXPECT_CALL(*clientMock, Terminate());
}

TEST_F(TransportAdapterTest, Connect_ServerNotInitialized) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(false));
  EXPECT_CALL(*serverMock, CreateConnection(dev_id, app_handle)).Times(0);
  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::BAD_STATE, res);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, Connect_Success) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(dev_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::OK, res);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, Connect_DeviceAddedTwice) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(dev_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::OK, res);

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  TransportAdapter::Error newres =
      transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::ALREADY_EXISTS, newres);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, ConnectDevice_ServerNotAdded_DeviceAdded) {
  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  std::shared_ptr<MockDevice> mockdev =
      std::make_shared<MockDevice>(dev_id, uniq_id);
  transport_adapter.AddDevice(mockdev);

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);

  int app_handle = 1;
  std::vector<int> intList = {app_handle};
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));

  TransportAdapter::Error res = transport_adapter.ConnectDevice(uniq_id);
  EXPECT_EQ(TransportAdapter::FAIL, res);
}

TEST_F(TransportAdapterTest, ConnectDevice_DeviceNotAdded) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);
  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  std::string uniq_id = "unique_device_id";

  EXPECT_CALL(*serverMock, IsInitialised()).Times(0);
  EXPECT_CALL(*serverMock, CreateConnection(_, _)).Times(0);
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id))
      .WillOnce(Return(std::shared_ptr<MockDevice>()));
  TransportAdapter::Error res = transport_adapter.ConnectDevice(uniq_id);
  EXPECT_EQ(TransportAdapter::BAD_PARAM, res);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, ConnectDevice_DeviceAdded) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  std::shared_ptr<MockDevice> mockdev =
      std::make_shared<MockDevice>(dev_id, uniq_id);
  transport_adapter.AddDevice(mockdev);

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);

  int app_handle = 1;
  std::vector<int> intList = {app_handle};
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(uniq_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  TransportAdapter::Error res = transport_adapter.ConnectDevice(uniq_id);
  EXPECT_EQ(TransportAdapter::OK, res);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, ConnectDevice_DeviceAddedTwice) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  std::shared_ptr<MockDevice> mockdev =
      std::make_shared<MockDevice>(dev_id, uniq_id);
  transport_adapter.AddDevice(mockdev);

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);

  int app_handle = 1;
  std::vector<int> intList = {app_handle};
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(uniq_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  TransportAdapter::Error res = transport_adapter.ConnectDevice(uniq_id);
  EXPECT_EQ(TransportAdapter::OK, res);

  // Try to connect device second time

  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(uniq_id, app_handle)).Times(0);
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  TransportAdapter::Error newres = transport_adapter.ConnectDevice(uniq_id);
  EXPECT_EQ(TransportAdapter::OK, newres);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, Disconnect_ConnectDoneSuccess) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(dev_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::OK, res);

  auto mock_connection = std::make_shared<MockConnection>();
  transport_adapter.ConnectionCreated(mock_connection, dev_id, app_handle);

  EXPECT_CALL(transport_adapter, Store());
  transport_adapter.ConnectDone(dev_id, app_handle);

  EXPECT_CALL(*mock_connection, Disconnect())
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error new_res =
      transport_adapter.Disconnect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::OK, new_res);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, DisconnectDevice_DeviceAddedConnectionCreated) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  std::shared_ptr<MockDevice> mockdev =
      std::make_shared<MockDevice>(dev_id, uniq_id);
  transport_adapter.AddDevice(mockdev);

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);

  std::vector<int> intList = {app_handle};
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(uniq_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id))
      .WillRepeatedly(Return(mockdev));
  TransportAdapter::Error res = transport_adapter.ConnectDevice(uniq_id);
  EXPECT_EQ(TransportAdapter::OK, res);

  auto mock_connection = std::make_shared<MockConnection>();
  transport_adapter.ConnectionCreated(mock_connection, uniq_id, app_handle);

  EXPECT_CALL(*mock_connection, Disconnect())
      .WillOnce(Return(TransportAdapter::OK));

  TransportAdapter::Error new_res = transport_adapter.DisconnectDevice(uniq_id);
  EXPECT_EQ(TransportAdapter::OK, new_res);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, DeviceDisconnected) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  MockTransportAdapterListener mock_listener;
  transport_adapter.AddListener(&mock_listener);

  std::shared_ptr<MockDevice> mockdev =
      std::make_shared<MockDevice>(dev_id, uniq_id);
  EXPECT_CALL(mock_listener, OnDeviceListUpdated(&transport_adapter));
  transport_adapter.AddDevice(mockdev);

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);

  std::vector<int> intList = {app_handle};
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(uniq_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error res = transport_adapter.ConnectDevice(uniq_id);
  EXPECT_EQ(TransportAdapter::OK, res);

  std::shared_ptr<MockConnection> mock_connection =
      std::make_shared<MockConnection>();
  transport_adapter.ConnectionCreated(mock_connection, uniq_id, app_handle);

  EXPECT_CALL(mock_listener, OnDeviceListUpdated(&transport_adapter));
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));
  EXPECT_CALL(
      mock_listener,
      OnUnexpectedDisconnect(&transport_adapter, uniq_id, app_handle, _));
  EXPECT_CALL(mock_listener,
              OnDisconnectDeviceDone(&transport_adapter, uniq_id));
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  DisconnectDeviceError error;
  transport_adapter.DeviceDisconnected(uniq_id, error);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, AbortedConnectSuccess) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(dev_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::OK, res);

  MockTransportAdapterListener mock_listener;
  transport_adapter.AddListener(&mock_listener);

  CommunicationError ce;
  EXPECT_CALL(mock_listener, OnUnexpectedDisconnect(_, dev_id, app_handle, _));
  transport_adapter.ConnectionAborted(dev_id, app_handle, ce);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, SendData) {
  MockDeviceScanner* dev_mock = new MockDeviceScanner();
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      dev_mock, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*dev_mock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(dev_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::OK, res);

  auto mock_connection = std::make_shared<MockConnection>();
  transport_adapter.ConnectionCreated(mock_connection, dev_id, app_handle);

  EXPECT_CALL(transport_adapter, Store());
  transport_adapter.ConnectDone(dev_id, app_handle);

  const unsigned int kSize = 3;
  unsigned char data[kSize] = {0x20, 0x07, 0x01};
  const RawMessagePtr kMessage =
      std::make_shared<RawMessage>(1, 1, data, kSize);

  EXPECT_CALL(*mock_connection, SendData(kMessage))
      .WillOnce(Return(TransportAdapter::OK));
  res = transport_adapter.SendData(dev_id, app_handle, kMessage);
  EXPECT_EQ(TransportAdapter::OK, res);

  EXPECT_CALL(*dev_mock, Terminate());
  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, SendData_ConnectionNotEstablished) {
  MockDeviceScanner* dev_mock = new MockDeviceScanner();
  MockClientConnectionListener* clientMock = new MockClientConnectionListener();
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(dev_mock,
                                             serverMock,
                                             clientMock,
                                             last_state_,
                                             transport_manager_settings);

  EXPECT_CALL(*dev_mock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*clientMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(dev_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::OK, res);

  auto mock_connection = std::make_shared<MockConnection>();
  transport_adapter.ConnectionCreated(mock_connection, dev_id, app_handle);

  const unsigned int kSize = 3;
  unsigned char data[kSize] = {0x20, 0x07, 0x01};
  const RawMessagePtr kMessage =
      std::make_shared<RawMessage>(1, 1, data, kSize);

  EXPECT_CALL(*mock_connection, SendData(kMessage)).Times(0);
  res = transport_adapter.SendData(dev_id, app_handle, kMessage);
  EXPECT_EQ(TransportAdapter::BAD_PARAM, res);

  EXPECT_CALL(*dev_mock, Terminate());
  EXPECT_CALL(*clientMock, Terminate());
  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, StartClientListening_ClientNotInitialized) {
  MockDeviceScanner* dev_mock = new MockDeviceScanner();
  MockClientConnectionListener* clientMock = new MockClientConnectionListener();
  MockTransportAdapterImpl transport_adapter(
      dev_mock, NULL, clientMock, last_state_, transport_manager_settings);

  EXPECT_CALL(*dev_mock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*clientMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*clientMock, IsInitialised()).WillOnce(Return(false));
  EXPECT_CALL(*clientMock, StartListening()).Times(0);

  TransportAdapter::Error res = transport_adapter.StartClientListening();
  EXPECT_EQ(TransportAdapter::BAD_STATE, res);

  EXPECT_CALL(*dev_mock, Terminate());
  EXPECT_CALL(*clientMock, Terminate());
}

TEST_F(TransportAdapterTest, StartClientListening) {
  MockDeviceScanner* dev_mock = new MockDeviceScanner();
  MockClientConnectionListener* clientMock = new MockClientConnectionListener();
  MockTransportAdapterImpl transport_adapter(
      dev_mock, NULL, clientMock, last_state_, transport_manager_settings);

  EXPECT_CALL(*dev_mock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*clientMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*clientMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*clientMock, StartListening())
      .WillOnce(Return(TransportAdapter::OK));

  TransportAdapter::Error res = transport_adapter.StartClientListening();
  EXPECT_EQ(TransportAdapter::OK, res);

  EXPECT_CALL(*dev_mock, Terminate());
  EXPECT_CALL(*clientMock, Terminate());
}

TEST_F(TransportAdapterTest, StopClientListening_Success) {
  MockDeviceScanner* dev_mock = new MockDeviceScanner();
  MockClientConnectionListener* clientMock = new MockClientConnectionListener();
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(dev_mock,
                                             serverMock,
                                             clientMock,
                                             last_state_,
                                             transport_manager_settings);

  EXPECT_CALL(*dev_mock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*clientMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(dev_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::OK, res);

  EXPECT_CALL(*clientMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*clientMock, StopListening())
      .WillOnce(Return(TransportAdapter::OK));

  res = transport_adapter.StopClientListening();
  EXPECT_EQ(TransportAdapter::OK, res);

  EXPECT_CALL(*dev_mock, Terminate());
  EXPECT_CALL(*clientMock, Terminate());
  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, FindNewApplicationsRequest) {
  MockDeviceScanner* dev_mock = new MockDeviceScanner();
  MockClientConnectionListener* clientMock = new MockClientConnectionListener();
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(dev_mock,
                                             serverMock,
                                             clientMock,
                                             last_state_,
                                             transport_manager_settings);

  EXPECT_CALL(*dev_mock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*clientMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  MockTransportAdapterListener mock_listener;
  transport_adapter.AddListener(&mock_listener);

  EXPECT_CALL(mock_listener, OnFindNewApplicationsRequest(&transport_adapter));
  transport_adapter.FindNewApplicationsRequest();

  EXPECT_CALL(*dev_mock, Terminate());
  EXPECT_CALL(*clientMock, Terminate());
  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, GetDeviceAndApplicationLists) {
  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  std::shared_ptr<MockDevice> mockdev =
      std::make_shared<MockDevice>(dev_id, uniq_id);
  transport_adapter.AddDevice(mockdev);

  std::vector<std::string> devList = transport_adapter.GetDeviceList();
  ASSERT_EQ(1u, devList.size());
  EXPECT_EQ(uniq_id, devList[0]);

  int app_handle = 1;
  std::vector<int> intList = {app_handle};
  EXPECT_CALL(*mockdev, GetApplicationList()).WillOnce(Return(intList));
  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  std::vector<int> res = transport_adapter.GetApplicationList(uniq_id);
  ASSERT_EQ(1u, res.size());
  EXPECT_EQ(intList[0], res[0]);
}

TEST_F(TransportAdapterTest, FindEstablishedConnection) {
  MockServerConnectionFactory* serverMock = new MockServerConnectionFactory();
  MockTransportAdapterImpl transport_adapter(
      NULL, serverMock, NULL, last_state_, transport_manager_settings);

  EXPECT_CALL(*serverMock, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  EXPECT_CALL(*serverMock, IsInitialised()).WillOnce(Return(true));
  EXPECT_CALL(*serverMock, CreateConnection(dev_id, app_handle))
      .WillOnce(Return(TransportAdapter::OK));
  TransportAdapter::Error res = transport_adapter.Connect(dev_id, app_handle);
  EXPECT_EQ(TransportAdapter::OK, res);

  ConnectionSPtr mock_connection = std::make_shared<MockConnection>();
  transport_adapter.ConnectionCreated(mock_connection, dev_id, app_handle);

  EXPECT_CALL(transport_adapter, Store());
  transport_adapter.ConnectDone(dev_id, app_handle);

  ConnectionSPtr conn =
      transport_adapter.FindStatedConnection(dev_id, app_handle);
  EXPECT_EQ(mock_connection, conn);

  EXPECT_CALL(*serverMock, Terminate());
}

TEST_F(TransportAdapterTest, RunAppOnDevice_NoDeviseWithAskedId_UNSUCCESS) {
  const std::string bundle_id = "test_bundle_id";

  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);

  std::shared_ptr<MockDevice> mock_device =
      std::make_shared<MockDevice>("test_device_name", "test_device_uid0");

  transport_adapter.AddDevice(mock_device);

  EXPECT_CALL(*mock_device, LaunchApp(bundle_id)).Times(0);
  EXPECT_CALL(transport_adapter, FindDevice("test_device_uid1"))
      .WillOnce(Return(std::shared_ptr<MockDevice>()));

  transport_adapter.RunAppOnDevice("test_device_uid1", bundle_id);
}

TEST_F(TransportAdapterTest, RunAppOnDevice_DeviseWithAskedIdWasFound_SUCCESS) {
  const std::string bundle_id = "test_bundle_id";
  const std::string device_uid = "test_device_uid";

  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);

  std::shared_ptr<MockDevice> mock_device =
      std::make_shared<MockDevice>("test_device_name", device_uid);

  transport_adapter.AddDevice(mock_device);

  EXPECT_CALL(*mock_device, LaunchApp(bundle_id));
  EXPECT_CALL(transport_adapter, FindDevice(device_uid))
      .WillOnce(Return(mock_device));

  transport_adapter.RunAppOnDevice(device_uid, bundle_id);
}

TEST_F(TransportAdapterTest, StopDevice) {
  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  auto mockdev = std::make_shared<MockDevice>(dev_id, uniq_id);
  transport_adapter.AddDevice(mockdev);

  EXPECT_CALL(transport_adapter, FindDevice(uniq_id)).WillOnce(Return(mockdev));
  EXPECT_CALL(*mockdev, Stop());

  transport_adapter.StopDevice(uniq_id);
}

TEST_F(TransportAdapterTest, TransportConfigUpdated) {
  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  MockTransportAdapterListener mock_listener;
  transport_adapter.AddListener(&mock_listener);

  TransportConfig config;
  config[tc_enabled] = std::string("true");
  config[tc_tcp_ip_address] = std::string("192.168.1.1");
  config[tc_tcp_port] = std::string("12345");

  EXPECT_CALL(mock_listener, OnTransportConfigUpdated(_));
  transport_adapter.TransportConfigUpdated(config);
}

TEST_F(TransportAdapterTest, GetTransportConfigration) {
  MockTransportAdapterImpl transport_adapter(
      NULL, NULL, NULL, last_state_, transport_manager_settings);
  EXPECT_CALL(transport_adapter, Restore()).WillOnce(Return(true));
  transport_adapter.Init();

  TransportConfig empty_config;

  EXPECT_EQ(empty_config, transport_adapter.GetTransportConfiguration());
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

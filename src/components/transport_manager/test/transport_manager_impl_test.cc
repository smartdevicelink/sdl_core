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
#include "protocol/raw_message.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager_impl.h"

#include "include/transport_adapter_mock.h"
#include "include/transport_manager_listener_mock.h"
#include "include/transport_adapter_listener_mock.h"
#include "include/mock_telemetry_observer.h"

#include "transport_manager/transport_adapter/transport_adapter_event.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

using ::protocol_handler::RawMessage;
using ::protocol_handler::RawMessagePtr;

namespace test {
namespace components {
namespace transport_manager_test {

using namespace ::transport_manager;

class TransportManagerTest : public TransportManagerImpl {
 public:
  void TestHandle(TransportAdapterEvent test_event) { Handle(test_event); }
};

class TransportManagerImplTest : public ::testing::Test {
 protected:
  virtual void SetUp() {
    tm.Init();
    mock_adapter = new TransportAdapterMock();
    tm_listener = new TransportManagerListenerMock();

    EXPECT_EQ(E_SUCCESS, tm.AddEventListener(tm_listener));
    EXPECT_CALL(*mock_adapter, AddListener(_));
    EXPECT_CALL(*mock_adapter, IsInitialised()).WillOnce(Return(true));
    EXPECT_EQ(::transport_manager::E_SUCCESS,
              tm.AddTransportAdapter(mock_adapter));

    device_handle_ = 1;
    connection_key_ = 1;
    mac_address_ = "MA:CA:DR:ES:S";
    error_ = new BaseError();

    const unsigned int version_protocol_ = 1;
    const unsigned int kSize = 12;
    unsigned char data[kSize] = {0x20, 0x07, 0x01, 0x00, 0x00, 0x00,
                                 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    test_message_ =
        new RawMessage(connection_key_, version_protocol_, data, kSize);
  }

  virtual void TearDown() { delete tm_listener; }

  void HandleDeviceListUpdated();
  void HandleConnection();
  void HandleSendDone();
  void HandleSendFailed();
  void HandleSearchDone();
  void HandleSearchFail();
  void HandleFindNewApplicationsRequest();
  void HandleConnectionFailed();
  void HandleConnectionClosed();
  void HandleDisconnectionFailed();
  void HandleReceiveDone();

  TransportManagerTest tm;
  TransportAdapterMock* mock_adapter;

  TransportManagerListenerMock* tm_listener;

  const ApplicationHandle application_id = 1;

  // count of connections
  ConnectionUID connection_key_;
  RawMessagePtr test_message_;
  DeviceHandle device_handle_;
  std::string mac_address_;

  DeviceList dev;
  BaseErrorPtr error_;
};

void TransportManagerImplTest::HandleDeviceListUpdated() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DEVICE_LIST_UPDATED);
  const DeviceInfo dev_info(device_handle_, mac_address_, "TestDeviceName",
                            "BTMAC");

  TransportAdapterEvent test_event(type, mock_adapter, dev_info.mac_address(),
                                   application_id, test_message_, error_);
  dev.push_back(dev_info.mac_address());
  std::vector<DeviceInfo> vector_dev_info;
  std::vector<DeviceInfo>::iterator it = vector_dev_info.begin();
  vector_dev_info.insert(it, dev_info);

  EXPECT_CALL(*mock_adapter, GetDeviceList())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(dev));
  EXPECT_CALL(*mock_adapter, DeviceName(dev_info.mac_address()))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(dev_info.name()));
  EXPECT_CALL(*mock_adapter, GetConnectionType())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(dev_info.connection_type()));

  EXPECT_CALL(*tm_listener, OnDeviceFound(dev_info));
  EXPECT_CALL(*tm_listener, OnDeviceAdded(dev_info));
  EXPECT_CALL(*tm_listener, OnDeviceListUpdated(vector_dev_info));

  tm.TestHandle(test_event);
  dev.pop_back();
}

void TransportManagerImplTest::HandleConnection() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE);
  const DeviceInfo dev_info(device_handle_, mac_address_, "TestDeviceName",
                            "BTMAC");

  TransportAdapterEvent test_event(type, mock_adapter, dev_info.mac_address(),
                                   application_id, test_message_, error_);

  EXPECT_CALL(*mock_adapter, DeviceName(dev_info.mac_address()))
      .WillOnce(Return(dev_info.name()));
  EXPECT_CALL(*mock_adapter, GetConnectionType())
      .WillOnce(Return(dev_info.connection_type()));

  EXPECT_CALL(*tm_listener, OnConnectionEstablished(dev_info, connection_key_));

  tm.TestHandle(test_event);
}

void TransportManagerImplTest::HandleConnectionFailed() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL);

  const DeviceInfo dev_info(device_handle_, mac_address_, "TestDeviceName",
                            "BTMAC");

  TransportAdapterEvent test_event(type, mock_adapter, dev_info.mac_address(),
                                   application_id, test_message_, error_);

  EXPECT_CALL(*mock_adapter, DeviceName(dev_info.mac_address()))
      .WillOnce(Return(dev_info.name()));
  EXPECT_CALL(*mock_adapter, GetConnectionType())
      .WillOnce(Return(dev_info.connection_type()));

  EXPECT_CALL(*tm_listener, OnConnectionFailed(dev_info, _));

  tm.TestHandle(test_event);
}

void TransportManagerImplTest::HandleSendDone() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE);
  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  EXPECT_CALL(*tm_listener, OnTMMessageSend(test_message_));

  tm.TestHandle(test_event);
}

void TransportManagerImplTest::HandleReceiveDone() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE);
  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  EXPECT_CALL(*tm_listener, OnTMMessageReceived(test_message_));

  tm.TestHandle(test_event);
}

void TransportManagerImplTest::HandleSendFailed() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL);

  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  tm.TestHandle(test_event);
}

void TransportManagerImplTest::HandleSearchDone() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE);

  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  EXPECT_CALL(*tm_listener, OnScanDevicesFinished());

  tm.TestHandle(test_event);
}

void TransportManagerImplTest::HandleSearchFail() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL);

  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_));

  tm.TestHandle(test_event);
}

void TransportManagerImplTest::HandleFindNewApplicationsRequest() {
  int type = static_cast<int>(TransportAdapterListenerImpl::EventTypeEnum::
                                  ON_FIND_NEW_APPLICATIONS_REQUEST);

  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  EXPECT_CALL(*tm_listener, OnFindNewApplicationsRequest());

  tm.TestHandle(test_event);
}

void TransportManagerImplTest::HandleConnectionClosed() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE);

  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  EXPECT_CALL(*tm_listener, OnConnectionClosed(application_id));

  tm.TestHandle(test_event);
}

void TransportManagerImplTest::HandleDisconnectionFailed() {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL);

  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  EXPECT_CALL(*tm_listener, OnDisconnectFailed(device_handle_, _));

  tm.TestHandle(test_event);
}

TEST(TransportManagerTest, SearchDevices_AdaptersNotAdded) {
  TransportManagerTest tm;
  tm.Init();

  EXPECT_EQ(E_SUCCESS, tm.SearchDevices());
}

TEST(TransportManagerTest, AddTransportAdapter) {
  TransportManagerTest tm;
  tm.Init();

  TransportAdapterMock* mock_adapter = new TransportAdapterMock();
  TransportManagerListenerMock* tm_listener =
      new TransportManagerListenerMock();

  EXPECT_EQ(E_SUCCESS, tm.AddEventListener(tm_listener));
  EXPECT_CALL(*mock_adapter, AddListener(_));
  EXPECT_CALL(*mock_adapter, IsInitialised()).WillOnce(Return(false));
  EXPECT_CALL(*mock_adapter, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(::transport_manager::E_SUCCESS,
            tm.AddTransportAdapter(mock_adapter));
}

TEST_F(TransportManagerImplTest, AddTransportAdapterSecondTime) {
  EXPECT_EQ(E_ADAPTER_EXISTS, tm.AddTransportAdapter(mock_adapter));
}

TEST_F(TransportManagerImplTest, ConnectDevice) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, ConnectDevice_DeviceNotHandled) {
  EXPECT_CALL(*mock_adapter, ConnectDevice(mac_address_)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, tm.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, ConnectDevice_DeviceNotConnected) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_INTERNAL_ERROR, tm.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter, DisconnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));

  EXPECT_EQ(E_SUCCESS, tm.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice_DeviceNotConnected) {
  EXPECT_CALL(*mock_adapter, DisconnectDevice(mac_address_)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, tm.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, Disconnect) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter, Disconnect(mac_address_, application_id))
      .WillOnce(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_SUCCESS, tm.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, Disconnect_ConnectionNotExist) {
  EXPECT_CALL(*mock_adapter, Disconnect(mac_address_, application_id)).Times(0);
  // Assert
  EXPECT_EQ(E_INVALID_HANDLE, tm.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, Disconnect_ConnectionDoesNotExists) {
  // Arrange
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter, ConnectDevice(mac_address_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter, Disconnect(mac_address_, application_id))
      .WillRepeatedly(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_INVALID_HANDLE, tm.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, DisconnectForce) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter, Disconnect(mac_address_, application_id))
      .WillRepeatedly(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_SUCCESS, tm.DisconnectForce(connection_key_));
}

TEST_F(TransportManagerImplTest, DisconnectForce_) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter, Disconnect(mac_address_, application_id))
      .WillRepeatedly(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_SUCCESS, tm.DisconnectForce(connection_key_));
}

TEST_F(TransportManagerImplTest, SearchDevices_DeviceConnected) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter, SearchDevices())
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm.SearchDevices());

  HandleSearchDone();
}

TEST_F(TransportManagerImplTest, SearchDevices_DeviceNotFound) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter, SearchDevices())
      .WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_ADAPTERS_FAIL, tm.SearchDevices());
}

TEST_F(TransportManagerImplTest, SearchDevices_AdapterNotSupported) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter, SearchDevices())
      .WillOnce(Return(TransportAdapter::NOT_SUPPORTED));
  EXPECT_EQ(E_ADAPTERS_FAIL, tm.SearchDevices());
}

TEST_F(TransportManagerImplTest, SearchDevices_AdapterWithBadState) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter, SearchDevices())
      .WillOnce(Return(TransportAdapter::BAD_STATE));
  EXPECT_EQ(E_ADAPTERS_FAIL, tm.SearchDevices());
}

TEST_F(TransportManagerImplTest, SendMessageToDevice) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter,
              SendData(mac_address_, application_id, test_message_))
      .WillOnce(Return(TransportAdapter::OK));

  EXPECT_EQ(E_SUCCESS, tm.SendMessageToDevice(test_message_));
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_SendingFailed) {
  // Arrange
  HandleConnection();

  MockTMTelemetryObserver* mock_metric_observer = new MockTMTelemetryObserver();
  tm.SetTelemetryObserver(mock_metric_observer);
  EXPECT_CALL(*mock_metric_observer, StartRawMsg(_));

  EXPECT_CALL(*mock_adapter,
              SendData(mac_address_, application_id, test_message_))
      .WillOnce(Return(TransportAdapter::FAIL));

  EXPECT_CALL(*tm_listener, OnTMMessageSendFailed(_, test_message_));
  EXPECT_EQ(E_SUCCESS, tm.SendMessageToDevice(test_message_));

  EXPECT_CALL(*mock_metric_observer, StopRawMsg(_)).Times(0);

  delete mock_metric_observer;
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_StartTimeObserver) {
  // Arrange
  HandleConnection();

  MockTMTelemetryObserver* mock_metric_observer = new MockTMTelemetryObserver();
  tm.SetTelemetryObserver(mock_metric_observer);
  EXPECT_CALL(*mock_adapter,
              SendData(mac_address_, application_id, test_message_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*mock_metric_observer, StartRawMsg(_));

  EXPECT_EQ(E_SUCCESS, tm.SendMessageToDevice(test_message_));
  delete mock_metric_observer;
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_SendDone) {
  // Arrange
  HandleConnection();

  MockTMTelemetryObserver* mock_metric_observer = new MockTMTelemetryObserver();
  tm.SetTelemetryObserver(mock_metric_observer);
  EXPECT_CALL(*mock_adapter,
              SendData(mac_address_, application_id, test_message_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_CALL(*mock_metric_observer, StartRawMsg(_));

  EXPECT_EQ(E_SUCCESS, tm.SendMessageToDevice(test_message_));

  EXPECT_CALL(*mock_metric_observer, StopRawMsg(_));
  HandleSendDone();

  delete mock_metric_observer;
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, SendMessageFailed_GetHandleSendFailed) {
  // Arrange
  HandleConnection();

  MockTMTelemetryObserver* mock_metric_observer = new MockTMTelemetryObserver();
  tm.SetTelemetryObserver(mock_metric_observer);
  EXPECT_CALL(*mock_metric_observer, StartRawMsg(_));

  EXPECT_CALL(*mock_adapter,
              SendData(mac_address_, application_id, test_message_))
      .WillOnce(Return(TransportAdapter::FAIL));

  EXPECT_CALL(*tm_listener, OnTMMessageSendFailed(_, test_message_));
  EXPECT_EQ(E_SUCCESS, tm.SendMessageToDevice(test_message_));

  EXPECT_CALL(*mock_metric_observer, StopRawMsg(_));

  HandleSendFailed();
  delete mock_metric_observer;
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, RemoveDevice_DeviceWasAdded) {
  // Arrange
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm.ConnectDevice(device_handle_));

  // Assert
  EXPECT_EQ(E_SUCCESS, tm.RemoveDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, SetVisibilityOn_StartClientListening) {
  EXPECT_CALL(*mock_adapter, StartClientListening())
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(::transport_manager::E_SUCCESS, tm.Visibility(true));
}

TEST_F(TransportManagerImplTest, SetVisibilityOff_StopClientListening) {
  EXPECT_CALL(*mock_adapter, StopClientListening())
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(::transport_manager::E_SUCCESS, tm.Visibility(false));
}

TEST_F(TransportManagerImplTest, StopTransportManager) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter, ConnectDevice(mac_address_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter, DisconnectDevice(mac_address_))
      .WillRepeatedly(Return(TransportAdapter::OK));

  EXPECT_CALL(*mock_adapter, Terminate());
  EXPECT_EQ(E_SUCCESS, tm.Stop());
}

TEST_F(TransportManagerImplTest, Reinit) {
  EXPECT_CALL(*mock_adapter, Terminate());
  EXPECT_CALL(*mock_adapter, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm.Reinit());
}

TEST_F(TransportManagerImplTest, Reinit_InitAdapterFailed) {
  EXPECT_CALL(*mock_adapter, Terminate());
  EXPECT_CALL(*mock_adapter, Init()).WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_ADAPTERS_FAIL, tm.Reinit());
}

TEST_F(TransportManagerImplTest, UpdateDeviceList_AddNewDevice) {
  const DeviceInfo dev_info(device_handle_, mac_address_, "TestDeviceName",
                            "BTMAC");

  dev.push_back(dev_info.mac_address());
  std::vector<DeviceInfo> vector_dev_info;
  std::vector<DeviceInfo>::iterator it = vector_dev_info.begin();
  vector_dev_info.insert(it, dev_info);

  EXPECT_CALL(*mock_adapter, GetDeviceList()).WillOnce(Return(dev));
  EXPECT_CALL(*mock_adapter, DeviceName(dev_info.mac_address()))
      .WillOnce(Return(dev_info.name()));
  EXPECT_CALL(*mock_adapter, GetConnectionType())
      .WillOnce(Return(dev_info.connection_type()));
  EXPECT_CALL(*tm_listener, OnDeviceAdded(dev_info));

  tm.UpdateDeviceList(mock_adapter);
  dev.pop_back();
}

TEST_F(TransportManagerImplTest, UpdateDeviceList_RemoveDevice) {
  const DeviceInfo dev_info(device_handle_, mac_address_, "TestDeviceName",
                            "BTMAC");

  dev.push_back(dev_info.mac_address());
  std::vector<DeviceInfo> vector_dev_info;
  std::vector<DeviceInfo>::iterator it = vector_dev_info.begin();
  vector_dev_info.insert(it, dev_info);

  ::testing::InSequence seq;
  EXPECT_CALL(*mock_adapter, GetDeviceList()).WillOnce(Return(dev));
  EXPECT_CALL(*mock_adapter, GetConnectionType())
      .WillOnce(Return(dev_info.connection_type()));
  EXPECT_CALL(*mock_adapter, DeviceName(dev_info.mac_address()))
      .WillOnce(Return(dev_info.name()));
  EXPECT_CALL(*tm_listener, OnDeviceAdded(dev_info));
  tm.UpdateDeviceList(mock_adapter);
  dev.pop_back();

  // Device list is empty now
  EXPECT_CALL(*mock_adapter, GetDeviceList()).WillOnce(Return(dev));
  EXPECT_CALL(*tm_listener, OnDeviceRemoved(dev_info));
  tm.UpdateDeviceList(mock_adapter);
}

/*
 * Tests which check correct handling and receiving events
 */
TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_OnSearchDeviceDone) {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE);

  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  EXPECT_CALL(*tm_listener, OnScanDevicesFinished());

  tm.ReceiveEventFromDevice(test_event);
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_OnSearchDeviceFail) {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL);

  TransportAdapterEvent test_event(type, mock_adapter, mac_address_,
                                   application_id, test_message_, error_);

  EXPECT_CALL(*tm_listener, OnScanDevicesFailed(_));

  tm.ReceiveEventFromDevice(test_event);
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_DeviceListUpdated) {
  int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DEVICE_LIST_UPDATED);
  const DeviceInfo dev_info(device_handle_, mac_address_, "TestDeviceName",
                            "BTMAC");

  TransportAdapterEvent test_event(type, mock_adapter, dev_info.mac_address(),
                                   application_id, test_message_, error_);
  dev.push_back(dev_info.mac_address());
  std::vector<DeviceInfo> vector_dev_info;
  std::vector<DeviceInfo>::iterator it = vector_dev_info.begin();
  vector_dev_info.insert(it, dev_info);

  EXPECT_CALL(*mock_adapter, GetDeviceList())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(dev));
  EXPECT_CALL(*mock_adapter, DeviceName(dev_info.mac_address()))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(dev_info.name()));
  EXPECT_CALL(*mock_adapter, GetConnectionType())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(dev_info.connection_type()));

  EXPECT_CALL(*tm_listener, OnDeviceFound(dev_info));
  EXPECT_CALL(*tm_listener, OnDeviceAdded(dev_info));
  EXPECT_CALL(*tm_listener, OnDeviceListUpdated(vector_dev_info));

  tm.ReceiveEventFromDevice(test_event);
  dev.pop_back();
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, CheckEvents) {
  HandleDeviceListUpdated();
  HandleConnection();
  HandleSendDone();
  HandleSendFailed();
  HandleSearchDone();
  HandleSearchFail();
  HandleFindNewApplicationsRequest();
  HandleConnectionFailed();
  HandleConnectionClosed();

  HandleDisconnectionFailed();
}

TEST_F(TransportManagerImplTest, CheckReceiveEvent) {
  HandleConnection();
  HandleReceiveDone();
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

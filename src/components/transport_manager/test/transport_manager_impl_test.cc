/*
 * Copyright (c) 2017, Ford Motor Company
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

#include <string>
#include "gtest/gtest.h"
#include "protocol/raw_message.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager_impl.h"
#include "transport_manager/mock_telemetry_observer.h"
#include "transport_manager/mock_transport_manager_listener.h"
#include "transport_manager/mock_telemetry_observer.h"
#include "transport_manager/transport_adapter/mock_transport_adapter.h"
#include "transport_manager/mock_transport_manager_impl.h"
#include "transport_manager/mock_transport_manager_settings.h"
#include "resumption/last_state_impl.h"

#include "utils/test_async_waiter.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::DoAll;

using ::protocol_handler::RawMessage;
using ::protocol_handler::RawMessagePtr;

using std::make_shared;

namespace test {
namespace components {
namespace transport_manager_test {

namespace {
const std::string kAppStorageFolder = "app_storage_folder";
const std::string kAppInfoFolder = "app_info_folder";
const uint32_t kAsyncExpectationsTimeout = 10000u;
}

class TransportManagerImplTest : public ::testing::Test {
 protected:
  TransportManagerImplTest()
      : mock_adapter_(NULL)
      , tm_(mock_transport_manager_settings_)
      , mac_address_("MA:CA:DR:ES:S")
      , connection_type_("BTMAC")
      , device_name_("TestDeviceName")
      , device_handle_(
            tm_.get_converter().UidToHandle(mac_address_, connection_type_))
      , dev_info_(
            device_handle_, mac_address_, device_name_, connection_type_) {}

  void SetUp() OVERRIDE {
    resumption::LastStateImpl last_state_("app_storage_folder",
                                          "app_info_storage");
    tm_.Init(last_state_);
    mock_adapter_ = new MockTransportAdapter();
    tm_listener_ = std::make_shared<MockTransportManagerListener>();

#ifdef TELEMETRY_MONITOR
    tm_.SetTelemetryObserver(&mock_metric_observer_);
#endif  // TELEMETRY_MONITOR
    EXPECT_EQ(E_SUCCESS, tm_.AddEventListener(tm_listener_.get()));
    EXPECT_CALL(*mock_adapter_, AddListener(_));
    EXPECT_CALL(*mock_adapter_, IsInitialised()).WillOnce(Return(true));
    EXPECT_EQ(E_SUCCESS, tm_.AddTransportAdapter(mock_adapter_));

    connection_key_ = 1;
    error_ = std::make_shared<BaseError>();

    const unsigned int version_protocol_ = 1;
    const unsigned int kSize = 12;
    unsigned char data[kSize] = {
        0x20, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    test_message_ = std::make_shared<RawMessage>(
        connection_key_, version_protocol_, data, kSize);
  }

  DeviceInfo ConstructDeviceInfo(const std::string& mac_address,
                                 const std::string& connection_type,
                                 const std::string& device_name) {
    const auto device_handle(
        tm_.get_converter().UidToHandle(mac_address, connection_type));

    return DeviceInfo(device_handle, mac_address, device_name, connection_type);
  }

  void SetOnDeviceExpectations(const DeviceInfo& device_info) {
    EXPECT_CALL(*tm_listener_, OnDeviceAdded(device_info));
    EXPECT_CALL(*tm_listener_, OnDeviceListUpdated(_));
  }

  void SetDeviceExpectations(MockTransportAdapter* mock_adapter,
                             const DeviceList& device_list,
                             const DeviceInfo& device_info) {
    EXPECT_CALL(*mock_adapter, GetDeviceList())
        .WillRepeatedly(Return(device_list));

    EXPECT_CALL(*mock_adapter, DeviceName(device_info.mac_address()))
        .WillRepeatedly(Return(device_info.name()));

    EXPECT_CALL(*mock_adapter, GetConnectionType())
        .WillRepeatedly(Return(device_info.connection_type()));
  }

  void SetAddDeviceExpectations(MockTransportAdapter* mock_adapter,
                                transport_adapter::DeviceType type,
                                const DeviceList& device_list,
                                const DeviceInfo& device_info) {
    SetDeviceExpectations(mock_adapter, device_list, device_info);

    EXPECT_CALL(*mock_adapter, GetDeviceType()).WillRepeatedly(Return(type));

    SetOnDeviceExpectations(device_info);
  }

  void HandleDeviceListUpdated() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_DEVICE_LIST_UPDATED,
                                     mock_adapter_,
                                     dev_info_.mac_address(),
                                     application_id_,
                                     test_message_,
                                     error_);
    device_list_.push_back(dev_info_.mac_address());
    std::vector<DeviceInfo> vector_dev_info;
    vector_dev_info.push_back(dev_info_);

    EXPECT_CALL(*mock_adapter_, GetDeviceList())
        .Times(AtLeast(1))
        .WillRepeatedly(Return(device_list_));
    EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
        .Times(AtLeast(1))
        .WillRepeatedly(Return(dev_info_.name()));
    EXPECT_CALL(*mock_adapter_, GetConnectionType())
        .Times(AtLeast(1))
        .WillRepeatedly(Return(dev_info_.connection_type()));

    EXPECT_CALL(*tm_listener_, OnDeviceAdded(dev_info_));
    EXPECT_CALL(*tm_listener_, OnDeviceListUpdated(_));

    tm_.TestHandle(test_event);
    device_list_.pop_back();
  }

  void HandleConnection() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_CONNECT_DONE,
                                     mock_adapter_,
                                     dev_info_.mac_address(),
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
        .WillOnce(Return(dev_info_.name()));
    EXPECT_CALL(*mock_adapter_, GetConnectionType())
        .WillRepeatedly(Return(dev_info_.connection_type()));

    EXPECT_CALL(*tm_listener_,
                OnConnectionEstablished(dev_info_, connection_key_));

    tm_.TestHandle(test_event);
  }

  void HandlePending() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_CONNECT_PENDING,
                                     mock_adapter_,
                                     dev_info_.mac_address(),
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
        .WillOnce(Return(dev_info_.name()));
    EXPECT_CALL(*mock_adapter_, GetConnectionType())
        .WillRepeatedly(Return(dev_info_.connection_type()));

    EXPECT_CALL(*tm_listener_, OnConnectionPending(dev_info_, connection_key_));

    tm_.TestHandle(test_event);
  }

  void HandleConnectionFailed() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_CONNECT_FAIL,
                                     mock_adapter_,
                                     dev_info_.mac_address(),
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
        .WillOnce(Return(dev_info_.name()));
    EXPECT_CALL(*mock_adapter_, GetConnectionType())
        .WillRepeatedly(Return(dev_info_.connection_type()));

    EXPECT_CALL(*tm_listener_, OnConnectionFailed(dev_info_, _));

    tm_.TestHandle(test_event);
  }

  void HandleSendDone() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_SEND_DONE,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);
#ifdef TELEMETRY_MONITOR
    EXPECT_CALL(mock_metric_observer_, StopRawMsg(test_event.event_data.get()));
#endif  // TELEMETRY_MONITOR
    EXPECT_CALL(*tm_listener_, OnTMMessageSend(test_message_));

    tm_.TestHandle(test_event);
  }

  void HandleReceiveDone() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_RECEIVED_DONE,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);
#ifdef TELEMETRY_MONITOR
    EXPECT_CALL(mock_metric_observer_, StopRawMsg(_));
#endif  // TELEMETRY_MONITOR
    EXPECT_CALL(*tm_listener_, OnTMMessageReceived(test_message_));

    tm_.TestHandle(test_event);
  }

  void HandleSendFailed() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_SEND_FAIL,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);
#ifdef TELEMETRY_MONITOR
    EXPECT_CALL(mock_metric_observer_, StopRawMsg(_));
#endif  // TELEMETRY_MONITOR
    tm_.TestHandle(test_event);
  }

  void HandleSearchDone() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_SEARCH_DONE,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*tm_listener_, OnScanDevicesFinished());

    tm_.TestHandle(test_event);
  }

  void HandleSearchFail() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_SEARCH_FAIL,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*tm_listener_, OnScanDevicesFailed(_));

    tm_.TestHandle(test_event);
  }

  void HandleFindNewApplicationsRequest() {
    TransportAdapterEvent test_event(
        EventTypeEnum::ON_FIND_NEW_APPLICATIONS_REQUEST,
        mock_adapter_,
        mac_address_,
        application_id_,
        test_message_,
        error_);

    EXPECT_CALL(*tm_listener_, OnFindNewApplicationsRequest());

    tm_.TestHandle(test_event);
  }

  void HandleConnectionClosed() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_DISCONNECT_DONE,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*tm_listener_, OnConnectionClosed(application_id_));
    EXPECT_CALL(*mock_adapter_,
                RemoveFinalizedConnection(mac_address_, application_id_));

    tm_.TestHandle(test_event);
  }

  void HandleDisconnectionFailed() {
    TransportAdapterEvent test_event(EventTypeEnum::ON_DISCONNECT_FAIL,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*tm_listener_, OnDisconnectFailed(device_handle_, _));

    tm_.TestHandle(test_event);
  }

  void UninitializeTM() {
    EXPECT_CALL(*mock_adapter_, Terminate());
    ASSERT_EQ(E_SUCCESS, tm_.Stop());
  }

  MockTransportManagerSettings mock_transport_manager_settings_;
#ifdef TELEMETRY_MONITOR
  MockTMTelemetryObserver mock_metric_observer_;
#endif  // TELEMETRY_MONITOR
  MockTransportAdapter* mock_adapter_;

  std::shared_ptr<MockTransportManagerListener> tm_listener_;
  MockTransportManagerImpl tm_;
  const ApplicationHandle application_id_ = 1;
  ConnectionUID connection_key_;
  RawMessagePtr test_message_;
  std::string mac_address_;
  std::string connection_type_;
  std::string device_name_;
  DeviceHandle device_handle_;

  const DeviceInfo dev_info_;
  DeviceList device_list_;
  BaseErrorPtr error_;
};

TEST_F(TransportManagerImplTest, SearchDevices_AdaptersNotAdded) {
  EXPECT_CALL(*mock_adapter_, SearchDevices())
      .WillOnce(
          Return(transport_manager::transport_adapter::TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm_.SearchDevices());
}

TEST_F(TransportManagerImplTest, AddTransportAdapterSecondTime) {
  EXPECT_EQ(E_ADAPTER_EXISTS, tm_.AddTransportAdapter(mock_adapter_));
}

TEST_F(TransportManagerImplTest, ConnectDevice) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm_.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, ConnectDevice_DeviceNotHandled) {
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, tm_.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, ConnectDevice_DeviceNotConnected) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_INTERNAL_ERROR, tm_.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm_.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter_, DisconnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));

  EXPECT_EQ(E_SUCCESS, tm_.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice_ConnectionFailed) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_INTERNAL_ERROR, tm_.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter_, DisconnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::FAIL));

  // Even with fail, we get Success
  EXPECT_EQ(E_SUCCESS, tm_.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice_DeviceNotConnected) {
  EXPECT_CALL(*mock_adapter_, DisconnectDevice(mac_address_)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, tm_.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, Pending) {
  // Calling HandlePending twice verifies the connection_id stays the same if
  // the connection exists.
  HandlePending();
  HandlePending();

  // Now create pending event for new app id and verify connection_id is
  // incremented
  TransportAdapterEvent test_event(EventTypeEnum::ON_CONNECT_PENDING,
                                   mock_adapter_,
                                   dev_info_.mac_address(),
                                   application_id_ + 1,
                                   test_message_,
                                   error_);

  EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
      .WillOnce(Return(dev_info_.name()));
  EXPECT_CALL(*mock_adapter_, GetConnectionType())
      .WillRepeatedly(Return(dev_info_.connection_type()));

  EXPECT_CALL(*tm_listener_,
              OnConnectionPending(dev_info_, connection_key_ + 1));

  tm_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, Disconnect) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .WillOnce(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_SUCCESS, tm_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, Disconnect_DisconnectionFailed) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .WillOnce(Return(TransportAdapter::FAIL));
  // Assert
  // Even with fail, we get Success
  EXPECT_EQ(E_SUCCESS, tm_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, Disconnect_ConnectionNotExist) {
  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .Times(0);
  // Assert
  EXPECT_EQ(E_INVALID_HANDLE, tm_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, Disconnect_ConnectionDoesNotExists) {
  // Arrange
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm_.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_INVALID_HANDLE, tm_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, DisconnectForce_TMIsInitialized) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_SUCCESS, tm_.DisconnectForce(connection_key_));
}

TEST_F(TransportManagerImplTest, DisconnectForce_) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_SUCCESS, tm_.DisconnectForce(connection_key_));
}

TEST_F(TransportManagerImplTest, SearchDevices_DeviceConnected) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, SearchDevices())
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm_.SearchDevices());

  HandleSearchDone();
}

TEST_F(TransportManagerImplTest, SearchDevices_DeviceNotFound) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, SearchDevices())
      .WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_ADAPTERS_FAIL, tm_.SearchDevices());
}

TEST_F(TransportManagerImplTest, SearchDevices_AdapterNotSupported) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, SearchDevices())
      .WillOnce(Return(TransportAdapter::NOT_SUPPORTED));
  EXPECT_EQ(E_ADAPTERS_FAIL, tm_.SearchDevices());
}

TEST_F(TransportManagerImplTest, SearchDevices_AdapterWithBadState) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, SearchDevices())
      .WillOnce(Return(TransportAdapter::BAD_STATE));
  EXPECT_EQ(E_ADAPTERS_FAIL, tm_.SearchDevices());
}

TEST_F(TransportManagerImplTest, SendMessageToDevice) {
  // Arrange
  HandleConnection();

  TestAsyncWaiter waiter;
  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter), Return(TransportAdapter::OK)));

#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(test_message_.get()));
#endif  // TELEMETRY_MONITOR

  EXPECT_EQ(E_SUCCESS, tm_.SendMessageToDevice(test_message_));

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_SendingFailed) {
  // Arrange
  HandleConnection();

#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(_));
#endif  // TELEMETRY_MONITOR

  TestAsyncWaiter waiter;
  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(Return(TransportAdapter::FAIL));

  EXPECT_CALL(*tm_listener_, OnTMMessageSendFailed(_, test_message_))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  EXPECT_EQ(E_SUCCESS, tm_.SendMessageToDevice(test_message_));
#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StopRawMsg(_)).Times(0);
#endif  // TELEMETRY_MONITOR

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_StartTimeObserver) {
  // Arrange
  HandleConnection();

  TestAsyncWaiter waiter;
  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter), Return(TransportAdapter::OK)));

#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(_));
#endif  // TELEMETRY_MONITOR

  EXPECT_EQ(E_SUCCESS, tm_.SendMessageToDevice(test_message_));

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_SendDone) {
  // Arrange
  HandleConnection();

  TestAsyncWaiter waiter;
  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(
          DoAll(NotifyTestAsyncWaiter(&waiter), Return(TransportAdapter::OK)));

#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(test_message_.get()));
#endif  // TELEMETRY_MONITOR

  EXPECT_EQ(E_SUCCESS, tm_.SendMessageToDevice(test_message_));

  HandleSendDone();

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}

TEST_F(TransportManagerImplTest, SendMessageFailed_GetHandleSendFailed) {
  // Arrange
  HandleConnection();

  TestAsyncWaiter waiter;
  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(&waiter),
                      Return(TransportAdapter::FAIL)));

#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(test_message_.get()));
#endif  // TELEMETRY_MONITOR

  EXPECT_CALL(*tm_listener_, OnTMMessageSendFailed(_, test_message_));
  EXPECT_EQ(E_SUCCESS, tm_.SendMessageToDevice(test_message_));

  HandleSendFailed();

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}

TEST_F(TransportManagerImplTest, RemoveDevice_DeviceWasAdded) {
  // Arrange
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm_.ConnectDevice(device_handle_));

  // Assert
  EXPECT_EQ(E_SUCCESS, tm_.RemoveDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, SetVisibilityOn_StartClientListening) {
  EXPECT_CALL(*mock_adapter_, StartClientListening())
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(::transport_manager::E_SUCCESS, tm_.Visibility(true));
}

TEST_F(TransportManagerImplTest, SetVisibilityOff_StopClientListening) {
  EXPECT_CALL(*mock_adapter_, StopClientListening())
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(::transport_manager::E_SUCCESS, tm_.Visibility(false));
}

TEST_F(TransportManagerImplTest, StopTransportManager) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm_.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter_, DisconnectDevice(mac_address_))
      .WillRepeatedly(Return(TransportAdapter::OK));

  EXPECT_CALL(*mock_adapter_, Terminate());
  EXPECT_EQ(E_SUCCESS, tm_.Stop());
}

TEST_F(TransportManagerImplTest, Reinit) {
  EXPECT_CALL(*mock_adapter_, Terminate());
  EXPECT_CALL(*mock_adapter_, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, tm_.Reinit());
}

TEST_F(TransportManagerImplTest, Reinit_InitAdapterFailed) {
  EXPECT_CALL(*mock_adapter_, Terminate());
  EXPECT_CALL(*mock_adapter_, Init()).WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_ADAPTERS_FAIL, tm_.Reinit());
}

TEST_F(TransportManagerImplTest, UpdateDeviceList_AddNewDevice) {
  device_list_.push_back(dev_info_.mac_address());

  SetDeviceExpectations(mock_adapter_, device_list_, dev_info_);
  EXPECT_CALL(*tm_listener_, OnDeviceFound(dev_info_));
  SetOnDeviceExpectations(dev_info_);
  tm_.OnDeviceListUpdated(mock_adapter_);
  device_list_.pop_back();
}

TEST_F(TransportManagerImplTest, UpdateDeviceList_RemoveDevice) {
  device_list_.push_back(dev_info_.mac_address());
  {
    SetDeviceExpectations(mock_adapter_, device_list_, dev_info_);
    ::testing::InSequence s;
    EXPECT_CALL(*tm_listener_, OnDeviceFound(dev_info_));
    SetOnDeviceExpectations(dev_info_);
    tm_.OnDeviceListUpdated(mock_adapter_);
  }
  device_list_.pop_back();

  SetDeviceExpectations(mock_adapter_, device_list_, dev_info_);

  // Device list is empty now
  ::testing::InSequence s;
  EXPECT_CALL(*tm_listener_, OnDeviceRemoved(dev_info_));
  EXPECT_CALL(*tm_listener_, OnDeviceListUpdated(_));
  tm_.OnDeviceListUpdated(mock_adapter_);
}

/*
 * Tests which check correct handling and receiving events
 */
TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_OnSearchDeviceDone) {
  TestAsyncWaiter waiter;
  TransportAdapterEvent test_event(EventTypeEnum::ON_SEARCH_DONE,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);

  EXPECT_CALL(*tm_listener_, OnScanDevicesFinished())
      .WillOnce(NotifyTestAsyncWaiter(&waiter));

  tm_.ReceiveEventFromDevice(test_event);

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}

TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_OnSearchDeviceFail) {
  TestAsyncWaiter waiter;
  TransportAdapterEvent test_event(EventTypeEnum::ON_SEARCH_FAIL,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);

  EXPECT_CALL(*tm_listener_, OnScanDevicesFailed(_))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));

  tm_.ReceiveEventFromDevice(test_event);

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}

TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_DeviceListUpdated) {
  TransportAdapterEvent test_event(EventTypeEnum::ON_DEVICE_LIST_UPDATED,
                                   mock_adapter_,
                                   dev_info_.mac_address(),
                                   application_id_,
                                   test_message_,
                                   error_);
  device_list_.push_back(dev_info_.mac_address());
  std::vector<DeviceInfo> vector_dev_info;
  vector_dev_info.push_back(dev_info_);

  TestAsyncWaiter waiter;
  EXPECT_CALL(*mock_adapter_, GetDeviceList())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(device_list_));
  EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
      .Times(AtLeast(1))
      .WillRepeatedly(Return(dev_info_.name()));
  EXPECT_CALL(*mock_adapter_, GetConnectionType())
      .Times(AtLeast(1))
      .WillRepeatedly(Return(dev_info_.connection_type()));

  EXPECT_CALL(*tm_listener_, OnDeviceFound(dev_info_))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));
  EXPECT_CALL(*tm_listener_, OnDeviceAdded(dev_info_))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));

  tm_.ReceiveEventFromDevice(test_event);
  device_list_.pop_back();

  EXPECT_TRUE(waiter.WaitFor(2, kAsyncExpectationsTimeout));
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

TEST_F(TransportManagerImplTest, CheckReceiveFailedEvent) {
  // Arrange
  TransportAdapterEvent test_event(EventTypeEnum::ON_RECEIVED_FAIL,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
  // Check before act
  HandleConnection();
  // Act and Assert
  EXPECT_CALL(*tm_listener_, OnTMMessageReceiveFailed(_));
  tm_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, CheckUnexpectedDisconnect) {
  // Arrange
  TransportAdapterEvent test_event(EventTypeEnum::ON_UNEXPECTED_DISCONNECT,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
  // Check before act
  HandleConnection();
  // Act and Assert
  EXPECT_CALL(*tm_listener_, OnUnexpectedDisconnect(connection_key_, _));
  EXPECT_CALL(*mock_adapter_,
              RemoveFinalizedConnection(mac_address_, application_id_));
  tm_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, ConnectDevice_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, ConnectDevice(_)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, DisconnectDevice(_)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, Disconnect_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, Disconnect(_, _)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, DisconnectForce_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, Disconnect(_, _)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.DisconnectForce(connection_key_));
}

TEST_F(TransportManagerImplTest, DisconnectForce_ConnectionNotExist) {
  // SetUp does not add connections
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, Disconnect(_, _)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, tm_.DisconnectForce(connection_key_));
}

TEST_F(TransportManagerImplTest, Stop_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, DisconnectDevice(_)).Times(0);
  EXPECT_CALL(*mock_adapter_, Terminate()).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.Stop());
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, SendData(_, _, _)).Times(0);
  EXPECT_CALL(*tm_listener_, OnTMMessageSendFailed(_, _)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.SendMessageToDevice(test_message_));
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_ConnectionNotExist) {
  // SetUp does not add connections
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, SendData(_, _, _)).Times(0);
  EXPECT_CALL(*tm_listener_, OnTMMessageSendFailed(_, _)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, tm_.SendMessageToDevice(test_message_));
}

TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_TMIsNotInitialized) {
  // Arrange
  TransportAdapterEvent test_event(EventTypeEnum::ON_SEARCH_DONE,
                                   NULL,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*tm_listener_, OnScanDevicesFinished()).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.ReceiveEventFromDevice(test_event));
}

TEST_F(TransportManagerImplTest, RemoveDevice_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.RemoveDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, Visibility_TMIsNotInitialized) {
  // Arrange
  const bool visible = true;
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, StartClientListening()).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.Visibility(visible));
}

TEST_F(TransportManagerImplTest, HandleMessage_ConnectionNotExist) {
  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_)).Times(0);

  TestAsyncWaiter waiter;
  EXPECT_CALL(*tm_listener_, OnTMMessageSendFailed(_, test_message_))
      .WillOnce(NotifyTestAsyncWaiter(&waiter));

  tm_.TestHandle(test_message_);

  EXPECT_TRUE(waiter.WaitFor(1, kAsyncExpectationsTimeout));
}

TEST_F(TransportManagerImplTest, SearchDevices_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, SearchDevices()).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, tm_.SearchDevices());
}

TEST_F(TransportManagerImplTest, SetVisibilityOn_TransportAdapterNotSupported) {
  EXPECT_CALL(*mock_adapter_, StartClientListening())
      .WillOnce(Return(TransportAdapter::NOT_SUPPORTED));
  EXPECT_EQ(E_SUCCESS, tm_.Visibility(true));
}

TEST_F(TransportManagerImplTest,
       SetVisibilityOff_TransportAdapterNotSupported) {
  EXPECT_CALL(*mock_adapter_, StopClientListening())
      .WillOnce(Return(TransportAdapter::NOT_SUPPORTED));
  EXPECT_EQ(E_SUCCESS, tm_.Visibility(false));
}

TEST_F(TransportManagerImplTest,
       CheckEventOnDisconnectDone_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  TransportAdapterEvent test_event(EventTypeEnum::ON_DISCONNECT_DONE,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);

  // Act and Assert
  EXPECT_CALL(*tm_listener_, OnConnectionClosed(_)).Times(0);

  tm_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, CheckEventOnSendDone_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  TransportAdapterEvent test_event(EventTypeEnum::ON_SEND_DONE,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
#ifdef TELEMETRY_MONITOR
  // Act and Assert
  EXPECT_CALL(mock_metric_observer_, StopRawMsg(_));
#endif  // TELEMETRY_MONITOR
  EXPECT_CALL(*tm_listener_, OnTMMessageSend(_)).Times(0);

  tm_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, CheckEventOnReceivedDone_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  TransportAdapterEvent test_event(EventTypeEnum::ON_RECEIVED_DONE,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
#ifdef TELEMETRY_MONITOR
  // Act and Assert
  EXPECT_CALL(mock_metric_observer_, StopRawMsg(_)).Times(0);
#endif  // TELEMETRY_MONITOR
  EXPECT_CALL(*tm_listener_, OnTMMessageReceived(_)).Times(0);
  tm_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, CheckEventOnReceivedFail_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  TransportAdapterEvent test_event(EventTypeEnum::ON_RECEIVED_FAIL,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
  // Act and Assert
  EXPECT_CALL(*tm_listener_, OnTMMessageReceiveFailed(_)).Times(0);
  tm_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest,
       CheckEventOnUnexpectedDisconnect_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  TransportAdapterEvent test_event(EventTypeEnum::ON_UNEXPECTED_DISCONNECT,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
  // Act and Assert
  EXPECT_CALL(*tm_listener_, OnUnexpectedDisconnect(_, _)).Times(0);
  tm_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, RunAppOnDevice_TransportAdapterFound_SUCCESS) {
  HandleDeviceListUpdated();
  const std::string bundle_id = "test_bundle_id";
  EXPECT_CALL(*mock_adapter_, RunAppOnDevice(mac_address_, bundle_id));
  tm_.RunAppOnDevice(device_handle_, bundle_id);
}

TEST_F(TransportManagerImplTest,
       UpdateDeviceList_AddDevices_TwoTransportAdapters_ExpectSuccess) {
  // Arrange
  MockTransportAdapter* second_mock_adapter = new MockTransportAdapter();
  device_list_.push_back(dev_info_.mac_address());
  // Check before Act
  EXPECT_CALL(*second_mock_adapter, AddListener(_));
  EXPECT_CALL(*second_mock_adapter, IsInitialised()).WillOnce(Return(true));
  EXPECT_EQ(E_SUCCESS, tm_.AddTransportAdapter(second_mock_adapter));

  // Act and Assert
  SetDeviceExpectations(mock_adapter_, device_list_, dev_info_);
  EXPECT_CALL(*tm_listener_, OnDeviceFound(dev_info_));
  SetOnDeviceExpectations(dev_info_);
  tm_.OnDeviceListUpdated(mock_adapter_);

  const std::string mac_address("NE:WA:DR:ES:SS");
  const std::string connection_type("TCP");
  const std::string device_name("TestName");
  const transport_manager::DeviceHandle device_handle(
      tm_.get_converter().UidToHandle(mac_address, connection_type));

  DeviceInfo second_device(
      device_handle, mac_address, device_name, connection_type);
  DeviceList device_list_2;
  device_list_2.push_back(second_device.mac_address());

  SetDeviceExpectations(second_mock_adapter, device_list_2, second_device);

  EXPECT_CALL(*tm_listener_, OnDeviceFound(second_device));
  SetOnDeviceExpectations(second_device);
  tm_.OnDeviceListUpdated(second_mock_adapter);

  device_list_.pop_back();
}

TEST_F(
    TransportManagerImplTest,
    UpdateDeviceList_AddSameUUIDNonSwitchableDevices_TwoTransportAdapters_ExpectNoSwitch) {
  device_list_.push_back(dev_info_.mac_address());
  SetAddDeviceExpectations(mock_adapter_,
                           transport_adapter::DeviceType::TCP,
                           device_list_,
                           dev_info_);

  tm_.OnDeviceListUpdated(mock_adapter_);

  // Adapter will be removed by TM on destruction
  MockTransportAdapter* second_mock_adapter = new MockTransportAdapter();
  EXPECT_CALL(*second_mock_adapter, AddListener(_));
  EXPECT_CALL(*second_mock_adapter, IsInitialised()).WillOnce(Return(true));
  EXPECT_EQ(E_SUCCESS, tm_.AddTransportAdapter(second_mock_adapter));

  const auto usb_serial = "USB_serial";
  DeviceInfo second_device =
      ConstructDeviceInfo(usb_serial, "USB_IOS", "SecondDeviceName");

  DeviceList second_adapter_device_list;
  second_adapter_device_list.push_back(usb_serial);

  SetAddDeviceExpectations(second_mock_adapter,
                           transport_adapter::DeviceType::IOS_USB,
                           second_adapter_device_list,
                           second_device);

  tm_.OnDeviceListUpdated(second_mock_adapter);

  // Act
  EXPECT_CALL(*second_mock_adapter, StopDevice(_)).Times(0);
  EXPECT_CALL(*second_mock_adapter, DeviceSwitched(_)).Times(0);

  tm_.TestHandle(
      TransportAdapterEvent(EventTypeEnum::ON_TRANSPORT_SWITCH_REQUESTED,
                            second_mock_adapter,
                            mac_address_,
                            application_id_,
                            test_message_,
                            error_));

  device_list_.pop_back();
}

TEST_F(TransportManagerImplTest, OnlyOneDeviceShouldNotTriggerSwitch) {
  device_list_.push_back(dev_info_.mac_address());
  SetDeviceExpectations(mock_adapter_, device_list_, dev_info_);
  SetOnDeviceExpectations(dev_info_);

  EXPECT_CALL(*mock_adapter_, StopDevice(_)).Times(0);
  EXPECT_CALL(*mock_adapter_, DeviceSwitched(_)).Times(0);
  EXPECT_CALL(*tm_listener_, OnDeviceSwitchingStart(_, _)).Times(0);

  tm_.TestHandle(TransportAdapterEvent(EventTypeEnum::ON_DEVICE_LIST_UPDATED,
                                       mock_adapter_,
                                       mac_address_,
                                       application_id_,
                                       test_message_,
                                       error_));

  device_list_.pop_back();
}

TEST_F(TransportManagerImplTest,
       TwoTransportAdapterAddSameSwitchableDevice_ExpectSuccess) {
  device_list_.push_back(dev_info_.mac_address());
  const uint32_t timeout = 0;

  SetAddDeviceExpectations(mock_adapter_,
                           transport_adapter::DeviceType::IOS_BT,
                           device_list_,
                           dev_info_);

  EXPECT_CALL(*tm_listener_, OnDeviceFound(dev_info_));

  tm_.TestHandle(TransportAdapterEvent(EventTypeEnum::ON_DEVICE_LIST_UPDATED,
                                       mock_adapter_,
                                       mac_address_,
                                       application_id_,
                                       test_message_,
                                       error_));

  auto second_mock_adapter = std::make_shared<MockTransportAdapter>();

  const auto usb_serial = "USB_serial";
  DeviceInfo second_device =
      ConstructDeviceInfo(usb_serial, "USB_IOS", "SecondDeviceName");

  DeviceList second_adapter_devices;
  second_adapter_devices.push_back(second_device.mac_address());

  SetAddDeviceExpectations(second_mock_adapter.get(),
                           transport_adapter::DeviceType::IOS_USB,
                           second_adapter_devices,
                           second_device);

  EXPECT_CALL(*tm_listener_, OnDeviceFound(second_device));
  EXPECT_CALL(*second_mock_adapter, DeviceSwitched(_)).Times(0);

  tm_.TestHandle(TransportAdapterEvent(EventTypeEnum::ON_DEVICE_LIST_UPDATED,
                                       second_mock_adapter.get(),
                                       mac_address_,
                                       application_id_,
                                       test_message_,
                                       error_));

  // Act
  const auto uuid = "ABC-DEF-GHJ-KLM";
  SwitchableDevices bt_switchables;
  bt_switchables.insert(std::make_pair(dev_info_.mac_address(), uuid));
  EXPECT_CALL(*mock_adapter_, GetSwitchableDevices())
      .WillOnce(Return(bt_switchables));

  SwitchableDevices usb_switchables;
  usb_switchables.insert(std::make_pair(second_device.mac_address(), uuid));
  EXPECT_CALL(*second_mock_adapter, GetSwitchableDevices())
      .WillOnce(Return(usb_switchables));

  EXPECT_CALL(*mock_adapter_, StopDevice(mac_address_));
  EXPECT_CALL(*second_mock_adapter, DeviceSwitched(usb_serial));
  EXPECT_CALL(mock_transport_manager_settings_, app_transport_change_timer())
      .WillOnce(Return(timeout));
  EXPECT_CALL(mock_transport_manager_settings_,
              app_transport_change_timer_addition()).WillOnce(Return(0));

  EXPECT_CALL(*tm_listener_, OnDeviceSwitchingStart(mac_address_, usb_serial));

  tm_.TestHandle(
      TransportAdapterEvent(EventTypeEnum::ON_TRANSPORT_SWITCH_REQUESTED,
                            second_mock_adapter.get(),
                            mac_address_,
                            application_id_,
                            test_message_,
                            error_));

  // There is internal timer started on switching. Need to wait for timeout.
  sleep(1);
  device_list_.pop_back();
}

TEST_F(TransportManagerImplTest,
       TwoTransportAdapterAddSameDeviceSecondSkipped) {
  device_list_.push_back(dev_info_.mac_address());

  SetAddDeviceExpectations(mock_adapter_,
                           transport_adapter::DeviceType::IOS_BT,
                           device_list_,
                           dev_info_);

  EXPECT_CALL(*tm_listener_, OnDeviceFound(_));

  tm_.TestHandle(TransportAdapterEvent(EventTypeEnum::ON_DEVICE_LIST_UPDATED,
                                       mock_adapter_,
                                       mac_address_,
                                       application_id_,
                                       test_message_,
                                       error_));

  auto second_mock_adapter = std::make_shared<MockTransportAdapter>();

  DeviceInfo second_device =
      ConstructDeviceInfo("MA:CA:DR:ES:S", "USB_IOS", "SecondDeviceName");

  SetAddDeviceExpectations(second_mock_adapter.get(),
                           transport_adapter::DeviceType::IOS_USB,
                           device_list_,
                           second_device);

  EXPECT_CALL(*tm_listener_, OnDeviceFound(_)).Times(0);

  tm_.TestHandle(TransportAdapterEvent(EventTypeEnum::ON_DEVICE_LIST_UPDATED,
                                       second_mock_adapter.get(),
                                       mac_address_,
                                       application_id_,
                                       test_message_,
                                       error_));

  device_list_.pop_back();
}

TEST_F(TransportManagerImplTest, NoDeviceTransportSwitchRequest_Fail) {
  device_list_.push_back(dev_info_.mac_address());
  SetAddDeviceExpectations(mock_adapter_,
                           transport_adapter::DeviceType::IOS_USB,
                           device_list_,
                           dev_info_);

  EXPECT_CALL(*tm_listener_, OnDeviceFound(_));

  tm_.TestHandle(TransportAdapterEvent(EventTypeEnum::ON_DEVICE_LIST_UPDATED,
                                       mock_adapter_,
                                       mac_address_,
                                       application_id_,
                                       test_message_,
                                       error_));

  EXPECT_CALL(*mock_adapter_, StopDevice(mac_address_)).Times(0);

  EXPECT_CALL(*tm_listener_, OnDeviceSwitchingStart(mac_address_, mac_address_))
      .Times(0);

  tm_.TestHandle(
      TransportAdapterEvent(EventTypeEnum::ON_TRANSPORT_SWITCH_REQUESTED,
                            mock_adapter_,
                            mac_address_,
                            application_id_,
                            test_message_,
                            error_));

  device_list_.pop_back();
}

TEST_F(TransportManagerImplTest,
       UpdateDeviceList_RemoveDevices_TwoTransportAdapters_ExpectSuccess) {
  // Arrange
  MockTransportAdapter* second_mock_adapter = new MockTransportAdapter();
  device_list_.push_back(dev_info_.mac_address());
  // Check before Act
  EXPECT_CALL(*second_mock_adapter, AddListener(_));
  EXPECT_CALL(*second_mock_adapter, IsInitialised()).WillOnce(Return(true));
  EXPECT_EQ(E_SUCCESS, tm_.AddTransportAdapter(second_mock_adapter));

  // Act and Assert
  SetDeviceExpectations(mock_adapter_, device_list_, dev_info_);

  EXPECT_CALL(*tm_listener_, OnDeviceFound(dev_info_));
  SetOnDeviceExpectations(dev_info_);
  tm_.OnDeviceListUpdated(mock_adapter_);

  const std::string mac_address("NE:WA:DR:ES:SS");
  const std::string connection_type("TCP");
  const std::string device_name("TestName");
  const transport_manager::DeviceHandle device_handle(
      tm_.get_converter().UidToHandle(mac_address, connection_type));

  DeviceInfo second_device(
      device_handle, mac_address, device_name, connection_type);
  DeviceList device_list_2;
  device_list_2.push_back(second_device.mac_address());
  SetDeviceExpectations(second_mock_adapter, device_list_2, second_device);

  EXPECT_CALL(*tm_listener_, OnDeviceFound(second_device));
  SetOnDeviceExpectations(second_device);
  tm_.OnDeviceListUpdated(second_mock_adapter);

  device_list_.pop_back();
  device_list_2.pop_back();

  EXPECT_CALL(*second_mock_adapter, GetDeviceList())
      .WillRepeatedly(Return(device_list_2));
  EXPECT_CALL(*tm_listener_, OnDeviceRemoved(second_device));
  EXPECT_CALL(*tm_listener_, OnDeviceListUpdated(_));
  tm_.OnDeviceListUpdated(second_mock_adapter);

  EXPECT_CALL(*mock_adapter_, GetDeviceList())
      .WillRepeatedly(Return(device_list_));
  EXPECT_CALL(*tm_listener_, OnDeviceRemoved(dev_info_));
  EXPECT_CALL(*tm_listener_, OnDeviceListUpdated(_));
  tm_.OnDeviceListUpdated(mock_adapter_);
}

TEST_F(TransportManagerImplTest, OnTransportConfigUpdated) {
  TransportAdapterEvent test_event(EventTypeEnum::ON_TRANSPORT_CONFIG_UPDATED,
                                   mock_adapter_,
                                   "",
                                   0,
                                   test_message_,
                                   error_);

  transport_adapter::TransportConfig config;
  config[transport_manager::transport_adapter::tc_enabled] =
      std::string("true");
  config[transport_manager::transport_adapter::tc_tcp_ip_address] =
      std::string("192.168.1.1");
  config[transport_manager::transport_adapter::tc_tcp_port] =
      std::string("12345");

  EXPECT_CALL(*mock_adapter_, GetTransportConfiguration())
      .WillOnce(Return(config));

  EXPECT_CALL(*tm_listener_, OnTransportConfigUpdated(config));
  tm_.TestHandle(test_event);
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

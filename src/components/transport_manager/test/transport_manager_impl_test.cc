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

#include <string>
#include "gtest/gtest.h"
#include "protocol/raw_message.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager_impl.h"

#include "include/transport_adapter_mock.h"
#include "include/transport_manager_listener_mock.h"
#include "include/transport_adapter_listener_mock.h"
#include "include/mock_telemetry_observer.h"

#include "transport_manager/transport_adapter/transport_adapter_event.h"
#include "transport_manager/mock_transport_manager_impl.h"
#include "telemetry_monitor/mock_telemetry_observer.h"
#include "utils/make_shared.h"
#include "utils/shared_ptr.h"
#include "resumption/last_state.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::Return;

using ::protocol_handler::RawMessage;
using ::protocol_handler::RawMessagePtr;

using utils::MakeShared;

namespace test {
namespace components {
namespace transport_manager_test {

const std::string kAppStorageFolder = "app_storage_folder";
const std::string kAppInfoFolder = "app_info_folder";

class TransportManagerImplTest : public ::testing::Test {
 protected:
  TransportManagerImplTest()
      : device_handle_(1)
      , mac_address_("MA:CA:DR:ES:S")
      , dev_info_(device_handle_, mac_address_, "TestDeviceName", "BTMAC")
      , last_state_(kAppStorageFolder, kAppInfoFolder) {}

  void SetUp() OVERRIDE {
    mock_transport_manager_.Init(last_state_);
    mock_adapter_ = new TransportAdapterMock();
    mock_transport_manager_listener_ = MakeShared<TransportManagerListenerMock>();

#ifdef TELEMETRY_MONITOR
    mock_transport_manager_.SetTelemetryObserver(&mock_metric_observer_);
#endif  // TELEMETRY_MONITOR
    EXPECT_EQ(E_SUCCESS, mock_transport_manager_.AddEventListener(mock_transport_manager_listener_.get()));
    EXPECT_CALL(*mock_adapter_, AddListener(_));
    EXPECT_CALL(*mock_adapter_, IsInitialised()).WillOnce(Return(true));
    EXPECT_EQ(E_SUCCESS, mock_transport_manager_.AddTransportAdapter(mock_adapter_));

    connection_key_ = 1;
    error_ = MakeShared<BaseError>();

    const unsigned int version_protocol_ = 1;
    const unsigned int kSize = 12;
    unsigned char data[kSize] = {
        0x20, 0x07, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    test_message_ =
        MakeShared<RawMessage>(connection_key_, version_protocol_, data, kSize);
  }

  void HandleDeviceListUpdated() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_DEVICE_LIST_UPDATED);

    TransportAdapterEvent test_event(type,
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

    EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceFound(dev_info_));
    EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceAdded(dev_info_));
    EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceListUpdated(vector_dev_info));

    mock_transport_manager_.TestHandle(test_event);
    device_list_.pop_back();
  }

  void HandleConnection() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_DONE);

    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     dev_info_.mac_address(),
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
        .WillOnce(Return(dev_info_.name()));
    EXPECT_CALL(*mock_adapter_, GetConnectionType())
        .WillOnce(Return(dev_info_.connection_type()));

    EXPECT_CALL(*mock_transport_manager_listener_,
                OnConnectionEstablished(dev_info_, connection_key_));

    mock_transport_manager_.TestHandle(test_event);
  }

  void HandleConnectionFailed() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_CONNECT_FAIL);

    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     dev_info_.mac_address(),
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
        .WillOnce(Return(dev_info_.name()));
    EXPECT_CALL(*mock_adapter_, GetConnectionType())
        .WillOnce(Return(dev_info_.connection_type()));

    EXPECT_CALL(*mock_transport_manager_listener_, OnConnectionFailed(dev_info_, _));

    mock_transport_manager_.TestHandle(test_event);
  }

  void HandleSendDone() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE);
    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);
#ifdef TELEMETRY_MONITOR
    EXPECT_CALL(mock_metric_observer_, StopRawMsg(_));
#endif  // TELEMETRY_MONITOR
    EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageSend(test_message_));

    mock_transport_manager_.TestHandle(test_event);
  }

  void HandleReceiveDone() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE);
    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);
#ifdef TELEMETRY_MONITOR
    EXPECT_CALL(mock_metric_observer_, StopRawMsg(_));
#endif  // TELEMETRY_MONITOR
    EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageReceived(test_message_));

    mock_transport_manager_.TestHandle(test_event);
  }

  void HandleSendFailed() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_FAIL);

    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);
#ifdef TELEMETRY_MONITOR
    EXPECT_CALL(mock_metric_observer_, StopRawMsg(_));
#endif  // TELEMETRY_MONITOR
    mock_transport_manager_.TestHandle(test_event);
  }

  void HandleSearchDone() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE);

    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_transport_manager_listener_, OnScanDevicesFinished());

    mock_transport_manager_.TestHandle(test_event);
  }

  void HandleSearchFail() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL);

    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_transport_manager_listener_, OnScanDevicesFailed(_));

    mock_transport_manager_.TestHandle(test_event);
  }

  void HandleFindNewApplicationsRequest() {
    const int type =
        static_cast<int>(TransportAdapterListenerImpl::EventTypeEnum::
                             ON_FIND_NEW_APPLICATIONS_REQUEST);

    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_transport_manager_listener_, OnFindNewApplicationsRequest());

    mock_transport_manager_.TestHandle(test_event);
  }

  void HandleConnectionClosed() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE);

    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_transport_manager_listener_, OnConnectionClosed(application_id_));

    mock_transport_manager_.TestHandle(test_event);
  }

  void HandleDisconnectionFailed() {
    const int type = static_cast<int>(
        TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_FAIL);

    TransportAdapterEvent test_event(type,
                                     mock_adapter_,
                                     mac_address_,
                                     application_id_,
                                     test_message_,
                                     error_);

    EXPECT_CALL(*mock_transport_manager_listener_, OnDisconnectFailed(device_handle_, _));

    mock_transport_manager_.TestHandle(test_event);
  }

  void UninitializeTM() {
    EXPECT_CALL(*mock_adapter_, Terminate());
    ASSERT_EQ(E_SUCCESS, mock_transport_manager_.Stop());
  }

  MockTransportManagerImpl mock_transport_manager_;
#ifdef TELEMETRY_MONITOR
  TMTelemetryObserverMock mock_metric_observer_;
#endif  // TELEMETRY_MONITOR
  TransportAdapterMock* mock_adapter_;

  utils::SharedPtr<TransportManagerListenerMock> mock_transport_manager_listener_;

  const ApplicationHandle application_id_ = 1;

  ConnectionUID connection_key_;
  RawMessagePtr test_message_;
  DeviceHandle device_handle_;
  std::string mac_address_;

  const DeviceInfo dev_info_;
  DeviceList device_list_;
  BaseErrorPtr error_;
  resumption::LastState last_state_;
};

TEST_F(TransportManagerImplTest, SearchDevices_AdaptersNotAdded) {
  ON_CALL(*mock_adapter_, SearchDevices()).WillByDefault(Return(
        ::transport_manager::transport_adapter::TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.SearchDevices());
}

TEST_F(TransportManagerImplTest, AddTransportAdapter) {

  TransportAdapterMock* mock_adapter = new TransportAdapterMock();
  utils::SharedPtr<TransportManagerListenerMock> mock_transport_manager_listener =
      MakeShared<TransportManagerListenerMock>();

  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.AddEventListener(mock_transport_manager_listener.get()));
  EXPECT_CALL(*mock_adapter, AddListener(_));
  EXPECT_CALL(*mock_adapter, IsInitialised()).WillOnce(Return(false));
  EXPECT_CALL(*mock_adapter, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.AddTransportAdapter(mock_adapter));
}

TEST_F(TransportManagerImplTest, AddTransportAdapterSecondTime) {
  EXPECT_EQ(E_ADAPTER_EXISTS, mock_transport_manager_.AddTransportAdapter(mock_adapter_));
}

TEST_F(TransportManagerImplTest, ConnectDevice) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, ConnectDevice_DeviceNotHandled) {
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, mock_transport_manager_.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, ConnectDevice_DeviceNotConnected) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_INTERNAL_ERROR, mock_transport_manager_.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter_, DisconnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));

  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice_ConnectionFailed) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_INTERNAL_ERROR, mock_transport_manager_.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter_, DisconnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::FAIL));

  // Even with fail, we get Success
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice_DeviceNotConnected) {
  EXPECT_CALL(*mock_adapter_, DisconnectDevice(mac_address_)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, mock_transport_manager_.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, Disconnect) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .WillOnce(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, Disconnect_DisconnectionFailed) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .WillOnce(Return(TransportAdapter::FAIL));
  // Assert
  // Even with fail, we get Success
  EXPECT_EQ(E_SUCCESS,  mock_transport_manager_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, Disconnect_ConnectionNotExist) {
  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .Times(0);
  // Assert
  EXPECT_EQ(E_INVALID_HANDLE, mock_transport_manager_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, Disconnect_ConnectionDoesNotExists) {
  // Arrange
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_INVALID_HANDLE, mock_transport_manager_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, DisconnectForce_TMIsInitialized) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_, Disconnect(mac_address_, application_id_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  // Assert
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.DisconnectForce(connection_key_));
}

TEST_F(TransportManagerImplTest, SearchDevices_DeviceConnected) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, SearchDevices())
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.SearchDevices());

  HandleSearchDone();
}

TEST_F(TransportManagerImplTest, SearchDevices_DeviceNotFound) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, SearchDevices())
      .WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_ADAPTERS_FAIL, mock_transport_manager_.SearchDevices());
}

TEST_F(TransportManagerImplTest, SearchDevices_AdapterNotSupported) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, SearchDevices())
      .WillOnce(Return(TransportAdapter::NOT_SUPPORTED));
  EXPECT_EQ(E_ADAPTERS_FAIL, mock_transport_manager_.SearchDevices());
}

TEST_F(TransportManagerImplTest, SearchDevices_AdapterWithBadState) {
  HandleDeviceListUpdated();

  EXPECT_CALL(*mock_adapter_, SearchDevices())
      .WillOnce(Return(TransportAdapter::BAD_STATE));
  EXPECT_EQ(E_ADAPTERS_FAIL, mock_transport_manager_.SearchDevices());
}

TEST_F(TransportManagerImplTest, SendMessageToDevice) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(Return(TransportAdapter::OK));
#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(_));
#endif  // TELEMETRY_MONITOR
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.SendMessageToDevice(test_message_));
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_SendingFailed) {
  // Arrange
  HandleConnection();
#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(_));
#endif  // TELEMETRY_MONITOR
  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(Return(TransportAdapter::FAIL));

  EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageSendFailed(_, test_message_));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.SendMessageToDevice(test_message_));
#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StopRawMsg(_)).Times(0);
#endif  // TELEMETRY_MONITOR
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_StartTimeObserver) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(Return(TransportAdapter::OK));
#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(_));
#endif  // TELEMETRY_MONITOR
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.SendMessageToDevice(test_message_));
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_SendDone) {
  // Arrange
  HandleConnection();

  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(Return(TransportAdapter::OK));
#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(_));
#endif  // TELEMETRY_MONITOR
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.SendMessageToDevice(test_message_));
  HandleSendDone();

  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, SendMessageFailed_GetHandleSendFailed) {
  // Arrange
  HandleConnection();
#ifdef TELEMETRY_MONITOR
  EXPECT_CALL(mock_metric_observer_, StartRawMsg(_));
#endif  // TELEMETRY_MONITOR
  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_))
      .WillOnce(Return(TransportAdapter::FAIL));

  EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageSendFailed(_, test_message_));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.SendMessageToDevice(test_message_));

  HandleSendFailed();
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, RemoveDevice_DeviceWasAdded) {
  // Arrange
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.ConnectDevice(device_handle_));

  // Assert
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.RemoveDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, SetVisibilityOn_StartClientListening) {
  EXPECT_CALL(*mock_adapter_, StartClientListening())
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.Visibility(true));
}

TEST_F(TransportManagerImplTest, SetVisibilityOff_StopClientListening) {
  EXPECT_CALL(*mock_adapter_, StopClientListening())
      .WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.Visibility(false));
}

TEST_F(TransportManagerImplTest, StopTransportManager) {
  HandleDeviceListUpdated();
  EXPECT_CALL(*mock_adapter_, ConnectDevice(mac_address_))
      .WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.ConnectDevice(device_handle_));

  EXPECT_CALL(*mock_adapter_, DisconnectDevice(mac_address_))
      .WillRepeatedly(Return(TransportAdapter::OK));

  EXPECT_CALL(*mock_adapter_, Terminate());
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.Stop());
}

TEST_F(TransportManagerImplTest, Reinit) {
  EXPECT_CALL(*mock_adapter_, Terminate());
  EXPECT_CALL(*mock_adapter_, Init()).WillOnce(Return(TransportAdapter::OK));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.Reinit());
}

TEST_F(TransportManagerImplTest, Reinit_InitAdapterFailed) {
  EXPECT_CALL(*mock_adapter_, Terminate());
  EXPECT_CALL(*mock_adapter_, Init()).WillOnce(Return(TransportAdapter::FAIL));
  EXPECT_EQ(E_ADAPTERS_FAIL, mock_transport_manager_.Reinit());
}

TEST_F(TransportManagerImplTest, UpdateDeviceList_AddNewDevice) {
  device_list_.push_back(dev_info_.mac_address());

  EXPECT_CALL(*mock_adapter_, GetDeviceList()).WillOnce(Return(device_list_));
  EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
      .WillOnce(Return(dev_info_.name()));
  EXPECT_CALL(*mock_adapter_, GetConnectionType())
      .WillOnce(Return(dev_info_.connection_type()));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceAdded(dev_info_));

  mock_transport_manager_.UpdateDeviceList(mock_adapter_);
  device_list_.pop_back();
}

TEST_F(TransportManagerImplTest, UpdateDeviceList_RemoveDevice) {
  device_list_.push_back(dev_info_.mac_address());

  ::testing::InSequence seq;
  EXPECT_CALL(*mock_adapter_, GetDeviceList()).WillOnce(Return(device_list_));
  EXPECT_CALL(*mock_adapter_, GetConnectionType())
      .WillOnce(Return(dev_info_.connection_type()));
  EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
      .WillOnce(Return(dev_info_.name()));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceAdded(dev_info_));
  mock_transport_manager_.UpdateDeviceList(mock_adapter_);
  device_list_.pop_back();

  // Device list is empty now
  EXPECT_CALL(*mock_adapter_, GetDeviceList()).WillOnce(Return(device_list_));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceRemoved(dev_info_));
  mock_transport_manager_.UpdateDeviceList(mock_adapter_);
}

/*
 * Tests which check correct handling and receiving events
 */
TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_OnSearchDeviceDone) {
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE);

  TransportAdapterEvent test_event(type,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);

  EXPECT_CALL(*mock_transport_manager_listener_, OnScanDevicesFinished());

  mock_transport_manager_.TestReceiveEventFromDevice(test_event);
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_OnSearchDeviceFail) {
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_FAIL);

  TransportAdapterEvent test_event(type,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);

  EXPECT_CALL(*mock_transport_manager_listener_, OnScanDevicesFailed(_));

  mock_transport_manager_.TestReceiveEventFromDevice(test_event);
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_DeviceListUpdated) {
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DEVICE_LIST_UPDATED);

  TransportAdapterEvent test_event(type,
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

  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceFound(dev_info_));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceAdded(dev_info_));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceListUpdated(vector_dev_info));

  mock_transport_manager_.TestReceiveEventFromDevice(test_event);
  device_list_.pop_back();
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

TEST_F(TransportManagerImplTest, CheckReceiveFailedEvent) {
  // Arrange
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL);

  TransportAdapterEvent test_event(type,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
  // Check before act
  HandleConnection();
  // Act and Assert
  EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageReceiveFailed(_));
  mock_transport_manager_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, CheckUnexpectedDisconnect) {
  // Arrange
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_UNEXPECTED_DISCONNECT);

  TransportAdapterEvent test_event(type,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
  // Check before act
  HandleConnection();
  // Act and Assert
  EXPECT_CALL(*mock_transport_manager_listener_, OnUnexpectedDisconnect(connection_key_, _));
  mock_transport_manager_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, ConnectDevice_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, ConnectDevice(_)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.ConnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, DisconnectDevice_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, DisconnectDevice(_)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.DisconnectDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, Disconnect_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, Disconnect(_, _)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.Disconnect(connection_key_));
}

TEST_F(TransportManagerImplTest, DisconnectForce_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, Disconnect(_, _)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.DisconnectForce(connection_key_));
}

TEST_F(TransportManagerImplTest, DisconnectForce_ConnectionNotExist) {
  // SetUp does not add connections
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, Disconnect(_, _)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, mock_transport_manager_.DisconnectForce(connection_key_));
}

TEST_F(TransportManagerImplTest, Stop_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, DisconnectDevice(_)).Times(0);
  EXPECT_CALL(*mock_adapter_, Terminate()).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.Stop());
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, SendData(_, _, _)).Times(0);
  EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageSendFailed(_, _)).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.SendMessageToDevice(test_message_));
}

TEST_F(TransportManagerImplTest, SendMessageToDevice_ConnectionNotExist) {
  // SetUp does not add connections
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, SendData(_, _, _)).Times(0);
  EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageSendFailed(_, _)).Times(0);
  EXPECT_EQ(E_INVALID_HANDLE, mock_transport_manager_.SendMessageToDevice(test_message_));
}

TEST_F(TransportManagerImplTest, ReceiveEventFromDevice_TMIsNotInitialized) {
  // Arrange
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEARCH_DONE);
  TransportAdapterEvent test_event(
      type, NULL, mac_address_, application_id_, test_message_, error_);
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_transport_manager_listener_, OnScanDevicesFinished()).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.ReceiveEventFromDevice(test_event));
}

TEST_F(TransportManagerImplTest, RemoveDevice_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.RemoveDevice(device_handle_));
}

TEST_F(TransportManagerImplTest, Visibility_TMIsNotInitialized) {
  // Arrange
  const bool visible = true;
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, StartClientListening()).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.Visibility(visible));
}

TEST_F(TransportManagerImplTest, HandleMessage_ConnectionNotExist) {
  EXPECT_CALL(*mock_adapter_,
              SendData(mac_address_, application_id_, test_message_)).Times(0);
  EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageSendFailed(_, test_message_));

  mock_transport_manager_.TestHandle(test_message_);
  testing::Mock::AsyncVerifyAndClearExpectations(10000);
}

TEST_F(TransportManagerImplTest, SearchDevices_TMIsNotInitialized) {
  // Check before Act
  UninitializeTM();
  // Act and Assert
  EXPECT_CALL(*mock_adapter_, SearchDevices()).Times(0);
  EXPECT_EQ(E_TM_IS_NOT_INITIALIZED, mock_transport_manager_.SearchDevices());
}

TEST_F(TransportManagerImplTest, SetVisibilityOn_TransportAdapterNotSupported) {
  EXPECT_CALL(*mock_adapter_, StartClientListening())
      .WillOnce(Return(TransportAdapter::NOT_SUPPORTED));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.Visibility(true));
}

TEST_F(TransportManagerImplTest,
       SetVisibilityOff_TransportAdapterNotSupported) {
  EXPECT_CALL(*mock_adapter_, StopClientListening())
      .WillOnce(Return(TransportAdapter::NOT_SUPPORTED));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.Visibility(false));
}

TEST_F(TransportManagerImplTest,
       UpdateDeviceList_AddDevices_TwoTransportAdapters) {
  // Arrange
  TransportAdapterMock* second_mock_adapter = new TransportAdapterMock();
  device_list_.push_back(dev_info_.mac_address());
  // Check before Act
  EXPECT_CALL(*second_mock_adapter, AddListener(_));
  EXPECT_CALL(*second_mock_adapter, IsInitialised()).WillOnce(Return(true));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.AddTransportAdapter(second_mock_adapter));

  // Act and Assert
  EXPECT_CALL(*second_mock_adapter, GetDeviceList())
      .WillOnce(Return(device_list_));
  EXPECT_CALL(*second_mock_adapter, DeviceName(dev_info_.mac_address()))
      .WillOnce(Return(dev_info_.name()));
  EXPECT_CALL(*second_mock_adapter, GetConnectionType())
      .WillOnce(Return(dev_info_.connection_type()));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceAdded(dev_info_));
  mock_transport_manager_.UpdateDeviceList(second_mock_adapter);

  EXPECT_CALL(*mock_adapter_, GetDeviceList()).WillOnce(Return(device_list_));
  EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
      .WillOnce(Return(dev_info_.name()));
  EXPECT_CALL(*mock_adapter_, GetConnectionType())
      .WillOnce(Return(dev_info_.connection_type()));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceAdded(dev_info_));
  mock_transport_manager_.UpdateDeviceList(mock_adapter_);

  device_list_.pop_back();
}

TEST_F(TransportManagerImplTest,
       UpdateDeviceList_RemoveDevices_TwoTransportAdapters) {
  // Arrange
  TransportAdapterMock* second_mock_adapter = new TransportAdapterMock();
  device_list_.push_back(dev_info_.mac_address());
  // Check before Act
  EXPECT_CALL(*second_mock_adapter, AddListener(_));
  EXPECT_CALL(*second_mock_adapter, IsInitialised()).WillOnce(Return(true));
  EXPECT_EQ(E_SUCCESS, mock_transport_manager_.AddTransportAdapter(second_mock_adapter));

  // Act and Assert
  EXPECT_CALL(*second_mock_adapter, GetDeviceList())
      .WillOnce(Return(device_list_));
  EXPECT_CALL(*second_mock_adapter, DeviceName(dev_info_.mac_address()))
      .WillOnce(Return(dev_info_.name()));
  EXPECT_CALL(*second_mock_adapter, GetConnectionType())
      .WillOnce(Return(dev_info_.connection_type()));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceAdded(dev_info_));
  mock_transport_manager_.UpdateDeviceList(second_mock_adapter);

  EXPECT_CALL(*mock_adapter_, GetDeviceList()).WillOnce(Return(device_list_));
  EXPECT_CALL(*mock_adapter_, DeviceName(dev_info_.mac_address()))
      .WillOnce(Return(dev_info_.name()));
  EXPECT_CALL(*mock_adapter_, GetConnectionType())
      .WillOnce(Return(dev_info_.connection_type()));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceAdded(dev_info_));
  mock_transport_manager_.UpdateDeviceList(mock_adapter_);

  device_list_.pop_back();

  EXPECT_CALL(*second_mock_adapter, GetDeviceList())
      .WillOnce(Return(device_list_));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceRemoved(dev_info_));
  mock_transport_manager_.UpdateDeviceList(second_mock_adapter);

  EXPECT_CALL(*mock_adapter_, GetDeviceList()).WillOnce(Return(device_list_));
  EXPECT_CALL(*mock_transport_manager_listener_, OnDeviceRemoved(dev_info_));
  mock_transport_manager_.UpdateDeviceList(mock_adapter_);
}

TEST_F(TransportManagerImplTest,
       CheckEventOnDisconnectDone_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_DISCONNECT_DONE);

  TransportAdapterEvent test_event(type,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);

  // Act and Assert
  EXPECT_CALL(*mock_transport_manager_listener_, OnConnectionClosed(_)).Times(0);

  mock_transport_manager_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, CheckEventOnSendDone_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_SEND_DONE);

  TransportAdapterEvent test_event(type,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
#ifdef TELEMETRY_MONITOR
  // Act and Assert
  EXPECT_CALL(mock_metric_observer_, StopRawMsg(_));
#endif  // TELEMETRY_MONITOR
  EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageSend(_)).Times(0);

  mock_transport_manager_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, CheckEventOnReceivedDone_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_DONE);
  TransportAdapterEvent test_event(type,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
#ifdef TELEMETRY_MONITOR
  // Act and Assert
  EXPECT_CALL(mock_metric_observer_, StopRawMsg(_)).Times(0);
#endif  // TELEMETRY_MONITOR
  EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageReceived(_)).Times(0);
  mock_transport_manager_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest, CheckEventOnReceivedFail_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_RECEIVED_FAIL);
  TransportAdapterEvent test_event(type,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
  // Act and Assert
  EXPECT_CALL(*mock_transport_manager_listener_, OnTMMessageReceiveFailed(_)).Times(0);
  mock_transport_manager_.TestHandle(test_event);
}

TEST_F(TransportManagerImplTest,
       CheckEventOnUnexpectedDisconnect_ConnectionNotExist) {
  // SetUp does not add connections
  // Arrange
  const int type = static_cast<int>(
      TransportAdapterListenerImpl::EventTypeEnum::ON_UNEXPECTED_DISCONNECT);
  TransportAdapterEvent test_event(type,
                                   mock_adapter_,
                                   mac_address_,
                                   application_id_,
                                   test_message_,
                                   error_);
  // Act and Assert
  EXPECT_CALL(*mock_transport_manager_listener_, OnUnexpectedDisconnect(_, _)).Times(0);
  mock_transport_manager_.TestHandle(test_event);
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

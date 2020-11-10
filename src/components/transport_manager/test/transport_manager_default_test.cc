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

#include "transport_manager/transport_manager_default.h"
#include "gtest/gtest.h"
#include "resumption/last_state_wrapper_impl.h"
#include "resumption/mock_last_state.h"
#include "transport_manager/bt/mock_bluetooth_transport_adapter.h"
#include "transport_manager/cloud/mock_cloud_websocket_transport_adapter.h"
#include "transport_manager/mock_transport_manager_settings.h"
#include "transport_manager/tcp/mock_tcp_transport_adapter.h"
#include "transport_manager/transport_adapter/mock_device.h"
#include "transport_manager/transport_adapter/mock_transport_adapter_listener.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/usb/mock_usb_aoa_adapter.h"

namespace test {
namespace components {
namespace transport_manager_test {

using resumption_test::MockLastState;
using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const std::string kDeviceName = "name";
const std::string kDeviceAddress = "address";
const std::string kDeviceApplications = "applications";
const std::string kApplicationPort = "port";
const std::string kApplicationPortValue = "12345";
const std::string kApplicationRfcomm = "rfcomm_channel";
const std::string kApplicationRfcommValue = "7";
const std::string kTransportManager = "TransportManager";
const std::string kTcpAdapter = "TcpAdapter";
const std::string kBluetoothAdapter = "BluetoothAdapter";
const std::string kDevices = "devices";
const uint16_t kPort = 12345u;
const std::string kAddress = "127.0.0.1";
const std::string kServerCertPath = "server_certificate.crt";
const std::string kServerCACertPath = "ca-certificate.crt";
const std::string kWSServerKeyPathKey = "WSServerKeyPath";
const std::string kWSServerCACertPath = "WSServerCACertificatePath";
std::vector<uint8_t> kBTUUID = {0x93,
                                0x6D,
                                0xA0,
                                0x1F,
                                0x9A,
                                0xBD,
                                0x4D,
                                0x9D,
                                0x80,
                                0xC7,
                                0x02,
                                0xAF,
                                0x85,
                                0xC8,
                                0x22,
                                0xA8};
}  // namespace

class TestTransportManagerDefault : public ::testing::Test {
 public:
  TestTransportManagerDefault()
      : transport_manager_settings_()
      , mock_last_state_(std::make_shared<MockLastState>())
      , unique_tcp_dev_name_("unique_tcp_device_name")
      , dev_id_("device_id")
      , tcp_adapter_port_(1u)
      , network_interface_("test_iface")
      , last_state_wrapper_(std::make_shared<resumption::LastStateWrapperImpl>(
            mock_last_state_)) {}

  void SetUp() OVERRIDE {
    EXPECT_CALL(transport_manager_settings_,
                transport_manager_tcp_adapter_network_interface())
        .WillRepeatedly(ReturnRef(network_interface_));

    // Replace creation of real transport adapters by mock objects
    // to be able to check related function calls
    mock_bt_ta_ = new MockBluetoothTransportAdapter();
    mock_tcp_ta_ = new MockTCPTransportAdapter(
        tcp_adapter_port_, last_state_wrapper_, transport_manager_settings_);
    mock_usb_aoa_ta_ =
        new MockUsbAoaAdapter(last_state_wrapper_, transport_manager_settings_);
    mock_cloud_websocket_ta_ = new MockCloudWebsocketTransportAdapter(
        last_state_wrapper_, transport_manager_settings_);

    TransportAdapterFactory ta_factory;
#ifdef BLUETOOTH_SUPPORT
    ta_factory.ta_bluetooth_creator_ =
        [&](resumption::LastStateWrapperPtr& last_state_wrapper,
            const TransportManagerSettings& settings) {
          UNUSED(last_state_wrapper);
          UNUSED(settings);
          return mock_bt_ta_;
        };
#endif
    ta_factory.ta_tcp_creator_ =
        [&](const uint16_t port,
            resumption::LastStateWrapperPtr& last_state_wrapper,
            const TransportManagerSettings& settings) {
          UNUSED(port);
          UNUSED(last_state_wrapper);
          UNUSED(settings);
          return mock_tcp_ta_;
        };
#if defined(USB_SUPPORT)
    ta_factory.ta_usb_creator_ =
        [&](resumption::LastStateWrapperPtr& last_state_wrapper,
            const TransportManagerSettings& settings) {
          UNUSED(last_state_wrapper);
          UNUSED(settings);
          return mock_usb_aoa_ta_;
        };
#endif
#if defined(CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT)
    ta_factory.ta_cloud_creator_ =
        [&](resumption::LastStateWrapperPtr& last_state_wrapper,
            const TransportManagerSettings& settings) {
          UNUSED(last_state_wrapper);
          UNUSED(settings);
          return mock_cloud_websocket_ta_;
        };
#endif
    transport_manager_ = std::unique_ptr<TransportManagerDefault>(
        new TransportManagerDefault(transport_manager_settings_, ta_factory));
  }

  void ExpectationsSettings_TM(const bool use_last_state);
  void ExpectationsBluetooth_TA();
  void ExpectationsTCP_TA();
  void ExpectationsUSB_TA();
  void ExpectationsCloudWebsocket_TA();

 protected:
  MockTransportManagerSettings transport_manager_settings_;
  std::unique_ptr<TransportManagerDefault> transport_manager_;
  std::shared_ptr<MockLastState> mock_last_state_;
  Json::Value custom_dictionary_;
  const std::string unique_tcp_dev_name_;
  const std::string dev_id_;
  const uint16_t tcp_adapter_port_;
  std::string network_interface_;
  std::string dummy_parameter_;
  // Arrange necessary transport adapters mock objects
  MockBluetoothTransportAdapter* mock_bt_ta_;
  MockTCPTransportAdapter* mock_tcp_ta_;
  MockUsbAoaAdapter* mock_usb_aoa_ta_;
  MockCloudWebsocketTransportAdapter* mock_cloud_websocket_ta_;
  std::shared_ptr<resumption::LastStateWrapperImpl> last_state_wrapper_;
};

void TestTransportManagerDefault::ExpectationsSettings_TM(
    const bool use_last_state) {
  // Arrange TM Settings expectations
  Json::Value tcp_device;
  tcp_device[kDeviceName] = unique_tcp_dev_name_;
  tcp_device[kDeviceAddress] = kAddress;
  tcp_device[kDeviceApplications][0][kApplicationPort] = "1";
  Json::Value bluetooth_device;

  std::string unique_bt_dev_name("unique_bluetooth_device_name");
  bluetooth_device[kDeviceName] = unique_bt_dev_name;
  bluetooth_device[kDeviceAddress] = "AB:CD:EF:GH:IJ:KL";
  bluetooth_device[kDeviceApplications][0][kApplicationRfcomm] =
      kApplicationRfcommValue;
  custom_dictionary_[kTransportManager][kTcpAdapter][kDevices][0] = tcp_device;
  custom_dictionary_[kTransportManager][kBluetoothAdapter][kDevices][0] =
      bluetooth_device;
  ON_CALL(transport_manager_settings_, websocket_server_port())
      .WillByDefault(Return(kPort));
  ON_CALL(transport_manager_settings_, websocket_server_address())
      .WillByDefault(ReturnRef(kAddress));
  ON_CALL(transport_manager_settings_, ws_server_cert_path())
      .WillByDefault(ReturnRef(kServerCertPath));
  ON_CALL(transport_manager_settings_, ws_server_key_path())
      .WillByDefault(ReturnRef(kWSServerKeyPathKey));
  ON_CALL(transport_manager_settings_, ws_server_ca_cert_path())
      .WillByDefault(ReturnRef(kWSServerCACertPath));
  ON_CALL(*mock_last_state_, dictionary())
      .WillByDefault(Return(custom_dictionary_));
  ON_CALL(*mock_last_state_, get_dictionary())
      .WillByDefault(ReturnRef(custom_dictionary_));

  EXPECT_CALL(transport_manager_settings_, use_last_state())
      .WillRepeatedly(Return(use_last_state));
  EXPECT_CALL(transport_manager_settings_, transport_manager_tcp_adapter_port())
      .WillRepeatedly(Return(tcp_adapter_port_));

  EXPECT_CALL(transport_manager_settings_, bluetooth_uuid())
      .WillRepeatedly(Return(kBTUUID.data()));

  EXPECT_CALL(transport_manager_settings_, aoa_filter_manufacturer())
      .WillRepeatedly(ReturnRef(dummy_parameter_));
  EXPECT_CALL(transport_manager_settings_, aoa_filter_model_name())
      .WillRepeatedly(ReturnRef(dummy_parameter_));
  EXPECT_CALL(transport_manager_settings_, aoa_filter_description())
      .WillRepeatedly(ReturnRef(dummy_parameter_));
  EXPECT_CALL(transport_manager_settings_, aoa_filter_version())
      .WillRepeatedly(ReturnRef(dummy_parameter_));
  EXPECT_CALL(transport_manager_settings_, aoa_filter_uri())
      .WillRepeatedly(ReturnRef(dummy_parameter_));
  EXPECT_CALL(transport_manager_settings_, aoa_filter_serial_number())
      .WillRepeatedly(ReturnRef(dummy_parameter_));
}

void TestTransportManagerDefault::ExpectationsBluetooth_TA() {
  // Expectations for Mock of bluetooth transport adapter
#ifdef BLUETOOTH_SUPPORT
  EXPECT_CALL(*mock_bt_ta_, AddListener(_));
  EXPECT_CALL(*mock_bt_ta_, IsInitialised()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_bt_ta_, Init())
      .WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_CALL(*mock_bt_ta_, Terminate());
#endif
}

void TestTransportManagerDefault::ExpectationsTCP_TA() {
  // Expectations for Mock of TCP transport adapter
  EXPECT_CALL(*mock_tcp_ta_, AddListener(_));
  EXPECT_CALL(*mock_tcp_ta_, IsInitialised()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_tcp_ta_, Init())
      .WillRepeatedly(Return(TransportAdapter::OK));

  std::shared_ptr<MockDevice> mockdev =
      std::make_shared<MockDevice>(dev_id_, unique_tcp_dev_name_);
  EXPECT_CALL(*mock_tcp_ta_, FindDevice(unique_tcp_dev_name_))
      .WillRepeatedly(Return(mockdev));
  const int app_handle = 1;
  EXPECT_CALL(*mock_tcp_ta_, Connect(unique_tcp_dev_name_, app_handle))
      .Times(0);
  EXPECT_CALL(*mock_tcp_ta_, Terminate());
}

void TestTransportManagerDefault::ExpectationsUSB_TA() {
  // Expectations for Mock of USB transport adapter
#if defined(USB_SUPPORT)
  EXPECT_CALL(*mock_usb_aoa_ta_, AddListener(_));
  EXPECT_CALL(*mock_usb_aoa_ta_, IsInitialised()).WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_usb_aoa_ta_, Init())
      .WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_CALL(*mock_usb_aoa_ta_, Terminate());
#endif
}

void TestTransportManagerDefault::ExpectationsCloudWebsocket_TA() {
  // Expectations for Mock of Cloud Websocket transport adapter
#if defined(CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT)
  EXPECT_CALL(*mock_cloud_websocket_ta_, AddListener(_));
  EXPECT_CALL(*mock_cloud_websocket_ta_, IsInitialised())
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_cloud_websocket_ta_, Init())
      .WillRepeatedly(Return(TransportAdapter::OK));
  EXPECT_CALL(*mock_cloud_websocket_ta_, Terminate());
#endif
}

TEST_F(TestTransportManagerDefault, Init_LastStateNotUsed) {
  const bool use_last_state = false;
  ExpectationsSettings_TM(use_last_state);

  ExpectationsBluetooth_TA();
  ExpectationsTCP_TA();
  ExpectationsUSB_TA();
  ExpectationsCloudWebsocket_TA();

  // Act
  transport_manager_->Init(last_state_wrapper_);
  transport_manager_->Stop();
}

TEST_F(TestTransportManagerDefault, Init_LastStateUsed) {
  const bool use_last_state = true;
  ExpectationsSettings_TM(use_last_state);

  ExpectationsBluetooth_TA();
  ExpectationsTCP_TA();
  ExpectationsUSB_TA();
  ExpectationsCloudWebsocket_TA();

  // Act
  transport_manager_->Init(last_state_wrapper_);
  transport_manager_->Stop();
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

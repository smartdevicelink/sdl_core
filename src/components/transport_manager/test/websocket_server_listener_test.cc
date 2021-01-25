/*
 * Copyright (c) 2020, Ford Motor Company
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

#include "gmock/gmock.h"
#include "gtest/gtest.h"
#include "transport_manager/mock_transport_manager_settings.h"
#include "transport_manager/transport_adapter/mock_transport_adapter_controller.h"
#include "transport_manager/websocket_server/websocket_device.h"
#include "transport_manager/websocket_server/websocket_listener.h"
#include "utils/timer.h"
#include "utils/timer_task_impl.h"

#include <thread>
#include "transport_manager/websocket_server/websocket_sample_client.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnArg;
using ::testing::ReturnPointee;
using ::testing::ReturnRef;
using namespace ::transport_manager;
using namespace ::transport_manager::transport_adapter;

namespace {
const std::string kDefaultAddress = "127.0.0.1";
const std::string kDefaultCertPath = "";
const std::string kDefaultKeyPath = "";
const std::string kDefaultCACertPath = "";
const uint32_t kDefaultPort = 2021;
const std::string kDefaultPortStr = "2021";
const uint32_t kWrongPort = 1000;
const std::string kCACertPath = "./test_certs/ca-cert.pem";
const std::string kClientCertPath = "./test_certs/client-cert.pem";
const std::string kClientKeyPath = "./test_certs/client-key.pem";
const std::string kServerCert = "./test_certs/server-cert.pem";
const std::string kServerKey = "./test_certs/server-key.pem";
const std::string kCACert = "./test_certs/ca-cert.pem";
const std::string kDefaultDeviceName = "Web Engine";
const int kNumThreads = 2;
}  // namespace

class WebSocketListenerTest : public ::testing::Test {
 protected:
  MockTransportAdapterController mock_ta_controller_;
  MockTransportManagerSettings mock_tm_settings_;

 public:
  WebSocketListenerTest() {}
  ~WebSocketListenerTest() {}

  void SetUp() OVERRIDE {
    ON_CALL(mock_tm_settings_, websocket_server_address())
        .WillByDefault(ReturnRef(kDefaultAddress));
    ON_CALL(mock_tm_settings_, websocket_server_port())
        .WillByDefault(Return(kDefaultPort));
    ON_CALL(mock_tm_settings_, ws_server_cert_path())
        .WillByDefault(ReturnRef(kServerCert));
    ON_CALL(mock_tm_settings_, ws_server_key_path())
        .WillByDefault(ReturnRef(kServerKey));
    ON_CALL(mock_tm_settings_, ws_server_ca_cert_path())
        .WillByDefault(ReturnRef(kCACert));
    ON_CALL(mock_tm_settings_, wss_server_supported())
        .WillByDefault(Return(true));
  }

  std::shared_ptr<WebSocketDevice> CreateDevice(const bool secure_connection) {
    return std::make_shared<WebSocketDevice>(kDefaultDeviceName,
                                             kDefaultDeviceName);
  }
};

TEST_F(WebSocketListenerTest, StartListening_ClientConnect_SUCCESS) {
  ON_CALL(mock_tm_settings_, ws_server_cert_path())
      .WillByDefault(ReturnRef(kDefaultCertPath));
  ON_CALL(mock_tm_settings_, ws_server_key_path())
      .WillByDefault(ReturnRef(kDefaultKeyPath));
  ON_CALL(mock_tm_settings_, ws_server_ca_cert_path())
      .WillByDefault(ReturnRef(kDefaultCACertPath));
  ON_CALL(mock_tm_settings_, wss_server_supported())
      .WillByDefault(Return(false));

  const auto ws_listener = std::make_shared<WebSocketListener>(
      &mock_ta_controller_, mock_tm_settings_, kNumThreads);
  const auto ws_client =
      std::make_shared<WSSampleClient<WS> >(kDefaultAddress, kDefaultPortStr);

  EXPECT_CALL(mock_ta_controller_, ConnectDone(_, _));
  EXPECT_CALL(mock_ta_controller_, ConnectionCreated(_, _, _));
  EXPECT_CALL(mock_ta_controller_, GetWebEngineDevice())
      .WillOnce(Return(CreateDevice(false)));

  ws_listener->StartListening();
  EXPECT_TRUE(ws_client->Run());
  ws_client->Stop();
}

#ifdef ENABLE_SECURITY
TEST_F(WebSocketListenerTest, StartListening_ClientConnectSecure_SUCCESS) {
  const auto ws_listener = std::make_shared<WebSocketListener>(
      &mock_ta_controller_, mock_tm_settings_, kNumThreads);
  const SecurityParams params{kCACertPath, kClientCertPath, kClientKeyPath};
  const auto wss_client = std::make_shared<WSSampleClient<WSS> >(
      kDefaultAddress, kDefaultPortStr, params);

  EXPECT_CALL(mock_ta_controller_, ConnectDone(_, _));
  EXPECT_CALL(mock_ta_controller_, ConnectionCreated(_, _, _));
  EXPECT_CALL(mock_ta_controller_, GetWebEngineDevice())
      .WillOnce(Return(CreateDevice(true)));

  ws_listener->StartListening();
  EXPECT_TRUE(wss_client->Run());
  wss_client->Stop();
}

TEST_F(WebSocketListenerTest,
       StartListening_ClientConnectSecureInvalidCert_FAIL) {
  const auto ws_listener = std::make_shared<WebSocketListener>(
      &mock_ta_controller_, mock_tm_settings_, kNumThreads);
  const SecurityParams params{kCACertPath,
                              "./test_certs/invalid_cert.pem",
                              "./test_certs/invalid_key.pem"};
  const auto wss_client = std::make_shared<WSSampleClient<WSS> >(
      kDefaultAddress, kDefaultPortStr, params);

  EXPECT_CALL(mock_ta_controller_, ConnectDone(_, _));
  EXPECT_CALL(mock_ta_controller_, ConnectionCreated(_, _, _));
  EXPECT_CALL(mock_ta_controller_, GetWebEngineDevice())
      .WillOnce(Return(CreateDevice(true)));
  EXPECT_CALL(mock_ta_controller_, ConnectionAborted(_, _, _));

  ws_listener->StartListening();
  timer::Timer handshake_timer(
      "HandshakeTimer",
      new ::timer::TimerTaskImpl<WSSampleClient<WSS> >(
          wss_client.get(), &WSSampleClient<WSS>::OnHandshakeTimeout));
  handshake_timer.Start(3000, timer::kSingleShot);
  wss_client->Run();
  EXPECT_EQ(wss_client->IsHandshakeSuccessful(), false);
}

TEST_F(WebSocketListenerTest, StartListening_CertificateNotFound_Fail) {
  const std::string server_cert = "./test_certs/server-cert.pem";
  const std::string server_key = "./test_certs/server-key.pem";
  const std::string ca_cert = "./not_valid_path/ca-cert.pem";
  ON_CALL(mock_tm_settings_, ws_server_cert_path())
      .WillByDefault(ReturnRef(server_cert));
  ON_CALL(mock_tm_settings_, ws_server_key_path())
      .WillByDefault(ReturnRef(server_key));
  ON_CALL(mock_tm_settings_, ws_server_ca_cert_path())
      .WillByDefault(ReturnRef(ca_cert));

  const auto ws_listener_ = std::make_shared<WebSocketListener>(
      &mock_ta_controller_, mock_tm_settings_, kNumThreads);
  const auto ws_client_ =
      std::make_shared<WSSampleClient<WS> >(kDefaultAddress, kDefaultPortStr);

  EXPECT_EQ(TransportAdapter::Error::FAIL, ws_listener_->StartListening());
}

TEST_F(WebSocketListenerTest, StartListening_WrongConfig_FAIL) {
  const std::string server_cert = "./test_certs/server-cert.pem";

  ON_CALL(mock_tm_settings_, ws_server_cert_path())
      .WillByDefault(ReturnRef(server_cert));
  ON_CALL(mock_tm_settings_, ws_server_key_path())
      .WillByDefault(ReturnRef(kDefaultKeyPath));
  EXPECT_CALL(mock_tm_settings_, ws_server_ca_cert_path())
      .WillOnce(ReturnRef(kDefaultCACertPath));

  const auto ws_listener_ = std::make_shared<WebSocketListener>(
      &mock_ta_controller_, mock_tm_settings_, kNumThreads);
  const auto ws_client_ =
      std::make_shared<WSSampleClient<WS> >(kDefaultAddress, kDefaultPortStr);

  EXPECT_EQ(TransportAdapter::Error::FAIL, ws_listener_->StartListening());
}

TEST_F(WebSocketListenerTest, StartListening_BindToTheServerAddress_FAIL) {
  ON_CALL(mock_tm_settings_, ws_server_cert_path())
      .WillByDefault(ReturnRef(kDefaultCertPath));
  ON_CALL(mock_tm_settings_, ws_server_key_path())
      .WillByDefault(ReturnRef(kDefaultKeyPath));
  ON_CALL(mock_tm_settings_, wss_server_supported())
      .WillByDefault(Return(false));
  EXPECT_CALL(mock_tm_settings_, ws_server_ca_cert_path())
      .WillOnce(ReturnRef(kDefaultCACertPath));
  EXPECT_CALL(mock_tm_settings_, websocket_server_port())
      .WillOnce(Return(kWrongPort));

  const auto ws_listener_ = std::make_shared<WebSocketListener>(
      &mock_ta_controller_, mock_tm_settings_, kNumThreads);
  const auto ws_client_ =
      std::make_shared<WSSampleClient<WS> >(kDefaultAddress, kDefaultPortStr);

  EXPECT_EQ(TransportAdapter::Error::FAIL, ws_listener_->StartListening());
}

TEST_F(WebSocketListenerTest, StartListening_AcceptorIsOpen_SUCCESS) {
  ON_CALL(mock_tm_settings_, ws_server_cert_path())
      .WillByDefault(ReturnRef(kDefaultCertPath));
  ON_CALL(mock_tm_settings_, ws_server_key_path())
      .WillByDefault(ReturnRef(kDefaultKeyPath));
  ON_CALL(mock_tm_settings_, wss_server_supported())
      .WillByDefault(Return(false));

  EXPECT_CALL(mock_ta_controller_, ConnectDone(_, _));
  EXPECT_CALL(mock_ta_controller_, ConnectionCreated(_, _, _));
  EXPECT_CALL(mock_ta_controller_, GetWebEngineDevice())
      .WillOnce(Return(CreateDevice(false)));

  EXPECT_CALL(mock_tm_settings_, ws_server_ca_cert_path())
      .WillOnce(ReturnRef(kDefaultCACertPath));

  const auto ws_listener = std::make_shared<WebSocketListener>(
      &mock_ta_controller_, mock_tm_settings_, kNumThreads);
  const auto ws_client =
      std::make_shared<WSSampleClient<WS> >(kDefaultAddress, kDefaultPortStr);

  ws_listener->StartListening();
  ws_client->Run();
  EXPECT_EQ(TransportAdapter::Error::OK, ws_listener->StartListening());
  ws_client->Stop();
}
#endif

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

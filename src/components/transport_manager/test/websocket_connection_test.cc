/*
 * Copyright (c) 2019, Ford Motor Company
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
#include "resumption/last_state_impl.h"
#include "transport_manager/cloud/cloud_websocket_transport_adapter.h"
#include "transport_manager/cloud/sample_websocket_server.h"
#include "transport_manager/cloud/websocket_client_connection.h"
#include "transport_manager/transport_adapter/connection.h"
#include "transport_manager/transport_adapter/transport_adapter_impl.h"

#include "transport_manager/mock_transport_manager_settings.h"

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::_;
using ::testing::NiceMock;
using ::testing::NotNull;
using ::testing::Return;
using namespace ::transport_manager;
using namespace ::transport_manager::transport_adapter;

class WebsocketConnectionTest : public ::testing::Test {
 public:
  void StartWSServer() {
    ws_session = std::make_shared<WSSession>(kHost, kPort);
    ws_session->run();
  }

  void StartWSSServer() {
    wss_session =
        std::make_shared<WSSSession>(kHost, kPort, kCertificate, kPrivateKey);
    wss_session->run();
  }

 protected:
  WebsocketConnectionTest()
      : last_state_("app_storage_folder", "app_info_storage") {}

  ~WebsocketConnectionTest() {}
  void SetUp() OVERRIDE {
    app_handle = 0;
    ON_CALL(transport_manager_settings, use_last_state())
        .WillByDefault(Return(true));
  }

  NiceMock<MockTransportManagerSettings> transport_manager_settings;
  resumption::LastStateImpl last_state_;
  std::string dev_id;
  std::string uniq_id;
  int app_handle;
  std::string kHost = "127.0.0.1";
  uint16_t kPort = 8080;
  std::shared_ptr<WSSession> ws_session;
  std::shared_ptr<WSSSession> wss_session;
  // Sample certificate for localhost
  std::string kCertificate =
      "-----BEGIN "
      "CERTIFICATE-----\nMIIC/"
      "jCCAeagAwIBAgIJAIZjLucUID1mMA0GCSqGSIb3DQEBCwUAMBQxEjAQBgNV\nBAMMCTEyNy4"
      "wLjAuMTAeFw0xOTA1MDYxNDA1MzdaFw0yMjA1MDUxNDA1MzdaMBQx\nEjAQBgNVBAMMCTEyN"
      "y4wLjAuMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\nggEBALfE5Qhc2mHTIux30l7"
      "eHFjFLdzYeXE8vcaXKodalCG8EkRxojDOfUv+y2DV\nJzHAMiGxMFAEcSz71k+"
      "jFZwhNG8PLY0b36ChnUsrGkOSJWq3OKUFrg8KxO9At9dL\nJsa+"
      "R0N0D1bMoPYdpCi3m0b0q2ITHe56qKuTLTrIPia+"
      "qXGEVD7EoEhU9tnwlcwE\npsUURMXCn2+FhHyqN9wjFkldmu4k8U3OJOK4385L+"
      "4RJoIV8dsYawAMAf+"
      "WuxyWq\niPQTPxr8q33ZZm6z0XrgsgxHYCCsryx8L9Ub9Zwu0mie7eL63rYdr86gULvnF1bY"
      "\ncOunKFc71rBYFalbD6YYsre733kCAwEAAaNTMFEwHQYDVR0OBBYEFKW9ByUNG84Z\nYiSc"
      "hUrB7KV9FinZMB8GA1UdIwQYMBaAFKW9ByUNG84ZYiSchUrB7KV9FinZMA8G\nA1UdEwEB/"
      "wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAHYROS1EL2qnrwmAsR6c\nqo8yrk8qzLKt4os"
      "41nv7QMUQFIO+QLPE8SbH4fK1YOMMUJ4ppARQBDaIIR3UEhp1\nZHT/"
      "wGjK9qxcuQ1EXLyHOY0rxS5q57dYGxOyQo4v6IoLVMZ1ij+RJGPYI/"
      "2fDXs0\nbDldeJyC1voHmG6lqTN5nLG7Y3j9j6rtSqJyRz5muaecQNiFPQOM2OTp0rC4VeAF"
      "\ndejmTmLhUFVuHMzLF+"
      "bpzsN76GnnQquJy2jexzFoWgbEfFVLKuyhTHQAalRb4ccq\nXCIx1hecNDYRY3Sc2Gzv5qxk"
      "kWF8zqltT/0d5tx0JwN3k5nP4SlaEldFvD6BELxy\nVkU=\n-----END "
      "CERTIFICATE-----\n";
  // Sample private key
  std::string kPrivateKey =
      "-----BEGIN PRIVATE "
      "KEY-----"
      "\nMIIEvwIBADANBgkqhkiG9w0BAQEFAASCBKkwggSlAgEAAoIBAQC3xOUIXNph0yLs\nd9Je"
      "3hxYxS3c2HlxPL3GlyqHWpQhvBJEcaIwzn1L/"
      "stg1ScxwDIhsTBQBHEs+9ZP\noxWcITRvDy2NG9+"
      "goZ1LKxpDkiVqtzilBa4PCsTvQLfXSybGvkdDdA9WzKD2HaQo\nt5tG9KtiEx3ueqirky06y"
      "D4mvqlxhFQ+xKBIVPbZ8JXMBKbFFETFwp9vhYR8qjfc\nIxZJXZruJPFNziTiuN/OS/"
      "uESaCFfHbGGsADAH/lrsclqoj0Ez8a/Kt92WZus9F6\n4LIMR2AgrK8sfC/VG/"
      "WcLtJonu3i+t62Ha/"
      "OoFC75xdW2HDrpyhXO9awWBWpWw+m\nGLK3u995AgMBAAECggEBALR7lukgsIY1I+"
      "6UO7NadwKkHUfx/"
      "0u8eTIKkwU+a4+"
      "6\nM0KvS7idhCdYBYyDq6vL9DBs4mMkCbdBWxj5taaSYfnVMUqOHpKXR3Fk0rWcWk01\nx1c"
      "jffBeBk5oBGZY86trg3f3C32XGVq+"
      "f9RRhxooAA4hclsecnuX32sE8S2CQc4u\nm5HkpmUYXcFtmxaK6WT9mQDcsv/"
      "29IY+2AxX4U+"
      "yNywPrDZKDJPzom3v0FNb7GWr\nW41ALJrreCq7MBEJibEHjeXBjkhUgXjC9VLs+"
      "tFbdqYwDaPfuEIHSeUV6GNpkhvm\nKuBcRCjrmGMwHcqzGOIr4uokWXcXOR2oLRMT7qlrduU"
      "CgYEA3qP+lx0jiuhMaUrL\nhEAgHQP533jFnxSyMFiTn9Eg4uF6DguswJ7mUS11/"
      "8EhROthn8advoJe1dXpz3SP\nn6TSR/"
      "vxN6q46WsDspxtCrVBbF3R5m+FEgiZ+vnTsT3e52ecdskCkJ6/XjE8IU0B\nDR89DKtCZ/"
      "vN2uDWvhcuDQyz6l8CgYEA003gEPMJy9Z8mfnRxMMv1LuNi0gbFQWS\nkyUTVwzxZ63B50N9"
      "WQZoa8HBUftfPupK4YcLV7nxwE9WYxMSbceiXboTI/Pjj4QM\n2O+uKDQjGA441Bb2bH/"
      "AVOz+vhhAc2glMncI5UvjUA2gfiy+JcBUDbQNHR/ux4Nd\ntz/R1W0itScCgYEAmM/"
      "K0IGJgbALskFKCs3QvNmbycylJ5kW7KP/"
      "PzRU9CR6l+Vf\nfx4RSyp+"
      "0ToH0bwVV4sFflsRKIgYYPHKGnMQeaPtXp3pKRzwfslq9myOKQkEJrvo\nhAYvWdnbeM3Ujt"
      "PIyqcRAZ5UDxyP/"
      "vNRg2YriuSJQcHM+"
      "yxTeEmErCRJ4NUCgYEA\nlKh2GhaSbsjgcodyUAauMPEeXL4G55w7CbCM0bJ2Z+"
      "WzxFsT5bz/W8g9lMrPsHiu\n48nZbgeQkCaA9UTmszs+/Me7TZD5KO/"
      "TBhBhq0E662KeEoBxL9YU3uq5Mc3oEglU\nGhPquz7PlnNv1TTvNaoAuH9StonPuKewGAsbO"
      "hcE0wMCgYAjokPhXZi0z3YgwAEY\nCkSLMiHvAn+gd/pr7GIhvfR4MkxEFBoj1GxBJM/By4/"
      "tHYMJQCuxFVZqPYH/"
      "AWQ2\noVeuFoEjZXaGNV4SfkhNW7W+Btqt+aXT5+8F176+"
      "cvG4uex59XGWG52wKOgLFkmk\nJ00j679GO7h2Fpt4I9bml2VfWw==\n-----END "
      "PRIVATE KEY-----\n";
  std::string kIncorrectCertificate =
      "-----BEGIN "
      "CERTIFICATE-----\nMIIC/"
      "jCCAeagAwIBAgIJAIZjLucUID1mMA0GCSqGSIb3DQEBCwUAMBQxEjAQBgNV\nBAMMCTEyNy4"
      "wLjAuMTAeFw0xOTA1MDYxNDA1MzdaFw0yOjA1MDZxNDA1MzdaMBQx\nEjAQBgNVBAMMCTEyN"
      "y4wLjAuMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoC\nggEBALfE5Qhc2mHTIux30l7"
      "eHFjFLdzYeXE8vcaXKodalCG8EkRxojDOfUv+y2DV\nJzHAMiGxMFAEcSz71k+"
      "jFZwhNG8PLY0b36ChnUsrGkOSJWq3OKUFrg8KxO9At9dL\nJsa+"
      "R0N0D1bMoPYdpCi3m0b0q2ITHe56qKuTLTrIPia+"
      "qXGEVD7EoEhU9tnwlcwE\npsUURMXCn2+FhHyqN9wjFkldmu4k8U3OJOK4385L+"
      "4RJoIV8dsYawAMAf+"
      "WuxyWq\niPQTPxr8q33ZZm6z0XrgsgxHYCCsryx8L9Ub9Zwu0mie7eL63rYdr86gULvnF1bY"
      "\ncOunKFc71rBYFalbD6YYsre733kCAwEAAaNTMFEwHQYDVR0OBBYEFKW9ByUNG84Z\nYiSc"
      "hUrB7KV9FinZMB8GA1UdIwQYMBaAFKW9ByUNG84ZYiSchUrB7KV9FinZMA8G\nA1UdEwEB/"
      "wQFMAMBAf8wDQYJKoZIhvcNAQELBQADggEBAHYROS1EL2qnrwmAsR6c\nqo8yrk8qzLKt4os"
      "41nv7QMUQFIO+QLPE8SbH4fK1YOMMUJ4ppARQBDaIIR3UEhp1\nZHT/"
      "wGjK9qxcuQ1EXLyHOY0rxS5q57dYGxOyQo4v6IoLVMZ1ij+RJGPYI/"
      "2fDXs0\nbDldeJyC1voHmG6lqTN5nLG7Y3j9j6rtSqJyRz5muaecQNiFPQOM2OTp0rC4VeAF"
      "\ndejmTmLhUFVuHMzLF+"
      "bpzsN76GnnQquJy2jexzFoWgbEfFVLKuyhTHQAalRb4ccq\nXCIx1hecNDYRY3Sc2Gzv5qxk"
      "kWF8zqltT/0d5tx0JwN3k5nP4SlaEldFvD6BELxy\nVkU=\n-----END "
      "CERTIFICATE-----\n";
};

TEST_F(WebsocketConnectionTest, WSConnection_SUCCESS) {
  dev_id = "ws://" + kHost + ":" + std::to_string(kPort) + "/";
  uniq_id = dev_id;

  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = dev_id,
      .certificate = "no cert",
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSServer, this);
  sleep(1);

  // Start client
  CloudWebsocketTransportAdapter cta(last_state_, transport_manager_settings);
  cta.SetAppCloudTransportConfig(uniq_id, properties);

  TransportAdapterImpl* ta_cloud = &cta;
  ta_cloud->CreateDevice(uniq_id);

  auto connection = cta.FindPendingConnection(dev_id, app_handle);

  EXPECT_NE(connection, nullptr);

  std::shared_ptr<WebsocketClientConnection> ws_connection =
      std::dynamic_pointer_cast<WebsocketClientConnection>(connection);

  EXPECT_NE(ws_connection.use_count(), 0);

  // Check websocket connection
  TransportAdapter::Error ret_code = ws_connection->Start();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop client
  ret_code = ws_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  ws_session->stop();
  t1.join();
}

TEST_F(WebsocketConnectionTest, WSSConnection_SUCCESS) {
  dev_id = "wss://" + kHost + ":" + std::to_string(kPort) + "/";
  uniq_id = dev_id;
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = dev_id,
      .certificate = kCertificate,
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WSS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSSServer, this);
  sleep(1);

  // Start client
  CloudWebsocketTransportAdapter cta(last_state_, transport_manager_settings);
  cta.SetAppCloudTransportConfig(uniq_id, properties);

  TransportAdapterImpl* ta_cloud = &cta;
  ta_cloud->CreateDevice(uniq_id);

  auto connection = cta.FindPendingConnection(dev_id, app_handle);

  EXPECT_NE(connection, nullptr);

  std::shared_ptr<WebsocketClientConnection> wss_connection =
      std::dynamic_pointer_cast<WebsocketClientConnection>(connection);

  EXPECT_NE(wss_connection.use_count(), 0);

  // Check websocket connection
  TransportAdapter::Error ret_code = wss_connection->Start();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop client
  ret_code = wss_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  wss_session->stop();
  t1.join();
}

TEST_F(WebsocketConnectionTest, WSSConnection_FAILURE_IncorrectCert) {
  dev_id = "wss://" + kHost + ":" + std::to_string(kPort) + "/";
  uniq_id = dev_id;
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = dev_id,
      .certificate = kIncorrectCertificate,
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WSS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartWSSServer, this);
  sleep(1);

  // Start client
  CloudWebsocketTransportAdapter cta(last_state_, transport_manager_settings);
  cta.SetAppCloudTransportConfig(uniq_id, properties);

  TransportAdapterImpl* ta_cloud = &cta;
  ta_cloud->CreateDevice(uniq_id);

  auto connection = cta.FindPendingConnection(dev_id, app_handle);

  EXPECT_NE(connection, nullptr);

  std::shared_ptr<WebsocketClientConnection> wss_connection =
      std::dynamic_pointer_cast<WebsocketClientConnection>(connection);

  EXPECT_NE(wss_connection.use_count(), 0);

  // Check websocket connection
  TransportAdapter::Error ret_code = wss_connection->Start();
  EXPECT_EQ(TransportAdapter::FAIL, ret_code);

  // Stop client
  ret_code = wss_connection->Disconnect();
  EXPECT_EQ(TransportAdapter::OK, ret_code);

  // Stop server thread
  wss_session->stop();
  t1.join();
}
}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

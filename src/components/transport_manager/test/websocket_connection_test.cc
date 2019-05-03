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
  void StartServer() {
    ws_session = std::make_shared<WSSession>(kHost, kPort);
    ws_session->run();
  }

 protected:
  WebsocketConnectionTest()
      : last_state_("app_storage_folder", "app_info_storage") {}

  ~WebsocketConnectionTest() {}
  void SetUp() OVERRIDE {
    dev_id = "ws://" + kHost + ":" + std::to_string(kPort) + "/";
    uniq_id = dev_id;
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
};

TEST_F(WebsocketConnectionTest, WSConnection) {
  transport_manager::transport_adapter::CloudAppProperties properties{
      .endpoint = dev_id,
      .certificate = "no cert",
      .enabled = true,
      .auth_token = "auth_token",
      .cloud_transport_type = "WS",
      .hybrid_app_preference = "CLOUD"};

  // Start server
  std::thread t1(&WebsocketConnectionTest::StartServer, this);
  sleep(1);

  // Start client
  CloudWebsocketTransportAdapter cta(last_state_, transport_manager_settings);
  cta.SetAppCloudTransportConfig(uniq_id, properties);

  TransportAdapterImpl* ta_cloud = &cta;
  ta_cloud->CreateDevice(uniq_id);

  auto connection = cta.FindPendingConnection(dev_id, app_handle);
  std::cout << "Pending connection: " << (connection != NULL) << std::endl;

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

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

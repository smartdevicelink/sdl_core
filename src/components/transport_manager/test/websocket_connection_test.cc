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

#include "gtest/gtest.h"

#include <memory>
#include <string>

#include "resumption/last_state_impl.h"
#include "transport_manager/websocket_server/websocket_connection.h"
#include "transport_manager/websocket_server/websocket_session.h"

#include "transport_manager/mock_transport_manager_settings.h"
#include "transport_manager/transport_adapter/mock_transport_adapter_controller.h"

namespace {
const std::string kHost = "127.0.0.1";
const uint16_t kPort = 8080;
const std::string kPath = "/folder/file.html/";
const std::uint32_t kConnectionKey = 1u;
const std::uint32_t kProtocolVersion = 5u;
const std::string kDeviceUid_ = "deviceUID";
const transport_manager::ApplicationHandle kAppHandle = 12345u;
}  // namespace

namespace test {
namespace components {
namespace transport_manager_test {

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;

using protocol_handler::RawMessagePtr;
using protocol_handler::ServiceType;

using namespace ::transport_manager;
using namespace ::transport_manager::transport_adapter;

class WebsocketNotSecureSessionConnectionTest : public testing::Test {
 public:
  WebsocketNotSecureSessionConnectionTest()
      : websocket_connection_(
            std::make_shared<WebSocketConnection<WebSocketSession<> > >(
                kDeviceUid_,
                kAppHandle,
                boost::asio::ip::tcp::socket(i_co_),
                &mock_transport_adapter_ctrl_))

      , last_state_("test_app_folder", "test_app_info_storage") {}

  void SetUp() OVERRIDE {
    ON_CALL(mock_transport_manager_settings_, use_last_state())
        .WillByDefault(Return(true));
  }

  RawMessagePtr CreateDefaultRawMessage() {
    const uint32_t data_sending_size = 3u;
    unsigned char data_sending[data_sending_size] = {0x20, 0x07, 0x01};
    RawMessagePtr raw_message_ptr(
        new ::protocol_handler::RawMessage(kConnectionKey,
                                           kProtocolVersion,
                                           data_sending,
                                           data_sending_size,
                                           false,
                                           ServiceType::kAudio));
    return raw_message_ptr;
  }

 protected:
  boost::asio::io_context i_co_;
  std::shared_ptr<WebSocketConnection<WebSocketSession<> > >
      websocket_connection_;
  NiceMock<MockTransportAdapterController> mock_transport_adapter_ctrl_;
  NiceMock<MockTransportManagerSettings> mock_transport_manager_settings_;
  resumption::LastStateImpl last_state_;
};

TEST_F(WebsocketNotSecureSessionConnectionTest, Disconnect_SUCCESS) {
  websocket_connection_->Run();
  EXPECT_CALL(mock_transport_adapter_ctrl_,
              DisconnectDone(kDeviceUid_, kAppHandle))
      .Times(1);

  auto error = websocket_connection_->Disconnect();

  ASSERT_EQ(TransportAdapter::Error::OK, error);
}

TEST_F(WebsocketNotSecureSessionConnectionTest, SecondDisconnect_UNSUCCESS) {
  websocket_connection_->Run();
  EXPECT_CALL(mock_transport_adapter_ctrl_,
              DisconnectDone(kDeviceUid_, kAppHandle))
      .Times(1);

  auto result_error = websocket_connection_->Disconnect();

  ASSERT_EQ(TransportAdapter::Error::OK, result_error);

  EXPECT_CALL(mock_transport_adapter_ctrl_,
              DisconnectDone(kDeviceUid_, kAppHandle))
      .Times(0);

  result_error = websocket_connection_->Disconnect();

  ASSERT_EQ(TransportAdapter::Error::BAD_STATE, result_error);
}

TEST_F(WebsocketNotSecureSessionConnectionTest, SUCCESS_SendData) {
  auto message = CreateDefaultRawMessage();

  auto error = websocket_connection_->SendData(message);

  ASSERT_EQ(TransportAdapter::Error::OK, error);
}

TEST_F(WebsocketNotSecureSessionConnectionTest, UNSUCCESS_SendData_BAD_STATE) {
  websocket_connection_->Run();
  auto message = CreateDefaultRawMessage();

  auto disconnect_error = websocket_connection_->Disconnect();

  auto send_data_error = websocket_connection_->SendData(message);

  ASSERT_EQ(TransportAdapter::Error::OK, disconnect_error);
  ASSERT_EQ(TransportAdapter::Error::BAD_STATE, send_data_error);
}

TEST_F(WebsocketNotSecureSessionConnectionTest, DataReceive_SUCCESS) {
  auto message = CreateDefaultRawMessage();

  EXPECT_CALL(mock_transport_adapter_ctrl_,
              DataReceiveDone(kDeviceUid_, kAppHandle, _))
      .Times(1);

  websocket_connection_->DataReceive(message);
}

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

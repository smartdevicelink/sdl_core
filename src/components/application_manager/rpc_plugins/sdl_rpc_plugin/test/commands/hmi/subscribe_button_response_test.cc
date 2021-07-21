/*
 * Copyright (c) 2021, Ford Motor Company
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

#include "application_manager/mock_event_dispatcher.h"
#include "gtest/gtest.h"
#include "hmi/subscribe_button_response.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace subscribe_button_response {

namespace am = ::application_manager;
using application_manager::ExpiredButtonRequestData;
using application_manager::commands::ResponseFromHMI;
using sdl_rpc_plugin::commands::hmi::SubscribeButtonResponse;
using ::test::components::event_engine_test::MockEventDispatcher;
typedef std::shared_ptr<ResponseFromHMI> ResponseFromHMIPtr;

namespace {
const uint32_t kCorrelationId = 2u;
const uint32_t kAppId = 1u;
const hmi_apis::Common_ButtonName::eType kButtonName =
    hmi_apis::Common_ButtonName::SEEKLEFT;
const hmi_apis::FunctionID::eType kFunctionID =
    hmi_apis::FunctionID::Buttons_UnsubscribeButton;
}  // namespace

class HMISubscribeButtonResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  HMISubscribeButtonResponseTest() {
    ON_CALL(app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(event_dispatcher_));
  }

  MessageSharedPtr CreateCommandMsg() {
    MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
    (*command_msg)[strings::params][strings::correlation_id] = kCorrelationId;
    (*command_msg)[am::strings::params][am::hmi_response::code] =
        hmi_apis::Common_Result::SUCCESS;

    return command_msg;
  }

  MockEventDispatcher event_dispatcher_;
};

TEST_F(HMISubscribeButtonResponseTest,
       RUN_ProcessExpiredRequest_SendUnsubscribeRequestToHMI) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  ResponseFromHMIPtr command(
      CreateCommand<SubscribeButtonResponse>(command_msg));

  ExpiredButtonRequestData expired_data;
  expired_data.app_id_ = kAppId;
  expired_data.button_name_ = kButtonName;
  utils::Optional<ExpiredButtonRequestData> expired_data_opt = expired_data;

  ON_CALL(app_mngr_, GetExpiredButtonRequestData(kCorrelationId))
      .WillByDefault(Return(expired_data_opt));
  ON_CALL(mock_message_helper_,
          CreateButtonSubscriptionHandlingRequestToHmi(
              kAppId, kButtonName, kFunctionID, _))
      .WillByDefault(Return(command_msg));
  EXPECT_CALL(event_dispatcher_, raise_event(_)).Times(0);
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(command_msg));

  command->Run();
}

TEST_F(
    HMISubscribeButtonResponseTest,
    RUN_ProcessUnsuccessfulExpiredRequest_EventNotRaisedUnsubscribeRequestNotSent) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  (*command_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::GENERIC_ERROR;

  ResponseFromHMIPtr command(
      CreateCommand<SubscribeButtonResponse>(command_msg));

  ExpiredButtonRequestData expired_data;
  expired_data.app_id_ = kAppId;
  expired_data.button_name_ = kButtonName;
  utils::Optional<ExpiredButtonRequestData> expired_data_opt = expired_data;

  ON_CALL(app_mngr_, GetExpiredButtonRequestData(kCorrelationId))
      .WillByDefault(Return(expired_data_opt));
  EXPECT_CALL(event_dispatcher_, raise_event(_)).Times(0);
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_)).Times(0);

  command->Run();
}

TEST_F(HMISubscribeButtonResponseTest, RUN_NoExpiredRequests_EventRaised) {
  MessageSharedPtr command_msg = CreateCommandMsg();
  ResponseFromHMIPtr command(
      CreateCommand<SubscribeButtonResponse>(command_msg));

  ON_CALL(app_mngr_, GetExpiredButtonRequestData(kCorrelationId))
      .WillByDefault(Return(utils::Optional<ExpiredButtonRequestData>::EMPTY));

  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_)).Times(0);
  EXPECT_CALL(event_dispatcher_, raise_event(_));

  command->Run();
}

}  // namespace subscribe_button_response
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

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

#include <stdint.h>
#include <string>
#include <set>

#include "application_manager/commands/mobile/scrollable_message_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace scrollable_message_request {

namespace am = application_manager;
namespace hmi_response = am::hmi_response;
namespace mobile_result = mobile_apis::Result;
namespace am = ::application_manager;

using am::commands::ScrollableMessageRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Eq;
using ::testing::Ref;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;

using namespace am::strings;
using test::components::policy_test::MockPolicyHandlerInterface;

namespace {
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const uint32_t kTimeOut = 30000u;
const uint32_t kCorrelationId = 10u;
const uint32_t kFunctionID = 3u;
}  // namespace

class ScrollableMessageRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ScrollableMessageRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {}
  MockMessageHelper& mock_message_helper_;
  typedef TypeIf<kMocksAreNice,
                 NiceMock<application_manager_test::MockHMICapabilities>,
                 application_manager_test::MockHMICapabilities>::Result
      MockHMICapabilities;
  sync_primitives::Lock lock_;

  MessageSharedPtr CreateFullParamsUISO() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    smart_objects::SmartObject menu_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    menu_params[am::strings::position] = 10;
    menu_params[am::strings::menu_name] = "LG";

    smart_objects::SmartObject msg_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    msg_params[am::strings::cmd_id] = kCmdId;
    msg_params[am::strings::menu_params] = menu_params;
    msg_params[am::strings::app_id] = kAppId;
    msg_params[am::strings::cmd_icon] = 1;
    msg_params[am::strings::cmd_icon][am::strings::value] = "10";
    (*msg)[am::strings::msg_params] = msg_params;

    return msg;
  }

  void SetUp() OVERRIDE {
    mock_app_ = CreateMockApp();
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    command_ = CreateCommand<ScrollableMessageRequest>(msg_);
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  MockPolicyHandlerInterface mock_policy_handler_;
  MockAppPtr mock_app_;
  MessageSharedPtr msg_;
  SharedPtr<ScrollableMessageRequest> command_;
};

typedef ScrollableMessageRequestTest::MockHMICapabilities MockHMICapabilities;

TEST_F(ScrollableMessageRequestTest, OnEvent_UI_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_ui = CreateFullParamsUISO();
  (*msg_ui)[am::strings::params][am::strings::connection_key] = kConnectionKey;

  utils::SharedPtr<ScrollableMessageRequest> command =
      CreateCommand<ScrollableMessageRequest>(msg_ui);

  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  ON_CALL(*mock_app, app_id()).WillByDefault(Return(kConnectionKey));

  MockHMICapabilities hmi_capabilities;
  ON_CALL(app_mngr_, hmi_capabilities())
      .WillByDefault(ReturnRef(hmi_capabilities));
  ON_CALL(hmi_capabilities, is_ui_cooperating()).WillByDefault(Return(true));

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[am::strings::msg_params][am::strings::info] = "info1";

  Event event(hmi_apis::FunctionID::UI_ScrollableMessage);
  event.set_smart_object(*msg);

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->on_event(event);

  EXPECT_EQ((*ui_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            true);
  EXPECT_EQ(
      (*ui_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
  if ((*ui_command_result)[am::strings::msg_params].keyExists(
          am::strings::info)) {
    EXPECT_FALSE(
        (*ui_command_result)[am::strings::msg_params][am::strings::info]
            .asString()
            .empty());
  }
  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ScrollableMessageRequestTest, Init_CorrectTimeout_SUCCESS) {
  (*msg_)[msg_params][timeout] = kTimeOut;
  (*msg_)[msg_params][interaction_mode] =
      mobile_apis::InteractionMode::MANUAL_ONLY;
  EXPECT_EQ(kDefaultTimeout_, command_->default_timeout());
  command_->Init();
  EXPECT_EQ(kTimeOut, command_->default_timeout());
}

TEST_F(ScrollableMessageRequestTest, Init_CorrectTimeout_UNSUCCESS) {
  (*msg_)[msg_params][interaction_mode] =
      mobile_apis::InteractionMode::MANUAL_ONLY;
  EXPECT_EQ(kDefaultTimeout_, command_->default_timeout());
  command_->Init();
  EXPECT_EQ(kTimeOut, command_->default_timeout());
}

TEST_F(ScrollableMessageRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));
  command_->Run();
}

TEST_F(ScrollableMessageRequestTest,
       Run_SoftButtonProcessingResultWrogParameters_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));
  const mobile_apis::Result::eType processing_result =
      mobile_apis::Result::ABORTED;
  smart_objects::SmartObject& msg_params = (*msg_)[am::strings::msg_params];
  EXPECT_CALL(
      mock_message_helper_,
      ProcessSoftButtons(
          msg_params, Eq(mock_app_), Ref(mock_policy_handler_), Ref(app_mngr_)))
      .WillOnce(Return(processing_result));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command_)));
  EXPECT_EQ(processing_result,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][result_code].asInt()));
}

TEST_F(ScrollableMessageRequestTest, Run_SoftButtonProcessingResult_SUCCESS) {
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app_));
  smart_objects::SmartObject& msg_params = (*msg_)[am::strings::msg_params];
  (*msg_)[am::strings::params][am::strings::function_id] = kFunctionID;
  (*msg_)[am::strings::msg_params][am::strings::soft_buttons][0]
         [am::strings::soft_button_id] = 0;
  EXPECT_CALL(
      mock_message_helper_,
      ProcessSoftButtons(
          msg_params, Eq(mock_app_), Ref(mock_policy_handler_), Ref(app_mngr_)))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(mock_message_helper_, SubscribeApplicationToSoftButton(_, _, _));

  MessageSharedPtr result_msg(CatchHMICommandResult(CallRun(*command_)));
  EXPECT_EQ(hmi_apis::FunctionID::UI_ScrollableMessage,
            static_cast<hmi_apis::FunctionID::eType>(
                (*result_msg)[params][function_id].asInt()));
}

TEST_F(ScrollableMessageRequestTest, OnEvent_ReceivedUnknownEvent_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  command_->on_event(event);
}

TEST_F(ScrollableMessageRequestTest,
       OnEvent_ReceivedUIOnResetTimeoutEvent_SUCCESS) {
  (*msg_)[params][connection_key] = kConnectionKey;
  (*msg_)[params][correlation_id] = kCorrelationId;
  EXPECT_CALL(
      app_mngr_,
      updateRequestTimeout(kConnectionKey, kCorrelationId, kDefaultTimeout_));
  Event event(hmi_apis::FunctionID::UI_OnResetTimeout);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(ScrollableMessageRequestTest,
       DISABLED_OnEvent_ReceivedUIScrollableMessage_SUCCESS) {
  (*msg_)[params][hmi_response::code] = hmi_apis::Common_Result::SUCCESS;

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _));
  Event event(hmi_apis::FunctionID::UI_ScrollableMessage);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(ScrollableMessageRequestTest,
       DISABLED_OnEvent_UnsupportedRCAndUICoop_SUCCESS) {
  (*msg_)[params][hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_apis::Result::UNSUPPORTED_RESOURCE), _));
  Event event(hmi_apis::FunctionID::UI_ScrollableMessage);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

}  // namespace scrollable_message_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests

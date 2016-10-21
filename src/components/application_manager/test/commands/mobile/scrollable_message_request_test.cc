/*

 Copyright (c) 2016, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <string>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/event_engine/event.h"
#include "mobile/scrollable_message_request.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace scrollable_message_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Eq;
using ::testing::Ref;
using am::commands::ScrollableMessageRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using test::components::policy_test::MockPolicyHandlerInterface;
using namespace am::strings;

namespace mobile_result = mobile_apis::Result;
namespace am = ::application_manager;
namespace hmi_response = am::hmi_response;

namespace {
const uint32_t kTimeOut = 30000u;
const uint32_t kCorrelationId = 10u;
const uint32_t kConnectionKey = 5u;
const uint32_t kFunctionID = 3u;
}

typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

class ScrollableMessageRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ScrollableMessageRequestTest()
      : msg_(CreateMessage(smart_objects::SmartType_Map)) {
    (*msg_)[msg_params][soft_buttons] = 0;
  }

 protected:
  void SetUp() OVERRIDE {
    mock_app_ = CreateMockApp();
    InitGetters();
    command_ = CreateCommand<ScrollableMessageRequest>(msg_);
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void InitGetters() {
    ON_CALL(mock_app_manager_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
  }

  MockPolicyHandlerInterface mock_policy_handler_;
  MockAppPtr mock_app_;
  MessageSharedPtr msg_;
  SharedPtr<ScrollableMessageRequest> command_;
};

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
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));
  command_->Run();
}

TEST_F(ScrollableMessageRequestTest,
       Run_SoftButtonProcessingResultWrogParameters_UNSUCCESS) {
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(mock_app_));
  const mobile_apis::Result::eType processing_result =
      mobile_apis::Result::ABORTED;
  smart_objects::SmartObject& msg_params = (*msg_)[am::strings::msg_params];
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons(msg_params,
                                 Eq(mock_app_),
                                 Ref(mock_policy_handler_),
                                 Ref(mock_app_manager_)))
      .WillOnce(Return(processing_result));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command_)));
  EXPECT_EQ(processing_result,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][result_code].asInt()));
}

TEST_F(ScrollableMessageRequestTest, Run_SoftButtonProcessingResult_SUCCESS) {
  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(mock_app_));
  smart_objects::SmartObject& msg_params = (*msg_)[am::strings::msg_params];
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons(msg_params,
                                 Eq(mock_app_),
                                 Ref(mock_policy_handler_),
                                 Ref(mock_app_manager_)))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_message_helper_, SubscribeApplicationToSoftButton(_, _, _));
  MessageSharedPtr result_msg(CatchHMICommandResult(CallRun(*command_)));
  EXPECT_EQ(hmi_apis::FunctionID::UI_ScrollableMessage,
            static_cast<hmi_apis::FunctionID::eType>(
                (*result_msg)[params][function_id].asInt()));
}

TEST_F(ScrollableMessageRequestTest, OnEvent_ReceivedUnknownEvent_UNSUCCESS) {
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  command_->on_event(event);
}

TEST_F(ScrollableMessageRequestTest,
       OnEvent_ReceivedUIOnResetTimeoutEvent_SUCCESS) {
  (*msg_)[params][connection_key] = kConnectionKey;
  (*msg_)[params][correlation_id] = kCorrelationId;
  EXPECT_CALL(
      mock_app_manager_,
      updateRequestTimeout(kConnectionKey, kCorrelationId, kDefaultTimeout_));
  Event event(hmi_apis::FunctionID::UI_OnResetTimeout);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(ScrollableMessageRequestTest,
       OnEvent_ReceivedUIScrollableMessage_SUCCESS) {
  (*msg_)[params][hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  MockHMICapabilities mock_hmi_capabilities;
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities));
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _));
  Event event(hmi_apis::FunctionID::UI_ScrollableMessage);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(ScrollableMessageRequestTest, OnEvent_UnsupportedRCAndUICoop_SUCCESS) {
  (*msg_)[params][hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  MockHMICapabilities mock_hmi_capabilities;
  EXPECT_CALL(mock_app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities));
  EXPECT_CALL(mock_hmi_capabilities, is_ui_cooperating())
      .WillOnce(Return(true));
  EXPECT_CALL(
      mock_app_manager_,
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
}  // namespace test

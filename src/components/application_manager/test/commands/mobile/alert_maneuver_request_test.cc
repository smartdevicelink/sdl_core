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

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/event_engine/event.h"
#include "mobile/alert_maneuver_request.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace alert_maneuver_request {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::Eq;
using ::testing::Ref;
namespace am = ::application_manager;
using am::commands::AlertManeuverRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using test::components::policy_test::MockPolicyHandlerInterface;
using namespace am::strings;

typedef SharedPtr<AlertManeuverRequest> CommandPtr;

namespace {
const uint32_t kCorrelationId = 10u;
const uint32_t kConnectionKey = 5u;
const uint32_t kFunctionID = 3u;
}

class AlertManeuverRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  AlertManeuverRequestTest()
      : msg_(CreateMessage(smart_objects::SmartType_Map)) {
    (*msg_)[msg_params][soft_buttons] = 0;
  }

 protected:
  void SetUp() OVERRIDE {
    mock_app_ = CreateMockApp();
    InitGetters();
    command_ = CreateCommand<AlertManeuverRequest>(msg_);
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
  SharedPtr<AlertManeuverRequest> command_;
};

TEST_F(AlertManeuverRequestTest, Run_RequiredFieldsDoesNotExist_UNSUCCESS) {
  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  CommandPtr command(CreateCommand<AlertManeuverRequest>());
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][result_code].asInt()));
}

TEST_F(AlertManeuverRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  (*msg_)[msg_params][tts_chunks] = 0;
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command_)));
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][result_code].asInt()));
}

TEST_F(AlertManeuverRequestTest, Run_SoftButtnoProcessingResult_UNSUCCESS) {
  (*msg_)[params][connection_key] = kConnectionKey;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
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

TEST_F(AlertManeuverRequestTest, Run_IsWhiteSpaceExist_UNSUCCESS) {
  (*msg_)[msg_params][tts_chunks] = SmartObject(smart_objects::SmartType_Array);
  SmartObject tts_chunk(smart_objects::SmartType_Map);
  tts_chunk[text] = "wrong chunk syntax\t\n";
  (*msg_)[msg_params][tts_chunks].asArray()->push_back(tts_chunk);
  (*msg_)[params][connection_key] = kConnectionKey;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command_)));
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][result_code].asInt()));
}

TEST_F(AlertManeuverRequestTest, Run_SoftButtnoProcessingResult_SUCCESS) {
  (*msg_)[params][connection_key] = kConnectionKey;
  (*msg_)[params][function_id] = kFunctionID;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  smart_objects::SmartObject& msg_params = (*msg_)[am::strings::msg_params];
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons(msg_params,
                                 Eq(mock_app_),
                                 Ref(mock_policy_handler_),
                                 Ref(mock_app_manager_)))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(
      mock_message_helper_,
      SubscribeApplicationToSoftButton(msg_params, Eq(mock_app_), kFunctionID));
  MessageSharedPtr result_msg(CatchHMICommandResult(CallRun(*command_)));
  EXPECT_EQ(hmi_apis::FunctionID::Navigation_AlertManeuver,
            static_cast<hmi_apis::FunctionID::eType>(
                (*result_msg)[params][function_id].asInt()));
}

TEST_F(AlertManeuverRequestTest, Run_TTSCorrect_SUCCESS) {
  (*msg_)[params][function_id] = kFunctionID;
  (*msg_)[msg_params][tts_chunks] = SmartObject(smart_objects::SmartType_Array);
  SmartObject tts_chunk(smart_objects::SmartType_Map);
  tts_chunk[text] = "correct_chunk";
  (*msg_)[msg_params][tts_chunks].asArray()->push_back(tts_chunk);
  (*msg_)[params][connection_key] = kConnectionKey;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  smart_objects::SmartObject& msg_params = (*msg_)[am::strings::msg_params];
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons(msg_params,
                                 Eq(mock_app_),
                                 Ref(mock_policy_handler_),
                                 Ref(mock_app_manager_)))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(
      mock_message_helper_,
      SubscribeApplicationToSoftButton(msg_params, Eq(mock_app_), kFunctionID));
  EXPECT_CALL(mock_app_manager_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::Navigation_AlertManeuver)));
  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_Speak)));
  command_->Run();
}

TEST_F(AlertManeuverRequestTest, OnEvent_ReceivedUnknownEvent_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command_, event)));
  EXPECT_EQ(mobile_apis::Result::INVALID_ENUM,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][result_code].asInt()));
}

TEST_F(AlertManeuverRequestTest,
       OnEvent_ReceivedNavigationAlertManeuverEvent_SUCCESS) {
  const hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg_)[params][am::hmi_response::code] = response_code;
  mobile_apis::Result::eType casted_response_code =
      static_cast<mobile_apis::Result::eType>(response_code);
  EXPECT_CALL(mock_message_helper_, MobileToHMIResult(casted_response_code))
      .WillOnce(Return(hmi_apis::Common_Result::SUCCESS));
  Event event(hmi_apis::FunctionID::Navigation_AlertManeuver);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(AlertManeuverRequestTest,
       OnEvent_ReceivedNavigationAlertManeuverEvent_UNSUCCESS) {
  const hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::GENERIC_ERROR;
  (*msg_)[params][am::hmi_response::code] = response_code;
  mobile_apis::Result::eType casted_response_code =
      static_cast<mobile_apis::Result::eType>(response_code);
  EXPECT_CALL(mock_message_helper_, MobileToHMIResult(casted_response_code))
      .WillOnce(Return(hmi_apis::Common_Result::GENERIC_ERROR));
  Event event(hmi_apis::FunctionID::Navigation_AlertManeuver);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(AlertManeuverRequestTest, OnEvent_ReceivedTTSSpeakEvent_UNSUCCESS) {
  const hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::GENERIC_ERROR;
  (*msg_)[params][am::hmi_response::code] = response_code;
  mobile_apis::Result::eType casted_response_code =
      static_cast<mobile_apis::Result::eType>(response_code);
  EXPECT_CALL(mock_message_helper_, MobileToHMIResult(casted_response_code))
      .WillOnce(Return(hmi_apis::Common_Result::GENERIC_ERROR));
  Event event(hmi_apis::FunctionID::TTS_Speak);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(AlertManeuverRequestTest, OnEvent_ReceivedTTSSpeakEvent_SUCCESS) {
  const hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg_)[params][am::hmi_response::code] = response_code;
  mobile_apis::Result::eType casted_response_code =
      static_cast<mobile_apis::Result::eType>(response_code);
  EXPECT_CALL(mock_message_helper_, MobileToHMIResult(casted_response_code))
      .WillOnce(Return(hmi_apis::Common_Result::SUCCESS));
  Event event(hmi_apis::FunctionID::TTS_Speak);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

TEST_F(AlertManeuverRequestTest,
       OnEvent_ReceivedTTSOnResetTimeoutEvent_SUCCESS) {
  (*msg_)[params][connection_key] = kConnectionKey;
  (*msg_)[params][correlation_id] = kCorrelationId;
  EXPECT_CALL(
      mock_app_manager_,
      updateRequestTimeout(kConnectionKey, kCorrelationId, kDefaultTimeout_));
  Event event(hmi_apis::FunctionID::TTS_OnResetTimeout);
  event.set_smart_object(*msg_);
  command_->on_event(event);
}

}  // namespace alert_maneuver_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

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

#include "mobile/alert_request.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace alert_request {

using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AtLeast;
namespace am = ::application_manager;
using am::commands::AlertRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using am::MockMessageHelper;
using policy_test::MockPolicyHandlerInterface;

typedef SharedPtr<AlertRequest> CommandPtr;
namespace {
const uint32_t kDefaultTimeout = 1000u;
const uint32_t kConnectionKey = 1u;
const uint32_t kCorrelationId = 2u;
const mobile_apis::FunctionID::eType kFunctionId =
    mobile_apis::FunctionID::AlertID;
}  // namespace

class AlertRequestTest : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  AlertRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , mock_app_(CreateMockApp())
      , msg_(CreateMessage(smart_objects::SmartType_Map)) {}

  void SetUp() OVERRIDE {
    (*msg_)[am::strings::params][am::strings::function_id] = kFunctionId;
    (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg_)[am::strings::params][am::strings::correlation_id] = kCorrelationId;

    ON_CALL(mock_app_manager_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(
        *mock_app_,
        AreCommandLimitsExceeded(kFunctionId, am::TLimitSource::POLICY_TABLE))
        .WillByDefault(Return(false));
    ON_CALL(mock_app_manager_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    ON_CALL(*mock_app_, hmi_level())
        .WillByDefault(Return(mobile_apis::HMILevel::HMI_FULL));
    ON_CALL(*mock_app_, hmi_level())
        .WillByDefault(Return(mobile_apis::HMILevel::HMI_BACKGROUND));

    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }
  void AddAlertTextsToMsg() {
    (*msg_)[am::strings::msg_params][am::strings::alert_text1] = "alert_text1";
    (*msg_)[am::strings::msg_params][am::strings::alert_text2] = "alert_text2";
    (*msg_)[am::strings::msg_params][am::strings::alert_text3] = "alert_text3";
  }
  void AddTTSChunkToMsg() {
    (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
           [am::strings::text] = "tts_chunk_text";
  }

  void ExpectCallHmiLevel(const mobile_apis::HMILevel::eType level) {
    EXPECT_CALL(*mock_app_, hmi_level()).WillRepeatedly(Return(level));
  }

  void ExpectManageMobileCommandWithResultCode(
      const mobile_apis::Result::eType code) {
    EXPECT_CALL(
        mock_app_manager_,
        ManageMobileCommand(MobileResultCodeIs(code),
                            am::commands::Command::CommandOrigin::ORIGIN_SDL));
  }

  void ExpectManageHmiCommandTTSAndUI() {
    EXPECT_CALL(
        mock_app_manager_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::UI_Alert)))
        .WillOnce(Return(true));
    EXPECT_CALL(
        mock_app_manager_,
        ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_Speak)))
        .WillOnce(Return(true));
  }

  MockMessageHelper& mock_message_helper_;
  MockAppPtr mock_app_;
  MessageSharedPtr msg_;
  MockPolicyHandlerInterface mock_policy_handler_;
};

class CallOnTimeOut {
 public:
  CallOnTimeOut(CommandRequestImpl& command) : command_(command) {}

  void operator()() {
    command_.onTimeOut();
  }

  CommandRequestImpl& command_;
};

TEST_F(AlertRequestTest, Init_DurationExists_SUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::duration] = kDefaultTimeout;
  (*msg_)[am::strings::msg_params][am::strings::soft_buttons] = "soft_buttons";

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  EXPECT_TRUE(command->Init());
}

TEST_F(AlertRequestTest, Init_DurationNotExists_SUCCESS) {
  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  EXPECT_TRUE(command->Init());
}

TEST_F(AlertRequestTest, OnTimeOut_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::soft_buttons] = 0;
  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->onTimeOut();
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
}

TEST_F(AlertRequestTest, OnTimeOut_SUCCESS) {
  MessageSharedPtr result_msg(CreateMessage(smart_objects::SmartType_Null));
  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, mobile_apis::Result::GENERIC_ERROR))
      .WillOnce(Return(result_msg));

  CommandPtr command(CreateCommand<AlertRequest>());
  MessageSharedPtr received_result_msg(
      CatchMobileCommandResult(CallOnTimeOut(*command)));
  EXPECT_EQ(result_msg, received_result_msg);
}

TEST_F(AlertRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  MockAppPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(invalid_app));

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  ExpectManageMobileCommandWithResultCode(
      mobile_apis::Result::APPLICATION_NOT_REGISTERED);
  command->Run();
}

TEST_F(AlertRequestTest, Run_AlertFrequencyIsTooHigh_UNSUCCESS) {
  EXPECT_CALL(
      *mock_app_,
      AreCommandLimitsExceeded(kFunctionId, am::TLimitSource::POLICY_TABLE))
      .WillOnce(Return(true));

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(mobile_apis::Result::REJECTED,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertRequestTest, Run_FailToProcessSoftButtons_UNSUCCESS) {
  const mobile_apis::Result::eType result_code =
      mobile_apis::Result::INVALID_ENUM;

  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(result_code));

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  EXPECT_EQ(result_code,
            static_cast<mobile_apis::Result::eType>(
                (*result_msg)[am::strings::msg_params][am::strings::result_code]
                    .asInt()));
}

TEST_F(AlertRequestTest, Run_MandatoryParametersAreMissed_UNSUCCESS) {
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);
  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();
}

TEST_F(AlertRequestTest, Run_MandatoryParametersAreInvalid_UNSUCCESS) {
  AddAlertTextsToMsg();
  (*msg_)[am::strings::msg_params][am::strings::alert_text2] =
      "invalid\t\nParam";

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);
  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();
}

TEST_F(AlertRequestTest, Run_SUCCESS) {
  AddAlertTextsToMsg();
  AddTTSChunkToMsg();

  (*msg_)[am::strings::msg_params][am::strings::soft_buttons] = "soft_buttons";
  (*msg_)[am::strings::msg_params][am::strings::progress_indicator] =
      "progress_indicator";
  (*msg_)[am::strings::msg_params][am::strings::play_tone] = true;

  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(mock_message_helper_,
              SubscribeApplicationToSoftButton(
                  (*msg_)[am::strings::msg_params], _, kFunctionId));

  ExpectManageHmiCommandTTSAndUI();
  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();
}

TEST_F(AlertRequestTest, OnEvent_InvalidEventId_UNSUCCESS) {
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  event.set_smart_object(*msg_);

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->on_event(event);
}

TEST_F(AlertRequestTest, OnEvent_UI_OnResetTimeout_SUCCESS) {
  AddAlertTextsToMsg();
  (*msg_)[am::strings::msg_params][am::strings::duration] = kDefaultTimeout;

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  EXPECT_TRUE(command->Init());

  EXPECT_CALL(
      mock_app_manager_,
      updateRequestTimeout(kConnectionKey, kCorrelationId, kDefaultTimeout));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_ENUM);

  Event event(hmi_apis::FunctionID::UI_OnResetTimeout);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(AlertRequestTest, OnEvent_UIAlertHasHmiResponsesToWait_UNSUCCESS) {
  AddAlertTextsToMsg();
  AddTTSChunkToMsg();

  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::WARNINGS;
  (*msg_)[am::strings::msg_params][am::strings::play_tone] = true;

  CommandPtr command(CreateCommand<AlertRequest>(msg_));

  ExpectCallHmiLevel(mobile_apis::HMILevel::HMI_FULL);

  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  ExpectManageHmiCommandTTSAndUI();

  command->Run();

  Event event(hmi_apis::FunctionID::UI_Alert);
  event.set_smart_object(*msg_);

  EXPECT_CALL(mock_app_manager_,
              ManageHMICommand(
                  HMIResultCodeIs(hmi_apis::FunctionID::TTS_StopSpeaking)));

  command->on_event(event);
}

TEST_F(AlertRequestTest, OnEvent_TTSWarnings_SUCCESS) {
  AddTTSChunkToMsg();
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::WARNINGS;
  (*msg_)[am::strings::msg_params][am::strings::play_tone] = true;

  ExpectCallHmiLevel(mobile_apis::HMILevel::HMI_FULL);

  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_Speak)))
      .WillOnce(Return(true));

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::WARNINGS);

  Event event(hmi_apis::FunctionID::TTS_Speak);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(AlertRequestTest, OnEvent_TTSUnsupportedResource_SUCCESS) {
  AddTTSChunkToMsg();
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg_)[am::strings::msg_params][am::strings::play_tone] = true;

  ExpectCallHmiLevel(mobile_apis::HMILevel::HMI_FULL);

  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_Speak)))
      .WillOnce(Return(true));

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::WARNINGS);

  Event event(hmi_apis::FunctionID::TTS_Speak);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(AlertRequestTest, OnEvent_TTSUnsupportedResourceUiAlertSent_SUCCESS) {
  AddAlertTextsToMsg();
  AddTTSChunkToMsg();
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg_)[am::strings::msg_params][am::strings::play_tone] = true;
  (*msg_)[am::strings::msg_params][am::strings::soft_buttons] = "soft_buttons";

  ExpectCallHmiLevel(mobile_apis::HMILevel::HMI_FULL);
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_message_helper_,
              SubscribeApplicationToSoftButton(
                  (*msg_)[am::strings::msg_params], _, kFunctionId));

  ExpectManageHmiCommandTTSAndUI();

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();

  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_StopSpeaking)))
      .WillOnce(Return(true));

  Event ui_event(hmi_apis::FunctionID::UI_Alert);
  ui_event.set_smart_object(*msg_);
  command->on_event(ui_event);

  Event tts_stop_event(hmi_apis::FunctionID::TTS_StopSpeaking);
  tts_stop_event.set_smart_object(*msg_);
  command->on_event(tts_stop_event);

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::WARNINGS);

  Event event(hmi_apis::FunctionID::TTS_Speak);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(AlertRequestTest, OnEvent_TTSUnsupportedResourceUiAlertSuccess_SUCCESS) {
  AddAlertTextsToMsg();
  AddTTSChunkToMsg();
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg_)[am::strings::msg_params][am::strings::play_tone] = true;
  (*msg_)[am::strings::msg_params][am::strings::soft_buttons] = "soft_buttons";

  ExpectCallHmiLevel(mobile_apis::HMILevel::HMI_FULL);
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_message_helper_,
              SubscribeApplicationToSoftButton(
                  (*msg_)[am::strings::msg_params], _, kFunctionId));
  ExpectManageHmiCommandTTSAndUI();

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();

  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;

  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_StopSpeaking)))
      .WillOnce(Return(true));

  Event ui_event(hmi_apis::FunctionID::UI_Alert);
  ui_event.set_smart_object(*msg_);
  command->on_event(ui_event);

  Event tts_stop_event(hmi_apis::FunctionID::TTS_StopSpeaking);
  tts_stop_event.set_smart_object(*msg_);
  command->on_event(tts_stop_event);

  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::WARNINGS);

  Event event(hmi_apis::FunctionID::TTS_Speak);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(AlertRequestTest, OnEvent_TTSSuccesUiAlertInvalidEnum_SUCCESS) {
  AddTTSChunkToMsg();
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg_)[am::strings::msg_params][am::strings::play_tone] = true;

  ExpectCallHmiLevel(mobile_apis::HMILevel::HMI_FULL);
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_Speak)))
      .WillOnce(Return(true));

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();

  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::INVALID_ENUM;

  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_StopSpeaking)))
      .WillOnce(Return(true));

  Event ui_event(hmi_apis::FunctionID::UI_Alert);
  ui_event.set_smart_object(*msg_);
  command->on_event(ui_event);

  Event tts_stop_event(hmi_apis::FunctionID::TTS_StopSpeaking);
  tts_stop_event.set_smart_object(*msg_);
  command->on_event(tts_stop_event);

  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::SUCCESS);

  Event event(hmi_apis::FunctionID::TTS_Speak);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(AlertRequestTest, OnEvent_TTSAbortedUiAlertNotSent_SUCCESS) {
  AddTTSChunkToMsg();
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg_)[am::strings::msg_params][am::strings::play_tone] = true;

  ExpectCallHmiLevel(mobile_apis::HMILevel::HMI_FULL);
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_Speak)))
      .WillOnce(Return(true));

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();

  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::INVALID_ENUM;

  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_StopSpeaking)))
      .WillOnce(Return(true));

  Event ui_event(hmi_apis::FunctionID::UI_Alert);
  ui_event.set_smart_object(*msg_);
  command->on_event(ui_event);

  Event tts_stop_event(hmi_apis::FunctionID::TTS_StopSpeaking);
  tts_stop_event.set_smart_object(*msg_);
  command->on_event(tts_stop_event);

  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::ABORTED;

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::ABORTED);

  Event event(hmi_apis::FunctionID::TTS_Speak);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(AlertRequestTest, OnEvent_TTSWarningUiAlertWarning_SUCCESS) {
  AddAlertTextsToMsg();
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::WARNINGS;
  (*msg_)[am::strings::msg_params][am::strings::play_tone] = true;

  ExpectCallHmiLevel(mobile_apis::HMILevel::HMI_FULL);
  EXPECT_CALL(mock_message_helper_,
              ProcessSoftButtons((*msg_)[am::strings::msg_params], _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  ExpectManageHmiCommandTTSAndUI();

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();

  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_StopSpeaking)))
      .WillOnce(Return(true));

  Event ui_event(hmi_apis::FunctionID::UI_Alert);
  ui_event.set_smart_object(*msg_);
  command->on_event(ui_event);

  Event tts_stop_event(hmi_apis::FunctionID::TTS_StopSpeaking);
  tts_stop_event.set_smart_object(*msg_);
  command->on_event(tts_stop_event);

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::WARNINGS);

  Event event(hmi_apis::FunctionID::TTS_Speak);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(AlertRequestTest, Run_InvalidAlert2_UNSUCCESS) {
  AddAlertTextsToMsg();
  (*msg_)[am::strings::msg_params][am::strings::alert_text2] =
      "invalid_text_with_empty_str\\n";

  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _)).Times(0);
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();
}

TEST_F(AlertRequestTest, Run_InvalidAlert3_UNSUCCESS) {
  AddAlertTextsToMsg();
  (*msg_)[am::strings::msg_params][am::strings::alert_text3] =
      "invalid_text_with_empty_str\\n";

  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _)).Times(0);
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();
}

TEST_F(AlertRequestTest, Run_InvalidTTSChunk_UNSUCCESS) {
  AddAlertTextsToMsg();
  (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
         [am::strings::text] = "invalid_text_with_empty_str\\n";

  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _)).Times(0);
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  CommandPtr command(CreateCommand<AlertRequest>(msg_));
  command->Run();
}

}  // namespace alert_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

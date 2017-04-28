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

#include "application_manager/commands/mobile/perform_audio_pass_thru_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace perform_audio_pass_thru_request {

namespace am = application_manager;
using am::commands::PerformAudioPassThruRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using am::MockHmiInterfaces;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::InSequence;

namespace {
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 2u;
const std::string kCorrectPrompt = "CorrectPrompt";
const std::string kCorrectType = "CorrectType";
const std::string kCorrectDisplayText1 = "CorrectDisplayText1";
const std::string kCorrectDisplayText2 = "CorrectDisplayText2";
const std::string kFunctionId = "FunctionId";
const uint32_t kTimeoutForTTSSpeak = 1u;
}  // namespace

class PerformAudioPassThruRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  PerformAudioPassThruRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock())
      , mock_app_(CreateMockApp())
      , message_(utils::MakeShared<SmartObject>(::smart_objects::SmartType_Map))
      , msg_params_((*message_)[am::strings::msg_params]) {}

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

  void TestWrongSyntaxInField(const std::string& field) {
    if (field == am::strings::initial_prompt) {
      msg_params_[field][0][am::strings::text] = "prompt\\n";
    } else {
      msg_params_[field] = "prompt\\n";
    }

    EXPECT_CALL(*application_sptr_, hmi_level())
        .WillOnce(Return(am::mobile_api::HMILevel::HMI_FULL));

    CallRun caller(*command_sptr_);
    MessageSharedPtr result_message = CatchMobileCommandResult(caller);

    const am::mobile_api::Result::eType result =
        static_cast<am::mobile_api::Result::eType>(
            (*result_message)[am::strings::msg_params][am::strings::result_code]
                .asInt());

    EXPECT_EQ(am::mobile_api::Result::INVALID_DATA, result);
  }

  void SetUp() OVERRIDE {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(hmi_interfaces_));
    command_sptr_ =
        CreateCommand<am::commands::PerformAudioPassThruRequest>(message_);

    application_sptr_ = CreateMockApp();
    ON_CALL(app_mngr_, application(_)).WillByDefault(Return(application_sptr_));
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void ResultCommandExpectations(MessageSharedPtr msg,
                                 const std::string& info) {
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::success].asBool(),
              true);
    EXPECT_EQ(
        (*msg)[am::strings::msg_params][am::strings::result_code].asInt(),
        static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
    EXPECT_EQ((*msg)[am::strings::msg_params][am::strings::info].asString(),
              info);
  }

  sync_primitives::Lock lock_;
  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  MockMessageHelper& mock_message_helper_;
  MockAppPtr mock_app_;
  MessageSharedPtr message_;
  ::smart_objects::SmartObject& msg_params_;
  utils::SharedPtr<am::commands::PerformAudioPassThruRequest> command_sptr_;
  MockAppPtr application_sptr_;
};

TEST_F(PerformAudioPassThruRequestTest, OnTimeout_GENERIC_ERROR) {
  MessageSharedPtr msg_ui = CreateMessage(smart_objects::SmartType_Map);
  (*msg_ui)[am::strings::msg_params][am::strings::result_code] =
      am::mobile_api::Result::GENERIC_ERROR;
  (*msg_ui)[am::strings::msg_params][am::strings::success] = false;

  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>();

  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, StopAudioPassThru(_));

  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, am::mobile_api::Result::GENERIC_ERROR))
      .WillOnce(Return(msg_ui));

  MessageSharedPtr vr_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));

  command->onTimeOut();
  EXPECT_EQ((*vr_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            false);
  EXPECT_EQ(
      (*vr_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(am::mobile_api::Result::GENERIC_ERROR));
  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_UIHmiSendUnsupportedResource_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_ui = CreateFullParamsUISO();
  (*msg_ui)[am::strings::params][am::strings::connection_key] = kConnectionKey;

  utils::SharedPtr<PerformAudioPassThruRequest> command =
      CreateCommand<PerformAudioPassThruRequest>(msg_ui);

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*msg)[am::strings::msg_params][am::strings::info] =
      "UI is not supported by system";

  am::event_engine::Event event(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  event.set_smart_object(*msg);

  ON_CALL(hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  ON_CALL(hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr response_msg_tts =
      CreateMessage(smart_objects::SmartType_Map);
  (*response_msg_tts)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response_msg_tts)[am::strings::msg_params][am::strings::cmd_id] = kCmdId;
  am::event_engine::Event event_tts(hmi_apis::FunctionID::TTS_Speak);
  event_tts.set_smart_object(*response_msg_tts);
  ON_CALL(mock_message_helper_,
          HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillByDefault(Return(am::mobile_api::Result::SUCCESS));
  command->on_event(event_tts);

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->on_event(event);

  ResultCommandExpectations(ui_command_result, "UI is not supported by system");
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_InvalidApp_ApplicationNotRegisteredResponce) {
  utils::SharedPtr<am::Application> null_application_sptr;
  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(null_application_sptr));

  CallRun caller(*command_sptr_);
  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const am::mobile_api::Result::eType result =
      static_cast<am::mobile_api::Result::eType>(
          (*result_message)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(am::mobile_api::Result::APPLICATION_NOT_REGISTERED, result);
}

TEST_F(PerformAudioPassThruRequestTest, Run_HmiLevelNone_Rejected) {
  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(am::mobile_api::HMILevel::HMI_NONE));

  CallRun caller(*command_sptr_);
  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const am::mobile_api::Result::eType result =
      static_cast<am::mobile_api::Result::eType>(
          (*result_message)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(am::mobile_api::Result::REJECTED, result);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_WhitespaceInInitialPrompt_InvalidData) {
  TestWrongSyntaxInField(am::strings::initial_prompt);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_WhitespaceInAudioPassDisplayText1_InvalidData) {
  TestWrongSyntaxInField(am::strings::audio_pass_display_text1);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_WhitespaceInAudioPassDisplayText2_InvalidData) {
  TestWrongSyntaxInField(am::strings::audio_pass_display_text2);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_InitPromptCorrect_TTSSpeakIsAbsent) {
  // First we need to call SendSpeakRequest()
  // to enable the "is_active_tts_speak" key

  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(am::mobile_api::HMILevel::HMI_FULL));

  msg_params_[am::strings::initial_prompt][0][am::strings::text] =
      kCorrectPrompt;
  msg_params_[am::strings::initial_prompt][0][am::strings::type] = kCorrectType;

  MessageSharedPtr speak_reqeust_result_msg;
  MessageSharedPtr perform_result_msg;
  {
    InSequence dummy;
    // Send speak request sending
    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId));
    ON_CALL(hmi_interfaces_,
            GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_Speak))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
    ON_CALL(hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    EXPECT_CALL(app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&speak_reqeust_result_msg), Return(true)));

    // Perform audio path thru request sending
    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId));
    ON_CALL(
        hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::UI_PerformAudioPassThru))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    ON_CALL(hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    EXPECT_CALL(app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&perform_result_msg), Return(true)));
  }
  CallRun run_caller(*command_sptr_);
  run_caller();

  const ::smart_objects::SmartObject& speak_msg_params =
      (*speak_reqeust_result_msg)[am::strings::msg_params];

  const std::string result_initial_prompt =
      speak_msg_params[am::hmi_request::tts_chunks][0][am::strings::text]
          .asString();
  const std::string result_prompt_type =
      speak_msg_params[am::hmi_request::tts_chunks][0][am::strings::type]
          .asString();

  EXPECT_EQ(kCorrectPrompt, result_initial_prompt);
  EXPECT_EQ(kCorrectType, result_prompt_type);

  // Now we recieve on_event()

  am::event_engine::Event event(hmi_apis::FunctionID::UI_PerformAudioPassThru);
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::GENERIC_ERROR;
  event.set_smart_object(*message_);

  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));

  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  ON_CALL(hmi_interfaces_,
          GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_StopSpeaking))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  CallOnEvent on_event_caller(*command_sptr_, event);
  MessageSharedPtr command_result = CatchHMICommandResult(on_event_caller);

  const hmi_apis::FunctionID::eType result_function_id =
      static_cast<hmi_apis::FunctionID::eType>(
          (*command_result)[am::strings::params][am::strings::function_id]
              .asInt());

  EXPECT_EQ(hmi_apis::FunctionID::TTS_StopSpeaking, result_function_id);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_InitPromptCorrect_SpeakAndPerformAPTRequestsSendMuteTrue) {
  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(am::mobile_api::HMILevel::HMI_FULL));

  msg_params_[am::strings::initial_prompt][0][am::strings::text] =
      kCorrectPrompt;
  msg_params_[am::strings::initial_prompt][0][am::strings::type] = kCorrectType;
  msg_params_[am::strings::audio_pass_display_text1] = kCorrectDisplayText1;
  msg_params_[am::strings::audio_pass_display_text2] = kCorrectDisplayText2;

  MessageSharedPtr speak_reqeust_result_msg;
  MessageSharedPtr perform_result_msg;
  {
    InSequence dummy;
    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId));
    ON_CALL(hmi_interfaces_,
            GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_Speak))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
    ON_CALL(hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    EXPECT_CALL(app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&speak_reqeust_result_msg), Return(true)));

    // Perform audio path thru request sending
    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId));
    ON_CALL(
        hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::UI_PerformAudioPassThru))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    ON_CALL(hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    EXPECT_CALL(app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&perform_result_msg), Return(true)));
  }
  CallRun caller(*command_sptr_);
  caller();

  const ::smart_objects::SmartObject& speak_msg_params =
      (*speak_reqeust_result_msg)[am::strings::msg_params];
  const ::smart_objects::SmartObject& perform_msg_params =
      (*perform_result_msg)[am::strings::msg_params];

  const std::string result_initial_prompt =
      speak_msg_params[am::hmi_request::tts_chunks][0][am::strings::text]
          .asString();
  const std::string result_prompt_type =
      speak_msg_params[am::hmi_request::tts_chunks][0][am::strings::type]
          .asString();
  const std::string result_display_text_1 =
      perform_msg_params[am::hmi_request::audio_pass_display_texts][0]
                        [am::hmi_request::field_text].asString();
  const std::string result_display_text_2 =
      perform_msg_params[am::hmi_request::audio_pass_display_texts][1]
                        [am::hmi_request::field_text].asString();

  EXPECT_EQ(kCorrectPrompt, result_initial_prompt);
  EXPECT_EQ(kCorrectType, result_prompt_type);
  EXPECT_EQ(kCorrectDisplayText1, result_display_text_1);
  EXPECT_EQ(kCorrectDisplayText2, result_display_text_2);

  EXPECT_EQ(true, perform_msg_params[am::strings::mute_audio].asBool());
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_InitPromptCorrect_SpeakAndPerformAPTRequestsSendMuteFalse) {
  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(am::mobile_api::HMILevel::HMI_FULL));

  msg_params_[am::strings::initial_prompt][0][am::strings::text] =
      kCorrectPrompt;
  msg_params_[am::strings::initial_prompt][0][am::strings::type] = kCorrectType;

  const bool muted = false;

  msg_params_[am::strings::mute_audio] = muted;

  MessageSharedPtr speak_reqeust_result_msg;
  MessageSharedPtr perform_result_msg;
  {
    InSequence dummy;
    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId));
    ON_CALL(hmi_interfaces_,
            GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_Speak))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
    ON_CALL(hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    EXPECT_CALL(app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&speak_reqeust_result_msg), Return(true)));

    // Perform audio path thru request sending
    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId));
    ON_CALL(
        hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::UI_PerformAudioPassThru))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    ON_CALL(hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    EXPECT_CALL(app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&perform_result_msg), Return(true)));
  }
  CallRun caller(*command_sptr_);
  caller();

  EXPECT_EQ(
      muted,
      (*perform_result_msg)[am::strings::msg_params][am::strings::mute_audio]
          .asBool());
}

TEST_F(
    PerformAudioPassThruRequestTest,
    Run_InitPromptEmpty_PerformAndRecordStartNotificationsAndStartRecording) {
  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(am::mobile_api::HMILevel::HMI_FULL));

  MessageSharedPtr start_record_result_msg;
  MessageSharedPtr perform_result_msg;
  {
    InSequence dummy;

    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId));
    ON_CALL(
        hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::UI_PerformAudioPassThru))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
    ON_CALL(hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    // Perform audio path thru request sending
    EXPECT_CALL(app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&perform_result_msg), Return(true)));

    // Perform audio path thru request sending
    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId));
    ON_CALL(hmi_interfaces_,
            GetInterfaceFromFunction(hmi_apis::FunctionID::UI_OnRecordStart))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    ON_CALL(hmi_interfaces_, GetInterfaceState(_))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    // Start recording notification sending
    EXPECT_CALL(app_mngr_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&start_record_result_msg), Return(true)));
  }

  // Start microphone recording cals
  EXPECT_CALL(app_mngr_, BeginAudioPassThrough());
  EXPECT_CALL(app_mngr_, StartAudioPassThruThread(_, _, _, _, _, _));

  CallRun caller(*command_sptr_);
  caller();

  const hmi_apis::FunctionID::eType start_record_result_function_id =
      static_cast<hmi_apis::FunctionID::eType>(
          (*start_record_result_msg)[am::strings::params]
                                    [am::strings::function_id].asInt());
  EXPECT_EQ(hmi_apis::FunctionID::UI_OnRecordStart,
            start_record_result_function_id);
}

TEST_F(PerformAudioPassThruRequestTest, OnEvent_UIPAPT_Rejected) {
  am::event_engine::Event event(hmi_apis::FunctionID::UI_PerformAudioPassThru);

  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::REJECTED;
  event.set_smart_object(*message_);

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::REJECTED))
      .WillOnce(Return(am::mobile_api::Result::REJECTED));

  CallOnEvent caller(*command_sptr_, event);

  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const am::mobile_api::Result::eType result_code =
      static_cast<am::mobile_api::Result::eType>(
          (*result_message)[am::strings::msg_params][am::strings::result_code]
              .asInt());

  EXPECT_EQ(am::mobile_api::Result::REJECTED, result_code);
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_TTSSpeakSuccess_UpdateRequestTimeout) {
  am::event_engine::Event event(hmi_apis::FunctionID::TTS_Speak);

  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  event.set_smart_object(*message_);

  // Start recording notification sending
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).WillOnce(Return(true));

  // Start microphone recording cals
  EXPECT_CALL(app_mngr_, BeginAudioPassThrough());
  EXPECT_CALL(app_mngr_, StartAudioPassThruThread(_, _, _, _, _, _));

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _));

  ON_CALL(hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(_))
      .WillOnce(Return(am::mobile_api::Result::SUCCESS));

  CallOnEvent caller(*command_sptr_, event);
  caller();
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_PAPTunsupportedResource_CorrectInfo) {
  const std::string return_info = "Unsupported phoneme type sent in a prompt";

  am::event_engine::Event event_speak(hmi_apis::FunctionID::TTS_Speak);
  am::event_engine::Event event_perform(
      hmi_apis::FunctionID::UI_PerformAudioPassThru);

  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  event_speak.set_smart_object(*message_);

  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  event_perform.set_smart_object(*message_);

  ON_CALL(hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  // First call on_event for setting result_tts_speak_ to UNSUPPORTED_RESOURCE
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _));
  CallOnEvent caller_speak(*command_sptr_, event_speak);
  caller_speak();

  // Second call for test correct behavior of UI_PerformAudioPassThru event
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(false));
  EXPECT_CALL(app_mngr_, StopAudioPassThru(_)).Times(0);
  ON_CALL(hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  CallOnEvent caller_perform(*command_sptr_, event_perform);

  MessageSharedPtr perform_event_result =
      CatchMobileCommandResult(caller_perform);

  EXPECT_EQ(return_info,
            (*perform_event_result)[am::strings::msg_params][am::strings::info]
                .asString());
}

TEST_F(PerformAudioPassThruRequestTest,
       DISABLED_OnEvent_TTSSpeak_UpdateTimeout) {
  am::event_engine::Event event(hmi_apis::FunctionID::TTS_Speak);

  msg_params_[am::strings::connection_key] = kConnectionKey;
  msg_params_[am::strings::function_id] = kFunctionId;
  msg_params_[am::strings::correlation_id] = kCorrelationId;

  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).WillOnce(Return(true));

  EXPECT_CALL(app_mngr_, BeginAudioPassThrough()).WillOnce(Return(true));

  EXPECT_CALL(
      app_mngr_,
      StartAudioPassThruThread(kConnectionKey, kCorrelationId, _, _, _, _));

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _));
  ON_CALL(hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  ON_CALL(mock_message_helper_, HMIToMobileResult(_))
      .WillByDefault(Return(am::mobile_api::Result::SUCCESS));
  CallOnEvent caller(*command_sptr_, event);
  caller();

  EXPECT_EQ(kConnectionKey, msg_params_[am::strings::connection_key].asUInt());
  EXPECT_EQ(kFunctionId, msg_params_[am::strings::function_id].asString());
}

TEST_F(PerformAudioPassThruRequestTest,
       DISABLED_OnEvent_TTSOnResetTimeout_UpdateTimeout) {
  am::event_engine::Event event(hmi_apis::FunctionID::TTS_OnResetTimeout);

  msg_params_[am::strings::connection_key] = kConnectionKey;
  msg_params_[am::strings::function_id] = kFunctionId;

  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, BeginAudioPassThrough()).WillOnce(Return(true));

  EXPECT_CALL(
      app_mngr_,
      StartAudioPassThruThread(kConnectionKey, kCorrelationId, _, _, _, _));
  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _));
  ON_CALL(hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  CallOnEvent caller(*command_sptr_, event);
  caller();

  EXPECT_EQ(kConnectionKey, msg_params_[am::strings::connection_key].asUInt());
  EXPECT_EQ(kFunctionId, msg_params_[am::strings::function_id].asString());
}

TEST_F(PerformAudioPassThruRequestTest, OnEvent_DefaultCase) {
  am::event_engine::Event event(hmi_apis::FunctionID::INVALID_ENUM);

  EXPECT_CALL(app_mngr_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).Times(0);

  CallOnEvent caller(*command_sptr_, event);
  caller();
}

TEST_F(PerformAudioPassThruRequestTest, Init_CorrectTimeout) {
  const uint32_t kDefaultTimeout = command_sptr_->default_timeout();
  const uint32_t kMaxDuration = 10000u;

  msg_params_[am::strings::max_duration] = kMaxDuration;

  command_sptr_->Init();

  EXPECT_EQ(kDefaultTimeout + kMaxDuration, command_sptr_->default_timeout());
}

TEST_F(PerformAudioPassThruRequestTest,
       onTimeOut_ttsSpeakNotActive_DontSendHMIReqeust) {
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, StopAudioPassThru(_));

  // For setting current_state_ -> kCompleted
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  command_sptr_->SendResponse(true, am::mobile_api::Result::SUCCESS);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);

  command_sptr_->onTimeOut();
}

TEST_F(PerformAudioPassThruRequestTest,
       DISABLED_onTimeOut_ttsSpeakActive_SendHMIReqeust) {
  EXPECT_CALL(app_mngr_, EndAudioPassThrough()).WillOnce(Return(true));
  EXPECT_CALL(app_mngr_, StopAudioPassThru(_));

  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(am::mobile_api::HMILevel::HMI_FULL));

  msg_params_[am::strings::initial_prompt][0][am::strings::text] =
      kCorrectPrompt;
  msg_params_[am::strings::initial_prompt][0][am::strings::type] = kCorrectType;
  MessageSharedPtr speak_reqeust_result_msg;
  MessageSharedPtr perform_result_msg;
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  ON_CALL(hmi_interfaces_,
          GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_Speak))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  EXPECT_CALL(app_mngr_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&speak_reqeust_result_msg), Return(true)));

  // Perform audio path thru request sending
  ON_CALL(app_mngr_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));
  ON_CALL(
      hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_PerformAudioPassThru))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(hmi_interfaces_, GetInterfaceState(_))
      .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  EXPECT_CALL(app_mngr_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&perform_result_msg), Return(true)));

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  EXPECT_CALL(
      mock_message_helper_,
      CreateNegativeResponse(_, _, _, am::mobile_api::Result::GENERIC_ERROR))
      .WillOnce(Return(msg));

  // For setting is_active_tts_speak -> true
  EXPECT_CALL(app_mngr_, ManageHMICommand(_))
      .Times(2)
      .WillRepeatedly(Return(false));
  CallRun caller(*command_sptr_);
  caller();

  // For setting current_state_ -> kCompleted

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(_, am::commands::Command::ORIGIN_SDL));
  command_sptr_->SendResponse(true, am::mobile_api::Result::SUCCESS);

  EXPECT_CALL(
      app_mngr_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_StopSpeaking)))
      .WillOnce(Return(false));
  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(_))
      .WillOnce(Return(am::mobile_api::Result::SUCCESS));

  command_sptr_->onTimeOut();
}

}  // namespace perform_audio_pass_thru_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests

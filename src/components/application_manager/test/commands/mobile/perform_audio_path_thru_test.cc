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

#include "application_manager/commands/command_request_test.h"

#include "mobile/perform_audio_pass_thru_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace perform_audio_pass_thru_request {

using namespace application_manager;

using ::testing::Return;
using ::testing::_;
using ::testing::InSequence;

namespace {
const std::string kCorrectPrompt = "CorrectPrompt";
const std::string kCorrectType = "CorrectType";
const std::string kCorrectDisplayText1 = "CorrectDisplayText1";
const std::string kCorrectDisplayText2 = "CorrectDisplayText2";
}

class PerformAudioPassThruRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  PerformAudioPassThruRequestTest()
      : message_(utils::MakeShared<SmartObject>(::smart_objects::SmartType_Map))
      , msg_params_((*message_)[strings::msg_params]) {}
  void SetUp() OVERRIDE {
    command_sptr_ = CreateCommand<
        application_manager::commands::PerformAudioPassThruRequest>(message_);

    application_sptr_ = CreateMockApp();
    ON_CALL(mock_app_manager_, application(_))
        .WillByDefault(Return(application_sptr_));
  }

 protected:
  void TestWrongSyntaxInField(const std::string& field) {
    if (field == strings::initial_prompt) {
      msg_params_[field][0][strings::text] = "prompt\\n";
    } else {
      msg_params_[field] = "prompt\\n";
    }

    EXPECT_CALL(*application_sptr_, hmi_level())
        .WillOnce(Return(mobile_api::HMILevel::HMI_FULL));

    CallRun caller(*command_sptr_);
    MessageSharedPtr result_message = CatchMobileCommandResult(caller);

    const mobile_api::Result::eType kResult =
        static_cast<mobile_api::Result::eType>(
            (*result_message)[strings::msg_params][strings::result_code]
                .asInt());

    EXPECT_EQ(mobile_api::Result::INVALID_DATA, kResult);
  }

  MessageSharedPtr message_;
  ::smart_objects::SmartObject& msg_params_;
  utils::SharedPtr<commands::PerformAudioPassThruRequest> command_sptr_;
  MockAppPtr application_sptr_;
};

TEST_F(PerformAudioPassThruRequestTest,
       Run_InvalidApp_ApplicationNotRegisteredResponce) {
  utils::SharedPtr<Application> null_application_sptr;
  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(null_application_sptr));

  CallRun caller(*command_sptr_);
  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const mobile_api::Result::eType kResult =
      static_cast<mobile_api::Result::eType>(
          (*result_message)[strings::msg_params][strings::result_code].asInt());
  EXPECT_EQ(mobile_api::Result::APPLICATION_NOT_REGISTERED, kResult);
}

TEST_F(PerformAudioPassThruRequestTest, Run_HmiLevelNone_Rejected) {
  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(mobile_api::HMILevel::HMI_NONE));

  CallRun caller(*command_sptr_);
  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const mobile_api::Result::eType kResult =
      static_cast<mobile_api::Result::eType>(
          (*result_message)[strings::msg_params][strings::result_code].asInt());
  EXPECT_EQ(mobile_api::Result::REJECTED, kResult);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_WhitespaceInInitialPrompt_InvalidData) {
  TestWrongSyntaxInField(strings::initial_prompt);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_WhitespaceInAudioPassDisplayText1_InvalidData) {
  TestWrongSyntaxInField(strings::audio_pass_display_text1);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_WhitespaceInAudioPassDisplayText2_InvalidData) {
  TestWrongSyntaxInField(strings::audio_pass_display_text2);
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_InitPromptCorrect_SpeakAndPerformAPTRequestsSendMuteTrue) {
  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(mobile_api::HMILevel::HMI_FULL));

  msg_params_[strings::initial_prompt][0][strings::text] = kCorrectPrompt;
  msg_params_[strings::initial_prompt][0][strings::type] = kCorrectType;
  msg_params_[strings::audio_pass_display_text1] = kCorrectDisplayText1;
  msg_params_[strings::audio_pass_display_text2] = kCorrectDisplayText2;

  MessageSharedPtr speak_reqeust_result_msg;
  MessageSharedPtr perform_result_msg;
  {
    InSequence dummy;
    // Send speak request sending
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&speak_reqeust_result_msg), Return(true)));

    // Perform audio path thru request sending
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&perform_result_msg), Return(true)));
  }
  CallRun caller(*command_sptr_);
  caller();

  const ::smart_objects::SmartObject& kSpeakMsgParams_ =
      (*speak_reqeust_result_msg)[strings::msg_params];
  const ::smart_objects::SmartObject& kPerformMsgParams =
      (*perform_result_msg)[strings::msg_params];

  const std::string& kResultInitialPrompt =
      kSpeakMsgParams_[hmi_request::tts_chunks][0][strings::text].asString();
  const std::string& kResultPromptType =
      kSpeakMsgParams_[hmi_request::tts_chunks][0][strings::type].asString();
  const std::string& kResultDisplayText1 =
      kPerformMsgParams[hmi_request::audio_pass_display_texts][0]
                       [hmi_request::field_text].asString();
  const std::string& kResultDisplayText2 =
      kPerformMsgParams[hmi_request::audio_pass_display_texts][1]
                       [hmi_request::field_text].asString();

  EXPECT_EQ(kCorrectPrompt, kResultInitialPrompt);
  EXPECT_EQ(kCorrectType, kResultPromptType);
  EXPECT_EQ(kCorrectDisplayText1, kResultDisplayText1);
  EXPECT_EQ(kCorrectDisplayText2, kResultDisplayText2);

  EXPECT_EQ(true, kPerformMsgParams[strings::mute_audio].asBool());
}

TEST_F(PerformAudioPassThruRequestTest,
       Run_InitPromptCorrect_SpeakAndPerformAPTRequestsSendMuteFalse) {
  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(mobile_api::HMILevel::HMI_FULL));

  msg_params_[strings::initial_prompt][0][strings::text] = kCorrectPrompt;
  msg_params_[strings::initial_prompt][0][strings::type] = kCorrectType;

  const bool kMuted = false;

  msg_params_[strings::mute_audio] = kMuted;

  MessageSharedPtr speak_reqeust_result_msg;
  MessageSharedPtr perform_result_msg;
  {
    InSequence dummy;
    // Send speak request sending
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&speak_reqeust_result_msg), Return(true)));

    // Perform audio path thru request sending
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&perform_result_msg), Return(true)));
  }
  CallRun caller(*command_sptr_);
  caller();

  EXPECT_EQ(
      kMuted,
      (*perform_result_msg)[strings::msg_params][strings::mute_audio].asBool());
}

TEST_F(
    PerformAudioPassThruRequestTest,
    Run_InitPromptEmpty_PerformAndRecordStartNotificationsAndStartRecording) {
  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(mobile_api::HMILevel::HMI_FULL));

  MessageSharedPtr start_record_result_msg;
  MessageSharedPtr perform_result_msg;
  {
    InSequence dummy;
    // Perform audio path thru request sending
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&perform_result_msg), Return(true)));

    // Start recording notification sending
    EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
        .WillOnce(DoAll(SaveArg<0>(&start_record_result_msg), Return(true)));
  }

  // Start microphone recording cals
  EXPECT_CALL(mock_app_manager_, BeginAudioPassThrough());
  EXPECT_CALL(mock_app_manager_, StartAudioPassThruThread(_, _, _, _, _, _));

  CallRun caller(*command_sptr_);
  caller();

  const hmi_apis::FunctionID::eType kStartRecordResultFunctionId =
      static_cast<hmi_apis::FunctionID::eType>(
          (*start_record_result_msg)[strings::params][strings::function_id]
              .asInt());
  EXPECT_EQ(hmi_apis::FunctionID::UI_OnRecordStart,
            kStartRecordResultFunctionId);
}

TEST_F(PerformAudioPassThruRequestTest, OnEvent_UIPAPT_Rejected) {
  event_engine::Event event(hmi_apis::FunctionID::UI_PerformAudioPassThru);

  (*message_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::REJECTED;
  event.set_smart_object(*message_);

  CallOnEvent caller(*command_sptr_, event);

  MessageSharedPtr result_message = CatchMobileCommandResult(caller);

  const mobile_apis::Result::eType kResultCode =
      static_cast<mobile_api::Result::eType>(
          (*result_message)[strings::msg_params][strings::result_code].asInt());

  EXPECT_EQ(mobile_apis::Result::REJECTED, kResultCode);
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_TTSSpeakSuccess_UpdateRequestTimeout) {
  event_engine::Event event(hmi_apis::FunctionID::TTS_Speak);

  (*message_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  event.set_smart_object(*message_);

  // Start recording notification sending
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(_)).WillOnce(Return(true));

  // Start microphone recording cals
  EXPECT_CALL(mock_app_manager_, BeginAudioPassThrough());
  EXPECT_CALL(mock_app_manager_, StartAudioPassThruThread(_, _, _, _, _, _));

  EXPECT_CALL(mock_app_manager_, updateRequestTimeout(_, _, _));

  CallOnEvent caller(*command_sptr_, event);
  caller();
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_PAPTunsupportedResource_CorrectInfo) {
  const std::string kReturnInfo = "Unsupported phoneme type sent in a prompt";

  event_engine::Event event_speak(hmi_apis::FunctionID::TTS_Speak);
  event_engine::Event event_perform(
      hmi_apis::FunctionID::UI_PerformAudioPassThru);

  (*message_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  event_speak.set_smart_object(*message_);

  (*message_)[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  event_perform.set_smart_object(*message_);

  // First call on_event for setting result_tts_speak_ to UNSUPPORTED_RESOURCE
  EXPECT_CALL(mock_app_manager_, updateRequestTimeout(_, _, _)).Times(0);
  CallOnEvent caller_speak(*command_sptr_, event_speak);
  caller_speak();

  // Second call for test correct behavior of UI_PerformAudioPassThru event
  EXPECT_CALL(mock_app_manager_, EndAudioPassThrough()).WillOnce(Return(false));
  EXPECT_CALL(mock_app_manager_, StopAudioPassThru(_)).Times(0);

  CallOnEvent caller_perform(*command_sptr_, event_perform);

  MessageSharedPtr perform_event_result =
      CatchMobileCommandResult(caller_perform);

  EXPECT_EQ(
      kReturnInfo,
      (*perform_event_result)[strings::msg_params][strings::info].asString());
}

TEST_F(PerformAudioPassThruRequestTest,
       OnEvent_TTSOnResetTimeout_UpdateTimeout) {
  event_engine::Event event(hmi_apis::FunctionID::TTS_Speak);

  EXPECT_CALL(mock_app_manager_, updateRequestTimeout(_, _, _));

  CallOnEvent caller(*command_sptr_, event);
  caller();
}

TEST_F(PerformAudioPassThruRequestTest, OnEvent_DefaultCase) {
  event_engine::Event event(hmi_apis::FunctionID::INVALID_ENUM);

  EXPECT_CALL(mock_app_manager_, updateRequestTimeout(_, _, _)).Times(0);
  EXPECT_CALL(mock_app_manager_, EndAudioPassThrough()).Times(0);

  CallOnEvent caller(*command_sptr_, event);
  caller();
}

TEST_F(PerformAudioPassThruRequestTest, Init_CorrectTimeout) {
  const uint32_t kDefaultTimeout = command_sptr_->default_timeout();
  const uint32_t kMaxDuration = 10000u;

  msg_params_[strings::max_duration] = kMaxDuration;

  command_sptr_->Init();

  EXPECT_EQ(kDefaultTimeout + kMaxDuration, command_sptr_->default_timeout());
}

TEST_F(PerformAudioPassThruRequestTest,
       onTimeOut_ttsSpeakNotActive_DontSendHMIReqeust) {
  EXPECT_CALL(mock_app_manager_, EndAudioPassThrough()).WillOnce(Return(true));
  EXPECT_CALL(mock_app_manager_, StopAudioPassThru(_));

  // For setting current_state_ -> kCompleted
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _));
  command_sptr_->SendResponse(true, mobile_api::Result::SUCCESS);

  EXPECT_CALL(mock_app_manager_, ManageHMICommand(_)).Times(0);

  command_sptr_->onTimeOut();
}

TEST_F(PerformAudioPassThruRequestTest,
       onTimeOut_ttsSpeakActive_SendHMIReqeust) {
  EXPECT_CALL(mock_app_manager_, EndAudioPassThrough()).WillOnce(Return(true));
  EXPECT_CALL(mock_app_manager_, StopAudioPassThru(_));

  EXPECT_CALL(*application_sptr_, hmi_level())
      .WillOnce(Return(mobile_api::HMILevel::HMI_FULL));

  msg_params_[strings::initial_prompt][0][strings::text] = kCorrectPrompt;
  msg_params_[strings::initial_prompt][0][strings::type] = kCorrectType;

  // For setting is_active_tts_speak -> true
  EXPECT_CALL(mock_app_manager_, ManageHMICommand(_))
      .Times(2)
      .WillRepeatedly(Return(true));
  CallRun caller(*command_sptr_);
  caller();

  // For setting current_state_ -> kCompleted
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _));
  command_sptr_->SendResponse(true, mobile_api::Result::SUCCESS);

  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_StopSpeaking)))
      .WillOnce(Return(true));
  command_sptr_->onTimeOut();
}

}  // namespace perform_audio_pass_thru_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

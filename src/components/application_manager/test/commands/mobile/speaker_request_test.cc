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

#include <string>
#include "gtest/gtest.h"
#include "mobile/speak_request.h"
#include "utils/shared_ptr.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/smart_object_keys.h"
#include "mock_message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace speak_request {

using application_manager::commands::SpeakRequest;
using ::testing::DefaultValue;
using application_manager::MockMessageHelper;

namespace mobile_result = mobile_apis::Result;
namespace am = application_manager;

typedef SharedPtr<SpeakRequest> CommandPtr;

namespace {
const uint32_t kAppId = 10u;
const uint32_t kConnectionKey = 5u;
}

class SpeakRequestTest : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SpeakRequestTest()
      : app_(CreateMockApp())
      , msg_(CreateMessage(smart_objects::SmartType_Map))
      , mock_message_helper_(*am::MockMessageHelper::message_helper_mock()) {
    testing::Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

 protected:
  MockAppPtr app_;
  MessageSharedPtr msg_;
  am::MockMessageHelper& mock_message_helper_;
};

TEST_F(SpeakRequestTest, Run_ApplicationIsNotRegistered) {
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_))
      .WillByDefault(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(SpeakRequestTest, Run_MsgWithWhiteSpace_InvalidData) {
  (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
         [am::strings::text] = " ";
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(SpeakRequestTest, Run_MsgWithIncorrectChar1_InvalidData) {
  (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
         [am::strings::text] = "sd\\t";
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(SpeakRequestTest, Run_MsgWithIncorrectChar2_InvalidData) {
  (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
         [am::strings::text] = "sd\\n";
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(SpeakRequestTest, Run_MsgWithIncorrectChar3_InvalidData) {
  (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
         [am::strings::text] = "sd\tdf";
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(SpeakRequestTest, Run_MsgWithIncorrectChar4_InvalidData) {
  (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
         [am::strings::text] = "sd\n rer";
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(SpeakRequestTest, Run_MsgWithIncorrectCharInfirstPlace_InvalidData) {
  (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
         [am::strings::text] = "\n";
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _));

  command->Run();
}

TEST_F(SpeakRequestTest, Run_MsgWithEmptyString_Success) {
  (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
         [am::strings::text] = "";
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));
  ON_CALL(*app_, app_id()).WillByDefault(Return(kAppId));

  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_Speak)));

  command->Run();
}

TEST_F(SpeakRequestTest, Run_MsgCorrect_Success) {
  (*msg_)[am::strings::msg_params][am::strings::tts_chunks][0]
         [am::strings::text] = "asda";
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));
  ON_CALL(*app_, app_id()).WillByDefault(Return(kAppId));

  EXPECT_CALL(
      mock_app_manager_,
      ManageHMICommand(HMIResultCodeIs(hmi_apis::FunctionID::TTS_Speak)));

  command->Run();
}

TEST_F(SpeakRequestTest, OnEvent_WrongEventId_UNSUCCESS) {
  Event event(Event::EventID::INVALID_ENUM);
  CommandPtr command(CreateCommand<SpeakRequest>());

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  command->on_event(event);
}

TEST_F(SpeakRequestTest, OnEvent_TTS_Speak_SUCCESS) {
  Event event(Event::EventID::TTS_Speak);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  hmi_apis::Common_Result::eType hmi_result = hmi_apis::Common_Result::SUCCESS;
  (*event_msg)[am::strings::msg_params][am::strings::tts_chunks][0]
              [am::strings::text] = "1234";
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_result;
  (*event_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  event.set_smart_object(*event_msg);
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(hmi_result))
      .WillOnce(Return(am::mobile_api::Result::SUCCESS));
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));
  command->on_event(event);
}

TEST_F(SpeakRequestTest, OnEvent_TTS_SpeakWithWarning_WarningWithSuccess) {
  Event event(Event::EventID::TTS_Speak);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  hmi_apis::Common_Result::eType hmi_result = hmi_apis::Common_Result::WARNINGS;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_result;
  (*event_msg)[am::strings::msg_params][am::strings::tts_chunks][0]
              [am::strings::text] = "asda";
  event.set_smart_object(*event_msg);
  CommandPtr command(CreateCommand<SpeakRequest>());

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));

  const std::string info = "Unsupported phoneme type sent in a prompt";
  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(hmi_result))
      .WillOnce(Return(am::mobile_api::Result::WARNINGS));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::WARNINGS, info, true), _));
  command->on_event(event);
}

TEST_F(SpeakRequestTest,
       OnEvent_TTS_SpeakWithUnsupportedResourse_WarningWithoutSuccess) {
  Event event(Event::EventID::TTS_Speak);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  hmi_apis::Common_Result::eType hmi_result =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_result;
  (*event_msg)[am::strings::msg_params][am::strings::tts_chunks][0]
              [am::strings::text] = "asda";
  event.set_smart_object(*event_msg);
  CommandPtr command(CreateCommand<SpeakRequest>());

  ON_CALL(mock_app_manager_, application(_)).WillByDefault(Return(app_));

  const std::string info = "Unsupported phoneme type sent in a prompt";
  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(hmi_result))
      .WillOnce(Return(am::mobile_api::Result::UNSUPPORTED_RESOURCE));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::WARNINGS, info, false), _));
  command->on_event(event);
}

TEST_F(SpeakRequestTest, OnEvent_TTS_OnResetTimeout_UpdateTimeout) {
  Event event(Event::EventID::TTS_OnResetTimeout);
  (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_)[am::strings::params][am::strings::correlation_id] = kAppId;
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  EXPECT_CALL(mock_app_manager_,
              updateRequestTimeout(kConnectionKey, kAppId, _));

  command->on_event(event);
}

TEST_F(SpeakRequestTest, OnEvent_ApplicationIsNotRegistered_UNSUCCESS) {
  const hmi_apis::Common_Result::eType hmi_result =
      hmi_apis::Common_Result::SUCCESS;

  Event event(Event::EventID::TTS_Speak);
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::msg_params][am::strings::tts_chunks][0]
              [am::strings::text] = "text";
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_result;
  (*event_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  event.set_smart_object(*event_msg);
  CommandPtr command(CreateCommand<SpeakRequest>(msg_));

  EXPECT_CALL(mock_app_manager_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  command->on_event(event);
}

}  // namespace speak_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

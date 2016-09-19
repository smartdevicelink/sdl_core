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
#include <vector>

#include "mobile/reset_global_properties_request.h"
#include "mobile/reset_global_properties_response.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "smart_objects/smart_object.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

namespace am = ::application_manager;

using am::commands::ResetGlobalPropertiesRequest;
using am::commands::ResetGlobalPropertiesResponse;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using am::MockMessageHelper;

typedef SharedPtr<ResetGlobalPropertiesRequest> ResetGlobalPropertiesRequestPtr;
typedef SharedPtr<ResetGlobalPropertiesResponse>
    ResetGlobalPropertiesResponsePtr;

namespace {
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 10u;
}  // namespace

class ResetGlobalPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    message_ = CreateMessage();
    command_ = CreateCommand<ResetGlobalPropertiesRequest>(message_);
    app_ = CreateMockApp();
    ON_CALL(*app_, app_id()).WillByDefault(Return(kConnectionKey));
  }
  ResetGlobalPropertiesRequestPtr command_;
  MessageSharedPtr message_;
  MockAppPtr app_;
};

class ResetGlobalPropertiesResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(ResetGlobalPropertiesRequestTest, Run_InvalidApp_UNSUCCESS) {
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  MockAppPtr invalid_app;
  EXPECT_CALL(mock_app_manager_, application(_)).WillOnce(Return(invalid_app));

  MessageSharedPtr command_result;
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&command_result), Return(true)));

  command_->Run();
  EXPECT_EQ(
      (*command_result)[am::strings::msg_params][am::strings::success].asBool(),
      false);
  EXPECT_EQ(
      (*command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(mobile_apis::Result::APPLICATION_NOT_REGISTERED));
}

TEST_F(ResetGlobalPropertiesRequestTest, Run_SUCCESS) {
  (*message_)[am::strings::msg_params][am::strings::properties][0] =
      mobile_apis::GlobalProperty::HELPPROMPT;
  (*message_)[am::strings::msg_params][am::strings::properties][1] =
      mobile_apis::GlobalProperty::TIMEOUTPROMPT;
  (*message_)[am::strings::msg_params][am::strings::properties][2] =
      mobile_apis::GlobalProperty::VRHELPTITLE;
  (*message_)[am::strings::msg_params][am::strings::properties][3] =
      mobile_apis::GlobalProperty::MENUNAME;
  (*message_)[am::strings::msg_params][am::strings::properties][4] =
      mobile_apis::GlobalProperty::MENUICON;
  (*message_)[am::strings::msg_params][am::strings::properties][5] =
      mobile_apis::GlobalProperty::KEYBOARDPROPERTIES;

  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app_));
  EXPECT_CALL(mock_app_manager_,
              RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  smart_objects::SmartObject so_prompt =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  EXPECT_CALL(*app_, set_help_prompt(so_prompt));

  EXPECT_CALL(mock_app_manager_, get_settings())
      .WillOnce(ReturnRef(mock_app_manager_settings_));

  std::vector<std::string> time_out_prompt;
  time_out_prompt.push_back("time_out");
  EXPECT_CALL(mock_app_manager_settings_, time_out_promt())
      .WillOnce(ReturnRef(time_out_prompt));

  smart_objects::SmartObject timeout_prompt =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  timeout_prompt[am::strings::text] = time_out_prompt[0];
  timeout_prompt[am::strings::type] =
      hmi_apis::Common_SpeechCapabilities::SC_TEXT;

  smart_objects::SmartObject so_time_out_prompt =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  so_time_out_prompt[0] = timeout_prompt;

  EXPECT_CALL(*app_, set_timeout_prompt(so_time_out_prompt));

  EXPECT_CALL(*app_, reset_vr_help_title());
  EXPECT_CALL(*app_, reset_vr_help());

  EXPECT_CALL(*app_, set_reset_global_properties_active(true));

  smart_objects::SmartObjectSPtr vr_help =
      ::utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  EXPECT_CALL((*am::MockMessageHelper::message_helper_mock()),
              CreateAppVrHelp(_)).WillOnce(Return(vr_help));

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[am::hmi_request::menu_title] = "";

  EXPECT_CALL(*app_, set_menu_title(msg_params[am::hmi_request::menu_title]));

  const smart_objects::SmartObjectSPtr so_help_prompt =
      ::utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  EXPECT_CALL(*app_, help_prompt()).WillOnce(Return(so_help_prompt.get()));
  EXPECT_CALL(*app_, timeout_prompt()).WillOnce(Return(so_help_prompt.get()));

  EXPECT_CALL(mock_app_manager_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetGlobalProperties)))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_app_manager_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::TTS_SetGlobalProperties)))
      .WillOnce(Return(true));

  command_->Run();
}

TEST_F(ResetGlobalPropertiesRequestTest, OnEvent_InvalidEventId_UNSUCCESS) {
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  Event event(hmi_apis::FunctionID::INVALID_ENUM);

  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app_));
  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  command_->on_event(event);
}

TEST_F(ResetGlobalPropertiesRequestTest,
       OnEvent_UI_SetGlobalProperties_SUCCESS) {
  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::SUCCESS;

  (*message_)[am::strings::msg_params][am::strings::properties][0] =
      mobile_apis::GlobalProperty::VRHELPTITLE;

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  EXPECT_CALL(*app_, reset_vr_help_title());
  EXPECT_CALL(*app_, reset_vr_help());

  EXPECT_CALL(*app_, set_reset_global_properties_active(true));

  smart_objects::SmartObjectSPtr vr_help =
      ::utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  EXPECT_CALL((*am::MockMessageHelper::message_helper_mock()),
              CreateAppVrHelp(_)).WillOnce(Return(vr_help));

  ON_CALL(mock_app_manager_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));

  command_->Run();

  event.set_smart_object(*message_);

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::eType::SUCCESS),
                  am::commands::Command::ORIGIN_SDL));
  EXPECT_CALL(*app_, UpdateHash());

  command_->on_event(event);
}

TEST_F(ResetGlobalPropertiesRequestTest,
       OnEvent_TTS_SetGlobalProperties_SUCCESS) {
  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*message_)[am::strings::params][am::strings::correlation_id] =
      kCorrelationId;
  (*message_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::UNSUPPORTED_RESOURCE;

  (*message_)[am::strings::msg_params][am::strings::properties][0] =
      mobile_apis::GlobalProperty::VRHELPTITLE;

  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  EXPECT_CALL(*app_, reset_vr_help_title());
  EXPECT_CALL(*app_, reset_vr_help());

  EXPECT_CALL(*app_, set_reset_global_properties_active(true));

  smart_objects::SmartObjectSPtr vr_help =
      ::utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);
  EXPECT_CALL((*am::MockMessageHelper::message_helper_mock()),
              CreateAppVrHelp(_)).WillOnce(Return(vr_help));

  ON_CALL(mock_app_manager_, GetNextHMICorrelationID())
      .WillByDefault(Return(kCorrelationId));

  command_->Run();

  event.set_smart_object(*message_);

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_apis::Result::eType::UNSUPPORTED_RESOURCE),
          am::commands::Command::ORIGIN_SDL));
  EXPECT_CALL(*app_, UpdateHash()).Times(0);

  command_->on_event(event);
}

TEST_F(ResetGlobalPropertiesRequestTest, OnEvent_PendingRequest_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  (*message_)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(app_));
  event.set_smart_object(*message_);

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);
  EXPECT_CALL(*app_, UpdateHash()).Times(0);

  command_->on_event(event);
}

TEST_F(ResetGlobalPropertiesResponseTest, Run_SendMessage_SUCCESS) {
  MessageSharedPtr message(CreateMessage());
  ResetGlobalPropertiesResponsePtr command(
      CreateCommand<ResetGlobalPropertiesResponse>(message));

  EXPECT_CALL(mock_app_manager_, SendMessageToMobile(message, _));
  command->Run();
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

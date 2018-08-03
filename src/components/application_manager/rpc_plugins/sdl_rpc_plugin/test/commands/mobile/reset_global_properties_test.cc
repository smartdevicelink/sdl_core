/*
 * Copyright (c) 2018, Ford Motor Company
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
namespace reset_global_properties {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

namespace am = ::application_manager;

using sdl_rpc_plugin::commands::ResetGlobalPropertiesRequest;
using sdl_rpc_plugin::commands::ResetGlobalPropertiesResponse;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using am::MockMessageHelper;

typedef std::shared_ptr<ResetGlobalPropertiesRequest>
    ResetGlobalPropertiesRequestPtr;
typedef std::shared_ptr<ResetGlobalPropertiesResponse>
    ResetGlobalPropertiesResponsePtr;

namespace {
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 10u;
}  // namespace

class ResetGlobalPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  ResetGlobalPropertiesRequestTest()
      : msg_(CreateMessage()), mock_app_(CreateMockApp()) {}

  void SetUp() OVERRIDE {
    (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg_)[am::strings::params][am::strings::correlation_id] = kCorrelationId;

    command_ = CreateCommand<ResetGlobalPropertiesRequest>(msg_);

    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(app_mngr_, GetNextHMICorrelationID())
        .WillByDefault(Return(kCorrelationId));
  }

  MessageSharedPtr msg_;
  MockAppPtr mock_app_;
  ResetGlobalPropertiesRequestPtr command_;
};

class ResetGlobalPropertiesResponseTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {};

TEST_F(ResetGlobalPropertiesRequestTest, Run_InvalidApp_UNSUCCESS) {
  MockAppPtr invalid_app;
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(invalid_app));

  MessageSharedPtr command_result;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
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

TEST_F(ResetGlobalPropertiesRequestTest, Run_InvalidVrHelp_UNSUCCESS) {
  am::ResetGlobalPropertiesResult result;
  result.help_prompt = true;
  result.timeout_prompt = true;
  result.vr_help_title_items = true;
  result.menu_name = true;
  result.menu_icon = true;
  result.keyboard_properties = true;

  EXPECT_CALL(app_mngr_, ResetGlobalProperties(_, _)).WillOnce(Return(result));

  EXPECT_CALL(mock_message_helper_, CreateUIResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(smart_objects::SmartObjectSPtr()));

  ON_CALL(mock_message_helper_, CreateTTSResetGlobalPropertiesRequest(_, _))
      .WillByDefault(Return(std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map)));

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(1);

  command_->Run();
}

TEST_F(ResetGlobalPropertiesRequestTest, Run_SUCCESS) {
  am::ResetGlobalPropertiesResult result;
  result.help_prompt = true;
  result.timeout_prompt = true;
  result.vr_help_title_items = true;
  result.menu_name = true;
  result.menu_icon = true;
  result.keyboard_properties = true;
  result.number_of_reset_vr = 1;

  ON_CALL(app_mngr_, ResetGlobalProperties(_, _)).WillByDefault(Return(result));

  EXPECT_CALL(*mock_app_, set_reset_global_properties_active(true));

  smart_objects::SmartObjectSPtr msg_params =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  EXPECT_CALL(mock_message_helper_, CreateTTSResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));
  EXPECT_CALL(mock_message_helper_, CreateUIResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetGlobalProperties)))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::TTS_SetGlobalProperties)))
      .WillOnce(Return(true));

  command_->Run();
}

TEST_F(ResetGlobalPropertiesRequestTest, OnEvent_InvalidEventId_UNSUCCESS) {
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);
  command_->on_event(event);
}

TEST_F(ResetGlobalPropertiesRequestTest,
       OnEvent_UI_SetGlobalProperties_SUCCESS) {
  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  const hmi_apis::Common_Result::eType result_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg_)[am::strings::params][am::hmi_response::code] = result_code;

  am::ResetGlobalPropertiesResult result;
  result.vr_help_title_items = true;

  EXPECT_CALL(app_mngr_, ResetGlobalProperties(_, _)).WillOnce(Return(result));

  smart_objects::SmartObjectSPtr msg_params =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  ON_CALL(mock_message_helper_, CreateTTSResetGlobalPropertiesRequest(_, _))
      .WillByDefault(Return(msg_params));

  EXPECT_CALL(mock_message_helper_, CreateUIResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));

  smart_objects::SmartObjectSPtr vr_help =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetGlobalProperties)))
      .WillOnce(Return(true));

  command_->Run();

  event.set_smart_object(*msg_);
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::eType::SUCCESS),
                  am::commands::Command::SOURCE_SDL));

  command_->on_event(event);
}

TEST_F(ResetGlobalPropertiesRequestTest,
       OnEvent_TTS_SetGlobalProperties_SUCCESS) {
  Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::UNSUPPORTED_RESOURCE;

  (*msg_)[am::strings::msg_params][am::strings::properties][0] =
      mobile_apis::GlobalProperty::TIMEOUTPROMPT;
  (*msg_)[am::strings::msg_params][am::strings::properties][1] =
      mobile_apis::GlobalProperty::MENUICON;

  am::ResetGlobalPropertiesResult result;
  result.timeout_prompt = true;
  result.menu_icon = true;

  EXPECT_CALL(app_mngr_, ResetGlobalProperties(_, _)).WillOnce(Return(result));

  EXPECT_CALL(*mock_app_, set_reset_global_properties_active(true));

  smart_objects::SmartObjectSPtr msg_params =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  EXPECT_CALL(mock_message_helper_, CreateTTSResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));
  EXPECT_CALL(mock_message_helper_, CreateUIResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));

  MessageSharedPtr ui_msg = CreateMessage();
  (*ui_msg)[am::strings::params][am::strings::correlation_id] = kCorrelationId;
  (*ui_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::SUCCESS;
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetGlobalProperties)))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::TTS_SetGlobalProperties)))
      .WillOnce(Return(true));
  Event ui_event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  ui_event.set_smart_object(*ui_msg);

  command_->Run();
  command_->on_event(ui_event);
  event.set_smart_object(*msg_);
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::WARNINGS),
                          am::commands::Command::SOURCE_SDL));

  command_->on_event(event);
}

TEST_F(ResetGlobalPropertiesResponseTest, Run_Sendmsg_SUCCESS) {
  MessageSharedPtr message(CreateMessage());
  ResetGlobalPropertiesResponsePtr command(
      CreateCommand<ResetGlobalPropertiesResponse>(message));

  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(message, _));
  command->Run();
}

TEST_F(ResetGlobalPropertiesRequestTest, OnEvent_InvalidApp_NoHashUpdate) {
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::SUCCESS;

  (*msg_)[am::strings::msg_params][am::strings::properties][0] =
      mobile_apis::GlobalProperty::VRHELPTITLE;

  am::ResetGlobalPropertiesResult result;
  result.vr_help_title_items = true;

  EXPECT_CALL(app_mngr_, ResetGlobalProperties(_, _)).WillOnce(Return(result));

  EXPECT_CALL(*mock_app_, set_reset_global_properties_active(true));

  smart_objects::SmartObjectSPtr msg_params =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  EXPECT_CALL(mock_message_helper_, CreateTTSResetGlobalPropertiesRequest(_, _))
      .Times(0);
  EXPECT_CALL(mock_message_helper_, CreateUIResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetGlobalProperties)))
      .WillOnce(Return(true));
  smart_objects::SmartObjectSPtr vr_help =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);

  ResetGlobalPropertiesRequestPtr command =
      CreateCommand<ResetGlobalPropertiesRequest>(msg_);
  command->Run();
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::eType::SUCCESS),
                  am::commands::Command::SOURCE_SDL));

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg_);
  command->on_event(event);
}

TEST_F(ResetGlobalPropertiesRequestTest,
       Run_WaitTTS_Timeout_GENERIC_ERROR_TTSNotRespond) {
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::UNSUPPORTED_RESOURCE;

  (*msg_)[am::strings::msg_params][am::strings::properties][0] =
      mobile_apis::GlobalProperty::TIMEOUTPROMPT;
  (*msg_)[am::strings::msg_params][am::strings::properties][1] =
      mobile_apis::GlobalProperty::MENUICON;

  am::ResetGlobalPropertiesResult result;
  result.timeout_prompt = true;
  result.menu_icon = true;

  EXPECT_CALL(app_mngr_, ResetGlobalProperties(_, _)).WillOnce(Return(result));

  EXPECT_CALL(*mock_app_, set_reset_global_properties_active(true));

  smart_objects::SmartObjectSPtr msg_params =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  EXPECT_CALL(mock_message_helper_, CreateTTSResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));
  EXPECT_CALL(mock_message_helper_, CreateUIResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetGlobalProperties)))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::TTS_SetGlobalProperties)))
      .WillOnce(Return(true));

  ResetGlobalPropertiesRequestPtr command =
      CreateCommand<ResetGlobalPropertiesRequest>(msg_);
  command->Run();

  // Received response only from UI
  MessageSharedPtr ui_msg = CreateMessage();
  (*ui_msg)[am::strings::params][am::strings::correlation_id] = kCorrelationId;
  (*ui_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::SUCCESS;
  (*ui_msg)[am::strings::msg_params] =
      SmartObject(smart_objects::SmartType_Map);
  Event ui_event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  ui_event.set_smart_object(*ui_msg);
  command->on_event(ui_event);

  // TTS doesn't respond, so timeout should send generic error
  smart_objects::SmartObjectSPtr response =
      std::make_shared<smart_objects::SmartObject>();
  (*response)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::GENERIC_ERROR;
  EXPECT_CALL(mock_message_helper_, CreateNegativeResponse(_, _, _, _))
      .WillOnce(Return(response));
  const std::string info = "TTS component does not respond";
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResponseIs(mobile_apis::Result::GENERIC_ERROR, info, false),
          am::commands::Command::SOURCE_SDL));
  command->onTimeOut();
}

TEST_F(ResetGlobalPropertiesRequestTest,
       Run_WaitUI_Timeout_GENERIC_ERROR_UINotRespond) {
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::UNSUPPORTED_RESOURCE;

  (*msg_)[am::strings::msg_params][am::strings::properties][0] =
      mobile_apis::GlobalProperty::TIMEOUTPROMPT;
  (*msg_)[am::strings::msg_params][am::strings::properties][1] =
      mobile_apis::GlobalProperty::MENUICON;

  am::ResetGlobalPropertiesResult result;
  result.timeout_prompt = true;
  result.menu_icon = true;

  EXPECT_CALL(app_mngr_, ResetGlobalProperties(_, _)).WillOnce(Return(result));

  smart_objects::SmartObjectSPtr msg_params =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  EXPECT_CALL(mock_message_helper_, CreateTTSResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));
  EXPECT_CALL(mock_message_helper_, CreateUIResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));

  EXPECT_CALL(*mock_app_, set_reset_global_properties_active(true));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetGlobalProperties)))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::TTS_SetGlobalProperties)))
      .WillOnce(Return(true));

  command_->Run();

  // Received response only from TTS
  MessageSharedPtr tts_msg = CreateMessage();
  (*tts_msg)[am::strings::params][am::strings::correlation_id] = kCorrelationId;
  (*tts_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::SUCCESS;

  Event tts_event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  tts_event.set_smart_object(*tts_msg);
  command_->on_event(tts_event);

  // UI doesn't respond, so timeout should send generic error
  smart_objects::SmartObjectSPtr response =
      std::make_shared<smart_objects::SmartObject>();
  (*response)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::GENERIC_ERROR;
  EXPECT_CALL(mock_message_helper_, CreateNegativeResponse(_, _, _, _))
      .WillOnce(Return(response));

  const std::string info = "UI component does not respond";
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResponseIs(mobile_apis::Result::GENERIC_ERROR, info, false),
          am::commands::Command::SOURCE_SDL));
  command_->onTimeOut();
}

TEST_F(ResetGlobalPropertiesRequestTest,
       Run_WaitUIAndTTS_Timeout_GENERIC_ERROR_TTSAndUINotRespond) {
  Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  (*msg_)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::UNSUPPORTED_RESOURCE;

  (*msg_)[am::strings::msg_params][am::strings::properties][0] =
      mobile_apis::GlobalProperty::TIMEOUTPROMPT;
  (*msg_)[am::strings::msg_params][am::strings::properties][1] =
      mobile_apis::GlobalProperty::MENUICON;

  am::ResetGlobalPropertiesResult result;
  result.timeout_prompt = true;
  result.menu_icon = true;

  EXPECT_CALL(app_mngr_, ResetGlobalProperties(_, _)).WillOnce(Return(result));

  smart_objects::SmartObjectSPtr msg_params =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Map);

  EXPECT_CALL(mock_message_helper_, CreateTTSResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));
  EXPECT_CALL(mock_message_helper_, CreateUIResetGlobalPropertiesRequest(_, _))
      .WillOnce(Return(msg_params));

  EXPECT_CALL(*mock_app_, set_reset_global_properties_active(true));

  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetGlobalProperties)))
      .WillOnce(Return(true));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::TTS_SetGlobalProperties)))
      .WillOnce(Return(true));

  command_->Run();
  // TTS and UI don't respond, so timeout should send generic error
  std::string info = "TTS, UI component does not respond";
  smart_objects::SmartObjectSPtr response =
      std::make_shared<smart_objects::SmartObject>();
  (*response)[am::strings::msg_params][am::strings::result_code] =
      mobile_apis::Result::GENERIC_ERROR;
  EXPECT_CALL(mock_message_helper_, CreateNegativeResponse(_, _, _, _))
      .WillOnce(Return(response));

  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResponseIs(mobile_apis::Result::GENERIC_ERROR, info, false),
          am::commands::Command::SOURCE_SDL));
  command_->onTimeOut();
}

}  // namespace reset_global_properties
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

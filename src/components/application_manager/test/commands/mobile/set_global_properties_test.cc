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

#include "application_manager/commands/mobile/set_global_properties_request.h"

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
namespace set_global_properties_request {

namespace am = application_manager;
using am::commands::SetGlobalPropertiesRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using am::CommandsMap;
using utils::custom_string::CustomString;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 1u;
const std::string kText = "one";
const uint32_t kPosition = 1u;
}  // namespace

class SetGlobalPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SetGlobalPropertiesRequestTest()
      : lock_ptr_(std::make_shared<sync_primitives::Lock>())
      , mock_app_(CreateMockApp()) {}

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
    msg_params[am::strings::vr_help_title] = "vr_help_title";
    msg_params[am::strings::vr_help][0][am::strings::text] = "vr_help";
    msg_params[am::strings::vr_help][0][am::strings::position] = 1u;
    msg_params[am::strings::help_prompt][0][am::strings::text] = "help_promt";
    (*msg)[am::strings::msg_params] = msg_params;

    return msg;
  }

  MessageSharedPtr CreateMsgParams() {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
    return msg;
  }

  void VRArraySetupHelper(MessageSharedPtr msg,
                          SmartObject& vr_help_title,
                          SmartObject& vr_help_array) {
    (*msg)[am::strings::msg_params][am::strings::vr_help_title] = vr_help_title;
    vr_help_array[0] = SmartObject(smart_objects::SmartType_Map);
    vr_help_array[0][am::strings::text] = kText;
    vr_help_array[0][am::strings::position] = kPosition;
    (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help_array;
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
  }

  void OnEventUISetupHelper(MessageSharedPtr msg,
                            SharedPtr<SetGlobalPropertiesRequest> command) {
    SmartObject vr_help_title("yes");
    SmartObject vr_help_array(smart_objects::SmartType_Array);
    VRArraySetupHelper(msg, vr_help_title, vr_help_array);
    EXPECT_CALL(mock_message_helper_,
                VerifyImageVrHelpItems(vr_help_array, _, _))
        .WillOnce((Return(mobile_apis::Result::SUCCESS)));
    EXPECT_CALL(app_mngr_,
                RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
    EXPECT_CALL(*mock_app_, set_vr_help_title(vr_help_title));
    EXPECT_CALL(*mock_app_, set_vr_help(vr_help_array));
    EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
    EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(&vr_help_array));
    EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_keyboard_props(_)).Times(0);
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

    command->Run();
  }

  void OnEventTTSSetupHelper(MessageSharedPtr msg,
                             SharedPtr<SetGlobalPropertiesRequest> command) {
    SmartObject help_prompt(smart_objects::SmartType_Array);
    help_prompt[0][am::strings::text] = "Help_Prompt_One";
    (*msg)[am::strings::msg_params][am::strings::help_prompt] = help_prompt;
    SmartObject timeout_prompt(smart_objects::SmartType_Array);
    timeout_prompt[0][am::strings::text] = "Timeout_Prompt_One";
    (*msg)[am::strings::msg_params][am::strings::timeout_prompt] =
        timeout_prompt;

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
    EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(help_prompt, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(timeout_prompt, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(app_mngr_,
                RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
    SmartObject vr_help_title("title");
    EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
    EXPECT_CALL(*mock_app_, set_help_prompt(help_prompt));
    EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(&help_prompt));
    EXPECT_CALL(*mock_app_, set_timeout_prompt(timeout_prompt));
    EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(&timeout_prompt));
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

    command->Run();
  }

  void EmptyExpectationsSetupHelper() {
    EXPECT_CALL(*mock_app_, set_vr_help_title(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_vr_help(_)).Times(0);
    EXPECT_CALL(*mock_app_, vr_help_title()).Times(0);
    EXPECT_CALL(*mock_app_, vr_help()).Times(0);
    EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_keyboard_props(_)).Times(0);
    EXPECT_CALL(*mock_app_, app_id()).Times(0);
  }

  void ExpectInvalidData() {
    EXPECT_CALL(rpc_service_,
                ManageMobileCommand(
                    MobileResultCodeIs(mobile_apis::Result::INVALID_DATA),
                    am::commands::Command::SOURCE_SDL));
  }

  void ExpectVerifyImageVrHelpSuccess(SmartObject& smart_obj) {
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(smart_obj, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  }

  void ExpectVerifyImageVrHelpUnsuccess() {
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
    EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  }

  void ExpectVerifyImageSuccess(SmartObject& smart_obj) {
    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_message_helper_, VerifyImage(smart_obj, _, _))
        .WillOnce(Return(mobile_apis::Result::SUCCESS));
    EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  }

  void SetUp() OVERRIDE {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(mock_hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
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

  void ExpectationsHmiInterface_Run() {
    EXPECT_CALL(
        mock_hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
        .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
    EXPECT_CALL(
        mock_hmi_interfaces_,
        GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
        .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
    ON_CALL(mock_hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
    ON_CALL(mock_hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));
  }
  std::shared_ptr<sync_primitives::Lock> lock_ptr_;
  MockAppPtr mock_app_;
};

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_UIHmiSendSuccess_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_vr = CreateFullParamsUISO();
  (*msg_vr)[am::strings::msg_params][am::strings::vr_commands][0] =
      "vr_command";

  utils::SharedPtr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(msg_vr);

  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  const smart_objects::SmartObject* vr_help_title =
      &((*msg_vr)[am::strings::msg_params][am::strings::vr_help_title]);
  const smart_objects::SmartObject* vr_help =
      &((*msg_vr)[am::strings::msg_params][am::strings::vr_help]);
  const smart_objects::SmartObject* vr_help_prompt =
      &((*msg_vr)[am::strings::msg_params][am::strings::help_prompt]);
  ON_CALL(*mock_app_, vr_help_title()).WillByDefault(Return(vr_help_title));
  ON_CALL(*mock_app_, vr_help()).WillByDefault(Return(vr_help));
  ON_CALL(*mock_app_, help_prompt()).WillByDefault(Return(vr_help_prompt));

  ExpectationsHmiInterface_Run();

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;
  (*msg)[am::strings::msg_params][am::strings::info] =
      "UI is not supported by system";

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  ON_CALL(mock_message_helper_,
          VerifyImageVrHelpItems(
              (*msg_vr)[am::strings::msg_params][am::strings::vr_help], _, _))
      .WillByDefault(Return(mobile_apis::Result::SUCCESS));

  ON_CALL(mock_message_helper_, VerifyImage(_, _, _))
      .WillByDefault(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(
      mock_message_helper_,
      VerifyTtsFiles(
          (*msg_vr)[am::strings::msg_params][am::strings::help_prompt], _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::UI_SetGlobalProperties)))
      .WillOnce(Return(true));
  EXPECT_CALL(rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::TTS_SetGlobalProperties)))
      .WillOnce(Return(true));
  (*msg_vr)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  Event event_vr(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event_vr.set_smart_object(*msg_vr);

  command->Run();
  command->on_event(event_vr);

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->on_event(event);

  ResultCommandExpectations(ui_command_result, "UI is not supported by system");
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_SUCCESS_Expect_MessageNotSend) {
  MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
  (*response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[am::strings::msg_params][am::strings::info] = "test";

  am::event_engine::Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*response);

  utils::SharedPtr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  MockAppPtr mock_app(CreateMockApp());
  ON_CALL(app_mngr_, application(_)).WillByDefault(Return(mock_app));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .Times(0);
  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_UNSUPPORTED_RESOURCE_Expect_false) {
  MessageSharedPtr response = CreateMessage(smart_objects::SmartType_Map);
  (*response)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*response)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*response)[am::strings::msg_params][am::strings::info] = "qwe";

  am::event_engine::Event event_tts(
      hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  am::event_engine::Event event_ui(
      hmi_apis::FunctionID::UI_SetGlobalProperties);
  event_tts.set_smart_object(*response);
  utils::SharedPtr<SetGlobalPropertiesRequest> command =
      CreateCommand<SetGlobalPropertiesRequest>(response);

  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr response_to_mobile;
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&response_to_mobile), Return(true)));

  command->Run();
  command->on_event(event_ui);
  command->on_event(event_tts);

  EXPECT_EQ((*response_to_mobile)[am::strings::msg_params][am::strings::success]
                .asBool(),
            false);
  EXPECT_EQ(
      (*response_to_mobile)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(mobile_apis::Result::INVALID_DATA));
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRNoMenuAndKeyboard_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));

  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  OnEventUISetupHelper(msg, command);
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRWithMenuAndKeyboard_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help_array;
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;

  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EXPECT_CALL(*mock_app_, set_vr_help_title(vr_help_title));
  EXPECT_CALL(*mock_app_, set_vr_help(vr_help_array));
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(&vr_help_array));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));
  EXPECT_CALL(*mock_app_, set_menu_icon(menu_icon));
  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));

  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRBrokenMenuIcon_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  (*msg)[am::strings::msg_params][am::strings::vr_help_title] = vr_help_title;
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::ABORTED)));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRBrokenVRHelp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);

  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::ABORTED)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRIncorrectSyntax_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  SmartObject vr_help_title("wrong syntax string\\n");
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[am::strings::value] = "1";
  (*msg)[am::strings::msg_params][am::hmi_request::menu_icon] = menu_icon;
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);

  EXPECT_CALL(mock_message_helper_, VerifyImage(menu_icon, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRMissingTitle_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  vr_help_array[0] = SmartObject(smart_objects::SmartType_Map);
  vr_help_array[0][am::strings::text] = kText;
  vr_help_array[0][am::strings::position] = kPosition;
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help_array;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRMissingArray_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  (*msg)[am::strings::msg_params][am::strings::vr_help_title] = vr_help_title;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRWrongOrder_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);
  vr_help_array[1] = SmartObject(smart_objects::SmartType_Map);
  vr_help_array[1][am::strings::text] = "two";
  vr_help_array[1][am::strings::position] = 3u;
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help_array;

  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(vr_help_array, _, _))
      .WillOnce((Return(mobile_apis::Result::SUCCESS)));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVR_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title("Menu_Title");
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRCouldNotGenerate_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;
  SmartObject menu_title("Menu_Title");
  (*msg)[am::strings::msg_params][am::hmi_request::menu_title] = menu_title;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject* vr_help_title = NULL;
  CommandsMap commands_map;
  SmartObject empty_msg(smart_objects::SmartType_Map);
  commands_map.insert(std::pair<uint32_t, SmartObject*>(1u, &empty_msg));
  DataAccessor<CommandsMap> accessor(commands_map, lock_ptr_);
  EXPECT_CALL(*mock_app_, commands_map()).WillOnce(Return(accessor));
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(vr_help_title));
  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::INVALID_DATA),
                          am::commands::Command::SOURCE_SDL));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVRNoDataNoDefault_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title(smart_objects::SmartType_Null);
  EXPECT_CALL(*mock_app_, vr_help_title())
      .WillOnce(Return(&vr_help_title))
      .WillOnce(Return(&vr_help_title));

  CommandsMap commands_map;
  DataAccessor<CommandsMap> accessor(commands_map, lock_ptr_);
  EXPECT_CALL(*mock_app_, commands_map()).WillOnce(Return(accessor));
  const CustomString name("name");
  EXPECT_CALL(*mock_app_, name()).WillOnce(ReturnRef(name));
  EXPECT_CALL(*mock_app_, set_vr_help_title(SmartObject(name)));
  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(_));
  EXPECT_CALL(*mock_app_, app_id());

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVRNoDataDefaultCreated_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title(smart_objects::SmartType_Null);
  EXPECT_CALL(*mock_app_, vr_help_title())
      .Times(2)
      .WillRepeatedly(Return(&vr_help_title));

  CommandsMap commands_map;
  SmartObject command_text(smart_objects::SmartType_Map);
  commands_map[0] = &command_text;
  (*commands_map[0])[am::strings::vr_commands] = SmartObject("one");
  DataAccessor<CommandsMap> accessor(commands_map, lock_ptr_);
  EXPECT_CALL(*mock_app_, commands_map()).WillOnce(Return(accessor));
  EXPECT_CALL(*mock_app_, set_vr_help(_));
  const CustomString name("name");
  EXPECT_CALL(*mock_app_, name()).WillOnce(ReturnRef(name));
  EXPECT_CALL(*mock_app_, set_vr_help_title(SmartObject(name)));
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(&vr_help_array));
  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVRNoDataFromSynonyms_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[am::strings::msg_params][am::hmi_request::keyboard_properties] =
      keyboard_properties;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title(smart_objects::SmartType_Null);
  EXPECT_CALL(*mock_app_, vr_help_title())
      .Times(2)
      .WillRepeatedly(Return(&vr_help_title));

  CommandsMap commands_map;
  DataAccessor<CommandsMap> accessor(commands_map, lock_ptr_);
  EXPECT_CALL(*mock_app_, commands_map()).WillOnce(Return(accessor));
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  vr_help_array[0] = SmartObject(smart_objects::SmartType_Map);
  vr_help_array[0][am::strings::text] = kText;
  vr_help_array[0][am::strings::position] = kPosition;
  SmartObject vr_synonyms(smart_objects::SmartType_Array);
  vr_synonyms[0] = vr_help_array;
  const CustomString name("name");
  EXPECT_CALL(*mock_app_, name()).WillOnce(ReturnRef(name));
  EXPECT_CALL(*mock_app_, set_vr_help_title(SmartObject(name)));
  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSHelpAndTimeout_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject help_prompt(smart_objects::SmartType_Array);
  help_prompt[0][am::strings::text] = "Help_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::help_prompt] = help_prompt;
  SmartObject timeout_prompt(smart_objects::SmartType_Array);
  timeout_prompt[0][am::strings::text] = "Timeout_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::timeout_prompt] = timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(help_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(timeout_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title("title");
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, set_help_prompt(help_prompt));
  EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(&help_prompt));
  EXPECT_CALL(*mock_app_, set_timeout_prompt(timeout_prompt));
  EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(&timeout_prompt));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSOnlyHelp_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject help_prompt(smart_objects::SmartType_Array);
  help_prompt[0][am::strings::text] = "Help_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::help_prompt] = help_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(help_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title("title");
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, set_help_prompt(help_prompt));
  EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(&help_prompt));
  EXPECT_CALL(*mock_app_, set_timeout_prompt(_)).Times(0);
  EXPECT_CALL(*mock_app_, timeout_prompt()).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSOnlyTimeout_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject timeout_prompt(smart_objects::SmartType_Array);
  timeout_prompt[0][am::strings::text] = "Timeout_Prompt_One";
  (*msg)[am::strings::msg_params][am::strings::timeout_prompt] = timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(mock_message_helper_, VerifyTtsFiles(timeout_prompt, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title("title");
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, set_help_prompt(_)).Times(0);
  EXPECT_CALL(*mock_app_, help_prompt()).Times(0);
  EXPECT_CALL(*mock_app_, set_timeout_prompt(timeout_prompt));
  EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(&timeout_prompt));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));
  EXPECT_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillOnce(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSIncorrectSyntax_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject timeout_prompt(smart_objects::SmartType_Array);
  timeout_prompt[0][am::strings::text] = "Timeout_Prompt_One\\n";
  (*msg)[am::strings::msg_params][am::strings::timeout_prompt] = timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidHelpPromptText_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject help_prompt(smart_objects::SmartType_Array);
  help_prompt[0][am::strings::text] =
      "invalid help prompt text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::help_prompt] = help_prompt;

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidVrHelpText_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help(smart_objects::SmartType_Array);
  vr_help[0][am::strings::text] =
      "invalid vr_help text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help;

  ExpectVerifyImageVrHelpSuccess(vr_help);
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidImageValue_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help(smart_objects::SmartType_Array);
  vr_help[0][am::strings::text] = "vr_help";
  vr_help[0][am::strings::image][am::strings::value] =
      "invalid value text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::vr_help] = vr_help;

  ExpectVerifyImageVrHelpSuccess(vr_help);
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidMenuIcon_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject menu_icon(smart_objects::SmartType_Array);
  menu_icon[am::strings::value] =
      "invalid menu icon text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::menu_icon] = menu_icon;

  ExpectVerifyImageSuccess(menu_icon);
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidMenuTitle_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject menu_title(smart_objects::SmartType_Array);
  menu_title = "invalid menu title text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::menu_title] = menu_title;

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_InvalidLimitedCharacterList_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject limited_character_list(smart_objects::SmartType_Array);
  limited_character_list[0] =
      "invalid limited character list text with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::keyboard_properties]
        [am::strings::limited_character_list] = limited_character_list;

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest,
       Run_InvalidAutoCompleteText_INVALID_DATA) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject auto_complete_text(smart_objects::SmartType_Array);
  auto_complete_text =
      "invalid auto completetext with empty line in the end\\n";
  (*msg)[am::strings::msg_params][am::strings::keyboard_properties]
        [am::strings::auto_complete_text] = auto_complete_text;

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  ExpectInvalidData();

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoData_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  ExpectVerifyImageVrHelpUnsuccess();
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;

  ExpectVerifyImageVrHelpUnsuccess();

  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_PendingRequest_UNSUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_UIAndSuccessResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;

  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));
  OnEventUISetupHelper(msg, command);

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .WillOnce(Return(true));

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_UIAndWarningResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::WARNINGS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));
  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  OnEventUISetupHelper(msg, command);

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .WillOnce(Return(true));

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::WARNINGS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;
  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::UI_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventUISetupHelper(msg, command);
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(MockAppPtr()));

  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_InvalidEventID_Canceled) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey)).Times(0);
  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);

  Event event(hmi_apis::FunctionID::TTS_Stopped);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_TTSAndSuccessResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;
  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventTTSSetupHelper(msg, command);
  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  EXPECT_CALL(rpc_service_,
              ManageMobileCommand(_, am::commands::Command::SOURCE_SDL))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_TTSAndWarningsResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  const hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::WARNINGS;
  (*msg)[am::strings::params][am::hmi_response::code] = response_code;
  ON_CALL(
      mock_hmi_interfaces_,
      GetInterfaceFromFunction(hmi_apis::FunctionID::TTS_SetGlobalProperties))
      .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_TTS));
  ON_CALL(mock_hmi_interfaces_,
          GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_TTS))
      .WillByDefault(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));
  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventTTSSetupHelper(msg, command);

  EXPECT_CALL(mock_hmi_interfaces_, GetInterfaceState(_))
      .WillRepeatedly(Return(am::HmiInterfaces::STATE_NOT_AVAILABLE));

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);

  EXPECT_EQ((*ui_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            true);
  EXPECT_EQ(
      (*ui_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(hmi_apis::Common_Result::WARNINGS));
}

}  // namespace set_global_properties_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

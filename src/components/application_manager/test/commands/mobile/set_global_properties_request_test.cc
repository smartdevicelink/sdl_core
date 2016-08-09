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
#include <vector>
#include <string>

#include "gtest/gtest.h"
#include "application_manager/commands/mobile/set_global_properties_request.h"
#include "utils/shared_ptr.h"
#include "utils/data_accessor.h"
#include "utils/custom_string.h"
#include "commands/command_request_test.h"
#include "commands/commands_test.h"
#include "smart_objects/smart_object.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/application.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using application_manager::commands::MessageSharedPtr;
using application_manager::commands::SetGlobalPropertiesRequest;
using application_manager::MockMessageHelper;
using application_manager::CommandsMap;
using application_manager::event_engine::Event;
using utils::SharedPtr;
using utils::custom_string::CustomString;
using smart_objects::SmartObject;
using testing::_;
using testing::Return;

namespace strings = application_manager::strings;
namespace hmi_request = application_manager::hmi_request;
namespace hmi_response = application_manager::hmi_response;

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kAppID = 2u;
const std::string kText = "one";
const uint32_t kPosition = 1u;
}  // namespace

class SetGlobalPropertiesRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SetGlobalPropertiesRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {
    mock_app_ = CreateMockApp();
  }

  MessageSharedPtr CreateMsgParams() {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*msg)[strings::msg_params][strings::app_id] = kAppID;
    return msg;
  }

  void VRArraySetupHelper(MessageSharedPtr msg,
                          SmartObject& vr_help_title,
                          SmartObject& vr_help_array) {
    (*msg)[strings::msg_params][strings::vr_help_title] = vr_help_title;
    vr_help_array[0] = SmartObject(smart_objects::SmartType_Map);
    vr_help_array[0][strings::text] = kText;
    vr_help_array[0][strings::position] = kPosition;
    (*msg)[strings::msg_params][strings::vr_help] = vr_help_array;
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
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

    command->Run();
  }

  void OnEventTTSSetupHelper(MessageSharedPtr msg,
                             SharedPtr<SetGlobalPropertiesRequest> command) {
    SmartObject help_prompt(smart_objects::SmartType_Array);
    help_prompt[0][strings::text] = "Help_Prompt_One";
    (*msg)[strings::msg_params][strings::help_prompt] = help_prompt;
    SmartObject timeout_prompt(smart_objects::SmartType_Array);
    timeout_prompt[0][strings::text] = "Timeout_Prompt_One";
    (*msg)[strings::msg_params][strings::timeout_prompt] = timeout_prompt;

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
    EXPECT_CALL(app_mngr_,
                RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
    SmartObject vr_help_title("title");
    EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
    EXPECT_CALL(*mock_app_, set_help_prompt(help_prompt));
    EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(&help_prompt));
    EXPECT_CALL(*mock_app_, set_timeout_prompt(timeout_prompt));
    EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(&timeout_prompt));
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

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

  MockAppPtr mock_app_;
  MockMessageHelper& mock_message_helper_;
  sync_primitives::Lock lock_;
};

TEST_F(SetGlobalPropertiesRequestTest, Run_VRNoMenuAndKeyboard_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventUISetupHelper(msg, command);
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRWithMenuAndKeyboard_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  VRArraySetupHelper(msg, vr_help_title, vr_help_array);
  (*msg)[strings::msg_params][strings::vr_help] = vr_help_array;
  SmartObject menu_title("Menu_Title");
  (*msg)[strings::msg_params][hmi_request::menu_title] = menu_title;
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[strings::value] = "1";
  (*msg)[strings::msg_params][hmi_request::menu_icon] = menu_icon;
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[strings::msg_params][hmi_request::keyboard_properties] =
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
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_VRBrokenMenuIcon_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject vr_help_title("yes");
  (*msg)[strings::msg_params][strings::vr_help_title] = vr_help_title;
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[strings::value] = "1";
  (*msg)[strings::msg_params][hmi_request::menu_icon] = menu_icon;

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
  menu_icon[strings::value] = "1";
  (*msg)[strings::msg_params][hmi_request::menu_icon] = menu_icon;
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
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;
  (*msg)[strings::msg_params][strings::app_id] = kAppID;
  SmartObject vr_help_title("wrong syntax string\\n");
  SmartObject menu_icon(smart_objects::SmartType_Map);
  menu_icon[strings::value] = "1";
  (*msg)[strings::msg_params][hmi_request::menu_icon] = menu_icon;
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
  vr_help_array[0][strings::text] = kText;
  vr_help_array[0][strings::position] = kPosition;
  (*msg)[strings::msg_params][strings::vr_help] = vr_help_array;

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
  (*msg)[strings::msg_params][strings::vr_help_title] = vr_help_title;

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
  vr_help_array[1][strings::text] = "two";
  vr_help_array[1][strings::position] = 3u;
  (*msg)[strings::msg_params][strings::vr_help] = vr_help_array;

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
  (*msg)[strings::msg_params][hmi_request::keyboard_properties] =
      keyboard_properties;
  SmartObject menu_title("Menu_Title");
  (*msg)[strings::msg_params][hmi_request::menu_title] = menu_title;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title("Menu_Title");
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, set_menu_title(menu_title));
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVRNoDataNoDefault_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[strings::msg_params][hmi_request::keyboard_properties] =
      keyboard_properties;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title(smart_objects::SmartType_Null);
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));

  CommandsMap commands_map;
  DataAccessor<CommandsMap> accessor(commands_map, lock_);
  EXPECT_CALL(*mock_app_, commands_map()).WillOnce(Return(accessor));
  EXPECT_CALL(*mock_app_, vr_synonyms()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(_)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVRNoDataDefaultCreated_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[strings::msg_params][hmi_request::keyboard_properties] =
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
  (*commands_map[0])[strings::vr_commands] = SmartObject("one");
  DataAccessor<CommandsMap> accessor(commands_map, lock_);
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
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoVRNoDataFromSynonyms_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject keyboard_properties(smart_objects::SmartType_Map);
  (*msg)[strings::msg_params][hmi_request::keyboard_properties] =
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
  DataAccessor<CommandsMap> accessor(commands_map, lock_);
  EXPECT_CALL(*mock_app_, commands_map()).WillOnce(Return(accessor));
  SmartObject vr_help_array(smart_objects::SmartType_Array);
  vr_help_array[0] = SmartObject(smart_objects::SmartType_Map);
  vr_help_array[0][strings::text] = kText;
  vr_help_array[0][strings::position] = kPosition;
  SmartObject vr_synonyms(smart_objects::SmartType_Array);
  vr_synonyms[0] = vr_help_array;
  EXPECT_CALL(*mock_app_, vr_synonyms()).WillOnce(Return(&vr_synonyms));
  EXPECT_CALL(*mock_app_, set_vr_help(vr_help_array));
  const CustomString name("name");
  EXPECT_CALL(*mock_app_, name()).WillOnce(ReturnRef(name));
  EXPECT_CALL(*mock_app_, set_vr_help_title(SmartObject(name)));
  EXPECT_CALL(*mock_app_, vr_help()).WillOnce(Return(&vr_help_array));
  EXPECT_CALL(*mock_app_, set_menu_title(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_menu_icon(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_keyboard_props(keyboard_properties));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSHelpAndTimeout_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject help_prompt(smart_objects::SmartType_Array);
  help_prompt[0][strings::text] = "Help_Prompt_One";
  (*msg)[strings::msg_params][strings::help_prompt] = help_prompt;
  SmartObject timeout_prompt(smart_objects::SmartType_Array);
  timeout_prompt[0][strings::text] = "Timeout_Prompt_One";
  (*msg)[strings::msg_params][strings::timeout_prompt] = timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title("title");
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, set_help_prompt(help_prompt));
  EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(&help_prompt));
  EXPECT_CALL(*mock_app_, set_timeout_prompt(timeout_prompt));
  EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(&timeout_prompt));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSOnlyHelp_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject help_prompt(smart_objects::SmartType_Array);
  help_prompt[0][strings::text] = "Help_Prompt_One";
  (*msg)[strings::msg_params][strings::help_prompt] = help_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title("title");
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, set_help_prompt(help_prompt));
  EXPECT_CALL(*mock_app_, help_prompt()).WillOnce(Return(&help_prompt));
  EXPECT_CALL(*mock_app_, set_timeout_prompt(_)).Times(0);
  EXPECT_CALL(*mock_app_, timeout_prompt()).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSOnlyTimeout_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject timeout_prompt(smart_objects::SmartType_Array);
  timeout_prompt[0][strings::text] = "Timeout_Prompt_One";
  (*msg)[strings::msg_params][strings::timeout_prompt] = timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(kConnectionKey));
  SmartObject vr_help_title("title");
  EXPECT_CALL(*mock_app_, vr_help_title()).WillOnce(Return(&vr_help_title));
  EXPECT_CALL(*mock_app_, set_help_prompt(_)).Times(0);
  EXPECT_CALL(*mock_app_, help_prompt()).Times(0);
  EXPECT_CALL(*mock_app_, set_timeout_prompt(timeout_prompt));
  EXPECT_CALL(*mock_app_, timeout_prompt()).WillOnce(Return(&timeout_prompt));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_TTSIncorrectSyntax_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject timeout_prompt(smart_objects::SmartType_Array);
  timeout_prompt[0][strings::text] = "Timeout_Prompt_One\\n";
  (*msg)[strings::msg_params][strings::timeout_prompt] = timeout_prompt;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  SmartObject vr_help_title("title");
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_NoData_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, Run_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::connection_key] = kConnectionKey;

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(mock_message_helper_, VerifyImageVrHelpItems(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, RemoveAppFromTTSGlobalPropertiesList(_)).Times(0);
  EmptyExpectationsSetupHelper();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  command->Run();
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_UIAndSuccessResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::SUCCESS;
  (*msg)[strings::params][hmi_response::code] = response_code;

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventUISetupHelper(msg, command);

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(response_code))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, UpdateHash());

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_UIAndWarningResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::WARNINGS;
  (*msg)[strings::params][hmi_response::code] = response_code;

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventUISetupHelper(msg, command);

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(_)).Times(0);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, UpdateHash());

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::WARNINGS;
  (*msg)[strings::params][hmi_response::code] = response_code;

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventUISetupHelper(msg, command);

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(_)).Times(0);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(*mock_app_, UpdateHash()).Times(0);

  Event event(hmi_apis::FunctionID::UI_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest, OnEvent_InvalidEventID_Canceled) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(_)).Times(0);
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
  (*msg)[strings::params][hmi_response::code] = response_code;

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventTTSSetupHelper(msg, command);

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(response_code))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, UpdateHash());

  Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(SetGlobalPropertiesRequestTest,
       OnEvent_TTSAndWarningsResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  hmi_apis::Common_Result::eType response_code =
      hmi_apis::Common_Result::WARNINGS;
  (*msg)[strings::params][hmi_response::code] = response_code;

  SharedPtr<SetGlobalPropertiesRequest> command(
      CreateCommand<SetGlobalPropertiesRequest>(msg));

  OnEventTTSSetupHelper(msg, command);

  EXPECT_CALL(mock_message_helper_, HMIToMobileResult(_)).Times(0);
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, UpdateHash());

  Event event(hmi_apis::FunctionID::TTS_SetGlobalProperties);
  event.set_smart_object(*msg);

  command->on_event(event);
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

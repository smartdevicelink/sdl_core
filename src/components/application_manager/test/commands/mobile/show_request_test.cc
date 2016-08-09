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
#include "application_manager/commands/mobile/show_request.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "utils/shared_ptr.h"
#include "commands/command_request_test.h"
#include "smart_objects/smart_object.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using application_manager::commands::MessageSharedPtr;
using application_manager::commands::ShowRequest;
using application_manager::MockMessageHelper;
using application_manager::event_engine::Event;
using test::components::policy_test::MockPolicyHandlerInterface;
using utils::SharedPtr;
using utils::custom_string::CustomString;
using smart_objects::SmartObject;
using testing::_;
using testing::Return;
using testing::ReturnRef;
using testing::Mock;

namespace strings = application_manager::strings;
namespace hmi_request = application_manager::hmi_request;
namespace hmi_response = application_manager::hmi_response;

namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kAppID = 2u;
const uint32_t kFunctionID = 3u;
}  // namespace

class ShowRequestTest : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ShowRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {
    mock_app_ = CreateMockApp();
  }

  MessageSharedPtr CreateMsgParams() {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*msg)[strings::params][strings::function_id] = kFunctionID;
    return msg;
  }

  void TestSetupHelper(MessageSharedPtr msg,
                       hmi_apis::Common_TextFieldName::eType field_name,
                       const char* field) {
    SmartObject msg_params(smart_objects::SmartType_Map);
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*msg)[strings::params][strings::function_id] = kFunctionID;
    msg_params[field] = text_field_;
    (*msg)[strings::msg_params] = msg_params;

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

    msg_params[strings::app_id] = kAppID;
    msg_params[hmi_request::show_strings] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    msg_params.erase(field);
    msg_params[hmi_request::show_strings][0][hmi_request::field_name] =
        static_cast<int32_t>(field_name);
    msg_params[hmi_request::show_strings][0][hmi_request::field_text] =
        text_field_;

    EXPECT_CALL(app_mngr_, ManageHMICommand(_));
    EXPECT_CALL(*mock_app_, set_show_command(msg_params));
  }

  void TestSetupHelperWrongSyntax(
      MessageSharedPtr msg,
      hmi_apis::Common_TextFieldName::eType field_name,
      const char* field) {
    SmartObject msg_params(smart_objects::SmartType_Map);
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    msg_params[field] = text_field_;
    (*msg)[strings::msg_params] = msg_params;

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
    EXPECT_CALL(*mock_app_, app_id()).Times(0);

    EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);
  }

  MockAppPtr mock_app_;
  MockMessageHelper& mock_message_helper_;
  std::string text_field_;
};

TEST_F(ShowRequestTest, Run_SoftButtonExists_SUCCESS) {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::soft_buttons] = "Soft_Buttons";
  (*msg)[strings::msg_params] = msg_params;
  SmartObject creation_msg_params(msg_params);
  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  MockPolicyHandlerInterface mock_policy_handler;
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler));
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  msg_params[strings::app_id] = kAppID;
  msg_params[hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(
      mock_message_helper_,
      SubscribeApplicationToSoftButton(creation_msg_params, _, kFunctionID));
  EXPECT_CALL(app_mngr_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();

  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ShowRequestTest, Run_SoftButtonNotExists_SUCCESS) {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::soft_buttons] = "";
  (*msg)[strings::msg_params] = msg_params;
  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  MockPolicyHandlerInterface mock_policy_handler;
  EXPECT_CALL(app_mngr_, GetPolicyHandler()).Times(0);
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  msg_params[strings::app_id] = kAppID;
  msg_params[hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(*mock_app_, UnsubscribeFromSoftButtons(kFunctionID));
  EXPECT_CALL(app_mngr_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();

  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ShowRequestTest, Run_SoftButtonExists_Canceled) {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::soft_buttons] = "Soft_Buttons";
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  MockPolicyHandlerInterface mock_policy_handler;
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(mock_policy_handler));
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .WillOnce(Return(mobile_apis::Result::ABORTED));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));

  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(mock_message_helper_, SubscribeApplicationToSoftButton(_, _, _))
      .Times(0);
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);

  command->Run();

  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ShowRequestTest, Run_Graphic_SUCCESS) {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[strings::value] = "1";
  msg_params[strings::graphic] = graphic;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  msg_params[strings::app_id] = kAppID;
  msg_params[hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();

  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ShowRequestTest, Run_Graphic_Canceled) {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[strings::value] = "1";
  msg_params[strings::graphic] = graphic;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _))
      .WillOnce(Return(mobile_apis::Result::ABORTED));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(msg_params)).Times(0);

  command->Run();

  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ShowRequestTest, Run_Graphic_WrongSyntax) {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[strings::value] = "\\n";
  msg_params[strings::graphic] = graphic;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(msg_params)).Times(0);

  command->Run();

  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ShowRequestTest, Run_SecondaryGraphic_SUCCESS) {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[strings::value] = "1";
  msg_params[strings::secondary_graphic] = graphic;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  msg_params[strings::app_id] = kAppID;
  msg_params[hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();

  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ShowRequestTest, Run_SecondaryGraphic_Canceled) {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[strings::value] = "1";
  msg_params[strings::secondary_graphic] = graphic;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _))
      .WillOnce(Return(mobile_apis::Result::ABORTED));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(msg_params)).Times(0);

  command->Run();

  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ShowRequestTest, Run_SecondaryGraphic_WrongSyntax) {
  Mock::VerifyAndClearExpectations(&mock_message_helper_);

  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[strings::value] = "";
  msg_params[strings::secondary_graphic] = graphic;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _)).Times(0);
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(msg_params)).Times(0);

  command->Run();

  Mock::VerifyAndClearExpectations(&mock_message_helper_);
}

TEST_F(ShowRequestTest, Run_MainField1_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_1";
  TestSetupHelper(
      msg, hmi_apis::Common_TextFieldName::mainField1, strings::main_field_1);

  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField1_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_1\\n";
  TestSetupHelperWrongSyntax(
      msg, hmi_apis::Common_TextFieldName::mainField1, strings::main_field_1);

  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField2_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_2";
  TestSetupHelper(
      msg, hmi_apis::Common_TextFieldName::mainField2, strings::main_field_2);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField2_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_2\\n";
  TestSetupHelperWrongSyntax(
      msg, hmi_apis::Common_TextFieldName::mainField2, strings::main_field_2);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField3_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_3";
  TestSetupHelper(
      msg, hmi_apis::Common_TextFieldName::mainField3, strings::main_field_3);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField3_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_3\\n";
  TestSetupHelperWrongSyntax(
      msg, hmi_apis::Common_TextFieldName::mainField3, strings::main_field_3);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField4_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_4";
  TestSetupHelper(
      msg, hmi_apis::Common_TextFieldName::mainField4, strings::main_field_4);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField4_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_4\\n";
  TestSetupHelperWrongSyntax(
      msg, hmi_apis::Common_TextFieldName::mainField4, strings::main_field_4);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MediaClock_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Media_Clock";
  TestSetupHelper(
      msg, hmi_apis::Common_TextFieldName::mediaClock, strings::media_clock);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MediaClock_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Media_Clock\\n";
  TestSetupHelperWrongSyntax(
      msg, hmi_apis::Common_TextFieldName::mediaClock, strings::media_clock);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MediaTrack_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Media_Track";
  TestSetupHelper(
      msg, hmi_apis::Common_TextFieldName::mediaTrack, strings::media_track);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MediaTrack_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Media_Track\\n";
  TestSetupHelperWrongSyntax(
      msg, hmi_apis::Common_TextFieldName::mediaTrack, strings::media_track);
  command->Run();
}

TEST_F(ShowRequestTest, Run_StatusBar_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Status_Bar";
  TestSetupHelper(
      msg, hmi_apis::Common_TextFieldName::statusBar, strings::status_bar);
  command->Run();
}

TEST_F(ShowRequestTest, Run_StatusBar_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Status_Bar\\n";
  TestSetupHelperWrongSyntax(
      msg, hmi_apis::Common_TextFieldName::statusBar, strings::status_bar);
  command->Run();
}

TEST_F(ShowRequestTest, Run_Alignment_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[strings::alignment] = "Alignment";
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  msg_params[strings::app_id] = kAppID;
  msg_params[hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();
}

TEST_F(ShowRequestTest, Run_CustomPresets_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject custom_presets(smart_objects::SmartType_Array);
  custom_presets[0] = "Custom_Presets";
  msg_params[strings::custom_presets] = custom_presets;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppID));

  msg_params[strings::app_id] = kAppID;
  msg_params[hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();
}

TEST_F(ShowRequestTest, Run_CustomPresets_WrongSyntax) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject custom_presets(smart_objects::SmartType_Array);
  custom_presets[0] = "Custom_Presets\\t";
  msg_params[strings::custom_presets] = custom_presets;
  (*msg)[strings::msg_params] = msg_params;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, Run_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, Run_EmptyParams_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, OnEvent_SuccessResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][hmi_response::code] = mobile_apis::Result::SUCCESS;
  (*msg)[strings::msg_params] = SmartObject(smart_objects::SmartType_Map);

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));

  Event event(hmi_apis::FunctionID::UI_Show);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(ShowRequestTest, OnEvent_WarningsResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][hmi_response::code] = mobile_apis::Result::WARNINGS;
  (*msg)[strings::params][hmi_response::message] = "Response Info";
  (*msg)[strings::msg_params] = SmartObject(smart_objects::SmartType_Map);

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _));

  Event event(hmi_apis::FunctionID::UI_Show);
  event.set_smart_object(*msg);

  command->on_event(event);
}

TEST_F(ShowRequestTest, OnEvent_WrongFunctionID_Canceled) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][hmi_response::code] = mobile_apis::Result::SUCCESS;

  SharedPtr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  Event event(hmi_apis::FunctionID::UI_Alert);
  event.set_smart_object(*msg);

  command->on_event(event);
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

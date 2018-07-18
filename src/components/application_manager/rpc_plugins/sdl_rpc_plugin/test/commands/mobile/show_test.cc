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
#include <set>

#include "mobile/show_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace show_request {

namespace am = application_manager;
using sdl_rpc_plugin::commands::ShowRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using test::components::policy_test::MockPolicyHandlerInterface;

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const int32_t kCommandId = 1;
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
const uint32_t kFunctionID = 3u;
}  // namespace

class ShowRequestTest : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  ShowRequestTest() {
    mock_app_ = CreateMockApp();
  }
  sync_primitives::Lock lock_;

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

  MessageSharedPtr CreateMsgParams() {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg)[am::strings::params][am::strings::function_id] = kFunctionID;
    return msg;
  }

  void TestSetupHelper(MessageSharedPtr msg,
                       hmi_apis::Common_TextFieldName::eType field_name,
                       const char* field) {
    SmartObject msg_params(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg)[am::strings::params][am::strings::function_id] = kFunctionID;
    msg_params[field] = text_field_;
    (*msg)[am::strings::msg_params] = msg_params;

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

    msg_params[am::strings::app_id] = kAppId;
    msg_params[am::hmi_request::show_strings] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    msg_params.erase(field);
    msg_params[am::hmi_request::show_strings][0][am::hmi_request::field_name] =
        static_cast<int32_t>(field_name);
    msg_params[am::hmi_request::show_strings][0][am::hmi_request::field_text] =
        text_field_;
    EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_));
    EXPECT_CALL(*mock_app_, set_show_command(msg_params));
  }

  void TestSetupHelperWrongSyntax(
      MessageSharedPtr msg,
      hmi_apis::Common_TextFieldName::eType field_name,
      const char* field) {
    SmartObject msg_params(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    msg_params[field] = text_field_;
    (*msg)[am::strings::msg_params] = msg_params;

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));
    EXPECT_CALL(*mock_app_, app_id()).Times(0);

    EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
    EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);
  }

  void TestSetupHelperWithMetadata(
      MessageSharedPtr msg,
      hmi_apis::Common_TextFieldName::eType field_name,
      const char* field,
      size_t num_tags,
      int32_t* field_tags,
      bool set_field_text = true) {
    SmartObject msg_params(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg)[am::strings::params][am::strings::function_id] = kFunctionID;
    if (set_field_text) {
      msg_params[field] = text_field_;
    }
    msg_params[am::strings::metadata_tags][field] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    for (size_t i = 0; i < num_tags; ++i) {
      const int32_t current_tag = field_tags[i];
      msg_params[am::strings::metadata_tags][field][i] = current_tag;
    }
    (*msg)[am::strings::msg_params] = msg_params;

    EXPECT_EQ((*msg)[am::strings::msg_params], msg_params);

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

    msg_params.erase(field);
    msg_params.erase(am::strings::metadata_tags);

    msg_params[am::strings::app_id] = kAppId;
    msg_params[am::hmi_request::show_strings] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    if (set_field_text) {
      msg_params[am::hmi_request::show_strings][0]
                [am::hmi_request::field_name] =
                    static_cast<int32_t>(field_name);
      msg_params[am::hmi_request::show_strings][0]
                [am::hmi_request::field_text] = text_field_;
      msg_params[am::hmi_request::show_strings][0]
                [am::hmi_request::field_types] =
                    smart_objects::SmartObject(smart_objects::SmartType_Array);
      for (size_t i = 0; i < num_tags; ++i) {
        const int32_t current_tag = field_tags[i];
        msg_params[am::hmi_request::show_strings][0]
                  [am::hmi_request::field_types][i] = current_tag;
      }
    }

    EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_));
    EXPECT_CALL(*mock_app_, set_show_command(msg_params));
  }

  MockAppPtr mock_app_;
  std::string text_field_;
};

TEST_F(ShowRequestTest, OnEvent_UI_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr msg_vr = CreateFullParamsUISO();
  (*msg_vr)[am::strings::msg_params][am::strings::menu_params]
           [am::hmi_request::parent_id] = 10u;
  (*msg_vr)[am::strings::msg_params][am::strings::menu_params]
           [am::strings::menu_name] = "menu_name";

  std::shared_ptr<ShowRequest> command = CreateCommand<ShowRequest>(msg_vr);

  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));
  ON_CALL(*mock_app, app_id()).WillByDefault(Return(kConnectionKey));

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;

  Event event(hmi_apis::FunctionID::UI_Show);
  event.set_smart_object(*msg);

  MessageSharedPtr vr_command_result;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&vr_command_result), Return(true)));

  command->on_event(event);

  EXPECT_EQ((*vr_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            true);
  EXPECT_EQ(
      (*vr_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
  if ((*vr_command_result)[am::strings::msg_params].keyExists(
          am::strings::info)) {
    EXPECT_FALSE(
        (*vr_command_result)[am::strings::msg_params][am::strings::info]
            .asString()
            .empty());
  }
}

TEST_F(ShowRequestTest, Run_SoftButtonExists_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[am::strings::soft_buttons] = "Soft_Buttons";
  (*msg)[am::strings::msg_params] = msg_params;
  SmartObject creation_msg_params(msg_params);
  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  msg_params[am::strings::app_id] = kAppId;
  msg_params[am::hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(
      mock_message_helper_,
      SubscribeApplicationToSoftButton(creation_msg_params, _, kFunctionID));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();
}

TEST_F(ShowRequestTest, Run_SoftButtonNotExists_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[am::strings::soft_buttons] = "";
  (*msg)[am::strings::msg_params] = msg_params;
  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(_, _, _, _)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  msg_params[am::strings::app_id] = kAppId;
  msg_params[am::hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(*mock_app_, UnsubscribeFromSoftButtons(kFunctionID));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();
}

TEST_F(ShowRequestTest, Run_SoftButtonExists_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[am::strings::soft_buttons] = "Soft_Buttons";
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, ProcessSoftButtons(msg_params, _, _, _))
      .WillOnce(Return(mobile_apis::Result::ABORTED));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));

  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(mock_message_helper_, SubscribeApplicationToSoftButton(_, _, _))
      .Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, Run_Graphic_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[am::strings::value] = "1";
  msg_params[am::strings::graphic] = graphic;
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  msg_params[am::strings::app_id] = kAppId;
  msg_params[am::hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();
}

TEST_F(ShowRequestTest, Run_Graphic_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[am::strings::value] = "1";
  msg_params[am::strings::graphic] = graphic;
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _))
      .WillOnce(Return(mobile_apis::Result::ABORTED));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(msg_params)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, Run_Graphic_WrongSyntax) {
  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[am::strings::value] = "\\n";
  msg_params[am::strings::graphic] = graphic;
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(_, _, _)).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(msg_params)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, Run_SecondaryGraphic_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[am::strings::value] = "1";
  msg_params[am::strings::secondary_graphic] = graphic;
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  msg_params[am::strings::app_id] = kAppId;
  msg_params[am::hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();
}

TEST_F(ShowRequestTest, Run_SecondaryGraphic_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[am::strings::value] = "1";
  msg_params[am::strings::secondary_graphic] = graphic;
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _))
      .WillOnce(Return(mobile_apis::Result::ABORTED));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(msg_params)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, Run_SecondaryGraphic_WrongSyntax) {
  MessageSharedPtr msg = CreateMsgParams();

  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject graphic(smart_objects::SmartType_Map);
  graphic[am::strings::value] = "";
  msg_params[am::strings::secondary_graphic] = graphic;
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_message_helper_, VerifyImage(graphic, _, _)).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(msg_params)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField1_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_1";
  TestSetupHelper(msg,
                  hmi_apis::Common_TextFieldName::mainField1,
                  am::strings::main_field_1);

  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField1_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_1\\n";
  TestSetupHelperWrongSyntax(msg,
                             hmi_apis::Common_TextFieldName::mainField1,
                             am::strings::main_field_1);

  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField2_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_2";
  TestSetupHelper(msg,
                  hmi_apis::Common_TextFieldName::mainField2,
                  am::strings::main_field_2);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField2_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_2\\n";
  TestSetupHelperWrongSyntax(msg,
                             hmi_apis::Common_TextFieldName::mainField2,
                             am::strings::main_field_2);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField3_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_3";
  TestSetupHelper(msg,
                  hmi_apis::Common_TextFieldName::mainField3,
                  am::strings::main_field_3);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField3_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_3\\n";
  TestSetupHelperWrongSyntax(msg,
                             hmi_apis::Common_TextFieldName::mainField3,
                             am::strings::main_field_3);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField4_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_4";
  TestSetupHelper(msg,
                  hmi_apis::Common_TextFieldName::mainField4,
                  am::strings::main_field_4);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField4_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_4\\n";
  TestSetupHelperWrongSyntax(msg,
                             hmi_apis::Common_TextFieldName::mainField4,
                             am::strings::main_field_4);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField1_MetadataTag) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_1";
  const size_t num_tags = 1;
  int32_t tags[num_tags] = {hmi_apis::Common_MetadataType::mediaArtist};
  TestSetupHelperWithMetadata(msg,
                              hmi_apis::Common_TextFieldName::mainField1,
                              am::strings::main_field_1,
                              num_tags,
                              tags);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField1_MultipleMetadataTags) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_1";
  const size_t num_tags = 5;
  int32_t tags[num_tags] = {hmi_apis::Common_MetadataType::mediaTitle,
                            hmi_apis::Common_MetadataType::mediaArtist,
                            hmi_apis::Common_MetadataType::rating,
                            hmi_apis::Common_MetadataType::humidity,
                            hmi_apis::Common_MetadataType::currentTemperature};
  TestSetupHelperWithMetadata(msg,
                              hmi_apis::Common_TextFieldName::mainField1,
                              am::strings::main_field_1,
                              num_tags,
                              tags);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField2_MetadataTag) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_2";
  const size_t num_tags = 1;
  int32_t tags[num_tags] = {hmi_apis::Common_MetadataType::mediaArtist};
  TestSetupHelperWithMetadata(msg,
                              hmi_apis::Common_TextFieldName::mainField2,
                              am::strings::main_field_2,
                              num_tags,
                              tags);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField3_MetadataTag) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_3";
  const size_t num_tags = 1;
  int32_t tags[num_tags] = {hmi_apis::Common_MetadataType::mediaArtist};
  TestSetupHelperWithMetadata(msg,
                              hmi_apis::Common_TextFieldName::mainField3,
                              am::strings::main_field_3,
                              num_tags,
                              tags);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField4_MetadataTag) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_4";
  const size_t num_tags = 1;
  int32_t tags[num_tags] = {hmi_apis::Common_MetadataType::mediaArtist};
  TestSetupHelperWithMetadata(msg,
                              hmi_apis::Common_TextFieldName::mainField4,
                              am::strings::main_field_4,
                              num_tags,
                              tags);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MainField1_MetadataTagWithNoFieldData) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Main_Field_1";
  const size_t num_tags = 1;
  int32_t tags[num_tags] = {hmi_apis::Common_MetadataType::mediaArtist};
  TestSetupHelperWithMetadata(msg,
                              hmi_apis::Common_TextFieldName::mainField1,
                              am::strings::main_field_1,
                              num_tags,
                              tags,
                              false);
  command->Run();

  MessageSharedPtr ev_msg = CreateMessage(smart_objects::SmartType_Map);
  (*ev_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  (*ev_msg)[am::strings::msg_params][am::strings::app_id] = kConnectionKey;
  (*ev_msg)[am::strings::msg_params][am::strings::info] = "";

  Event event(hmi_apis::FunctionID::UI_Show);
  event.set_smart_object(*ev_msg);

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(_, am::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->on_event(event);

  EXPECT_EQ((*ui_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            true);
  EXPECT_EQ(
      (*ui_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(mobile_apis::Result::WARNINGS));
}

TEST_F(ShowRequestTest, Run_MediaClock_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Media_Clock";
  TestSetupHelper(msg,
                  hmi_apis::Common_TextFieldName::mediaClock,
                  am::strings::media_clock);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MediaClock_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Media_Clock\\n";
  TestSetupHelperWrongSyntax(msg,
                             hmi_apis::Common_TextFieldName::mediaClock,
                             am::strings::media_clock);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MediaTrack_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Media_Track";
  TestSetupHelper(msg,
                  hmi_apis::Common_TextFieldName::mediaTrack,
                  am::strings::media_track);
  command->Run();
}

TEST_F(ShowRequestTest, Run_MediaTrack_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Media_Track\\n";
  TestSetupHelperWrongSyntax(msg,
                             hmi_apis::Common_TextFieldName::mediaTrack,
                             am::strings::media_track);
  command->Run();
}

TEST_F(ShowRequestTest, Run_StatusBar_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Status_Bar";
  TestSetupHelper(
      msg, hmi_apis::Common_TextFieldName::statusBar, am::strings::status_bar);
  command->Run();
}

TEST_F(ShowRequestTest, Run_StatusBar_WrongSyntax) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  text_field_ = "Status_Bar\\n";
  TestSetupHelperWrongSyntax(
      msg, hmi_apis::Common_TextFieldName::statusBar, am::strings::status_bar);
  command->Run();
}

TEST_F(ShowRequestTest, Run_Alignment_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  msg_params[am::strings::alignment] = "Alignment";
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  msg_params[am::strings::app_id] = kAppId;
  msg_params[am::hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();
}

TEST_F(ShowRequestTest, Run_CustomPresets_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject custom_presets(smart_objects::SmartType_Array);
  custom_presets[0] = "Custom_Presets";
  msg_params[am::strings::custom_presets] = custom_presets;
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId));

  msg_params[am::strings::app_id] = kAppId;
  msg_params[am::hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_));
  EXPECT_CALL(*mock_app_, set_show_command(msg_params));

  command->Run();
}

TEST_F(ShowRequestTest, Run_CustomPresets_WrongSyntax) {
  MessageSharedPtr msg = CreateMsgParams();
  SmartObject msg_params(smart_objects::SmartType_Map);
  SmartObject custom_presets(smart_objects::SmartType_Array);
  custom_presets[0] = "Custom_Presets\\t";
  msg_params[am::strings::custom_presets] = custom_presets;
  (*msg)[am::strings::msg_params] = msg_params;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, Run_InvalidApp_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, Run_EmptyParams_Canceled) {
  MessageSharedPtr msg = CreateMsgParams();

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));
  EXPECT_CALL(*mock_app_, app_id()).Times(0);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);
  EXPECT_CALL(*mock_app_, set_show_command(_)).Times(0);

  command->Run();
}

TEST_F(ShowRequestTest, OnEvent_SuccessResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::eType::SUCCESS;
  (*msg)[am::strings::msg_params] = SmartObject(smart_objects::SmartType_Map);

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_apis::Result::eType::SUCCESS), _));

  Event event(hmi_apis::FunctionID::UI_Show);
  event.set_smart_object(*msg);

  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(mock_app_));

  command->on_event(event);
}

TEST_F(ShowRequestTest, OnEvent_WarningsResultCode_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::WARNINGS;
  (*msg)[am::strings::params][am::hmi_response::message] = "Response Info";
  (*msg)[am::strings::msg_params] = SmartObject(smart_objects::SmartType_Map);

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _));

  Event event(hmi_apis::FunctionID::UI_Show);
  event.set_smart_object(*msg);

  EXPECT_CALL(app_mngr_, application(_)).WillRepeatedly(Return(mock_app_));

  command->on_event(event);
}

TEST_F(ShowRequestTest, OnEvent_WrongFunctionID_Canceled) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::SUCCESS;

  std::shared_ptr<ShowRequest> command(CreateCommand<ShowRequest>(msg));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);

  Event event(hmi_apis::FunctionID::UI_Alert);
  event.set_smart_object(*msg);

  command->on_event(event);
}

}  // namespace show_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests

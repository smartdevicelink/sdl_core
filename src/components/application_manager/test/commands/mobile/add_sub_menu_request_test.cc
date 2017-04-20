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

#include "application_manager/commands/mobile/add_sub_menu_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace add_sub_menu_request {

namespace am = ::application_manager;
using am::commands::AddSubMenuRequest;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using am::MockHmiInterfaces;
using am::MockMessageHelper;
using ::testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnNull;

typedef SharedPtr<AddSubMenuRequest> AddSubMenuPtr;

namespace {
const uint32_t kConnectionKey = 2u;
const int32_t kMenuId = 5;
const std::string kMenuName = "valid_sub_menu_name";
const std::string kFileStorage = "test_storage";
const std::string kFolderName = "folder_name";
const std::string kTestPosition = "test_position";
const std::string kFileName = "file_name";
}  // namespace

class AddSubMenuRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  AddSubMenuRequestTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {}
  void SetUp() OVERRIDE {
    app = CreateMockApp();
    command_msg_ = CreateMessage(smart_objects::SmartType_Map);

    (*command_msg_)[am::strings::msg_params][am::strings::menu_id] = kMenuId;
    (*command_msg_)[am::strings::params][am::strings::connection_key] =
        kConnectionKey;

    ON_CALL(app_mngr_, hmi_interfaces())
        .WillByDefault(ReturnRef(hmi_interfaces_));

    ON_CALL(hmi_interfaces_,
            GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
        .WillByDefault(Return(am::HmiInterfaces::STATE_AVAILABLE));

    ON_CALL(hmi_interfaces_, GetInterfaceFromFunction(_))
        .WillByDefault(Return(am::HmiInterfaces::HMI_INTERFACE_UI));
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  MockAppPtr app;
  MessageSharedPtr command_msg_;

  NiceMock<MockHmiInterfaces> hmi_interfaces_;
  MockMessageHelper& mock_message_helper_;
};

TEST_F(AddSubMenuRequestTest, OnEvent_UI_UNSUPPORTED_RESOURCE) {
  const uint32_t menu_id = 10u;
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg)[am::strings::msg_params][am::strings::menu_id] = menu_id;

  utils::SharedPtr<AddSubMenuRequest> command =
      CreateCommand<AddSubMenuRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));
  ON_CALL(*mock_app, app_id()).WillByDefault(Return(kConnectionKey));
  EXPECT_CALL(*mock_app, AddSubMenu(menu_id, _));
  EXPECT_CALL(*mock_app, UpdateHash());

  MessageSharedPtr ev_msg = CreateMessage(smart_objects::SmartType_Map);
  (*ev_msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*ev_msg)[am::strings::msg_params][am::strings::info] = "info";

  Event event(hmi_apis::FunctionID::UI_AddSubMenu);
  event.set_smart_object(*ev_msg);

  MockHmiInterfaces hmi_interfaces;
  ON_CALL(app_mngr_, hmi_interfaces()).WillByDefault(ReturnRef(hmi_interfaces));
  EXPECT_CALL(hmi_interfaces,
              GetInterfaceState(am::HmiInterfaces::HMI_INTERFACE_UI))
      .WillOnce(Return(am::HmiInterfaces::STATE_AVAILABLE));

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE))
      .WillOnce(Return(mobile_apis::Result::UNSUPPORTED_RESOURCE));

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  command->on_event(event);

  EXPECT_EQ((*ui_command_result)[am::strings::msg_params][am::strings::success]
                .asBool(),
            true);
  EXPECT_EQ(
      (*ui_command_result)[am::strings::msg_params][am::strings::result_code]
          .asInt(),
      static_cast<int32_t>(hmi_apis::Common_Result::UNSUPPORTED_RESOURCE));
  if ((*ui_command_result)[am::strings::msg_params].keyExists(
          am::strings::info)) {
    EXPECT_FALSE(
        (*ui_command_result)[am::strings::msg_params][am::strings::info]
            .asString()
            .empty());
  }
}

TEST_F(AddSubMenuRequestTest, OnEvent_SUCCESS) {
  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::SUCCESS;
  (*event_msg)[am::strings::msg_params] = 0;

  Event event(hmi_apis::FunctionID::UI_AddSubMenu);
  event.set_smart_object(*event_msg);
  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::SUCCESS))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command, event)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::SUCCESS, received_result);
}

TEST_F(AddSubMenuRequestTest,
       OnEvent_SubMenuWithIcon_NotExistsInFolder_ExpectInfo) {
  // Message will be sent on hmi with icon that not exists in folder
  (*command_msg_)[am::strings::msg_params][am::strings::sub_menu_icon]
                 [am::strings::value] = "icon.png";

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));
  (*event_msg)[am::strings::params][am::hmi_response::code] =
      mobile_apis::Result::WARNINGS;
  (*event_msg)[am::strings::msg_params] = 0;

  Event event(hmi_apis::FunctionID::UI_AddSubMenu);
  event.set_smart_object(*event_msg);

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));

  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));

  EXPECT_CALL(app_mngr_, get_settings())
      .WillOnce(ReturnRef(app_mngr_settings_));

  EXPECT_CALL(app_mngr_settings_, app_storage_folder())
      .WillOnce(ReturnRef(kFileStorage));

  EXPECT_CALL(mock_message_helper_,
              HMIToMobileResult(hmi_apis::Common_Result::WARNINGS))
      .WillOnce(Return(mobile_apis::Result::WARNINGS));

  MessageSharedPtr result_msg(
      CatchMobileCommandResult(CallOnEvent(*command, event)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());

  EXPECT_EQ(mobile_apis::Result::WARNINGS, received_result);
  const std::string received_info =
      (*result_msg)[am::strings::msg_params][am::strings::info].asString();
  const std::string expected_info = "Reference image(s) not found";
  EXPECT_EQ(expected_info, received_info);
}

TEST_F(AddSubMenuRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>());
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::APPLICATION_NOT_REGISTERED, received_result);
}

TEST_F(AddSubMenuRequestTest, Run_InvalidSubMenuId_UNSUCCESS) {
  MessageSharedPtr dummy_sub_menu(CreateMessage(smart_objects::SmartType_Null));
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId))
      .WillOnce(Return(dummy_sub_menu.get()));

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::INVALID_ID, received_result);
}

TEST_F(AddSubMenuRequestTest, Run_DuplicatedSubMenuName_UNSUCCESS) {
  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));

  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId)).WillOnce(ReturnNull());
  EXPECT_CALL(*app, IsSubMenuNameAlreadyExist(_)).WillOnce(Return(true));

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::DUPLICATE_NAME, received_result);
}

TEST_F(AddSubMenuRequestTest, Run_NotValidSubMenuName_UNSUCCESS) {
  // Not valid sub
  (*command_msg_)[am::strings::msg_params][am::strings::menu_name] = "\t\n";
  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId)).WillOnce(ReturnNull());

  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, received_result);
}

TEST_F(AddSubMenuRequestTest, Run_SUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::strings::menu_name] = kMenuName;
  (*command_msg_)[am::strings::msg_params][am::strings::position] =
      kTestPosition;
  (*command_msg_)[am::strings::msg_params][am::strings::sub_menu_icon]
                 [am::strings::image_type] = mobile_apis::ImageType::DYNAMIC;
  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));
  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  EXPECT_CALL(app_mngr_settings_, app_storage_folder())
      .WillOnce(ReturnRef(kFileStorage));
  EXPECT_CALL(*app, folder_name()).WillOnce(Return(kFolderName));
  (*command_msg_)[am::strings::msg_params][am::strings::sub_menu_icon]
                 [am::strings::value] = kFileName;
  EXPECT_CALL(*app, FindSubMenu(kMenuId)).WillOnce(ReturnNull());
  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));
  MessageSharedPtr result_msg(CatchHMICommandResult(CallRun(*command)));
  const hmi_apis::FunctionID::eType received_result =
      static_cast<hmi_apis::FunctionID::eType>(
          (*result_msg)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(hmi_apis::FunctionID::UI_AddSubMenu, received_result);
}

TEST_F(AddSubMenuRequestTest, Run_MenuIconCorrectName_SUCCESS) {
  (*command_msg_)[am::strings::msg_params][am::strings::menu_name] = kMenuName;
  (*command_msg_)[am::strings::msg_params][am::strings::position] =
      kTestPosition;
  (*command_msg_)[am::strings::msg_params][am::strings::sub_menu_icon]
                 [am::strings::image_type] = mobile_apis::ImageType::DYNAMIC;

  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));
  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  EXPECT_CALL(app_mngr_settings_, app_storage_folder())
      .WillOnce(ReturnRef(kFileStorage));
  EXPECT_CALL(*app, folder_name()).WillOnce(Return(kFolderName));

  (*command_msg_)[am::strings::msg_params][am::strings::sub_menu_icon]
                 [am::strings::value] = kFileName;
  EXPECT_CALL(*app, FindSubMenu(kMenuId)).WillOnce(ReturnNull());
  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));
  MessageSharedPtr result_msg(CatchHMICommandResult(CallRun(*command)));
  const hmi_apis::FunctionID::eType received_result =
      static_cast<hmi_apis::FunctionID::eType>(
          (*result_msg)[am::strings::params][am::strings::function_id].asInt());
  EXPECT_EQ(hmi_apis::FunctionID::UI_AddSubMenu, received_result);

  const std::string full_file_path =
      kFileStorage + "/" + kFolderName + "/" + kFileName;

  EXPECT_EQ(full_file_path,
            (*result_msg)[am::strings::msg_params][am::strings::sub_menu_icon]
                         [am::strings::value].asString());
}

TEST_F(AddSubMenuRequestTest, Run_MenuIconNewLineChar_SendINVALID_DATA) {
  // Not valid sub_menu_icon
  (*command_msg_)[am::strings::msg_params][am::strings::menu_name] = kMenuName;
  (*command_msg_)[am::strings::msg_params][am::strings::sub_menu_icon]
                 [am::strings::value] = "ico\nn.png";

  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId)).WillOnce(ReturnNull());

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, received_result);

  EXPECT_FALSE((*result_msg)[am::strings::msg_params].keyExists(
      am::strings::sub_menu_icon));
}

TEST_F(AddSubMenuRequestTest, Run_MenuIconTabChar_SendINVALID_DATA) {
  // Not valid sub_menu_icon
  (*command_msg_)[am::strings::msg_params][am::strings::menu_name] = kMenuName;
  (*command_msg_)[am::strings::msg_params][am::strings::sub_menu_icon]
                 [am::strings::value] = "ico\tn.png";

  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId)).WillOnce(ReturnNull());

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, received_result);

  EXPECT_FALSE((*result_msg)[am::strings::msg_params].keyExists(
      am::strings::sub_menu_icon));
}

TEST_F(AddSubMenuRequestTest, Run_MenuIconWhiteSpace_SendINVALID_DATA) {
  (*command_msg_)[am::strings::msg_params][am::strings::menu_name] = kMenuName;
  (*command_msg_)[am::strings::msg_params][am::strings::sub_menu_icon]
                 [am::strings::value] = "ico n.png";

  EXPECT_CALL(app_mngr_, application(kConnectionKey)).WillOnce(Return(app));
  EXPECT_CALL(*app, FindSubMenu(kMenuId)).WillOnce(ReturnNull());

  AddSubMenuPtr command(CreateCommand<AddSubMenuRequest>(command_msg_));
  MessageSharedPtr result_msg(CatchMobileCommandResult(CallRun(*command)));
  const mobile_apis::Result::eType received_result =
      static_cast<mobile_apis::Result::eType>(
          (*result_msg)[am::strings::msg_params][am::strings::result_code]
              .asInt());
  EXPECT_EQ(mobile_apis::Result::INVALID_DATA, received_result);

  EXPECT_FALSE((*result_msg)[am::strings::msg_params].keyExists(
      am::strings::sub_menu_icon));
}

}  // namespace add_sub_menu_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

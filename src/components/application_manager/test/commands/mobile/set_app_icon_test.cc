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

#include "application_manager/commands/mobile/set_app_icon_request.h"

#include "gtest/gtest.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "test/protocol_handler/mock_protocol_handler.h"
#include "test/protocol_handler/mock_protocol_handler_settings.h"

#include "utils/file_system.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace set_app_icon_request {

namespace am = application_manager;
using am::commands::SetAppIconRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockMessageHelper;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using application_manager_test::MockApplicationManagerSettings;
using protocol_handler_test::MockProtocolHandler;
using protocol_handler_test::MockProtocolHandlerSettings;

namespace {
const uint32_t kAppId = 1u;
const uint32_t kCmdId = 1u;
const uint32_t kConnectionKey = 2u;
}  // namespace

class SetAppIconRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
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

 private:
  void SetUp() OVERRIDE {
    const std::string dir_path = "./";
    ON_CALL(app_mngr_settings_, app_icons_folder())
        .WillByDefault(ReturnRef(dir_path));
  }
};

TEST_F(SetAppIconRequestTest, OnEvent_UI_UNSUPPORTED_RESOURCE) {
  const std::string file_path = "file_path";
  MessageSharedPtr msg_vr = CreateFullParamsUISO();
  (*msg_vr)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg_vr)[am::strings::msg_params][am::strings::sync_file_name]
           [am::strings::value] = file_path;

  utils::SharedPtr<SetAppIconRequest> req_vr =
      CreateCommand<SetAppIconRequest>(msg_vr);

  MockAppPtr mock_app = CreateMockApp();
  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app));

  ON_CALL(*mock_app, app_id()).WillByDefault(Return(kConnectionKey));
  ON_CALL(*mock_app, set_app_icon_path(_)).WillByDefault(Return(true));
  ON_CALL(*mock_app, app_icon_path()).WillByDefault(ReturnRef(file_path));

  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
  (*msg)[am::strings::msg_params][am::strings::info] = "info1";

  Event event(hmi_apis::FunctionID::UI_SetAppIcon);
  event.set_smart_object(*msg);

  MessageSharedPtr ui_command_result;
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(_, am::commands::Command::CommandOrigin::ORIGIN_SDL))
      .WillOnce(DoAll(SaveArg<0>(&ui_command_result), Return(true)));

  req_vr->on_event(event);

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

TEST_F(SetAppIconRequestTest, Run_AllCorrectData_SendHMIRequest) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app));

  MockApplicationManagerSettings mock_app_mngr_settings;
  EXPECT_CALL(app_mngr_, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_settings));

  const std::string app_storeage_folder = "./storage";
  EXPECT_CALL(mock_app_mngr_settings, app_storage_folder())
      .WillRepeatedly(ReturnRef(app_storeage_folder));

  const std::string app_folder = "Test_app";
  EXPECT_CALL((*mock_app), folder_name()).WillRepeatedly(Return(app_folder));

  MockProtocolHandler mock_app_mngr_protocol_hendler;
  EXPECT_CALL(app_mngr_, protocol_handler())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler));

  MockProtocolHandlerSettings mock_app_mngr_protocol_hendler_settings;
  EXPECT_CALL(mock_app_mngr_protocol_hendler, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler_settings));

  const protocol_handler::MajorProtocolVersion mock_app_mngr_protocol_version =
      protocol_handler::PROTOCOL_VERSION_4;
  EXPECT_CALL(mock_app_mngr_protocol_hendler_settings,
              max_supported_protocol_version())
      .WillRepeatedly(Return(mock_app_mngr_protocol_version));

  const std::string icon_folder = app_storeage_folder + "/" + app_folder;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder())
      .WillRepeatedly(ReturnRef(icon_folder));

  const uint32_t storage_max_size = 10;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder_max_size())
      .WillRepeatedly(ReturnRef(storage_max_size));

  const std::string app_policy_id = "test_icon";
  EXPECT_CALL((*mock_app), policy_app_id())
      .WillRepeatedly(Return(app_policy_id));

  const uint32_t app_id = 152;
  EXPECT_CALL((*mock_app), app_id()).WillRepeatedly(Return(app_id));

  MessageSharedPtr result;
  EXPECT_CALL(app_mngr_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  std::string directory = app_storeage_folder + "/" + app_folder;
  std::string full_file_path = directory + "/" + sync_file_name;
  file_system::CreateDirectoryRecursively(directory);
  file_system::CreateFile(full_file_path);
  req->Run();
  file_system::RemoveDirectory(app_storeage_folder);

  const std::string expect_sync_file_name =
      file_system::ConvertPathForURL(full_file_path);
  const std::string actual_sync_file_name =
      (*result)[am::strings::msg_params][am::strings::sync_file_name]
               [am::strings::value].asString();
  EXPECT_EQ(expect_sync_file_name, actual_sync_file_name);

  const uint32_t expect_app_id = app_id;
  const uint32_t actual_app_id = static_cast<uint32_t>(
      (*result)[am::strings::msg_params][am::strings::app_id].asInt());
  EXPECT_EQ(expect_app_id, actual_app_id);
}

TEST_F(SetAppIconRequestTest, Run_NonExistentApp_ApplicationNotRegistered) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .Times(1)
      .WillOnce(Return(mock_app));

  MessageSharedPtr result;
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  req->Run();

  const mobile_apis::Result::eType expect_result_code =
      mobile_apis::Result::APPLICATION_NOT_REGISTERED;
  const mobile_apis::Result::eType actual_result_code =
      static_cast<const mobile_apis::Result::eType>(
          (*result)[am::strings::msg_params][am::strings::result_code].asInt());
  EXPECT_EQ(expect_result_code, actual_result_code);
}

TEST_F(SetAppIconRequestTest,
       Run_SyncFileNameWithForbiddenSymbols_InvalidData) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file/";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .Times(1)
      .WillOnce(Return(mock_app));

  MessageSharedPtr result;
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  req->Run();

  const mobile_apis::Result::eType expect_result_code =
      mobile_apis::Result::INVALID_DATA;
  const mobile_apis::Result::eType actual_result_code =
      static_cast<const mobile_apis::Result::eType>(
          (*result)[am::strings::msg_params][am::strings::result_code].asInt());
  EXPECT_EQ(expect_result_code, actual_result_code);
}

TEST_F(SetAppIconRequestTest, Run_NonExistentFile_InvalidData) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .Times(1)
      .WillOnce(Return(mock_app));

  MockApplicationManagerSettings mock_app_mngr_settings;
  EXPECT_CALL(app_mngr_, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_settings));

  const std::string app_storeage_folder = "./storage";
  EXPECT_CALL(mock_app_mngr_settings, app_storage_folder())
      .WillRepeatedly(ReturnRef(app_storeage_folder));

  const std::string app_folder = "Test_app";
  EXPECT_CALL((*mock_app), folder_name()).WillRepeatedly(Return(app_folder));

  MessageSharedPtr result;
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  req->Run();

  const mobile_apis::Result::eType expect_result_code =
      mobile_apis::Result::INVALID_DATA;
  const mobile_apis::Result::eType actual_result_code =
      static_cast<const mobile_apis::Result::eType>(
          (*result)[am::strings::msg_params][am::strings::result_code].asInt());
  EXPECT_EQ(expect_result_code, actual_result_code);
}

TEST_F(SetAppIconRequestTest,
       CopyToIconStorage_ProtocolVersionLessThanSupported_IconCopyingSkipped) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app));

  MockApplicationManagerSettings mock_app_mngr_settings;
  EXPECT_CALL(app_mngr_, get_settings())
      .Times(1)
      .WillOnce(ReturnRef(mock_app_mngr_settings));

  const std::string app_storeage_folder = "./storage";
  EXPECT_CALL(mock_app_mngr_settings, app_storage_folder())
      .WillRepeatedly(ReturnRef(app_storeage_folder));

  const std::string app_folder = "Test_app";
  EXPECT_CALL((*mock_app), folder_name()).WillRepeatedly(Return(app_folder));

  MockProtocolHandler mock_app_mngr_protocol_hendler;
  EXPECT_CALL(app_mngr_, protocol_handler())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler));

  MockProtocolHandlerSettings mock_app_mngr_protocol_hendler_settings;
  EXPECT_CALL(mock_app_mngr_protocol_hendler, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler_settings));

  const protocol_handler::MajorProtocolVersion mock_app_mngr_protocol_version =
      protocol_handler::PROTOCOL_VERSION_3;
  EXPECT_CALL(mock_app_mngr_protocol_hendler_settings,
              max_supported_protocol_version())
      .WillRepeatedly(Return(mock_app_mngr_protocol_version));

  std::string directory = app_storeage_folder + "/" + app_folder;
  std::string full_file_path = directory + "/" + sync_file_name;
  file_system::CreateDirectoryRecursively(directory);
  file_system::CreateFile(full_file_path);
  req->Run();
  file_system::RemoveDirectory(app_storeage_folder);
}

TEST_F(SetAppIconRequestTest,
       CopyToIconStorage_MaximumStorageSizeLessFileSize_IconCopyingSkipped) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app));

  MockApplicationManagerSettings mock_app_mngr_settings;
  EXPECT_CALL(app_mngr_, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_settings));

  const std::string app_storeage_folder = "./storage";
  EXPECT_CALL(mock_app_mngr_settings, app_storage_folder())
      .WillRepeatedly(ReturnRef(app_storeage_folder));

  const std::string app_folder = "Test_app";
  EXPECT_CALL((*mock_app), folder_name()).WillRepeatedly(Return(app_folder));

  MockProtocolHandler mock_app_mngr_protocol_hendler;
  EXPECT_CALL(app_mngr_, protocol_handler())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler));

  MockProtocolHandlerSettings mock_app_mngr_protocol_hendler_settings;
  EXPECT_CALL(mock_app_mngr_protocol_hendler, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler_settings));

  const protocol_handler::MajorProtocolVersion mock_app_mngr_protocol_version =
      protocol_handler::PROTOCOL_VERSION_4;
  EXPECT_CALL(mock_app_mngr_protocol_hendler_settings,
              max_supported_protocol_version())
      .WillRepeatedly(Return(mock_app_mngr_protocol_version));

  const std::string icon_folder = app_storeage_folder + "/" + app_folder;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder())
      .WillRepeatedly(ReturnRef(icon_folder));

  const uint32_t storage_max_size = 5;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder_max_size())
      .WillRepeatedly(ReturnRef(storage_max_size));

  std::string directory = app_storeage_folder + "/" + app_folder;
  std::string full_file_path = directory + "/" + sync_file_name;
  file_system::CreateDirectoryRecursively(directory);
  file_system::CreateFile(full_file_path);
  const std::vector<uint8_t> data(10, 10);
  file_system::Write(full_file_path, data);
  req->Run();
  file_system::RemoveDirectory(app_storeage_folder);
}

TEST_F(SetAppIconRequestTest, CopyToIconStorage_ZeroIcons_IconSavingSkipped) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .Times(1)
      .WillOnce(Return(mock_app));

  MockApplicationManagerSettings mock_app_mngr_settings;
  EXPECT_CALL(app_mngr_, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_settings));

  const std::string app_storeage_folder = "./storage";
  EXPECT_CALL(mock_app_mngr_settings, app_storage_folder())
      .WillRepeatedly(ReturnRef(app_storeage_folder));

  const std::string app_folder = "Test_app";
  EXPECT_CALL((*mock_app), folder_name()).WillRepeatedly(Return(app_folder));

  MockProtocolHandler mock_app_mngr_protocol_hendler;
  EXPECT_CALL(app_mngr_, protocol_handler())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler));

  MockProtocolHandlerSettings mock_app_mngr_protocol_hendler_settings;
  EXPECT_CALL(mock_app_mngr_protocol_hendler, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler_settings));

  const protocol_handler::MajorProtocolVersion mock_app_mngr_protocol_version =
      protocol_handler::PROTOCOL_VERSION_4;
  EXPECT_CALL(mock_app_mngr_protocol_hendler_settings,
              max_supported_protocol_version())
      .WillRepeatedly(Return(mock_app_mngr_protocol_version));

  const std::string icon_folder = app_storeage_folder + "/" + app_folder;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder())
      .WillRepeatedly(ReturnRef(icon_folder));

  const uint32_t storage_max_size = 11;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder_max_size())
      .WillRepeatedly(ReturnRef(storage_max_size));

  const uint32_t icons_amount = 0;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_amount_to_remove())
      .WillRepeatedly(ReturnRef(icons_amount));

  std::string directory = app_storeage_folder + "/" + app_folder;
  std::string full_file_path = directory + "/" + sync_file_name;
  file_system::CreateDirectoryRecursively(directory);
  file_system::CreateFile(full_file_path);
  const std::vector<uint8_t> data(10, 10);
  file_system::Write(full_file_path, data);
  req->Run();
  file_system::RemoveDirectory(app_storeage_folder);
}

TEST_F(SetAppIconRequestTest,
       CopyToIconStorage_NonExistentApp_ApplicationNotRegistered) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  MockAppPtr mock_app_nonexistent;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app))
      .WillOnce(Return(mock_app_nonexistent));

  MockApplicationManagerSettings mock_app_mngr_settings;
  EXPECT_CALL(app_mngr_, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_settings));

  const std::string app_storeage_folder = "./storage";
  EXPECT_CALL(mock_app_mngr_settings, app_storage_folder())
      .WillRepeatedly(ReturnRef(app_storeage_folder));

  const std::string app_folder = "Test_app";
  EXPECT_CALL((*mock_app), folder_name()).WillRepeatedly(Return(app_folder));

  MockProtocolHandler mock_app_mngr_protocol_hendler;
  EXPECT_CALL(app_mngr_, protocol_handler())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler));

  MockProtocolHandlerSettings mock_app_mngr_protocol_hendler_settings;
  EXPECT_CALL(mock_app_mngr_protocol_hendler, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler_settings));

  const protocol_handler::MajorProtocolVersion mock_app_mngr_protocol_version =
      protocol_handler::PROTOCOL_VERSION_4;
  EXPECT_CALL(mock_app_mngr_protocol_hendler_settings,
              max_supported_protocol_version())
      .WillRepeatedly(Return(mock_app_mngr_protocol_version));

  const std::string icon_folder = app_storeage_folder + "/" + app_folder;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder())
      .WillRepeatedly(ReturnRef(icon_folder));

  const uint32_t storage_max_size = 10;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder_max_size())
      .WillRepeatedly(ReturnRef(storage_max_size));

  EXPECT_CALL((*mock_app), policy_app_id()).Times(0);

  std::string directory = app_storeage_folder + "/" + app_folder;
  std::string full_file_path = directory + "/" + sync_file_name;
  file_system::CreateDirectoryRecursively(directory);
  file_system::CreateFile(full_file_path);
  req->Run();
  file_system::RemoveDirectory(app_storeage_folder);
}

TEST_F(SetAppIconRequestTest,
       CopyToIconStorage_PolicyAppIdEqualFolderName_CantCreateIcon) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillRepeatedly(Return(mock_app));

  MockApplicationManagerSettings mock_app_mngr_settings;
  EXPECT_CALL(app_mngr_, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_settings));

  const std::string app_storeage_folder = "./storage";
  EXPECT_CALL(mock_app_mngr_settings, app_storage_folder())
      .WillRepeatedly(ReturnRef(app_storeage_folder));

  const std::string app_folder = "Test_app";
  EXPECT_CALL((*mock_app), folder_name()).WillRepeatedly(Return(app_folder));

  MockProtocolHandler mock_app_mngr_protocol_hendler;
  EXPECT_CALL(app_mngr_, protocol_handler())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler));

  MockProtocolHandlerSettings mock_app_mngr_protocol_hendler_settings;
  EXPECT_CALL(mock_app_mngr_protocol_hendler, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler_settings));

  const protocol_handler::MajorProtocolVersion mock_app_mngr_protocol_version =
      protocol_handler::PROTOCOL_VERSION_4;
  EXPECT_CALL(mock_app_mngr_protocol_hendler_settings,
              max_supported_protocol_version())
      .WillRepeatedly(Return(mock_app_mngr_protocol_version));

  const std::string icon_folder = app_storeage_folder + "/" + app_folder;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder())
      .WillRepeatedly(ReturnRef(icon_folder));

  const uint32_t storage_max_size = 10;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder_max_size())
      .WillRepeatedly(ReturnRef(storage_max_size));

  const std::string app_policy_id = "test_icon";
  EXPECT_CALL((*mock_app), policy_app_id())
      .WillRepeatedly(Return(app_policy_id));

  std::string directory = app_storeage_folder + "/" + app_folder;
  std::string full_file_path = directory + "/" + sync_file_name;
  file_system::CreateDirectoryRecursively(directory);
  file_system::CreateDirectoryRecursively(directory + "/" + app_policy_id);
  file_system::CreateFile(full_file_path);
  req->Run();
  file_system::RemoveDirectory(app_storeage_folder);
}

TEST_F(SetAppIconRequestTest, CopyToIconStorage__) {
  MessageSharedPtr msg = CreateFullParamsUISO();
  const std::string sync_file_name = "sync_file";
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = sync_file_name;

  utils::SharedPtr<SetAppIconRequest> req =
      CreateCommand<SetAppIconRequest>(msg);

  MockAppPtr mock_app = CreateMockApp();
  MockAppPtr mock_app_nonexistent;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app))
      .WillOnce(Return(mock_app_nonexistent));

  MockApplicationManagerSettings mock_app_mngr_settings;
  EXPECT_CALL(app_mngr_, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_settings));

  const std::string app_storeage_folder = "./storage";
  EXPECT_CALL(mock_app_mngr_settings, app_storage_folder())
      .WillRepeatedly(ReturnRef(app_storeage_folder));

  const std::string app_folder = "Test_app";
  EXPECT_CALL((*mock_app), folder_name()).WillRepeatedly(Return(app_folder));

  MockProtocolHandler mock_app_mngr_protocol_hendler;
  EXPECT_CALL(app_mngr_, protocol_handler())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler));

  MockProtocolHandlerSettings mock_app_mngr_protocol_hendler_settings;
  EXPECT_CALL(mock_app_mngr_protocol_hendler, get_settings())
      .WillRepeatedly(ReturnRef(mock_app_mngr_protocol_hendler_settings));

  const protocol_handler::MajorProtocolVersion mock_app_mngr_protocol_version =
      protocol_handler::PROTOCOL_VERSION_4;
  EXPECT_CALL(mock_app_mngr_protocol_hendler_settings,
              max_supported_protocol_version())
      .WillRepeatedly(Return(mock_app_mngr_protocol_version));

  const std::string icon_folder = app_storeage_folder + "/" + app_folder;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder())
      .WillRepeatedly(ReturnRef(icon_folder));

  const uint32_t storage_max_size = 11;
  const uint32_t storage_max_size_max = 120;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_folder_max_size())
      .WillOnce(ReturnRef(storage_max_size))
      .WillOnce(ReturnRef(storage_max_size))
      .WillOnce(ReturnRef(storage_max_size_max));

  const uint32_t icons_amount = 1;
  EXPECT_CALL(mock_app_mngr_settings, app_icons_amount_to_remove())
      .WillRepeatedly(ReturnRef(icons_amount));

  std::string directory = app_storeage_folder + "/" + app_folder;
  std::string full_file_path = directory + "/" + sync_file_name;
  file_system::CreateDirectoryRecursively(directory);
  file_system::CreateFile(full_file_path);
  const std::vector<uint8_t> data(10, 10);
  file_system::Write(full_file_path, data);
  req->Run();
  file_system::RemoveDirectory(app_storeage_folder);
}

}  // namespace set_app_icon_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace tests

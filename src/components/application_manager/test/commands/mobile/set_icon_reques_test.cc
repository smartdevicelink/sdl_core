/*
 Copyright (c) 2016, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include <stdint.h>
#include <string>

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/file_system.h"
#include "commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "mobile/set_icon_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace set_icon_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::Mock;
using ::testing::_;

using am::commands::SetIconRequest;
using am::commands::MessageSharedPtr;

typedef ::utils::SharedPtr<SetIconRequest> CommandPtr;

namespace {
const uint32_t kConnectionKey = 1u;
const std::string kFileName = "file.txt";
}  // namespace

MATCHER_P(CheckMsgType, msg_type, "") {
  return msg_type ==
         static_cast<int32_t>(
             (*arg)[am::strings::params][am::strings::message_type].asInt());
}

class SetIconRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    ON_CALL(mock_app_manager_, get_settings())
        .WillByDefault(ReturnRef(mock_app_manager_settings_));
  }
  void TearDown() OVERRIDE {
    if (file_system::FileExists("./" + kFileName)) {
      EXPECT_TRUE(file_system::DeleteFile("./" + kFileName));
    }
  }
};

TEST_F(SetIconRequestTest, Run_InvalidApp_UNSUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  CommandPtr command(CreateCommand<SetIconRequest>(msg));
  MockAppPtr invalid_mock_app;
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(invalid_mock_app));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(SetIconRequestTest, Run_InvalidFile_UNSUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = kFileName;

  CommandPtr command(CreateCommand<SetIconRequest>(msg));
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  const std::string folder_path = file_system::CurrentWorkingDirectory();
  EXPECT_CALL(mock_app_manager_settings_, app_storage_folder())
      .WillOnce(ReturnRef(folder_path));
  EXPECT_CALL(*mock_app, folder_name()).WillOnce(Return("./"));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::INVALID_DATA),
                          am::commands::Command::CommandOrigin::ORIGIN_SDL));

  command->Run();
}

TEST_F(SetIconRequestTest, Run_SUCCESS) {
  MessageSharedPtr msg(CreateMessage(smart_objects::SmartType_Map));
  (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  (*msg)[am::strings::msg_params][am::strings::sync_file_name] = kFileName;

  file_system::CreateFile("./" + kFileName);
  CommandPtr command(CreateCommand<SetIconRequest>(msg));
  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  const std::string folder_path = file_system::CurrentWorkingDirectory();
  EXPECT_CALL(mock_app_manager_settings_, app_storage_folder())
      .WillOnce(ReturnRef(folder_path));
  EXPECT_CALL(*mock_app, folder_name()).WillOnce(Return("./"));
  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kConnectionKey));

  EXPECT_CALL(mock_app_manager_,
              ManageHMICommand(CheckMsgType(am::MessageType::kRequest)))
      .WillOnce(Return(true));

  command->Run();
}

TEST_F(SetIconRequestTest, OnEvent_InvalidEventId_UNSUCCESS) {
  CommandPtr command(CreateCommand<SetIconRequest>());
  am::event_engine::Event event(hmi_apis::FunctionID::INVALID_ENUM);
  SmartObject msg(smart_objects::SmartType_Map);

  event.set_smart_object(msg);

  EXPECT_CALL(mock_app_manager_, application(_)).Times(0);
  command->on_event(event);
}

TEST_F(SetIconRequestTest, OnEvent_SUCCESS) {
  const std::string file_path = "./file.txt";
  MessageSharedPtr message = CreateMessage();
  (*message)[am::strings::msg_params][am::strings::sync_file_name]
            [am::strings::value] = file_path;
  (*message)[am::strings::params][am::strings::connection_key] = kConnectionKey;

  CommandPtr command(CreateCommand<SetIconRequest>(message));

  am::event_engine::Event event(hmi_apis::FunctionID::UI_SetAppIcon);
  MessageSharedPtr msg = CreateMessage();
  (*msg)[am::strings::msg_params] = 0;
  (*msg)[am::strings::params][am::hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
  event.set_smart_object(*msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(mock_app_manager_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, set_app_icon_path(file_path)).WillOnce(Return(true));
  EXPECT_CALL(*mock_app, app_icon_path()).WillOnce(ReturnRef(file_path));

  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS),
                                  am::commands::Command::ORIGIN_SDL));

  command->on_event(event);
}

}  // namespace set_display_layout_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

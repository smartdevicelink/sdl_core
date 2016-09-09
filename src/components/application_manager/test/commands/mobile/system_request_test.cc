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
#include "mobile/system_request.h"
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
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "smart_objects/smart_object.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {

using application_manager::commands::SystemRequest;
using ::testing::DefaultValue;
using application_manager::MockMessageHelper;

namespace mobile_result = mobile_apis::Result;
namespace am = application_manager;

typedef SharedPtr<SystemRequest> CommandPtr;

namespace {
const uint32_t kAppId = 10u;
const std::string kPolicyAppId = "12345";
const uint32_t kKey = 5u;
const std::string kBinaryFolder = "test_binary_folder";
const std::string kStorageFolder = "test_storage_folder";
const std::string kStorageFolder2 = "test_storage_folder2";
}

class SystemRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  SystemRequestTest()
      : app_(CreateMockApp()), info_(""), file_name_("file_name") {
    msg_ = CreateMessage(smart_objects::SmartType_Map);
    (*msg_)[am::strings::params][am::strings::connection_key] = kKey;
  }

  void SetUp() OVERRIDE {
    ON_CALL(*app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(*app_, policy_app_id()).WillByDefault(Return(kPolicyAppId));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(policy_handler_mock));
  }

  MockAppPtr app_;
  MessageSharedPtr msg_;
  std::string info_;
  policy_test::MockPolicyHandlerInterface policy_handler_mock;
  const std::string file_name_;
};

TEST_F(SystemRequestTest, Run_ApplicationIsNotRegistered) {
  CommandPtr command(CreateCommand<SystemRequest>(msg_));

  ON_CALL(app_mngr_, application(_))
      .WillByDefault(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(SystemRequestTest, Run_TypeQueryAps_UnsupportedResourse) {
  (*msg_)[am::strings::msg_params][am::strings::request_type] =
      mobile_apis::RequestType::QUERY_APPS;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::UNSUPPORTED_RESOURCE, info_, false),
          _));

  command->Run();
}

TEST_F(SystemRequestTest, Run_TypeDisallowed_DisallowedResult) {
  mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::CLIMATE;
  (*msg_)[am::strings::msg_params][am::strings::request_type] = request_type;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(policy_handler_mock));

  EXPECT_CALL(policy_handler_mock,
              IsRequestTypeAllowed(kPolicyAppId, request_type))
      .WillOnce(Return(false));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::DISALLOWED, info_, false), _));

  command->Run();
}

TEST_F(SystemRequestTest, Run_FileNameContainSlash_InvalidData) {
  mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::CLIMATE;
  (*msg_)[am::strings::msg_params][am::strings::request_type] = request_type;
  (*msg_)[am::strings::msg_params][am::strings::file_name] = "file_name/";
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(policy_handler_mock,
              IsRequestTypeAllowed(kPolicyAppId, request_type))
      .WillOnce(Return(true));
  info_ = "Sync file name contains forbidden symbols.";
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::INVALID_DATA, info_, false), _));

  command->Run();
}

TEST_F(SystemRequestTest, Run_FileWithBinaryDataSaveUnsuccessful_GenericError) {
  mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::CLIMATE;
  std::vector<uint8_t> binary_data;
  binary_data.push_back(0x1);
  binary_data.push_back(0x2);
  binary_data.push_back(0x4);
  (*msg_)[am::strings::msg_params][am::strings::request_type] = request_type;
  (*msg_)[am::strings::msg_params][am::strings::file_name] = file_name_;
  (*msg_)[am::strings::params][am::strings::binary_data] = binary_data;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(policy_handler_mock,
              IsRequestTypeAllowed(kPolicyAppId, request_type))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_settings_, system_files_path())
      .WillRepeatedly(ReturnRef(kBinaryFolder));
  EXPECT_CALL(app_mngr_, SaveBinary(binary_data, kBinaryFolder, file_name_, 0))
      .WillOnce(Return(mobile_result::INVALID_DATA));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::GENERIC_ERROR, info_, false), _));

  command->Run();
}

TEST_F(SystemRequestTest,
       Run_FileWithoutBinaryDataNotHTTPTypeNotSystemFile_Rejected) {
  mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::CLIMATE;
  (*msg_)[am::strings::msg_params][am::strings::request_type] = request_type;
  (*msg_)[am::strings::msg_params][am::strings::file_name] = file_name_;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(policy_handler_mock,
              IsRequestTypeAllowed(kPolicyAppId, request_type))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_settings_, system_files_path())
      .WillRepeatedly(ReturnRef(kBinaryFolder));
  EXPECT_CALL(app_mngr_, SaveBinary(_, _, _, _)).Times(0);

  EXPECT_CALL(app_mngr_settings_, app_storage_folder())
      .WillRepeatedly(ReturnRef(kStorageFolder));
  am::AppFile ap_file;
  ON_CALL(*app_, folder_name()).WillByDefault(Return(kStorageFolder2));
  ap_file.is_download_complete = false;
  EXPECT_CALL(*app_,
              GetFile(kStorageFolder + file_system::GetPathDelimiter() +
                      kStorageFolder2 + file_system::GetPathDelimiter() +
                      file_name_)).WillRepeatedly(Return(&ap_file));
  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::REJECTED, info_, false), _));

  command->Run();
}

TEST_F(
    SystemRequestTest,
    Run_FileInFileSystemWithoutBinaryDataNotHTTPTypeNotSystemFile_SendRequestToHMI) {
  mobile_apis::RequestType::eType request_type = mobile_apis::RequestType::HTTP;
  (*msg_)[am::strings::msg_params][am::strings::request_type] = request_type;
  (*msg_)[am::strings::msg_params][am::strings::file_name] = "IVSU";
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(policy_handler_mock,
              IsRequestTypeAllowed(kPolicyAppId, request_type))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_settings_, system_files_path())
      .WillRepeatedly(ReturnRef(kBinaryFolder));
  EXPECT_CALL(app_mngr_, SaveBinary(_, _, _, _)).Times(0);

  EXPECT_CALL(app_mngr_settings_, app_storage_folder())
      .WillRepeatedly(ReturnRef(kStorageFolder));
  ON_CALL(*app_, folder_name()).WillByDefault(Return("app_storage"));
  am::AppFile ap_file;
  EXPECT_CALL(*app_,
              GetFile(kStorageFolder + file_system::GetPathDelimiter() +
                      "app_storage" + file_system::GetPathDelimiter() +
                      "0IVSU")).WillRepeatedly(Return(&ap_file));
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::BasicCommunication_SystemRequest)))
      .WillOnce(Return(true));

  command->Run();
}

TEST_F(SystemRequestTest, OnEvent_SuccessResult_SuccessfulCommand) {
  Event event(Event::EventID::BasicCommunication_SystemRequest);

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));

  const hmi_apis::Common_Result::eType hmi_code =
      hmi_apis::Common_Result::SUCCESS;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_code;
  smart_objects::SmartObject& unused = (*event_msg)[am::strings::msg_params];
  // msg_params is unused in this case
  UNUSED(unused)
  event.set_smart_object(*event_msg);

  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::SUCCESS, info_, true), _));
  command->on_event(event);
}

TEST_F(SystemRequestTest, OnEvent_WarningResult_SuccessfulCommand) {
  Event event(Event::EventID::BasicCommunication_SystemRequest);

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));

  const hmi_apis::Common_Result::eType hmi_code =
      hmi_apis::Common_Result::WARNINGS;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_code;
  smart_objects::SmartObject& unused = (*event_msg)[am::strings::msg_params];
  // msg_params is unused in this case
  UNUSED(unused)
  event.set_smart_object(*event_msg);

  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::WARNINGS, info_, true), _));
  command->on_event(event);
}

TEST_F(SystemRequestTest, OnEvent_UnsuccesfulResult_SendUnsuccessfulCommand) {
  Event event(Event::EventID::BasicCommunication_SystemRequest);

  MessageSharedPtr event_msg(CreateMessage(smart_objects::SmartType_Map));

  // Code is neither SUCCESS nor WARNINGS
  const hmi_apis::Common_Result::eType hmi_code =
      hmi_apis::Common_Result::GENERIC_ERROR;
  (*event_msg)[am::strings::params][am::hmi_response::code] = hmi_code;
  smart_objects::SmartObject& unused = (*event_msg)[am::strings::msg_params];
  // msg_params is unused in this case
  UNUSED(unused)
  event.set_smart_object(*event_msg);

  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(app_mngr_, application(kKey)).WillByDefault(Return(app_));

  // Result is false
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::GENERIC_ERROR, info_, false), _));
  command->on_event(event);
}

}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

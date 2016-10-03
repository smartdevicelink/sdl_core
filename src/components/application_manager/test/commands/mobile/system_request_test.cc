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
#include <sstream>

#include "gtest/gtest.h"
#include "mobile/system_request.h"
#include "utils/shared_ptr.h"
#include "commands/commands_test.h"
#include "commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "smart_objects/smart_object.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace system_request {

using application_manager::commands::SystemRequest;
using ::testing::DefaultValue;

namespace mobile_result = mobile_apis::Result;
namespace am = application_manager;

typedef SharedPtr<SystemRequest> CommandPtr;

namespace {
const uint32_t kAppId = 10u;
const std::string kPolicyAppId = "12345";
const uint32_t kConnectionKey = 5u;
const std::string kBinaryFolder = "test_binary_folder";
const std::string kStorageFolder = "test_storage_folder";
const std::string kStorageFolder2 = "test_storage_folder2";
const mobile_apis::RequestType::eType kRequestType =
    mobile_apis::RequestType::CLIMATE;
}

class SystemRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  SystemRequestTest()
      : app_(CreateMockApp()), info_(""), file_name_("file_name") {
    msg_ = CreateMessage(smart_objects::SmartType_Map);
    (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
  }

  void SetUp() OVERRIDE {
    ON_CALL(*app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(*app_, policy_app_id()).WillByDefault(Return(kPolicyAppId));
    ON_CALL(mock_app_manager_, GetPolicyHandler())
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

  ON_CALL(mock_app_manager_, application(_))
      .WillByDefault(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(SystemRequestTest, Run_TypeDisallowed_DisallowedResult) {
  (*msg_)[am::strings::msg_params][am::strings::request_type] = kRequestType;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  EXPECT_CALL(policy_handler_mock,
              IsRequestTypeAllowed(kPolicyAppId, kRequestType))
      .WillOnce(Return(false));
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::DISALLOWED, info_, false), _));

  command->Run();
}

TEST_F(SystemRequestTest, Run_FileWithBinaryDataSaveUnsuccessful_GenericError) {
  std::vector<uint8_t> binary_data;
  binary_data.push_back(0x1);
  binary_data.push_back(0x2);
  binary_data.push_back(0x4);
  (*msg_)[am::strings::msg_params][am::strings::request_type] = kRequestType;
  (*msg_)[am::strings::msg_params][am::strings::file_name] = file_name_;
  (*msg_)[am::strings::params][am::strings::binary_data] = binary_data;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  EXPECT_CALL(policy_handler_mock,
              IsRequestTypeAllowed(kPolicyAppId, kRequestType))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_app_manager_settings_, system_files_path())
      .WillRepeatedly(ReturnRef(kBinaryFolder));
  EXPECT_CALL(mock_app_manager_,
              SaveBinary(binary_data, kBinaryFolder, file_name_, 0))
      .WillOnce(Return(mobile_result::INVALID_DATA));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::GENERIC_ERROR, info_, false), _));

  command->Run();
}

TEST_F(SystemRequestTest,
       Run_FileWithoutBinaryDataNotHTTPTypeNotSystemFile_Rejected) {
  (*msg_)[am::strings::msg_params][am::strings::request_type] = kRequestType;
  (*msg_)[am::strings::msg_params][am::strings::file_name] = file_name_;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  EXPECT_CALL(policy_handler_mock,
              IsRequestTypeAllowed(kPolicyAppId, kRequestType))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_app_manager_settings_, system_files_path())
      .WillRepeatedly(ReturnRef(kBinaryFolder));
  EXPECT_CALL(mock_app_manager_, SaveBinary(_, _, _, _)).Times(0);

  EXPECT_CALL(mock_app_manager_settings_, app_storage_folder())
      .WillRepeatedly(ReturnRef(kStorageFolder));
  am::AppFile ap_file;
  ON_CALL(*app_, folder_name()).WillByDefault(Return(kStorageFolder2));
  ap_file.is_download_complete = false;
  EXPECT_CALL(*app_,
              GetFile(kStorageFolder + file_system::GetPathDelimiter() +
                      kStorageFolder2 + file_system::GetPathDelimiter() +
                      file_name_)).WillRepeatedly(Return(&ap_file));
  EXPECT_CALL(mock_app_manager_,
              ManageMobileCommand(
                  MobileResponseIs(mobile_result::REJECTED, info_, false), _));

  command->Run();
}

TEST_F(
    SystemRequestTest,
    Run_FileInFileSystemWithoutBinaryDataNotHTTPTypeNotSystemFile_SendRequestToHMI) {
  const std::string file_name = "IVSU";
  mobile_apis::RequestType::eType request_type = mobile_apis::RequestType::HTTP;
  (*msg_)[am::strings::msg_params][am::strings::request_type] = request_type;
  (*msg_)[am::strings::msg_params][am::strings::file_name] = file_name;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  EXPECT_CALL(policy_handler_mock,
              IsRequestTypeAllowed(kPolicyAppId, request_type))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_app_manager_settings_, system_files_path())
      .WillRepeatedly(ReturnRef(kBinaryFolder));
  EXPECT_CALL(mock_app_manager_, SaveBinary(_, _, _, _)).Times(0);

  EXPECT_CALL(mock_app_manager_settings_, app_storage_folder())
      .WillRepeatedly(ReturnRef(kStorageFolder));
  ON_CALL(*app_, folder_name()).WillByDefault(Return("app_storage"));
  am::AppFile ap_file;

  // To avoid override of existing file,
  // command append index as a suffix to
  // the file name and increment it, each time
  // when command saves a file.
  std::ostringstream final_file_name;
  final_file_name << SystemRequest::file_index() << file_name;

  const std::string app_full_file_path = file_system::ConcatPath(
      kStorageFolder, "app_storage", final_file_name.str());

  EXPECT_CALL(*app_, GetFile(app_full_file_path))
      .WillRepeatedly(Return(&ap_file));

  EXPECT_CALL(mock_app_manager_, ManageMobileCommand(_, _)).Times(0);

  EXPECT_CALL(mock_app_manager_,
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
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  EXPECT_CALL(mock_app_manager_,
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
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  EXPECT_CALL(mock_app_manager_,
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
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));

  // Result is false
  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(
          MobileResponseIs(mobile_result::GENERIC_ERROR, info_, false), _));
  command->on_event(event);
}

TEST_F(SystemRequestTest, Run_InvalidQueryAppData_GENERIC_ERROR) {
  const mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::QUERY_APPS;
  (*msg_)[am::strings::msg_params][am::strings::file_name] = file_name_;
  (*msg_)[am::strings::msg_params][am::strings::request_type] = request_type;

  // json_data was not contain required parameter, `json::response`
  const std::string json_data("{\"invalid_json_data\":{}}");
  const std::vector<uint8_t> binary_data(json_data.begin(), json_data.end());
  (*msg_)[am::strings::params][am::strings::binary_data] = binary_data;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));

  ON_CALL(policy_handler_mock, IsRequestTypeAllowed(kPolicyAppId, request_type))
      .WillByDefault(Return(true));
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  ON_CALL(mock_app_manager_settings_, system_files_path())
      .WillByDefault(ReturnRef(kBinaryFolder));
  ON_CALL(mock_app_manager_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kStorageFolder));

  ON_CALL(mock_app_manager_, SaveBinary(_, kBinaryFolder, file_name_, 0))
      .WillByDefault(Return(mobile_result::SUCCESS));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::GENERIC_ERROR), _));

  command->Run();
}

TEST_F(SystemRequestTest, Run_ValidQueryAppData_SUCCESS) {
  const mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::QUERY_APPS;
  (*msg_)[am::strings::msg_params][am::strings::file_name] = file_name_;
  (*msg_)[am::strings::msg_params][am::strings::request_type] = request_type;

  std::ostringstream json_data_stream;
  json_data_stream << "{\"" << am::json::response << "\":{}}";

  // json_data should contain `json::response` parameter, to be valid
  const std::string json_data(json_data_stream.str());
  const std::vector<uint8_t> binary_data(json_data.begin(), json_data.end());

  (*msg_)[am::strings::params][am::strings::binary_data] = binary_data;
  CommandPtr command(CreateCommand<SystemRequest>(msg_));

  ON_CALL(policy_handler_mock, IsRequestTypeAllowed(kPolicyAppId, request_type))
      .WillByDefault(Return(true));
  ON_CALL(mock_app_manager_, application(kConnectionKey))
      .WillByDefault(Return(app_));
  ON_CALL(mock_app_manager_settings_, system_files_path())
      .WillByDefault(ReturnRef(kBinaryFolder));
  ON_CALL(mock_app_manager_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kStorageFolder));

  ON_CALL(mock_app_manager_, SaveBinary(_, kBinaryFolder, file_name_, 0))
      .WillByDefault(Return(mobile_result::SUCCESS));

  EXPECT_CALL(mock_app_manager_, ProcessQueryApp(_, kConnectionKey));

  EXPECT_CALL(
      mock_app_manager_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  command->Run();
}

}  // namespace system_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

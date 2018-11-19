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

#include "gtest/gtest.h"

#include "mobile/system_request.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace system_request {

namespace am = application_manager;
using sdl_rpc_plugin::commands::SystemRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockHmiInterfaces;
using am::event_engine::Event;
using am::MessageType;
using policy_test::MockPolicyHandlerInterface;

using ::testing::_;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;

namespace {
const uint32_t kConnectionKey = 2u;
const std::string kAppPolicyId = "fake-app-id";
const uint32_t kDefaultTimeout = 1000u;
const std::string kAppFolderName = "fake-app-name";
const std::string kAppStorageFolder = "fake-storage";
const std::string kSystemFilesPath = "/fake/system/files";
const std::string kFileName = "Filename";
const uint32_t kHmiAppId = 3u;
}  // namespace

class SystemRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SystemRequestTest() : mock_app_(CreateMockApp()) {}

 protected:
  MessageSharedPtr CreateIVSUMessage() {
    MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Map);
    (*msg)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg)[am::strings::msg_params][am::strings::request_type] =
        mobile_apis::RequestType::HTTP;
    (*msg)[am::strings::msg_params][am::strings::file_name] = kFileName;
    return msg;
  }

  void PreConditions() {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));

    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(*mock_app_, policy_app_id()).WillByDefault(Return(kAppPolicyId));
    ON_CALL(*mock_app_, folder_name()).WillByDefault(Return(kAppFolderName));
    ON_CALL(*mock_app_, hmi_app_id()).WillByDefault(Return(kHmiAppId));

    ON_CALL(app_mngr_settings_, system_files_path())
        .WillByDefault(ReturnRef(kSystemFilesPath));
    ON_CALL(app_mngr_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));

    ON_CALL(mock_policy_handler_, IsRequestTypeAllowed(kAppPolicyId, _))
        .WillByDefault(Return(true));
  }

  void ExpectManageMobileCommandWithResultCode(
      const mobile_apis::Result::eType code) {
    EXPECT_CALL(
        mock_rpc_service_,
        ManageMobileCommand(MobileResultCodeIs(code),
                            am::commands::Command::CommandSource::SOURCE_SDL));
  }

  MockAppPtr mock_app_;
};

TEST_F(SystemRequestTest, Run_HTTP_FileName_no_binary_data_REJECTED) {
  PreConditions();
  MessageSharedPtr msg = CreateIVSUMessage();

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::REJECTED);

  std::shared_ptr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  command->Run();
}

TEST_F(SystemRequestTest,
       Run_RequestTypeAllowedAndRequestSubTypeAllowed_SendHMIRequest) {
  MessageSharedPtr msg = CreateIVSUMessage();

  (*msg)[am::strings::msg_params][am::strings::request_type] =
      mobile_apis::RequestType::OEM_SPECIFIC;

  const std::string request_subtype = "fakeSubType";
  (*msg)[am::strings::msg_params][am::strings::request_subtype] =
      request_subtype;

  const std::vector<uint8_t> binary_data = {1u, 2u};
  (*msg)[am::strings::params][am::strings::binary_data] = binary_data;

  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId,
                                   mobile_apis::RequestType::OEM_SPECIFIC))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_policy_handler_,
              IsRequestSubTypeAllowed(kAppPolicyId, request_subtype))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_,
              SaveBinary(binary_data, kSystemFilesPath, kFileName, 0u))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  smart_objects::SmartObjectSPtr result;
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_))
      .WillOnce(DoAll(SaveArg<0>(&result), Return(true)));

  std::shared_ptr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  ASSERT_TRUE(command->Init());
  command->Run();

  EXPECT_EQ(MessageType::kRequest,
            (*result)[am::strings::params][am::strings::message_type].asInt());
  EXPECT_EQ(
      mobile_apis::RequestType::OEM_SPECIFIC,
      (*result)[am::strings::msg_params][am::strings::request_type].asInt());
  EXPECT_EQ(
      request_subtype,
      (*msg)[am::strings::msg_params][am::strings::request_subtype].asString());
}

TEST_F(
    SystemRequestTest,
    Run_RequestTypeAllowedAndRequestSubTypeDisallowed_SendDisallowedResponse) {
  MessageSharedPtr msg = CreateIVSUMessage();

  (*msg)[am::strings::msg_params][am::strings::request_type] =
      mobile_apis::RequestType::OEM_SPECIFIC;

  const std::string request_subtype = "fakeSubType2";
  (*msg)[am::strings::msg_params][am::strings::request_subtype] =
      request_subtype;

  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId,
                                   mobile_apis::RequestType::OEM_SPECIFIC))
      .WillOnce(Return(true));

  EXPECT_CALL(mock_policy_handler_,
              IsRequestSubTypeAllowed(kAppPolicyId, request_subtype))
      .WillOnce(Return(false));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::DISALLOWED);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);

  std::shared_ptr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(SystemRequestTest, Run_RequestTypeDisallowed_SendDisallowedResponse) {
  MessageSharedPtr msg = CreateIVSUMessage();

  (*msg)[am::strings::msg_params][am::strings::request_type] =
      mobile_apis::RequestType::OEM_SPECIFIC;

  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId,
                                   mobile_apis::RequestType::OEM_SPECIFIC))
      .WillOnce(Return(false));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::DISALLOWED);
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_)).Times(0);

  std::shared_ptr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(SystemRequestTest, Run_FileWithBinaryDataSaveUnsuccessful_GenericError) {
  am::BinaryData binary_data;
  binary_data.push_back(0x1);
  binary_data.push_back(0x2);
  binary_data.push_back(0x4);

  const std::string file_name = "file_name";
  MessageSharedPtr msg = CreateIVSUMessage();
  const mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::CLIMATE;
  (*msg)[am::strings::msg_params][am::strings::request_type] = request_type;
  (*msg)[am::strings::msg_params][am::strings::file_name] = file_name;
  (*msg)[am::strings::params][am::strings::binary_data] = binary_data;

  PreConditions();

  ON_CALL(mock_policy_handler_,
          IsRequestTypeAllowed(kAppPolicyId, request_type))
      .WillByDefault(Return(true));

  const std::string binary_folder = "test_binary_folder";
  EXPECT_CALL(app_mngr_settings_, system_files_path())
      .WillRepeatedly(ReturnRef(binary_folder));
  EXPECT_CALL(app_mngr_, SaveBinary(binary_data, binary_folder, file_name, 0))
      .WillOnce(Return((mobile_apis::Result::INVALID_DATA)));

  const std::string info;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResponseIs(mobile_apis::Result::GENERIC_ERROR, info, false),
          _));

  std::shared_ptr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(SystemRequestTest,
       Run_FileWithoutBinaryDataNotHTTPTypeNotSystemFile_Rejected) {
  const std::string storage_folder2 = "test_storage_folder2";
  const std::string file_name = "file_name";
  MessageSharedPtr msg = CreateIVSUMessage();
  const mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::CLIMATE;
  (*msg)[am::strings::msg_params][am::strings::request_type] = request_type;
  (*msg)[am::strings::msg_params][am::strings::file_name] = file_name;

  PreConditions();

  ON_CALL(mock_policy_handler_,
          IsRequestTypeAllowed(kAppPolicyId, request_type))
      .WillByDefault(Return(true));

  EXPECT_CALL(app_mngr_, SaveBinary(_, _, _, _)).Times(0);

  const std::string info;
  am::AppFile ap_file;
  ON_CALL(*mock_app_, folder_name()).WillByDefault(Return(storage_folder2));
  ap_file.is_download_complete = false;
  EXPECT_CALL(*mock_app_,
              GetFile(kAppStorageFolder + "/" + storage_folder2 + "/" +
                      file_name)).WillRepeatedly(Return(&ap_file));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResponseIs(mobile_apis::Result::REJECTED, info, false), _));

  std::shared_ptr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(SystemRequestTest, Run_TypeDisallowed_DisallowedResult) {
  MessageSharedPtr msg = CreateIVSUMessage();
  const mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::CLIMATE;
  (*msg)[am::strings::msg_params][am::strings::request_type] = request_type;

  PreConditions();

  ON_CALL(mock_policy_handler_,
          IsRequestTypeAllowed(kAppPolicyId, request_type))
      .WillByDefault(Return(false));

  const std::string info;
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResponseIs(mobile_apis::Result::DISALLOWED, info, false), _));
  std::shared_ptr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  ASSERT_TRUE(command->Init());
  command->Run();
}

TEST_F(SystemRequestTest, Run_FileNameContainSlash_InvalidData) {
  MessageSharedPtr msg = CreateIVSUMessage();
  const mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::CLIMATE;
  (*msg)[am::strings::msg_params][am::strings::request_type] = request_type;
  (*msg)[am::strings::msg_params][am::strings::file_name] = "file_name/";

  PreConditions();

  ON_CALL(mock_policy_handler_,
          IsRequestTypeAllowed(kAppPolicyId, request_type))
      .WillByDefault(Return(true));

  const std::string info = "Sync file name contains forbidden symbols.";
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          MobileResponseIs(mobile_apis::Result::INVALID_DATA, info, false), _));
  std::shared_ptr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  ASSERT_TRUE(command->Init());
  command->Run();
}

}  // namespace system_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

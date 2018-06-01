/*
 * Copyright (c) 2017, Ford Motor Company
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

#include "application_manager/commands/mobile/system_request.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_interface.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application.h"
#include "utils/file_system.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace system_request {

namespace am = application_manager;
using am::commands::SystemRequest;
using am::commands::CommandImpl;
using am::commands::MessageSharedPtr;
using am::MockHmiInterfaces;
using am::event_engine::Event;
using policy_test::MockPolicyHandlerInterface;
using ::utils::SharedPtr;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const uint32_t kConnectionKey = 2u;
const std::string kAppPolicyId = "fake-app-id";
const uint32_t kDefaultTimeout = 1000u;
const std::string kAppFolderName = "fake-app-name";
const std::string kAppStorageFolder = "fake-storage";
const std::string kSystemFilesPath = "fake/system/files";
const std::string kFileName = "Filename";
const mobile_apis::RequestType::eType kRequestHttpType =
    mobile_apis::RequestType::HTTP;
const mobile_apis::RequestType::eType kRequestClimate =
    mobile_apis::RequestType::CLIMATE;
const mobile_apis::RequestType::eType kRequestQueryApps =
    mobile_apis::RequestType::QUERY_APPS;
}  // namespace

class SystemRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  SystemRequestTest()
      : mock_app_(CreateMockApp())
      , binary_data_folder(kAppStorageFolder + "/" + kAppFolderName + "/")
      , app_full_file_path(binary_data_folder + kFileName)
      , binary_data_(10, 10) {}

 protected:
  MessageSharedPtr CreateIVSUMessage(
      const mobile_apis::RequestType::eType request_type = kRequestHttpType) {
    MessageSharedPtr message = CreateMessage(smart_objects::SmartType_Map);
    (*message)[am::strings::params][am::strings::connection_key] =
        kConnectionKey;
    (*message)[am::strings::msg_params][am::strings::request_type] =
        request_type;
    (*message)[am::strings::msg_params][am::strings::file_name] = kFileName;
    return message;
  }

  void PreConditions() {
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));

    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(*mock_app_, policy_app_id()).WillByDefault(Return(kAppPolicyId));
    ON_CALL(*mock_app_, folder_name()).WillByDefault(Return(kAppFolderName));

    ON_CALL(app_mngr_settings_, system_files_path())
        .WillByDefault(ReturnRef(kSystemFilesPath));
    ON_CALL(app_mngr_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
  }

  void ExpectManageMobileCommandWithResultCode(
      const mobile_apis::Result::eType code) {
    EXPECT_CALL(
        app_mngr_,
        ManageMobileCommand(MobileResultCodeIs(code),
                            am::commands::Command::CommandOrigin::ORIGIN_SDL));
  }

  void Run(MessageSharedPtr message) {
    SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(message));
    ASSERT_TRUE(command->Init());
    command->Run();
  }

  void SetUp() OVERRIDE {
    file_system::CreateDirectoryRecursively(binary_data_folder);
    file_system::CreateDirectoryRecursively(kSystemFilesPath + "/");
    file_system::CreateFile(app_full_file_path);
    file_system::CreateFile(kSystemFilesPath + "/" + kFileName);
  }

  void TearDown() OVERRIDE {
    file_system::RemoveDirectory(kAppStorageFolder);
    file_system::RemoveDirectory(kSystemFilesPath);
  }

  MockAppPtr mock_app_;
  MockPolicyHandlerInterface mock_policy_handler_;
  const std::string binary_data_folder;
  const std::string app_full_file_path;
  std::vector<uint8_t> binary_data_;
};

TEST_F(SystemRequestTest, Run_HTTP_FileName_no_binary_data_REJECTED) {
  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId, kRequestHttpType))
      .WillOnce(Return(true));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::REJECTED);

  Run(CreateIVSUMessage());
}

TEST_F(SystemRequestTest,
       Run_HttpRequestTypeAndExistsFileName_SuccessResponce) {
  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId, kRequestHttpType))
      .WillOnce(Return(true));

  am::AppFile file;
  file.is_download_complete = true;
  EXPECT_CALL(*mock_app_, GetFile(app_full_file_path)).WillOnce(Return(&file));

  EXPECT_CALL(mock_policy_handler_, ReceiveMessageFromSDK(kFileName, _));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::SUCCESS);
  Run(CreateIVSUMessage());
}

TEST_F(SystemRequestTest, Run_RequestType_SuccessRequest) {
  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId, kRequestClimate))
      .WillOnce(Return(true));

  am::AppFile file;
  file.is_download_complete = true;
  EXPECT_CALL(*mock_app_, GetFile(app_full_file_path)).WillOnce(Return(&file));

  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::BasicCommunication_SystemRequest)));

  Run(CreateIVSUMessage(kRequestClimate));
}

TEST_F(SystemRequestTest, Run_NonExistentApp_ApplicationNotRegisteredResponse) {
  mock_app_.reset();

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));

  ExpectManageMobileCommandWithResultCode(
      mobile_apis::Result::APPLICATION_NOT_REGISTERED);

  Run(CreateIVSUMessage());
}

TEST_F(SystemRequestTest,
       Run_EmptyParams_DisallowedRequestType_DisallowedResponse) {
  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId, kRequestHttpType))
      .WillOnce(Return(false));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::DISALLOWED);

  Run(CreateIVSUMessage());
}

TEST_F(SystemRequestTest, Run_InvalidFileNameSyntax_InvalidData) {
  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId, kRequestHttpType))
      .WillOnce(Return(true));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  MessageSharedPtr message = CreateIVSUMessage();
  (*message)[am::strings::msg_params][am::strings::file_name] =
      kFileName + "\n";

  Run(message);
}

TEST_F(SystemRequestTest, Run_InvalidFileName_InvalidData) {
  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId, kRequestHttpType))
      .WillOnce(Return(true));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  MessageSharedPtr message = CreateIVSUMessage();
  (*message)[am::strings::msg_params][am::strings::file_name] = kFileName + "/";

  Run(message);
}
TEST_F(SystemRequestTest, Run_InvalidSaveBinary_GenericError) {
  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId, kRequestHttpType))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_,
              SaveBinary(binary_data_, kSystemFilesPath, kFileName, 0))
      .WillOnce(Return(mobile_apis::Result::INVALID_ENUM));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::GENERIC_ERROR);

  MessageSharedPtr message = CreateIVSUMessage();
  (*message)[am::strings::params][am::strings::binary_data] = binary_data_;

  Run(message);
}

TEST_F(SystemRequestTest,
       Run_QueryAppsRequestTypeAndValidJson_SuccessResponce) {
  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId, kRequestQueryApps))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_, SaveBinary(_, kSystemFilesPath, kFileName, 0))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(app_mngr_, application_by_policy_id(_))
      .WillRepeatedly(Return(mock_app_));

  EXPECT_CALL(app_mngr_, ProcessQueryApp(_, kConnectionKey));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::SUCCESS);

  binary_data_.clear();
  file_system::ReadBinaryFile("validJSON.json", binary_data_);

  MessageSharedPtr message = CreateIVSUMessage(kRequestQueryApps);
  (*message)[am::strings::params][am::strings::binary_data] = binary_data_;

  Run(message);
}

TEST_F(SystemRequestTest, Run_QueryAppsRequestTypeAndInvalidJson_GenericError) {
  PreConditions();

  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kAppPolicyId, kRequestQueryApps))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_, SaveBinary(_, kSystemFilesPath, kFileName, 0))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));

  EXPECT_CALL(app_mngr_, application_by_policy_id(_))
      .WillRepeatedly(Return(mock_app_));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::GENERIC_ERROR);

  binary_data_.clear();
  file_system::ReadBinaryFile("invalidJSON.json", binary_data_);

  MessageSharedPtr message = CreateIVSUMessage(kRequestQueryApps);
  (*message)[am::strings::params][am::strings::binary_data] = binary_data_;

  Run(message);
}

TEST_F(SystemRequestTest, OnEvent_BasicCommunicationSystemRequest_Success) {
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::SUCCESS);

  MessageSharedPtr message = CreateIVSUMessage();
  Event event(hmi_apis::FunctionID::BasicCommunication_SystemRequest);
  event.set_smart_object(*message);
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(message));
  command->on_event(event);
}

TEST_F(SystemRequestTest, OnEvent_NonExistentApp_NullPointer) {
  MockAppPtr mock_app_nonexistent;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app_nonexistent));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);
  MessageSharedPtr message = CreateIVSUMessage();
  Event event(hmi_apis::FunctionID::BasicCommunication_SystemRequest);
  event.set_smart_object(*message);
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(message));
  command->on_event(event);
}

TEST_F(SystemRequestTest, OnEvent_InvalidEnum_ReceivedUnknownEvent) {
  EXPECT_CALL(app_mngr_, application(_)).Times(0);
  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  MessageSharedPtr message = CreateIVSUMessage();
  Event event(hmi_apis::FunctionID::INVALID_ENUM);
  event.set_smart_object(*message);
  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(message));
  command->on_event(event);
}

}  // namespace system_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

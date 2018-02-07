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
#include <vector>

#include "gtest/gtest.h"

#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"

#include "mobile/put_file_response.h"
#include "mobile/put_file_request.h"

#include "utils/make_shared.h"
#include "utils/file_system.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/application.h"
#include "application_manager/mock_application.h"
#include "application_manager/policies/mock_policy_handler_interface.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace put_file {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AtLeast;

namespace am = ::application_manager;

using sdl_rpc_plugin::commands::PutFileRequest;
using sdl_rpc_plugin::commands::PutFileResponse;
using am::commands::MessageSharedPtr;
using policy_test::MockPolicyHandlerInterface;

typedef SharedPtr<PutFileRequest> PutFileRequestPtr;
typedef SharedPtr<PutFileResponse> PutFileResponsePtr;

namespace {
const uint32_t kConnectionKey = 1u;
const std::string kFileName = "sync_file_name.txt";
const int64_t kOffset = 10u;
const int64_t kZeroOffset = 0u;
const std::string kStorageFolder = "./storage";
const std::string kFolder = "folder";
const std::string kAppFolder = "app_folder";
}

class PutFileRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  PutFileRequestTest()
      : msg_(CreateMessage(::smart_objects::SmartType_Map))
      , mock_app_(CreateMockApp()) {}

  void SetUp() OVERRIDE {
    binary_data_.push_back(1u);

    (*msg_)[am::strings::params][am::strings::connection_key] = kConnectionKey;
    (*msg_)[am::strings::msg_params][am::strings::sync_file_name] = kFileName;
    (*msg_)[am::strings::msg_params][am::strings::persistent_file] = true;
    (*msg_)[am::strings::msg_params][am::strings::file_type] =
        mobile_apis::FileType::JSON;
    (*msg_)[am::strings::params][am::strings::binary_data] = binary_data_;

    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(*mock_app_, hmi_level())
        .WillByDefault(Return(mobile_apis::HMILevel::HMI_FULL));
  }

  void ExpectReceiveMessageFromSDK() {
    EXPECT_CALL(mock_policy_handler_,
                ReceiveMessageFromSDK(kFileName, binary_data_))
        .WillOnce(Return(false));
  }
  void ExpectManageMobileCommandWithResultCode(
      const mobile_apis::Result::eType code) {
    EXPECT_CALL(
        mock_rpc_service_,
        ManageMobileCommand(MobileResultCodeIs(code),
                            am::commands::Command::CommandSource::SOURCE_SDL));
  }

  MessageSharedPtr msg_;
  MockAppPtr mock_app_;
  std::vector<uint8_t> binary_data_;
};

class PutFileResponceTest : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  PutFileResponceTest() : message_(CreateMessage()) {}

  void SetUp() OVERRIDE {
    command_sptr_ = CreateCommand<PutFileResponse>(message_);
  }

  MessageSharedPtr message_;
  SharedPtr<PutFileResponse> command_sptr_;
};

TEST_F(PutFileResponceTest, Run_InvalidApp_ApplicationNotRegisteredResponce) {
  ::smart_objects::SmartObject& message_ref = *message_;

  message_ref[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  utils::SharedPtr<am::Application> null_application_sptr;
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(null_application_sptr));
  EXPECT_CALL(
      mock_rpc_service_,
      SendMessageToMobile(
          MobileResultCodeIs(mobile_apis::Result::APPLICATION_NOT_REGISTERED),
          _));
  command_sptr_->Run();
}

TEST_F(PutFileResponceTest, Run_ApplicationRegistered_Success) {
  ::smart_objects::SmartObject& message_ref = *message_;

  message_ref[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  message_ref[am::strings::msg_params][am::strings::success] = true;

  utils::SharedPtr<am::Application> application_sptr =
      utils::MakeShared<MockApplication>();

  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(application_sptr));
  EXPECT_CALL(
      mock_rpc_service_,
      SendMessageToMobile(MobileResultCodeIs(mobile_apis::Result::SUCCESS), _));
  command_sptr_->Run();
}

TEST_F(PutFileRequestTest, Run_ApplicationIsNotRegistered_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(ApplicationSharedPtr()));
  ExpectManageMobileCommandWithResultCode(
      mobile_apis::Result::APPLICATION_NOT_REGISTERED);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_HmiLevelNone_UNSUCCESS) {
  EXPECT_CALL(*mock_app_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));

  const uint32_t settings_put_file_in_none = 1u;
  const uint32_t app_put_file_in_none_count = 2u;
  EXPECT_CALL(app_mngr_settings_, put_file_in_none())
      .WillOnce(ReturnRef(settings_put_file_in_none));
  EXPECT_CALL(*mock_app_, put_file_in_none_count())
      .WillOnce(Return(app_put_file_in_none_count));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::REJECTED);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_BinaryDataDoesNotExists_UNSUCCESS) {
  (*msg_)[am::strings::params].erase(am::strings::binary_data);
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_SyncFileNameDoesNotExists_UNSUCCESS) {
  (*msg_)[am::strings::msg_params].erase(am::strings::sync_file_name);

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_FileTypeDoesNotExists_UNSUCCESS) {
  (*msg_)[am::strings::msg_params].erase(am::strings::file_type);

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_BinaryDataGreaterThanAvaliableSpace_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::offset] = kOffset;
  (*msg_)[am::strings::msg_params][am::strings::system_file] = false;

  ExpectReceiveMessageFromSDK();
  EXPECT_CALL(app_mngr_settings_, app_storage_folder())
      .WillOnce(ReturnRef(kStorageFolder));
  EXPECT_CALL(*mock_app_, folder_name()).WillOnce(Return(kAppFolder));

  const uint32_t avaliable_space = 0u;
  EXPECT_CALL(*mock_app_, GetAvailableDiskSpace())
      .WillOnce(Return(avaliable_space));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::OUT_OF_MEMORY);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_IvalidCreationDirectory_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::offset] = kOffset;
  (*msg_)[am::strings::msg_params][am::strings::system_file] = true;

  ExpectReceiveMessageFromSDK();

  const std::string storage_folder = "/storage";
  EXPECT_CALL(app_mngr_settings_, system_files_path())
      .WillOnce(ReturnRef(storage_folder));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::GENERIC_ERROR);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_IvalidUpdateFile_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::offset] = kZeroOffset;
  (*msg_)[am::strings::msg_params][am::strings::system_file] = false;

  ExpectReceiveMessageFromSDK();
  EXPECT_CALL(app_mngr_settings_, app_storage_folder())
      .WillOnce(ReturnRef(kStorageFolder));
  EXPECT_CALL(*mock_app_, folder_name()).WillOnce(Return(kAppFolder));

  const uint32_t avaliable_space = 2u;
  EXPECT_CALL(*mock_app_, GetAvailableDiskSpace())
      .WillOnce(Return(avaliable_space))
      .WillOnce(Return(avaliable_space));

  const std::string file_path = kStorageFolder + "/" + kAppFolder;
  EXPECT_CALL(app_mngr_,
              SaveBinary(binary_data_, file_path, kFileName, kZeroOffset))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, AddFile(_)).WillOnce(Return(false));
  EXPECT_CALL(*mock_app_, UpdateFile(_)).WillOnce(Return(false));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_AddFile_SUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::offset] = kZeroOffset;
  (*msg_)[am::strings::msg_params][am::strings::system_file] = false;

  ExpectReceiveMessageFromSDK();
  EXPECT_CALL(app_mngr_settings_, app_storage_folder())
      .WillOnce(ReturnRef(kStorageFolder));
  EXPECT_CALL(*mock_app_, folder_name()).WillOnce(Return(kAppFolder));

  const uint32_t avaliable_space = 2u;
  EXPECT_CALL(*mock_app_, GetAvailableDiskSpace())
      .WillOnce(Return(avaliable_space))
      .WillOnce(Return(avaliable_space));

  const std::string file_path = kStorageFolder + "/" + kAppFolder;
  EXPECT_CALL(app_mngr_,
              SaveBinary(binary_data_, file_path, kFileName, kZeroOffset))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, AddFile(_)).WillOnce(Return(true));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::SUCCESS);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_SendOnPutFileNotification_SUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::offset] = kZeroOffset;
  (*msg_)[am::strings::msg_params][am::strings::system_file] = true;

  ExpectReceiveMessageFromSDK();
  EXPECT_CALL(app_mngr_settings_, system_files_path())
      .WillOnce(ReturnRef(kStorageFolder));
  EXPECT_CALL(app_mngr_,
              SaveBinary(binary_data_, kStorageFolder, kFileName, kZeroOffset))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::BasicCommunication_OnPutFile)))
      .WillOnce(Return(true));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::SUCCESS);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_InvalidPutFile_UNSUCCESS) {
  (*msg_)[am::strings::msg_params][am::strings::offset] = kZeroOffset;
  (*msg_)[am::strings::msg_params][am::strings::system_file] = true;

  ExpectReceiveMessageFromSDK();
  EXPECT_CALL(app_mngr_settings_, system_files_path())
      .WillOnce(ReturnRef(kStorageFolder));
  EXPECT_CALL(app_mngr_,
              SaveBinary(binary_data_, kStorageFolder, kFileName, kZeroOffset))
      .WillOnce(Return(mobile_apis::Result::INVALID_DATA));
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::INVALID_DATA);

  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  command->Run();
}

TEST_F(PutFileRequestTest, Run_CrcSumEqual_SendSuccessResponse) {
  binary_data_ = {1u};
  (*msg_)[am::strings::params][am::strings::binary_data] = binary_data_;
  const uint32_t correct_crc_sum =
      2768625435u;  // calculated using the GetCrc32CheckSum method
  (*msg_)[am::strings::msg_params][am::strings::crc32_check_sum] =
      correct_crc_sum;

  ExpectReceiveMessageFromSDK();
  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kStorageFolder));
  ON_CALL(*mock_app_, folder_name()).WillByDefault(Return(kFolder));
  const size_t available_space = binary_data_.size() + 1;
  ON_CALL(*mock_app_, GetAvailableDiskSpace())
      .WillByDefault(Return(available_space));
  ON_CALL(*mock_app_, AddFile(_)).WillByDefault(Return(true));

  const std::string file_path = kStorageFolder + "/" + kFolder;
  EXPECT_CALL(app_mngr_, SaveBinary(binary_data_, file_path, kFileName, 0u))
      .WillOnce(Return(mobile_apis::Result::SUCCESS));
  EXPECT_CALL(*mock_app_, increment_put_file_in_none_count());
  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::SUCCESS);
  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  ASSERT_TRUE(command->Init());
  command->Run();
  // The folder was created in the "Run" method
  EXPECT_TRUE(file_system::RemoveDirectory(kStorageFolder, true));
}

TEST_F(PutFileRequestTest, Run_CrcSumUnequal_SendCorruptedDataResponse) {
  binary_data_ = {1u};
  (*msg_)[am::strings::params][am::strings::binary_data] = binary_data_;
  const uint32_t incorrect_crc_sum = 0u;
  (*msg_)[am::strings::msg_params][am::strings::crc32_check_sum] =
      incorrect_crc_sum;

  ExpectReceiveMessageFromSDK();
  ON_CALL(app_mngr_, get_settings())
      .WillByDefault(ReturnRef(app_mngr_settings_));
  ON_CALL(app_mngr_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kStorageFolder));
  ON_CALL(*mock_app_, folder_name()).WillByDefault(Return(kFolder));
  const size_t available_space = binary_data_.size() + 1;
  ON_CALL(*mock_app_, GetAvailableDiskSpace())
      .WillByDefault(Return(available_space));
  ON_CALL(*mock_app_, AddFile(_)).WillByDefault(Return(true));

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::CORRUPTED_DATA);
  EXPECT_CALL(app_mngr_, SaveBinary(_, _, _, _)).Times(0);
  PutFileRequestPtr command(CreateCommand<PutFileRequest>(msg_));
  ASSERT_TRUE(command->Init());
  command->Run();
  // The folder was created in the "Run" method
  EXPECT_TRUE(file_system::RemoveDirectory(kStorageFolder, true));
}

}  // namespace put_file
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

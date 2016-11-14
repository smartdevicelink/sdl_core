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
#include "mobile/put_file_request.h"
#include "commands/command_request_test.h"
#include "utils/shared_ptr.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace put_file_request {

namespace mobile_result = mobile_apis::Result;
namespace am = application_manager;
namespace strings = am::strings;

using am::commands::MessageSharedPtr;
using am::commands::PutFileRequest;
using utils::SharedPtr;
using testing::_;
using testing::Return;

namespace {
const uint32_t kConnectionKey = 1u;
const std::string kSyncFileName = "sync_file_name";
const std::string kFolderName = "folder_name";
}  // namespace

class PutFileRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  PutFileRequestTest() {
    mock_app_ = CreateMockApp();
  }

  void BasicSetup(MessageSharedPtr msg, std::vector<uint8_t>& binary_data) {
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*msg)[strings::params][strings::binary_data] = binary_data;
    (*msg)[strings::msg_params][strings::sync_file_name] = kSyncFileName;
    (*msg)[strings::msg_params][strings::file_type] =
        mobile_apis::FileType::AUDIO_MP3;

    EXPECT_CALL(app_mngr_, application(kConnectionKey))
        .WillOnce(Return(mock_app_));
    EXPECT_CALL(*mock_app_, hmi_level())
        .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));
  }

  void CanceledSetup(mobile_result::eType mobile_result) {
    EXPECT_CALL(app_mngr_, SaveBinary(_, _, _, _)).Times(0);
    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(MobileResultCodeIs(mobile_result), _));
    EXPECT_CALL(app_mngr_, ManageHMICommand(_)).Times(0);

    EXPECT_CALL(*mock_app_, AddFile(_)).Times(0);
    EXPECT_CALL(*mock_app_, UpdateFile(_)).Times(0);
  }

  void CanceledSetupWithInvalidArgument(mobile_result::eType mobile_result) {
    EXPECT_CALL(app_mngr_settings_, app_storage_folder()).Times(0);
    EXPECT_CALL(*mock_app_, folder_name()).Times(0);
    EXPECT_CALL(*mock_app_, GetAvailableDiskSpace()).Times(0);
    CanceledSetup(mobile_result);
  }

  void DeleteDirectoryRecursevlyHelper(std::string file_name) {
    const bool recursive = true;
    file_system::RemoveDirectory(file_name, recursive);
  }

  MockAppPtr mock_app_;
};

TEST_F(PutFileRequestTest, Run_FileNameContainBackslash_InvalidData) {
  MessageSharedPtr msg = CreateMessage();
  std::vector<uint8_t> binary_data;
  BasicSetup(msg, binary_data);
  (*msg)[strings::msg_params][strings::sync_file_name] = kSyncFileName + "\\";

  SharedPtr<PutFileRequest> command(CreateCommand<PutFileRequest>(msg));

  CanceledSetupWithInvalidArgument(mobile_result::INVALID_DATA);

  command->Run();
  DeleteDirectoryRecursevlyHelper(kFolderName);
}

}  // namespace put_file_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

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
#include <map>

#include "mobile/delete_file_request.h"
#include "mobile/delete_file_response.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "utils/file_system.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/event_engine/event.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/mock_application_manager_settings.h"

#include "interfaces/MOBILE_API.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "application_manager/policies/policy_handler.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace delete_file {

using ::testing::_;
using ::testing::Test;
using ::testing::Return;
using ::testing::ReturnRef;
namespace am = ::application_manager;
using am::commands::DeleteFileRequest;
using am::commands::DeleteFileResponse;
using am::commands::MessageSharedPtr;
using am::event_engine::Event;
using am::MockMessageHelper;

typedef SharedPtr<DeleteFileRequest> DeleteFileRequestPtr;
typedef SharedPtr<DeleteFileResponse> DeleteFileResponsePtr;
typedef NiceMock<
    ::test::components::application_manager_test::MockHMICapabilities>
    MockHMICapabilities;

ACTION_TEMPLATE(SetArgPointer,
                HAS_1_TEMPLATE_PARAMS(int, k),
                AND_1_VALUE_PARAMS(vec)) {
  *std::tr1::get<k>(args) = *vec;
}

namespace {
const uint32_t kConnectionKey = 2u;
const uint32_t kCorrelationId = 10u;
const int32_t kMenuId = 5;
}  // namespace

class DeleteFileRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    message_ = CreateMessage();
    command_ = CreateCommand<DeleteFileRequest>(message_);
    mock_app_ = CreateMockApp();
  }

  DeleteFileRequestPtr command_;
  MessageSharedPtr message_;
  MockAppPtr mock_app_;
};

TEST_F(DeleteFileRequestTest, Run_HMILevelNone_UNSUCCESS) {
  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, hmi_level())
      .WillOnce(Return(am::mobile_api::HMILevel::HMI_NONE));

  EXPECT_CALL(app_mngr_, get_settings())
      .WillOnce(ReturnRef(app_mngr_settings_));
  const uint32_t num = 0;
  EXPECT_CALL(app_mngr_settings_, delete_file_in_none())
      .WillOnce(ReturnRef(num));
  EXPECT_CALL(*mock_app_, delete_file_in_none_count()).WillOnce(Return(1));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::REJECTED),
                          am::commands::Command::CommandOrigin::ORIGIN_SDL));

  command_->Run();
}

TEST_F(DeleteFileRequestTest, Run_InvalidFile_UNSUCCESS) {
  const std::string file_name = "test_file.txt";
  (*message_)[am::strings::msg_params][am::strings::sync_file_name] = file_name;

  EXPECT_CALL(app_mngr_, application(_)).WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, hmi_level())
      .WillOnce(Return(am::mobile_api::HMILevel::HMI_FULL));

  EXPECT_CALL(app_mngr_, get_settings())
      .WillOnce(ReturnRef(app_mngr_settings_));
  const std::string kFullFilePath = file_system::CurrentWorkingDirectory();
  EXPECT_CALL(app_mngr_settings_, app_storage_folder())
      .WillOnce(ReturnRef(kFullFilePath));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_apis::Result::REJECTED),
                          am::commands::Command::CommandOrigin::ORIGIN_SDL));
  command_->Run();
}

}  // namespace delete_file
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

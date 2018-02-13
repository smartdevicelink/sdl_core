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
const std::string kSystemFilesPath = "/fake/system/files";
const std::string kFileName = "Filename";
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
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));

    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
    ON_CALL(*mock_app_, policy_app_id()).WillByDefault(Return(kAppPolicyId));
    ON_CALL(*mock_app_, folder_name()).WillByDefault(Return(kAppFolderName));

    ON_CALL(app_mngr_settings_, system_files_path())
        .WillByDefault(ReturnRef(kSystemFilesPath));
    ON_CALL(app_mngr_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));

    ON_CALL(mock_policy_handler_, IsRequestTypeAllowed(kAppPolicyId, _))
        .WillByDefault(Return(true));
  }

  void ExpectManageMobileCommandWithResultCode(
      const mobile_apis::Result::eType code) {
    ON_CALL(app_mngr_, GetRPCService()).WillByDefault(ReturnRef(rpc_service_));
    EXPECT_CALL(
        rpc_service_,
        ManageMobileCommand(MobileResultCodeIs(code),
                            am::commands::Command::CommandSource::SOURCE_SDL));
  }

  MockAppPtr mock_app_;
  MockPolicyHandlerInterface mock_policy_handler_;
};

TEST_F(SystemRequestTest, Run_HTTP_FileName_no_binary_data_REJECTED) {
  PreConditions();
  MessageSharedPtr msg = CreateIVSUMessage();

  ExpectManageMobileCommandWithResultCode(mobile_apis::Result::REJECTED);

  SharedPtr<SystemRequest> command(CreateCommand<SystemRequest>(msg));
  command->Run();
}

}  // namespace system_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

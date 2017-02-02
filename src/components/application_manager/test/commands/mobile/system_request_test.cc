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
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/mock_message_helper.h"
#include "mobile/system_request.h"
#include "commands/command_request_test.h"
#include "utils/shared_ptr.h"
#include "utils/file_system.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace system_request {

namespace mobile_result = mobile_apis::Result;
namespace am = application_manager;
namespace strings = am::strings;

using am::commands::SystemRequest;
using am::MockMessageHelper;
using policy_test::MockPolicyHandlerInterface;
using ::testing::DefaultValue;

typedef SharedPtr<SystemRequest> CommandPtr;

namespace {
const uint32_t kConnectionKey = 5u;
const uint32_t kAppId = 10u;
const std::string kPolicyAppId = "12345";
const std::string kFileName = "test_file_name";
}

class SystemRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  SystemRequestTest() : mock_app_(CreateMockApp()) {
    msg_ = CreateMessage(smart_objects::SmartType_Map);
    (*msg_)[strings::params][strings::connection_key] = kConnectionKey;

    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(*mock_app_, policy_app_id()).WillByDefault(Return(kPolicyAppId));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
  }

  MockAppPtr mock_app_;
  MessageSharedPtr msg_;
  MockPolicyHandlerInterface mock_policy_handler_;
};

TEST_F(SystemRequestTest, Run_FileNameContainBackslash_InvalidData) {
  mobile_apis::RequestType::eType request_type =
      mobile_apis::RequestType::CLIMATE;
  (*msg_)[strings::msg_params][strings::request_type] = request_type;
  (*msg_)[strings::msg_params][strings::file_name] = kFileName + "\\";
  CommandPtr command(CreateCommand<SystemRequest>(msg_));

  ON_CALL(app_mngr_, application(kConnectionKey))
      .WillByDefault(Return(mock_app_));
  EXPECT_CALL(mock_policy_handler_,
              IsRequestTypeAllowed(kPolicyAppId, request_type))
      .WillOnce(Return(true));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::INVALID_DATA), _))
      .WillOnce(Return(false));

  command->Run();
}

}  // namespace system_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

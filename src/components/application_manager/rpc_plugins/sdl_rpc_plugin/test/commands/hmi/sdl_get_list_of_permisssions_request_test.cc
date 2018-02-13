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

#include "gtest/gtest.h"
#include "hmi/sdl_get_list_of_permissions_request.h"
#include "application_manager/mock_application.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/commands/command_request_test.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace sdl_get_list_of_permissions_request {

using application_manager::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::SDLGetListOfPermissionsRequest;
using test::components::policy_test::MockPolicyHandlerInterface;
using smart_objects::SmartObject;
using testing::Return;
using testing::ReturnRef;

namespace {
const uint32_t kCorrelationID = 1u;
const uint32_t kAppID = 2u;
const uint32_t kConnectionKey = 0u;
}  // namespace

namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

class SDLGetListOfPermissionsRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    mock_app_ = CreateMockApp();
  }

  void InitCommand(const uint32_t& timeout) OVERRIDE {
    CommandRequestTest<CommandsTestMocks::kIsNice>::InitCommand(timeout);
    ON_CALL((*mock_app_), app_id()).WillByDefault(Return(kAppID));
  }
  MockAppPtr mock_app_;
};

TEST_F(SDLGetListOfPermissionsRequestTest, Run_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  (*msg)[strings::params][strings::correlation_id] = kCorrelationID;
  (*msg)[strings::msg_params][strings::app_id] = kAppID;

  EXPECT_CALL(app_mngr_, application_by_hmi_app(kAppID))
      .WillOnce(Return(mock_app_));

  SharedPtr<SDLGetListOfPermissionsRequest> command(
      CreateCommand<SDLGetListOfPermissionsRequest>(msg));

  MockPolicyHandlerInterface policy_handler;
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(policy_handler));
  EXPECT_CALL(policy_handler, OnGetListOfPermissions(kAppID, kCorrelationID));

  command->Run();
}

TEST_F(SDLGetListOfPermissionsRequestTest, Run_KeyDoesntExist_SUCCESS) {
  MessageSharedPtr msg = CreateMessage(smart_objects::SmartType_Binary);
  (*msg)[strings::params][strings::correlation_id] = kCorrelationID;

  SharedPtr<SDLGetListOfPermissionsRequest> command(
      CreateCommand<SDLGetListOfPermissionsRequest>(msg));

  MockPolicyHandlerInterface policy_handler;
  EXPECT_CALL(app_mngr_, GetPolicyHandler())
      .WillOnce(ReturnRef(policy_handler));
  EXPECT_CALL(policy_handler,
              OnGetListOfPermissions(kConnectionKey, kCorrelationID));

  command->Run();
}

}  // namespace sdl_get_list_of_permissions_request
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

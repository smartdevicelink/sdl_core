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

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/commands/mobile/unregister_app_interface_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace unregister_app_interface_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::_;

using am::commands::UnregisterAppInterfaceRequest;
using am::commands::MessageSharedPtr;

typedef ::utils::SharedPtr<UnregisterAppInterfaceRequest> CommandPtr;

class UnregisterAppInterfaceRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {};

TEST_F(UnregisterAppInterfaceRequestTest, Run_AppNotRegistered_UNSUCCESS) {
  CommandPtr command(CreateCommand<UnregisterAppInterfaceRequest>());

  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(UnregisterAppInterfaceRequestTest, Run_SUCCESS) {
  const uint32_t kConnectionKey = 1u;

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  CommandPtr command(CreateCommand<UnregisterAppInterfaceRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  const mobile_apis::AppInterfaceUnregisteredReason::eType kUnregisterReason =
      mobile_apis::AppInterfaceUnregisteredReason::INVALID_ENUM;

  MessageSharedPtr dummy_msg(CreateMessage());
  EXPECT_CALL(*am::MockMessageHelper::message_helper_mock(),
              GetOnAppInterfaceUnregisteredNotificationToMobile(
                  kConnectionKey, kUnregisterReason))
      .WillOnce(Return(dummy_msg));
  {
    ::testing::InSequence sequence;

    EXPECT_CALL(app_mngr_, ManageMobileCommand(dummy_msg, _));

    EXPECT_CALL(app_mngr_,
                UnregisterApplication(
                    kConnectionKey, mobile_apis::Result::SUCCESS, _, _));

    EXPECT_CALL(app_mngr_,
                ManageMobileCommand(
                    MobileResultCodeIs(mobile_apis::Result::SUCCESS), _));
  }

  command->Run();
}

}  // namespace unregister_app_interface_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

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

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "application_manager/commands/command_request_test.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/commands/mobile/unsubscribe_button_request.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace unsubscribe_button_request {

namespace am = ::application_manager;
namespace mobile_result = mobile_apis::Result;

using ::testing::_;

using am::commands::UnsubscribeButtonRequest;
using am::commands::MessageSharedPtr;
using application_manager_test::MockHMICapabilities;

typedef ::utils::SharedPtr<UnsubscribeButtonRequest> CommandPtr;

namespace {
const uint32_t kConnectionKey = 1u;
const mobile_apis::ButtonName::eType kButtonId = mobile_apis::ButtonName::OK;
}  // namespace

class UnsubscribeButtonRequestTest
    : public CommandRequestTest<CommandsTestMocks::kIsNice> {
 public:
  MOCK(MockHMICapabilities) mock_hmi_capabilities_;
  UnsubscribeButtonRequestTest() {
    ON_CALL(app_mngr_, hmi_capabilities())
        .WillByDefault(ReturnRef(mock_hmi_capabilities_));
  }
};

TEST_F(UnsubscribeButtonRequestTest, Run_AppNotRegistered_UNSUCCESS) {
  CommandPtr command(CreateCommand<UnsubscribeButtonRequest>());

  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(
          MobileResultCodeIs(mobile_result::APPLICATION_NOT_REGISTERED), _));

  command->Run();
}

TEST_F(UnsubscribeButtonRequestTest,
       Run_UnsubscribeNotSubscribedButton_UNSUPPORTED_RESOURCE) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::strings::button_name] = kButtonId;

  CommandPtr command(CreateCommand<UnsubscribeButtonRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(app_mngr_,
              ManageMobileCommand(
                  MobileResultCodeIs(mobile_result::UNSUPPORTED_RESOURCE), _));

  command->Run();
}

TEST_F(UnsubscribeButtonRequestTest, Run_SUCCESS) {
  const mobile_apis::ButtonName::eType kButtonId = mobile_apis::ButtonName::OK;

  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  (*command_msg)[am::strings::msg_params][am::strings::button_name] = kButtonId;

  CommandPtr command(CreateCommand<UnsubscribeButtonRequest>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(mock_hmi_capabilities_, is_ui_cooperating())
      .WillOnce(Return(true));
  MessageSharedPtr button_cap(CreateMessage(smart_objects::SmartType_Array));
  (*button_cap)[0][am::hmi_response::button_name] = kButtonId;
  EXPECT_CALL(mock_hmi_capabilities_, button_capabilities())
      .WillOnce(Return(button_cap.get()));

  EXPECT_CALL(*mock_app, UnsubscribeFromButton(kButtonId))
      .WillOnce(Return(true));

  EXPECT_CALL(app_mngr_,
              ManageHMICommand(HMIResultCodeIs(
                  hmi_apis::FunctionID::Buttons_OnButtonSubscription)));
  EXPECT_CALL(
      app_mngr_,
      ManageMobileCommand(MobileResultCodeIs(mobile_result::SUCCESS), _));

  EXPECT_CALL(*mock_app, UpdateHash());

  command->Run();
}

}  // namespace unsubscribe_button_request
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

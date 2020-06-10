/*
 * Copyright (c) 2019, Ford Motor Company
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

#include "hmi/on_bc_system_capability_updated_notification_from_hmi.h"

#include "application_manager/commands/commands_test.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace on_bc_system_capability_updated_notification_from_hmi {

using sdl_rpc_plugin::commands::OnBCSystemCapabilityUpdatedNotificationFromHMI;
using ::testing::_;
using ::testing::Return;

typedef std::shared_ptr<OnBCSystemCapabilityUpdatedNotificationFromHMI>
    OnBCSystemCapabilityUpdatedNotificationFromHMIPtr;

namespace strings = application_manager::strings;
namespace {
const uint32_t kAppId = 1u;
}  // namespace

MATCHER(CheckMessageToMobile, "") {
  const auto function_id = mobile_apis::FunctionID::OnSystemCapabilityUpdatedID;

  const bool is_function_id_matched =
      function_id == static_cast<am::mobile_api::FunctionID::eType>(
                         (*arg)[strings::params][strings::function_id].asInt());
  const bool app_id_exist =
      (*arg)[strings::msg_params].keyExists(strings::app_id);
  bool is_connection_key_correct = true;
  if ((*arg)[strings::msg_params].keyExists(strings::connection_key)) {
    is_connection_key_correct =
        (*arg)[strings::params][strings::connection_key] == kAppId;
  }
  return is_function_id_matched && !app_id_exist && is_connection_key_correct;
}

MATCHER_P(CheckDisplayCapabilitiesNotChanged, display_capability, "") {
  return display_capability == arg;
}

class OnBCSystemCapabilityUpdatedNotificationFromHMITest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 protected:
  OnBCSystemCapabilityUpdatedNotificationFromHMITest()
      : message_(CreateMessage())
      , display_capability_(
            (*message_)[am::strings::msg_params][strings::system_capability]
                       [strings::display_capabilities]) {
    command_ =
        CreateCommand<OnBCSystemCapabilityUpdatedNotificationFromHMI>(message_);
    mock_app_ = CreateMockApp();
  }

  OnBCSystemCapabilityUpdatedNotificationFromHMIPtr command_;
  MockAppPtr mock_app_;
  MessageSharedPtr message_;
  const SmartObject display_capability_;
};

TEST_F(
    OnBCSystemCapabilityUpdatedNotificationFromHMITest,
    Run_AppIdNotPresentedInMessage_SetSystemDisplayCapabilitiesToHMICapabilities_SendMessageToMobile) {
  (*message_)[am::strings::msg_params][strings::system_capability]
             [am::strings::system_capability_type] =
                 mobile_apis::SystemCapabilityType::DISPLAYS;

  EXPECT_CALL(mock_hmi_capabilities_,
              set_system_display_capabilities(
                  CheckDisplayCapabilitiesNotChanged(display_capability_)));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          CheckMessageToMobile(),
          ::application_manager::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(OnBCSystemCapabilityUpdatedNotificationFromHMITest,
       Run_AppNotRegisteredWithPresentedAppIdInMessage_MessageNotSendToMobile) {
  (*message_)[am::strings::msg_params][strings::system_capability]
             [am::strings::system_capability_type] =
                 mobile_apis::SystemCapabilityType::DISPLAYS;
  (*message_)[strings::msg_params][strings::app_id] = kAppId;
  ApplicationSharedPtr app;  // Empty application shared pointer

  ON_CALL(app_mngr_, application(kAppId)).WillByDefault(Return(app));
  EXPECT_CALL(mock_rpc_service_, ManageMobileCommand(_, _)).Times(0);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(
    OnBCSystemCapabilityUpdatedNotificationFromHMITest,
    Run_AppRegisteredWithPresentedAppIdInMessage_SetDisplayCapabilitiesToApp_SendMessageToMobile) {
  (*message_)[am::strings::msg_params][strings::system_capability]
             [am::strings::system_capability_type] =
                 mobile_apis::SystemCapabilityType::DISPLAYS;
  (*message_)[strings::msg_params][strings::app_id] = kAppId;

  ON_CALL(app_mngr_, application(kAppId)).WillByDefault(Return(mock_app_));

  EXPECT_CALL(*mock_app_,
              set_display_capabilities(
                  CheckDisplayCapabilitiesNotChanged(display_capability_)));
  EXPECT_CALL(
      mock_rpc_service_,
      ManageMobileCommand(
          CheckMessageToMobile(),
          ::application_manager::commands::Command::CommandSource::SOURCE_SDL))
      .WillOnce(Return(true));

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

}  // namespace on_bc_system_capability_updated_notification_from_hmi
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

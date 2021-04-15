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

#include "hmi/on_bc_system_capability_updated_notification.h"

#include "application_manager/commands/commands_test.h"
#include "gtest/gtest.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace on_bc_system_capability_updated_notification {

using sdl_rpc_plugin::commands::OnBCSystemCapabilityUpdatedNotification;
using ::testing::_;
using ::testing::Return;

typedef std::shared_ptr<OnBCSystemCapabilityUpdatedNotification>
    OnBCSystemCapabilityUpdatedNotificationPtr;

namespace strings = application_manager::strings;
namespace {
const uint32_t kConnectionKey = 1u;
}

MATCHER_P(CheckDisplayCapabilities, display_capabilities, "") {
  return display_capabilities ==
         (*arg)[strings::msg_params][strings::system_capability]
               [strings::display_capabilities];
}

class OnBCSystemCapabilityUpdatedNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    message_ = CreateMessage();
    (*message_)[strings::params][strings::connection_key] = kConnectionKey;

    command_ = CreateCommand<OnBCSystemCapabilityUpdatedNotification>(message_);
    mock_app_ = CreateMockApp();
  }

  OnBCSystemCapabilityUpdatedNotificationPtr command_;
  MockAppPtr mock_app_;
  MessageSharedPtr message_;
};

TEST_F(OnBCSystemCapabilityUpdatedNotificationTest,
       SystemDisplayCapabilities_SUCCESS) {
  (*message_)[am::strings::msg_params][strings::system_capability]
             [am::strings::system_capability_type] =
                 mobile_apis::SystemCapabilityType::DISPLAYS;

  smart_objects::SmartObjectSPtr system_display_capabilities =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Null);

  EXPECT_CALL(mock_hmi_capabilities_, system_display_capabilities())
      .Times(2)
      .WillRepeatedly(Return(system_display_capabilities));

  EXPECT_CALL(
      mock_rpc_service_,
      SendMessageToHMI(CheckDisplayCapabilities(*system_display_capabilities)));

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(OnBCSystemCapabilityUpdatedNotificationTest,
       SystemDisplayCapabilities_DATA_NOT_AVAILABLE) {
  (*message_)[am::strings::msg_params][strings::system_capability]
             [am::strings::system_capability_type] =
                 mobile_apis::SystemCapabilityType::DISPLAYS;
  EXPECT_CALL(mock_hmi_capabilities_, system_display_capabilities())
      .WillOnce(Return(nullptr));

  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_)).Times(0);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

}  // namespace on_bc_system_capability_updated_notification
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

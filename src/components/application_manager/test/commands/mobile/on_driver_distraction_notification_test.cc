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
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/mock_message_helper.h"
#include "mobile/on_driver_distraction_notification.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_driver_distraction_notification {

namespace am = ::application_manager;

using ::testing::_;
using ::testing::Return;
using ::testing::Mock;
using am::commands::CommandImpl;
using am::MockMessageHelper;
using am::commands::MessageSharedPtr;
using am::commands::mobile::OnDriverDistractionNotification;

typedef ::utils::SharedPtr<OnDriverDistractionNotification> CommandPtr;

class MobileOnDriverDistractionNotificationTest
    : public CommandsTest<CommandsTestMocks::kNotNice> {
 public:
  MobileOnDriverDistractionNotificationTest()
      : mock_message_helper_(*MockMessageHelper::message_helper_mock()) {
    const uint32_t timeout = kDefaultTimeout_;
    ON_CALL(mock_app_manager_settings_, default_timeout())
        .WillByDefault(ReturnRef(timeout));
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void SetSendNotificationExpectations(MessageSharedPtr& msg) {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    EXPECT_CALL(mock_message_helper_, PrintSmartObject(_))
        .WillOnce(Return(false));
    EXPECT_CALL(mock_app_manager_, SendMessageToMobile(msg, _));
  }

  void VerifySendNotificationData(MessageSharedPtr& msg) {
    ASSERT_EQ(am::MessageType::kNotification,
              (*msg)[am::strings::params][am::strings::message_type].asInt());
    ASSERT_EQ(CommandImpl::mobile_protocol_type_,
              (*msg)[am::strings::params][am::strings::protocol_type].asInt());
    ASSERT_EQ(
        CommandImpl::protocol_version_,
        (*msg)[am::strings::params][am::strings::protocol_version].asInt());
  }

  MockMessageHelper& mock_message_helper_;
};

TEST_F(MobileOnDriverDistractionNotificationTest, Run_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));

  CommandPtr command(
      CreateCommand<OnDriverDistractionNotification>(command_msg));

  SetSendNotificationExpectations(command_msg);

  command->Run();

  VerifySendNotificationData(command_msg);
}

}  // namespace on_driver_distraction_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

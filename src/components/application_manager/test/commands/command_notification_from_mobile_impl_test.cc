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

#include "application_manager/commands/command_notification_from_mobile_impl.h"

#include "gtest/gtest.h"
#include "utils/shared_ptr.h"
#include "smart_objects/smart_object.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/commands/commands_test.h"
#include "application_manager/commands/command.h"
#include "application_manager/mock_message_helper.h"

namespace test {
namespace components {
namespace commands_test {
namespace command_response_impl {
namespace command_notification_from_mobile_impl {

namespace strings = ::application_manager::strings;
namespace hmi_response = ::application_manager::hmi_response;

using ::testing::_;
using ::testing::Mock;
using ::testing::DoAll;
using ::testing::Return;
using ::testing::SaveArg;
using ::utils::SharedPtr;
using ::application_manager::commands::MessageSharedPtr;
using ::application_manager::commands::CommandNotificationFromMobileImpl;

typedef SharedPtr<CommandNotificationFromMobileImpl>
    CommandNotificationFromMobileImplPtr;

class CommandNotificationFromMobileImplTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  CommandNotificationFromMobileImplTest()
      : mock_message_helper_(am::MockMessageHelper::message_helper_mock()) {}

  void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(mock_message_helper_);
  }

 protected:
  am::MockMessageHelper* mock_message_helper_;
};

TEST_F(CommandNotificationFromMobileImplTest, BasicMethodsOverloads_SUCCESS) {
  CommandNotificationFromMobileImplPtr command =
      CreateCommand<CommandNotificationFromMobileImpl>();
  // Current implementation always return `true`
  EXPECT_TRUE(command->Init());
  EXPECT_TRUE(command->CleanUp());
  EXPECT_NO_THROW(command->Run());
}

TEST_F(CommandNotificationFromMobileImplTest, SendNotification_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();
  CommandNotificationFromMobileImplPtr command =
      CreateCommand<CommandNotificationFromMobileImpl>(msg);

  MessageSharedPtr result;
  EXPECT_CALL(mock_app_manager_, SendMessageToMobile(_, false))
      .WillOnce(DoAll(SaveArg<0>(&result), Return()));

  command->SendNotification();

  EXPECT_EQ(
      (*result)[strings::params][strings::message_type].asInt(),
      static_cast<int32_t>(application_manager::MessageType::kNotification));
}

}  // namespace command_notification_from_mobile_impl
}  // namespace command_response_impl
}  // namespace commands_test
}  // namespace components
}  // namespace test

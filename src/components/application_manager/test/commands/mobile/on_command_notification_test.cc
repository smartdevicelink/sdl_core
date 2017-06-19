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
#include "application_manager/commands/mobile/on_command_notification.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_command_notification {

namespace am = ::application_manager;

using ::testing::_;
using ::testing::Return;

using am::commands::MessageSharedPtr;
using am::commands::OnCommandNotification;

typedef ::utils::SharedPtr<OnCommandNotification> CommandPtr;

namespace {
const uint32_t kAppId = 1u;
const uint32_t kCommandId = 5u;
}  // namespace

class OnCommandNotificationTest
    : public CommandsTest<CommandsTestMocks::kNotNice> {};

TEST_F(OnCommandNotificationTest, Run_AppNotRegistered_UNSUCCESS) {
  CommandPtr command(CreateCommand<OnCommandNotification>());

  EXPECT_CALL(app_mngr_, application(_))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  command->Run();
}

TEST_F(OnCommandNotificationTest, Run_NoAppsForTheCommand_UNSUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  (*command_msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;

  CommandPtr command(CreateCommand<OnCommandNotification>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kAppId)).WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, FindCommand(kCommandId))
      .WillOnce(Return(static_cast<SmartObject*>(NULL)));

  EXPECT_CALL(app_mngr_, ManageMobileCommand(_, _)).Times(0);

  command->Run();
}

MATCHER(CheckNotificationMessage, "") {
  const bool kIsMobileProtocolTypeCorrect =
      (*arg)[am::strings::params][am::strings::protocol_type].asInt() ==
      am::commands::CommandImpl::mobile_protocol_type_;

  const bool kIsProtocolVersionCorrect =
      (*arg)[am::strings::params][am::strings::protocol_version].asInt() ==
      am::commands::CommandImpl::protocol_version_;

  const bool kIsNotificationCorrect =
      (*arg)[am::strings::params][am::strings::message_type].asInt() ==
      am::MessageType::kNotification;

  return kIsMobileProtocolTypeCorrect && kIsProtocolVersionCorrect &&
         kIsNotificationCorrect;
}

TEST_F(OnCommandNotificationTest, Run_SUCCESS) {
  MessageSharedPtr command_msg(CreateMessage(smart_objects::SmartType_Map));
  (*command_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  (*command_msg)[am::strings::msg_params][am::strings::cmd_id] = kCommandId;

  CommandPtr command(CreateCommand<OnCommandNotification>(command_msg));

  MockAppPtr mock_app(CreateMockApp());
  EXPECT_CALL(app_mngr_, application(kAppId)).WillOnce(Return(mock_app));

  MessageSharedPtr dummy_msg(CreateMessage());
  EXPECT_CALL(*mock_app, FindCommand(kCommandId))
      .WillOnce(Return(dummy_msg.get()));

  EXPECT_CALL(app_mngr_, SendMessageToMobile(CheckNotificationMessage(), _));

  command->Run();
}

}  // namespace on_command_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

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

#include "gtest/gtest.h"
#include "application_manager/commands/mobile/on_keyboard_input_notification.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include <application_manager/smart_object_keys.h>
#include "application_manager/message.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/commands/command_impl.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_keyboard_input_notification {

namespace strings = application_manager::strings;

namespace {
const uint32_t kConnectionKey = 1u;
}  // namespace

using application_manager::commands::mobile::OnKeyBoardInputNotification;
using application_manager::MockMessageHelper;
using application_manager::commands::CommandImpl;
using application_manager::ApplicationSet;
using testing::Mock;
using testing::Return;
using testing::_;

class OnKeyBoardInputNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  void SetSendNotificationExpectations(MessageSharedPtr msg) {
    EXPECT_CALL(mock_message_helper_, PrintSmartObject(_))
        .WillOnce(Return(false));
    EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, _));
  }

  void SetSendNotificationVariables(MessageSharedPtr msg) {
    ASSERT_EQ(application_manager::MessageType::kNotification,
              (*msg)[strings::params][strings::message_type].asInt());
    ASSERT_EQ(CommandImpl::mobile_protocol_type_,
              (*msg)[strings::params][strings::protocol_type].asInt());
    ASSERT_EQ(CommandImpl::protocol_version_,
              (*msg)[strings::params][strings::protocol_version].asInt());
  }

  MockAppPtr InitAppSetDataAccessor(std::shared_ptr<ApplicationSet>& app_set) {
    app_set = (!app_set ? ::std::make_shared<ApplicationSet>() : app_set);
    MockAppPtr app(CreateMockApp());
    app_set->insert(app);
    EXPECT_CALL(app_mngr_, applications())
        .WillOnce(Return(DataAccessor<ApplicationSet>(*app_set, lock_)));
    return app;
  }

  std::shared_ptr<ApplicationSet> app_set_;
  sync_primitives::Lock lock_;
};

TEST_F(OnKeyBoardInputNotificationTest, Run_ActionActive_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<OnKeyBoardInputNotification> command =
      CreateCommand<OnKeyBoardInputNotification>(msg);

  MockAppPtr mock_app(InitAppSetDataAccessor(app_set_));
  EXPECT_CALL(*mock_app, is_perform_interaction_active()).WillOnce(Return(1));
  EXPECT_CALL(*mock_app, perform_interaction_layout())
      .WillOnce(Return(mobile_apis::LayoutMode::KEYBOARD));
  EXPECT_CALL(*mock_app, hmi_level()).Times(0);

  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kConnectionKey));

  SetSendNotificationExpectations(msg);

  command->Run();

  SetSendNotificationVariables(msg);

  ASSERT_EQ(kConnectionKey,
            (*msg)[strings::params][strings::connection_key].asInt());
}

TEST_F(OnKeyBoardInputNotificationTest, Run_ActionNotActive_SUCCESS) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<OnKeyBoardInputNotification> command =
      CreateCommand<OnKeyBoardInputNotification>(msg);

  MockAppPtr mock_app(InitAppSetDataAccessor(app_set_));
  EXPECT_CALL(*mock_app, is_perform_interaction_active())
      .WillRepeatedly(Return(0));

  EXPECT_CALL(*mock_app, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::eType::HMI_FULL));

  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kConnectionKey));

  SetSendNotificationExpectations(msg);

  command->Run();

  SetSendNotificationVariables(msg);

  ASSERT_EQ(kConnectionKey,
            (*msg)[strings::params][strings::connection_key].asInt());
}

TEST_F(OnKeyBoardInputNotificationTest, Run_InvalidApp_NoNotification) {
  MessageSharedPtr msg = CreateMessage();

  std::shared_ptr<OnKeyBoardInputNotification> command =
      CreateCommand<OnKeyBoardInputNotification>(msg);

  MockAppPtr mock_app(InitAppSetDataAccessor(app_set_));
  EXPECT_CALL(*mock_app, is_perform_interaction_active())
      .WillRepeatedly(Return(0));

  EXPECT_CALL(*mock_app, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::eType::HMI_BACKGROUND));

  EXPECT_CALL(mock_message_helper_, PrintSmartObject(_)).Times(0);
  EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, _)).Times(0);

  command->Run();
}

}  // namespace on_keyboard_input_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

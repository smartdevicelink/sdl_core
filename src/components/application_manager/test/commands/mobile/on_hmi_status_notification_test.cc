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
#include "application_manager/commands/mobile/on_hmi_status_notification.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/mock_message_helper.h"
#include "application_manager/commands/command_impl.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_hmi_status_notification {

namespace {
const uint32_t kConnectionKey = 1u;
}  // namespace

namespace strings = application_manager::strings;

using application_manager::commands::OnHMIStatusNotification;
using application_manager::ProtocolVersion;
using application_manager::MockMessageHelper;
using application_manager::commands::CommandImpl;
using testing::Mock;
using testing::Return;
using testing::_;

class OnHMIStatusNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnHMIStatusNotificationTest()
      : message_helper_(*MockMessageHelper::message_helper_mock()) {}

  MessageSharedPtr CreateMsgParams(
      const mobile_apis::HMILevel::eType kHMIState) {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*msg)[strings::msg_params][strings::hmi_level] = kHMIState;
    return msg;
  }

  void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&message_helper_);
  }

  void SetSendNotificationExpectations(MessageSharedPtr& msg) {
    Mock::VerifyAndClearExpectations(&message_helper_);
    EXPECT_CALL(message_helper_, PrintSmartObject(_)).WillOnce(Return(false));
    EXPECT_CALL(app_mngr_, SendMessageToMobile(msg, _));
  }

  void VerifySendNotificationData(MessageSharedPtr& msg) {
    ASSERT_EQ(application_manager::MessageType::kNotification,
              (*msg)[strings::params][strings::message_type].asInt());
    ASSERT_EQ(CommandImpl::mobile_protocol_type_,
              (*msg)[strings::params][strings::protocol_type].asInt());
    ASSERT_EQ(CommandImpl::protocol_version_,
              (*msg)[strings::params][strings::protocol_version].asInt());
  }

  MockMessageHelper& message_helper_;
};

TEST_F(OnHMIStatusNotificationTest, Run_InvalidApp_NoNotification) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_FULL);

  SharedPtr<OnHMIStatusNotification> command =
      CreateCommand<OnHMIStatusNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));

  EXPECT_CALL(*mock_app, tts_properties_in_full()).Times(0);
  EXPECT_CALL(*mock_app, set_tts_properties_in_full(_)).Times(0);
  EXPECT_CALL(*mock_app, app_id()).Times(0);
  EXPECT_CALL(app_mngr_, AddAppToTTSGlobalPropertiesList(kConnectionKey))
      .Times(0);

  command->Run();
}

TEST_F(OnHMIStatusNotificationTest, Run_InvalidEnum_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::INVALID_ENUM);

  SharedPtr<OnHMIStatusNotification> command =
      CreateCommand<OnHMIStatusNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  SetSendNotificationExpectations(msg);

  command->Run();

  VerifySendNotificationData(msg);
}

TEST_F(OnHMIStatusNotificationTest, Run_BackgroundAndFalseProperties_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_BACKGROUND);
  SetSendNotificationExpectations(msg);

  SharedPtr<OnHMIStatusNotification> command =
      CreateCommand<OnHMIStatusNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, tts_properties_in_none()).WillOnce(Return(false));
  EXPECT_CALL(*mock_app, set_tts_properties_in_none(true));
  EXPECT_CALL(message_helper_, SendTTSGlobalProperties(_, false, _));

  command->Run();

  VerifySendNotificationData(msg);
}

TEST_F(OnHMIStatusNotificationTest, Run_BackgroundAndTrueProperties_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_BACKGROUND);

  SharedPtr<OnHMIStatusNotification> command =
      CreateCommand<OnHMIStatusNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, tts_properties_in_none()).WillOnce(Return(true));

  SetSendNotificationExpectations(msg);

  command->Run();

  VerifySendNotificationData(msg);
}

TEST_F(OnHMIStatusNotificationTest, Run_FullAndFalseProperties_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_FULL);

  SharedPtr<OnHMIStatusNotification> command =
      CreateCommand<OnHMIStatusNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, tts_properties_in_full()).WillOnce(Return(false));
  EXPECT_CALL(*mock_app, set_tts_properties_in_full(true));
  EXPECT_CALL(*mock_app, app_id()).WillOnce(Return(kConnectionKey));
  EXPECT_CALL(app_mngr_, AddAppToTTSGlobalPropertiesList(kConnectionKey));

  SetSendNotificationExpectations(msg);

  command->Run();

  VerifySendNotificationData(msg);
}

TEST_F(OnHMIStatusNotificationTest, Run_FullAndTrueProperties_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_FULL);

  SharedPtr<OnHMIStatusNotification> command =
      CreateCommand<OnHMIStatusNotification>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, tts_properties_in_full()).WillOnce(Return(true));

  SetSendNotificationExpectations(msg);

  command->Run();

  VerifySendNotificationData(msg);
}

}  // namespace on_hmi_status_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

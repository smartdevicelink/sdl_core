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
#include "application_manager/commands/mobile/on_hmi_status_notification_from_mobile.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include <application_manager/smart_object_keys.h>
#include "application_manager/message.h"
#include "application_manager/application_manager.h"
#include "application_manager/mock_message_helper.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_hmi_status_notification_from_mobile {

namespace {
const uint32_t kConnectionKey = 1u;
const connection_handler::DeviceHandle kHandle = 2u;
}  // namespace

namespace strings = application_manager::strings;

using application_manager::commands::OnHMIStatusNotificationFromMobile;
using application_manager::ProtocolVersion;
using application_manager::ApplicationSet;
using testing::Mock;
using testing::Return;
using testing::_;

class OnHMIStatusNotificationFromMobileTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  MessageSharedPtr CreateMsgParams(
      const mobile_apis::HMILevel::eType kHMIState) {
    MessageSharedPtr msg = CreateMessage();
    (*msg)[strings::params][strings::connection_key] = kConnectionKey;
    (*msg)[strings::msg_params][strings::hmi_level] = kHMIState;
    return msg;
  }
  ApplicationSet app_set_;
  sync_primitives::Lock lock_;
};

TEST_F(OnHMIStatusNotificationFromMobileTest,
       Run_CurrentStateForeground_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_FULL);

  SharedPtr<OnHMIStatusNotificationFromMobile> command =
      CreateCommand<OnHMIStatusNotificationFromMobile>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, set_foreground(true));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(app_mngr_, IsAppsQueriedFrom(kHandle)).WillOnce(Return(true));

  DataAccessor<ApplicationSet> accessor(app_set_, lock_);
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app, protocol_version())
      .WillRepeatedly(Return(ProtocolVersion::kV4));
  EXPECT_CALL(*mock_app, is_foreground()).WillRepeatedly(Return(true));

  command->Run();

  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
}

TEST_F(OnHMIStatusNotificationFromMobileTest, Run_InvalidApp_NoNotification) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_FULL);

  SharedPtr<OnHMIStatusNotificationFromMobile> command =
      CreateCommand<OnHMIStatusNotificationFromMobile>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(MockAppPtr()));
  EXPECT_CALL(*mock_app, set_foreground(true)).Times(0);

  EXPECT_CALL(*mock_app, device()).Times(0);
  EXPECT_CALL(app_mngr_, IsAppsQueriedFrom(kHandle)).Times(0);

  EXPECT_CALL(app_mngr_, applications()).Times(0);

  EXPECT_CALL(*mock_app, protocol_version()).Times(0);
  EXPECT_CALL(*mock_app, is_foreground()).Times(0);

  command->Run();

  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
}

TEST_F(OnHMIStatusNotificationFromMobileTest,
       Run_CurrentStateNotForeground_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_BACKGROUND);

  SharedPtr<OnHMIStatusNotificationFromMobile> command =
      CreateCommand<OnHMIStatusNotificationFromMobile>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, set_foreground(false));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(app_mngr_, IsAppsQueriedFrom(kHandle)).WillOnce(Return(true));

  DataAccessor<ApplicationSet> accessor(app_set_, lock_);
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app, protocol_version())
      .WillRepeatedly(Return(ProtocolVersion::kV4));
  EXPECT_CALL(*mock_app, is_foreground()).WillRepeatedly(Return(true));

  command->Run();

  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
}

TEST_F(OnHMIStatusNotificationFromMobileTest,
       Run_ProtocolVersionKV3_NoNotification) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_BACKGROUND);

  SharedPtr<OnHMIStatusNotificationFromMobile> command =
      CreateCommand<OnHMIStatusNotificationFromMobile>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, set_foreground(false));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(app_mngr_, IsAppsQueriedFrom(kHandle)).WillOnce(Return(true));

  DataAccessor<ApplicationSet> accessor(app_set_, lock_);
  EXPECT_CALL(app_mngr_, applications()).Times(0);

  EXPECT_CALL(*mock_app, protocol_version())
      .WillOnce(Return(ProtocolVersion::kV3));
  EXPECT_CALL(*mock_app, is_foreground()).Times(0);

  command->Run();

  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
}

TEST_F(OnHMIStatusNotificationFromMobileTest,
       Run_AppNotRequestedBeforeAndKV3_NoNotification) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_FULL);

  SharedPtr<OnHMIStatusNotificationFromMobile> command =
      CreateCommand<OnHMIStatusNotificationFromMobile>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, set_foreground(true));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(app_mngr_, IsAppsQueriedFrom(kHandle)).WillOnce(Return(false));

  EXPECT_CALL(app_mngr_, applications()).Times(0);

  EXPECT_CALL(*mock_app, protocol_version())
      .WillOnce(Return(ProtocolVersion::kV3));
  EXPECT_CALL(*mock_app, is_foreground()).Times(0);

  command->Run();

  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
}

TEST_F(OnHMIStatusNotificationFromMobileTest,
       Run_AppNotRequestedBefore_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_FULL);

  SharedPtr<OnHMIStatusNotificationFromMobile> command =
      CreateCommand<OnHMIStatusNotificationFromMobile>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, set_foreground(true));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(app_mngr_, IsAppsQueriedFrom(kHandle)).WillOnce(Return(false));

  EXPECT_CALL(*mock_app, protocol_version())
      .WillOnce(Return(ProtocolVersion::kV4));

  EXPECT_CALL(app_mngr_, applications()).Times(0);

  EXPECT_CALL(*mock_app, is_foreground()).WillOnce(Return(true));

  application_manager::MockMessageHelper& mock_message_helper =
      *application_manager::MockMessageHelper::message_helper_mock();
  Mock::VerifyAndClearExpectations(&mock_message_helper);
  EXPECT_CALL(mock_message_helper, SendQueryApps(kConnectionKey, _));

  command->Run();

  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
  Mock::VerifyAndClearExpectations(&mock_message_helper);
}

TEST_F(OnHMIStatusNotificationFromMobileTest,
       Run_AnotherForegroundSDLApp_SUCCESS) {
  MessageSharedPtr msg = CreateMsgParams(mobile_apis::HMILevel::HMI_FULL);

  SharedPtr<OnHMIStatusNotificationFromMobile> command =
      CreateCommand<OnHMIStatusNotificationFromMobile>(msg);

  MockAppPtr mock_app = CreateMockApp();
  EXPECT_CALL(app_mngr_, application(kConnectionKey))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(*mock_app, set_foreground(true));

  EXPECT_CALL(*mock_app, device()).WillOnce(Return(kHandle));
  EXPECT_CALL(app_mngr_, IsAppsQueriedFrom(kHandle)).WillOnce(Return(true));

  DataAccessor<ApplicationSet> accessor(app_set_, lock_);
  EXPECT_CALL(app_mngr_, applications()).WillOnce(Return(accessor));

  EXPECT_CALL(*mock_app, protocol_version())
      .WillRepeatedly(Return(ProtocolVersion::kV4));
  EXPECT_CALL(*mock_app, is_foreground()).WillRepeatedly(Return(false));

  EXPECT_CALL(app_mngr_, MarkAppsGreyOut(kHandle, false));
  EXPECT_CALL(app_mngr_, SendUpdateAppList());

  command->Run();

  ASSERT_EQ(application_manager::MessageType::kNotification,
            (*msg)[strings::params][strings::message_type].asInt());
}

}  // namespace on_hmi_status_notification_from_mobile
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

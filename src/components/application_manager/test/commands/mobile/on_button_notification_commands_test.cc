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

#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application.h"

#include "application_manager/include/application_manager/commands/command_impl.h"
#include "application_manager/test/include/application_manager/commands/commands_test.h"
#include "application_manager/test/include/application_manager/commands/command_request_test.h"
#include "application_manager/commands/mobile/on_button_event_notification.h"
#include "application_manager/commands/mobile/on_button_press_notification.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_button_notification {

namespace am = ::application_manager;
namespace commands = am::commands;

using ::testing::_;
using ::testing::Types;
using ::testing::Return;

using ::utils::SharedPtr;
using am::commands::MessageSharedPtr;

namespace {
const uint32_t kAppId = 5u;
const uint32_t kCustomButtonId = 3u;
const mobile_apis::ButtonName::eType kButtonName = mobile_apis::ButtonName::OK;
}  // namespace

template <class NotificationT,
          mobile_apis::FunctionID::eType kExpectedFunctionId>
struct NotificationData {
  typedef NotificationT Notification;
  enum { kFunctionId = kExpectedFunctionId };
};

template <class NotificationDataT>
class OnButtonNotificationCommandsTest
    : public CommandsTest<CommandsTestMocks::kNotNice>,
      public NotificationDataT {};

typedef Types<NotificationData<commands::mobile::OnButtonEventNotification,
                               mobile_apis::FunctionID::OnButtonEventID>,
              NotificationData<commands::mobile::OnButtonPressNotification,
                               mobile_apis::FunctionID::OnButtonPressID> >
    OnButtonNotificationCommandsList;

MATCHER_P(CheckNotificationMessage, function_id, "") {
  const bool kIsMobileProtocolTypeCorrect =
      (*arg)[am::strings::params][am::strings::protocol_type].asInt() ==
      commands::CommandImpl::mobile_protocol_type_;

  const bool kIsProtocolVersionCorrect =
      (*arg)[am::strings::params][am::strings::protocol_version].asInt() ==
      commands::CommandImpl::protocol_version_;

  const bool kIsNotificationCorrect =
      (*arg)[am::strings::params][am::strings::message_type].asInt() ==
      am::MessageType::kNotification;

  const bool kIsFunctionIdCorrect =
      (*arg)[am::strings::params][am::strings::function_id].asInt() ==
      function_id;

  bool is_custom_button_id_correct = true;
  if ((*arg)[am::strings::msg_params].keyExists(
          am::hmi_response::custom_button_id)) {
    is_custom_button_id_correct =
        (*arg)[am::strings::msg_params][am::strings::custom_button_id] ==
        kCustomButtonId;
  }

  return kIsMobileProtocolTypeCorrect && kIsProtocolVersionCorrect &&
         kIsNotificationCorrect && kIsFunctionIdCorrect &&
         is_custom_button_id_correct;
}

TYPED_TEST_CASE(OnButtonNotificationCommandsTest,
                OnButtonNotificationCommandsList);

TYPED_TEST(OnButtonNotificationCommandsTest,
           Run_CustomButton_NoAppId_UNSUCCESS) {
  typedef typename TestFixture::Notification Notification;

  MessageSharedPtr notification_msg(
      this->CreateMessage(smart_objects::SmartType_Map));

  (*notification_msg)[am::strings::msg_params][am::hmi_response::button_name] =
      mobile_apis::ButtonName::CUSTOM_BUTTON;

  SharedPtr<Notification> command(
      this->template CreateCommand<Notification>(notification_msg));

  EXPECT_CALL(this->app_mngr_, SendMessageToMobile(_, _)).Times(0);

  command->Run();
}

TYPED_TEST(OnButtonNotificationCommandsTest,
           Run_CustomButton_NoCustomButtonId_UNSUCCESS) {
  typedef typename TestFixture::Notification Notification;

  MessageSharedPtr notification_msg(
      this->CreateMessage(smart_objects::SmartType_Map));

  (*notification_msg)[am::strings::msg_params][am::hmi_response::button_name] =
      mobile_apis::ButtonName::CUSTOM_BUTTON;
  (*notification_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;

  SharedPtr<Notification> command(
      this->template CreateCommand<Notification>(notification_msg));

  typename TestFixture::MockAppPtr mock_app = this->CreateMockApp();
  EXPECT_CALL(this->app_mngr_, application(kAppId)).WillOnce(Return(mock_app));

  EXPECT_CALL(this->app_mngr_, SendMessageToMobile(_, _)).Times(0);

  command->Run();
}

TYPED_TEST(OnButtonNotificationCommandsTest,
           Run_CustomButton_AppNotRegistered_UNSUCCESS) {
  typedef typename TestFixture::Notification Notification;

  MessageSharedPtr notification_msg(
      this->CreateMessage(smart_objects::SmartType_Map));

  (*notification_msg)[am::strings::msg_params][am::hmi_response::button_name] =
      mobile_apis::ButtonName::CUSTOM_BUTTON;
  (*notification_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  (*notification_msg)[am::strings::msg_params]
                     [am::hmi_response::custom_button_id] = kCustomButtonId;

  SharedPtr<Notification> command(
      this->template CreateCommand<Notification>(notification_msg));

  EXPECT_CALL(this->app_mngr_, application(kAppId))
      .WillOnce(Return(ApplicationSharedPtr()));

  EXPECT_CALL(this->app_mngr_, SendMessageToMobile(_, _)).Times(0);

  command->Run();
}

TYPED_TEST(OnButtonNotificationCommandsTest,
           Run_CustomButton_AppNotSubscribedToCustomButtonId_UNSUCCESS) {
  typedef typename TestFixture::Notification Notification;

  MessageSharedPtr notification_msg(
      this->CreateMessage(smart_objects::SmartType_Map));

  (*notification_msg)[am::strings::msg_params][am::hmi_response::button_name] =
      mobile_apis::ButtonName::CUSTOM_BUTTON;
  (*notification_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  (*notification_msg)[am::strings::msg_params]
                     [am::hmi_response::custom_button_id] = kCustomButtonId;

  SharedPtr<Notification> command(
      this->template CreateCommand<Notification>(notification_msg));

  typename TestFixture::MockAppPtr mock_app = this->CreateMockApp();
  EXPECT_CALL(this->app_mngr_, application(kAppId)).WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, IsSubscribedToSoftButton(kCustomButtonId))
      .WillOnce(Return(false));

  EXPECT_CALL(this->app_mngr_, SendMessageToMobile(_, _)).Times(0);

  command->Run();
}

TYPED_TEST(OnButtonNotificationCommandsTest, Run_CustomButton_SUCCESS) {
  typedef typename TestFixture::Notification Notification;

  MessageSharedPtr notification_msg(
      this->CreateMessage(smart_objects::SmartType_Map));

  (*notification_msg)[am::strings::msg_params][am::hmi_response::button_name] =
      mobile_apis::ButtonName::CUSTOM_BUTTON;
  (*notification_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  (*notification_msg)[am::strings::msg_params]
                     [am::hmi_response::custom_button_id] = kCustomButtonId;

  SharedPtr<Notification> command(
      this->template CreateCommand<Notification>(notification_msg));

  typename TestFixture::MockAppPtr mock_app = this->CreateMockApp();
  EXPECT_CALL(this->app_mngr_, application(kAppId)).WillOnce(Return(mock_app));

  EXPECT_CALL(*mock_app, IsSubscribedToSoftButton(kCustomButtonId))
      .WillOnce(Return(true));

  EXPECT_CALL(this->app_mngr_,
              SendMessageToMobile(
                  CheckNotificationMessage(TestFixture::kFunctionId), _));

  command->Run();
}

TYPED_TEST(OnButtonNotificationCommandsTest, Run_NoSubscribedApps_UNSUCCESS) {
  typedef typename TestFixture::Notification Notification;

  MessageSharedPtr notification_msg(
      this->CreateMessage(smart_objects::SmartType_Map));

  (*notification_msg)[am::strings::msg_params][am::hmi_response::button_name] =
      kButtonName;

  SharedPtr<Notification> command(
      this->template CreateCommand<Notification>(notification_msg));

  const std::vector<ApplicationSharedPtr> empty_subscribed_apps_list;
  EXPECT_CALL(this->app_mngr_, applications_by_button(kButtonName))
      .WillOnce(Return(empty_subscribed_apps_list));

  EXPECT_CALL(this->app_mngr_, SendMessageToMobile(_, _)).Times(0);

  command->Run();
}

TYPED_TEST(OnButtonNotificationCommandsTest, Run_InvalidHmiLevel_UNSUCCESS) {
  typedef typename TestFixture::Notification Notification;

  MessageSharedPtr notification_msg(
      this->CreateMessage(smart_objects::SmartType_Map));

  (*notification_msg)[am::strings::msg_params][am::hmi_response::button_name] =
      kButtonName;

  SharedPtr<Notification> command(
      this->template CreateCommand<Notification>(notification_msg));

  typename TestFixture::MockAppPtr mock_app = this->CreateMockApp();
  std::vector<ApplicationSharedPtr> subscribed_apps_list;
  subscribed_apps_list.push_back(mock_app);

  EXPECT_CALL(*mock_app, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_NONE));

  EXPECT_CALL(this->app_mngr_, applications_by_button(kButtonName))
      .WillOnce(Return(subscribed_apps_list));

  EXPECT_CALL(this->app_mngr_, SendMessageToMobile(_, _)).Times(0);

  command->Run();
}

TYPED_TEST(OnButtonNotificationCommandsTest,
           Run_ButtonOkOnlyForHmiLevelFull_UNSUCCESS) {
  typedef typename TestFixture::Notification Notification;

  MessageSharedPtr notification_msg(
      this->CreateMessage(smart_objects::SmartType_Map));

  (*notification_msg)[am::strings::msg_params][am::hmi_response::button_name] =
      kButtonName;

  SharedPtr<Notification> command(
      this->template CreateCommand<Notification>(notification_msg));

  typename TestFixture::MockAppPtr mock_app = this->CreateMockApp();
  std::vector<ApplicationSharedPtr> subscribed_apps_list;
  subscribed_apps_list.push_back(mock_app);

  EXPECT_CALL(*mock_app, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_LIMITED));

  EXPECT_CALL(this->app_mngr_, applications_by_button(kButtonName))
      .WillOnce(Return(subscribed_apps_list));

  EXPECT_CALL(this->app_mngr_, SendMessageToMobile(_, _)).Times(0);

  command->Run();
}

TYPED_TEST(OnButtonNotificationCommandsTest, Run_SUCCESS) {
  typedef typename TestFixture::Notification Notification;

  MessageSharedPtr notification_msg(
      this->CreateMessage(smart_objects::SmartType_Map));

  (*notification_msg)[am::strings::msg_params][am::strings::app_id] = kAppId;
  (*notification_msg)[am::strings::msg_params][am::hmi_response::button_name] =
      kButtonName;

  SharedPtr<Notification> command(
      this->template CreateCommand<Notification>(notification_msg));

  typename TestFixture::MockAppPtr mock_app = this->CreateMockApp();
  std::vector<ApplicationSharedPtr> subscribed_apps_list;
  subscribed_apps_list.push_back(mock_app);

  EXPECT_CALL(*mock_app, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));

  ON_CALL(*mock_app, IsFullscreen()).WillByDefault(Return(true));

  EXPECT_CALL(this->app_mngr_, applications_by_button(kButtonName))
      .WillOnce(Return(subscribed_apps_list));

  EXPECT_CALL(this->app_mngr_,
              SendMessageToMobile(
                  CheckNotificationMessage(TestFixture::kFunctionId), _));

  command->Run();
}

}  // namespace on_button_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

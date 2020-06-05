/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "application_manager/commands/commands_test.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "application_manager/smart_object_keys.h"
#include "gtest/gtest.h"
#include "hmi/on_driver_distraction_notification.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/smart_object.h"
#include "utils/data_accessor.h"
#include "utils/lock.h"

namespace test {
namespace components {
namespace commands_test {
namespace hmi_commands_test {
namespace on_driver_distraction_notification {

using ::testing::_;
using ::testing::DoAll;
using ::testing::Eq;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::SaveArg;
using ::testing::SetArgumentPointee;

namespace am = ::application_manager;
using am::commands::MessageSharedPtr;
using sdl_rpc_plugin::commands::hmi::OnDriverDistractionNotification;
using namespace am::commands;
using test::components::commands_test::MobileResultCodeIs;

namespace {
const std::string kDefaultLanguage = "en-us";
const mobile_apis::Language::eType kMobileLanguage =
    mobile_apis::Language::EN_US;

// LSDW - lock screen dismissal warning
const std::string kLockScreenDismissalWarningMessage_en =
    "Swipe down to dismiss, acknowledging that you are not the driver";
const uint32_t kConnectionKey = 2u;
}  // namespace

class HMIOnDriverDistractionNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  HMIOnDriverDistractionNotificationTest()
      : mock_app_(CreateMockApp())
      , app_set_lock_(std::make_shared<sync_primitives::Lock>())
      , accessor(app_set_, app_set_lock_) {
    app_set_.insert(mock_app_);
    InitMocksRelations();
  }

  typedef std::shared_ptr<OnDriverDistractionNotification> NotificationPtr;
  typedef boost::optional<bool> OptionalBool;

  void SetUp() OVERRIDE {
    ON_CALL(*mock_app_, ui_language())
        .WillByDefault(ReturnRef(kMobileLanguage));
    ON_CALL(app_mngr_, application(kConnectionKey))
        .WillByDefault(Return(mock_app_));
    ON_CALL(mock_message_helper_, MobileLanguageToString(kMobileLanguage))
        .WillByDefault(Return(kDefaultLanguage));
  }

  MockAppPtr mock_app_;
  std::shared_ptr<sync_primitives::Lock> app_set_lock_;
  am::ApplicationSet app_set_;
  DataAccessor<am::ApplicationSet> accessor;
  NiceMock<policy_test::MockPolicyHandlerInterface>
      mock_policy_handler_interface_;

  void InitMocksRelations() {
    ON_CALL(app_mngr_, applications()).WillByDefault(Return(accessor));
    ON_CALL(app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_interface_));
  }
};

MATCHER_P2(CheckNotificationParams, function_id, state, "") {
  bool is_function_id_matched =
      function_id ==
      static_cast<am::mobile_api::FunctionID::eType>(
          (*arg)[am::strings::params][am::strings::function_id].asInt());
  bool is_state_matched =
      state ==
      static_cast<hmi_apis::Common_DriverDistractionState::eType>(
          (*arg)[am::strings::msg_params][am::mobile_notification::state]
              .asInt());
  return is_function_id_matched && is_state_matched;
}

ACTION_P(GetArg4, result) {
  arg4 = *result;
}

ACTION_P(SetMessage, lockScreenDismissalWarning) {
  smart_objects::SmartObject& notification = arg0;

  notification[application_manager::strings::msg_params]
              [application_manager::mobile_notification::
                   lock_screen_dismissal_warning] = lockScreenDismissalWarning;
}

TEST_F(HMIOnDriverDistractionNotificationTest,
       Run_SendNotificationToMobile_SUCCESS) {
  const auto state = hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr commands_msg(CreateMessage(smart_objects::SmartType_Map));
  (*commands_msg)[am::strings::msg_params][am::hmi_notification::state] = state;
  (*commands_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  NotificationPtr command(
      CreateCommand<OnDriverDistractionNotification>(commands_msg));

  EXPECT_CALL(app_mngr_, set_driver_distraction_state(Eq(state)));

  ON_CALL(mock_policy_handler_interface_, LockScreenDismissalEnabledState())
      .WillByDefault(Return(OptionalBool(true)));

  ON_CALL(mock_policy_handler_interface_, LockScreenDismissalWarningMessage(_))
      .WillByDefault(Return(kLockScreenDismissalWarningMessage_en));

  policy::CheckPermissionResult result;
  result.hmi_level_permitted = policy::kRpcAllowed;
  EXPECT_CALL(mock_policy_handler_interface_, CheckPermissions(_, _, _, _, _))
      .WillOnce(GetArg4(&result));

  MessageSharedPtr message_to_mobile(
      CreateMessage(smart_objects::SmartType_Map));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  CheckNotificationParams(
                      am::mobile_api::FunctionID::OnDriverDistractionID, state),
                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&message_to_mobile), Return(true)));
  command->Run();

  ASSERT_TRUE((*message_to_mobile)[am::strings::msg_params].keyExists(
      am::mobile_notification::lock_screen_dismissal_warning));

  auto LSDW_message =
      (*message_to_mobile)
          [am::strings::msg_params]
          [am::mobile_notification::lock_screen_dismissal_warning]
              .asString();

  EXPECT_EQ(kLockScreenDismissalWarningMessage_en, LSDW_message);
}

TEST_F(HMIOnDriverDistractionNotificationTest,
       Run_PushMobileMessage_If_DisallowedByPolicy) {
  const auto state = hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr commands_msg(CreateMessage(smart_objects::SmartType_Map));
  (*commands_msg)[am::strings::msg_params][am::hmi_notification::state] = state;
  (*commands_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;
  NotificationPtr command(
      CreateCommand<OnDriverDistractionNotification>(commands_msg));

  ON_CALL(mock_policy_handler_interface_, LockScreenDismissalEnabledState())
      .WillByDefault(Return(boost::optional<bool>(true)));

  ON_CALL(mock_policy_handler_interface_, LockScreenDismissalWarningMessage(_))
      .WillByDefault(Return(kLockScreenDismissalWarningMessage_en));

  policy::CheckPermissionResult result;
  result.hmi_level_permitted = policy::kRpcDisallowed;
  EXPECT_CALL(app_mngr_, GetPolicyHandler()).Times(3);
  EXPECT_CALL(mock_policy_handler_interface_, CheckPermissions(_, _, _, _, _))
      .WillOnce(GetArg4(&result));

  MessageSharedPtr pushed_message(CreateMessage(smart_objects::SmartType_Map));
  EXPECT_CALL(*mock_app_,
              PushMobileMessage(CheckNotificationParams(
                  am::mobile_api::FunctionID::OnDriverDistractionID, state)))
      .WillOnce(SaveArg<0>(&pushed_message));

  EXPECT_CALL(app_mngr_, set_driver_distraction_state(Eq(state)));
  command->Run();

  ASSERT_TRUE((*pushed_message)[am::strings::msg_params].keyExists(
      am::mobile_notification::lock_screen_dismissal_warning));

  auto lock_screen_dismissal_warning_message =
      (*pushed_message)[am::strings::msg_params]
                       [am::mobile_notification::lock_screen_dismissal_warning]
                           .asString();

  EXPECT_EQ(kLockScreenDismissalWarningMessage_en,
            lock_screen_dismissal_warning_message);
}

TEST_F(HMIOnDriverDistractionNotificationTest,
       Run_SendNotificationIfLockScreenDismissalMissed) {
  const auto state = hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr commands_msg(CreateMessage(smart_objects::SmartType_Map));
  (*commands_msg)[am::strings::msg_params][am::hmi_notification::state] = state;

  NotificationPtr command(
      CreateCommand<OnDriverDistractionNotification>(commands_msg));

  ON_CALL(mock_policy_handler_interface_, LockScreenDismissalEnabledState())
      .WillByDefault(Return(boost::optional<bool>()));

  policy::CheckPermissionResult result;
  result.hmi_level_permitted = policy::kRpcAllowed;
  ON_CALL(mock_policy_handler_interface_, CheckPermissions(_, _, _, _, _))
      .WillByDefault(GetArg4(&result));

  MessageSharedPtr command_result;
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&command_result), Return(true)));

  command->Run();

  auto& msg_params =
      (*command_result)[application_manager::strings::msg_params];
  EXPECT_FALSE(msg_params.keyExists(
      application_manager::mobile_notification::lock_screen_dismissal_enabled));
  EXPECT_FALSE(msg_params.keyExists(
      application_manager::mobile_notification::lock_screen_dismissal_warning));
}

// LSDW - lock screen dimissal
TEST_F(HMIOnDriverDistractionNotificationTest,
       Run_SendNotificationToMobile_LSDWMessageIsAbsent_SUCCESS) {
  const auto state = hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr commands_msg(CreateMessage(smart_objects::SmartType_Map));
  (*commands_msg)[am::strings::msg_params][am::hmi_notification::state] = state;
  (*commands_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  NotificationPtr command(
      CreateCommand<OnDriverDistractionNotification>(commands_msg));

  EXPECT_CALL(app_mngr_, set_driver_distraction_state(Eq(state)));

  ON_CALL(mock_policy_handler_interface_, LockScreenDismissalEnabledState())
      .WillByDefault(Return(OptionalBool(false)));

  // LockScreenDismissalWarning won't be added to message if value of
  // LockScreenDismissalEnabledState is false
  std::string required_language = "en-us";
  EXPECT_CALL(mock_policy_handler_interface_,
              LockScreenDismissalWarningMessage(required_language))
      .Times(0);

  policy::CheckPermissionResult result;
  result.hmi_level_permitted = policy::kRpcAllowed;
  MessageSharedPtr message_to_mobile(
      CreateMessage(smart_objects::SmartType_Map));
  EXPECT_CALL(app_mngr_, GetPolicyHandler()).Times(2);
  EXPECT_CALL(mock_policy_handler_interface_, CheckPermissions(_, _, _, _, _))
      .WillOnce(GetArg4(&result));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  CheckNotificationParams(
                      am::mobile_api::FunctionID::OnDriverDistractionID, state),
                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&message_to_mobile), Return(true)));
  command->Run();

  EXPECT_FALSE((*message_to_mobile)[am::strings::msg_params].keyExists(
      am::mobile_notification::lock_screen_dismissal_warning));
}

TEST_F(HMIOnDriverDistractionNotificationTest,
       Run_SendNotificationToMobile_SpecifiedLanguageIsAbsent_SUCCESS) {
  const auto state = hmi_apis::Common_DriverDistractionState::DD_ON;
  MessageSharedPtr commands_msg(CreateMessage(smart_objects::SmartType_Map));
  (*commands_msg)[am::strings::msg_params][am::hmi_notification::state] = state;
  (*commands_msg)[am::strings::params][am::strings::connection_key] =
      kConnectionKey;

  NotificationPtr command(
      CreateCommand<OnDriverDistractionNotification>(commands_msg));

  EXPECT_CALL(app_mngr_, set_driver_distraction_state(Eq(state)));

  ON_CALL(mock_policy_handler_interface_, LockScreenDismissalEnabledState())
      .WillByDefault(Return(OptionalBool(true)));

  ON_CALL(mock_policy_handler_interface_, LockScreenDismissalWarningMessage(_))
      .WillByDefault(Return(kLockScreenDismissalWarningMessage_en));

  // In case when specified language is absent in policy table, will added
  // message on default language (en-us)
  const mobile_apis::Language::eType mobile_language =
      mobile_apis::Language::FR_FR;
  std::string required_language = "FR-FR";
  ON_CALL(*mock_app_, ui_language()).WillByDefault(ReturnRef(mobile_language));
  ON_CALL(mock_message_helper_, MobileLanguageToString(mobile_language))
      .WillByDefault(Return(required_language));

  policy::CheckPermissionResult result;
  result.hmi_level_permitted = policy::kRpcAllowed;
  EXPECT_CALL(mock_policy_handler_interface_, CheckPermissions(_, _, _, _, _))
      .WillOnce(GetArg4(&result));

  MessageSharedPtr message_to_mobile(
      CreateMessage(smart_objects::SmartType_Map));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(
                  CheckNotificationParams(
                      am::mobile_api::FunctionID::OnDriverDistractionID, state),
                  Command::CommandSource::SOURCE_SDL))
      .WillOnce(DoAll(SaveArg<0>(&message_to_mobile), Return(true)));
  command->Run();

  ASSERT_TRUE((*message_to_mobile)[am::strings::msg_params].keyExists(
      am::mobile_notification::lock_screen_dismissal_warning));

  auto lock_screen_dismissal_warning_message =
      (*message_to_mobile)
          [am::strings::msg_params]
          [am::mobile_notification::lock_screen_dismissal_warning]
              .asString();

  EXPECT_EQ(kLockScreenDismissalWarningMessage_en,
            lock_screen_dismissal_warning_message);
}
}  // namespace on_driver_distraction_notification
}  // namespace hmi_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

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

#include "mobile/on_system_capability_updated_notification.h"

#include "application_manager/commands/commands_test.h"
#include "application_manager/display_capabilities_builder.h"
#include "gtest/gtest.h"
#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"
#include "sdl_rpc_plugin/sdl_rpc_plugin.h"

namespace test {
namespace components {
namespace commands_test {
namespace mobile_commands_test {
namespace on_system_capability_updated_notification {

using sdl_rpc_plugin::commands::mobile::OnSystemCapabilityUpdatedNotification;
using ::testing::_;
using ::testing::Return;

typedef std::shared_ptr<OnSystemCapabilityUpdatedNotification>
    OnSystemCapabilityUpdatedNotificationPtr;

namespace strings = application_manager::strings;
namespace {
const uint32_t kConnectionKey = 1u;
const uint32_t kAppId = 2u;
}  // namespace

MATCHER_P(CheckDisplayCapabilities, display_capabilities, "") {
  return *display_capabilities ==
         (*arg)[strings::msg_params][strings::system_capability]
               [strings::display_capabilities];
}

class OnSystemCapabilityUpdatedNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 protected:
  void SetUp() OVERRIDE {
    message_ = CreateMessage();
    (*message_)[strings::params][strings::connection_key] = kConnectionKey;

    command_ = CreateCommand<OnSystemCapabilityUpdatedNotification>(message_);
    mock_app_ = CreateMockApp();
  }

  OnSystemCapabilityUpdatedNotificationPtr command_;
  MockAppPtr mock_app_;
  MessageSharedPtr message_;
};

TEST_F(
    OnSystemCapabilityUpdatedNotificationTest,
    Run_AppExistSubscribedToNotification_SystemDisplayCapabilitiesSendToMobile) {
  (*message_)[am::strings::msg_params][strings::system_capability]
             [am::strings::system_capability_type] =
                 mobile_apis::SystemCapabilityType::DISPLAYS;

  smart_objects::SmartObjectSPtr system_display_capabilities =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Null);

  ON_CALL(mock_hmi_capabilities_, system_display_capabilities())
      .WillByDefault(Return(system_display_capabilities));

  sdl_rpc_plugin::SDLRPCPlugin sdl_rpc_plugin;

  std::shared_ptr<sdl_rpc_plugin::SystemCapabilityAppExtension>
      system_capability_app_extension(
          std::make_shared<sdl_rpc_plugin::SystemCapabilityAppExtension>(
              sdl_rpc_plugin, *mock_app_));
  system_capability_app_extension->SubscribeTo(
      mobile_apis::SystemCapabilityType::DISPLAYS);
  application_manager::ApplicationSet apps({mock_app_});
  std::shared_ptr<sync_primitives::Lock> apps_lock_(
      std::make_shared<sync_primitives::Lock>());
  DataAccessor<application_manager::ApplicationSet> apps_data(
      DataAccessor<application_manager::ApplicationSet>(apps, apps_lock_));

  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_data));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
  ON_CALL(*mock_app_, display_capabilities()).WillByDefault(Return(nullptr));
  ON_CALL(*mock_app_,
          QueryInterface(sdl_rpc_plugin::SystemCapabilityAppExtension::
                             SystemCapabilityAppExtensionUID))
      .WillByDefault(Return(system_capability_app_extension));
  application_manager::DisplayCapabilitiesBuilder builder(*mock_app_);
  ON_CALL(*mock_app_, display_capabilities_builder())
      .WillByDefault(ReturnRef(builder));

  EXPECT_CALL(
      mock_rpc_service_,
      SendMessageToMobile(CheckDisplayCapabilities(system_display_capabilities),
                          false));

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(
    OnSystemCapabilityUpdatedNotificationTest,
    Run_AppExistConnectionKeyNotEqualWithAppId_SystemDisplayCapabilitiesNotSendToMobile) {
  (*message_)[am::strings::msg_params][strings::system_capability]
             [am::strings::system_capability_type] =
                 mobile_apis::SystemCapabilityType::DISPLAYS;

  smart_objects::SmartObjectSPtr system_display_capabilities =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Null);

  ON_CALL(mock_hmi_capabilities_, system_display_capabilities())
      .WillByDefault(Return(system_display_capabilities));

  sdl_rpc_plugin::SDLRPCPlugin sdl_rpc_plugin;

  std::shared_ptr<sdl_rpc_plugin::SystemCapabilityAppExtension>
      system_capability_app_extension(
          std::make_shared<sdl_rpc_plugin::SystemCapabilityAppExtension>(
              sdl_rpc_plugin, *mock_app_));
  system_capability_app_extension->SubscribeTo(
      mobile_apis::SystemCapabilityType::DISPLAYS);
  application_manager::ApplicationSet apps({mock_app_});
  std::shared_ptr<sync_primitives::Lock> apps_lock_(
      std::make_shared<sync_primitives::Lock>());
  DataAccessor<application_manager::ApplicationSet> apps_data(
      DataAccessor<application_manager::ApplicationSet>(apps, apps_lock_));

  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_data));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
  ON_CALL(*mock_app_, display_capabilities()).WillByDefault(Return(nullptr));
  ON_CALL(*mock_app_,
          QueryInterface(sdl_rpc_plugin::SystemCapabilityAppExtension::
                             SystemCapabilityAppExtensionUID))
      .WillByDefault(Return(system_capability_app_extension));

  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, _)).Times(0);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(
    OnSystemCapabilityUpdatedNotificationTest,
    Run_AppExistNotSubscribedToNotification_SystemDisplayCapabilitiesNotSendToMobile) {
  (*message_)[am::strings::msg_params][strings::system_capability]
             [am::strings::system_capability_type] =
                 mobile_apis::SystemCapabilityType::DISPLAYS;

  smart_objects::SmartObjectSPtr system_display_capabilities =
      std::make_shared<smart_objects::SmartObject>(
          smart_objects::SmartType_Null);

  ON_CALL(mock_hmi_capabilities_, system_display_capabilities())
      .WillByDefault(Return(system_display_capabilities));

  sdl_rpc_plugin::SDLRPCPlugin sdl_rpc_plugin;

  std::shared_ptr<sdl_rpc_plugin::SystemCapabilityAppExtension>
      system_capability_app_extension(
          std::make_shared<sdl_rpc_plugin::SystemCapabilityAppExtension>(
              sdl_rpc_plugin, *mock_app_));

  application_manager::ApplicationSet apps({mock_app_});
  std::shared_ptr<sync_primitives::Lock> apps_lock_(
      std::make_shared<sync_primitives::Lock>());
  DataAccessor<application_manager::ApplicationSet> apps_data(
      DataAccessor<application_manager::ApplicationSet>(apps, apps_lock_));

  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_data));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kConnectionKey));
  ON_CALL(*mock_app_, display_capabilities()).WillByDefault(Return(nullptr));
  ON_CALL(*mock_app_,
          QueryInterface(sdl_rpc_plugin::SystemCapabilityAppExtension::
                             SystemCapabilityAppExtensionUID))
      .WillByDefault(Return(system_capability_app_extension));

  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, _)).Times(0);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

TEST_F(
    OnSystemCapabilityUpdatedNotificationTest,
    Run_AppDataEmpty_SystemDisplayCapabilitiesUpdatedNotificationNotSendToMobile) {
  (*message_)[am::strings::msg_params][strings::system_capability]
             [am::strings::system_capability_type] =
                 mobile_apis::SystemCapabilityType::DISPLAYS;
  application_manager::ApplicationSet apps;
  std::shared_ptr<sync_primitives::Lock> apps_lock_(
      std::make_shared<sync_primitives::Lock>());
  DataAccessor<application_manager::ApplicationSet> apps_data(
      DataAccessor<application_manager::ApplicationSet>(apps, apps_lock_));

  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_data));
  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, _)).Times(0);

  ASSERT_TRUE(command_->Init());
  command_->Run();
}

}  // namespace on_system_capability_updated_notification
}  // namespace mobile_commands_test
}  // namespace commands_test
}  // namespace components
}  // namespace test

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

#include "application_manager/application.h"
#include "application_manager/mock_application.h"
#include "application_manager/commands/commands_test.h"
#include "rc_rpc_plugin/commands/mobile/on_interior_vehicle_data_notification.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "rc_rpc_plugin/mock/mock_resource_allocation_manager.h"
#include "rc_rpc_plugin/mock/mock_interior_data_cache.h"
#include "rc_rpc_plugin/mock/mock_interior_data_manager.h"
#include "gtest/gtest.h"
#include "interfaces/MOBILE_API.h"

using ::testing::_;
using ::testing::Return;
using ::testing::NiceMock;
using ::testing::SaveArg;
using ::testing::Mock;
using application_manager::ApplicationSet;
using application_manager::commands::MessageSharedPtr;
using test::components::application_manager_test::MockApplication;
using test::components::application_manager_test::MockApplicationManager;
using test::components::commands_test::CommandsTest;
using test::components::commands_test::CommandsTestMocks;

namespace {
const uint32_t kAppId = 0u;
const uint32_t kConnectionKey = 1u;
const std::string kPolicyAppId = "Test";
const int kModuleId = 153u;
}

namespace rc_rpc_plugin_test {
using namespace rc_rpc_plugin;
using application_manager::ApplicationSet;

class OnInteriorVehicleDataNotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnInteriorVehicleDataNotificationTest()
      : mock_app_(std::make_shared<NiceMock<MockApplication> >())
      , rc_app_extention_(std::make_shared<RCAppExtension>(kModuleId))
      , apps_lock_(std::make_shared<sync_primitives::Lock>())
      , apps_da_(apps_, apps_lock_) {
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(*mock_app_, is_remote_control_supported())
        .WillByDefault(Return(true));
    ON_CALL(*mock_app_, QueryInterface(_))
        .WillByDefault(Return(rc_app_extention_));
  }

  MessageSharedPtr CreateBasicMessage() {
    MessageSharedPtr message = CreateMessage();
    (*message)[application_manager::strings::params]
              [application_manager::strings::function_id] =
                  mobile_apis::FunctionID::OnInteriorVehicleDataID;
    (*message)[application_manager::strings::params]
              [application_manager::strings::connection_key] = kConnectionKey;
    (*message)[application_manager::strings::params]
              [application_manager::strings::app_id] = kAppId;
    smart_objects::SmartObject& msg_param =
        (*message)[application_manager::strings::msg_params];
    msg_param[message_params::kModuleData][message_params::kModuleType] =
        mobile_apis::ModuleType::CLIMATE;
    return message;
  }

  template <class Command>
  std::shared_ptr<Command> CreateRCCommand(MessageSharedPtr& msg) {
    InitCommand(kDefaultTimeout_);
    RCCommandParams params{app_mngr_,
                           mock_rpc_service_,
                           mock_hmi_capabilities_,
                           mock_policy_handler_,
                           mock_allocation_manager_,
                           mock_interior_data_cache_,
                           mock_interior_data_manager_};
    return ::std::make_shared<Command>(msg ? msg : msg = CreateMessage(),
                                       params);
  }

 protected:
  std::shared_ptr<MockApplication> mock_app_;
  std::shared_ptr<RCAppExtension> rc_app_extention_;
  testing::NiceMock<rc_rpc_plugin_test::MockResourceAllocationManager>
      mock_allocation_manager_;
  testing::NiceMock<rc_rpc_plugin_test::MockInteriorDataCache>
      mock_interior_data_cache_;
  testing::NiceMock<rc_rpc_plugin_test::MockInteriorDataManager>
      mock_interior_data_manager_;
  application_manager::ApplicationSet apps_;
  const std::shared_ptr<sync_primitives::Lock> apps_lock_;
  DataAccessor<application_manager::ApplicationSet> apps_da_;
};

TEST_F(OnInteriorVehicleDataNotificationTest,
       Run_SendMessageToMobile_Notification) {
  // Arrange
  MessageSharedPtr mobile_message = CreateBasicMessage();
  apps_.insert(mock_app_);
  rc_app_extention_->SubscribeToInteriorVehicleData(enums_value::kClimate);
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_da_));

  // Expectations
  EXPECT_CALL(mock_interior_data_cache_, Add(enums_value::kClimate, _));
  MessageSharedPtr message;
  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, false))
      .WillOnce(SaveArg<0>(&message));
  // Act
  std::shared_ptr<rc_rpc_plugin::commands::OnInteriorVehicleDataNotification>
      command = CreateRCCommand<
          rc_rpc_plugin::commands::OnInteriorVehicleDataNotification>(
          mobile_message);
  command->Run();

  // Assertions
  ASSERT_TRUE(message.get());
  Mock::VerifyAndClearExpectations(&app_mngr_);
}

}  // namespace rc_rpc_plugin_test

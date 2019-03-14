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
#include "application_manager/mock_app_service_manager.h"
#include "application_manager/commands/commands_test.h"
#include "app_service_rpc_plugin/commands/mobile/on_app_service_data_notification.h"
#include "app_service_rpc_plugin/app_service_rpc_plugin.h"
#include "app_service_rpc_plugin/app_service_app_extension.h"
#include "gtest/gtest.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/commands/command_impl.h"
#include "resumption/last_state_impl.h"

namespace am = application_manager;
using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::ReturnNull;
using ::testing::NiceMock;
using ::testing::SaveArg;
using ::testing::Mock;
using am::ApplicationSet;
using am::commands::MessageSharedPtr;
using test::components::application_manager_test::MockApplication;
using test::components::application_manager_test::MockApplicationManager;
using test::components::application_manager_test::MockAppServiceManager;
using test::components::commands_test::CommandsTest;
using test::components::commands_test::CommandsTestMocks;
using app_service_rpc_plugin::commands::OnAppServiceDataNotification;

namespace {
const uint32_t kAppId = 0u;
const uint32_t kConnectionKey = 1u;
const std::string kPolicyAppId = "Test";
const int kModuleId = 153u;
}

namespace app_service_plugin_test {
using namespace app_service_rpc_plugin;
using application_manager::ApplicationSet;

class OnAppServiceDatanotificationTest
    : public CommandsTest<CommandsTestMocks::kIsNice> {
 public:
  OnAppServiceDatanotificationTest()
      : mock_app_(std::make_shared<NiceMock<MockApplication> >())
      , app_service_app_extension_(
            std::make_shared<app_service_rpc_plugin::AppServiceAppExtension>(
                app_service_plugin_, *mock_app_))
      , apps_lock_(std::make_shared<sync_primitives::Lock>())
      , apps_da_(apps_, apps_lock_)
      , last_state_("app_storage_folder", "app_info_storage")
      , app_service_manager_(app_mngr_, last_state_) {
    ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId));
    ON_CALL(*mock_app_, is_remote_control_supported())
        .WillByDefault(Return(true));
    ON_CALL(*mock_app_, QueryInterface(_))
        .WillByDefault(Return(app_service_app_extension_));
  }

  MessageSharedPtr CreateBasicMessage() {
    MessageSharedPtr message = CreateMessage();
    (*message)[application_manager::strings::params]
              [application_manager::strings::function_id] =
                  mobile_apis::FunctionID::OnAppServiceDataID;
    (*message)[application_manager::strings::params]
              [application_manager::strings::connection_key] = kConnectionKey;
    (*message)[application_manager::strings::params]
              [application_manager::strings::app_id] = kAppId;

    return message;
  }

 protected:
  std::shared_ptr<MockApplication> mock_app_;
  std::shared_ptr<AppServiceAppExtension> app_service_app_extension_;
  app_service_rpc_plugin::AppServiceRpcPlugin app_service_plugin_;
  application_manager::ApplicationSet apps_;
  const std::shared_ptr<sync_primitives::Lock> apps_lock_;
  DataAccessor<application_manager::ApplicationSet> apps_da_;
  resumption::LastStateImpl last_state_;
  MockAppServiceManager app_service_manager_;
};

TEST_F(OnAppServiceDatanotificationTest, Run_SendMessageToMobile_Notification) {
  MessageSharedPtr mobile_message = CreateBasicMessage();

  (*mobile_message)[am::strings::msg_params][am::strings::service_data]
                   [am::strings::service_type] = "MEDIA";
  (*mobile_message)[am::strings::msg_params][am::strings::service_data]
                   [am::strings::service_id] = "mediaid1234";
  (*mobile_message)[am::strings::msg_params][am::strings::service_data]
                   ["mediaServiceData"]["mediaTitle"] = "media title";

  apps_.insert(mock_app_);
  app_service_app_extension_->SubscribeToAppService("MEDIA");
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_da_));

  am::AppService service;
  service.connection_key = 1;
  service.mobile_service = true;
  service
      .record[am::strings::service_manifest][am::strings::allow_app_consumers] =
      true;
  EXPECT_CALL(app_service_manager_, FindServiceByID(_))
      .WillOnce(Return(&service));

  EXPECT_CALL(app_mngr_, GetAppServiceManager())
      .WillRepeatedly(ReturnRef(app_service_manager_));

  std::shared_ptr<OnAppServiceDataNotification> command =
      CreateCommand<OnAppServiceDataNotification>(mobile_message);

  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, false))
      .WillOnce(SaveArg<0>(&mobile_message));
  // Act
  command->Run();

  // Assertions
  ASSERT_TRUE(mobile_message.get());
  Mock::VerifyAndClearExpectations(&app_mngr_);
}

TEST_F(OnAppServiceDatanotificationTest, Run_NoService) {
  MessageSharedPtr mobile_message = CreateBasicMessage();

  (*mobile_message)[am::strings::msg_params][am::strings::service_data]
                   [am::strings::service_type] = "MEDIA";
  (*mobile_message)[am::strings::msg_params][am::strings::service_data]
                   [am::strings::service_id] = "mediaid1234";
  (*mobile_message)[am::strings::msg_params][am::strings::service_data]
                   ["mediaServiceData"]["mediaTitle"] = "media title";

  apps_.insert(mock_app_);
  app_service_app_extension_->SubscribeToAppService("MEDIA");
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_da_));

  EXPECT_CALL(app_service_manager_, FindServiceByID(_)).WillOnce(ReturnNull());

  EXPECT_CALL(app_mngr_, GetAppServiceManager())
      .WillRepeatedly(ReturnRef(app_service_manager_));

  std::shared_ptr<OnAppServiceDataNotification> command =
      CreateCommand<OnAppServiceDataNotification>(mobile_message);

  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, false)).Times(0);
  command->Run();

  Mock::VerifyAndClearExpectations(&app_mngr_);
}

TEST_F(OnAppServiceDatanotificationTest, Run_NoSubscribedApps) {
  MessageSharedPtr mobile_message = CreateBasicMessage();

  (*mobile_message)[am::strings::msg_params][am::strings::service_data]
                   [am::strings::service_type] = "MEDIA";
  (*mobile_message)[am::strings::msg_params][am::strings::service_data]
                   [am::strings::service_id] = "mediaid1234";
  (*mobile_message)[am::strings::msg_params][am::strings::service_data]
                   ["mediaServiceData"]["mediaTitle"] = "media title";

  apps_.insert(mock_app_);
  ON_CALL(app_mngr_, applications()).WillByDefault(Return(apps_da_));

  am::AppService service;
  service.connection_key = 1;
  service.mobile_service = true;
  service
      .record[am::strings::service_manifest][am::strings::allow_app_consumers] =
      true;
  EXPECT_CALL(app_service_manager_, FindServiceByID(_))
      .WillOnce(Return(&service));

  EXPECT_CALL(app_mngr_, GetAppServiceManager())
      .WillRepeatedly(ReturnRef(app_service_manager_));

  std::shared_ptr<OnAppServiceDataNotification> command =
      CreateCommand<OnAppServiceDataNotification>(mobile_message);

  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, false)).Times(0);
  command->Run();

  Mock::VerifyAndClearExpectations(&app_mngr_);
}

}  // namespace rc_rpc_plugin_test

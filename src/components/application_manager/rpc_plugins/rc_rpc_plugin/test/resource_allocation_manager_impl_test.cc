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
 * POSSIBILITY OF SUCH DAMAGE. */

#include <algorithm>
#include "gtest/gtest.h"

#include "rc_rpc_plugin/resource_allocation_manager_impl.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_rpc_service.h"
#include "application_manager/policies/mock_policy_handler_interface.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

using test::components::application_manager_test::MockApplication;
using test::components::application_manager_test::MockApplicationManager;
using test::components::application_manager_test::MockRPCService;

using ::testing::_;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::StrictMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SaveArg;
using ::testing::Eq;
using ::testing::DoAll;
using ::testing::SetArgPointee;
using ::application_manager::Message;
using ::application_manager::MessageType;
using ::application_manager::ApplicationSharedPtr;
using ::protocol_handler::MessagePriority;

namespace {
const bool kDeviceHandle = 1u;
const std::string kModuleType1 = "CLIMATE";
const std::string kModuleType2 = "RADIO";
const int32_t kConnectionKey = 5;
const int32_t kCorrelationId = 5;
const uint32_t kAppId1 = 11u;
const uint32_t kHMIAppId1 = 1u;
const uint32_t kAppId2 = 22u;
const std::string policy_app_id_1_ = "policy_id_1";
const uint32_t kSizeOfModules = 6u;
}

namespace rc_rpc_plugin_test {

using namespace rc_rpc_plugin;

class RAManagerTest : public ::testing::Test {
 public:
  RAManagerTest()
      : message_(std::make_shared<Message>(MessagePriority::FromServiceType(
            protocol_handler::ServiceType::kRpc)))
      , mock_app_1_(std::make_shared<NiceMock<MockApplication> >())
      , mock_app_2_(std::make_shared<NiceMock<MockApplication> >())
      , apps_lock_ptr_(std::make_shared<sync_primitives::Lock>())
      , apps_da_(apps_, apps_lock_ptr_) {
    ON_CALL(mock_app_mngr_, GetPolicyHandler())
        .WillByDefault(ReturnRef(mock_policy_handler_));
    auto plugin_id = rc_rpc_plugin::RCRPCPlugin::kRCPluginID;
    app_ext_ptr_ = std::make_shared<rc_rpc_plugin::RCAppExtension>(plugin_id);
    ON_CALL(*mock_app_1_, app_id()).WillByDefault(Return(kAppId1));

    OnRCStatusNotificationExpectations();
  }

  void CheckResultWithHMILevelAndAccessMode(
      ResourceAllocationManagerImpl& ra_manager,
      mobile_apis::HMILevel::eType app_level,
      const rc_rpc_plugin::AcquireResult::eType expected_result,
      const hmi_apis::Common_RCAccessMode::eType access_mode);

  void OnRCStatusNotificationExpectations();

 protected:
  NiceMock<MockApplicationManager> mock_app_mngr_;
  NiceMock<MockRPCService> mock_rpc_service_;
  test::components::policy_test::MockPolicyHandlerInterface
      mock_policy_handler_;
  application_manager::MessagePtr message_;

  application_manager::AppExtensionPtr app_ext_ptr_;
  std::shared_ptr<NiceMock<MockApplication> > mock_app_1_;
  std::shared_ptr<NiceMock<MockApplication> > mock_app_2_;
  application_manager::ApplicationSet apps_;
  std::shared_ptr<sync_primitives::Lock> apps_lock_ptr_;
  DataAccessor<application_manager::ApplicationSet> apps_da_;
};

void RAManagerTest::CheckResultWithHMILevelAndAccessMode(
    ResourceAllocationManagerImpl& ra_manager,
    mobile_apis::HMILevel::eType app_level,
    const AcquireResult::eType expected_result,
    const hmi_apis::Common_RCAccessMode::eType access_mode) {
  // Arrange

  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillOnce(Return(mock_app_1_));
  ra_manager.SetAccessMode(access_mode);

  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
  EXPECT_CALL(mock_app_mngr_, application(kAppId2))
      .WillOnce(Return(mock_app_2_));
  EXPECT_CALL(*mock_app_2_, hmi_level()).WillOnce(Return(app_level));
  // Second app tries to get already acquired resource by 1st app
  EXPECT_EQ(expected_result, ra_manager.AcquireResource(kModuleType1, kAppId2));
}

void RAManagerTest::OnRCStatusNotificationExpectations() {
  ON_CALL(mock_app_mngr_, application(kAppId1))
      .WillByDefault(Return(mock_app_1_));
  ON_CALL(*mock_app_1_, QueryInterface(rc_rpc_plugin::RCRPCPlugin::kRCPluginID))
      .WillByDefault(Return(app_ext_ptr_));
  apps_.insert(mock_app_1_);
  ON_CALL(mock_app_mngr_, applications()).WillByDefault(Return(apps_da_));
}

TEST_F(RAManagerTest, AcquireResource_NoAppRegistered_Expect_InUse) {
  // Arrange
  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillOnce(Return(ApplicationSharedPtr()));
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  // Act & Assert
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::IN_USE,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
}

TEST_F(RAManagerTest,
       AcquireResource_AppRegisteredAnyHmiLevelResourceFree_Expect_Allowed) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  // Act & Assert
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
}

TEST_F(
    RAManagerTest,
    AcquireResource_AppInAnyHmiLevelWantsToAcquireSameResourceTwice_Expect_Allowed) {
  // Arrange
  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillOnce(Return(mock_app_1_));
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillOnce(Return(mock_app_1_));
  // Same app tries to get already acquired resource
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
}

TEST_F(
    RAManagerTest,
    AcquireResource_App2_NotInFULLWantsToGetAcquiredResource_Expect_Rejected) {
  using namespace mobile_apis;
  using namespace hmi_apis;
  using namespace rc_rpc_plugin;
  const HMILevel::eType app_level = HMILevel::eType::HMI_BACKGROUND;
  const AcquireResult::eType expected_result = AcquireResult::REJECTED;
  const Common_RCAccessMode::eType access_mode =
      Common_RCAccessMode::eType::AUTO_ALLOW;
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  CheckResultWithHMILevelAndAccessMode(
      ra_manager, app_level, expected_result, access_mode);
}

TEST_F(
    RAManagerTest,
    AcquireResource_App2_InFULLWantsToGetAcquiredResource_AUTO_DENY_Expect_InUse) {
  using namespace mobile_apis;
  using namespace hmi_apis;
  using namespace rc_rpc_plugin;
  const HMILevel::eType app_level = HMILevel::eType::HMI_FULL;
  const AcquireResult::eType expected_result = AcquireResult::IN_USE;
  const Common_RCAccessMode::eType access_mode =
      Common_RCAccessMode::eType::AUTO_DENY;
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  CheckResultWithHMILevelAndAccessMode(
      ra_manager, app_level, expected_result, access_mode);
}

TEST_F(
    RAManagerTest,
    AcquireResource_App2_InFULLWantsToGetAcquiredResource_AUTO_ALLOW_Expect_Allowed) {
  using namespace mobile_apis;
  using namespace hmi_apis;
  using namespace rc_rpc_plugin;
  const HMILevel::eType app_level = HMILevel::eType::HMI_FULL;
  const AcquireResult::eType expected_result = AcquireResult::ALLOWED;
  const Common_RCAccessMode::eType access_mode =
      Common_RCAccessMode::eType::AUTO_ALLOW;
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  CheckResultWithHMILevelAndAccessMode(
      ra_manager, app_level, expected_result, access_mode);
}

TEST_F(
    RAManagerTest,
    AcquireResource_App2_InFULLWantsToGetAcquiredResource_ASK_DRIVER_Expect_AskDriver) {
  using namespace mobile_apis;
  using namespace hmi_apis;
  using namespace rc_rpc_plugin;
  const HMILevel::eType app_level = HMILevel::eType::HMI_FULL;
  const AcquireResult::eType expected_result = AcquireResult::ASK_DRIVER;
  const Common_RCAccessMode::eType access_mode =
      Common_RCAccessMode::eType::ASK_DRIVER;
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  CheckResultWithHMILevelAndAccessMode(
      ra_manager, app_level, expected_result, access_mode);
}

TEST_F(RAManagerTest,
       AcquireResource_AcquiredModuleIsRejectedForApp2_ExpectApp2Rejected) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));
  EXPECT_EQ(AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  // Act
  ra_manager.OnDriverDisallowed(kModuleType1, kAppId2);

  // Assert
  EXPECT_CALL(mock_app_mngr_, application(kAppId2))
      .WillOnce(Return(mock_app_2_));
  EXPECT_EQ(AcquireResult::REJECTED,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AppExit_ReleaseResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));

  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  // Act
  ra_manager.OnApplicationEvent(
      application_manager::plugin_manager::kApplicationExit, mock_app_1_);

  EXPECT_CALL(mock_app_mngr_, application(kAppId2))
      .WillRepeatedly(Return(mock_app_2_));

  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AnotherAppExit_NoReleaseResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillOnce(Return(mock_app_1_));

  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  EXPECT_CALL(mock_app_mngr_, application(kAppId2))
      .WillRepeatedly(Return(mock_app_2_));

  // Act
  application_manager::ApplicationSharedPtr app_ptr(mock_app_2_);
  EXPECT_CALL(*mock_app_2_, app_id()).WillRepeatedly(Return(kAppId2));
  ra_manager.OnApplicationEvent(
      application_manager::plugin_manager::kApplicationExit, app_ptr);

  EXPECT_CALL(mock_app_mngr_, application(kAppId2))
      .WillOnce(Return(mock_app_2_));
  EXPECT_CALL(*mock_app_2_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::IN_USE,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AppUnregistered_ReleaseResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  RCAppExtensionPtr rc_extention_ptr =
      std::make_shared<RCAppExtension>(application_manager::AppExtensionUID(
          rc_rpc_plugin::RCRPCPlugin::kRCPluginID));

  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  // Act
  application_manager::ApplicationSharedPtr app_ptr(mock_app_1_);
  EXPECT_CALL(*mock_app_1_, app_id()).WillRepeatedly(Return(kAppId1));

  ra_manager.OnApplicationEvent(
      application_manager::plugin_manager::kApplicationUnregistered, app_ptr);

  EXPECT_CALL(mock_app_mngr_, application(kAppId2))
      .WillOnce(Return(mock_app_2_));

  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AnotherAppUnregistered_NoReleaseResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillOnce(Return(mock_app_1_));
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillRepeatedly(Return(mock_app_2_));

  // Act
  application_manager::ApplicationSharedPtr app_ptr(mock_app_2_);
  EXPECT_CALL(*mock_app_2_, app_id()).WillRepeatedly(Return(kAppId2));

  ra_manager.OnApplicationEvent(
      application_manager::plugin_manager::kApplicationUnregistered, app_ptr);

  EXPECT_CALL(mock_app_mngr_, application(kAppId2))
      .WillOnce(Return(mock_app_2_));
  EXPECT_CALL(*mock_app_2_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::IN_USE,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AppsDisallowed_ReleaseAllResources) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));

  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType2, kAppId1));

  application_manager::ApplicationSet apps;
  apps.insert(mock_app_1_);
  std::shared_ptr<sync_primitives::Lock> apps_lock =
      std::make_shared<sync_primitives::Lock>();
  DataAccessor<application_manager::ApplicationSet> apps_da(apps, apps_lock);

  EXPECT_CALL(mock_app_mngr_, applications()).WillRepeatedly(Return(apps_da));

  RCAppExtensionPtr rc_extention_ptr =
      std::make_shared<RCAppExtension>(application_manager::AppExtensionUID(
          rc_rpc_plugin::RCRPCPlugin::kRCPluginID));

  EXPECT_CALL(*mock_app_1_, QueryInterface(RCRPCPlugin::kRCPluginID))
      .WillRepeatedly(Return(rc_extention_ptr));

  // Act
  ra_manager.OnPolicyEvent(
      application_manager::plugin_manager::PolicyEvent::kApplicationsDisabled);

  EXPECT_CALL(mock_app_mngr_, application(kAppId2))
      .WillRepeatedly(Return(mock_app_2_));
  EXPECT_CALL(*mock_app_2_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType2, kAppId2));
  Mock::VerifyAndClearExpectations(&mock_app_mngr_);
}

TEST_F(RAManagerTest, AppGotRevokedModulesWithPTU_ReleaseRevokedResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));

  RCAppExtensionPtr rc_extention_ptr =
      std::make_shared<rc_rpc_plugin::RCAppExtension>(
          application_manager::AppExtensionUID(
              rc_rpc_plugin::RCRPCPlugin::kRCPluginID));

  EXPECT_CALL(*mock_app_1_, QueryInterface(RCRPCPlugin::kRCPluginID))
      .WillRepeatedly(Return(rc_extention_ptr));

  ON_CALL(*mock_app_1_, is_remote_control_supported())
      .WillByDefault(Return(true));

  EXPECT_CALL(mock_app_mngr_, application(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));

  EXPECT_CALL(*mock_app_1_, app_id()).WillRepeatedly(Return(kAppId1));

  EXPECT_CALL(*mock_app_1_, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_1_));

  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType2, kAppId1));

  application_manager::ApplicationSet apps;
  apps.insert(mock_app_1_);
  std::shared_ptr<sync_primitives::Lock> apps_lock =
      std::make_shared<sync_primitives::Lock>();
  DataAccessor<application_manager::ApplicationSet> apps_da(apps, apps_lock);

  EXPECT_CALL(mock_app_mngr_, applications()).WillRepeatedly(Return(apps_da));

  Resources allowed_modules;
  allowed_modules.push_back(kModuleType1);

  EXPECT_CALL(mock_policy_handler_, GetModuleTypes(policy_app_id_1_, _))
      .WillOnce(DoAll(SetArgPointee<1>(allowed_modules), Return(true)));

  // Act
  ra_manager.OnPolicyEvent(application_manager::plugin_manager::PolicyEvent::
                               kApplicationPolicyUpdated);

  EXPECT_CALL(mock_app_mngr_, application(kAppId2))
      .WillRepeatedly(Return(mock_app_2_));
  EXPECT_CALL(*mock_app_2_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::IN_USE,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType2, kAppId2));
  Mock::VerifyAndClearExpectations(&mock_app_mngr_);
}

TEST_F(RAManagerTest, GetAccessMode_ExpectedSameAsHadSet) {
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);

  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::AUTO_DENY);
  EXPECT_EQ(hmi_apis::Common_RCAccessMode::AUTO_DENY,
            ra_manager.GetAccessMode());

  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::ASK_DRIVER);
  EXPECT_EQ(hmi_apis::Common_RCAccessMode::ASK_DRIVER,
            ra_manager.GetAccessMode());

  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::AUTO_ALLOW);
  EXPECT_EQ(hmi_apis::Common_RCAccessMode::AUTO_ALLOW,
            ra_manager.GetAccessMode());

  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::INVALID_ENUM);
  EXPECT_EQ(hmi_apis::Common_RCAccessMode::INVALID_ENUM,
            ra_manager.GetAccessMode());
}

TEST_F(RAManagerTest, OnRCStatus_AppRegistation_RC_allowed) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ON_CALL((*mock_app_1_), is_remote_control_supported())
      .WillByDefault(Return(true));
  ON_CALL((*mock_app_1_), hmi_app_id()).WillByDefault(Return(kHMIAppId1));

  application_manager::commands::MessageSharedPtr message_to_mob;
  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, false))
      .WillOnce(SaveArg<0>(&message_to_mob));
  application_manager::commands::MessageSharedPtr message_to_hmi;
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_)).Times(0);

  // Act
  ra_manager.SendOnRCStatusNotifications(NotificationTrigger::APP_REGISTRATION,
                                         mock_app_1_);

  auto msg_to_mob_params =
      (*message_to_mob)[application_manager::strings::msg_params];

  // Assert
  EXPECT_EQ(msg_to_mob_params[message_params::kAllowed].asBool(), true);
  EXPECT_EQ(
      msg_to_mob_params[message_params::kAllocatedModules].asArray()->size(),
      0u);
  EXPECT_EQ(msg_to_mob_params[message_params::kFreeModules].asArray()->size(),
            kSizeOfModules);
}

TEST_F(RAManagerTest, OnRCStatus_AppRegistation_RC_disallowed) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ra_manager.set_rc_enabled(false);

  ON_CALL((*mock_app_1_), is_remote_control_supported())
      .WillByDefault(Return(true));

  application_manager::commands::MessageSharedPtr message_to_mob;
  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, false))
      .WillOnce(SaveArg<0>(&message_to_mob));
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_)).Times(0);

  // Act
  ra_manager.SendOnRCStatusNotifications(NotificationTrigger::APP_REGISTRATION,
                                         mock_app_1_);

  auto msg_to_mob_params =
      (*message_to_mob)[application_manager::strings::msg_params];
  // Assert
  EXPECT_EQ(msg_to_mob_params[message_params::kAllowed].asBool(), false);
  EXPECT_EQ(
      msg_to_mob_params[message_params::kAllocatedModules].asArray()->size(),
      0u);
  EXPECT_EQ(msg_to_mob_params[message_params::kFreeModules].asArray()->size(),
            0u);
}

TEST_F(RAManagerTest, OnRCStatus_RCStateChanging_RC_disabling) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ON_CALL((*mock_app_1_), is_remote_control_supported())
      .WillByDefault(Return(true));
  ON_CALL((*mock_app_1_), hmi_app_id()).WillByDefault(Return(kHMIAppId1));

  application_manager::commands::MessageSharedPtr message_to_mob;
  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, false))
      .WillOnce(SaveArg<0>(&message_to_mob));
  application_manager::commands::MessageSharedPtr message_to_hmi;
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_))
      .WillOnce(SaveArg<0>(&message_to_hmi));

  // Act
  ra_manager.set_rc_enabled(false);

  auto msg_to_mob_params =
      (*message_to_mob)[application_manager::strings::msg_params];
  auto msg_to_hmi_params =
      (*message_to_hmi)[application_manager::strings::msg_params];
  // Assert
  EXPECT_EQ(msg_to_mob_params[message_params::kAllowed].asBool(), false);
  EXPECT_EQ(
      msg_to_mob_params[message_params::kAllocatedModules].asArray()->size(),
      0u);
  EXPECT_EQ(msg_to_mob_params[message_params::kFreeModules].asArray()->size(),
            0u);
  EXPECT_EQ(
      msg_to_hmi_params[message_params::kAllocatedModules].asArray()->size(),
      0u);
  EXPECT_EQ(msg_to_hmi_params[message_params::kFreeModules].asArray()->size(),
            kSizeOfModules);
  EXPECT_EQ(msg_to_hmi_params[application_manager::strings::app_id].asInt(),
            kHMIAppId1);
}

TEST_F(RAManagerTest, OnRCStatus_RCStateChanging_RC_enabling) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);
  ON_CALL((*mock_app_1_), is_remote_control_supported())
      .WillByDefault(Return(true));
  ON_CALL((*mock_app_1_), hmi_app_id()).WillByDefault(Return(kHMIAppId1));

  application_manager::commands::MessageSharedPtr message_to_mob;
  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, false))
      .WillOnce(SaveArg<0>(&message_to_mob));
  application_manager::commands::MessageSharedPtr message_to_hmi;
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_))
      .WillOnce(SaveArg<0>(&message_to_hmi));

  // Act
  ra_manager.set_rc_enabled(true);

  auto msg_to_mob_params =
      (*message_to_mob)[application_manager::strings::msg_params];
  auto msg_to_hmi_params =
      (*message_to_hmi)[application_manager::strings::msg_params];
  // Assert
  EXPECT_EQ(msg_to_mob_params[message_params::kAllowed].asBool(), true);
  EXPECT_EQ(
      msg_to_mob_params[message_params::kAllocatedModules].asArray()->size(),
      0u);
  EXPECT_EQ(msg_to_mob_params[message_params::kFreeModules].asArray()->size(),
            kSizeOfModules);
  EXPECT_EQ(
      msg_to_hmi_params[message_params::kAllocatedModules].asArray()->size(),
      0u);
  EXPECT_EQ(msg_to_hmi_params[message_params::kFreeModules].asArray()->size(),
            kSizeOfModules);
  EXPECT_EQ(msg_to_hmi_params[application_manager::strings::app_id].asInt(),
            kHMIAppId1);
}

TEST_F(RAManagerTest, OnRCStatus_ModuleAllocation) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_app_mngr_, mock_rpc_service_);

  ON_CALL((*mock_app_1_), is_remote_control_supported())
      .WillByDefault(Return(true));
  ON_CALL((*mock_app_1_), hmi_app_id()).WillByDefault(Return(kHMIAppId1));

  EXPECT_EQ(rc_rpc_plugin::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  application_manager::commands::MessageSharedPtr message_to_mob;
  EXPECT_CALL(mock_rpc_service_, SendMessageToMobile(_, false))
      .WillOnce(SaveArg<0>(&message_to_mob));

  application_manager::commands::MessageSharedPtr message_to_hmi;
  EXPECT_CALL(mock_rpc_service_, SendMessageToHMI(_))
      .WillOnce(SaveArg<0>(&message_to_hmi));

  // Act
  ra_manager.SendOnRCStatusNotifications(
      NotificationTrigger::MODULE_ALLOCATION,
      std::shared_ptr<application_manager::Application>());

  auto msg_to_mob_params =
      (*message_to_mob)[application_manager::strings::msg_params];
  auto msg_to_hmi_params =
      (*message_to_hmi)[application_manager::strings::msg_params];
  // Assert
  EXPECT_EQ(msg_to_mob_params[message_params::kAllowed].asBool(), true);
  EXPECT_EQ(
      msg_to_mob_params[message_params::kAllocatedModules].asArray()->size(),
      1u);
  EXPECT_EQ(msg_to_mob_params[message_params::kFreeModules].asArray()->size(),
            kSizeOfModules - 1u);
  EXPECT_EQ(
      msg_to_hmi_params[message_params::kAllocatedModules].asArray()->size(),
      1u);
  EXPECT_EQ(msg_to_hmi_params[message_params::kFreeModules].asArray()->size(),
            kSizeOfModules - 1u);
  EXPECT_EQ(msg_to_hmi_params[application_manager::strings::app_id].asInt(),
            kHMIAppId1);
}

}  // namespace rc_rpc_plugin

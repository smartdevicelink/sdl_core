/*
 * Copyright (c) 2017, Ford Motor Company
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

#include "gtest/gtest.h"
#include <algorithm>
#include "remote_control/resource_allocation_manager_impl.h"
#include "mock_resource_allocation_manager.h"
#include "mock_remote_control_plugin.h"
#include "remote_control/rc_app_extension.h"
#include "remote_control/rc_module_constants.h"
#include "remote_control/message_helper.h"
#include "functional_module/module_observer.h"
#include "application_manager/mock_application.h"
#include "include/mock_service.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

using functional_modules::PluginInfo;
using functional_modules::ProcessResult;
using functional_modules::RCFunctionID;
using application_manager::ServicePtr;

using application_manager::MockService;
using namespace test::components;
using application_manager_test::MockApplication;

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
const uint32_t kAppId2 = 22u;
const std::string policy_app_id_1_ = "policy_id_1";
const functional_modules::ModuleID kDefaultModuleID = 0;
}

namespace remote_control {

class RAManagerTest : public ::testing::Test {
 public:
  RAManagerTest()
      : mock_service_(utils::MakeShared<NiceMock<MockService> >())
      , mock_app_1_(utils::MakeShared<NiceMock<MockApplication> >())
      , mock_app_2_(utils::MakeShared<NiceMock<MockApplication> >())
      , message_(utils::MakeShared<Message>(MessagePriority::FromServiceType(
            protocol_handler::ServiceType::kRpc))) {
    EXPECT_CALL(mock_module_, service()).WillRepeatedly(Return(mock_service_));
    EXPECT_CALL(mock_module_, GetModuleID())
        .WillRepeatedly(Return(kDefaultModuleID));
  }

  void CheckResultWithHMILevelAndAccessMode(
      ResourceAllocationManagerImpl& ra_manager,
      mobile_apis::HMILevel::eType app_level,
      const remote_control::AcquireResult::eType expected_result,
      const hmi_apis::Common_RCAccessMode::eType access_mode) {
    // Arrange
    EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
        .WillOnce(Return(mock_app_1_));
    ra_manager.SetAccessMode(access_mode);
    EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
              ra_manager.AcquireResource(kModuleType1, kAppId1));
    EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
        .WillOnce(Return(mock_app_2_));
    EXPECT_CALL(*mock_app_2_, hmi_level()).WillOnce(Return(app_level));
    // Second app tries to get already acquired resource by 1st app
    EXPECT_EQ(expected_result,
              ra_manager.AcquireResource(kModuleType1, kAppId2));
  }

 protected:
  utils::SharedPtr<NiceMock<MockService> > mock_service_;
  utils::SharedPtr<NiceMock<MockApplication> > mock_app_1_;
  utils::SharedPtr<NiceMock<MockApplication> > mock_app_2_;
  application_manager::MessagePtr message_;
  remote_control_test::MockRemotePluginInterface mock_module_;
  RemotePluginInterface::RCPluginEventDispatcher event_dispatcher_;
};

TEST_F(RAManagerTest, AcquireResource_NoAppRegistered_Expect_InUse) {
  // Arrange
  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillOnce(Return(ApplicationSharedPtr()));
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  // Act & Assert
  EXPECT_EQ(remote_control::AcquireResult::IN_USE,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
}

TEST_F(RAManagerTest,
       AcquireResource_AppRegisteredAnyHmiLevelResourceFree_Expect_Allowed) {
  // Arrange
  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillOnce(Return(mock_app_1_));
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  // Act & Assert
  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
}

TEST_F(
    RAManagerTest,
    AcquireResource_AppInAnyHmiLevelWantsToAcquireSameResourceTwice_Expect_Allowed) {
  // Arrange
  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillOnce(Return(mock_app_1_));
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillOnce(Return(mock_app_1_));
  // Same app tries to get already acquired resource
  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
}

TEST_F(
    RAManagerTest,
    AcquireResource_App2_NotInFULLWantsToGetAcquiredResource_Expect_Rejected) {
  using namespace mobile_apis;
  using namespace hmi_apis;
  using namespace remote_control;
  const HMILevel::eType app_level = HMILevel::eType::HMI_BACKGROUND;
  const AcquireResult::eType expected_result = AcquireResult::REJECTED;
  const Common_RCAccessMode::eType access_mode =
      Common_RCAccessMode::eType::AUTO_ALLOW;
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  CheckResultWithHMILevelAndAccessMode(
      ra_manager, app_level, expected_result, access_mode);
}

TEST_F(
    RAManagerTest,
    AcquireResource_App2_InFULLWantsToGetAcquiredResource_AUTO_DENY_Expect_InUse) {
  using namespace mobile_apis;
  using namespace hmi_apis;
  using namespace remote_control;
  const HMILevel::eType app_level = HMILevel::eType::HMI_FULL;
  const AcquireResult::eType expected_result = AcquireResult::IN_USE;
  const Common_RCAccessMode::eType access_mode =
      Common_RCAccessMode::eType::AUTO_DENY;
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  CheckResultWithHMILevelAndAccessMode(
      ra_manager, app_level, expected_result, access_mode);
}

TEST_F(
    RAManagerTest,
    AcquireResource_App2_InFULLWantsToGetAcquiredResource_AUTO_ALLOW_Expect_Allowed) {
  using namespace mobile_apis;
  using namespace hmi_apis;
  using namespace remote_control;
  const HMILevel::eType app_level = HMILevel::eType::HMI_FULL;
  const AcquireResult::eType expected_result = AcquireResult::ALLOWED;
  const Common_RCAccessMode::eType access_mode =
      Common_RCAccessMode::eType::AUTO_ALLOW;
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  CheckResultWithHMILevelAndAccessMode(
      ra_manager, app_level, expected_result, access_mode);
}

TEST_F(
    RAManagerTest,
    AcquireResource_App2_InFULLWantsToGetAcquiredResource_ASK_DRIVER_Expect_AskDriver) {
  using namespace mobile_apis;
  using namespace hmi_apis;
  using namespace remote_control;
  const HMILevel::eType app_level = HMILevel::eType::HMI_FULL;
  const AcquireResult::eType expected_result = AcquireResult::ASK_DRIVER;
  const Common_RCAccessMode::eType access_mode =
      Common_RCAccessMode::eType::ASK_DRIVER;
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  CheckResultWithHMILevelAndAccessMode(
      ra_manager, app_level, expected_result, access_mode);
}

TEST_F(RAManagerTest,
       AcquireResource_AcquiredModuleIsRejectedForApp2_ExpectApp2Rejected) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillOnce(Return(mock_app_1_));
  EXPECT_EQ(AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  // Act
  ra_manager.OnDriverDisallowed(kModuleType1, kAppId2);

  // Assert
  EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
      .WillOnce(Return(mock_app_2_));
  EXPECT_EQ(AcquireResult::REJECTED,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AppExit_ReleaseResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));

  RCAppExtensionPtr rc_extention_ptr = utils::MakeShared<RCAppExtension>(
      application_manager::AppExtensionUID(kDefaultModuleID));

  EXPECT_CALL(*mock_app_1_, QueryInterface(_))
      .WillOnce(Return(rc_extention_ptr));

  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  // Act
  application_manager::ApplicationSharedPtr app_ptr(mock_app_1_);
  EXPECT_CALL(*mock_app_1_, app_id()).WillRepeatedly(Return(kAppId1));

  ra_manager.OnApplicationEvent(
      functional_modules::ApplicationEvent::kApplicationExit, app_ptr);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
      .WillRepeatedly(Return(mock_app_2_));

  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AnotherAppExit_NoReleaseResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillOnce(Return(mock_app_1_));

  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
      .WillRepeatedly(Return(mock_app_2_));

  RCAppExtensionPtr rc_extention_ptr = utils::MakeShared<RCAppExtension>(
      application_manager::AppExtensionUID(kDefaultModuleID));

  EXPECT_CALL(*mock_app_2_, QueryInterface(_))
      .WillOnce(Return(rc_extention_ptr));

  // Act
  application_manager::ApplicationSharedPtr app_ptr(mock_app_2_);
  EXPECT_CALL(*mock_app_2_, app_id()).WillRepeatedly(Return(kAppId2));
  ra_manager.OnApplicationEvent(
      functional_modules::ApplicationEvent::kApplicationExit, app_ptr);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
      .WillOnce(Return(mock_app_2_));
  EXPECT_CALL(*mock_app_2_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_EQ(remote_control::AcquireResult::IN_USE,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AppUnregistered_ReleaseResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  RCAppExtensionPtr rc_extention_ptr = utils::MakeShared<RCAppExtension>(
      application_manager::AppExtensionUID(kDefaultModuleID));

  EXPECT_CALL(*mock_app_1_, QueryInterface(_))
      .WillOnce(Return(rc_extention_ptr));

  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));

  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  // Act
  application_manager::ApplicationSharedPtr app_ptr(mock_app_1_);
  EXPECT_CALL(*mock_app_1_, app_id()).WillRepeatedly(Return(kAppId1));

  ra_manager.OnApplicationEvent(
      functional_modules::ApplicationEvent::kApplicationUnregistered, app_ptr);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
      .WillOnce(Return(mock_app_2_));

  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AnotherAppUnregistered_NoReleaseResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillOnce(Return(mock_app_1_));
  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));

  EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
      .WillRepeatedly(Return(mock_app_2_));

  RCAppExtensionPtr rc_extention_ptr = utils::MakeShared<RCAppExtension>(
      application_manager::AppExtensionUID(kDefaultModuleID));

  EXPECT_CALL(*mock_app_2_, QueryInterface(_))
      .WillOnce(Return(rc_extention_ptr));

  // Act
  application_manager::ApplicationSharedPtr app_ptr(mock_app_2_);
  EXPECT_CALL(*mock_app_2_, app_id()).WillRepeatedly(Return(kAppId2));

  ra_manager.OnApplicationEvent(
      functional_modules::ApplicationEvent::kApplicationUnregistered, app_ptr);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
      .WillOnce(Return(mock_app_2_));
  EXPECT_CALL(*mock_app_2_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_EQ(remote_control::AcquireResult::IN_USE,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
}

TEST_F(RAManagerTest, AppsDisallowed_ReleaseAllResources) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));

  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType2, kAppId1));

  std::vector<ApplicationSharedPtr> apps;
  apps.push_back(mock_app_1_);
  EXPECT_CALL(*mock_service_, GetApplications(_)).WillRepeatedly(Return(apps));

  RCAppExtensionPtr rc_extention_ptr = utils::MakeShared<RCAppExtension>(
      application_manager::AppExtensionUID(kDefaultModuleID));

  EXPECT_CALL(*mock_app_1_, QueryInterface(_))
      .WillOnce(Return(rc_extention_ptr));

  // Act
  ra_manager.OnPolicyEvent(
      functional_modules::PolicyEvent::kApplicationsDisabled);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
      .WillRepeatedly(Return(mock_app_2_));
  EXPECT_CALL(*mock_app_2_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType2, kAppId2));
}

TEST_F(RAManagerTest, AppGotRevokedModulesWithPTU_ReleaseRevokedResource) {
  // Arrange
  ResourceAllocationManagerImpl ra_manager(mock_module_);
  ra_manager.SetAccessMode(hmi_apis::Common_RCAccessMode::eType::AUTO_DENY);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));

  RCAppExtensionPtr rc_extention_ptr = utils::MakeShared<RCAppExtension>(
      application_manager::AppExtensionUID(kDefaultModuleID));

  EXPECT_CALL(*mock_app_1_, QueryInterface(_))
      .WillOnce(Return(rc_extention_ptr));

  EXPECT_CALL(*mock_service_, GetApplication(kAppId1))
      .WillRepeatedly(Return(mock_app_1_));

  EXPECT_CALL(*mock_app_1_, app_id()).WillRepeatedly(Return(kAppId1));

  EXPECT_CALL(*mock_app_1_, policy_app_id())
      .WillRepeatedly(Return(policy_app_id_1_));

  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType1, kAppId1));
  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType2, kAppId1));

  std::vector<application_manager::ApplicationSharedPtr> apps;
  apps.push_back(mock_app_1_);
  EXPECT_CALL(*mock_service_, GetApplications(_)).WillOnce(Return(apps));

  Resources allowed_modules;
  allowed_modules.push_back(kModuleType1);

  EXPECT_CALL(*mock_service_, GetModuleTypes(policy_app_id_1_, _))
      .WillOnce(DoAll(SetArgPointee<1>(allowed_modules), Return(true)));

  // Act
  ra_manager.OnPolicyEvent(
      functional_modules::PolicyEvent::kApplicationPolicyUpdated);

  EXPECT_CALL(*mock_service_, GetApplication(kAppId2))
      .WillRepeatedly(Return(mock_app_2_));
  EXPECT_CALL(*mock_app_2_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_EQ(remote_control::AcquireResult::IN_USE,
            ra_manager.AcquireResource(kModuleType1, kAppId2));
  EXPECT_EQ(remote_control::AcquireResult::ALLOWED,
            ra_manager.AcquireResource(kModuleType2, kAppId2));
}

TEST_F(RAManagerTest, GetAccessMode_ExpectedSameAsHadSet) {
  ResourceAllocationManagerImpl ra_manager(mock_module_);

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

}  // namespace remote_control

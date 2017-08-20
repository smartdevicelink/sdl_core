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
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include <string>
#include <vector>
#include <fstream>
#include "gmock/gmock.h"

#include "application_manager/policies/policy_handler.h"
#include "application_manager/policies/delegates/app_permission_delegate.h"
#include "policy/mock_cache_manager.h"
#include "application_manager/mock_message_helper.h"
#include "connection_handler/mock_connection_handler_settings.h"
#include "policy/policy_types.h"
#include "policy/access_remote.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/value.h"
#include "smart_objects/smart_object.h"
#include "utils/make_shared.h"
#include "utils/custom_string.h"
#include "interfaces/MOBILE_API.h"
#include "policy/mock_policy_settings.h"
#include "application_manager/mock_application.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "protocol_handler/mock_session_observer.h"
#include "connection_handler/mock_connection_handler.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/policies/mock_policy_handler_observer.h"
#include "application_manager/mock_event_dispatcher.h"
#include "policy/mock_policy_manager.h"

namespace test {
namespace components {
namespace rc_policy_handler_test {

using namespace application_manager;
using namespace policy;
using namespace utils::custom_string;
using testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::DoAll;
using ::testing::SetArgPointee;

class RCPolicyHandlerTest : public ::testing::Test {
 public:
  RCPolicyHandlerTest()
      : policy_handler_(policy_settings_, app_manager_)
      , kPolicyAppId_("fake_app_id")
      , kMacAddr_("kMacAddr_ess")
      , kDeviceId_("fake_device_id")
      , kHmiLevel_("NONE")
      , default_hmi_("fake_hmi")
      , app_set(test_app, app_lock)
      , kAppId1_(10u)
      , kAppId2_(11u)
      , kConnectionKey_(1u)
      , kCorrelationKey_(2u)
      , kUrl_("test_url")
      , mock_message_helper_(*MockMessageHelper::message_helper_mock()) {}

 protected:
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;
  utils::SharedPtr<application_manager_test::MockApplication> mock_app_;
  connection_handler_test::MockConnectionHandler conn_handler;
  protocol_handler_test::MockSessionObserver mock_session_observer;
  components::usage_statistics_test::MockStatisticsManager
      mock_statistics_manager_;
  PolicyHandler policy_handler_;
  utils::SharedPtr<policy_manager_test::MockPolicyManager> mock_policy_manager_;
  application_manager_test::MockApplicationManager app_manager_;
  const std::string kPolicyAppId_;
  const std::string kMacAddr_;
  const std::string kDeviceId_;
  const std::string kHmiLevel_;
  std::string default_hmi_;
  ApplicationSet test_app;
  sync_primitives::Lock app_lock;
  DataAccessor<ApplicationSet> app_set;
  const uint32_t kAppId1_;
  const uint32_t kAppId2_;
  const uint32_t kConnectionKey_;
  const uint32_t kCorrelationKey_;
  const std::string kUrl_;
  application_manager::MockMessageHelper& mock_message_helper_;

  virtual void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    ON_CALL(app_manager_, applications()).WillByDefault(Return(app_set));
    ON_CALL(policy_settings_, enable_policy()).WillByDefault(Return(true));
    mock_policy_manager_ =
        utils::MakeShared<policy_manager_test::MockPolicyManager>();
    ASSERT_TRUE(mock_policy_manager_.valid());

    ON_CALL(app_manager_, connection_handler())
        .WillByDefault(ReturnRef(conn_handler));
    ON_CALL(conn_handler, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_observer));

    mock_app_ = utils::MakeShared<application_manager_test::MockApplication>();
  }

  virtual void TearDown() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  void ChangePolicyManagerToMock() {
    policy_handler_.SetPolicyManager(mock_policy_manager_);
  }

  void EnablePolicy() {
    ON_CALL(policy_settings_, enable_policy()).WillByDefault(Return(true));
  }

  void EnablePolicyAndPolicyManagerMock() {
    EnablePolicy();
    ChangePolicyManagerToMock();
  }
};

TEST_F(RCPolicyHandlerTest,
       SendMessageToSDK_RemoteControlInvalidMobileAppId_UNSUCCESS) {
  // Precondition
  BinaryMessage msg;
  EnablePolicyAndPolicyManagerMock();

  EXPECT_CALL(app_manager_, applications()).WillOnce(Return(app_set));
  test_app.insert(mock_app_);

  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId1_));
  ON_CALL(*mock_app_, hmi_level())
      .WillByDefault(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_CALL(*mock_app_, IsRegistered()).WillOnce(Return(true));

  EXPECT_CALL(app_manager_, application(kAppId1_))
      .WillRepeatedly(Return(mock_app_));
  const std::string empty_mobile_app_id("");
  EXPECT_CALL(*mock_app_, policy_app_id())
      .WillOnce(Return(empty_mobile_app_id));

  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(_))
      .WillOnce(Return(kDeviceAllowed));

  EXPECT_CALL(mock_message_helper_, SendPolicySnapshotNotification(_, _, _, _))
      .Times(0);
  EXPECT_FALSE(policy_handler_.SendMessageToSDK(msg, kUrl_));
}

TEST_F(RCPolicyHandlerTest, SendMessageToSDK_RemoteControl_SUCCESS) {
  BinaryMessage msg;
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(app_manager_, applications()).WillOnce(Return(app_set));
  test_app.insert(mock_app_);

  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId1_));
  ON_CALL(*mock_app_, hmi_level())
      .WillByDefault(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_CALL(*mock_app_, IsRegistered()).WillOnce(Return(true));

  EXPECT_CALL(app_manager_, application(kAppId1_))
      .WillRepeatedly(Return(mock_app_));

  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));

  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(_))
      .WillOnce(Return(kDeviceAllowed));

  EXPECT_CALL(mock_message_helper_,
              SendPolicySnapshotNotification(kAppId1_, _, kUrl_, _));
  EXPECT_TRUE(policy_handler_.SendMessageToSDK(msg, kUrl_));
}

TEST_F(RCPolicyHandlerTest, OnUpdateHMILevel_InvalidApp_UNSUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  utils::SharedPtr<application_manager_test::MockApplication> invalid_app;
  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(_)).Times(0);

  const std::string hmi_level("HMI_FULL");
  policy_handler_.OnUpdateHMILevel(kDeviceId_, kPolicyAppId_, hmi_level);
}

TEST_F(RCPolicyHandlerTest, OnUpdateHMILevel_InvalidHmiLevel_UNSUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(mock_app_));

  const std::string hmi_level("INVALID_ENUM");
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(hmi_level))
      .WillOnce(Return(mobile_apis::HMILevel::INVALID_ENUM));

  policy_handler_.OnUpdateHMILevel(kDeviceId_, kPolicyAppId_, hmi_level);
}

TEST_F(RCPolicyHandlerTest, OnUpdateHMILevel_HmiLevelFull_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(mock_app_));

  const std::string hmi_level("HMI_FULL");
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(hmi_level))
      .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));

  EXPECT_CALL(*mock_app_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  EXPECT_CALL(mock_message_helper_, SendActivateAppToHMI(kAppId1_, _, _, _));

  policy_handler_.OnUpdateHMILevel(kDeviceId_, kPolicyAppId_, hmi_level);
}

TEST_F(RCPolicyHandlerTest, OnUpdateHMILevel_HmiLevelChanged_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(mock_app_));

  const std::string hmi_level("HMI_LIMITED");
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(hmi_level))
      .WillOnce(Return(mobile_apis::HMILevel::HMI_LIMITED));

  EXPECT_CALL(*mock_app_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));

  EXPECT_CALL(app_manager_,
              ChangeAppsHMILevel(kAppId1_, mobile_apis::HMILevel::HMI_LIMITED));

  EXPECT_CALL(mock_message_helper_, SendHMIStatusNotification(_, _));

  policy_handler_.OnUpdateHMILevel(kDeviceId_, kPolicyAppId_, hmi_level);
}

TEST_F(RCPolicyHandlerTest, CheckAccess_ValidParams_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  const PTString module("module");
  const PTString pt_rpc("rpc");
  const std::vector<PTString> params;

  EXPECT_CALL(*mock_policy_manager_,
              CheckAccess(kDeviceId_, kPolicyAppId_, module))
      .WillOnce(Return(policy::TypeAccess::kDisallowed));
  EXPECT_EQ(application_manager::TypeAccess::kDisallowed,
            policy_handler_.CheckAccess(kDeviceId_, kPolicyAppId_, module));

  EXPECT_CALL(*mock_policy_manager_,
              CheckAccess(kDeviceId_, kPolicyAppId_, module))
      .WillOnce(Return(policy::TypeAccess::kAllowed));

  EXPECT_EQ(application_manager::TypeAccess::kAllowed,
            policy_handler_.CheckAccess(kDeviceId_, kPolicyAppId_, module));
}

TEST_F(RCPolicyHandlerTest, SetAccess_ValidParams_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  const PTString module("module");
  const bool allowed(true);
  EXPECT_CALL(*mock_policy_manager_,
              SetAccess(kDeviceId_, kPolicyAppId_, module, allowed));

  policy_handler_.SetAccess(kDeviceId_, kPolicyAppId_, module, allowed);
}

TEST_F(RCPolicyHandlerTest, ResetAccess_ValidParams_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  EXPECT_CALL(*mock_policy_manager_, ResetAccess(kDeviceId_, kPolicyAppId_));

  policy_handler_.ResetAccess(kDeviceId_, kPolicyAppId_);
}

TEST_F(RCPolicyHandlerTest, ResetAccess_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  const PTString module("module");
  EXPECT_CALL(*mock_policy_manager_, ResetAccess(module));

  policy_handler_.ResetAccess(module);
}

TEST_F(RCPolicyHandlerTest, CheckModule_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  const PTString module("module");
  EXPECT_CALL(*mock_policy_manager_, CheckModule(kPolicyAppId_, module))
      .WillOnce(Return(true));

  EXPECT_TRUE(policy_handler_.CheckModule(kPolicyAppId_, module));
}

ACTION_P(SetDeviceHandle, handle) {
  *arg1 = handle;
}

TEST_F(RCPolicyHandlerTest, GetRemoteControl_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(*mock_policy_manager_, GetRemoteControl()).WillOnce(Return(true));

  EXPECT_TRUE(policy_handler_.GetRemoteControl());
}

TEST_F(RCPolicyHandlerTest, SetRemoteControl_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  const bool enabled(true);
  EXPECT_CALL(*mock_policy_manager_, SetRemoteControl(enabled));

  policy_handler_.SetRemoteControl(enabled);
}

TEST_F(RCPolicyHandlerTest,
       OnRemoteAppPermissionsChanged_DifferentDeviceHandle_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(*mock_policy_manager_,
              SendAppPermissionsChanged(kDeviceId_, kPolicyAppId_));

  policy_handler_.OnRemoteAppPermissionsChanged(kDeviceId_, kPolicyAppId_);
}

TEST_F(RCPolicyHandlerTest, OnUpdateHMIStatus_InvalidApp_UNSUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  utils::SharedPtr<application_manager_test::MockApplication> invalid_app;
  EXPECT_CALL(app_manager_, application(_, _)).WillOnce(Return(invalid_app));
  EXPECT_CALL(app_manager_, ChangeAppsHMILevel(_, _)).Times(0);

  const std::string hmi_level("HMI_NONE");
  policy_handler_.OnUpdateHMIStatus(kDeviceId_, kPolicyAppId_, hmi_level);
}

TEST_F(RCPolicyHandlerTest, OnUpdateHMIStatus_HmiLevelInvalidEnum_UNSUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(mock_app_));

  const std::string hmi_level("INVALID_ENUM");
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(hmi_level))
      .WillOnce(Return(mobile_apis::HMILevel::INVALID_ENUM));

  EXPECT_CALL(app_manager_, ChangeAppsHMILevel(_, _)).Times(0);

  policy_handler_.OnUpdateHMIStatus(kDeviceId_, kPolicyAppId_, hmi_level);
}

TEST_F(RCPolicyHandlerTest, OnUpdateHMIStatus_ValidAppAndHmiLevel_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(mock_app_));

  const std::string hmi_level("HMI_NONE");
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(hmi_level))
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));

  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));

  EXPECT_CALL(app_manager_,
              ChangeAppsHMILevel(kAppId1_, mobile_apis::HMILevel::HMI_NONE));
  EXPECT_CALL(mock_message_helper_, SendHMIStatusNotification(_, _));

  policy_handler_.OnUpdateHMIStatus(kDeviceId_, kPolicyAppId_, hmi_level);
}

TEST_F(RCPolicyHandlerTest,
       OnUpdateHMIStatusFourParams_HmiLevelInvalidEnum_UNSUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(mock_app_));

  const std::string hmi_level("INVALID_ENUM");
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(hmi_level))
      .WillOnce(Return(mobile_apis::HMILevel::INVALID_ENUM));

  EXPECT_CALL(app_manager_, ChangeAppsHMILevel(_, _)).Times(0);
  policy_handler_.OnUpdateHMIStatus(kDeviceId_, kPolicyAppId_, hmi_level);
}

TEST_F(RCPolicyHandlerTest, OnUpdateHMIStatus_ValidParams_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(mock_app_));

  const std::string hmi_level("HMI_NONE");
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(hmi_level))
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));

  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));
  EXPECT_CALL(app_manager_,
              ChangeAppsHMILevel(kAppId1_, mobile_apis::HMILevel::HMI_NONE));
  EXPECT_CALL(mock_message_helper_, SendHMIStatusNotification(_, _));

  policy_handler_.OnUpdateHMIStatus(kDeviceId_, kPolicyAppId_, hmi_level);
}

TEST_F(RCPolicyHandlerTest, GetModuleTypes_GetModuleTypes_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  std::vector<std::string> modules;

  EXPECT_CALL(*mock_policy_manager_, GetModuleTypes(kPolicyAppId_, &modules))
      .WillOnce(Return(true));

  EXPECT_TRUE(policy_handler_.GetModuleTypes(kPolicyAppId_, &modules));
}

TEST_F(RCPolicyHandlerTest, CheckHMIType_ValidTypes_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  mobile_apis::AppHMIType::eType hmi = mobile_apis::AppHMIType::MEDIA;

  const smart_objects::SmartObjectSPtr app_types =
      utils::MakeShared<smart_objects::SmartObject>(
          smart_objects::SmartType_Array);
  (*app_types)[strings::app_hmi_type][0] = mobile_apis::AppHMIType::MEDIA;
  (*app_types)[strings::app_hmi_type][1] =
      mobile_apis::AppHMIType::BACKGROUND_PROCESS;

  std::vector<int> policy_hmi_types;
  policy_hmi_types.push_back(mobile_apis::AppHMIType::MEDIA);
  policy_hmi_types.push_back(mobile_apis::AppHMIType::BACKGROUND_PROCESS);

  EXPECT_CALL(*mock_policy_manager_, GetHMITypes(kPolicyAppId_, _))
      .WillOnce(DoAll(SetArgPointee<1>(policy_hmi_types), Return(true)));

  EXPECT_TRUE(policy_handler_.CheckHMIType(
      kPolicyAppId_, hmi, &(*app_types.get())[strings::app_hmi_type]));
}

}  // namespace rc_policy_handler_test
}  // namespace components
}  // namespace test

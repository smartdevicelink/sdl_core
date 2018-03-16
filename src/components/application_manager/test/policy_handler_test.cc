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

#include <string>
#include <vector>
#include <fstream>
#include "gmock/gmock.h"

#include "application_manager/policies/policy_handler.h"
#include "application_manager/policies/delegates/app_permission_delegate.h"
#include "connection_handler/connection_handler_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "security_manager/mock_security_manager.h"
#include "security_manager/mock_crypto_manager.h"
#include "application_manager/mock_message_helper.h"
#include "connection_handler/mock_connection_handler_settings.h"
#include "transport_manager/mock_transport_manager.h"
#include "policy/policy_types.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/value.h"
#include "smart_objects/smart_object.h"
#include "utils/file_system.h"
#include "utils/make_shared.h"
#include "utils/custom_string.h"
#include "policy/usage_statistics/counter.h"
#include "policy/usage_statistics/statistics_manager.h"
#include "interfaces/MOBILE_API.h"
#include "policy/mock_policy_settings.h"
#include "utils/make_shared.h"
#include "application_manager/mock_application.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "protocol_handler/mock_session_observer.h"
#include "connection_handler/mock_connection_handler.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/policies/mock_policy_handler_observer.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_state_controller.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "policy/mock_policy_manager.h"
#include "policy/usage_statistics/mock_statistics_manager.h"

namespace test {
namespace components {
namespace policy_handler_test {

using namespace application_manager;
using namespace policy;
using namespace utils::custom_string;
using testing::_;
using ::testing::Mock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::NiceMock;
using ::testing::SetArgReferee;
using ::testing::SetArgPointee;
using ::testing::DoAll;
using ::testing::SetArgReferee;
using ::testing::Mock;

const std::string kDummyData = "some_data";

class PolicyHandlerTest : public ::testing::Test {
 public:
  PolicyHandlerTest()
      : policy_handler_(policy_settings_, app_manager_)
      , kPolicyAppId_("fake_app_id")
      , kMacAddr_("kMacAddr_ess")
      , kDeviceId_("fake_device_id")
      , kHmiLevel_("NONE")
      , kRpc_("fake_rpc")
      , priority_("fake_priority")
      , default_hmi_("fake_hmi")
      , kPreloadPTFile_("sdl_preloaded_pt.json")
      , kAppStorageFolder_("storage")
      , app_set(test_app, app_lock)
      , kAppId1_(10u)
      , kAppId2_(11u)
      , kConnectionKey_(1u)
      , kCorrelationKey_(2u)
      , kSnapshotFile_("snapshot")
      , kSnapshotStorage_("snapshot_storage")
      , kGroupAliasAllowed_("allowed")
      , kGroupAliasDisallowed_("disallowed")
      , kGroupNameAllowed_("name_allowed")
      , kGroupNameDisallowed_("name_disallowed")
      , kCallsCount_(1u)
      , kTimeout_(1000u)
      , mock_message_helper_(*MockMessageHelper::message_helper_mock()) {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
  }

  ~PolicyHandlerTest() {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    Mock::VerifyAndClearExpectations(&app_manager_);
  }

 protected:
  NiceMock<MockPolicySettings> policy_settings_;
  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;
  utils::SharedPtr<application_manager_test::MockApplication> mock_app_;
  connection_handler_test::MockConnectionHandler conn_handler;
  protocol_handler_test::MockSessionObserver mock_session_observer;
  application_manager_test::MockStateController mock_state_controller;
  components::usage_statistics_test::MockStatisticsManager
      mock_statistics_manager_;
  PolicyHandler policy_handler_;
  utils::SharedPtr<policy_manager_test::MockPolicyManager> mock_policy_manager_;
  application_manager_test::MockApplicationManager app_manager_;
  const std::string kPolicyAppId_;
  const std::string kMacAddr_;
  const std::string kDeviceId_;
  const std::string kHmiLevel_;
  const std::string kRpc_;
  std::string priority_;
  std::string default_hmi_;
  const std::string kPreloadPTFile_;
  const std::string kAppStorageFolder_;
  ApplicationSet test_app;
  sync_primitives::Lock app_lock;
  DataAccessor<ApplicationSet> app_set;
  const uint32_t kAppId1_;
  const uint32_t kAppId2_;
  const uint32_t kConnectionKey_;
  const uint32_t kCorrelationKey_;
  const std::string kSnapshotFile_;
  const std::string kSnapshotStorage_;
  const std::string kGroupAliasAllowed_;
  const std::string kGroupAliasDisallowed_;
  const std::string kGroupNameAllowed_;
  const std::string kGroupNameDisallowed_;
  const uint32_t kCallsCount_;
  const uint32_t kTimeout_;
  application_manager::MockMessageHelper& mock_message_helper_;

  virtual void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    ON_CALL(app_manager_, applications()).WillByDefault(Return(app_set));
    ON_CALL(policy_settings_, enable_policy()).WillByDefault(Return(true));
    ON_CALL(app_manager_, event_dispatcher())
        .WillByDefault(ReturnRef(mock_event_dispatcher_));
    std::string path = file_system::CreateDirectory("storage");
    file_system::CreateFile(path + "/" + "certificate");
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
    ON_CALL(mock_event_dispatcher_, remove_observer(_, _));
  }

  void ChangePolicyManagerToMock() {
    policy_handler_.SetPolicyManager(mock_policy_manager_);
  }

  void EnablePolicy() {
    ON_CALL(policy_settings_, enable_policy()).WillByDefault(Return(true));
    ON_CALL(policy_settings_, preloaded_pt_file())
        .WillByDefault(ReturnRef(kPreloadPTFile_));
    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder_));
  }

  void EnablePolicyAndPolicyManagerMock() {
    EnablePolicy();
    ChangePolicyManagerToMock();
  }

  void TestActivateApp(const uint32_t connection_key,
                       const uint32_t correlation_id);
  void GetAppIDForSending();
  void OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType hmi_level);

  void ExtendedPolicyExpectations() {
    const std::vector<int> retry_sequence_delay_seconds;

    EXPECT_CALL(policy_settings_, policies_snapshot_file_name())
        .WillOnce(ReturnRef(kSnapshotFile_));
    EXPECT_CALL(policy_settings_, system_files_path())
        .WillOnce(ReturnRef(kSnapshotStorage_));
#ifdef PROPRIETARY_MODE
    EXPECT_CALL(*mock_policy_manager_, TimeoutExchangeMSec())
        .WillOnce(Return(1));
    EXPECT_CALL(*mock_policy_manager_, RetrySequenceDelaysSeconds())
        .WillOnce(Return(retry_sequence_delay_seconds));
#endif  // PROPRIETARY_MODE
    EXPECT_CALL(mock_message_helper_, SendPolicyUpdate(_, _, _, _));
  }

  void TestOnPermissionsUpdated(const std::string& default_hmi_level,
                                const mobile_apis::HMILevel::eType hmi_level) {
    EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
        .WillRepeatedly(Return(mock_app_));
    EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));
    EXPECT_CALL(*mock_app_, hmi_level())
        .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));

    EXPECT_CALL(mock_message_helper_, StringToHMILevel(default_hmi_level))
        .WillOnce(Return(hmi_level));
    EXPECT_CALL(mock_message_helper_,
                SendOnPermissionsChangeNotification(kAppId1_, _, _));
    EXPECT_CALL(app_manager_, state_controller()).Times(0);

    Permissions permissions;
    policy_handler_.OnPermissionsUpdated(
        kPolicyAppId_, permissions, default_hmi_level);
  }

  void CreateFunctionalGroupPermission(
      const GroupConsent state,
      const std::string& group_alias,
      const std::string& group_name,
      policy::FunctionalGroupPermission& group_permission) {
    group_permission.state = state;
    group_permission.group_alias = group_alias;
    group_permission.group_name = group_name;
  }

  policy_table::AppHmiTypes HmiTypes(const policy_table::AppHMIType hmi_type) {
    policy_table::AppHmiTypes hmi_types;
    hmi_types.push_back(hmi_type);
    return hmi_types;
  }
};

namespace {
/**
 * @brief The WaitAsync class
 * can wait for a certain amount of function calls from different
 * threads, or a timeout expires.
 */
class WaitAsync {
 public:
  WaitAsync(const uint32_t count, const uint32_t timeout)
      : count_(count), timeout_(timeout) {}

  void Notify() {
    count_--;
    cond_var_.NotifyOne();
  }

  bool Wait(sync_primitives::AutoLock& auto_lock) {
    while (count_ > 0) {
      sync_primitives::ConditionalVariable::WaitStatus wait_status =
          cond_var_.WaitFor(auto_lock, timeout_);
      if (wait_status == sync_primitives::ConditionalVariable::kTimeout) {
        return false;
      }
    }
    return true;
  }

 private:
  int count_;
  const uint32_t timeout_;
  sync_primitives::ConditionalVariable cond_var_;
};
}

TEST_F(PolicyHandlerTest, LoadPolicyLibrary_Method_ExpectLibraryLoaded) {
  // Check before policy enabled from ini file
  EXPECT_CALL(policy_settings_, enable_policy()).WillRepeatedly(Return(false));
  EXPECT_FALSE(policy_handler_.LoadPolicyLibrary());
  EXPECT_CALL(policy_settings_, enable_policy()).WillRepeatedly(Return(true));
  // Check
  EXPECT_TRUE(policy_handler_.LoadPolicyLibrary());

  EXPECT_TRUE(policy_handler_.UnloadPolicyLibrary());
}

TEST_F(PolicyHandlerTest,
       InitPolicyTable_WithoutPreloadedFile_ExpectPolicyTableNotInitialized) {
  // Check
  EXPECT_FALSE(policy_handler_.InitPolicyTable());
}

TEST_F(PolicyHandlerTest,
       InitPolicyTable_WithPreloadedFile_ExpectPolicyTableInitialized) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(policy_handler_.LoadPolicyLibrary());
  // Check
  EXPECT_TRUE(policy_handler_.InitPolicyTable());
  EXPECT_TRUE(policy_handler_.UnloadPolicyLibrary());
}

TEST_F(PolicyHandlerTest,
       ResetPolicyTable_WithoutPreloadedFile_ExpectPolicyTableNotReset) {
  // Check
  EXPECT_FALSE(policy_handler_.ResetPolicyTable());
}

#ifdef EXTERNAL_PROPRIETARY_MODE
TEST_F(PolicyHandlerTest, ResetPolicyTable_PTNotInitialised_PTNotReset) {
  // Arrange
  EXPECT_CALL(app_manager_, event_dispatcher());
  EnablePolicy();
  EXPECT_TRUE(policy_handler_.LoadPolicyLibrary());
  // Check
  EXPECT_FALSE(policy_handler_.ResetPolicyTable());
}
#else
TEST_F(PolicyHandlerTest, ResetPolicyTable_PTNotInitialised_PTNotReset) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(policy_handler_.LoadPolicyLibrary());
  // Check
  EXPECT_TRUE(policy_handler_.ResetPolicyTable());
}

#endif

TEST_F(PolicyHandlerTest,
       ResetPolicyTable_WithPreloadedFile_ExpectPolicyTableReset) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(policy_handler_.LoadPolicyLibrary());
  EXPECT_TRUE(policy_handler_.InitPolicyTable());
  // Check
  EXPECT_TRUE(policy_handler_.ResetPolicyTable());
}

TEST_F(PolicyHandlerTest, ResetPolicyTable_ExpectCallPMResetPT) {
  ChangePolicyManagerToMock();
  EnablePolicy();
  EXPECT_CALL(*mock_policy_manager_, ResetPT(_));
  policy_handler_.ResetPolicyTable();
}

TEST_F(PolicyHandlerTest, ClearUserConsent) {
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(*mock_policy_manager_, ResetUserConsent());
  policy_handler_.ClearUserConsent();
}

TEST_F(PolicyHandlerTest, ReceiveMessageFromSDK) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(policy_handler_.LoadPolicyLibrary());
  // Check
  EXPECT_TRUE(policy_handler_.InitPolicyTable());
  ChangePolicyManagerToMock();
  std::string file_name("sdl_pt_update.json");
  std::ifstream ifile(file_name);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    json = root.toStyledString();
  }
  ifile.close();
  BinaryMessage msg(json.begin(), json.end());
  // Checks
  EXPECT_CALL(app_manager_, GetNextHMICorrelationID());
  EXPECT_CALL(mock_message_helper_, CreateGetVehicleDataRequest(_, _, _));
  EXPECT_CALL(*mock_policy_manager_, PTUpdatedAt(_, _));
  EXPECT_CALL(*mock_policy_manager_, LoadPT("", msg)).WillOnce(Return(true));
  EXPECT_CALL(*mock_policy_manager_, CleanupUnpairedDevices());
  policy_handler_.ReceiveMessageFromSDK("", msg);
}

TEST_F(PolicyHandlerTest, ReceiveMessageFromSDK_PTNotLoaded) {
  // Arrange
  ChangePolicyManagerToMock();
  BinaryMessage msg;
  // Checks

  EXPECT_CALL(*mock_policy_manager_, LoadPT("", msg)).WillOnce(Return(false));
  EXPECT_CALL(*mock_policy_manager_, ForcePTExchange()).WillOnce(Return(""));
  EXPECT_CALL(app_manager_, GetNextHMICorrelationID()).Times(0);
  EXPECT_CALL(mock_message_helper_, CreateGetVehicleDataRequest(_, _, _))
      .Times(0);
  policy_handler_.ReceiveMessageFromSDK("", msg);
}

TEST_F(PolicyHandlerTest, UnloadPolicyLibrary_method_ExpectLibraryUnloaded) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(policy_handler_.LoadPolicyLibrary());
  EXPECT_TRUE(policy_handler_.InitPolicyTable());
  ChangePolicyManagerToMock();
  // Act
  EXPECT_TRUE(policy_handler_.UnloadPolicyLibrary());
  // Check
  EXPECT_FALSE(policy_handler_.InitPolicyTable());
}

TEST_F(PolicyHandlerTest, OnPermissionsUpdated_method_With2Parameters) {
  // Check expectations
  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _));
  // Act
  Permissions perms;
  policy_handler_.OnPermissionsUpdated(kPolicyAppId_, perms);
}

TEST_F(PolicyHandlerTest, OnPermissionsUpdated_TwoParams_InvalidApp_UNSUCCESS) {
  utils::SharedPtr<application_manager_test::MockApplication> invalid_app;
  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(_, _, _)).Times(0);

  Permissions permissions;
  policy_handler_.OnPermissionsUpdated(kPolicyAppId_, permissions);
}

TEST_F(PolicyHandlerTest, OnPermissionsUpdated_InvalidApp_UNSUCCESS) {
  utils::SharedPtr<application_manager_test::MockApplication> invalid_app;
  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .WillOnce(Return(mock_app_))
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _));

  Permissions permissions;
  policy_handler_.OnPermissionsUpdated(kPolicyAppId_, permissions, "HMI_FULL");
}

TEST_F(PolicyHandlerTest, OnPermissionsUpdated_HmiLevelInvalidEnum_UNSUCCESS) {
  TestOnPermissionsUpdated("INVALID_ENUM", mobile_apis::HMILevel::INVALID_ENUM);
}

TEST_F(PolicyHandlerTest,
       OnPermissionsUpdated_HmiLevelEqualsToCurrentHmiLevel_UNSUCCESS) {
  TestOnPermissionsUpdated("HMI_NONE", mobile_apis::HMILevel::HMI_NONE);
}

TEST_F(PolicyHandlerTest,
       OnPermissionsUpdated_MethodWith3Parameters_FromNONE_ToFULL) {
  // Set hmi level from NONE to FULL
  const std::string new_kHmiLevel_string = "HMI_FULL";
  mobile_apis::HMILevel::eType new_hmi_level = mobile_apis::HMILevel::HMI_FULL;
  // Check expectations
  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .Times(2)
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(new_kHmiLevel_string))
      .WillOnce(Return(new_hmi_level));

  EXPECT_CALL(*mock_app_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _));

  EXPECT_CALL(app_manager_, state_controller())
      .WillRepeatedly(ReturnRef(mock_state_controller));
  EXPECT_CALL(mock_state_controller, SetRegularState(_, new_hmi_level, true));
  // Act
  Permissions perms;
  policy_handler_.OnPermissionsUpdated(
      kPolicyAppId_, perms, new_kHmiLevel_string);
}

TEST_F(PolicyHandlerTest,
       OnPermissionsUpdated_MethodWith3Parameters_FromNONE_ToNotFull) {
  // Set hmi level from NONE to Limited
  const std::string new_kHmiLevel_string = "HMI_LIMITED";
  mobile_apis::HMILevel::eType new_hmi_level =
      mobile_apis::HMILevel::HMI_LIMITED;
  // Check expectations
  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .Times(2)
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(new_kHmiLevel_string))
      .WillOnce(Return(new_hmi_level));

  EXPECT_CALL(*mock_app_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _));

  EXPECT_CALL(app_manager_, state_controller())
      .WillRepeatedly(ReturnRef(mock_state_controller));
  EXPECT_CALL(mock_state_controller, SetRegularState(_, new_hmi_level, false));
  // Act
  Permissions perms;
  policy_handler_.OnPermissionsUpdated(
      kPolicyAppId_, perms, new_kHmiLevel_string);
}

TEST_F(PolicyHandlerTest,
       OnPermissionsUpdated_MethodWith3Parameters_FromNotNONE) {
  // Set hmi level from LIMITED to FULL
  std::string new_kHmiLevel_string = "HMI_FULL";
  mobile_apis::HMILevel::eType new_hmi_level = mobile_apis::HMILevel::HMI_FULL;
  // Check expectations
  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .Times(2)
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  EXPECT_CALL(mock_message_helper_, StringToHMILevel(new_kHmiLevel_string))
      .WillOnce(Return(new_hmi_level));

  EXPECT_CALL(*mock_app_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_LIMITED));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _));

  EXPECT_CALL(app_manager_, state_controller()).Times(0);
  // Act
  Permissions perms;
  policy_handler_.OnPermissionsUpdated(
      kPolicyAppId_, perms, new_kHmiLevel_string);
}

TEST_F(PolicyHandlerTest, GetPriority) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, GetPriority(kPolicyAppId_, &priority_));
  // Act
  policy_handler_.GetPriority(kPolicyAppId_, &priority_);
}

TEST_F(PolicyHandlerTest, CheckPermissions) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  CheckPermissionResult result;
  RPCParams kRpc_params;
  // Check expectations
  const uint32_t device = 3;
  const mobile_apis::HMILevel::eType hmi_level =
      mobile_apis::HMILevel::HMI_NONE;
  EXPECT_CALL(*mock_app_, hmi_level()).WillOnce(Return(hmi_level));
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(device));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(
      *mock_policy_manager_,
      CheckPermissions(kPolicyAppId_, kHmiLevel_, kRpc_, kRpc_params, _));

#else   // EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(*mock_policy_manager_,
              CheckPermissions(
                  kDeviceId, kPolicyAppId_, kHmiLevel_, kRpc_, kRpc_params, _));
#endif  // EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(mock_message_helper_, StringifiedHMILevel(hmi_level))
      .WillOnce(Return(kHmiLevel_));
  EXPECT_CALL(mock_message_helper_, GetDeviceMacAddressForHandle(device, _))
      .WillOnce(Return(kDeviceId));
  // Act
  policy_handler_.CheckPermissions(mock_app_, kRpc_, kRpc_params, result);
}

TEST_F(PolicyHandlerTest, GetNotificationsNumber) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, GetNotificationsNumber(priority_));
  // Act
  policy_handler_.GetNotificationsNumber(priority_);
}

TEST_F(PolicyHandlerTest, GetUserConsentForDevice) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(kDeviceId_))
      .WillOnce(Return(DeviceConsent::kDeviceHasNoConsent));
  // Act
  policy_handler_.GetUserConsentForDevice(kDeviceId_);
}

TEST_F(PolicyHandlerTest, GetDefaultHmi) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_,
              GetDefaultHmi(kPolicyAppId_, &default_hmi_));
  // Act
  policy_handler_.GetDefaultHmi(kPolicyAppId_, &default_hmi_);
}

TEST_F(PolicyHandlerTest, GetInitialAppData) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  StringArray* nicknames = NULL;
  StringArray* app_hmi_types = NULL;
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_,
              GetInitialAppData(kPolicyAppId_, nicknames, app_hmi_types));
  // Act
  policy_handler_.GetInitialAppData(kPolicyAppId_, nicknames, app_hmi_types);
}

TEST_F(PolicyHandlerTest, GetUpdateUrls) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  EndpointUrls endpoints;
  const std::string service_type = "0x0";
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, GetUpdateUrls(service_type, _));
  // Act
  policy_handler_.GetUpdateUrls(service_type, endpoints);
}

TEST_F(PolicyHandlerTest, NextRetryTimeout) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, NextRetryTimeout());
  // Act
  policy_handler_.NextRetryTimeout();
}

TEST_F(PolicyHandlerTest, TimeoutExchangeSec) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, TimeoutExchangeMSec())
      .WillOnce(Return(1000));
  // Act
  EXPECT_EQ(1u, policy_handler_.TimeoutExchangeSec());
}

TEST_F(PolicyHandlerTest, TimeoutExchangeMsec) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, TimeoutExchangeMSec())
      .WillOnce(Return(1000));
  // Act
  EXPECT_EQ(1000u, policy_handler_.TimeoutExchangeMSec());
}

TEST_F(PolicyHandlerTest, OnExceededTimeout) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, OnExceededTimeout());
  // Act
  policy_handler_.OnExceededTimeout();
}

TEST_F(PolicyHandlerTest, OnSystemReady) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, OnSystemReady());
  // Act
  policy_handler_.OnSystemReady();
}

TEST_F(PolicyHandlerTest, PTUpdatedAt_method_UseCounter_KILOMETERS) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  const int value = 1000;
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, PTUpdatedAt(Counters::KILOMETERS, value));
  // Act
  policy_handler_.PTUpdatedAt(Counters::KILOMETERS, value);
}

TEST_F(PolicyHandlerTest, PTUpdatedAt_method_UseCounter_DAYS_AFTER_EPOCH) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  const int value = 16000;
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_,
              PTUpdatedAt(Counters::DAYS_AFTER_EPOCH, value));
  // Act
  policy_handler_.PTUpdatedAt(Counters::DAYS_AFTER_EPOCH, value);
}

TEST_F(PolicyHandlerTest, CheckSystemAction_method_WithType_KEEP_CONTEXT) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  mobile_apis::SystemAction::eType system_action =
      mobile_apis::SystemAction::eType::KEEP_CONTEXT;
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, CanAppKeepContext(kPolicyAppId_));
  // Act
  policy_handler_.CheckSystemAction(system_action, kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, CheckSystemAction_method_WithType_STEAL_FOCUS) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  mobile_apis::SystemAction::eType system_action =
      mobile_apis::SystemAction::eType::STEAL_FOCUS;
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, CanAppStealFocus(kPolicyAppId_));
  // Act
  policy_handler_.CheckSystemAction(system_action, kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, CheckSystemAction_method_WithType_DEFAULT_ACTION) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  mobile_apis::SystemAction::eType system_action =
      mobile_apis::SystemAction::eType::DEFAULT_ACTION;
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, CanAppStealFocus(kPolicyAppId_)).Times(0);
  EXPECT_CALL(*mock_policy_manager_, CanAppKeepContext(kPolicyAppId_)).Times(0);
  // Act
  EXPECT_TRUE(policy_handler_.CheckSystemAction(system_action, kPolicyAppId_));
}

TEST_F(PolicyHandlerTest, CheckSystemAction_method_WithType_INVALID_ENUM) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  mobile_apis::SystemAction::eType system_action =
      mobile_apis::SystemAction::eType::INVALID_ENUM;
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, CanAppStealFocus(kPolicyAppId_)).Times(0);
  EXPECT_CALL(*mock_policy_manager_, CanAppKeepContext(kPolicyAppId_)).Times(0);
  // Act
  EXPECT_FALSE(policy_handler_.CheckSystemAction(system_action, kPolicyAppId_));
}

TEST_F(PolicyHandlerTest, KmsChanged) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  const int kilometers = 1600;
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, KmsChanged(kilometers));
  // Act
  policy_handler_.KmsChanged(kilometers);
}
// policy_handler l:1026

void PolicyHandlerTest::TestActivateApp(const uint32_t connection_key,
                                        const uint32_t correlation_id) {
  utils::SharedPtr<application_manager_test::MockApplication> application1 =
      utils::MakeShared<application_manager_test::MockApplication>();
  EXPECT_CALL(app_manager_, ActivateApplication(_))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(app_manager_, application(connection_key))
      .WillRepeatedly(Return(application1));

  AppPermissions permissions(kPolicyAppId_);
  permissions.appPermissionsConsentNeeded = true;

// Check expectations
#ifdef EXTERNAL_PROPRIETARY_MODE
  const connection_handler::DeviceHandle device_handle = 0u;
  EXPECT_CALL(app_manager_, connection_handler())
      .WillRepeatedly(ReturnRef(conn_handler));
  EXPECT_CALL(conn_handler, get_session_observer())
      .WillOnce(ReturnRef(mock_session_observer));
  utils::SharedPtr<usage_statistics_test::MockStatisticsManager>
      mock_statistics_manager =
          utils::MakeShared<usage_statistics_test::MockStatisticsManager>();
  UsageStatistics usage_stats(
      "0",
      utils::SharedPtr<usage_statistics::StatisticsManager>(
          mock_statistics_manager));
  EXPECT_CALL(*application1, usage_report()).WillOnce(ReturnRef(usage_stats));
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(_))
      .WillOnce(Return(DeviceConsent::kDeviceHasNoConsent));
  EXPECT_CALL(app_manager_, state_controller())
      .WillRepeatedly(ReturnRef(mock_state_controller));
  EXPECT_CALL(*mock_statistics_manager, Increment(_, _))
      .WillRepeatedly(Return());
  EXPECT_CALL(*application1, device()).WillRepeatedly(Return(device_handle));
  EXPECT_CALL(*application1, is_audio()).WillRepeatedly(Return(false));
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _));
  EXPECT_CALL(
      mock_session_observer,
      GetDataOnDeviceID(
          testing::Matcher<transport_manager::DeviceHandle>(device_handle),
          _,
          _,
          _,
          _));
#endif  // EXTERNAL_PROPRIETARY_MODE

  EXPECT_CALL(*application1, policy_app_id()).WillOnce(Return(kPolicyAppId_));
  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));
  ON_CALL(*mock_policy_manager_, Increment(_, _)).WillByDefault(Return());
  EXPECT_CALL(*mock_policy_manager_, RemovePendingPermissionChanges(_));
  EXPECT_CALL(mock_message_helper_, SendSDLActivateAppResponse(_, _, _));
  ON_CALL(*application1, app_id()).WillByDefault(Return(kAppId1_));
  // Act
  policy_handler_.OnActivateApp(connection_key, correlation_id);
}

TEST_F(PolicyHandlerTest, OnActivateApp) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  TestActivateApp(kConnectionKey_, kCorrelationKey_);
}

TEST_F(PolicyHandlerTest, OnActivateApp_InvalidApp_UNSUCCESS) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  utils::SharedPtr<application_manager_test::MockApplication> invalid_app;
  EXPECT_CALL(app_manager_, application(kConnectionKey_))
      .WillOnce(Return(invalid_app));

  EXPECT_CALL(mock_message_helper_, SendSDLActivateAppResponse(_, _, _))
      .Times(0);
  policy_handler_.OnActivateApp(kConnectionKey_, kCorrelationKey_);
}

TEST_F(PolicyHandlerTest, OnActivateApp_AppIsRevoked_AppNotActivated) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(app_manager_, ActivateApplication(_)).Times(0);
  EXPECT_CALL(app_manager_, application(kConnectionKey_))
      .WillOnce(Return(mock_app_));

  AppPermissions permissions(kPolicyAppId_);
  permissions.appRevoked = true;

#ifdef EXTERNAL_PROPRIETARY_MODE
  utils::SharedPtr<usage_statistics_test::MockStatisticsManager>
      mock_statistics_manager =
          utils::MakeShared<usage_statistics_test::MockStatisticsManager>();
  UsageStatistics usage_stats(
      "0",
      utils::SharedPtr<usage_statistics::StatisticsManager>(
          mock_statistics_manager));
  EXPECT_CALL(*mock_app_, usage_report()).WillOnce(ReturnRef(usage_stats));
  const std::string default_mac = "00:00:00:00:00:00";
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(default_mac))
      .WillOnce(Return(DeviceConsent::kDeviceAllowed));
#endif  // EXTERNAL_PROPRIETARY_MODE

  // Check expectations
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));
  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));
  ON_CALL(*mock_policy_manager_, Increment(_, _)).WillByDefault(Return());
  EXPECT_CALL(*mock_policy_manager_, RemovePendingPermissionChanges(_));
  EXPECT_CALL(mock_message_helper_, SendSDLActivateAppResponse(_, _, _));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId1_));
  // Act
  policy_handler_.OnActivateApp(kConnectionKey_, kCorrelationKey_);
}

TEST_F(PolicyHandlerTest, OnIgnitionCycleOver) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, IncrementIgnitionCycles());
  // Act
  policy_handler_.OnIgnitionCycleOver();
}

void PolicyHandlerTest::OnPendingPermissionChangePrecondition(
    mobile_apis::HMILevel::eType hmi_level) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  utils::SharedPtr<application_manager_test::MockApplication> application =
      utils::MakeShared<application_manager_test::MockApplication>();

  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .WillOnce(Return(application));
  EXPECT_CALL(*application, app_id()).WillRepeatedly(Return(kAppId1_));
  EXPECT_CALL(*application, hmi_level()).WillRepeatedly(Return(hmi_level));
}

TEST_F(PolicyHandlerTest,
       OnPendingPermissionChange_AppInLimitedConsentnotNeeded) {
  // Arrange
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_LIMITED);
  // Check expectations
  AppPermissions permissions(kPolicyAppId_);
  permissions.appPermissionsConsentNeeded = false;
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _)).Times(0);

  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));
  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_)).Times(0);
  // Act
  policy_handler_.OnPendingPermissionChange(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, OnPendingPermissionChange_AppInLimitedConsentNeeded) {
  // Arrange
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_LIMITED);
  AppPermissions permissions(kPolicyAppId_);
  permissions.appPermissionsConsentNeeded = true;
  // Check expectations
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _));
  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));
  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));
  // Act
  policy_handler_.OnPendingPermissionChange(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, OnPendingPermissionChange_AppLimitedAndRevoked) {
  // Arrange
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_LIMITED);
  AppPermissions permissions(kPolicyAppId_);
  permissions.appRevoked = true;

  // Check expectations
  EXPECT_CALL(app_manager_, state_controller())
      .WillRepeatedly(ReturnRef(mock_state_controller));
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _));
  EXPECT_CALL(mock_state_controller,
              SetRegularState(_,
                              mobile_apis::HMILevel::HMI_NONE,
                              mobile_apis::AudioStreamingState::NOT_AUDIBLE,
                              mobile_apis::VideoStreamingState::NOT_STREAMABLE,
                              true));

  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));
  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));
  // Act
  policy_handler_.OnPendingPermissionChange(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, OnPendingPermissionChange_AppInBackgroundAndRevoked) {
  // Arrange
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_BACKGROUND);
  AppPermissions permissions(kPolicyAppId_);
  permissions.isAppPermissionsRevoked = true;

  // Check expectations
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _));

  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));
  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));
  // Act
  policy_handler_.OnPendingPermissionChange(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest,
       OnPendingPermissionChange_AppInLIMITEDAndUnauthorized) {
  // Arrange
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_LIMITED);
  AppPermissions permissions(kPolicyAppId_);
  permissions.appUnauthorized = true;

  NsSmartDeviceLink::NsSmartObjects::SmartObjectSPtr message =
      utils::MakeShared<NsSmartDeviceLink::NsSmartObjects::SmartObject>();
  // Check expectations
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _));

  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));

  EXPECT_CALL(mock_message_helper_,
              GetOnAppInterfaceUnregisteredNotificationToMobile(
                  kAppId1_,
                  mobile_api::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED))
      .WillOnce(Return(message));
  EXPECT_CALL(app_manager_,
              ManageMobileCommand(_, commands::Command::ORIGIN_SDL));

  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));

  // Act
  policy_handler_.OnPendingPermissionChange(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest,
       OnPendingPermissionChange_AppInLIMITEDAndRequestTypeChanged_SUCCESS) {
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_LIMITED);
  AppPermissions permissions(kPolicyAppId_);
  permissions.requestTypeChanged = true;

  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _));

  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));

  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));

  policy_handler_.OnPendingPermissionChange(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest,
       OnPendingPermissionChange_AppInBackgroundAndUnauthorized) {
  // Arrange
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_BACKGROUND);
  AppPermissions permissions(kPolicyAppId_);
  permissions.appUnauthorized = true;

  NsSmartDeviceLink::NsSmartObjects::SmartObjectSPtr message =
      utils::MakeShared<NsSmartDeviceLink::NsSmartObjects::SmartObject>();
  // Check expectations
  // Notification won't be sent
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _)).Times(0);

  EXPECT_CALL(*mock_policy_manager_, GetAppPermissionsChanges(_))
      .WillOnce(Return(permissions));

  EXPECT_CALL(mock_message_helper_,
              GetOnAppInterfaceUnregisteredNotificationToMobile(
                  kAppId1_,
                  mobile_api::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED))
      .WillOnce(Return(message));
  EXPECT_CALL(app_manager_,
              ManageMobileCommand(_, commands::Command::ORIGIN_SDL));

  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));

  // Act
  policy_handler_.OnPendingPermissionChange(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, PTExchangeAtUserRequest) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, ForcePTExchangeAtUserRequest());
  EXPECT_CALL(mock_message_helper_, SendUpdateSDLResponse(_, _, _));
  // Act
  const uint32_t correlation_id = 2;
  policy_handler_.PTExchangeAtUserRequest(correlation_id);
}

TEST_F(PolicyHandlerTest, AddDevice) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  const std::string connection_type("BT");
  EXPECT_CALL(*mock_policy_manager_, AddDevice(kDeviceId_, connection_type));
  // Act
  policy_handler_.AddDevice(kDeviceId_, connection_type);
}

TEST_F(PolicyHandlerTest, SetDeviceInfo) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  std::string kDeviceId_;
  const DeviceInfo device_info;
  EXPECT_CALL(*mock_policy_manager_, SetDeviceInfo(kDeviceId_, _));
  // Act
  policy_handler_.SetDeviceInfo(kDeviceId_, device_info);
}

TEST_F(PolicyHandlerTest, OnGetUserFriendlyMessage) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  std::vector<std::string> message_codes;
  const std::string language("ru-ru");
  const uint32_t correlation_id = 2;
#ifdef EXTERNAL_PROPRIETARY_MODE
  const hmi_apis::Common_Language::eType default_language =
      hmi_apis::Common_Language::EN_US;
  const std::string default_language_string = "EN_US";
  application_manager_test::MockHMICapabilities mock_hmi_capabilities;
  EXPECT_CALL(app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities));
  EXPECT_CALL(mock_message_helper_, CommonLanguageToString(default_language))
      .WillOnce(Return(default_language_string));
  EXPECT_CALL(mock_hmi_capabilities, active_ui_language())
      .WillOnce(Return(default_language));
  EXPECT_CALL(
      *mock_policy_manager_,
      GetUserFriendlyMessages(message_codes, language, default_language_string))
      .WillOnce(Return(std::vector<UserFriendlyMessage>()));
#else
  EXPECT_CALL(*mock_policy_manager_,
              GetUserFriendlyMessages(message_codes, language))
      .WillOnce(Return(std::vector<UserFriendlyMessage>()));
#endif  // EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(mock_message_helper_,
              SendGetUserFriendlyMessageResponse(_, _, _));
  // Act
  policy_handler_.OnGetUserFriendlyMessage(
      message_codes, language, correlation_id);
}

TEST_F(PolicyHandlerTest, OnGetStatusUpdate) {
  // Arrange
  ChangePolicyManagerToMock();
  const uint32_t correlation_id = 2;
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, GetPolicyTableStatus());
  EXPECT_CALL(mock_message_helper_,
              SendGetStatusUpdateResponse(_, correlation_id, _));
  // Act
  policy_handler_.OnGetStatusUpdate(correlation_id);
}

TEST_F(PolicyHandlerTest, OnUpdateStatusChanged) {
  ChangePolicyManagerToMock();
  // Check expectations
  const std::string& status("new status");
  EXPECT_CALL(mock_message_helper_, SendOnStatusUpdate(status, _));
  // Act
  policy_handler_.OnUpdateStatusChanged(status);
}

TEST_F(PolicyHandlerTest, OnCurrentDeviceIdUpdateRequired) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  utils::SharedPtr<application_manager_test::MockApplication> application =
      utils::MakeShared<application_manager_test::MockApplication>();
  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .WillOnce(Return(application));
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));
  protocol_handler_test::MockSessionObserver session_observer;

  EXPECT_CALL(conn_handler, get_session_observer())
      .WillOnce(ReturnRef(session_observer));

  const transport_manager::DeviceHandle handle = 0u;

  EXPECT_CALL(session_observer,
              GetDataOnDeviceID(
                  testing::Matcher<transport_manager::DeviceHandle>(handle),
                  _,
                  _,
                  _,
                  _));

  // Act
  policy_handler_.OnCurrentDeviceIdUpdateRequired(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, OnSystemInfoChanged) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  const std::string language("ru-ru");
  EXPECT_CALL(*mock_policy_manager_, SetSystemLanguage(language));
  // Act
  policy_handler_.OnSystemInfoChanged(language);
}

TEST_F(PolicyHandlerTest, OnGetSystemInfo) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  const std::string ccpu_version("4.1.3.B_EB355B");
  const std::string wers_country_code("WAEGB");
  const std::string language("ru-ru");
  EXPECT_CALL(*mock_policy_manager_,
              SetSystemInfo(ccpu_version, wers_country_code, language));
  // Act
  policy_handler_.OnGetSystemInfo(ccpu_version, wers_country_code, language);
}

TEST_F(PolicyHandlerTest, IsApplicationRevoked) {
  // Arrange
  EnablePolicy();
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, IsApplicationRevoked(kPolicyAppId_));
  // Act
  policy_handler_.IsApplicationRevoked(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, OnSystemInfoUpdateRequired) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(mock_message_helper_, SendGetSystemInfoRequest(_));
  // Act
  policy_handler_.OnSystemInfoUpdateRequired();
}

TEST_F(PolicyHandlerTest, GetAppRequestTypes) {
  // Arrange
  EnablePolicy();
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, GetAppRequestTypes(kPolicyAppId_))
      .WillOnce(Return(std::vector<std::string>()));
  // Act
  policy_handler_.GetAppRequestTypes(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, OnVIIsReady) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(app_manager_, GetNextHMICorrelationID());
  EXPECT_CALL(mock_message_helper_, CreateGetVehicleDataRequest(_, _, _));
  // Act
  policy_handler_.OnVIIsReady();
}

TEST_F(PolicyHandlerTest, RemoveDevice) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, MarkUnpairedDevice(kDeviceId_));
  // Act
  policy_handler_.RemoveDevice(kDeviceId_);
}

TEST_F(PolicyHandlerTest, GetAppName) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  const CustomString app_name("my_mobile_app");

  utils::SharedPtr<application_manager_test::MockApplication> application =
      utils::MakeShared<application_manager_test::MockApplication>();

  EXPECT_CALL(*application, name()).WillOnce(ReturnRef(app_name));
  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .WillOnce(Return(application));
  // Act
  EXPECT_EQ(app_name, policy_handler_.GetAppName(kPolicyAppId_));
}

TEST_F(PolicyHandlerTest, OnUpdateRequestSentToMobile) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, OnUpdateStarted());
  // Act
  policy_handler_.OnUpdateRequestSentToMobile();
}

TEST_F(PolicyHandlerTest, OnUpdateHMIAppType) {
  // Arrange
  EnablePolicy();
  application_manager_test::MockPolicyHandlerObserver policy_handler_observer;
  policy_handler_.add_listener(&policy_handler_observer);
  std::map<std::string, StringArray> app_hmi_types;
  StringArray arr;
  arr.push_back("test_hmi_type");
  app_hmi_types["app1"] = arr;
  // Check expectations
  EXPECT_CALL(policy_handler_observer, OnUpdateHMIAppType(_));
  // Act
  policy_handler_.OnUpdateHMIAppType(app_hmi_types);
}

TEST_F(PolicyHandlerTest, SendOnAppPermissionsChanged) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  utils::SharedPtr<application_manager_test::MockApplication> application =
      utils::MakeShared<application_manager_test::MockApplication>();
  // Check expectations
  EXPECT_CALL(app_manager_, application_by_policy_id(kPolicyAppId_))
      .WillOnce(Return(application));
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(_, _, _));
  AppPermissions permissions(kPolicyAppId_);
  // Act
  policy_handler_.SendOnAppPermissionsChanged(permissions, kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, OnPTExchangeNeeded) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, ForcePTExchange());
  // Act
  policy_handler_.OnPTExchangeNeeded();
}

TEST_F(PolicyHandlerTest, AddApplication) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(
      *mock_policy_manager_,
      AddApplication(kPolicyAppId_, HmiTypes(policy_table::AHT_DEFAULT)))
      .WillOnce(Return(utils::MakeShared<utils::CallNothing>()));
  // Act
  policy_handler_.AddApplication(kPolicyAppId_,
                                 HmiTypes(policy_table::AHT_DEFAULT));
}

TEST_F(PolicyHandlerTest, HeartBeatTimeout) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, HeartBeatTimeout(kPolicyAppId_));
  // Act
  policy_handler_.HeartBeatTimeout(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, OnAppsSearchStarted) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, OnAppsSearchStarted());
  // Act
  policy_handler_.OnAppsSearchStarted();
}

TEST_F(PolicyHandlerTest, OnAppsSearchCompleted) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, OnAppsSearchCompleted(true));
  // Act
  policy_handler_.OnAppsSearchCompleted(true);
}

TEST_F(PolicyHandlerTest, OnAppRegisteredOnMobile) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations

  EXPECT_CALL(*mock_policy_manager_, OnAppRegisteredOnMobile(kPolicyAppId_));
  // Act
  policy_handler_.OnAppRegisteredOnMobile(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, IsRequestTypeAllowed) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations

  mobile_apis::RequestType::eType type =
      mobile_apis::RequestType::eType::EMERGENCY;
  EXPECT_CALL(*mock_policy_manager_, GetAppRequestTypes(kPolicyAppId_))
      .WillOnce(Return(std::vector<std::string>()));
  // Act
  policy_handler_.IsRequestTypeAllowed(kPolicyAppId_, type);
}

TEST_F(PolicyHandlerTest, GetVehicleInfo) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, GetVehicleInfo())
      .WillOnce(Return(VehicleInfo()));
  // Act
  policy_handler_.GetVehicleInfo();
}

TEST_F(PolicyHandlerTest, Increment_method_WithOneParameter) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  usage_statistics::GlobalCounterId type =
      usage_statistics::GlobalCounterId::IAP_BUFFER_FULL;
  EXPECT_CALL(*mock_policy_manager_, Increment(type));
  // Act
  policy_handler_.Increment(type);
}

TEST_F(PolicyHandlerTest, Increment_method_WithTwoParameters) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations

  usage_statistics::AppCounterId type =
      usage_statistics::AppCounterId::USER_SELECTIONS;
  EXPECT_CALL(*mock_policy_manager_, Increment(kPolicyAppId_, type));
  // Act
  policy_handler_.Increment(kPolicyAppId_, type);
}

TEST_F(PolicyHandlerTest, Set) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations

  const std::string value("ru-ru");
  usage_statistics::AppInfoId type = usage_statistics::AppInfoId::LANGUAGE_GUI;
  EXPECT_CALL(*mock_policy_manager_, Set(kPolicyAppId_, type, value));
  // Act
  policy_handler_.Set(kPolicyAppId_, type, value);
}

TEST_F(PolicyHandlerTest, Add) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations

  const int32_t timespan_seconds = 100;
  usage_statistics::AppStopwatchId type =
      usage_statistics::AppStopwatchId::SECONDS_HMI_FULL;
  EXPECT_CALL(*mock_policy_manager_,
              Add(kPolicyAppId_, type, timespan_seconds));
  // Act
  policy_handler_.Add(kPolicyAppId_, type, timespan_seconds);
}

TEST_F(PolicyHandlerTest, GetAvailableApps) {
  // Set does not have any apps
  std::queue<std::string> apps;
  policy_handler_.GetAvailableApps(apps);
  EXPECT_EQ(0u, apps.size());

  // Added one app
  test_app.insert(mock_app_);

  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return("app_id"));

  policy_handler_.GetAvailableApps(apps);

  EXPECT_EQ(1u, apps.size());
}

TEST_F(PolicyHandlerTest, OnGetListOfPermissions) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  const uint32_t app_id = 10u;
  const uint32_t corr_id = 1u;
  const std::string default_mac = "00:00:00:00:00:00";
  test_app.insert(mock_app_);

  // Expectations
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForApp(default_mac, _, _));

  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));
  EXPECT_CALL(conn_handler, get_session_observer())
      .WillOnce(ReturnRef(mock_session_observer));
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(0));
  EXPECT_CALL(app_manager_, application(app_id))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(std::string()));
  EXPECT_CALL(mock_session_observer,
              GetDataOnDeviceID(
                  testing::An<transport_manager::DeviceHandle>(), _, _, _, _));

  policy_handler_.OnGetListOfPermissions(app_id, corr_id);
}

TEST_F(PolicyHandlerTest, OnGetListOfPermissions_WithoutConnectionKey) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  const uint32_t app_id = 0u;
  const uint32_t corr_id = 1u;
  const std::string default_mac = "00:00:00:00:00:00";
  test_app.insert(mock_app_);

  // Expectations
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForApp(default_mac, _, _));

  EXPECT_CALL(app_manager_, application(app_id))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));
  EXPECT_CALL(conn_handler, get_session_observer())
      .WillOnce(ReturnRef(mock_session_observer));
  EXPECT_CALL(app_manager_, applications()).WillRepeatedly(Return(app_set));
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(0));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(std::string()));
  EXPECT_CALL(mock_session_observer,
              GetDataOnDeviceID(
                  testing::An<transport_manager::DeviceHandle>(), _, _, _, _));

#ifdef EXTERNAL_PROPRIETARY_MODE
  policy::ExternalConsentStatus external_consent_status =
      policy::ExternalConsentStatus();
  EXPECT_CALL(
      mock_message_helper_,
      SendGetListOfPermissionsResponse(_, external_consent_status, corr_id, _));
  EXPECT_CALL(*mock_policy_manager_, GetExternalConsentStatus())
      .WillOnce(Return(external_consent_status));
#else
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(_, corr_id, _));
#endif  // #ifdef EXTERNAL_PROPRIETARY_MODE

  policy_handler_.OnGetListOfPermissions(app_id, corr_id);
}

ACTION_P(SetGroupPermissions, permissions) {
  arg2 = permissions;
}

TEST_F(PolicyHandlerTest, OnGetListOfPermissions_GroupPermissions_SUCCESS) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  policy::FunctionalGroupPermission group_permission_disallowed1;
  CreateFunctionalGroupPermission(GroupConsent::kGroupDisallowed,
                                  kGroupAliasDisallowed_,
                                  kGroupNameDisallowed_,
                                  group_permission_disallowed1);

  policy::FunctionalGroupPermission group_permission_disallowed2;
  CreateFunctionalGroupPermission(GroupConsent::kGroupDisallowed,
                                  kGroupAliasDisallowed_,
                                  kGroupNameDisallowed_,
                                  group_permission_disallowed2);

  policy::FunctionalGroupPermission group_permission_allowed1;
  CreateFunctionalGroupPermission(GroupConsent::kGroupAllowed,
                                  kGroupAliasAllowed_,
                                  kGroupNameAllowed_,
                                  group_permission_allowed1);

  policy::FunctionalGroupPermission group_permission_allowed2;
  CreateFunctionalGroupPermission(GroupConsent::kGroupAllowed,
                                  kGroupAliasAllowed_,
                                  kGroupNameAllowed_,
                                  group_permission_allowed2);

  std::vector<policy::FunctionalGroupPermission> group_permissions;
  group_permissions.push_back(group_permission_allowed1);
  group_permissions.push_back(group_permission_allowed2);
  group_permissions.push_back(group_permission_disallowed1);
  group_permissions.push_back(group_permission_disallowed2);

  const uint32_t app_id = 0u;
  const uint32_t corr_id = 1u;
  const std::string default_mac = "00:00:00:00:00:00";
  test_app.insert(mock_app_);

  // Expectations
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForApp(_, _, _))
      .WillOnce(SetGroupPermissions(group_permissions));

  EXPECT_CALL(app_manager_, application(app_id))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));
  EXPECT_CALL(conn_handler, get_session_observer())
      .WillOnce(ReturnRef(mock_session_observer));
  EXPECT_CALL(app_manager_, applications()).WillRepeatedly(Return(app_set));
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(0));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(std::string()));
  EXPECT_CALL(mock_session_observer,
              GetDataOnDeviceID(
                  testing::An<transport_manager::DeviceHandle>(), _, _, _, _));

#ifdef EXTERNAL_PROPRIETARY_MODE
  policy::ExternalConsentStatus external_consent_status =
      policy::ExternalConsentStatus();
  EXPECT_CALL(
      mock_message_helper_,
      SendGetListOfPermissionsResponse(_, external_consent_status, corr_id, _));
  EXPECT_CALL(*mock_policy_manager_, GetExternalConsentStatus())
      .WillOnce(Return(external_consent_status));
#else
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(_, corr_id, _));
#endif  // #ifdef EXTERNAL_PROPRIETARY_MODE

  policy_handler_.OnGetListOfPermissions(app_id, corr_id);
}

TEST_F(PolicyHandlerTest, RetrieveCertificate) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  const std::string test_certificate = "test certificate";
  EXPECT_CALL(*mock_policy_manager_, RetrieveCertificate())
      .WillOnce(Return(test_certificate));
  EXPECT_EQ(test_certificate, policy_handler_.RetrieveCertificate());
}

TEST_F(PolicyHandlerTest, OnSnapshotCreated_UrlNotAdded) {
  EnablePolicyAndPolicyManagerMock();
  BinaryMessage msg;
  EndpointUrls test_data;
#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
  ExtendedPolicyExpectations();
#endif  // PROPRIETARY_MODE || EXTERNAL_PROPRIETARY_MODE
#ifdef EXTERNAL_PROPRIETARY_MODE
  std::vector<int> retry_delay_seconds;
  const uint32_t timeout_exchange = 10u;
  // TODO(AKutsan): Policy move issues
  EXPECT_CALL(*mock_policy_manager_, GetUpdateUrls("0x07", _))
      .WillRepeatedly(SetArgReferee<1>(test_data));
  policy_handler_.OnSnapshotCreated(msg, retry_delay_seconds, timeout_exchange);
#else   // EXTERNAL_PROPRIETARY_MODE
  policy_handler_.OnSnapshotCreated(msg);
#endif  // EXTERNAL_PROPRIETARY_MODE
}

TEST_F(PolicyHandlerTest,
       OnAllowSDLFunctionalityNotification_AllowedWithDevId_AppActivated) {
  // Arrange

  EnablePolicyAndPolicyManagerMock();
  const uint32_t connection_key = 1u;
  const uint32_t correlation_id = 2u;
  TestActivateApp(connection_key, correlation_id);

  const bool is_allowed = true;
  std::vector<std::string> device_macs;
  device_macs.push_back(kPolicyAppId_);

  // Not called because id was setted
  EXPECT_CALL(conn_handler, GetConnectedDevicesMAC(_)).Times(0);
  EXPECT_CALL(conn_handler, GetDeviceID(kPolicyAppId_, _))
      .WillRepeatedly(Return(true));

  policy_handler_.OnAllowSDLFunctionalityNotification(is_allowed,
                                                      kPolicyAppId_);
}
#ifdef EXTERNAL_PROPRIETARY_MODE
TEST_F(PolicyHandlerTest, OnSnapshotCreated_UrlAdded) {
  EnablePolicyAndPolicyManagerMock();
  BinaryMessage msg;
  EndpointUrls test_data;
  EndpointData data("some_data");
  std::vector<int> retry_delay_seconds;
  const uint32_t timeout_exchange = 10u;
  test_data.push_back(data);

  ExtendedPolicyExpectations();

  EXPECT_CALL(app_manager_, application(kAppId1_))
      .WillRepeatedly(Return(mock_app_));

  policy_handler_.OnSnapshotCreated(msg, retry_delay_seconds, timeout_exchange);
}
#else  // EXTERNAL_PROPRIETARY_MODE
// TODO(LevchenkoS): Find out what is wrong with this test on HTTP Policy
TEST_F(PolicyHandlerTest, DISABLED_OnSnapshotCreated_UrlAdded) {
  EnablePolicyAndPolicyManagerMock();
  BinaryMessage msg;
  EndpointUrls test_data;
  EndpointData data(kDummyData);
  test_data.push_back(data);
  ApplicationSharedPtr mock_app;

#ifdef PROPRIETARY_MODE
  ExtendedPolicyExpectations();
#else
  AppIdURL next_app_url = std::make_pair(0, 0);
  EXPECT_CALL(*mock_policy_manager_, GetUpdateUrls("0x07", _))
      .WillRepeatedly(SetArgReferee<1>(test_data));
  EXPECT_CALL(*mock_policy_manager_, GetNextUpdateUrl(_))
      .WillOnce(Return(next_app_url));
  EXPECT_CALL(app_manager_, application_by_policy_id(_))
      .WillOnce(Return(mock_app));
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));
  EXPECT_CALL(conn_handler, get_session_observer())
      .WillOnce(ReturnRef(mock_session_observer));
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(0));
  EXPECT_CALL(app_manager_, applications()).WillOnce(Return(app_set));
  EXPECT_CALL(mock_message_helper_, SendPolicySnapshotNotification(_, _, _, _));
  // Check expectations for get app id
  GetAppIDForSending();
  // Expectations
  EXPECT_CALL(app_manager_, application(kAppId1_))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));
#endif  // PROPRIETARY_MODE

  EXPECT_CALL(*mock_policy_manager_, OnUpdateStarted());
  policy_handler_.OnSnapshotCreated(msg);
}
#endif  // EXTERNAL_PROPRIETARY_MODE

TEST_F(PolicyHandlerTest,
       OnAllowSDLFunctionalityNotification_Allowed_WithoutDevId_AppActivated) {
  // Arrange
  const bool is_allowed = true;
  std::vector<std::string> device_macs;
  device_macs.push_back(kPolicyAppId_);
  EnablePolicyAndPolicyManagerMock();
  TestActivateApp(kConnectionKey_, kCorrelationKey_);

  // Device ID is not setted by us
  EXPECT_CALL(conn_handler, GetConnectedDevicesMAC(_))
      .WillOnce(SetArgReferee<0>(device_macs));

  EXPECT_CALL(*mock_policy_manager_,
              SetUserConsentForDevice(kPolicyAppId_, is_allowed));

  policy_handler_.OnAllowSDLFunctionalityNotification(is_allowed, "");
}

TEST_F(PolicyHandlerTest,
       OnAllowSDLFunctionalityNotification_DefaultDeviceId_UNSUCCESS) {
  const std::string default_mac_address("00:00:00:00:00:00");
  std::vector<std::string> device_macs;
  device_macs.push_back(default_mac_address);
  EnablePolicyAndPolicyManagerMock();
  TestActivateApp(kConnectionKey_, kCorrelationKey_);

  EXPECT_CALL(conn_handler, GetConnectedDevicesMAC(_))
      .WillOnce(SetArgReferee<0>(device_macs));

  EXPECT_CALL(*mock_policy_manager_, SetUserConsentForDevice(_, _)).Times(0);

  const bool is_allowed = true;
  policy_handler_.OnAllowSDLFunctionalityNotification(is_allowed, "");
}

TEST_F(PolicyHandlerTest, OnDeviceConsentChanged_ConsentAllowed) {
  const bool is_allowed = true;
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  connection_handler::DeviceHandle test_device_id = 100u;
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));

  EXPECT_CALL(conn_handler, GetDeviceID(kPolicyAppId_, _))
      .WillOnce(DoAll(SetArgPointee<1>(test_device_id), Return(true)));

  test_app.insert(mock_app_);
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(test_device_id));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));

  EXPECT_CALL(*mock_policy_manager_, IsPredataPolicy(kPolicyAppId_))
      .WillOnce(Return(true));

  EXPECT_CALL(*mock_policy_manager_,
              ReactOnUserDevConsentForApp(kPolicyAppId_, is_allowed));
  EXPECT_CALL(*mock_policy_manager_,
              SendNotificationOnPermissionsUpdated(kPolicyAppId_));

  policy_handler_.OnDeviceConsentChanged(kPolicyAppId_, is_allowed);
}

TEST_F(PolicyHandlerTest, OnDeviceConsentChanged_ConsentNotAllowed) {
  const bool is_allowed = false;
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  connection_handler::DeviceHandle test_device_id = 100u;
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));

  // Check expectations
  EXPECT_CALL(conn_handler, GetDeviceID(kPolicyAppId_, _))
      .WillOnce(DoAll(SetArgPointee<1>(test_device_id), Return(true)));
  test_app.insert(mock_app_);

  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(test_device_id));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));

  EXPECT_CALL(*mock_policy_manager_, IsPredataPolicy(kPolicyAppId_))
      .WillOnce(Return(true));

  EXPECT_CALL(*mock_policy_manager_, ReactOnUserDevConsentForApp(_, _))
      .Times(0);
  EXPECT_CALL(*mock_policy_manager_, SendNotificationOnPermissionsUpdated(_))
      .Times(0);

  policy_handler_.OnDeviceConsentChanged(kPolicyAppId_, is_allowed);
}

TEST_F(PolicyHandlerTest, OnDeviceConsentChanged_PredatePolicyNotAllowed) {
  const bool is_allowed = false;
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  connection_handler::DeviceHandle test_device_id = 100u;
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));

  EXPECT_CALL(conn_handler, GetDeviceID(kPolicyAppId_, _))
      .WillOnce(DoAll(SetArgPointee<1>(test_device_id), Return(true)));

  test_app.insert(mock_app_);

  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(test_device_id));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));

  // App does not have predate policy
  EXPECT_CALL(*mock_policy_manager_, IsPredataPolicy(kPolicyAppId_))
      .WillOnce(Return(false));

  EXPECT_CALL(*mock_policy_manager_,
              ReactOnUserDevConsentForApp(kPolicyAppId_, is_allowed));
  EXPECT_CALL(*mock_policy_manager_,
              SendNotificationOnPermissionsUpdated(kPolicyAppId_));

  policy_handler_.OnDeviceConsentChanged(kPolicyAppId_, is_allowed);
}
#ifdef EXTERNAL_PROPRIETARY_MODE
TEST_F(PolicyHandlerTest, OnCertificateUpdated) {
  const std::string app_storage = "storage";
  file_system::CreateFile("storage/certificate");
  EXPECT_CALL(policy_settings_, app_storage_folder())
      .WillOnce(ReturnRef(app_storage));

  const std::string cert_data = "data";

  const std::string full_file_name =
      file_system::GetAbsolutePath(app_storage) + "/certificate";
  EXPECT_CALL(mock_message_helper_,
              SendDecryptCertificateToHMI(full_file_name, _));
  policy_handler_.OnCertificateUpdated(cert_data);
}
#else
TEST_F(PolicyHandlerTest, OnCertificateUpdated) {
  application_manager_test::MockPolicyHandlerObserver policy_handler_observer;
  policy_handler_.add_listener(&policy_handler_observer);
  const std::string cert_data = "data";
  EnablePolicy();

  EXPECT_CALL(policy_handler_observer, OnCertificateUpdated(cert_data));
  policy_handler_.OnCertificateUpdated(cert_data);
}
#endif

TEST_F(PolicyHandlerTest, GetAppIdForSending_WithoutApps) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  EXPECT_CALL(app_manager_, applications()).WillRepeatedly(Return(app_set));
  // Set does not include any applications
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(_)).Times(0);
  EXPECT_EQ(0u, policy_handler_.GetAppIdForSending());
}

TEST_F(PolicyHandlerTest, GetAppIdForSending_GetDefaultMacAddress) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  // Set does not include any applications
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(_)).Times(0);
  // Check expectations
  test_app.insert(mock_app_);
  EXPECT_CALL(*mock_app_, IsRegistered()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));
  EXPECT_CALL(*mock_app_, hmi_level())
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_FULL));
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));

  EXPECT_CALL(conn_handler, get_session_observer())
      .WillOnce(ReturnRef(mock_session_observer));

  EXPECT_CALL(app_manager_, applications()).WillRepeatedly(Return(app_set));

  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(_))
      .WillRepeatedly(Return(kDeviceAllowed));
  // Act
  EXPECT_EQ(kAppId1_, policy_handler_.GetAppIdForSending());
}

void PolicyHandlerTest::GetAppIDForSending() {
  EnablePolicyAndPolicyManagerMock();
  test_app.insert(mock_app_);

  // Check expectations
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));
  EXPECT_CALL(*mock_app_, IsRegistered()).WillOnce(Return(true));
  EXPECT_CALL(*mock_app_, hmi_level())
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_FULL));
  EXPECT_CALL(mock_session_observer,
              GetDataOnDeviceID(
                  testing::An<transport_manager::DeviceHandle>(), _, _, _, _))
      .WillOnce(DoAll(SetArgPointee<3>(kMacAddr_), Return(0)));

  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(kMacAddr_))
      .WillRepeatedly(Return(kDeviceAllowed));
}

TEST_F(PolicyHandlerTest, GetAppIdForSending_SetMacAddress) {
  // Arrange
  GetAppIDForSending();
  // Act
  EXPECT_EQ(kAppId1_, policy_handler_.GetAppIdForSending());
}

TEST_F(PolicyHandlerTest, GetAppIdForSending_ExpectReturnAnyIdButNone) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  utils::SharedPtr<application_manager_test::MockApplication> mock_app_in_full =
      utils::MakeShared<application_manager_test::MockApplication>();
  const uint32_t app_in_full_id = 1;
  EXPECT_CALL(*mock_app_in_full, app_id())
      .WillRepeatedly(Return(app_in_full_id));
  EXPECT_CALL(*mock_app_in_full, hmi_level())
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_FULL));
  ON_CALL(*mock_app_in_full, IsRegistered()).WillByDefault(Return(true));

  test_app.insert(mock_app_in_full);

  utils::SharedPtr<application_manager_test::MockApplication>
      mock_app_in_limited =
          utils::MakeShared<application_manager_test::MockApplication>();
  const uint32_t app_in_limited_id = 2;
  EXPECT_CALL(*mock_app_in_limited, app_id())
      .WillRepeatedly(Return(app_in_limited_id));
  EXPECT_CALL(*mock_app_in_limited, hmi_level())
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_LIMITED));
  ON_CALL(*mock_app_in_limited, IsRegistered()).WillByDefault(Return(true));

  test_app.insert(mock_app_in_limited);

  utils::SharedPtr<application_manager_test::MockApplication>
      mock_app_in_background =
          utils::MakeShared<application_manager_test::MockApplication>();
  const uint32_t app_in_background_id = 3;
  EXPECT_CALL(*mock_app_in_background, app_id())
      .WillRepeatedly(Return(app_in_background_id));
  EXPECT_CALL(*mock_app_in_background, hmi_level())
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_BACKGROUND));
  ON_CALL(*mock_app_in_background, IsRegistered()).WillByDefault(Return(true));

  test_app.insert(mock_app_in_background);

  utils::SharedPtr<application_manager_test::MockApplication> mock_app_in_none =
      utils::MakeShared<application_manager_test::MockApplication>();
  const uint32_t app_in_none_id = 4;
  EXPECT_CALL(*mock_app_in_none, app_id())
      .WillRepeatedly(Return(app_in_none_id));
  EXPECT_CALL(*mock_app_in_none, hmi_level())
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_NONE));
  EXPECT_CALL(*mock_app_in_none, IsRegistered()).Times(0);

  test_app.insert(mock_app_in_none);

  // Check expectations

  EXPECT_CALL(mock_session_observer,
              GetDataOnDeviceID(
                  testing::An<transport_manager::DeviceHandle>(), _, _, _, _))
      .WillOnce(DoAll(SetArgPointee<3>(kMacAddr_), Return(0)));

  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(kMacAddr_))
      .WillRepeatedly(Return(kDeviceAllowed));

  // Act
  EXPECT_NE(app_in_none_id, policy_handler_.GetAppIdForSending());
}

TEST_F(PolicyHandlerTest, GetAppIdForSending_ExpectReturnAnyAppInNone) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  utils::SharedPtr<application_manager_test::MockApplication>
      mock_app_in_none_1 =
          utils::MakeShared<application_manager_test::MockApplication>();
  const uint32_t app_in_none_id_1 = 1;
  EXPECT_CALL(*mock_app_in_none_1, app_id())
      .WillRepeatedly(Return(app_in_none_id_1));
  EXPECT_CALL(*mock_app_in_none_1, hmi_level())
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_FULL));
  ON_CALL(*mock_app_in_none_1, IsRegistered()).WillByDefault(Return(true));

  test_app.insert(mock_app_in_none_1);

  utils::SharedPtr<application_manager_test::MockApplication>
      mock_app_in_none_2 =
          utils::MakeShared<application_manager_test::MockApplication>();
  const uint32_t app_in_none_id_2 = 2;
  EXPECT_CALL(*mock_app_in_none_2, app_id())
      .WillRepeatedly(Return(app_in_none_id_2));
  EXPECT_CALL(*mock_app_in_none_2, hmi_level())
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_NONE));
  ON_CALL(*mock_app_in_none_2, IsRegistered()).WillByDefault(Return(true));

  test_app.insert(mock_app_in_none_2);

  // Check expectations

  EXPECT_CALL(mock_session_observer,
              GetDataOnDeviceID(
                  testing::An<transport_manager::DeviceHandle>(), _, _, _, _))
      .WillOnce(DoAll(SetArgPointee<3>(kMacAddr_), Return(0)));

  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(kMacAddr_))
      .WillRepeatedly(Return(kDeviceAllowed));

  // Act

  const uint32_t app_id = policy_handler_.GetAppIdForSending();

  EXPECT_EQ(app_in_none_id_1 || app_in_none_id_2, app_id);
}

TEST_F(PolicyHandlerTest,
       SendMessageToSDK_SuitableAppPresent_ExpectedNotificationSending) {
  BinaryMessage msg;
  const std::string url = "test_url";
  EnablePolicyAndPolicyManagerMock();
  test_app.insert(mock_app_);

  EXPECT_CALL(app_manager_, application(kAppId1_))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));
  EXPECT_CALL(*mock_app_, policy_app_id())
      .WillRepeatedly(Return(kPolicyAppId_));
  EXPECT_CALL(*mock_app_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_CALL(*mock_app_, IsRegistered()).WillRepeatedly(Return(true));

  const connection_handler::DeviceHandle test_device_id = 1u;
  EXPECT_CALL(*mock_app_, device()).WillRepeatedly(Return(test_device_id));
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(_))
      .WillOnce(Return(kDeviceAllowed));

  // Act
  EXPECT_CALL(mock_message_helper_,
              SendPolicySnapshotNotification(kAppId1_, msg, url, _));
  EXPECT_TRUE(policy_handler_.SendMessageToSDK(msg, url));
}

TEST_F(PolicyHandlerTest,
       SendMessageToSDK_NoSuitableApp_ExpectedNotificationNotSent) {
  BinaryMessage msg;
  const std::string url = "test_url";
  EnablePolicyAndPolicyManagerMock();
  test_app.insert(mock_app_);

  EXPECT_CALL(app_manager_, application(kAppId1_))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));
  EXPECT_CALL(*mock_app_, policy_app_id())
      .WillRepeatedly(Return(kPolicyAppId_));
  EXPECT_CALL(*mock_app_, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_NONE));
  EXPECT_CALL(*mock_app_, IsRegistered()).WillRepeatedly(Return(true));

  const connection_handler::DeviceHandle test_device_id = 1u;
  EXPECT_CALL(*mock_app_, device()).WillRepeatedly(Return(test_device_id));
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(_))
      .WillOnce(Return(kDeviceDisallowed));

  // Expected to get 0 as application id so SDL does not have valid application
  // with such id
  EXPECT_CALL(app_manager_, application(0))
      .WillOnce(Return(
          utils::SharedPtr<application_manager_test::MockApplication>()));

  EXPECT_FALSE(policy_handler_.SendMessageToSDK(msg, url));
}

TEST_F(PolicyHandlerTest, CanUpdate) {
  GetAppIDForSending();
  EXPECT_TRUE(policy_handler_.CanUpdate());
}

TEST_F(PolicyHandlerTest, CanUpdate_TwoApplicationForSending_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  utils::SharedPtr<application_manager_test::MockApplication> second_mock_app =
      utils::MakeShared<application_manager_test::MockApplication>();

  EXPECT_CALL(*mock_app_, hmi_level())
      .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_CALL(*second_mock_app, hmi_level())
      .WillRepeatedly(Return(mobile_apis::HMILevel::HMI_LIMITED));

  EXPECT_CALL(*second_mock_app, app_id()).WillRepeatedly(Return(kAppId2_));
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));

  // Check expectations
  EXPECT_CALL(*mock_app_, IsRegistered()).WillOnce(Return(true));
  test_app.insert(mock_app_);
  test_app.insert(second_mock_app);

  EXPECT_CALL(mock_session_observer,
              GetDataOnDeviceID(
                  testing::An<transport_manager::DeviceHandle>(), _, _, _, _))
      .WillOnce(DoAll(SetArgPointee<3>(kMacAddr_), Return(0)));

  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(kMacAddr_))
      .WillRepeatedly(Return(kDeviceAllowed));

  EXPECT_TRUE(policy_handler_.CanUpdate());
}

ACTION_P(NotifyAsync, waiter) {
  waiter->Notify();
}

TEST_F(PolicyHandlerTest,
       DISABLED_OnAppPermissionConsentInternal_ValidConnectionKey_SUCCESS) {
  ChangePolicyManagerToMock();
  const uint32_t device = 2u;

  PermissionConsent permissions;
  permissions.device_id = kDeviceId_;
  permissions.consent_source = "consent_source";

  policy::FunctionalGroupPermission group_permission_allowed;
  CreateFunctionalGroupPermission(GroupConsent::kGroupAllowed,
                                  kGroupAliasAllowed_,
                                  kGroupNameAllowed_,
                                  group_permission_allowed);

  permissions.group_permissions.push_back(group_permission_allowed);

  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));
  EXPECT_CALL(conn_handler, get_session_observer())
      .WillOnce(ReturnRef(mock_session_observer));
  EXPECT_CALL(mock_session_observer,
              GetDataOnDeviceID(
                  testing::Matcher<transport_manager::DeviceHandle>(device),
                  _,
                  NULL,
                  _,
                  _)).WillOnce(Return(1u));

  EXPECT_CALL(app_manager_, application(kConnectionKey_))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(device));

  sync_primitives::Lock wait_hmi_lock_first;
  sync_primitives::AutoLock auto_lock_first(wait_hmi_lock_first);
  WaitAsync waiter_first(kCallsCount_, kTimeout_);
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(*mock_policy_manager_, SetUserConsentForApp(_, _))
      .WillOnce(NotifyAsync(&waiter_first));
#else
  EXPECT_CALL(*mock_policy_manager_, SetUserConsentForApp(_))
      .WillOnce(NotifyAsync(&waiter_first));
#endif
  ExternalConsentStatusItem item(1u, 1u, kStatusOn);
  ExternalConsentStatus external_consent_status;
  external_consent_status.insert(item);

#ifdef EXTERNAL_PROPRIETARY_MODE
  sync_primitives::Lock wait_hmi_lock_second;
  sync_primitives::AutoLock auto_lock_second(wait_hmi_lock_second);
  WaitAsync waiter_second(kCallsCount_, kTimeout_);

  EXPECT_CALL(*mock_policy_manager_,
              SetExternalConsentStatus(external_consent_status))
      .WillOnce(Return(true));
  policy_handler_.OnAppPermissionConsent(
      kConnectionKey_, permissions, external_consent_status);
#else
  policy_handler_.OnAppPermissionConsent(kConnectionKey_, permissions);

#endif
  EXPECT_TRUE(waiter_first.Wait(auto_lock_first));
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_TRUE(waiter_second.Wait(auto_lock_second));
#endif
}

TEST_F(PolicyHandlerTest,
       OnAppPermissionConsentInternal_NoAppsPreviouslyStored_UNUSUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  const uint32_t invalid_connection_key = 0u;

  PermissionConsent permissions;
  permissions.device_id = kDeviceId_;
  permissions.consent_source = "consent_source";

  policy::FunctionalGroupPermission group_permission_allowed;
  CreateFunctionalGroupPermission(GroupConsent::kGroupAllowed,
                                  kGroupAliasAllowed_,
                                  kGroupNameAllowed_,
                                  group_permission_allowed);

  permissions.group_permissions.push_back(group_permission_allowed);

  sync_primitives::Lock wait_hmi_lock;
  sync_primitives::AutoLock auto_lock(wait_hmi_lock);
  WaitAsync waiter(kCallsCount_, kTimeout_);

  EXPECT_CALL(app_manager_, application(_)).Times(0);

  ExternalConsentStatusItem item = {1u, 1u, kStatusOn};
  ExternalConsentStatus external_consent_status;
  external_consent_status.insert(item);
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(*mock_policy_manager_,
              SetExternalConsentStatus(external_consent_status))
      .WillOnce(Return(true));
  policy_handler_.OnAppPermissionConsent(
      invalid_connection_key, permissions, external_consent_status);
#else
  policy_handler_.OnAppPermissionConsent(invalid_connection_key, permissions);
#endif

  EXPECT_FALSE(waiter.Wait(auto_lock));
}

TEST_F(PolicyHandlerTest,
       OnAppPermissionConsentInternal_NoAppsPreviouslyStored_FAILED) {
  EnablePolicyAndPolicyManagerMock();
  const uint32_t invalid_connection_key = 0u;

  PermissionConsent permissions;
  permissions.device_id = kDeviceId_;
  permissions.consent_source = "consent_source";

  policy::FunctionalGroupPermission group_permission_allowed;
  CreateFunctionalGroupPermission(GroupConsent::kGroupAllowed,
                                  kGroupAliasAllowed_,
                                  kGroupNameAllowed_,
                                  group_permission_allowed);

  permissions.group_permissions.push_back(group_permission_allowed);

  sync_primitives::Lock wait_hmi_lock;
  sync_primitives::AutoLock auto_lock(wait_hmi_lock);
  WaitAsync waiter(kCallsCount_, kTimeout_);

  EXPECT_CALL(app_manager_, application(_)).Times(0);

  ExternalConsentStatusItem item = {1u, 1u, kStatusOn};
  ExternalConsentStatus external_consent_status;
  external_consent_status.insert(item);
#ifdef EXTERNAL_PROPRIETARY_MODE
  ON_CALL(*mock_policy_manager_, IsNeedToUpdateExternalConsentStatus(_))
      .WillByDefault(Return(false));
  EXPECT_CALL(*mock_policy_manager_,
              SetExternalConsentStatus(external_consent_status))
      .WillOnce(Return(true));
  policy_handler_.OnAppPermissionConsent(
      invalid_connection_key, permissions, external_consent_status);
#else
  policy_handler_.OnAppPermissionConsent(invalid_connection_key, permissions);
#endif

  EXPECT_FALSE(waiter.Wait(auto_lock));
}

ACTION_P(SetDeviceParamsMacAdress, mac_adress) {
  *arg3 = mac_adress;
}

TEST_F(PolicyHandlerTest,
       OnAppPermissionConsentInternal_ExistAppsPreviouslyStored_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));

  test_app.insert(mock_app_);

  const uint32_t invalid_connection_key = 0u;
  const uint32_t device = 2u;

  PermissionConsent permissions;
  permissions.device_id = kDeviceId_;
  permissions.consent_source = "consent_source";

  policy::FunctionalGroupPermission group_permission_allowed;
  CreateFunctionalGroupPermission(GroupConsent::kGroupAllowed,
                                  kGroupAliasAllowed_,
                                  kGroupNameAllowed_,
                                  group_permission_allowed);

  permissions.group_permissions.push_back(group_permission_allowed);
  EXPECT_CALL(app_manager_, applications()).WillRepeatedly(Return(app_set));

  EXPECT_CALL(*mock_app_, device()).WillRepeatedly(Return(1u));
  EXPECT_CALL(*mock_app_, policy_app_id())
      .WillRepeatedly(Return(kPolicyAppId_));

  EXPECT_CALL(
      mock_session_observer,
      GetDataOnDeviceID(
          testing::An<transport_manager::DeviceHandle>(), _, NULL, _, _))
      .WillRepeatedly(DoAll(SetDeviceParamsMacAdress(kMacAddr_), (Return(1u))));

  EXPECT_CALL(app_manager_, connection_handler())
      .WillRepeatedly(ReturnRef(conn_handler));
  EXPECT_CALL(conn_handler, get_session_observer())
      .WillRepeatedly(ReturnRef(mock_session_observer));

  EXPECT_CALL(*mock_app_, device()).WillRepeatedly(Return(device));

  ExternalConsentStatusItem item = {1u, 1u, kStatusOn};
  ExternalConsentStatus external_consent_status;
  external_consent_status.insert(item);
#ifdef EXTERNAL_PROPRIETARY_MODE
  sync_primitives::Lock wait_hmi_lock;
  sync_primitives::AutoLock auto_lock(wait_hmi_lock);
  WaitAsync waiter(kCallsCount_, kTimeout_);

  EXPECT_CALL(*mock_policy_manager_,
              SetExternalConsentStatus(external_consent_status))
      .WillOnce(DoAll(NotifyAsync(&waiter), Return(true)));
  policy_handler_.OnAppPermissionConsent(
      invalid_connection_key, permissions, external_consent_status);
#else
  policy_handler_.OnAppPermissionConsent(invalid_connection_key, permissions);
#endif

  Mock::VerifyAndClearExpectations(mock_app_.get());
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_TRUE(waiter.Wait(auto_lock));
#endif
}

TEST_F(PolicyHandlerTest,
       OnAppPermissionConsentInternal_ExistAppsPreviouslyStored_FAIL) {
  EnablePolicyAndPolicyManagerMock();

  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));

  test_app.insert(mock_app_);

  const uint32_t invalid_connection_key = 0u;
  const uint32_t device = 2u;

  PermissionConsent permissions;
  permissions.device_id = kDeviceId_;
  permissions.consent_source = "consent_source";

  policy::FunctionalGroupPermission group_permission_allowed;
  CreateFunctionalGroupPermission(GroupConsent::kGroupAllowed,
                                  kGroupAliasAllowed_,
                                  kGroupNameAllowed_,
                                  group_permission_allowed);

  permissions.group_permissions.push_back(group_permission_allowed);
  EXPECT_CALL(app_manager_, applications()).WillRepeatedly(Return(app_set));

  EXPECT_CALL(*mock_app_, device()).WillRepeatedly(Return(1u));
  EXPECT_CALL(*mock_app_, policy_app_id())
      .WillRepeatedly(Return(kPolicyAppId_));

  EXPECT_CALL(
      mock_session_observer,
      GetDataOnDeviceID(
          testing::An<transport_manager::DeviceHandle>(), _, NULL, _, _))
      .WillRepeatedly(DoAll(SetDeviceParamsMacAdress(kMacAddr_), (Return(1u))));

  EXPECT_CALL(app_manager_, connection_handler())
      .WillRepeatedly(ReturnRef(conn_handler));
  EXPECT_CALL(conn_handler, get_session_observer())
      .WillRepeatedly(ReturnRef(mock_session_observer));

  EXPECT_CALL(*mock_app_, device()).WillRepeatedly(Return(device));

  ExternalConsentStatusItem item = {1u, 1u, kStatusOn};
  ExternalConsentStatus external_consent_status;
  external_consent_status.insert(item);
#ifdef EXTERNAL_PROPRIETARY_MODE
  sync_primitives::Lock wait_hmi_lock;
  sync_primitives::AutoLock auto_lock(wait_hmi_lock);
  WaitAsync waiter(kCallsCount_, kTimeout_);

  ON_CALL(*mock_policy_manager_, IsNeedToUpdateExternalConsentStatus(_))
      .WillByDefault(Return(false));
  EXPECT_CALL(*mock_policy_manager_,
              SetExternalConsentStatus(external_consent_status))
      .WillOnce(Return(true));
  policy_handler_.OnAppPermissionConsent(
      invalid_connection_key, permissions, external_consent_status);
#else
  policy_handler_.OnAppPermissionConsent(invalid_connection_key, permissions);
#endif

  Mock::VerifyAndClearExpectations(mock_app_.get());
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_FALSE(waiter.Wait(auto_lock));
#endif
}

TEST_F(PolicyHandlerTest, GetLockScreenIconUrl_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(*mock_policy_manager_, GetLockScreenIconUrl());

  policy_handler_.GetLockScreenIconUrl();
}

TEST_F(PolicyHandlerTest, RemoveListener_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  application_manager_test::MockPolicyHandlerObserver policy_handler_observer;
  policy_handler_.add_listener(&policy_handler_observer);
  std::map<std::string, StringArray> app_hmi_types;
  StringArray arr;
  arr.push_back("test_hmi_type");
  app_hmi_types["app1"] = arr;

  EXPECT_CALL(policy_handler_observer, OnUpdateHMIAppType(_));
  policy_handler_.OnUpdateHMIAppType(app_hmi_types);

  policy_handler_.remove_listener(&policy_handler_observer);

  EXPECT_CALL(policy_handler_observer, OnUpdateHMIAppType(_)).Times(0);
  policy_handler_.OnUpdateHMIAppType(app_hmi_types);
}

TEST_F(PolicyHandlerTest, AddStatisticsInfo_UnknownStatistics_UNSUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  policy_handler_.AddStatisticsInfo(
      hmi_apis::Common_StatisticsType::INVALID_ENUM);
}

TEST_F(PolicyHandlerTest, AddStatisticsInfo_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  sync_primitives::Lock wait_hmi_lock;
  sync_primitives::AutoLock auto_lock(wait_hmi_lock);
  WaitAsync waiter(kCallsCount_, kTimeout_);

  EXPECT_CALL(*mock_policy_manager_, Increment(_))
      .WillOnce(NotifyAsync(&waiter));

  policy_handler_.AddStatisticsInfo(
      hmi_apis::Common_StatisticsType::iAPP_BUFFER_FULL);
  EXPECT_TRUE(waiter.Wait(auto_lock));
}

TEST_F(PolicyHandlerTest, OnSystemError_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  sync_primitives::Lock wait_hmi_lock;
  sync_primitives::AutoLock auto_lock(wait_hmi_lock);
  WaitAsync waiter(kCallsCount_, kTimeout_);
  EXPECT_CALL(*mock_policy_manager_, Increment(_))
      .WillOnce(NotifyAsync(&waiter));

  policy_handler_.OnSystemError(hmi_apis::Common_SystemError::SYNC_REBOOTED);
  EXPECT_TRUE(waiter.Wait(auto_lock));

  WaitAsync waiter1(kCallsCount_, kTimeout_);
  EXPECT_CALL(*mock_policy_manager_, Increment(_))
      .WillOnce(NotifyAsync(&waiter1));

  policy_handler_.OnSystemError(
      hmi_apis::Common_SystemError::SYNC_OUT_OF_MEMMORY);
  EXPECT_TRUE(waiter1.Wait(auto_lock));
}

ACTION_P(SetEndpoint, endpoint) {
  arg1 = endpoint;
}

TEST_F(PolicyHandlerTest, RemoteAppsUrl_EndpointsEmpty_UNSUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  const std::string service_type("queryAppsUrl");
  EndpointUrls endpoints;

  EXPECT_CALL(*mock_policy_manager_, GetUpdateUrls(service_type, _))
      .WillOnce(SetEndpoint(endpoints));

  const std::string default_url("");
  EXPECT_EQ(default_url, policy_handler_.RemoteAppsUrl());
}

TEST_F(PolicyHandlerTest, RemoteAppsUrl_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  const std::string url("url");
  EndpointData endpoint_data;
  endpoint_data.url.push_back(url);

  EndpointUrls endpoints;
  endpoints.push_back(endpoint_data);

  const std::string service_type("queryAppsUrl");
  EXPECT_CALL(*mock_policy_manager_, GetUpdateUrls(service_type, _))
      .WillOnce(SetEndpoint(endpoints));

  EXPECT_EQ(url, policy_handler_.RemoteAppsUrl());
}

}  // namespace policy_handler_test
}  // namespace components
}  // namespace test

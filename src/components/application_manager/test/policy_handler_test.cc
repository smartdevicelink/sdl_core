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

#include <fstream>
#include <memory>
#include <string>
#include <vector>
#include "gmock/gmock.h"

#include "application_manager/application_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/policies/delegates/app_permission_delegate.h"
#include "application_manager/policies/policy_handler.h"
#include "connection_handler/connection_handler_impl.h"
#ifdef ENABLE_SECURITY
#include "security_manager/mock_crypto_manager.h"
#include "security_manager/mock_security_manager.h"
#endif  // ENABLE_SECURITY
#include "application_manager/mock_message_helper.h"
#include "connection_handler/mock_connection_handler_settings.h"
#include "json/reader.h"
#include "json/value.h"
#include "json/writer.h"
#include "policy/policy_types.h"
#include "smart_objects/smart_object.h"
#include "transport_manager/mock_transport_manager.h"
#include "utils/file_system.h"

#include "interfaces/MOBILE_API.h"
#include "policy/mock_policy_settings.h"
#include "policy/usage_statistics/counter.h"
#include "policy/usage_statistics/statistics_manager.h"
#include "utils/custom_string.h"

#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_hmi_capabilities.h"
#include "application_manager/mock_rpc_service.h"
#include "application_manager/mock_state_controller.h"
#include "application_manager/policies/mock_policy_handler_observer.h"
#include "connection_handler/mock_connection_handler.h"
#include "policy/mock_policy_manager.h"
#include "policy/usage_statistics/mock_statistics_manager.h"
#include "protocol_handler/mock_session_observer.h"
#include "utils/test_async_waiter.h"

#include "smart_objects/enum_schema_item.h"

namespace test {
namespace components {
namespace policy_handler_test {

using namespace application_manager;
using namespace policy;
using namespace utils::custom_string;
using testing::_;
using ::testing::DoAll;
using ::testing::Mock;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgPointee;
using ::testing::SetArgReferee;

typedef NiceMock<application_manager_test::MockRPCService> MockRPCService;

namespace {
const std::string kFakeNickname = "fake_nickname";
const std::string kDummyData = "some_data";
const WindowID kDefaultWindowId =
    mobile_apis::PredefinedWindows::DEFAULT_WINDOW;
}  // namespace

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
      , app_lock_(std::make_shared<sync_primitives::Lock>())
      , app_set(test_app, app_lock_)
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
  std::shared_ptr<application_manager_test::MockApplication> mock_app_;
  connection_handler_test::MockConnectionHandler conn_handler;
  protocol_handler_test::MockSessionObserver mock_session_observer;
  application_manager_test::MockStateController mock_state_controller;
  components::usage_statistics_test::MockStatisticsManager
      mock_statistics_manager_;
  PolicyHandler policy_handler_;
  std::shared_ptr<policy_manager_test::MockPolicyManager> mock_policy_manager_;
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
  std::shared_ptr<sync_primitives::Lock> app_lock_;
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
  MockRPCService mock_rpc_service_;

  virtual void SetUp() OVERRIDE {
    Mock::VerifyAndClearExpectations(&mock_message_helper_);
    ON_CALL(app_manager_, applications()).WillByDefault(Return(app_set));
    ON_CALL(policy_settings_, enable_policy()).WillByDefault(Return(true));
    ON_CALL(app_manager_, event_dispatcher())
        .WillByDefault(ReturnRef(mock_event_dispatcher_));
    const std::string path("storage");
    if (file_system::CreateDirectory(path))
      file_system::CreateFile(path + "/" + "certificate");
    mock_policy_manager_ =
        std::make_shared<policy_manager_test::MockPolicyManager>();
    ASSERT_TRUE(mock_policy_manager_.use_count() != 0);

    ON_CALL(app_manager_, connection_handler())
        .WillByDefault(ReturnRef(conn_handler));
    ON_CALL(conn_handler, get_session_observer())
        .WillByDefault(ReturnRef(mock_session_observer));

    mock_app_ = std::make_shared<application_manager_test::MockApplication>();
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
    EXPECT_CALL(app_manager_, application(kDeviceId, kPolicyAppId_))
        .WillRepeatedly(Return(mock_app_));
    EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));
    EXPECT_CALL(*mock_app_, hmi_level(kDefaultWindowId))
        .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));

    ChangePolicyManagerToMock();
    const policy::EncryptionRequired require_encryption;
    EXPECT_CALL(*mock_policy_manager_, GetAppEncryptionRequired(kPolicyAppId_))
        .WillOnce(Return(require_encryption));
    EXPECT_CALL(mock_message_helper_,
                SendOnPermissionsChangeNotification(kAppId1_, _, _, _));
    EXPECT_CALL(app_manager_, state_controller()).Times(0);

    Permissions permissions;
    policy_handler_.OnPermissionsUpdated(
        kDeviceId, kPolicyAppId_, permissions, default_hmi_level);
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

  void SetExpectationsAndCheckCloudAppPropertiesStatus(
      const policy::AppProperties& app_properties,
      const smart_objects::SmartObject& properties,
      const policy::PolicyHandlerInterface::AppPropertiesState&
          app_properties_state) {
    EXPECT_CALL(*mock_policy_manager_, GetAppProperties(kPolicyAppId_, _))
        .WillOnce(DoAll(SetArgReferee<1>(app_properties), Return(true)));
    EXPECT_CALL(*mock_policy_manager_, GetInitialAppData(kPolicyAppId_, _, _));
    EXPECT_EQ(
        app_properties_state,
        policy_handler_.GetAppPropertiesStatus(properties, kPolicyAppId_));
  }
};

ACTION_P(SetDeviceParamsMacAdress, mac_adress) {
  *arg3 = mac_adress;
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

TEST_F(PolicyHandlerTest, AppServiceUpdate_CheckAppService) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(policy_handler_.LoadPolicyLibrary());
  // Check
  EXPECT_TRUE(policy_handler_.InitPolicyTable());
  ChangePolicyManagerToMock();
  std::string file_name("sdl_pt_update.json");
  std::ifstream ifile(file_name);
  Json::CharReaderBuilder reader_builder;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() &&
      Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
    json = root.toStyledString();
  }
  ifile.close();
  BinaryMessage msg(json.begin(), json.end());
  // Checks
  EXPECT_CALL(*mock_policy_manager_, LoadPT("", msg))
      .WillOnce(Return(PolicyManager::PtProcessingResult::kSuccess));
  EXPECT_CALL(*mock_policy_manager_,
              OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess));
  policy_handler_.ReceiveMessageFromSDK("", msg);

  policy_table::AppServiceParameters app_service_params =
      policy_table::AppServiceParameters();
  std::string kServiceType = "MEDIA";
  std::string as_app_id = "1010101010";
  std::string service_name1 = "SDL Music";
  std::string service_name2 = "SDL App";
  (app_service_params)[kServiceType] = policy_table::AppServiceInfo();
  (app_service_params)[kServiceType].service_names->push_back(service_name2);
  (app_service_params)[kServiceType].service_names->push_back(service_name1);
  (app_service_params)[kServiceType].service_names->mark_initialized();
  policy_table::AppServiceHandledRpc handled_rpc;
  handled_rpc.function_id = 41;
  (app_service_params)[kServiceType].handled_rpcs.push_back(handled_rpc);
  EXPECT_CALL(*mock_policy_manager_, GetAppServiceParameters(_, _))
      .WillRepeatedly(SetArgPointee<1>(app_service_params));

  ns_smart_device_link::ns_smart_objects::SmartArray requested_handled_rpcs;
  ns_smart_device_link::ns_smart_objects::SmartObject rpc_id(41);
  requested_handled_rpcs.push_back(rpc_id);

  ns_smart_device_link::ns_smart_objects::SmartArray fake_handled_rpcs;
  ns_smart_device_link::ns_smart_objects::SmartObject fake_rpc_id(40);
  fake_handled_rpcs.push_back(fake_rpc_id);

  EXPECT_TRUE(policy_handler_.CheckAppServiceParameters(
      as_app_id, service_name1, kServiceType, &requested_handled_rpcs));
  EXPECT_TRUE(policy_handler_.CheckAppServiceParameters(
      as_app_id, service_name2, kServiceType, &requested_handled_rpcs));
  EXPECT_TRUE(policy_handler_.CheckAppServiceParameters(
      as_app_id, service_name2, kServiceType, NULL));
  EXPECT_TRUE(policy_handler_.CheckAppServiceParameters(
      as_app_id, "", kServiceType, NULL));

  EXPECT_FALSE(
      policy_handler_.CheckAppServiceParameters(as_app_id, "", "", NULL));
  EXPECT_FALSE(policy_handler_.CheckAppServiceParameters(
      as_app_id, service_name2, "NAVIGATION", &requested_handled_rpcs));
  EXPECT_FALSE(policy_handler_.CheckAppServiceParameters(
      as_app_id, "MUSIC", kServiceType, &requested_handled_rpcs));
  EXPECT_FALSE(policy_handler_.CheckAppServiceParameters(
      as_app_id, service_name2, kServiceType, &fake_handled_rpcs));
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
  Json::CharReaderBuilder reader_builder;

  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() &&
      Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
    json = root.toStyledString();
  }
  ifile.close();
  BinaryMessage msg(json.begin(), json.end());
  // Checks
  EXPECT_CALL(app_manager_, GetNextHMICorrelationID());
  EXPECT_CALL(mock_message_helper_, CreateGetVehicleDataRequest(_, _, _));
  EXPECT_CALL(*mock_policy_manager_, PTUpdatedAt(_, _));
  EXPECT_CALL(*mock_policy_manager_, LoadPT("", msg))
      .WillOnce(Return(PolicyManager::PtProcessingResult::kSuccess));
  EXPECT_CALL(*mock_policy_manager_,
              OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess));
  EXPECT_CALL(*mock_policy_manager_, CleanupUnpairedDevices());
  policy_handler_.ReceiveMessageFromSDK("", msg);
}

TEST_F(PolicyHandlerTest, ReceiveMessageFromSDK_PTNotLoaded) {
  // Arrange
  ChangePolicyManagerToMock();
  BinaryMessage msg;
  // Checks

  EXPECT_CALL(*mock_policy_manager_, LoadPT("", msg))
      .WillOnce(Return(PolicyManager::PtProcessingResult::kWrongPtReceived));
  EXPECT_CALL(
      *mock_policy_manager_,
      OnPTUFinished(PolicyManager::PtProcessingResult::kWrongPtReceived));
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
  ChangePolicyManagerToMock();
  const policy::EncryptionRequired require_encryption;
  EXPECT_CALL(*mock_policy_manager_, GetAppEncryptionRequired(kPolicyAppId_))
      .WillOnce(Return(require_encryption));
  EXPECT_CALL(app_manager_, application(kDeviceId, kPolicyAppId_))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _, _));
  // Act
  Permissions perms;
  policy_handler_.OnPermissionsUpdated(kDeviceId, kPolicyAppId_, perms);
}

TEST_F(PolicyHandlerTest, OnPermissionsUpdated_TwoParams_InvalidApp_UNSUCCESS) {
  std::shared_ptr<application_manager_test::MockApplication> invalid_app;
  EXPECT_CALL(app_manager_, application(kDeviceId, kPolicyAppId_))
      .WillOnce(Return(invalid_app));
  ChangePolicyManagerToMock();
  const policy::EncryptionRequired require_encryption;
  EXPECT_CALL(*mock_policy_manager_, GetAppEncryptionRequired(kPolicyAppId_))
      .Times(0);
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(_, _, _, _))
      .Times(0);

  Permissions permissions;
  policy_handler_.OnPermissionsUpdated(kDeviceId, kPolicyAppId_, permissions);
}

TEST_F(PolicyHandlerTest, OnPermissionsUpdated_InvalidApp_UNSUCCESS) {
  std::shared_ptr<application_manager_test::MockApplication> invalid_app;
  EXPECT_CALL(app_manager_, application(kDeviceId, kPolicyAppId_))
      .WillOnce(Return(mock_app_))
      .WillOnce(Return(invalid_app));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  ChangePolicyManagerToMock();
  const policy::EncryptionRequired require_encryption;
  EXPECT_CALL(*mock_policy_manager_, GetAppEncryptionRequired(kPolicyAppId_))
      .WillOnce(Return(require_encryption));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _, _));

  Permissions permissions;
  policy_handler_.OnPermissionsUpdated(
      kDeviceId, kPolicyAppId_, permissions, "HMI_FULL");
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
  const std::string new_kHmiLevel_string = "FULL";
  mobile_apis::HMILevel::eType new_hmi_level = mobile_apis::HMILevel::HMI_FULL;
  // Check expectations
  EXPECT_CALL(app_manager_, application(kDeviceId, kPolicyAppId_))
      .Times(2)
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  EXPECT_CALL(*mock_app_, hmi_level(kDefaultWindowId))
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));
  ChangePolicyManagerToMock();
  const policy::EncryptionRequired require_encryption;
  EXPECT_CALL(*mock_policy_manager_, GetAppEncryptionRequired(kPolicyAppId_))
      .WillOnce(Return(require_encryption));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _, _));

  EXPECT_CALL(app_manager_, state_controller())
      .WillRepeatedly(ReturnRef(mock_state_controller));
  EXPECT_CALL(mock_state_controller,
              SetRegularState(_, kDefaultWindowId, new_hmi_level, true));
  // Act
  Permissions perms;
  policy_handler_.OnPermissionsUpdated(
      kDeviceId, kPolicyAppId_, perms, new_kHmiLevel_string);
}

TEST_F(PolicyHandlerTest,
       OnPermissionsUpdated_MethodWith3Parameters_FromNONE_ToNotFull) {
  // Set hmi level from NONE to Limited
  const std::string new_kHmiLevel_string = "LIMITED";
  mobile_apis::HMILevel::eType new_hmi_level =
      mobile_apis::HMILevel::HMI_LIMITED;
  // Check expectations
  EXPECT_CALL(app_manager_, application(kDeviceId, kPolicyAppId_))
      .Times(2)
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));
  EXPECT_CALL(*mock_app_, hmi_level(kDefaultWindowId))
      .WillOnce(Return(mobile_apis::HMILevel::HMI_NONE));
  ChangePolicyManagerToMock();
  const policy::EncryptionRequired require_encryption;
  EXPECT_CALL(*mock_policy_manager_, GetAppEncryptionRequired(kPolicyAppId_))
      .WillOnce(Return(require_encryption));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _, _));

  EXPECT_CALL(app_manager_, state_controller())
      .WillRepeatedly(ReturnRef(mock_state_controller));
  EXPECT_CALL(mock_state_controller,
              SetRegularState(_, kDefaultWindowId, new_hmi_level, false));
  // Act
  Permissions perms;
  policy_handler_.OnPermissionsUpdated(
      kDeviceId, kPolicyAppId_, perms, new_kHmiLevel_string);
}

TEST_F(PolicyHandlerTest,
       OnPermissionsUpdated_MethodWith3Parameters_FromNotNONE) {
  // Set hmi level from LIMITED to FULL
  std::string new_kHmiLevel_string = "FULL";
  // Check expectations
  EXPECT_CALL(app_manager_, application(kDeviceId, kPolicyAppId_))
      .Times(2)
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, app_id()).WillOnce(Return(kAppId1_));

  EXPECT_CALL(*mock_app_, hmi_level(kDefaultWindowId))
      .WillOnce(Return(mobile_apis::HMILevel::HMI_LIMITED));
  ChangePolicyManagerToMock();
  const policy::EncryptionRequired require_encryption;
  EXPECT_CALL(*mock_policy_manager_, GetAppEncryptionRequired(kPolicyAppId_))
      .WillOnce(Return(require_encryption));
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _, _));

  EXPECT_CALL(app_manager_, state_controller()).Times(0);
  // Act
  Permissions perms;
  policy_handler_.OnPermissionsUpdated(
      kDeviceId, kPolicyAppId_, perms, new_kHmiLevel_string);
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
  EXPECT_CALL(*mock_app_, hmi_level(kDefaultWindowId))
      .WillOnce(Return(hmi_level));
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(device));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(*mock_policy_manager_,
              CheckPermissions(
                  kDeviceId, kPolicyAppId_, kHmiLevel_, kRpc_, kRpc_params, _));

#else   // EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(*mock_policy_manager_,
              CheckPermissions(
                  kDeviceId, kPolicyAppId_, kHmiLevel_, kRpc_, kRpc_params, _));
#endif  // EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(mock_message_helper_, GetDeviceMacAddressForHandle(device, _))
      .WillOnce(Return(kDeviceId));
  // Act
  policy_handler_.CheckPermissions(
      mock_app_, kDefaultWindowId, kRpc_, kRpc_params, result);
}

TEST_F(PolicyHandlerTest, GetNotificationsNumber) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, GetNotificationsNumber(priority_, false));
  // Act
  policy_handler_.GetNotificationsNumber(priority_, false);
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
              GetDefaultHmi(kDeviceId_, kPolicyAppId_, &default_hmi_));
  // Act
  policy_handler_.GetDefaultHmi(kDeviceId_, kPolicyAppId_, &default_hmi_);
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
  std::shared_ptr<application_manager_test::MockApplication> application1 =
      std::make_shared<application_manager_test::MockApplication>();
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
  std::shared_ptr<usage_statistics_test::MockStatisticsManager>
      mock_statistics_manager =
          std::make_shared<usage_statistics_test::MockStatisticsManager>();
  UsageStatistics usage_stats(
      "0",
      std::shared_ptr<usage_statistics::StatisticsManager>(
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
  EXPECT_CALL(*application1, policy_app_id())
      .WillRepeatedly(Return(kPolicyAppId_));
  EXPECT_CALL(*mock_policy_manager_,
              GetAppPermissionsChanges(kMacAddr_, kPolicyAppId_))
      .WillOnce(Return(permissions));
  ON_CALL(*mock_policy_manager_, Increment(_, _)).WillByDefault(Return());
  EXPECT_CALL(*mock_policy_manager_, RemovePendingPermissionChanges(_));
  EXPECT_CALL(mock_message_helper_, SendSDLActivateAppResponse(_, _, _));
  ON_CALL(*application1, app_id()).WillByDefault(Return(kAppId1_));
  ON_CALL(*application1, mac_address()).WillByDefault(ReturnRef(kMacAddr_));
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
  std::shared_ptr<application_manager_test::MockApplication> invalid_app;
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
  std::shared_ptr<usage_statistics_test::MockStatisticsManager>
      mock_statistics_manager =
          std::make_shared<usage_statistics_test::MockStatisticsManager>();
  UsageStatistics usage_stats(
      "0",
      std::shared_ptr<usage_statistics::StatisticsManager>(
          mock_statistics_manager));
  EXPECT_CALL(*mock_app_, usage_report()).WillOnce(ReturnRef(usage_stats));
  const std::string default_mac = "00:00:00:00:00:00";
  EXPECT_CALL(*mock_policy_manager_, GetUserConsentForDevice(default_mac))
      .WillOnce(Return(DeviceConsent::kDeviceAllowed));
#endif  // EXTERNAL_PROPRIETARY_MODE

  // Check expectations
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));
  EXPECT_CALL(*mock_policy_manager_,
              GetAppPermissionsChanges(kDefaultId, kPolicyAppId_))
      .WillOnce(Return(permissions));
  ON_CALL(*mock_policy_manager_, Increment(_, _)).WillByDefault(Return());
  EXPECT_CALL(*mock_policy_manager_, RemovePendingPermissionChanges(_));
  EXPECT_CALL(mock_message_helper_, SendSDLActivateAppResponse(_, _, _));
  ON_CALL(*mock_app_, app_id()).WillByDefault(Return(kAppId1_));
  ON_CALL(*mock_app_, mac_address()).WillByDefault(ReturnRef(kDefaultId));
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

  std::shared_ptr<application_manager_test::MockApplication> application =
      std::make_shared<application_manager_test::MockApplication>();

  EXPECT_CALL(app_manager_, application(_, _)).WillOnce(Return(application));
  EXPECT_CALL(*application, app_id()).WillRepeatedly(Return(kAppId1_));
  EXPECT_CALL(*application, hmi_level(kDefaultWindowId))
      .WillRepeatedly(Return(hmi_level));
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
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _))
      .Times(0);
  EXPECT_CALL(*mock_policy_manager_,
              GetAppPermissionsChanges(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(permissions));
  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));
  // Act
  policy_handler_.OnPendingPermissionChange(kDeviceId_, kPolicyAppId_);
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
  EXPECT_CALL(*mock_policy_manager_,
              GetAppPermissionsChanges(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(permissions));
  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));
  // Act
  policy_handler_.OnPendingPermissionChange(kDeviceId_, kPolicyAppId_);
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
                              kDefaultWindowId,
                              mobile_apis::HMILevel::HMI_NONE,
                              mobile_apis::AudioStreamingState::NOT_AUDIBLE,
                              mobile_apis::VideoStreamingState::NOT_STREAMABLE,
                              true));

  EXPECT_CALL(*mock_policy_manager_,
              GetAppPermissionsChanges(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(permissions));
  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));
  // Act
  policy_handler_.OnPendingPermissionChange(kDeviceId_, kPolicyAppId_);
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

  EXPECT_CALL(*mock_policy_manager_,
              GetAppPermissionsChanges(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(permissions));
  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));
  // Act
  policy_handler_.OnPendingPermissionChange(kDeviceId_, kPolicyAppId_);
}

TEST_F(PolicyHandlerTest,
       OnPendingPermissionChange_AppInLIMITEDAndUnauthorized) {
  // Arrange
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_LIMITED);
  AppPermissions permissions(kPolicyAppId_);
  permissions.appUnauthorized = true;

  ns_smart_device_link::ns_smart_objects::SmartObjectSPtr message =
      std::make_shared<ns_smart_device_link::ns_smart_objects::SmartObject>();
  // Check expectations
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _));

  EXPECT_CALL(*mock_policy_manager_,
              GetAppPermissionsChanges(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(permissions));

  EXPECT_CALL(mock_message_helper_,
              GetOnAppInterfaceUnregisteredNotificationToMobile(
                  kAppId1_,
                  mobile_api::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED))
      .WillOnce(Return(message));
  ON_CALL(app_manager_, GetRPCService())
      .WillByDefault(ReturnRef(mock_rpc_service_));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, commands::Command::SOURCE_SDL));

  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));

  // Act
  policy_handler_.OnPendingPermissionChange(kDeviceId_, kPolicyAppId_);
}

TEST_F(PolicyHandlerTest,
       OnPendingPermissionChange_AppInLIMITEDAndRequestTypeChanged_SUCCESS) {
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_LIMITED);
  AppPermissions permissions(kPolicyAppId_);
  permissions.requestTypeChanged = true;

  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _));

  EXPECT_CALL(*mock_policy_manager_,
              GetAppPermissionsChanges(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(permissions));

  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));

  policy_handler_.OnPendingPermissionChange(kDeviceId_, kPolicyAppId_);
}

TEST_F(PolicyHandlerTest,
       OnPendingPermissionChange_AppInBackgroundAndUnauthorized) {
  // Arrange
  OnPendingPermissionChangePrecondition(
      mobile_apis::HMILevel::eType::HMI_BACKGROUND);
  AppPermissions permissions(kPolicyAppId_);
  permissions.appUnauthorized = true;

  ns_smart_device_link::ns_smart_objects::SmartObjectSPtr message =
      std::make_shared<ns_smart_device_link::ns_smart_objects::SmartObject>();
  // Check expectations
  // Notification won't be sent
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(kAppId1_, _, _))
      .Times(0);

  EXPECT_CALL(*mock_policy_manager_,
              GetAppPermissionsChanges(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(permissions));

  EXPECT_CALL(mock_message_helper_,
              GetOnAppInterfaceUnregisteredNotificationToMobile(
                  kAppId1_,
                  mobile_api::AppInterfaceUnregisteredReason::APP_UNAUTHORIZED))
      .WillOnce(Return(message));
  ON_CALL(app_manager_, GetRPCService())
      .WillByDefault(ReturnRef(mock_rpc_service_));
  EXPECT_CALL(mock_rpc_service_,
              ManageMobileCommand(_, commands::Command::SOURCE_SDL));

  EXPECT_CALL(*mock_policy_manager_,
              RemovePendingPermissionChanges(kPolicyAppId_));

  // Act
  policy_handler_.OnPendingPermissionChange(kDeviceId_, kPolicyAppId_);
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
  const std::string language("RU-RU");
  const uint32_t correlation_id = 2;
#ifdef EXTERNAL_PROPRIETARY_MODE
  const hmi_apis::Common_Language::eType default_language =
      hmi_apis::Common_Language::EN_US;
  const std::string default_language_string = "EN-US";
  application_manager_test::MockHMICapabilities mock_hmi_capabilities;
  EXPECT_CALL(app_manager_, hmi_capabilities())
      .WillOnce(ReturnRef(mock_hmi_capabilities));
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
  std::shared_ptr<application_manager_test::MockApplication> application =
      std::make_shared<application_manager_test::MockApplication>();
  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
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
                  _))
      .WillOnce(DoAll(SetArgPointee<3>(kDeviceId_), Return(0)));

  // Act
  policy_handler_.OnCurrentDeviceIdUpdateRequired(handle, kPolicyAppId_);
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

TEST_F(PolicyHandlerTest, IsApplicationRevoked) {
  // Arrange
  EnablePolicy();
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, IsApplicationRevoked(kPolicyAppId_));
  // Act
  policy_handler_.IsApplicationRevoked(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, GetAppRequestTypes) {
  // Arrange
  EnablePolicy();
  ChangePolicyManagerToMock();
  const transport_manager::DeviceHandle handle = 0u;
// Check expectations
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(*mock_policy_manager_, GetAppRequestTypes(handle, kPolicyAppId_))
      .WillOnce(Return(std::vector<std::string>()));
#else
  EXPECT_CALL(*mock_policy_manager_, GetAppRequestTypes(kPolicyAppId_))
      .WillOnce(Return(std::vector<std::string>()));
#endif
  // Act
  policy_handler_.GetAppRequestTypes(handle, kPolicyAppId_);
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

  std::shared_ptr<application_manager_test::MockApplication> application =
      std::make_shared<application_manager_test::MockApplication>();

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

  std::shared_ptr<application_manager_test::MockApplication> application =
      std::make_shared<application_manager_test::MockApplication>();
  // Check expectations
  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(application));
  EXPECT_CALL(mock_message_helper_,
              SendOnAppPermissionsChangedNotification(_, _, _));
  AppPermissions permissions(kPolicyAppId_);
  // Act
  policy_handler_.SendOnAppPermissionsChanged(
      permissions, kDeviceId_, kPolicyAppId_);
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
      AddApplication(
          kMacAddr_, kPolicyAppId_, HmiTypes(policy_table::AHT_DEFAULT)))
      .WillOnce(Return(std::make_shared<utils::CallNothing>()));
  // Act
  policy_handler_.AddApplication(
      kMacAddr_, kPolicyAppId_, HmiTypes(policy_table::AHT_DEFAULT));
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

  EXPECT_CALL(*mock_policy_manager_,
              OnAppRegisteredOnMobile(kDeviceId, kPolicyAppId_));
  // Act
  policy_handler_.OnAppRegisteredOnMobile(kDeviceId, kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, IsRequestTypeAllowed) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations

  mobile_apis::RequestType::eType type =
      mobile_apis::RequestType::eType::EMERGENCY;

  const transport_manager::DeviceHandle handle = 0u;

  EXPECT_CALL(*mock_policy_manager_, GetAppRequestTypesState(kPolicyAppId_))
      .WillOnce(Return(policy::RequestType::State::AVAILABLE));
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(*mock_policy_manager_, GetAppRequestTypes(handle, kPolicyAppId_))
      .WillOnce(Return(std::vector<std::string>({"HTTP"})));
#else
  EXPECT_CALL(*mock_policy_manager_, GetAppRequestTypes(kPolicyAppId_))
      .WillOnce(Return(std::vector<std::string>({"HTTP"})));
#endif
  // Act
  EXPECT_FALSE(
      policy_handler_.IsRequestTypeAllowed(handle, kPolicyAppId_, type));
}

TEST_F(PolicyHandlerTest, IsRequestSubTypeAllowed) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  // Check expectations
  EXPECT_CALL(*mock_policy_manager_, GetAppRequestSubTypesState(kPolicyAppId_))
      .WillOnce(Return(policy::RequestSubType::State::AVAILABLE));
  EXPECT_CALL(*mock_policy_manager_, GetAppRequestSubTypes(kPolicyAppId_))
      .WillOnce(
          Return(std::vector<std::string>({"fakeSubType", "fakeSubType2"})));

  // Act
  const std::string subtype = "fakeSubType";
  EXPECT_TRUE(policy_handler_.IsRequestSubTypeAllowed(kPolicyAppId_, subtype));
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

#ifdef EXTERNAL_PROPRIETARY_MODE
  policy::ExternalConsentStatus external_consent_status =
      policy::ExternalConsentStatus();
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(
                  _, external_consent_status, corr_id, _, true));
  EXPECT_CALL(*mock_policy_manager_, GetExternalConsentStatus())
      .WillOnce(Return(external_consent_status));
#else
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(_, corr_id, _, true));
#endif  // #ifdef EXTERNAL_PROPRIETARY_MODE

  policy_handler_.OnGetListOfPermissions(app_id, corr_id);
}

TEST_F(PolicyHandlerTest, OnGetListOfPermissions_CollectResult_false) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  const uint32_t app_id = 10u;
  const uint32_t corr_id = 1u;
  test_app.insert(mock_app_);

  EXPECT_CALL(app_manager_, application(app_id))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(conn_handler, get_session_observer())
      .WillOnce(ReturnRef(mock_session_observer));
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(0));
  EXPECT_CALL(mock_session_observer,
              GetDataOnDeviceID(
                  testing::An<transport_manager::DeviceHandle>(), _, _, _, _))
      .WillRepeatedly(
          DoAll(SetDeviceParamsMacAdress(std::string()), (Return(1u))));

#ifdef EXTERNAL_PROPRIETARY_MODE
  policy::ExternalConsentStatus external_consent_status =
      policy::ExternalConsentStatus();
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(
                  _, external_consent_status, corr_id, _, false))
      .WillOnce(Return());
  EXPECT_CALL(*mock_policy_manager_, GetExternalConsentStatus())
      .WillOnce(Return(external_consent_status));
#else
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(_, corr_id, _, false))
      .WillOnce(Return());
#endif  // #ifdef EXTERNAL_PROPRIETARY_MODE

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
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(
                  _, external_consent_status, corr_id, _, true));
  EXPECT_CALL(*mock_policy_manager_, GetExternalConsentStatus())
      .WillOnce(Return(external_consent_status));
#else
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(_, corr_id, _, true));
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
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(
                  _, external_consent_status, corr_id, _, true));
  EXPECT_CALL(*mock_policy_manager_, GetExternalConsentStatus())
      .WillOnce(Return(external_consent_status));
#else
  EXPECT_CALL(mock_message_helper_,
              SendGetListOfPermissionsResponse(_, corr_id, _, true));
#endif  // #ifdef EXTERNAL_PROPRIETARY_MODE

  policy_handler_.OnGetListOfPermissions(app_id, corr_id);
}

#ifdef ENABLE_SECURITY
TEST_F(PolicyHandlerTest, RetrieveCertificate) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  const std::string test_certificate = "test certificate";
  EXPECT_CALL(*mock_policy_manager_, RetrieveCertificate())
      .WillOnce(Return(test_certificate));
  EXPECT_EQ(test_certificate, policy_handler_.RetrieveCertificate());
}
#endif  // ENABLE_SECURITY

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
  policy_handler_.OnSnapshotCreated(msg,
                                    policy::PTUIterationType::DefaultIteration);
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
  EXPECT_CALL(mock_message_helper_,
              SendPolicySnapshotNotification(
                  _, testing::A<const std::vector<uint8_t>&>(), _, _));
  // Check expectations for get app id
  GetAppIDForSending();
  // Expectations
  EXPECT_CALL(app_manager_, application(kAppId1_))
      .WillRepeatedly(Return(mock_app_));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));
#endif  // PROPRIETARY_MODE

  EXPECT_CALL(*mock_policy_manager_, OnUpdateStarted());
  policy_handler_.OnSnapshotCreated(msg,
                                    policy::PTUIterationType::DefaultIteration);
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
  EXPECT_CALL(app_manager_, applications()).WillRepeatedly(Return(app_set));

  EXPECT_CALL(conn_handler, GetDeviceID(kDeviceId, _))
      .WillOnce(DoAll(SetArgPointee<1>(test_device_id), Return(true)));

  test_app.insert(mock_app_);
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(test_device_id));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));

  EXPECT_CALL(*mock_policy_manager_, IsPredataPolicy(kPolicyAppId_))
      .WillRepeatedly(Return(true));

  EXPECT_CALL(
      *mock_policy_manager_,
      ReactOnUserDevConsentForApp(test_device_id, kPolicyAppId_, is_allowed));
  EXPECT_CALL(*mock_policy_manager_,
              SendNotificationOnPermissionsUpdated(kDeviceId, kPolicyAppId_));

  policy_handler_.OnDeviceConsentChanged(kDeviceId, is_allowed);
}

TEST_F(PolicyHandlerTest, OnDeviceConsentChanged_ConsentNotAllowed) {
  const bool is_allowed = false;
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  connection_handler::DeviceHandle handle = 100u;
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));

  // Check expectations
  EXPECT_CALL(conn_handler, GetDeviceID(kMacAddr_, _))
      .WillOnce(DoAll(SetArgPointee<1>(handle), Return(true)));
  test_app.insert(mock_app_);

  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(handle));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));

  EXPECT_CALL(*mock_policy_manager_, IsPredataPolicy(kPolicyAppId_))
      .WillRepeatedly(Return(true));

  EXPECT_CALL(*mock_policy_manager_,
              ReactOnUserDevConsentForApp(handle, kPolicyAppId_, is_allowed))
      .Times(0);
  EXPECT_CALL(*mock_policy_manager_,
              SendNotificationOnPermissionsUpdated(kMacAddr_, kPolicyAppId_))
      .Times(0);

  policy_handler_.OnDeviceConsentChanged(kMacAddr_, is_allowed);
}

TEST_F(PolicyHandlerTest, OnDeviceConsentChanged_PredatePolicyNotAllowed) {
  const bool is_allowed = false;
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  connection_handler::DeviceHandle test_device_id = 100u;
  EXPECT_CALL(app_manager_, connection_handler())
      .WillOnce(ReturnRef(conn_handler));
  EXPECT_CALL(app_manager_, applications()).WillRepeatedly(Return(app_set));

  EXPECT_CALL(conn_handler, GetDeviceID(kMacAddr_, _))
      .WillOnce(DoAll(SetArgPointee<1>(test_device_id), Return(true)));

  test_app.insert(mock_app_);

  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(test_device_id));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));

  // App does not have predate policy
  EXPECT_CALL(*mock_policy_manager_, IsPredataPolicy(kPolicyAppId_))
      .WillRepeatedly(Return(false));

  EXPECT_CALL(
      *mock_policy_manager_,
      ReactOnUserDevConsentForApp(test_device_id, kPolicyAppId_, is_allowed));
  EXPECT_CALL(*mock_policy_manager_,
              SendNotificationOnPermissionsUpdated(kMacAddr_, kPolicyAppId_));

  policy_handler_.OnDeviceConsentChanged(kMacAddr_, is_allowed);
}
#ifdef ENABLE_SECURITY
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
#endif  // EXTERNAL_PROPRIETARY_MODE
#endif  // ENABLE_SECURITY

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
  EXPECT_CALL(*mock_app_, hmi_level(kDefaultWindowId))
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
  EXPECT_CALL(*mock_app_, hmi_level(kDefaultWindowId))
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

  std::shared_ptr<application_manager_test::MockApplication> mock_app_in_full =
      std::make_shared<application_manager_test::MockApplication>();
  const uint32_t app_in_full_id = 1;
  EXPECT_CALL(*mock_app_in_full, app_id())
      .WillRepeatedly(Return(app_in_full_id));
  EXPECT_CALL(*mock_app_in_full, hmi_level(kDefaultWindowId))
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_FULL));
  ON_CALL(*mock_app_in_full, IsRegistered()).WillByDefault(Return(true));

  test_app.insert(mock_app_in_full);

  std::shared_ptr<application_manager_test::MockApplication>
      mock_app_in_limited =
          std::make_shared<application_manager_test::MockApplication>();
  const uint32_t app_in_limited_id = 2;
  EXPECT_CALL(*mock_app_in_limited, app_id())
      .WillRepeatedly(Return(app_in_limited_id));
  EXPECT_CALL(*mock_app_in_limited, hmi_level(kDefaultWindowId))
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_LIMITED));
  ON_CALL(*mock_app_in_limited, IsRegistered()).WillByDefault(Return(true));

  test_app.insert(mock_app_in_limited);

  std::shared_ptr<application_manager_test::MockApplication>
      mock_app_in_background =
          std::make_shared<application_manager_test::MockApplication>();
  const uint32_t app_in_background_id = 3;
  EXPECT_CALL(*mock_app_in_background, app_id())
      .WillRepeatedly(Return(app_in_background_id));
  EXPECT_CALL(*mock_app_in_background, hmi_level(kDefaultWindowId))
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_BACKGROUND));
  ON_CALL(*mock_app_in_background, IsRegistered()).WillByDefault(Return(true));

  test_app.insert(mock_app_in_background);

  std::shared_ptr<application_manager_test::MockApplication> mock_app_in_none =
      std::make_shared<application_manager_test::MockApplication>();
  const uint32_t app_in_none_id = 4;
  EXPECT_CALL(*mock_app_in_none, app_id())
      .WillRepeatedly(Return(app_in_none_id));
  EXPECT_CALL(*mock_app_in_none, hmi_level(kDefaultWindowId))
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

  std::shared_ptr<application_manager_test::MockApplication>
      mock_app_in_none_1 =
          std::make_shared<application_manager_test::MockApplication>();
  const uint32_t app_in_none_id_1 = 1;
  EXPECT_CALL(*mock_app_in_none_1, app_id())
      .WillRepeatedly(Return(app_in_none_id_1));
  EXPECT_CALL(*mock_app_in_none_1, hmi_level(kDefaultWindowId))
      .WillRepeatedly(Return(mobile_api::HMILevel::HMI_FULL));
  ON_CALL(*mock_app_in_none_1, IsRegistered()).WillByDefault(Return(true));

  test_app.insert(mock_app_in_none_1);

  std::shared_ptr<application_manager_test::MockApplication>
      mock_app_in_none_2 =
          std::make_shared<application_manager_test::MockApplication>();
  const uint32_t app_in_none_id_2 = 2;
  EXPECT_CALL(*mock_app_in_none_2, app_id())
      .WillRepeatedly(Return(app_in_none_id_2));
  EXPECT_CALL(*mock_app_in_none_2, hmi_level(kDefaultWindowId))
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

TEST_F(PolicyHandlerTest, CanUpdate) {
  GetAppIDForSending();
  EXPECT_TRUE(policy_handler_.CanUpdate());
}

TEST_F(PolicyHandlerTest, CanUpdate_TwoApplicationForSending_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();
  std::shared_ptr<application_manager_test::MockApplication> second_mock_app =
      std::make_shared<application_manager_test::MockApplication>();

  EXPECT_CALL(*mock_app_, hmi_level(kDefaultWindowId))
      .WillOnce(Return(mobile_apis::HMILevel::HMI_FULL));
  EXPECT_CALL(*second_mock_app, hmi_level(kDefaultWindowId))
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
                  _))
      .WillOnce(Return(1u));

  EXPECT_CALL(app_manager_, application(kConnectionKey_))
      .WillOnce(Return(mock_app_));
  EXPECT_CALL(*mock_app_, policy_app_id()).WillOnce(Return(kPolicyAppId_));
  EXPECT_CALL(*mock_app_, device()).WillOnce(Return(device));

  auto waiter_first = TestAsyncWaiter::createInstance();
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_CALL(*mock_policy_manager_, SetUserConsentForApp(_, _))
      .WillOnce(NotifyTestAsyncWaiter(waiter_first));
#else
  EXPECT_CALL(*mock_policy_manager_, SetUserConsentForApp(_))
      .WillOnce(NotifyTestAsyncWaiter(waiter_first));
#endif
  ExternalConsentStatusItem item(1u, 1u, kStatusOn);
  ExternalConsentStatus external_consent_status;
  external_consent_status.insert(item);

#ifdef EXTERNAL_PROPRIETARY_MODE
  auto waiter_second = TestAsyncWaiter::createInstance();

  EXPECT_CALL(*mock_policy_manager_,
              SetExternalConsentStatus(external_consent_status))
      .WillOnce(Return(true));
  policy_handler_.OnAppPermissionConsent(
      kConnectionKey_, permissions, external_consent_status);
#else
  policy_handler_.OnAppPermissionConsent(kConnectionKey_, permissions);

#endif
  EXPECT_TRUE(waiter_first->WaitFor(kCallsCount_, kTimeout_));
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_TRUE(waiter_second->WaitFor(kCallsCount_, kTimeout_));
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

  auto waiter = TestAsyncWaiter::createInstance();

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

  EXPECT_FALSE(waiter->WaitFor(kCallsCount_, kTimeout_));
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

  auto waiter = TestAsyncWaiter::createInstance();

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

  EXPECT_FALSE(waiter->WaitFor(kCallsCount_, kTimeout_));
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
  auto waiter = TestAsyncWaiter::createInstance();

  EXPECT_CALL(*mock_policy_manager_,
              SetExternalConsentStatus(external_consent_status))
      .WillOnce(DoAll(NotifyTestAsyncWaiter(waiter), Return(true)));
  policy_handler_.OnAppPermissionConsent(
      invalid_connection_key, permissions, external_consent_status);
#else
  policy_handler_.OnAppPermissionConsent(invalid_connection_key, permissions);
#endif

  Mock::VerifyAndClearExpectations(mock_app_.get());
#ifdef EXTERNAL_PROPRIETARY_MODE
  EXPECT_TRUE(waiter->WaitFor(kCallsCount_, kTimeout_));
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
  auto waiter = TestAsyncWaiter::createInstance();

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
  EXPECT_FALSE(waiter->WaitFor(kCallsCount_, kTimeout_));
#endif
}

ACTION_P(SetEndpoint, endpoint) {
  arg1 = endpoint;
}

TEST_F(PolicyHandlerTest, GetLockScreenIconUrl_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  const std::string url_str = "test_icon_url";
  EndpointData data(url_str);

  EndpointUrls endpoints;
  endpoints.push_back(data);

  const std::string service_type = "lock_screen_icon_url";
  EXPECT_CALL(*mock_policy_manager_, GetUpdateUrls(service_type, _))
      .WillOnce(SetEndpoint(endpoints));

  EXPECT_EQ(url_str, policy_handler_.GetLockScreenIconUrl(kPolicyAppId_));
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

  auto waiter = TestAsyncWaiter::createInstance();

  EXPECT_CALL(*mock_policy_manager_, Increment(_))
      .WillOnce(NotifyTestAsyncWaiter(waiter));

  policy_handler_.AddStatisticsInfo(
      hmi_apis::Common_StatisticsType::iAPP_BUFFER_FULL);
  EXPECT_TRUE(waiter->WaitFor(kCallsCount_, kTimeout_));
}

TEST_F(PolicyHandlerTest, OnSystemError_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  auto waiter_first = TestAsyncWaiter::createInstance();
  EXPECT_CALL(*mock_policy_manager_, Increment(_))
      .WillOnce(NotifyTestAsyncWaiter(waiter_first));

  policy_handler_.OnSystemError(hmi_apis::Common_SystemError::SYNC_REBOOTED);
  EXPECT_TRUE(waiter_first->WaitFor(kCallsCount_, kTimeout_));

  auto waiter_second = TestAsyncWaiter::createInstance();

  EXPECT_CALL(*mock_policy_manager_, Increment(_))
      .WillOnce(NotifyTestAsyncWaiter(waiter_second));
  policy_handler_.OnSystemError(
      hmi_apis::Common_SystemError::SYNC_OUT_OF_MEMMORY);
  EXPECT_TRUE(waiter_second->WaitFor(kCallsCount_, kTimeout_));
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

TEST_F(PolicyHandlerTest, OnSetCloudAppProperties_AllProperties_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  bool enabled = true;
  std::string app_name = "anAppName";
  std::string auth_token = "anAuthToken";
  std::string cloud_transport_type = "aTransportType";
  mobile_apis::HybridAppPreference::eType hybrid_app_preference =
      mobile_apis::HybridAppPreference::CLOUD;
  std::string hybrid_app_preference_str = "CLOUD";
  std::string endpoint = "anEndpoint";
  const policy::AppProperties app_properties(endpoint,
                                             " ",
                                             enabled,
                                             auth_token,
                                             cloud_transport_type,
                                             hybrid_app_preference_str);

  StringArray nicknames_vec;
  nicknames_vec.push_back(app_name);

  smart_objects::SmartObject message(smart_objects::SmartType_Map);
  smart_objects::SmartObject properties(smart_objects::SmartType_Map);
  smart_objects::SmartObject nicknames(smart_objects::SmartType_Array);

  properties[strings::app_id] = kPolicyAppId_;
  nicknames[0] = app_name;
  properties[strings::nicknames] = nicknames;
  properties[strings::enabled] = enabled;
  properties[strings::auth_token] = auth_token;
  properties[strings::cloud_transport_type] = cloud_transport_type;
  properties[strings::hybrid_app_preference] = hybrid_app_preference;
  properties[strings::endpoint] = endpoint;
  message[strings::msg_params][strings::properties] = properties;

  application_manager_test::MockPolicyHandlerObserver policy_handler_observer;
  policy_handler_.add_listener(&policy_handler_observer);

  EXPECT_CALL(*mock_policy_manager_, InitCloudApp(kPolicyAppId_));
  EXPECT_CALL(*mock_policy_manager_,
              SetCloudAppEnabled(kPolicyAppId_, enabled));
  EXPECT_CALL(*mock_policy_manager_,
              SetAppNicknames(kPolicyAppId_, nicknames_vec));
  EXPECT_CALL(*mock_policy_manager_,
              SetAppAuthToken(kPolicyAppId_, auth_token));
  EXPECT_CALL(*mock_policy_manager_,
              SetAppCloudTransportType(kPolicyAppId_, cloud_transport_type));
  EXPECT_CALL(*mock_policy_manager_,
              SetHybridAppPreference(kPolicyAppId_, hybrid_app_preference_str));
  EXPECT_CALL(*mock_policy_manager_, SetAppEndpoint(kPolicyAppId_, endpoint));
  EXPECT_CALL(*mock_policy_manager_, GetAppProperties(kPolicyAppId_, _))
      .WillOnce(DoAll(SetArgReferee<1>(app_properties), Return(true)));
  EXPECT_CALL(app_manager_, RefreshCloudAppInformation());
  EXPECT_CALL(policy_handler_observer,
              OnAuthTokenUpdated(kPolicyAppId_, auth_token));

  policy_handler_.OnSetCloudAppProperties(message);
}

TEST_F(PolicyHandlerTest, GetCloudAppParameters_AllProperties_SUCCESS) {
  EnablePolicyAndPolicyManagerMock();

  bool enabled = true;
  std::string certificate = "aCertificate";
  std::string auth_token = "anAuthToken";
  std::string cloud_transport_type = "aTransportType";
  std::string hybrid_app_preference_str = "CLOUD";
  std::string endpoint = "anEndpoint";

  const policy::AppProperties app_properties(endpoint,
                                             certificate,
                                             enabled,
                                             auth_token,
                                             cloud_transport_type,
                                             hybrid_app_preference_str);

  application_manager_test::MockPolicyHandlerObserver policy_handler_observer;
  policy_handler_.add_listener(&policy_handler_observer);

  EXPECT_CALL(*mock_policy_manager_, GetAppProperties(kPolicyAppId_, _))
      .WillOnce(DoAll(SetArgReferee<1>(app_properties), Return(true)));

  policy::AppProperties out_app_properties;
  EXPECT_TRUE(
      policy_handler_.GetAppProperties(kPolicyAppId_, out_app_properties));
  EXPECT_EQ(app_properties.enabled, out_app_properties.enabled);
  EXPECT_EQ(app_properties.endpoint, out_app_properties.endpoint);
  EXPECT_EQ(app_properties.certificate, out_app_properties.certificate);
  EXPECT_EQ(app_properties.auth_token, out_app_properties.auth_token);
  EXPECT_EQ(app_properties.transport_type, out_app_properties.transport_type);
  EXPECT_EQ(app_properties.hybrid_app_preference,
            out_app_properties.hybrid_app_preference);
}

TEST_F(PolicyHandlerTest, SendOnAppPropertiesChangeNotification_SUCCESS) {
  using namespace smart_objects;
  auto notification = std::make_shared<SmartObject>(SmartType_Null);

  ON_CALL(app_manager_, GetRPCService())
      .WillByDefault(ReturnRef(mock_rpc_service_));

  EXPECT_CALL(mock_message_helper_,
              CreateOnAppPropertiesChangeNotification(kPolicyAppId_, _))
      .WillOnce(Return(notification));
  EXPECT_CALL(mock_rpc_service_,
              ManageHMICommand(notification, commands::Command::SOURCE_HMI));

  policy_handler_.SendOnAppPropertiesChangeNotification(kPolicyAppId_);
}

TEST_F(PolicyHandlerTest, GetApplicationPolicyIDs_GetEmptyIDsVector_SUCCESS) {
  ChangePolicyManagerToMock();

  std::vector<std::string> app_ids_vector;
  ON_CALL(*mock_policy_manager_, GetApplicationPolicyIDs())
      .WillByDefault(Return(app_ids_vector));

  EXPECT_CALL(*mock_policy_manager_, GetApplicationPolicyIDs());
  EXPECT_EQ(app_ids_vector, policy_handler_.GetApplicationPolicyIDs());
}

TEST_F(PolicyHandlerTest,
       GetApplicationPolicyIDs_GetIDFromAppIDsVectorWithWrongIDs_SUCCESS) {
  ChangePolicyManagerToMock();

  std::vector<std::string> app_ids_vector = {policy::kDefaultId,
                                             policy::kPreDataConsentId,
                                             policy::kDeviceId,
                                             kPolicyAppId_};
  ON_CALL(*mock_policy_manager_, GetApplicationPolicyIDs())
      .WillByDefault(Return(app_ids_vector));

  EXPECT_CALL(*mock_policy_manager_, GetApplicationPolicyIDs());

  auto policy_ids = policy_handler_.GetApplicationPolicyIDs();
  EXPECT_NE(app_ids_vector, policy_ids);
  EXPECT_EQ(1u, policy_ids.size());
  EXPECT_EQ(kPolicyAppId_, policy_ids[0]);
}

TEST_F(PolicyHandlerTest, CheckCloudAppEnabled_SUCCESS) {
  ChangePolicyManagerToMock();

  policy::AppProperties out_app_properties;
  out_app_properties.enabled = true;

  EXPECT_CALL(*mock_policy_manager_, GetAppProperties(kPolicyAppId_, _))
      .WillOnce(DoAll(SetArgReferee<1>(out_app_properties), Return(true)));
  EXPECT_TRUE(policy_handler_.CheckCloudAppEnabled(kPolicyAppId_));
}

TEST_F(PolicyHandlerTest, CheckCloudAppNotEnabled_SUCCESS) {
  ChangePolicyManagerToMock();
  EXPECT_CALL(*mock_policy_manager_, GetAppProperties(kPolicyAppId_, _));
  EXPECT_FALSE(policy_handler_.CheckCloudAppEnabled(kPolicyAppId_));
}

TEST_F(PolicyHandlerTest, OnLocalAppAdded_SUCCESS) {
  ChangePolicyManagerToMock();
  EXPECT_CALL(*mock_policy_manager_, OnLocalAppAdded());
  policy_handler_.OnLocalAppAdded();
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_NoPropertiesChanged) {
  ChangePolicyManagerToMock();

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;

  EXPECT_CALL(*mock_policy_manager_, GetAppProperties(kPolicyAppId_, _))
      .WillOnce(Return(false));
  EXPECT_CALL(*mock_policy_manager_, GetInitialAppData(kPolicyAppId_, _, _))
      .WillOnce(Return(false));
  EXPECT_EQ(policy::PolicyHandlerInterface::AppPropertiesState::NO_CHANGES,
            policy_handler_.GetAppPropertiesStatus(properties, kPolicyAppId_));
}

TEST_F(PolicyHandlerTest,
       GetAppPropertiesStatus_EnableFlagSwitchChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  policy::AppProperties app_properties;
  app_properties.enabled = false;

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::enabled] = !app_properties.enabled;

  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::ENABLED_FLAG_SWITCH);
}

TEST_F(PolicyHandlerTest,
       GetAppPropertiesStatus_EnableFlagSwitchNotChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  policy::AppProperties app_properties;
  app_properties.enabled = false;

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::enabled] = app_properties.enabled;

  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::NO_CHANGES);
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_AuthTokenChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  const std::string kCurrentToken = "kCurrentToken";
  const std::string kNewToken = "kNewToken";

  policy::AppProperties app_properties;
  app_properties.auth_token = kCurrentToken;

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::auth_token] = kNewToken;

  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::AUTH_TOKEN_CHANGED);
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_AuthTokenNotChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  const std::string kCurrentToken = "kCurrentToken";

  policy::AppProperties app_properties;
  app_properties.auth_token = kCurrentToken;

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::auth_token] = app_properties.auth_token;

  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::NO_CHANGES);
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_TransportTypeChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  const std::string kCurrentTransportType = "kCurrentTransportType";
  const std::string kNewTransportType = "kNewTransportType";

  policy::AppProperties app_properties;
  app_properties.transport_type = kCurrentTransportType;

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::cloud_transport_type] = kNewTransportType;

  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::
          TRANSPORT_TYPE_CHANGED);
}

TEST_F(PolicyHandlerTest,
       GetAppPropertiesStatus_TransportTypeNotChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  const std::string kCurrentTransportType = "kCurrentTransportType";

  policy::AppProperties app_properties;
  app_properties.transport_type = kCurrentTransportType;

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::cloud_transport_type] = app_properties.transport_type;
  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::NO_CHANGES);
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_EndPointChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  const std::string kCurrentEndPoint = "kCurrentEndPoint";
  const std::string kNewEndPoint = "kNewEndPoint";

  policy::AppProperties app_properties;
  app_properties.endpoint = kCurrentEndPoint;

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::endpoint] = kNewEndPoint;

  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::ENDPOINT_CHANGED);
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_EndPointNotChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  const std::string kCurrentEndPoint = "kCurrentEndPoint";

  policy::AppProperties app_properties;
  app_properties.endpoint = kCurrentEndPoint;

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::endpoint] = app_properties.endpoint;

  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::NO_CHANGES);
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_NicknameChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::nicknames] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  properties[strings::nicknames].asArray()->push_back(
      smart_objects::SmartObject(kFakeNickname));

  std::shared_ptr<policy::StringArray> nicknames =
      std::make_shared<policy::StringArray>();

  const auto expected_app_properties_state =
      policy::PolicyHandlerInterface::AppPropertiesState::NICKNAMES_CHANGED;

  EXPECT_CALL(*mock_policy_manager_, GetAppProperties(kPolicyAppId_, _))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_policy_manager_, GetInitialAppData(kPolicyAppId_, _, _))
      .WillOnce(DoAll(SetArgPointee<1>(*nicknames), Return(true)));
  EXPECT_EQ(expected_app_properties_state,
            policy_handler_.GetAppPropertiesStatus(properties, kPolicyAppId_));
}

TEST_F(PolicyHandlerTest,
       GetAppPropertiesStatus_RemoveNickname_NicknamesChanged) {
  ChangePolicyManagerToMock();

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::nicknames] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  properties[strings::nicknames].asArray()->push_back(
      smart_objects::SmartObject(kFakeNickname));

  auto nicknames = std::make_shared<policy::StringArray>(2, kFakeNickname);

  const auto expected_app_properties_state =
      policy::PolicyHandlerInterface::AppPropertiesState::NICKNAMES_CHANGED;

  EXPECT_CALL(*mock_policy_manager_, GetAppProperties(kPolicyAppId_, _))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_policy_manager_, GetInitialAppData(kPolicyAppId_, _, _))
      .WillOnce(DoAll(SetArgPointee<1>(*nicknames), Return(true)));
  EXPECT_EQ(expected_app_properties_state,
            policy_handler_.GetAppPropertiesStatus(properties, kPolicyAppId_));
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_NicknameNotChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::nicknames] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  properties[strings::nicknames].asArray()->push_back(
      smart_objects::SmartObject(kFakeNickname));

  std::shared_ptr<policy::StringArray> nicknames =
      std::make_shared<policy::StringArray>();
  nicknames->push_back(kFakeNickname);

  const auto expected_app_properties_state =
      policy::PolicyHandlerInterface::AppPropertiesState::NO_CHANGES;

  EXPECT_CALL(*mock_policy_manager_, GetAppProperties(kPolicyAppId_, _))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_policy_manager_, GetInitialAppData(kPolicyAppId_, _, _))
      .WillOnce(DoAll(SetArgPointee<1>(*nicknames), Return(true)));
  EXPECT_EQ(expected_app_properties_state,
            policy_handler_.GetAppPropertiesStatus(properties, kPolicyAppId_));
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_HybridAppChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  const auto kCurrentHybridAppProperties =
      mobile_apis::HybridAppPreference::eType::CLOUD;
  const auto kNewHybridAppProperties =
      mobile_apis::HybridAppPreference::eType::MOBILE;

  policy::AppProperties app_properties;
  smart_objects::EnumConversionHelper<mobile_apis::HybridAppPreference::eType>::
      EnumToString(kCurrentHybridAppProperties,
                   &app_properties.hybrid_app_preference);

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::hybrid_app_preference] = kNewHybridAppProperties;

  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::
          HYBRYD_APP_PROPERTIES_CHANGED);
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_HybridAppNotChanged_SUCCESS) {
  ChangePolicyManagerToMock();

  const auto kCurrentHybridAppProperties =
      mobile_apis::HybridAppPreference::eType::CLOUD;

  policy::AppProperties app_properties;
  smart_objects::EnumConversionHelper<mobile_apis::HybridAppPreference::eType>::
      EnumToString(kCurrentHybridAppProperties,
                   &app_properties.hybrid_app_preference);

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;
  properties[strings::hybrid_app_preference] = kCurrentHybridAppProperties;

  SetExpectationsAndCheckCloudAppPropertiesStatus(
      app_properties,
      properties,
      policy::PolicyHandlerInterface::AppPropertiesState::NO_CHANGES);
}

TEST_F(PolicyHandlerTest, GetAppPropertiesStatus_PolicyDisabled_FAIL) {
  EXPECT_CALL(policy_settings_, enable_policy()).WillOnce(Return(false));
  policy_handler_.LoadPolicyLibrary();

  smart_objects::SmartObject properties;
  properties[strings::app_id] = kPolicyAppId_;

  const auto expected_app_properties_state =
      policy::PolicyHandlerInterface::AppPropertiesState::NO_CHANGES;
  EXPECT_EQ(expected_app_properties_state,
            policy_handler_.GetAppPropertiesStatus(properties, kPolicyAppId_));
}

TEST_F(PolicyHandlerTest, GetEnabledLocalApps_SUCCESS) {
  ChangePolicyManagerToMock();
  std::vector<std::string> enabled_local_apps;

  EXPECT_CALL(*mock_policy_manager_, GetEnabledLocalApps())
      .WillOnce(Return(enabled_local_apps));
  EXPECT_EQ(enabled_local_apps, policy_handler_.GetEnabledLocalApps());

  enabled_local_apps.push_back("local_app");
  EXPECT_CALL(*mock_policy_manager_, GetEnabledLocalApps())
      .WillOnce(Return(enabled_local_apps));
  EXPECT_EQ(enabled_local_apps, policy_handler_.GetEnabledLocalApps());
}

TEST_F(PolicyHandlerTest, PushAppIdToPTUQueue_PolicyEnabled_SUCCESS) {
  ChangePolicyManagerToMock();
  const uint32_t expected_apps_count = 1u;
  EXPECT_CALL(*mock_policy_manager_,
              UpdatePTUReadyAppsCount(expected_apps_count));
  policy_handler_.PushAppIdToPTUQueue(kAppId1_);
  EXPECT_EQ(expected_apps_count,
            policy_handler_.applications_ptu_queue_.size());
}

TEST_F(PolicyHandlerTest, PushAppIdToPTUQueue_PolicyDisabled_FAIL) {
  EXPECT_CALL(policy_settings_, enable_policy()).WillOnce(Return(false));

  policy_handler_.LoadPolicyLibrary();
  policy_handler_.PushAppIdToPTUQueue(kAppId1_);
  const uint32_t expected_apps_count = 0u;
  EXPECT_EQ(expected_apps_count,
            policy_handler_.applications_ptu_queue_.size());
}

TEST_F(PolicyHandlerTest, StopRetrySequence_PolicyEnabled_SUCCESS) {
  ChangePolicyManagerToMock();
  EXPECT_CALL(*mock_policy_manager_, StopRetrySequence());
  policy_handler_.StopRetrySequence();
}

TEST_F(PolicyHandlerTest, GetPolicyTableData_PolicyEnabled_SUCCESS) {
  ChangePolicyManagerToMock();
  Json::Value expected_table_data(Json::objectValue);
  expected_table_data["test_key"] = "test_value";
  EXPECT_CALL(*mock_policy_manager_, GetPolicyTableData())
      .WillOnce(Return(expected_table_data));
  EXPECT_EQ(expected_table_data, policy_handler_.GetPolicyTableData());
}

TEST_F(PolicyHandlerTest, GetPolicyTableData_PolicyDisabled_FAIL) {
  EXPECT_CALL(policy_settings_, enable_policy()).WillOnce(Return(false));

  policy_handler_.LoadPolicyLibrary();

  Json::Value expected_table_data;
  EXPECT_EQ(expected_table_data, policy_handler_.GetPolicyTableData());
}

TEST_F(PolicyHandlerTest, GetRemovedVehicleDataItems_PolicyEnabled_SUCCESS) {
  using rpc::policy_table_interface_base::VehicleDataItem;

  ChangePolicyManagerToMock();

  std::vector<VehicleDataItem> expected_removed_items;
  expected_removed_items.push_back(VehicleDataItem());

  EXPECT_CALL(*mock_policy_manager_, GetRemovedVehicleDataItems())
      .WillOnce(Return(expected_removed_items));

  const auto& actually_removed_items =
      policy_handler_.GetRemovedVehicleDataItems();
  ASSERT_EQ(expected_removed_items.size(), actually_removed_items.size());
  EXPECT_TRUE(expected_removed_items[0] == actually_removed_items[0]);
}

TEST_F(PolicyHandlerTest, GetRemovedVehicleDataItems_PolicyDisabled_FAIL) {
  using rpc::policy_table_interface_base::VehicleDataItem;

  EXPECT_CALL(policy_settings_, enable_policy()).WillOnce(Return(false));

  policy_handler_.LoadPolicyLibrary();

  EXPECT_TRUE(policy_handler_.GetRemovedVehicleDataItems().empty());
}

TEST_F(PolicyHandlerTest, PopAppIdFromPTUQueue_PolicyEnabled_SUCCESS) {
  ChangePolicyManagerToMock();

  policy_handler_.PushAppIdToPTUQueue(kAppId1_);
  ASSERT_EQ(1u, policy_handler_.applications_ptu_queue_.size());

  policy_handler_.PopAppIdFromPTUQueue();
  EXPECT_EQ(0u, policy_handler_.applications_ptu_queue_.size());
}

TEST_F(PolicyHandlerTest, PopAppIdFromPTUQueue_PolicyDisabled_FAIL) {
  ChangePolicyManagerToMock();

  const uint32_t expected_apps_count = 0u;
  EXPECT_CALL(policy_settings_, enable_policy()).WillOnce(Return(false));
  EXPECT_CALL(*mock_policy_manager_,
              UpdatePTUReadyAppsCount(expected_apps_count))
      .Times(0);

  policy_handler_.LoadPolicyLibrary();
  policy_handler_.PopAppIdFromPTUQueue();
  EXPECT_EQ(expected_apps_count,
            policy_handler_.applications_ptu_queue_.size());
}

TEST_F(PolicyHandlerTest, OnLocalAppAdded_PolicyEnabled_SUCCESS) {
  ChangePolicyManagerToMock();
  EXPECT_CALL(*mock_policy_manager_, OnLocalAppAdded());
  policy_handler_.OnLocalAppAdded();
}

TEST_F(PolicyHandlerTest, OnPermissionsUpdated_PolicyEnabled_SUCCESS) {
  ChangePolicyManagerToMock();

  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_))
      .WillOnce(Return(mock_app_));

  const rpc::Optional<rpc::Boolean> encryption_requiered;
  EXPECT_CALL(*mock_policy_manager_, GetAppEncryptionRequired(kPolicyAppId_))
      .WillOnce(Return(encryption_requiered));
  EXPECT_CALL(*mock_app_, app_id()).WillRepeatedly(Return(kAppId1_));

  Permissions app_permissions;
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(kAppId1_, _, _, _));
  policy_handler_.OnPermissionsUpdated(
      kDeviceId_, kPolicyAppId_, app_permissions);
}

TEST_F(PolicyHandlerTest, OnPermissionsUpdated_PolicyDisabled_FAIL) {
  EXPECT_CALL(policy_settings_, enable_policy()).WillOnce(Return(false));
  policy_handler_.LoadPolicyLibrary();

  EXPECT_CALL(app_manager_, application(kDeviceId_, kPolicyAppId_)).Times(0);
  EXPECT_CALL(*mock_app_, app_id()).Times(0);

  Permissions app_permissions;
  EXPECT_CALL(mock_message_helper_,
              SendOnPermissionsChangeNotification(_, _, _, _))
      .Times(0);
  policy_handler_.OnPermissionsUpdated(
      kDeviceId_, kPolicyAppId_, app_permissions);
}

TEST_F(PolicyHandlerTest, IsNewApplication_PolicyEnabled_SUCCESS) {
  ChangePolicyManagerToMock();

  EXPECT_CALL(*mock_policy_manager_, IsNewApplication(kPolicyAppId_))
      .WillOnce(Return(true));
  EXPECT_TRUE(policy_handler_.IsNewApplication(kPolicyAppId_));
}

}  // namespace policy_handler_test
}  // namespace components
}  // namespace test

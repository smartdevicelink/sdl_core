/*
 * Copyright (c) 2015, Ford Motor Company
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
#include "config_profile/profile.h"
#include "policy_handler.h"
#include "mock_policy_manager.h"
#include "connection_handler/connection_handler_impl.h"
#include "application_manager/application_manager_impl.h"
#include "application_impl.h"
#include "security_manager_mock.h"
#include "mock_message_helper.h"
#include "policy/policy_types.h"
#include "json/reader.h"
#include "json/writer.h"
#include "json/value.h"
#include "smart_objects/smart_object.h"
#include "utils/file_system.h"
#include "utils/make_shared.h"
#include "usage_statistics/counter.h"
#include "usage_statistics/statistics_manager.h"
#include "interfaces/MOBILE_API.h"

namespace test {
namespace components {
namespace policy_handler {

using namespace application_manager;
using namespace policy;

using testing::_;
using ::testing::Return;

class PolicyHandlerTest : public ::testing::Test {
 public:
  PolicyHandlerTest()
      : instance_(NULL),
        app_manager_(NULL),
        app_id_("fake_app_id"),
        device_id_("fake_device_id"),
        hmi_level_("default"),
        rpc_("fake_rpc"),
        priority_("fake_priority"),
        default_hmi_("fake_hmi") {}

 protected:
  PolicyHandler* instance_;
  utils::SharedPtr<policy_manager::MockPolicyManager> pm_;
  ApplicationManagerImpl* app_manager_;
  const std::string app_id_;
  const std::string device_id_;
  const std::string hmi_level_;
  const std::string rpc_;
  std::string priority_;
  std::string default_hmi_;

  virtual void SetUp() OVERRIDE {
    instance_ = policy::PolicyHandler::instance();
    app_manager_ = ApplicationManagerImpl::instance();
    ASSERT_TRUE(instance_);
    ASSERT_TRUE(app_manager_);
    std::string path = file_system::CreateDirectory("storage");
    file_system::CreateFile(path + "/" + "certificate");
    pm_ = utils::MakeShared<policy_manager::MockPolicyManager>();
    ASSERT_TRUE(pm_.valid());
  }

  virtual void TearDown() OVERRIDE {
    policy::PolicyHandler::destroy();
    ApplicationManagerImpl::destroy();
    profile::Profile::instance()->config_file_name("smartDeviceLink.ini");
  }

  void ChangePolicyManagerToMock() { instance_->SetPolicyManager(pm_); }

  void EnablePolicy() {
    // Change default ini file to test ini file with policy enabled value
    profile::Profile::instance()->config_file_name("smartDeviceLink_test2.ini");
    EXPECT_TRUE(instance_->PolicyEnabled());
  }

  void EnablePolicyAndPolicyManagerMock() {
    EnablePolicy();
    ChangePolicyManagerToMock();
  }
};

TEST_F(PolicyHandlerTest, PolicyEnabled_ExpectCorrectValue) {
  // Check policy disabled
  EXPECT_FALSE(instance_->PolicyEnabled());
  // Enable policy & check
  EnablePolicy();
}

TEST_F(PolicyHandlerTest, Test_LoadPolicyLibrary_Method_ExpectLibraryLoaded) {
  // Check before policy enabled from ini file
  EXPECT_FALSE(instance_->PolicyEnabled());
  EXPECT_FALSE(instance_->LoadPolicyLibrary());
  EnablePolicy();
  // Check
  EXPECT_TRUE(instance_->LoadPolicyLibrary());
}

TEST_F(PolicyHandlerTest,
       InitPolicyTable_WithoutPreloadedFile_ExpectPolicyTableNotInitialized) {
  // Check
  EXPECT_FALSE(instance_->InitPolicyTable());
}

TEST_F(PolicyHandlerTest,
       InitPolicyTable_WithPreloadedFile_ExpectPolicyTableInitialized) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(instance_->LoadPolicyLibrary());
  // Check
  EXPECT_TRUE(instance_->InitPolicyTable());
}

TEST_F(PolicyHandlerTest,
       ResetPolicyTable_WithoutPreloadedFile_ExpectPolicyTableNotReset) {
  // Check
  EXPECT_FALSE(instance_->ResetPolicyTable());
}

TEST_F(PolicyHandlerTest,
       ResetPolicyTable_WithPreloadedFile_ExpectPolicyTableReset) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(instance_->LoadPolicyLibrary());
  // Check
  EXPECT_TRUE(instance_->ResetPolicyTable());
}

TEST_F(PolicyHandlerTest, ResetPolicyTable_ExpectCallPMResetPT) {
  ChangePolicyManagerToMock();
  EXPECT_CALL(*pm_, ResetPT(_));
  instance_->ResetPolicyTable();
}

TEST_F(PolicyHandlerTest, Test_ClearUserConsent_method) {
  EnablePolicyAndPolicyManagerMock();
  EXPECT_CALL(*pm_, ResetUserConsent());
  instance_->ClearUserConsent();
}

TEST_F(PolicyHandlerTest, Test_ReceiveMessageFromSDK_method) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(instance_->LoadPolicyLibrary());
  // Check
  EXPECT_TRUE(instance_->InitPolicyTable());
  ChangePolicyManagerToMock();
  std::string file_name("sdl_pt_update.json");
  std::ifstream ifile(file_name);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile != NULL && reader.parse(ifile, root, true)) {
    json = root.toStyledString();
  }
  ifile.close();
  BinaryMessage msg(json.begin(), json.end());
  // Checks
  EXPECT_CALL(*app_manager_, GetNextHMICorrelationID());
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              CreateGetVehicleDataRequest(_, _));
  EXPECT_CALL(*pm_, PTUpdatedAt(_, _));
  EXPECT_CALL(*pm_, LoadPT("", msg)).WillOnce(Return(true));
  EXPECT_CALL(*pm_, CleanupUnpairedDevices());
  instance_->ReceiveMessageFromSDK("", msg);
}

TEST_F(PolicyHandlerTest,
       Test_UnloadPolicyLibrary_method_ExpectLibraryUnloaded) {
  // Arrange
  EnablePolicy();
  EXPECT_TRUE(instance_->LoadPolicyLibrary());
  EXPECT_TRUE(instance_->InitPolicyTable());
  ChangePolicyManagerToMock();
  // Act
  EXPECT_TRUE(instance_->UnloadPolicyLibrary());
  // Check
  EXPECT_FALSE(instance_->InitPolicyTable());
}

TEST_F(PolicyHandlerTest, Test_OnPermissionsUpdated_method_With2Parameters) {
  // Check expectations
  EXPECT_CALL(*app_manager_, application_by_policy_id(_))
      .WillOnce(Return(ApplicationSharedPtr()));
  // Act
  Permissions perms;
  instance_->OnPermissionsUpdated(app_id_, perms);
}

TEST_F(PolicyHandlerTest, Test_OnPermissionsUpdated_method_With3Parameters) {
  // Check expectations
  EXPECT_CALL(*app_manager_, application_by_policy_id(_))
      .Times(2)
      .WillRepeatedly(Return(ApplicationSharedPtr()));
  // Act
  Permissions perms;
  instance_->OnPermissionsUpdated(app_id_, perms, hmi_level_);
}

TEST_F(PolicyHandlerTest, Test_GetPriority_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, GetPriority(app_id_, &priority_));
  // Act
  instance_->GetPriority(app_id_, &priority_);
}

TEST_F(PolicyHandlerTest, Test_CheckPermissions_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  CheckPermissionResult result;
  RPCParams rpc_params;
  // Check expectations
  EXPECT_CALL(*pm_, CheckPermissions(app_id_, hmi_level_, rpc_, rpc_params, _));
  // Act
  instance_->CheckPermissions(app_id_, hmi_level_, rpc_, rpc_params, result);
}

TEST_F(PolicyHandlerTest, Test_GetNotificationsNumber_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, GetNotificationsNumber(priority_));
  // Act
  instance_->GetNotificationsNumber(priority_);
}

TEST_F(PolicyHandlerTest, Test_GetUserConsentForDevice_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, GetUserConsentForDevice(device_id_))
      .WillOnce(Return(DeviceConsent::kDeviceHasNoConsent));
  // Act
  instance_->GetUserConsentForDevice(device_id_);
}

TEST_F(PolicyHandlerTest, Test_GetDefaultHmi_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, GetDefaultHmi(app_id_, &default_hmi_));
  // Act
  instance_->GetDefaultHmi(app_id_, &default_hmi_);
}

TEST_F(PolicyHandlerTest, Test_GetInitialAppData_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  StringArray* nicknames = NULL;
  StringArray* app_hmi_types = NULL;
  // Check expectations
  EXPECT_CALL(*pm_, GetInitialAppData(app_id_, nicknames, app_hmi_types));
  // Act
  instance_->GetInitialAppData(app_id_, nicknames, app_hmi_types);
}

TEST_F(PolicyHandlerTest, Test_GetUpdateUrls_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  EndpointUrls endpoints;
  const int service_type_ = 1;
  // Check expectations
  EXPECT_CALL(*pm_, GetUpdateUrls(service_type_, _));
  // Act
  instance_->GetUpdateUrls(service_type_, endpoints);
}

TEST_F(PolicyHandlerTest, Test_ResetRetrySequence_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, ResetRetrySequence());
  // Act
  instance_->ResetRetrySequence();
}

TEST_F(PolicyHandlerTest, Test_NextRetryTimeout_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, NextRetryTimeout());
  // Act
  instance_->NextRetryTimeout();
}

TEST_F(PolicyHandlerTest, Test_TimeoutExchange_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, TimeoutExchange());
  // Act
  instance_->TimeoutExchange();
}

TEST_F(PolicyHandlerTest, Test_OnExceededTimeout_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, OnExceededTimeout());
  // Act
  instance_->OnExceededTimeout();
}

TEST_F(PolicyHandlerTest, Test_OnSystemReady_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, OnSystemReady());
  // Act
  instance_->OnSystemReady();
}

TEST_F(PolicyHandlerTest, Test_PTUpdatedAt_method_UseCounter_KILOMETERS) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  const int value = 1000;
  // Check expectations
  EXPECT_CALL(*pm_, PTUpdatedAt(Counters::KILOMETERS, value));
  // Act
  instance_->PTUpdatedAt(Counters::KILOMETERS, value);
}

TEST_F(PolicyHandlerTest, Test_PTUpdatedAt_method_UseCounter_DAYS_AFTER_EPOCH) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  const int value = 16000;
  // Check expectations
  EXPECT_CALL(*pm_, PTUpdatedAt(Counters::DAYS_AFTER_EPOCH, value));
  // Act
  instance_->PTUpdatedAt(Counters::DAYS_AFTER_EPOCH, value);
}

TEST_F(PolicyHandlerTest, Test_CheckSystemAction_method_WithType_KEEP_CONTEXT) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  mobile_apis::SystemAction::eType system_action =
      mobile_apis::SystemAction::eType::KEEP_CONTEXT;
  // Check expectations
  EXPECT_CALL(*pm_, CanAppKeepContext(app_id_));
  // Act
  instance_->CheckSystemAction(system_action, app_id_);
}

TEST_F(PolicyHandlerTest, Test_CheckSystemAction_method_WithType_STEAL_FOCUS) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  mobile_apis::SystemAction::eType system_action =
      mobile_apis::SystemAction::eType::STEAL_FOCUS;
  // Check expectations
  EXPECT_CALL(*pm_, CanAppStealFocus(app_id_));
  // Act
  instance_->CheckSystemAction(system_action, app_id_);
}

TEST_F(PolicyHandlerTest,
       Test_CheckSystemAction_method_WithType_DEFAULT_ACTION) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  mobile_apis::SystemAction::eType system_action =
      mobile_apis::SystemAction::eType::DEFAULT_ACTION;
  // Check expectations
  EXPECT_CALL(*pm_, CanAppStealFocus(app_id_)).Times(0);
  EXPECT_CALL(*pm_, CanAppKeepContext(app_id_)).Times(0);
  // Act
  EXPECT_TRUE(instance_->CheckSystemAction(system_action, app_id_));
}

TEST_F(PolicyHandlerTest, Test_CheckSystemAction_method_WithType_INVALID_ENUM) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  mobile_apis::SystemAction::eType system_action =
      mobile_apis::SystemAction::eType::INVALID_ENUM;
  // Check expectations
  EXPECT_CALL(*pm_, CanAppStealFocus(app_id_)).Times(0);
  EXPECT_CALL(*pm_, CanAppKeepContext(app_id_)).Times(0);
  // Act
  EXPECT_FALSE(instance_->CheckSystemAction(system_action, app_id_));
}

TEST_F(PolicyHandlerTest, Test_KmsChanged_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  const int kilometers = 1600;
  // Check expectations
  EXPECT_CALL(*pm_, KmsChanged(kilometers));
  // Act
  instance_->KmsChanged(kilometers);
}

TEST_F(PolicyHandlerTest, Test_OnActivateApp_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  uint32_t app_id = 123;
  std::string policy_app_id("mobile_app_id");
  std::string app_name("my_mobile_app");
  const uint32_t connection_key = 1;
  const uint32_t correlation_id = 2;

  StateContext state_context;
  EXPECT_CALL(*app_manager_, CreateRegularState(app_id, _, _, _))
      .WillOnce(Return(HmiStatePtr(
          new HmiState(app_id, state_context, HmiState::STATE_ID_REGULAR))));

  ApplicationSharedPtr application1(new ApplicationImpl(
      app_id, policy_app_id, app_name, instance_->GetStatisticManager()));

  EXPECT_CALL(*app_manager_, application(connection_key))
      .Times(1)
      .WillRepeatedly(Return(application1));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              GetDeviceInfoForApp(_, _));

  AppPermissions permissions(policy_app_id);
  // Check expectations
  EXPECT_CALL(*pm_, GetAppPermissionsChanges(_)).WillOnce(Return(permissions));
  EXPECT_CALL(*pm_, GetUserConsentForDevice(_))
      .WillOnce(Return(DeviceConsent::kDeviceHasNoConsent));
  ON_CALL(*pm_, Increment(_, _)).WillByDefault(Return());
  EXPECT_CALL(*pm_, RemovePendingPermissionChanges(_));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendSDLActivateAppResponse(_, _));
  // Act
  instance_->OnActivateApp(connection_key, correlation_id);
}

TEST_F(PolicyHandlerTest, Test_OnIgnitionCycleOver_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*pm_, IncrementIgnitionCycles());
  // Act
  instance_->OnIgnitionCycleOver();
}

TEST_F(PolicyHandlerTest, Test_OnPendingPermissionChange_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  uint32_t app_id = 123;
  std::string policy_app_id("mobile_app_id");
  std::string app_name("my_mobile_app");

  StateContext state_context;
  EXPECT_CALL(*app_manager_, CreateRegularState(app_id, _, _, _))
      .WillOnce(Return(HmiStatePtr(
          new HmiState(app_id, state_context, HmiState::STATE_ID_REGULAR))));

  ApplicationSharedPtr application(new ApplicationImpl(
      app_id, policy_app_id, app_name, instance_->GetStatisticManager()));

  EXPECT_CALL(*app_manager_, application_by_policy_id(policy_app_id))
      .WillOnce(Return(application));
  AppPermissions permissions(policy_app_id);
  EXPECT_CALL(*pm_, GetAppPermissionsChanges(_)).WillOnce(Return(permissions));
  EXPECT_CALL(*pm_, RemovePendingPermissionChanges(policy_app_id)).Times(0);
  // Act
  instance_->OnPendingPermissionChange(policy_app_id);
}

TEST_F(PolicyHandlerTest, Test_PTExchangeAtUserRequest_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*pm_, ForcePTExchange());
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendUpdateSDLResponse(_, _));
  // Act
  const uint32_t correlation_id = 2;
  instance_->PTExchangeAtUserRequest(correlation_id);
}

TEST_F(PolicyHandlerTest, Test_AddDevice_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  const std::string connection_type("BT");
  EXPECT_CALL(*pm_, AddDevice(device_id_, connection_type));
  // Act
  instance_->AddDevice(device_id_, connection_type);
}

TEST_F(PolicyHandlerTest, Test_SetDeviceInfo_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  std::string device_id_;
  const DeviceInfo device_info;
  EXPECT_CALL(*pm_, SetDeviceInfo(device_id_, _));
  // Act
  instance_->SetDeviceInfo(device_id_, device_info);
}

TEST_F(PolicyHandlerTest, Test_OnGetUserFriendlyMessage_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  std::vector<std::string> message_codes;
  const std::string language("ru-ru");
  const uint32_t correlation_id = 2;
  EXPECT_CALL(*pm_, GetUserFriendlyMessages(message_codes, language))
      .WillOnce(Return(std::vector<UserFriendlyMessage>()));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendGetUserFriendlyMessageResponse(_, _));
  // Act
  instance_->OnGetUserFriendlyMessage(message_codes, language, correlation_id);
}

TEST_F(PolicyHandlerTest, Test_OnGetStatusUpdate_method) {
  // Arrange
  ChangePolicyManagerToMock();
  const uint32_t correlation_id = 2;
  // Check expectations
  EXPECT_CALL(*pm_, GetPolicyTableStatus());
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendGetStatusUpdateResponse(_, correlation_id));
  // Act
  instance_->OnGetStatusUpdate(correlation_id);
}

TEST_F(PolicyHandlerTest, Test_OnUpdateStatusChanged_method) {
  // Check expectations
  const std::string& status("new status");
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnStatusUpdate(status));
  // Act
  instance_->OnUpdateStatusChanged(status);
}

TEST_F(PolicyHandlerTest, Test_OnCurrentDeviceIdUpdateRequired_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  uint32_t app_id = 123;
  std::string policy_app_id("mobile_app_id");
  std::string app_name("my_mobile_app");

  StateContext state_context;
  EXPECT_CALL(*app_manager_, CreateRegularState(app_id, _, _, _))
      .WillOnce(Return(HmiStatePtr(
          new HmiState(app_id, state_context, HmiState::STATE_ID_REGULAR))));

  ApplicationSharedPtr application(new ApplicationImpl(
      app_id, policy_app_id, app_name, instance_->GetStatisticManager()));

  EXPECT_CALL(*app_manager_, application_by_policy_id(policy_app_id))
      .WillOnce(Return(application));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              GetDeviceInfoForApp(_, _));
  // Act
  instance_->OnCurrentDeviceIdUpdateRequired(policy_app_id);
}

TEST_F(PolicyHandlerTest, Test_OnSystemInfoChanged_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  const std::string language("ru-ru");
  EXPECT_CALL(*pm_, SetSystemLanguage(language));
  // Act
  instance_->OnSystemInfoChanged(language);
}

TEST_F(PolicyHandlerTest, Test_OnGetSystemInfo_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  const std::string ccpu_version("4.1.3.B_EB355B");
  const std::string wers_country_code("WAEGB");
  const std::string language("ru-ru");
  EXPECT_CALL(*pm_, SetSystemInfo(ccpu_version, wers_country_code, language));
  // Act
  instance_->OnGetSystemInfo(ccpu_version, wers_country_code, language);
}

TEST_F(PolicyHandlerTest, Test_IsApplicationRevoked_method) {
  // Arrange
  EnablePolicy();
  std::string policy_app_id("mobile_app_id");
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*pm_, IsApplicationRevoked(policy_app_id));
  // Act
  instance_->IsApplicationRevoked(policy_app_id);
}

TEST_F(PolicyHandlerTest, Test_OnSystemInfoUpdateRequired_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendGetSystemInfoRequest());
  // Act
  instance_->OnSystemInfoUpdateRequired();
}

TEST_F(PolicyHandlerTest, Test_GetAppRequestTypes_method) {
  // Arrange
  EnablePolicy();
  ChangePolicyManagerToMock();
  // Check expectations
  std::string policy_app_id("mobile_app_id");
  EXPECT_CALL(*pm_, GetAppRequestTypes(policy_app_id))
      .WillOnce(Return(std::vector<std::string>()));
  // Act
  instance_->GetAppRequestTypes(policy_app_id);
}

TEST_F(PolicyHandlerTest, Test_OnVIIsReady_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*app_manager_, GetNextHMICorrelationID());
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              CreateGetVehicleDataRequest(_, _));
  // Act
  instance_->OnVIIsReady();
}

TEST_F(PolicyHandlerTest, Test_OnVehicleDataUpdated_method) {
  // Arrange
  ChangePolicyManagerToMock();
  // Check expectations
  EXPECT_CALL(*pm_, SetVINValue(_));
  // Act
  ::smart_objects::SmartObject message_(::smart_objects::SmartType_Map);
  message_[strings::msg_params][strings::vin] = "XXXXX";
  instance_->OnVehicleDataUpdated(message_);
}

TEST_F(PolicyHandlerTest, Test_RemoveDevice_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  connection_handler::ConnectionHandlerImpl* conn_handler =
      connection_handler::ConnectionHandlerImpl::instance();
  EXPECT_CALL(*app_manager_, connection_handler())
      .WillOnce(Return(conn_handler));
  EXPECT_CALL(*pm_, MarkUnpairedDevice(device_id_));
  // Act
  instance_->RemoveDevice(device_id_);
}

TEST_F(PolicyHandlerTest, Test_GetAppName_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  uint32_t app_id = 123;
  std::string policy_app_id("mobile_app_id");
  std::string app_name("my_mobile_app");

  StateContext state_context;
  EXPECT_CALL(*app_manager_, CreateRegularState(app_id, _, _, _))
      .WillOnce(Return(HmiStatePtr(
          new HmiState(app_id, state_context, HmiState::STATE_ID_REGULAR))));

  ApplicationSharedPtr application(new ApplicationImpl(
      app_id, policy_app_id, app_name, instance_->GetStatisticManager()));

  EXPECT_CALL(*app_manager_, application_by_policy_id(policy_app_id))
      .WillOnce(Return(application));
  // Act
  EXPECT_EQ(app_name, instance_->GetAppName(policy_app_id));
}

TEST_F(PolicyHandlerTest, Test_OnUpdateRequestSentToMobile_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, OnUpdateStarted());
  // Act
  instance_->OnUpdateRequestSentToMobile();
}

TEST_F(PolicyHandlerTest, Test_OnUpdateHMIAppType_method) {
  // Arrange
  EnablePolicy();
  instance_->add_listener(app_manager_);
  std::map<std::string, StringArray> app_hmi_types;
  StringArray arr;
  arr.push_back("test_hmi_type");
  app_hmi_types["app1"] = arr;
  // Check expectations
  EXPECT_CALL(*app_manager_, OnUpdateHMIAppType(_));
  // Act
  instance_->OnUpdateHMIAppType(app_hmi_types);
}

TEST_F(PolicyHandlerTest, Test_OnCertificateDecrypted_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();

  ::test::components::security_manager_test::CryptoManagerMock crypto_manager;
  // security_manager::CryptoManagerImpl crypto_manager;
  instance_->add_listener(&crypto_manager);
  // Check expectations
  EXPECT_CALL(crypto_manager, OnCertificateUpdated(_)).Times(0);
  EXPECT_CALL(crypto_manager, OnCertificateUpdated(_)).Times(1);
  // Check expectations
  EXPECT_CALL(*pm_, SetDecryptedCertificate(_)).Times(0);
  EXPECT_CALL(*pm_, SetDecryptedCertificate(_)).Times(1);
  // Act
  instance_->OnCertificateDecrypted(true);
  instance_->OnCertificateDecrypted(false);
}

TEST_F(PolicyHandlerTest, Test_SendOnAppPermissionsChanged_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  uint32_t app_id = 123;
  std::string policy_app_id("mobile_app_id");
  std::string app_name("my_mobile_app");

  StateContext state_context;
  EXPECT_CALL(*app_manager_, CreateRegularState(app_id, _, _, _))
      .WillOnce(Return(HmiStatePtr(
          new HmiState(app_id, state_context, HmiState::STATE_ID_REGULAR))));

  ApplicationSharedPtr application(new ApplicationImpl(
      app_id, policy_app_id, app_name,
      policy::PolicyHandler::instance()->GetStatisticManager()));
  // Check expectations
  EXPECT_CALL(*app_manager_, application_by_policy_id(policy_app_id))
      .WillOnce(Return(application));
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(),
              SendOnAppPermissionsChangedNotification(_, _));
  AppPermissions permissions(policy_app_id);
  // Act
  instance_->SendOnAppPermissionsChanged(permissions, policy_app_id);
}

TEST_F(PolicyHandlerTest, Test_OnPTExchangeNeeded_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, ForcePTExchange());
  EXPECT_CALL(*MockMessageHelper::message_helper_mock(), SendOnStatusUpdate(_));
  // Act
  instance_->OnPTExchangeNeeded();
}

TEST_F(PolicyHandlerTest, Test_AddApplication_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  std::string policy_app_id("mobile_app_id");
  // Check expectations
  EXPECT_CALL(*pm_, AddApplication(policy_app_id));
  // Act
  instance_->AddApplication(policy_app_id);
}

TEST_F(PolicyHandlerTest, Test_HeartBeatTimeout_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  std::string policy_app_id("mobile_app_id");
  EXPECT_CALL(*pm_, HeartBeatTimeout(policy_app_id));
  // Act
  instance_->HeartBeatTimeout(policy_app_id);
}

TEST_F(PolicyHandlerTest, Test_OnAppsSearchStarted_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, OnAppsSearchStarted());
  // Act
  instance_->OnAppsSearchStarted();
}

TEST_F(PolicyHandlerTest, Test_OnAppsSearchCompleted_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, OnAppsSearchCompleted());
  // Act
  instance_->OnAppsSearchCompleted();
}

TEST_F(PolicyHandlerTest, Test_OnAppRegisteredOnMobile_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  std::string policy_app_id("mobile_app_id");
  EXPECT_CALL(*pm_, OnAppRegisteredOnMobile(policy_app_id));
  // Act
  instance_->OnAppRegisteredOnMobile(policy_app_id);
}

TEST_F(PolicyHandlerTest, Test_IsRequestTypeAllowed_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  std::string policy_app_id("mobile_app_id");
  mobile_apis::RequestType::eType type =
      mobile_apis::RequestType::eType::EMERGENCY;
  EXPECT_CALL(*pm_, GetAppRequestTypes(policy_app_id))
      .WillOnce(Return(std::vector<std::string>()));
  // Act
  instance_->IsRequestTypeAllowed(policy_app_id, type);
}

TEST_F(PolicyHandlerTest, Test_GetVehicleInfo_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, GetVehicleInfo()).WillOnce(Return(VehicleInfo()));
  // Act
  instance_->GetVehicleInfo();
}

TEST_F(PolicyHandlerTest, Test_GetMetaInfo_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  EXPECT_CALL(*pm_, GetMetaInfo()).WillOnce(Return(MetaInfo()));
  // Act
  instance_->GetMetaInfo();
}

TEST_F(PolicyHandlerTest, Test_Increment_method_WithOneParameter) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  usage_statistics::GlobalCounterId type =
      usage_statistics::GlobalCounterId::IAP_BUFFER_FULL;
  EXPECT_CALL(*pm_, Increment(type));
  // Act
  instance_->Increment(type);
}

TEST_F(PolicyHandlerTest, Test_Increment_method_WithTwoParameters) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  std::string policy_app_id("mobile_app_id");
  usage_statistics::AppCounterId type =
      usage_statistics::AppCounterId::USER_SELECTIONS;
  EXPECT_CALL(*pm_, Increment(policy_app_id, type));
  // Act
  instance_->Increment(policy_app_id, type);
}

TEST_F(PolicyHandlerTest, Test_Set_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  std::string policy_app_id("mobile_app_id");
  const std::string value("ru-ru");
  usage_statistics::AppInfoId type = usage_statistics::AppInfoId::LANGUAGE_GUI;
  EXPECT_CALL(*pm_, Set(policy_app_id, type, value));
  // Act
  instance_->Set(policy_app_id, type, value);
}

TEST_F(PolicyHandlerTest, Test_Add_method) {
  // Arrange
  EnablePolicyAndPolicyManagerMock();
  // Check expectations
  std::string policy_app_id("mobile_app_id");
  int32_t timespan_seconds = 100;
  usage_statistics::AppStopwatchId type =
      usage_statistics::AppStopwatchId::SECONDS_HMI_FULL;
  EXPECT_CALL(*pm_, Add(policy_app_id, type, timespan_seconds));
  // Act
  instance_->Add(policy_app_id, type, timespan_seconds);
}

}  // namespace policy_handler
}  // namespace components
}  // namespace test

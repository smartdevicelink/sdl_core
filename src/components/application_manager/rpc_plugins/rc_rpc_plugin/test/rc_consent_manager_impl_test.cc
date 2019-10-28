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

#include <ctime>
#include <memory>
#include <string>
#include <vector>

#include "gmock/gmock.h"
#include "gtest/gtest.h"

#include "application_manager/mock_application_manager_settings.h"
#include "rc_rpc_plugin/rc_consent_manager_impl.h"
#include "resumption/last_state.h"
#include "resumption/last_state_impl.h"
#include "utils/date_time.h"
#include "utils/file_system.h"

#include "application_manager/mock_application_manager.h"

using test::components::application_manager_test::MockApplicationManager;
using test::components::application_manager_test::
    MockApplicationManagerSettings;

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const std::string kPolicyApp1Id = "app1_id";
const std::string kPolicyApp2Id = "app2_id";
const std::string kMacAddress1 = "device1";
const std::string kMacAddress2 = "device2";
const std::string kClimateModule = "CLIMATE";
const std::string kRadioModule = "RADIO";
const std::string kAppTestStorageFolder = "app_test_storage_folder";
const std::string kAppTestInfoStorage = "app_test_info_storage";
const uint32_t kPeriodOfConsentExpired = 30;  // in days
}  // namespace

namespace rc_rpc_plugin_test {
namespace rc_consent_manager_test {
using namespace rc_rpc_plugin::rc_rpc_types;
using namespace rc_rpc_plugin;

class RCConsentManagerImplTest : public ::testing::Test {
 public:
  RCConsentManagerImplTest()
      : current_date_(0u)
      , last_state_(new resumption::LastStateImpl(kAppTestStorageFolder,
                                                  kAppTestInfoStorage))
      , rc_consent_manager_(new rc_rpc_plugin::RCConsentManagerImpl(
            *last_state_, mock_app_mngr_, kPeriodOfConsentExpired)) {}

  void SetUp() OVERRIDE {
    current_date_ = std::time(0);
    RemoveTemproraryTetsFiles();

    ON_CALL(mock_app_mngr_, get_settings())
        .WillByDefault(ReturnRef(mock_app_mnrg_settings_));
    ON_CALL(mock_app_mnrg_settings_, period_for_consent_expiration())
        .WillByDefault(Return(kPeriodOfConsentExpired));
  }

  void RemoveTemproraryTetsFiles() {
    // Remove exists storage file
    if (file_system::FileExists(kAppTestInfoStorage)) {
      file_system::DeleteFile(kAppTestInfoStorage);
    }

    // Remove exists storage folder
    if (file_system::DirectoryExists(kAppTestStorageFolder)) {
      file_system::RemoveDirectory(kAppTestStorageFolder);
    }

    last_state_->get_dictionary().clear();
  }

 protected:
  time_t current_date_;
  NiceMock<MockApplicationManagerSettings> mock_app_mnrg_settings_;
  NiceMock<MockApplicationManager> mock_app_mngr_;
  std::unique_ptr<resumption::LastState> last_state_;
  std::unique_ptr<rc_rpc_plugin::RCConsentManagerImpl> rc_consent_manager_;
};

TEST_F(RCConsentManagerImplTest, SaveAndGetModuleConsents_SUCCESS) {
  const std::string radio_moduleId = "eb7739ea-b263-4fe1-af9c-9311d1acac2d";
  const std::string climate_moduleId = "34045662-a9dc-4823-8435-91056d4c26cb";

  ModuleIdConsent radio_module_consent{
      {kRadioModule, radio_moduleId}, ModuleConsent::CONSENTED, current_date_};

  ModuleIdConsent climate_module_consent{{kClimateModule, climate_moduleId},
                                         ModuleConsent::NOT_CONSENTED,
                                         current_date_};

  ModuleIdConsentVector module_consents{radio_module_consent,
                                        climate_module_consent};

  rc_consent_manager_->SaveModuleConsents(
      kPolicyApp1Id, kMacAddress1, module_consents);
  last_state_->SaveStateToFileSystem();

  auto radio_consent = rc_consent_manager_->GetModuleConsent(
      kPolicyApp1Id, kMacAddress1, {kRadioModule, radio_moduleId});
  auto climate_consent = rc_consent_manager_->GetModuleConsent(
      kPolicyApp1Id, kMacAddress1, {kClimateModule, climate_moduleId});

  EXPECT_EQ(ModuleConsent::CONSENTED, radio_consent);
  EXPECT_EQ(ModuleConsent::NOT_CONSENTED, climate_consent);
}

TEST_F(RCConsentManagerImplTest, ModuleId_NOT_EXISTS) {
  const std::string radio_moduleId = "eb7739ea-b263-4fe1-af9c-9311d1acac2d";
  const std::string climate_moduleId = "34045662-a9dc-4823-8435-91056d4c26cb";

  ModuleIdConsent radio_module_consent{
      {kRadioModule, radio_moduleId}, ModuleConsent::CONSENTED, current_date_};
  ModuleIdConsent climate_module_consent{{kClimateModule, climate_moduleId},
                                         ModuleConsent::CONSENTED,
                                         current_date_};

  const std::string uknown_moduleId = "639f1b7f-1d25-4dca-a2ce-00530860adea";
  const std::string uknown_moduleType = "UNKNOWN_TYPE";

  ModuleUid unknown_module{uknown_moduleId, uknown_moduleType};

  ModuleIdConsentVector module_consents{radio_module_consent,
                                        climate_module_consent};

  rc_consent_manager_->SaveModuleConsents(
      kPolicyApp1Id, kMacAddress1, module_consents);
  last_state_->SaveStateToFileSystem();

  ModuleConsent consent = rc_consent_manager_->GetModuleConsent(
      kPolicyApp1Id, kMacAddress1, unknown_module);

  EXPECT_EQ(ModuleConsent::NOT_EXISTS, consent);
}

TEST_F(RCConsentManagerImplTest, ConsentIsExpired) {
  time_t expired_date_45_days =
      current_date_ - (date_time::SECONDS_IN_DAY * 45);
  time_t expired_date_30_days =
      current_date_ - (date_time::SECONDS_IN_DAY * 30);
  time_t not_expired_date_29_days =
      current_date_ - (date_time::SECONDS_IN_DAY * 29);

  const std::string moduleId1 = "dbef7693-adf9-42f1-95c7-5dff314d9985";
  const std::string moduleId2 = "1536b2ac-213f-463f-8589-a11453cd3729";
  const std::string moduleId3 = "e8b8169a-9f29-4499-a60b-2d126a1beffc";

  ModuleUid module_resource1{kRadioModule, moduleId1};
  ModuleUid module_resource2{kRadioModule, moduleId2};
  ModuleUid module_resource3{kRadioModule, moduleId3};

  ModuleIdConsent module_consent_expired1{
      module_resource1, ModuleConsent::CONSENTED, expired_date_45_days};
  ModuleIdConsent module_consent_expired2{
      module_resource2, ModuleConsent::CONSENTED, expired_date_30_days};

  ModuleIdConsent module_consent_not_expired{
      module_resource3, ModuleConsent::CONSENTED, not_expired_date_29_days};

  ModuleIdConsentVector module_consents{module_consent_expired1,
                                        module_consent_expired2,
                                        module_consent_not_expired};
  rc_consent_manager_->SaveModuleConsents(
      kPolicyApp1Id, kMacAddress1, module_consents);
  last_state_->SaveStateToFileSystem();

  // All consents, which consent has been saved more than 30 days, will be
  // removed.
  rc_consent_manager_->RemoveExpiredConsents();

  ModuleConsent expired_consent_45_days = rc_consent_manager_->GetModuleConsent(
      kPolicyApp1Id, kMacAddress1, module_resource1);
  EXPECT_EQ(ModuleConsent::NOT_EXISTS, expired_consent_45_days);

  ModuleConsent expired_consent_30_days = rc_consent_manager_->GetModuleConsent(
      kPolicyApp1Id, kMacAddress1, module_resource2);
  EXPECT_EQ(ModuleConsent::NOT_EXISTS, expired_consent_30_days);

  ModuleConsent not_expired_consent = rc_consent_manager_->GetModuleConsent(
      kPolicyApp1Id, kMacAddress1, module_resource3);
  EXPECT_EQ(ModuleConsent::CONSENTED, not_expired_consent);
}

TEST_F(RCConsentManagerImplTest,
       SaveModuleConsents_SameAppID_DifferentDeviceID_NotReplaced) {
  const std::string moduleId = "cef4eae6-e22c-4943-bdc3-60e792414c4c";
  ModuleUid module_resource{kRadioModule, moduleId};

  ModuleIdConsent radio_module_consent_app1_deviceId1{
      module_resource, ModuleConsent::CONSENTED, current_date_};

  ModuleIdConsent radio_module_consent_app1_deviceId2{
      module_resource, ModuleConsent::NOT_CONSENTED, current_date_};

  ModuleIdConsentVector module_consents_app1{
      radio_module_consent_app1_deviceId1};
  ModuleIdConsentVector module_consents_app2{
      radio_module_consent_app1_deviceId2};

  rc_consent_manager_->SaveModuleConsents(
      kPolicyApp1Id, kMacAddress1, module_consents_app1);

  rc_consent_manager_->SaveModuleConsents(
      kPolicyApp1Id, kMacAddress2, module_consents_app2);

  last_state_->SaveStateToFileSystem();

  // Module consents for same app ids and different device_id (mac_adress) will
  // haven't been replaced each other

  ModuleConsent module_consent_deviceID1 =
      rc_consent_manager_->GetModuleConsent(
          kPolicyApp1Id, kMacAddress1, module_resource);

  ModuleConsent module_consent_deviceID2 =
      rc_consent_manager_->GetModuleConsent(
          kPolicyApp1Id, kMacAddress2, module_resource);

  EXPECT_EQ(ModuleConsent::CONSENTED, module_consent_deviceID1);

  //  // Module consent for app_id2 has different value as has been saved above
  EXPECT_EQ(ModuleConsent::NOT_CONSENTED, module_consent_deviceID2);
}

TEST_F(RCConsentManagerImplTest,
       SaveModuleConsents_SameAppID_SameDeviceID_Replaced) {
  const std::string moduleId = "194aee9e-26cb-4f48-a775-8841a658002d";

  ModuleUid radio_module_resource_consented{kRadioModule, moduleId};
  ModuleUid radio_module_resource_not_consented{kRadioModule, moduleId};

  ModuleIdConsent radio_module_consent_app1_deviceId1{
      radio_module_resource_consented, ModuleConsent::CONSENTED, current_date_};

  ModuleIdConsent radio_module_consent_app1_deviceId2{
      radio_module_resource_not_consented,
      ModuleConsent::NOT_CONSENTED,
      current_date_};

  ModuleIdConsentVector module_consents_app1{
      radio_module_consent_app1_deviceId1};
  ModuleIdConsentVector module_consents_app2{
      radio_module_consent_app1_deviceId2};

  rc_consent_manager_->SaveModuleConsents(
      kPolicyApp1Id, kMacAddress1, module_consents_app1);

  last_state_->SaveStateToFileSystem();

  rc_consent_manager_->SaveModuleConsents(
      kPolicyApp1Id, kMacAddress1, module_consents_app2);

  last_state_->SaveStateToFileSystem();

  // Module consents for same app ids and same device_id (mac_adress) will
  // have been replaced each other

  ModuleConsent module_consent = rc_consent_manager_->GetModuleConsent(
      kPolicyApp1Id, kMacAddress1, radio_module_resource_not_consented);

  EXPECT_EQ(ModuleConsent::NOT_CONSENTED, module_consent);
}

TEST_F(RCConsentManagerImplTest, RemoveAllModuleConsents_SUCCESS) {
  const std::string moduleId = "1c5408f2-9766-464c-a7bf-d834b62f43fa";

  ModuleUid module_resource{kRadioModule, moduleId};

  ModuleIdConsent radio_module_consent_app1_deviceId1{
      module_resource, ModuleConsent::CONSENTED, current_date_};

  ModuleIdConsent radio_module_consent_app1_deviceId2{
      module_resource, ModuleConsent::NOT_CONSENTED, current_date_};

  ModuleIdConsentVector module_consents_app1{
      radio_module_consent_app1_deviceId1};
  ModuleIdConsentVector module_consents_app2{
      radio_module_consent_app1_deviceId2};

  rc_consent_manager_->SaveModuleConsents(
      kPolicyApp1Id, kMacAddress1, module_consents_app1);

  rc_consent_manager_->SaveModuleConsents(
      kPolicyApp1Id, kMacAddress2, module_consents_app2);

  last_state_->SaveStateToFileSystem();

  rc_consent_manager_->RemoveAllConsents();

  ModuleConsent module_consent_deviceID1 =
      rc_consent_manager_->GetModuleConsent(
          kPolicyApp1Id, kMacAddress1, module_resource);

  ModuleConsent module_consent_deviceID2 =
      rc_consent_manager_->GetModuleConsent(
          kPolicyApp1Id, kMacAddress2, module_resource);

  EXPECT_EQ(ModuleConsent::NOT_EXISTS, module_consent_deviceID1);

  EXPECT_EQ(ModuleConsent::NOT_EXISTS, module_consent_deviceID2);
}

}  // namespace rc_consent_manager_test
}  // namespace rc_rpc_plugin_test

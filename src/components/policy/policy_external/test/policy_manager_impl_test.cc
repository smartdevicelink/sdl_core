/*
 * Copyright (c) 2020, Ford Motor Company
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
#include <utility>

#include "gtest/gtest.h"

#include "json/reader.h"
#include "utils/date_time.h"
#include "utils/file_system.h"
#include "utils/gen_hash.h"
#include "utils/jsoncpp_reader_wrapper.h"

#include "policy/policy_manager.h"
#include "policy/policy_manager_impl.h"

#include "policy/mock_cache_manager.h"
#include "policy/mock_policy_listener.h"
#include "policy/mock_policy_settings.h"

namespace test {
namespace components {
namespace policy_test {

using namespace policy;
using namespace rpc::policy_table_interface_base;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

namespace {
const std::string kSdlPreloadedPtJson = "json/sdl_preloaded_pt.json";
const int kServiceTypeInt = 0;
const std::string kServiceTypeString = "0x07";
}  // namespace

class PolicyManagerImplTest : public ::testing::Test {
 public:
  PolicyManagerImplTest()
      : unpaired_device_id_("08-00-27-CE-76-FE")
      , app_id_("1234")
      , device_id_("XXX123456789ZZZ")
      , policy_manager_(NULL)
      , cache_manager_(NULL)
      , in_memory_(true)
      , app_storage_folder_("app_storage_folder") {}

 protected:
  const std::string unpaired_device_id_;
  const std::string app_id_;
  const std::string device_id_;

  PolicyManagerImpl* policy_manager_;
  NiceMock<MockCacheManagerInterface>* cache_manager_;
  NiceMock<MockPolicyListener> listener_;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;
  bool in_memory_;
  const std::string app_storage_folder_;

  void SetUp() {
    policy_manager_ = new PolicyManagerImpl(in_memory_);
    cache_manager_ = new NiceMock<MockCacheManagerInterface>();
    policy_manager_->set_cache_manager(cache_manager_);
    policy_manager_->set_listener(&listener_);

    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(app_storage_folder_));
  }

  void TearDown() {
    delete policy_manager_;
  }

  ::testing::AssertionResult IsValid(const policy_table::Table& table) {
    if (table.is_valid()) {
      return ::testing::AssertionSuccess();
    } else {
      ::rpc::ValidationReport report(" - table");
      table.ReportErrors(&report);
      return ::testing::AssertionFailure() << ::rpc::PrettyFormat(report);
    }
  }
};

TEST_F(PolicyManagerImplTest, GetNotificationsNumber) {
  std::string priority = "EMERGENCY";
  uint32_t notif_number = 100;
  EXPECT_CALL(*cache_manager_, GetNotificationsNumber(priority))
      .WillOnce(Return(notif_number));

  EXPECT_EQ(notif_number, policy_manager_->GetNotificationsNumber(priority));
}

TEST_F(PolicyManagerImplTest, IncrementGlobalCounter) {
  EXPECT_CALL(*cache_manager_, Increment(usage_statistics::SYNC_REBOOTS));
  policy_manager_->Increment(usage_statistics::SYNC_REBOOTS);
}

TEST_F(PolicyManagerImplTest, IncrementAppCounter) {
  EXPECT_CALL(*cache_manager_,
              Increment("12345", usage_statistics::USER_SELECTIONS));
  policy_manager_->Increment("12345", usage_statistics::USER_SELECTIONS);
}

TEST_F(PolicyManagerImplTest, SetAppInfo) {
  EXPECT_CALL(*cache_manager_,
              Set("12345", usage_statistics::LANGUAGE_GUI, "de-de"));
  policy_manager_->Set("12345", usage_statistics::LANGUAGE_GUI, "de-de");
}

TEST_F(PolicyManagerImplTest, AddAppStopwatch) {
  EXPECT_CALL(*cache_manager_,
              Add("12345", usage_statistics::SECONDS_HMI_FULL, 30));
  policy_manager_->Add("12345", usage_statistics::SECONDS_HMI_FULL, 30);
}

TEST_F(PolicyManagerImplTest, IncrementIgnitionCycles) {
  EXPECT_CALL(*cache_manager_, IncrementIgnitionCycles());
  policy_manager_->IncrementIgnitionCycles();
}

TEST_F(PolicyManagerImplTest, SetCloudAppEnabled) {
  EXPECT_CALL(*cache_manager_, SetCloudAppEnabled(app_id_, true));
  policy_manager_->SetCloudAppEnabled(app_id_, true);
}

TEST_F(PolicyManagerImplTest, InitCloudApp) {
  EXPECT_CALL(*cache_manager_, InitCloudApp(app_id_));
  policy_manager_->InitCloudApp(app_id_);
}

TEST_F(PolicyManagerImplTest, SetAppAuthToken) {
  EXPECT_CALL(*cache_manager_, SetAppAuthToken(app_id_, "auth_token"));
  policy_manager_->SetAppAuthToken(app_id_, "auth_token");
}

TEST_F(PolicyManagerImplTest, SetAppCloudTransportType) {
  EXPECT_CALL(*cache_manager_,
              SetAppCloudTransportType(app_id_, "transport_type"));
  policy_manager_->SetAppCloudTransportType(app_id_, "transport_type");
}

TEST_F(PolicyManagerImplTest, SetAppEndpoint) {
  EXPECT_CALL(*cache_manager_, SetAppEndpoint(app_id_, "endpoint"));
  policy_manager_->SetAppEndpoint(app_id_, "endpoint");
}

TEST_F(PolicyManagerImplTest, SetHybridAppPreference) {
  EXPECT_CALL(*cache_manager_,
              SetHybridAppPreference(app_id_, "app_preference"));
  policy_manager_->SetHybridAppPreference(app_id_, "app_preference");
}

TEST_F(PolicyManagerImplTest,
       UnknownRPCPassthroughAllowed_CacheReturnsTrue_ReturnTrue) {
  EXPECT_CALL(*cache_manager_, UnknownRPCPassthroughAllowed(app_id_))
      .WillOnce(Return(true));
  EXPECT_TRUE(policy_manager_->UnknownRPCPassthroughAllowed(app_id_));
}

TEST_F(PolicyManagerImplTest,
       UnknownRPCPassthroughAllowed_CacheReturnsFalse_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, UnknownRPCPassthroughAllowed(app_id_))
      .WillOnce(Return(false));
  EXPECT_FALSE(policy_manager_->UnknownRPCPassthroughAllowed(app_id_));
}

TEST_F(PolicyManagerImplTest, InitPT_NoAppStorageFolder_ReturnFalse) {
  EXPECT_FALSE(policy_manager_->InitPT(kSdlPreloadedPtJson, &policy_settings_));
}

TEST_F(PolicyManagerImplTest, InitPT_CacheReturnsFalse_ReturnFalse) {
  file_system::CreateDirectory(app_storage_folder_);
  EXPECT_CALL(*cache_manager_, Init(kSdlPreloadedPtJson, &policy_settings_))
      .WillOnce(Return(false));
  EXPECT_FALSE(policy_manager_->InitPT(kSdlPreloadedPtJson, &policy_settings_));
  file_system::RemoveDirectory(app_storage_folder_, true);
}

TEST_F(PolicyManagerImplTest, InitPT_CacheReturnsTrue_ReturnTrue) {
  file_system::CreateDirectory(app_storage_folder_);
  EXPECT_CALL(*cache_manager_, Init(kSdlPreloadedPtJson, &policy_settings_))
      .WillOnce(Return(true));
  EXPECT_TRUE(policy_manager_->InitPT(kSdlPreloadedPtJson, &policy_settings_));
  file_system::RemoveDirectory(app_storage_folder_, true);
}

TEST_F(PolicyManagerImplTest, ResetPT_CacheReturnsFalse_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, ResetCalculatedPermissions());
  EXPECT_CALL(*cache_manager_, ResetPT(kSdlPreloadedPtJson))
      .WillOnce(Return(false));

  EXPECT_FALSE(policy_manager_->ResetPT(kSdlPreloadedPtJson));
}

TEST_F(PolicyManagerImplTest, ResetPT_CacheReturnsTrue_ReturnTrue) {
  EXPECT_CALL(*cache_manager_, ResetCalculatedPermissions());
  EXPECT_CALL(*cache_manager_, ResetPT(kSdlPreloadedPtJson))
      .WillOnce(Return(true));

  EXPECT_TRUE(policy_manager_->ResetPT(kSdlPreloadedPtJson));
}

TEST_F(PolicyManagerImplTest, AppNeedEncryption_EncryptionRequired_ReturnTrue) {
  EncryptionRequired encryption_required;
  *encryption_required = true;
  EXPECT_CALL(*cache_manager_, GetAppEncryptionRequiredFlag(app_id_))
      .WillOnce(Return(encryption_required));

  EXPECT_TRUE(policy_manager_->AppNeedEncryption(app_id_));
}

TEST_F(PolicyManagerImplTest,
       AppNeedEncryption_EncryptionNotRequired_ReturnFalse) {
  EncryptionRequired encryption_required;
  *encryption_required = false;
  EXPECT_CALL(*cache_manager_, GetAppEncryptionRequiredFlag(app_id_))
      .WillOnce(Return(encryption_required));

  EXPECT_FALSE(policy_manager_->AppNeedEncryption(app_id_));
}

TEST_F(PolicyManagerImplTest,
       AppNeedEncryption_EncryptionNotInitialized_ReturnFalse) {
  EncryptionRequired encryption_required;
  EXPECT_CALL(*cache_manager_, GetAppEncryptionRequiredFlag(app_id_))
      .WillOnce(Return(encryption_required));

  EXPECT_TRUE(policy_manager_->AppNeedEncryption(app_id_));
}

TEST_F(PolicyManagerImplTest,
       GetAppEncryptionRequired_CacheReturnsTrue_ReturnTrue) {
  EncryptionRequired encryption_required;
  *encryption_required = true;
  EXPECT_CALL(*cache_manager_, GetAppEncryptionRequiredFlag(app_id_))
      .WillOnce(Return(encryption_required));

  EXPECT_TRUE(*policy_manager_->GetAppEncryptionRequired(app_id_));
}

TEST_F(PolicyManagerImplTest,
       GetAppEncryptionRequired_CacheReturnsFalse_ReturnFalse) {
  EncryptionRequired encryption_required;
  *encryption_required = false;
  EXPECT_CALL(*cache_manager_, GetAppEncryptionRequiredFlag(app_id_))
      .WillOnce(Return(encryption_required));

  EXPECT_FALSE(*policy_manager_->GetAppEncryptionRequired(app_id_));
}

TEST_F(PolicyManagerImplTest,
       GetFunctionalGroupsForApp_NoGroupsInParams_ReturnEmptyVector) {
  EXPECT_CALL(*cache_manager_, GetApplicationParams(app_id_, _));

  EXPECT_TRUE(policy_manager_->GetFunctionalGroupsForApp(app_id_).empty());
}

TEST_F(PolicyManagerImplTest,
       GetApplicationPolicyIDs_ValidPolicyIds_ReturnNotEmptyVector) {
  Strings app_ids;
  app_ids.push_back("1234");
  app_ids.push_back("default");

  EXPECT_CALL(*cache_manager_, GetPolicyAppIDs()).WillOnce(Return(app_ids));
  std::vector<std::string> returned_app_ids =
      policy_manager_->GetApplicationPolicyIDs();
  EXPECT_EQ(returned_app_ids.size(), 2u);
  EXPECT_EQ(returned_app_ids[0], "1234");
  EXPECT_EQ(returned_app_ids[1], "default");
}

TEST_F(PolicyManagerImplTest,
       FunctionGroupNeedEncryption_NoGroups_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupings(_));

  EXPECT_FALSE(policy_manager_->FunctionGroupNeedEncryption("Base-6"));
}

TEST_F(PolicyManagerImplTest,
       GetRPCsForFunctionGroup_NoGroups_ReturnEmptyVector) {
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupings(_));

  EXPECT_TRUE(policy_manager_->GetRPCsForFunctionGroup("Base-6").empty());
}

TEST_F(PolicyManagerImplTest,
       GetAppRequestTypesState_CacheReturnsState_ReturnSameState) {
  EXPECT_CALL(*cache_manager_, GetAppRequestTypesState(app_id_))
      .WillOnce(Return(RequestType::State::EMPTY));

  EXPECT_EQ(policy_manager_->GetAppRequestTypesState(app_id_),
            RequestType::State::EMPTY);
}

TEST_F(PolicyManagerImplTest,
       GetAppRequestSubTypesState_CacheReturnsState_ReturnSameState) {
  EXPECT_CALL(*cache_manager_, GetAppRequestSubTypesState(app_id_))
      .WillOnce(Return(RequestSubType::State::EMPTY));

  EXPECT_EQ(policy_manager_->GetAppRequestSubTypesState(app_id_),
            RequestSubType::State::EMPTY);
}

TEST_F(PolicyManagerImplTest,
       CheckPermissions_PermissionsIsCalculatedAndAppRevoked_ReturnRpcAllowed) {
  std::string hmi_level = "NONE";
  std::string rpc = "OnHMIStatus";
  RPCParams params;
  CheckPermissionResult result;

  EXPECT_CALL(*cache_manager_, IsPermissionsCalculated(device_id_, app_id_, _))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, IsApplicationRevoked(app_id_))
      .WillOnce(Return(true));

  policy_manager_->CheckPermissions(
      device_id_, app_id_, hmi_level, rpc, params, result);

  EXPECT_EQ(result.hmi_level_permitted, kRpcAllowed);
}

TEST_F(
    PolicyManagerImplTest,
    CheckPermissions_PermissionsNotCalculatedAndAppRevoked_ReturnRpcDisallowed) {
  std::string hmi_level = "FULL";
  std::string rpc = "OnHMIStatus";
  RPCParams params;
  CheckPermissionResult result;

  EXPECT_CALL(*cache_manager_, IsPermissionsCalculated(device_id_, app_id_, _))
      .WillOnce(Return(false));

  ON_CALL(*cache_manager_, IsApplicationRepresented(app_id_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(device_id_))
      .WillByDefault(Return(kDeviceAllowed));
  ON_CALL(*cache_manager_, IsDefaultPolicy(app_id_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetPermissionsForApp(device_id_, app_id_, _))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillByDefault(Return(true));

  EXPECT_CALL(*cache_manager_, IsApplicationRevoked(app_id_))
      .WillOnce(Return(true));

  policy_manager_->CheckPermissions(
      device_id_, app_id_, hmi_level, rpc, params, result);

  EXPECT_EQ(result.hmi_level_permitted, kRpcDisallowed);
}

TEST_F(PolicyManagerImplTest,
       CheckPermissions_RpcNotFound_ReturnRpcDisallowed) {
  std::string hmi_level = "NONE";
  std::string rpc = "OnHMIStatus";
  RPCParams params;
  CheckPermissionResult result;

  EXPECT_CALL(*cache_manager_, IsPermissionsCalculated(device_id_, app_id_, _))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, IsApplicationRevoked(app_id_))
      .WillOnce(Return(false));

  policy_manager_->CheckPermissions(
      device_id_, app_id_, hmi_level, rpc, params, result);

  EXPECT_EQ(result.hmi_level_permitted, kRpcDisallowed);
}

TEST_F(PolicyManagerImplTest,
       GetPermissionsForApp_CannotGetPermissionsForApp_ReturnEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, IsApplicationRepresented(app_id_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(device_id_))
      .WillByDefault(Return(kDeviceAllowed));
  ON_CALL(*cache_manager_, IsDefaultPolicy(app_id_))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_, GetPermissionsForApp(device_id_, kDefaultId, _))
      .WillOnce(Return(false));

  policy_manager_->GetPermissionsForApp(device_id_, app_id_, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest,
       GetPermissionsForApp_NoSuchApp_ReturnEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(app_id_))
      .WillOnce(Return(false));
  policy_manager_->GetPermissionsForApp(device_id_, app_id_, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest,
       GetPermissionsForApp_PredataPolicy_ReturnEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, IsApplicationRepresented(app_id_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(device_id_))
      .WillByDefault(Return(kDeviceDisallowed));
  ON_CALL(*cache_manager_, IsPredataPolicy(app_id_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_,
          GetPermissionsForApp(device_id_, kPreDataConsentId, _))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillOnce(Return(true));

  policy_manager_->GetPermissionsForApp(device_id_, app_id_, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest,
       GetPermissionsForApp_NotPredataOrDefaultPolicy_ReturnEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, IsApplicationRepresented(app_id_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(device_id_))
      .WillByDefault(Return(kDeviceDisallowed));
  ON_CALL(*cache_manager_, IsPredataPolicy(app_id_))
      .WillByDefault(Return(false));
  ON_CALL(*cache_manager_, IsDefaultPolicy(app_id_))
      .WillByDefault(Return(false));
  ON_CALL(*cache_manager_,
          GetPermissionsForApp(device_id_, kPreDataConsentId, _))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillOnce(Return(true));

  policy_manager_->GetPermissionsForApp(device_id_, app_id_, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest, LoadPT_InvalidPT_ReturnkWrongPtReceived) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;

  std::ifstream ifile(kSdlPreloadedPtJson);
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  ifile.close();
  policy_table::Table update(&root);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  ASSERT_FALSE(IsValid(update));

  std::string json = root.toStyledString();
  BinaryMessage msg(json.begin(), json.end());

  EXPECT_CALL(*cache_manager_, GetVehicleDataItems())
      .WillOnce(Return(vehicle_items));
  EXPECT_CALL(*cache_manager_, SaveUpdateRequired(false)).Times(0);
  EXPECT_EQ(policy_manager_->LoadPT("filename", msg),
            PolicyManager::PtProcessingResult::kWrongPtReceived);
}

TEST_F(PolicyManagerImplTest,
       LoadPT_SnapshotNotGenerated_ReturnkNewPtRequired) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;

  std::ifstream ifile(kSdlPreloadedPtJson);
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  root["policy_table"]["module_config"].removeMember("preloaded_pt");
  ifile.close();
  policy_table::Table update(&root);

  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  ASSERT_TRUE(IsValid(update));

  std::string json = root.toStyledString();
  BinaryMessage msg(json.begin(), json.end());

  EXPECT_CALL(*cache_manager_, GetVehicleDataItems())
      .WillOnce(Return(vehicle_items));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(nullptr));
  EXPECT_EQ(policy_manager_->LoadPT("filename", msg),
            PolicyManager::PtProcessingResult::kNewPtRequired);
}

TEST_F(PolicyManagerImplTest, LoadPT_UpdateNotApplied_ReturnkNewPtRequired) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;

  std::ifstream ifile(kSdlPreloadedPtJson);
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  root["policy_table"]["module_config"].removeMember("preloaded_pt");
  ifile.close();
  policy_table::Table update(&root);

  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  ASSERT_TRUE(IsValid(update));

  std::string json = root.toStyledString();
  BinaryMessage msg(json.begin(), json.end());

  EXPECT_CALL(*cache_manager_, GetVehicleDataItems())
      .WillOnce(Return(vehicle_items));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot())
      .WillOnce(Return(std::make_shared<policy_table::Table>(update)));
  EXPECT_CALL(*cache_manager_, ApplyUpdate(_)).WillOnce(Return(false));

  EXPECT_EQ(policy_manager_->LoadPT("filename", msg),
            PolicyManager::PtProcessingResult::kNewPtRequired);
}

TEST_F(PolicyManagerImplTest, LoadPT_NoHMIAppTypes_ReturnkSuccess) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;

  std::ifstream ifile(kSdlPreloadedPtJson);
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  root["policy_table"]["module_config"].removeMember("preloaded_pt");
  ifile.close();
  policy_table::Table update(&root);

  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  ASSERT_TRUE(IsValid(update));

  std::string json = root.toStyledString();
  BinaryMessage msg(json.begin(), json.end());
  ExternalConsentStatus status;
  GroupsByExternalConsentStatus groups_by_status;
  std::map<std::string, std::string> known_links;
  known_links["device_1"] = "app_1";

  ON_CALL(*cache_manager_, GetVehicleDataItems())
      .WillByDefault(Return(vehicle_items));
  ON_CALL(*cache_manager_, GenerateSnapshot())
      .WillByDefault(Return(std::make_shared<policy_table::Table>(update)));
  ON_CALL(*cache_manager_, ApplyUpdate(_)).WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetExternalConsentStatus())
      .WillByDefault(Return(status));
  ON_CALL(*cache_manager_, GetGroupsWithSameEntities(status))
      .WillByDefault(Return(groups_by_status));
  ON_CALL(*cache_manager_, GetKnownLinksFromPT())
      .WillByDefault(Return(known_links));

  EXPECT_EQ(policy_manager_->LoadPT("filename", msg),
            PolicyManager::PtProcessingResult::kSuccess);
}

TEST_F(PolicyManagerImplTest,
       HasCertificate_CacheReturnsEmptyString_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, GetCertificate()).WillOnce(Return(""));

  EXPECT_FALSE(policy_manager_->HasCertificate());
}

TEST_F(PolicyManagerImplTest,
       HasCertificate_CacheReturnsNotEmptyString_ReturnTrue) {
  EXPECT_CALL(*cache_manager_, GetCertificate())
      .WillOnce(Return("Certificate"));

  EXPECT_TRUE(policy_manager_->HasCertificate());
}

TEST_F(PolicyManagerImplTest, RetrieveCertificate_ReturnCertificate) {
  EXPECT_CALL(*cache_manager_, GetCertificate())
      .WillOnce(Return("Certificate"));

  EXPECT_EQ(policy_manager_->RetrieveCertificate(), "Certificate");
}

TEST_F(PolicyManagerImplTest, SetDecryptedCertificate_CertificateIsSetted) {
  EXPECT_CALL(*cache_manager_, SetDecryptedCertificate("Certificate"));

  policy_manager_->SetDecryptedCertificate("Certificate");
}

TEST_F(PolicyManagerImplTest, AddApplication_NewApplication_ReturnCallNothing) {
  AppHmiTypes hmi_types;
  ExternalConsentStatus ext_consent_status;
  GroupsByExternalConsentStatus groups;

  EXPECT_CALL(*cache_manager_, GetDeviceConsent(device_id_))
      .WillOnce(Return(kDeviceAllowed));
  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(app_id_))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, IsPredataPolicy(app_id_)).WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, GetExternalConsentStatus())
      .WillOnce(Return(ext_consent_status));
  EXPECT_CALL(*cache_manager_, GetGroupsWithSameEntities(ext_consent_status))
      .WillOnce(Return(groups));

  EXPECT_NE(policy_manager_->AddApplication(device_id_, app_id_, hmi_types),
            nullptr);
}

TEST_F(PolicyManagerImplTest, SetAppNicknames_NicknamesAreSetted) {
  StringArray nicknames;
  nicknames.push_back("some_app");
  EXPECT_CALL(*cache_manager_, SetAppNicknames(app_id_, nicknames));

  policy_manager_->SetAppNicknames(app_id_, nicknames);
}

TEST_F(PolicyManagerImplTest, RemoveAppConsentForGroup_ConsentIsRemoved) {
  EXPECT_CALL(*cache_manager_, RemoveAppConsentForGroup(app_id_, "Base-6"));

  policy_manager_->RemoveAppConsentForGroup(app_id_, "Base-6");
}

TEST_F(PolicyManagerImplTest, IsPredataPolicy_CacheReturnsTrue_ReturnTrue) {
  EXPECT_CALL(*cache_manager_, IsPredataPolicy(app_id_)).WillOnce(Return(true));

  EXPECT_TRUE(policy_manager_->IsPredataPolicy(app_id_));
}

TEST_F(PolicyManagerImplTest, IsPredataPolicy_CacheReturnsFalse_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, IsPredataPolicy(app_id_))
      .WillOnce(Return(false));

  EXPECT_FALSE(policy_manager_->IsPredataPolicy(app_id_));
}

TEST_F(PolicyManagerImplTest, GetAppServiceParameters_GetParameters) {
  AppServiceParameters app_service_parameters;
  EXPECT_CALL(*cache_manager_,
              GetAppServiceParameters(app_id_, &app_service_parameters));

  policy_manager_->GetAppServiceParameters(app_id_, &app_service_parameters);
}

TEST_F(PolicyManagerImplTest,
       OnPTUFinished_PtuResultIskNewPtRequired_InvokeForcePTExchange) {
  std::string initial_pt_status = policy_manager_->GetPolicyTableStatus();

  EXPECT_CALL(listener_, CanUpdate()).WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(nullptr));

  policy_manager_->OnPTUFinished(
      PolicyManager::PtProcessingResult::kNewPtRequired);
  std::string final_pt_status = policy_manager_->GetPolicyTableStatus();
  EXPECT_NE(initial_pt_status, final_pt_status);
}

TEST_F(
    PolicyManagerImplTest,
    OnPTUFinished_PtuResultIskWrongPtReceived_NoPTExchangeAndNoRefreshRetrySequence) {
  std::string initial_pt_status = policy_manager_->GetPolicyTableStatus();

  EXPECT_CALL(listener_, CanUpdate()).Times(0);
  EXPECT_CALL(*cache_manager_, TimeoutResponse()).Times(0);
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_)).Times(0);

  policy_manager_->OnPTUFinished(
      PolicyManager::PtProcessingResult::kWrongPtReceived);
  std::string final_pt_status = policy_manager_->GetPolicyTableStatus();
  EXPECT_EQ(initial_pt_status, final_pt_status);
}

TEST_F(PolicyManagerImplTest,
       OnPTUFinished_PtuResultIskSuccess_InvokeRefreshRetrySequence) {
  std::string initial_pt_status = policy_manager_->GetPolicyTableStatus();

  EXPECT_CALL(*cache_manager_, TimeoutResponse());
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_));

  policy_manager_->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
  std::string final_pt_status = policy_manager_->GetPolicyTableStatus();
  EXPECT_EQ(initial_pt_status, final_pt_status);
}

TEST_F(PolicyManagerImplTest, SetSystemLanguage_LanguageIsSetted) {
  EXPECT_CALL(*cache_manager_, SetSystemLanguage("eng-eng"));

  policy_manager_->SetSystemLanguage("eng-eng");
}

TEST_F(PolicyManagerImplTest, GetUpdateUrl_NoEndpoints_ReturnEmptyString) {
  EXPECT_CALL(*cache_manager_, GetUpdateUrls(kServiceTypeInt, _));

  EXPECT_TRUE((policy_manager_->GetUpdateUrl(kServiceTypeInt)).empty());
}

TEST_F(PolicyManagerImplTest, GetUpdateUrls_InvokeCacheManager) {
  EndpointUrls out_end_points;
  EXPECT_CALL(*cache_manager_, GetUpdateUrls(kServiceTypeString, _));

  policy_manager_->GetUpdateUrls(kServiceTypeString, out_end_points);
}

TEST_F(PolicyManagerImplTest, RequestPTUpdate_PTIsValid_PTIsUpdated) {
  std::ifstream ifile(kSdlPreloadedPtJson);
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  root["policy_table"]["consumer_friendly_messages"].removeMember("messages");
  ifile.close();
  policy_table::Table update(&root);
  auto snapshot = std::make_shared<policy_table::Table>();
  snapshot->policy_table = update.policy_table;
  if (update.policy_table.vehicle_data.is_initialized()) {
    snapshot->policy_table.vehicle_data =
        rpc::Optional<policy_table::VehicleData>();
    snapshot->policy_table.vehicle_data->mark_initialized();
    snapshot->policy_table.vehicle_data->schema_version =
        update.policy_table.vehicle_data->schema_version;
  }

  snapshot->SetPolicyTableType(policy_table::PT_SNAPSHOT);
  ASSERT_TRUE(IsValid(*snapshot));

  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(snapshot));
  EXPECT_CALL(listener_, OnSnapshotCreated(_, _, _));
  policy_manager_->RequestPTUpdate();
}

TEST_F(PolicyManagerImplTest, RequestPTUpdate_InvalidPT_PTNotUpdated) {
  std::ifstream ifile(kSdlPreloadedPtJson);
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  ifile.close();
  policy_table::Table update(&root);
  auto snapshot = std::make_shared<policy_table::Table>();
  snapshot->policy_table = update.policy_table;

  snapshot->SetPolicyTableType(policy_table::PT_SNAPSHOT);
  ASSERT_FALSE(IsValid(*snapshot));

  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(snapshot));
  EXPECT_CALL(listener_, OnSnapshotCreated(_, _, _)).Times(0);
  policy_manager_->RequestPTUpdate();
}

TEST_F(PolicyManagerImplTest, ResetUserConsent_CacheReturnsTrue_ReturnTrue) {
  EXPECT_CALL(*cache_manager_, ResetUserConsent()).WillOnce(Return(true));

  EXPECT_TRUE(policy_manager_->ResetUserConsent());
}

TEST_F(PolicyManagerImplTest, ResetUserConsent_CacheReturnsFalse_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, ResetUserConsent()).WillOnce(Return(false));

  EXPECT_FALSE(policy_manager_->ResetUserConsent());
}

TEST_F(PolicyManagerImplTest,
       SendNotificationOnPermissionsUpdated_EmptyDeviceID_NoNotifications) {
  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(app_id_)).Times(0);

  policy_manager_->SendNotificationOnPermissionsUpdated("", app_id_);
}

TEST_F(
    PolicyManagerImplTest,
    SendNotificationOnPermissionsUpdated_ValidAppAndDeviceIDs_PermissionsIsUpdated) {
  ON_CALL(*cache_manager_, IsApplicationRepresented(app_id_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(device_id_))
      .WillByDefault(Return(kDeviceAllowed));
  ON_CALL(*cache_manager_, IsDefaultPolicy(app_id_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetPermissionsForApp(device_id_, app_id_, _))
      .WillByDefault(Return(false));

  EXPECT_CALL(listener_, OnPermissionsUpdated(device_id_, app_id_, _, _));
  policy_manager_->SendNotificationOnPermissionsUpdated(device_id_, app_id_);
}

TEST_F(PolicyManagerImplTest,
       IsApplicationRevoked_CacheReturnsTrue_ReturnTrue) {
  EXPECT_CALL(*cache_manager_, IsApplicationRevoked(app_id_))
      .WillOnce(Return(true));

  EXPECT_TRUE(policy_manager_->IsApplicationRevoked(app_id_));
}

TEST_F(PolicyManagerImplTest,
       IsApplicationRevoked_CacheReturnsFalse_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, IsApplicationRevoked(app_id_))
      .WillOnce(Return(false));

  EXPECT_FALSE(policy_manager_->IsApplicationRevoked(app_id_));
}

TEST_F(PolicyManagerImplTest, SetVINValue_ValueIsSetted) {
  EXPECT_CALL(*cache_manager_, SetVINValue("value"));

  policy_manager_->SetVINValue("value");
}

TEST_F(PolicyManagerImplTest, KmsChanged_StartPTExchangeAndUpdatePT) {
  int kilometers = 1000;

  EXPECT_CALL(*cache_manager_, KilometersBeforeExchange(kilometers))
      .WillOnce(Return(0));
  EXPECT_CALL(*cache_manager_,
              SetCountersPassedForSuccessfulUpdate(KILOMETERS, kilometers));
  EXPECT_CALL(*cache_manager_, ResetIgnitionCycles());

  policy_manager_->KmsChanged(kilometers);
}

TEST_F(PolicyManagerImplTest, KmsChanged_NoPTExchangeAndNotUpdatePT) {
  int kilometers = 1000;

  EXPECT_CALL(*cache_manager_, KilometersBeforeExchange(kilometers))
      .WillOnce(Return(100));
  EXPECT_CALL(*cache_manager_,
              SetCountersPassedForSuccessfulUpdate(KILOMETERS, kilometers))
      .Times(0);
  EXPECT_CALL(*cache_manager_, ResetIgnitionCycles()).Times(0);

  policy_manager_->KmsChanged(kilometers);
}

TEST_F(PolicyManagerImplTest,
       SetExternalConsentStatus_EmptyStatus_ReturnFalse) {
  ExternalConsentStatus consent_status;

  EXPECT_FALSE(policy_manager_->SetExternalConsentStatus(consent_status));
}

TEST_F(PolicyManagerImplTest,
       SetExternalConsentStatus_CacheNotSetConsentStatus_ReturnFalse) {
  ExternalConsentStatus consent_status;
  consent_status.insert(ExternalConsentStatusItem());

  EXPECT_FALSE(policy_manager_->SetExternalConsentStatus(consent_status));
}

TEST_F(PolicyManagerImplTest,
       SetExternalConsentStatus_ConsentStatusIsSettedByCache_ReturnTrue) {
  ExternalConsentStatus consent_status;
  consent_status.insert(ExternalConsentStatusItem());
  GroupsByExternalConsentStatus groups_by_status;
  std::map<std::string, std::string> known_links;
  known_links["device_1"] = "app_1";

  ON_CALL(*cache_manager_, SetExternalConsentStatus(consent_status))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetGroupsWithSameEntities(consent_status))
      .WillByDefault(Return(groups_by_status));
  ON_CALL(*cache_manager_, GetKnownLinksFromPT())
      .WillByDefault(Return(known_links));

  EXPECT_TRUE(policy_manager_->SetExternalConsentStatus(consent_status));
}

TEST_F(PolicyManagerImplTest, CanAppKeepContext_CacheReturnsTrue_ReturnTrue) {
  EXPECT_CALL(*cache_manager_, CanAppKeepContext(app_id_))
      .WillOnce(Return(true));

  EXPECT_TRUE(policy_manager_->CanAppKeepContext(app_id_));
}

TEST_F(PolicyManagerImplTest, CanAppKeepContext_CacheReturnsFalse_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, CanAppKeepContext(app_id_))
      .WillOnce(Return(false));

  EXPECT_FALSE(policy_manager_->CanAppKeepContext(app_id_));
}

TEST_F(PolicyManagerImplTest, CanAppStealFocus_CacheReturnsTrue_ReturnTrue) {
  EXPECT_CALL(*cache_manager_, CanAppStealFocus(app_id_))
      .WillOnce(Return(true));

  EXPECT_TRUE(policy_manager_->CanAppStealFocus(app_id_));
}

TEST_F(PolicyManagerImplTest, CanAppStealFocus_CacheReturnsFalse_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, CanAppStealFocus(app_id_))
      .WillOnce(Return(false));

  EXPECT_FALSE(policy_manager_->CanAppStealFocus(app_id_));
}

TEST_F(PolicyManagerImplTest,
       MarkUnpairedDevice_UnpairedDeviceIsNotSetted_NoUserConsentForDevice) {
  EXPECT_CALL(*cache_manager_, SetUnpairedDevice(device_id_, _))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager_, SetDeviceConsent(device_id_, false)).Times(0);

  policy_manager_->MarkUnpairedDevice(device_id_);
}

TEST_F(PolicyManagerImplTest,
       MarkUnpairedDevice_UnpairedDeviceIsSetted_SetUserConsentForDevice) {
  EXPECT_CALL(*cache_manager_, SetUnpairedDevice(device_id_, _))
      .WillOnce(Return(true));
  EXPECT_CALL(listener_, CanUpdate()).WillRepeatedly(Return(true));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(nullptr));

  policy_manager_->MarkUnpairedDevice(device_id_);
}

TEST_F(PolicyManagerImplTest, SetDeviceInfo_DataIsSetted) {
  DeviceInfo device_info;
  device_info.hardware = "";
  device_info.firmware_rev = "";
  device_info.os = "";
  device_info.os_ver = "";
  device_info.carrier = "";
  device_info.max_number_rfcom_ports = 0u;
  device_info.connection_type = "";

  EXPECT_CALL(*cache_manager_,
              SetDeviceData(device_id_,
                            device_info.hardware,
                            device_info.firmware_rev,
                            device_info.os,
                            device_info.os_ver,
                            device_info.carrier,
                            device_info.max_number_rfcom_ports,
                            device_info.connection_type))
      .WillOnce(Return(true));

  policy_manager_->SetDeviceInfo(device_id_, device_info);
}

TEST_F(PolicyManagerImplTest, OnLocalAppAdded_StartPTExchange) {
  EXPECT_CALL(listener_, CanUpdate()).WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(nullptr));

  policy_manager_->OnLocalAppAdded();
}

TEST_F(PolicyManagerImplTest, SetDefaultHmiTypes_GotCurrentDeviceID) {
  transport_manager::DeviceHandle handle;
  std::vector<int> hmi_types;
  hmi_types.push_back(0);

  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(handle, app_id_))
      .WillOnce(Return(device_id_));
  policy_manager_->SetDefaultHmiTypes(handle, app_id_, hmi_types);
}

TEST_F(PolicyManagerImplTest,
       SetUserConsentForApp_UserPermissionsNotSetted_NotificationIsNotSent) {
  PermissionConsent perm_consent;
  perm_consent.device_id = device_id_;
  perm_consent.policy_app_id = app_id_;
  perm_consent.consent_source = "VR";

  FunctionalGroupPermission group_perm;
  group_perm.group_alias = "Notifications";
  group_perm.group_name = "Notifications";
  group_perm.group_id = ::utils::Djb2HashFromString("Notifications");
  group_perm.state = GroupConsent::kGroupAllowed;

  std::vector<FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group_perm);
  perm_consent.group_permissions = groups_permissions;

  ON_CALL(*cache_manager_, GetPermissionsForApp(device_id_, app_id_, _))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_, SetUserPermissionsForApp(_, _))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(app_id_)).Times(0);

  policy_manager_->SetUserConsentForApp(perm_consent,
                                        PolicyManager::kNotifyApplicationMode);
}

TEST_F(PolicyManagerImplTest,
       SetUserConsentForApp_SilentMode_NotificationIsNotSent) {
  PermissionConsent perm_consent;
  perm_consent.device_id = device_id_;
  perm_consent.policy_app_id = app_id_;
  perm_consent.consent_source = "VR";

  FunctionalGroupPermission group_perm;
  group_perm.group_alias = "Notifications";
  group_perm.group_name = "Notifications";
  group_perm.group_id = ::utils::Djb2HashFromString("Notifications");
  group_perm.state = GroupConsent::kGroupAllowed;

  std::vector<FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group_perm);
  perm_consent.group_permissions = groups_permissions;

  ON_CALL(*cache_manager_, GetPermissionsForApp(device_id_, app_id_, _))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_, SetUserPermissionsForApp(_, _))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(app_id_)).Times(0);

  policy_manager_->SetUserConsentForApp(perm_consent,
                                        PolicyManager::kSilentMode);
}

}  // namespace policy_test
}  // namespace components
}  // namespace test

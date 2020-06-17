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
const std::string kDeviceNumber = "XXX123456789ZZZ";
const std::string kAppStorageFolder = "app_storage_folder";
const std::string kValidAppId = "1234";
}  // namespace

class PolicyManagerImplTest : public ::testing::Test {
 public:
  PolicyManagerImplTest()
      : policy_manager_(nullptr), cache_manager_(nullptr), in_memory_(true) {}

 protected:
  std::shared_ptr<PolicyManagerImpl> policy_manager_;
  NiceMock<MockCacheManagerInterface>* cache_manager_;
  NiceMock<MockPolicyListener> listener_;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;
  bool in_memory_;

  void SetUp() {
    policy_manager_ = std::make_shared<PolicyManagerImpl>(in_memory_);
    cache_manager_ = new NiceMock<MockCacheManagerInterface>();
    policy_manager_->set_cache_manager(cache_manager_);
    policy_manager_->set_listener(&listener_);

    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
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

TEST_F(PolicyManagerImplTest, InitPT_NoAppStorageFolder_ReturnFalse) {
  EXPECT_FALSE(policy_manager_->InitPT(kSdlPreloadedPtJson, &policy_settings_));
}

TEST_F(PolicyManagerImplTest, InitPT_InitializationNotSuccessful_ReturnFalse) {
  file_system::CreateDirectory(kAppStorageFolder);
  EXPECT_CALL(*cache_manager_, Init(kSdlPreloadedPtJson, &policy_settings_))
      .WillOnce(Return(false));
  EXPECT_FALSE(policy_manager_->InitPT(kSdlPreloadedPtJson, &policy_settings_));
  file_system::RemoveDirectory(kAppStorageFolder, true);
}

TEST_F(PolicyManagerImplTest, InitPT_InitializationIsSuccessful_ReturnTrue) {
  file_system::CreateDirectory(kAppStorageFolder);
  EXPECT_CALL(*cache_manager_, Init(kSdlPreloadedPtJson, &policy_settings_))
      .WillOnce(Return(true));
  EXPECT_TRUE(policy_manager_->InitPT(kSdlPreloadedPtJson, &policy_settings_));
  file_system::RemoveDirectory(kAppStorageFolder, true);
}

TEST_F(PolicyManagerImplTest, ResetPT_NoRefreshRetrySequence_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, ResetCalculatedPermissions());
  EXPECT_CALL(*cache_manager_, ResetPT(kSdlPreloadedPtJson))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager_, TimeoutResponse()).Times(0);
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_)).Times(0);

  EXPECT_FALSE(policy_manager_->ResetPT(kSdlPreloadedPtJson));
}

TEST_F(PolicyManagerImplTest, ResetPT_ExecuteRefreshRetrySequence_ReturnTrue) {
  EXPECT_CALL(*cache_manager_, ResetCalculatedPermissions());
  EXPECT_CALL(*cache_manager_, ResetPT(kSdlPreloadedPtJson))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, TimeoutResponse());
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_));

  EXPECT_TRUE(policy_manager_->ResetPT(kSdlPreloadedPtJson));
}

TEST_F(PolicyManagerImplTest,
       AppNeedEncryption_EncryptionNotRequired_ReturnFalse) {
  EncryptionRequired encryption_required;
  *encryption_required = false;
  EXPECT_CALL(*cache_manager_, GetAppEncryptionRequiredFlag(kValidAppId))
      .WillOnce(Return(encryption_required));

  EXPECT_FALSE(policy_manager_->AppNeedEncryption(kValidAppId));
}

TEST_F(PolicyManagerImplTest,
       AppNeedEncryption_EncryptionNotInitialized_ReturnTrue) {
  EncryptionRequired encryption_required;
  EXPECT_CALL(*cache_manager_, GetAppEncryptionRequiredFlag(kValidAppId))
      .WillOnce(Return(encryption_required));

  EXPECT_TRUE(policy_manager_->AppNeedEncryption(kValidAppId));
}

TEST_F(PolicyManagerImplTest,
       FunctionGroupNeedEncryption_NoGroups_ReturnFalse) {
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupings(_));
  const std::string group("Base-6");

  EXPECT_FALSE(policy_manager_->FunctionGroupNeedEncryption(group));
}

TEST_F(PolicyManagerImplTest,
       CheckPermissions_PermissionsIsCalculatedAndAppRevoked_ReturnRpcAllowed) {
  const std::string hmi_level = "NONE";
  const std::string rpc = "OnHMIStatus";
  RPCParams params;
  CheckPermissionResult result;

  EXPECT_CALL(*cache_manager_,
              IsPermissionsCalculated(kDeviceNumber, kValidAppId, _))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, IsApplicationRevoked(kValidAppId))
      .WillOnce(Return(true));

  policy_manager_->CheckPermissions(
      kDeviceNumber, kValidAppId, hmi_level, rpc, params, result);

  EXPECT_EQ(kRpcAllowed, result.hmi_level_permitted);
}

TEST_F(
    PolicyManagerImplTest,
    CheckPermissions_PermissionsNotCalculatedAndAppRevoked_ReturnRpcDisallowed) {
  std::string hmi_level = "FULL";
  std::string rpc = "OnHMIStatus";
  RPCParams params;
  CheckPermissionResult result;

  EXPECT_CALL(*cache_manager_,
              IsPermissionsCalculated(kDeviceNumber, kValidAppId, _))
      .WillOnce(Return(false));

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(kDeviceNumber))
      .WillByDefault(Return(kDeviceAllowed));
  ON_CALL(*cache_manager_, IsDefaultPolicy(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetPermissionsForApp(kDeviceNumber, kValidAppId, _))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillByDefault(Return(true));

  EXPECT_CALL(*cache_manager_, IsApplicationRevoked(kValidAppId))
      .WillOnce(Return(true));

  policy_manager_->CheckPermissions(
      kDeviceNumber, kValidAppId, hmi_level, rpc, params, result);

  EXPECT_EQ(kRpcDisallowed, result.hmi_level_permitted);
}

TEST_F(PolicyManagerImplTest,
       CheckPermissions_RpcNotFound_ReturnRpcDisallowed) {
  const std::string hmi_level = "NONE";
  const std::string rpc = "OnHMIStatus";
  RPCParams params;
  CheckPermissionResult result;

  EXPECT_CALL(*cache_manager_,
              IsPermissionsCalculated(kDeviceNumber, kValidAppId, _))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, IsApplicationRevoked(kValidAppId))
      .WillOnce(Return(false));

  policy_manager_->CheckPermissions(
      kDeviceNumber, kValidAppId, hmi_level, rpc, params, result);

  EXPECT_EQ(kRpcDisallowed, result.hmi_level_permitted);
}

TEST_F(PolicyManagerImplTest,
       GetPermissionsForApp_CannotGetPermissionsForApp_ReturnEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(kDeviceNumber))
      .WillByDefault(Return(kDeviceAllowed));
  ON_CALL(*cache_manager_, IsDefaultPolicy(kValidAppId))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_,
              GetPermissionsForApp(kDeviceNumber, kDefaultId, _))
      .WillOnce(Return(false));

  policy_manager_->GetPermissionsForApp(
      kDeviceNumber, kValidAppId, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest,
       GetPermissionsForApp_NoSuchApp_ReturnEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillOnce(Return(false));
  policy_manager_->GetPermissionsForApp(
      kDeviceNumber, kValidAppId, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest,
       GetPermissionsForApp_PredataPolicy_ReturnEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(kDeviceNumber))
      .WillByDefault(Return(kDeviceDisallowed));
  ON_CALL(*cache_manager_, IsPredataPolicy(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_,
          GetPermissionsForApp(kDeviceNumber, kPreDataConsentId, _))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillOnce(Return(true));

  policy_manager_->GetPermissionsForApp(
      kDeviceNumber, kValidAppId, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest,
       GetPermissionsForApp_NotPredataOrDefaultPolicy_ReturnEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(kDeviceNumber))
      .WillByDefault(Return(kDeviceDisallowed));
  ON_CALL(*cache_manager_, IsPredataPolicy(kValidAppId))
      .WillByDefault(Return(false));
  ON_CALL(*cache_manager_, IsDefaultPolicy(kValidAppId))
      .WillByDefault(Return(false));
  ON_CALL(*cache_manager_,
          GetPermissionsForApp(kDeviceNumber, kPreDataConsentId, _))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillOnce(Return(true));

  policy_manager_->GetPermissionsForApp(
      kDeviceNumber, kValidAppId, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest,
       GetPermissionsForApp_NoFunctionalGroupsNames_ReturnEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(kDeviceNumber))
      .WillByDefault(Return(kDeviceDisallowed));
  ON_CALL(*cache_manager_, IsPredataPolicy(kValidAppId))
      .WillByDefault(Return(false));
  ON_CALL(*cache_manager_, IsDefaultPolicy(kValidAppId))
      .WillByDefault(Return(false));
  ON_CALL(*cache_manager_,
          GetPermissionsForApp(kDeviceNumber, kPreDataConsentId, _))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillOnce(Return(false));

  policy_manager_->GetPermissionsForApp(
      kDeviceNumber, kValidAppId, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest, LoadPT_InvalidPT_ReturnkWrongPtReceived) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;
  std::ifstream ifile(kSdlPreloadedPtJson);
  ASSERT_TRUE(ifile.good());

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
  const std::string filename("preloaded_pt.json");
  EXPECT_EQ(policy_manager_->LoadPT(filename, msg),
            PolicyManager::PtProcessingResult::kWrongPtReceived);
}

TEST_F(PolicyManagerImplTest,
       LoadPT_SnapshotNotGenerated_ReturnkNewPtRequired) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;
  std::ifstream ifile(kSdlPreloadedPtJson);

  ASSERT_TRUE(ifile.good());
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
  const std::string filename("preloaded_pt.json");
  EXPECT_EQ(policy_manager_->LoadPT(filename, msg),
            PolicyManager::PtProcessingResult::kNewPtRequired);
}

TEST_F(PolicyManagerImplTest, LoadPT_UpdateNotApplied_ReturnkNewPtRequired) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;
  std::ifstream ifile(kSdlPreloadedPtJson);
  ASSERT_TRUE(ifile.good());

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
  const std::string filename("preloaded_pt.json");
  EXPECT_EQ(policy_manager_->LoadPT(filename, msg),
            PolicyManager::PtProcessingResult::kNewPtRequired);
}

TEST_F(PolicyManagerImplTest, LoadPT_NoHMIAppTypes_ReturnkSuccess) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;
  std::ifstream ifile(kSdlPreloadedPtJson);
  ASSERT_TRUE(ifile.good());

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
  const std::string linked_device("device_1");
  const std::string linked_app("app_1");
  known_links[linked_device] = linked_app;

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

  const std::string filename("preloaded_pt.json");
  EXPECT_EQ(policy_manager_->LoadPT(filename, msg),
            PolicyManager::PtProcessingResult::kSuccess);
}

TEST_F(PolicyManagerImplTest,
       AddApplication_NewApplication_ReturnCallStatusChanges) {
  AppHmiTypes hmi_types;
  ExternalConsentStatus ext_consent_status;
  GroupsByExternalConsentStatus groups;

  ON_CALL(*cache_manager_, GetDeviceConsent(kDeviceNumber))
      .WillByDefault(Return(kDeviceAllowed));
  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(false));
  ON_CALL(*cache_manager_, GetExternalConsentStatus())
      .WillByDefault(Return(ext_consent_status));
  ON_CALL(*cache_manager_, GetGroupsWithSameEntities(ext_consent_status))
      .WillByDefault(Return(groups));

  EXPECT_CALL(*cache_manager_, IsPredataPolicy(kValidAppId)).Times(0);

  policy_manager_->AddApplication(kDeviceNumber, kValidAppId, hmi_types);
}

TEST_F(PolicyManagerImplTest,
       AddApplication_ExistedApplication_ReturnCallNothing) {
  AppHmiTypes hmi_types;
  ExternalConsentStatus ext_consent_status;
  GroupsByExternalConsentStatus groups;

  ON_CALL(*cache_manager_, GetDeviceConsent(kDeviceNumber))
      .WillByDefault(Return(kDeviceAllowed));
  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetExternalConsentStatus())
      .WillByDefault(Return(ext_consent_status));
  ON_CALL(*cache_manager_, GetGroupsWithSameEntities(ext_consent_status))
      .WillByDefault(Return(groups));

  EXPECT_CALL(*cache_manager_, IsPredataPolicy(kValidAppId))
      .WillOnce(Return(true));

  policy_manager_->AddApplication(kDeviceNumber, kValidAppId, hmi_types);
}

TEST_F(PolicyManagerImplTest,
       OnPTUFinished_PtuResultIskNewPtRequired_InvokeForcePTExchange) {
  std::string initial_pt_status = policy_manager_->GetPolicyTableStatus();

  // EXPECT_CALL(listener_, CanUpdate()).WillOnce(Return(true));
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

TEST_F(PolicyManagerImplTest, GetUpdateUrl_NoEndpoints_ReturnEmptyString) {
  EXPECT_CALL(*cache_manager_, GetUpdateUrls(kServiceTypeInt, _));

  EXPECT_TRUE((policy_manager_->GetUpdateUrl(kServiceTypeInt)).empty());
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

TEST_F(PolicyManagerImplTest,
       SendNotificationOnPermissionsUpdated_EmptyDeviceID_NoNotifications) {
  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId)).Times(0);

  const std::string device_id;
  policy_manager_->SendNotificationOnPermissionsUpdated(device_id, kValidAppId);
}

TEST_F(
    PolicyManagerImplTest,
    SendNotificationOnPermissionsUpdated_ValidAppAndDeviceIDs_PermissionsIsUpdated) {
  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetDeviceConsent(kDeviceNumber))
      .WillByDefault(Return(kDeviceAllowed));
  ON_CALL(*cache_manager_, IsDefaultPolicy(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetPermissionsForApp(kDeviceNumber, kValidAppId, _))
      .WillByDefault(Return(false));

  EXPECT_CALL(listener_,
              OnPermissionsUpdated(kDeviceNumber, kValidAppId, _, _));
  policy_manager_->SendNotificationOnPermissionsUpdated(kDeviceNumber,
                                                        kValidAppId);
}

TEST_F(PolicyManagerImplTest, KmsChanged_StartPTExchangeAndUpdatePT) {
  int kilometers = 1000;

  EXPECT_CALL(*cache_manager_, KilometersBeforeExchange(kilometers))
      .WillOnce(Return(0));

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
  const std::string linked_device("device_1");
  const std::string linked_app("app_1");
  known_links[linked_device] = linked_app;

  ON_CALL(*cache_manager_, SetExternalConsentStatus(consent_status))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetGroupsWithSameEntities(consent_status))
      .WillByDefault(Return(groups_by_status));
  ON_CALL(*cache_manager_, GetKnownLinksFromPT())
      .WillByDefault(Return(known_links));

  EXPECT_TRUE(policy_manager_->SetExternalConsentStatus(consent_status));
}

TEST_F(PolicyManagerImplTest,
       MarkUnpairedDevice_UnpairedDeviceIsNotSetted_NoUserConsentForDevice) {
  EXPECT_CALL(*cache_manager_, SetUnpairedDevice(kDeviceNumber, _))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager_, SetDeviceConsent(kDeviceNumber, false)).Times(0);

  policy_manager_->MarkUnpairedDevice(kDeviceNumber);
}

TEST_F(PolicyManagerImplTest,
       MarkUnpairedDevice_UnpairedDeviceIsSetted_SetUserConsentForDevice) {
  EXPECT_CALL(*cache_manager_, SetUnpairedDevice(kDeviceNumber, _))
      .WillOnce(Return(true));
  EXPECT_CALL(listener_, CanUpdate()).WillRepeatedly(Return(true));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(nullptr));

  policy_manager_->MarkUnpairedDevice(kDeviceNumber);
}

TEST_F(PolicyManagerImplTest,
       SetUserConsentForApp_UserPermissionsNotSetted_NotificationIsNotSent) {
  PermissionConsent perm_consent;
  perm_consent.device_id = kDeviceNumber;
  perm_consent.policy_app_id = kValidAppId;
  const std::string source("VR");
  perm_consent.consent_source = source;

  const std::string group_name("Notifications");
  FunctionalGroupPermission group_perm;
  group_perm.group_alias = group_name;
  group_perm.group_name = group_name;
  group_perm.group_id = ::utils::Djb2HashFromString(group_name);
  group_perm.state = GroupConsent::kGroupAllowed;

  std::vector<FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group_perm);
  perm_consent.group_permissions = groups_permissions;

  ON_CALL(*cache_manager_, GetPermissionsForApp(kDeviceNumber, kValidAppId, _))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, SetUserPermissionsForApp(_, _))
      .WillByDefault(Return(false));

  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId)).Times(0);

  policy_manager_->SetUserConsentForApp(perm_consent,
                                        PolicyManager::kNotifyApplicationMode);
}

TEST_F(PolicyManagerImplTest,
       SetUserConsentForApp_SilentMode_NotificationIsNotSent) {
  PermissionConsent perm_consent;
  perm_consent.device_id = kDeviceNumber;
  perm_consent.policy_app_id = kValidAppId;
  const std::string source("VR");
  perm_consent.consent_source = source;

  const std::string group_name("Notifications");
  FunctionalGroupPermission group_perm;
  group_perm.group_alias = group_name;
  group_perm.group_name = group_name;
  group_perm.group_id = ::utils::Djb2HashFromString(group_name);
  group_perm.state = GroupConsent::kGroupAllowed;

  std::vector<FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group_perm);
  perm_consent.group_permissions = groups_permissions;

  ON_CALL(*cache_manager_, GetPermissionsForApp(kDeviceNumber, kValidAppId, _))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, SetUserPermissionsForApp(_, _))
      .WillByDefault(Return(true));

  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId)).Times(0);

  policy_manager_->SetUserConsentForApp(perm_consent,
                                        PolicyManager::kSilentMode);
}

TEST_F(PolicyManagerImplTest,
       SetUserConsentForApp_AppPermissionsNotChanged_NotificationIsNotSent) {
  PermissionConsent perm_consent;
  perm_consent.device_id = kDeviceNumber;
  perm_consent.policy_app_id = kValidAppId;
  const std::string source("VR");
  perm_consent.consent_source = source;

  const std::string group_name("Notifications");
  FunctionalGroupPermission group_perm;
  group_perm.group_alias = group_name;
  group_perm.group_name = group_name;
  group_perm.group_id = ::utils::Djb2HashFromString(group_name);
  group_perm.state = GroupConsent::kGroupAllowed;

  std::vector<FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group_perm);
  perm_consent.group_permissions = groups_permissions;

  ON_CALL(*cache_manager_, GetPermissionsForApp(kDeviceNumber, kValidAppId, _))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, SetUserPermissionsForApp(_, _))
      .WillByDefault(Return(true));

  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId)).Times(0);

  policy_manager_->SetUserConsentForApp(perm_consent,
                                        PolicyManager::kNotifyApplicationMode);
}

TEST_F(PolicyManagerImplTest,
       GetUserConsentForApp_NoPermissionsForApp_NoConsent) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, GetPermissionsForApp(kDeviceNumber, kValidAppId, _))
      .WillByDefault(Return(false));
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupNames(_)).Times(0);

  policy_manager_->GetUserConsentForApp(
      kDeviceNumber, kValidAppId, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(PolicyManagerImplTest,
       GetUserConsentForApp_NoFunctionalGroupsNames_NoConsent) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, GetPermissionsForApp(kDeviceNumber, kValidAppId, _))
      .WillByDefault(Return(true));
  EXPECT_CALL(*cache_manager_, GetFunctionalGroupNames(_))
      .WillOnce(Return(false));

  policy_manager_->GetUserConsentForApp(
      kDeviceNumber, kValidAppId, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(
    PolicyManagerImplTest,
    ReactOnUserDevConsentForApp_AppPoliciesIsChanged_AppPermissionsNotChanged) {
  transport_manager::DeviceHandle device_handle;
  bool is_device_allowed = true;

  ON_CALL(listener_,
          OnCurrentDeviceIdUpdateRequired(device_handle, kValidAppId))
      .WillByDefault(Return(kDeviceNumber));
  ON_CALL(*cache_manager_, GetDeviceConsent(kDeviceNumber))
      .WillByDefault(Return(kDeviceAllowed));
  ON_CALL(*cache_manager_,
          ReactOnUserDevConsentForApp(kValidAppId, is_device_allowed))
      .WillByDefault(Return(true));

  EXPECT_CALL(listener_,
              SendOnAppPermissionsChanged(_, kDeviceNumber, kValidAppId))
      .Times(0);
  EXPECT_TRUE(policy_manager_->ReactOnUserDevConsentForApp(
      device_handle, kValidAppId, is_device_allowed));
}

TEST_F(PolicyManagerImplTest,
       GetInitialAppData_HandleNullPointers_ReturnFalse) {
  EXPECT_FALSE(
      policy_manager_->GetInitialAppData(kValidAppId, nullptr, nullptr));
}

TEST_F(PolicyManagerImplTest,
       GetInitialAppData_HandleValidPointers_ReturnTrue) {
  StringArray nicknames;
  StringArray app_hmi_types;
  ON_CALL(*cache_manager_,
          GetInitialAppData(kValidAppId, nicknames, app_hmi_types))
      .WillByDefault(Return(true));

  EXPECT_TRUE(policy_manager_->GetInitialAppData(
      kValidAppId, &nicknames, &app_hmi_types));
}

TEST_F(PolicyManagerImplTest,
       GetAppPermissionsChanges_NoPermissionsChanges_GeneratePermissions) {
  EXPECT_CALL(*cache_manager_,
              CountUnconsentedGroups(kValidAppId, kDeviceNumber));
  EXPECT_CALL(*cache_manager_, IsApplicationRevoked(kValidAppId));
  EXPECT_CALL(*cache_manager_, GetPriority(kValidAppId, _));

  policy_manager_->GetAppPermissionsChanges(kDeviceNumber, kValidAppId);
}

TEST_F(PolicyManagerImplTest, GetHMITypes_AppIsDefaultPolicy_ReturnFalse) {
  std::vector<int> app_types;
  EXPECT_CALL(*cache_manager_, IsDefaultPolicy(kValidAppId))
      .WillOnce(Return(true));

  EXPECT_FALSE(policy_manager_->GetHMITypes(kValidAppId, &app_types));
}

TEST_F(PolicyManagerImplTest, GetHMITypes_NoHmiTypes_ReturnFalse) {
  std::vector<int> app_types;
  EXPECT_CALL(*cache_manager_, GetHMITypes(kValidAppId))
      .WillOnce(Return(nullptr));
  EXPECT_FALSE(policy_manager_->GetHMITypes(kValidAppId, &app_types));
}

TEST_F(PolicyManagerImplTest, GetHMITypes_ValidHmiTypes_ReturnTrue) {
  std::vector<int> app_types;
  AppHMITypes hmi_types;
  EXPECT_CALL(*cache_manager_, GetHMITypes(kValidAppId))
      .WillOnce(Return(&hmi_types));
  EXPECT_TRUE(policy_manager_->GetHMITypes(kValidAppId, &app_types));
}

}  // namespace policy_test
}  // namespace components
}  // namespace test

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

#include "policy/policy_manager_impl.h"

#include "policy/mock_access_remote.h"
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

typedef std::shared_ptr<policy_table::Table> PolicyTableSPtr;

namespace {
const std::string kSdlPreloadedPtJson = "json/sdl_preloaded_pt.json";
const int kServiceTypeInt = 0;
const std::string kDeviceNumber = "XXX123456789ZZZ";
const std::string kAppStorageFolder = "app_storage_folder";
const std::string kValidAppId = "1234";
const std::string kInValidFilename = "invalid_filename.json";
}  // namespace

class PolicyManagerImplTest : public ::testing::Test {
 public:
  PolicyManagerImplTest()
      : policy_manager_(nullptr)
      , cache_manager_(nullptr)
      , access_remote_(nullptr) {}

 protected:
  std::shared_ptr<PolicyManagerImpl> policy_manager_;
  NiceMock<MockCacheManagerInterface>* cache_manager_;
  NiceMock<MockPolicyListener> listener_;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;
  std::shared_ptr<NiceMock<access_remote_test::MockAccessRemote> >
      access_remote_;

  void SetUp() {
    policy_manager_ = std::make_shared<PolicyManagerImpl>();
    cache_manager_ = new NiceMock<MockCacheManagerInterface>();
    policy_manager_->set_cache_manager(cache_manager_);
    policy_manager_->set_listener(&listener_);
    access_remote_ =
        std::make_shared<NiceMock<access_remote_test::MockAccessRemote> >();
    policy_manager_->set_access_remote(access_remote_);

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
  const std::string absent_group("Base-6");

  EXPECT_FALSE(policy_manager_->FunctionGroupNeedEncryption(absent_group));
}

TEST_F(PolicyManagerImplTest,
       CheckPermissions_AppNotRepresented_ReturnRpcAllowed) {
  const std::string hmi_level = "NONE";
  const std::string rpc = "OnHMIStatus";
  const RPCParams params;
  CheckPermissionResult result;

  EXPECT_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillOnce(Return(false));

  policy_manager_->CheckPermissions(
      kDeviceNumber, kValidAppId, hmi_level, rpc, params, result);
  EXPECT_EQ(kRpcDisallowed, result.hmi_level_permitted);
}

TEST_F(PolicyManagerImplTest,
       CheckPermissions_AppIsRemoteControlAndNotRevoked_ReturnkRpcDisallowed) {
  const std::string hmi_level = "NONE";
  const std::string rpc = "OnHMIStatus";
  const RPCParams params;
  CheckPermissionResult result;
  Strings groups;

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*access_remote_, IsAppRemoteControl(_)).WillByDefault(Return(true));
  ON_CALL(*access_remote_, GetGroups(_)).WillByDefault(ReturnRef(groups));

  policy_manager_->CheckPermissions(
      kDeviceNumber, kValidAppId, hmi_level, rpc, params, result);
  EXPECT_EQ(kRpcDisallowed, result.hmi_level_permitted);
}

TEST_F(PolicyManagerImplTest,
       CheckPermissions_NotRemoteControlAppIsRevoked_ReturnkRpcAllowed) {
  const std::string hmi_level = "NONE";
  const std::string rpc = "OnHMIStatus";
  const RPCParams params;
  CheckPermissionResult result;
  Strings groups;

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetGroups(_)).WillByDefault(ReturnRef(groups));
  ON_CALL(*cache_manager_, IsApplicationRevoked(kValidAppId))
      .WillByDefault(Return(true));

  policy_manager_->CheckPermissions(
      kDeviceNumber, kValidAppId, hmi_level, rpc, params, result);

  EXPECT_EQ(kRpcAllowed, result.hmi_level_permitted);
}

TEST_F(PolicyManagerImplTest,
       CheckPermissions_NotRemoteControlAppIsRevoked_ReturnkRpcDisallowed) {
  const std::string hmi_level = "FULL";
  const std::string rpc = "OnHMIStatus";
  const RPCParams params;
  CheckPermissionResult result;
  Strings groups;

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*cache_manager_, GetGroups(_)).WillByDefault(ReturnRef(groups));
  ON_CALL(*cache_manager_, IsApplicationRevoked(kValidAppId))
      .WillByDefault(Return(true));

  policy_manager_->CheckPermissions(
      kDeviceNumber, kValidAppId, hmi_level, rpc, params, result);

  EXPECT_EQ(kRpcDisallowed, result.hmi_level_permitted);
}

TEST_F(
    PolicyManagerImplTest,
    GetPermissionsForApp_CannotGetPermissionsForRemoteDefaultApp_GetEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, IsDefaultPolicy(kValidAppId))
      .WillByDefault(Return(true));
  ON_CALL(*access_remote_, IsAppRemoteControl(_)).WillByDefault(Return(true));
  EXPECT_CALL(*access_remote_,
              GetPermissionsForApp(kDeviceNumber, kValidAppId, _))
      .WillOnce(Return(false));

  policy_manager_->GetPermissionsForApp(
      kDeviceNumber, kValidAppId, permissions);
  EXPECT_TRUE(permissions.empty());
}

TEST_F(
    PolicyManagerImplTest,
    GetPermissionsForApp_CannotGetFunctionalGroupsNamesForNotRemotePredataApp_GetEmptyVector) {
  std::vector<FunctionalGroupPermission> permissions;

  ON_CALL(*cache_manager_, IsPredataPolicy(kValidAppId))
      .WillByDefault(Return(true));
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
  root["policy_table"].removeMember("app_policies");
  ifile.close();
  policy_table::Table update(&root);
  update.SetPolicyTableType(policy_table::PT_UPDATE);

  ASSERT_FALSE(IsValid(update));

  std::string json = root.toStyledString();
  BinaryMessage msg(json.begin(), json.end());

  EXPECT_CALL(*cache_manager_, GetVehicleDataItems())
      .WillOnce(Return(vehicle_items));
  EXPECT_CALL(*cache_manager_, SaveUpdateRequired(false)).Times(0);
  EXPECT_EQ(PolicyManager::PtProcessingResult::kWrongPtReceived,
            policy_manager_->LoadPT(kInValidFilename, msg));
}

TEST_F(PolicyManagerImplTest,
       LoadPT_SnapshotNotGenerated_ReturnkNewPtRequired) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;

  std::ifstream ifile(kSdlPreloadedPtJson);
  ASSERT_TRUE(ifile.good());

  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  ifile.close();
  policy_table::Table update(&root);

  update.SetPolicyTableType(policy_table::PT_UPDATE);

  ASSERT_TRUE(IsValid(update));

  std::string json = root.toStyledString();
  BinaryMessage msg(json.begin(), json.end());

  EXPECT_CALL(*cache_manager_, GetVehicleDataItems())
      .WillOnce(Return(vehicle_items));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(nullptr));
  EXPECT_EQ(PolicyManager::PtProcessingResult::kNewPtRequired,
            policy_manager_->LoadPT(kInValidFilename, msg));
}

TEST_F(PolicyManagerImplTest, LoadPT_UpdateNotApplied_ReturnkNewPtRequired) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;
  std::ifstream ifile(kSdlPreloadedPtJson);
  ASSERT_TRUE(ifile.good());

  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  ifile.close();
  policy_table::Table update(&root);

  update.SetPolicyTableType(policy_table::PT_UPDATE);

  ASSERT_TRUE(IsValid(update));

  std::string json = root.toStyledString();
  BinaryMessage msg(json.begin(), json.end());

  EXPECT_CALL(*cache_manager_, GetVehicleDataItems())
      .WillOnce(Return(vehicle_items));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot())
      .WillOnce(Return(std::make_shared<policy_table::Table>(update)));
  EXPECT_CALL(*cache_manager_, ApplyUpdate(_)).WillOnce(Return(false));
  EXPECT_EQ(PolicyManager::PtProcessingResult::kNewPtRequired,
            policy_manager_->LoadPT(kInValidFilename, msg));
}
TEST_F(PolicyManagerImplTest, LoadPT_NoHMIAppTypes_ReturnkSuccess) {
  std::vector<policy_table::VehicleDataItem> vehicle_items;

  std::ifstream ifile(kSdlPreloadedPtJson);
  ASSERT_TRUE(ifile.good());

  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  ifile.close();
  policy_table::Table update(&root);

  update.SetPolicyTableType(policy_table::PT_UPDATE);

  ASSERT_TRUE(IsValid(update));

  std::string json = root.toStyledString();
  BinaryMessage msg(json.begin(), json.end());

  ON_CALL(*cache_manager_, GetVehicleDataItems())
      .WillByDefault(Return(vehicle_items));
  ON_CALL(*cache_manager_, GenerateSnapshot())
      .WillByDefault(Return(std::make_shared<policy_table::Table>(update)));
  ON_CALL(*cache_manager_, ApplyUpdate(_)).WillByDefault(Return(true));

  EXPECT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            policy_manager_->LoadPT(kInValidFilename, msg));
}

TEST_F(PolicyManagerImplTest,
       AddApplication_NewApplication_ReturnCallStatusChanges) {
  AppHmiTypes hmi_types;

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(false));

  EXPECT_CALL(*cache_manager_, IsPredataPolicy(kValidAppId)).Times(0);

  policy_manager_->AddApplication(kDeviceNumber, kValidAppId, hmi_types);
}

TEST_F(PolicyManagerImplTest,
       AddApplication_ExistedApplication_ReturnCallNothing) {
  AppHmiTypes hmi_types;

  ON_CALL(*cache_manager_, IsApplicationRepresented(kValidAppId))
      .WillByDefault(Return(true));

  EXPECT_CALL(*cache_manager_, IsPredataPolicy(kValidAppId))
      .WillOnce(Return(true));

  policy_manager_->AddApplication(kDeviceNumber, kValidAppId, hmi_types);
}

TEST_F(PolicyManagerImplTest,
       OnPTUFinished_PtuResultIskNewPtRequired_InvokeForcePTExchange) {
  const std::string initial_pt_status = policy_manager_->GetPolicyTableStatus();

  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(nullptr));

  policy_manager_->OnPTUFinished(
      PolicyManager::PtProcessingResult::kNewPtRequired);
  const std::string final_pt_status = policy_manager_->GetPolicyTableStatus();
  EXPECT_NE(initial_pt_status, final_pt_status);
}

TEST_F(
    PolicyManagerImplTest,
    OnPTUFinished_PtuResultIskWrongPtReceived_NoPTExchangeAndNoRefreshRetrySequence) {
  const std::string initial_pt_status = policy_manager_->GetPolicyTableStatus();

  EXPECT_CALL(*cache_manager_, TimeoutResponse()).Times(0);
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_)).Times(0);

  policy_manager_->OnPTUFinished(
      PolicyManager::PtProcessingResult::kWrongPtReceived);
  const std::string final_pt_status = policy_manager_->GetPolicyTableStatus();
  EXPECT_EQ(initial_pt_status, final_pt_status);
}

TEST_F(PolicyManagerImplTest,
       OnPTUFinished_PtuResultIskSuccess_InvokeRefreshRetrySequence) {
  const std::string initial_pt_status = policy_manager_->GetPolicyTableStatus();

  EXPECT_CALL(*cache_manager_, TimeoutResponse());
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_));

  policy_manager_->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
  const std::string final_pt_status = policy_manager_->GetPolicyTableStatus();
  EXPECT_EQ(initial_pt_status, final_pt_status);
}

TEST_F(PolicyManagerImplTest, RequestPTUpdate_SnapshotPtrIsNull_ReturnFalse) {
  ON_CALL(*cache_manager_, GenerateSnapshot()).WillByDefault(Return(nullptr));

  EXPECT_FALSE(
      policy_manager_->RequestPTUpdate(PTUIterationType::DefaultIteration));
}

TEST_F(PolicyManagerImplTest, RequestPTUpdate_PTIsValid_PTIsUpdated) {
  std::ifstream ifile(kSdlPreloadedPtJson);
  ASSERT_TRUE(ifile.good());

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

  ON_CALL(*cache_manager_, GenerateSnapshot()).WillByDefault(Return(snapshot));
  EXPECT_CALL(listener_,
              OnSnapshotCreated(_, PTUIterationType::DefaultIteration));
  EXPECT_TRUE(
      policy_manager_->RequestPTUpdate(PTUIterationType::DefaultIteration));
}

TEST_F(
    PolicyManagerImplTest,
    SendNotificationOnPermissionsUpdated_AppIsRemoteControl_PermissionsUpdated) {
  EXPECT_CALL(*access_remote_, IsAppRemoteControl(_))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(listener_, OnPermissionsUpdated(kDeviceNumber, _, _));

  policy_manager_->SendNotificationOnPermissionsUpdated(kDeviceNumber,
                                                        kValidAppId);
}

TEST_F(
    PolicyManagerImplTest,
    SendNotificationOnPermissionsUpdated_NotRemoteControlApp_PermissionsUpdated) {
  const std::string default_hmi = "NONE";
  EXPECT_CALL(listener_,
              OnPermissionsUpdated(kDeviceNumber, _, _, default_hmi));

  policy_manager_->SendNotificationOnPermissionsUpdated(kDeviceNumber,
                                                        kValidAppId);
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

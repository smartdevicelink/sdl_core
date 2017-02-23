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

#include "json/reader.h"
#include "gtest/gtest.h"

#include "policy/policy_manager_impl_test_base.h"
#include "utils/make_shared.h"

using ::testing::_;
using ::testing::Return;
using ::testing::SetArgReferee;
using ::testing::AtLeast;

namespace test {
namespace components {
namespace policy_test {

TEST_F(
    PolicyManagerImplTest,
    RefreshRetrySequence_SetSecondsBetweenRetries_ExpectRetryTimeoutSequenceWithSameSeconds) {
  // Arrange
  std::vector<int> seconds;
  seconds.push_back(50);
  seconds.push_back(100);
  seconds.push_back(200);

  // Assert
  EXPECT_CALL(*cache_manager_, TimeoutResponse()).WillOnce(Return(60));
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_))
      .WillOnce(DoAll(SetArgReferee<0>(seconds), Return(true)));

  // Act
  manager_->RefreshRetrySequence();

  // Assert
  EXPECT_EQ(50, manager_->NextRetryTimeout());
  EXPECT_EQ(100, manager_->NextRetryTimeout());
  EXPECT_EQ(200, manager_->NextRetryTimeout());
  EXPECT_EQ(0, manager_->NextRetryTimeout());
}

TEST_F(PolicyManagerImplTest, GetNotificationsNumber) {
  std::string priority = "EMERGENCY";
  uint32_t notif_number = 100;
  EXPECT_CALL(*cache_manager_, GetNotificationsNumber(priority))
      .WillOnce(Return(notif_number));

  EXPECT_EQ(notif_number, manager_->GetNotificationsNumber(priority));
}

TEST_F(PolicyManagerImplTest, IncrementGlobalCounter) {
  // Assert
  EXPECT_CALL(*cache_manager_, Increment(usage_statistics::SYNC_REBOOTS));
  manager_->Increment(usage_statistics::SYNC_REBOOTS);
}

TEST_F(PolicyManagerImplTest, IncrementAppCounter) {
  // Assert
  EXPECT_CALL(*cache_manager_,
              Increment("12345", usage_statistics::USER_SELECTIONS));
  manager_->Increment("12345", usage_statistics::USER_SELECTIONS);
}

TEST_F(PolicyManagerImplTest, SetAppInfo) {
  // Assert
  EXPECT_CALL(*cache_manager_,
              Set("12345", usage_statistics::LANGUAGE_GUI, "de-de"));
  manager_->Set("12345", usage_statistics::LANGUAGE_GUI, "de-de");
}

TEST_F(PolicyManagerImplTest, AddAppStopwatch) {
  // Assert
  EXPECT_CALL(*cache_manager_,
              Add("12345", usage_statistics::SECONDS_HMI_FULL, 30));
  manager_->Add("12345", usage_statistics::SECONDS_HMI_FULL, 30);
}

TEST_F(PolicyManagerImplTest, ResetPT) {
  EXPECT_CALL(*cache_manager_, ResetPT("filename"))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager_, IsPTPreloaded())
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager_, ResetCalculatedPermissions()).Times(AtLeast(1));
  EXPECT_CALL(*cache_manager_, TimeoutResponse());
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_));

  policy::CacheManagerInterfaceSPtr cache = manager_->GetCache();
  EXPECT_TRUE(manager_->ResetPT("filename"));
  EXPECT_TRUE(cache->IsPTPreloaded());
  EXPECT_FALSE(manager_->ResetPT("filename"));
  EXPECT_FALSE(cache->IsPTPreloaded());
}

TEST_F(PolicyManagerImplTest, LoadPT_SetPT_PTIsLoaded) {
  // Arrange
  manager_->ForcePTExchange();
  manager_->OnUpdateStarted();
  Json::Value table = createPTforLoad();

  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  // Assert
  ASSERT_TRUE(IsValid(update));

  EXPECT_CALL(*cache_manager_, GetHMIAppTypeAfterUpdate(_)).Times(AtLeast(1));

  // Act
  std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  utils::SharedPtr<policy_table::Table> snapshot =
      new policy_table::Table(update.policy_table);
  // Assert
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(snapshot));
  EXPECT_CALL(*cache_manager_, ApplyUpdate(_)).WillOnce(Return(true));
  EXPECT_CALL(listener_, GetAppName("1234"))
      .WillOnce(Return(custom_str::CustomString("")));
  EXPECT_CALL(listener_, OnUpdateStatusChanged(_));
  EXPECT_CALL(*cache_manager_, SaveUpdateRequired(false));
  EXPECT_CALL(*cache_manager_, TimeoutResponse());
  EXPECT_CALL(*cache_manager_, SecondsBetweenRetries(_));

  EXPECT_TRUE(manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_CALL(*cache_manager_, IsPTPreloaded());
  EXPECT_FALSE(manager_->GetCache()->IsPTPreloaded());
}

TEST_F(PolicyManagerImplTest2,
       KmsChanged_SetExceededKms_ExpectCorrectSchedule) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  ::policy::Counters counter = ::policy::Counters::KILOMETERS;
  manager_->PTUpdatedAt(counter, 50000);
  EXPECT_EQ("UP_TO_DATE", manager_->GetPolicyTableStatus());
  // Set kms changed but not exceed limit
  manager_->KmsChanged(51500);
  EXPECT_EQ("UP_TO_DATE", manager_->GetPolicyTableStatus());
  // Set kms changed and exceed limit
  manager_->KmsChanged(52500);
  EXPECT_EQ("UPDATE_NEEDED", manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, ForcePTExchange_ExpectUpdateNeeded) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  EXPECT_EQ("UP_TO_DATE", manager_->GetPolicyTableStatus());
  // Force OT Exchange
  manager_->ForcePTExchange();
  // Check update required
  EXPECT_EQ("UPDATE_NEEDED", manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, OnSystemReady) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  // Check
  EXPECT_CALL(listener_, OnSystemInfoUpdateRequired());
  manager_->OnSystemReady();
}

TEST_F(PolicyManagerImplTest2, ResetRetrySequence) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  manager_->ResetRetrySequence();
  EXPECT_EQ("UPDATE_NEEDED", manager_->GetPolicyTableStatus());
  manager_->OnUpdateStarted();
  EXPECT_EQ("UPDATING", manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, NextRetryTimeout_ExpectTimeoutsFromPT) {
  // Arrange
  std::ifstream ifile(preloadet_pt_filename_);
  Json::Reader reader;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    Json::Value seconds_between_retries = Json::Value(Json::arrayValue);
    seconds_between_retries =
        root["policy_table"]["module_config"]["seconds_between_retries"];
    uint32_t size = seconds_between_retries.size();
    CreateLocalPT(preloadet_pt_filename_);
    for (uint32_t i = 0; i < size; ++i) {
      EXPECT_EQ(seconds_between_retries[i], manager_->NextRetryTimeout());
    }
  }
}

TEST_F(PolicyManagerImplTest2, TimeOutExchange) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  // Check value taken from PT
  EXPECT_EQ(70, manager_->TimeoutExchange());
}

TEST_F(PolicyManagerImplTest,
       RequestPTUpdate_SetPT_GeneratedSnapshotAndPTUpdate) {
  Json::Value table = createPTforLoad();
  utils::SharedPtr<policy_table::Table> p_table =
      utils::MakeShared<policy_table::Table>(&table);
  ASSERT_TRUE(p_table);
  p_table->SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);
  EXPECT_TRUE(IsValid(*p_table));

  EXPECT_CALL(listener_, OnSnapshotCreated(_, _, _));
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(p_table));

  manager_->RequestPTUpdate();
}

TEST_F(PolicyManagerImplTest, RequestPTUpdate_InvalidPT_PTUpdateFail) {
  utils::SharedPtr<policy_table::Table> p_table =
      utils::MakeShared<policy_table::Table>();
  ASSERT_TRUE(p_table);
  EXPECT_FALSE(IsValid(*p_table));

  EXPECT_CALL(listener_, OnSnapshotCreated(_, _, _)).Times(0);
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(p_table));

  manager_->RequestPTUpdate();
}

TEST_F(PolicyManagerImplTest, RequestPTUpdate_InvalidSnapshot_PTUpdateFail) {
  utils::SharedPtr<policy_table::Table> p_table;
  EXPECT_FALSE(p_table);

  EXPECT_CALL(listener_, OnSnapshotCreated(_, _, _)).Times(0);
  EXPECT_CALL(*cache_manager_, GenerateSnapshot()).WillOnce(Return(p_table));

  manager_->RequestPTUpdate();
}

TEST_F(PolicyManagerImplTest, ResetUserConsent_ResetOnlyOnce) {
  EXPECT_CALL(*cache_manager_, ResetUserConsent())
      .WillOnce(Return(true))
      .WillOnce(Return(false));

  EXPECT_TRUE(manager_->ResetUserConsent());
  EXPECT_FALSE(manager_->ResetUserConsent());
}

TEST_F(PolicyManagerImplTest2, GetPolicyTableStatus_ExpectUpToDate) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  // Check
  EXPECT_EQ("UP_TO_DATE", manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest,
       SetUpdateStarted_GetPolicyTableStatus_Expect_Updating) {
  // Arrange
  manager_->ForcePTExchange();
  EXPECT_CALL(*cache_manager_, SaveUpdateRequired(true));
  manager_->OnUpdateStarted();
  // Check
  EXPECT_EQ("UPDATING", manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2,
       RetrySequenceDelaysSeconds_Expect_CorrectValues) {
  // Arrange
  std::ifstream ifile(preloadet_pt_filename_);
  Json::Reader reader;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    Json::Value seconds_between_retries = Json::Value(Json::arrayValue);
    seconds_between_retries =
        root["policy_table"]["module_config"]["seconds_between_retries"];
    uint32_t size = seconds_between_retries.size();
    CreateLocalPT(preloadet_pt_filename_);
    std::vector<int> delaySecs = manager_->RetrySequenceDelaysSeconds();
    // Check
    ASSERT_EQ(size, delaySecs.size());
    for (uint32_t i = 0; i < size; ++i) {
      EXPECT_EQ(seconds_between_retries[i], delaySecs[i]);
    }
  }
}

TEST_F(PolicyManagerImplTest2,
       OnExceededTimeout_GetPolicyTableStatus_ExpectUpdateNeeded) {
  // Arrange
  CreateLocalPT(preloadet_pt_filename_);
  manager_->ForcePTExchange();
  manager_->OnExceededTimeout();
  // Check
  EXPECT_EQ("UPDATE_NEEDED", manager_->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest, MarkUnpairedDevice) {
  // Assert
  EXPECT_CALL(*cache_manager_, SetUnpairedDevice(unpaired_device_id_, true))
      .WillOnce(Return(true));
  EXPECT_CALL(*cache_manager_, SetDeviceConsent(unpaired_device_id_, false));
  EXPECT_CALL(*cache_manager_,
              HasDeviceSpecifiedConsent(unpaired_device_id_, false))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*cache_manager_, IgnitionCyclesBeforeExchange());
  EXPECT_CALL(*cache_manager_, DaysBeforeExchange(_));
  // Act
  manager_->MarkUnpairedDevice(unpaired_device_id_);
}

TEST_F(PolicyManagerImplTest2, GetCurrentDeviceId) {
  // Arrange
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_2_)).Times(1);
  EXPECT_EQ("", manager_->GetCurrentDeviceId(app_id_2_));
}

TEST_F(PolicyManagerImplTest2,
       CCS_SetCCSStatusWhileAppExists_ExpectUserConsentsUpdateForApp) {
  using namespace policy_table;
  using namespace rpc;

  PreconditionCCSPreparePTWithAppGroupsAndConsents();

  // Act
  utils::SharedPtr<policy_table::Table> pt =
      policy_manager_->GetCache()->GetPT();

  // Checking groups consents before setting CCS status
  policy_table::DeviceData::const_iterator updated_device_data =
      pt->policy_table.device_data->find(device_id_1_);

  EXPECT_TRUE(pt->policy_table.device_data->end() != updated_device_data);

  UserConsentRecords::const_iterator updated_consent_records =
      updated_device_data->second.user_consent_records->find(app_id_1_);

  EXPECT_TRUE(updated_device_data->second.user_consent_records->end() !=
              updated_consent_records);

  ConsentGroups::const_iterator updated_group_1 =
      updated_consent_records->second.consent_groups->find(CCS::group_name_1);
  EXPECT_TRUE(updated_consent_records->second.consent_groups->end() !=
              updated_group_1);

  EXPECT_EQ(Boolean(true), updated_group_1->second);

  ConsentGroups::const_iterator updated_group_2 =
      updated_consent_records->second.consent_groups->find(CCS::group_name_2);

  EXPECT_TRUE(updated_consent_records->second.consent_groups->end() !=
              updated_group_2);

  EXPECT_EQ(Boolean(false), updated_group_2->second);

  ConsentGroups::const_iterator updated_group_3 =
      updated_consent_records->second.consent_groups->find(CCS::group_name_3);

  EXPECT_FALSE(updated_consent_records->second.consent_groups->end() !=
               updated_group_3);

  CCSStatus status;
  status.insert(CCSStatusItem(CCS::type_1, CCS::id_1, "ON"));
  status.insert(CCSStatusItem(CCS::type_2, CCS::id_2, "ON"));
  status.insert(CCSStatusItem(CCS::type_3, CCS::id_3, "ON"));

  EXPECT_CALL(listener_, OnPermissionsUpdated(app_id_1_, _));

  EXPECT_TRUE(policy_manager_->SetCCSStatus(status));

  // Checking groups consent after setting CCS status
  EXPECT_EQ(Boolean(false), updated_group_1->second);
  EXPECT_EQ(Boolean(true), updated_group_2->second);

  // Check, that no consent has been added for third groups
  updated_group_3 =
      updated_consent_records->second.consent_groups->find(CCS::group_name_3);

  EXPECT_FALSE(updated_consent_records->second.consent_groups->end() !=
               updated_group_3);
}

TEST_F(PolicyManagerImplTest2,
       CCS_SetCCSStatusWhileAppExists_ExpectCCSConsentsUpdateForApp) {
  using namespace policy_table;
  using namespace rpc;

  PreconditionCCSPreparePTWithAppGroupsAndConsents();

  // Act
  utils::SharedPtr<policy_table::Table> pt =
      policy_manager_->GetCache()->GetPT();

  // Checking CCS consents before setting new CCS status
  policy_table::DeviceData::const_iterator updated_device_data =
      pt->policy_table.device_data->find(device_id_1_);

  EXPECT_TRUE(pt->policy_table.device_data->end() != updated_device_data);

  UserConsentRecords::const_iterator updated_consent_records =
      updated_device_data->second.user_consent_records->find(app_id_1_);

  EXPECT_TRUE(updated_device_data->second.user_consent_records->end() !=
              updated_consent_records);

  EXPECT_TRUE(updated_consent_records->second.ccs_consent_groups->empty());

  CCSStatus status;
  status.insert(CCSStatusItem(CCS::type_1, CCS::id_1, "ON"));
  status.insert(CCSStatusItem(CCS::type_2, CCS::id_2, "ON"));
  status.insert(CCSStatusItem(CCS::type_3, CCS::id_3, "ON"));

  EXPECT_CALL(listener_, OnPermissionsUpdated(app_id_1_, _));

  EXPECT_TRUE(policy_manager_->SetCCSStatus(status));

  // Checking CCS consents after setting new CCS status
  const ConsentGroups& ccs_consents =
      *updated_consent_records->second.ccs_consent_groups;

  ApplicationPolicies::const_iterator app_parameters =
      pt->policy_table.app_policies_section.apps.find(app_id_1_);

  EXPECT_TRUE(pt->policy_table.app_policies_section.apps.end() !=
              app_parameters);

  EXPECT_EQ(app_parameters->second.groups.size(), ccs_consents.size());

  ConsentGroups::const_iterator updated_group_1 =
      ccs_consents.find(CCS::group_name_1);

  EXPECT_TRUE(ccs_consents.end() != updated_group_1);

  ConsentGroups::const_iterator updated_group_2 =
      ccs_consents.find(CCS::group_name_2);

  EXPECT_TRUE(ccs_consents.end() != updated_group_2);

  EXPECT_EQ(Boolean(false), updated_group_1->second);
  EXPECT_EQ(Boolean(true), updated_group_2->second);
}

TEST_F(PolicyManagerImplTest2,
       CCS_SetCCSStatusNewAppAddedAfterward_ExpectCCSConsentsUpdateForApp) {
  using namespace policy_table;
  using namespace rpc;

  PreconditionCCSPreparePTWithAppPolicy();

  // Act
  utils::SharedPtr<policy_table::Table> pt =
      policy_manager_->GetCache()->GetPT();

  CCSStatus status;
  status.insert(CCSStatusItem(CCS::type_1, CCS::id_1, "ON"));
  status.insert(CCSStatusItem(CCS::type_2, CCS::id_2, "ON"));
  status.insert(CCSStatusItem(CCS::type_3, CCS::id_3, "ON"));

  EXPECT_TRUE(policy_manager_->SetCCSStatus(status));

  // Checking CCS consents after setting new CCS status
  policy_table::DeviceData::const_iterator updated_device_data =
      pt->policy_table.device_data->find(device_id_1_);

  EXPECT_FALSE(pt->policy_table.device_data->end() != updated_device_data);

  EXPECT_CALL(listener_, OnPermissionsUpdated(app_id_1_, _));
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_1_))
      .WillRepeatedly(Return(device_id_1_));

  policy_manager_->AddApplication(app_id_1_, AppHmiTypes());

  // Check CCS consents for application
  updated_device_data = pt->policy_table.device_data->find(device_id_1_);

  EXPECT_TRUE(pt->policy_table.device_data->end() != updated_device_data);

  UserConsentRecords::const_iterator updated_consent_records =
      updated_device_data->second.user_consent_records->find(app_id_1_);

  const ConsentGroups& ccs_consents =
      *updated_consent_records->second.ccs_consent_groups;

  ApplicationPolicies::const_iterator app_parameters =
      pt->policy_table.app_policies_section.apps.find(app_id_1_);

  app_parameters = pt->policy_table.app_policies_section.apps.find(app_id_1_);

  EXPECT_TRUE(pt->policy_table.app_policies_section.apps.end() !=
              app_parameters);

  EXPECT_EQ(app_parameters->second.groups.size(), ccs_consents.size());

  ConsentGroups::const_iterator updated_group_1 =
      ccs_consents.find(CCS::group_name_1);

  EXPECT_TRUE(ccs_consents.end() != updated_group_1);

  ConsentGroups::const_iterator updated_group_2 =
      ccs_consents.find(CCS::group_name_2);

  EXPECT_TRUE(ccs_consents.end() != updated_group_2);

  EXPECT_EQ(Boolean(false), updated_group_1->second);
  EXPECT_EQ(Boolean(true), updated_group_2->second);
}

TEST_F(PolicyManagerImplTest2,
       CCS_SetCCSStatusNewAppAddedAfterward_ExpectUserConsentsUpdateForApp) {
  using namespace policy_table;
  using namespace rpc;

  PreconditionCCSPreparePTWithAppPolicy();

  // Act
  utils::SharedPtr<policy_table::Table> pt =
      policy_manager_->GetCache()->GetPT();

  CCSStatus status;
  status.insert(CCSStatusItem(CCS::type_1, CCS::id_1, "ON"));
  status.insert(CCSStatusItem(CCS::type_2, CCS::id_2, "ON"));
  status.insert(CCSStatusItem(CCS::type_3, CCS::id_3, "ON"));

  EXPECT_TRUE(policy_manager_->SetCCSStatus(status));

  // Checking CCS consents after setting new CCS status
  policy_table::DeviceData::const_iterator updated_device_data =
      pt->policy_table.device_data->find(device_id_1_);

  EXPECT_FALSE(pt->policy_table.device_data->end() != updated_device_data);

  EXPECT_CALL(listener_, OnPermissionsUpdated(app_id_1_, _));
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_1_))
      .WillRepeatedly(Return(device_id_1_));

  policy_manager_->AddApplication(app_id_1_, AppHmiTypes());

  // Checking CCS consents after setting new CCS status
  ApplicationPolicies::const_iterator app_parameters =
      pt->policy_table.app_policies_section.apps.find(app_id_1_);

  EXPECT_TRUE(pt->policy_table.app_policies_section.apps.end() !=
              app_parameters);

  updated_device_data = pt->policy_table.device_data->find(device_id_1_);

  EXPECT_TRUE(pt->policy_table.device_data->end() != updated_device_data);

  UserConsentRecords::const_iterator updated_consent_records =
      updated_device_data->second.user_consent_records->find(app_id_1_);

  EXPECT_TRUE(updated_device_data->second.user_consent_records->end() !=
              updated_consent_records);

  const ConsentGroups& ccs_consents =
      *updated_consent_records->second.consent_groups;

  EXPECT_EQ(app_parameters->second.groups.size(), ccs_consents.size());

  ConsentGroups::const_iterator updated_group_1 =
      ccs_consents.find(CCS::group_name_1);

  EXPECT_TRUE(ccs_consents.end() != updated_group_1);

  ConsentGroups::const_iterator updated_group_2 =
      ccs_consents.find(CCS::group_name_2);

  EXPECT_TRUE(ccs_consents.end() != updated_group_2);

  EXPECT_EQ(Boolean(false), updated_group_1->second);
  EXPECT_EQ(Boolean(true), updated_group_2->second);
}

TEST_F(PolicyManagerImplTest2,
       CCS_SetCCSStatusNewAppPromotedAfterward_ExpectUserConsentsUpdateForApp) {
  using namespace policy_table;
  using namespace rpc;

  CreateLocalPT(preloaded_pt_filename_);
  Table t = PreparePTWithGroupsHavingCCS();

  EXPECT_TRUE(policy_manager_->GetCache()->ApplyUpdate(t));

  EXPECT_CALL(listener_, OnPermissionsUpdated(app_id_1_, _)).Times(AtLeast(2));
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_1_))
      .WillOnce(Return(device_id_1_))
      .WillOnce(Return(""))  // not registered
      .WillRepeatedly(Return(device_id_1_));

  // First register w/o app having groups to consent
  policy_manager_->AddApplication(app_id_1_, AppHmiTypes());

  // Act
  utils::SharedPtr<policy_table::Table> pt =
      policy_manager_->GetCache()->GetPT();

  CCSStatus status;
  status.insert(CCSStatusItem(CCS::type_1, CCS::id_1, "ON"));
  status.insert(CCSStatusItem(CCS::type_2, CCS::id_2, "ON"));
  status.insert(CCSStatusItem(CCS::type_3, CCS::id_3, "ON"));

  EXPECT_TRUE(policy_manager_->SetCCSStatus(status));

  // Checking CCS consents after setting new CCS status
  policy_table::DeviceData::const_iterator updated_device_data =
      pt->policy_table.device_data->find(device_id_1_);

  EXPECT_FALSE(pt->policy_table.device_data->end() != updated_device_data);

  ApplicationParams app_params;
  app_params.groups.push_back(CCS::group_name_1);
  app_params.groups.push_back(CCS::group_name_2);

  t.policy_table.app_policies_section.apps.insert(
      std::make_pair(app_id_1_, app_params));

  // Add groups to consent, will be pending
  EXPECT_TRUE(policy_manager_->GetCache()->ApplyUpdate(t));

  // Second time register w/ app having groups to consent
  policy_manager_->AddApplication(app_id_1_, AppHmiTypes());

  // Checking CCS consents after setting new CCS status
  ApplicationPolicies::const_iterator app_parameters =
      pt->policy_table.app_policies_section.apps.find(app_id_1_);

  EXPECT_TRUE(pt->policy_table.app_policies_section.apps.end() !=
              app_parameters);

  updated_device_data = pt->policy_table.device_data->find(device_id_1_);

  EXPECT_TRUE(pt->policy_table.device_data->end() != updated_device_data);

  UserConsentRecords::const_iterator updated_consent_records =
      updated_device_data->second.user_consent_records->find(app_id_1_);

  EXPECT_TRUE(updated_device_data->second.user_consent_records->end() !=
              updated_consent_records);

  const ConsentGroups& ccs_consents =
      *updated_consent_records->second.consent_groups;

  EXPECT_EQ(app_parameters->second.groups.size(), ccs_consents.size());

  ConsentGroups::const_iterator updated_group_1 =
      ccs_consents.find(CCS::group_name_1);

  EXPECT_TRUE(ccs_consents.end() != updated_group_1);

  ConsentGroups::const_iterator updated_group_2 =
      ccs_consents.find(CCS::group_name_2);

  EXPECT_TRUE(ccs_consents.end() != updated_group_2);

  EXPECT_EQ(Boolean(false), updated_group_1->second);
  EXPECT_EQ(Boolean(true), updated_group_2->second);
}

TEST_F(PolicyManagerImplTest2,
       CCS_SetCCSStatusNewAppPromotedAfterward_ExpectCCSConsentsUpdateForApp) {
  using namespace policy_table;
  using namespace rpc;

  CreateLocalPT(preloaded_pt_filename_);
  Table t = PreparePTWithGroupsHavingCCS();

  EXPECT_TRUE(policy_manager_->GetCache()->ApplyUpdate(t));

  EXPECT_CALL(listener_, OnPermissionsUpdated(app_id_1_, _)).Times(AtLeast(2));
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_1_))
      .WillOnce(Return(device_id_1_))
      .WillOnce(Return(""))  // not registered
      .WillRepeatedly(Return(device_id_1_));

  // First register w/o app having groups to consent
  policy_manager_->AddApplication(app_id_1_, AppHmiTypes());

  // Act
  utils::SharedPtr<policy_table::Table> pt =
      policy_manager_->GetCache()->GetPT();

  CCSStatus status;
  status.insert(CCSStatusItem(CCS::type_1, CCS::id_1, "ON"));
  status.insert(CCSStatusItem(CCS::type_2, CCS::id_2, "ON"));
  status.insert(CCSStatusItem(CCS::type_3, CCS::id_3, "ON"));

  EXPECT_TRUE(policy_manager_->SetCCSStatus(status));

  // Checking CCS consents after setting new CCS status
  policy_table::DeviceData::const_iterator updated_device_data =
      pt->policy_table.device_data->find(device_id_1_);

  EXPECT_FALSE(pt->policy_table.device_data->end() != updated_device_data);

  ApplicationParams app_params;
  app_params.groups.push_back(CCS::group_name_1);
  app_params.groups.push_back(CCS::group_name_2);

  t.policy_table.app_policies_section.apps.insert(
      std::make_pair(app_id_1_, app_params));

  // Add groups to consent, will be pending i.e. not consented yet
  EXPECT_TRUE(policy_manager_->GetCache()->ApplyUpdate(t));

  // Second time register w/ app having groups to consent
  policy_manager_->AddApplication(app_id_1_, AppHmiTypes());

  // Check CCS consents for application
  updated_device_data = pt->policy_table.device_data->find(device_id_1_);

  EXPECT_TRUE(pt->policy_table.device_data->end() != updated_device_data);

  UserConsentRecords::const_iterator updated_consent_records =
      updated_device_data->second.user_consent_records->find(app_id_1_);

  const ConsentGroups& ccs_consents =
      *updated_consent_records->second.ccs_consent_groups;

  ApplicationPolicies::const_iterator app_parameters =
      pt->policy_table.app_policies_section.apps.find(app_id_1_);

  app_parameters = pt->policy_table.app_policies_section.apps.find(app_id_1_);

  EXPECT_TRUE(pt->policy_table.app_policies_section.apps.end() !=
              app_parameters);

  EXPECT_EQ(app_parameters->second.groups.size(), ccs_consents.size());

  ConsentGroups::const_iterator updated_group_1 =
      ccs_consents.find(CCS::group_name_1);

  EXPECT_TRUE(ccs_consents.end() != updated_group_1);

  ConsentGroups::const_iterator updated_group_2 =
      ccs_consents.find(CCS::group_name_2);

  EXPECT_TRUE(ccs_consents.end() != updated_group_2);

  EXPECT_EQ(Boolean(false), updated_group_1->second);
  EXPECT_EQ(Boolean(true), updated_group_2->second);
}

TEST_F(PolicyManagerImplTest2,
       CCS_PTUWithNewGroups_ExpectCCSConsentsUpdateForApp) {
  using namespace policy_table;
  using namespace rpc;

  PreconditionCCSPreparePTWithAppGroupsAndConsents();

  const uint32_t type_4 = 6;
  const uint32_t id_4 = 7;
  const std::string group_name_4 = "NewGroup";

  CCSStatus status;
  status.insert(CCSStatusItem(CCS::type_1, CCS::id_1, "ON"));
  status.insert(CCSStatusItem(CCS::type_2, CCS::id_2, "ON"));
  status.insert(CCSStatusItem(type_4, id_4, "ON"));

  EXPECT_TRUE(policy_manager_->SetCCSStatus(status));

  EXPECT_CALL(listener_, OnPermissionsUpdated(app_id_1_, _)).Times(AtLeast(1));
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(app_id_1_))
      .WillRepeatedly(Return(device_id_1_));

  policy_manager_->AddApplication(app_id_1_, AppHmiTypes());

  utils::SharedPtr<policy_table::Table> pt =
      policy_manager_->GetCache()->GetPT();

  // Check CCS consents for application
  policy_table::DeviceData::const_iterator initial_device_data =
      pt->policy_table.device_data->find(device_id_1_);

  EXPECT_TRUE(pt->policy_table.device_data->end() != initial_device_data);

  UserConsentRecords::const_iterator initial_consent_records =
      initial_device_data->second.user_consent_records->find(app_id_1_);

  EXPECT_TRUE(initial_device_data->second.user_consent_records->end() !=
              initial_consent_records);

  const ConsentGroups& user_consents =
      *initial_consent_records->second.consent_groups;

  ConsentGroups::const_iterator group_1 = user_consents.find(CCS::group_name_1);

  EXPECT_TRUE(user_consents.end() != group_1);

  ConsentGroups::const_iterator group_2 = user_consents.find(CCS::group_name_2);

  EXPECT_TRUE(user_consents.end() != group_2);

  ConsentGroups::const_iterator group_4 = user_consents.find(group_name_4);

  EXPECT_FALSE(user_consents.end() != group_4);

  EXPECT_EQ(Boolean(false), group_1->second);
  EXPECT_EQ(Boolean(true), group_2->second);

  ApplicationPolicies::const_iterator app_parameters =
      pt->policy_table.app_policies_section.apps.find(app_id_1_);

  app_parameters = pt->policy_table.app_policies_section.apps.find(app_id_1_);

  EXPECT_TRUE(pt->policy_table.app_policies_section.apps.end() !=
              app_parameters);

  EXPECT_EQ(app_parameters->second.groups.size(), user_consents.size());

  std::ifstream ifile(preloaded_pt_filename_);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    Table t = PreparePTWithGroupsHavingCCS();

    CCS_Entity entity_4(type_4, id_4);

    Rpcs rpcs_4;
    rpcs_4.disallowed_by_ccs_entities_on->push_back(entity_4);
    *rpcs_4.user_consent_prompt = group_name_4;
    rpcs_4.rpcs.set_to_null();

    t.policy_table.functional_groupings.insert(
        std::make_pair(group_name_4, rpcs_4));

    ApplicationParams app_params;
    app_params.groups.push_back(CCS::group_name_1);
    app_params.groups.push_back(CCS::group_name_2);
    app_params.groups.push_back(group_name_4);
    app_params.keep_context = Boolean(true);
    app_params.steal_focus = Boolean(true);
    app_params.default_hmi = HL_FULL;
    app_params.priority = P_EMERGENCY;

    t.policy_table.app_policies_section.apps.insert(
        std::make_pair(app_id_1_, app_params));

    const Json::Value overriden_table = t.ToJsonValue();

    root["policy_table"]["functional_groupings"] =
        overriden_table["policy_table"]["functional_groupings"];

    root["policy_table"]["app_policies"][app_id_1_] =
        overriden_table["policy_table"]["app_policies"][app_id_1_];

    json = root.toStyledString();
  }
  ifile.close();

  const BinaryMessage msg(json.begin(), json.end());

  ON_CALL(listener_, GetRegisteredApps())
      .WillByDefault(Return(policy::KnownConsentsIds()));

  EXPECT_CALL(listener_, OnCertificateUpdated(_));

  EXPECT_TRUE(policy_manager_->LoadPT("DummyFileName", msg));

  pt = policy_manager_->GetCache()->GetPT();

  policy_table::DeviceData::const_iterator updated_device_data =
      pt->policy_table.device_data->find(device_id_1_);

  EXPECT_TRUE(pt->policy_table.device_data->end() != updated_device_data);

  UserConsentRecords::const_iterator updated_consent_records =
      updated_device_data->second.user_consent_records->find(app_id_1_);

  EXPECT_TRUE(updated_device_data->second.user_consent_records->end() !=
              updated_consent_records);

  const ConsentGroups& updated_user_consents =
      *updated_consent_records->second.consent_groups;

  group_1 = updated_user_consents.find(CCS::group_name_1);

  EXPECT_TRUE(updated_user_consents.end() != group_1);

  group_2 = updated_user_consents.find(CCS::group_name_2);

  EXPECT_TRUE(updated_user_consents.end() != group_2);

  group_4 = updated_user_consents.find(group_name_4);

  EXPECT_TRUE(updated_user_consents.end() != group_4);

  EXPECT_EQ(Boolean(false), group_1->second);
  EXPECT_EQ(Boolean(true), group_2->second);
  EXPECT_EQ(Boolean(false), group_4->second);

  app_parameters = pt->policy_table.app_policies_section.apps.find(app_id_1_);

  EXPECT_TRUE(pt->policy_table.app_policies_section.apps.end() !=
              app_parameters);

  EXPECT_EQ(app_parameters->second.groups.size(), user_consents.size());
}

}  // namespace policy
}  // namespace components
}  // namespace test

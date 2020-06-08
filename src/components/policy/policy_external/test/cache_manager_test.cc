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

#include <stdint.h>
#include <fstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "policy/cache_manager.h"
#include "policy/policy_table/enums.h"
#include "policy/policy_types.h"

#include "policy/mock_policy_settings.h"

#include "json/reader.h"
#include "utils/date_time.h"
#include "utils/file_system.h"
#include "utils/gen_hash.h"
#include "utils/jsoncpp_reader_wrapper.h"

namespace test {
namespace components {
namespace policy_test {

using namespace policy;
using namespace rpc::policy_table_interface_base;

using ::testing::_;
using ::testing::NiceMock;
using ::testing::ReturnRef;

namespace {
const std::string kDefaultHMILevel = "NONE";
const std::string kInvalidApp = "invalid_app";
const std::string kSdlPreloadedPtJson = "json/sdl_preloaded_pt.json";
const std::string kValidAppId = "1234";
const std::string kDeviceNumber = "XXX123456789ZZZ";
const std::string kAppStorageFolder = "app_storage_folder";
const std::string kConnectionType = "Bluetooth";
}  // namespace

void ValidateJsonTable(const std::string& string_table,
                       Json::Value& json_table) {
  utils::JsonReader reader;
  ASSERT_TRUE(reader.parse(string_table, &json_table));
}

policy_table::Table CreateCustomPT(const std::string& string_table) {
  Json::Value json_table(Json::objectValue);
  ValidateJsonTable(string_table, json_table);
  policy_table::Table table(&json_table);
  return table;
}

class CacheManagerTest : public ::testing::Test {
 public:
  CacheManagerTest()
      : in_memory_(true), cache_manager_(nullptr), pt_(nullptr) {}

 protected:
  const bool in_memory_;
  std::shared_ptr<CacheManager> cache_manager_;
  std::shared_ptr<policy_table::Table> pt_;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;

  void SetUp() {
    cache_manager_ = std::make_shared<CacheManager>(in_memory_);
    pt_ = cache_manager_->GetPT();

    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
  }
};

TEST_F(CacheManagerTest,
       GetNotificationsNumber_PriorityExists_ReturnNumberFromPT) {
  std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"module_config\": {"
      "\"notifications_per_minute_by_priority\": {"
      "\"EMERGENCY\": 1,"
      "\"NAVIGATION\": 2,"
      "\"VOICECOM\": 3,"
      "\"COMMUNICATION\": 4,"
      "\"NORMAL\": 5,"
      "\"NONE\": 6"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  std::string priority = "EMERGENCY";
  uint32_t notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(1u, notif_number);

  priority = "NAVIGATION";
  notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(2u, notif_number);

  priority = "VOICECOM";
  notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(3u, notif_number);

  priority = "NORMAL";
  notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(5u, notif_number);

  priority = "NONE";
  notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(6u, notif_number);
}

TEST_F(CacheManagerTest, GetNotificationsNumber_PriorityNotExist_ReturnZero) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"module_config\": {"
      "\"notifications_per_minute_by_priority\": {"
      "\"EMERGENCY\": 1,"
      "\"NAVIGATION\": 2,"
      "\"VOICECOM\": 3,"
      "\"COMMUNICATION\": 4,"
      "\"NORMAL\": 5,"
      "\"NONE\": 6"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  const std::string priority = "OTHER_PRIORITY";
  uint32_t notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(0u, notif_number);
}

TEST_F(CacheManagerTest,
       GetConsentsPriority_DeviceIDNotFound_ReturnkExternalConsentPrio) {
  const auto priority =
      cache_manager_->GetConsentsPriority(kDeviceNumber, kValidAppId);

  EXPECT_EQ(ConsentPriorityType::kExternalConsentPrio, priority);
}

TEST_F(CacheManagerTest,
       GetConsentsPriority_AppIDNotFound_ReturnkExternalConsentPrio) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  const auto priority =
      cache_manager_->GetConsentsPriority(kDeviceNumber, kValidAppId);
  EXPECT_EQ(ConsentPriorityType::kExternalConsentPrio, priority);
}

TEST_F(CacheManagerTest,
       GetConsentsPriority_UserConsentHasPriority_ReturnkUserConsentPrio) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "\"1234\": {"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  auto& user_record =
      (*pt_->policy_table.device_data)[kDeviceNumber].user_consent_records;
  (*user_record)[kValidAppId].consent_last_updated = 20;
  (*user_record)[kValidAppId].ext_consent_last_updated = 10;

  const auto priority =
      cache_manager_->GetConsentsPriority(kDeviceNumber, kValidAppId);
  EXPECT_EQ(ConsentPriorityType::kUserConsentPrio, priority);
}

TEST_F(
    CacheManagerTest,
    GetConsentsPriority_ExternalConsentHasPriority_ReturnkExternalConsentPrio) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "\"1234\": {"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  (*(*pt_->policy_table.device_data)[kDeviceNumber]
        .user_consent_records)[kValidAppId]
      .consent_last_updated = 10;
  (*(*pt_->policy_table.device_data)[kDeviceNumber]
        .user_consent_records)[kValidAppId]
      .ext_consent_last_updated = 20;

  auto priority =
      cache_manager_->GetConsentsPriority(kDeviceNumber, kValidAppId);
  EXPECT_EQ(ConsentPriorityType::kExternalConsentPrio, priority);
}

TEST_F(CacheManagerTest, CanAppKeepContext_DeviceApp_ReturnTrue) {
  pt_->policy_table.app_policies_section.device.keep_context = true;
  EXPECT_TRUE(cache_manager_->CanAppKeepContext(kDeviceId));
}

TEST_F(CacheManagerTest, CanAppKeepContext_ValidApp_ReturnTrue) {
  pt_->policy_table.app_policies_section.apps[kValidAppId].keep_context = true;
  EXPECT_TRUE(cache_manager_->CanAppKeepContext(kValidAppId));
}

TEST_F(CacheManagerTest, CanAppKeepContext_InvalidApp_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->CanAppKeepContext(kInvalidApp));
}

TEST_F(CacheManagerTest, HeartBeatTimeout_ValueInitialized_ReturnValue) {
  *pt_->policy_table.app_policies_section.apps[kValidAppId]
       .heart_beat_timeout_ms = 100u;
  EXPECT_EQ(100u, cache_manager_->HeartBeatTimeout(kValidAppId));
}

TEST_F(CacheManagerTest, HeartBeatTimeout_ValueNotInitialized_ReturnZero) {
  pt_->policy_table.app_policies_section.apps[kValidAppId] = {};
  EXPECT_EQ(0u, cache_manager_->HeartBeatTimeout(kValidAppId));
}

TEST_F(CacheManagerTest, HeartBeatTimeout_AppNotRepresented_ReturnZero) {
  EXPECT_EQ(0u, cache_manager_->HeartBeatTimeout(kInvalidApp));
}

TEST_F(CacheManagerTest, GetHMITypes_NoSuchAppNoHmiTypes_ReturnNull) {
  EXPECT_EQ(nullptr, cache_manager_->GetHMITypes(kInvalidApp));
}

TEST_F(CacheManagerTest, GetHMITypes_ValidApp_ReturnNotNullPtr) {
  AppHMITypes hmi_types;
  hmi_types.push_back(policy_table::AHT_DEFAULT);

  *pt_->policy_table.app_policies_section.apps[kValidAppId].AppHMIType =
      hmi_types;
  EXPECT_NE(nullptr, cache_manager_->GetHMITypes(kValidAppId));
}

TEST_F(CacheManagerTest, CanAppStealFocus_AppIdIsDevice_ReturnTrue) {
  pt_->policy_table.app_policies_section.device.steal_focus = true;
  EXPECT_TRUE(cache_manager_->CanAppStealFocus(kDeviceId));
}

TEST_F(CacheManagerTest, CanAppStealFocus_ValidApp_ReturnTrue) {
  pt_->policy_table.app_policies_section.apps[kValidAppId].steal_focus = true;
  EXPECT_TRUE(cache_manager_->CanAppStealFocus(kValidAppId));
}

TEST_F(CacheManagerTest, CanAppStealFocus_AppNotRepresented_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->CanAppStealFocus(kInvalidApp));
}

TEST_F(CacheManagerTest, GetDefaultHMI_AppIdIsDevice_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"device\": {"
      "\"default_hmi\": \"NONE\""
      "}"
      "}"
      "}"
      "}");

  *pt_ = CreateCustomPT(string_table);
  std::string returned_hmi_level;
  const std::string expected_hmi_level("NONE");

  EXPECT_TRUE(cache_manager_->GetDefaultHMI(kDeviceId, returned_hmi_level));
  EXPECT_EQ(expected_hmi_level, returned_hmi_level);
}

TEST_F(CacheManagerTest, GetDefaultHMI_ValidApp_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"default_hmi\": \"NONE\""
      "}"
      "}"
      "}"
      "}");

  *pt_ = CreateCustomPT(string_table);
  std::string returned_hmi_level;
  const std::string expected_hmi_level("NONE");

  EXPECT_TRUE(cache_manager_->GetDefaultHMI(kValidAppId, returned_hmi_level));
  EXPECT_EQ(expected_hmi_level, returned_hmi_level);
}

TEST_F(CacheManagerTest, GetDefaultHMI_InvalidApp_ReturnFalse) {
  std::string returned_hmi_level;
  EXPECT_FALSE(cache_manager_->GetDefaultHMI(kInvalidApp, returned_hmi_level));
  EXPECT_TRUE(returned_hmi_level.empty());
}

TEST_F(CacheManagerTest, GetAllAppGroups_AppIdIsDevice_AppendGroupId) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"device\": {"
      "\"groups\": [\"default\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetAllAppGroups(kDeviceId, group_ids);
  EXPECT_EQ(1u, group_ids.size());
}

TEST_F(CacheManagerTest, GetAllAppGroups_InvalidAppId_EmptyVectorOfGroupId) {
  FunctionalGroupIDs group_ids;
  cache_manager_->GetAllAppGroups(kInvalidApp, group_ids);
  EXPECT_TRUE(group_ids.empty());
}

TEST_F(CacheManagerTest, GetAllAppGroups_ValidAppId_AppendGroupID) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"groups\": [\"default\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetAllAppGroups(kValidAppId, group_ids);
  EXPECT_EQ(1u, group_ids.size());
}

TEST_F(CacheManagerTest, ApplyUpdate_ValidPT_ReturnTrue) {
  std::ifstream ifile(kSdlPreloadedPtJson);
  ASSERT_TRUE(ifile.good());
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  ifile.close();
  policy_table::Table update(&root);

  EXPECT_TRUE(cache_manager_->ApplyUpdate(update));
  EXPECT_FALSE(cache_manager_->IsPTPreloaded());
}

TEST_F(CacheManagerTest, AppHasHMIType_NoSuchType_ReturnFalse) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"AppHMIType\": []"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_FALSE(
      cache_manager_->AppHasHMIType(kValidAppId, policy_table::AHT_DEFAULT));
}

TEST_F(CacheManagerTest, AppHasHMIType_TypeExists_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"AppHMIType\": [\"AHT_DEFAULT\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_TRUE(
      cache_manager_->AppHasHMIType(kValidAppId, policy_table::AHT_DEFAULT));
}

TEST_F(CacheManagerTest, AppHasHMIType_NoSuchApp_ReturnFalse) {
  EXPECT_FALSE(
      cache_manager_->AppHasHMIType(kInvalidApp, policy_table::AHT_DEFAULT));
}

TEST_F(CacheManagerTest, AddDevice_ReturnTrue) {
  *pt_->policy_table.module_config.preloaded_pt = true;
  EXPECT_TRUE(cache_manager_->AddDevice(kDeviceNumber, kConnectionType));
  EXPECT_FALSE(cache_manager_->IsPTPreloaded());
}

TEST_F(CacheManagerTest, SetDeviceData_NoSuchDevice_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->SetDeviceData(kDeviceNumber));
}

TEST_F(CacheManagerTest, SetDeviceData_ValidDevice_ReturnTrue) {
  EXPECT_TRUE(cache_manager_->AddDevice(kDeviceNumber, kConnectionType));
  EXPECT_TRUE(cache_manager_->SetDeviceData(kDeviceNumber));
}

TEST_F(CacheManagerTest, ResetUserConsent_ConsentIsResetted_ReturnTrue) {
  cache_manager_->AddDevice(kDeviceNumber, kConnectionType);
  policy_table::DeviceParams& params =
      (*(pt_->policy_table.device_data))[kDeviceNumber];
  policy_table::UserConsentRecords user_records;
  user_records[kDeviceNumber] = policy_table::ConsentRecords();
  *params.user_consent_records = user_records;

  EXPECT_FALSE((params.user_consent_records)->empty());
  EXPECT_TRUE(cache_manager_->ResetUserConsent());
  EXPECT_TRUE((params.user_consent_records)->empty());
}

TEST_F(CacheManagerTest, SetUserPermissionsForDevice_ReturnTrue) {
  cache_manager_->AddDevice(kDeviceNumber, kConnectionType);

  StringArray consented_groups;
  StringArray disallowed_groups;
  const std::string consented_group("DataConsent-2");
  const std::string disallowed_group("Navigation-1");
  consented_groups.push_back(consented_group);
  disallowed_groups.push_back(disallowed_group);

  EXPECT_TRUE(cache_manager_->SetUserPermissionsForDevice(
      kDeviceNumber, consented_groups, disallowed_groups));

  StringArray returned_consented_groups;
  StringArray returned_disallowed_groups;
  EXPECT_TRUE(cache_manager_->GetUserPermissionsForDevice(
      kDeviceNumber, returned_consented_groups, returned_disallowed_groups));
  EXPECT_EQ(consented_groups, returned_consented_groups);
  EXPECT_EQ(disallowed_groups, returned_disallowed_groups);
}

TEST_F(CacheManagerTest, GetUserPermissionsForDevice_NoSuchDevice_ReturnFalse) {
  StringArray consented_groups;
  StringArray disallowed_groups;

  EXPECT_FALSE(cache_manager_->GetUserPermissionsForDevice(
      kDeviceNumber, consented_groups, disallowed_groups));
}

TEST_F(CacheManagerTest, GetPreConsentedGroups_DeviceAppId_GetGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"device\": {"
      "\"preconsented_groups\": [\"DataConsent-2\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetPreConsentedGroups(kDeviceId, group_ids);
  EXPECT_EQ(1u, group_ids.size());
  EXPECT_EQ(cache_manager_->GenerateHash("DataConsent-2"), group_ids[0]);
}

TEST_F(CacheManagerTest, GetPreConsentedGroups_NoSuchAppId_NoGroups) {
  FunctionalGroupIDs group_ids;

  cache_manager_->GetPreConsentedGroups(kInvalidApp, group_ids);
  EXPECT_TRUE(group_ids.empty());
}

TEST_F(CacheManagerTest, GetPreConsentedGroups_ValidAppId_GetGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"preconsented_groups\": [\"DataConsent-2\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetPreConsentedGroups(kValidAppId, group_ids);
  EXPECT_EQ(1u, group_ids.size());
  EXPECT_EQ(cache_manager_->GenerateHash("DataConsent-2"), group_ids[0]);
}

TEST_F(CacheManagerTest, CheckPermissions_ValidParams_ReturnValidPermission) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"functional_groupings\": {"
      "\"Base-4\": {"
      "\"rpcs\": {"
      "\"AddCommand\": {"
      "\"hmi_levels\": ["
      "\"BACKGROUND\","
      "\"FULL\","
      "\"LIMITED\""
      "]"
      "}"
      "}"
      "}"
      "},"
      "\"app_policies\": {"
      "\"default\": {"
      "\"groups\": ["
      "\"Base-4\""
      "]"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  const std::string hmi_level("FULL");
  const std::string rpc("AddCommand");
  CheckPermissionResult result;

  cache_manager_->CheckPermissions(kDefaultId, hmi_level, rpc, result);
  EXPECT_EQ(kRpcAllowed, result.hmi_level_permitted);
}

TEST_F(CacheManagerTest, CheckPermissions_InvalidApp_ReturnInvalidPermission) {
  const std::string hmi_level("FULL");
  const std::string rpc("AddCommand");
  CheckPermissionResult result;
  cache_manager_->CheckPermissions(kInvalidApp, hmi_level, rpc, result);
  EXPECT_EQ(kRpcDisallowed, result.hmi_level_permitted);
}

TEST_F(CacheManagerTest, GetAppRequestTypesState_GetAllStates) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"RequestType\": [\"PROPRIETARY\"]"
      "},"
      "\"default\": {"
      "\"RequestType\": [],"
      "},"
      "\"pre_DataConsent\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_EQ(RequestType::State::EMPTY,
            cache_manager_->GetAppRequestTypesState(kDefaultId));
  EXPECT_EQ(RequestType::State::UNAVAILABLE,
            cache_manager_->GetAppRequestTypesState(kInvalidApp));
  EXPECT_EQ(RequestType::State::OMITTED,
            cache_manager_->GetAppRequestTypesState(kPreDataConsentId));
  EXPECT_EQ(RequestType::State::AVAILABLE,
            cache_manager_->GetAppRequestTypesState(kValidAppId));
}

TEST_F(CacheManagerTest, GetAppRequestSubTypesState_GetAllStates) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"RequestSubType\": [\"PROPRIETARY\"]"
      "},"
      "\"default\": {"
      "\"RequestSubType\": [],"
      "},"
      "\"pre_DataConsent\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_EQ(RequestSubType::State::EMPTY,
            cache_manager_->GetAppRequestSubTypesState(kDefaultId));
  EXPECT_EQ(RequestSubType::State::UNAVAILABLE,
            cache_manager_->GetAppRequestSubTypesState(kInvalidApp));
  EXPECT_EQ(RequestSubType::State::OMITTED,
            cache_manager_->GetAppRequestSubTypesState(kPreDataConsentId));
  EXPECT_EQ(RequestSubType::State::AVAILABLE,
            cache_manager_->GetAppRequestSubTypesState(kValidAppId));
}

TEST_F(CacheManagerTest, IgnitionCyclesBeforeExchange_GetValue) {
  // if result of computation is positive, method returns difference between
  // limit from pt and current number of ignition cycles
  pt_->policy_table.module_config.exchange_after_x_ignition_cycles = 100u;
  *(pt_->policy_table.module_meta->ignition_cycles_since_last_exchange) = 80u;

  EXPECT_EQ(20, cache_manager_->IgnitionCyclesBeforeExchange());

  // if result of computation is less or equal to zero, method returns 0
  pt_->policy_table.module_config.exchange_after_x_ignition_cycles = 80u;
  *(pt_->policy_table.module_meta->ignition_cycles_since_last_exchange) = 100u;

  EXPECT_EQ(0, cache_manager_->IgnitionCyclesBeforeExchange());
}

TEST_F(CacheManagerTest, KilometersBeforeExchange_GetValue) {
  // if result of computation is positive, method returns difference between
  // limit from pt and actual number of kilometers
  pt_->policy_table.module_config.exchange_after_x_kilometers = 1000u;
  *(pt_->policy_table.module_meta->pt_exchanged_at_odometer_x) = 500u;

  EXPECT_EQ(600, cache_manager_->KilometersBeforeExchange(900));

  // if result of computation is less or equal to zero, method returns 0
  pt_->policy_table.module_config.exchange_after_x_kilometers = 500u;
  *(pt_->policy_table.module_meta->pt_exchanged_at_odometer_x) = 200u;

  EXPECT_EQ(0, cache_manager_->KilometersBeforeExchange(800));
}

TEST_F(CacheManagerTest,
       DaysBeforeExchange_DaysNotInitialized_ReturnNegativeOne) {
  EXPECT_EQ(cache_manager_->DaysBeforeExchange(20), -1);
}

TEST_F(CacheManagerTest, DaysBeforeExchange_DaysIsInitialized_ReturnValue) {
  *(pt_->policy_table.module_meta->pt_exchanged_x_days_after_epoch) = 5;
  pt_->policy_table.module_config.exchange_after_x_days = 30;
  EXPECT_EQ(15, cache_manager_->DaysBeforeExchange(20));
}

TEST_F(CacheManagerTest,
       SetCountersPassedForSuccessfulUpdate_AllPositiveCases_ReturnTrue) {
  EXPECT_TRUE(cache_manager_->SetCountersPassedForSuccessfulUpdate(
      Counters::KILOMETERS, 1000));
  EXPECT_TRUE(cache_manager_->SetCountersPassedForSuccessfulUpdate(
      Counters::DAYS_AFTER_EPOCH, 20));
}

TEST_F(CacheManagerTest,
       GetDeviceConsent_EmptyPermissions_ReturnDeviceAllowed) {
  cache_manager_->AddDevice(kDeviceNumber, kConnectionType);
  EXPECT_EQ(kDeviceAllowed, cache_manager_->GetDeviceConsent(kDeviceNumber));
}

TEST_F(CacheManagerTest, GetPriority_DeviceAppID_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"device\": {"
      "\"priority\": \"NONE\""
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  std::string returned_priority;
  EXPECT_TRUE(cache_manager_->GetPriority(kDeviceId, returned_priority));

  const std::string expected_priority("NONE");
  EXPECT_EQ(expected_priority, returned_priority);
}

TEST_F(CacheManagerTest, GetPriority_ValidAppID_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"priority\": \"NONE\""
      "}"
      "}"
      "}"
      "}");

  *pt_ = CreateCustomPT(string_table);
  std::string returned_priority;
  EXPECT_TRUE(cache_manager_->GetPriority(kValidAppId, returned_priority));

  const std::string expected_priority("NONE");
  EXPECT_EQ(expected_priority, returned_priority);
}

TEST_F(CacheManagerTest, GetPriority_InvalidAppID_ReturnFalse) {
  std::string priority;

  EXPECT_FALSE(cache_manager_->GetPriority(kInvalidApp, priority));
  EXPECT_TRUE(priority.empty());
}

TEST_F(CacheManagerTest, SetDefaultPolicy_DefaultAppExists_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"default\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_TRUE(cache_manager_->SetDefaultPolicy(kInvalidApp));
}

TEST_F(CacheManagerTest, IsDefaultPolicy_DefaultValidApp_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": \"default\""
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_TRUE(cache_manager_->IsDefaultPolicy(kValidAppId));
}

TEST_F(CacheManagerTest, IsDefaultPolicy_InvalidAppId_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->IsDefaultPolicy(kInvalidApp));
}

TEST_F(CacheManagerTest, IsDefaultPolicy_NotDefaultValidAppId_ReturnFalse) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"priority\": [\"NONE\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_FALSE(cache_manager_->IsDefaultPolicy(kValidAppId));
}

TEST_F(CacheManagerTest, SetIsDefault_InvalidAppId_DefaultNotAssigned) {
  EXPECT_TRUE(cache_manager_->SetIsDefault(kInvalidApp));
  EXPECT_FALSE(cache_manager_->IsDefaultPolicy(kInvalidApp));
}

TEST_F(CacheManagerTest, SetIsDefault_ValidAppId_ReturnTrue) {
  EXPECT_TRUE(cache_manager_->SetIsDefault(kValidAppId));
  EXPECT_FALSE(cache_manager_->IsDefaultPolicy(kValidAppId));
}

TEST_F(CacheManagerTest, IsPredataPolicy_InvalidAppId_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->IsPredataPolicy(kInvalidApp));
}

TEST_F(CacheManagerTest, IsPredataPolicy_NotPredataValidAppId_ReturnFalse) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"priority\": [\"NONE\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  EXPECT_FALSE(cache_manager_->IsPredataPolicy(kValidAppId));
}

TEST_F(CacheManagerTest, IsPredataPolicy_PredataValidAppId_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": \"pre_DataConsent\""
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_TRUE(cache_manager_->IsPredataPolicy(kValidAppId));
}

TEST_F(CacheManagerTest, SetPredataPolicy_NoPredataSection_ReturnFalse) {
  pt_->policy_table.app_policies_section.apps.erase(kPreDataConsentId);
  EXPECT_FALSE(cache_manager_->SetPredataPolicy(kValidAppId));
}

TEST_F(CacheManagerTest, SetPredataPolicy_ValidPredataSection_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"pre_DataConsent\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_TRUE(cache_manager_->SetPredataPolicy(kValidAppId));
  EXPECT_TRUE(cache_manager_->IsPredataPolicy(kValidAppId));
}

TEST_F(CacheManagerTest, SetIsPredata_InvalidAppID_PredataNotAssigned) {
  EXPECT_TRUE(cache_manager_->SetIsPredata(kInvalidApp));
  EXPECT_FALSE(cache_manager_->IsPredataPolicy(kInvalidApp));
}

TEST_F(CacheManagerTest, SetIsPredata_ValidAppID_PredataIsAssigned) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_TRUE(cache_manager_->SetIsPredata(kValidAppId));
  EXPECT_TRUE(cache_manager_->IsPredataPolicy(kValidAppId));
}

TEST_F(CacheManagerTest, SetUnpairedDevice_NoSuchDevice_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->SetUnpairedDevice(kDeviceNumber, true));
}

TEST_F(CacheManagerTest, SetUnpairedDevice_UnpairedDevice_ReturnTrue) {
  cache_manager_->AddDevice(kDeviceNumber, "kConnectionType");
  EXPECT_TRUE(cache_manager_->SetUnpairedDevice(kDeviceNumber, true));
}

TEST_F(CacheManagerTest, SetUnpairedDevice_NotUnpairedDevice_ReturnTrue) {
  cache_manager_->AddDevice(kDeviceNumber, "kConnectionType");
  EXPECT_TRUE(cache_manager_->SetUnpairedDevice(kDeviceNumber, false));
}

TEST_F(CacheManagerTest, GetHMIAppTypeAfterUpdate_NoAppTypes_MapIsEmpty) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"AppHMIType\": [\"AHT_DEFAULT\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  std::map<std::string, StringArray> app_hmi_types;

  cache_manager_->GetHMIAppTypeAfterUpdate(app_hmi_types);
  EXPECT_TRUE(!app_hmi_types.empty());
}

TEST_F(CacheManagerTest,
       ReactOnUserDevConsentForApp_DeviceNotAllowed_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": \"default\""
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  EXPECT_TRUE(cache_manager_->ReactOnUserDevConsentForApp(kValidAppId, false));
  EXPECT_TRUE(cache_manager_->IsPredataPolicy(kValidAppId));
}

TEST_F(CacheManagerTest,
       ReactOnUserDevConsentForApp_DeviceAllowedWithPredataPolicy_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": \"pre_DataConsent\","
      "\"default\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  EXPECT_TRUE(cache_manager_->ReactOnUserDevConsentForApp(kValidAppId, true));
  EXPECT_TRUE(cache_manager_->IsDefaultPolicy(kValidAppId));
}

TEST_F(CacheManagerTest,
       ReactOnUserDevConsentForApp_DeviceAllowedAndNoPredataPolicy_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  EXPECT_TRUE(cache_manager_->ReactOnUserDevConsentForApp(kValidAppId, true));
  EXPECT_FALSE(cache_manager_->IsDefaultPolicy(kValidAppId));
}

TEST_F(CacheManagerTest, CountUnconsentedGroups_InvalidAppID_ReturnZero) {
  EXPECT_EQ(0,
            cache_manager_->CountUnconsentedGroups(kInvalidApp, kDeviceNumber));
}

TEST_F(CacheManagerTest,
       CountUnconsentedGroups_ValidAppIDIsPredata_ReturnZero) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": \"pre_DataConsent\""
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_EQ(0,
            cache_manager_->CountUnconsentedGroups(kValidAppId, kDeviceNumber));
}

TEST_F(CacheManagerTest,
       CountUnconsentedGroups_ValidAppIDIsDefault_ReturnZero) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": \"default\""
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_EQ(0,
            cache_manager_->CountUnconsentedGroups(kValidAppId, kDeviceNumber));
}

TEST_F(
    CacheManagerTest,
    CountUnconsentedGroups_AppIDIsDevice_AllGroupsAlreadyConsented_ReturnZero) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"device\": {"
      "\"groups\": [\"Location-1\"],"
      "\"preconsented_groups\": [\"Location-1\"],"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Location-1\": {"
      "\"user_consent_prompt\": \"Location\","
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_EQ(0,
            cache_manager_->CountUnconsentedGroups(kDeviceId, kDeviceNumber));
}

TEST_F(CacheManagerTest,
       CountUnconsentedGroups_ValidAppID_NoSuchDevice_ReturnOneGroup) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"groups\": [\"Location-1\"],"
      "\"preconsented_groups\": [\"Base-4\"],"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Location-1\": {"
      "\"user_consent_prompt\": \"Location\","
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_EQ(1,
            cache_manager_->CountUnconsentedGroups(kValidAppId, kDeviceNumber));
}

TEST_F(CacheManagerTest,
       CountUnconsentedGroups_ValidAppID_NoUserConsentRecords_ReturnTwoGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"groups\": [\"Location-1\", \"Notifications\"],"
      "\"preconsented_groups\": [\"Base-4\"],"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Location-1\": {"
      "\"user_consent_prompt\": \"Location\","
      "},"
      "\"Notifications\": {"
      "\"user_consent_prompt\": \"Notifications\","
      "},"
      "},"
      "\"device_data\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_EQ(2,
            cache_manager_->CountUnconsentedGroups(kValidAppId, kDeviceNumber));
}

TEST_F(
    CacheManagerTest,
    CountUnconsentedGroups_ValidAppID_GroupNotInUserConsentRecords_ReturnOneGroup) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"groups\": [\"Location-1\"],"
      "\"preconsented_groups\": [\"Base-4\"],"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Location-1\": {"
      "\"user_consent_prompt\": \"Location\","
      "}"
      "},"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "\"1234\": {"
      "\"consent_groups\":{"
      "\"Notifications\":{"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_EQ(1,
            cache_manager_->CountUnconsentedGroups(kValidAppId, kDeviceNumber));
}

TEST_F(CacheManagerTest, IsMetaInfoPresent_NoMetaInfo_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->IsMetaInfoPresent());
}

TEST_F(CacheManagerTest, IsMetaInfoPresent_MetaInfoIsPresent_ReturnTrue) {
  const std::string ccpu_version("ccpu_version");
  const std::string country_code("country_code");
  const std::string language("en-us");

  EXPECT_TRUE(
      cache_manager_->SetMetaInfo(ccpu_version, country_code, language));
  EXPECT_TRUE(cache_manager_->IsMetaInfoPresent());
}

TEST_F(CacheManagerTest, IsApplicationRevoked_InvalidAppID_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->IsApplicationRevoked(kInvalidApp));
}

TEST_F(CacheManagerTest,
       IsApplicationRevoked_ValidAppIDNotRevoked_ReturnFalse) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": \"default\""
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_FALSE(cache_manager_->IsApplicationRevoked(kValidAppId));
}

TEST_F(CacheManagerTest, IsApplicationRevoked_ValidAppIDIsRevoked_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": \"default\""
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  pt_->policy_table.app_policies_section.apps[kValidAppId].set_to_null();

  EXPECT_TRUE(cache_manager_->IsApplicationRevoked(kValidAppId));
}

TEST_F(CacheManagerTest, ResetPT_CannotRefreshDB_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->ResetPT(kSdlPreloadedPtJson));
}

TEST_F(CacheManagerTest, ResetPT_InvalidFilename_ReturnFalse) {
  cache_manager_->Init(kSdlPreloadedPtJson, &policy_settings_);
  const std::string invalid_filename("invalid_filename.json");

  EXPECT_FALSE(cache_manager_->ResetPT(invalid_filename));
}

TEST_F(CacheManagerTest, ResetPT_InitializedTableAndValidFilename_ReturnTrue) {
  cache_manager_->Init(kSdlPreloadedPtJson, &policy_settings_);
  EXPECT_TRUE(cache_manager_->ResetPT(kSdlPreloadedPtJson));
}

TEST_F(CacheManagerTest, GetAppRequestTypes_DeviceAppID_NoTypes) {
  std::vector<std::string> request_types;
  cache_manager_->GetAppRequestTypes(kDeviceId, request_types);

  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, GetAppRequestTypes_InvalidAppID_NoTypes) {
  std::vector<std::string> request_types;
  cache_manager_->GetAppRequestTypes(kInvalidApp, request_types);

  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, GetAppRequestTypes_ValidAppID_AddTypesIfPresent) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"RequestType\": [\"OnSystemRequest\", \"AddCommandRequest\"]"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  std::vector<std::string> request_types;

  cache_manager_->GetAppRequestTypes(kValidAppId, request_types);
  EXPECT_EQ(2u, request_types.size());
}

TEST_F(CacheManagerTest, GetInitialAppData_ValidAppID_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"AppHMIType\": [\"AHT_DEFAULT\"],"
      "\"nicknames\": [\"Test app\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  StringArray nicknames;
  StringArray app_hmi_types;

  EXPECT_TRUE(
      cache_manager_->GetInitialAppData(kValidAppId, nicknames, app_hmi_types));
  EXPECT_FALSE(nicknames.empty());
  EXPECT_FALSE(app_hmi_types.empty());
}

TEST_F(CacheManagerTest, GetInitialAppData_InvalidAppId_ReturnTrue) {
  StringArray nicknames;
  StringArray app_hmi_types;

  EXPECT_TRUE(
      cache_manager_->GetInitialAppData(kInvalidApp, nicknames, app_hmi_types));
  EXPECT_TRUE(nicknames.empty());
  EXPECT_TRUE(app_hmi_types.empty());
}

TEST_F(CacheManagerTest, GetAppRequestSubTypes_DeviceAppID_NoTypes) {
  std::vector<std::string> request_types;

  cache_manager_->GetAppRequestSubTypes(kDeviceId, request_types);
  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, GetAppRequestSubTypes_InvalidAppID_NoTypes) {
  std::vector<std::string> request_types;

  cache_manager_->GetAppRequestSubTypes(kInvalidApp, request_types);
  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, GetAppRequestSubTypes_ValidAppID_AddSubtype) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"RequestSubType\": [\"Some_subtype\"]"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  std::vector<std::string> request_subtypes;

  cache_manager_->GetAppRequestSubTypes(kValidAppId, request_subtypes);
  EXPECT_FALSE(request_subtypes.empty());
}

TEST_F(CacheManagerTest, Init_TableAlreadyExistsNotMerged_ReturnFalse) {
  EXPECT_TRUE(cache_manager_->Init(kSdlPreloadedPtJson, &policy_settings_));
  const std::string invalid_filename("invalid_filename.json");

  EXPECT_FALSE(cache_manager_->Init(invalid_filename, &policy_settings_));
}

TEST_F(CacheManagerTest, Init_TableAlreadyExistsAndIsMerged_ReturnTrue) {
  const std::string initial_version_of_pt(
      "json/sdl_preloaded_pt_for_merge_initial.json");
  const std::string latest_version_of_pt(
      "json/sdl_preloaded_pt_for_merge_latest.json");

  EXPECT_TRUE(cache_manager_->Init(initial_version_of_pt, &policy_settings_));
  EXPECT_TRUE(cache_manager_->Init(latest_version_of_pt, &policy_settings_));
}

TEST_F(CacheManagerTest, GetCertificate_NoCertificateReturnEmptyString) {
  std::string certificate = cache_manager_->GetCertificate();
  EXPECT_TRUE(certificate.empty());
}

TEST_F(CacheManagerTest, GetCertificate_CertificateExists_ReturnCertificate) {
  const std::string certificate_to_set("some_certificate");
  cache_manager_->SetDecryptedCertificate(certificate_to_set);

  std::string returned_certificate = cache_manager_->GetCertificate();
  EXPECT_EQ(certificate_to_set, returned_certificate);
}

TEST_F(CacheManagerTest, SetExternalConsentStatus_EmptyStatus_ReturnFalse) {
  EXPECT_FALSE(
      cache_manager_->SetExternalConsentStatus(ExternalConsentStatus()));
}

TEST_F(CacheManagerTest, SetExternalConsentStatus_ValidStatus_ReturnTrue) {
  ExternalConsentStatus consent_status;
  consent_status.insert(ExternalConsentStatusItem());

  EXPECT_TRUE(cache_manager_->Init(kSdlPreloadedPtJson, &policy_settings_));
  EXPECT_TRUE(cache_manager_->SetExternalConsentStatus(consent_status));
}

TEST_F(CacheManagerTest,
       SetUserPermissionsForApp_PermissionsNotChanged_ReturnTrue) {
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
  group_perm.state = GroupConsent::kGroupUndefined;

  std::vector<FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group_perm);
  perm_consent.group_permissions = groups_permissions;

  bool app_permissions_changed;
  EXPECT_TRUE(cache_manager_->SetUserPermissionsForApp(
      perm_consent, &app_permissions_changed));
  EXPECT_FALSE(app_permissions_changed);
}

TEST_F(CacheManagerTest,
       SetUserPermissionsForApp_PermissionsIsChanged_ReturnTrue) {
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

  bool app_permissions_changed;
  EXPECT_TRUE(cache_manager_->SetUserPermissionsForApp(
      perm_consent, &app_permissions_changed));
  EXPECT_TRUE(app_permissions_changed);
}

TEST_F(CacheManagerTest, SetDeviceConsent_ConsentIsSetted_DeviceAllowed) {
  cache_manager_->AddDevice(kDeviceNumber, kConnectionType);
  StringArray consented_groups, disallowed_groups;
  const std::string consented_group("DataConsent-2");
  consented_groups.push_back(consented_group);
  EXPECT_TRUE(cache_manager_->SetUserPermissionsForDevice(
      kDeviceNumber, consented_groups, disallowed_groups));

  cache_manager_->SetDeviceConsent(kDeviceNumber, true);
  EXPECT_EQ(kDeviceAllowed, cache_manager_->GetDeviceConsent(kDeviceNumber));
}

TEST_F(CacheManagerTest,
       GetVehicleDataItems_VehicleDataIsInitialized_GotDataItems) {
  cache_manager_->Init(kSdlPreloadedPtJson, &policy_settings_);
  const auto items = cache_manager_->GetVehicleDataItems();
  EXPECT_FALSE(items.empty());
}

TEST_F(CacheManagerTest,
       GetVehicleDataItems_VehicleDataNotInitialized_ReturnEmptyVector) {
  const auto items = cache_manager_->GetVehicleDataItems();
  EXPECT_TRUE(items.empty());
}

TEST_F(CacheManagerTest, LockScreenDismissalWarningMessage_ReturnValidMessage) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"consumer_friendly_messages\": {"
      "\"messages\": {"
      "\"LockScreenDismissalWarning\": {"
      "\"languages\": {"
      "\"en-us\": {"
      "\"textBody\": \"Swipe down to dismiss, acknowledging that you are not "
      "the driver\""
      "}"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  const std::string expected_msg(
      "Swipe down to dismiss, acknowledging that you are not the driver");
  const std::string language("en-us");

  EXPECT_EQ(expected_msg,
            *cache_manager_->LockScreenDismissalWarningMessage(language));
}

TEST_F(CacheManagerTest,
       LockScreenDismissalEnabledState_EnabledInPT_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"module_config\": {"
      "\"lock_screen_dismissal_enabled\": true"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  EXPECT_TRUE(*cache_manager_->LockScreenDismissalEnabledState());
}

TEST_F(CacheManagerTest,
       UnknownRPCPassthroughAllowed_InvalidAppID_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->UnknownRPCPassthroughAllowed(kInvalidApp));
}

TEST_F(CacheManagerTest, UnknownRPCPassthroughAllowed_ValidAppID_ReturnTrue) {
  pt_->policy_table.app_policies_section.apps[kValidAppId]
      .allow_unknown_rpc_passthrough = rpc::Optional<rpc::Boolean>(true);
  EXPECT_TRUE(cache_manager_->UnknownRPCPassthroughAllowed(kValidAppId));
}

TEST_F(CacheManagerTest, GetAppProperties_InvalidAppID_ReturnFalse) {
  AppProperties out_app_properties;
  EXPECT_FALSE(
      cache_manager_->GetAppProperties(kInvalidApp, out_app_properties));
}

TEST_F(CacheManagerTest, GetAppProperties_ValidAppID_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  AppProperties out_app_properties;
  const std::string endpoint("endpoint");
  const std::string auth_token("auth_token");
  const std::string transport_type("transport_type");
  const std::string hybrid_app_preference("CLOUD");

  cache_manager_->SetAppEndpoint(kValidAppId, endpoint);
  cache_manager_->SetAppAuthToken(kValidAppId, auth_token);
  cache_manager_->SetAppCloudTransportType(kValidAppId, transport_type);
  cache_manager_->SetHybridAppPreference(kValidAppId, hybrid_app_preference);
  cache_manager_->SetCloudAppEnabled(kValidAppId, true);

  EXPECT_TRUE(
      cache_manager_->GetAppProperties(kValidAppId, out_app_properties));
  EXPECT_EQ(endpoint, out_app_properties.endpoint);
  EXPECT_EQ(auth_token, out_app_properties.auth_token);
  EXPECT_EQ(transport_type, out_app_properties.transport_type);
  EXPECT_EQ(hybrid_app_preference, out_app_properties.hybrid_app_preference);
  EXPECT_TRUE(out_app_properties.enabled);
}

TEST_F(CacheManagerTest,
       SetExternalConsentForApp_ConsentsSame_ConsentNotSetted) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "\"1234\": {"
      "\"external_consent_status_groups\":{"
      "\"Notifications\":true"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

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

  policy_table::ConsentGroups initial_consent_groups =
      *(*(*pt_->policy_table.device_data)[kDeviceNumber]
             .user_consent_records)[kValidAppId]
           .external_consent_status_groups;
  cache_manager_->SetExternalConsentForApp(perm_consent);

  policy_table::ConsentGroups updated_consent_groups =
      *(*(*pt_->policy_table.device_data)[kDeviceNumber]
             .user_consent_records)[kValidAppId]
           .external_consent_status_groups;
  EXPECT_EQ(initial_consent_groups, updated_consent_groups);
}

TEST_F(CacheManagerTest,
       SetExternalConsentForApp_ConsentsNewGroup_ConsentIsSetted) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "\"1234\": {"
      "\"external_consent_status_groups\":{"
      "\"Notifications\":true"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  PermissionConsent perm_consent;
  perm_consent.device_id = kDeviceNumber;
  perm_consent.policy_app_id = kValidAppId;
  const std::string source("VR");
  perm_consent.consent_source = source;

  const std::string group_name("Base-4");

  FunctionalGroupPermission group_perm;
  group_perm.group_alias = group_name;
  group_perm.group_name = group_name;
  group_perm.group_id = ::utils::Djb2HashFromString(group_name);
  group_perm.state = GroupConsent::kGroupAllowed;

  std::vector<FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group_perm);
  perm_consent.group_permissions = groups_permissions;

  policy_table::ConsentGroups initial_consent_groups =
      *(*(*pt_->policy_table.device_data)[kDeviceNumber]
             .user_consent_records)[kValidAppId]
           .external_consent_status_groups;
  cache_manager_->SetExternalConsentForApp(perm_consent);

  policy_table::ConsentGroups updated_consent_groups =
      *(*(*pt_->policy_table.device_data)[kDeviceNumber]
             .user_consent_records)[kValidAppId]
           .external_consent_status_groups;
  EXPECT_NE(initial_consent_groups, updated_consent_groups);
}

TEST_F(CacheManagerTest, Add_MinutesAreAdded) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  int result;
  int seconds;

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .minutes_in_hmi_full = 0;
  seconds = 60;
  cache_manager_->Add(kValidAppId, usage_statistics::SECONDS_HMI_FULL, seconds);
  result = static_cast<int>(
      (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
          .minutes_in_hmi_full);
  EXPECT_EQ(1, result);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .minutes_in_hmi_limited = 0;
  seconds = 120;
  cache_manager_->Add(
      kValidAppId, usage_statistics::SECONDS_HMI_LIMITED, seconds);
  result = static_cast<int>(
      (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
          .minutes_in_hmi_limited);
  EXPECT_EQ(2, result);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .minutes_in_hmi_background = 0;
  seconds = 180;
  cache_manager_->Add(
      kValidAppId, usage_statistics::SECONDS_HMI_BACKGROUND, seconds);
  result = static_cast<int>(
      (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
          .minutes_in_hmi_background);
  EXPECT_EQ(3, result);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .minutes_in_hmi_none = 0;
  seconds = 240;
  cache_manager_->Add(kValidAppId, usage_statistics::SECONDS_HMI_NONE, seconds);
  result = static_cast<int>(
      (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
          .minutes_in_hmi_none);
  EXPECT_EQ(4, result);
}

TEST_F(CacheManagerTest, Set_ValuesAreSetted) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  const std::string language("eng-eng");

  cache_manager_->Set(kValidAppId, usage_statistics::LANGUAGE_GUI, language);
  EXPECT_EQ(
      language,
      std::string(
          (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
              .app_registration_language_gui));

  cache_manager_->Set(kValidAppId, usage_statistics::LANGUAGE_VUI, language);
  EXPECT_EQ(
      language,
      std::string(
          (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
              .app_registration_language_vui));
}

TEST_F(CacheManagerTest, Increment_GlobalCounterIsIncremented) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  *pt_->policy_table.usage_and_error_counts->count_of_iap_buffer_full = 0;
  cache_manager_->Increment(usage_statistics::IAP_BUFFER_FULL);
  EXPECT_EQ(
      1, *pt_->policy_table.usage_and_error_counts->count_of_iap_buffer_full);

  *pt_->policy_table.usage_and_error_counts->count_sync_out_of_memory = 0;
  cache_manager_->Increment(usage_statistics::SYNC_OUT_OF_MEMORY);
  EXPECT_EQ(
      1, *pt_->policy_table.usage_and_error_counts->count_sync_out_of_memory);

  *pt_->policy_table.usage_and_error_counts->count_of_sync_reboots = 0;
  cache_manager_->Increment(usage_statistics::SYNC_REBOOTS);
  EXPECT_EQ(1,
            *pt_->policy_table.usage_and_error_counts->count_of_sync_reboots);
}

TEST_F(CacheManagerTest, Increment_AppCounterIsIncremented) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .count_of_user_selections = 0;
  cache_manager_->Increment(kValidAppId, usage_statistics::USER_SELECTIONS);
  EXPECT_EQ(1,
            (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
                .count_of_user_selections);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .count_of_rejections_sync_out_of_memory = 0;
  cache_manager_->Increment(kValidAppId,
                            usage_statistics::REJECTIONS_SYNC_OUT_OF_MEMORY);
  EXPECT_EQ(1,
            (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
                .count_of_rejections_sync_out_of_memory);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .count_of_rejections_nickname_mismatch = 0;
  cache_manager_->Increment(kValidAppId,
                            usage_statistics::REJECTIONS_NICKNAME_MISMATCH);
  EXPECT_EQ(1,
            (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
                .count_of_rejections_nickname_mismatch);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .count_of_rejections_duplicate_name = 0;
  cache_manager_->Increment(kValidAppId,
                            usage_statistics::REJECTIONS_DUPLICATE_NAME);
  EXPECT_EQ(1,
            (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
                .count_of_rejections_duplicate_name);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .count_of_rejected_rpc_calls = 0;
  cache_manager_->Increment(kValidAppId, usage_statistics::REJECTED_RPC_CALLS);
  EXPECT_EQ(1,
            (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
                .count_of_rejected_rpc_calls);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .count_of_rpcs_sent_in_hmi_none = 0;
  cache_manager_->Increment(kValidAppId, usage_statistics::RPCS_IN_HMI_NONE);
  EXPECT_EQ(1,
            (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
                .count_of_rpcs_sent_in_hmi_none);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .count_of_removals_for_bad_behavior = 0;
  cache_manager_->Increment(kValidAppId, usage_statistics::REMOVALS_MISBEHAVED);
  EXPECT_EQ(1,
            (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
                .count_of_removals_for_bad_behavior);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .count_of_run_attempts_while_revoked = 0;
  cache_manager_->Increment(kValidAppId,
                            usage_statistics::RUN_ATTEMPTS_WHILE_REVOKED);
  EXPECT_EQ(1,
            (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
                .count_of_run_attempts_while_revoked);

  (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
      .count_of_tls_errors = 0;
  cache_manager_->Increment(kValidAppId, usage_statistics::COUNT_OF_TLS_ERRORS);
  EXPECT_EQ(1,
            (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
                .count_of_tls_errors);
}

TEST_F(CacheManagerTest, GetUnconsentedGroups_InvalidAppId_ReturnNoGroups) {
  FunctionalGroupIDs group_ids;
  cache_manager_->GetUnconsentedGroups(kDeviceNumber, kInvalidApp, group_ids);
  EXPECT_TRUE(group_ids.empty());
}

TEST_F(CacheManagerTest,
       GetUnconsentedGroups_DeviceApp_NoSuchFunctionalGroup_NoGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"device\": {"
      "\"groups\": [\"Location-1\"],"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Base-4\": {"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetUnconsentedGroups(kDeviceNumber, kDeviceId, group_ids);
  EXPECT_TRUE(group_ids.empty());
}

TEST_F(CacheManagerTest,
       GetUnconsentedGroups_ValidApp_NoUserConsentsForGroup_NoGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"groups\": [\"Location-1\"]"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Location-1\": {"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetUnconsentedGroups(kDeviceNumber, kValidAppId, group_ids);
  EXPECT_TRUE(group_ids.empty());
}

TEST_F(CacheManagerTest,
       GetUnconsentedGroups_ValidAppId_NoSuchDevice_NoGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"groups\": [\"Location-1\"]"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Location-1\": {"
      "\"user_consent_prompt\": \"Location\""
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetUnconsentedGroups(kDeviceNumber, kValidAppId, group_ids);
  EXPECT_TRUE(group_ids.empty());
}

TEST_F(
    CacheManagerTest,
    GetUnconsentedGroups_ValidAppId_NoSuchAppInUserConsentRecords_ReturnGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"groups\": [\"Location-1\"],"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Location-1\": {"
      "\"user_consent_prompt\": \"Location\","
      "}"
      "},"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetUnconsentedGroups(kDeviceNumber, kValidAppId, group_ids);
  EXPECT_FALSE(group_ids.empty());
}

TEST_F(
    CacheManagerTest,
    GetUnconsentedGroups_ValidAppId_UserConsentPromptInitialized_ReturnGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"groups\": [\"Location-1\"],"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Location-1\": {"
      "\"user_consent_prompt\": \"Location\","
      "}"
      "},"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "\"1234\": {"
      "\"consent_groups\":{"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetUnconsentedGroups(kDeviceNumber, kValidAppId, group_ids);
  EXPECT_FALSE(group_ids.empty());
}

TEST_F(CacheManagerTest, GetConsentedGroups_NoSuchDevice_ReturnNoGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"device_data\": {"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs allowed_groups;
  FunctionalGroupIDs disallowed_groups;

  cache_manager_->GetConsentedGroups(
      kDeviceNumber, kValidAppId, allowed_groups, disallowed_groups);
  EXPECT_TRUE(allowed_groups.empty());
  EXPECT_TRUE(disallowed_groups.empty());
}

TEST_F(CacheManagerTest, GetConsentedGroups_NoAppForDevice_ReturnNoGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs allowed_groups;
  FunctionalGroupIDs disallowed_groups;

  cache_manager_->GetConsentedGroups(
      kDeviceNumber, kValidAppId, allowed_groups, disallowed_groups);
  EXPECT_TRUE(allowed_groups.empty());
  EXPECT_TRUE(disallowed_groups.empty());
}

TEST_F(CacheManagerTest, GetConsentedGroups_GroupsExist_ReturnGroups) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "\"1234\": {"
      "\"consent_groups\":{"
      "\"Location-1\": true,"
      "\"Base-4\": false,"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  FunctionalGroupIDs allowed_groups;
  FunctionalGroupIDs disallowed_groups;

  cache_manager_->GetConsentedGroups(
      kDeviceNumber, kValidAppId, allowed_groups, disallowed_groups);
  EXPECT_FALSE(allowed_groups.empty());
  EXPECT_FALSE(disallowed_groups.empty());
}

TEST_F(CacheManagerTest, RemoveAppConsentForGroup_GroupIsRemoved) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"1234\": {"
      "\"groups\": [\"Location-1\"]"
      "}"
      "},"
      "\"functional_groupings\": {"
      "\"Location-1\": {"
      "\"user_consent_prompt\": \"Location\""
      "}"
      "},"
      "\"device_data\": {"
      "\"XXX123456789ZZZ\": {"
      "\"user_consent_records\": {"
      "\"1234\": {"
      "\"consent_groups\":{"
      "\"Location-1\": true"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);

  FunctionalGroupIDs allowed_groups_before_removal,
      disallowed_groups_before_removal;
  cache_manager_->GetConsentedGroups(kDeviceNumber,
                                     kValidAppId,
                                     allowed_groups_before_removal,
                                     disallowed_groups_before_removal);
  EXPECT_FALSE(allowed_groups_before_removal.empty());

  FunctionalGroupIDs unconsented_groups_before_removal;
  cache_manager_->GetUnconsentedGroups(
      kDeviceNumber, kValidAppId, unconsented_groups_before_removal);
  EXPECT_TRUE(unconsented_groups_before_removal.empty());

  const std::string group_to_remove("Location-1");
  cache_manager_->RemoveAppConsentForGroup(kValidAppId, group_to_remove);

  FunctionalGroupIDs allowed_groups_after_removal,
      disallowed_groups_after_removal;
  cache_manager_->GetConsentedGroups(kDeviceNumber,
                                     kValidAppId,
                                     allowed_groups_after_removal,
                                     disallowed_groups_after_removal);
  EXPECT_TRUE(allowed_groups_after_removal.empty());

  FunctionalGroupIDs unconsented_groups_after_removal;
  cache_manager_->GetUnconsentedGroups(
      kDeviceNumber, kValidAppId, unconsented_groups_after_removal);
  EXPECT_FALSE(unconsented_groups_after_removal.empty());
}

}  // namespace policy_test
}  // namespace components
}  // namespace test

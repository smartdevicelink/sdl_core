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
namespace policy_table = rpc::policy_table_interface_base;

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
  CacheManagerTest() : cache_manager_(nullptr), pt_(nullptr) {}

 protected:
  std::shared_ptr<CacheManager> cache_manager_;
  std::shared_ptr<policy_table::Table> pt_;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;

  void SetUp() {
    cache_manager_ = std::make_shared<CacheManager>();
    pt_ = cache_manager_->pt();

    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
  }
};

TEST_F(CacheManagerTest,
       GetNotificationsNumber_PriorityExists_ReturnNumberFromPT) {
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

  std::string priority = "OTHER_PRIORITY";
  uint32_t notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(0u, notif_number);
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
  policy_table::AppHMITypes hmi_types;
  hmi_types.push_back(policy_table::AHT_DEFAULT);

  *pt_->policy_table.app_policies_section.apps[kValidAppId].AppHMIType =
      hmi_types;
  EXPECT_NE(nullptr, cache_manager_->GetHMITypes(kValidAppId));
}

TEST_F(CacheManagerTest, GetAllAppGroups_AppIdIsDevice_AppendGroupId) {
  FunctionalGroupIDs group_ids;

  cache_manager_->GetAllAppGroups(kDeviceId, group_ids);
  EXPECT_TRUE(group_ids.empty());
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

TEST_F(CacheManagerTest, CheckPermissions_ValidParams_ReturnkRpcAllowed) {
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
  policy_table::Strings groups;
  groups.push_back("Base-4");
  const PTString hmi_level("FULL");
  const PTString rpc("AddCommand");
  CheckPermissionResult result;

  cache_manager_->CheckPermissions(groups, hmi_level, rpc, result);
  EXPECT_EQ(kRpcAllowed, result.hmi_level_permitted);
}

TEST_F(CacheManagerTest, CheckPermissions_ValidParams_ReturnkRpcDisallowed) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"functional_groupings\": {"
      "\"Base-4\": {"
      "\"rpcs\": {"
      "\"AddCommand\": {"
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
  policy_table::Strings groups;
  groups.push_back("Base-4");
  const PTString hmi_level("FULL");
  const PTString rpc("AddCommand");
  CheckPermissionResult result;

  cache_manager_->CheckPermissions(groups, hmi_level, rpc, result);
  EXPECT_EQ(kRpcDisallowed, result.hmi_level_permitted);
}

TEST_F(CacheManagerTest,
       CheckPermissions_NoSuchFunctionalGroup_ReturnkRpcDisallowed) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"functional_groupings\": {"
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
  policy_table::Strings groups;
  groups.push_back("Base-4");
  const PTString hmi_level("FULL");
  const PTString rpc("AddCommand");
  CheckPermissionResult result;

  cache_manager_->CheckPermissions(groups, hmi_level, rpc, result);
  EXPECT_EQ(kRpcDisallowed, result.hmi_level_permitted);
}

TEST_F(CacheManagerTest, CheckPermissions_NoSuchRpc_ReturnkRpcDisallowed) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"functional_groupings\": {"
      "\"Base-4\": {"
      "\"rpcs\": {"
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
  policy_table::Strings groups;
  groups.push_back("Base-4");
  const PTString hmi_level("FULL");
  const PTString rpc("AddCommand");
  CheckPermissionResult result;

  cache_manager_->CheckPermissions(groups, hmi_level, rpc, result);
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
  // if result of computation is positive
  pt_->policy_table.module_config.exchange_after_x_ignition_cycles = 100u;
  *(pt_->policy_table.module_meta->ignition_cycles_since_last_exchange) = 80u;

  EXPECT_EQ(20, cache_manager_->IgnitionCyclesBeforeExchange());

  // if result of computation is negative
  pt_->policy_table.module_config.exchange_after_x_ignition_cycles = 80u;
  *(pt_->policy_table.module_meta->ignition_cycles_since_last_exchange) = 100u;

  EXPECT_EQ(0, cache_manager_->IgnitionCyclesBeforeExchange());
}

TEST_F(CacheManagerTest, KilometersBeforeExchange_GetValue) {
  // if result of computation is positive
  pt_->policy_table.module_config.exchange_after_x_kilometers = 1000u;
  *(pt_->policy_table.module_meta->pt_exchanged_at_odometer_x) = 500u;

  EXPECT_EQ(600, cache_manager_->KilometersBeforeExchange(900));

  // if result of computation is negative
  pt_->policy_table.module_config.exchange_after_x_kilometers = 500u;
  *(pt_->policy_table.module_meta->pt_exchanged_at_odometer_x) = 200u;

  EXPECT_EQ(0, cache_manager_->KilometersBeforeExchange(800));
}

TEST_F(CacheManagerTest,
       DaysBeforeExchange_DaysNotInitialized_ReturnNegativeOne) {
  EXPECT_EQ(-1, cache_manager_->DaysBeforeExchange(20));
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

TEST_F(CacheManagerTest, GetPriority_DeviceAppID_ReturnTrue) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"app_policies\": {"
      "\"device\": {"
      "\"priority\": [\"NONE\"],"
      "}"
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  std::string priority;
  EXPECT_TRUE(cache_manager_->GetPriority(kDeviceId, priority));
}

TEST_F(CacheManagerTest, GetPriority_ValidAppID_ReturnTrue) {
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
  std::string priority;
  EXPECT_TRUE(cache_manager_->GetPriority(kValidAppId, priority));
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
}

TEST_F(CacheManagerTest, SetUnpairedDevice_NoSuchDevice_ReturnFalse) {
  EXPECT_FALSE(cache_manager_->SetUnpairedDevice(kDeviceNumber, true));
}

TEST_F(CacheManagerTest, SetUnpairedDevice_UnpairedDevice_ReturnTrue) {
  cache_manager_->AddDevice(kDeviceNumber, kConnectionType);
  EXPECT_TRUE(cache_manager_->SetUnpairedDevice(kDeviceNumber, true));
}

TEST_F(CacheManagerTest, SetUnpairedDevice_NotUnpairedDevice_ReturnTrue) {
  cache_manager_->AddDevice(kDeviceNumber, kConnectionType);
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
  file_system::CreateDirectory(kAppStorageFolder);
  EXPECT_TRUE(cache_manager_->Init(kSdlPreloadedPtJson, &policy_settings_));
  const std::string invalid_filename = "invalid_filename.json";
  EXPECT_FALSE(cache_manager_->Init(invalid_filename, &policy_settings_));
  file_system::RemoveDirectory(kAppStorageFolder, true);
}

TEST_F(CacheManagerTest, Init_TableAlreadyExistsAndIsMerged_ReturnTrue) {
  file_system::CreateDirectory(kAppStorageFolder);
  const std::string earlier_version_of_preloaded_pt =
      "json/sdl_preloaded_pt_for_merge_initial.json";
  const std::string latest_version_of_preloaded_pt =
      "json/sdl_preloaded_pt_for_merge_latest.json";

  EXPECT_TRUE(
      cache_manager_->Init(earlier_version_of_preloaded_pt, &policy_settings_));
  EXPECT_TRUE(
      cache_manager_->Init(latest_version_of_preloaded_pt, &policy_settings_));
  file_system::RemoveDirectory(kAppStorageFolder, true);
}

TEST_F(CacheManagerTest, GetCertificate_NoCertificateReturnEmptyString) {
  std::string certificate = cache_manager_->GetCertificate();
  EXPECT_TRUE(certificate.empty());
}

TEST_F(CacheManagerTest, GetCertificate_CertificateExists_ReturnCertificate) {
  const std::string string_table(
      "{"
      "\"policy_table\": {"
      "\"module_config\": {"
      "\"certificate\": \"some_certificate\""
      "}"
      "}"
      "}");
  *pt_ = CreateCustomPT(string_table);
  std::string certificate = cache_manager_->GetCertificate();
  EXPECT_FALSE(certificate.empty());
}

TEST_F(CacheManagerTest,
       GetVehicleDataItems_VehicleDataIsInitialized_GotDataItems) {
  file_system::CreateDirectory(kAppStorageFolder);
  cache_manager_->Init(kSdlPreloadedPtJson, &policy_settings_);
  auto items = cache_manager_->GetVehicleDataItems();
  EXPECT_FALSE(items.empty());
  file_system::RemoveDirectory(kAppStorageFolder, true);
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

  std::string expected_msg(
      "Swipe down to dismiss, acknowledging that you are not the driver");
  EXPECT_EQ(expected_msg,
            *cache_manager_->LockScreenDismissalWarningMessage("en-us"));
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
  cache_manager_->SetAppEndpoint(kValidAppId, "endpoint");
  cache_manager_->SetAppAuthToken(kValidAppId, "auth_token");
  cache_manager_->SetAppCloudTransportType(kValidAppId, "transport_type");
  cache_manager_->SetHybridAppPreference(kValidAppId, "CLOUD");
  cache_manager_->SetCloudAppEnabled(kValidAppId, true);

  EXPECT_TRUE(
      cache_manager_->GetAppProperties(kValidAppId, out_app_properties));
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
  int result, seconds;

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

  cache_manager_->Set(kValidAppId, usage_statistics::LANGUAGE_GUI, "eng-eng");
  EXPECT_EQ(
      "eng-eng",
      std::string(
          (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
              .app_registration_language_gui));

  cache_manager_->Set(kValidAppId, usage_statistics::LANGUAGE_VUI, "ru-ru");
  EXPECT_EQ(
      "ru-ru",
      std::string(
          (*pt_->policy_table.usage_and_error_counts->app_level)[kValidAppId]
              .app_registration_language_vui));
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

TEST_F(CacheManagerTest, IsApplicationRepresented_DeviceApp_ReturnTrue) {
  EXPECT_TRUE(cache_manager_->IsApplicationRepresented(kDeviceId));
}

}  // namespace policy_test
}  // namespace components
}  // namespace test

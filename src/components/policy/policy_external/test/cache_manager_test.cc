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
#include "policy/policy_table/types.h"

#include "policy/mock_policy_settings.h"

#include "interfaces/MOBILE_API.h"
#include "json/reader.h"
#include "utils/date_time.h"
#include "utils/file_system.h"
#include "utils/gen_hash.h"
#include "utils/jsoncpp_reader_wrapper.h"

namespace mobile_api = mobile_apis;
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
const std::string kInvalidAppId = "Invalid_App";
const std::string kDeviceAppId = "device";
const std::string kDefaultAppId = "default";
const std::string kPreDataConsent = "pre_DataConsent";
const std::string kSdlPreloadedPtJson = "json/sdl_preloaded_pt.json";
}  // namespace

Json::Value createPTforLoad() {
  const std::string load_table(
      "{"
      "\"policy_table\": {"
      "\"module_config\": {"
      "\"exchange_after_x_ignition_cycles\": 10,"
      "\"exchange_after_x_kilometers\": 100,"
      "\"exchange_after_x_days\": 5,"
      "\"timeout_after_x_seconds\": 500,"
      "\"seconds_between_retries\": [10, 20, 30],"
      "\"endpoints\": {"
      "\"0x00\": {"
      "\"default\": [\"http://ford.com/cloud/default\"]"
      "}"
      "},"
      "\"notifications_per_minute_by_priority\": {"
      "\"EMERGENCY\": 1,"
      "\"NAVIGATION\": 2,"
      "\"VOICECOM\": 3,"
      "\"COMMUNICATION\": 4,"
      "\"NORMAL\": 5,"
      "\"NONE\": 6"
      "},"
      "\"vehicle_make\" : \"MakeT\","
      "\"vehicle_model\" : \"ModelT\","
      "\"vehicle_year\": \"2014\""
      "},"
      "\"app_policies\": {"
      "\"default\": {"
      "\"memory_kb\": 50,"
      "\"heart_beat_timeout_ms\": 100,"
      "\"groups\": [\"default\"],"
      "\"keep_context\": true,"
      "\"steal_focus\": true,"
      "\"priority\": \"EMERGENCY\","
      "\"default_hmi\": \"FULL\","
      "\"certificate\": \"sign\""
      "},   "
      "\"pre_DataConsent\": {"
      "\"memory_kb\": 50,"
      "\"heart_beat_timeout_ms\": 100,"
      "\"groups\": [\"default\"],"
      "\"keep_context\": true,"
      "\"steal_focus\": true,"
      "\"priority\": \"EMERGENCY\","
      "\"default_hmi\": \"FULL\","
      "\"certificate\": \"sign\""
      "},         "
      "\"device\": {"
      "\"memory_kb\": 50,"
      "\"heart_beat_timeout_ms\": 100,"
      "\"groups\": [\"default\"],"
      "\"keep_context\": true,"
      "\"steal_focus\": true,"
      "\"priority\": \"EMERGENCY\","
      "\"default_hmi\": \"FULL\","
      "\"certificate\": \"sign\""
      "},"
      "\"1234\": {"
      "\"memory_kb\": 50,"
      "\"heart_beat_timeout_ms\": 100,"
      "\"groups\": [\"default\"],"
      "\"keep_context\": true,"
      "\"steal_focus\": true,"
      "\"priority\": \"EMERGENCY\","
      "\"default_hmi\": \"FULL\","
      "\"certificate\": \"sign\""
      "}"
      "},"
      "\"consumer_friendly_messages\": {"
      "\"version\": \"1.2\""
      "},"
      "\"functional_groupings\": {"
      "\"default\": {"
      "\"rpcs\": {"
      "\"Update\": {"
      "\"hmi_levels\": [\"FULL\"],"
      "\"parameters\" : [\"speed\"]"
      "}"
      "}"
      "}"
      "}"
      "}"
      "}");

  Json::Value table(Json::objectValue);
  utils::JsonReader reader;
  EXPECT_TRUE(reader.parse(load_table, &table));
  return table;
}

bool UpdatePT(CacheManager* cache_manager) {
  Json::Value table = createPTforLoad();
  policy_table::Table update(&table);
  update.SetPolicyTableType(PT_UPDATE);
  return cache_manager->ApplyUpdate(update);
}

class CacheManagerTest : public ::testing::Test {
 public:
  CacheManagerTest()
      : app_id_("1234")
      , device_id_("XXX123456789ZZZ")
      , app_storage_folder_("app_storage_folder")
      , preloaded_pt_filename_(kSdlPreloadedPtJson)
      , in_memory_(true)
      , cache_manager_(nullptr) {}

 protected:
  const std::string app_id_;
  const std::string device_id_;
  const std::string app_storage_folder_;
  const std::string preloaded_pt_filename_;
  const bool in_memory_;

  CacheManager* cache_manager_;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;

  void SetUp() {
    file_system::CreateDirectory(app_storage_folder_);
    cache_manager_ = new CacheManager(in_memory_);
    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(app_storage_folder_));
  }

  void TearDown() {
    delete cache_manager_;
    file_system::RemoveDirectory(app_storage_folder_, true);
  }
};

TEST_F(CacheManagerTest, GetNotificationsNumber_AfterPTUpdate) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  EXPECT_TRUE(cache_manager_->IsPTPreloaded());

  UpdatePT(cache_manager_);

  EXPECT_FALSE(cache_manager_->IsPTPreloaded());

  std::string priority = "EMERGENCY";
  uint32_t notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(1u, notif_number);

  priority = "NAVIGATION";
  notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(2u, notif_number);

  priority = "EMERGENCY";
  notif_number = cache_manager_->GetNotificationsNumber(priority);
  EXPECT_EQ(1u, notif_number);

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

TEST_F(CacheManagerTest,
       GetConsentsPriority_DeviceIDNotFound_ReturnkExternalConsentPrio) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  auto priority = cache_manager_->GetConsentsPriority(device_id_, app_id_);

  EXPECT_EQ(ConsentPriorityType::kExternalConsentPrio, priority);
}

TEST_F(CacheManagerTest, GetGroups_ReturnDataFromPT) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  policy_table::Strings groups = cache_manager_->GetGroups(app_id_);

  EXPECT_EQ(1u, groups.size());
  EXPECT_EQ(kDefaultAppId, std::string(groups[0]));
}

TEST_F(CacheManagerTest, GetPolicyAppIDs_ReturnDataFromPT) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  policy_table::Strings app_ids = cache_manager_->GetPolicyAppIDs();

  EXPECT_EQ(2u, app_ids.size());
  EXPECT_EQ(kDefaultAppId, std::string(app_ids[0]));
  EXPECT_EQ(kPreDataConsent, std::string(app_ids[1]));
}

TEST_F(CacheManagerTest, CanAppKeepContext_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->CanAppKeepContext(kDefaultAppId));
  EXPECT_FALSE(cache_manager_->CanAppKeepContext(kPreDataConsent));
}

TEST_F(CacheManagerTest, CanAppKeepContext_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->CanAppKeepContext(kDefaultAppId));
  EXPECT_TRUE(cache_manager_->CanAppKeepContext(kPreDataConsent));
}

TEST_F(CacheManagerTest, HeartBeatTimeout_ReturnValue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_EQ(cache_manager_->HeartBeatTimeout(kDefaultAppId), 100u);
  EXPECT_EQ(cache_manager_->HeartBeatTimeout(kPreDataConsent), 100u);
  EXPECT_EQ(cache_manager_->HeartBeatTimeout(app_id_), 100u);
}

TEST_F(CacheManagerTest, HeartBeatTimeout_ReturnValueFromPT) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_EQ(cache_manager_->HeartBeatTimeout(kDefaultAppId), 100u);
  EXPECT_EQ(cache_manager_->HeartBeatTimeout(kPreDataConsent), 100u);
  EXPECT_EQ(cache_manager_->HeartBeatTimeout(app_id_), 100u);
}

TEST_F(CacheManagerTest, HeartBeatTimeout_AppNotRepresented_ReturnZero) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_EQ(cache_manager_->HeartBeatTimeout(""), 0u);
  EXPECT_EQ(cache_manager_->HeartBeatTimeout(kInvalidAppId), 0u);
}

TEST_F(CacheManagerTest, GetHMITypes_NoHMITypes_ReturnNull) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_EQ(cache_manager_->GetHMITypes(app_id_), nullptr);
}

TEST_F(CacheManagerTest, GenerateHash_EmptyString_ReturnInitialValue) {
  EXPECT_EQ(cache_manager_->GenerateHash(""), 5381);
}

TEST_F(CacheManagerTest, CanAppStealFocus_AppIdIsDevice_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->CanAppStealFocus(kDeviceAppId));
}

TEST_F(CacheManagerTest, CanAppStealFocus_AppNotRepresented_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->CanAppStealFocus(kInvalidAppId));
}

TEST_F(CacheManagerTest, CanAppStealFocus_ValidApp_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->CanAppStealFocus(app_id_));
}

TEST_F(CacheManagerTest, GetDefaultHMI_AppIdIsDevice_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(cache_manager_->GetDefaultHMI(
      kDeviceAppId, const_cast<std::string&>(kDefaultHMILevel)));
}

TEST_F(CacheManagerTest, GetDefaultHMI_ValidApp_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->GetDefaultHMI(
      app_id_, const_cast<std::string&>(kDefaultHMILevel)));
}

TEST_F(CacheManagerTest, GetDefaultHMI_InvalidApp_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_FALSE(cache_manager_->GetDefaultHMI(
      kInvalidAppId, const_cast<std::string&>(kDefaultHMILevel)));
}

TEST_F(CacheManagerTest, GetAllAppGroups_AppIdIsDevice_AppendGroupId) {
  FunctionalGroupIDs group_ids;
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  cache_manager_->GetAllAppGroups(kDeviceAppId, group_ids);

  EXPECT_EQ(group_ids.size(), 1u);
}

TEST_F(CacheManagerTest, GetAllAppGroups_InvalidAppId_AppendGroupId) {
  FunctionalGroupIDs group_ids;
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  cache_manager_->GetAllAppGroups(kInvalidAppId, group_ids);

  EXPECT_TRUE(group_ids.empty());
}

TEST_F(CacheManagerTest, GetAllAppGroups_ValidAppId_EmptyVectorOfGroups) {
  FunctionalGroupIDs group_ids;
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  cache_manager_->GetAllAppGroups(app_id_, group_ids);

  EXPECT_EQ(group_ids.size(), 1u);
}

TEST_F(CacheManagerTest, ApplyUpdate_ValidPT_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  policy_table::Table new_table_for_update;
  cache_manager_->LoadFromFile(preloaded_pt_filename_, new_table_for_update);

  EXPECT_TRUE(cache_manager_->ApplyUpdate(new_table_for_update));
  EXPECT_FALSE(cache_manager_->IsPTPreloaded());
}

TEST_F(CacheManagerTest, AppHasHMIType_NoSuchType_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(
      cache_manager_->AppHasHMIType(kDefaultAppId, policy_table::AHT_DEFAULT));
}

TEST_F(CacheManagerTest, AppHasHMIType_NoSuchApp_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(
      cache_manager_->AppHasHMIType(kInvalidAppId, policy_table::AHT_DEFAULT));
}

TEST_F(CacheManagerTest, AddDevice_NoSuchApp_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(cache_manager_->IsPTPreloaded());
  EXPECT_TRUE(cache_manager_->AddDevice(device_id_, "Bluetooth"));
  EXPECT_FALSE(cache_manager_->IsPTPreloaded());
}

TEST_F(CacheManagerTest, AddDevice_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(cache_manager_->IsPTPreloaded());
  EXPECT_TRUE(cache_manager_->AddDevice(device_id_, "Bluetooth"));
  EXPECT_FALSE(cache_manager_->IsPTPreloaded());
}

TEST_F(CacheManagerTest, SetDeviceData_NoSuchDevice_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->SetDeviceData(device_id_));
}

TEST_F(CacheManagerTest, SetDeviceData_ValidDevice_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(cache_manager_->AddDevice(device_id_, "Bluetooth"));
  EXPECT_TRUE(cache_manager_->SetDeviceData(device_id_));
}

TEST_F(CacheManagerTest, GetPermissionsForApp_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  FunctionalIdType group_types;

  EXPECT_TRUE(
      cache_manager_->GetPermissionsForApp(device_id_, app_id_, group_types));
}

TEST_F(CacheManagerTest, ResetUserConsent_ConsentIsResetted_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");
  auto pt = cache_manager_->GetPT();
  policy_table::DeviceParams& params =
      (*(pt->policy_table.device_data))[device_id_];
  policy_table::UserConsentRecords user_records;
  policy_table::ConsentRecords record;
  user_records[device_id_] = record;
  *params.user_consent_records = user_records;

  EXPECT_FALSE((*params.user_consent_records).empty());
  EXPECT_TRUE(cache_manager_->ResetUserConsent());
  EXPECT_TRUE((*params.user_consent_records).empty());
}

TEST_F(CacheManagerTest, SetUserPermissionsForDevice_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");

  StringArray consented_groups;
  StringArray disallowed_groups;
  consented_groups.push_back(std::string("DataConsent-2"));
  disallowed_groups.push_back(std::string("Navigation-1"));

  EXPECT_TRUE(cache_manager_->SetUserPermissionsForDevice(
      device_id_, consented_groups, disallowed_groups));
  EXPECT_TRUE(cache_manager_->GetUserPermissionsForDevice(
      device_id_, consented_groups, disallowed_groups));
}

TEST_F(CacheManagerTest, GetUserPermissionsForDevice_NoSuchDevice_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  StringArray consented_groups;
  StringArray disallowed_groups;
  consented_groups.push_back(std::string("DataConsent-2"));
  disallowed_groups.push_back(std::string("Navigation-1"));

  EXPECT_FALSE(cache_manager_->GetUserPermissionsForDevice(
      device_id_, consented_groups, disallowed_groups));
}

TEST_F(CacheManagerTest, GetUserPermissionsForDevice_ValidDevice_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");

  StringArray consented_groups;
  StringArray disallowed_groups;
  consented_groups.push_back(std::string("DataConsent-2"));
  disallowed_groups.push_back(std::string("Navigation-1"));

  EXPECT_TRUE(cache_manager_->GetUserPermissionsForDevice(
      device_id_, consented_groups, disallowed_groups));
}

TEST_F(CacheManagerTest, GetPreConsentedGroups_DeviceAppId_GetGroups) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  FunctionalGroupIDs group_ids;

  auto pt = cache_manager_->GetPT();
  policy_table::DevicePolicy& device =
      pt->policy_table.app_policies_section.device;
  rpc::String<0, 255> group_name("group_1");
  (*device.preconsented_groups).push_back(group_name);
  int32_t group_hash = cache_manager_->GenerateHash("group_1");

  cache_manager_->GetPreConsentedGroups(kDeviceAppId, group_ids);
  EXPECT_EQ(group_ids.size(), 1u);
  EXPECT_EQ(group_ids[0], group_hash);
}

TEST_F(CacheManagerTest, GetPreConsentedGroups_NoSuchAppId_NoGroups) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetPreConsentedGroups(kInvalidAppId, group_ids);
  EXPECT_EQ(group_ids.size(), 0u);
}

TEST_F(CacheManagerTest, GetPreConsentedGroups_ValidAppId_GetGroups) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  FunctionalGroupIDs group_ids;

  auto pt = cache_manager_->GetPT();
  Strings preconsented_groups;
  preconsented_groups.push_back(rpc::String<0, 255>("group_1"));
  *pt->policy_table.app_policies_section.apps[app_id_].preconsented_groups =
      preconsented_groups;
  int32_t group_hash = cache_manager_->GenerateHash("group_1");

  cache_manager_->GetPreConsentedGroups(app_id_, group_ids);
  EXPECT_EQ(group_ids.size(), 1u);
  EXPECT_EQ(group_ids[0], group_hash);
}

TEST_F(CacheManagerTest, CheckPermissions_ValidParams_ReturnValidPermission) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  CheckPermissionResult result;

  cache_manager_->CheckPermissions(kDefaultAppId, "FULL", "AddCommand", result);

  EXPECT_NE(result.hmi_level_permitted, kRpcDisallowed);
}

TEST_F(CacheManagerTest,
       CheckPermissions_InvalidParams_ReturnInvalidPermission) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  CheckPermissionResult result;

  cache_manager_->CheckPermissions(kInvalidAppId, "FULL", "AddCommand", result);

  EXPECT_EQ(result.hmi_level_permitted, kRpcDisallowed);
}

TEST_F(CacheManagerTest, GetAppRequestTypesState_GetState) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_EQ(cache_manager_->GetAppRequestTypesState(kDefaultAppId),
            RequestType::State::EMPTY);
  EXPECT_EQ(cache_manager_->GetAppRequestTypesState(kInvalidAppId),
            RequestType::State::UNAVAILABLE);
}

TEST_F(CacheManagerTest, GetAppRequestSubTypesState_GetState) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_EQ(cache_manager_->GetAppRequestSubTypesState(kDefaultAppId),
            RequestSubType::State::EMPTY);
  EXPECT_EQ(cache_manager_->GetAppRequestSubTypesState(kInvalidAppId),
            RequestSubType::State::UNAVAILABLE);
}

TEST_F(CacheManagerTest, IgnitionCyclesBeforeExchange_GetValue) {
  auto pt = cache_manager_->GetPT();

  // if result of computation is positive
  pt->policy_table.module_config.exchange_after_x_ignition_cycles = 100u;
  *(pt->policy_table.module_meta->ignition_cycles_since_last_exchange) = 80u;

  EXPECT_EQ(cache_manager_->IgnitionCyclesBeforeExchange(), 20);

  // if result of computation is negative
  pt->policy_table.module_config.exchange_after_x_ignition_cycles = 80u;
  *(pt->policy_table.module_meta->ignition_cycles_since_last_exchange) = 100u;

  EXPECT_EQ(cache_manager_->IgnitionCyclesBeforeExchange(), 0);
}

TEST_F(CacheManagerTest, KilometersBeforeExchange_GetValue) {
  auto pt = cache_manager_->GetPT();

  // if result of computation is positive
  pt->policy_table.module_config.exchange_after_x_kilometers = 1000u;
  *(pt->policy_table.module_meta->pt_exchanged_at_odometer_x) = 500u;

  EXPECT_EQ(cache_manager_->KilometersBeforeExchange(900), 600);

  // if result of computation is negative
  pt->policy_table.module_config.exchange_after_x_kilometers = 500u;
  *(pt->policy_table.module_meta->pt_exchanged_at_odometer_x) = 200u;

  EXPECT_EQ(cache_manager_->KilometersBeforeExchange(800), 0);
}

TEST_F(CacheManagerTest,
       DaysBeforeExchange_DaysNotInitialized_ReturnNegativeOne) {
  EXPECT_EQ(cache_manager_->DaysBeforeExchange(20), -1);
}

TEST_F(CacheManagerTest, DaysBeforeExchange_DaysIsInitialized_ReturnValue) {
  auto pt = cache_manager_->GetPT();
  *(pt->policy_table.module_meta->pt_exchanged_x_days_after_epoch) = 5;
  pt->policy_table.module_config.exchange_after_x_days = 30;

  EXPECT_EQ(cache_manager_->DaysBeforeExchange(20), 15);
}

TEST_F(CacheManagerTest, IncrementIgnitionCycles_CyclesAreIncremented) {
  auto pt = cache_manager_->GetPT();
  *pt->policy_table.module_meta->ignition_cycles_since_last_exchange = 0;

  cache_manager_->IncrementIgnitionCycles();
  EXPECT_EQ(*pt->policy_table.module_meta->ignition_cycles_since_last_exchange,
            1);

  cache_manager_->IncrementIgnitionCycles();
  EXPECT_EQ(*pt->policy_table.module_meta->ignition_cycles_since_last_exchange,
            2);
}

TEST_F(CacheManagerTest,
       SetCountersPassedForSuccessfulUpdate_Kilometers_ValueAssigned) {
  auto pt = cache_manager_->GetPT();

  EXPECT_TRUE(cache_manager_->SetCountersPassedForSuccessfulUpdate(
      Counters::KILOMETERS, 1000));
  EXPECT_EQ(*pt->policy_table.module_meta->pt_exchanged_at_odometer_x, 1000u);
}

TEST_F(CacheManagerTest,
       SetCountersPassedForSuccessfulUpdate_Kilometers_ReturnTrue) {
  auto pt = cache_manager_->GetPT();

  EXPECT_TRUE(cache_manager_->SetCountersPassedForSuccessfulUpdate(
      Counters::KILOMETERS, 1000));
  EXPECT_EQ(*pt->policy_table.module_meta->pt_exchanged_at_odometer_x, 1000u);
}

TEST_F(CacheManagerTest,
       SetCountersPassedForSuccessfulUpdate_DaysAfterEpoch_ReturnTrue) {
  auto pt = cache_manager_->GetPT();

  EXPECT_TRUE(cache_manager_->SetCountersPassedForSuccessfulUpdate(
      Counters::DAYS_AFTER_EPOCH, 20));
  EXPECT_EQ(*pt->policy_table.module_meta->pt_exchanged_x_days_after_epoch,
            20u);
}

TEST_F(CacheManagerTest, ResetIgnitionCycles_IgnitionCyclesAreResetted) {
  auto pt = cache_manager_->GetPT();
  *pt->policy_table.module_meta->ignition_cycles_since_last_exchange = 20;

  cache_manager_->ResetIgnitionCycles();
  EXPECT_EQ(*pt->policy_table.module_meta->ignition_cycles_since_last_exchange,
            0);
}

TEST_F(CacheManagerTest, TimeoutResponse_ReturnTimeoutDuration) {
  auto pt = cache_manager_->GetPT();
  pt->policy_table.module_config.timeout_after_x_seconds = 20u;

  EXPECT_EQ(cache_manager_->TimeoutResponse(), 20000);
}

TEST_F(CacheManagerTest, SecondsBetweenRetries_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  std::vector<int> expected_seconds({1, 5, 25, 125, 625});
  std::vector<int> seconds_from_PT;

  EXPECT_TRUE(cache_manager_->SecondsBetweenRetries(seconds_from_PT));
  EXPECT_EQ(expected_seconds, seconds_from_PT);
}

TEST_F(CacheManagerTest, GetDeviceConsent_ReturnkDeviceHasNoConsent) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");

  EXPECT_EQ(cache_manager_->GetDeviceConsent(device_id_), kDeviceHasNoConsent);
}

TEST_F(CacheManagerTest, GetPriority_DeviceAppID_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::string priority;

  EXPECT_TRUE(cache_manager_->GetPriority(kDeviceAppId, priority));
  EXPECT_EQ(priority, "NONE");

  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->GetPriority(kDeviceAppId, priority));
  EXPECT_EQ(priority, "EMERGENCY");
}

TEST_F(CacheManagerTest, GetPriority_DefaultAppID_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::string priority;

  EXPECT_TRUE(cache_manager_->GetPriority(kDefaultAppId, priority));
  EXPECT_EQ(priority, "NONE");

  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->GetPriority(kDefaultAppId, priority));
  EXPECT_EQ(priority, "EMERGENCY");
}

TEST_F(CacheManagerTest, GetPriority_InvalidAppID_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::string priority;

  EXPECT_FALSE(cache_manager_->GetPriority(kInvalidAppId, priority));
  EXPECT_TRUE(priority.empty());
}

TEST_F(CacheManagerTest, SetDefaultPolicy_ValidAppId_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->SetDefaultPolicy(app_id_));
}

TEST_F(CacheManagerTest, IsDefaultPolicy_DefaultValidAppId_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->SetDefaultPolicy(app_id_));
  EXPECT_TRUE(cache_manager_->IsDefaultPolicy(app_id_));
}

TEST_F(CacheManagerTest, IsDefaultPolicy_InvalidAppId_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->IsDefaultPolicy(kInvalidAppId));
}

TEST_F(CacheManagerTest, IsDefaultPolicy_NotDefaultValidAppId_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->IsDefaultPolicy(kDeviceAppId));
}

TEST_F(CacheManagerTest, SetIsDefault_InvalidAppId_DefaultNotAssigned) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(cache_manager_->SetIsDefault(kInvalidAppId));
  EXPECT_FALSE(cache_manager_->IsDefaultPolicy(kInvalidAppId));
}

TEST_F(CacheManagerTest, SetIsDefault_ValidAppId_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->SetIsDefault(app_id_));
}

TEST_F(CacheManagerTest, IsPredataPolicy_InvalidAppId_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->IsPredataPolicy(kInvalidAppId));
}

TEST_F(CacheManagerTest, IsPredataPolicy_NotPredataValidAppId_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_FALSE(cache_manager_->IsPredataPolicy(app_id_));
}

TEST_F(CacheManagerTest, IsPredataPolicy_PredataValidAppId_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  cache_manager_->SetIsPredata(app_id_);

  EXPECT_TRUE(cache_manager_->IsPredataPolicy(app_id_));
}

TEST_F(CacheManagerTest, SetPredataPolicy_NoPredataSection_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  auto pt = cache_manager_->GetPT();
  pt->policy_table.app_policies_section.apps.erase(kPreDataConsent);

  EXPECT_FALSE(cache_manager_->SetPredataPolicy(app_id_));
}

TEST_F(CacheManagerTest, SetPredataPolicy_ValidPredataSection_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->SetPredataPolicy(app_id_));
}

TEST_F(CacheManagerTest, SetIsPredata_InvalidAppID_PredataNotAssigned) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(cache_manager_->SetIsPredata(kInvalidAppId));
  EXPECT_FALSE(cache_manager_->IsPredataPolicy(kInvalidAppId));
}

TEST_F(CacheManagerTest, SetIsPredata_ValidAppID_PredataIsAssigned) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->SetIsPredata(app_id_));
  EXPECT_TRUE(cache_manager_->IsPredataPolicy(app_id_));
}

TEST_F(CacheManagerTest, SetUnpairedDevice_NoSuchDevice_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->SetUnpairedDevice(device_id_, true));
}

TEST_F(CacheManagerTest, SetUnpairedDevice_UnpairedDevice_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");

  EXPECT_TRUE(cache_manager_->SetUnpairedDevice(device_id_, true));
}

TEST_F(CacheManagerTest, SetUnpairedDevice_NotUnpairedDevice_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");

  EXPECT_TRUE(cache_manager_->SetUnpairedDevice(device_id_, false));
}

TEST_F(CacheManagerTest, SetVINValue_ValueIsSetted_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  auto pt = cache_manager_->GetPT();
  *pt->policy_table.module_meta->vin = "old_value";

  EXPECT_TRUE(cache_manager_->SetVINValue("new_value"));
  EXPECT_EQ(std::string("new_value"),
            std::string(*pt->policy_table.module_meta->vin));
}

TEST_F(CacheManagerTest, GetHMIAppTypeAfterUpdate_NoAppTypes_MapIsEmpty) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  std::map<std::string, StringArray> app_hmi_types;

  cache_manager_->GetHMIAppTypeAfterUpdate(app_hmi_types);
  EXPECT_TRUE(app_hmi_types.empty());
}

TEST_F(CacheManagerTest,
       ReactOnUserDevConsentForApp_DeviceNotAllowed_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->ReactOnUserDevConsentForApp(app_id_, false));
  EXPECT_TRUE(cache_manager_->IsPredataPolicy(app_id_));
}

TEST_F(CacheManagerTest,
       ReactOnUserDevConsentForApp_DeviceAllowedWithPredataPolicy_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  EXPECT_TRUE(cache_manager_->SetIsPredata(app_id_));

  EXPECT_TRUE(cache_manager_->ReactOnUserDevConsentForApp(app_id_, true));
  EXPECT_TRUE(cache_manager_->IsDefaultPolicy(app_id_));
}

TEST_F(CacheManagerTest,
       ReactOnUserDevConsentForApp_DeviceAllowedAndNoPredataPolicy_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_TRUE(cache_manager_->ReactOnUserDevConsentForApp(app_id_, true));
  EXPECT_FALSE(cache_manager_->IsDefaultPolicy(app_id_));
}

TEST_F(CacheManagerTest, CountUnconsentedGroups_InvalidAppID_ReturnZero) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_EQ(cache_manager_->CountUnconsentedGroups(kInvalidAppId, device_id_),
            0);
}

TEST_F(CacheManagerTest,
       CountUnconsentedGroups_AppIDIsInvalidOrDefaultOrPredata_ReturnZero) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_EQ(cache_manager_->CountUnconsentedGroups(kInvalidAppId, device_id_),
            0);
  EXPECT_EQ(cache_manager_->CountUnconsentedGroups(kDefaultAppId, device_id_),
            0);
  EXPECT_EQ(cache_manager_->CountUnconsentedGroups(kPreDataConsent, device_id_),
            0);
}

TEST_F(CacheManagerTest, CountUnconsentedGroups_AppIDIsDevice_ReturnOne) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");

  EXPECT_EQ(cache_manager_->CountUnconsentedGroups(kDeviceAppId, device_id_),
            1);
}

TEST_F(CacheManagerTest, SetMetaInfo_MetaInfoIsSetted_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  EXPECT_TRUE(cache_manager_->IsPTPreloaded());

  EXPECT_TRUE(
      cache_manager_->SetMetaInfo("ccpu_version", "country_code", "en-us"));
  EXPECT_FALSE(cache_manager_->IsPTPreloaded());
}

TEST_F(CacheManagerTest, IsMetaInfoPresent_NoMetaInfo_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->IsMetaInfoPresent());
}

TEST_F(CacheManagerTest, IsMetaInfoPresent_MetaInfoIsPresent_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(
      cache_manager_->SetMetaInfo("ccpu_version", "country_code", "en-us"));
  EXPECT_TRUE(cache_manager_->IsMetaInfoPresent());
}

TEST_F(CacheManagerTest, SetSystemLanguage_LanguageIsSetted_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(cache_manager_->SetSystemLanguage("en-us"));

  auto pt = cache_manager_->GetPT();
  EXPECT_EQ(*pt->policy_table.module_meta->language, "en-us");
}

TEST_F(CacheManagerTest, GetFunctionalGroupNames_GetGroupName_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  FunctionalGroupNames group_names;

  EXPECT_TRUE(cache_manager_->GetFunctionalGroupNames(group_names));
  EXPECT_EQ(group_names.size(), 1u);
  EXPECT_EQ(group_names[cache_manager_->GenerateHash("default")].second,
            "default");
}

TEST_F(CacheManagerTest, UpdateRequired_UpdateNotRequired_ReturnFalse) {
  cache_manager_->SaveUpdateRequired(false);
  EXPECT_FALSE(cache_manager_->UpdateRequired());
}

TEST_F(CacheManagerTest, UpdateRequired_UpdateIsRequired_ReturnTrue) {
  cache_manager_->SaveUpdateRequired(true);
  EXPECT_TRUE(cache_manager_->UpdateRequired());
}

TEST_F(CacheManagerTest, IsApplicationRevoked_InvalidAppID_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->IsApplicationRevoked(kInvalidAppId));
}

TEST_F(CacheManagerTest,
       IsApplicationRevoked_ValidAppIDNotRevoked_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_FALSE(cache_manager_->IsApplicationRevoked(app_id_));
}

TEST_F(CacheManagerTest, IsApplicationRevoked_ValidAppIDIsRevoked_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  auto pt = cache_manager_->GetPT();
  pt->policy_table.app_policies_section.apps[app_id_].set_to_null();

  EXPECT_TRUE(cache_manager_->IsApplicationRevoked(app_id_));
}

TEST_F(CacheManagerTest, LoadFromBackup_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(cache_manager_->LoadFromBackup());
}

TEST_F(CacheManagerTest, ResetPT_TableIsResettedFromFile_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  EXPECT_FALSE(cache_manager_->IsPTPreloaded());
  EXPECT_TRUE(cache_manager_->ResetPT(preloaded_pt_filename_));
  EXPECT_TRUE(cache_manager_->IsPTPreloaded());
}

TEST_F(CacheManagerTest, GetAppRequestTypes_DeviceAppID_NoTypes) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::vector<std::string> request_types;

  cache_manager_->GetAppRequestTypes(kDeviceAppId, request_types);
  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, GetAppRequestTypes_InvalidAppID_NoTypes) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::vector<std::string> request_types;

  cache_manager_->GetAppRequestTypes(kInvalidAppId, request_types);
  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, GetAppRequestTypes_ValidAppID_AddTypesIfPresent) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::vector<std::string> request_types;

  cache_manager_->GetAppRequestTypes(kDefaultAppId, request_types);
  EXPECT_TRUE(request_types.empty());

  request_types.clear();
  cache_manager_->GetAppRequestTypes(kPreDataConsent, request_types);
  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, GetInitialAppData_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  StringArray nicknames;
  StringArray app_hmi_types;

  EXPECT_TRUE(cache_manager_->GetInitialAppData(
      kDefaultAppId, nicknames, app_hmi_types));
  EXPECT_TRUE(nicknames.empty());
  EXPECT_TRUE(app_hmi_types.empty());
}

TEST_F(CacheManagerTest, GetFunctionalGroupings_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  FunctionalGroupings groups;

  EXPECT_TRUE(cache_manager_->GetFunctionalGroupings(groups));
  EXPECT_EQ(groups.size(), 27u);
}

TEST_F(CacheManagerTest, GetAppRequestSubTypes_DeviceAppID_NoTypes) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::vector<std::string> request_types;

  cache_manager_->GetAppRequestSubTypes(kDeviceAppId, request_types);
  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, GetAppRequestSubTypes_InvalidAppID_NoTypes) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::vector<std::string> request_types;

  cache_manager_->GetAppRequestSubTypes(kInvalidAppId, request_types);
  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, GetAppRequestSubTypes_ValidAppID_AddTypesIfPresent) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::vector<std::string> request_types;

  cache_manager_->GetAppRequestSubTypes(kDefaultAppId, request_types);
  EXPECT_TRUE(request_types.empty());

  request_types.clear();
  cache_manager_->GetAppRequestSubTypes(kPreDataConsent, request_types);
  EXPECT_TRUE(request_types.empty());
}

TEST_F(CacheManagerTest, Init_TableAlreadyExistsNotMerged_ReturnFalse) {
  EXPECT_TRUE(cache_manager_->Init(preloaded_pt_filename_, &policy_settings_));
  EXPECT_FALSE(
      cache_manager_->Init("invalid_filename.json", &policy_settings_));
}

TEST_F(CacheManagerTest, Init_TableAlreadyExistsAndIsMerged_ReturnTrue) {
  EXPECT_TRUE(cache_manager_->Init(
      "json/sdl_preloaded_pt_for_merge_initial.json", &policy_settings_));
  EXPECT_TRUE(cache_manager_->Init(
      "json/sdl_preloaded_pt_for_merge_latest.json", &policy_settings_));
}

TEST_F(CacheManagerTest, GetMetaInfo_ReturnMetaInfo) {
  EXPECT_TRUE(
      cache_manager_->SetMetaInfo("ccpu_version", "country_code", "en-us"));

  MetaInfo returned_meta_info = cache_manager_->GetMetaInfo();
  EXPECT_EQ("ccpu_version", returned_meta_info.ccpu_version);
  EXPECT_EQ("country_code", returned_meta_info.wers_country_code);
  EXPECT_EQ("en-us", returned_meta_info.language);
}

TEST_F(CacheManagerTest, GetCertificate_NoCertificateReturnEmptyString) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  std::string certificate = cache_manager_->GetCertificate();
  EXPECT_TRUE(certificate.empty());
}

TEST_F(CacheManagerTest, GetCertificate_CertificateExists_ReturnCertificate) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->SetDecryptedCertificate("certificate");

  std::string certificate = cache_manager_->GetCertificate();
  EXPECT_EQ(certificate, "certificate");
}

TEST_F(CacheManagerTest, SetExternalConsentStatus_EmptyStatus_ReturnFalse) {
  EXPECT_FALSE(
      cache_manager_->SetExternalConsentStatus(ExternalConsentStatus()));
}

TEST_F(CacheManagerTest, SetExternalConsentStatus_ValidStatus_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  ExternalConsentStatus consent_status;
  consent_status.insert(ExternalConsentStatusItem());

  EXPECT_TRUE(cache_manager_->SetExternalConsentStatus(consent_status));
}

TEST_F(CacheManagerTest,
       GetExternalConsentStatus_ValidStatus_ReturnConsentStatus) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  ExternalConsentStatus initial_consent_status;
  initial_consent_status.insert(ExternalConsentStatusItem());

  EXPECT_TRUE(cache_manager_->SetExternalConsentStatus(initial_consent_status));
  ExternalConsentStatus returned_consent_status =
      cache_manager_->GetExternalConsentStatus();
  EXPECT_EQ(initial_consent_status, returned_consent_status);
}

TEST_F(CacheManagerTest, GetExternalConsentEntities_NoEntities_EmptyStatus) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  ExternalConsentStatus consent_status =
      cache_manager_->GetExternalConsentEntities();
  EXPECT_TRUE(consent_status.empty());
}

TEST_F(CacheManagerTest, GetGroupsWithSameEntities_NoEntities_EmptyStatus) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  ExternalConsentStatus entities_to_found;
  entities_to_found.insert(ExternalConsentStatusItem());

  GroupsByExternalConsentStatus consent_status =
      cache_manager_->GetGroupsWithSameEntities(entities_to_found);
  EXPECT_TRUE(consent_status.empty());
}

TEST_F(CacheManagerTest, GetKnownLinksFromPT_NoLinks_ReturnEmptyMap) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  std::map<std::string, std::string> links =
      cache_manager_->GetKnownLinksFromPT();
  EXPECT_TRUE(links.empty());
}

TEST_F(CacheManagerTest, SetUserPermissionsForApp_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

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

  bool app_permissions_changed;
  EXPECT_TRUE(cache_manager_->SetUserPermissionsForApp(
      perm_consent, &app_permissions_changed));
  EXPECT_TRUE(app_permissions_changed);
}

TEST_F(CacheManagerTest, SetDeviceConsent_ConsentIsSetted_DeviceAllowed) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");

  StringArray consented_groups, disallowed_groups;
  consented_groups.push_back(std::string("DataConsent-2"));
  EXPECT_TRUE(cache_manager_->SetUserPermissionsForDevice(
      device_id_, consented_groups, disallowed_groups));

  cache_manager_->SetDeviceConsent(device_id_, true);
  EXPECT_EQ(kDeviceAllowed, cache_manager_->GetDeviceConsent(device_id_));
}

TEST_F(CacheManagerTest, SetDeviceConsent_ConsentIsSetted_DeviceDisallowed) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");

  cache_manager_->SetDeviceConsent(device_id_, false);
  EXPECT_EQ(kDeviceDisallowed, cache_manager_->GetDeviceConsent(device_id_));
}

TEST_F(CacheManagerTest, GetVehicleDataItems_GotDataItems) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::vector<policy_table::VehicleDataItem> items =
      cache_manager_->GetVehicleDataItems();
  EXPECT_EQ(items.size(), 30u);
}

TEST_F(CacheManagerTest, LockScreenDismissalWarningMessage_ReturnValidMessage) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  std::string expected_msg(
      "Swipe down to dismiss, acknowledging that you are not the driver");
  EXPECT_EQ(*cache_manager_->LockScreenDismissalWarningMessage("en-us"),
            expected_msg);
}

TEST_F(CacheManagerTest,
       LockScreenDismissalEnabledState_EnabledInPT_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_TRUE(*cache_manager_->LockScreenDismissalEnabledState());
}

TEST_F(CacheManagerTest,
       UnknownRPCPassthroughAllowed_InvalidAppID_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  EXPECT_FALSE(cache_manager_->UnknownRPCPassthroughAllowed(kInvalidAppId));
}

TEST_F(CacheManagerTest, UnknownRPCPassthroughAllowed_ValidAppID_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  auto pt = cache_manager_->GetPT();
  pt->policy_table.app_policies_section.apps[app_id_]
      .allow_unknown_rpc_passthrough = rpc::Optional<rpc::Boolean>(true);

  EXPECT_TRUE(cache_manager_->UnknownRPCPassthroughAllowed(app_id_));
}

TEST_F(CacheManagerTest, GetAppProperties_InvalidAppID_ReturnFalse) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  AppProperties out_app_properties;

  EXPECT_FALSE(
      cache_manager_->GetAppProperties(kInvalidAppId, out_app_properties));
}

TEST_F(CacheManagerTest, GetAppProperties_ValidAppID_ReturnTrue) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  AppProperties out_app_properties;

  cache_manager_->SetAppEndpoint(app_id_, "endpoint");
  cache_manager_->SetAppAuthToken(app_id_, "auth_token");
  cache_manager_->SetAppCloudTransportType(app_id_, "transport_type");
  cache_manager_->SetHybridAppPreference(app_id_, "CLOUD");
  cache_manager_->SetCloudAppEnabled(app_id_, true);

  EXPECT_TRUE(cache_manager_->GetAppProperties(app_id_, out_app_properties));
  EXPECT_EQ(out_app_properties.endpoint, "endpoint");
  EXPECT_EQ(out_app_properties.auth_token, "auth_token");
  EXPECT_EQ(out_app_properties.transport_type, "transport_type");
  EXPECT_EQ(out_app_properties.hybrid_app_preference, "CLOUD");
  EXPECT_TRUE(out_app_properties.enabled);
}

TEST_F(CacheManagerTest, SetExternalConsentForApp_ConsentIsSetted) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

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

  cache_manager_->AddDevice(device_id_, "Bluetooth");
  cache_manager_->SetDeviceData(device_id_);

  auto pt = cache_manager_->GetPT();
  policy_table::ConsentGroups initial_consent_groups =
      *(*(*pt->policy_table.device_data)[device_id_]
             .user_consent_records)[app_id_]
           .external_consent_status_groups;
  cache_manager_->SetExternalConsentForApp(perm_consent);

  policy_table::ConsentGroups updated_consent_groups =
      *(*(*pt->policy_table.device_data)[device_id_]
             .user_consent_records)[app_id_]
           .external_consent_status_groups;
  EXPECT_NE(initial_consent_groups, updated_consent_groups);
}

TEST_F(CacheManagerTest, Add_MinutesAreAdded) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  auto pt = cache_manager_->GetPT();
  int result;

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .minutes_in_hmi_full = 0;
  cache_manager_->Add(app_id_, usage_statistics::SECONDS_HMI_FULL, 60);
  result = static_cast<int>(
      (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
          .minutes_in_hmi_full);
  EXPECT_EQ(result, 1);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .minutes_in_hmi_limited = 0;
  cache_manager_->Add(app_id_, usage_statistics::SECONDS_HMI_LIMITED, 120);
  result = static_cast<int>(
      (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
          .minutes_in_hmi_limited);
  EXPECT_EQ(result, 2);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .minutes_in_hmi_background = 0;
  cache_manager_->Add(app_id_, usage_statistics::SECONDS_HMI_BACKGROUND, 180);
  result = static_cast<int>(
      (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
          .minutes_in_hmi_background);
  EXPECT_EQ(result, 3);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .minutes_in_hmi_none = 0;
  cache_manager_->Add(app_id_, usage_statistics::SECONDS_HMI_NONE, 240);
  result = static_cast<int>(
      (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
          .minutes_in_hmi_none);
  EXPECT_EQ(result, 4);
}

TEST_F(CacheManagerTest, Set_ValuesAreSetted) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  auto pt = cache_manager_->GetPT();

  cache_manager_->Set(app_id_, usage_statistics::LANGUAGE_GUI, "eng-eng");
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .app_registration_language_gui,
            "eng-eng");

  cache_manager_->Set(app_id_, usage_statistics::LANGUAGE_VUI, "ru-ru");
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .app_registration_language_vui,
            "ru-ru");
}

TEST_F(CacheManagerTest, Increment_GlobalCounterIsIncremented) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  auto pt = cache_manager_->GetPT();

  *pt->policy_table.usage_and_error_counts->count_of_iap_buffer_full = 0;
  cache_manager_->Increment(usage_statistics::IAP_BUFFER_FULL);
  EXPECT_EQ(
      (*pt->policy_table.usage_and_error_counts->count_of_iap_buffer_full), 1);

  *pt->policy_table.usage_and_error_counts->count_sync_out_of_memory = 0;
  cache_manager_->Increment(usage_statistics::SYNC_OUT_OF_MEMORY);
  EXPECT_EQ(
      (*pt->policy_table.usage_and_error_counts->count_sync_out_of_memory), 1);

  *pt->policy_table.usage_and_error_counts->count_of_sync_reboots = 0;
  cache_manager_->Increment(usage_statistics::SYNC_REBOOTS);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->count_of_sync_reboots),
            1);
}

TEST_F(CacheManagerTest, Increment_AppCounterIsIncremented) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  auto pt = cache_manager_->GetPT();

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .count_of_user_selections = 0;
  cache_manager_->Increment(app_id_, usage_statistics::USER_SELECTIONS);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .count_of_user_selections,
            1);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .count_of_rejections_sync_out_of_memory = 0;
  cache_manager_->Increment(app_id_,
                            usage_statistics::REJECTIONS_SYNC_OUT_OF_MEMORY);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .count_of_rejections_sync_out_of_memory,
            1);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .count_of_rejections_nickname_mismatch = 0;
  cache_manager_->Increment(app_id_,
                            usage_statistics::REJECTIONS_NICKNAME_MISMATCH);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .count_of_rejections_nickname_mismatch,
            1);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .count_of_rejections_duplicate_name = 0;
  cache_manager_->Increment(app_id_,
                            usage_statistics::REJECTIONS_DUPLICATE_NAME);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .count_of_rejections_duplicate_name,
            1);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .count_of_rejected_rpc_calls = 0;
  cache_manager_->Increment(app_id_, usage_statistics::REJECTED_RPC_CALLS);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .count_of_rejected_rpc_calls,
            1);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .count_of_rpcs_sent_in_hmi_none = 0;
  cache_manager_->Increment(app_id_, usage_statistics::RPCS_IN_HMI_NONE);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .count_of_rpcs_sent_in_hmi_none,
            1);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .count_of_removals_for_bad_behavior = 0;
  cache_manager_->Increment(app_id_, usage_statistics::REMOVALS_MISBEHAVED);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .count_of_removals_for_bad_behavior,
            1);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .count_of_run_attempts_while_revoked = 0;
  cache_manager_->Increment(app_id_,
                            usage_statistics::RUN_ATTEMPTS_WHILE_REVOKED);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .count_of_run_attempts_while_revoked,
            1);

  (*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
      .count_of_tls_errors = 0;
  cache_manager_->Increment(app_id_, usage_statistics::COUNT_OF_TLS_ERRORS);
  EXPECT_EQ((*pt->policy_table.usage_and_error_counts->app_level)[app_id_]
                .count_of_tls_errors,
            1);
}

TEST_F(CacheManagerTest, GetUnconsentedGroups_InvalidAppId_ReturnNoGroups) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  FunctionalGroupIDs group_ids;

  cache_manager_->GetUnconsentedGroups(device_id_, kInvalidAppId, group_ids);
  EXPECT_TRUE(group_ids.empty());
}

TEST_F(
    CacheManagerTest,
    GetUnconsentedGroups_ValidAppId_UserConsentPromptNotInitialized_NoGroups) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");
  cache_manager_->SetDeviceData(device_id_);

  FunctionalGroupIDs group_ids;

  cache_manager_->GetUnconsentedGroups(device_id_, app_id_, group_ids);
  EXPECT_TRUE(group_ids.empty());
}

TEST_F(CacheManagerTest, GetConsentedGroups_ValidAppId_ReturnGroups) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");
  cache_manager_->SetDeviceData(device_id_);
  auto pt = cache_manager_->GetPT();
  ConsentGroups consented_groups;
  consented_groups.insert(std::make_pair("Base-4", true));
  consented_groups.insert(std::make_pair("Base-6", false));
  *(*(*pt->policy_table.device_data)[device_id_].user_consent_records)[app_id_]
       .consent_groups = consented_groups;

  FunctionalGroupIDs allowed_groups;
  FunctionalGroupIDs disallowed_groups;

  cache_manager_->GetConsentedGroups(
      device_id_, app_id_, allowed_groups, disallowed_groups);
  EXPECT_FALSE(allowed_groups.empty());
  EXPECT_FALSE(disallowed_groups.empty());
}

TEST_F(CacheManagerTest, RemoveAppConsentForGroup_GroupIsRemoved) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  cache_manager_->AddDevice(device_id_, "Bluetooth");
  cache_manager_->SetDeviceData(device_id_);
  auto pt = cache_manager_->GetPT();
  ConsentGroups consented_groups;
  consented_groups.insert(std::make_pair("Base-4", true));
  consented_groups.insert(std::make_pair("Base-6", false));
  *(*(*pt->policy_table.device_data)[device_id_].user_consent_records)[app_id_]
       .consent_groups = consented_groups;

  FunctionalGroupIDs initial_allowed_groups;
  FunctionalGroupIDs initial_disallowed_groups;
  cache_manager_->GetConsentedGroups(
      device_id_, app_id_, initial_allowed_groups, initial_disallowed_groups);
  EXPECT_FALSE(initial_allowed_groups.empty());
  EXPECT_FALSE(initial_disallowed_groups.empty());

  cache_manager_->RemoveAppConsentForGroup(app_id_, "Base-6");

  FunctionalGroupIDs updated_allowed_groups;
  FunctionalGroupIDs updated_disallowed_groups;
  cache_manager_->GetConsentedGroups(
      device_id_, app_id_, updated_allowed_groups, updated_disallowed_groups);
  EXPECT_FALSE(updated_allowed_groups.empty());
  EXPECT_TRUE(updated_disallowed_groups.empty());
}

TEST_F(CacheManagerTest, GetEnabledCloudApps_GetApp) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);
  std::vector<std::string> enabled_apps;

  cache_manager_->SetCloudAppEnabled(app_id_, true);
  cache_manager_->GetEnabledCloudApps(enabled_apps);
  EXPECT_FALSE(enabled_apps.empty());
}

TEST_F(CacheManagerTest, GetLockScreenIconUrl_ReturnEmptyString) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);

  std::string icon_url = cache_manager_->GetLockScreenIconUrl();
  EXPECT_TRUE(icon_url.empty());
}

TEST_F(CacheManagerTest, GetIconUrl_IconNotDefinedReturnEmptyString) {
  cache_manager_->Init(preloaded_pt_filename_, &policy_settings_);
  UpdatePT(cache_manager_);

  std::string icon_url = cache_manager_->GetIconUrl(app_id_);
  EXPECT_TRUE(icon_url.empty());
}

}  // namespace policy_test
}  // namespace components
}  // namespace test

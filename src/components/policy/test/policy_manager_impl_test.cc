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

#include <vector>
#include <fstream>
#include <string>
#include <ctime>

#include "json/reader.h"
#include "gtest/gtest.h"
#include "mock_policy_listener.h"
#include "mock_pt_representation.h"
#include "mock_cache_manager.h"
#include "mock_update_status_manager.h"
#include "policy/policy_manager_impl.h"
#include "config_profile/profile.h"
#include "table_struct/enums.h"
#include "table_struct/types.h"
#include "utils/file_system.h"
#include "utils/date_time.h"
#include "utils/make_shared.h"

using ::testing::Return;
using ::testing::NiceMock;
using ::testing::_;
using ::testing::SetArgReferee;
using ::testing::AtLeast;

using ::policy::MockPolicyListener;

using ::policy::MockCacheManagerInterface;
using ::policy::MockUpdateStatusManager;

using ::policy::PolicyManagerImpl;
using ::policy::PolicyTable;

namespace policy_table = rpc::policy_table_interface_base;

namespace test {
namespace components {
namespace policy {

namespace custom_str = utils::custom_string;

typedef std::multimap<std::string, policy_table::Rpcs&>
    UserConsentPromptToRpcsConnections;

template <typename T>
std::string NumberToString(T Number) {
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}

template <typename T>
void SortAndCheckEquality(std::vector<T> first, std::vector<T> second) {
  ASSERT_EQ(first.size(), second.size());
  std::sort(first.begin(), first.end());
  std::sort(second.begin(), second.end());
  // Checks
  for (uint32_t i = 0; i < first.size(); ++i) {
    EXPECT_EQ(first[i], second[i]);
  }
}

class PolicyManagerImplTest : public ::testing::Test {
 protected:
  PolicyManagerImpl* manager;
  MockCacheManagerInterface* cache_manager;
  NiceMock<MockPolicyListener> listener;

  void SetUp() OVERRIDE {
    manager = new PolicyManagerImpl();
    manager->set_listener(&listener);
    cache_manager = new MockCacheManagerInterface();
    manager->set_cache_manager(cache_manager);
  }

  void TearDown() OVERRIDE {
    delete manager;
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

class PolicyManagerImplTest2 : public ::testing::Test {
 public:
  PolicyManagerImplTest2()
      : app_id1("123456789")
      , app_id2("1766825573")
      , dev_id1("XXX123456789ZZZ")
      , dev_id2("08-00-27-CE-76-FE")
      , PTU_request_types(Json::arrayValue) {}

 protected:
  PolicyManagerImpl* manager;
  NiceMock<MockPolicyListener> listener;
  std::vector<std::string> hmi_level;
  std::vector<std::string> PT_request_types;
  uint32_t PTU_request_types_size;
  unsigned int index;
  const std::string app_id1;
  const std::string app_id2;
  const std::string dev_id1;
  const std::string dev_id2;
  Json::Value PTU_request_types;

  void SetUp() OVERRIDE {
    file_system::CreateDirectory("storage1");

    profile::Profile::instance()->config_file_name("smartDeviceLink2.ini");
    manager = new PolicyManagerImpl();
    manager->set_listener(&listener);
    const char* levels[] = {"BACKGROUND", "FULL", "LIMITED", "NONE"};
    hmi_level.assign(levels, levels + sizeof(levels) / sizeof(levels[0]));
    srand(time(NULL));
    index = rand() % 3;
  }

  std::vector<std::string> JsonToVectorString(
      const Json::Value& PTU_request_types) {
    std::vector<std::string> result;
    for (uint32_t i = 0; i < PTU_request_types.size(); ++i) {
      result.push_back(PTU_request_types[i].asString());
    }
    return result;
  }

  const Json::Value GetPTU(std::string file_name) {
    // Get PTU
    std::ifstream ifile(file_name);
    Json::Reader reader;
    std::string json;
    Json::Value root(Json::objectValue);
    if (ifile.is_open() && reader.parse(ifile, root, true)) {
      json = root.toStyledString();
    }
    ifile.close();
    ::policy::BinaryMessage msg(json.begin(), json.end());
    // Load Json to cache
    EXPECT_TRUE(manager->LoadPT("file_pt_update.json", msg));
    return root;
  }

  void CreateLocalPT(std::string file_name) {
    file_system::remove_directory_content("storage1");
    ASSERT_TRUE(manager->InitPT(file_name));
  }

  void AddRTtoPT(const std::string& update_file_name,
                 const std::string& section_name,
                 const uint32_t rt_number,
                 const uint32_t invalid_rt_number) {
    // Arrange
    CreateLocalPT("sdl_preloaded_pt.json");
    // Get RequestTypes from section of preloaded_pt app_policies
    PT_request_types = manager->GetAppRequestTypes(section_name);
    EXPECT_EQ(rt_number, PT_request_types.size());
    Json::Value root = GetPTU(update_file_name);
    // Get Request Types from JSON (PTU)
    PTU_request_types =
        root["policy_table"]["app_policies"][section_name]["RequestType"];
    PTU_request_types_size = PTU_request_types.size();
    PT_request_types.clear();
    // Get RequestTypes from section of PT app policies after update
    PT_request_types = manager->GetAppRequestTypes(section_name);
    // Check number of RT in PTU and PT now are equal
    ASSERT_EQ(PTU_request_types_size - invalid_rt_number,
              PT_request_types.size());
  }

  void AddRTtoAppSectionPT(const std::string& update_file_name,
                           const std::string& section_name,
                           const uint32_t rt_number,
                           const uint32_t invalid_rt_number) {
    // Arrange
    CreateLocalPT("sdl_preloaded_pt.json");
    // Add app
    manager->AddApplication(section_name);
    // Check app gets RequestTypes from pre_DataConsent of app_policies
    // section
    PT_request_types = manager->GetAppRequestTypes(section_name);
    EXPECT_EQ(rt_number, PT_request_types.size());
    EXPECT_CALL(listener, OnPendingPermissionChange(section_name)).Times(1);
    Json::Value root = GetPTU(update_file_name);

    // Get App Request Types from PTU
    PTU_request_types =
        root["policy_table"]["app_policies"][section_name]["RequestType"];
    PTU_request_types_size = PTU_request_types.size();

    PT_request_types.clear();
    // Get RequestTypes from <app_id> section of app policies after PT update
    PT_request_types = manager->GetAppRequestTypes(section_name);
    // Check sizes of Request types of PT and PTU
    ASSERT_EQ(PTU_request_types_size - invalid_rt_number,
              PT_request_types.size());

    ::policy::AppPermissions permissions =
        manager->GetAppPermissionsChanges(section_name);
    EXPECT_TRUE(permissions.requestTypeChanged);
  }

  std::vector<policy_table::RequestType> PushRequestTypesToContainer(
      const std::vector<std::string>& temp_result) {
    policy_table::RequestType filtered_result;
    std::vector<policy_table::RequestType> final_result;
    for (uint32_t i = 0; i < temp_result.size(); ++i) {
      if (policy_table::EnumFromJsonString(temp_result[i], &filtered_result)) {
        final_result.push_back(filtered_result);
      }
    }
    return final_result;
  }

  void CheckResultForValidRT() {
    // Convert Json Array to std::vector<std::string>
    const std::vector<std::string>& result =
        JsonToVectorString(PTU_request_types);
    // Checks
    SortAndCheckEquality(PT_request_types, result);
  }

  void CheckResultForInvalidRT() {
    // Convert Json Array to std::vector<std::string>
    const std::vector<std::string>& temp_result =
        JsonToVectorString(PTU_request_types);
    std::vector<policy_table::RequestType> result1 =
        PushRequestTypesToContainer(temp_result);
    std::vector<policy_table::RequestType> result2 =
        PushRequestTypesToContainer(PT_request_types);
    // Checks
    SortAndCheckEquality(result1, result2);
  }
  void FillMultimapFromFunctionalGroupings(
      UserConsentPromptToRpcsConnections& input_multimap,
      policy_table::FunctionalGroupings& fg_table) {
    policy_table::FunctionalGroupings::iterator fg_itter = fg_table.begin();
    const policy_table::FunctionalGroupings::iterator fg_itter_end =
        fg_table.end();
    for (; fg_itter != fg_itter_end; ++fg_itter) {
      // RPCS getting
      policy_table::Rpcs& rpcs_ref = fg_itter->second;
      // User_consent_prompt getting
      rpc::Optional<rpc::String<1, 255> >& optional_ref =
          rpcs_ref.user_consent_prompt;
      rpc::String<1, 255>& ucp_string = *optional_ref;
      const std::string& ucp_std_string =
          static_cast<const std::string&>(ucp_string);
      // Multimap inserting
      input_multimap.insert(std::pair<std::string, policy_table::Rpcs&>(
          ucp_std_string, rpcs_ref));
    }
  }

  void GetFunctionalGroupingsFromManager(
      policy_table::FunctionalGroupings& input_functional_groupings) {
    // Get cache
    ::policy::CacheManagerInterfaceSPtr cache = manager->GetCache();
    // Get table_snapshot
    utils::SharedPtr<policy_table::Table> table = cache->GenerateSnapshot();
    // Set functional groupings from policy table
    input_functional_groupings = table->policy_table.functional_groupings;
  }

  void TearDown() OVERRIDE {
    profile::Profile::instance()->config_file_name("smartDeviceLink.ini");
    delete manager;
  }
};

Json::Value CreatePTforLoad() {
  const std::string load_table(
  "{"
  "\"policy_table\": {"
      "\"module_config\": {"
          "\"preloaded_pt\": true,"
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
              "\"emergency\": 1,"
              "\"navigation\": 2,"
              "\"VOICECOMM\": 3,"
              "\"communication\": 4,"
              "\"normal\": 5,"
              "\"none\": 6"
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
  Json::Reader reader;
  EXPECT_TRUE(reader.parse(load_table, table));
  return table;
}

TEST_F(PolicyManagerImplTest, GetNotificationsNumber) {
  const std::string priority = "EMERGENCY";
  const uint32_t notif_number = 100u;
  EXPECT_CALL(*cache_manager, GetNotificationsNumber(priority))
      .WillOnce(Return(notif_number));

  EXPECT_EQ(notif_number, manager->GetNotificationsNumber(priority));
}

TEST_F(PolicyManagerImplTest2, GetNotificationsNumberAfterPTUpdate) {
  // Arrange  
  Json::Value table = CreatePTforLoad();
  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);
  // Act
  const std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());
  EXPECT_CALL(listener, OnUpdateStatusChanged(_));
  EXPECT_TRUE(manager->LoadPT("file_pt_update.json", msg));

  std::string priority = "emergency";
  uint32_t notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(1u, notif_number);

  priority = "navigation";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(2u, notif_number);

  priority = "emergency";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(1u, notif_number);

  priority = "VOICECOMM";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(3u, notif_number);

  priority = "normal";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(5u, notif_number);

  priority = "none";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(6u, notif_number);
}

TEST_F(PolicyManagerImplTest2, IsAppRevoked_SetRevokedAppID_ExpectAppRevoked) {
  // Arrange
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    root["policy_table"]["app_policies"][app_id1] = Json::nullValue;
    json = root.toStyledString();
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_TRUE(manager->LoadPT("file_pt_update.json", msg));
  EXPECT_TRUE(manager->IsApplicationRevoked(app_id1));
}

TEST_F(PolicyManagerImplTest, IncrementGlobalCounter) {
  // Assert
  EXPECT_CALL(*cache_manager, Increment(usage_statistics::SYNC_REBOOTS));
  manager->Increment(usage_statistics::SYNC_REBOOTS);
}

TEST_F(PolicyManagerImplTest, IncrementAppCounter) {
  // Assert
  EXPECT_CALL(*cache_manager,
              Increment("12345", usage_statistics::USER_SELECTIONS));
  manager->Increment("12345", usage_statistics::USER_SELECTIONS);
}

TEST_F(PolicyManagerImplTest, SetAppInfo) {
  // Assert
  EXPECT_CALL(*cache_manager,
              Set("12345", usage_statistics::LANGUAGE_GUI, "de-de"));
  manager->Set("12345", usage_statistics::LANGUAGE_GUI, "de-de");
}

TEST_F(PolicyManagerImplTest, AddAppStopwatch) {
  // Assert
  EXPECT_CALL(*cache_manager,
              Add("12345", usage_statistics::SECONDS_HMI_FULL, 30));
  manager->Add("12345", usage_statistics::SECONDS_HMI_FULL, 30);
}

TEST_F(PolicyManagerImplTest, ResetPT) {
  EXPECT_CALL(*cache_manager, ResetPT("filename"))
      .WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager, ResetCalculatedPermissions()).Times(AtLeast(1));
  EXPECT_CALL(*cache_manager, TimeoutResponse());
  EXPECT_CALL(*cache_manager, SecondsBetweenRetries(_));

  EXPECT_TRUE(manager->ResetPT("filename"));
  EXPECT_FALSE(manager->ResetPT("filename"));
}

TEST_F(PolicyManagerImplTest, LoadPT_SetPT_PTIsLoaded) {
  // Arrange
  Json::Value table = CreatePTforLoad();
  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  // Assert
  ASSERT_TRUE(IsValid(update));

  EXPECT_CALL(*cache_manager, GetHMIAppTypeAfterUpdate(_)).Times(AtLeast(1));

  // Act
  const std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  utils::SharedPtr<policy_table::Table> snapshot =
      utils::MakeShared<policy_table::Table>(update.policy_table);
  // Assert
  EXPECT_CALL(*cache_manager, GenerateSnapshot()).WillOnce(Return(snapshot));
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).WillOnce(Return(true));
  EXPECT_CALL(listener, GetAppName("1234"))
      .WillOnce(Return(custom_str::CustomString("")));
  EXPECT_CALL(listener, OnUpdateStatusChanged(_));
  EXPECT_CALL(*cache_manager, SaveUpdateRequired(false));
  EXPECT_CALL(*cache_manager, TimeoutResponse());
  EXPECT_CALL(*cache_manager, SecondsBetweenRetries(_));

  EXPECT_TRUE(manager->LoadPT("file_pt_update.json", msg));
}

TEST_F(PolicyManagerImplTest, LoadPT_SetInvalidUpdatePT_PTIsNotLoaded) {
  // Arrange
  Json::Value table(Json::objectValue);

  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  // Assert update is invalid
  ASSERT_FALSE(IsValid(update));

  // Act
  std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  // Assert
  EXPECT_CALL(*cache_manager, GenerateSnapshot()).Times(0);
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).Times(0);
  EXPECT_CALL(listener, GetAppName(_)).Times(0);
  EXPECT_CALL(listener, OnUpdateStatusChanged(_)).Times(1);
  EXPECT_CALL(*cache_manager, SaveUpdateRequired(false)).Times(0);
  EXPECT_CALL(*cache_manager, TimeoutResponse()).Times(0);
  EXPECT_CALL(*cache_manager, SecondsBetweenRetries(_)).Times(0);
  EXPECT_FALSE(manager->LoadPT("file_pt_update.json", msg));
}

TEST_F(PolicyManagerImplTest2,
       KmsChanged_SetExceededKms_ExpectCorrectSchedule) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ::policy::Counters counter = ::policy::Counters::KILOMETERS;
  manager->PTUpdatedAt(counter, 50000);
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  // Set kms changed but not exceed limit
  manager->KmsChanged(51500);
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
  // Set kms changed and exceed limit
  manager->KmsChanged(52500);
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(
    PolicyManagerImplTest2,
    AddApplication_AddNewApplicationFromDeviceWithoutConsent_ExpectUpdateRequired) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id1);
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(
    PolicyManagerImplTest2,
    AddApplication_AddExistingApplicationFromDeviceWithoutConsent_ExpectNoUpdateRequired) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  GetPTU("valid_sdl_pt_update.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  // Try to add existing app
  manager->AddApplication(app_id2);
  // Check no update required
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2,
       PTUpdatedAt_DaysNotExceedLimit_ExpectNoUpdateRequired) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  TimevalStruct current_time = date_time::DateTime::getCurrentTime();
  const int kSecondsInDay = 60 * 60 * 24;
  int days = current_time.tv_sec / kSecondsInDay;
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());

  GetPTU("valid_sdl_pt_update.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());

  manager->AddApplication(app_id2);
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  ::policy::Counters counter = ::policy::Counters::DAYS_AFTER_EPOCH;
  // Set PT was updated 10 days ago (limit is 30 days for now)
  // So no limit exceeded
  manager->PTUpdatedAt(counter, days - 10);
  manager->OnAppRegisteredOnMobile(app_id2);
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, ForcePTExchange_ExpectUpdateNeeded) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  // Force OT Exchange
  manager->ForcePTExchange();
  // Check update required
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, OnSystemReady) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  // Check
  EXPECT_CALL(listener, OnSystemInfoUpdateRequired());
  manager->OnSystemReady();
}

TEST_F(PolicyManagerImplTest2, ResetRetrySequence) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->ResetRetrySequence();
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
  manager->OnUpdateStarted();
  EXPECT_EQ("UPDATING", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, NextRetryTimeout_ExpectTimeoutsFromPT) {
  // Arrange
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::Reader reader;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    Json::Value seconds_between_retries = Json::Value(Json::arrayValue);
    seconds_between_retries =
        root["policy_table"]["module_config"]["seconds_between_retries"];
    uint32_t size = seconds_between_retries.size();
    CreateLocalPT("sdl_preloaded_pt.json");

    uint32_t waiting_timeout = 0u;

    for (uint32_t retry_number = 0u; retry_number < size; ++retry_number) {
      waiting_timeout += seconds_between_retries[retry_number].asInt();
      waiting_timeout += manager->TimeoutExchange();

      // it's in miliseconds
      EXPECT_EQ(waiting_timeout * date_time::DateTime::MILLISECONDS_IN_SECOND,
                manager->NextRetryTimeout());
    }
  }
}

TEST_F(PolicyManagerImplTest2, TimeOutExchange) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  // Check value taken from PT
  EXPECT_EQ(70, manager->TimeoutExchange());
}

TEST_F(PolicyManagerImplTest2, GetPolicyTableStatus_ExpectUpToDate) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  // Check
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2,
       RetrySequenceDelaysSeconds_Expect_CorrectValues) {
  // Arrange
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::Reader reader;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    Json::Value seconds_between_retries = Json::Value(Json::arrayValue);
    seconds_between_retries =
        root["policy_table"]["module_config"]["seconds_between_retries"];
    uint32_t size = seconds_between_retries.size();
    CreateLocalPT("sdl_preloaded_pt.json");
    std::vector<int> delaySecs = manager->RetrySequenceDelaysSeconds();
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
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->OnExceededTimeout();
  // Check
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, GetInitialAppData_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id2);
  ::policy::StringArray app_nicknames;
  ::policy::StringArray app_hmi_types;
  manager->GetInitialAppData(app_id2, &app_nicknames, &app_hmi_types);
  // Expect Empty nicknames and AppHMITypes
  EXPECT_EQ(0u, app_nicknames.size());
  EXPECT_EQ(0u, app_hmi_types.size());

  Json::Value root = GetPTU("valid_sdl_pt_update.json");

  Json::Value appHmiTypes = Json::Value(Json::arrayValue);
  appHmiTypes = root["policy_table"]["app_policies"][app_id2]["AppHMIType"];
  uint32_t appHmiType_size = appHmiTypes.size();

  Json::Value appNicknames = Json::Value(Json::arrayValue);
  appNicknames = root["policy_table"]["app_policies"][app_id2]["nicknames"];
  uint32_t appNicknames_size = appNicknames.size();

  ::policy::StringArray app_nicknames1;
  ::policy::StringArray app_hmi_types1;
  manager->GetInitialAppData(app_id2, &app_nicknames1, &app_hmi_types1);
  uint32_t nick_names_size = app_nicknames1.size();
  uint32_t app_hmi_types_size = app_hmi_types1.size();
  ASSERT_EQ(appHmiType_size, app_hmi_types_size);
  ASSERT_EQ(appNicknames_size, nick_names_size);
  ASSERT_GT(nick_names_size, 0u);
  ASSERT_GT(app_hmi_types_size, 0u);
  // Check nicknames match
  for (uint32_t i = 0; i < nick_names_size; ++i) {
    EXPECT_EQ(app_nicknames1[i], appNicknames[i].asString());
  }
  // Check AppHMITypes match
  for (uint32_t i = 0; i < app_hmi_types_size; ++i) {
    EXPECT_EQ(app_hmi_types1[i], appHmiTypes[i].asString());
  }
}

TEST_F(PolicyManagerImplTest2,
       CanAppKeepContext_SetPoliciesForAppUpdated_ExpectAppCanKeepContext) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id2);
  GetPTU("valid_sdl_pt_update.json");
  // Check keep context in updated policies for app
  EXPECT_TRUE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2,
       CanAppStealFocus_SetPoliciesForAppUpdated_ExpectAppCanStealFocus) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id2);
  GetPTU("valid_sdl_pt_update.json");
  // Check keep context in updated policies for app
  EXPECT_TRUE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2, GetCurrentDeviceId) {
  // Arrange
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).Times(2);
  EXPECT_EQ(custom_str::CustomString(""), manager->GetCurrentDeviceId(app_id2));
  EXPECT_EQ("", manager->GetCurrentDeviceId(app_id2));
}

TEST_F(PolicyManagerImplTest2,
       GetVehicleInfo_SetVehicleInfo_ExpectReceivedInfoCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  GetPTU("valid_sdl_pt_update.json");
  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();
  policy_table::ModuleConfig& module_config = pt->policy_table.module_config;
  ::policy::VehicleInfo vehicle_info = manager->GetVehicleInfo();

  EXPECT_EQ(static_cast<std::string>(*module_config.vehicle_make),
            vehicle_info.vehicle_make);
  EXPECT_EQ(static_cast<std::string>(*module_config.vehicle_model),
            vehicle_info.vehicle_model);
  EXPECT_EQ(static_cast<std::string>(*module_config.vehicle_year),
            vehicle_info.vehicle_year);
}

}  // namespace policy
}  // namespace components
}  // namespace test

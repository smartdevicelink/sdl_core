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

#include <ctime>
#include <fstream>
#include <string>
#include <vector>

#include "gtest/gtest.h"
#include "json/writer.h"
#include "utils/jsoncpp_reader_wrapper.h"

#include "config_profile/profile.h"
#include "connection_handler/connection_handler.h"
#include "policy/mock_cache_manager.h"
#include "policy/mock_policy_listener.h"
#include "policy/mock_policy_settings.h"
#include "policy/mock_update_status_manager.h"
#include "policy/policy_manager_impl.h"
#include "policy/policy_table/enums.h"
#include "policy/policy_table/types.h"

#include "utils/date_time.h"
#include "utils/file_system.h"
#include "utils/gen_hash.h"
#include "utils/macro.h"

#include "policy/mock_access_remote.h"

using ::testing::_;
using ::testing::AtLeast;
using ::testing::DoAll;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::SetArgReferee;

using ::policy::PolicyManagerImpl;
using ::policy::PolicyTable;

namespace test {
namespace components {
namespace policy_test {
namespace policy_table = rpc::policy_table_interface_base;

namespace custom_str = utils::custom_string;

typedef std::multimap<std::string, policy_table::Rpcs&>
    UserConsentPromptToRpcsConnections;

typedef std::shared_ptr<policy_table::Table> PolicyTableSPtr;

namespace {
std::string kSpeed = "speed";
std::string kRPM = "rpm";
std::string kFuelLevel = "fuelLevel";
}  // namespace

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

struct StringsForUpdate {
  std::string new_field_value_;
  std::string new_field_name_;
  std::string new_date_;
};

char GenRandomString(const char* alphanum) {
  const int stringLength = sizeof(alphanum) - 1;
  return alphanum[rand() % stringLength];
}

struct StringsForUpdate CreateNewRandomData(StringsForUpdate& str) {
  // Generate random date
  srand(time(NULL));
  unsigned int day = 1 + rand() % 31;      // Day from 1 - 31
  unsigned int month = 1 + rand() % 12;    // Month from 1 - 12
  unsigned int year = 1985 + rand() % 31;  // Year from 1985 - 2015

  // Convert date to string
  str.new_date_ = NumberToString(year) + '-' + NumberToString(month) + '-' +
                  NumberToString(day);

  // Create new field
  unsigned int number = 1 + rand() % 100;  // Number from 1 - 100
  str.new_field_name_ += NumberToString(number);

  // Create new field random value
  const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

  for (unsigned int i = 0; i < 5; ++i) {
    str.new_field_value_ += GenRandomString(alphanum);
  }
  return str;
}

policy_table::AppHmiTypes HmiTypes(const policy_table::AppHMIType hmi_type) {
  policy_table::AppHmiTypes hmi_types;
  hmi_types.push_back(hmi_type);
  return hmi_types;
}

class PolicyManagerImplTest : public ::testing::Test {
 public:
  PolicyManagerImplTest() : device_id("08-00-27-CE-76-FE") {}

 protected:
  PolicyManagerImpl* manager;
  NiceMock<MockCacheManagerInterface>* cache_manager;
  NiceMock<MockPolicyListener> listener;
  NiceMock<MockUpdateStatusManager> update_status_manager;
  const std::string device_id;
  std::shared_ptr<access_remote_test::MockAccessRemote> access_remote;
  PolicyTableSPtr default_pt_snapshot_;

  void SetUp() OVERRIDE {
    manager = new PolicyManagerImpl();
    manager->set_listener(&listener);
    cache_manager = new NiceMock<MockCacheManagerInterface>();
    manager->set_cache_manager(cache_manager);
    access_remote = std::make_shared<access_remote_test::MockAccessRemote>();
    manager->set_access_remote(access_remote);
    // Json::Value table = createPTforLoad();
    default_pt_snapshot_ = std::make_shared<policy_table::Table>();
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

  void PrepareUpdateWithFunctionalGroupingContent(policy_table::Table& update) {
    using namespace application_manager;

    update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

    policy_table::Rpcs rpcs;
    rpcs.mark_initialized();

    policy_table::Rpc rpc;
    rpc.mark_initialized();

    policy_table::RpcParameters rpc_params;
    rpc_params.mark_initialized();
    rpc_params.hmi_levels.push_back(
        rpc::policy_table_interface_base::HmiLevel::HL_FULL);
    rpc_params.hmi_levels.push_back(
        rpc::policy_table_interface_base::HmiLevel::HL_BACKGROUND);

    (*rpc_params.parameters).push_back(kRPM);
    (*rpc_params.parameters).push_back(kSpeed);

    rpc["GetVehicleData"] = rpc_params;
    rpcs.rpcs = rpc;

    policy_table::FunctionalGroupings fg;
    fg["TestGroup1"] = rpcs;

    update.policy_table.functional_groupings = fg;

    policy_table::ApplicationParams app_params;
    app_params.mark_initialized();
    app_params.priority =
        rpc::policy_table_interface_base::Priority::P_COMMUNICATION;
    app_params.groups.push_back("TestGroup1");
    update.policy_table.app_policies_section.apps["1234"] = app_params;
  }

  void ExpectOnPermissionsUpdated() {
    std::string dev_id_1 = "dev_id_1";
    std::string app_id = "1234";

    EXPECT_CALL(*cache_manager, IsDefaultPolicy(app_id)).WillOnce(Return(true));
    EXPECT_CALL(*access_remote, IsAppRemoteControl(_)).WillOnce(Return(true));

    FunctionalGroupNames fg_names;
    fg_names[0] =
        std::make_pair<std::string, std::string>("fg_name_1", "fg_name_2");
    EXPECT_CALL(*cache_manager, GetFunctionalGroupNames(_))
        .WillOnce(DoAll(SetArgReferee<0>(fg_names), Return(true)));

    std::vector<std::string> device_ids;
    device_ids.push_back(dev_id_1);
    EXPECT_CALL(listener, GetDevicesIds(app_id))
        .WillRepeatedly(Return(device_ids));

    int32_t group_id = 0;
    FunctionalGroupIDs group_ids;
    group_ids.push_back(group_id);
    FunctionalIdType group_types;
    group_types[GroupType::kTypeGeneral] = group_ids;
    EXPECT_CALL(*access_remote, GetPermissionsForApp(dev_id_1, app_id, _))
        .WillRepeatedly(DoAll(SetArgReferee<2>(group_types), Return(true)));

    EXPECT_CALL(listener, OnPermissionsUpdated(dev_id_1, app_id, _, _))
        .Times(1);

    EXPECT_CALL(listener, GetAppName(_))
        .WillOnce(Return(custom_str::CustomString("")));

    EXPECT_CALL(listener, OnUpdateStatusChanged(_)).Times(1);
  }
};

class PolicyManagerImplTest2 : public ::testing::Test {
 public:
  PolicyManagerImplTest2()
      : app_id1("123456789")
      , app_id2("1010101010")
      , dev_id1("XXX123456789ZZZ")
      , dev_id2("08-00-27-CE-76-FE")
      , dev_handle1(123456789)
      , dev_handle2(1010101010)
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
  const connection_handler::DeviceHandle dev_handle1;
  const connection_handler::DeviceHandle dev_handle2;
  Json::Value PTU_request_types;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;
  const std::string kAppStorageFolder = "storage_PolicyManagerImplTest2";

  void SetUp() OVERRIDE {
    file_system::CreateDirectory(kAppStorageFolder);
    file_system::DeleteFile("policy.sqlite");

    manager = new PolicyManagerImpl();
    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
    ON_CALL(policy_settings_, use_full_app_id()).WillByDefault(Return(true));
    manager->set_listener(&listener);
    const char* levels[] = {"FULL", "LIMITED", "BACKGROUND", "NONE"};
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
    Json::CharReaderBuilder reader_builder;
    std::string json;
    Json::Value root(Json::objectValue);
    if (ifile.is_open() &&
        Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
      json = root.toStyledString();
    }
    ifile.close();
    ::policy::BinaryMessage msg(json.begin(), json.end());
    // Load Json to cache
    EXPECT_EQ(PolicyManager::PtProcessingResult::kSuccess,
              manager->LoadPT("file_pt_update.json", msg));
    manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
    return root;
  }

  void CreateLocalPT(const std::string& file_name) {
    file_system::remove_directory_content(kAppStorageFolder);
    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
    ON_CALL(policy_settings_, use_full_app_id()).WillByDefault(Return(true));
    ASSERT_TRUE(manager->InitPT(file_name, &policy_settings_));
  }

  Json::Value AddWidgetSupportToPt(const std::string& section_name,
                                   const uint32_t group_number) {
    std::ifstream ifile("sdl_preloaded_pt.json");
    Json::CharReaderBuilder reader_builder;
    Json::Value root(Json::objectValue);
    if (ifile.is_open() &&
        Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
      auto& groups =
          root["policy_table"]["app_policies"][section_name]["groups"];
      if (groups.empty()) {
        groups = Json::Value(Json::arrayValue);
      }
      groups[group_number] = Json::Value("WidgetSupport");
    }
    ifile.close();
    return root;
  }

  std::string AddWidgetSupportToPt(Json::Value* root,
                                   const std::string& section_name,
                                   const uint32_t group_number) {
    if (root) {
      auto& groups =
          (*root)["policy_table"]["app_policies"][section_name]["groups"];
      if (groups.empty()) {
        groups = Json::Value(Json::arrayValue);
      }
      groups[group_number] = Json::Value("WidgetSupport");
      return root->toStyledString();
    }
    return std::string();
  }

  std::string AddWidgetSupportToFunctionalGroups(Json::Value* root,
                                                 const std::string& rpc_name,
                                                 const std::string& hmi_level) {
    if (root) {
      Json::Value val(Json::objectValue);
      Json::Value val2(Json::arrayValue);
      val2[0] = hmi_level;
      val[rpc_name]["hmi_levels"] = val2;
      (*root)["policy_table"]["functional_groupings"]["WidgetSupport"]["rpcs"] =
          val;
      return root->toStyledString();
    }
    return std::string();
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
    manager->AddApplication(
        dev_id1, section_name, HmiTypes(policy_table::AHT_DEFAULT));
    // Check app gets RequestTypes from pre_DataConsent of app_policies
    // section
    PT_request_types = manager->GetAppRequestTypes(section_name);
    EXPECT_EQ(rt_number, PT_request_types.size());
    EXPECT_CALL(listener, OnPendingPermissionChange(_, section_name)).Times(1);
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
        manager->GetAppPermissionsChanges(dev_id1, section_name);
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
    std::shared_ptr<policy_table::Table> table = cache->GenerateSnapshot();
    // Set functional groupings from policy table
    input_functional_groupings = table->policy_table.functional_groupings;
  }

  void TearDown() OVERRIDE {
    delete manager;
    file_system::remove_directory_content(kAppStorageFolder);
    file_system::RemoveDirectory(kAppStorageFolder, true);
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

TEST_F(PolicyManagerImplTest, GetNotificationsNumber) {
  const std::string priority = "EMERGENCY";
  const uint32_t notif_number = 100u;
  EXPECT_CALL(*cache_manager, GetNotificationsNumber(priority))
      .WillOnce(Return(notif_number));

  EXPECT_EQ(notif_number, manager->GetNotificationsNumber(priority));
}

TEST_F(PolicyManagerImplTest2, GetNotificationsNumberAfterPTUpdate) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  Json::Value table = CreatePTforLoad();
  manager->ForcePTExchange();
  manager->SetSendOnUpdateSentOut(false);
  manager->OnUpdateStarted();
  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);
  // Act
  const std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));

  EXPECT_CALL(listener, OnUpdateStatusChanged(_));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);

  std::string priority = "EMERGENCY";
  uint32_t notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(1u, notif_number);

  priority = "NAVIGATION";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(2u, notif_number);

  priority = "EMERGENCY";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(1u, notif_number);

  priority = "VOICECOM";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(3u, notif_number);

  priority = "NORMAL";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(5u, notif_number);

  priority = "NONE";
  notif_number = manager->GetNotificationsNumber(priority);
  EXPECT_EQ(6u, notif_number);
}

TEST_F(PolicyManagerImplTest2, IsAppRevoked_SetRevokedAppID_ExpectAppRevoked) {
  CreateLocalPT("sdl_preloaded_pt.json");
  // Arrange
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::CharReaderBuilder reader_builder;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() &&
      Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
    root["policy_table"]["app_policies"][app_id1] = Json::nullValue;
    json = root.toStyledString();
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);

  EXPECT_TRUE(manager->IsApplicationRevoked(app_id1));
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_SetRevokedAppID_ExpectRPCDisallowed) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  (manager->GetCache())->AddDevice(dev_id1, "Bluetooth");
  (manager->GetCache())
      ->SetDeviceData(dev_id1,
                      "hardware IPX",
                      "v.8.0.1",
                      "Android",
                      "4.4.2",
                      "Life",
                      2,
                      "Bluetooth");
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(_, app_id1))
      .WillRepeatedly(Return(dev_id1));
  manager->SetUserConsentForDevice(dev_id1, true);
  // Add app from consented device. App will be assigned with default policies
  manager->AddApplication(
      dev_id1, app_id1, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_CALL(listener, GetDevicesIds(app_id1))
      .WillRepeatedly(Return(transport_manager::DeviceList()));
  // Check before action
  policy_table::RpcParameters rpc_parameters;
  rpc_parameters.hmi_levels.push_back(policy_table::HL_FULL);

  policy_table::Rpc rpc;
  rpc["Alert"] = rpc_parameters;

  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;

  manager->CheckPermissions(
      dev_id1, app_id1, std::string("FULL"), "Alert", input_params, output);

  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());
  // Act
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::CharReaderBuilder reader_builder;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() &&
      Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
    root["policy_table"]["app_policies"][app_id1] = Json::nullValue;
    json = root.toStyledString();
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);

  manager->CheckPermissions(
      dev_id1, app_id1, std::string("FULL"), "Alert", input_params, output);
  // Check RPC is disallowed
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());
}

TEST_F(
    PolicyManagerImplTest2,
    CheckPermissions_PersistsWidgetAppPermissionsAfter_PTU_ExpectRPCAllowed) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  (manager->GetCache())->AddDevice(dev_id1, "Bluetooth");
  (manager->GetCache())
      ->SetDeviceData(dev_id1,
                      "hardware IPX",
                      "v.8.0.1",
                      "Android",
                      "4.4.2",
                      "Life",
                      2,
                      "Bluetooth");
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(_, app_id1))
      .WillRepeatedly(Return(dev_id1));
  manager->SetUserConsentForDevice(dev_id1, true);
  // Add app from consented device. App will be assigned with default policies
  manager->AddApplication(
      app_id1, app_id1, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_CALL(listener, GetDevicesIds(app_id1))
      .WillRepeatedly(Return(transport_manager::DeviceList()));
  // Act
  const char* const rpc_name = "CreateWindow";
  const char* const hmi_level = "NONE";
  const uint32_t group_number = 0;
  Json::Value root = AddWidgetSupportToPt("default", group_number);
  std::string json =
      AddWidgetSupportToFunctionalGroups(&root, rpc_name, hmi_level);

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);

  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;

  manager->CheckPermissions(
      dev_id1, app_id1, hmi_level, rpc_name, input_params, output);

  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());
  // Act
  json = AddWidgetSupportToPt(&root, app_id1, group_number);
  msg = BinaryMessage(json.begin(), json.end());
  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);

  output.hmi_level_permitted = ::policy::kRpcDisallowed;
  manager->CheckPermissions(
      dev_id1, app_id1, hmi_level, rpc_name, input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());
}

TEST_F(
    PolicyManagerImplTest2,
    CheckPermissions_AbsenceOfWidgetPermissionsAfter_PTU_ExpectRPCDisallowed) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  (manager->GetCache())->AddDevice(dev_id1, "Bluetooth");
  (manager->GetCache())
      ->SetDeviceData(dev_id1,
                      "hardware IPX",
                      "v.8.0.1",
                      "Android",
                      "4.4.2",
                      "Life",
                      2,
                      "Bluetooth");
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(_, app_id1))
      .WillRepeatedly(Return(dev_id1));
  manager->SetUserConsentForDevice(dev_id1, true);
  // Add app from consented device. App will be assigned with default policies
  manager->AddApplication(
      dev_id1, app_id1, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_CALL(listener, GetDevicesIds(app_id1))
      .WillRepeatedly(Return(transport_manager::DeviceList()));
  // Act
  const char* const rpc_name = "DeleteWindow";
  const char* const hmi_level = "NONE";
  const uint32_t group_number = 0;
  Json::Value root = AddWidgetSupportToPt("default", group_number);
  std::string json =
      AddWidgetSupportToFunctionalGroups(&root, rpc_name, hmi_level);

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);

  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;

  manager->CheckPermissions(
      dev_id1, app_id1, hmi_level, rpc_name, input_params, output);

  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());
  output.hmi_level_permitted = ::policy::kRpcDisallowed;
  // Act
  root["policy_table"]["app_policies"][app_id1]["groups"] =
      Json::Value(Json::arrayValue);
  root["policy_table"]["app_policies"][app_id1]["groups"][group_number] =
      Json::Value("Base-4");
  json = root.toStyledString();
  msg = BinaryMessage(json.begin(), json.end());
  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);

  manager->CheckPermissions(
      dev_id1, app_id1, hmi_level, rpc_name, input_params, output);
  // Check RPC is disallowed
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());
}

TEST_F(PolicyManagerImplTest2,
       CheckPermissions_SetAppIDwithPolicies_ExpectRPCAllowed) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddDevice(dev_id1, "Bluetooth");

  ASSERT_TRUE((manager->GetCache())
                  ->SetDeviceData(dev_id1,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(_, "1234"))
      .WillRepeatedly(Return(dev_id1));
  manager->SetUserConsentForDevice(dev_id1, true);
  // Add app from consented device. App will be assigned with default policies
  manager->AddApplication(dev_id1, "1234", HmiTypes(policy_table::AHT_MEDIA));
  // Emulate PTU with new policies for app added above
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::CharReaderBuilder reader_builder;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() &&
      Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
    // Add AppID with policies
    root["policy_table"]["app_policies"]["1234"] =
        Json::Value(Json::objectValue);
    root["policy_table"]["app_policies"]["1234"]["memory_kb"] = Json::Value(50);
    root["policy_table"]["app_policies"]["1234"]["heart_beat_timeout_ms"] =
        Json::Value(100);
    root["policy_table"]["app_policies"]["1234"]["AppHMIType"] =
        Json::Value(Json::arrayValue);
    root["policy_table"]["app_policies"]["1234"]["AppHMIType"][0] =
        Json::Value("MEDIA");
    root["policy_table"]["app_policies"]["1234"]["groups"] =
        Json::Value(Json::arrayValue);
    root["policy_table"]["app_policies"]["1234"]["groups"][0] =
        Json::Value("Base-4");
    root["policy_table"]["app_policies"]["1234"]["priority"] =
        Json::Value("EMERGENCY");
    root["policy_table"]["app_policies"]["1234"]["default_hmi"] =
        Json::Value("FULL");
    root["policy_table"]["app_policies"]["1234"]["keep_context"] =
        Json::Value(true);
    root["policy_table"]["app_policies"]["1234"]["steal_focus"] =
        Json::Value(true);
    root["policy_table"]["app_policies"]["1234"]["certificate"] =
        Json::Value("sign");
    json = root.toStyledString();
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  // Load Json to cache
  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);

  policy_table::RpcParameters rpc_parameters;
  rpc_parameters.hmi_levels.push_back(policy_table::HL_FULL);

  policy_table::Rpc rpc;
  rpc["Alert"] = rpc_parameters;
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;

  (manager->GetCache())->AddDevice(dev_id1, "Bluetooth");
  (manager->GetCache())
      ->SetDeviceData(dev_id1,
                      "hardware IPX",
                      "v.8.0.1",
                      "Android",
                      "4.4.2",
                      "Life",
                      2,
                      "Bluetooth");
  manager->CheckPermissions(dev_id1,
                            std::string("1234"),
                            std::string("FULL"),
                            "Alert",
                            input_params,
                            output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  // Check list of parameters empty
  ASSERT_TRUE(output.list_of_allowed_params.empty());
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
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillOnce(Return(default_pt_snapshot_));
  manager->ForcePTExchange();
  manager->OnUpdateStarted();
  Json::Value table = CreatePTforLoad();
  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  // Assert
  ASSERT_TRUE(IsValid(update));

  EXPECT_CALL(*cache_manager, GetHMIAppTypeAfterUpdate(_)).Times(AtLeast(1));

  // Act
  const std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  std::shared_ptr<policy_table::Table> snapshot =
      std::make_shared<policy_table::Table>(update.policy_table);
  // Assert
  EXPECT_CALL(*cache_manager, GenerateSnapshot()).WillOnce(Return(snapshot));
  EXPECT_CALL(*cache_manager, GetVehicleDataItems())
      .WillOnce(Return(std::vector<policy_table::VehicleDataItem>()));
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).WillOnce(Return(true));
  EXPECT_CALL(listener, GetDevicesIds("1234"))
      .WillRepeatedly(Return(transport_manager::DeviceList()));
  EXPECT_CALL(*cache_manager, SaveUpdateRequired(false));
  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));

  EXPECT_CALL(*cache_manager, TimeoutResponse());
  EXPECT_CALL(*cache_manager, SecondsBetweenRetries(_));
  EXPECT_CALL(listener, OnUpdateStatusChanged(_));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
}

TEST_F(PolicyManagerImplTest, LoadPT_FunctionalGroup_removeRPC_SendUpdate) {
  // Arrange
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillOnce(Return(default_pt_snapshot_));
  manager->ForcePTExchange();
  manager->OnUpdateStarted();
  Json::Value table = CreatePTforLoad();
  policy_table::Table update(&table);

  PrepareUpdateWithFunctionalGroupingContent(update);

  std::shared_ptr<policy_table::Table> snapshot =
      std::make_shared<policy_table::Table>(update.policy_table);

  const auto& fg_found =
      update.policy_table.functional_groupings.find("TestGroup1");
  fg_found->second.rpcs.erase("GetVehicleData");

  ASSERT_TRUE(IsValid(update));

  const std::string json = update.ToJsonValue().toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  // Assert
  EXPECT_CALL(*cache_manager, GetVehicleDataItems())
      .WillOnce(Return(std::vector<policy_table::VehicleDataItem>()));
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillRepeatedly(Return(snapshot));
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).WillOnce(Return(true));
  ExpectOnPermissionsUpdated();

  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
}

TEST_F(PolicyManagerImplTest,
       LoadPT_FunctionalGroup_removeRPCParams_SendUpdate) {
  // Arrange
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillOnce(Return(default_pt_snapshot_));
  manager->ForcePTExchange();
  manager->OnUpdateStarted();
  Json::Value table = CreatePTforLoad();
  policy_table::Table update(&table);

  PrepareUpdateWithFunctionalGroupingContent(update);

  std::shared_ptr<policy_table::Table> snapshot =
      std::make_shared<policy_table::Table>(update.policy_table);

  const auto& fg_found =
      update.policy_table.functional_groupings.find("TestGroup1");
  policy_table::RpcParameters& new_rpc_params =
      fg_found->second.rpcs["GetVehicleData"];
  new_rpc_params.parameters->erase(new_rpc_params.parameters->begin());

  ASSERT_TRUE(IsValid(update));

  const std::string json = update.ToJsonValue().toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  // Assert
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillRepeatedly(Return(snapshot));
  EXPECT_CALL(*cache_manager, GetVehicleDataItems())
      .WillOnce(Return(std::vector<policy_table::VehicleDataItem>()));
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).WillOnce(Return(true));
  ExpectOnPermissionsUpdated();

  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
}

TEST_F(PolicyManagerImplTest,
       LoadPT_FunctionalGroup_removeRPC_HMILevels_SendUpdate) {
  // Arrange
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillOnce(Return(default_pt_snapshot_));
  manager->ForcePTExchange();
  manager->OnUpdateStarted();
  Json::Value table = CreatePTforLoad();
  policy_table::Table update(&table);

  PrepareUpdateWithFunctionalGroupingContent(update);

  std::shared_ptr<policy_table::Table> snapshot =
      std::make_shared<policy_table::Table>(update.policy_table);

  const auto& fg_found =
      update.policy_table.functional_groupings.find("TestGroup1");
  policy_table::RpcParameters& new_rpc_params =
      fg_found->second.rpcs["GetVehicleData"];
  new_rpc_params.hmi_levels.erase(new_rpc_params.hmi_levels.begin());

  ASSERT_TRUE(IsValid(update));

  const std::string json = update.ToJsonValue().toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  // Assert
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillRepeatedly(Return(snapshot));
  EXPECT_CALL(*cache_manager, GetVehicleDataItems())
      .WillOnce(Return(std::vector<policy_table::VehicleDataItem>()));
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).WillOnce(Return(true));
  ExpectOnPermissionsUpdated();

  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
}

TEST_F(PolicyManagerImplTest,
       LoadPT_FunctionalGroup_addRPC_HMILevels_SendUpdate) {
  // Arrange
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillOnce(Return(default_pt_snapshot_));
  manager->ForcePTExchange();
  manager->OnUpdateStarted();
  Json::Value table = CreatePTforLoad();
  policy_table::Table update(&table);

  PrepareUpdateWithFunctionalGroupingContent(update);

  std::shared_ptr<policy_table::Table> snapshot =
      std::make_shared<policy_table::Table>(update.policy_table);

  const auto& fg_found =
      update.policy_table.functional_groupings.find("TestGroup1");
  policy_table::RpcParameters& new_rpc_params =
      fg_found->second.rpcs["GetVehicleData"];
  new_rpc_params.hmi_levels.push_back(
      rpc::policy_table_interface_base::HmiLevel::HL_LIMITED);

  ASSERT_TRUE(IsValid(update));

  const std::string json = update.ToJsonValue().toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  // Assert
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillRepeatedly(Return(snapshot));
  EXPECT_CALL(*cache_manager, GetVehicleDataItems())
      .WillOnce(Return(std::vector<policy_table::VehicleDataItem>()));
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).WillOnce(Return(true));
  ExpectOnPermissionsUpdated();

  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
}

TEST_F(PolicyManagerImplTest, LoadPT_FunctionalGroup_addRPCParams_SendUpdate) {
  using namespace application_manager;
  // Arrange
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillOnce(Return(default_pt_snapshot_));
  manager->ForcePTExchange();
  manager->OnUpdateStarted();
  Json::Value table = CreatePTforLoad();
  policy_table::Table update(&table);

  PrepareUpdateWithFunctionalGroupingContent(update);

  std::shared_ptr<policy_table::Table> snapshot =
      std::make_shared<policy_table::Table>(update.policy_table);

  const auto& fg_found =
      update.policy_table.functional_groupings.find("TestGroup1");
  policy_table::RpcParameters& new_rpc_params =
      fg_found->second.rpcs["GetVehicleData"];
  (*new_rpc_params.parameters).push_back(kFuelLevel);

  ASSERT_TRUE(IsValid(update));

  const std::string json = update.ToJsonValue().toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  // Assert
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillRepeatedly(Return(snapshot));
  EXPECT_CALL(*cache_manager, GetVehicleDataItems())
      .WillOnce(Return(std::vector<policy_table::VehicleDataItem>()));
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).WillOnce(Return(true));
  ExpectOnPermissionsUpdated();

  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
}

TEST_F(PolicyManagerImplTest, LoadPT_FunctionalGroup_NoUpdate_DONT_SendUpdate) {
  // Arrange
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillOnce(Return(default_pt_snapshot_));
  manager->ForcePTExchange();
  manager->OnUpdateStarted();
  Json::Value table = CreatePTforLoad();
  policy_table::Table update(&table);

  PrepareUpdateWithFunctionalGroupingContent(update);

  std::shared_ptr<policy_table::Table> snapshot =
      std::make_shared<policy_table::Table>(update.policy_table);

  ASSERT_TRUE(IsValid(update));

  const std::string json = update.ToJsonValue().toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  // Assert
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillRepeatedly(Return(snapshot));
  EXPECT_CALL(*cache_manager, GetVehicleDataItems())
      .WillOnce(Return(std::vector<policy_table::VehicleDataItem>()));
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).WillOnce(Return(true));

  ASSERT_EQ(PolicyManager::PtProcessingResult::kSuccess,
            manager->LoadPT("file_pt_update.json", msg));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kSuccess);
}

TEST_F(PolicyManagerImplTest, LoadPT_SetInvalidUpdatePT_PTIsNotLoaded) {
  // Arrange
  Json::Value table(Json::objectValue);
  EXPECT_CALL(*cache_manager, GenerateSnapshot())
      .WillOnce(Return(default_pt_snapshot_));
  manager->ForcePTExchange();
  manager->OnUpdateStarted();

  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  // Assert update is invalid
  ASSERT_FALSE(IsValid(update));

  // Act
  std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  std::shared_ptr<policy_table::Table> snapshot =
      std::make_shared<policy_table::Table>(update.policy_table);
  ON_CALL(*cache_manager, GenerateSnapshot()).WillByDefault(Return(snapshot));
  ON_CALL(*cache_manager, GetVehicleDataItems())
      .WillByDefault(Return(std::vector<policy_table::VehicleDataItem>()));

  // Assert
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).Times(0);
  EXPECT_CALL(listener, GetAppName(_)).Times(0);
  EXPECT_CALL(*cache_manager, SaveUpdateRequired(false)).Times(0);
  EXPECT_CALL(*cache_manager, TimeoutResponse()).Times(0);
  EXPECT_CALL(*cache_manager, SecondsBetweenRetries(_)).Times(0);
  ASSERT_EQ(PolicyManager::PtProcessingResult::kWrongPtReceived,
            manager->LoadPT("file_pt_update.json", msg));

  EXPECT_CALL(listener, OnUpdateStatusChanged(_));
  manager->OnPTUFinished(PolicyManager::PtProcessingResult::kWrongPtReceived);
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
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  // Set kms changed and exceed limit
  manager->KmsChanged(52500);
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(
    PolicyManagerImplTest2,
    AddApplication_AddNewApplicationFromDeviceWithoutConsent_ExpectUpdateRequired) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ::policy::StatusNotifier notifyer = manager->AddApplication(
      dev_id1, app_id1, HmiTypes(policy_table::AHT_DEFAULT));
  DCHECK(notifyer);
  (*notifyer)();
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(
    PolicyManagerImplTest2,
    ReactOnUserDevConsentForApp_AddNewApplicationFromDeviceWithConsent_ExpectDefault) {
  // Arrange
  // RequestTypes for default & preDataConsent are different
  CreateLocalPT("ptu_requestType.json");
  manager->AddApplication(
      dev_id1, app_id1, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_FALSE(manager->IsPredataPolicy(app_id1));
  manager->ReactOnUserDevConsentForApp(dev_handle1, app_id1, true);
  EXPECT_FALSE(manager->IsPredataPolicy(app_id1));
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id1));
}

TEST_F(
    PolicyManagerImplTest2,
    ReactOnUserDevConsentForApp_AddNewApplicationFromDeviceWithConsent_ExpectPreDataConsent) {
  // Arrange
  // RequestTypes for default & preDataConsent are the same
  CreateLocalPT("ptu2_requestType.json");
  manager->AddApplication(
      dev_id1, app_id1, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_FALSE(manager->IsPredataPolicy(app_id1));
  EXPECT_CALL(listener, OnPendingPermissionChange(_, app_id1)).Times(0);
  manager->ReactOnUserDevConsentForApp(dev_handle1, app_id1, true);
  EXPECT_FALSE(manager->IsPredataPolicy(app_id1));
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id1));
}

TEST_F(
    PolicyManagerImplTest2,
    AddApplication_AddExistingApplicationFromDeviceWithoutConsent_ExpectNoUpdateRequired) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  ON_CALL(listener, GetDevicesIds(_))
      .WillByDefault(Return(transport_manager::DeviceList()));
  GetPTU("valid_sdl_pt_update.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  // Try to add existing app
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  // Check no update required
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, UpdateApplication_AppServices) {
  std::string kServiceType = "MEDIA";
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  ON_CALL(listener, GetDevicesIds(_))
      .WillByDefault(Return(transport_manager::DeviceList()));
  GetPTU("valid_sdl_pt_update.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  // Try to add existing app
  policy_table::AppServiceParameters app_service_parameters =
      policy_table::AppServiceParameters();
  manager->GetAppServiceParameters(app_id2, &app_service_parameters);

  ASSERT_FALSE(app_service_parameters.find(kServiceType) ==
               app_service_parameters.end());

  auto service_names = *(app_service_parameters[kServiceType].service_names);

  ASSERT_TRUE(service_names.is_initialized());
  ASSERT_EQ(service_names.size(), 2u);
  EXPECT_EQ(static_cast<std::string>(service_names[0]), "SDL App");
  EXPECT_EQ(static_cast<std::string>(service_names[1]), "SDL Music");

  auto handled_rpcs = app_service_parameters[kServiceType].handled_rpcs;

  ASSERT_TRUE(handled_rpcs.is_initialized());
  EXPECT_EQ(handled_rpcs[0].function_id, 41);

  // Check no update required
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2,
       PTUpdatedAt_DaysNotExceedLimit_ExpectNoUpdateRequired) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  date_time::TimeDuration current_time = date_time::getCurrentTime();
  const int kSecondsInDay = 60 * 60 * 24;
  int days = date_time::getSecs(current_time) / kSecondsInDay;
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  ON_CALL(listener, GetDevicesIds(_))
      .WillByDefault(Return(transport_manager::DeviceList()));

  GetPTU("valid_sdl_pt_update.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());

  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  ::policy::Counters counter = ::policy::Counters::DAYS_AFTER_EPOCH;
  // Set PT was updated 10 days ago (limit is 30 days for now)
  // So no limit exceeded
  manager->PTUpdatedAt(counter, days - 10);
  manager->OnAppRegisteredOnMobile(dev_id2, app_id2);
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

TEST_F(PolicyManagerImplTest2, ResetRetrySequence) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->ResetRetrySequence(ResetRetryCountType::kResetWithStatusUpdate);
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
  manager->SetSendOnUpdateSentOut(false);
  manager->OnUpdateStarted();
  EXPECT_EQ("UPDATING", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, NextRetryTimeout_ExpectTimeoutsFromPT) {
  // Arrange
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() &&
      Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
    Json::Value seconds_between_retries = Json::Value(Json::arrayValue);
    seconds_between_retries =
        root["policy_table"]["module_config"]["seconds_between_retries"];
    CreateLocalPT("sdl_preloaded_pt.json");
    // Check data
    uint32_t timeout_after_x_seconds =
        root["policy_table"]["module_config"]["timeout_after_x_seconds"]
            .asInt() *
        date_time::MILLISECONDS_IN_SECOND;
    const uint32_t first_retry = timeout_after_x_seconds;
    EXPECT_EQ(first_retry, manager->NextRetryTimeout());
    uint32_t next_retry = first_retry + seconds_between_retries[0].asInt() *
                                            date_time::MILLISECONDS_IN_SECOND;
    EXPECT_EQ(next_retry, manager->NextRetryTimeout());
    next_retry =
        first_retry + next_retry +
        seconds_between_retries[1].asInt() * date_time::MILLISECONDS_IN_SECOND;
    EXPECT_EQ(next_retry, manager->NextRetryTimeout());
    next_retry =
        first_retry + next_retry +
        seconds_between_retries[2].asInt() * date_time::MILLISECONDS_IN_SECOND;
    EXPECT_EQ(next_retry, manager->NextRetryTimeout());
    next_retry =
        first_retry + next_retry +
        seconds_between_retries[3].asInt() * date_time::MILLISECONDS_IN_SECOND;
    EXPECT_EQ(next_retry, manager->NextRetryTimeout());
  }
}

TEST_F(PolicyManagerImplTest2, TimeOutExchange) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  // Check value taken from PT
  EXPECT_EQ(70000u, manager->TimeoutExchangeMSec());
}

TEST_F(PolicyManagerImplTest2, UpdatedPreloadedPT_ExpectLPT_IsUpdated) {
  // Arrange necessary pre-conditions
  StringsForUpdate new_data;
  new_data.new_field_name_ = "Notifications-";
  CreateNewRandomData(new_data);
  // Create Initial LocalPT from preloadedPT
  CreateLocalPT("sdl_preloaded_pt.json");
  // Update preloadedPT
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);

  if (ifile.is_open() &&
      Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
    root["policy_table"]["module_config"]["preloaded_date"] =
        new_data.new_date_;
    Json::Value val(Json::objectValue);
    Json::Value val2(Json::arrayValue);
    val2[0] = hmi_level[index];
    val[new_data.new_field_value_]["hmi_levels"] = val2;
    root["policy_table"]["functional_groupings"][new_data.new_field_name_]
        ["rpcs"] = val;
    root["policy_table"]["functional_groupings"][new_data.new_field_name_]
        ["user_consent_prompt"] = new_data.new_field_name_;
  }
  ifile.close();

  std::ofstream ofile("sdl_preloaded_pt.json");
  ofile << root;
  ofile.flush();
  ofile.close();

  //  Make PolicyManager to update LocalPT
  EXPECT_TRUE(manager->InitPT("sdl_preloaded_pt.json", &policy_settings_));

  // Arrange
  ::policy::CacheManagerInterfaceSPtr cache = manager->GetCache();
  std::shared_ptr<policy_table::Table> table = cache->GenerateSnapshot();
  // Get FunctionalGroupings
  policy_table::FunctionalGroupings& fc =
      table->policy_table.functional_groupings;
  // Get RPCs for new added field in functional_group
  policy_table::Rpcs& rpcs = fc[new_data.new_field_name_];
  // Get user consent prompt
  const std::string& ucp = *(rpcs.user_consent_prompt);
  // Get Rpcs
  policy_table::Rpc& rpc = rpcs.rpcs;
  // Get RPC's parameters
  policy_table::RpcParameters& rpc_param = rpc[new_data.new_field_value_];

  // Check preloaded date
  EXPECT_EQ(static_cast<std::string>(
                *(table->policy_table.module_config.preloaded_date)),
            new_data.new_date_);
  // Check if new field exists in Local PT
  EXPECT_TRUE(fc.find(new_data.new_field_name_) != fc.end());
  // Check if user_consent_propmp is correct
  EXPECT_EQ(new_data.new_field_name_, ucp);
  // Check if new RPC exists
  EXPECT_TRUE(rpc.find(new_data.new_field_value_) != rpc.end());
  // Check HMI level of new RPC
  EXPECT_EQ(index, static_cast<uint32_t>(rpc_param.hmi_levels[0]));
  // Check if new field matches field added to preloaded PT
  EXPECT_EQ(std::string((*(fc.find(new_data.new_field_name_))).first),
            new_data.new_field_name_);
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
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() &&
      Json::parseFromStream(reader_builder, ifile, &root, nullptr)) {
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
  manager->ForcePTExchange();
  manager->OnExceededTimeout();
  // Check
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2,
       GetUserConsentForDevice_SetDeviceAllowed_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2, "Bluetooth"));

  ASSERT_TRUE((manager->GetCache())
                  ->SetDeviceData(dev_id2,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));

  manager->SetUserConsentForDevice(dev_id2, true);
  ::policy::DeviceConsent consent = manager->GetUserConsentForDevice(dev_id2);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
}

TEST_F(PolicyManagerImplTest2,
       DISABLED_GetDefaultHmi_SetDeviceAllowed_ExpectReceivedHmiCorrect) {
  // Arrange
  CreateLocalPT("ptu2_requestType.json");
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  // Check if app has preData policy
  EXPECT_FALSE(manager->IsPredataPolicy(app_id2));
  std::string default_hmi1;
  manager->GetDefaultHmi(dev_id2, app_id2, &default_hmi1);
  EXPECT_EQ("", default_hmi1);
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2, "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())
                  ->SetDeviceData(dev_id2,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  manager->SetUserConsentForDevice(dev_id2, true);
  ::policy::DeviceConsent consent = manager->GetUserConsentForDevice(dev_id2);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(_, app_id2))
      .WillRepeatedly(Return(dev_id2));
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id2));
  std::string default_hmi2;
  manager->GetDefaultHmi(dev_id2, app_id2, &default_hmi2);
  EXPECT_EQ("", default_hmi2);
}

TEST_F(PolicyManagerImplTest2,
       GetDefaultPriority_SetDeviceAllowed_ExpectReceivedPriorityCorrect) {
  // Arrange
  CreateLocalPT("ptu2_requestType.json");
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  // Check if app has preData policy
  EXPECT_FALSE(manager->IsPredataPolicy(app_id2));
  std::string priority1;
  EXPECT_TRUE(manager->GetPriority(app_id2, &priority1));
  EXPECT_EQ("EMERGENCY", priority1);
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2, "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())
                  ->SetDeviceData(dev_id2,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  manager->SetUserConsentForDevice(dev_id2, true);
  ::policy::DeviceConsent consent = manager->GetUserConsentForDevice(dev_id2);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(_, app_id2))
      .WillRepeatedly(Return(dev_id2));
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id2));
  std::string priority2;
  EXPECT_TRUE(manager->GetPriority(app_id2, &priority2));
  EXPECT_EQ("EMERGENCY", priority2);
}

TEST_F(PolicyManagerImplTest2, GetInitialAppData_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_CALL(listener, GetDevicesIds(app_id2))
      .WillRepeatedly(Return(transport_manager::DeviceList()));
  ::policy::StringArray app_nicknames;
  ::policy::StringArray app_hmi_types;
  manager->GetInitialAppData(app_id2, &app_nicknames, &app_hmi_types);
  // Expect Empty nicknames and AppHmiTypes
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
  // Check AppHmiTypes match
  for (uint32_t i = 0; i < app_hmi_types_size; ++i) {
    EXPECT_EQ(app_hmi_types1[i], appHmiTypes[i].asString());
  }
}

TEST_F(
    PolicyManagerImplTest2,
    CanAppKeepContext_AddAppFromUnconsentedDevice_ExpectAppCannotKeepContext) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  // Check if app has preData policy
  EXPECT_FALSE(manager->IsPredataPolicy(app_id2));
  // Check keep context in preData policy
  EXPECT_TRUE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2,
       CanAppKeepContext_AddAppFromConsentedDevice_ExpectAppCannotKeepContext) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2, "Bluetooth"));
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  ASSERT_TRUE((manager->GetCache())
                  ->SetDeviceData(dev_id2,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  manager->SetUserConsentForDevice(dev_id2, true);
  ::policy::DeviceConsent consent = manager->GetUserConsentForDevice(dev_id2);
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(_, app_id2))
      .WillRepeatedly(Return(dev_id2));
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id2));
  // Check keep context in default policy
  EXPECT_TRUE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2,
       CanAppKeepContext_SetPoliciesForAppUpdated_ExpectAppCanKeepContext) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_CALL(listener, GetDevicesIds(app_id2))
      .WillRepeatedly(Return(transport_manager::DeviceList()));
  GetPTU("valid_sdl_pt_update.json");
  // Check keep context in updated policies for app
  EXPECT_TRUE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2,
       CanAppStealFocus_AddAppFromConsentedDevice_ExpectAppCannotStealFocus) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2, "Bluetooth"));
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  ASSERT_TRUE((manager->GetCache())
                  ->SetDeviceData(dev_id2,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));
  manager->SetUserConsentForDevice(dev_id2, true);
  ::policy::DeviceConsent consent = manager->GetUserConsentForDevice(dev_id2);
  EXPECT_EQ(::policy::DeviceConsent::kDeviceAllowed, consent);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(_, app_id2))
      .WillRepeatedly(Return(dev_id2));
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id2));
  // Check keep context in default policy
  EXPECT_TRUE(manager->CanAppStealFocus(app_id2));
}

TEST_F(PolicyManagerImplTest2,
       CanAppStealFocus_SetPoliciesForAppUpdated_ExpectAppCanStealFocus) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_CALL(listener, GetDevicesIds(app_id2))
      .WillRepeatedly(Return(transport_manager::DeviceList()));
  GetPTU("valid_sdl_pt_update.json");
  // Check keep context in updated policies for app
  EXPECT_TRUE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2, GetCurrentDeviceId) {
  // Arrange
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(dev_handle2, app_id2))
      .Times(2);
  EXPECT_EQ(custom_str::CustomString(""),
            manager->GetCurrentDeviceId(dev_handle2, app_id2));
  EXPECT_EQ("", manager->GetCurrentDeviceId(dev_handle2, app_id2));
}

TEST_F(
    PolicyManagerImplTest2,
    GetPermissionsForApp_SetUserConsentForApp_ExpectReceivedPermissionsCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");

  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2, "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())
                  ->SetDeviceData(dev_id2,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));

  ::policy::StringArray consented_groups;
  ::policy::StringArray disallowed_groups;
  consented_groups.push_back(std::string("Notifications"));
  (manager->GetCache())
      ->SetUserPermissionsForDevice(
          dev_id2, consented_groups, disallowed_groups);
  manager->SetUserConsentForDevice(dev_id2, true);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(_, app_id2))
      .WillRepeatedly(Return(dev_id2));
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_CALL(listener, GetDevicesIds(app_id2))
      .WillRepeatedly(Return(transport_manager::DeviceList()));

  GetPTU("valid_sdl_pt_update.json");
  ::policy::PermissionConsent perm_consent;
  perm_consent.device_id = dev_id2;
  perm_consent.policy_app_id = app_id2;
  perm_consent.consent_source = "VR";

  ::policy::FunctionalGroupPermission group1_perm;
  group1_perm.group_alias = "Notifications";
  group1_perm.group_name = "Notifications";
  group1_perm.group_id = ::utils::Djb2HashFromString("Notifications");
  group1_perm.state = ::policy::GroupConsent::kGroupAllowed;

  std::vector< ::policy::FunctionalGroupPermission> groups_permissions;
  groups_permissions.push_back(group1_perm);
  perm_consent.group_permissions = groups_permissions;

  manager->SetUserConsentForApp(perm_consent);
  manager->SendNotificationOnPermissionsUpdated(dev_id2, app_id2);
  std::vector< ::policy::FunctionalGroupPermission> actual_groups_permissions;
  std::vector< ::policy::FunctionalGroupPermission>::iterator it;
  manager->GetPermissionsForApp(dev_id2, app_id2, actual_groups_permissions);
  uint32_t index = 0;
  for (; index < actual_groups_permissions.size(); ++index) {
    if (actual_groups_permissions[index].group_id == group1_perm.group_id) {
      break;
    }
  }
  // Check
  EXPECT_EQ(group1_perm.group_alias,
            actual_groups_permissions[index].group_alias);
  EXPECT_EQ(group1_perm.group_name,
            actual_groups_permissions[index].group_name);
  EXPECT_EQ(group1_perm.group_id, actual_groups_permissions[index].group_id);
  EXPECT_EQ(group1_perm.state, actual_groups_permissions[index].state);
}

TEST_F(
    PolicyManagerImplTest2,
    HertBeatTimeout_AddApp_UpdateAppPolicies_ExpectReceivedHertBeatTimeoutCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  std::shared_ptr<policy_table::Table> pt = (manager->GetCache())->pt();
  ::policy_table::PolicyTableType type1 =
      ::policy_table::PolicyTableType::PT_PRELOADED;
  pt->SetPolicyTableType(type1);
  if (!pt->is_valid()) {
    std::cout << "\nPolicy table is not valid."
              << "\n";
    rpc::ValidationReport report("policy_table");
    pt->ReportErrors(&report);
  }
  // Add new app
  manager->AddApplication(
      dev_id2, app_id2, HmiTypes(policy_table::AHT_DEFAULT));
  EXPECT_CALL(listener, GetDevicesIds(app_id2))
      .WillRepeatedly(Return(transport_manager::DeviceList()));
  uint32_t result = manager->HeartBeatTimeout(app_id2);
  // By default hertbeat timeout is 0
  EXPECT_EQ(0u, result);
  Json::Value root = GetPTU("valid_sdl_pt_update.json");

  ::policy_table::PolicyTableType type2 =
      ::policy_table::PolicyTableType::PT_UPDATE;
  pt->SetPolicyTableType(type2);
  if (!pt->is_valid()) {
    std::cout << "\nPolicy table is not valid."
              << "\n";
    rpc::ValidationReport report("policy_table");
    pt->ReportErrors(&report);
  }

  Json::Value heart_beat_timeout = Json::Value(Json::uintValue);
  heart_beat_timeout =
      root["policy_table"]["app_policies"][app_id2]["heart_beat_timeout_ms"];
  result = manager->HeartBeatTimeout(app_id2);
  EXPECT_EQ(heart_beat_timeout.asUInt(), result);
}

TEST_F(PolicyManagerImplTest2, CacheManager_RemoveRPCSpecVehicleDataItems) {
  policy_table::VehicleDataItems init;
  policy_table::VehicleDataItem test_item;
  test_item.name = "headLampStatus";
  init.push_back(test_item);

  const auto ret = CacheManager::CollectCustomVDItems(init);

  EXPECT_EQ(0u, ret.size());
}

TEST_F(PolicyManagerImplTest2,
       CacheManager_RemoveRPCSpecVehicleDataItemsAndRemainCustom) {
  policy_table::VehicleDataItems init;
  policy_table::VehicleDataItem rpc_spec_item;
  rpc_spec_item.name = "headLampStatus";
  policy_table::VehicleDataItem custom_item;
  custom_item.name = "Custom";
  init.push_back(rpc_spec_item);
  init.push_back(custom_item);

  EXPECT_EQ(2u, init.size());

  const auto ret = CacheManager::CollectCustomVDItems(init);

  EXPECT_EQ(1u, ret.size());
  EXPECT_EQ(ret.at(0).name, "Custom");
}

TEST_F(PolicyManagerImplTest2, CacheManager_RemainCustomVehicleDataItems) {
  policy_table::VehicleDataItems init;
  policy_table::VehicleDataItem custom_item;
  custom_item.name = "Custom";
  init.push_back(custom_item);

  EXPECT_EQ(1u, init.size());

  const auto& ret = CacheManager::CollectCustomVDItems(init);

  EXPECT_EQ(1u, ret.size());
}

TEST_F(PolicyManagerImplTest2,
       CacheManager_CollectCustomItemWithRemainingRPCSpecItem) {
  policy_table::VehicleDataItems init;
  policy_table::VehicleDataItem rpc_spec_item;
  rpc_spec_item.name = "headLampStatus";
  policy_table::VehicleDataItem custom_item;
  custom_item.name = "Custom";
  init.push_back(rpc_spec_item);
  init.push_back(custom_item);

  EXPECT_EQ(2u, init.size());

  const auto& ret = CacheManager::CollectCustomVDItems(init);

  EXPECT_EQ(1u, ret.size());
  EXPECT_EQ(ret.at(0).name, custom_item.name);
}

TEST_F(PolicyManagerImplTest2,
       CacheManager_RemainRPCSpecVehicleDataItemsNoCustomItems) {
  policy_table::VehicleDataItems init;
  policy_table::VehicleDataItem custom_item;
  custom_item.name = "headLampStatus";
  init.push_back(custom_item);

  EXPECT_EQ(1u, init.size());

  const auto& ret = CacheManager::CollectCustomVDItems(init);

  EXPECT_EQ(0u, ret.size());
}

}  // namespace policy_test
}  // namespace components
}  // namespace test

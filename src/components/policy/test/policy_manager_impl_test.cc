/*
 * Copyright (c) 2015, Ford Motor Company
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
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <sstream>
#include <algorithm>

#include "json/reader.h"
#include "json/writer.h"
#include "json/value.h"
#include "gtest/gtest.h"
#include "mock_policy_listener.h"
#include "mock_pt_ext_representation.h"
#include "mock_cache_manager.h"
#include "mock_update_status_manager.h"
#include "policy/policy_manager_impl.h"
#include "policy/cache_manager.h"
#include "policy/cache_manager_interface.h"
#include "config_profile/profile.h"
#include "sqlite_wrapper/sql_error.h"
#include "sqlite_wrapper/sql_database.h"
#include "sqlite_wrapper/sql_query.h"
#include "table_struct_ext/types.h"
#include "table_struct_ext/enums.h"
#include "utils/macro.h"
#include "utils/file_system.h"
#include "utils/date_time.h"
#include "utils/gen_hash.h"

using ::utils::dbms::SQLError;
using ::utils::dbms::SQLDatabase;
using ::utils::dbms::SQLQuery;

using ::testing::_;
using ::testing::Return;
using ::testing::DoAll;
using ::testing::SetArgReferee;
using ::testing::NiceMock;
using ::testing::AtLeast;

using ::policy::PTRepresentation;
using ::policy::MockPolicyListener;
using ::policy::MockPTRepresentation;
using ::policy::MockPTExtRepresentation;
using ::policy::MockCacheManagerInterface;

using ::policy::MockUpdateStatusManager;

using ::policy::PolicyManagerImpl;
using ::policy::PolicyTable;
using ::policy::EndpointUrls;

namespace policy_table = rpc::policy_table_interface_base;

namespace test {
namespace components {
namespace policy {

template<typename T>
std::string NumberToString(T Number) {
  std::ostringstream ss;
  ss << Number;
  return ss.str();
}

template<typename T>
void SortAndCheckEquality(std::vector<T> first,
                          std::vector<T> second) {
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
  unsigned int day = 1 + rand() % 31;  // Day from 1 - 31
  unsigned int month = 1 + rand() % 12;  // Month from 1 - 12
  unsigned int year = 1985 + rand() % 31;  // Year from 1985 - 2015

  // Convert date to string
  str.new_date_ = NumberToString(year) + '-' + NumberToString(month) + '-'
      + NumberToString(day);

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

class PolicyManagerImplTest : public ::testing::Test {
 protected:
  PolicyManagerImpl* manager;
  MockCacheManagerInterface* cache_manager;
  MockUpdateStatusManager update_manager;
  NiceMock<MockPolicyListener> listener;

  void SetUp() {
    manager = new PolicyManagerImpl();
    cache_manager = new MockCacheManagerInterface();
    manager->set_cache_manager(cache_manager);
    manager->set_listener(&listener);
  }

  void TearDown() {
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
      : app_id1("123456789"),
        app_id2("1766825573"),
        app_id3("584421907"),
        dev_id1("XXX123456789ZZZ"),
        dev_id2("08-00-27-CE-76-FE"),
        PTU_request_types(Json::arrayValue) {}

  protected:
    PolicyManagerImpl* manager;
    NiceMock<MockPolicyListener> listener;
    std::vector<std::string> hmi_level;
    std::vector<std::string> PT_request_types;
    uint32_t PTU_request_types_size;
    unsigned int index;
    const std::string app_id1;
    const std::string app_id2;
    const std::string app_id3;
    const std::string dev_id1;
    const std::string dev_id2;
    Json::Value PTU_request_types;

    void SetUp() {
      file_system::CreateDirectory("storage1");

      profile::Profile::instance()->config_file_name("smartDeviceLink2.ini");
      manager = new PolicyManagerImpl();
      manager->set_listener(&listener);
      const char* levels[] = {"BACKGROUND", "FULL", "LIMITED", "NONE"};
      hmi_level.assign(levels, levels + sizeof(levels) / sizeof(levels[0]));
      srand(time(NULL));
      index = rand() % 3;
    }

    std::vector<std::string> JsonToVectorString(const Json::Value& PTU_request_types) {
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
      if (ifile != NULL && reader.parse(ifile, root, true)) {
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
                   const std::string& section_name, const uint32_t rt_number,
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
        if (policy_table::EnumFromJsonString(temp_result[i],
                                             &filtered_result)) {
          final_result.push_back(filtered_result);
        }
      }
      return final_result;
    }

    void CheckResultForValidRT() {
      // Convert Json Array to std::vector<std::string>
      const std::vector<std::string>& result = JsonToVectorString(PTU_request_types);
      // Checks
      SortAndCheckEquality(PT_request_types, result);
    }

    void CheckResultForInvalidRT() {
      // Convert Json Array to std::vector<std::string>
      const std::vector<std::string>& temp_result = JsonToVectorString(PTU_request_types);
      std::vector<policy_table::RequestType> result1 = PushRequestTypesToContainer(temp_result);
      std::vector<policy_table::RequestType> result2 = PushRequestTypesToContainer(PT_request_types);
      // Checks
      SortAndCheckEquality(result1, result2);
    }

    void TearDown() {
      profile::Profile::instance()->config_file_name("smartDeviceLink.ini");
      delete manager;
    }
};

TEST_F(PolicyManagerImplTest, RefreshRetrySequence_SetSecondsBetweenRetries_ExpectRetryTimeoutSequenceWithSameSeconds) {
  // Arrange
  std::vector<int> seconds;
  seconds.push_back(50);
  seconds.push_back(100);
  seconds.push_back(200);

  // Assert
  EXPECT_CALL(*cache_manager, TimeoutResponse()).WillOnce(Return(60));
  EXPECT_CALL(*cache_manager, SecondsBetweenRetries(_)).WillOnce(
      DoAll(SetArgReferee<0>(seconds), Return(true)));

  // Act
  manager->RefreshRetrySequence();

  // Assert
  EXPECT_EQ(50, manager->NextRetryTimeout());
  EXPECT_EQ(100, manager->NextRetryTimeout());
  EXPECT_EQ(200, manager->NextRetryTimeout());
  EXPECT_EQ(0, manager->NextRetryTimeout());
}

TEST_F(PolicyManagerImplTest2, GetUpdateUrl) {
  // Arrange
  GetPTU("sdl_preloaded_pt.json");
  // Check expectations
  EXPECT_EQ("http://policies.telematics.ford.com/api/policies",
            manager->GetUpdateUrl(7));
  EXPECT_EQ("", manager->GetUpdateUrl(4));
}

TEST_F(PolicyManagerImplTest2, IsAppRevoked_SetRevokedAppID_ExpectAppRevoked) {
  // Arrange
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile != NULL && reader.parse(ifile, root, true)) {
    root["policy_table"]["app_policies"][app_id1] = Json::nullValue;
    json = root.toStyledString();
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_TRUE(manager->LoadPT("file_pt_update.json", msg));
  EXPECT_TRUE(manager->IsApplicationRevoked(app_id1));
}

TEST_F(PolicyManagerImplTest2, CheckPermissions_SetRevokedAppID_ExpectRPCDisallowed) {
  // Arrange
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile != NULL && reader.parse(ifile, root, true)) {
    root["policy_table"]["app_policies"][app_id1] = Json::nullValue;
    json = root.toStyledString();
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_TRUE(manager->LoadPT("file_pt_update.json", msg));
  policy_table::RpcParameters rpc_parameters;
  rpc_parameters.hmi_levels.push_back(policy_table::HL_FULL);

  policy_table::Rpc rpc;
  rpc["Alert"] = rpc_parameters;

  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;

  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id1)).WillOnce(Return(dev_id1));

  (manager->GetCache())->AddDevice(dev_id1,
                                   "Bluetooth");
  (manager->GetCache())->SetDeviceData(dev_id1,
                                       "hardware IPX",
                                       "v.8.0.1",
                                       "Android",
                                       "4.4.2",
                                       "Life",
                                       2,
                                       "Bluetooth");

  manager->CheckPermissions(app_id1, std::string("FULL"), "Alert" , input_params, output);
  // Check RPC is disallowed
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  ASSERT_TRUE(output.list_of_allowed_params.empty());
}

TEST_F(PolicyManagerImplTest2, CheckPermissions_SetAppIDwithPolicies_ExpectRPCAllowed) {
  // Arrange
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile != NULL && reader.parse(ifile, root, true)) {
    // Add AppID with policies
    root["policy_table"]["app_policies"]["1234"] = Json::Value(Json::objectValue);
    root["policy_table"]["app_policies"]["1234"]["memory_kb"] = Json::Value(50);
    root["policy_table"]["app_policies"]["1234"]["heart_beat_timeout_ms"] = Json::Value(100);
    root["policy_table"]["app_policies"]["1234"]["AppHMIType"] = Json::Value(Json::arrayValue);
    root["policy_table"]["app_policies"]["1234"]["AppHMIType"][0] = Json::Value("MEDIA");
    root["policy_table"]["app_policies"]["1234"]["groups"] = Json::Value(Json::arrayValue);
    root["policy_table"]["app_policies"]["1234"]["groups"][0] = Json::Value("Base-4");
    root["policy_table"]["app_policies"]["1234"]["priority"] = Json::Value("EMERGENCY");
    root["policy_table"]["app_policies"]["1234"]["default_hmi"] = Json::Value("FULL");
    root["policy_table"]["app_policies"]["1234"]["keep_context"] = Json::Value(true);
    root["policy_table"]["app_policies"]["1234"]["steal_focus"] = Json::Value(true);
    root["policy_table"]["app_policies"]["1234"]["certificate"] = Json::Value("sign");
    json = root.toStyledString();
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  // Load Json to cache
  EXPECT_TRUE(manager->LoadPT("file_pt_update.json", msg));

  policy_table::RpcParameters rpc_parameters;
  rpc_parameters.hmi_levels.push_back(policy_table::HL_FULL);

  policy_table::Rpc rpc;
  rpc["Alert"] = rpc_parameters;
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;

  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired("1234")).WillOnce(Return(dev_id1));
  (manager->GetCache())->AddDevice(dev_id1,
                                   "Bluetooth");
  (manager->GetCache())->SetDeviceData(dev_id1,
                                       "hardware IPX",
                                       "v.8.0.1",
                                       "Android",
                                       "4.4.2",
                                       "Life",
                                       2,
                                       "Bluetooth");
  manager->CheckPermissions(std::string("1234"), std::string("FULL"), "Alert" , input_params, output);
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
  EXPECT_CALL(*cache_manager, Increment("12345",
          usage_statistics::USER_SELECTIONS));
  manager->Increment("12345", usage_statistics::USER_SELECTIONS);
}

TEST_F(PolicyManagerImplTest, SetAppInfo) {
  // Assert
  EXPECT_CALL(*cache_manager, Set("12345", usage_statistics::LANGUAGE_GUI,
          "de-de"));
  manager->Set("12345", usage_statistics::LANGUAGE_GUI, "de-de");
}

TEST_F(PolicyManagerImplTest, AddAppStopwatch) {
  // Assert
  EXPECT_CALL(*cache_manager, Add("12345", usage_statistics::SECONDS_HMI_FULL,
          30));
  manager->Add("12345", usage_statistics::SECONDS_HMI_FULL, 30);
}

TEST_F(PolicyManagerImplTest, ResetPT) {
  EXPECT_CALL(*cache_manager, ResetPT("filename")).WillOnce(Return(true))
      .WillOnce(Return(false));
  EXPECT_CALL(*cache_manager, ResetCalculatedPermissions()).Times(AtLeast(1));
  EXPECT_CALL(*cache_manager, TimeoutResponse());
  EXPECT_CALL(*cache_manager, SecondsBetweenRetries(_));

  EXPECT_TRUE(manager->ResetPT("filename"));
  EXPECT_FALSE(manager->ResetPT("filename"));
}

TEST_F(PolicyManagerImplTest, LoadPT_SetPT_PTIsLoaded) {
  // Arrange
  Json::Value table(Json::objectValue);
  table["policy_table"] = Json::Value(Json::objectValue);

  Json::Value& policy_table = table["policy_table"];
  policy_table["module_config"] = Json::Value(Json::objectValue);
  policy_table["functional_groupings"] = Json::Value(Json::objectValue);
  policy_table["consumer_friendly_messages"] = Json::Value(Json::objectValue);
  policy_table["app_policies"] = Json::Value(Json::objectValue);

  Json::Value& module_config = policy_table["module_config"];
  module_config["preloaded_pt"] = Json::Value(true);
  module_config["exchange_after_x_ignition_cycles"] = Json::Value(10);
  module_config["exchange_after_x_kilometers"] = Json::Value(100);
  module_config["exchange_after_x_days"] = Json::Value(5);
  module_config["timeout_after_x_seconds"] = Json::Value(500);
  module_config["seconds_between_retries"] = Json::Value(Json::arrayValue);
  module_config["seconds_between_retries"][0] = Json::Value(10);
  module_config["seconds_between_retries"][1] = Json::Value(20);
  module_config["seconds_between_retries"][2] = Json::Value(30);
  module_config["endpoints"] = Json::Value(Json::objectValue);
  module_config["endpoints"]["0x00"] = Json::Value(Json::objectValue);
  module_config["endpoints"]["0x00"]["default"] = Json::Value(Json::arrayValue);
  module_config["endpoints"]["0x00"]["default"][0] = Json::Value(
      "http://ford.com/cloud/default");
  module_config["notifications_per_minute_by_priority"] = Json::Value(
      Json::objectValue);
  module_config["notifications_per_minute_by_priority"]["emergency"] =
      Json::Value(1);
  module_config["notifications_per_minute_by_priority"]["navigation"] =
      Json::Value(2);
  module_config["notifications_per_minute_by_priority"]["VOICECOMM"] =
      Json::Value(3);
  module_config["notifications_per_minute_by_priority"]["communication"] =
      Json::Value(4);
  module_config["notifications_per_minute_by_priority"]["normal"] = Json::Value(
      5);
  module_config["notifications_per_minute_by_priority"]["none"] = Json::Value(
      6);
  module_config["vehicle_make"] = Json::Value("MakeT");
  module_config["vehicle_model"] = Json::Value("ModelT");
  module_config["vehicle_year"] = Json::Value("2014");

  Json::Value& functional_groupings = policy_table["functional_groupings"];
  functional_groupings["default"] = Json::Value(Json::objectValue);
  Json::Value& default_group = functional_groupings["default"];
  default_group["rpcs"] = Json::Value(Json::objectValue);
  default_group["rpcs"]["Update"] = Json::Value(Json::objectValue);
  default_group["rpcs"]["Update"]["hmi_levels"] = Json::Value(Json::arrayValue);
  default_group["rpcs"]["Update"]["hmi_levels"][0] = Json::Value("FULL");
  default_group["rpcs"]["Update"]["parameters"] = Json::Value(Json::arrayValue);
  default_group["rpcs"]["Update"]["parameters"][0] = Json::Value("speed");

  Json::Value& consumer_friendly_messages =
      policy_table["consumer_friendly_messages"];
  consumer_friendly_messages["version"] = Json::Value("1.2");

  Json::Value& app_policies = policy_table["app_policies"];
  app_policies["default"] = Json::Value(Json::objectValue);
  app_policies["default"]["memory_kb"] = Json::Value(50);
  app_policies["default"]["heart_beat_timeout_ms"] = Json::Value(100);
  app_policies["default"]["groups"] = Json::Value(Json::arrayValue);
  app_policies["default"]["groups"][0] = Json::Value("default");
  app_policies["default"]["priority"] = Json::Value("EMERGENCY");
  app_policies["default"]["default_hmi"] = Json::Value("FULL");
  app_policies["default"]["keep_context"] = Json::Value(true);
  app_policies["default"]["steal_focus"] = Json::Value(true);
  app_policies["default"]["certificate"] = Json::Value("sign");
  app_policies["pre_DataConsent"] = Json::Value(Json::objectValue);
  app_policies["pre_DataConsent"]["memory_kb"] = Json::Value(50);
  app_policies["pre_DataConsent"]["heart_beat_timeout_ms"] = Json::Value(100);
  app_policies["pre_DataConsent"]["groups"] = Json::Value(Json::arrayValue);
  app_policies["pre_DataConsent"]["groups"][0] = Json::Value("default");
  app_policies["pre_DataConsent"]["priority"] = Json::Value("EMERGENCY");
  app_policies["pre_DataConsent"]["default_hmi"] = Json::Value("FULL");
  app_policies["pre_DataConsent"]["keep_context"] = Json::Value(true);
  app_policies["pre_DataConsent"]["steal_focus"] = Json::Value(true);
  app_policies["pre_DataConsent"]["certificate"] = Json::Value("sign");
  app_policies["1234"] = Json::Value(Json::objectValue);
  app_policies["1234"]["memory_kb"] = Json::Value(50);
  app_policies["1234"]["heart_beat_timeout_ms"] = Json::Value(100);
  app_policies["1234"]["groups"] = Json::Value(Json::arrayValue);
  app_policies["1234"]["groups"][0] = Json::Value("default");
  app_policies["1234"]["priority"] = Json::Value("EMERGENCY");
  app_policies["1234"]["default_hmi"] = Json::Value("FULL");
  app_policies["1234"]["keep_context"] = Json::Value(true);
  app_policies["1234"]["steal_focus"] = Json::Value(true);
  app_policies["1234"]["certificate"] = Json::Value("sign");
  app_policies["device"]["groups"] = Json::Value(Json::arrayValue);
  app_policies["device"]["groups"][0] = Json::Value("default");
  app_policies["device"]["priority"] = Json::Value("EMERGENCY");
  app_policies["device"]["default_hmi"] = Json::Value("FULL");
  app_policies["device"]["keep_context"] = Json::Value(true);
  app_policies["device"]["steal_focus"] = Json::Value(true);

  policy_table::Table update(&table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  // Assert
  ASSERT_TRUE(IsValid(update));

  EXPECT_CALL(*cache_manager, GetHMIAppTypeAfterUpdate(_)).Times(AtLeast(1));

  // Act
  std::string json = table.toStyledString();
  ::policy::BinaryMessage msg(json.begin(), json.end());

  utils::SharedPtr<policy_table::Table> snapshot = new policy_table::Table(
      update.policy_table);

  // Assert
  EXPECT_CALL(*cache_manager, GenerateSnapshot()).WillOnce(Return(snapshot));
  EXPECT_CALL(*cache_manager, ApplyUpdate(_)).WillOnce(Return(true));
  EXPECT_CALL(listener, GetAppName("1234")).WillOnce(Return(""));
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

TEST_F(PolicyManagerImplTest2, KmsChanged_SetExceededKms_ExpectCorrectSchedule) {
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

TEST_F(PolicyManagerImplTest2, AddApplication_AddNewApplicationFromDeviceWithoutConsent_ExpectUpdateRequired) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id1);
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, ReactOnUserDevConsentForApp_AddNewApplicationFromDeviceWithoutConsent_ExpectPreDataConsent) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id1);
  ASSERT_TRUE(manager->IsPredataPolicy(app_id1));
  manager->ReactOnUserDevConsentForApp(app_id1, false);
  EXPECT_TRUE(manager->IsPredataPolicy(app_id1));
}

TEST_F(PolicyManagerImplTest2, ReactOnUserDevConsentForApp_AddNewApplicationFromDeviceWithConsent_ExpectDefault) {
  // Arrange
  // RequestTypes for default & preDataConsent are different
  CreateLocalPT("ptu_requestType.json");
  manager->AddApplication(app_id1);
  ASSERT_TRUE(manager->IsPredataPolicy(app_id1));
  manager->ReactOnUserDevConsentForApp(app_id1, true);
  EXPECT_FALSE(manager->IsPredataPolicy(app_id1));
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id1));
}

TEST_F(PolicyManagerImplTest2, ReactOnUserDevConsentForApp_AddNewApplicationFromDeviceWithConsent_ExpectPreDataConsent) {
  // Arrange
  // RequestTypes for default & preDataConsent are the same
  CreateLocalPT("ptu2_requestType.json");
  manager->AddApplication(app_id1);
  ASSERT_TRUE(manager->IsPredataPolicy(app_id1));
  EXPECT_CALL(listener, OnPendingPermissionChange(app_id1)).Times(0);
  manager->ReactOnUserDevConsentForApp(app_id1, true);
  EXPECT_FALSE(manager->IsPredataPolicy(app_id1));
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id1));
}

TEST_F(PolicyManagerImplTest2, AddApplication_AddExistingApplicationFromDeviceWithoutConsent_ExpectNoUpdateRequired) {
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

TEST_F(PolicyManagerImplTest2, PTUpdatedAt_DaysNotExceedLimit_ExpectNoUpdateRequired) {
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

TEST_F(PolicyManagerImplTest2, PTUpdatedAt_DaysExceedLimit_ExpectUpdateRequired) {
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
  // Set PT was updated 50 days ago (limit is 30 days for now)
  manager->PTUpdatedAt(counter, days - 50);
  manager->OnAppRegisteredOnMobile(app_id2);
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, OnIgnitionCyclesExceeded_SetExceededIgnitionCycles_ExpectUpdateScheduled) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  TimevalStruct current_time = date_time::DateTime::getCurrentTime();
  const int kSecondsInDay = 60 * 60 * 24;
  int days = current_time.tv_sec / kSecondsInDay;
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  GetPTU("valid_sdl_pt_update.json");
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  // Try to add existing app
  manager->AddApplication(app_id2);
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
  ::policy::Counters counter = ::policy::Counters::DAYS_AFTER_EPOCH;
  // Set PT was updated 10 days ago (limit is 30 days for now)
  // So no limit exceeded
  manager->PTUpdatedAt(counter, days - 10);
  int ign_cycles = (manager->GetCache())->IgnitionCyclesBeforeExchange();
  // Set ignition cycles to value = 99 (limit is 100 which initiates auto PTExchange)
  for(int i = 0; i < ign_cycles; ++i) {
    manager->IncrementIgnitionCycles();
  }
  manager->OnAppRegisteredOnMobile(app_id2);
  // Check update required
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
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
  if (ifile != NULL && reader.parse(ifile, root, true)) {
    Json::Value seconds_between_retries  = Json::Value(Json::arrayValue);
    seconds_between_retries = root["policy_table"]["module_config"]["seconds_between_retries"];
    uint32_t size = seconds_between_retries.size();
    CreateLocalPT("sdl_preloaded_pt.json");
    for(uint32_t i = 0; i < size; ++i) {
      EXPECT_EQ(seconds_between_retries[i], manager->NextRetryTimeout());
    }
  }
}

TEST_F(PolicyManagerImplTest2, TimeOutExchange) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  // Check value taken from PT
  EXPECT_EQ(70, manager->TimeoutExchange());
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
  Json::Reader reader;
  Json::Value root(Json::objectValue);

  if (ifile != NULL && reader.parse(ifile, root, true)) {
    root["policy_table"]["module_config"]["preloaded_date"] = new_data.new_date_;
    Json::Value val(Json::objectValue);
    Json::Value val2(Json::arrayValue);
    val2[0] = hmi_level[index];
    val[new_data.new_field_value_]["hmi_levels"] = val2;
    root["policy_table"]["functional_groupings"][new_data.new_field_name_]["rpcs"] = val;
    root["policy_table"]["functional_groupings"][new_data.new_field_name_]["user_consent_prompt"] = new_data.new_field_name_;
  }
  ifile.close();

  Json::StyledStreamWriter writer;
  std::ofstream ofile("sdl_preloaded_pt.json");
  writer.write(ofile, root);
  ofile.flush();
  ofile.close();

  //  Make PolicyManager to update LocalPT
  EXPECT_TRUE(manager->InitPT("sdl_preloaded_pt.json"));

  // Arrange
  ::policy::CacheManagerInterfaceSPtr cache = manager->GetCache();
  utils::SharedPtr<policy_table::Table> table = cache->GenerateSnapshot();
  // Get FunctionalGroupings
  policy_table::FunctionalGroupings& fc = table->policy_table.functional_groupings;
  // Get RPCs for new added field in functional_group
  policy_table::Rpcs& rpcs = fc[new_data.new_field_name_];
  // Get user consent prompt
  const std::string& ucp = *(rpcs.user_consent_prompt);
  // Get Rpcs
  policy_table::Rpc& rpc = rpcs.rpcs;
  // Get RPC's parameters
  policy_table::RpcParameters& rpc_param = rpc[new_data.new_field_value_];

  // Check preloaded date
  EXPECT_EQ(static_cast<std::string>(*(table->policy_table.module_config.preloaded_date)), new_data.new_date_);
  // Check if new field exists in Local PT
  EXPECT_TRUE(fc.find(new_data.new_field_name_) != fc.end());
  // Check if user_consent_propmp is correct
  EXPECT_EQ(new_data.new_field_name_, ucp);
  // Check if new RPC exists
  EXPECT_TRUE(rpc.find(new_data.new_field_value_) != rpc.end());
  // Check HMI level of new RPC
  EXPECT_EQ(index, static_cast<unsigned int>(rpc_param.hmi_levels[0]));
  // Check if new field matches field added to preloaded PT
  EXPECT_EQ(std::string((*(fc.find(new_data.new_field_name_))).first), new_data.new_field_name_);
}

TEST_F(PolicyManagerImplTest,
       RequestPTUpdate_SetPT_GeneratedSnapshotAndPTUpdate) {
  // Arrange
  ::utils::SharedPtr< ::policy_table::Table > p_table =
      new ::policy_table::Table();

  // Assert
  EXPECT_CALL(listener, OnSnapshotCreated(_, _, _));
  EXPECT_CALL(*cache_manager, GenerateSnapshot()).WillOnce(Return(p_table));

  // Act
  manager->RequestPTUpdate();
}

TEST_F(PolicyManagerImplTest, ResetUserConsent_ResetOnlyOnce) {
  EXPECT_CALL(*cache_manager, ResetUserConsent()).
      WillOnce(Return(true)).
      WillOnce(Return(false));

  EXPECT_TRUE(manager->ResetUserConsent());
  EXPECT_FALSE(manager->ResetUserConsent());
}

TEST_F(PolicyManagerImplTest2, GetPolicyTableStatus_ExpectUpToDate) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  // Check
  EXPECT_EQ("UP_TO_DATE", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest, SetUpdateStarted_GetPolicyTableStatus_Expect_Updating) {
  // Arrange
  manager->OnUpdateStarted();
  // Check
  EXPECT_EQ("UPDATING", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, RetrySequenceDelaysSeconds_Expect_CorrectValues) {
  // Arrange
  std::ifstream ifile("sdl_preloaded_pt.json");
  Json::Reader reader;
  Json::Value root(Json::objectValue);
  if (ifile != NULL && reader.parse(ifile, root, true)) {
    Json::Value seconds_between_retries  = Json::Value(Json::arrayValue);
    seconds_between_retries = root["policy_table"]["module_config"]["seconds_between_retries"];
    uint32_t size = seconds_between_retries.size();
    CreateLocalPT("sdl_preloaded_pt.json");
    std::vector<int> delaySecs = manager->RetrySequenceDelaysSeconds();
    // Check
    ASSERT_EQ(size, delaySecs.size());
    for(uint32_t i = 0; i < size; ++i) {
    EXPECT_EQ(seconds_between_retries[i], delaySecs[i]);
    }
  }
}

TEST_F(PolicyManagerImplTest2, OnExceededTimeout_GetPolicyTableStatus_ExpectUpdateNeeded) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->OnExceededTimeout();
  // Check
  EXPECT_EQ("UPDATE_NEEDED", manager->GetPolicyTableStatus());
}

TEST_F(PolicyManagerImplTest2, GetUserConsentForDevice_SetDeviceWithoutConcent_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  GetPTU("valid_sdl_pt_update.json");
  ::policy::DeviceConsent consent = manager->GetUserConsentForDevice(app_id2);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceHasNoConsent, consent);
}

TEST_F(PolicyManagerImplTest2, GetUserConsentForDevice_SetDeviceAllowed_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                               "Bluetooth"));

  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
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

TEST_F(PolicyManagerImplTest2, GetUserConsentForDevice_SetDeviceDisallowed_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                               "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
                                                   "hardware IPX",
                                                   "v.8.0.1",
                                                   "Android",
                                                   "4.4.2",
                                                   "Life",
                                                   2,
                                                   "Bluetooth"));

  manager->SetUserConsentForDevice(dev_id2, false);
  ::policy::DeviceConsent consent = manager->GetUserConsentForDevice(dev_id2);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceDisallowed, consent);
}

TEST_F(PolicyManagerImplTest2, GetDefaultHmi_SetDeviceDisallowed_ExpectReceivedHmiCorrect) {
  // Arrange
  CreateLocalPT("ptu2_requestType.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                               "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
                                                   "hardware IPX",
                                                   "v.8.0.1",
                                                   "Android",
                                                   "4.4.2",
                                                   "Life",
                                                   2,
                                                   "Bluetooth"));

  manager->SetUserConsentForDevice(dev_id2, false);
  ::policy::DeviceConsent consent = manager->GetUserConsentForDevice(dev_id2);
  // Check
  EXPECT_EQ(::policy::DeviceConsent::kDeviceDisallowed, consent);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillRepeatedly(Return(dev_id2));
  manager->AddApplication(app_id2);
  std::string default_hmi;
  manager->GetDefaultHmi(app_id2, &default_hmi);
  EXPECT_EQ("NONE", default_hmi);
}

TEST_F(PolicyManagerImplTest2, GetDefaultHmi_SetDeviceAllowed_ExpectReceivedHmiCorrect) {
// Arrange
CreateLocalPT("ptu2_requestType.json");
manager->AddApplication(app_id2);
// Check if app has preData policy
EXPECT_TRUE(manager->IsPredataPolicy(app_id2));
std::string default_hmi1;
manager->GetDefaultHmi(app_id2, &default_hmi1);
EXPECT_EQ("NONE", default_hmi1);
ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                             "Bluetooth"));
ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
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
EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillRepeatedly(Return(dev_id2));
manager->AddApplication(app_id2);
EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id2));
std::string default_hmi2;
manager->GetDefaultHmi(app_id2, &default_hmi2);
EXPECT_EQ("LIMITED", default_hmi2);
}

TEST_F(PolicyManagerImplTest2, GetDefaultPriority_SetDeviceAllowed_ExpectReceivedPriorityCorrect) {
  // Arrange
  CreateLocalPT("ptu2_requestType.json");
  manager->AddApplication(app_id2);
  // Check if app has preData policy
  EXPECT_TRUE(manager->IsPredataPolicy(app_id2));
  std::string priority1;
  manager->GetPriority(app_id2, &priority1);
  EXPECT_EQ("NONE", priority1);
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                               "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
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
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillRepeatedly(Return(dev_id2));
  manager->AddApplication(app_id2);
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id2));
  std::string priority2;
  manager->GetPriority(app_id2, &priority2);
  EXPECT_EQ("EMERGENCY", priority2);
}

TEST_F(PolicyManagerImplTest2, GetUserFirendlyMessages_ExpectReceivedCorrectMessages) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  std::vector<std::string> message_code;
  message_code.push_back("SettingEnableUpdates");
  message_code.push_back("AppPermissions");
  std::string language = "en-us";
  std::vector< ::policy::UserFriendlyMessage > result = manager->GetUserFriendlyMessages(message_code, language);
  uint32_t size = result.size();
  EXPECT_GT(size, 0u);
  std::vector< ::policy::UserFriendlyMessage >::iterator result_iter;

  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();

  policy_table::ConsumerFriendlyMessages& consumer_friendly_messages = *(pt->policy_table.consumer_friendly_messages);
  policy_table::Messages& Messages = *(consumer_friendly_messages.messages);

  policy_table::Messages::const_iterator messages_iter = Messages.begin();
    // Loop until end of messages
    for (; messages_iter != Messages.end(); ++messages_iter) {

      if (messages_iter->first == "AppPermissions") {

        for (result_iter = result.begin(); result_iter != result.end(); ++result_iter) {
          if (result_iter->message_code == "AppPermissions") {

            const policy_table::MessageLanguages& MessageLanguages  = messages_iter->second;
            const policy_table::Languages& Languages = MessageLanguages.languages;
            policy_table::Languages::const_iterator languages_iter = Languages.find("en-us");
            // If necessary language found
            if (languages_iter != Languages.end()) {
            const policy_table::MessageString& MessageString = languages_iter->second;
            EXPECT_EQ(static_cast<std::string>(*(MessageString.line1)), result_iter->line1);
            EXPECT_EQ(static_cast<std::string>(*(MessageString.line2)), result_iter->line2);
            EXPECT_EQ(static_cast<std::string>(*(MessageString.tts)), result_iter->tts);
            EXPECT_EQ(static_cast<std::string>(*(MessageString.label)), result_iter->label);
            EXPECT_EQ(static_cast<std::string>(*(MessageString.textBody)), result_iter->text_body);
            }
          }
        }
      }
      else if (messages_iter->first == "SettingEnableUpdates") {
        for (result_iter = result.begin(); result_iter != result.end(); ++result_iter) {
          if (result_iter->message_code == "SettingEnableUpdates") {
            const policy_table::MessageLanguages& MessageLanguages  = messages_iter->second;
            const policy_table::Languages& Languages = MessageLanguages.languages;
            policy_table::Languages::const_iterator languages_iter = Languages.find("en-us");
            // If necessary language found
            if (languages_iter != Languages.end()) {
            const policy_table::MessageString& MessageString2 = languages_iter->second;
            EXPECT_EQ(static_cast<std::string>(*(MessageString2.line1)), result_iter->line1);
          }
        }
      }
    }
  }
}

TEST_F(PolicyManagerImplTest2, SetDeviceInfo_ExpectDevInfoAddedToPT) {
  // Arrange
  ::policy::DeviceInfo dev_info;
  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();
  dev_info.hardware = "hardware IPX";
  dev_info.firmware_rev = "v.8.0.1";
  dev_info.os = "Android";
  dev_info.os_ver = "4.4.2";
  dev_info.carrier = "Life";
  dev_info.max_number_rfcom_ports = 2;
  dev_info.connection_type = "Bluetooth";
  manager->AddDevice(dev_id1, "Bluetooth");
  manager->SetDeviceInfo(dev_id1, dev_info);
  // Find device in PT
  policy_table::DeviceData::const_iterator iter =
      (*(pt->policy_table.device_data)).find(dev_id1);
  // Checks
  ASSERT_TRUE(iter != (*(pt->policy_table.device_data)).end());
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.hardware), dev_info.hardware);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.firmware_rev), dev_info.firmware_rev);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.os), dev_info.os);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.os_version), dev_info.os_ver);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.carrier), dev_info.carrier);
  EXPECT_EQ(static_cast<std::string>(*(*iter).second.connection_type), dev_info.connection_type);
  EXPECT_EQ(static_cast<uint8_t>(*(*iter).second.max_number_rfcom_ports), dev_info.max_number_rfcom_ports);
}

TEST_F(PolicyManagerImplTest2, GetUserConsentForApp_SetUserConsentForApp_ExpectReceivedConsentCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                               "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
                                                   "hardware IPX",
                                                   "v.8.0.1",
                                                   "Android",
                                                   "4.4.2",
                                                   "Life",
                                                   2,
                                                   "Bluetooth"));

  std::vector<std::string> consented_groups;
  std::vector<std::string> disallowed_groups;
  consented_groups.push_back(std::string("Notifications"));
  consented_groups.push_back(std::string("Notifications"));
  (manager->GetCache())->SetUserPermissionsForDevice(dev_id2,
                                                     consented_groups,
                                                     disallowed_groups);
  manager->SetUserConsentForDevice(dev_id2, true);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillRepeatedly(Return(dev_id2));
  manager->AddApplication(app_id2);
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

  std::vector< ::policy::FunctionalGroupPermission > groups_permissions;
  groups_permissions.push_back(group1_perm);
  perm_consent.group_permissions = groups_permissions;

  manager->SetUserConsentForApp(perm_consent);
  manager->SendNotificationOnPermissionsUpdated(app_id2);
  std::vector< ::policy::FunctionalGroupPermission > actual_groups_permissions;
  std::vector< ::policy::FunctionalGroupPermission >::iterator it;
  manager->GetUserConsentForApp(dev_id2, app_id2, actual_groups_permissions);
  uint32_t index = 0;
  for (; index < actual_groups_permissions.size(); ++index) {
    if(actual_groups_permissions[index].group_id == group1_perm.group_id) {
      break;
    }
  }
  // Check
  EXPECT_EQ(group1_perm.group_alias, actual_groups_permissions[index].group_alias);
  EXPECT_EQ(group1_perm.group_name, actual_groups_permissions[index].group_name);
  EXPECT_EQ(group1_perm.group_id, actual_groups_permissions[index].group_id);
  EXPECT_EQ(group1_perm.state, actual_groups_permissions[index].state);
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

  Json::Value appHmiTypes  = Json::Value(Json::arrayValue);
  appHmiTypes = root["policy_table"]["app_policies"][app_id2]["AppHMIType"];
  uint32_t appHmiType_size = appHmiTypes.size();

  Json::Value appNicknames  = Json::Value(Json::arrayValue);
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
  for(uint32_t i = 0; i < nick_names_size; ++i) {
    EXPECT_EQ(app_nicknames1[i], appNicknames[i].asString());
  }
  // Check AppHMITypes match
  for(uint32_t i = 0; i < app_hmi_types_size; ++i) {
    EXPECT_EQ(app_hmi_types1[i], appHmiTypes[i].asString());
  }
}

TEST_F(PolicyManagerImplTest, MarkUnpairedDevice) {
  // Assert
  EXPECT_CALL(*cache_manager, SetUnpairedDevice("12345", true)).
      WillOnce(Return(true));
  EXPECT_CALL(*cache_manager, GetDeviceGroupsFromPolicies(_, _));
  // Act
  manager->MarkUnpairedDevice("12345");
}

TEST_F(PolicyManagerImplTest2, SetSystemLanguage_ExpectSystemLanguageSetSuccessfully) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->SetSystemLanguage("it-it");
  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();
  ::policy_table::ModuleMeta& ModuleMeta = *(pt->policy_table.module_meta);
  EXPECT_EQ("it-it", static_cast<std::string>(*(ModuleMeta.language)));
}

TEST_F(PolicyManagerImplTest2, SetVINValue_ExpectVINSetSuccessfully) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  std::string vin_code("1FAPP6242VH100001");
  manager->SetVINValue(vin_code);
  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();
  ::policy_table::ModuleMeta& ModuleMeta = *(pt->policy_table.module_meta);
  EXPECT_EQ(vin_code, static_cast<std::string>(*(ModuleMeta.vin)));
}

TEST_F(PolicyManagerImplTest2, SetSystemInfo_ExpectSystemInfoSetSuccessfully) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->SetSystemInfo("4.1.3.B_EB355B", "WAEGB", "ru-ru");
  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();
  ::policy_table::ModuleMeta& ModuleMeta = *(pt->policy_table.module_meta);
  EXPECT_EQ("ru-ru", static_cast<std::string>(*(ModuleMeta.language)));
  EXPECT_EQ("4.1.3.B_EB355B", static_cast<std::string>(*(ModuleMeta.ccpu_version)));
  EXPECT_EQ("WAEGB", static_cast<std::string>(*(ModuleMeta.wers_country_code)));
}

TEST_F(PolicyManagerImplTest2, CanAppKeepContext_AddAppFromUnconsentedDevice_ExpectAppCannotKeepContext) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id2);
  // Check if app has preData policy
  EXPECT_TRUE(manager->IsPredataPolicy(app_id2));
  // Check keep context in preData policy
  EXPECT_FALSE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2, CanAppKeepContext_AddAppFromConsentedDevice_ExpectAppCannotKeepContext) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                               "Bluetooth"));
  manager->AddApplication(app_id2);
  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
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
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillRepeatedly(Return(dev_id2));
  manager->AddApplication(app_id2);
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id2));
  // Check keep context in default policy
  EXPECT_FALSE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2, CanAppKeepContext_SetPoliciesForAppUpdated_ExpectAppCanKeepContext) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id2);
  GetPTU("valid_sdl_pt_update.json");
  // Check keep context in updated policies for app
  EXPECT_TRUE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2, CanAppStealFocus_AddAppFromUnconsentedDevice_ExpectAppCannotStealFocus) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id2);
  // Check if app has preData policy
  EXPECT_TRUE(manager->IsPredataPolicy(app_id2));
  // Check keep context in preData policy
  EXPECT_FALSE(manager->CanAppStealFocus(app_id2));
}

TEST_F(PolicyManagerImplTest2, CanAppStealFocus_AddAppFromConsentedDevice_ExpectAppCannotStealFocus) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                               "Bluetooth"));
  manager->AddApplication(app_id2);
  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
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
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillRepeatedly(Return(dev_id2));
  manager->AddApplication(app_id2);
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id2));
  // Check keep context in default policy
  EXPECT_FALSE(manager->CanAppStealFocus(app_id2));
}

TEST_F(PolicyManagerImplTest2, CanAppStealFocus_SetPoliciesForAppUpdated_ExpectAppCanStealFocus) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id2);
  GetPTU("valid_sdl_pt_update.json");
  // Check keep context in updated policies for app
  EXPECT_TRUE(manager->CanAppKeepContext(app_id2));
}

TEST_F(PolicyManagerImplTest2, GetCurrentDeviceId) {
  // Arrange
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).Times(1);
  EXPECT_EQ("", manager->GetCurrentDeviceId(app_id2));
}

TEST_F(PolicyManagerImplTest2, IsPredataPolicy_SetAppWIthPredataPolicy_ExpectPredataPolicy) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  manager->AddApplication(app_id2);
  // Check if app has preData policy
  EXPECT_TRUE(manager->IsPredataPolicy(app_id2));
}

TEST_F(PolicyManagerImplTest2, CleanUnpairedDevice_ExpectDevicesDeleted) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  // Add first device
  ::policy::DeviceInfo dev_info1;
  dev_info1.hardware = "hardware IPX";
  dev_info1.firmware_rev = "v.8.0.1";
  dev_info1.os = "Android";
  dev_info1.os_ver = "4.4.2";
  dev_info1.carrier = "Life";
  dev_info1.max_number_rfcom_ports = 2;
  dev_info1.connection_type = "Bluetooth";
  manager->AddDevice(dev_id1, "Bluetooth");
  manager->SetDeviceInfo(dev_id1, dev_info1);

  // Add second device
  ::policy::DeviceInfo dev_info2;
  dev_info2.hardware = "hardware SPX";
  dev_info2.firmware_rev = "v.6.0.1";
  dev_info2.os = "Android";
  dev_info2.os_ver = "4.1.1";
  dev_info2.carrier = "MTS";
  dev_info2.max_number_rfcom_ports = 2;
  dev_info2.connection_type = "Bluetooth";
  manager->AddDevice("ZZZ123456789YYY", "Bluetooth");
  manager->SetDeviceInfo("ZZZ123456789YYY", dev_info2);

  // Add third device
  ::policy::DeviceInfo dev_info3;
  dev_info3.hardware = "hardware DNPX";
  dev_info3.firmware_rev = "v.4.0.1";
  dev_info3.os = "Android";
  dev_info3.os_ver = "5.0.1 Lollipop";
  dev_info3.carrier = "Kyivstar";
  dev_info3.max_number_rfcom_ports = 2;
  dev_info3.connection_type = "Bluetooth";
  manager->AddDevice("AAA123456789RRR", "Bluetooth");
  manager->SetDeviceInfo("AAA123456789RRR", dev_info3);

  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();
  // Try to find first device in PT
  policy_table::DeviceData::const_iterator iter =
      (*(pt->policy_table.device_data)).find(dev_id1);
  // Check first device successfully added to PT
  ASSERT_TRUE(iter != (*(pt->policy_table.device_data)).end());

  // Try to find second device in PT
  iter = (*(pt->policy_table.device_data)).find("ZZZ123456789YYY");
  // Check second device successfully added to PT
  ASSERT_TRUE(iter != (*(pt->policy_table.device_data)).end());

  // Try to find third device in PT
  iter = (*(pt->policy_table.device_data)).find("AAA123456789RRR");
  // Check third device successfully added to PT
  ASSERT_TRUE(iter != (*(pt->policy_table.device_data)).end());

  manager->MarkUnpairedDevice(dev_id1);
  manager->MarkUnpairedDevice("ZZZ123456789YYY");
  manager->MarkUnpairedDevice("AAA123456789RRR");
  manager->CleanupUnpairedDevices();

  // Try to find first device in PT
  iter = (*(pt->policy_table.device_data)).find(dev_id1);
  // Check first device successfully deleted from PT
  ASSERT_TRUE(iter == (*(pt->policy_table.device_data)).end());

  // Try to find second device in PT
  iter = (*(pt->policy_table.device_data)).find("ZZZ123456789YYY");
  // Check second device successfully deleted from PT
  ASSERT_TRUE(iter == (*(pt->policy_table.device_data)).end());

  // Try to find third device in PT
  iter = (*(pt->policy_table.device_data)).find("AAA123456789RRR");
  // Check third device successfully deleted from PT
  ASSERT_TRUE(iter == (*(pt->policy_table.device_data)).end());
}

TEST_F(PolicyManagerImplTest2, GetVehicleInfo_SetVehicleInfo_ExpectReceivedInfoCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  GetPTU("valid_sdl_pt_update.json");
  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();
  policy_table::ModuleConfig& module_config =
      pt->policy_table.module_config;
  ::policy::VehicleInfo vehicle_info = manager->GetVehicleInfo();

  EXPECT_EQ(static_cast<std::string>(*module_config.vehicle_make), vehicle_info.vehicle_make);
  EXPECT_EQ(static_cast<std::string>(*module_config.vehicle_model), vehicle_info.vehicle_model);
  EXPECT_EQ(static_cast<std::string>(*module_config.vehicle_year), vehicle_info.vehicle_year);
}

TEST_F(PolicyManagerImplTest2, GetPermissionsForApp_SetUserConsentForApp_ExpectReceivedPermissionsCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");

  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                               "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
                                                   "hardware IPX",
                                                   "v.8.0.1",
                                                   "Android",
                                                   "4.4.2",
                                                   "Life",
                                                   2,
                                                   "Bluetooth"));

  std::vector<std::string> consented_groups;
  std::vector<std::string> disallowed_groups;
  consented_groups.push_back(std::string("Notifications"));
  (manager->GetCache())->SetUserPermissionsForDevice(dev_id2,
                                                     consented_groups,
                                                     disallowed_groups);
  manager->SetUserConsentForDevice(dev_id2, true);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillRepeatedly(Return(dev_id2));
  manager->AddApplication(app_id2);

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

  std::vector< ::policy::FunctionalGroupPermission > groups_permissions;
  groups_permissions.push_back(group1_perm);
  perm_consent.group_permissions = groups_permissions;

  manager->SetUserConsentForApp(perm_consent);
  manager->SendNotificationOnPermissionsUpdated(app_id2);
  std::vector< ::policy::FunctionalGroupPermission > actual_groups_permissions;
  std::vector< ::policy::FunctionalGroupPermission >::iterator it;
  manager->GetPermissionsForApp(dev_id2, app_id2, actual_groups_permissions);
  uint32_t index = 0;
  for (; index < actual_groups_permissions.size(); ++index) {
    if(actual_groups_permissions[index].group_id == group1_perm.group_id) {
      break;
    }
  }
  // Check
  EXPECT_EQ(group1_perm.group_alias, actual_groups_permissions[index].group_alias);
  EXPECT_EQ(group1_perm.group_name, actual_groups_permissions[index].group_name);
  EXPECT_EQ(group1_perm.group_id, actual_groups_permissions[index].group_id);
  EXPECT_EQ(group1_perm.state, actual_groups_permissions[index].state);
}

TEST_F(PolicyManagerImplTest2, GetAppRequestTypes_AddApp_UpdateAppPolicies_ExpectReceivedRequestTypesCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");

  manager->AddApplication(app_id3);
  std::vector<std::string> app_requests = manager->GetAppRequestTypes(app_id3);
  EXPECT_EQ(1u, app_requests.size());

  Json::Value root = GetPTU("ptu_requestType.json");
  Json::Value request_Types  = Json::Value(Json::arrayValue);
  request_Types = root["policy_table"]["app_policies"][app_id3]["RequestType"];
  app_requests = manager->GetAppRequestTypes(app_id3);
  EXPECT_EQ(request_Types.size(), app_requests.size());
  // Check nicknames match
  for(uint32_t i = 0; i < request_Types.size(); ++i) {
    EXPECT_EQ(request_Types[i], app_requests[i]);
  }
}

TEST_F(PolicyManagerImplTest2, HertBeatTimeout_AddApp_UpdateAppPolicies_ExpectReceivedHertBeatTimeoutCorrect) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();
  ::policy_table::PolicyTableType type1 = ::policy_table::PolicyTableType::PT_PRELOADED;
  pt->SetPolicyTableType(type1);
  if (!pt->is_valid()) {
  std:: cout << "\nPolicy table is not valid." << "\n";
  rpc::ValidationReport report("policy_table");
  pt->ReportErrors(&report);
  }
  // Add new app
  manager->AddApplication(app_id2);
  uint32_t result = manager->HeartBeatTimeout(app_id2);
  // By default hertbeat timeout is 0
  EXPECT_EQ(0u, result);
  Json::Value root = GetPTU("valid_sdl_pt_update.json");

  ::policy_table::PolicyTableType type2 = ::policy_table::PolicyTableType::PT_UPDATE;
  pt->SetPolicyTableType(type2);
  if (!pt->is_valid()) {
  std:: cout << "\nPolicy table is not valid." << "\n";
  rpc::ValidationReport report("policy_table");
  pt->ReportErrors(&report);
  }

  Json::Value heart_beat_timeout  = Json::Value(Json::uintValue);
  heart_beat_timeout = root["policy_table"]["app_policies"][app_id2]["heart_beat_timeout_ms"];
  result = manager->HeartBeatTimeout(app_id2);
  EXPECT_EQ(heart_beat_timeout.asUInt(), result);
}

TEST_F(PolicyManagerImplTest2, SendNotificationOnPermissionsUpdated_SetDeviceAllowed_ExpectNotificationSent) {
  // Arrange
  CreateLocalPT("ptu2_requestType.json");
  manager->AddApplication(app_id2);
  // Check if app has preData policy
  EXPECT_TRUE(manager->IsPredataPolicy(app_id2));
  std::string default_hmi1;
  manager->GetDefaultHmi(app_id2, &default_hmi1);
  EXPECT_EQ("NONE", default_hmi1);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillOnce(Return(""));
  EXPECT_CALL(listener, OnPermissionsUpdated(app_id2, _ , default_hmi1)).Times(0);
  manager->SendNotificationOnPermissionsUpdated(app_id2);

  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                             "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
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
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillRepeatedly(Return(dev_id2));
  manager->AddApplication(app_id2);
  EXPECT_TRUE((manager->GetCache())->IsDefaultPolicy(app_id2));
  std::string default_hmi2;
  manager->GetDefaultHmi(app_id2, &default_hmi2);
  EXPECT_EQ("LIMITED", default_hmi2);
  EXPECT_CALL(listener, OnPermissionsUpdated(app_id2, _ , default_hmi2));
  manager->SendNotificationOnPermissionsUpdated(app_id2);
}

TEST_F(PolicyManagerImplTest2, RemoveAppConsentForGroup_SetUserConsentForApp_ExpectAppConsentDeleted) {
  // Arrange
  CreateLocalPT("sdl_preloaded_pt.json");
  ASSERT_TRUE((manager->GetCache())->AddDevice(dev_id2,
                                               "Bluetooth"));
  ASSERT_TRUE((manager->GetCache())->SetDeviceData(dev_id2,
                                                   "hardware IPX",
                                                   "v.8.0.1",
                                                   "Android",
                                                   "4.4.2",
                                                   "Life",
                                                   2,
                                                   "Bluetooth"));

  std::vector<std::string> consented_groups;
  std::vector<std::string> disallowed_groups;
  consented_groups.push_back(std::string("Notifications"));
  (manager->GetCache())->SetUserPermissionsForDevice(dev_id2,
                                                     consented_groups,
                                                     disallowed_groups);
  manager->SetUserConsentForDevice(dev_id2, true);
  EXPECT_CALL(listener, OnCurrentDeviceIdUpdateRequired(app_id2)).WillRepeatedly(Return(dev_id2));
  manager->AddApplication(app_id2);
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

  std::vector< ::policy::FunctionalGroupPermission > groups_permissions;
  groups_permissions.push_back(group1_perm);
  perm_consent.group_permissions = groups_permissions;

  manager->SetUserConsentForApp(perm_consent);
  manager->SendNotificationOnPermissionsUpdated(app_id2);
  std::vector< ::policy::FunctionalGroupPermission > actual_groups_permissions;
  std::vector< ::policy::FunctionalGroupPermission >::iterator it;
  manager->GetPermissionsForApp(dev_id2, app_id2, actual_groups_permissions);
  uint32_t index = 0;
  for (; index < actual_groups_permissions.size(); ++index) {
    if(actual_groups_permissions[index].group_id == group1_perm.group_id) {
      break;
    }
  }
  // Check
  EXPECT_EQ(group1_perm.group_alias, actual_groups_permissions[index].group_alias);
  EXPECT_EQ(group1_perm.group_name, actual_groups_permissions[index].group_name);
  EXPECT_EQ(group1_perm.group_id, actual_groups_permissions[index].group_id);
  EXPECT_EQ(group1_perm.state, actual_groups_permissions[index].state);
  utils::SharedPtr<policy_table::Table> pt = (manager->GetCache())->GetPT();
  uint32_t ucr_size = 0;
  ::policy_table::DeviceData& device_data = *pt->policy_table.device_data;
  ::policy_table::DeviceData::const_iterator dev_data_iter = device_data.find(dev_id2);
  if (dev_data_iter != device_data.end()) {
    const ::policy_table::DeviceParams& dev_params = dev_data_iter->second;
    const ::policy_table::UserConsentRecords& ucr = *(dev_params.user_consent_records);
    ucr_size = ucr.size();
    ASSERT_GT(ucr_size, 0u);
    ::policy_table::UserConsentRecords::const_iterator ucr_iter = ucr.find(app_id2);
    if (ucr_iter != ucr.end()) {
      EXPECT_TRUE((*(ucr_iter->second.consent_groups)).find("Notifications") != (*(ucr_iter->second.consent_groups)).end());
      manager->RemoveAppConsentForGroup(app_id2, "Notifications");
      EXPECT_TRUE((*(ucr_iter->second.consent_groups)).find("Notifications") == (*(ucr_iter->second.consent_groups)).end());
    }
  }
}

TEST_F(PolicyManagerImplTest2, AddValidRequestTypesToPT_default_Section_ExpectRTAdded) {
  // Arrange
  AddRTtoPT("PTU.json", "default", 3u, 0);
  CheckResultForValidRT();
}

TEST_F(PolicyManagerImplTest2, AddValidRequestTypeToPT_preDataConsentSection_ExpectRTAdded) {
  // Arrange
  AddRTtoPT("PTU.json", "pre_DataConsent", 1u, 0u);
  CheckResultForValidRT();
}

TEST_F(PolicyManagerImplTest2, AddInvalidRequestTypeToPT_defaultSection_ExpectIgnored) {
  // Arrange
  AddRTtoPT("PTU3.json", "default", 3u, 2u);
  CheckResultForInvalidRT();
}


TEST_F(PolicyManagerImplTest2, AddInvalidRequestTypeToPT_pre_DataConsentSection_ExpectIgnored) {
  // Arrange
  AddRTtoPT("PTU3.json", "pre_DataConsent", 1u, 1u);
  CheckResultForInvalidRT();
}

TEST_F(PolicyManagerImplTest2, AddValidRequestTypeToPT_GetNewAppWithSpecificPoliciesViaPTU_ExpectRTAdded) {
  // Arrange
  AddRTtoAppSectionPT("PTU2.json", "1234", 1u, 0u);
  std::vector<std::string> result;
  for (uint32_t i = 0; i < PTU_request_types_size; ++i) {
     result.push_back(PTU_request_types[i].asString());
  }
  std::sort(PT_request_types.begin(), PT_request_types.end());
  std::sort(result.begin(), result.end());
  // Checks
  ASSERT_EQ(PT_request_types.size(), result.size());
  for (uint32_t i = 0 ; i < PT_request_types.size(); ++i) {
    EXPECT_EQ(PT_request_types[i], result[i]);
  }
}

TEST_F(PolicyManagerImplTest2, AddInvalidRequestTypeToPT_GetNewAppWithSpecificPoliciesViaPTU_ExpectRTAdded) {
 // Arrange
  AddRTtoAppSectionPT("PTU4.json", "1234", 1u, 1u);
  policy_table::RequestType temp_res1;
  std::vector<policy_table::RequestType> result1;
  for (uint32_t i = 0 ; i < PTU_request_types_size; ++i) {
    if (::rpc::policy_table_interface_base::EnumFromJsonString(PTU_request_types[i].asString(), &temp_res1)) {
      result1.push_back(temp_res1);
    }
  }
  policy_table::RequestType temp_res2;
  std::vector<policy_table::RequestType> result2;
  for (uint32_t i = 0; i < PT_request_types.size(); ++i) {
    if(::rpc::policy_table_interface_base::EnumFromJsonString(PT_request_types[i], &temp_res2)) {
      result2.push_back(temp_res2);
    }
  }
  ASSERT_EQ(result1.size(), result2.size());
  std::sort(result1.begin(), result1.end());
  std::sort(result2.begin(), result2.end());
  // Checks
  for (uint32_t i = 0 ; i < PT_request_types.size(); ++i) {
    EXPECT_EQ(result1[i], result2[i]);
  }
}

}  // namespace policy
}  // namespace components
}  // namespace test

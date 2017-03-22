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

#include <string>
#include <fstream>
#include <stdint.h>
#include <vector>

#include "policy/policy_manager_impl_test_base.h"

#include "utils/file_system.h"
#include "utils/make_shared.h"
#include "utils/gen_hash.h"
#include "json/reader.h"

#include "policy/mock_pt_ext_representation.h"

namespace test {
namespace components {
namespace policy_test {

using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::ContainerEq;
using ::testing::_;

// Help functions
char GenRandomChar(char range_from, char range_to) {
  if (range_from > range_to) {
    std::swap(range_from, range_to);
  }
  const int range_size = range_to - range_from;
  return rand() % range_size + range_from;
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

  for (unsigned int i = 0; i < 5; ++i) {
    str.new_field_value_ += GenRandomChar('A', 'Z');
  }
  return str;
}

void CheckIsParamInList(const ::policy::RPCParams& list,
                        const std::string& parameter) {
  EXPECT_TRUE(std::find(list.begin(), list.end(), parameter) != list.end())
      << "Parameter not exists: " << parameter;
}

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

void InsertRpcParametersInList(::policy::RPCParams& input_params) {
  input_params.insert("longitudeDegrees");
  input_params.insert("latitudeDegrees");
  input_params.insert("locationName");
  input_params.insert("locationDescription");
  input_params.insert("addressLines");
  input_params.insert("phoneNumber");
  input_params.insert("locationImage");
  input_params.insert("deliveryMode");
  input_params.insert("timeStamp");
  input_params.insert("address");
}

policy_table::AppHmiTypes HmiTypes(const policy_table::AppHMIType hmi_type) {
  policy_table::AppHmiTypes hmi_types;
  hmi_types.push_back(hmi_type);
  return hmi_types;
}

// PolicyManagerImplTest class methods
PolicyManagerImplTest::PolicyManagerImplTest()
    : unpaired_device_id_("08-00-27-CE-76-FE")
    , manager_(NULL)
    , cache_manager_(NULL) {}

void PolicyManagerImplTest::SetUp() {
  manager_ = new PolicyManagerImpl();
  cache_manager_ = new MockCacheManagerInterface();
  manager_->set_cache_manager(cache_manager_);
  manager_->set_listener(&listener_);

  ON_CALL(*cache_manager_, GetExternalConsentStatus())
      .WillByDefault(Return(ExternalConsentStatus()));
  ON_CALL(*cache_manager_, GetGroupsWithSameEntities(_))
      .WillByDefault(Return(GroupsByExternalConsentStatus()));
  ON_CALL(*cache_manager_, GetKnownLinksFromPT())
      .WillByDefault(Return(std::map<std::string, std::string>()));
  ON_CALL(listener_, GetRegisteredLinks(_)).WillByDefault(Return());
}

void PolicyManagerImplTest::TearDown() {
  delete manager_;
}

::testing::AssertionResult PolicyManagerImplTest::IsValid(
    const policy_table::Table& table) {
  if (table.is_valid()) {
    return ::testing::AssertionSuccess();
  } else {
    ::rpc::ValidationReport report(" - table");
    table.ReportErrors(&report);
    return ::testing::AssertionFailure() << ::rpc::PrettyFormat(report);
  }
}

// PolicyManagerImplTest2 class methods
PolicyManagerImplTest2::PolicyManagerImplTest2()
    : app_id_1_("123456789")
    , app_id_2_("1766825573")
    , app_id_3_("584421907")
    , device_id_1_("XXX123456789ZZZ")
    , device_id_2_("08-00-27-CE-76-FE")
    , application_id_("1234")
    , app_storage_folder_("storage1")
    , preloaded_pt_filename_(kSdlPreloadedPtJson)
    , in_memory_(true)
    , policy_manager_(NULL)
    , ptu_request_types_size_(0u)
    , index_(0u)
    , ptu_request_types_(Json::arrayValue) {}

void PolicyManagerImplTest2::SetUp() {
  ON_CALL(listener_, GetRegisteredLinks(_)).WillByDefault(Return());

  file_system::CreateDirectory(app_storage_folder_);

  policy_manager_ = new PolicyManagerImpl(in_memory_);
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(app_storage_folder_));
  policy_manager_->set_listener(&listener_);
  const char* levels[] = {"BACKGROUND", "FULL", "LIMITED", "NONE"};
  hmi_level_.assign(levels, levels + sizeof(levels) / sizeof(levels[0]));
  srand(time(NULL));
  index_ = rand() % 3;
}

::policy::StringArray PolicyManagerImplTest2::JsonToVectorString(
    const Json::Value& PTU_request_types) {
  ::policy::StringArray result;
  for (uint32_t i = 0; i < PTU_request_types.size(); ++i) {
    result.push_back(PTU_request_types[i].asString());
  }
  return result;
}

const Json::Value PolicyManagerImplTest2::GetPTU(const std::string& file_name) {
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
  EXPECT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(policy_manager_->GetCache()->IsPTPreloaded());
  return root;
}

void PolicyManagerImplTest2::CreateLocalPT(const std::string& file_name) {
  file_system::remove_directory_content(app_storage_folder_);
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(app_storage_folder_));
  ASSERT_TRUE(policy_manager_->InitPT(file_name, &policy_settings_));
  EXPECT_TRUE(policy_manager_->GetCache()->IsPTPreloaded());
}

void PolicyManagerImplTest2::AddRTtoPT(const std::string& update_file_name,
                                       const std::string& section_name,
                                       const uint32_t rt_number,
                                       const uint32_t invalid_rt_number) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  // Get RequestTypes from section of preloaded_pt app_policies
  pt_request_types_ = policy_manager_->GetAppRequestTypes(section_name);
  EXPECT_EQ(rt_number, pt_request_types_.size());
  Json::Value root = GetPTU(update_file_name);
  // Get Request Types from JSON (PTU)
  ptu_request_types_ =
      root["policy_table"]["app_policies"][section_name]["RequestType"];
  ptu_request_types_size_ = ptu_request_types_.size();
  pt_request_types_.clear();
  // Get RequestTypes from section of PT app policies after update
  pt_request_types_ = policy_manager_->GetAppRequestTypes(section_name);
  // Check number of RT in PTU and PT now are equal
  ASSERT_EQ(ptu_request_types_size_ - invalid_rt_number,
            pt_request_types_.size());
}

void PolicyManagerImplTest2::AddRTtoAppSectionPT(
    const std::string& update_file_name,
    const std::string& section_name,
    const uint32_t rt_number,
    const uint32_t invalid_rt_number) {
  // Arrange
  CreateLocalPT(preloaded_pt_filename_);
  // Add app
  policy_manager_->AddApplication(section_name,
                                  HmiTypes(policy_table::AHT_DEFAULT));
  // Check app gets RequestTypes from pre_DataConsent of app_policies
  // section
  pt_request_types_ = policy_manager_->GetAppRequestTypes(section_name);
  EXPECT_EQ(rt_number, pt_request_types_.size());
  EXPECT_CALL(listener_, OnPendingPermissionChange(section_name)).Times(1);
  Json::Value root = GetPTU(update_file_name);

  // Get App Request Types from PTU
  ptu_request_types_ =
      root["policy_table"]["app_policies"][section_name]["RequestType"];
  ptu_request_types_size_ = ptu_request_types_.size();

  pt_request_types_.clear();
  // Get RequestTypes from <app_id> section of app policies after PT update
  pt_request_types_ = policy_manager_->GetAppRequestTypes(section_name);
  // Check sizes of Request types of PT and PTU
  ASSERT_EQ(ptu_request_types_size_ - invalid_rt_number,
            pt_request_types_.size());

  ::policy::AppPermissions permissions =
      policy_manager_->GetAppPermissionsChanges(section_name);
  EXPECT_TRUE(permissions.requestTypeChanged);
}

std::vector<policy_table::RequestType>
PolicyManagerImplTest2::PushRequestTypesToContainer(
    const ::policy::StringArray& temp_result) {
  policy_table::RequestType filtered_result;
  std::vector<policy_table::RequestType> final_result;
  for (size_t i = 0; i < temp_result.size(); ++i) {
    if (policy_table::EnumFromJsonString(temp_result[i], &filtered_result)) {
      final_result.push_back(filtered_result);
    }
  }
  return final_result;
}

void PolicyManagerImplTest2::CheckResultForValidRT() {
  // Convert Json Array to ::policy::StringArray
  const ::policy::StringArray& result = JsonToVectorString(ptu_request_types_);
  // Checks
  SortAndCheckEquality(pt_request_types_, result);
}

void PolicyManagerImplTest2::CheckResultForInvalidRT() {
  // Convert Json Array to ::policy::StringArray
  const ::policy::StringArray& temp_result =
      JsonToVectorString(ptu_request_types_);
  const std::vector<policy_table::RequestType>& result1 =
      PushRequestTypesToContainer(temp_result);
  const std::vector<policy_table::RequestType>& result2 =
      PushRequestTypesToContainer(pt_request_types_);
  // Checks
  SortAndCheckEquality(result1, result2);
}

void PolicyManagerImplTest2::FillMultimapFromFunctionalGroupings(
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
    input_multimap.insert(
        std::pair<std::string, policy_table::Rpcs&>(ucp_std_string, rpcs_ref));
  }
}

void PolicyManagerImplTest2::GetFunctionalGroupingsFromManager(
    policy_table::FunctionalGroupings& input_functional_groupings) {
  // Get cache
  ::policy::CacheManagerInterfaceSPtr cache = policy_manager_->GetCache();
  // Get table_snapshot
  utils::SharedPtr<policy_table::Table> table = cache->GenerateSnapshot();
  // Set functional groupings from policy table
  input_functional_groupings = table->policy_table.functional_groupings;
}

void PolicyManagerImplTest2::TearDown() {
  delete policy_manager_;
  file_system::RemoveDirectory(app_storage_folder_, true);
}

void PolicyManagerImplTest2::ResetOutputList(
    ::policy::CheckPermissionResult& output) {
  // Reset output
  output.hmi_level_permitted = ::policy::kRpcDisallowed;
  output.list_of_allowed_params.clear();
  output.list_of_disallowed_params.clear();
  output.list_of_undefined_params.clear();
}
// To avoid duplicate test with different json files
void PolicyManagerImplTest2::
    CheckPermissions_AllParamsAllowed_CheckRpcsInDiffLvls(
        const std::string& update_file) {
  // Arrange
  CreateLocalPT("json/sdl_preloaded_pt_send_location.json");
  policy_manager_->AddDevice(device_id_1_, "Bluetooth");
  policy::CacheManagerInterfaceSPtr cache = policy_manager_->GetCache();
  ASSERT_TRUE(cache->SetDeviceData(device_id_1_,
                                   "hardware IPX",
                                   "v.8.0.1",
                                   "Android",
                                   "4.4.2",
                                   "Life",
                                   2,
                                   "Bluetooth"));

  // Add app from consented device. App will be assigned with default policies
  policy_manager_->AddApplication(application_id_,
                                  HmiTypes(policy_table::AHT_DEFAULT));

  // Expect all parameters are allowed
  std::ifstream ifile(update_file);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open()) {
    reader.parse(ifile, root, true);
  }
  json = root.toStyledString();
  ifile.close();
  ::policy::BinaryMessage msg(json.begin(), json.end());
  EXPECT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(cache->IsPTPreloaded());

  // Will be called each time permissions are checked
  EXPECT_CALL(listener_, OnCurrentDeviceIdUpdateRequired(application_id_))
      .Times(4)
      .WillRepeatedly(Return(device_id_1_));

  // Check RPC in each level
  ::policy::RPCParams input_params;
  InsertRpcParametersInList(input_params);

  ::policy::CheckPermissionResult output;
  // Rpc in FULL level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelFull, "SendLocation", input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  // Check list of allowed parameters is not empty
  EXPECT_FALSE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_allowed_params.size());
  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  ResetOutputList(output);

  // Rpc in LIMITED level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelLimited, "SendLocation", input_params, output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  // Check list of allowed parameters is not empty
  EXPECT_FALSE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_allowed_params.size());
  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  ResetOutputList(output);

  // Rpc in BACKGROUND level
  policy_manager_->CheckPermissions(application_id_,
                                    kHmiLevelBackground,
                                    "SendLocation",
                                    input_params,
                                    output);
  // Check RPC is allowed
  EXPECT_EQ(::policy::kRpcAllowed, output.hmi_level_permitted);
  // Check list of allowed parameters is not empty
  EXPECT_FALSE(output.list_of_allowed_params.empty());
  EXPECT_EQ(10u, output.list_of_allowed_params.size());
  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  // Reset output
  ResetOutputList(output);

  // Rpc in NONE level
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelNone, "SendLocation", input_params, output);
  // Check RPC is disallowed
  EXPECT_EQ(::policy::kRpcDisallowed, output.hmi_level_permitted);
  // Check lists of parameters are  empty
  EXPECT_TRUE(output.list_of_allowed_params.empty());
  EXPECT_TRUE(output.list_of_disallowed_params.empty());
  EXPECT_TRUE(output.list_of_undefined_params.empty());
}

void PolicyManagerImplTest2::CheckRpcPermissions(
    const std::string& rpc_name, const PermitResult& expected_permission) {
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
  policy_manager_->CheckPermissions(
      application_id_, kHmiLevelFull, rpc_name, input_params, output);
  EXPECT_EQ(expected_permission, output.hmi_level_permitted);
}

void PolicyManagerImplTest2::CheckRpcPermissions(
    const std::string& app_id,
    const std::string& rpc_name,
    const policy::PermitResult& out_expected_permission) {
  ::policy::RPCParams input_params;
  ::policy::CheckPermissionResult output;
  policy_manager_->CheckPermissions(
      app_id, kHmiLevelFull, rpc_name, input_params, output);
  EXPECT_EQ(out_expected_permission, output.hmi_level_permitted);
}

void PolicyManagerImplTest2::EmulatePTAppRevoked(const std::string& ptu_name) {
  std::ifstream ifile(ptu_name);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    // Emulate application is revoked
    root["policy_table"]["app_policies"]["1234"]["is_revoked"] = 1;
    json = root.toStyledString();
  }
  ifile.close();

  ::policy::BinaryMessage msg(json.begin(), json.end());
  ASSERT_TRUE(policy_manager_->LoadPT(kDummyUpdateFileName, msg));
}

// To avoid duplicate arrange of test
void PolicyManagerImplTest2::AddSetDeviceData() {
  CreateLocalPT("json/sdl_preloaded_pt_send_location.json");
  policy_manager_->AddDevice(device_id_1_, "Bluetooth");
  ASSERT_TRUE((policy_manager_->GetCache())
                  ->SetDeviceData(device_id_1_,
                                  "hardware IPX",
                                  "v.8.0.1",
                                  "Android",
                                  "4.4.2",
                                  "Life",
                                  2,
                                  "Bluetooth"));

  // Add app from consented device. App will be assigned with default policies
  policy_manager_->AddApplication(application_id_,
                                  HmiTypes(policy_table::AHT_DEFAULT));
  (policy_manager_->GetCache())->AddDevice(device_id_1_, "Bluetooth");
}

// Load Json File and set it as PTU
void PolicyManagerImplTest2::LoadPTUFromJsonFile(
    const std::string& update_file) {
  // Load Json to cache
  std::ifstream ifile(update_file);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open()) {
    reader.parse(ifile, root, true);
  }
  json = root.toStyledString();
  ifile.close();
  ::policy::BinaryMessage msg(json.begin(), json.end());
  EXPECT_TRUE(policy_manager_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(policy_manager_->GetCache()->IsPTPreloaded());
}

// PolicyManagerImplTest_RequestTypes class methods
PolicyManagerImplTest_RequestTypes::PolicyManagerImplTest_RequestTypes()
    : kJsonFiles{"json/PTU_with_one_invalid_requestType.json",
                 "json/PTU_with_invalid_requestType_between_correct.json",
                 "json/PTU_without_requestType_field.json",
                 "json/PTU_with_empty_requestType_array.json",
                 "json/preloadedPT_with_invalid_default_requestType.json",
                 "json/preloadedPT_with_several_invalid_default_requestTypes."
                 "json",
                 "json/"
                 "preloadedPT_with_invalid_default_reqestType_between_valid."
                 "json",
                 "json/PTU_default_app.json",
                 "json/"
                 "PTU_default_app_app_invalid_values_RequestType_array.json",
                 "json/PTU_default_app_empty_RequestType_array.json",
                 "json/PTU_default_app_omitted_RequestType_array.json",
                 "json/"
                 "PTU_default_app_one_invalid_value_RequestType_array.json",
                 "json/PTU_pre_data_consent_app.json",
                 "json/"
                 "PTU_pre_data_consent_app_invalid_values_RequestType_array."
                 "json",
                 "json/PTU_pre_data_consent_app_empty_RequestType_array.json",
                 "json/"
                 "PTU_pre_data_consent_app_omitted_RequestType_array.json",
                 "json/"
                 "PTU_pre_data_consent_app_one_invalid_value_RequestType_"
                 "array."
                 "json"}
    , kAppId("1766825573")
    , kDefaultAppId(policy::kDefaultId)
    , app_storage_folder_("storage3")
    , preloaded_pt_filename_(kSdlPreloadedPtJson) {}

void PolicyManagerImplTest_RequestTypes::SetUp() {
  ON_CALL(listener_, GetRegisteredLinks(_)).WillByDefault(Return());

  file_system::CreateDirectory(app_storage_folder_);
  const bool in_memory = true;
  policy_manager_impl_sptr_ = utils::MakeShared<PolicyManagerImpl>(in_memory);
  policy_manager_impl_sptr_->set_listener(&listener_);
}

const Json::Value PolicyManagerImplTest_RequestTypes::GetPTU(
    const std::string& file_name) {
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
  EXPECT_TRUE(policy_manager_impl_sptr_->LoadPT(kFilePtUpdateJson, msg));
  EXPECT_FALSE(policy_manager_impl_sptr_->GetCache()->IsPTPreloaded());
  return root;
}

void PolicyManagerImplTest_RequestTypes::RefreshPT(
    const std::string& preloaded_pt_file, const std::string& update_pt_file) {
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(app_storage_folder_));
  ASSERT_TRUE(
      policy_manager_impl_sptr_->InitPT(preloaded_pt_file, &policy_settings_))
      << "can`t load preloaded file";
  EXPECT_TRUE(policy_manager_impl_sptr_->GetCache()->IsPTPreloaded());
  GetPTU(update_pt_file);
}

PolicyTableSPtr PolicyManagerImplTest_RequestTypes::GetPolicyTableSnapshot() {
  // Get cache
  ::policy::CacheManagerInterfaceSPtr cache =
      policy_manager_impl_sptr_->GetCache();
  // Get and return table_snapshot
  return cache->GenerateSnapshot();
}

policy_table::RequestTypes
PolicyManagerImplTest_RequestTypes::GetRequestTypesForApplication(
    const std::string& app_id) {
  // Get table_snapshot
  PolicyTableSPtr table = GetPolicyTableSnapshot();
  // Get request types
  policy_table::PolicyTable& pt = table->policy_table;
  policy_table::ApplicationPolicies& app_policies =
      pt.app_policies_section.apps;
  policy_table::ApplicationPolicies::const_iterator app_iter =
      app_policies.find(app_id);

  return *(app_iter->second.RequestType);
}

void PolicyManagerImplTest_RequestTypes::CompareAppRequestTypesWithDefault(
    const std::string& app_id, const std::string& ptu_file) {
  // Refresh policy table with invalid RequestType in application
  RefreshPT(preloaded_pt_filename_, ptu_file);

  // Get <app_id> RequestType array
  policy_table::RequestTypes app_request_types =
      GetRequestTypesForApplication(app_id);
  // Get "default" RequestType array
  policy_table::RequestTypes default_request_types =
      GetRequestTypesForApplication(policy::kDefaultId);

  // Expect
  const size_t app_requests_size = app_request_types.size();
  const size_t default_requests_size = default_request_types.size();
  ASSERT_EQ(default_requests_size, app_requests_size);
  EXPECT_THAT(default_request_types, ContainerEq(app_request_types));
}

policy_table::RequestTypes
PolicyManagerImplTest_RequestTypes::CreateDefaultAppPTURequestValues() {
  policy_table::RequestTypes request_types;
  request_types.push_back(policy_table::RequestType::RT_QUERY_APPS);
  request_types.push_back(policy_table::RequestType::RT_LAUNCH_APP);
  request_types.push_back(policy_table::RequestType::RT_PROPRIETARY);
  request_types.push_back(policy_table::RequestType::RT_LOCK_SCREEN_ICON_URL);
  return request_types;
}

policy_table::RequestTypes
PolicyManagerImplTest_RequestTypes::CreateDefaultAppDatabaseRequestValues() {
  policy_table::RequestTypes request_types;
  request_types.push_back(policy_table::RequestType::RT_QUERY_APPS);
  request_types.push_back(policy_table::RequestType::RT_LAUNCH_APP);
  request_types.push_back(policy_table::RequestType::RT_PROPRIETARY);
  return request_types;
}

policy_table::RequestTypes
PolicyManagerImplTest_RequestTypes::CreatePreDataConsentAppPTURequestValues() {
  policy_table::RequestTypes request_types;
  request_types.push_back(policy_table::RequestType::RT_FILE_RESUME);
  request_types.push_back(policy_table::RequestType::RT_AUTH_REQUEST);
  request_types.push_back(policy_table::RequestType::RT_AUTH_CHALLENGE);
  request_types.push_back(policy_table::RequestType::RT_AUTH_ACK);
  return request_types;
}

void PolicyManagerImplTest_RequestTypes::CompareRequestTypesContainers(
    const policy_table::RequestTypes& expected_data,
    const policy_table::RequestTypes& received_data) {
  const size_t received_size = received_data.size();
  const size_t expected_size = expected_data.size();
  ASSERT_EQ(expected_size, received_size);
  EXPECT_THAT(expected_data, ContainerEq(received_data));
}

void PolicyManagerImplTest_RequestTypes::TearDown() {
  file_system::RemoveDirectory(app_storage_folder_, true);
}

void PolicyManagerImplTest_ExternalConsent::
    PreconditionExternalConsentPreparePTWithAppGroupsAndConsents() {
  using namespace policy_table;
  using namespace rpc;

  CreateLocalPT(preloaded_pt_filename_);
  Table t = PreparePTWithGroupsHavingExternalConsent();

  ApplicationParams app_params;
  app_params.groups.push_back(group_name_1_);
  app_params.groups.push_back(group_name_2_);

  t.policy_table.app_policies_section.apps.insert(
      std::make_pair(app_id_1_, app_params));

  EXPECT_TRUE(policy_manager_->GetCache()->ApplyUpdate(t));

  // User allows first group and disallows second group. Third is kept
  // untouched.
  PermissionConsent permissions;
  permissions.device_id = device_id_1_;
  permissions.policy_app_id = app_id_1_;

  FunctionalGroupPermission group_permissions_1;
  group_permissions_1.group_name = group_name_1_;
  group_permissions_1.group_alias = group_name_1_;
  group_permissions_1.group_id = utils::Djb2HashFromString(group_name_1_);
  group_permissions_1.state = kGroupAllowed;

  FunctionalGroupPermission group_permissions_2;
  group_permissions_2.group_name = group_name_2_;
  group_permissions_2.group_alias = group_name_2_;
  group_permissions_2.group_id = utils::Djb2HashFromString(group_name_2_);
  group_permissions_2.state = kGroupDisallowed;

  permissions.group_permissions.push_back(group_permissions_1);
  permissions.group_permissions.push_back(group_permissions_2);

  policy_manager_->SetUserConsentForApp(permissions);
}

void PolicyManagerImplTest_ExternalConsent::
    PreconditionExternalConsentPreparePTWithAppPolicy() {
  using namespace policy_table;
  using namespace rpc;

  // PT has 3 functional groups with some entities in
  // disallowed_by_external_consent_entities_on/off. Groups consents can be
  // changed.
  CreateLocalPT(preloaded_pt_filename_);
  Table t = PreparePTWithGroupsHavingExternalConsent();

  ApplicationParams app_params;
  app_params.groups.push_back(group_name_1_);
  app_params.groups.push_back(group_name_2_);

  t.policy_table.app_policies_section.apps.insert(
      std::make_pair(app_id_1_, app_params));

  EXPECT_TRUE(policy_manager_->GetCache()->ApplyUpdate(t));
}

rpc::policy_table_interface_base::Table PolicyManagerImplTest_ExternalConsent::
    PreparePTWithGroupsHavingExternalConsent() {
  using namespace policy_table;
  using namespace rpc;

  // PT has 3 functional groups with some entities in
  // disallowed_by_external_consent_entities_on/off. Groups consents can be
  // changed.

  ExternalConsentEntity entity_1(type_1_, id_1_);
  ExternalConsentEntity entity_2(type_2_, id_2_);
  ExternalConsentEntity entity_3(type_3_, id_3_);

  Rpcs rpcs_1;
  rpcs_1.disallowed_by_external_consent_entities_on->push_back(entity_1);
  *rpcs_1.user_consent_prompt = group_name_1_;
  rpcs_1.rpcs.set_to_null();

  Rpcs rpcs_2;
  rpcs_2.disallowed_by_external_consent_entities_off->push_back(entity_2);
  *rpcs_2.user_consent_prompt = group_name_2_;
  rpcs_2.rpcs.set_to_null();

  Rpcs rpcs_3;
  rpcs_3.disallowed_by_external_consent_entities_on->push_back(entity_3);
  *rpcs_3.user_consent_prompt = group_name_3_;
  rpcs_3.rpcs.set_to_null();

  Table t;
  t.policy_table.functional_groupings.insert(
      std::make_pair(group_name_1_, rpcs_1));
  t.policy_table.functional_groupings.insert(
      std::make_pair(group_name_2_, rpcs_2));
  t.policy_table.functional_groupings.insert(
      std::make_pair(group_name_3_, rpcs_3));

  return t;
}

std::string PolicyManagerImplTest_ExternalConsent::PreparePTUWithNewGroup(
    const uint32_t type, const uint32_t id, const std::string& group_name) {
  using namespace policy_table;
  using namespace rpc;

  std::ifstream ifile(preloaded_pt_filename_);
  Json::Reader reader;
  std::string json;
  Json::Value root(Json::objectValue);
  if (ifile.is_open() && reader.parse(ifile, root, true)) {
    Table t = PreparePTWithGroupsHavingExternalConsent();

    ExternalConsentEntity entity_4(type, id);

    Rpcs rpcs_4;
    rpcs_4.disallowed_by_external_consent_entities_on->push_back(entity_4);
    *rpcs_4.user_consent_prompt = group_name;
    rpcs_4.rpcs.set_to_null();

    t.policy_table.functional_groupings.insert(
        std::make_pair(group_name, rpcs_4));

    ApplicationParams app_params;
    app_params.groups.push_back(group_name_1_);
    app_params.groups.push_back(group_name_2_);
    app_params.groups.push_back(group_name);
    app_params.keep_context = Boolean(true);
    app_params.steal_focus = Boolean(true);
    app_params.default_hmi = HL_FULL;
    app_params.priority = P_EMERGENCY;

    t.policy_table.app_policies_section.apps.insert(
        std::make_pair(app_id_1_, app_params));

    const Json::Value overriden_table = t.ToJsonValue();

    const std::string policy_table_key = "policy_table";
    const std::string functional_groupings_key = "functional_groupings";
    const std::string app_policies_key = "app_policies";

    root[policy_table_key][functional_groupings_key] =
        overriden_table[policy_table_key][functional_groupings_key];

    root[policy_table_key][app_policies_key][app_id_1_] =
        overriden_table[policy_table_key][app_policies_key][app_id_1_];

    root[policy_table_key]["module_config"].removeMember("preloaded_pt");
    root[policy_table_key]["module_config"].removeMember("preloaded_date");

    json = root.toStyledString();
  }
  ifile.close();

  return json;
}

}  // namespace policy_test
}  // namespace components
}  // namespace test

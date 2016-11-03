/* Copyright (c) 2016, Ford Motor Company
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
#include <string>
#include <algorithm>
#include <fstream>
#include <memory>
#include <stdio.h>
#include <sys/stat.h>

#include "gtest/gtest.h"
#include "policy/driver_dbms.h"
#include "policy/sql_pt_representation.h"
#include "policy/policy_types.h"
#include "policy/mock_policy_settings.h"
#include "json/writer.h"
#include "json/reader.h"
#include "rpc_base/rpc_base.h"
#include "policy/mock_policy_settings.h"
#include "utils/shared_ptr.h"
#include "utils/make_shared.h"
#include "utils/file_system.h"
#include "policy/policy_table/types.h"
#include "policy/policy_table/enums.h"
#include "rpc_base/rpc_base.h"

#include "utils/sql_wrapper.h"

namespace policy_table = rpc::policy_table_interface_base;
using policy::SQLPTRepresentation;
using policy::CheckPermissionResult;
using policy::UserFriendlyMessage;
using policy::EndpointUrls;
using policy::VehicleInfo;

using testing::ReturnRef;
using testing::Return;
using testing::NiceMock;
using testing::Mock;
using utils::json::JsonValue;

namespace test {
namespace components {
namespace policy_test {

namespace {
const std::string kStorageFolder = ".";
const int kAttemptsToOpenPolicyDb = 5;
const int kOpenAttemptTimeoutMs = 7000;
}  // namespace

class SQLPTRepresentationTest : public SQLPTRepresentation,
                                public ::testing::Test {
 protected:
  SQLPTRepresentationTest()
      : SQLPTRepresentation(
            kStorageFolder, kAttemptsToOpenPolicyDb, kOpenAttemptTimeoutMs) {}
  static DBMS* dbms;
  static SQLPTRepresentation* reps;
  static const std::string kDatabaseName;
  // Gtest can show message that this object doesn't destroyed
  static std::unique_ptr<policy_handler_test::MockPolicySettings>
      policy_settings_;

  static void SetUpTestCase() {
    const std::string kAppStorageFolder = ".";
    file_system::RemoveDirectory(kAppStorageFolder, false);
    file_system::DeleteFile("policy.sqlite");
    reps = new SQLPTRepresentation(
        kStorageFolder, kAttemptsToOpenPolicyDb, kOpenAttemptTimeoutMs);
    dbms = new DBMS(kDatabaseName);
    policy_settings_ = std::unique_ptr<policy_handler_test::MockPolicySettings>(
        new policy_handler_test::MockPolicySettings());
    ON_CALL(*policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
    EXPECT_EQ(::policy::SUCCESS, reps->Init(policy_settings_.get()));
    EXPECT_TRUE(dbms->Open());
  }

  void TearDown() OVERRIDE {
    EXPECT_TRUE(reps->Clear());
  }

  static void TearDownTestCase() {
    EXPECT_TRUE(reps->Drop());
    EXPECT_TRUE(reps->Close());
    reps->RemoveDB();
    delete reps;
    dbms->Close();
    policy_settings_.reset();
    file_system::DeleteFile("policy.sqlite");
  }

  virtual utils::dbms::SQLDatabase* db() const OVERRIDE {
    return reps->db();
  }

  void GatherModuleMeta(policy_table::ModuleMeta* meta) const OVERRIDE {
    ::SQLPTRepresentation::GatherModuleMeta(meta);
  }

  void GatherModuleConfig(policy_table::ModuleConfig* config) const OVERRIDE {
    ::SQLPTRepresentation::GatherModuleConfig(config);
  }

  bool GatherUsageAndErrorCounts(
      policy_table::UsageAndErrorCounts* counts) const OVERRIDE {
    return ::SQLPTRepresentation::GatherUsageAndErrorCounts(counts);
  }

  bool GatherApplicationPoliciesSection(
      policy_table::ApplicationPoliciesSection* policies) const OVERRIDE {
    return ::SQLPTRepresentation::GatherApplicationPoliciesSection(policies);
  }
  virtual void GatherDeviceData(policy_table::DeviceData* data) const OVERRIDE {
    ::SQLPTRepresentation::GatherDeviceData(data);
  }

  virtual bool GatherConsumerFriendlyMessages(
      policy_table::ConsumerFriendlyMessages* messages) const OVERRIDE {
    return ::SQLPTRepresentation::GatherConsumerFriendlyMessages(messages);
  }

  bool GatherAppGroup(const std::string& app_id,
                      policy_table::Strings* app_groups) const {
    return ::SQLPTRepresentation::GatherAppGroup(app_id, app_groups);
  }

  bool GatherAppType(const std::string& app_id,
                     policy_table::AppHMITypes* app_types) const {
    return ::SQLPTRepresentation::GatherAppType(app_id, app_types);
  }

  bool GatherRequestType(const std::string& app_id,
                         policy_table::RequestTypes* request_types) const {
    return ::SQLPTRepresentation::GatherRequestType(app_id, request_types);
  }

  bool GatherNickName(const std::string& app_id,
                      policy_table::Strings* nicknames) const {
    return ::SQLPTRepresentation::GatherNickName(app_id, nicknames);
  }

  void CheckAppPoliciesSection(
      policy_table::ApplicationPoliciesSection& policies,
      uint16_t apps_size,
      policy_table::Priority prio,
      const std::string& section,
      uint16_t memory_kb,
      uint32_t heart_beat_timeout_ms,
      policy_table::Strings& groups) const {
    if (section != "device") {
      policy_table::ApplicationPolicies& apps = policies.apps;
      EXPECT_EQ(apps_size, apps.size());
      policy_table::ApplicationPolicies::iterator apps_iter =
          apps.find(section);
      ASSERT_TRUE(apps.end() != apps_iter);
      policy_table::Strings& temp_groups = apps_iter->second.groups;
      StringsCompare(groups, temp_groups);
      EXPECT_EQ(0u, (*(apps_iter->second.nicknames)).size());
      EXPECT_EQ(prio, apps_iter->second.priority);
      EXPECT_EQ(0u, (*(apps_iter->second.AppHMIType)).size());
      EXPECT_EQ(memory_kb, (*(apps_iter->second.memory_kb)));
      EXPECT_EQ(heart_beat_timeout_ms,
                (*(apps_iter->second.heart_beat_timeout_ms)));
    } else {
      policy_table::DevicePolicy& device = policies.device;
      EXPECT_EQ(prio, device.priority);
    }
  }

  void StringsCompare(policy_table::Strings& groups1,
                      policy_table::Strings& groups2) const {
    EXPECT_EQ(groups1.size(), groups2.size());
    std::sort(groups1.begin(), groups1.end());
    std::sort(groups2.begin(), groups2.end());
    EXPECT_TRUE(groups1 == groups2);
  }

  void CheckAppGroups(const std::string& app_id,
                      policy_table::Strings& groups) {
    policy_table::Strings app_groups;
    GatherAppGroup(app_id, &app_groups);
    StringsCompare(groups, app_groups);
  }

  void PolicyTableUpdatePrepare(utils::json::JsonValueRef table) {
    table["policy_table"] = utils::json::ValueType::OBJECT_VALUE;
    utils::json::JsonValueRef policy_table = table["policy_table"];
    policy_table["module_config"] = utils::json::ValueType::OBJECT_VALUE;
    policy_table["functional_groupings"] = utils::json::ValueType::OBJECT_VALUE;
    policy_table["consumer_friendly_messages"] =
        utils::json::ValueType::OBJECT_VALUE;
    policy_table["app_policies"] = utils::json::ValueType::OBJECT_VALUE;

    utils::json::JsonValueRef module_config = policy_table["module_config"];
    module_config["preloaded_pt"] = utils::json::JsonValue(true);
    module_config["preloaded_date"] = utils::json::JsonValue("");
    module_config["exchange_after_x_ignition_cycles"] = JsonValue::Int(10);
    module_config["exchange_after_x_kilometers"] = JsonValue::Int(100);
    module_config["exchange_after_x_days"] = JsonValue::Int(5);
    module_config["timeout_after_x_seconds"] = JsonValue::Int(500);
    module_config["seconds_between_retries"] =
        utils::json::ValueType::ARRAY_VALUE;
    utils::json::JsonValueRef seconds_between_retries =
        module_config["seconds_between_retries"];
    seconds_between_retries[uint32_t(0)] = JsonValue::Int(10);
    seconds_between_retries[1] = JsonValue::Int(20);
    seconds_between_retries[2] = JsonValue::Int(30);
    module_config["endpoints"] = utils::json::ValueType::OBJECT_VALUE;
    utils::json::JsonValueRef endpoins = module_config["endpoints"];
    endpoins["0x00"] = utils::json::ValueType::OBJECT_VALUE;
    endpoins["0x00"]["default"] = utils::json::ValueType::ARRAY_VALUE;
    endpoins["0x00"]["default"][uint32_t(0)] =
        JsonValue("http://ford.com/cloud/default");
    module_config["notifications_per_minute_by_priority"] =
        utils::json::ValueType::OBJECT_VALUE;
    module_config["notifications_per_minute_by_priority"]["emergency"] =
        JsonValue::Int(1);
    module_config["notifications_per_minute_by_priority"]["navigation"] =
        JsonValue::Int(2);
    module_config["notifications_per_minute_by_priority"]["VOICECOMM"] =
        JsonValue::Int(3);
    module_config["notifications_per_minute_by_priority"]["communication"] =
        JsonValue::Int(4);
    module_config["notifications_per_minute_by_priority"]["normal"] =
        JsonValue::Int(5);
    module_config["notifications_per_minute_by_priority"]["none"] =
        JsonValue::Int(6);
    module_config["vehicle_make"] = JsonValue("Ford");
    module_config["vehicle_model"] = JsonValue("Mustang");
    module_config["vehicle_year"] = JsonValue("2016");
    module_config["certificate"] = JsonValue("");

    utils::json::JsonValueRef functional_groupings =
        policy_table["functional_groupings"];
    functional_groupings["default"] = utils::json::ValueType::OBJECT_VALUE;
    utils::json::JsonValueRef default_group = functional_groupings["default"];
    default_group["rpcs"] = utils::json::ValueType::OBJECT_VALUE;
    default_group["rpcs"]["Update"] = utils::json::ValueType::OBJECT_VALUE;
    default_group["rpcs"]["Update"]["hmi_levels"] =
        utils::json::ValueType::ARRAY_VALUE;
    default_group["rpcs"]["Update"]["hmi_levels"][uint32_t(0)] =
        JsonValue("FULL");
    default_group["rpcs"]["Update"]["parameters"] =
        utils::json::ValueType::ARRAY_VALUE;
    default_group["rpcs"]["Update"]["parameters"][uint32_t(0)] =
        JsonValue("speed");

    utils::json::JsonValueRef consumer_friendly_messages =
        policy_table["consumer_friendly_messages"];
    consumer_friendly_messages["version"] = JsonValue("1.2");
    consumer_friendly_messages["messages"] =
        utils::json::ValueType::OBJECT_VALUE;
    consumer_friendly_messages["messages"]["MSG1"] =
        utils::json::ValueType::OBJECT_VALUE;
    utils::json::JsonValueRef msg1 =
        consumer_friendly_messages["messages"]["MSG1"];
    msg1["languages"] = utils::json::ValueType::OBJECT_VALUE;
    msg1["languages"]["en-us"] = utils::json::ValueType::OBJECT_VALUE;
    msg1["languages"]["en-us"]["tts"] = JsonValue("TTS message");
    msg1["languages"]["en-us"]["label"] = JsonValue("LABEL message");
    msg1["languages"]["en-us"]["line1"] = JsonValue("LINE1 message");
    msg1["languages"]["en-us"]["line2"] = JsonValue("LINE2 message");
    msg1["languages"]["en-us"]["textBody"] = JsonValue("TEXTBODY message");

    utils::json::JsonValueRef app_policies = policy_table["app_policies"];
    app_policies["default"] = utils::json::ValueType::OBJECT_VALUE;
    app_policies["default"]["priority"] = JsonValue("EMERGENCY");
    app_policies["default"]["memory_kb"] = JsonValue::Int(50);
    app_policies["default"]["heart_beat_timeout_ms"] = JsonValue::Int(100);
    app_policies["default"]["groups"] = utils::json::ValueType::ARRAY_VALUE;
    app_policies["default"]["groups"][uint32_t(0)] = JsonValue("default");
    app_policies["default"]["priority"] = JsonValue("EMERGENCY");
    app_policies["default"]["is_revoked"] = utils::json::JsonValue(true);
    app_policies["default"]["default_hmi"] = utils::json::JsonValue("FULL");
    app_policies["default"]["keep_context"] = utils::json::JsonValue(true);
    app_policies["default"]["steal_focus"] = utils::json::JsonValue(true);

    app_policies["pre_DataConsent"] = utils::json::ValueType::OBJECT_VALUE;
    app_policies["pre_DataConsent"]["memory_kb"] = JsonValue::Int(40);
    app_policies["pre_DataConsent"]["heart_beat_timeout_ms"] =
        JsonValue::Int(90);
    app_policies["pre_DataConsent"]["groups"] =
        utils::json::ValueType::ARRAY_VALUE;
    app_policies["pre_DataConsent"]["groups"][uint32_t(0)] =
        JsonValue("default");
    app_policies["pre_DataConsent"]["priority"] = JsonValue("EMERGENCY");
    app_policies["pre_DataConsent"]["default_hmi"] =
        utils::json::JsonValue("FULL");
    app_policies["pre_DataConsent"]["is_revoked"] =
        utils::json::JsonValue(false);
    app_policies["pre_DataConsent"]["keep_context"] =
        utils::json::JsonValue(true);
    app_policies["pre_DataConsent"]["steal_focus"] =
        utils::json::JsonValue(true);
    app_policies["1234"] = utils::json::ValueType::OBJECT_VALUE;
    app_policies["1234"]["memory_kb"] = JsonValue::Int(150);
    app_policies["1234"]["heart_beat_timeout_ms"] = JsonValue::Int(200);
    app_policies["1234"]["groups"] = utils::json::ValueType::ARRAY_VALUE;
    app_policies["1234"]["groups"][uint32_t(0)] = JsonValue("default");
    app_policies["1234"]["priority"] = JsonValue("EMERGENCY");
    app_policies["1234"]["default_hmi"] = utils::json::ValueType::OBJECT_VALUE;
    app_policies["1234"]["default_hmi"] = utils::json::JsonValue("FULL");
    app_policies["1234"]["is_revoked"] = utils::json::JsonValue(true);
    app_policies["1234"]["keep_context"] = utils::json::JsonValue(false);
    app_policies["1234"]["steal_focus"] = utils::json::JsonValue(false);
    app_policies["device"] = utils::json::ValueType::OBJECT_VALUE;
    app_policies["device"]["groups"] = utils::json::ValueType::ARRAY_VALUE;
    app_policies["device"]["groups"][uint32_t(0)] = JsonValue("default");
    app_policies["device"]["priority"] = JsonValue("EMERGENCY");
    app_policies["device"]["is_revoked"] = utils::json::JsonValue(true);
    app_policies["device"]["default_hmi"] = utils::json::JsonValue("FULL");
    app_policies["device"]["keep_context"] = utils::json::JsonValue(true);
    app_policies["device"]["steal_focus"] = utils::json::JsonValue(true);
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

DBMS* SQLPTRepresentationTest::dbms = 0;
SQLPTRepresentation* SQLPTRepresentationTest::reps = 0;
const std::string SQLPTRepresentationTest::kDatabaseName = "policy.sqlite";
std::unique_ptr<policy_handler_test::MockPolicySettings>
    SQLPTRepresentationTest::policy_settings_;

class SQLPTRepresentationTest2 : public ::testing::Test {
 protected:
  SQLPTRepresentationTest2()
      : kAppStorageFolder("storage1")
      , kOpenAttemptTimeoutMs(700u)
      , kAttemptsToOpenPolicyDB(8u) {}

  void SetUp() OVERRIDE {
    file_system::CreateDirectory(kAppStorageFolder);
    chmod(kAppStorageFolder.c_str(), 00000);
    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
    ON_CALL(policy_settings_, open_attempt_timeout_ms())
        .WillByDefault(Return(kOpenAttemptTimeoutMs));
    ON_CALL(policy_settings_, attempts_to_open_policy_db())
        .WillByDefault(Return(kAttemptsToOpenPolicyDB));
    reps = new SQLPTRepresentation(
        kAppStorageFolder, kAttemptsToOpenPolicyDB, kAttemptsToOpenPolicyDB);
  }

  void TearDown() OVERRIDE {
    file_system::RemoveDirectory(kAppStorageFolder, true);
    delete reps;
  }

  SQLPTRepresentation* reps;
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;
  const std::string kAppStorageFolder;
  const uint16_t kOpenAttemptTimeoutMs;
  const uint16_t kAttemptsToOpenPolicyDB;
};

// {AKozoriz} : Unknown behavior (must try 8 times, tried 2 and opened)
TEST_F(SQLPTRepresentationTest2,
       DISABLED_OpenAttemptTimeOut_ExpectCorrectNumber) {
  EXPECT_EQ(::policy::FAIL, reps->Init(&policy_settings_));
  // Check  Actual attempts number made to try to open DB
  EXPECT_EQ(kAttemptsToOpenPolicyDB, reps->open_counter());
  // Check timeot value correctly read from config file.
  EXPECT_EQ(700u, kOpenAttemptTimeoutMs);
}

TEST_F(SQLPTRepresentationTest,
       RefreshDB_DropExistedPTThenRefreshDB_ExpectTablesWithInitialData) {
  // Check
  const char* query_select =
      "SELECT COUNT(*) FROM sqlite_master WHERE `type` = 'table'";
  // In normally created PT there are more than 0 tables
  ASSERT_GT(dbms->FetchOneInt(query_select), 0);
  ASSERT_TRUE(reps->Drop());
  ASSERT_EQ(0, dbms->FetchOneInt(query_select));
  ASSERT_TRUE(reps->RefreshDB());
  // Check PT structure destroyed and tables number is 0
  ASSERT_EQ(25, dbms->FetchOneInt(query_select));
  const char* query_select_count_of_iap_buffer_full =
      "SELECT `count_of_iap_buffer_full` FROM `usage_and_error_count`";
  const char* query_select_count_sync_out_of_memory =
      "SELECT `count_sync_out_of_memory` FROM `usage_and_error_count`";
  const char* query_select_count_of_sync_reboots =
      "SELECT `count_of_sync_reboots` FROM `usage_and_error_count`";
  const char* query_select_pt_exchanged_at_odometer_x =
      "SELECT `pt_exchanged_at_odometer_x` FROM `module_meta`";
  const char* query_select_pt_exchanged_x_days_after_epoch =
      "SELECT `pt_exchanged_x_days_after_epoch` FROM `module_meta`";
  const char* query_select_flag_update_required =
      "SELECT `flag_update_required` FROM `module_meta`";
  const char* query_select_ignition_cycles_since_last_exchange =
      "SELECT `ignition_cycles_since_last_exchange` FROM `module_meta`";
  const char* query_select_preloaded_pt =
      "SELECT `preloaded_pt` FROM `module_config`";
  const char* query_select_is_first_run =
      "SELECT `is_first_run` FROM `module_config`";
  const char* query_select_exchange_after_x_ignition_cycles =
      "SELECT `exchange_after_x_ignition_cycles` FROM `module_config`";
  const char* query_select_exchange_after_x_kilometers =
      "SELECT `exchange_after_x_kilometers` FROM `module_config`";
  const char* query_select_exchange_after_x_days =
      "SELECT `exchange_after_x_days` FROM `module_config`";
  const char* query_select_timeout_after_x_seconds =
      "SELECT `timeout_after_x_seconds` FROM `module_config`";
  const char* query_select_priorities = "SELECT COUNT(`value`) FROM `priority`";
  const char* query_select_hmi_levels =
      "SELECT COUNT(`value`) FROM `hmi_level`";
  const char* query_select_version = "SELECT `number` FROM `version`";

  ASSERT_EQ(0, dbms->FetchOneInt(query_select_count_of_iap_buffer_full));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_count_sync_out_of_memory));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_count_of_sync_reboots));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_pt_exchanged_at_odometer_x));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_pt_exchanged_x_days_after_epoch));
  ASSERT_EQ(
      0, dbms->FetchOneInt(query_select_ignition_cycles_since_last_exchange));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_flag_update_required));
  ASSERT_EQ(1, dbms->FetchOneInt(query_select_preloaded_pt));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_is_first_run));
  ASSERT_EQ(0,
            dbms->FetchOneInt(query_select_exchange_after_x_ignition_cycles));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_exchange_after_x_kilometers));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_exchange_after_x_days));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_timeout_after_x_seconds));
  ASSERT_EQ(6, dbms->FetchOneInt(query_select_priorities));
  ASSERT_EQ(4, dbms->FetchOneInt(query_select_hmi_levels));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_version));
}

TEST_F(
    SQLPTRepresentationTest,
    CheckPermissionsAllowed_SetValuesInAppGroupRpcFunctionalGroup_GetEqualParamsInCheckPermissionResult) {
  // Arrange
  const char* query =
      "INSERT OR REPLACE INTO `application` (`id`, `memory_kb`,"
      " `heart_beat_timeout_ms`) VALUES ('12345', 5, 10); "
      "INSERT OR REPLACE INTO functional_group (`id`, `name`)"
      "  VALUES (1, 'Base-4'); "
      "INSERT OR REPLACE INTO `app_group` (`application_id`,"
      " `functional_group_id`) VALUES ('12345', 1); "
      "INSERT OR REPLACE INTO `rpc` (`name`, `parameter`, `hmi_level_value`,"
      " `functional_group_id`) VALUES ('Update', 'gps', 'FULL', 1); "
      "INSERT OR REPLACE INTO `rpc` (`name`, `parameter`, `hmi_level_value`,"
      " `functional_group_id`) VALUES ('Update', 'speed', 'FULL', 1);";

  // Assert
  ASSERT_TRUE(dbms->Exec(query));

  // Act
  CheckPermissionResult ret;
  reps->CheckPermissions("12345", "FULL", "Update", ret);

  // Assert
  EXPECT_TRUE(ret.hmi_level_permitted == ::policy::kRpcAllowed);
  ASSERT_EQ(2u, ret.list_of_allowed_params.size());
  EXPECT_EQ("gps", ret.list_of_allowed_params[0]);
  EXPECT_EQ("speed", ret.list_of_allowed_params[1]);
}

TEST_F(
    SQLPTRepresentationTest,
    CheckPermissionsAllowedWithoutParameters_SetLimitedPermissions_ExpectEmptyListOfAllowedParams) {
  // Arrange
  const char* query =
      "INSERT OR REPLACE INTO `application` (`id`, `memory_kb`,"
      " `heart_beat_timeout_ms`) VALUES ('12345', 5, 10); "
      "INSERT OR REPLACE INTO functional_group (`id`, `name`)"
      "  VALUES (1, 'Base-4'); "
      "INSERT OR REPLACE INTO `app_group` (`application_id`,"
      " `functional_group_id`) VALUES ('12345', 1); "
      "DELETE FROM `rpc`; "
      "INSERT OR REPLACE INTO `rpc` (`name`, `hmi_level_value`,"
      " `functional_group_id`) VALUES ('Update', 'LIMITED', 1);";

  // Assert
  ASSERT_TRUE(dbms->Exec(query));

  // Act
  CheckPermissionResult ret;
  reps->CheckPermissions("12345", "LIMITED", "Update", ret);

  // Assert
  EXPECT_TRUE(ret.hmi_level_permitted == ::policy::kRpcAllowed);
  EXPECT_TRUE(ret.list_of_allowed_params.empty());
}

TEST_F(
    SQLPTRepresentationTest,
    CheckPermissionsDisallowedWithoutParameters_DeletedAppGroupAndSetFULLLevel_ExpectHmiLevelIsDissalowed) {
  // Arrange
  const char* query = "DELETE FROM `app_group`";

  // Assert
  ASSERT_TRUE(dbms->Exec(query));

  // Act
  CheckPermissionResult ret;
  reps->CheckPermissions("12345", "FULL", "Update", ret);

  // Assert
  EXPECT_EQ(::policy::kRpcDisallowed, ret.hmi_level_permitted);
  EXPECT_TRUE(ret.list_of_allowed_params.empty());
}

TEST_F(SQLPTRepresentationTest,
       PTPReloaded_UpdateModuleConfig_ReturnIsPTPreloadedTRUE) {
  // Arrange
  const char* query = "UPDATE `module_config` SET `preloaded_pt` = 1";

  // Assert
  ASSERT_TRUE(dbms->Exec(query));
  EXPECT_TRUE(reps->IsPTPreloaded());
}

TEST_F(SQLPTRepresentationTest,
       GetUpdateUrls_DeleteAndInsertEndpoints_ExpectUpdateUrls) {
  // Arrange
  const char* query_delete = "DELETE FROM `endpoint`; ";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_delete));

  // Act
  EndpointUrls ret = reps->GetUpdateUrls(7);

  // Assert
  EXPECT_TRUE(ret.empty());

  // Act
  const char* query_insert =
      "INSERT INTO `endpoint` (`application_id`, `url`, `service`) "
      "  VALUES ('12345', 'http://ford.com/cloud/1', 7);"
      "INSERT INTO `endpoint` (`application_id`, `url`, `service`) "
      "  VALUES ('12345', 'http://ford.com/cloud/2', 7);";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_insert));
  // Act
  ret = reps->GetUpdateUrls(7);

  // Assert
  ASSERT_EQ(2u, ret.size());
  EXPECT_EQ("http://ford.com/cloud/1", ret[0].url[0]);
  EXPECT_EQ("http://ford.com/cloud/2", ret[1].url[0]);

  // Act
  ret = reps->GetUpdateUrls(0);

  // Assert
  EXPECT_TRUE(ret.empty());
}

TEST_F(SQLPTRepresentationTest,
       IgnitionCyclesBeforeExchange_WithParametersOfQueryEqualZero) {
  // Arrange
  const char* query_zeros =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 0; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 0";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_zeros));
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());

  // Act
  reps->IncrementIgnitionCycles();

  // Assert
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       IgnitionCyclesBeforeExchange_WithParametersOfQueryAreLessLimit) {
  // Arrange
  const char* query_less_limit =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 5; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 10";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_less_limit));
  EXPECT_EQ(5, reps->IgnitionCyclesBeforeExchange());

  // Act
  reps->IncrementIgnitionCycles();

  // Assert
  EXPECT_EQ(4, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       IgnitionCyclesBeforeExchange_WithLimitCountOfParametersOfQuery) {
  // Arrange
  const char* query_limit =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 9; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 10";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_limit));
  EXPECT_EQ(1, reps->IgnitionCyclesBeforeExchange());
  // Act
  reps->IncrementIgnitionCycles();
  // Assert
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       IgnitionCyclesBeforeExchange_WithMoreLimitCountOfParametersOfQuery) {
  // Arrange
  const char* query_more_limit =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 12; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 10";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_more_limit));
  // Chceck
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       IgnitionCyclesBeforeExchange_WithNegativeLimitOfParametersOfQuery) {
  // Arrange
  const char* query_negative_limit =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 3; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = -1";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_negative_limit));
  // Check
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(
    SQLPTRepresentationTest,
    IgnitionCyclesBeforeExchange_WithNegativeLimitOfCurrentParameterOfQuery) {
  // Arrange
  const char* query_negative_current =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = -1; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 2";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_negative_current));
  // Check
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       KilometersBeforeExchange_WithParametersOfQueryEqualZero) {
  // Arrange
  const char* query_zeros =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_at_odometer_x` = 0; "
      "  UPDATE `module_config` SET `exchange_after_x_kilometers` = 0";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_zeros));
  // Checks
  EXPECT_EQ(0, reps->KilometersBeforeExchange(0));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(-10));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest,
       KilometersBeforeExchange_QueryWithNegativeLimit) {
  // Arrange
  const char* query_negative_limit =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_at_odometer_x` = 10; "
      "  UPDATE `module_config` SET `exchange_after_x_kilometers` = -10";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_negative_limit));
  // Checks
  EXPECT_EQ(0, reps->KilometersBeforeExchange(0));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest,
       KilometersBeforeExchange_QueryWithNegativeCurrentLimit) {
  // Arrange
  const char* query_negative_last =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_at_odometer_x` = -10; "
      "  UPDATE `module_config` SET `exchange_after_x_kilometers` = 20";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_negative_last));
  // Checks
  EXPECT_EQ(0, reps->KilometersBeforeExchange(0));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest,
       KilometersBeforeExchange_QueryWithLimitParameters) {
  // Arrange
  const char* query_limit =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_at_odometer_x` = 10; "
      "  UPDATE `module_config` SET `exchange_after_x_kilometers` = 100";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_limit));
  // Checks
  EXPECT_EQ(0, reps->KilometersBeforeExchange(120));
  EXPECT_EQ(60, reps->KilometersBeforeExchange(50));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(5));
}

TEST_F(SQLPTRepresentationTest,
       DaysBeforeExchange_WithParametersOfQueryEqualZero) {
  // Arrange
  const char* query_zeros =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_x_days_after_epoch` = 0; "
      "  UPDATE `module_config` SET `exchange_after_x_days` = 0";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_zeros));
  // Checks
  EXPECT_EQ(0, reps->DaysBeforeExchange(0));
  EXPECT_EQ(0, reps->DaysBeforeExchange(-10));
  EXPECT_EQ(0, reps->DaysBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest, DaysBeforeExchange_QueryWithNegativeLimit) {
  // Arrange
  const char* query_negative_limit =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_x_days_after_epoch` = 10; "
      "  UPDATE `module_config` SET `exchange_after_x_days` = -10";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_negative_limit));
  // Checks
  EXPECT_EQ(0, reps->DaysBeforeExchange(0));
  EXPECT_EQ(0, reps->DaysBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest,
       DaysBeforeExchange_QueryWithNegativeCurrentLimit) {
  // Arrange
  const char* query_negative_last =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_x_days_after_epoch` = -10; "
      "  UPDATE `module_config` SET `exchange_after_x_days` = 20";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_negative_last));
  // Checks
  EXPECT_EQ(0, reps->DaysBeforeExchange(0));
  EXPECT_EQ(0, reps->DaysBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest, DaysBeforeExchange_QueryWithLimitParameters) {
  // Arrange
  const char* query_limit =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_x_days_after_epoch` = 10; "
      "  UPDATE `module_config` SET `exchange_after_x_days` = 100";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_limit));
  // Checks
  EXPECT_EQ(0, reps->DaysBeforeExchange(120));
  EXPECT_EQ(60, reps->DaysBeforeExchange(50));
  EXPECT_EQ(0, reps->DaysBeforeExchange(5));
}

TEST_F(
    SQLPTRepresentationTest,
    SecondsBetweenRetries_DeletedAndInsertedSecondsBetweenRetry_ExpectCountOfSecondsEqualInserted) {
  // Arrange
  std::vector<int> seconds;
  const char* query_delete = "DELETE FROM `seconds_between_retry`; ";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_delete));
  ASSERT_TRUE(reps->SecondsBetweenRetries(&seconds));
  EXPECT_EQ(0u, seconds.size());

  // Arrange
  const char* query_insert =
      "INSERT INTO `seconds_between_retry` (`index`, `value`) "
      "  VALUES (0, 10); "
      "INSERT INTO `seconds_between_retry` (`index`, `value`) "
      "  VALUES (1, 20); ";

  // Assert
  ASSERT_TRUE(dbms->Exec(query_insert));
  ASSERT_TRUE(reps->SecondsBetweenRetries(&seconds));
  // Checks
  ASSERT_EQ(2u, seconds.size());
  EXPECT_EQ(10, seconds[0]);
  EXPECT_EQ(20, seconds[1]);
}

TEST_F(SQLPTRepresentationTest, TimeoutResponse_Set60Seconds_GetEqualTimeout) {
  // Arrange
  const char* query =
      "UPDATE `module_config` SET `timeout_after_x_seconds` = 60";

  // Assert
  ASSERT_TRUE(dbms->Exec(query));
  // Check
  EXPECT_EQ(60, reps->TimeoutResponse());
}

TEST_F(SQLPTRepresentationTest,
       IsPTPreloaded_SetPTPreloadedThenCheck_ExpectCorrectValue) {
  // Arrange
  const char* query_insert = "UPDATE `module_config` SET `preloaded_pt` = 1";
  ASSERT_TRUE(dbms->Exec(query_insert));
  // Check
  ASSERT_TRUE(reps->IsPTPreloaded());
}

TEST_F(
    SQLPTRepresentationTest,
    SetCountersPassedForSuccessfulUpdate_SetCounters_ExpectValueChangedInPT) {
  // Arrange
  const char* query_select_odometer =
      "SELECT  `pt_exchanged_at_odometer_x` FROM`module_meta`";
  const char* query_select_days_after_epoch =
      "SELECT `pt_exchanged_x_days_after_epoch` FROM`module_meta`";
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_odometer));
  ASSERT_EQ(0, dbms->FetchOneInt(query_select_days_after_epoch));
  // Act
  ASSERT_TRUE(reps->SetCountersPassedForSuccessfulUpdate(100, 10000));
  ASSERT_EQ(100, dbms->FetchOneInt(query_select_odometer));
  ASSERT_EQ(10000, dbms->FetchOneInt(query_select_days_after_epoch));
}

TEST_F(
    SQLPTRepresentationTest,
    IncrementIgnitionCycles_SetIgnitionCyclesValueThenIncrement_ExpectValueIncrementedInPT) {
  // Arrange
  const char* query_insert =
      "UPDATE `module_meta` SET `ignition_cycles_since_last_exchange` = 54";
  const char* query_select =
      "SELECT `ignition_cycles_since_last_exchange`FROM `module_meta`";
  ASSERT_TRUE(dbms->Exec(query_insert));
  // Act
  reps->IncrementIgnitionCycles();
  // Check
  ASSERT_EQ(55, dbms->FetchOneInt(query_select));
}

TEST_F(
    SQLPTRepresentationTest,
    ResetIgnitionCycles_SetIgnitionCyclesValueThenReset_ExpectZeroValueInPT) {
  // Arrange
  const char* query_insert =
      "UPDATE `module_meta` SET `ignition_cycles_since_last_exchange` = 55";
  const char* query_select =
      "SELECT `ignition_cycles_since_last_exchange` FROM `module_meta`";
  ASSERT_TRUE(dbms->Exec(query_insert));
  // Act
  reps->ResetIgnitionCycles();
  // Check
  ASSERT_EQ(0, dbms->FetchOneInt(query_select));
}

TEST_F(SQLPTRepresentationTest,
       GetUserFriendlyMsg_SetMsg_ExpectReceivedMsgSetInParams) {
  // Arrange

  const char* query_insert =
      "INSERT INTO `message` (`language_code`, `message_type_name`) VALUES "
      "('en-en', 'AppPermissions')";

  ASSERT_TRUE(dbms->Exec(query_insert));
  query_insert =
      "INSERT INTO `message_type` (`name`) VALUES ('AppPermissions')";
  ASSERT_TRUE(dbms->Exec(query_insert));
  std::vector<std::string> msg_code;
  msg_code.push_back("AppPermissions");
  // Act
  std::vector<UserFriendlyMessage> result =
      reps->GetUserFriendlyMsg(msg_code, std::string("en-en"));
  // Checks
  ASSERT_EQ(1u, result.size());
  EXPECT_EQ(result[0].message_code, "AppPermissions");
}

TEST_F(
    SQLPTRepresentationTest,
    GetNotificationNumber_SetNotificationsPriorities_ExpectReceivedValuesCorrect) {
  // Arrange
  const char* query_insert =
      "INSERT INTO `notifications_by_priority` (`priority_value`, `value`) "
      "VALUES ('NAVIGATION', 15) , "
      "('COMMUNICATION', 6), ('EMERGENCY', 60), ('NONE', 0), ('NORMAL', 4), "
      "('VOICECOMMUNICATION', 20)";

  ASSERT_TRUE(dbms->Exec(query_insert));
  EXPECT_EQ(6, reps->GetNotificationsNumber("COMMUNICATION"));
  EXPECT_EQ(60, reps->GetNotificationsNumber("EMERGENCY"));
  EXPECT_EQ(15, reps->GetNotificationsNumber("NAVIGATION"));
  EXPECT_EQ(0, reps->GetNotificationsNumber("NONE"));
  EXPECT_EQ(4, reps->GetNotificationsNumber("NORMAL"));
  EXPECT_EQ(20, reps->GetNotificationsNumber("VOICECOMMUNICATION"));
}

TEST_F(SQLPTRepresentationTest,
       GetPriority_SetAppsPrioritiesThenGet_ExpectReceivedValuesCorrect) {
  // Arrange
  const char* query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'default', 0, 0, 'NONE', 'NONE', 0, "
      "0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'pre_DataConsent', 0, 0, 'NONE', "
      "'NONE', 0, 0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'device', 0, 0, 'NONE', "
      "'COMMUNICATION', 0, 0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'EMERGENCY', "
      "0, 0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  std::string priority;
  // Checks
  EXPECT_TRUE(reps->GetPriority("default", &priority));
  EXPECT_EQ("NONE", priority);
  EXPECT_TRUE(reps->GetPriority("pre_DataConsent", &priority));
  EXPECT_EQ("NONE", priority);
  EXPECT_TRUE(reps->GetPriority("device", &priority));
  EXPECT_EQ("COMMUNICATION", priority);
  EXPECT_TRUE(reps->GetPriority("12345", &priority));
  EXPECT_EQ("EMERGENCY", priority);
}

namespace {
const std::string kAppStorageFolder = "storage";
}

TEST(SQLPTRepresentationTest3, Init_InitNewDataBase_ExpectResultSuccess) {
  // Arrange
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;
  SQLPTRepresentation* reps;
  reps = new SQLPTRepresentation(
      kStorageFolder, kAttemptsToOpenPolicyDb, kOpenAttemptTimeoutMs);
  // Checks
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kAppStorageFolder));
  EXPECT_EQ(::policy::SUCCESS, reps->Init(&policy_settings_));
  EXPECT_EQ(::policy::EXISTS, reps->Init(&policy_settings_));
  reps->RemoveDB();
  delete reps;
  file_system::DeleteFile("policy.sqlite");
}

TEST(SQLPTRepresentationTest3,
     Init_TryInitNotExistingDataBase_ExpectResultFail) {
  // Arrange
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kAppStorageFolder));
  SQLPTRepresentation reps(
      "/wrong_path", kAttemptsToOpenPolicyDb, kOpenAttemptTimeoutMs);
  // Check
  EXPECT_EQ(::policy::FAIL, reps.Init(&policy_settings_));
}

TEST(SQLPTRepresentationTest3,
     Close_InitNewDataBaseThenClose_ExpectResultSuccess) {
  // Arrange
  NiceMock<policy_handler_test::MockPolicySettings> policy_settings_;
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kAppStorageFolder));
  SQLPTRepresentation reps(
      kStorageFolder, kAttemptsToOpenPolicyDb, kOpenAttemptTimeoutMs);
  EXPECT_EQ(::policy::SUCCESS, reps.Init(&policy_settings_));

  EXPECT_TRUE(reps.Close());
  utils::dbms::SQLError error(utils::dbms::Error::OK);
  // Checks
  EXPECT_EQ(error.number(), (reps.db()->LastError().number()));
  reps.RemoveDB();
}

TEST_F(SQLPTRepresentationTest,
       Clear_InitNewDataBaseThenClear_ExpectResultSuccess) {
  // Arrange
  const char* query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'default', 0, 0, 'NONE', 'NONE', 0, "
      "0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'pre_DataConsent', 0, 0, 'NONE', "
      "'NONE', 0, 0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'device', 0, 0, 'NONE', "
      "'COMMUNICATION', 0, 0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'EMERGENCY', "
      "0, 0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  const char* query_insert =
      "INSERT INTO `notifications_by_priority` (`priority_value`, `value`) "
      "VALUES ('NAVIGATION', 15) , "
      "('COMMUNICATION', 6), ('EMERGENCY', 60), ('NONE', 0), ('NORMAL', 4), "
      "('VOICECOMMUNICATION', 20)";

  ASSERT_TRUE(dbms->Exec(query_insert));
  EXPECT_TRUE(reps->Clear());
  utils::dbms::SQLError error(utils::dbms::Error::OK);
  EXPECT_EQ(error.number(), (reps->db()->LastError().number()));
}

TEST_F(SQLPTRepresentationTest,
       GetInitialAppData_SetData_ExpectCorrectValuesReceived) {
  // Arrange
  const char* query_insert =
      "INSERT INTO `nickname` (`application_id`, `name`)"
      "VALUES ('1111', 'first_app') , "
      "('2222', 'second_app'), ('3333', 'third_app')";
  ASSERT_TRUE(dbms->Exec(query_insert));

  query_insert =
      "INSERT INTO `app_type` (`application_id`, `name`)"
      "VALUES ('1111', 'NAVIGATION') , "
      "('1111', 'MEDIA'), ('3333', 'COMMUNICATION')";
  ASSERT_TRUE(dbms->Exec(query_insert));
  ::policy::StringArray nicknames;
  ::policy::StringArray app_types;
  ASSERT_TRUE(reps->GetInitialAppData("1111", &nicknames, &app_types));
  EXPECT_EQ(1u, nicknames.size());
  EXPECT_TRUE(nicknames.end() !=
              std::find(nicknames.begin(), nicknames.end(), "first_app"));
  EXPECT_EQ(2u, app_types.size());
  EXPECT_TRUE(app_types.end() !=
              std::find(app_types.begin(), app_types.end(), "NAVIGATION"));
  EXPECT_TRUE(app_types.end() !=
              std::find(app_types.begin(), app_types.end(), "MEDIA"));
  nicknames.clear();
  app_types.clear();
  ASSERT_TRUE(reps->GetInitialAppData("2222", &nicknames, &app_types));
  EXPECT_EQ(1u, nicknames.size());
  EXPECT_TRUE(nicknames.end() !=
              std::find(nicknames.begin(), nicknames.end(), "second_app"));
  EXPECT_EQ(0u, app_types.size());
  nicknames.clear();
  app_types.clear();
  ASSERT_TRUE(reps->GetInitialAppData("3333", &nicknames, &app_types));
  EXPECT_EQ(1u, nicknames.size());
  EXPECT_TRUE(nicknames.end() !=
              std::find(nicknames.begin(), nicknames.end(), "third_app"));
  EXPECT_EQ(1u, app_types.size());
  EXPECT_TRUE(app_types.end() !=
              std::find(app_types.begin(), app_types.end(), "COMMUNICATION"));
}

TEST_F(
    SQLPTRepresentationTest,
    GetFunctionalGroupings_SetFunctionalGroupings_ExpectCorrectValuesReceived) {
  // Arrange
  const char* query_insert =
      "INSERT INTO `functional_group` (`id`, `user_consent_prompt`, `name`) "
      "VALUES (73072936, null, 'SendLocation'), (1533011474, null, "
      "'OnKeyboardInputOnlyGroup')";
  ASSERT_TRUE(dbms->Exec(query_insert));

  query_insert =
      "INSERT INTO `rpc` (`name`, `hmi_level_value`, `functional_group_id`) "
      "VALUES ('SendLocation', 'BACKGROUND', 73072936), ('SendLocation', "
      "'FULL', 73072936), ('SendLocation', 'LIMITED', 73072936)";
  ASSERT_TRUE(dbms->Exec(query_insert));

  query_insert =
      "INSERT INTO `rpc` (`name`, `hmi_level_value`, `functional_group_id`) "
      "VALUES ('OnKeyboardInput', 'FULL', 1533011474)";
  ASSERT_TRUE(dbms->Exec(query_insert));

  policy_table::FunctionalGroupings func_groups;
  ASSERT_TRUE(reps->GetFunctionalGroupings(func_groups));
  EXPECT_EQ(2u, func_groups.size());
  policy_table::FunctionalGroupings::iterator func_groups_it =
      func_groups.find("SendLocation");
  EXPECT_TRUE(func_groups.end() != func_groups_it);
  policy_table::Rpcs& rpcs = func_groups_it->second;
  EXPECT_EQ("", static_cast<std::string>(*rpcs.user_consent_prompt));
  policy_table::Rpc& rpc = rpcs.rpcs;
  EXPECT_EQ(1u, rpc.size());
  policy_table::Rpc::const_iterator rpc_it = rpc.find("SendLocation");
  EXPECT_TRUE(rpc.end() != rpc_it);
  const policy_table::HmiLevels& hmi_levels1 = rpc_it->second.hmi_levels;
  EXPECT_EQ(3u, hmi_levels1.size());
  EXPECT_TRUE(hmi_levels1.end() !=
              std::find(hmi_levels1.begin(),
                        hmi_levels1.end(),
                        policy_table::HmiLevel::HL_BACKGROUND));
  EXPECT_TRUE(hmi_levels1.end() !=
              std::find(hmi_levels1.begin(),
                        hmi_levels1.end(),
                        policy_table::HmiLevel::HL_LIMITED));
  EXPECT_TRUE(hmi_levels1.end() != std::find(hmi_levels1.begin(),
                                             hmi_levels1.end(),
                                             policy_table::HmiLevel::HL_FULL));

  func_groups_it = func_groups.find("OnKeyboardInputOnlyGroup");
  EXPECT_TRUE(func_groups.end() != func_groups_it);
  policy_table::Rpcs& rpcs2 = func_groups_it->second;
  EXPECT_EQ("", static_cast<std::string>(*rpcs2.user_consent_prompt));
  policy_table::Rpc& rpc2 = rpcs2.rpcs;
  EXPECT_EQ(1u, rpc2.size());
  rpc_it = rpc2.find("OnKeyboardInput");
  EXPECT_TRUE(rpc2.end() != rpc_it);
  const policy_table::HmiLevels& hmi_levels2 = rpc_it->second.hmi_levels;
  EXPECT_EQ(1u, hmi_levels2.size());
  EXPECT_TRUE(hmi_levels2.end() != std::find(hmi_levels2.begin(),
                                             hmi_levels2.end(),
                                             policy_table::HmiLevel::HL_FULL));
}

TEST_F(
    SQLPTRepresentationTest,
    UpdateRequired_SetUpdateNotRequiredFlagThenCheck_ExpectUpdateNotRequired) {
  // Arrange
  EXPECT_FALSE(reps->UpdateRequired());
}

TEST_F(SQLPTRepresentationTest,
       UpdateRequired_SetUpdateRequiredFlagThenCheck_ExpectUpdateRequired) {
  // Arrange
  const char* query_insert =
      "UPDATE `module_meta` SET `flag_update_required` = 1";
  // Assert
  ASSERT_TRUE(dbms->Exec(query_insert));
  // Check
  EXPECT_TRUE(reps->UpdateRequired());
}

TEST_F(SQLPTRepresentationTest,
       SaveUpdateRequired_SaveUpdateRequired_ExpectCorrectValues) {
  // Arrange
  reps->SaveUpdateRequired(true);
  // Check
  EXPECT_TRUE(reps->UpdateRequired());
  // Act
  reps->SaveUpdateRequired(false);
  // Check
  EXPECT_FALSE(reps->UpdateRequired());
}

TEST_F(SQLPTRepresentationTest,
       IsApplicationRepresented_Check_ExpectCorrectResult) {
  // Arrange
  const char* query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'default', 0, 0, 'NONE', 'NONE', 0, "
      "0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'device', 0, 0, 'NONE', "
      "'COMMUNICATION', 0, 0, 0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'EMERGENCY', "
      "0, 0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));
  // Checks
  EXPECT_TRUE(reps->IsApplicationRepresented("default"));
  EXPECT_TRUE(reps->IsApplicationRepresented("device"));
  EXPECT_TRUE(reps->IsApplicationRepresented("12345"));
  EXPECT_FALSE(reps->IsApplicationRepresented("1234"));
}

TEST_F(SQLPTRepresentationTest,
       IsApplicationRevoked_CheckApps_ExpectCorrectResult) {
  // Arrange
  const char* query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '7777', 0, 0, 'NONE', 'NONE', 1, "
      "0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'EMERGENCY', "
      "0, 0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));
  // Checks
  EXPECT_TRUE(reps->IsApplicationRevoked("7777"));
  EXPECT_FALSE(reps->IsApplicationRevoked("12345"));
}

TEST_F(SQLPTRepresentationTest,
       CopyApplication_CopyApplication_ExpectAppCopiedSuccesfully) {
  // Arrange
  const char* query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'default', 0, 0, 'NONE', 'NONE', 0, "
      "1, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '123', 1, 0, 'FULL', "
      "'COMMUNICATION', 1, 1, 0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));
  EXPECT_FALSE(reps->IsApplicationRepresented("7777"));
  EXPECT_FALSE(reps->IsApplicationRepresented("9999"));
  // Act
  EXPECT_TRUE(reps->CopyApplication("default", "7777"));
  EXPECT_TRUE(reps->CopyApplication("123", "9999"));
  // Checks
  EXPECT_TRUE(reps->IsApplicationRepresented("7777"));
  EXPECT_TRUE(reps->IsApplicationRepresented("9999"));
  EXPECT_FALSE(reps->IsApplicationRevoked("7777"));
  EXPECT_TRUE(reps->IsApplicationRevoked("9999"));
  EXPECT_TRUE(reps->IsDefaultPolicy("7777"));
  std::string priority1;
  std::string priority2;
  EXPECT_TRUE(reps->GetPriority("default", &priority1));
  EXPECT_TRUE(reps->GetPriority("7777", &priority2));
  EXPECT_EQ(priority1, priority2);
  EXPECT_TRUE(reps->GetPriority("123", &priority1));
  EXPECT_TRUE(reps->GetPriority("9999", &priority2));
  EXPECT_EQ(priority1, priority2);
}

TEST_F(SQLPTRepresentationTest,
       IsDefaultPolicy_SetAppPreDataThenCheck_ExpectNotDefaultPolicySet) {
  // Arrange
  const char* query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'NONE', 0, "
      "0, "
      "1, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));
  // Check
  EXPECT_FALSE(reps->IsDefaultPolicy("12345"));
}

TEST_F(SQLPTRepresentationTest,
       IsDefaultPolicy_SetAppDefaultThenCheck_ExpectNotDefaultPolicySet) {
  // Arrange
  const char* query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, `default_hmi`, `priority_value`, `is_revoked`, "
      "`is_default`, `is_predata`, `memory_kb`, `heart_beat_timeout_ms`) "
      "VALUES( '1234567', 0, 0, 'NONE', 'NONE', 0, 1, 0, 64, 10) ";

  ASSERT_TRUE(dbms->Exec(query_insert_app));
  // Check
  EXPECT_TRUE(reps->IsDefaultPolicy("1234567"));
}

TEST_F(SQLPTRepresentationTest, Drop_DropExistedPT_ExpectZeroTables) {
  // Check
  const char* query_select =
      "SELECT COUNT(*) FROM `sqlite_master` WHERE `type` = 'table'";
  // In normally created PT there are more than 0 tables
  ASSERT_TRUE(dbms->Exec(query_select));
  ASSERT_GT(dbms->FetchOneInt(query_select), 0);
  // Destroy schema
  ASSERT_TRUE(reps->Drop());
  // Check PT structure destroyed and tables number is 0
  ASSERT_EQ(0, dbms->FetchOneInt(query_select));
  // Restore schema
  ASSERT_TRUE(reps->RefreshDB());
}

TEST_F(SQLPTRepresentationTest,
       SetDefaultPolicy_SetDefaultPolicyThenCheck_ExpectDefaultPolicySet) {
  // Arrange
  const char* query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'default', 0, 0, 'NONE', 'NONE', 0, "
      "0, "
      "0, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));

  query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, `default_hmi`, `priority_value`, `is_revoked`, "
      "`is_default`, `is_predata`, `memory_kb`, `heart_beat_timeout_ms`) "
      "VALUES( '1234567', 0, 0, 'NONE', 'NONE', 0, 0, 1, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));
  EXPECT_FALSE(reps->IsDefaultPolicy("1234567"));
  // Act
  ASSERT_TRUE(reps->SetDefaultPolicy("1234567"));
  // Check
  EXPECT_TRUE(reps->IsDefaultPolicy("1234567"));
}

TEST_F(SQLPTRepresentationTest,
       SetPreloaded_SetPreloaded_ExpectPTSetToPreloaded) {
  // Arrange
  const char* query_insert = "UPDATE `module_config` SET `preloaded_pt` = 0";
  ASSERT_TRUE(dbms->Exec(query_insert));
  // Check
  ASSERT_FALSE(reps->IsPTPreloaded());
  // Act
  reps->SetPreloaded(true);
  // Check
  ASSERT_TRUE(reps->IsPTPreloaded());
  // Act
  reps->SetPreloaded(false);
  // Check
  ASSERT_FALSE(reps->IsPTPreloaded());
}

TEST_F(SQLPTRepresentationTest,
       SetIsDefault_SetIsDefault_ExpectDefaultFlagSet) {
  // Arrange
  const char* query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, `default_hmi`, `priority_value`, `is_revoked`, "
      "`is_default`, `is_predata`, `memory_kb`, `heart_beat_timeout_ms`) "
      "VALUES( '1234567', 0, 0, 'NONE', 'NONE', 0, 0, 1, 64, 10) ";
  ASSERT_TRUE(dbms->Exec(query_insert_app));
  const char* query_select =
      "SELECT `is_default` FROM `application`WHERE`id`= '1234567' ";
  EXPECT_EQ(0, dbms->FetchOneInt(query_select));
  // Act
  EXPECT_TRUE(reps->SetIsDefault("1234567", true));
  // Check
  EXPECT_EQ(1, dbms->FetchOneInt(query_select));
  // Act
  EXPECT_TRUE(reps->SetIsDefault("1234567", false));
  // Check
  EXPECT_EQ(0, dbms->FetchOneInt(query_select));
}

TEST(SQLPTRepresentationTest3, RemoveDB_RemoveDB_ExpectFileDeleted) {
  // Arrange
  policy_handler_test::MockPolicySettings policy_settings_;
  SQLPTRepresentation* reps = new SQLPTRepresentation(
      kStorageFolder, kAttemptsToOpenPolicyDb, kOpenAttemptTimeoutMs);
  EXPECT_EQ(::policy::SUCCESS, reps->Init(&policy_settings_));
  EXPECT_EQ(::policy::EXISTS, reps->Init(&policy_settings_));
  std::string path = (reps->db())->get_path();
  // Act
  reps->db()->Close();
  reps->RemoveDB();
  // Check
  EXPECT_FALSE(file_system::FileExists(path));
  delete reps;
}

// TODO {AKozoriz} : Snapshot must have module meta section, but test
// generates snapshot without it.
// TODO(OHerasym) : JsonValue Clear method works wrong
TEST_F(SQLPTRepresentationTest,
       DISABLED_GenerateSnapshot_SetPolicyTable_SnapshotIsPresent) {
  // Arrange
  utils::json::JsonValue table(utils::json::ValueType::OBJECT_VALUE);
  PolicyTableUpdatePrepare(table);

  policy_table::Table update(table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);

  // Assert
  // ASSERT_TRUE(IsValid(update));
  ASSERT_TRUE(reps->Save(update));

  // Act
  utils::SharedPtr<policy_table::Table> snapshot = reps->GenerateSnapshot();
  snapshot->SetPolicyTableType(rpc::policy_table_interface_base::PT_SNAPSHOT);
  // Remove fields which must be absent in snapshot
  table["policy_table"]["consumer_friendly_messages"]["messages"].Clear();
  table["policy_table"]["app_policies"]["1234"]["default_hmi"].Clear();
  table["policy_table"]["app_policies"]["1234"]["keep_context"].Clear();
  table["policy_table"]["app_policies"]["1234"]["steal_focus"].Clear();
  table["policy_table"]["app_policies"]["default"]["default_hmi"].Clear();
  table["policy_table"]["app_policies"]["default"]["keep_context"].Clear();
  table["policy_table"]["app_policies"]["default"]["steal_focus"].Clear();
  table["policy_table"]["app_policies"]["pre_DataConsent"]["default_hmi"]
      .Clear();
  table["policy_table"]["app_policies"]["pre_DataConsent"].Clear();
  table["policy_table"]["app_policies"]["pre_DataConsent"]["steal_focus"]
      .Clear();
  table["policy_table"]["app_policies"]["device"]["default_hmi"].Clear();
  table["policy_table"]["app_policies"]["device"]["steal_focus"].Clear();
  table["policy_table"]["app_policies"]["device"]["groups"].Clear();
  table["policy_table"]["device_data"] = utils::json::ValueType::OBJECT_VALUE;
  table["policy_table"]["module_meta"] = utils::json::ValueType::OBJECT_VALUE;
  policy_table::Table expected(table);

  // Checks
  EXPECT_EQ(expected.ToJsonValue().ToJson(true),
            snapshot->ToJsonValue().ToJson(true));
}

TEST_F(SQLPTRepresentationTest, Save_SetPolicyTableThenSave_ExpectSavedToPT) {
  // Arrange
  utils::json::JsonValue table(utils::json::ValueType::OBJECT_VALUE);
  PolicyTableUpdatePrepare(table);

  policy_table::Table update(table);
  update.SetPolicyTableType(rpc::policy_table_interface_base::PT_UPDATE);
  // Checks PT before Save
  policy_table::FunctionalGroupings func_groups;
  ASSERT_TRUE(reps->GetFunctionalGroupings(func_groups));
  // Check functional groupings section
  EXPECT_EQ(0u, func_groups.size());

  policy_table::ApplicationPoliciesSection policies;
  GatherApplicationPoliciesSection(&policies);
  // Check ApplicationPoliciesSection
  EXPECT_EQ(0u, policies.apps.size());
  EXPECT_EQ(policy_table::Priority::P_EMERGENCY, policies.device.priority);

  policy_table::ModuleConfig config;
  GatherModuleConfig(&config);
  // Check Module config section
  EXPECT_TRUE(*config.preloaded_pt);
  EXPECT_EQ(0, config.exchange_after_x_ignition_cycles);
  EXPECT_EQ(0, config.exchange_after_x_kilometers);
  EXPECT_EQ(0, config.exchange_after_x_days);
  EXPECT_EQ(0, config.timeout_after_x_seconds);
  EXPECT_EQ("", static_cast<std::string>(*config.vehicle_make));
  EXPECT_EQ("", static_cast<std::string>(*config.vehicle_model));
  EXPECT_EQ("", static_cast<std::string>(*config.vehicle_year));
  EXPECT_EQ("", static_cast<std::string>(*config.preloaded_date));
  EXPECT_EQ("", static_cast<std::string>(*config.certificate));
  EXPECT_EQ(0u, config.seconds_between_retries.size());
  EXPECT_EQ(0u, config.endpoints.size());
  EXPECT_EQ(0u, config.notifications_per_minute_by_priority.size());

  policy_table::ConsumerFriendlyMessages messages;
  GatherConsumerFriendlyMessages(&messages);
  EXPECT_EQ("0", static_cast<std::string>(messages.version));
  policy_table::DeviceData devices;
  GatherDeviceData(&devices);
  EXPECT_EQ(0u, devices.size());
  policy_table::UsageAndErrorCounts counts;
  GatherUsageAndErrorCounts(&counts);
  EXPECT_EQ(0u, counts.app_level->size());
  ASSERT_TRUE(IsValid(update));
  // Act
  ASSERT_TRUE(reps->Save(update));

  // Check Functional Groupings
  ASSERT_TRUE(reps->GetFunctionalGroupings(func_groups));
  // Checks
  EXPECT_EQ(1u, func_groups.size());
  policy_table::FunctionalGroupings::iterator func_groups_iter =
      func_groups.find("default");
  ASSERT_TRUE(func_groups.end() != func_groups_iter);
  policy_table::Rpcs& rpcs = func_groups_iter->second;
  EXPECT_EQ("", static_cast<std::string>(*rpcs.user_consent_prompt));
  policy_table::Rpc& rpc = rpcs.rpcs;
  EXPECT_EQ(1u, rpc.size());
  policy_table::Rpc::const_iterator rpc_iter = rpc.find("Update");
  EXPECT_TRUE(rpc.end() != rpc_iter);
  const policy_table::HmiLevels& hmi_levels = rpc_iter->second.hmi_levels;
  EXPECT_EQ(1u, hmi_levels.size());
  EXPECT_TRUE(hmi_levels.end() != std::find(hmi_levels.begin(),
                                            hmi_levels.end(),
                                            policy_table::HmiLevel::HL_FULL));

  const ::policy_table::Parameters& parameters = *(rpc_iter->second.parameters);
  EXPECT_EQ(1u, parameters.size());
  EXPECT_TRUE(parameters.end() != std::find(parameters.begin(),
                                            parameters.end(),
                                            policy_table::Parameter::P_SPEED));
  // Check Application Policies Section
  GatherApplicationPoliciesSection(&policies);
  const uint32_t apps_size = 3u;

  rpc::String<1ul, 255ul> str("default");
  policy_table::Strings groups;
  groups.push_back(str);
  CheckAppPoliciesSection(policies,
                          apps_size,
                          policy_table::Priority::P_EMERGENCY,
                          "1234",
                          150u,
                          200u,
                          groups);
  CheckAppPoliciesSection(policies,
                          apps_size,
                          policy_table::Priority::P_EMERGENCY,
                          "default",
                          50u,
                          100u,
                          groups);
  CheckAppPoliciesSection(policies,
                          apps_size,
                          policy_table::Priority::P_EMERGENCY,
                          "pre_DataConsent",
                          40u,
                          90u,
                          groups);
  CheckAppPoliciesSection(policies,
                          apps_size,
                          policy_table::Priority::P_EMERGENCY,
                          "device",
                          0u,
                          0u,
                          groups);

  CheckAppGroups("1234", groups);
  CheckAppGroups("default", groups);
  CheckAppGroups("pre_DataConsent", groups);

  GatherModuleConfig(&config);
  // Check Module Config section
  ASSERT_TRUE(*config.preloaded_pt);
  ASSERT_EQ("", static_cast<std::string>(*config.certificate));
  ASSERT_EQ("", static_cast<std::string>(*config.preloaded_date));
  ASSERT_EQ("2016", static_cast<std::string>(*config.vehicle_year));
  ASSERT_EQ("Mustang", static_cast<std::string>(*config.vehicle_model));
  ASSERT_EQ("Ford", static_cast<std::string>(*config.vehicle_make));
  ASSERT_EQ(10, config.exchange_after_x_ignition_cycles);
  ASSERT_EQ(100, config.exchange_after_x_kilometers);
  ASSERT_EQ(5, config.exchange_after_x_days);
  ASSERT_EQ(500, config.timeout_after_x_seconds);
  ASSERT_EQ(3u, config.seconds_between_retries.size());
  ASSERT_EQ(10, config.seconds_between_retries[0]);
  ASSERT_EQ(20, config.seconds_between_retries[1]);
  ASSERT_EQ(30, config.seconds_between_retries[2]);
  ASSERT_EQ(6u, config.notifications_per_minute_by_priority.size());
  ASSERT_EQ(1, config.notifications_per_minute_by_priority["emergency"]);
  ASSERT_EQ(2, config.notifications_per_minute_by_priority["navigation"]);
  ASSERT_EQ(3, config.notifications_per_minute_by_priority["VOICECOMM"]);
  ASSERT_EQ(4, config.notifications_per_minute_by_priority["communication"]);
  ASSERT_EQ(5, config.notifications_per_minute_by_priority["normal"]);
  ASSERT_EQ(6, config.notifications_per_minute_by_priority["none"]);
  EXPECT_EQ(1u, config.endpoints.size());
  policy_table::ServiceEndpoints& service_endpoints = config.endpoints;
  EXPECT_EQ("0x00", service_endpoints.begin()->first);
  policy_table::URLList& url_list = service_endpoints.begin()->second;
  EXPECT_EQ("default", url_list.begin()->first);
  policy_table::URL& url = url_list.begin()->second;
  EXPECT_EQ("http://ford.com/cloud/default", static_cast<std::string>(url[0]));
  GatherConsumerFriendlyMessages(&messages);
  EXPECT_EQ("1.2", static_cast<std::string>(messages.version));
}

}  // namespace policy_test
}  // namespace components
}  // namespace test

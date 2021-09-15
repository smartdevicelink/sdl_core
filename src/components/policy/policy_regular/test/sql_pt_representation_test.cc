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

#include "policy/sql_pt_representation.h"

#include <stdio.h>
#include <sys/stat.h>

#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

#include "gtest/gtest.h"

#include "json/reader.h"
#include "json/writer.h"
#include "policy/driver_dbms.h"
#include "policy/mock_policy_settings.h"
#include "policy/policy_table/enums.h"
#include "policy/policy_table/types.h"
#include "policy/policy_types.h"
#include "rpc_base/rpc_base.h"
#include "sqlite_wrapper/sql_query.h"
#include "utils/file_system.h"
#include "utils/sqlite_wrapper/sql_database.h"

namespace policy_table = rpc::policy_table_interface_base;
using policy::CheckPermissionResult;
using policy::EndpointUrls;
using policy::SQLPTRepresentation;
using policy::UserFriendlyMessage;
using policy::VehicleInfo;

using testing::Mock;
using testing::NiceMock;
using testing::Return;
using testing::ReturnRef;

namespace test {
namespace components {
namespace policy_test {

using policy_handler_test::MockPolicySettings;

namespace {
const std::string kSdlPreloadedPtJson = "json/sdl_preloaded_pt.json";
const std::string kSoftwareVersion = "4.1.3.B_EB355B";
const std::string kHardwareVersion = "1.1.1.0";
}  // namespace

policy_table::Table LoadPreloadedPT(const std::string& filename) {
  std::ifstream ifile(filename);
  EXPECT_TRUE(ifile.good());
  Json::CharReaderBuilder reader_builder;
  Json::Value root(Json::objectValue);
  Json::parseFromStream(reader_builder, ifile, &root, nullptr);
  root["policy_table"]["module_config"].removeMember("preloaded_pt");
  ifile.close();
  policy_table::Table table(&root);
  return table;
}

Json::Value GetDefaultSnapshotModuleMeta(policy_table::Table& policy_table) {
  auto json_table = policy_table.ToJsonValue();

  Json::Value default_module_meta = json_table["policy_table"]["module_meta"];
  default_module_meta["pt_exchanged_at_odometer_x"] = Json::Value(0);
  default_module_meta["pt_exchanged_x_days_after_epoch"] = Json::Value(0);
  default_module_meta["ignition_cycles_since_last_exchange"] = Json::Value(0);

  return default_module_meta;
}

class SQLPTRepresentationTest : protected SQLPTRepresentation,
                                public ::testing::Test {
 protected:
  std::shared_ptr<utils::dbms::SQLQuery> query_wrapper_;
  std::shared_ptr<SQLPTRepresentation> reps;
  policy_handler_test::MockPolicySettings policy_settings_;

  void SetUp() OVERRIDE {
    reps = std::make_shared<SQLPTRepresentation>(true);
    ASSERT_TRUE(reps != NULL);
    ASSERT_EQ(policy::SUCCESS, reps->Init(&policy_settings_));

    query_wrapper_ = std::make_shared<utils::dbms::SQLQuery>(reps->db());
    ASSERT_TRUE(query_wrapper_ != NULL);
  }

  void TearDown() OVERRIDE {
    EXPECT_TRUE(reps->Drop());
    EXPECT_TRUE(reps->Close());
  }

  virtual utils::dbms::SQLDatabase* db() const {
    return reps->db();
  }

  void GatherModuleMeta(policy_table::ModuleMeta* meta) const {
    ::SQLPTRepresentation::GatherModuleMeta(meta);
  }

  void GatherModuleConfig(policy_table::ModuleConfig* config) const {
    ::SQLPTRepresentation::GatherModuleConfig(config);
  }

  bool GatherUsageAndErrorCounts(
      policy_table::UsageAndErrorCounts* counts) const {
    return ::SQLPTRepresentation::GatherUsageAndErrorCounts(counts);
  }

  bool GatherApplicationPoliciesSection(
      policy_table::ApplicationPoliciesSection* policies) const {
    return ::SQLPTRepresentation::GatherApplicationPoliciesSection(policies);
  }

  bool GatherAppServiceParameters(
      const std::string& app_id,
      policy_table::AppServiceParameters* policies) const {
    return ::SQLPTRepresentation::GatherAppServiceParameters(app_id, policies);
  }

  virtual void GatherDeviceData(policy_table::DeviceData* data) const {
    ::SQLPTRepresentation::GatherDeviceData(data);
  }

  virtual bool GatherConsumerFriendlyMessages(
      policy_table::ConsumerFriendlyMessages* messages) const {
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

  int FetchOneInt(const std::string& query) {
    query_wrapper_->Prepare(query);
    query_wrapper_->Exec();
    const int ret = query_wrapper_->GetInteger(0);
    query_wrapper_->Finalize();
    return ret;
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

class SQLPTRepresentationTest2 : public ::testing::Test {
 protected:
  SQLPTRepresentationTest2()
      : kAppStorageFolder("storage123")
      , kOpenAttemptTimeoutMs(70u)
      , kAttemptsToOpenPolicyDB(2u) {}

  void SetUp() OVERRIDE {
    file_system::CreateDirectory(kAppStorageFolder);
    chmod(kAppStorageFolder.c_str(), 00000);
    ON_CALL(policy_settings_, app_storage_folder())
        .WillByDefault(ReturnRef(kAppStorageFolder));
    ON_CALL(policy_settings_, open_attempt_timeout_ms())
        .WillByDefault(Return(kOpenAttemptTimeoutMs));
    ON_CALL(policy_settings_, attempts_to_open_policy_db())
        .WillByDefault(Return(kAttemptsToOpenPolicyDB));
    reps = new SQLPTRepresentation;
  }

  void TearDown() OVERRIDE {
    ASSERT_TRUE(file_system::RemoveDirectory(kAppStorageFolder, true));
    delete reps;
  }

  SQLPTRepresentation* reps;
  NiceMock<MockPolicySettings> policy_settings_;
  const std::string kAppStorageFolder;
  const uint16_t kOpenAttemptTimeoutMs;
  const uint16_t kAttemptsToOpenPolicyDB;
};

class SQLPTRepresentationTest3 : public ::testing::Test {
 protected:
  SQLPTRepresentationTest3() : kAppStorageFolder("storage") {}

  void SetUp() OVERRIDE {
    file_system::CreateDirectory(kAppStorageFolder);
    reps = std::make_shared<SQLPTRepresentation>();
  }

  void TearDown() OVERRIDE {
    file_system::RemoveDirectory(kAppStorageFolder, true);
    reps.reset();
  }

  std::shared_ptr<SQLPTRepresentation> reps;
  NiceMock<MockPolicySettings> policy_settings_;
  const std::string kAppStorageFolder;
};

TEST_F(SQLPTRepresentationTest, VehicleDataItem_Store_Item) {
  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = "rpm";
  rpm.type = "Integer";
  rpm.key = "OEM_REF_RPM";
  rpm.mandatory = false;
  *rpm.array = false;
  rpm.params->mark_initialized();
  ASSERT_FALSE(reps->VehicleDataItemExists(rpm));
  ASSERT_TRUE(reps->InsertVehicleDataItem(rpm));
  ASSERT_TRUE(reps->VehicleDataItemExists(rpm));

  auto rpm_retrieved = reps->GetVehicleDataItem(rpm.name, rpm.key);
  ASSERT_EQ(rpm.ToJsonValue(), rpm_retrieved.begin()->ToJsonValue());
}

TEST_F(SQLPTRepresentationTest, VehicleDataItem_Store_Complete_Item) {
  policy_table::VehicleDataItem message;
  message.mark_initialized();
  message.name = "messsageName";
  message.type = "String";
  message.key = "OEM_REF_MSG";
  message.mandatory = false;
  *message.array = false;
  message.params->mark_initialized();
  *message.since = "1.0";
  *message.until = "5.0";
  *message.removed = false;
  *message.deprecated = false;
  *message.defvalue = "0";
  *message.minvalue = 0;
  *message.maxvalue = 255;
  *message.minsize = 0;
  *message.maxsize = 255;
  *message.minlength = 0;
  *message.maxlength = 255;
  ASSERT_TRUE(reps->InsertVehicleDataItem(message));

  auto message_retrieved = reps->GetVehicleDataItem(message.name, message.key);
  ASSERT_EQ(message.ToJsonValue(), message_retrieved.begin()->ToJsonValue());
}

TEST_F(SQLPTRepresentationTest, VehicleDataItem_Store_Struct) {
  policy_table::VehicleDataItem alss;
  alss.mark_initialized();
  alss.name = "ambientLightSensorStatus";
  alss.type = "AmbientLightStatus";
  alss.key = "OEM_REF_AMB_LIGHT";
  alss.mandatory = false;
  alss.params->mark_initialized();
  policy_table::VehicleDataItem lss;
  lss.mark_initialized();
  lss.name = "LightSensorStatus";
  lss.type = "Struct";
  lss.key = "OEM_REF_SEN_LIGHT";
  lss.mandatory = false;
  lss.params->mark_initialized();
  lss.params->push_back(alss);

  policy_table::VehicleDataItem hbo;
  hbo.mark_initialized();
  hbo.name = "highBeamsOn";
  hbo.type = "Boolean";
  hbo.key = "OEM_REF_HIGH_BEAM";
  hbo.mandatory = true;
  hbo.params->mark_initialized();
  policy_table::VehicleDataItem lbo;
  lbo.mark_initialized();
  lbo.name = "lowBeamsOn";
  lbo.type = "Boolean";
  lbo.key = "OEM_REF_LOW_BEAM";
  lbo.mandatory = false;
  lbo.params->mark_initialized();
  policy_table::VehicleDataItem hls;
  hls.mark_initialized();
  hls.name = "headLampStatus";
  hls.type = "Struct";
  hls.key = "OEM_REF_HLSTATUS";
  hls.mandatory = false;
  hls.params->mark_initialized();
  hls.params->push_back(lss);
  hls.params->push_back(lbo);
  hls.params->push_back(hbo);
  ASSERT_TRUE(reps->InsertVehicleDataItem(alss));
  ASSERT_TRUE(reps->InsertVehicleDataItem(hls));

  auto hls_retrieved = reps->GetVehicleDataItem(hls.name, hls.key);
  ASSERT_EQ(hls.ToJsonValue(), hls_retrieved.begin()->ToJsonValue());
}

TEST_F(SQLPTRepresentationTest, VehicleDataItem_Select_NonParameterizedVDI) {
  policy_table::VehicleDataItems non_parameterized_vdis;
  policy_table::VehicleDataItem rpm;
  rpm.mark_initialized();
  rpm.name = "rpm";
  rpm.type = "Integer";
  rpm.key = "OEM_REF_RPM";
  rpm.mandatory = false;
  *rpm.array = false;
  rpm.params->mark_initialized();
  ASSERT_TRUE(reps->InsertVehicleDataItem(rpm));
  policy_table::VehicleDataItem message;
  message.mark_initialized();
  message.name = "rpm";
  message.type = "String";
  message.key = "OEM_REF_MSG";
  message.mandatory = false;
  *message.array = false;
  message.params->mark_initialized();
  ASSERT_TRUE(reps->InsertVehicleDataItem(message));
  non_parameterized_vdis.push_back(rpm);
  non_parameterized_vdis.push_back(message);

  policy_table::VehicleDataItems parameterized_vdis;
  policy_table::VehicleDataItem alss;
  alss.mark_initialized();
  alss.name = "ambientLightSensorStatus";
  alss.type = "AmbientLightStatus";
  alss.key = "OEM_REF_AMB_LIGHT";
  alss.mandatory = false;
  alss.params->mark_initialized();
  policy_table::VehicleDataItem lss;
  lss.mark_initialized();
  lss.name = "LightSensorStatus";
  lss.type = "Struct";
  lss.key = "OEM_REF_SEN_LIGHT";
  lss.mandatory = false;
  lss.params->mark_initialized();
  lss.params->push_back(alss);
  ASSERT_TRUE(reps->InsertVehicleDataItem(lss));
  parameterized_vdis.push_back(lss);

  auto non_param_vdi_retrieved = reps->SelectPrimitiveVehicleDataItems();

  ASSERT_EQ(non_parameterized_vdis.ToJsonValue(),
            non_param_vdi_retrieved.ToJsonValue());

  auto param_vdi_retrieved = reps->SelectCompositeVehicleDataItems();
  ASSERT_EQ(parameterized_vdis.ToJsonValue(),
            param_vdi_retrieved.ToJsonValue());
}

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
  const std::string query_select =
      "SELECT COUNT(*) FROM sqlite_master WHERE `type` = 'table'";
  // In normally created PT there are more than 0 tables
  ASSERT_GT(FetchOneInt(query_select), 0);
  ASSERT_TRUE(reps->Drop());

  ASSERT_EQ(0, FetchOneInt(query_select));
  ASSERT_TRUE(reps->RefreshDB());
  // Check PT structure destroyed and tables number is 0

  // There are 38 tables in the database, now.
  const int32_t total_tables_number = 38;
  ASSERT_EQ(total_tables_number, FetchOneInt(query_select));

  const std::string query_select_count_of_iap_buffer_full =
      "SELECT `count_of_iap_buffer_full` FROM `usage_and_error_count`";
  ASSERT_EQ(0, FetchOneInt(query_select_count_of_iap_buffer_full));

  const std::string query_select_count_sync_out_of_memory =
      "SELECT `count_sync_out_of_memory` FROM `usage_and_error_count`";
  ASSERT_EQ(0, FetchOneInt(query_select_count_sync_out_of_memory));

  const std::string query_select_count_of_sync_reboots =
      "SELECT `count_of_sync_reboots` FROM `usage_and_error_count`";
  ASSERT_EQ(0, FetchOneInt(query_select_count_of_sync_reboots));

  const std::string query_select_pt_exchanged_at_odometer_x =
      "SELECT `pt_exchanged_at_odometer_x` FROM `module_meta`";
  ASSERT_EQ(0, FetchOneInt(query_select_pt_exchanged_at_odometer_x));

  const std::string query_select_pt_exchanged_x_days_after_epoch =
      "SELECT `pt_exchanged_x_days_after_epoch` FROM `module_meta`";
  ASSERT_EQ(0, FetchOneInt(query_select_pt_exchanged_x_days_after_epoch));

  const std::string query_select_flag_update_required =
      "SELECT `flag_update_required` FROM `module_meta`";
  ASSERT_EQ(0, FetchOneInt(query_select_flag_update_required));

  const std::string query_select_ignition_cycles_since_last_exchange =
      "SELECT `ignition_cycles_since_last_exchange` FROM `module_meta`";
  ASSERT_EQ(0, FetchOneInt(query_select_ignition_cycles_since_last_exchange));

  const std::string query_select_preloaded_pt =
      "SELECT `preloaded_pt` FROM `module_config`";
  ASSERT_EQ(1, FetchOneInt(query_select_preloaded_pt));

  const std::string query_select_is_first_run =
      "SELECT `is_first_run` FROM `module_config`";
  ASSERT_EQ(0, FetchOneInt(query_select_is_first_run));

  const std::string query_select_exchange_after_x_ignition_cycles =
      "SELECT `exchange_after_x_ignition_cycles` FROM `module_config`";
  ASSERT_EQ(0, FetchOneInt(query_select_exchange_after_x_ignition_cycles));

  const std::string query_select_exchange_after_x_kilometers =
      "SELECT `exchange_after_x_kilometers` FROM `module_config`";
  ASSERT_EQ(0, FetchOneInt(query_select_exchange_after_x_kilometers));

  const std::string query_select_exchange_after_x_days =
      "SELECT `exchange_after_x_days` FROM `module_config`";
  ASSERT_EQ(0, FetchOneInt(query_select_exchange_after_x_days));

  const std::string query_select_timeout_after_x_seconds =
      "SELECT `timeout_after_x_seconds` FROM `module_config`";
  ASSERT_EQ(0, FetchOneInt(query_select_timeout_after_x_seconds));

  const std::string query_select_priorities =
      "SELECT COUNT(`value`) FROM `priority`";
  ASSERT_EQ(6, FetchOneInt(query_select_priorities));

  const std::string query_select_hmi_levels =
      "SELECT COUNT(`value`) FROM `hmi_level`";
  ASSERT_EQ(4, FetchOneInt(query_select_hmi_levels));

  const std::string query_select_version = "SELECT `number` FROM `version`";
  ASSERT_EQ(0, FetchOneInt(query_select_version));
}

TEST_F(
    SQLPTRepresentationTest,
    CheckPermissionsAllowed_SetValuesInAppGroupRpcFunctionalGroup_GetEqualParamsInCheckPermissionResult) {
  // Arrange
  const std::string query =
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
  ASSERT_TRUE(query_wrapper_->Exec(query));

  // Act
  CheckPermissionResult ret;
  reps->CheckPermissions("12345", "FULL", "Update", ret);

  // Assert
  EXPECT_TRUE(ret.hmi_level_permitted == ::policy::kRpcAllowed);
  ASSERT_EQ(2u, ret.list_of_allowed_params.size());
  // TODO (AKutsan) Policy update
  //  EXPECT_EQ("gps", ret.list_of_allowed_params[0]);
  //  EXPECT_EQ("speed", ret.list_of_allowed_params[1]);
}

TEST_F(
    SQLPTRepresentationTest,
    CheckPermissionsAllowedWithoutParameters_SetLimitedPermissions_ExpectEmptyListOfAllowedParams) {
  // Arrange
  const std::string query =
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
  ASSERT_TRUE(query_wrapper_->Exec(query));

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
  const std::string query = "DELETE FROM `app_group`";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query));

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
  const std::string query = "UPDATE `module_config` SET `preloaded_pt` = 1";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query));
  EXPECT_TRUE(reps->IsPTPreloaded());
}

TEST_F(SQLPTRepresentationTest,
       GetUpdateUrls_DeleteAndInsertEndpoints_ExpectUpdateUrls) {
  // Arrange
  const std::string query_delete = "DELETE FROM `endpoint`; ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_delete));

  // Act
  EndpointUrls ret = reps->GetUpdateUrls(7);

  // Assert
  EXPECT_TRUE(ret.empty());

  // Act
  const std::string query_insert =
      "INSERT INTO `endpoint` (`application_id`, `url`, `service`) "
      "  VALUES ('12345', 'http://ford.com/cloud/1', 7);"
      "INSERT INTO `endpoint` (`application_id`, `url`, `service`) "
      "  VALUES ('12345', 'http://ford.com/cloud/2', 7);";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert));

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
  const std::string query_zeros =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 0; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 0";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_zeros));
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());

  // Act
  reps->IncrementIgnitionCycles();

  // Assert
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       IgnitionCyclesBeforeExchange_WithParametersOfQueryAreLessLimit) {
  // Arrange
  const std::string query_less_limit =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 5; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 10";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_less_limit));
  EXPECT_EQ(5, reps->IgnitionCyclesBeforeExchange());

  // Act
  reps->IncrementIgnitionCycles();

  // Assert
  EXPECT_EQ(4, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       IgnitionCyclesBeforeExchange_WithLimitCountOfParametersOfQuery) {
  // Arrange
  const std::string query_limit =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 9; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 10";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_limit));
  EXPECT_EQ(1, reps->IgnitionCyclesBeforeExchange());

  // Act
  reps->IncrementIgnitionCycles();

  // Assert
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       IgnitionCyclesBeforeExchange_WithMoreLimitCountOfParametersOfQuery) {
  // Arrange
  const std::string query_more_limit =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 12; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 10";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_more_limit));

  // Chceck
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       IgnitionCyclesBeforeExchange_WithNegativeLimitOfParametersOfQuery) {
  // Arrange
  const std::string query_negative_limit =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = 3; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = -1";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_negative_limit));

  // Check
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(
    SQLPTRepresentationTest,
    IgnitionCyclesBeforeExchange_WithNegativeLimitOfCurrentParameterOfQuery) {
  // Arrange
  const std::string query_negative_current =
      "UPDATE `module_meta` SET "
      "  `ignition_cycles_since_last_exchange` = -1; "
      "  UPDATE `module_config` SET `exchange_after_x_ignition_cycles` = 2";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_negative_current));

  // Check
  EXPECT_EQ(0, reps->IgnitionCyclesBeforeExchange());
}

TEST_F(SQLPTRepresentationTest,
       KilometersBeforeExchange_WithParametersOfQueryEqualZero) {
  // Arrange
  const std::string query_zeros =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_at_odometer_x` = 0; "
      "  UPDATE `module_config` SET `exchange_after_x_kilometers` = 0";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_zeros));

  // Checks
  EXPECT_EQ(0, reps->KilometersBeforeExchange(0));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(-10));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest,
       KilometersBeforeExchange_QueryWithNegativeLimit) {
  // Arrange
  const std::string query_negative_limit =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_at_odometer_x` = 10; "
      "  UPDATE `module_config` SET `exchange_after_x_kilometers` = -10";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_negative_limit));

  // Checks
  EXPECT_EQ(0, reps->KilometersBeforeExchange(0));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest,
       KilometersBeforeExchange_QueryWithNegativeCurrentLimit) {
  // Arrange
  const std::string query_negative_last =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_at_odometer_x` = -10; "
      "  UPDATE `module_config` SET `exchange_after_x_kilometers` = 20";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_negative_last));

  // Checks
  EXPECT_EQ(0, reps->KilometersBeforeExchange(0));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest,
       KilometersBeforeExchange_QueryWithLimitParameters) {
  // Arrange
  const std::string query_limit =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_at_odometer_x` = 10; "
      "  UPDATE `module_config` SET `exchange_after_x_kilometers` = 100";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_limit));

  // Checks
  EXPECT_EQ(0, reps->KilometersBeforeExchange(120));
  EXPECT_EQ(60, reps->KilometersBeforeExchange(50));
  EXPECT_EQ(0, reps->KilometersBeforeExchange(5));
}

TEST_F(SQLPTRepresentationTest,
       DaysBeforeExchange_WithParametersOfQueryEqualZero) {
  // Arrange
  const std::string query_zeros =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_x_days_after_epoch` = 0; "
      "  UPDATE `module_config` SET `exchange_after_x_days` = 0";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_zeros));

  // Checks
  EXPECT_EQ(0, reps->DaysBeforeExchange(0));
  EXPECT_EQ(0, reps->DaysBeforeExchange(-10));
  EXPECT_EQ(0, reps->DaysBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest, DaysBeforeExchange_QueryWithNegativeLimit) {
  // Arrange
  const std::string query_negative_limit =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_x_days_after_epoch` = 10; "
      "  UPDATE `module_config` SET `exchange_after_x_days` = -10";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_negative_limit));

  // Checks
  EXPECT_EQ(0, reps->DaysBeforeExchange(0));
  EXPECT_EQ(0, reps->DaysBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest,
       DaysBeforeExchange_QueryWithNegativeCurrentLimit) {
  // Arrange
  const std::string query_negative_last =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_x_days_after_epoch` = -10; "
      "  UPDATE `module_config` SET `exchange_after_x_days` = 20";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_negative_last));

  // Checks
  EXPECT_EQ(0, reps->DaysBeforeExchange(0));
  EXPECT_EQ(0, reps->DaysBeforeExchange(10));
}

TEST_F(SQLPTRepresentationTest, DaysBeforeExchange_QueryWithLimitParameters) {
  // Arrange
  const std::string query_limit =
      "UPDATE `module_meta` SET "
      "  `pt_exchanged_x_days_after_epoch` = 10; "
      "  UPDATE `module_config` SET `exchange_after_x_days` = 100";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_limit));

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
  const std::string query_delete = "DELETE FROM `seconds_between_retry`; ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_delete));
  ASSERT_TRUE(reps->SecondsBetweenRetries(&seconds));
  EXPECT_EQ(0u, seconds.size());

  // Arrange
  const std::string query_insert =
      "INSERT INTO `seconds_between_retry` (`index`, `value`) "
      "  VALUES (0, 10); "
      "INSERT INTO `seconds_between_retry` (`index`, `value`) "
      "  VALUES (1, 20); ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert));
  ASSERT_TRUE(reps->SecondsBetweenRetries(&seconds));

  // Checks
  ASSERT_EQ(2u, seconds.size());
  EXPECT_EQ(10, seconds[0]);
  EXPECT_EQ(20, seconds[1]);
}

TEST_F(SQLPTRepresentationTest, TimeoutResponse_Set60Seconds_GetEqualTimeout) {
  // Arrange
  const std::string query =
      "UPDATE `module_config` SET `timeout_after_x_seconds` = 60";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query));

  // Check
  EXPECT_EQ(60000, reps->TimeoutResponse());
}

TEST_F(SQLPTRepresentationTest,
       IsPTPreloaded_SetPTPreloadedThenCheck_ExpectCorrectValue) {
  // Arrange
  const std::string query_insert =
      "UPDATE `module_config` SET `preloaded_pt` = 1";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert));

  // Check
  ASSERT_TRUE(reps->IsPTPreloaded());
}

TEST_F(
    SQLPTRepresentationTest,
    SetCountersPassedForSuccessfulUpdate_SetCounters_ExpectValueChangedInPT) {
  // Arrange
  const std::string query_select_odometer =
      "SELECT  `pt_exchanged_at_odometer_x` FROM`module_meta`";
  const std::string query_select_days_after_epoch =
      "SELECT `pt_exchanged_x_days_after_epoch` FROM`module_meta`";
  ASSERT_EQ(0, FetchOneInt(query_select_odometer));
  ASSERT_EQ(0, FetchOneInt(query_select_days_after_epoch));
  // Act
  ASSERT_TRUE(reps->SetCountersPassedForSuccessfulUpdate(100, 10000));
  ASSERT_EQ(100, FetchOneInt(query_select_odometer));
  ASSERT_EQ(10000, FetchOneInt(query_select_days_after_epoch));
}

TEST_F(
    SQLPTRepresentationTest,
    IncrementIgnitionCycles_SetIgnitionCyclesValueThenIncrement_ExpectValueIncrementedInPT) {
  // Arrange
  const std::string query_insert =
      "UPDATE `module_meta` SET `ignition_cycles_since_last_exchange` = 54";
  const std::string query_select =
      "SELECT `ignition_cycles_since_last_exchange`FROM `module_meta`";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert));

  // Act
  reps->IncrementIgnitionCycles();

  // Check
  ASSERT_EQ(55, FetchOneInt(query_select));
}

TEST_F(
    SQLPTRepresentationTest,
    ResetIgnitionCycles_SetIgnitionCyclesValueThenReset_ExpectZeroValueInPT) {
  // Arrange
  const std::string query_insert =
      "UPDATE `module_meta` SET `ignition_cycles_since_last_exchange` = 55";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert));

  const std::string query_select =
      "SELECT `ignition_cycles_since_last_exchange` FROM `module_meta`";

  // Act
  reps->ResetIgnitionCycles();

  // Check
  ASSERT_EQ(0, FetchOneInt(query_select));
}

TEST_F(SQLPTRepresentationTest,
       GetUserFriendlyMsg_SetMsg_ExpectReceivedMsgSetInParams) {
  // Arrange

  const std::string query_insert_language_code =
      "INSERT INTO `message` (`language_code`, `message_type_name`) VALUES "
      "('en-en', 'AppPermissions')";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_language_code));

  const std::string query_insert_name =
      "INSERT INTO `message_type` (`name`) VALUES ('AppPermissions')";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_name));

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
  const std::string query_insert =
      "INSERT INTO `notifications_by_priority` (`priority_value`, `value`) "
      "VALUES ('NAVIGATION', 15) , "
      "('COMMUNICATION', 6), ('EMERGENCY', 60), ('NONE', 0), ('NORMAL', 4), "
      "('VOICECOMMUNICATION', 20)";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert));
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
  const std::string query_insert_app_default =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'default', 0, 0, 'NONE', 'NONE', 0, "
      "0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_default));

  const std::string query_insert_app_predataconsent =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'pre_DataConsent', 0, 0, 'NONE', "
      "'NONE', 0, 0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_predataconsent));

  const std::string query_insert_app_device =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'device', 0, 0, 'NONE', "
      "'COMMUNICATION', 0, 0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_device));

  const std::string query_insert_app_12345 =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'EMERGENCY', "
      "0, 0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_12345));

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

TEST_F(SQLPTRepresentationTest3, Init_InitNewDataBase_ExpectResultSuccess) {
  // Arrange
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kAppStorageFolder));
  // Checks
  EXPECT_EQ(::policy::SUCCESS, reps->Init(&policy_settings_));
  EXPECT_EQ(::policy::EXISTS, reps->Init(&policy_settings_));
  reps->RemoveDB();
}

TEST_F(SQLPTRepresentationTest3,
       Init_TryInitNotExistingDataBase_ExpectResultFail) {
  const std::string not_existing_path = "/not/existing/path";
  // Arrange
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(not_existing_path));
  // Check
  EXPECT_EQ(::policy::FAIL, reps->Init(&policy_settings_));
}

TEST_F(SQLPTRepresentationTest3,
       Close_InitNewDataBaseThenClose_ExpectResultSuccess) {
  // Arrange
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kAppStorageFolder));
  EXPECT_EQ(::policy::SUCCESS, reps->Init(&policy_settings_));
  EXPECT_TRUE(reps->Close());
  utils::dbms::SQLError error(utils::dbms::Error::OK);
  // Checks
  EXPECT_EQ(error.number(), (reps->db()->LastError().number()));
  reps->RemoveDB();
}

TEST_F(SQLPTRepresentationTest,
       Clear_InitNewDataBaseThenClear_ExpectResultSuccess) {
  // Arrange
  const std::string query_insert_app_default =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'default', 0, 0, 'NONE', 'NONE', 0, "
      "0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_default));

  const std::string query_insert_app_predataconsent =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'pre_DataConsent', 0, 0, 'NONE', "
      "'NONE', 0, 0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_predataconsent));

  const std::string query_insert_app_device =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'device', 0, 0, 'NONE', "
      "'COMMUNICATION', 0, 0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_device));

  const std::string query_insert_app_12345 =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'EMERGENCY', "
      "0, 0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_12345));

  const std::string query_insert =
      "INSERT INTO `notifications_by_priority` (`priority_value`, `value`) "
      "VALUES ('NAVIGATION', 15) , "
      "('COMMUNICATION', 6), ('EMERGENCY', 60), ('NONE', 0), ('NORMAL', 4), "
      "('VOICECOMMUNICATION', 20)";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert));
  EXPECT_TRUE(reps->Clear());
  utils::dbms::SQLError error(utils::dbms::Error::OK);
  EXPECT_EQ(error.number(), (reps->db()->LastError().number()));
}

TEST_F(SQLPTRepresentationTest,
       GetInitialAppData_SetData_ExpectCorrectValuesReceived) {
  // Arrange
  const std::string query_insert_nickname =
      "INSERT INTO `nickname` (`application_id`, `name`) "
      "VALUES ('1111', 'first_app') , "
      "('2222', 'second_app'), ('3333', 'third_app')";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_nickname));

  const std::string query_insert_app_type =
      "INSERT INTO `app_type` (`application_id`, `name`)"
      "VALUES ('1111', 'NAVIGATION') , "
      "('1111', 'MEDIA'), ('3333', 'COMMUNICATION')";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_type));

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
  const std::string query_insert_functional_group =
      "INSERT INTO `functional_group` (`id`, `user_consent_prompt`, `name`) "
      "VALUES (73072936, null, 'SendLocation'), (1533011474, null, "
      "'OnKeyboardInputOnlyGroup')";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_functional_group));

  const std::string query_insert_rpc_sendlocation =
      "INSERT INTO `rpc` (`name`, `hmi_level_value`, `functional_group_id`) "
      "VALUES ('SendLocation', 'BACKGROUND', 73072936), ('SendLocation', "
      "'FULL', 73072936), ('SendLocation', 'LIMITED', 73072936)";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_rpc_sendlocation));

  const std::string query_insert_rpc_onkeyboard_input =
      "INSERT INTO `rpc` (`name`, `hmi_level_value`, `functional_group_id`) "
      "VALUES ('OnKeyboardInput', 'FULL', 1533011474)";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_rpc_onkeyboard_input));

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
  const std::string query_insert =
      "UPDATE `module_meta` SET `flag_update_required` = 1";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert));

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
  const std::string query_insert_app_default =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'default', 0, 0, 'NONE', 'NONE', 0, "
      "0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_default));

  const std::string query_insert_app_device =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'device', 0, 0, 'NONE', "
      "'COMMUNICATION', 0, 0, 0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_device));

  const std::string query_insert_app_12345 =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'EMERGENCY', "
      "0, 0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_12345));

  // Checks
  EXPECT_TRUE(reps->IsApplicationRepresented("default"));
  EXPECT_TRUE(reps->IsApplicationRepresented("device"));
  EXPECT_TRUE(reps->IsApplicationRepresented("12345"));
  EXPECT_FALSE(reps->IsApplicationRepresented("1234"));
}

TEST_F(SQLPTRepresentationTest,
       IsApplicationRevoked_CheckApps_ExpectCorrectResult) {
  // Arrange
  const std::string query_insert_app_7777 =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '7777', 0, 0, 'NONE', 'NONE', 1, "
      "0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_7777));

  const std::string query_insert_app_12345 =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'EMERGENCY', "
      "0, 0, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_12345));

  // Checks
  EXPECT_TRUE(reps->IsApplicationRevoked("7777"));
  EXPECT_FALSE(reps->IsApplicationRevoked("12345"));
}

TEST_F(SQLPTRepresentationTest,
       CopyApplication_CopyApplication_ExpectAppCopiedSuccesfully) {
  // Arrange
  const std::string query_insert_app_default =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( 'default', 0, 0, 'NONE', 'NONE', 0, "
      "1, "
      "0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_default));

  const std::string query_insert_app_123 =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '123', 1, 0, 'FULL', "
      "'COMMUNICATION', 1, 1, 0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app_123));

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
  const std::string query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) VALUES( '12345', 0, 0, 'NONE', 'NONE', 0, "
      "0, "
      "1, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app));

  // Check
  EXPECT_FALSE(reps->IsDefaultPolicy("12345"));
}

TEST_F(SQLPTRepresentationTest,
       IsDefaultPolicy_SetAppDefaultThenCheck_ExpectNotDefaultPolicySet) {
  // Arrange
  const std::string query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, `default_hmi`, `priority_value`, `is_revoked`, "
      "`is_default`, `is_predata`, `memory_kb`, `heart_beat_timeout_ms`) "
      "VALUES( '1234567', 0, 0, 'NONE', 'NONE', 0, 1, 0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app));

  // Check
  EXPECT_TRUE(reps->IsDefaultPolicy("1234567"));
}

TEST_F(SQLPTRepresentationTest, Drop_DropExistedPT_ExpectZeroTables) {
  // Check
  const std::string query_select =
      "SELECT COUNT(*) FROM `sqlite_master` WHERE `type` = 'table'";
  // In normally created PT there are more than 0 tables

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_select));
  ASSERT_GT(FetchOneInt(query_select), 0);

  // Destroy schema
  ASSERT_TRUE(reps->Drop());

  // Check PT structure destroyed and tables number is 0
  ASSERT_EQ(0, FetchOneInt(query_select));

  // Restore schema
  ASSERT_TRUE(reps->RefreshDB());
}

TEST_F(SQLPTRepresentationTest,
       SetDefaultPolicy_SetDefaultPolicyThenCheck_ExpectDefaultPolicySet) {
  // Arrange
  const std::string kDefaultId = "default";
  const std::string kAppId = "app_1234567";
  const std::string kRequestType = "HTTP";
  const std::string kHmiType = "MEDIA";

  const std::string query_insert_default_app =
      "INSERT INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, "
      " `default_hmi`, `priority_value`, `is_revoked`, `is_default`, "
      "`is_predata`, `memory_kb`, "
      " `heart_beat_timeout_ms`) "
      "VALUES( '" +
      kDefaultId + "', 0, 0, 'NONE', 'NONE', 0, 0, 0, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_default_app));

  const std::string query_insert_default_app_request_types =
      "INSERT INTO `request_type` (`application_id`, `request_type`)  "
      "VALUES ('" +
      kDefaultId + "', '" + kRequestType + "')";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_default_app_request_types));

  const std::string query_insert_default_app_hmi_types =
      "INSERT INTO `app_type` (`application_id`, `name`)  "
      "VALUES ('" +
      kDefaultId + "', '" + kHmiType + "')";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_default_app_hmi_types));

  const std::string query_insert_new_app =
      "INSERT INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, `default_hmi`, `priority_value`, `is_revoked`, "
      "`is_default`, `is_predata`, `memory_kb`, `heart_beat_timeout_ms`) "
      "VALUES('" +
      kAppId + "', 0, 0, 'NONE', 'NONE', 0, 0, 1, 64, 10)";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_new_app));

  EXPECT_FALSE(reps->IsDefaultPolicy(kAppId));

  // Act
  ASSERT_TRUE(reps->SetDefaultPolicy(kAppId));

  // Check
  EXPECT_TRUE(reps->IsDefaultPolicy(kAppId));

  policy_table::RequestTypes request_types;
  GatherRequestType(kAppId, &request_types);
  ASSERT_TRUE(1 == request_types.size());
  EXPECT_EQ(policy_table::RT_HTTP, *request_types.begin());

  policy_table::AppHMITypes hmi_types;
  GatherAppType(kAppId, &hmi_types);
  ASSERT_TRUE(1 == hmi_types.size());
  EXPECT_EQ(policy_table::AHT_MEDIA, *hmi_types.begin());
}

TEST_F(SQLPTRepresentationTest,
       SetPreloaded_SetPreloaded_ExpectPTSetToPreloaded) {
  // Arrange
  const std::string query_insert =
      "UPDATE `module_config` SET `preloaded_pt` = 0";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert));

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
  const std::string query_insert_app =
      "INSERT OR IGNORE INTO `application`(`id`, `keep_context`, "
      "`steal_focus`, `default_hmi`, `priority_value`, `is_revoked`, "
      "`is_default`, `is_predata`, `memory_kb`, `heart_beat_timeout_ms`) "
      "VALUES( '1234567', 0, 0, 'NONE', 'NONE', 0, 0, 1, 64, 10) ";

  // Assert
  ASSERT_TRUE(query_wrapper_->Exec(query_insert_app));

  const std::string query_select =
      "SELECT `is_default` FROM `application`WHERE`id`= '1234567' ";
  EXPECT_EQ(0, FetchOneInt(query_select));

  // Act
  EXPECT_TRUE(reps->SetIsDefault("1234567", true));

  // Check
  EXPECT_EQ(1, FetchOneInt(query_select));

  // Act
  EXPECT_TRUE(reps->SetIsDefault("1234567", false));

  // Check
  EXPECT_EQ(0, FetchOneInt(query_select));
}

TEST_F(SQLPTRepresentationTest3, RemoveDB_RemoveDB_ExpectFileDeleted) {
  // Arrange
  ON_CALL(policy_settings_, app_storage_folder())
      .WillByDefault(ReturnRef(kAppStorageFolder));
  EXPECT_EQ(::policy::SUCCESS, reps->Init(&policy_settings_));
  EXPECT_EQ(::policy::EXISTS, reps->Init(&policy_settings_));
  std::string path = (reps->db())->get_path();
  // Act
  reps->RemoveDB();
  // Check
  EXPECT_FALSE(file_system::FileExists(path));
}

TEST_F(SQLPTRepresentationTest,
       GenerateSnapshot_DefaultContentOfModuleMeta_MetaInfoPresentInSnapshot) {
  policy_table::Table update = LoadPreloadedPT(kSdlPreloadedPtJson);

  ASSERT_TRUE(IsValid(update));
  EXPECT_TRUE(reps->Save(update));

  std::shared_ptr<policy_table::Table> snapshot = reps->GenerateSnapshot();

  auto expected_module_meta = GetDefaultSnapshotModuleMeta(update);
  auto& snapshot_module_meta = snapshot->policy_table.module_meta;
  EXPECT_EQ(expected_module_meta.toStyledString(),
            snapshot_module_meta.ToJsonValue().toStyledString());
}

TEST_F(SQLPTRepresentationTest,
       GenerateSnapshot_SetMetaInfo_NoSoftwareVersionInSnapshot) {
  policy_table::Table update = LoadPreloadedPT(kSdlPreloadedPtJson);

  ASSERT_TRUE(IsValid(update));
  EXPECT_TRUE(reps->Save(update));
  EXPECT_TRUE(reps->SetMetaInfo(kSoftwareVersion));

  std::shared_ptr<policy_table::Table> snapshot = reps->GenerateSnapshot();

  auto expected_module_meta = GetDefaultSnapshotModuleMeta(update);
  auto& snapshot_module_meta = snapshot->policy_table.module_meta;
  EXPECT_EQ(expected_module_meta.toStyledString(),
            snapshot_module_meta.ToJsonValue().toStyledString());
}

TEST_F(SQLPTRepresentationTest,
       GenerateSnapshot_SetHardwareVersion_NoHardwareVersionInSnapshot) {
  policy_table::Table update = LoadPreloadedPT(kSdlPreloadedPtJson);

  ASSERT_TRUE(IsValid(update));
  EXPECT_TRUE(reps->Save(update));
  reps->SetHardwareVersion(kHardwareVersion);

  std::shared_ptr<policy_table::Table> snapshot = reps->GenerateSnapshot();

  auto expected_module_meta = GetDefaultSnapshotModuleMeta(update);
  auto& snapshot_module_meta = snapshot->policy_table.module_meta;
  EXPECT_EQ(expected_module_meta.toStyledString(),
            snapshot_module_meta.ToJsonValue().toStyledString());
}

TEST_F(SQLPTRepresentationTest,
       SetMetaInfo_SetSoftwareVersion_ValueIsSetInModuleMeta) {
  EXPECT_TRUE(reps->SetMetaInfo(kSoftwareVersion));

  utils::dbms::SQLQuery query(reps->db());
  const std::string query_select_ccpu =
      "SELECT `ccpu_version` FROM `module_meta`";

  query.Prepare(query_select_ccpu);
  query.Next();
  EXPECT_EQ(kSoftwareVersion, query.GetString(0));
}

TEST_F(SQLPTRepresentationTest, SetHardwareVersion_ValueIsSetInModuleMeta) {
  reps->SetHardwareVersion(kHardwareVersion);

  utils::dbms::SQLQuery query(reps->db());
  const std::string query_select_hardware_version =
      "SELECT `hardware_version` FROM `module_meta`";

  query.Prepare(query_select_hardware_version);
  query.Next();
  EXPECT_EQ(kHardwareVersion, query.GetString(0));
}
}  // namespace policy_test
}  // namespace components
}  // namespace test

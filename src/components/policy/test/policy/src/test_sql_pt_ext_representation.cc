/* Copyright (c) 2013, Ford Motor Company
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

#include <gtest/gtest.h>
#include <sqlite3.h>
#include <vector>
#include <algorithm>
#include "json/value.h"
#include "policy/sql_pt_ext_representation.h"
#include "policy/policy_types.h"
#include "./types.h"
#include "./enums.h"

using policy::SQLPTExtRepresentation;

namespace test {
namespace components {
namespace policy {

class SQLPTExtRepresentationTest : public ::testing::Test {
  protected:
    static sqlite3* conn;
    static SQLPTExtRepresentation* reps;
    static const std::string kFileName;

    static void SetUpTestCase() {
      reps = new SQLPTExtRepresentation;
      EXPECT_EQ(::policy::SUCCESS, reps->Init());
      EXPECT_EQ(SQLITE_OK, sqlite3_open(kFileName.c_str(), &conn));
    }

    static void TearDownTestCase() {
      EXPECT_TRUE(reps->Drop());
      EXPECT_TRUE(reps->Close());
      delete reps;
      sqlite3_close(conn);
      remove(kFileName.c_str());
    }
};

sqlite3* SQLPTExtRepresentationTest::conn = 0;
SQLPTExtRepresentation* SQLPTExtRepresentationTest::reps = 0;
const std::string SQLPTExtRepresentationTest::kFileName = "policy.sqlite";

::testing::AssertionResult IsValid(const policy_table::Table& table) {
  if (table.is_valid()) {
    return ::testing::AssertionSuccess();
  } else {
    ::rpc::ValidationReport report(" - table");
    table.ReportErrors(&report);
    return ::testing::AssertionFailure() << ::rpc::PrettyFormat(report);
  }
}

TEST_F(SQLPTExtRepresentationTest, SaveGenerateSnapshot) {
  Json::Value expect(Json::objectValue);
  expect["policy_table"] = Json::Value(Json::objectValue);

  Json::Value& policy_table = expect["policy_table"];
  policy_table["module_meta"] = Json::Value(Json::objectValue);
  policy_table["module_config"] = Json::Value(Json::objectValue);
  policy_table["usage_and_error_counts"] = Json::Value(Json::objectValue);
  policy_table["device_data"] = Json::Value(Json::objectValue);
  policy_table["functional_groupings"] = Json::Value(Json::objectValue);
  policy_table["consumer_friendly_messages"] = Json::Value(Json::objectValue);
  policy_table["app_policies"] = Json::Value(Json::objectValue);

  Json::Value& module_meta = policy_table["module_meta"];
  module_meta["ccpu_version"] = Json::Value("ccpu version");
  module_meta["language"] = Json::Value("ru");
  module_meta["wers_country_code"] = Json::Value("ru");
  module_meta["pt_exchanged_at_odometer_x"] = Json::Value(0);
  module_meta["pt_exchanged_x_days_after_epoch"] = Json::Value(0);
  module_meta["ignition_cycles_since_last_exchange"] = Json::Value(0);
  module_meta["vin"] = Json::Value("vin");

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
  module_config["vehicle_year"] = Json::Value(2014);

  Json::Value& usage_and_error_counts = policy_table["usage_and_error_counts"];
  usage_and_error_counts["count_of_iap_buffer_full"] = Json::Value(0);
  usage_and_error_counts["count_sync_out_of_memory"] = Json::Value(0);
  usage_and_error_counts["count_of_sync_reboots"] = Json::Value(0);

  Json::Value& device_data = policy_table["device_data"];
  device_data["DEVICEHASH"] = Json::Value(Json::objectValue);
  device_data["DEVICEHASH"]["hardware"] = Json::Value("hardware");
  device_data["DEVICEHASH"]["firmware_rev"] = Json::Value("firmware_rev");
  device_data["DEVICEHASH"]["os"] = Json::Value("os");
  device_data["DEVICEHASH"]["os_version"] = Json::Value("os_version");
  device_data["DEVICEHASH"]["carrier"] = Json::Value("carrier");
  device_data["DEVICEHASH"]["max_number_rfcom_ports"] = Json::Value(10);

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
  app_policies["default"]["watchdog_timer_ms"] = Json::Value(100);
  app_policies["default"]["groups"] = Json::Value(Json::arrayValue);
  app_policies["default"]["groups"][0] = Json::Value("default");
  app_policies["default"]["priority"] = Json::Value("EMERGENCY");
  app_policies["default"]["default_hmi"] = Json::Value("FULL");
  app_policies["default"]["keep_context"] = Json::Value(true);
  app_policies["default"]["steal_focus"] = Json::Value(true);
  app_policies["default"]["certificate"] = Json::Value("sign");

  policy_table::Table table(&expect);

  ASSERT_TRUE(IsValid(table));
  ASSERT_TRUE(reps->Save(table));
  ASSERT_TRUE(reps->SetMetaInfo("ccpu version", "ru", "ru"));
  const char* query_vin = "UPDATE `module_meta` SET `vin` = 'vin'; ";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_vin, NULL, NULL, NULL));
  utils::SharedPtr<policy_table::Table> snapshot = reps->GenerateSnapshot();
  EXPECT_TRUE(IsValid(*snapshot));
  EXPECT_EQ(table.ToJsonValue().toStyledString(),
            snapshot->ToJsonValue().toStyledString());
}

TEST_F(SQLPTExtRepresentationTest, CanAppKeepContext) {
  const char* query_delete = "DELETE FROM `application`; ";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_delete, NULL, NULL, NULL));
  const char* query_insert = "INSERT INTO `application` (`id`, `memory_kb`,"
                             " `watchdog_timer_ms`, `keep_context`) VALUES ('12345', 5, 10, 1)";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_insert, NULL, NULL, NULL));
  EXPECT_FALSE(reps->CanAppKeepContext("0"));
  EXPECT_TRUE(reps->CanAppKeepContext("12345"));
}

TEST_F(SQLPTExtRepresentationTest, CanAppStealFocus) {
  const char* query_delete = "DELETE FROM `application`; ";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_delete, NULL, NULL, NULL));
  const char* query_insert = "INSERT INTO `application` (`id`, `memory_kb`,"
                             " `watchdog_timer_ms`, `steal_focus`) VALUES ('12345', 5, 10, 1)";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_insert, NULL, NULL, NULL));
  EXPECT_TRUE(reps->CanAppStealFocus("12345"));
  EXPECT_FALSE(reps->CanAppStealFocus("0"));
}

TEST_F(SQLPTExtRepresentationTest, IncrementGlobalCounter) {
  const char* query_update = "UPDATE `usage_and_error_count` SET"
                             " `count_of_sync_reboots` = 0";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_update, NULL, NULL, NULL));

  reps->Increment("count_of_sync_reboots");
  reps->Increment("count_of_sync_reboots");
  reps->Increment("count_of_sync_reboots");

  const char* query_select =
    "SELECT `count_of_sync_reboots` FROM `usage_and_error_count`";
  sqlite3_stmt* statement;
  ASSERT_EQ(SQLITE_OK,
            sqlite3_prepare(conn, query_select, -1, &statement, NULL));
  ASSERT_EQ(SQLITE_ROW, sqlite3_step(statement));
  EXPECT_EQ(3, sqlite3_column_int(statement, 0));
  EXPECT_EQ(SQLITE_DONE, sqlite3_step(statement));
}

TEST_F(SQLPTExtRepresentationTest, IncrementAppCounter) {
  const char* query_delete =
    "DELETE FROM `app_level` WHERE `application_id` = '12345'";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_delete, NULL, NULL, NULL));

  reps->Increment("12345", "count_of_user_selections");
  reps->Increment("12345", "count_of_user_selections");
  reps->Increment("12345", "count_of_user_selections");

  const char* query_select =
    "SELECT `count_of_user_selections` FROM `app_level`"
    "  WHERE `application_id` = '12345'";
  sqlite3_stmt* statement;
  ASSERT_EQ(SQLITE_OK,
            sqlite3_prepare(conn, query_select, -1, &statement, NULL));
  ASSERT_EQ(SQLITE_ROW, sqlite3_step(statement));
  EXPECT_EQ(3, sqlite3_column_int(statement, 0));
  EXPECT_EQ(SQLITE_DONE, sqlite3_step(statement));
}

TEST_F(SQLPTExtRepresentationTest, SetAppInfo) {
  const char* query_delete =
    "DELETE FROM `app_level` WHERE `application_id` = '12345'";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_delete, NULL, NULL, NULL));

  reps->Set("12345", "app_registration_language_gui", "ru-ru");
  reps->Set("12345", "app_registration_language_vui", "en-en");

  const char* query_select = "SELECT `app_registration_language_gui`, "
                             " `app_registration_language_vui` FROM `app_level`"
                             "  WHERE `application_id` = '12345'";
  sqlite3_stmt* statement;
  ASSERT_EQ(SQLITE_OK,
            sqlite3_prepare(conn, query_select, -1, &statement, NULL));
  ASSERT_EQ(SQLITE_ROW, sqlite3_step(statement));

  const unsigned char* gui = sqlite3_column_text(statement, 0);
  const unsigned char* vui = sqlite3_column_text(statement, 1);
  ASSERT_TRUE(gui);
  ASSERT_TRUE(vui);
  EXPECT_EQ("ru-ru", std::string(reinterpret_cast<const char*>(gui)));
  EXPECT_EQ("en-en", std::string(reinterpret_cast<const char*>(vui)));
  EXPECT_EQ(SQLITE_DONE, sqlite3_step(statement));
}

TEST_F(SQLPTExtRepresentationTest, AddAppStopwatch) {
  const char* query_delete =
    "DELETE FROM `app_level` WHERE `application_id` = '12345'";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_delete, NULL, NULL, NULL));

  reps->Add("12345", "minutes_in_hmi_full", 10);
  reps->Add("12345", "minutes_in_hmi_full", 60);

  const char* query_select = "SELECT `minutes_in_hmi_full` FROM `app_level`"
                             "  WHERE `application_id` = '12345'";
  sqlite3_stmt* statement;
  ASSERT_EQ(SQLITE_OK,
            sqlite3_prepare(conn, query_select, -1, &statement, NULL));
  ASSERT_EQ(SQLITE_ROW, sqlite3_step(statement));
  EXPECT_EQ(70, sqlite3_column_int(statement, 0));
  EXPECT_EQ(SQLITE_DONE, sqlite3_step(statement));
}

TEST_F(SQLPTExtRepresentationTest, SetUnpairedDevice) {
  const char* query_delete = "DELETE FROM `device`";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_delete, NULL, NULL, NULL));
  const char* query_insert = "INSERT INTO `device` (`id`) VALUES('12345')";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_insert, NULL, NULL, NULL));

  ASSERT_TRUE(reps->SetUnpairedDevice("12345"));

  const char* query_select = "SELECT `id` FROM `device` WHERE `unpaired` = 1";
  sqlite3_stmt* statement;
  ASSERT_EQ(SQLITE_OK,
            sqlite3_prepare(conn, query_select, -1, &statement, NULL));
  ASSERT_EQ(SQLITE_ROW, sqlite3_step(statement));
  std::string output(reinterpret_cast<const char*>(sqlite3_column_text(statement, 0)));
  EXPECT_EQ("12345", output);
  EXPECT_EQ(SQLITE_DONE, sqlite3_step(statement));
}

TEST_F(SQLPTExtRepresentationTest, UnpairedDevicesList) {
  const char* query_delete = "DELETE FROM `device`";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_delete, NULL, NULL, NULL));
  const char* query_insert = "INSERT INTO `device` (`id`, `unpaired`)"
                             " VALUES('12345', 1)";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_insert, NULL, NULL, NULL));
  query_insert = "INSERT INTO `device` (`id`, `unpaired`) VALUES('54321', 1)";
  ASSERT_EQ(SQLITE_OK, sqlite3_exec(conn, query_insert, NULL, NULL, NULL));

  std::vector<std::string> output;
  ASSERT_TRUE(reps->UnpairedDevicesList(&output));
  ASSERT_EQ(2u, output.size());
  EXPECT_NE(output.end(), std::find(output.begin(), output.end(), "12345"));
  EXPECT_NE(output.end(), std::find(output.begin(), output.end(), "54321"));
}

}  // namespace policy
}  // namespace components
}  // namespace test

int main(int argc, char** argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}

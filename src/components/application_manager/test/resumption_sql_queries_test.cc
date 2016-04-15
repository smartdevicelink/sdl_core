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

#include <algorithm>
#include <vector>
#include <string>
#include <sstream>
#include <utility>
#include "gtest/gtest.h"

#include "sqlite_wrapper/sql_database.h"
#include "sqlite_wrapper/sql_query.h"
#include "utils/file_system.h"
#include "config_profile/profile.h"
#include "resumption_sql_queries.h"
#include "policy/sql_pt_queries.h"

namespace resumption {
namespace test {

using std::string;
using std::pair;
using utils::dbms::SQLDatabase;
using utils::dbms::SQLQuery;

enum AccessoryVRCommand { kVRCommandFromChoice = 0, kVRCommandFromCommand };

const string kDeleteData =
    "BEGIN; "
    "DELETE FROM `resumption`; "
    "DELETE FROM `image`; "
    "DELETE FROM `applicationChoiceSet`; "
    "DELETE FROM `file`; "
    "DELETE FROM `subMenu`; "
    "DELETE FROM `TTSChunk`; "
    "DELETE FROM `vrHelpItem`; "
    "DELETE FROM `tableLimitedCharacterList`; "
    "DELETE FROM `characterArray`; "
    "DELETE FROM `choice`; "
    "DELETE FROM `command`; "
    "DELETE FROM `globalProperties`; "
    "DELETE FROM `choiceArray`; "
    "DELETE FROM `vrCommandsArray`; "
    "DELETE FROM `helpTimeoutPromptArray`; "
    "DELETE FROM `vrHelpItemArray`; "
    "DELETE FROM `application`; "
    "DELETE FROM `applicationChoiceSetArray`; "
    "DELETE FROM `applicationCommandsArray`; "
    "DELETE FROM `applicationFilesArray`; "
    "DELETE FROM `applicationSubMenuArray`; "
    "DELETE FROM `applicationSubscribtionsArray`; "
    "DELETE FROM `_internal_data`; "
    "COMMIT; "
    "VACUUM;";

const std::string kJournalOff = "PRAGMA journal_mode = OFF;";

class ResumptionSqlQueriesTest : public ::testing::Test {
 public:
  typedef pair<int, string> ValToPosPair;

 protected:
  std::vector<string> db_schema;
  static const string kDatabaseName;
  static SQLDatabase* db_;
  static const string app_id1;
  static const string app_id2;
  static const string device_id;
  static const string device_id2;
  static const string test_hash;
  static const string test_hash2;
  static const string test_image;
  static const int connection_key;
  static const int grammarID;
  static const int hmiAppID;
  static const int hmiAppID2;
  static const int hmiLevel;
  static const int hmiLevel2;
  static const int ign_off_count;
  static const int ign_off_count2;
  static const int timeStamp;
  static const int timeStamp2;

  static void SetUpTestCase() {
    db_ = new SQLDatabase();
    ASSERT_TRUE(db_->Open());
    ASSERT_TRUE(db_->IsReadWrite());
    SQLQuery query(db_);
    ASSERT_TRUE(query.Exec(resumption::kCreateSchema));
    SQLQuery query_logging_off(db_);
    ASSERT_TRUE(query_logging_off.Exec(kJournalOff));
  }

  static void TearDownTestCase() {
    db_->Close();
    delete db_;
    string file_to_delete = kDatabaseName + ".sqlite";
    file_system::DeleteFile(file_to_delete);
  }
  void TearDown() { DeleteTablesData();
                  }

  void CheckDeleteQuery(const string& count_query, const string& query_to_check,
                        pair<int, string> app_info, pair<int, string> dev_info,
                        const int value_before, const int value_after,
                        const int position_in_result);

  void CheckSelectQuery(const string& query_to_check, const int control_value,
                        const int position_in_result);

  void CheckSelectQuery(const string& query_to_check, const char* control_value,
                        const int position_in_result);

  void CheckSelectQuery(const string& query_to_check, const bool control_value,
                        const int position_in_result);

  void CheckSelectQuery(const string& query_to_check,
                        const string& control_value,
                        const int position_in_result);

  void CheckSelectQuery(const string& query_to_check,
                        pair<int, string> app_info, pair<int, string> dev_info,
                        const int control_value, const int position_in_result);

  void CheckSelectQuery(const string& query_to_check,
                        pair<int, string> app_info, pair<int, string> dev_info,
                        const string& control_value,
                        const int position_in_result);

  string IntToString(const int64_t key) const {
    std::stringstream ss;
    ss << key;
    return ss.str();
  }

  SQLQuery& FillGlobalPropertiesTable(
      SQLQuery& query, const string& vrHelpTitle, const string& menuTitle,
      const int language, const int keyboardLayout, const int keypressMode,
      const string& autoCompleteText, const int64_t image_key);

  SQLQuery& FillApplicationTable(SQLQuery& query, const int connection_key,
                                 const int grammarID, const string& hashID,
                                 const int hmiAppID, const int hmiLevel,
                                 const int ign_off_count, const int timeStamp,
                                 bool isMediaApplication, const string& appID,
                                 const string& deviceID,
                                 const int64_t glob_prop_key);

  SQLQuery& FillImageTable(SQLQuery& query, const int imageType,
                           const string& value);

  SQLQuery& FillTableLimitedCharacterListTable(
      SQLQuery& query, const string& limitedCharacterList);

  SQLQuery& FillCharacterArrayTable(SQLQuery& query,
                                    const int64_t glob_prop_key,
                                    const int64_t lim_char_list_key);

  SQLQuery& FillTTSChunkTable(SQLQuery& query, const int type,
                              const string& text);

  SQLQuery& FillHelpTimeoutPromptArrayTable(SQLQuery& query,
                                            const int64_t glob_prop_key,
                                            const int64_t tts_chunk_key,
                                            const int idhelpPrompt);

  SQLQuery& FillFileTable(SQLQuery& query, const int fileType,
                          bool is_download_complete, bool persistentFile,
                          const string& syncFileName);

  SQLQuery& FillApplicationFilesArrayTable(SQLQuery& query,
                                           const int64_t app_key,
                                           const int64_t file_key);

  SQLQuery& FillSubMenuTable(SQLQuery& query, const int menuID,
                             const string& menuName, const int position);

  SQLQuery& FillApplicationSubMenuArrayTable(SQLQuery& query, const int app_key,
                                             const int sub_menu_key);

  SQLQuery& FillAppSubscriptionsArrayTable(SQLQuery& query,
                                           const int vehicleValue,
                                           const int ButtonNameValue,
                                           const int app_key);

  SQLQuery& FillCommandTable(SQLQuery& query, const int cmdID,
                             const string& menuName, const int parentID,
                             const int position, const int64_t image_key);

  SQLQuery& FillApplicationCommandsArrayTable(SQLQuery& query,
                                              const int64_t application_key,
                                              const int64_t command_key);

  SQLQuery& FillChoiceTable(SQLQuery& query, const int choiceID,
                            const string& menuName, const string& secondaryText,
                            const string& tertiaryText,
                            const int64_t image_key);

  SQLQuery& FillChoiceArrayTable(SQLQuery& query,
                                 const int64_t app_choice_set_key,
                                 const int64_t choice_key);

  SQLQuery& FillApplicationChoiceSetTable(SQLQuery& query, const int grammarID,
                                          const int interactionChoiceSetID);

  SQLQuery& FillApplicationChoiceSetArrayTable(SQLQuery& query,
                                               const int64_t app_choice_set_key,
                                               const int64_t app_key);

  SQLQuery& FillVRCommandsArrayTable(SQLQuery& query, const string& vrCommand,
                                     AccessoryVRCommand value,
                                     const int64_t foreing_key);

  SQLQuery& FillVRHelpItemTable(SQLQuery& query, const string& text,
                                const int position, const int64_t image_key);

  SQLQuery& FillVRHelpItemArrayTable(SQLQuery& query,
                                     const int64_t global_prop_key,
                                     const int64_t vr_help_item_key);

  void CreateSchema() {
    SQLQuery query(db());
    EXPECT_TRUE(query.Exec(kCreateSchema));
  }

  void DestroySchema() {
    SQLQuery query(db());
    EXPECT_TRUE(query.Exec(kDropSchema));
  }

  void DeleteTablesData() {
    SQLQuery query(db());
    EXPECT_TRUE(query.Exec(kDeleteData));
  }

  SQLDatabase* db() const { return db_; }

  void db_schema_fill() {
    db_schema.push_back("resumption");
    db_schema.push_back("image");
    db_schema.push_back("applicationChoiceSet");
    db_schema.push_back("file");
    db_schema.push_back("subMenu");
    db_schema.push_back("TTSChunk");
    db_schema.push_back("vrHelpItem");
    db_schema.push_back("subscribedForWayPoints");
    db_schema.push_back("tableLimitedCharacterList");
    db_schema.push_back("characterArray");
    db_schema.push_back("choice");
    db_schema.push_back("command");
    db_schema.push_back("globalProperties");
    db_schema.push_back("choiceArray");
    db_schema.push_back("vrCommandsArray");
    db_schema.push_back("helpTimeoutPromptArray");
    db_schema.push_back("vrHelpItemArray");
    db_schema.push_back("application");
    db_schema.push_back("applicationChoiceSetArray");
    db_schema.push_back("applicationCommandsArray");
    db_schema.push_back("applicationFilesArray");
    db_schema.push_back("applicationSubMenuArray");
    db_schema.push_back("applicationSubscribtionsArray");
    db_schema.push_back("_internal_data");
    std::sort(db_schema.begin(), db_schema.end());
  }
};

SQLDatabase* ResumptionSqlQueriesTest::db_ = NULL;
const string ResumptionSqlQueriesTest::kDatabaseName = "test_database";
const string ResumptionSqlQueriesTest::app_id1 = "baax";
const string ResumptionSqlQueriesTest::app_id2 = "caax";
const string ResumptionSqlQueriesTest::device_id = "ZZZ";
const string ResumptionSqlQueriesTest::device_id2 = "XXX";
const string ResumptionSqlQueriesTest::test_hash = "test_hash";
const string ResumptionSqlQueriesTest::test_hash2 = "test_hash2";
const string ResumptionSqlQueriesTest::test_image = "test_image";
const int ResumptionSqlQueriesTest::connection_key = 1;
const int ResumptionSqlQueriesTest::grammarID = 2;
const int ResumptionSqlQueriesTest::hmiAppID = 111;
const int ResumptionSqlQueriesTest::hmiAppID2 = 112;
const int ResumptionSqlQueriesTest::hmiLevel = 2;
const int ResumptionSqlQueriesTest::hmiLevel2 = 3;
const int ResumptionSqlQueriesTest::ign_off_count = 3;
const int ResumptionSqlQueriesTest::ign_off_count2 = 4;
const int ResumptionSqlQueriesTest::timeStamp = 2015;
const int ResumptionSqlQueriesTest::timeStamp2 = 2016;

void ResumptionSqlQueriesTest::CheckDeleteQuery(const string& count_query,
                                                const string& query_to_check,
                                                pair<int, string> app_info,
                                                pair<int, string> dev_info,
                                                const int value_before,
                                                const int value_after,
                                                const int position_in_result) {
  SQLQuery query(db());
  EXPECT_TRUE(query.Prepare(count_query));
  EXPECT_TRUE(query.Exec());
  EXPECT_EQ(value_before, query.GetInteger(position_in_result));
  EXPECT_TRUE(query.Reset());
  // Act
  SQLQuery query_to_check_request(db());
  EXPECT_TRUE(query_to_check_request.Prepare(query_to_check));
  if (!app_info.second.empty()) {
    query_to_check_request.Bind(app_info.first, app_info.second);
  }
  if (!dev_info.second.empty()) {
    query_to_check_request.Bind(dev_info.first, dev_info.second);
  }
  EXPECT_TRUE(query_to_check_request.Exec());
  // Check after action
  EXPECT_TRUE(query.Exec());
  EXPECT_EQ(value_after, query.GetInteger(position_in_result));
}

void ResumptionSqlQueriesTest::CheckSelectQuery(const string& query_to_check,
                                                pair<int, string> app_info,
                                                pair<int, string> dev_info,
                                                const int control_value,
                                                const int position_in_result) {
  SQLQuery query_to_check_request(db());
  EXPECT_TRUE(query_to_check_request.Prepare(query_to_check));
  if (!app_info.second.empty()) {
    query_to_check_request.Bind(app_info.first, app_info.second);
  }
  if (!dev_info.second.empty()) {
    query_to_check_request.Bind(dev_info.first, dev_info.second);
  }
  EXPECT_TRUE(query_to_check_request.Exec());
  EXPECT_EQ(control_value,
            query_to_check_request.GetInteger(position_in_result));
}

void ResumptionSqlQueriesTest::CheckSelectQuery(const string& query_to_check,
                                                const int control_value,
                                                const int position_in_result) {
  SQLQuery query_to_check_request(db());
  EXPECT_TRUE(query_to_check_request.Prepare(query_to_check));
  EXPECT_TRUE(query_to_check_request.Exec());
  EXPECT_EQ(control_value,
            query_to_check_request.GetInteger(position_in_result));
}

void ResumptionSqlQueriesTest::CheckSelectQuery(const string& query_to_check,
                                                const bool control_value,
                                                const int position_in_result) {
  SQLQuery query_to_check_request(db());
  EXPECT_TRUE(query_to_check_request.Prepare(query_to_check));
  EXPECT_TRUE(query_to_check_request.Exec());
  EXPECT_EQ(control_value,
            query_to_check_request.GetBoolean(position_in_result));
}

void ResumptionSqlQueriesTest::CheckSelectQuery(const string& query_to_check,
                                                const string& control_value,
                                                const int position_in_result) {
  SQLQuery query_to_check_request(db());
  EXPECT_TRUE(query_to_check_request.Prepare(query_to_check));
  EXPECT_TRUE(query_to_check_request.Exec());
  EXPECT_EQ(control_value,
            query_to_check_request.GetString(position_in_result));
}

void ResumptionSqlQueriesTest::CheckSelectQuery(const string& query_to_check,
                                                const char* control_value,
                                                const int position_in_result) {
  SQLQuery query_to_check_request(db());
  EXPECT_TRUE(query_to_check_request.Prepare(query_to_check));
  EXPECT_TRUE(query_to_check_request.Exec());
  EXPECT_EQ(string(control_value),
            query_to_check_request.GetString(position_in_result));
}

void ResumptionSqlQueriesTest::CheckSelectQuery(const string& query_to_check,
                                                pair<int, string> app_info,
                                                pair<int, string> dev_info,
                                                const string& control_value,
                                                const int position_in_result) {
  SQLQuery query_to_check_request(db());
  EXPECT_TRUE(query_to_check_request.Prepare(query_to_check));
  if (!app_info.second.empty()) {
    query_to_check_request.Bind(app_info.first, app_info.second);
  }
  if (!dev_info.second.empty()) {
    query_to_check_request.Bind(dev_info.first, dev_info.second);
  }
  EXPECT_TRUE(query_to_check_request.Exec());
  EXPECT_EQ(control_value,
            query_to_check_request.GetString(position_in_result));
}

SQLQuery& ResumptionSqlQueriesTest::FillImageTable(SQLQuery& query,
                                                   const int imageType,
                                                   const string& value) {
  EXPECT_TRUE(query.Prepare(kInsertImage));
  query.Bind(0, imageType);
  query.Bind(1, value);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillTableLimitedCharacterListTable(
    SQLQuery& query, const string& limitedCharacterList) {
  EXPECT_TRUE(query.Prepare(kInsertTableLimitedCharacter));
  query.Bind(0, limitedCharacterList);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillCharacterArrayTable(
    SQLQuery& query, const int64_t glob_prop_key,
    const int64_t lim_char_list_key) {
  EXPECT_TRUE(query.Prepare(kInsertCharacterArray));
  query.Bind(0, glob_prop_key);
  query.Bind(1, lim_char_list_key);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillTTSChunkTable(SQLQuery& query,
                                                      const int type,
                                                      const string& text) {
  EXPECT_TRUE(query.Prepare(kInsertTTSChunk));
  query.Bind(0, type);
  query.Bind(1, text);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillHelpTimeoutPromptArrayTable(
    SQLQuery& query, int64_t glob_prop_key, int64_t tts_chunk_key,
    const int idhelpPrompt) {
  EXPECT_TRUE(query.Prepare(kInsertHelpTimeoutPromptArray));
  query.Bind(0, glob_prop_key);
  query.Bind(1, tts_chunk_key);
  query.Bind(2, idhelpPrompt);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillApplicationTable(
    SQLQuery& query, const int connection_key, const int grammarID,
    const string& hashID, const int hmiAppID, const int hmiLevel,
    const int ign_off_count, const int timeStamp, bool isMediaApplication,
    const string& appID, const string& deviceID, const int64_t glob_prop_key) {
  EXPECT_TRUE(query.Prepare(kInsertApplication));
  query.Bind(0, connection_key);
  query.Bind(1, grammarID);
  query.Bind(2, hashID);
  query.Bind(3, hmiAppID);
  query.Bind(4, hmiLevel);
  query.Bind(5, ign_off_count);
  query.Bind(6, timeStamp);
  query.Bind(7, glob_prop_key);
  query.Bind(8, isMediaApplication);
  query.Bind(9, appID);
  query.Bind(10, deviceID);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillGlobalPropertiesTable(
    SQLQuery& query, const string& vrHelpTitle, const string& menuTitle,
    const int language, const int keyboardLayout, const int keypressMode,
    const string& autoCompleteText, const int64_t image_key) {
  EXPECT_TRUE(query.Prepare(kInsertGlobalProperties));
  query.Bind(0, vrHelpTitle);
  query.Bind(1, menuTitle);
  query.Bind(2, image_key);
  query.Bind(3, language);
  query.Bind(4, keyboardLayout);
  query.Bind(5, keypressMode);
  query.Bind(6, autoCompleteText);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillApplicationCommandsArrayTable(
    SQLQuery& query, const int64_t application_key, const int64_t command_key) {
  EXPECT_TRUE(query.Prepare(kInsertApplicationCommandArray));
  query.Bind(0, application_key);
  query.Bind(1, command_key);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillCommandTable(
    SQLQuery& query, const int cmdID, const string& menuName,
    const int parentID, const int position, const int64_t image_key) {
  EXPECT_TRUE(query.Prepare(kInsertToCommand));
  query.Bind(0, cmdID);
  query.Bind(1, image_key);
  query.Bind(2, menuName);
  query.Bind(3, parentID);
  query.Bind(4, position);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillChoiceTable(SQLQuery& query,
                                                    const int choiceID,
                                                    const string& menuName,
                                                    const string& secondaryText,
                                                    const string& tertiaryText,
                                                    const int64_t image_key) {
  EXPECT_TRUE(query.Prepare(kInsertChoice));
  query.Bind(0, choiceID);
  query.Bind(1, menuName);
  query.Bind(2, secondaryText);
  query.Bind(3, tertiaryText);
  query.Bind(4, image_key);
  query.Bind(5, image_key);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillChoiceArrayTable(
    SQLQuery& query, const int64_t app_choice_set_key,
    const int64_t choice_key) {
  EXPECT_TRUE(query.Prepare(kInsertChoiceArray));
  query.Bind(0, app_choice_set_key);
  query.Bind(1, choice_key);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillApplicationChoiceSetTable(
    SQLQuery& query, const int grammarID, const int interactionChoiceSetID) {
  EXPECT_TRUE(query.Prepare(kInsertApplicationChoiceSet));
  query.Bind(0, grammarID);
  query.Bind(1, interactionChoiceSetID);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillApplicationChoiceSetArrayTable(
    SQLQuery& query, const int64_t app_choice_set_key, const int64_t app_key) {
  EXPECT_TRUE(query.Prepare(kInsertApplicationChoiceSetArray));
  query.Bind(0, app_choice_set_key);
  query.Bind(1, app_key);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillVRCommandsArrayTable(
    SQLQuery& query, const string& vrCommand, AccessoryVRCommand value,
    const int64_t foreing_key) {
  EXPECT_TRUE(query.Prepare(kInsertVrCommand));
  query.Bind(0, vrCommand);
  if (AccessoryVRCommand::kVRCommandFromCommand == value) {
    query.Bind(1, foreing_key);
    query.Bind(2);
  } else if (AccessoryVRCommand::kVRCommandFromChoice == value) {
    query.Bind(1);
    query.Bind(2, foreing_key);
  }
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillVRHelpItemTable(
    SQLQuery& query, const string& text, const int position,
    const int64_t image_key) {
  EXPECT_TRUE(query.Prepare(kInsertVRHelpItem));
  query.Bind(0, text);
  query.Bind(1, position);
  query.Bind(2, image_key);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillVRHelpItemArrayTable(
    SQLQuery& query, const int64_t global_prop_key,
    const int64_t vr_help_item_key) {
  EXPECT_TRUE(query.Prepare(kInsertVRHelpItemArray));
  query.Bind(0, global_prop_key);
  query.Bind(1, vr_help_item_key);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillApplicationSubMenuArrayTable(
    SQLQuery& query, const int app_key, const int sub_menu_key) {
  EXPECT_TRUE(query.Prepare(kInsertToApplicationSubMenuArray));
  query.Bind(0, app_key);
  query.Bind(1, sub_menu_key);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillAppSubscriptionsArrayTable(
    SQLQuery& query, const int vehicleValue, const int ButtonNameValue,
    const int app_key) {
  EXPECT_TRUE(query.Prepare(kInsertSubscriptions));
  query.Bind(0, app_key);
  query.Bind(1, vehicleValue);
  query.Bind(2, ButtonNameValue);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillFileTable(SQLQuery& query,
                                                  const int fileType,
                                                  bool is_download_complete,
                                                  bool persistentFile,
                                                  const string& syncFileName) {
  EXPECT_TRUE(query.Prepare(kInsertToFile));
  query.Bind(0, fileType);
  query.Bind(1, is_download_complete);
  query.Bind(2, persistentFile);
  query.Bind(3, syncFileName);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillSubMenuTable(SQLQuery& query,
                                                     const int menuID,
                                                     const string& menuName,
                                                     const int position) {
  EXPECT_TRUE(query.Prepare(kInsertToSubMenu));
  query.Bind(0, menuID);
  query.Bind(1, menuName);
  query.Bind(2, position);
  EXPECT_TRUE(query.Exec());
  return query;
}

SQLQuery& ResumptionSqlQueriesTest::FillApplicationFilesArrayTable(
    SQLQuery& query, const int64_t app_key, const int64_t file_key) {
  SQLQuery query_insert_application_files_array(db());
  EXPECT_TRUE(query_insert_application_files_array.Prepare(
      kInsertToApplicationFilesArray));
  query_insert_application_files_array.Bind(0, app_key);
  query_insert_application_files_array.Bind(1, file_key);
  EXPECT_TRUE(query_insert_application_files_array.Exec());
  return query;
}

TEST_F(ResumptionSqlQueriesTest, kCreateSchemaTest_ExpectSchemaCreated) {
  // Arrange
  SQLQuery query(db());
  DestroySchema();
  const std::string kCheckTablesNumber =
      "SELECT COUNT(*) FROM sqlite_master WHERE `type` = 'table'";
  // Check schema is not yet created
  CheckSelectQuery(kCheckTablesNumber, 0, 0);
  // Act
  CreateSchema();
  // Check schema created successfully
  EXPECT_TRUE(query.Prepare(kCheckTablesNumber) && query.Exec());
  EXPECT_GT(query.GetInteger(0), 0);
  ASSERT_TRUE(query.Reset());
  const std::string query_select_all_tbl_names =
      "SELECT `tbl_name` FROM sqlite_master WHERE `type` = 'table'";
  EXPECT_TRUE(query.Prepare(query_select_all_tbl_names));
  std::vector<std::string> result;
  db_schema_fill();
  while (query.Next()) {
    result.push_back(query.GetString(0));
  }
  std::sort(result.begin(), result.end());
  EXPECT_TRUE(db_schema == result);
}

TEST_F(ResumptionSqlQueriesTest, kDropSchemaTest_ExpectSchemaDestroyed) {
  // Arrange
  const std::string kCheckTablesNumber =
      "SELECT COUNT(*) FROM sqlite_master WHERE `type` = 'table'";
  // Act
  DestroySchema();
  // Check schema is already destroyed
  CheckSelectQuery(kCheckTablesNumber, 0, 0);
  // Restore Created database schema as used as shared source
  CreateSchema();
}

TEST_F(ResumptionSqlQueriesTest, kInsertInitData_ExpectInitDataInserted) {
  // Arrange
  SQLQuery query_insert_resumption(db());
  const std::string kCheckInitData = "SELECT COUNT(*) FROM `resumption`";
  CheckSelectQuery(kCheckInitData, 0, 0);
  const std::string kCheckInitData2 = "SELECT COUNT(*) FROM `_internal_data`";
  CheckSelectQuery(kCheckInitData2, 0, 0);
  // Act
  EXPECT_TRUE(query_insert_resumption.Exec(kInsertInitData));

  // Checks
  CheckSelectQuery(kCheckInitData, 1, 0);
  CheckSelectQuery(kCheckInitData2, 1, 0);

  const std::string kCheckResumptionInitValue =
      "SELECT last_ign_off_time FROM `resumption`";
  CheckSelectQuery(kCheckResumptionInitValue, 0, 0);
  const std::string kCheckInternalDataInitValue =
      "SELECT db_version_hash FROM `_internal_data`";
  CheckSelectQuery(kCheckInternalDataInitValue, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kChecksResumptionData_ExpectDataCorrect) {
  // Arrange
  CheckSelectQuery(kChecksResumptionData, 0, 0);
  // Act
  SQLQuery query_insert_resumption(db());
  EXPECT_TRUE(query_insert_resumption.Exec(kInsertInitData));
  // Check
  CheckSelectQuery(kChecksResumptionData, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCountHMILevel_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();

  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);

  ValToPosPair p1(0, device_id);
  ValToPosPair p2(1, app_id1);
  // Check
  CheckSelectQuery(kSelectCountHMILevel, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectHMILevel_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);
  ValToPosPair p1(0, device_id);
  ValToPosPair p2(1, app_id1);
  // Check
  CheckSelectQuery(kSelectHMILevel, p1, p2, hmiLevel, 0);
}

TEST_F(ResumptionSqlQueriesTest, kCheckHMIId_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);
  ValToPosPair p1(0, IntToString(hmiAppID));
  ValToPosPair p2(1, "");
  // Check
  CheckSelectQuery(kCheckHMIId, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectHMIId_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);
  ValToPosPair p1(0, device_id);
  ValToPosPair p2(1, app_id1);
  // Check
  CheckSelectQuery(kSelectHMIId, p1, p2, hmiAppID, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCountHMIId_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);

  ValToPosPair p1(0, device_id);
  ValToPosPair p2(1, app_id1);
  // Check
  CheckSelectQuery(kSelectCountHMIId, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kCountHashId_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);

  ValToPosPair p1(0, device_id);
  ValToPosPair p2(1, app_id1);
  // Check
  CheckSelectQuery(kCountHashId, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectHashId_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);
  ValToPosPair p1(0, device_id);
  ValToPosPair p2(1, app_id1);
  // Check
  CheckSelectQuery(kSelectHashId, p1, p2, test_hash, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectIgnOffTime_ExpectDataCorrect) {
  // Arrange
  SQLQuery query_insert_init_data(db());
  // Act
  EXPECT_TRUE(query_insert_init_data.Exec(kInsertInitData));
  // Check
  CheckSelectQuery(kSelectIgnOffTime, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kCheckApplication_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);
  ValToPosPair p1(0, device_id);
  ValToPosPair p2(1, app_id1);
  // Check
  CheckSelectQuery(kCheckApplication, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kCountApplications_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);

  key = FillImageTable(temp_query, 1, "tst_img2").LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash2,
                       hmiAppID2, hmiLevel2, ign_off_count, timeStamp2, false,
                       app_id2, device_id, key);
  // Check
  CheckSelectQuery(kCountApplications, 2, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kSelectDataForLoadResumeData_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();

  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);
  // Checks
  CheckSelectQuery(kSelectDataForLoadResumeData, hmiLevel, 0);
  CheckSelectQuery(kSelectDataForLoadResumeData, ign_off_count, 1);
  CheckSelectQuery(kSelectDataForLoadResumeData, timeStamp, 2);
  CheckSelectQuery(kSelectDataForLoadResumeData, app_id1, 3);
  CheckSelectQuery(kSelectDataForLoadResumeData, device_id, 4);
}

TEST_F(ResumptionSqlQueriesTest, kUpdateHMILevel_ExpectDataUpdated) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();

  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);
  // Act
  SQLQuery query_update_hmi_level(db());
  EXPECT_TRUE(query_update_hmi_level.Prepare(kUpdateHMILevel));
  query_update_hmi_level.Bind(0, hmiLevel2);
  query_update_hmi_level.Bind(1, device_id);
  query_update_hmi_level.Bind(2, app_id1);
  EXPECT_TRUE(query_update_hmi_level.Exec());
  ValToPosPair p1(0, device_id);
  ValToPosPair p2(1, app_id1);
  // Check
  CheckSelectQuery(kSelectHMILevel, p1, p2, hmiLevel2, 0);
}

TEST_F(ResumptionSqlQueriesTest, kUpdateIgnOffCount_ExpectDataUpdated) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);

  const std::string kSelectIgnOffCount =
      "SELECT ign_off_count FROM application;";
  // Check before action
  CheckSelectQuery(kSelectIgnOffCount, ign_off_count, 0);
  SQLQuery query(db());
  // Act
  EXPECT_TRUE(query.Exec(resumption::kUpdateIgnOffCount));
  // Check after action
  CheckSelectQuery(kSelectIgnOffCount, ign_off_count - 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kCountApplicationsIgnOff_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();

  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);

  FillApplicationTable(temp_query, connection_key, grammarID, test_hash2,
                       hmiAppID2, hmiLevel2, ign_off_count2, timeStamp2, false,
                       app_id2, device_id, key);

  ValToPosPair p1(0, IntToString(4));
  ValToPosPair p2(1, "");
  // Check
  CheckSelectQuery(kCountApplicationsIgnOff, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kSelectApplicationsIgnOffCount_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key);

  FillApplicationTable(temp_query, connection_key, grammarID, test_hash2,
                       hmiAppID2, hmiLevel2, ign_off_count2, timeStamp2, false,
                       app_id2, device_id, key);

  ValToPosPair p1(0, IntToString(4));
  ValToPosPair p2(1, "");

  CheckSelectQuery(kSelectApplicationsIgnOffCount, p1, p2, device_id, 0);
  CheckSelectQuery(kSelectApplicationsIgnOffCount, p1, p2, app_id2, 1);
}

TEST_F(ResumptionSqlQueriesTest, kUpdateSuspendData_ExpectDataUpdated) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  FillApplicationTable(temp_query, 1, 1, test_hash, 111, 1, 3, 45127, false,
                       app_id1, device_id, key);

  // Act
  SQLQuery query_update_suspend_data(db());
  EXPECT_TRUE(query_update_suspend_data.Exec(kUpdateSuspendData));
  // Check
  const std::string kSelectIgnOffCount =
      "SELECT ign_off_count FROM `application`;";
  CheckSelectQuery(kSelectIgnOffCount, 4, 0);
}

TEST_F(ResumptionSqlQueriesTest, KUpdateLastIgnOffTime_ExpectDataUpdated) {
  // Arrange
  SQLQuery query_insert_init_data(db());
  EXPECT_TRUE(query_insert_init_data.Exec(kInsertInitData));
  // Check before action
  CheckSelectQuery(kSelectIgnOffTime, 0, 0);
  // Act
  SQLQuery query_update_ign_off_time(db());
  EXPECT_TRUE(query_update_ign_off_time.Prepare(KUpdateLastIgnOffTime));
  query_update_ign_off_time.Bind(0, 1);
  EXPECT_TRUE(query_update_ign_off_time.Exec());
  // Check after action
  CheckSelectQuery(kSelectIgnOffTime, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteFile_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key1 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                   "auto", key1).LastInsertId();
  key1 = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                              hmiAppID, hmiLevel, ign_off_count, timeStamp,
                              false, app_id2, device_id, key1).LastInsertId();
  int64_t key2 =
      FillFileTable(temp_query, 1, true, true, "tst_name").LastInsertId();
  FillApplicationFilesArrayTable(temp_query, key1, key2);
  // Check before action
  const std::string select_count_file = "SELECT COUNT(*) from `file` ";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_file, kDeleteFile, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteApplicationFilesArray_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key1 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                   "auto", key1).LastInsertId();
  key1 = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                              hmiAppID, hmiLevel, ign_off_count, timeStamp,
                              false, app_id2, device_id, key1).LastInsertId();
  int64_t key2 =
      FillFileTable(temp_query, 1, true, true, "tst_name").LastInsertId();
  FillApplicationFilesArrayTable(temp_query, key1, key2);
  // Check
  const std::string select_count_applicationsFilesArray =
      "SELECT COUNT(*) from `applicationFilesArray` ";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_applicationsFilesArray,
                   kDeleteApplicationFilesArray, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteSubMenu_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  key = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                             hmiAppID, hmiLevel, ign_off_count, timeStamp,
                             false, app_id2, device_id, key).LastInsertId();
  int64_t submenu_key =
      FillSubMenuTable(temp_query, 1, "tst_menuName", 2).LastInsertId();

  FillApplicationSubMenuArrayTable(temp_query, key, submenu_key);
  // Check
  const std::string select_count_subMenu = "SELECT COUNT(*) FROM subMenu;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_subMenu, kDeleteSubMenu, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteApplicationSubMenuArray_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                  "auto", key).LastInsertId();
  key = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                             hmiAppID, hmiLevel, ign_off_count, timeStamp,
                             false, app_id2, device_id, key).LastInsertId();

  int64_t submenu_key =
      FillSubMenuTable(temp_query, 1, "tst_menuName", 2).LastInsertId();

  FillApplicationSubMenuArrayTable(temp_query, key, submenu_key);
  // Check
  const std::string select_count_subMenu =
      "SELECT COUNT(*) FROM applicationSubMenuArray;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_subMenu, kDeleteApplicationSubMenuArray, p1, p2,
                   1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteApplicationSubscribtionsArray_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  key1 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                   "auto", key1).LastInsertId();
  key1 = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                              hmiAppID, hmiLevel, ign_off_count, timeStamp,
                              false, app_id2, device_id, key1).LastInsertId();
  int64_t key2 =
      FillFileTable(temp_query, 1, true, true, "tst_name").LastInsertId();
  key1 = FillApplicationFilesArrayTable(temp_query, key1, key2).LastInsertId();
  FillAppSubscriptionsArrayTable(temp_query, 7, 2, key1);
  // Check
  const std::string select_count_applicationSubscribtionsArray =
      "SELECT COUNT(*) FROM applicationSubscribtionsArray;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_applicationSubscribtionsArray,
                   kDeleteApplicationSubscribtionsArray, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteImageFromCommands_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 = FillCommandTable(temp_query, 1, "tst_menu_name", 1, 2, key1)
                     .LastInsertId();
  key1 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                   "auto", key1).LastInsertId();
  key1 = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                              hmiAppID, hmiLevel, ign_off_count, timeStamp,
                              false, app_id2, device_id, key1).LastInsertId();
  FillApplicationCommandsArrayTable(temp_query, key1, key2);
  // Check before action
  const std::string select_count_image = "SELECT COUNT(*) FROM image;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_image, kDeleteImageFromCommands, p1, p2, 1, 0,
                   0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteVrCommands_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 = FillCommandTable(temp_query, 1, "tst_menu_name", 1, 2, key1)
                     .LastInsertId();
  key1 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                   "auto", key1).LastInsertId();
  key1 = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                              hmiAppID, hmiLevel, ign_off_count, timeStamp,
                              false, app_id2, device_id, key1).LastInsertId();

  FillVRCommandsArrayTable(temp_query, "tst_vr_command", kVRCommandFromCommand,
                           key2);
  FillApplicationCommandsArrayTable(temp_query, key1, key2);
  // Check
  const std::string select_count_vrCommandsArray =
      "SELECT COUNT(*) FROM vrCommandsArray;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_vrCommandsArray, kDeleteVrCommands, p1, p2, 1,
                   0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteCommands_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 = FillCommandTable(temp_query, 1, "tst_menu_name", 1, 2, key1)
                     .LastInsertId();

  key1 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                   "auto", key1).LastInsertId();
  key1 = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                              hmiAppID, hmiLevel, ign_off_count, timeStamp,
                              false, app_id2, device_id, key1).LastInsertId();

  FillApplicationCommandsArrayTable(temp_query, key1, key2);
  // Check
  const std::string select_count_command = "SELECT COUNT(*) FROM command;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_command, kDeleteCommands, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteApplicationCommandsArray_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 = FillCommandTable(temp_query, 1, "tst_menu_name", 1, 2, key1)
                     .LastInsertId();

  key1 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                   "auto", key1).LastInsertId();
  key1 = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                              hmiAppID, hmiLevel, ign_off_count, timeStamp,
                              false, app_id2, device_id, key1).LastInsertId();

  FillApplicationCommandsArrayTable(temp_query, key1, key2);
  // Check
  const std::string select_count_applicationCommandsArray =
      "SELECT COUNT(*) FROM applicationCommandsArray;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_applicationCommandsArray,
                   kDeleteApplicationCommandsArray, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteImageFromChoiceSet_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 =
      FillChoiceTable(temp_query, 2, "tst_menu_name", "secondary_txt",
                      "tst_tert_text", key1).LastInsertId();
  int64_t key3 =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id2, device_id, key1).LastInsertId();
  int64_t key4 = FillApplicationChoiceSetTable(temp_query, 1, 2).LastInsertId();
  FillChoiceArrayTable(temp_query, key4, key2);
  FillApplicationChoiceSetArrayTable(temp_query, key4, key3);

  // Check
  const std::string select_count_image = "SELECT COUNT(*) FROM image;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_image, kDeleteImageFromChoiceSet, p1, p2, 1, 0,
                   0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteVrCommandsFromChoiceSet_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 = FillChoiceTable(temp_query, 1, "tst_menu_name", "second_text",
                                 "tert_txt", key1).LastInsertId();
  key1 = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                              hmiAppID, hmiLevel, ign_off_count, timeStamp,
                              false, app_id2, device_id, key1).LastInsertId();

  FillVRCommandsArrayTable(temp_query, "tst_vr_command", kVRCommandFromChoice,
                           key2);

  int64_t key3 = FillApplicationChoiceSetTable(temp_query, 1, 2).LastInsertId();

  FillChoiceArrayTable(temp_query, key3, key2);
  FillApplicationChoiceSetArrayTable(temp_query, key3, key1);
  // Check before action
  const std::string select_count_vrCommandsArray =
      "SELECT COUNT(*) FROM vrCommandsArray;";
  // Check
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_vrCommandsArray, kDeleteVrCommandsFromChoiceSet,
                   p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteChoice_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 = FillChoiceTable(temp_query, 1, "tst_menu_name", "second_text",
                                 "tert_txt", key1).LastInsertId();
  key1 = FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                              hmiAppID, hmiLevel, ign_off_count, timeStamp,
                              false, app_id2, device_id, key1).LastInsertId();
  int64_t key3 = FillApplicationChoiceSetTable(temp_query, 1, 2).LastInsertId();
  FillChoiceArrayTable(temp_query, key3, key2);
  FillApplicationChoiceSetArrayTable(temp_query, key3, key1);

  // Check before action
  const std::string select_count_choice = "SELECT COUNT(*) FROM choice;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_choice, kDeleteChoice, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteChoiceArray_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t choice_table_key =
      FillChoiceTable(temp_query, 1, "tst_menu_name", "second_text", "tert_txt",
                      0).LastInsertId();

  int64_t application_choiceset_table_key =
      FillApplicationChoiceSetTable(temp_query, 1, 2).LastInsertId();
  int64_t application_table_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id2, device_id, 1).LastInsertId();
  FillApplicationChoiceSetArrayTable(
      temp_query, application_choiceset_table_key, application_table_key);
  FillChoiceArrayTable(temp_query, application_choiceset_table_key,
                       choice_table_key);
  // Check
  const std::string select_count_choice_array =
      "SELECT COUNT(*) FROM choiceArray;";

  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_choice_array, kDeleteChoiceArray, p1, p2, 1, 0,
                   0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteApplicationChoiceSet_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id2, device_id, 1).LastInsertId();

  int64_t key2 = FillApplicationChoiceSetTable(temp_query, 1, 2).LastInsertId();
  FillApplicationChoiceSetArrayTable(temp_query, key2, key1);

  // Check before action
  const std::string select_count_applicationChoiceSet =
      "SELECT COUNT(*) FROM applicationChoiceSet;";

  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_applicationChoiceSet,
                   kDeleteApplicationChoiceSet, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteApplicationChoiceSetArray_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id2, device_id, 1).LastInsertId();

  int64_t key2 = FillApplicationChoiceSetTable(temp_query, 1, 2).LastInsertId();
  FillApplicationChoiceSetArrayTable(temp_query, key2, key1);

  // Check
  const std::string select_count_applicationChoiceSetArray =
      "SELECT COUNT(*) FROM applicationChoiceSetArray;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_applicationChoiceSetArray,
                   kDeleteApplicationChoiceSetArray, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteImageFromGlobalProperties_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1,
                                           1, 2, "auto", key1).LastInsertId();
  key1 = FillVRHelpItemTable(temp_query, "tst_text", 1, key1).LastInsertId();
  FillVRHelpItemArrayTable(temp_query, key2, key1);
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id2, device_id, key2);
  // Check
  const std::string select_count_image = "SELECT COUNT(*) FROM image;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_image, kDeleteImageFromGlobalProperties, p1, p2,
                   1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeletevrHelpItem_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1,
                                           1, 2, "auto", key1).LastInsertId();
  key1 = FillVRHelpItemTable(temp_query, "tst_text", 1, key1).LastInsertId();
  FillVRHelpItemArrayTable(temp_query, key2, key1);
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id2, device_id, key2);
  // Check
  const std::string select_count_vrhelp_item =
      "SELECT COUNT(*) FROM vrHelpItem;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_vrhelp_item, kDeletevrHelpItem, p1, p2, 1, 0,
                   0);
}

TEST_F(ResumptionSqlQueriesTest, kDeletevrHelpItemArray_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t key1 = FillImageTable(temp_query, 1, test_image).LastInsertId();
  int64_t key2 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1,
                                           1, 2, "auto", key1).LastInsertId();
  key1 = FillVRHelpItemTable(temp_query, "tst_text", 1, key1).LastInsertId();
  FillVRHelpItemArrayTable(temp_query, key2, key1);
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id2, device_id, key2);
  // Check
  const std::string select_count_vrhelp_item_array =
      "SELECT COUNT(*) FROM vrHelpItemArray;";
  ValToPosPair p1(0, app_id2);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_vrhelp_item_array, kDeletevrHelpItemArray, p1,
                   p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteTableLimitedCharacterList_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  string character_list = "abcdefghij";
  int64_t key1 = FillTableLimitedCharacterListTable(temp_query, character_list)
                     .LastInsertId();
  int64_t key2 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1,
                                           1, 2, "auto", 0).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key2);
  FillCharacterArrayTable(temp_query, key2, key1);
  // Check
  const std::string select_count_tableLimitedCharacterList =
      "SELECT COUNT(*) FROM tableLimitedCharacterList;";
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_tableLimitedCharacterList,
                   kDeleteTableLimitedCharacterList, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteCharacterArray_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  string character_list = "abcdefghij";
  int64_t key1 = FillTableLimitedCharacterListTable(temp_query, character_list)
                     .LastInsertId();
  int64_t key2 = FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1,
                                           1, 2, "auto", 0).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, key2);
  FillCharacterArrayTable(temp_query, key2, key1);
  // Check
  const std::string select_count_characterArray =
      "SELECT COUNT(*) FROM characterArray;";
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_characterArray, kDeleteCharacterArray, p1, p2,
                   1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteTTSChunk_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t tts_chunk_key =
      FillTTSChunkTable(temp_query, 1, "tst_text").LastInsertId();
  int64_t glob_prop_key =
      FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                "auto", 0).LastInsertId();

  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, glob_prop_key);

  FillHelpTimeoutPromptArrayTable(temp_query, glob_prop_key, tts_chunk_key, 1);
  // Check
  const std::string select_count_tts_chunk = "SELECT COUNT(*) FROM TTSChunk;";
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_tts_chunk, kDeleteTTSChunk, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteFromApplicationTable_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, 1);
  // Check
  const std::string select_count_application =
      "SELECT COUNT(*) FROM application;";
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_application, kDeleteFromApplicationTable, p1,
                   p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kDeleteHelpTimeoutPromptArray_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t tts_chunk_key =
      FillTTSChunkTable(temp_query, 1, "tst_text").LastInsertId();
  int64_t glob_prop_key =
      FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                "auto", 0).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, glob_prop_key);

  FillHelpTimeoutPromptArrayTable(temp_query, glob_prop_key, tts_chunk_key, 1);
  // Check
  const std::string select_count_helpTimeoutPromptArray =
      "SELECT COUNT(*) FROM helpTimeoutPromptArray;";
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_helpTimeoutPromptArray,
                   kDeleteHelpTimeoutPromptArray, p1, p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kDeleteGlobalProperties_ExpectDataDeleted) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t glob_prop_key =
      FillGlobalPropertiesTable(temp_query, "vrHelp", "menuTitle", 1, 1, 2,
                                "auto", 0).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, glob_prop_key);
  // Check
  const std::string select_count_globalProperties =
      "SELECT COUNT(*) FROM globalProperties;";
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  CheckDeleteQuery(select_count_globalProperties, kDeleteGlobalProperties, p1,
                   p2, 1, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCountImage_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  FillImageTable(temp_query, 2, "tst_image");
  ValToPosPair p1(0, "tst_image");
  ValToPosPair p2(1, "");
  // Check
  CheckSelectQuery(kSelectCountImage, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectPrimaryKeyImage_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t image_key = FillImageTable(temp_query, 2, "tst_image").LastInsertId();
  ValToPosPair p1(0, "tst_image");
  ValToPosPair p2(1, "");
  // Check
  CheckSelectQuery(kSelectPrimaryKeyImage, p1, p2, image_key, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertImage_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillImageTable(temp_query, 2, "tst_image");
  const std::string select_count_image = "SELECT COUNT(*) FROM image;";
  // Check
  CheckSelectQuery(select_count_image, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertToFile_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillFileTable(temp_query, 1, true, true, "tst_file_name");
  // Check
  const std::string select_count_file = "SELECT COUNT(*) FROM file;";
  CheckSelectQuery(select_count_file, 1, 0);
  const std::string select_file_name = "SELECT syncFileName FROM file;";
  CheckSelectQuery(select_file_name, "tst_file_name", 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kInsertToApplicationFilesArray_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationFilesArrayTable(temp_query, 1, 2);
  // Checks
  const std::string select_count_applicationFilesArray =
      "SELECT COUNT(*) FROM applicationFilesArray;";
  CheckSelectQuery(select_count_applicationFilesArray, 1, 0);
  const std::string select_idApplication =
      "SELECT idApplication FROM applicationFilesArray;";
  CheckSelectQuery(select_idApplication, 1, 0);
  const std::string select_idfile = "SELECT idfile FROM applicationFilesArray;";
  CheckSelectQuery(select_idfile, 2, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertToSubMenu_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillSubMenuTable(temp_query, 1, "tst_menu", 3);
  // Checks
  const std::string select_count_subMenu = "SELECT COUNT(*) FROM subMenu;";
  CheckSelectQuery(select_count_subMenu, 1, 0);
  const std::string select_menuID = "SELECT menuID FROM subMenu;";
  CheckSelectQuery(select_menuID, 1, 0);
  const std::string select_menuName = "SELECT menuName FROM subMenu;";
  CheckSelectQuery(select_menuName, "tst_menu", 0);
  const std::string select_position = "SELECT position FROM subMenu;";
  CheckSelectQuery(select_position, 3, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kInsertToApplicationSubMenuArray_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationSubMenuArrayTable(temp_query, 2, 5);
  // Checks
  const std::string select_count_applicationFilesArray =
      "SELECT COUNT(*) FROM applicationSubMenuArray;";
  CheckSelectQuery(select_count_applicationFilesArray, 1, 0);
  const std::string select_idApplication =
      "SELECT idApplication FROM applicationSubMenuArray;";
  CheckSelectQuery(select_idApplication, 2, 0);
  const std::string select_idsubMenu =
      "SELECT idsubMenu FROM applicationSubMenuArray;";
  CheckSelectQuery(select_idsubMenu, 5, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertToCommand_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillCommandTable(temp_query, 1, "tst_menu", 1, 2, 5);
  // Checks
  const std::string select_count_command = "SELECT COUNT(*) FROM command;";
  CheckSelectQuery(select_count_command, 1, 0);
  const std::string select_cmdID = "SELECT cmdID FROM command;";
  CheckSelectQuery(select_cmdID, 1, 0);
  const std::string select_menuName = "SELECT menuName FROM command;";
  CheckSelectQuery(select_menuName, "tst_menu", 0);
  const std::string select_parentID = "SELECT parentID FROM command;";
  CheckSelectQuery(select_parentID, 1, 0);
  const std::string select_position = "SELECT position FROM command;";
  CheckSelectQuery(select_position, 2, 0);
  const std::string select_idimage = "SELECT idimage FROM command;";
  CheckSelectQuery(select_idimage, 5, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kInsertApplicationCommandArray_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationCommandsArrayTable(temp_query, 3, 7);
  // Checks
  const std::string select_count_applicationCommandsArray =
      "SELECT COUNT(*) FROM applicationCommandsArray;";
  CheckSelectQuery(select_count_applicationCommandsArray, 1, 0);
  const std::string select_idApplication =
      "SELECT idApplication FROM applicationCommandsArray;";
  CheckSelectQuery(select_idApplication, 3, 0);
  const std::string select_idcommand =
      "SELECT idcommand FROM applicationCommandsArray;";
  CheckSelectQuery(select_idcommand, 7, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kInsertVrCommandFromChoice_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillVRCommandsArrayTable(temp_query, "vr_cmd", kVRCommandFromChoice, 8);
  // Checks
  const std::string select_count_vrCommandsArray =
      "SELECT COUNT(*) FROM vrCommandsArray;";
  CheckSelectQuery(select_count_vrCommandsArray, 1, 0);
  const std::string select_vrCommand = "SELECT vrCommand FROM vrCommandsArray;";
  CheckSelectQuery(select_vrCommand, "vr_cmd", 0);
  const std::string select_idcommand = "SELECT idcommand FROM vrCommandsArray;";
  CheckSelectQuery(select_idcommand, 0, 0);
  const std::string select_idchoice = "SELECT idchoice FROM vrCommandsArray;";
  CheckSelectQuery(select_idchoice, 8, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kInsertVrCommandFromCommand_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillVRCommandsArrayTable(temp_query, "vr_cmd", kVRCommandFromCommand, 9);
  // Checks
  const std::string select_count_vrCommandsArray =
      "SELECT COUNT(*) FROM vrCommandsArray;";
  CheckSelectQuery(select_count_vrCommandsArray, 1, 0);
  const std::string select_vrCommand = "SELECT vrCommand FROM vrCommandsArray;";
  CheckSelectQuery(select_vrCommand, "vr_cmd", 0);
  const std::string select_idcommand = "SELECT idcommand FROM vrCommandsArray;";
  CheckSelectQuery(select_idcommand, 9, 0);
  const std::string select_idchoice = "SELECT idchoice FROM vrCommandsArray;";
  CheckSelectQuery(select_idchoice, 0, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertSubscriptions_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillAppSubscriptionsArrayTable(temp_query, 2, 3, 4);
  // Checks
  const std::string select_count_applicationSubscribtionsArray =
      "SELECT COUNT(*) FROM applicationSubscribtionsArray;";
  CheckSelectQuery(select_count_applicationSubscribtionsArray, 1, 0);
  const std::string select_idApplication =
      "SELECT idApplication FROM applicationSubscribtionsArray;";
  CheckSelectQuery(select_idApplication, 4, 0);
  const std::string select_vehicleValue =
      "SELECT vehicleValue FROM applicationSubscribtionsArray;";
  CheckSelectQuery(select_vehicleValue, 2, 0);
  const std::string select_ButtonNameValue =
      "SELECT ButtonNameValue FROM applicationSubscribtionsArray;";
  CheckSelectQuery(select_ButtonNameValue, 3, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertChoice_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillChoiceTable(temp_query, 1, "tst_menu", "second_text", "tert_txt", 5);
  // Checks
  const std::string select_count_choice = "SELECT COUNT(*) FROM choice;";
  CheckSelectQuery(select_count_choice, 1, 0);

  const std::string select_choiceID = "SELECT choiceID FROM choice;";
  CheckSelectQuery(select_choiceID, 1, 0);
  const std::string select_menuName = "SELECT menuName FROM choice;";
  CheckSelectQuery(select_menuName, "tst_menu", 0);
  const std::string select_secondaryText = "SELECT secondaryText FROM choice;";
  CheckSelectQuery(select_secondaryText, "second_text", 0);
  const std::string select_tertiaryText = "SELECT tertiaryText FROM choice;";
  CheckSelectQuery(select_tertiaryText, "tert_txt", 0);
  const std::string select_idimage = "SELECT idimage FROM choice;";
  CheckSelectQuery(select_idimage, 5, 0);
  const std::string select_idsecondaryImage =
      "SELECT idsecondaryImage FROM choice;";
  CheckSelectQuery(select_idsecondaryImage, 5, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kInsertApplicationChoiceSet_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationChoiceSetTable(temp_query, 7, 12);
  // Checks
  const std::string select_count_applicationChoiceSet =
      "SELECT COUNT(*) FROM applicationChoiceSet;";
  CheckSelectQuery(select_count_applicationChoiceSet, 1, 0);

  const std::string select_grammarID =
      "SELECT grammarID FROM applicationChoiceSet;";
  CheckSelectQuery(select_grammarID, 7, 0);
  const std::string select_interactionChoiceSetID =
      "SELECT interactionChoiceSetID FROM applicationChoiceSet;";
  CheckSelectQuery(select_interactionChoiceSetID, 12, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertChoiceArray_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillChoiceArrayTable(temp_query, 3, 5);
  // Checks
  const std::string select_count_choiceArray =
      "SELECT COUNT(*) FROM choiceArray;";
  CheckSelectQuery(select_count_choiceArray, 1, 0);
  const std::string select_idapplicationChoiceSet =
      "SELECT idapplicationChoiceSet FROM choiceArray;";
  CheckSelectQuery(select_idapplicationChoiceSet, 3, 0);
  const std::string select_idchoice = "SELECT idchoice FROM choiceArray;";
  CheckSelectQuery(select_idchoice, 5, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kInsertApplicationChoiceSetArray_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationChoiceSetArrayTable(temp_query, 3, 5);
  // Checks
  const std::string select_count_applicationChoiceSetArray =
      "SELECT COUNT(*) FROM applicationChoiceSetArray;";
  CheckSelectQuery(select_count_applicationChoiceSetArray, 1, 0);
  const std::string select_idapplicationChoiceSet =
      "SELECT idapplicationChoiceSet FROM applicationChoiceSetArray;";
  CheckSelectQuery(select_idapplicationChoiceSet, 3, 0);
  const std::string select_idApplication =
      "SELECT idApplication FROM applicationChoiceSetArray;";
  CheckSelectQuery(select_idApplication, 5, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertGlobalProperties_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillGlobalPropertiesTable(temp_query, "vr_title", "menu_title", 2, 1, 3,
                            "auto", 7);
  // Checks
  const std::string select_count_globalProperties =
      "SELECT COUNT(*) FROM globalProperties;";
  CheckSelectQuery(select_count_globalProperties, 1, 0);
  const std::string select_vrHelpTitle =
      "SELECT vrHelpTitle FROM globalProperties;";
  CheckSelectQuery(select_vrHelpTitle, "vr_title", 0);
  const std::string select_menuTitle =
      "SELECT menuTitle FROM globalProperties;";
  CheckSelectQuery(select_menuTitle, "menu_title", 0);
  const std::string select_idmenuIcon =
      "SELECT idmenuIcon FROM globalProperties;";
  CheckSelectQuery(select_idmenuIcon, 7, 0);
  const std::string select_language = "SELECT language FROM globalProperties;";
  CheckSelectQuery(select_language, 2, 0);
  const std::string select_keyboardLayout =
      "SELECT keyboardLayout FROM globalProperties;";
  CheckSelectQuery(select_keyboardLayout, 1, 0);
  const std::string select_keypressMode =
      "SELECT keypressMode FROM globalProperties;";
  CheckSelectQuery(select_keypressMode, 3, 0);
  const std::string select_autoCompleteText =
      "SELECT autoCompleteText FROM globalProperties;";
  CheckSelectQuery(select_autoCompleteText, "auto", 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kInsertTableLimitedCharacter_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  string character_list = "abcdefghijkl";
  FillTableLimitedCharacterListTable(temp_query, character_list);
  // Checks
  const std::string select_count_tableLimitedCharacterList =
      "SELECT COUNT(*) FROM tableLimitedCharacterList;";
  CheckSelectQuery(select_count_tableLimitedCharacterList, 1, 0);
  const std::string select_limitedCharacterList =
      "SELECT limitedCharacterList FROM tableLimitedCharacterList;";
  CheckSelectQuery(select_limitedCharacterList, character_list, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertCharacterArray_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillCharacterArrayTable(temp_query, 2, 4);
  // Checks
  const std::string select_count_characterArray =
      "SELECT COUNT(*) FROM characterArray;";
  CheckSelectQuery(select_count_characterArray, 1, 0);
  const std::string select_idglobalProperties =
      "SELECT idglobalProperties FROM characterArray;";
  CheckSelectQuery(select_idglobalProperties, 2, 0);
  const std::string select_idtableLimitedCharacterList =
      "SELECT idtableLimitedCharacterList FROM characterArray;";
  CheckSelectQuery(select_idtableLimitedCharacterList, 4, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertVRHelpItem_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillVRHelpItemTable(temp_query, "tst_txt", 1, 3);
  // Checks
  const std::string select_count_vrHelpItem =
      "SELECT COUNT(*) FROM vrHelpItem;";
  CheckSelectQuery(select_count_vrHelpItem, 1, 0);
  const std::string select_text = "SELECT text FROM vrHelpItem;";
  CheckSelectQuery(select_text, "tst_txt", 0);
  const std::string select_position = "SELECT position FROM vrHelpItem;";
  CheckSelectQuery(select_position, 1, 0);
  const std::string select_idimage = "SELECT idimage FROM vrHelpItem;";
  CheckSelectQuery(select_idimage, 3, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertVRHelpItemArray_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillVRHelpItemArrayTable(temp_query, 4, 5);
  // Checks
  const std::string select_count_vrHelpItemArray =
      "SELECT COUNT(*) FROM vrHelpItemArray;";
  CheckSelectQuery(select_count_vrHelpItemArray, 1, 0);
  const std::string select_idglobalProperties =
      "SELECT idglobalProperties FROM vrHelpItemArray;";
  CheckSelectQuery(select_idglobalProperties, 4, 0);
  const std::string select_idvrHelpItem =
      "SELECT idvrHelpItem FROM vrHelpItemArray;";
  CheckSelectQuery(select_idvrHelpItem, 5, 0);
}

TEST_F(ResumptionSqlQueriesTest,
       kInsertHelpTimeoutPromptArray_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillHelpTimeoutPromptArrayTable(temp_query, 1, 2, 3);
  // Checks
  const std::string select_count_helpTimeoutPromptArray =
      "SELECT COUNT(*) FROM helpTimeoutPromptArray;";
  CheckSelectQuery(select_count_helpTimeoutPromptArray, 1, 0);
  const std::string select_idglobalProperties =
      "SELECT idglobalProperties FROM helpTimeoutPromptArray;";
  CheckSelectQuery(select_idglobalProperties, 1, 0);
  const std::string select_idtimeoutPrompt =
      "SELECT idtimeoutPrompt FROM helpTimeoutPromptArray;";
  CheckSelectQuery(select_idtimeoutPrompt, 2, 0);
  const std::string select_idhelpPrompt =
      "SELECT idhelpPrompt FROM helpTimeoutPromptArray;";
  CheckSelectQuery(select_idhelpPrompt, 3, 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertTTSChunk_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillTTSChunkTable(temp_query, 2, "text");
  // Checks
  const std::string select_count_kInsertTTSChunk =
      "SELECT COUNT(*) FROM TTSChunk;";
  CheckSelectQuery(select_count_kInsertTTSChunk, 1, 0);
  const std::string select_type = "SELECT type FROM TTSChunk;";
  CheckSelectQuery(select_type, 2, 0);
  const std::string select_text = "SELECT text FROM TTSChunk;";
  CheckSelectQuery(select_text, "text", 0);
}

TEST_F(ResumptionSqlQueriesTest, kInsertApplication_ExpectDataInserted) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, 9);
  // Checks
  const std::string select_count_application =
      "SELECT COUNT(*) FROM application;";
  CheckSelectQuery(select_count_application, 1, 0);

  const std::string select_connection_key =
      "SELECT connection_key FROM application;";
  CheckSelectQuery(select_connection_key, connection_key, 0);
  const std::string select_grammarID = "SELECT grammarID FROM application;";
  CheckSelectQuery(select_grammarID, grammarID, 0);
  const std::string select_hashID = "SELECT hashID FROM application;";
  CheckSelectQuery(select_hashID, test_hash, 0);
  const std::string select_hmiAppID = "SELECT hmiAppID FROM application;";
  CheckSelectQuery(select_hmiAppID, hmiAppID, 0);
  const std::string select_hmiLevel = "SELECT hmiLevel FROM application;";
  CheckSelectQuery(select_hmiLevel, hmiLevel, 0);
  const std::string select_ign_off_count =
      "SELECT ign_off_count FROM application;";
  CheckSelectQuery(select_ign_off_count, ign_off_count, 0);
  const std::string select_timeStamp = "SELECT timeStamp FROM application;";
  CheckSelectQuery(select_timeStamp, timeStamp, 0);
  const std::string select_idglobalProperties =
      "SELECT idglobalProperties FROM application;";
  CheckSelectQuery(select_idglobalProperties, 9, 0);
  const std::string select_isMediaApplication =
      "SELECT isMediaApplication FROM application;";
  CheckSelectQuery(select_isMediaApplication, false, 0);
  const std::string select_appID = "SELECT appID FROM application;";
  CheckSelectQuery(select_appID, app_id1, 0);
  const std::string select_deviceID = "SELECT deviceID FROM application;";
  CheckSelectQuery(select_deviceID, device_id, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCountFiles_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  int64_t file_key =
      FillFileTable(temp_query, 1, true, true, "tst_name").LastInsertId();
  FillApplicationFilesArrayTable(temp_query, app_key, file_key);

  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectCountFiles, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectFiles_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  int64_t file_key =
      FillFileTable(temp_query, 1, true, true, "tst_name").LastInsertId();
  FillApplicationFilesArrayTable(temp_query, app_key, file_key);
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectFiles, p1, p2, 1, 0);
  CheckSelectQuery(kSelectFiles, p1, p2, true, 1);
  CheckSelectQuery(kSelectFiles, p1, p2, true, 2);
  CheckSelectQuery(kSelectFiles, p1, p2, "tst_name", 3);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCountSubMenu_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  int64_t submenu_key =
      FillSubMenuTable(temp_query, 1, "menu_name", 1).LastInsertId();
  FillApplicationSubMenuArrayTable(temp_query, app_key, submenu_key);
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectCountSubMenu, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectSubMenu_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  int64_t submenu_key =

      FillSubMenuTable(temp_query, 1, "menu_name", 1).LastInsertId();

  FillApplicationSubMenuArrayTable(temp_query, app_key, submenu_key);
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectSubMenu, p1, p2, 1, 0);
  CheckSelectQuery(kSelectSubMenu, p1, p2, "menu_name", 1);
  CheckSelectQuery(kSelectSubMenu, p1, p2, 1, 2);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCountCommands_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  int64_t command_key =
      FillCommandTable(temp_query, 1, "menu_name", 1, 2, 5).LastInsertId();
  FillApplicationCommandsArrayTable(temp_query, app_key, command_key);
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectCountCommands, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCommandsFromCommand_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  int64_t image_key = FillImageTable(temp_query, 2, "tst_image").LastInsertId();

  FillChoiceTable(temp_query, 1, "menu_name", "sec_text", "tert_text",
                  image_key).LastInsertId();
  int64_t command_key = FillCommandTable(temp_query, 1, "menu_name", 1, 2,
                                         image_key).LastInsertId();

  FillApplicationCommandsArrayTable(temp_query, app_key, command_key);
  FillVRCommandsArrayTable(temp_query, "best", kVRCommandFromCommand,
                           command_key);
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectCommands, p1, p2, 1, 0);
  CheckSelectQuery(kSelectCommands, p1, p2, 1, 1);
  CheckSelectQuery(kSelectCommands, p1, p2, "menu_name", 2);
  CheckSelectQuery(kSelectCommands, p1, p2, 1, 3);
  CheckSelectQuery(kSelectCommands, p1, p2, 2, 4);
  CheckSelectQuery(kSelectCommands, p1, p2, "tst_image", 5);
  CheckSelectQuery(kSelectCommands, p1, p2, 2, 6);
  CheckSelectQuery(kSelectCommands, p1, p2, "best", 7);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCommandsFromChoice_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  int64_t image_key = FillImageTable(temp_query, 2, "tst_image").LastInsertId();

  int64_t choice_key = FillChoiceTable(temp_query, 1, "menu_name", "sec_text",
                                       "tert_text", image_key).LastInsertId();
  int64_t command_key = FillCommandTable(temp_query, 1, "menu_name", 1, 2,
                                         image_key).LastInsertId();

  FillApplicationCommandsArrayTable(temp_query, app_key, command_key);
  FillVRCommandsArrayTable(temp_query, "best", kVRCommandFromChoice,
                           choice_key);
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  const std::string kChecksVrCommandFromSelectCommand =
      "SELECT COUNT(*) FROM (SELECT `command`.`idcommand`, `cmdID`, "
      "`menuName`, `parentID`, `position`, `value`, `imageType`, `vrCommand` "
      "FROM  `command` LEFT OUTER JOIN `image` on `command`.`idimage` = "
      "`image`.`idimage` "
      "LEFT OUTER JOIN  `vrcommandsarray` on `command`.`idcommand` = "
      "`vrcommandsarray`.`idcommand` "
      "WHERE `command`.`idcommand` IN (SELECT `idcommand` "
      "FROM `applicationCommandsArray` "
      "WHERE `idApplication` = (SELECT `idApplication` "
      "FROM `application` WHERE `appID` = ? AND `deviceID` = ?)))  "
      "WHERE vrCommand IS NULL";
  // Check
  CheckSelectQuery(kSelectCommands, p1, p2, 1, 0);
  CheckSelectQuery(kSelectCommands, p1, p2, 1, 1);
  CheckSelectQuery(kSelectCommands, p1, p2, "menu_name", 2);
  CheckSelectQuery(kSelectCommands, p1, p2, 1, 3);
  CheckSelectQuery(kSelectCommands, p1, p2, 2, 4);
  CheckSelectQuery(kSelectCommands, p1, p2, "tst_image", 5);
  CheckSelectQuery(kSelectCommands, p1, p2, 2, 6);
  CheckSelectQuery(kChecksVrCommandFromSelectCommand, p1, p2, true, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCountSubscriptions_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  FillAppSubscriptionsArrayTable(temp_query, 2, 3, app_key);
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectCountSubscriptions, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectSubscriptions_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  FillAppSubscriptionsArrayTable(temp_query, 2, 3, app_key);
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectSubscriptions, p1, p2, 2, 0);
  CheckSelectQuery(kSelectSubscriptions, p1, p2, 3, 1);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCountChoiceSet_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  FillApplicationChoiceSetArrayTable(temp_query, 2, app_key);
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectCountChoiceSet, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectChoiceSets_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t app_key =
      FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                           hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                           app_id1, device_id, 9).LastInsertId();
  int64_t image_key = FillImageTable(temp_query, 2, "tst_image").LastInsertId();
  int64_t choice_key = FillChoiceTable(temp_query, 1, "menu_name", "sec_text",
                                       "tert_text", image_key).LastInsertId();
  int64_t app_choice_set_key =
      FillApplicationChoiceSetTable(temp_query, 23, 2).LastInsertId();

  FillChoiceArrayTable(temp_query, app_choice_set_key, choice_key);
  FillApplicationChoiceSetArrayTable(temp_query, app_choice_set_key, app_key);
  FillVRCommandsArrayTable(temp_query, "best", kVRCommandFromChoice,
                           choice_key);

  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Check
  CheckSelectQuery(kSelectChoiceSets, p1, p2, 1, 0);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, 23, 1);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, 2, 2);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, 1, 3);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, 1, 4);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, "menu_name", 5);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, "sec_text", 6);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, "tert_text", 7);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, 1, 8);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, 1, 9);
  CheckSelectQuery(kSelectChoiceSets, p1, p2, "best", 10);
}

TEST_F(ResumptionSqlQueriesTest, kSelectImage_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t image_key = FillImageTable(temp_query, 2, "tst_image").LastInsertId();
  // Check
  ValToPosPair p1(0, IntToString(image_key));
  ValToPosPair p2(1, "");
  CheckSelectQuery(kSelectImage, p1, p2, 2, 0);
  CheckSelectQuery(kSelectImage, p1, p2, "tst_image", 1);
}

TEST_F(ResumptionSqlQueriesTest,
       kSelectCountGlobalProperties_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t glob_prop_key =
      FillGlobalPropertiesTable(temp_query, "tst_vr_title", "tst_menu", 2, 3, 3,
                                "auto", 5).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, glob_prop_key).LastInsertId();

  // Check
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  CheckSelectQuery(kSelectCountGlobalProperties, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectGlobalProperties_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t image_key = FillImageTable(temp_query, 2, "tst_image").LastInsertId();
  int64_t glob_prop_key =
      FillGlobalPropertiesTable(temp_query, "tst_vr_title", "tst_menu", 2, 3, 3,
                                "auto", image_key).LastInsertId();
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, glob_prop_key).LastInsertId();
  FillHelpTimeoutPromptArrayTable(temp_query, glob_prop_key, 3, 7);

  // Check
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, glob_prop_key, 0);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, "tst_vr_title", 1);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, "tst_menu", 2);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, 1, 3);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, 2, 4);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, 3, 5);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, 3, 6);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, "auto", 7);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, 7, 8);
  CheckSelectQuery(kSelectGlobalProperties, p1, p2, 3, 9);
}

TEST_F(ResumptionSqlQueriesTest, kChecksVrHelpItem_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t image_key = FillImageTable(temp_query, 2, "tst_image").LastInsertId();
  int64_t glob_prop_key =
      FillGlobalPropertiesTable(temp_query, "tst_vr_title", "tst_menu", 2, 3, 3,
                                "auto", image_key).LastInsertId();
  int64_t vr_help_item_key =
      FillVRHelpItemTable(temp_query, "tst_text", 2, image_key).LastInsertId();
  FillVRHelpItemArrayTable(temp_query, glob_prop_key, vr_help_item_key);
  // Check
  ValToPosPair p1(0, IntToString(glob_prop_key));
  ValToPosPair p2(1, "");
  CheckSelectQuery(kChecksVrHelpItem, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectVrHelpItem_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t image_key = FillImageTable(temp_query, 2, "tst_image").LastInsertId();
  int64_t glob_prop_key =
      FillGlobalPropertiesTable(temp_query, "tst_vr_title", "tst_menu", 2, 3, 3,
                                "auto", image_key).LastInsertId();
  int64_t vr_help_item_key =
      FillVRHelpItemTable(temp_query, "tst_text", 2, image_key).LastInsertId();
  FillVRHelpItemArrayTable(temp_query, glob_prop_key, vr_help_item_key);
  // Check
  ValToPosPair p1(0, IntToString(glob_prop_key));
  ValToPosPair p2(1, "");
  CheckSelectQuery(kSelectVrHelpItem, p1, p2, "tst_text", 0);
  CheckSelectQuery(kSelectVrHelpItem, p1, p2, 2, 1);
  CheckSelectQuery(kSelectVrHelpItem, p1, p2, 2, 2);
  CheckSelectQuery(kSelectVrHelpItem, p1, p2, "tst_image", 3);
}

TEST_F(ResumptionSqlQueriesTest, kChecksCharacter_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t glob_prop_key =
      FillGlobalPropertiesTable(temp_query, "tst_vr_title", "tst_menu", 2, 3, 3,
                                "auto", 2).LastInsertId();
  string character_list = "abcdefghijkl";
  int64_t lim_char_list_key = FillTableLimitedCharacterListTable(
                                  temp_query, character_list).LastInsertId();
  FillCharacterArrayTable(temp_query, glob_prop_key, lim_char_list_key);
  // Check
  ValToPosPair p1(0, IntToString(glob_prop_key));
  ValToPosPair p2(1, "");
  CheckSelectQuery(kChecksCharacter, p1, p2, 1, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectCharacter_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t glob_prop_key =
      FillGlobalPropertiesTable(temp_query, "tst_vr_title", "tst_menu", 2, 3, 3,
                                "auto", 2).LastInsertId();
  string character_list = "abcdefghijkl";
  int64_t lim_char_list_key = FillTableLimitedCharacterListTable(
                                  temp_query, character_list).LastInsertId();
  FillCharacterArrayTable(temp_query, glob_prop_key, lim_char_list_key);
  // Check
  ValToPosPair p1(0, IntToString(glob_prop_key));
  ValToPosPair p2(1, "");
  CheckSelectQuery(kSelectCharacter, p1, p2, character_list, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectAllApps_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, 1);

  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id2, device_id2, 2);
  // Checks
  SQLQuery query(db());
  EXPECT_TRUE(query.Prepare(kSelectAllApps) && query.Exec());
  EXPECT_EQ(app_id1, query.GetString(0));
  EXPECT_EQ(device_id, query.GetString(1));
  EXPECT_TRUE(query.Next());
  EXPECT_EQ(app_id2, query.GetString(0));
  EXPECT_EQ(device_id2, query.GetString(1));
}

TEST_F(ResumptionSqlQueriesTest, kUpdateApplicationData_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, false,
                       app_id1, device_id, 1);
  string select_hmi_level_and_time_stamp =
      "SELECT `hmiLevel`, `timeStamp`FROM `application` "
      "WHERE `appID` = ? AND `deviceID` = ?;";

  // Checks before action
  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  CheckSelectQuery(select_hmi_level_and_time_stamp, p1, p2, hmiLevel, 0);
  CheckSelectQuery(select_hmi_level_and_time_stamp, p1, p2, timeStamp, 1);
  // Act
  SQLQuery query(db());
  EXPECT_TRUE(query.Prepare(kUpdateApplicationData));
  query.Bind(0, 2);
  query.Bind(1, 2016);
  query.Bind(2, app_id1);
  query.Bind(3, device_id);
  EXPECT_TRUE(query.Exec());
  // Checks after action
  CheckSelectQuery(select_hmi_level_and_time_stamp, p1, p2, 2, 0);
  CheckSelectQuery(select_hmi_level_and_time_stamp, p1, p2, 2016, 1);
}

TEST_F(ResumptionSqlQueriesTest,
       kUpdateDBVersion_kSelectDBVersion_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  EXPECT_TRUE(temp_query.Exec(kInsertInitData));
  // Checks before action
  CheckSelectQuery(kSelectDBVersion, 0, 0);
  // Act
  SQLQuery query(db());
  EXPECT_TRUE(query.Prepare(kUpdateDBVersion));
  query.Bind(0, 2);
  EXPECT_TRUE(query.Exec());
  // Checks after action
  CheckSelectQuery(kSelectDBVersion, 2, 0);
}

TEST_F(ResumptionSqlQueriesTest, kSelectTTSChunk_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  int64_t tts_chunk_key =
      FillTTSChunkTable(temp_query, 2, "tst_txt").LastInsertId();
  ValToPosPair p1(0, IntToString(tts_chunk_key));
  ValToPosPair p2(1, "");
  // Checks
  CheckSelectQuery(kSelectTTSChunk, p1, p2, "tst_txt", 0);
  CheckSelectQuery(kSelectTTSChunk, p1, p2, 2, 1);
}

TEST_F(ResumptionSqlQueriesTest, kSelectAppTable_ExpectDataCorrect) {
  // Arrange
  SQLQuery temp_query(db());
  FillApplicationTable(temp_query, connection_key, grammarID, test_hash,
                       hmiAppID, hmiLevel, ign_off_count, timeStamp, true,
                       app_id1, device_id, 1);

  ValToPosPair p1(0, app_id1);
  ValToPosPair p2(1, device_id);
  // Checks
  CheckSelectQuery(kSelectAppTable, p1, p2, app_id1, 0);
  CheckSelectQuery(kSelectAppTable, p1, p2, connection_key, 1);
  CheckSelectQuery(kSelectAppTable, p1, p2, grammarID, 2);
  CheckSelectQuery(kSelectAppTable, p1, p2, test_hash, 3);
  CheckSelectQuery(kSelectAppTable, p1, p2, hmiAppID, 4);
  CheckSelectQuery(kSelectAppTable, p1, p2, hmiLevel, 5);
  CheckSelectQuery(kSelectAppTable, p1, p2, ign_off_count, 6);
  CheckSelectQuery(kSelectAppTable, p1, p2, timeStamp, 7);
  CheckSelectQuery(kSelectAppTable, p1, p2, device_id, 8);
  CheckSelectQuery(kSelectAppTable, p1, p2, true, 9);
}

}  // namespace test
}  // namespace resumption

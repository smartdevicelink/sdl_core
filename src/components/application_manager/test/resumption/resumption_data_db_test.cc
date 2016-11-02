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
#include <algorithm>
#include "gtest/gtest.h"
#include "application_manager/mock_application.h"
#include "application_manager/mock_application_manager_settings.h"
#include "interfaces/MOBILE_API.h"
#include "utils/sql_database.h"
#include "utils/sqlite_wrapper/sql_query.h"
#include "utils/make_shared.h"
#include "utils/file_system.h"
#include "smart_objects/smart_object.h"
#include "application_manager/resumption_data_test.h"
#include "application_manager/test_resumption_data_db.h"

#include "application_manager/resumption/resumption_sql_queries.h"
#include "application_manager/resumption/resumption_data_db.h"

#include "utils/sqlite_wrapper/sql_database_impl.h"
#include "utils/mock_sql_database.h"
#include "application_manager/mock_application_manager_settings.h"

#if defined(_MSC_VER)
#define snprintf _snprintf_s
#endif

namespace test {
namespace components {
namespace resumption_test {

using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnNull;
using ::testing::ReturnRef;
using ::testing::_;
using application_manager_test::MockApplication;

using ::test::components::utils::dbms::MockSQLDatabase;

namespace am = application_manager;
using namespace file_system;

using namespace resumption;
using namespace mobile_apis;

namespace {
const std::string kPath =
    file_system::CurrentWorkingDirectory() + "/" + "test_storage";
}
class ResumptionDataDBTest : public ResumptionDataTest {
 protected:
  void SetUp() OVERRIDE {
    app_mock = ::utils::MakeShared<
        NiceMock<application_manager_test::MockApplication> >();
    policy_app_id_ = "test_policy_app_id";
    app_id_ = 10;
    is_audio_ = true;
    hash_ = "saved_hash";
    hmi_level_ = HMILevel::eType::HMI_FULL;
    hmi_app_id_ = 8;
    ign_off_count_ = 0;
    grammar_id_ = 16;
  }
  void TearDown() OVERRIDE {
    ::utils::dbms::SQLQuery query(test_db());
    EXPECT_TRUE(query.Prepare(remove_all_tables));
    EXPECT_TRUE(query.Exec());
  }

  static void SetUpTestCase() {
    if (is_in_file) {
      path_ = "test_storage";
      CreateDirectory("./" + path_);
      test_db_ = new ::utils::dbms::SQLDatabaseImpl(kDatabaseName);
      test_db_->set_path(path_ + "/");
      res_db_ = new TestResumptionDataDB(In_File_Storage);
    } else {
      res_db_ = new TestResumptionDataDB(In_Memory_Storage);
      test_db_ = res_db_->get_db_handle();
    }

    EXPECT_TRUE(test_db_->Open());
    EXPECT_TRUE(test_db_->IsReadWrite());
  }

  static ::utils::dbms::SQLDatabase* test_db_;
  static std::string kDatabaseName;
  static std::string path_;

  static void TearDownTestCase() {
    test_db_->Close();
    if (is_in_file) {
      RemoveDirectory("./" + path_, true);
    }
    delete res_db_;
  }

  ::utils::dbms::SQLDatabase* test_db() {
    return test_db_;
  }
  std::string path() {
    return path_;
  }

  void SetZeroIgnOffTime() {
    ::utils::dbms::SQLQuery query(test_db());
    EXPECT_TRUE(query.Prepare(resumption::KUpdateLastIgnOffTime));
    query.Bind(0, 0);
    EXPECT_TRUE(query.Exec());
  }

  static TestResumptionDataDB* res_db_;

  TestResumptionDataDB* res_db() {
    return res_db_;
  }

  // Check that db includes tables with given elements
  void CheckSavedDB();
  static const bool is_in_file = false;
  const std::string tables_exist =
      "SELECT COUNT(*) FROM sqlite_master WHERE `type` = 'table';";
  const std::string init_last_ign_count =
      "SELECT `last_ign_off_time` FROM resumption;";
  const std::string internal_data = "SELECT COUNT(*) FROM _internal_data;";

  const std::string remove_all_tables =
      "DELETE FROM `resumption`; "
      "DELETE FROM `subscribedForWayPoints`; "
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
      "DELETE FROM `_internal_data`; ";

 private:
  void CheckExistenceApplication();
  void CheckAppData();
  void CheckAppFilesData();
  void CheckSubmenuData();
  void CheckCommandsData();
  void CheckChoiceSetData();

  void CheckGlobalProportiesData();
  void CheckCharacters(int64_t global_properties_key);
  void CheckVRHelpItem(int64_t global_properties_key);

  void BindId(::utils::dbms::SQLQuery& query);
};

::utils::dbms::SQLDatabase* ResumptionDataDBTest::test_db_ = NULL;
TestResumptionDataDB* ResumptionDataDBTest::res_db_ = NULL;
std::string ResumptionDataDBTest::kDatabaseName = "";
std::string ResumptionDataDBTest::path_ = "";

void ResumptionDataDBTest::CheckSavedDB() {
  ::utils::dbms::SQLQuery query_checks(test_db());
  EXPECT_TRUE(query_checks.Prepare(kChecksResumptionData));
  EXPECT_TRUE(query_checks.Exec());
  EXPECT_EQ(1, query_checks.GetInteger(0));

  CheckExistenceApplication();
  CheckAppData();

  CheckAppFilesData();
  CheckSubmenuData();
  CheckCommandsData();

  CheckChoiceSetData();
  CheckGlobalProportiesData();
}

void ResumptionDataDBTest::CheckExistenceApplication() {
  ::utils::dbms::SQLQuery query(test_db());
  EXPECT_TRUE(query.Prepare(kCheckApplication));
  query.Bind(0, kMacAddress_);
  query.Bind(1, policy_app_id_);
  EXPECT_TRUE(query.Exec());
  EXPECT_EQ(1, query.GetInteger(0));
}

void ResumptionDataDBTest::CheckAppData() {
  ::utils::dbms::SQLQuery query(test_db());
  EXPECT_TRUE(query.Prepare(kSelectAppTable));
  BindId(query);
  EXPECT_TRUE(query.Exec());
  EXPECT_EQ(policy_app_id_, query.GetString(0));
  EXPECT_EQ(app_id_, query.GetUInteger(1));
  EXPECT_EQ(grammar_id_, query.GetUInteger(2));
  EXPECT_EQ(hash_, query.GetString(3));
  EXPECT_EQ(hmi_app_id_, query.GetUInteger(4));
  EXPECT_EQ(hmi_level_, query.GetInteger(5));

  EXPECT_EQ(ign_off_count_, query.GetUInteger(6));

  EXPECT_EQ(kMacAddress_, query.GetString(8));
  EXPECT_EQ(is_audio_, query.GetBoolean(9));
}

void ResumptionDataDBTest::CheckGlobalProportiesData() {
  ::utils::dbms::SQLQuery select_globalproperties(test_db());

  EXPECT_TRUE(select_globalproperties.Prepare(kSelectCountGlobalProperties));
  BindId(select_globalproperties);
  EXPECT_TRUE(select_globalproperties.Exec());
  EXPECT_EQ(1, select_globalproperties.GetInteger(0));

  EXPECT_TRUE(select_globalproperties.Prepare(kSelectGlobalProperties));
  BindId(select_globalproperties);

  size_t help_prompt_idx = 0;
  size_t timeout_prompt_idx = 0;
  int64_t global_properties_key = 0;
  while (select_globalproperties.Next()) {
    if (global_properties_key != select_globalproperties.GetLongInt(0)) {
      global_properties_key = select_globalproperties.GetLongInt(0);
      EXPECT_EQ((*vr_help_title_).asString(),
                select_globalproperties.GetString(1));
      EXPECT_EQ((*menu_title_).asString(),
                select_globalproperties.GetString(2));
      EXPECT_EQ((*keyboard_props_)[am::strings::language].asInt(),
                select_globalproperties.GetInteger(4));
      EXPECT_EQ((*keyboard_props_)[am::hmi_request::keyboard_layout].asInt(),
                select_globalproperties.GetInteger(5));
      EXPECT_EQ((*keyboard_props_)[am::strings::key_press_mode].asInt(),
                select_globalproperties.GetInteger(6));
      EXPECT_EQ((*keyboard_props_)[am::strings::auto_complete_text].asString(),
                select_globalproperties.GetString(7));

      EXPECT_FALSE(select_globalproperties.IsNull(3));
      ::utils::dbms::SQLQuery select_image(test_db());
      EXPECT_TRUE(select_image.Prepare(kSelectImage));
      select_image.Bind(0, select_globalproperties.GetLongInt(3));
      EXPECT_TRUE(select_image.Exec());
      EXPECT_EQ((*menu_icon_)[am::strings::image_type].asInt(),
                select_image.GetInteger(0));
      EXPECT_EQ((*menu_icon_)[am::strings::value].asString(),
                select_image.GetString(1));
    }
    if (!select_globalproperties.IsNull(8)) {
      ::utils::dbms::SQLQuery select_tts_chunk(test_db());
      EXPECT_TRUE(select_tts_chunk.Prepare(kSelectTTSChunk));
      select_tts_chunk.Bind(0, select_globalproperties.GetLongInt(8));
      EXPECT_TRUE(select_tts_chunk.Exec());

      std::string text =
          (*help_prompt_)[help_prompt_idx][am::strings::text].asString();
      int type = (*help_prompt_)[help_prompt_idx][am::strings::type].asInt();
      EXPECT_EQ(text, select_tts_chunk.GetString(0));
      EXPECT_EQ(type, select_tts_chunk.GetInteger(1));
      help_prompt_idx++;
    }
    if (!select_globalproperties.IsNull(9)) {
      ::utils::dbms::SQLQuery select_tts_chunk(test_db());
      EXPECT_TRUE(select_tts_chunk.Prepare(kSelectTTSChunk));
      select_tts_chunk.Bind(0, select_globalproperties.GetLongInt(9));
      EXPECT_TRUE(select_tts_chunk.Exec());

      std::string text =
          (*timeout_prompt_)[timeout_prompt_idx][am::strings::text].asString();
      int type =
          (*timeout_prompt_)[timeout_prompt_idx][am::strings::type].asInt();
      EXPECT_EQ(text, select_tts_chunk.GetString(0));
      EXPECT_EQ(type, select_tts_chunk.GetInteger(1));
      timeout_prompt_idx++;
    }
    CheckCharacters(global_properties_key);
    CheckVRHelpItem(global_properties_key);
  }
}
void ResumptionDataDBTest::CheckVRHelpItem(int64_t global_properties_key) {
  ::utils::dbms::SQLQuery checks_vrhelp_item(test_db());
  EXPECT_TRUE(checks_vrhelp_item.Prepare(kChecksVrHelpItem));
  checks_vrhelp_item.Bind(0, global_properties_key);
  EXPECT_TRUE(checks_vrhelp_item.Exec());
  EXPECT_NE(0, checks_vrhelp_item.GetInteger(0));
  if (!checks_vrhelp_item.GetInteger(0)) {
    ::utils::dbms::SQLQuery select_vrhelp_item(test_db());
    EXPECT_TRUE(select_vrhelp_item.Prepare(kSelectVrHelpItem));
    select_vrhelp_item.Bind(0, global_properties_key);
    size_t vr_help_item_idx = 0;
    while (select_vrhelp_item.Next()) {
      std::string vr_text =
          (*vr_help_)[vr_help_item_idx][am::strings::text].asString();
      std::string vr_position =
          (*vr_help_)[vr_help_item_idx++][am::strings::position].asString();
      EXPECT_EQ(vr_text, select_vrhelp_item.GetString(0));
      EXPECT_EQ(vr_position, select_vrhelp_item.GetString(1));
    }
  }
}

void ResumptionDataDBTest::CheckCharacters(int64_t global_properties_key) {
  ::utils::dbms::SQLQuery checks_characters(test_db());
  EXPECT_TRUE(checks_characters.Prepare(kChecksCharacter));
  checks_characters.Bind(0, global_properties_key);
  EXPECT_TRUE(checks_characters.Exec());
  EXPECT_NE(0, checks_characters.GetInteger(0));
  if (!checks_characters.GetInteger(0)) {
    ::utils::dbms::SQLQuery select_characters(test_db());
    EXPECT_TRUE(select_characters.Prepare(kSelectCharacter));
    select_characters.Bind(0, global_properties_key);
    size_t characters_idx = 0;
    while (select_characters.Next()) {
      std::string character =
          (*keyboard_props_)[am::strings::limited_character_list]
                            [characters_idx++].asString();
      EXPECT_EQ(character, select_characters.GetString(0));
    }
  }
}

void ResumptionDataDBTest::CheckSubmenuData() {
  ::utils::dbms::SQLQuery select_submenu(test_db());

  EXPECT_TRUE(select_submenu.Prepare(kSelectCountSubMenu));
  BindId(select_submenu);
  EXPECT_TRUE(select_submenu.Exec());
  EXPECT_EQ(kCountOfSubmenues_, select_submenu.GetUInteger(0));

  EXPECT_TRUE(select_submenu.Prepare(kSelectSubMenu));
  BindId(select_submenu);
  int i = 10;
  while (select_submenu.Next()) {
    uint32_t test_id = (*test_submenu_map[i])[am::strings::menu_id].asUInt();
    std::string name =
        (*test_submenu_map[i])[am::strings::menu_name].asString();
    int position = (*test_submenu_map[i])[am::strings::position].asInt();
    std::string image_value =
        (*test_submenu_map[i])[am::strings::sub_menu_icon][am::strings::value]
            .asString();
    int image_type = (*test_submenu_map[i])[am::strings::sub_menu_icon]
                                           [am::strings::image_type].asInt();
    EXPECT_EQ(test_id, select_submenu.GetUInteger(0));
    EXPECT_EQ(name, select_submenu.GetString(1));
    EXPECT_EQ(position, select_submenu.GetInteger(2));
    EXPECT_EQ(image_value, select_submenu.GetString(3));
    EXPECT_EQ(image_type, select_submenu.GetInteger(4));
    i++;
  }
}

void ResumptionDataDBTest::CheckCommandsData() {
  ::utils::dbms::SQLQuery select_commands(test_db());

  EXPECT_TRUE(select_commands.Prepare(kSelectCountCommands));
  BindId(select_commands);
  EXPECT_TRUE(select_commands.Exec());
  EXPECT_EQ(kCountOfCommands_, select_commands.GetUInteger(0));

  EXPECT_TRUE(select_commands.Prepare(kSelectCommands));
  BindId(select_commands);

  int32_t i = -1;
  int64_t command_key = 0;
  int j = 0;
  while (select_commands.Next()) {
    if (command_key != select_commands.GetLongInt(0)) {
      ++i;
      uint32_t cmd = (*test_commands_map[i])[am::strings::cmd_id].asUInt();
      EXPECT_EQ(cmd, select_commands.GetUInteger(1));
      std::string name =
          (*test_commands_map[i])[am::strings::menu_params]
                                 [am::strings::menu_name].asString();
      EXPECT_EQ(name, select_commands.GetString(2));
      int position = (*test_commands_map[i])[am::strings::menu_params]
                                            [am::strings::position].asInt();
      EXPECT_EQ(position, select_commands.GetInteger(4));
      int parent_id =
          (*test_commands_map[i])[am::strings::menu_params]
                                 [am::hmi_request::parent_id].asInt();
      EXPECT_EQ(parent_id, select_commands.GetInteger(3));
      std::string icon_name =
          (*test_commands_map[i])[am::strings::cmd_icon][am::strings::value]
              .asString();
      EXPECT_EQ(icon_name, select_commands.GetString(5));

      int icon_type = (*test_commands_map[i])[am::strings::cmd_icon]
                                             [am::strings::image_type].asInt();
      EXPECT_EQ(icon_type, select_commands.GetInteger(6));

      j = 0;
      command_key = select_commands.GetLongInt(0);
    }
    std::string vr =
        (*test_commands_map[i])[am::strings::vr_commands][j].asString();
    EXPECT_EQ(vr, select_commands.GetString(7));
    j++;
  }
}

void ResumptionDataDBTest::CheckChoiceSetData() {
  ::utils::dbms::SQLQuery select_choice_set(test_db());
  EXPECT_TRUE(select_choice_set.Prepare(kSelectCountChoiceSet));
  BindId(select_choice_set);
  EXPECT_TRUE(select_choice_set.Exec());
  EXPECT_EQ(kCountOfChoiceSets_, select_choice_set.GetUInteger(0));

  EXPECT_TRUE(select_choice_set.Prepare(kSelectChoiceSets));
  int64_t app_set_key = 0;
  int64_t choice_key = 0;
  int32_t choice_set_idx = -1;
  int32_t choice_idx = -1;
  size_t vr_cmd_idx = 0;
  sm::SmartObject command;
  while (select_choice_set.Next()) {
    if (app_set_key != select_choice_set.GetLongInt(0)) {
      command = (*test_choiceset_map[app_set_key]);
      ++choice_set_idx;

      int choice_set_id =
          command[am::strings::interaction_choice_set_id].asUInt();
      int grammar_id = command[am::strings::grammar_id].asUInt();

      EXPECT_EQ(grammar_id, select_choice_set.GetInteger(1));
      EXPECT_EQ(choice_set_id, select_choice_set.GetInteger(2));

      app_set_key = select_choice_set.GetLongInt(0);
      choice_idx = -1;
    }

    if (choice_key != select_choice_set.GetLongInt(3)) {
      ++choice_idx;
      choice_key = select_choice_set.GetLongInt(3);
      int choice_id =
          command[am::strings::choice_set][choice_idx][am::strings::choice_id]
              .asUInt();
      std::string menu_name =
          command[am::strings::choice_set][choice_idx][am::strings::menu_name]
              .asString();
      std::string secondary_text =
          command[am::strings::choice_set][choice_idx]
                 [am::strings::secondary_text].asString();
      std::string tertiary_text =
          command[am::strings::choice_set][choice_idx]
                 [am::strings::tertiary_text].asString();

      EXPECT_EQ(choice_id, select_choice_set.GetInteger(4));
      EXPECT_EQ(menu_name, select_choice_set.GetString(5));
      EXPECT_EQ(secondary_text, select_choice_set.GetString(6));
      EXPECT_EQ(tertiary_text, select_choice_set.GetString(7));

      EXPECT_FALSE(select_choice_set.IsNull(8));
      ::utils::dbms::SQLQuery select_image(test_db());
      EXPECT_TRUE(select_image.Prepare(kSelectImage));
      select_image.Bind(0, select_choice_set.GetLongInt(8));
      EXPECT_TRUE(select_image.Exec());
      std::string image_value =
          command[am::strings::choice_set][choice_idx][am::strings::image]
                 [am::strings::value].asString();
      int image_type =
          command[am::strings::choice_set][choice_idx][am::strings::image]
                 [am::strings::image_type].asInt();
      EXPECT_EQ(image_value, select_image.GetString(1));
      EXPECT_EQ(image_type, select_image.GetInteger(0));

      EXPECT_FALSE(select_choice_set.IsNull(9));
      EXPECT_TRUE(select_image.Prepare(kSelectImage));
      select_image.Bind(0, select_choice_set.GetLongInt(9));
      EXPECT_TRUE(select_image.Exec());
      image_value =
          command[am::strings::choice_set][choice_idx]
                 [am::strings::secondary_image][am::strings::value].asString();
      image_type =
          command[am::strings::choice_set][choice_idx]
                 [am::strings::secondary_image][am::strings::image_type]
                     .asInt();
      EXPECT_EQ(image_value, select_image.GetString(1));
      EXPECT_EQ(image_type, select_image.GetInteger(0));

      vr_cmd_idx = 0;
    }
    std::string vr_comm =
        command[am::strings::choice_set][choice_idx][am::strings::vr_commands]
               [vr_cmd_idx++].asString();
    EXPECT_EQ(vr_comm, select_choice_set.GetString(10));
  }
}

void ResumptionDataDBTest::CheckAppFilesData() {
  ::utils::dbms::SQLQuery query(test_db());
  EXPECT_TRUE(query.Prepare(kSelectCountFiles));
  BindId(query);
  EXPECT_TRUE(query.Exec());
  EXPECT_EQ(kCountOfFiles_, query.GetUInteger(0));

  EXPECT_TRUE(query.Prepare(kSelectFiles));
  BindId(query);
  am::AppFile check_file;
  int i = 0;
  while (query.Next()) {
    char numb[12];
    snprintf(numb, 12, "%d", i);
    check_file = app_files_map_["test_file " + std::string(numb)];

    EXPECT_EQ(check_file.file_type, query.GetInteger(0));
    EXPECT_EQ(check_file.is_download_complete, query.GetBoolean(1));
    EXPECT_EQ(check_file.is_persistent, query.GetBoolean(2));
    EXPECT_EQ(check_file.file_name, query.GetString(3));
    i++;
  }
}

void ResumptionDataDBTest::BindId(::utils::dbms::SQLQuery& query) {
  query.Bind(0, policy_app_id_);
  query.Bind(1, kMacAddress_);
}

class ResumptionDBTest_WithMockStorage : public ::testing::Test {
 public:
  ResumptionDBTest_WithMockStorage()
      // Mock database will be destroyed by resumption_data_db in ~Destr.
      : mock_database_(new MockSQLDatabase()),
        resumption_data_db_(mock_database_, mock_am_settings_) {}

  void SetUp() OVERRIDE {
    ON_CALL(mock_am_settings_, attempts_to_open_resumption_db())
        .WillByDefault(Return(1));
    ON_CALL(mock_am_settings_, open_attempt_timeout_ms_resumption_db())
        .WillByDefault(Return(1));
    ON_CALL(mock_am_, get_settings())
        .WillByDefault(ReturnRef(mock_am_settings_));
  }
  void TearDown() OVERRIDE {
    EXPECT_CALL(*mock_database_, Close());
  }

  void InitRealDB() {
    real_db_.Open();

    EXPECT_CALL(*mock_database_, Open()).WillOnce(Return(true));
    EXPECT_CALL(*mock_database_, IsReadWrite()).WillOnce(Return(true));
    EXPECT_CALL(*mock_database_, conn())
        .WillRepeatedly(Return(real_db_.conn()));

    EXPECT_TRUE(resumption_data_db_.Init());
  }

  void ApplicationParamsCreateCalls(MockApplication* mock_app) {
    EXPECT_CALL(*mock_app, curHash()).WillOnce(ReturnRef(kEmptyString_));
    EXPECT_CALL(*mock_app, get_grammar_id()).WillOnce(Return(1u));
    EXPECT_CALL(*mock_app, app_id()).WillRepeatedly(Return(1u));
    EXPECT_CALL(*mock_app, hmi_app_id()).WillOnce(Return(1u));
    EXPECT_CALL(*mock_app, IsAudioApplication()).WillOnce(Return(true));
    EXPECT_CALL(mock_am_, IsAppSubscribedForWayPoints(_))
        .WillRepeatedly(Return(false));
  }

  void PrepareApplicationSO(smart_objects::SmartObject& app_data) {
    app_data[ ::application_manager::strings::app_id] = "1";
    app_data[ ::application_manager::strings::hash_id] = "1";
    app_data[ ::application_manager::strings::grammar_id] = 1;
    app_data[ ::application_manager::strings::connection_key] = 1;
    app_data[ ::application_manager::strings::hmi_app_id] = 1;
    app_data[ ::application_manager::strings::hmi_level] = HMILevel::HMI_FULL;
    app_data[ ::application_manager::strings::is_media_application] = true;
    app_data[ ::application_manager::strings::subscribed_for_way_points] =
        false;
    app_data[ ::application_manager::strings::ign_off_count] = 0;
    app_data[ ::application_manager::strings::device_id] = "0";
  }

  void GetCorrectAppData(smart_objects::SmartObject& data) {
    data =
        smart_objects::SmartObject(smart_objects::SmartType::SmartType_Array);
    data[0] = smart_objects::SmartObject(smart_objects::SmartType_Map);

    smart_objects::SmartObject& application_data = data[0];

    application_data["globalProperties"] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    application_data["applicationFiles"] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    application_data["applicationSubMenus"] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    application_data["applicationCommands"] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
    application_data["subscribtions"] =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    application_data["applicationChoiceSets"] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);

    // Filling app_data with correct application values
    PrepareApplicationSO(application_data);
  }

  void TestIncorrectAdditionalAppDataForWriting(const std::string& data_key) {
    // Processing of Application_data needed real DB frow write
    InitRealDB();
    // Correct data smart object
    smart_objects::SmartObject data;
    GetCorrectAppData(data);
    smart_objects::SmartObject& application_data = data[0];

    application_data[data_key][0] = kEmptyString_;

    EXPECT_CALL(*mock_database_, BeginTransaction()).Times(1);
    EXPECT_CALL(*mock_database_, conn())
        // Two times return reall db  connection to correct writing
        // of app_data to DB
        .WillOnce(Return(real_db_.conn()))
        .WillOnce(Return(real_db_.conn()))
        // Null connection for unsuccessful processing of FilesData
        .WillOnce(ReturnNull());
    EXPECT_CALL(*mock_database_, RollbackTransaction()).Times(1);
    EXPECT_FALSE(resumption_data_db_.SaveAllData(data));
  }

  void AddApplicationsToDB(smart_objects::SmartObject& apps) {
    EXPECT_CALL(*mock_database_, BeginTransaction()).Times(1);
    EXPECT_CALL(*mock_database_, conn())
        .WillRepeatedly(Return(real_db_.conn()));
    EXPECT_CALL(*mock_database_, RollbackTransaction()).Times(0);
    EXPECT_CALL(*mock_database_, CommitTransaction()).Times(1);

    EXPECT_TRUE(resumption_data_db_.SaveAllData(apps));
  }

  MockSQLDatabase* mock_database_;
  NiceMock<application_manager_test::MockApplicationManager> mock_am_;
  NiceMock<application_manager_test::MockApplicationManagerSettings>
      mock_am_settings_;
  ResumptionDataDB resumption_data_db_;
  ::utils::dbms::SQLDatabaseImpl real_db_;
  const std::string kEmptyString_ = "";
};

TEST_F(ResumptionDBTest_WithMockStorage, Init_DBNotOpened_AttemptsOpened) {
  EXPECT_CALL(*mock_database_, Open())
      .WillOnce(Return(false))
      .WillOnce(Return(true));
  EXPECT_CALL(*mock_database_, IsReadWrite()).WillOnce(Return(false));

  EXPECT_FALSE(resumption_data_db_.Init());
}

TEST_F(ResumptionDBTest_WithMockStorage,
       Init_DBNotOpened_AttemptsNotOpenedToo) {
  EXPECT_CALL(*mock_database_, Open()).WillRepeatedly(Return(false));
  EXPECT_CALL(*mock_database_, IsReadWrite()).Times(0);

  EXPECT_FALSE(resumption_data_db_.Init());
}

TEST_F(ResumptionDBTest_WithMockStorage, Init_CannotCreateSchema) {
  EXPECT_CALL(*mock_database_, Open()).WillOnce(Return(true));
  EXPECT_CALL(*mock_database_, IsReadWrite()).WillOnce(Return(true));
  EXPECT_CALL(*mock_database_, conn()).WillOnce(ReturnNull());

  EXPECT_FALSE(resumption_data_db_.Init());
}

TEST_F(ResumptionDBTest_WithMockStorage, Init_CannotChecksResumptionData) {
  ::utils::dbms::SQLDatabaseImpl real_db;
  real_db.Open();

  EXPECT_CALL(*mock_database_, Open()).WillOnce(Return(true));
  EXPECT_CALL(*mock_database_, IsReadWrite()).WillOnce(Return(true));
  EXPECT_CALL(*mock_database_, conn())
      .WillOnce(Return(real_db.conn()))
      .WillOnce(ReturnNull());

  EXPECT_FALSE(resumption_data_db_.Init());
}

TEST_F(ResumptionDBTest_WithMockStorage, Init_CannotInsertInitData) {
  ::utils::dbms::SQLDatabaseImpl real_db;
  real_db.Open();

  EXPECT_CALL(*mock_database_, Open()).WillOnce(Return(true));
  EXPECT_CALL(*mock_database_, IsReadWrite()).WillOnce(Return(true));
  EXPECT_CALL(*mock_database_, conn())
      .WillOnce(Return(real_db.conn()))
      .WillOnce(Return(real_db.conn()))
      .WillOnce(ReturnNull());

  EXPECT_FALSE(resumption_data_db_.Init());
}

TEST_F(ResumptionDBTest_WithMockStorage, Init_Positive) {
  ::utils::dbms::SQLDatabaseImpl real_db;
  real_db.Open();

  EXPECT_CALL(*mock_database_, Open()).WillOnce(Return(true));
  EXPECT_CALL(*mock_database_, IsReadWrite()).WillOnce(Return(true));
  EXPECT_CALL(*mock_database_, conn()).WillRepeatedly(Return(real_db.conn()));

  EXPECT_TRUE(resumption_data_db_.Init());
}

TEST_F(ResumptionDBTest_WithMockStorage, RefreshDB_CannotDropSchema) {
  EXPECT_CALL(*mock_database_, conn()).WillOnce(ReturnNull());

  EXPECT_FALSE(resumption_data_db_.RefreshDB());
}

TEST_F(ResumptionDBTest_WithMockStorage, RefreshDB_CannotCreateSchema) {
  ::utils::dbms::SQLDatabaseImpl real_db;
  real_db.Open();

  EXPECT_CALL(*mock_database_, conn())
      .WillOnce(Return(real_db.conn()))
      .WillOnce(ReturnNull());

  EXPECT_FALSE(resumption_data_db_.RefreshDB());
}

TEST_F(ResumptionDBTest_WithMockStorage, RefreshDB_CannotInsertInitData) {
  ::utils::dbms::SQLDatabaseImpl real_db;
  real_db.Open();

  EXPECT_CALL(*mock_database_, conn())
      .WillOnce(Return(real_db.conn()))
      .WillOnce(Return(real_db.conn()))
      .WillOnce(ReturnNull());

  EXPECT_FALSE(resumption_data_db_.RefreshDB());
}

TEST_F(ResumptionDBTest_WithMockStorage, RefreshDB_Positive) {
  ::utils::dbms::SQLDatabaseImpl real_db;
  real_db.Open();

  EXPECT_CALL(*mock_database_, conn()).WillRepeatedly(Return(real_db.conn()));

  EXPECT_TRUE(resumption_data_db_.RefreshDB());
}

TEST_F(ResumptionDBTest_WithMockStorage,
       IsDBVersionActual_CannotSelectDBVersion) {
  EXPECT_CALL(*mock_database_, conn()).WillOnce(ReturnNull());

  EXPECT_FALSE(resumption_data_db_.IsDBVersionActual());
}

TEST_F(ResumptionDBTest_WithMockStorage, IsDBVersionActual_Positive) {
  ::utils::dbms::SQLDatabaseImpl real_db;
  real_db.Open();

  EXPECT_CALL(*mock_database_, conn()).WillOnce(Return(real_db.conn()));

  EXPECT_FALSE(resumption_data_db_.IsDBVersionActual());
}

TEST_F(ResumptionDBTest_WithMockStorage, UpdateDBVersion_CannotPrepare) {
  EXPECT_CALL(*mock_database_, conn()).WillOnce(ReturnNull());

  EXPECT_FALSE(resumption_data_db_.UpdateDBVersion());
}

TEST_F(ResumptionDBTest_WithMockStorage, UpdateDBVersion_Positive) {
  ::utils::dbms::SQLDatabaseImpl real_db;
  real_db.Open();

  EXPECT_CALL(*mock_database_, conn()).WillOnce(Return(real_db.conn()));

  EXPECT_FALSE(resumption_data_db_.UpdateDBVersion());
}

TEST_F(ResumptionDBTest_WithMockStorage,
       SaveApplication_AppNotChanged_AppExist) {
  InitRealDB();
  MockApplication* app_ptr = new MockApplication();
  application_manager::ApplicationSharedPtr app_sptr(app_ptr);

  const std::string kPolicyAppId = "1";
  const std::string kMacAdress = "xx";

  ApplicationParamsCreateCalls(app_ptr);

  EXPECT_CALL(*app_ptr, policy_app_id()).WillRepeatedly(Return(kPolicyAppId));
  EXPECT_CALL(*app_ptr, mac_address()).WillRepeatedly(ReturnRef(kMacAdress));
  EXPECT_CALL(*app_ptr, is_application_data_changed())
      .WillRepeatedly(Return(false));
  EXPECT_CALL(*app_ptr, hmi_level()).WillRepeatedly(Return(HMILevel::HMI_FULL));
  EXPECT_CALL(*mock_database_, conn()).WillRepeatedly(Return(real_db_.conn()));
  EXPECT_CALL(*mock_database_, Backup()).Times(2);

  // App_not exists -> adding it to db
  resumption_data_db_.SaveApplication(app_sptr);
  // App exists update it in DB
  resumption_data_db_.SaveApplication(app_sptr);
}

TEST_F(ResumptionDBTest_WithMockStorage,
       SaveApplication_AppNotChanged_AppExist_DBProblem) {
  InitRealDB();
  MockApplication* app_ptr = new MockApplication();
  application_manager::ApplicationSharedPtr app_sptr(app_ptr);

  const std::string kPolicyAppId = "1";
  const std::string kMacAdress = "xx";

  ApplicationParamsCreateCalls(app_ptr);

  EXPECT_CALL(*app_ptr, policy_app_id()).WillRepeatedly(Return(kPolicyAppId));
  EXPECT_CALL(*app_ptr, mac_address()).WillRepeatedly(ReturnRef(kMacAdress));
  EXPECT_CALL(*app_ptr, is_application_data_changed())
      .WillRepeatedly(Return(false));
  EXPECT_CALL(*app_ptr, hmi_level()).WillRepeatedly(Return(HMILevel::HMI_FULL));
  EXPECT_CALL(*app_ptr, IsAppSubscribedForWayPoints(_))
      .WillRepeatedly(Return(true));
  EXPECT_CALL(*mock_database_, conn())
      .WillOnce(Return(real_db_.conn()))
      .WillOnce(Return(real_db_.conn()))
      .WillOnce(Return(real_db_.conn()))
      .WillOnce(ReturnNull());
  EXPECT_CALL(*mock_database_, Backup()).Times(1);

  // App_not exists -> adding it to db
  resumption_data_db_.SaveApplication(app_sptr);
  // App exists update it in DB
  resumption_data_db_.SaveApplication(app_sptr);
}

TEST_F(ResumptionDBTest_WithMockStorage, SaveAllData_DataNotArray) {
  smart_objects::SmartObject data(smart_objects::SmartType::SmartType_Map);

  EXPECT_CALL(*mock_database_, BeginTransaction()).Times(0);

  EXPECT_FALSE(resumption_data_db_.SaveAllData(data));
}

TEST_F(ResumptionDBTest_WithMockStorage,
       SaveAllData_IncorrectGlobalProperties) {
  // Correct smart type
  smart_objects::SmartObject data(smart_objects::SmartType::SmartType_Array);
  data[0] = smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject& application_data = data[0];

  application_data["globalProperties"]["property_1"] = 1u;

  EXPECT_CALL(*mock_database_, BeginTransaction()).Times(1);
  EXPECT_CALL(*mock_database_, RollbackTransaction()).Times(1);
  // Will create error while processing globalProperties
  EXPECT_CALL(*mock_database_, conn()).WillOnce(ReturnNull());

  EXPECT_FALSE(resumption_data_db_.SaveAllData(data));
}

TEST_F(ResumptionDBTest_WithMockStorage, SaveAllData_IncorrectApplicationData) {
  // Correct smart type
  smart_objects::SmartObject data(smart_objects::SmartType::SmartType_Array);
  data[0] = smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject& application_data = data[0];
  // When global properties empty processing returns true
  application_data["globalProperties"] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  // App data havent any fields for application (app_id, hmi_level)

  EXPECT_CALL(*mock_database_, BeginTransaction()).Times(1);
  EXPECT_CALL(*mock_database_, RollbackTransaction()).Times(1);

  EXPECT_FALSE(resumption_data_db_.SaveAllData(data));
}

TEST_F(ResumptionDBTest_WithMockStorage, SaveAllData_IncorrectFilesData) {
  TestIncorrectAdditionalAppDataForWriting("applicationFiles");
}

TEST_F(ResumptionDBTest_WithMockStorage, SaveAllData_IncorrectSubMenu) {
  TestIncorrectAdditionalAppDataForWriting("applicationSubMenus");
}

TEST_F(ResumptionDBTest_WithMockStorage, SaveAllData_IncorrectCommandsData) {
  TestIncorrectAdditionalAppDataForWriting("applicationCommands");
}

TEST_F(ResumptionDBTest_WithMockStorage,
       SaveAllData_IncorrectSubscriptionsData) {
  TestIncorrectAdditionalAppDataForWriting("subscribtions");
}

TEST_F(ResumptionDBTest_WithMockStorage, SaveAllData_IncorrectChoiceSetData) {
  TestIncorrectAdditionalAppDataForWriting("applicationChoiceSets");
}

TEST_F(ResumptionDBTest_WithMockStorage, SaveAllData_Positive) {
  // Processing of Application_data needed real DB frow write
  InitRealDB();
  // Correct smart type
  smart_objects::SmartObject data;
  GetCorrectAppData(data);

  AddApplicationsToDB(data);
}

TEST_F(ResumptionDBTest_WithMockStorage, GetAllData_DBProblem) {
  InitRealDB();
  EXPECT_CALL(*mock_database_, conn()).WillOnce(ReturnNull());

  smart_objects::SmartObject data;
  resumption_data_db_.GetAllData(data);
}

TEST_F(ResumptionDBTest_WithMockStorage, GetAllData_Positive) {
  InitRealDB();
  smart_objects::SmartObject input_data;
  GetCorrectAppData(input_data);
  AddApplicationsToDB(input_data);

  smart_objects::SmartObject output_data;
  EXPECT_TRUE(resumption_data_db_.GetAllData(output_data));

  // Timestamp added with writing to DB
  output_data[0].erase("timeStamp");

  EXPECT_TRUE(input_data == output_data);
}

TEST_F(ResumptionDataDBTest, Init) {
  ::utils::dbms::SQLQuery query_checks(test_db());

  EXPECT_TRUE(res_db()->Init());

  EXPECT_TRUE(query_checks.Prepare(tables_exist));
  EXPECT_TRUE(query_checks.Exec());
  EXPECT_NE(0, query_checks.GetInteger(0)) << query_checks.GetInteger(0);

  EXPECT_TRUE(query_checks.Prepare(kChecksResumptionData));
  EXPECT_TRUE(query_checks.Exec());
  EXPECT_NE(0, query_checks.GetInteger(0));

  EXPECT_TRUE(query_checks.Prepare(init_last_ign_count));
  EXPECT_TRUE(query_checks.Exec());
  EXPECT_EQ(0, query_checks.GetInteger(0));

  EXPECT_TRUE(query_checks.Prepare(internal_data));
  EXPECT_TRUE(query_checks.Exec());
  EXPECT_LE(0, query_checks.GetInteger(0));
}

TEST_F(ResumptionDataDBTest, SaveApplication) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
}

TEST_F(ResumptionDataDBTest, RemoveApplicationFromSaved) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  EXPECT_TRUE(
      res_db()->RemoveApplicationFromSaved(policy_app_id_, kMacAddress_));

  sm::SmartObject remove_app;
  EXPECT_FALSE(
      res_db()->GetSavedApplication(policy_app_id_, kMacAddress_, remove_app));
  EXPECT_TRUE(remove_app.empty());
}

TEST_F(ResumptionDataDBTest, RemoveApplicationFromSaved_AppNotSaved) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);

  sm::SmartObject saved_app;
  EXPECT_FALSE(
      res_db()->GetSavedApplication(policy_app_id_, "54321", saved_app));
  EXPECT_TRUE(saved_app.empty());
}

TEST_F(ResumptionDataDBTest, SavedApplicationTwice) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
}

TEST_F(ResumptionDataDBTest, SavedApplicationTwice_UpdateApp) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  (*vr_help_)[0][am::strings::position] = 2;

  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
}

TEST_F(ResumptionDataDBTest, IsApplicationSaved_ApplicationSaved) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  ssize_t result = res_db()->IsApplicationSaved(policy_app_id_, kMacAddress_);
  EXPECT_EQ(0, result);
}

TEST_F(ResumptionDataDBTest, IsApplicationSaved_ApplicationRemoved) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  EXPECT_TRUE(
      res_db()->RemoveApplicationFromSaved(policy_app_id_, kMacAddress_));
  ssize_t result = res_db()->IsApplicationSaved(policy_app_id_, kMacAddress_);
  EXPECT_EQ(-1, result);
}

TEST_F(ResumptionDataDBTest, GetSavedApplication) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();

  sm::SmartObject saved_app;
  EXPECT_TRUE(
      res_db()->GetSavedApplication(policy_app_id_, kMacAddress_, saved_app));
  CheckSavedApp(saved_app);
}

TEST_F(ResumptionDataDBTest, GetSavedApplication_AppNotSaved) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  sm::SmartObject saved_app;
  EXPECT_FALSE(
      res_db()->GetSavedApplication(policy_app_id_, "54321", saved_app));
  EXPECT_TRUE(saved_app.empty());
}

TEST_F(ResumptionDataDBTest, GetDataForLoadResumeData) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  sm::SmartObject saved_app;
  res_db()->GetDataForLoadResumeData(saved_app);

  EXPECT_EQ(policy_app_id_, saved_app[0][am::strings::app_id].asString());
  EXPECT_EQ(kMacAddress_, saved_app[0][am::strings::device_id].asString());
  EXPECT_EQ(hmi_level_,
            static_cast<HMILevel::eType>(
                saved_app[0][am::strings::hmi_level].asInt()));
  EXPECT_EQ(ign_off_count_, saved_app[0][am::strings::ign_off_count].asUInt());
}

TEST_F(ResumptionDataDBTest, GetDataForLoadResumeData_AppRemove) {
  sm::SmartObject saved_app;

  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  EXPECT_TRUE(
      res_db()->RemoveApplicationFromSaved(policy_app_id_, kMacAddress_));
  res_db()->GetDataForLoadResumeData(saved_app);
  EXPECT_TRUE(saved_app.empty());
}

TEST_F(ResumptionDataDBTest, UpdateHmiLevel) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  HMILevel::eType new_hmi_level = HMILevel::HMI_LIMITED;
  res_db()->UpdateHmiLevel(policy_app_id_, kMacAddress_, new_hmi_level);
  hmi_level_ = new_hmi_level;
  CheckSavedDB();
}

TEST_F(ResumptionDataDBTest, IsHMIApplicationIdExist_AppIsSaved) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  EXPECT_TRUE(res_db()->IsHMIApplicationIdExist(hmi_app_id_));
}

TEST_F(ResumptionDataDBTest, IsHMIApplicationIdExist_AppNotSaved) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  uint32_t new_hmi_app_id_ = hmi_app_id_ + 10;
  EXPECT_FALSE(res_db()->IsHMIApplicationIdExist(new_hmi_app_id_));
}

TEST_F(ResumptionDataDBTest, GetHMIApplicationID) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  EXPECT_EQ(hmi_app_id_,
            res_db()->GetHMIApplicationID(policy_app_id_, kMacAddress_));
}

TEST_F(ResumptionDataDBTest, GetHMIApplicationID_AppNotSaved) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();
  EXPECT_EQ(0u, res_db()->GetHMIApplicationID(policy_app_id_, "other_dev_id"));
}

TEST_F(ResumptionDataDBTest, OnSuspend) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  SetZeroIgnOffTime();
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();

  res_db()->OnSuspend();
  ign_off_count_++;
  CheckSavedDB();
}

TEST_F(ResumptionDataDBTest, OnSuspendFourTimes) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  SetZeroIgnOffTime();
  res_db()->SaveApplication(app_mock);
  CheckSavedDB();

  res_db()->OnSuspend();
  ign_off_count_++;
  CheckSavedDB();

  res_db()->OnSuspend();
  ign_off_count_++;
  CheckSavedDB();
  res_db()->OnSuspend();
  ign_off_count_++;
  CheckSavedDB();

  res_db()->OnSuspend();

  ssize_t result = res_db()->IsApplicationSaved(policy_app_id_, kMacAddress_);
  EXPECT_EQ(-1, result);
}

TEST_F(ResumptionDataDBTest, OnSuspendOnAwake) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  SetZeroIgnOffTime();

  res_db()->SaveApplication(app_mock);
  CheckSavedDB();

  res_db()->OnSuspend();

  ign_off_count_++;
  CheckSavedDB();
  res_db()->OnAwake();
  ign_off_count_ = 0;
  CheckSavedDB();
}

TEST_F(ResumptionDataDBTest, Awake_AppNotSuspended) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  SetZeroIgnOffTime();

  res_db()->SaveApplication(app_mock);
  CheckSavedDB();

  res_db()->OnAwake();
  ign_off_count_ = 0;
  CheckSavedDB();
}

TEST_F(ResumptionDataDBTest, TwiceAwake_AppNotSuspended) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  SetZeroIgnOffTime();

  res_db()->SaveApplication(app_mock);
  CheckSavedDB();

  res_db()->OnSuspend();
  res_db()->OnAwake();
  ign_off_count_ = 0;
  CheckSavedDB();

  res_db()->OnAwake();
  CheckSavedDB();
}

TEST_F(ResumptionDataDBTest, GetHashId) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());

  res_db()->SaveApplication(app_mock);

  std::string test_hash;
  EXPECT_TRUE(res_db()->GetHashId(policy_app_id_, kMacAddress_, test_hash));
  EXPECT_EQ(hash_, test_hash);
}

TEST_F(ResumptionDataDBTest, GetIgnOffTime_AfterSuspendAndAwake) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  SetZeroIgnOffTime();
  uint32_t last_ign_off_time;

  res_db()->SaveApplication(app_mock);

  last_ign_off_time = res_db()->GetIgnOffTime();
  EXPECT_EQ(0u, last_ign_off_time);

  res_db()->OnSuspend();

  uint32_t after_suspend;
  after_suspend = res_db()->GetIgnOffTime();
  EXPECT_LE(last_ign_off_time, after_suspend);

  uint32_t after_awake;
  res_db()->OnAwake();

  after_awake = res_db()->GetIgnOffTime();
  EXPECT_LE(after_suspend, after_awake);
}

TEST_F(ResumptionDataDBTest, DropAppResumptionData) {
  PrepareData();
  EXPECT_TRUE(res_db()->Init());
  SetZeroIgnOffTime();

  res_db()->SaveApplication(app_mock);

  EXPECT_TRUE(res_db()->DropAppDataResumption(kMacAddress_, policy_app_id_));

  ::smart_objects::SmartObject app;
  EXPECT_TRUE(res_db()->GetSavedApplication(policy_app_id_, kMacAddress_, app));

  EXPECT_TRUE(app.keyExists(am::strings::application_commands) &&
              app[am::strings::application_commands].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_submenus) &&
              app[am::strings::application_submenus].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_choice_sets) &&
              app[am::strings::application_choice_sets].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_global_properties) &&
              app[am::strings::application_global_properties].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_subscribtions) &&
              app[am::strings::application_subscribtions].empty());

  EXPECT_TRUE(app.keyExists(am::strings::application_files) &&
              app[am::strings::application_files].empty());

  EXPECT_FALSE(app.keyExists(am::strings::grammar_id));
}

}  // namespace resumption_test
}  // namespace components
}  // namespace test

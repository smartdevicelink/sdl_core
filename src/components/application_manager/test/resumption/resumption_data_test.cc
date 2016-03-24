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

#include <string>
#include <algorithm>
#include "gtest/gtest.h"

#include "application_manager/usage_statistics.h"
#include "include/resumption_data_mock.h"
#include "utils/custom_string.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application.h"
#include "utils/data_accessor.h"
#include "application_manager/message_helper.h"

#include "include/resumption_data_test.h"

namespace test {
namespace components {
namespace resumption_test {
namespace custom_str = utils::custom_string;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::ReturnPointee;

void ResumptionDataTest::CheckSavedApp(sm::SmartObject& resume_app_list) {
  EXPECT_EQ(mobile_app_id_, resume_app_list[am::strings::app_id].asString());
  EXPECT_EQ(grammar_id_, resume_app_list[am::strings::grammar_id].asUInt());
  EXPECT_EQ(app_id_, resume_app_list[am::strings::connection_key].asUInt());
  EXPECT_EQ(hmi_app_id_, resume_app_list[am::strings::hmi_app_id].asUInt());
  EXPECT_EQ(ign_off_count_,
            resume_app_list[am::strings::ign_off_count].asUInt());
  EXPECT_EQ(hmi_level_,
            static_cast<HMILevel::eType>(
                resume_app_list[am::strings::hmi_level].asInt()));
  EXPECT_EQ(is_audio_,
            resume_app_list[am::strings::is_media_application].asBool());
  EXPECT_EQ("12345", resume_app_list[am::strings::device_id].asString());

  CheckCommands(resume_app_list[am::strings::application_commands]);
  CheckSubmenues(resume_app_list[am::strings::application_submenus]);
  CheckChoiceSet(resume_app_list[am::strings::application_choice_sets]);
  CheckAppFiles(resume_app_list[am::strings::application_files]);

  CheckGlobalProporties(
      resume_app_list[am::strings::application_global_properties]);
  CheckSubscriptions(resume_app_list[am::strings::application_subscribtions]);
}

void ResumptionDataTest::CheckCommands(sm::SmartObject& res_list) {
  for (uint32_t i = 0; i < count_of_commands; ++i) {
    EXPECT_EQ(i, res_list[i][am::strings::cmd_id].asUInt());
    std::string name = (*test_commands_map[i])[am::strings::menu_params]
                                              [am::strings::menu_name]
                                                  .asString();
    EXPECT_EQ(name,
              res_list[i][am::strings::menu_params][am::strings::menu_name]
                  .asString());
    int position =
        (*test_commands_map[i])[am::strings::menu_params][am::strings::position]
            .asInt();
    EXPECT_EQ(
        position,
        res_list[i][am::strings::menu_params][am::strings::position].asInt());

    int parent_id = (*test_commands_map[i])[am::strings::menu_params]
                                           [am::hmi_request::parent_id]
                                               .asInt();
    EXPECT_EQ(parent_id,
              res_list[i][am::strings::menu_params][am::hmi_request::parent_id]
                  .asInt());

    std::string icon_name =
        (*test_commands_map[i])[am::strings::cmd_icon][am::strings::value]
            .asString();
    EXPECT_EQ(
        icon_name,
        res_list[i][am::strings::cmd_icon][am::strings::value].asString());

    int icon_type =
        (*test_commands_map[i])[am::strings::cmd_icon][am::strings::image_type]
            .asInt();
    EXPECT_EQ(
        icon_type,
        res_list[i][am::strings::cmd_icon][am::strings::image_type].asInt());

    for (uint32_t j = 0; j < count_of_choice; ++j) {
      std::string vr =
          (*test_commands_map[i])[am::strings::vr_commands][j].asString();
      EXPECT_EQ(vr, res_list[i][am::strings::vr_commands][j].asString());
    }
  }
}

void ResumptionDataTest::CheckSubmenues(sm::SmartObject& res_list) {
  for (uint32_t i = 0; i < count_of_submenues; ++i) {
    uint32_t test_id =
        (*test_submenu_map[i + 10])[am::strings::menu_id].asUInt();
    std::string name =
        (*test_submenu_map[i + 10])[am::strings::menu_name].asString();
    int position = (*test_submenu_map[i + 10])[am::strings::position].asInt();
    EXPECT_EQ(position, res_list[i][am::strings::position].asInt());
    EXPECT_EQ(test_id, res_list[i][am::strings::menu_id].asUInt());
    EXPECT_EQ(name, res_list[i][am::strings::menu_name].asString());
  }
}

void ResumptionDataTest::CheckSubscriptions(sm::SmartObject& res_list) {
  EXPECT_EQ(static_cast<uint32_t>(ButtonName::eType::OK),
            res_list[am::strings::application_buttons][0].asUInt());
  EXPECT_EQ(static_cast<uint32_t>(ButtonName::eType::CUSTOM_BUTTON),
            res_list[am::strings::application_buttons][1].asUInt());
  EXPECT_EQ(0u, res_list[am::strings::application_vehicle_info][0].asUInt());
  EXPECT_EQ(5u, res_list[am::strings::application_vehicle_info][1].asUInt());
}

void ResumptionDataTest::CheckChoiceSet(sm::SmartObject& res_list) {
  for (uint32_t i = 0; i < res_list.length(); ++i) {
    for (uint32_t j = 0; j < res_list[i][am::strings::choice_set].length();
         ++j) {
      sm::SmartObject command = res_list[i][am::strings::choice_set][j];
      EXPECT_EQ(i + j, command[am::strings::choice_id].asUInt());
      char numb[12];
      std::snprintf(numb, 12, "%d", i + j);
      std::string test_choice =
          (*test_choiceset_map[i])[am::strings::choice_set][j]
                                  [am::strings::vr_commands][0]
                                      .asString();
      EXPECT_EQ(test_choice, command[am::strings::vr_commands][0].asString());
      std::string menu_name =
          (*test_choiceset_map[i])[am::strings::choice_set][j]
                                  [am::strings::menu_name]
                                      .asString();
      EXPECT_EQ(menu_name, command[am::strings::menu_name].asString());
      std::string secondary_text =
          (*test_choiceset_map[i])[am::strings::choice_set][j]
                                  [am::strings::secondary_text]
                                      .asString();
      EXPECT_EQ(secondary_text,
                command[am::strings::secondary_text].asString());
      std::string tertiary_text =
          (*test_choiceset_map[i])[am::strings::choice_set][j]
                                  [am::strings::tertiary_text]
                                      .asString();
      EXPECT_EQ(tertiary_text, command[am::strings::tertiary_text].asString());

      std::string image_value =
          (*test_choiceset_map[i])[am::strings::choice_set][j]
                                  [am::strings::image][am::strings::value]
                                      .asString();
      EXPECT_EQ(image_value,
                command[am::strings::image][am::strings::value].asString());
      int image_type =
          (*test_choiceset_map[i])[am::strings::choice_set][j]
                                  [am::strings::image][am::strings::image_type]
                                      .asInt();
      EXPECT_EQ(image_type,
                command[am::strings::image][am::strings::image_type].asInt());

      image_value = (*test_choiceset_map[i])[am::strings::choice_set][j]
                                            [am::strings::secondary_image]
                                            [am::strings::value]
                                                .asString();
      EXPECT_EQ(
          image_value,
          command[am::strings::secondary_image][am::strings::value].asString());
      image_type = (*test_choiceset_map[i])[am::strings::choice_set][j]
                                           [am::strings::secondary_image]
                                           [am::strings::image_type]
                                               .asInt();
      EXPECT_EQ(image_type,
                command[am::strings::secondary_image][am::strings::image_type]
                    .asInt());
    }

    int choice_set_id =
        (*test_choiceset_map[i])[am::strings::interaction_choice_set_id]
            .asUInt();
    EXPECT_EQ(choice_set_id,
              res_list[i][am::strings::interaction_choice_set_id].asInt());

    int grammar_id = (*test_choiceset_map[i])[am::strings::grammar_id].asUInt();
    EXPECT_EQ(grammar_id, res_list[i][am::strings::grammar_id].asInt());
  }
}

void ResumptionDataTest::CheckAppFiles(sm::SmartObject& res_list) {
  am::AppFile check_file;

  for (uint i = 0; i < count_of_files; ++i) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    check_file = app_files_map_["test_file " + std::string(numb)];
    EXPECT_EQ(check_file.file_name,
              res_list[i][am::strings::sync_file_name].asString());
    EXPECT_EQ(check_file.file_type,
              static_cast<FileType::eType>(
                  res_list[i][am::strings::file_type].asInt()));
    EXPECT_EQ(check_file.is_download_complete,
              res_list[i][am::strings::is_download_complete].asBool());
    EXPECT_EQ(check_file.is_persistent,
              res_list[i][am::strings::persistent_file].asBool());
  }
}

void ResumptionDataTest::CheckGlobalProporties(sm::SmartObject& res_list) {
  CheckHelpPrompt(res_list[am::strings::help_prompt]);
  CheckTimeoutPrompt(res_list[am::strings::timeout_prompt]);
  CheckVRHelp(res_list[am::strings::vr_help]);
  CheckVRTitle(res_list[am::strings::vr_help_title]);
  CheckKeyboardProperties(res_list[am::strings::keyboard_properties]);
  CheckMenuTitle(res_list[am::strings::menu_title]);
  CheckMenuIcon(res_list[am::strings::menu_icon]);
}

void ResumptionDataTest::CheckKeyboardProperties(sm::SmartObject& res_list) {
  Language::eType testlanguage = static_cast<Language::eType>(
      (*keyboard_props_)[am::strings::language].asInt());
  KeyboardLayout::eType testlayout = static_cast<KeyboardLayout::eType>(
      (*keyboard_props_)[am::hmi_request::keyboard_layout].asInt());
  KeypressMode::eType testmode = static_cast<KeypressMode::eType>(
      (*keyboard_props_)[am::strings::key_press_mode].asInt());

  std::string auto_complete_text =
      (*keyboard_props_)[am::strings::auto_complete_text].asString();

  for (uint i = 0;
       i < (*keyboard_props_)[am::strings::limited_character_list].length();
       i++) {
    std::string character =
        (*keyboard_props_)[am::strings::limited_character_list][i].asString();
    EXPECT_EQ(character,
              res_list[am::strings::limited_character_list][i].asString());
  }

  EXPECT_EQ(
      testlanguage,
      static_cast<Language::eType>(res_list[am::strings::language].asInt()));
  EXPECT_EQ(testlayout,
            static_cast<KeyboardLayout::eType>(
                res_list[am::hmi_request::keyboard_layout].asInt()));
  EXPECT_EQ(testmode,
            static_cast<KeypressMode::eType>(
                res_list[am::strings::key_press_mode].asInt()));
  EXPECT_EQ(auto_complete_text,
            res_list[am::strings::auto_complete_text].asString());
}

void ResumptionDataTest::CheckMenuTitle(sm::SmartObject& res_list) {
  std::string value = (*menu_title_)[am::strings::menu_title].asString();
  EXPECT_EQ(value, res_list[am::strings::menu_title].asString());
}

void ResumptionDataTest::CheckMenuIcon(sm::SmartObject& res_list) {
  std::string value = (*menu_icon_)[am::strings::value].asString();
  ImageType::eType type = static_cast<ImageType::eType>(
      (*menu_icon_)[am::strings::image_type].asInt());

  EXPECT_EQ(value, res_list[am::strings::value].asString());
  EXPECT_EQ(
      type,
      static_cast<ImageType::eType>(res_list[am::strings::image_type].asInt()));
}

void ResumptionDataTest::CheckHelpPrompt(sm::SmartObject& res_list) {
  for (uint i = 0; i < tts_chunks_count; ++i) {
    std::string promt = (*help_prompt_)[i][am::strings::help_prompt].asString();
    std::string dict_promt = res_list[i][am::strings::help_prompt].asString();
    EXPECT_EQ(promt, dict_promt);
  }
}

void ResumptionDataTest::CheckTimeoutPrompt(
    NsSmartDeviceLink::NsSmartObjects::SmartObject& res_list) {
  for (uint i = 0; i < tts_chunks_count; ++i) {
    std::string text = (*timeout_prompt_)[i][am::strings::text].asString();
    SpeechCapabilities::eType speech = static_cast<SpeechCapabilities::eType>(
        (*timeout_prompt_)[i][am::strings::type].asInt());
    EXPECT_EQ(text, res_list[i][am::strings::text].asString());
    EXPECT_EQ(speech,
              static_cast<SpeechCapabilities::eType>(
                  res_list[i][am::strings::type].asInt()));
  }
}

void ResumptionDataTest::CheckVRHelp(sm::SmartObject& res_list) {
  std::string text;
  int position;
  for (uint i = 0; i < count_of_vrhelptitle; ++i) {
    text = (*vr_help_)[i][am::strings::text].asString();
    EXPECT_EQ(text, res_list[i][am::strings::text].asString());
    position = (*vr_help_)[i][am::strings::position].asInt();
    EXPECT_EQ(position, res_list[i][am::strings::position].asInt());
  }
}

void ResumptionDataTest::CheckVRTitle(
    NsSmartDeviceLink::NsSmartObjects::SmartObject& res_list) {
  std::string vtitle = (*vr_help_title_)[am::strings::vr_help_title].asString();
  EXPECT_EQ(vtitle, res_list[am::strings::vr_help_title].asString());
}

void ResumptionDataTest::PrepareData() {
  SetGlobalProporties();
  SetCommands();
  SetSubmenues();
  SetChoiceSet();
  SetAppFiles();

  DataAccessor<am::SubMenuMap> sub_menu_m(test_submenu_map, sublock_);
  DataAccessor<am::CommandsMap> commands_m(test_commands_map, comlock_);
  DataAccessor<am::ChoiceSetMap> choice_set_m(test_choiceset_map, setlock_);

  SetSubscriptions();
  DataAccessor<am::ButtonSubscriptions> btn_sub(btn_subscr, btnlock_);
  DataAccessor<am::VehicleInfoSubscriptions> ivi_access(ivi, ivilock_);

  ON_CALL(*app_mock, is_application_data_changed()).WillByDefault(Return(true));

  ON_CALL(*app_mock, mobile_app_id()).WillByDefault(Return(mobile_app_id_));
  ON_CALL(*app_mock, mac_address()).WillByDefault(ReturnRef(mac_address_));
  ON_CALL(*app_mock, curHash()).WillByDefault(ReturnRef(hash_));
  ON_CALL(*app_mock, get_grammar_id()).WillByDefault(Return(grammar_id_));
  ON_CALL(*app_mock, device()).WillByDefault(Return(device_handle_));
  ON_CALL(*app_mock, hmi_level()).WillByDefault(Return(hmi_level_));
  ON_CALL(*app_mock, app_id()).WillByDefault(Return(app_id_));
  ON_CALL(*app_mock, hmi_app_id()).WillByDefault(Return(hmi_app_id_));
  ON_CALL(*app_mock, IsAudioApplication()).WillByDefault(Return(is_audio_));
  ON_CALL(*app_mock, commands_map()).WillByDefault(Return(commands_m));
  ON_CALL(*app_mock, sub_menu_map()).WillByDefault(Return(sub_menu_m));
  ON_CALL(*app_mock, choice_set_map()).WillByDefault(Return(choice_set_m));

  ON_CALL(*app_mock, help_prompt()).WillByDefault(ReturnPointee(&help_prompt_));
  ON_CALL(*app_mock, timeout_prompt())
      .WillByDefault(ReturnPointee(&timeout_prompt_));
  ON_CALL(*app_mock, vr_help()).WillByDefault(ReturnPointee(&vr_help_));
  ON_CALL(*app_mock, vr_help_title())
      .WillByDefault(ReturnPointee(&vr_help_title_));

  ON_CALL(*app_mock, keyboard_props())
      .WillByDefault(ReturnPointee(&keyboard_props_));
  ON_CALL(*app_mock, menu_title()).WillByDefault(ReturnPointee(&menu_title_));
  ON_CALL(*app_mock, menu_icon()).WillByDefault(ReturnPointee(&menu_icon_));

  ON_CALL(*app_mock, SubscribedButtons()).WillByDefault(Return(btn_sub));
  ON_CALL(*app_mock, SubscribedIVI()).WillByDefault(Return(ivi_access));

  ON_CALL(*app_mock, getAppFiles()).WillByDefault(ReturnRef(app_files_map_));
}

void ResumptionDataTest::SetGlobalProporties() {
  SetKeyboardProperties();
  SetMenuTitleAndIcon();
  SetHelpAndTimeoutPrompt();
  SetVRHelpTitle();
}

void ResumptionDataTest::SetMenuTitleAndIcon() {
  custom_str::CustomString icon_name("test icon");
  sm::SmartObject sm_icon;
  sm_icon[am::strings::value] = "test icon";
  sm_icon[am::strings::image_type] = ImageType::STATIC;

  sm::SmartObject sm_title;
  sm_title = "test title";
  menu_title_ = new sm::SmartObject(sm_title);
  menu_icon_ = new sm::SmartObject(sm_icon);
}

void ResumptionDataTest::SetHelpAndTimeoutPrompt() {
  sm::SmartObject help_prompt;
  sm::SmartObject timeout_prompt;

  for (uint i = 0; i < tts_chunks_count + 3; ++i) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    help_prompt[i][am::strings::text] = "help prompt name" + std::string(numb);
    help_prompt[i][am::strings::type] = SpeechCapabilities::PRE_RECORDED;
  }
  help_prompt_ = new sm::SmartObject(help_prompt);
  for (uint i = 0; i < tts_chunks_count; ++i) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    timeout_prompt[i][am::strings::text] = "timeout test" + std::string(numb);
    timeout_prompt[i][am::strings::type] = SpeechCapabilities::SC_TEXT;
  }

  timeout_prompt_ = new sm::SmartObject(timeout_prompt);
}

void ResumptionDataTest::SetVRHelpTitle() {
  sm::SmartObject vr_help_title;
  vr_help_title = "vr help title";

  sm::SmartObject vr_help;
  for (uint i = 0; i < count_of_vrhelptitle; ++i) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    vr_help[i][am::strings::text] = "vr help " + std::string(numb);
    vr_help[i][am::strings::position] = i;
  }

  vr_help_ = new sm::SmartObject(vr_help);
  vr_help_title_ = new sm::SmartObject(vr_help_title);
}

void ResumptionDataTest::SetCommands() {
  sm::SmartObject sm_comm;

  sm::SmartObject vr_commandsvector;
  sm::SmartObject sm_icon;
  for (uint32_t i = 0; i < count_of_commands; ++i) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    sm_comm[am::strings::cmd_id] = i;
    sm_comm[am::strings::menu_params][am::strings::position] = i;
    sm_comm[am::strings::menu_params][am::hmi_request::parent_id] = i;
    sm_comm[am::strings::menu_params][am::strings::menu_name] =
        "Command" + std::string(numb);

    for (uint32_t j = 0; j < count_of_choice; ++j) {
      char vr[12];
      std::snprintf(vr, 12, "%d", i + j);
      vr_commandsvector[j] = "VrCommand " + std::string(vr);
    }
    sm_comm[am::strings::vr_commands] = vr_commandsvector;

    sm_icon[am::strings::value] = "command icon " + std::string(numb);
    sm_icon[am::strings::image_type] = ImageType::STATIC;

    sm_comm[am::strings::cmd_icon] = sm_icon;

    test_commands_map[i] = new sm::SmartObject(sm_comm);
  }
}

void ResumptionDataTest::SetSubmenues() {
  sm::SmartObject sm_comm;
  for (uint32_t i = 10; i < count_of_submenues + 10; ++i) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    sm_comm[am::strings::menu_id] = i;
    sm_comm[am::strings::position] = i;
    sm_comm[am::strings::menu_name] = "SubMenu" + std::string(numb);
    test_submenu_map[i] = new sm::SmartObject(sm_comm);
  }
}

void ResumptionDataTest::SetChoiceSet() {
  sm::SmartObject choice_vector;
  sm::SmartObject choice;
  sm::SmartObject vr_commandsvector;
  sm::SmartObject sm_icon;
  sm::SmartObject sec_icon;
  sm::SmartObject app_choice_set;
  sm::SmartObject application_choice_sets;
  for (uint32_t i = 0; i < count_of_choice_sets; ++i) {
    for (uint32_t j = 0; j < count_of_choice; ++j) {
      char numb[12];
      std::snprintf(numb, 12, "%d", i + j);

      choice[am::strings::choice_id] = i + j;
      vr_commandsvector[0] = "ChoiceSet VrCommand " + std::string(numb);
      choice[am::strings::vr_commands] = vr_commandsvector;
      choice[am::strings::menu_name] = "Choice name " + std::string(numb);
      choice[am::strings::secondary_text] =
          "secondaryText " + std::string(numb);
      choice[am::strings::tertiary_text] = "tertiaryText " + std::string(numb);

      sm_icon[am::strings::value] = "Choiceset icon " + std::string(numb);
      sm_icon[am::strings::image_type] = ImageType::STATIC;

      sec_icon[am::strings::value] =
          "Second choiceset icon " + std::string(numb);
      sec_icon[am::strings::image_type] = ImageType::STATIC;

      choice[am::strings::image] = sm_icon;
      choice[am::strings::secondary_image] = sec_icon;

      choice_vector[j] = choice;
    }
    app_choice_set[am::strings::interaction_choice_set_id] = i;
    app_choice_set[am::strings::grammar_id] = 100 + i;
    app_choice_set[am::strings::choice_set] = choice_vector;
    application_choice_sets[i] = app_choice_set;

    test_choiceset_map[i] = new sm::SmartObject(application_choice_sets[i]);
  }
}

void ResumptionDataTest::SetAppFiles() {
  am::AppFile test_file;
  int file_types;
  for (uint i = 0; i < count_of_files; ++i) {
    char numb[12];
    std::snprintf(numb, 12, "%d", i);
    file_types = i;
    test_file.is_persistent = true;
    test_file.is_download_complete = true;
    test_file.file_type = static_cast<FileType::eType>(file_types);
    test_file.file_name = "test_file " + std::string(numb);

    app_files_map_[test_file.file_name] = test_file;
  }
}

void ResumptionDataTest::SetKeyboardProperties() {
  sm::SmartObject keyboard;
  keyboard[am::strings::language] = Language::EN_US;
  keyboard[am::hmi_request::keyboard_layout] = KeyboardLayout::QWERTY;
  keyboard[am::strings::key_press_mode] = KeypressMode::SINGLE_KEYPRESS;
  keyboard[am::strings::auto_complete_text] = "complete";
  keyboard[am::strings::limited_character_list][0] = "y";
  keyboard[am::strings::limited_character_list][1] = "n";
  keyboard_props_ = new sm::SmartObject(keyboard);
}

void ResumptionDataTest::SetSubscriptions() {
  btn_subscr.insert(ButtonName::eType::CUSTOM_BUTTON);
  btn_subscr.insert(ButtonName::eType::OK);
  ivi.insert(0);
  ivi.insert(5);
}

}  // namespace resumption_test
}  // namespace components
}  // namespace test

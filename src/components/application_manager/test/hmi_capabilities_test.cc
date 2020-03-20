/*
 * Copyright (c) 2017, Ford Motor Company
 * All rights reserved.
 *
 * Copyright (c) 2017 Xevo Inc.
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
 * Neither the name of the copyright holders nor the names of their contributors
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

#include "application_manager/hmi_capabilities_for_testing.h"

#include <algorithm>
#include <memory>
#include <string>
#include <vector>

#include "application_manager/hmi_capabilities.h"
#include "application_manager/mock_message_helper.h"
#include "gtest/gtest.h"
#include "interfaces/HMI_API.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/smart_object.h"

#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/mock_rpc_service.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/state_controller.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "resumption/last_state_impl.h"
#include "resumption/last_state_wrapper_impl.h"
#include "utils/file_system.h"
#include "utils/jsoncpp_reader_wrapper.h"

namespace test {
namespace components {
namespace application_manager_test {

namespace {
const std::string kAppInfoDataFile = "./app_info_data";
const std::string kAppStorageFolder = "app_storage_folder";
const std::string kAppInfoStorage = "app_info_storage";
const std::string kHmiCapabilitiesDefaultFile = "hmi_capabilities.json";
const std::string kHmiCapabilitiesCacheDefaultFile =
    "hmi_capabilities_cache.json";
const uint32_t kEqualizerMaxChanelId = 10;
}  // namespace

using ::testing::_;
using ::testing::AtLeast;
using ::testing::InSequence;
using ::testing::Invoke;
using ::testing::NiceMock;
using ::testing::Return;
using ::testing::ReturnRef;

using namespace application_manager;

typedef std::map<std::string, hmi_apis::Common_Language::eType>
    LanguageCStringToEnumMap;

typedef std::map<std::string, hmi_apis::Common_LightName::eType>
    LightNameCStringToEnumMap;

static LanguageCStringToEnumMap languages_map;
static LightNameCStringToEnumMap light_names_map;

const std::vector<std::string> language_cstring_values{
    {"EN-US"}, {"ES-MX"}, {"FR-CA"}, {"DE-DE"}, {"ES-ES"}, {"EN-GB"}, {"RU-RU"},
    {"TR-TR"}, {"PL-PL"}, {"FR-FR"}, {"IT-IT"}, {"SV-SE"}, {"PT-PT"}, {"NL-NL"},
    {"EN-AU"}, {"ZH-CN"}, {"ZH-TW"}, {"JA-JP"}, {"AR-SA"}, {"KO-KR"}, {"PT-BR"},
    {"CS-CZ"}, {"DA-DK"}, {"NO-NO"}, {"NL-BE"}, {"EL-GR"}, {"HU-HU"}, {"FI-FI"},
    {"SK-SK"}, {"EN-IN"}, {"TH-TH"}, {"EN-SA"}, {"HE-IL"}, {"RO-RO"}, {"UK-UA"},
    {"ID-ID"}, {"VI-VN"}, {"MS-MY"}, {"HI-IN"}};

const std::vector<hmi_apis::Common_Language::eType> language_enum_values{
    hmi_apis::Common_Language::EN_US, hmi_apis::Common_Language::ES_MX,
    hmi_apis::Common_Language::FR_CA, hmi_apis::Common_Language::DE_DE,
    hmi_apis::Common_Language::ES_ES, hmi_apis::Common_Language::EN_GB,
    hmi_apis::Common_Language::RU_RU, hmi_apis::Common_Language::TR_TR,
    hmi_apis::Common_Language::PL_PL, hmi_apis::Common_Language::FR_FR,
    hmi_apis::Common_Language::IT_IT, hmi_apis::Common_Language::SV_SE,
    hmi_apis::Common_Language::PT_PT, hmi_apis::Common_Language::NL_NL,
    hmi_apis::Common_Language::EN_AU, hmi_apis::Common_Language::ZH_CN,
    hmi_apis::Common_Language::ZH_TW, hmi_apis::Common_Language::JA_JP,
    hmi_apis::Common_Language::AR_SA, hmi_apis::Common_Language::KO_KR,
    hmi_apis::Common_Language::PT_BR, hmi_apis::Common_Language::CS_CZ,
    hmi_apis::Common_Language::DA_DK, hmi_apis::Common_Language::NO_NO,
    hmi_apis::Common_Language::NL_BE, hmi_apis::Common_Language::EL_GR,
    hmi_apis::Common_Language::HU_HU, hmi_apis::Common_Language::FI_FI,
    hmi_apis::Common_Language::SK_SK, hmi_apis::Common_Language::EN_IN,
    hmi_apis::Common_Language::TH_TH, hmi_apis::Common_Language::EN_SA,
    hmi_apis::Common_Language::HE_IL, hmi_apis::Common_Language::RO_RO,
    hmi_apis::Common_Language::UK_UA, hmi_apis::Common_Language::ID_ID,
    hmi_apis::Common_Language::VI_VN, hmi_apis::Common_Language::MS_MY,
    hmi_apis::Common_Language::HI_IN};

const std::vector<hmi_apis::Common_LightName::eType> light_name_enum_values{
    hmi_apis::Common_LightName::eType::FRONT_LEFT_HIGH_BEAM,
    hmi_apis::Common_LightName::eType::FRONT_RIGHT_HIGH_BEAM,
    hmi_apis::Common_LightName::eType::FRONT_LEFT_LOW_BEAM,
    hmi_apis::Common_LightName::eType::FRONT_RIGHT_LOW_BEAM,
    hmi_apis::Common_LightName::eType::FRONT_LEFT_PARKING_LIGHT,
    hmi_apis::Common_LightName::eType::FRONT_RIGHT_PARKING_LIGHT,
    hmi_apis::Common_LightName::eType::FRONT_LEFT_FOG_LIGHT,
    hmi_apis::Common_LightName::eType::FRONT_RIGHT_FOG_LIGHT,
    hmi_apis::Common_LightName::eType::FRONT_LEFT_DAYTIME_RUNNING_LIGHT,
    hmi_apis::Common_LightName::eType::FRONT_RIGHT_DAYTIME_RUNNING_LIGHT,
    hmi_apis::Common_LightName::eType::FRONT_LEFT_TURN_LIGHT,
    hmi_apis::Common_LightName::eType::FRONT_RIGHT_TURN_LIGHT,
    hmi_apis::Common_LightName::eType::REAR_LEFT_FOG_LIGHT,
    hmi_apis::Common_LightName::eType::REAR_RIGHT_FOG_LIGHT,
    hmi_apis::Common_LightName::eType::REAR_LEFT_TAIL_LIGHT,
    hmi_apis::Common_LightName::eType::REAR_RIGHT_TAIL_LIGHT,
    hmi_apis::Common_LightName::eType::REAR_LEFT_BRAKE_LIGHT,
    hmi_apis::Common_LightName::eType::REAR_RIGHT_BRAKE_LIGHT,
    hmi_apis::Common_LightName::eType::REAR_LEFT_TURN_LIGHT,
    hmi_apis::Common_LightName::eType::REAR_RIGHT_TURN_LIGHT,
    hmi_apis::Common_LightName::eType::REAR_REGISTRATION_PLATE_LIGHT,
    hmi_apis::Common_LightName::eType::HIGH_BEAMS,
    hmi_apis::Common_LightName::eType::LOW_BEAMS,
    hmi_apis::Common_LightName::eType::FOG_LIGHTS,
    hmi_apis::Common_LightName::eType::RUNNING_LIGHTS,
    hmi_apis::Common_LightName::eType::PARKING_LIGHTS,
    hmi_apis::Common_LightName::eType::BRAKE_LIGHTS,
    hmi_apis::Common_LightName::eType::REAR_REVERSING_LIGHTS,
    hmi_apis::Common_LightName::eType::SIDE_MARKER_LIGHTS,
    hmi_apis::Common_LightName::eType::LEFT_TURN_LIGHTS,
    hmi_apis::Common_LightName::eType::RIGHT_TURN_LIGHTS,
    hmi_apis::Common_LightName::eType::HAZARD_LIGHTS,
    hmi_apis::Common_LightName::eType::REAR_CARGO_LIGHTS,
    hmi_apis::Common_LightName::eType::REAR_TRUCK_BED_LIGHTS,
    hmi_apis::Common_LightName::eType::REAR_TRAILER_LIGHTS,
    hmi_apis::Common_LightName::eType::LEFT_SPOT_LIGHTS,
    hmi_apis::Common_LightName::eType::RIGHT_SPOT_LIGHTS,
    hmi_apis::Common_LightName::eType::LEFT_PUDDLE_LIGHTS,
    hmi_apis::Common_LightName::eType::RIGHT_PUDDLE_LIGHTS,
    hmi_apis::Common_LightName::eType::AMBIENT_LIGHTS,
    hmi_apis::Common_LightName::eType::OVERHEAD_LIGHTS,
    hmi_apis::Common_LightName::eType::READING_LIGHTS,
    hmi_apis::Common_LightName::eType::TRUNK_LIGHTS,
    hmi_apis::Common_LightName::eType::EXTERIOR_FRONT_LIGHTS,
    hmi_apis::Common_LightName::eType::EXTERIOR_REAR_LIGHTS,
    hmi_apis::Common_LightName::eType::EXTERIOR_LEFT_LIGHTS,
    hmi_apis::Common_LightName::eType::EXTERIOR_RIGHT_LIGHTS,
    hmi_apis::Common_LightName::eType::EXTERIOR_ALL_LIGHTS};

const std::vector<std::string> light_name_string_values{
    {"FRONT_LEFT_HIGH_BEAM"},
    {"FRONT_RIGHT_HIGH_BEAM"},
    {"FRONT_LEFT_LOW_BEAM"},
    {"FRONT_RIGHT_LOW_BEAM"},
    {"FRONT_LEFT_PARKING_LIGHT"},
    {"FRONT_RIGHT_PARKING_LIGHT"},
    {"FRONT_LEFT_FOG_LIGHT"},
    {"FRONT_RIGHT_FOG_LIGHT"},
    {"FRONT_LEFT_DAYTIME_RUNNING_LIGHT"},
    {"FRONT_RIGHT_DAYTIME_RUNNING_LIGHT"},
    {"FRONT_LEFT_TURN_LIGHT"},
    {"FRONT_RIGHT_TURN_LIGHT"},
    {"REAR_LEFT_FOG_LIGHT"},
    {"REAR_RIGHT_FOG_LIGHT"},
    {"REAR_LEFT_TAIL_LIGHT"},
    {"REAR_RIGHT_TAIL_LIGHT"},
    {"REAR_LEFT_BRAKE_LIGHT"},
    {"REAR_RIGHT_BRAKE_LIGHT"},
    {"REAR_LEFT_TURN_LIGHT"},
    {"REAR_RIGHT_TURN_LIGHT"},
    {"REAR_REGISTRATION_PLATE_LIGHT"},
    {"HIGH_BEAMS"},
    {"LOW_BEAMS"},
    {"FOG_LIGHTS"},
    {"RUNNING_LIGHTS"},
    {"PARKING_LIGHTS"},
    {"BRAKE_LIGHTS"},
    {"REAR_REVERSING_LIGHTS"},
    {"SIDE_MARKER_LIGHTS"},
    {"LEFT_TURN_LIGHTS"},
    {"RIGHT_TURN_LIGHTS"},
    {"HAZARD_LIGHTS"},
    {"REAR_CARGO_LIGHTS"},
    {"REAR_TRUCK_BED_LIGHTS"},
    {"REAR_TRAILER_LIGHTS"},
    {"LEFT_SPOT_LIGHTS"},
    {"RIGHT_SPOT_LIGHTS"},
    {"LEFT_PUDDLE_LIGHTS"},
    {"RIGHT_PUDDLE_LIGHTS"},
    {"AMBIENT_LIGHTS"},
    {"OVERHEAD_LIGHTS"},
    {"READING_LIGHTS"},
    {"TRUNK_LIGHTS"},
    {"EXTERIOR_FRONT_LIGHTS"},
    {"EXTERIOR_REAR_LIGHTS"},
    {"EXTERIOR_LEFT_LIGHTS"},
    {"EXTERIOR_RIGHT_LIGHTS"},
    {"EXTERIOR_ALL_LIGHTS"}};

void InitLightNameCStringToEnumMap(
    LightNameCStringToEnumMap& out_light_names_map) {
  for (size_t i = 0; i < light_name_string_values.size(); ++i) {
    out_light_names_map[light_name_string_values[i]] =
        light_name_enum_values[i];
  }
}

bool LihgtNameStringToEnum(const std::string& light_name_str,
                           hmi_apis::Common_LightName::eType& out_value) {
  auto it = light_names_map.find(light_name_str);
  if (it == light_names_map.end()) {
    return false;
  }
  out_value = it->second;
  return true;
}

void InitLanguageCStringToEnumMap(LanguageCStringToEnumMap& out_languages_map) {
  for (size_t i = 0; i < language_cstring_values.size(); ++i) {
    out_languages_map[language_cstring_values[i]] = language_enum_values[i];
  }
}

bool LanguageStringToEnum(const std::string& language_str,
                          hmi_apis::Common_Language::eType& out_value) {
  LanguageCStringToEnumMap::const_iterator it =
      languages_map.find(language_str);
  if (it == languages_map.end()) {
    return false;
  }
  out_value = it->second;
  return true;
}

hmi_apis::Common_Language::eType TestCommonLanguageFromString(
    const std::string& language) {
  hmi_apis::Common_Language::eType value;
  if (LanguageStringToEnum(language, value)) {
    return value;
  }
  return hmi_apis::Common_Language::INVALID_ENUM;
}

hmi_apis::Common_LightName::eType TestCommonLightNameFromString(
    const std::string& light_name_str) {
  hmi_apis::Common_LightName::eType value;
  if (LihgtNameStringToEnum(light_name_str, value)) {
    return value;
  }
  return hmi_apis::Common_LightName::INVALID_ENUM;
}

bool IsLightNameExists(const hmi_apis::Common_LightName::eType& light_name) {
  auto it = std::find(
      light_name_enum_values.begin(), light_name_enum_values.end(), light_name);
  return !(it == light_name_enum_values.end());
}

class HMICapabilitiesTest : public ::testing::Test {
 protected:
  HMICapabilitiesTest()
      : last_state_wrapper_(std::make_shared<resumption::LastStateWrapperImpl>(
            std::make_shared<resumption::LastStateImpl>(kAppStorageFolder,
                                                        kAppInfoStorage)))
      , file_name_(kHmiCapabilitiesDefaultFile)
      , file_cache_name_(kHmiCapabilitiesCacheDefaultFile) {}
  virtual void SetUp() OVERRIDE {
    if (languages_map.empty()) {
      InitLanguageCStringToEnumMap(languages_map);
    }
    if (light_names_map.empty()) {
      InitLightNameCStringToEnumMap(light_names_map);
    }

    ON_CALL(mock_app_mngr_, IsSOStructValid(_, _)).WillByDefault(Return(true));

    EXPECT_CALL(mock_app_mngr_, event_dispatcher())
        .WillOnce(ReturnRef(mock_event_dispatcher));
    EXPECT_CALL(mock_app_mngr_, get_settings())
        .WillRepeatedly(ReturnRef(mock_application_manager_settings_));
    ON_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
        .WillByDefault(ReturnRef(file_name_));
    ON_CALL(mock_application_manager_settings_,
            hmi_capabilities_cache_file_name())
        .WillByDefault(ReturnRef(file_cache_name_));
    EXPECT_CALL(mock_event_dispatcher, add_observer(_, _, _));
    EXPECT_CALL(mock_event_dispatcher, remove_observer(_));
    ON_CALL(mock_application_manager_settings_, launch_hmi())
        .WillByDefault(Return(false));
    hmi_capabilities_test =
        std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr_);
    hmi_capabilities_test->Init(last_state_wrapper_);
  }

  void TearDown() OVERRIDE {
    DeleteFileIfExists(kHmiCapabilitiesCacheDefaultFile);
    hmi_capabilities_test.reset();
  }
  static void TearDownTestCase() {
    DeleteFileIfExists(kAppInfoDataFile);
  }

  static void DeleteFileIfExists(const std::string& file_path) {
    if (file_system::FileExists(file_path)) {
      EXPECT_TRUE(::file_system::DeleteFile(file_path));
    }
  }

  void SetCooperating();
  NiceMock<MockApplicationManager> mock_app_mngr_;
  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher;
  resumption::LastStateWrapperPtr last_state_wrapper_;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  std::shared_ptr<HMICapabilitiesForTesting> hmi_capabilities_test;
  const std::string file_name_;
  const std::string file_cache_name_;
  application_manager_test::MockRPCService mock_rpc_service_;
};

TEST_F(HMICapabilitiesTest, LoadCapabilitiesFromFile) {
  DeleteFileIfExists(kAppInfoDataFile);

  EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
              CommonLanguageFromString(_))
      .WillRepeatedly(Invoke(TestCommonLanguageFromString));
  EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
              CommonLightNameFromString(_))
      .WillRepeatedly(Invoke(TestCommonLightNameFromString));
  ASSERT_TRUE(hmi_capabilities_test->LoadCapabilitiesFromFile());

  // Check active languages
  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            hmi_capabilities_test->active_ui_language());
  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            hmi_capabilities_test->active_vr_language());
  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            hmi_capabilities_test->active_tts_language());

  // Check UI languages
  const auto ui_supported_languages_so =
      *(hmi_capabilities_test->ui_supported_languages());

  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            static_cast<hmi_apis::Common_Language::eType>(
                ui_supported_languages_so[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::ES_MX,
            static_cast<hmi_apis::Common_Language::eType>(
                ui_supported_languages_so[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::FR_CA,
            static_cast<hmi_apis::Common_Language::eType>(
                ui_supported_languages_so[2].asInt()));

  // Check VR languages
  const auto vr_supported_languages_so =
      *(hmi_capabilities_test->vr_supported_languages());

  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            static_cast<hmi_apis::Common_Language::eType>(
                vr_supported_languages_so[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::ES_MX,
            static_cast<hmi_apis::Common_Language::eType>(
                vr_supported_languages_so[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::FR_CA,
            static_cast<hmi_apis::Common_Language::eType>(
                vr_supported_languages_so[2].asInt()));

  // Check TTS languages
  const auto tts_supported_languages_so =
      *(hmi_capabilities_test->tts_supported_languages());

  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            static_cast<hmi_apis::Common_Language::eType>(
                tts_supported_languages_so[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::ES_MX,
            static_cast<hmi_apis::Common_Language::eType>(
                tts_supported_languages_so[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::FR_CA,
            static_cast<hmi_apis::Common_Language::eType>(
                tts_supported_languages_so[2].asInt()));

  // Check TTS capabilities
  const auto tts_capabilities_so =
      *(hmi_capabilities_test->speech_capabilities());
  EXPECT_EQ(hmi_apis::Common_SpeechCapabilities::SC_TEXT,
            static_cast<hmi_apis::Common_SpeechCapabilities::eType>(
                tts_capabilities_so[0].asInt()));

  // Check button capabilities
  const auto buttons_capabilities_so =
      *(hmi_capabilities_test->button_capabilities());

  // Count of buttons in json file
  const uint32_t btn_length =
      static_cast<uint32_t>(buttons_capabilities_so.length());
  EXPECT_EQ(16u, btn_length);
  for (uint32_t i = 0; i < btn_length; ++i) {
    auto index = static_cast<int32_t>(i);
    ASSERT_TRUE(
        (buttons_capabilities_so[index])
            .keyExists(rc_rpc_plugin::enums_value::kShortPressAvailable));
    ASSERT_TRUE(
        (buttons_capabilities_so[index])
            .keyExists(rc_rpc_plugin::enums_value::kLongPressAvailable));
    ASSERT_TRUE((buttons_capabilities_so[index])
                    .keyExists(rc_rpc_plugin::enums_value::kUpDownAvailable));
    EXPECT_TRUE(buttons_capabilities_so
                    [index][rc_rpc_plugin::enums_value::kShortPressAvailable]
                        .asBool());
    EXPECT_TRUE(
        buttons_capabilities_so[index]
                               [rc_rpc_plugin::enums_value::kLongPressAvailable]
                                   .asBool());
    EXPECT_TRUE(
        buttons_capabilities_so[index]
                               [rc_rpc_plugin::enums_value::kUpDownAvailable]
                                   .asBool());
  }

  const auto display_capabilities_so =
      *(hmi_capabilities_test->display_capabilities());

  // Check display type
  EXPECT_EQ(hmi_apis::Common_DisplayType::SDL_GENERIC,
            static_cast<hmi_apis::Common_DisplayType::eType>(
                display_capabilities_so[hmi_response::display_type].asInt()));

  EXPECT_EQ("GENERIC_DISPLAY",
            display_capabilities_so[hmi_response::display_name].asString());

  EXPECT_TRUE(display_capabilities_so["graphicSupported"].asBool());

  // Check text fields
  const uint32_t text_len =
      display_capabilities_so[hmi_response::text_fields].length();
  EXPECT_NE(0u, text_len);
  for (uint32_t i = 0; i < text_len; ++i) {
    const int32_t index = static_cast<int32_t>(i);
    EXPECT_TRUE((display_capabilities_so[hmi_response::text_fields][index])
                    .keyExists(strings::name));
    EXPECT_TRUE((display_capabilities_so[hmi_response::text_fields][index])
                    .keyExists(strings::character_set));
  }

  // Check image fields
  ASSERT_TRUE((display_capabilities_so).keyExists(hmi_response::image_fields));
  const uint64_t img_len =
      display_capabilities_so[hmi_response::image_fields].length();
  EXPECT_NE(0u, img_len);
  for (uint32_t i = 0; i < img_len; ++i) {
    const int32_t index = static_cast<int32_t>(i);
    EXPECT_TRUE((display_capabilities_so[hmi_response::image_fields][index])
                    .keyExists(strings::name));

    const hmi_apis::Common_ImageFieldName::eType field_name =
        static_cast<hmi_apis::Common_ImageFieldName::eType>(
            display_capabilities_so[hmi_response::image_fields][index]
                                   [strings::name]
                                       .asInt());

    if ((field_name == hmi_apis::Common_ImageFieldName::locationImage) ||
        (field_name == hmi_apis::Common_ImageFieldName::alertIcon)) {
      EXPECT_TRUE((display_capabilities_so[hmi_response::image_fields][index])
                      .keyExists(strings::image_type_supported));
      EXPECT_EQ(hmi_apis::Common_FileType::GRAPHIC_PNG,
                static_cast<hmi_apis::Common_FileType::eType>(
                    display_capabilities_so[hmi_response::image_fields][index]
                                           [strings::image_type_supported][0]
                                               .asInt()));
    }
  }

  // Check media clock formats
  EXPECT_TRUE(
      (display_capabilities_so).keyExists(hmi_response::media_clock_formats));
  const uint32_t media_length =
      display_capabilities_so[hmi_response::media_clock_formats].length();
  EXPECT_NE(0u, media_length);
  for (uint32_t i = 0; i < media_length; ++i) {
    EXPECT_EQ(
        i,
        display_capabilities_so[hmi_response::media_clock_formats][i].asUInt());
  }

  ASSERT_TRUE(
      (display_capabilities_so).keyExists(hmi_response::image_capabilities));
  EXPECT_EQ(hmi_apis::Common_ImageType::DYNAMIC,
            static_cast<hmi_apis::Common_ImageType::eType>(
                display_capabilities_so[hmi_response::image_capabilities][0]
                    .asInt()));
  EXPECT_EQ(hmi_apis::Common_ImageType::STATIC,
            static_cast<hmi_apis::Common_ImageType::eType>(
                display_capabilities_so[hmi_response::image_capabilities][1]
                    .asInt()));

  // templatesAvailable parameter could be as empty array
  ASSERT_TRUE(
      display_capabilities_so.keyExists(hmi_response::templates_available));

  // Check audio pass thru
  const auto audio_pass_thru_capabilities_so =
      *(hmi_capabilities_test->audio_pass_thru_capabilities());
  EXPECT_EQ(smart_objects::SmartType::SmartType_Array,
            audio_pass_thru_capabilities_so.getType());
  EXPECT_EQ(smart_objects::SmartType_Array,
            audio_pass_thru_capabilities_so.getType());
  EXPECT_EQ(1u, audio_pass_thru_capabilities_so.length());
  EXPECT_EQ(hmi_apis::Common_SamplingRate::RATE_44KHZ,
            static_cast<hmi_apis::Common_SamplingRate::eType>(
                audio_pass_thru_capabilities_so[0]["samplingRate"].asInt()));
  EXPECT_EQ(hmi_apis::Common_BitsPerSample::RATE_8_BIT,
            static_cast<hmi_apis::Common_BitsPerSample::eType>(
                audio_pass_thru_capabilities_so[0]["bitsPerSample"].asInt()));
  EXPECT_EQ(hmi_apis::Common_AudioType::PCM,
            static_cast<hmi_apis::Common_AudioType::eType>(
                audio_pass_thru_capabilities_so[0]["audioType"].asInt()));

  // Check hmi zone capabilities
  const auto hmi_zone_capabilities_so =
      *(hmi_capabilities_test->hmi_zone_capabilities());
  EXPECT_EQ(hmi_apis::Common_HmiZoneCapabilities::FRONT,
            static_cast<hmi_apis::Common_HmiZoneCapabilities::eType>(
                hmi_zone_capabilities_so.asInt()));

  // Check Soft Button capabilities
  const auto& soft_button_capabilities_so =
      *(hmi_capabilities_test->soft_button_capabilities());

  const uint32_t soft_butons_length =
      static_cast<uint32_t>(soft_button_capabilities_so.length());

  for (uint32_t i = 0; i < soft_butons_length; ++i) {
    int32_t index = static_cast<int32_t>(i);
    EXPECT_TRUE(soft_button_capabilities_so[index].keyExists(
        rc_rpc_plugin::enums_value::kShortPressAvailable));
    EXPECT_EQ(smart_objects::SmartType::SmartType_Boolean,
              soft_button_capabilities_so
                  [index][rc_rpc_plugin::enums_value::kShortPressAvailable]
                      .getType());

    EXPECT_TRUE(soft_button_capabilities_so[index].keyExists(
        rc_rpc_plugin::enums_value::kLongPressAvailable));
    EXPECT_EQ(smart_objects::SmartType::SmartType_Boolean,
              soft_button_capabilities_so
                  [index][rc_rpc_plugin::enums_value::kLongPressAvailable]
                      .getType());

    EXPECT_TRUE(soft_button_capabilities_so[index].keyExists(
        rc_rpc_plugin::enums_value::kUpDownAvailable));
    EXPECT_EQ(smart_objects::SmartType::SmartType_Boolean,
              soft_button_capabilities_so
                  [index][rc_rpc_plugin::enums_value::kUpDownAvailable]
                      .getType());

    EXPECT_TRUE(soft_button_capabilities_so[index].keyExists(
        hmi_response::image_supported));
    EXPECT_EQ(smart_objects::SmartType::SmartType_Boolean,
              soft_button_capabilities_so[index][hmi_response::image_supported]
                  .getType());
  }

  const auto preset_bank_so =
      *(hmi_capabilities_test->preset_bank_capabilities());
  EXPECT_TRUE(
      preset_bank_so.keyExists(hmi_response::on_screen_presets_available));
  EXPECT_EQ(
      smart_objects::SmartType::SmartType_Boolean,
      preset_bank_so[hmi_response::on_screen_presets_available].getType());

  // Check vehicle type
  const auto vehicle_type_so = *(hmi_capabilities_test->vehicle_type());

  EXPECT_EQ("SDL", vehicle_type_so["make"].asString());
  EXPECT_EQ("Generic", vehicle_type_so["model"].asString());
  EXPECT_EQ("2019", vehicle_type_so["modelYear"].asString());
  EXPECT_EQ("SE", vehicle_type_so["trim"].asString());

  // Check navigation capabilities
  const auto navigation_capability_so =
      *(hmi_capabilities_test->navigation_capability());

  EXPECT_TRUE(navigation_capability_so.keyExists("sendLocationEnabled"));
  EXPECT_TRUE(navigation_capability_so.keyExists("getWayPointsEnabled"));
  EXPECT_TRUE(navigation_capability_so["sendLocationEnabled"].asBool());
  EXPECT_TRUE(navigation_capability_so["getWayPointsEnabled"].asBool());

  // since we have navigation capabilities, the feature should be supported
  EXPECT_TRUE(hmi_capabilities_test->navigation_supported());

  const auto phone_capability_so = *(hmi_capabilities_test->phone_capability());

  EXPECT_TRUE(phone_capability_so.keyExists("dialNumberEnabled"));
  EXPECT_TRUE(phone_capability_so["dialNumberEnabled"].asBool());

  EXPECT_TRUE(hmi_capabilities_test->phone_call_supported());

  const auto vs_capability_so =
      *(hmi_capabilities_test->video_streaming_capability());

  EXPECT_TRUE(vs_capability_so.keyExists(strings::preferred_resolution));
  EXPECT_TRUE(vs_capability_so[strings::preferred_resolution].keyExists(
      strings::resolution_width));
  EXPECT_TRUE(vs_capability_so[strings::preferred_resolution].keyExists(
      strings::resolution_height));
  EXPECT_EQ(
      800,
      vs_capability_so[strings::preferred_resolution][strings::resolution_width]
          .asInt());
  EXPECT_EQ(350,
            vs_capability_so[strings::preferred_resolution]
                            [strings::resolution_height]
                                .asInt());
  EXPECT_TRUE(vs_capability_so.keyExists(strings::max_bitrate));
  EXPECT_EQ(10000, vs_capability_so[strings::max_bitrate].asInt());
  EXPECT_TRUE(vs_capability_so.keyExists(strings::supported_formats));
  const uint32_t supported_formats_len =
      vs_capability_so[strings::supported_formats].length();
  EXPECT_EQ(1u, supported_formats_len);

  EXPECT_TRUE(vs_capability_so[strings::supported_formats][0].keyExists(
      strings::protocol));
  EXPECT_TRUE(vs_capability_so[strings::supported_formats][0].keyExists(
      strings::codec));
  EXPECT_EQ(hmi_apis::Common_VideoStreamingProtocol::RAW,
            vs_capability_so[strings::supported_formats][0][strings::protocol]
                .asInt());
  EXPECT_EQ(
      hmi_apis::Common_VideoStreamingCodec::H264,
      vs_capability_so[strings::supported_formats][0][strings::codec].asInt());

  EXPECT_TRUE(
      vs_capability_so.keyExists(strings::haptic_spatial_data_supported));
  EXPECT_FALSE(
      vs_capability_so[strings::haptic_spatial_data_supported].asBool());

  EXPECT_TRUE(hmi_capabilities_test->video_streaming_supported());

  // Check Remote Control capabilites

  const auto rc_capability_so = *(hmi_capabilities_test->rc_capability());

  // Check climate control capabilities
  ASSERT_TRUE(rc_capability_so.keyExists(
      rc_rpc_plugin::strings::kclimateControlCapabilities));
  const auto& climate_control_capabilities_so =
      rc_capability_so[rc_rpc_plugin::strings::kclimateControlCapabilities][0];

  EXPECT_TRUE(climate_control_capabilities_so
                  [rc_rpc_plugin::strings::kFanSpeedAvailable]
                      .asBool());
  EXPECT_TRUE(climate_control_capabilities_so
                  [rc_rpc_plugin::strings::kDesiredTemperatureAvailable]
                      .asBool());
  EXPECT_TRUE(climate_control_capabilities_so
                  [rc_rpc_plugin::strings::kAcEnableAvailable]
                      .asBool());
  EXPECT_TRUE(climate_control_capabilities_so
                  [rc_rpc_plugin::strings::kAcMaxEnableAvailable]
                      .asBool());

  // Check radio control capabilities
  ASSERT_TRUE(rc_capability_so.keyExists(
      rc_rpc_plugin::strings::kradioControlCapabilities));
  const auto& radio_control_capabilities_so =
      rc_capability_so[rc_rpc_plugin::strings::kradioControlCapabilities][0];

  EXPECT_EQ("radio", radio_control_capabilities_so["moduleName"].asString());

  EXPECT_TRUE(radio_control_capabilities_so
                  [rc_rpc_plugin::message_params::kAvailableHdChannelsAvailable]
                      .asBool());
  EXPECT_TRUE(radio_control_capabilities_so
                  [rc_rpc_plugin::strings::kSignalChangeThresholdAvailable]
                      .asBool());
  EXPECT_TRUE(radio_control_capabilities_so
                  [rc_rpc_plugin::strings::kSignalStrengthAvailable]
                      .asBool());
  EXPECT_TRUE(radio_control_capabilities_so
                  [rc_rpc_plugin::strings::kHdRadioEnableAvailable]
                      .asBool());
  EXPECT_TRUE(radio_control_capabilities_so
                  [rc_rpc_plugin::strings::kSiriusxmRadioAvailable]
                      .asBool());
  EXPECT_TRUE(
      radio_control_capabilities_so[rc_rpc_plugin::strings::kSisDataAvailable]
          .asBool());
  EXPECT_TRUE(
      radio_control_capabilities_so[rc_rpc_plugin::strings::kStateAvailable]
          .asBool());
  EXPECT_TRUE(
      radio_control_capabilities_so[rc_rpc_plugin::strings::kRadioBandAvailable]
          .asBool());
  EXPECT_TRUE(radio_control_capabilities_so
                  [rc_rpc_plugin::strings::kRadioFrequencyAvailable]
                      .asBool());
  EXPECT_TRUE(
      radio_control_capabilities_so[rc_rpc_plugin::strings::kHdChannelAvailable]
          .asBool());
  EXPECT_TRUE(
      radio_control_capabilities_so[rc_rpc_plugin::strings::kRdsDataAvailable]
          .asBool());

  ASSERT_TRUE(radio_control_capabilities_so.keyExists(
      rc_rpc_plugin::message_params::kModuleInfo));
  ASSERT_TRUE(
      radio_control_capabilities_so[rc_rpc_plugin::message_params::kModuleInfo]
          .keyExists(rc_rpc_plugin::message_params::kModuleId));

  // Check button capabilities

  ASSERT_TRUE(
      rc_capability_so.keyExists(rc_rpc_plugin::strings::kbuttonCapabilities));
  const auto& button_capabilities_so =
      rc_capability_so[rc_rpc_plugin::strings::kbuttonCapabilities];

  const uint32_t button_capabilities_length =
      static_cast<uint32_t>(button_capabilities_so.length());

  for (uint32_t i = 0; i < button_capabilities_length; ++i) {
    const int32_t index = static_cast<int32_t>(i);

    ASSERT_TRUE(button_capabilities_so[index].keyExists("name"));

    ASSERT_TRUE(button_capabilities_so[index].keyExists(
        rc_rpc_plugin::enums_value::kLongPressAvailable));
    EXPECT_EQ(
        smart_objects::SmartType::SmartType_Boolean,
        button_capabilities_so[index]
                              [rc_rpc_plugin::enums_value::kLongPressAvailable]
                                  .getType());
    ASSERT_TRUE(button_capabilities_so[index].keyExists(
        rc_rpc_plugin::enums_value::kShortPressAvailable));
    EXPECT_EQ(
        smart_objects::SmartType::SmartType_Boolean,
        button_capabilities_so[index]
                              [rc_rpc_plugin::enums_value::kShortPressAvailable]
                                  .getType());

    ASSERT_TRUE(button_capabilities_so[index].keyExists(
        rc_rpc_plugin::enums_value::kUpDownAvailable));
    EXPECT_EQ(
        smart_objects::SmartType::SmartType_Boolean,
        button_capabilities_so[index]
                              [rc_rpc_plugin::enums_value::kUpDownAvailable]
                                  .getType());
  }

  // Check audio capabilities

  EXPECT_TRUE(rc_capability_so.keyExists(
      rc_rpc_plugin::strings::kaudioControlCapabilities));

  const auto& audio_capabilities_so =
      rc_capability_so[rc_rpc_plugin::strings::kaudioControlCapabilities][0];

  EXPECT_TRUE(audio_capabilities_so.keyExists(
      rc_rpc_plugin::message_params::kModuleInfo));

  const auto& audio_cap_module_info =
      audio_capabilities_so[rc_rpc_plugin::message_params::kModuleInfo];
  EXPECT_TRUE(audio_cap_module_info.keyExists(
      rc_rpc_plugin::message_params::kModuleId));
  EXPECT_TRUE(audio_cap_module_info.keyExists(
      rc_rpc_plugin::strings::kAllowMultipleAccess));
  EXPECT_TRUE(
      audio_cap_module_info[rc_rpc_plugin::strings::kAllowMultipleAccess]
          .asBool());

  EXPECT_TRUE(
      audio_capabilities_so[rc_rpc_plugin::strings::kSourceAvailable].asBool());
  EXPECT_TRUE(
      audio_capabilities_so[rc_rpc_plugin::strings::kVolumeAvailable].asBool());
  EXPECT_TRUE(audio_capabilities_so[rc_rpc_plugin::strings::kEqualizerAvailable]
                  .asBool());
  EXPECT_TRUE(
      audio_capabilities_so[rc_rpc_plugin::strings::kKeepContextAvailable]
          .asBool());
  EXPECT_EQ(
      kEqualizerMaxChanelId,
      audio_capabilities_so[rc_rpc_plugin::strings::kEqualizerMaxChannelId]
          .asInt());

  // Check Seat Capabilities

  EXPECT_TRUE(rc_capability_so.keyExists(
      rc_rpc_plugin::strings::kseatControlCapabilities));
  const auto& seat_capabilities_so =
      rc_capability_so[rc_rpc_plugin::strings::kseatControlCapabilities][0];

  EXPECT_TRUE(seat_capabilities_so.keyExists(
      rc_rpc_plugin::message_params::kModuleInfo));
  const auto& seat_cap_module_info =
      seat_capabilities_so[rc_rpc_plugin::message_params::kModuleInfo];
  EXPECT_TRUE(
      seat_cap_module_info.keyExists(rc_rpc_plugin::message_params::kModuleId));
  EXPECT_TRUE(seat_cap_module_info.keyExists(
      rc_rpc_plugin::strings::kAllowMultipleAccess));
  EXPECT_FALSE(
      seat_cap_module_info[rc_rpc_plugin::strings::kAllowMultipleAccess]
          .asBool());

  // Check Light Capabilities
  EXPECT_TRUE(rc_capability_so.keyExists(
      rc_rpc_plugin::strings::klightControlCapabilities));
  const auto& light_capabilities_so =
      rc_capability_so[rc_rpc_plugin::strings::klightControlCapabilities];

  EXPECT_TRUE(light_capabilities_so.keyExists(
      rc_rpc_plugin::message_params::kModuleInfo));
  const auto& light_cap_module_info =
      light_capabilities_so[rc_rpc_plugin::message_params::kModuleInfo];
  EXPECT_TRUE(light_cap_module_info.keyExists(
      rc_rpc_plugin::message_params::kModuleId));
  EXPECT_FALSE(
      light_cap_module_info[rc_rpc_plugin::strings::kAllowMultipleAccess]
          .asBool());

  EXPECT_TRUE(light_capabilities_so.keyExists(
      rc_rpc_plugin::strings::kSupportedLights));
  const auto& supported_lights =
      light_capabilities_so[rc_rpc_plugin::strings::kSupportedLights];

  const uint32_t supported_lights_length =
      static_cast<uint32_t>(supported_lights.length());
  for (uint32_t i = 0; i < supported_lights_length; ++i) {
    int32_t index = static_cast<int32_t>(i);

    EXPECT_TRUE(
        IsLightNameExists(static_cast<hmi_apis::Common_LightName::eType>(
            supported_lights[index]["name"].asInt())));
    EXPECT_TRUE(supported_lights[index].keyExists(
        rc_rpc_plugin::strings::kStatusAvailable));
    EXPECT_TRUE(supported_lights[index].keyExists(
        rc_rpc_plugin::strings::kDensityAvailable));
    EXPECT_TRUE(supported_lights[index].keyExists(
        rc_rpc_plugin::strings::kRGBColorSpaceAvailable));
  }

  EXPECT_TRUE(rc_capability_so.keyExists(
      rc_rpc_plugin::strings::klightControlCapabilities));
  const auto& light_ctrl_cap_so =
      rc_capability_so[rc_rpc_plugin::strings::klightControlCapabilities];

  EXPECT_TRUE(light_ctrl_cap_so.keyExists("moduleName"));
  EXPECT_TRUE(
      light_ctrl_cap_so.keyExists(rc_rpc_plugin::strings::kSupportedLights));
  EXPECT_EQ(
      smart_objects::SmartType::SmartType_Array,
      light_ctrl_cap_so[rc_rpc_plugin::strings::kSupportedLights].getType());

  const auto& supported_light_so =
      light_ctrl_cap_so[rc_rpc_plugin::strings::kSupportedLights];

  const uint32_t sup_lights_length =
      static_cast<uint32_t>(supported_light_so.length());

  for (uint32_t i = 0; i < sup_lights_length; ++i) {
    int32_t index = static_cast<int32_t>(i);
    EXPECT_TRUE(
        supported_light_so[index].keyExists(rc_rpc_plugin::strings::kName));
  }
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFileAndVerifyUnsupportedSystemCapabilities) {
  MockApplicationManager mock_app_mngr;
  NiceMock<event_engine_test::MockEventDispatcher> mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;

  const std::string hmi_capabilities_file = "hmi_capabilities_sc1.json";

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_file));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(file_cache_name_));

  DeleteFileIfExists(kAppInfoDataFile);

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);
  hmi_capabilities->Init(last_state_wrapper_);

  // Check system capabilities; only phone capability is available
  EXPECT_FALSE(hmi_capabilities->navigation_supported());
  EXPECT_TRUE(hmi_capabilities->phone_call_supported());
  EXPECT_FALSE(hmi_capabilities->video_streaming_supported());
  EXPECT_FALSE(hmi_capabilities->rc_supported());

  // verify phone capability
  const smart_objects::SmartObject phone_capability_so =
      *(hmi_capabilities->phone_capability());
  EXPECT_TRUE(phone_capability_so.keyExists("dialNumberEnabled"));
  EXPECT_TRUE(phone_capability_so["dialNumberEnabled"].asBool());
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFileAndVerifyEmptySystemCapabilities) {
  MockApplicationManager mock_app_mngr;
  NiceMock<event_engine_test::MockEventDispatcher> mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;

  const std::string hmi_capabilities_file = "hmi_capabilities_sc2.json";

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_file));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(file_cache_name_));

  DeleteFileIfExists(kAppInfoDataFile);

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);
  hmi_capabilities->Init(last_state_wrapper_);

  // Check system capabilities; only navigation capability is valid, the other
  // two are empty
  EXPECT_TRUE(hmi_capabilities->navigation_supported());
  EXPECT_FALSE(hmi_capabilities->phone_call_supported());
  EXPECT_FALSE(hmi_capabilities->video_streaming_supported());
  EXPECT_FALSE(hmi_capabilities->rc_supported());

  // verify navigation capabilities
  smart_objects::SmartObject navigation_capability_so =
      *(hmi_capabilities->navigation_capability());
  EXPECT_TRUE(navigation_capability_so.keyExists("sendLocationEnabled"));
  EXPECT_TRUE(navigation_capability_so.keyExists("getWayPointsEnabled"));
  EXPECT_TRUE(navigation_capability_so["sendLocationEnabled"].asBool());
  EXPECT_FALSE(navigation_capability_so["getWayPointsEnabled"].asBool());
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFileAndVerifyOldAudioPassThruCapabilities) {
  MockApplicationManager mock_app_mngr;
  event_engine_test::MockEventDispatcher mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;

  const std::string hmi_capabilities_file = "hmi_capabilities_old_apt.json";

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_file));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(file_cache_name_));

  EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
              CommonLanguageFromString(_))
      .WillRepeatedly(Invoke(TestCommonLanguageFromString));

  DeleteFileIfExists(kAppInfoDataFile);

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);
  hmi_capabilities->Init(last_state_wrapper_);

  // with old audio pass thru format, the object is an array containing a single
  // object
  smart_objects::SmartObjectSPtr audio_pass_thru_capabilities_so =
      hmi_capabilities->audio_pass_thru_capabilities();
  EXPECT_EQ(smart_objects::SmartType_Array,
            audio_pass_thru_capabilities_so->getType());
  EXPECT_EQ(1u, audio_pass_thru_capabilities_so->length());
  smart_objects::SmartObject& first_element =
      (*audio_pass_thru_capabilities_so)[0];
  EXPECT_TRUE(first_element.keyExists("samplingRate"));
  EXPECT_EQ(hmi_apis::Common_SamplingRate::RATE_22KHZ,
            static_cast<hmi_apis::Common_SamplingRate::eType>(
                first_element["samplingRate"].asInt()));
  EXPECT_TRUE(first_element.keyExists("bitsPerSample"));
  EXPECT_EQ(hmi_apis::Common_BitsPerSample::RATE_16_BIT,
            static_cast<hmi_apis::Common_BitsPerSample::eType>(
                first_element["bitsPerSample"].asInt()));
  EXPECT_TRUE(first_element.keyExists("audioType"));
  EXPECT_EQ(hmi_apis::Common_AudioType::PCM,
            static_cast<hmi_apis::Common_AudioType::eType>(
                first_element["audioType"].asInt()));
}

TEST_F(HMICapabilitiesTest, VerifyImageType) {
  const int32_t image_type = 1;
  smart_objects::SmartObject sm_obj;
  sm_obj[hmi_response::image_capabilities][0] = image_type;
  hmi_capabilities_test->set_display_capabilities(sm_obj);

  EXPECT_TRUE(hmi_capabilities_test->VerifyImageType(image_type));

  const int32_t new_image_type = 2;
  EXPECT_FALSE(hmi_capabilities_test->VerifyImageType(new_image_type));
}

void HMICapabilitiesTest::SetCooperating() {
  smart_objects::SmartObjectSPtr test_so;
  EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
              CreateModuleInfoSO(_, _))
      .WillRepeatedly(Return(test_so));
  EXPECT_CALL(mock_rpc_service_, ManageHMICommand(_, _))
      .WillRepeatedly(Return(true));
}

TEST_F(HMICapabilitiesTest, SetVRCooperating) {
  hmi_capabilities_test->set_is_vr_cooperating(true);
  EXPECT_EQ(true, hmi_capabilities_test->is_vr_cooperating());
}

TEST_F(HMICapabilitiesTest, SetTTSCooperating) {
  hmi_capabilities_test->set_is_tts_cooperating(true);
  EXPECT_EQ(true, hmi_capabilities_test->is_tts_cooperating());
}

TEST_F(HMICapabilitiesTest, SetUICooperating) {
  hmi_capabilities_test->set_is_ui_cooperating(true);
  EXPECT_EQ(true, hmi_capabilities_test->is_ui_cooperating());
}

TEST_F(HMICapabilitiesTest, SetIviCooperating) {
  hmi_capabilities_test->set_is_ivi_cooperating(true);
  EXPECT_EQ(true, hmi_capabilities_test->is_ivi_cooperating());
}

TEST_F(
    HMICapabilitiesTest,
    UpdateCapabilitiesDependingOn_ccpuVersion_FromCacheForOld_RequestForNew) {
  MockApplicationManager mock_app_mngr;
  event_engine_test::MockEventDispatcher mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;
  const std::string ccpu_version = "4.1.3.B_EB355B";
  const std::string ccpu_version_new = "5.1.3.B_EB355B";
  const std::string hmi_capabilities_invalid_file =
      "hmi_capabilities_invalid_file.json";

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_invalid_file));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(file_cache_name_));

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);

  EXPECT_CALL(mock_app_mngr, SetHMICooperating(true));
  EXPECT_CALL(mock_app_mngr, RequestForInterfacesAvailability()).Times(2);

  hmi_capabilities->set_ccpu_version(ccpu_version);
  hmi_capabilities->OnSoftwareVersionReceived(ccpu_version);

  EXPECT_EQ(ccpu_version, hmi_capabilities->ccpu_version());

  hmi_capabilities->OnSoftwareVersionReceived(ccpu_version_new);
  EXPECT_EQ(ccpu_version_new, hmi_capabilities->ccpu_version());
}

TEST_F(HMICapabilitiesTest,
       UpdateCapabilitiesForNew_ccpuVersion_DeleteCacheFile) {
  MockApplicationManager mock_app_mngr;
  event_engine_test::MockEventDispatcher mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;
  const std::string ccpu_version = "4.1.3.B_EB355B";
  const std::string ccpu_version_new = "5.1.3.B_EB355B";
  const std::string hmi_capabilities_invalid_file =
      "hmi_capabilities_invalid_file.json";

  file_system::CreateFile(file_cache_name_);
  EXPECT_TRUE(file_system::FileExists(file_cache_name_));

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_invalid_file));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(file_cache_name_));

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);

  hmi_capabilities->set_ccpu_version(ccpu_version);
  hmi_capabilities->OnSoftwareVersionReceived(ccpu_version_new);
  EXPECT_EQ(ccpu_version_new, hmi_capabilities->ccpu_version());

  EXPECT_FALSE(file_system::FileExists(file_cache_name_));
}

TEST_F(HMICapabilitiesTest,
       CacheFileNameNotSpecified_NoNeedToSave_ReturnSuccess) {
  MockApplicationManager mock_app_mngr;
  event_engine_test::MockEventDispatcher mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;
  const smart_objects::CSmartSchema schema;
  const std::string hmi_capabilities_empty_file_name = "";
  const std::vector<std::string> sections_to_update{hmi_response::language};

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_empty_file_name));

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);

  EXPECT_TRUE(hmi_capabilities->SaveCachedCapabilitiesToFile(
      hmi_interface::tts, sections_to_update, schema));
}

TEST_F(HMICapabilitiesTest, SaveCachedCapabilitiesToFile_ParseFile_Failed) {
  MockApplicationManager mock_app_mngr;
  event_engine_test::MockEventDispatcher mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;
  const smart_objects::CSmartSchema schema;
  const std::vector<std::string> sections_to_update{hmi_response::language};

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(file_cache_name_));

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);

  file_system::CreateFile(file_cache_name_);
  EXPECT_TRUE(file_system::FileExists(file_cache_name_));

  EXPECT_FALSE(hmi_capabilities->SaveCachedCapabilitiesToFile(
      hmi_interface::tts, sections_to_update, schema));
}

TEST_F(HMICapabilitiesTest,
       SaveCachedCapabilitiesToFile_ParsedFieldsSave_Success) {
  MockApplicationManager mock_app_mngr;
  event_engine_test::MockEventDispatcher mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;
  const smart_objects::CSmartSchema schema;
  const std::vector<std::string> sections_to_update{
      hmi_response::display_capabilities};
  const std::string content_to_save = "{\"field\" : \"value\" }";

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(file_cache_name_));

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);

  file_system::CreateFile(file_cache_name_);
  EXPECT_TRUE(file_system::FileExists(file_cache_name_));

  const std::vector<uint8_t> binary_data_to_save(content_to_save.begin(),
                                                 content_to_save.end());
  file_system::Write(file_cache_name_, binary_data_to_save);

  EXPECT_TRUE(hmi_capabilities->SaveCachedCapabilitiesToFile(
      hmi_interface::tts, sections_to_update, schema));

  std::string content_after_update;
  EXPECT_TRUE(file_system::ReadFile(file_cache_name_, content_after_update));
  EXPECT_NE(content_to_save, content_after_update);
}

TEST_F(HMICapabilitiesTest, PrepareJsonValueForSaving_Success) {
  MockApplicationManager mock_app_mngr;
  event_engine_test::MockEventDispatcher mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;
  const smart_objects::CSmartSchema schema;

  const std::vector<std::string> sections_to_update{
      hmi_response::display_capabilities,
      hmi_response::hmi_zone_capabilities,
      hmi_response::soft_button_capabilities,
      strings::audio_pass_thru_capabilities,
      strings::hmi_capabilities,
      strings::system_capabilities,
      hmi_response::languages};

  const std::vector<std::string> interfaces_name{
      hmi_interface::tts,
      hmi_interface::vr,
      hmi_interface::ui,
      hmi_interface::buttons,
      hmi_interface::vehicle_info,
      hmi_interface::rc,
      hmi_response::speech_capabilities,
      hmi_response::prerecorded_speech_capabilities,
      hmi_response::button_capabilities,
      hmi_response::preset_bank_capabilities};

  const std::string content_to_save = "{\"field\" : \"value\" }";
  const std::vector<uint8_t> binary_data_to_save(content_to_save.begin(),
                                                 content_to_save.end());

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(file_cache_name_));

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);

  file_system::CreateFile(file_cache_name_);
  EXPECT_TRUE(file_system::FileExists(file_cache_name_));

  file_system::Write(file_cache_name_, binary_data_to_save);

  for (size_t i = 0; i < interfaces_name.size(); ++i) {
    EXPECT_TRUE(hmi_capabilities->SaveCachedCapabilitiesToFile(
        interfaces_name[i], sections_to_update, schema));
  }

  std::string content_after_update;
  EXPECT_TRUE(file_system::ReadFile(file_cache_name_, content_after_update));

  Json::Value root_node;
  utils::JsonReader reader;
  EXPECT_TRUE(reader.parse(content_after_update, &root_node));

  for (size_t i = 0; i < interfaces_name.size(); ++i) {
    EXPECT_TRUE(static_cast<bool>(root_node[interfaces_name[i]]));
  }
}

TEST_F(HMICapabilitiesTest,
       OnCapabilityInitialized_RespondToAllPendingRAIRequestsIfTheyHold) {
  MockApplicationManager mock_app_mngr;
  NiceMock<event_engine_test::MockEventDispatcher> mock_dispatcher;
  MockApplicationManagerSettings mock_application_manager_settings;

  const std::string hmi_capabilities_file = "hmi_capabilities_sc2.json";

  ON_CALL(mock_app_mngr, event_dispatcher())
      .WillByDefault(ReturnRef(mock_dispatcher));
  ON_CALL(mock_app_mngr, get_settings())
      .WillByDefault(ReturnRef(mock_application_manager_settings));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_file));
  ON_CALL(mock_application_manager_settings, hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(file_cache_name_));

  auto hmi_capabilities =
      std::make_shared<HMICapabilitiesForTesting>(mock_app_mngr);
  hmi_capabilities->Init(last_state_wrapper_);

  EXPECT_TRUE(hmi_capabilities->navigation_supported());

  smart_objects::SmartObject navigation_capability_so =
      *(hmi_capabilities->navigation_capability());
  EXPECT_TRUE(navigation_capability_so.keyExists("sendLocationEnabled"));
  EXPECT_TRUE(navigation_capability_so.keyExists("getWayPointsEnabled"));
  EXPECT_TRUE(navigation_capability_so["sendLocationEnabled"].asBool());
  EXPECT_FALSE(navigation_capability_so["getWayPointsEnabled"].asBool());
  EXPECT_FALSE(navigation_capability_so["getWayPointsEnabled"].asBool());

  EXPECT_TRUE(hmi_capabilities->LoadCapabilitiesFromFile());
  EXPECT_CALL(mock_app_mngr, SetHMICooperating(true));

  // All pending RAI requests are responded
  EXPECT_CALL(mock_app_mngr, IsHMICooperating()).WillOnce(Return(true));
  hmi_capabilities->UpdateRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::UI_GetCapabilities);

  // All pending RAI requests are hold, need respond them
  EXPECT_CALL(mock_app_mngr, IsHMICooperating()).WillOnce(Return(false));
  hmi_capabilities->UpdateRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::UI_GetCapabilities);
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

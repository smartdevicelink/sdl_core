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

#include "application_manager/hmi_capabilities_impl.h"

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
const std::string kHmiCapabilitiesCacheFile = "hmi_capabilities_cache.json";
const uint32_t kEqualizerMaxChanelId = 10;
}  // namespace

using ::testing::_;
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

const std::vector<std::string> language_string_values{
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

void InitLightNameStringToEnumMap(
    LightNameCStringToEnumMap& out_light_names_map) {
  for (size_t i = 0; i < light_name_string_values.size(); ++i) {
    out_light_names_map[light_name_string_values[i]] =
        light_name_enum_values[i];
  }
}

bool LightNameStringToEnum(const std::string& light_name_str,
                           hmi_apis::Common_LightName::eType& out_value) {
  auto it = light_names_map.find(light_name_str);
  if (it == light_names_map.end()) {
    return false;
  }
  out_value = it->second;
  return true;
}

void InitLanguageStringToEnumMap(LanguageCStringToEnumMap& out_languages_map) {
  for (size_t i = 0; i < language_string_values.size(); ++i) {
    out_languages_map[language_string_values[i]] = language_enum_values[i];
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
  if (LightNameStringToEnum(light_name_str, value)) {
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
                                                        kAppInfoStorage))) {
    if (languages_map.empty()) {
      InitLanguageStringToEnumMap(languages_map);
    }
    if (light_names_map.empty()) {
      InitLightNameStringToEnumMap(light_names_map);
    }
  }

  void SetUp() OVERRIDE {
    ON_CALL(mock_app_mngr_, event_dispatcher())
        .WillByDefault(ReturnRef(mock_event_dispatcher_));
    ON_CALL(mock_app_mngr_, get_settings())
        .WillByDefault(ReturnRef(mock_application_manager_settings_));
    ON_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
        .WillByDefault(ReturnRef(kHmiCapabilitiesDefaultFile));
    ON_CALL(mock_application_manager_settings_,
            hmi_capabilities_cache_file_name())
        .WillByDefault(ReturnRef(kHmiCapabilitiesCacheFile));

    hmi_capabilities_ = std::make_shared<HMICapabilitiesImpl>(mock_app_mngr_);
  }

  void TearDown() OVERRIDE {
    DeleteFileIfExists(kHmiCapabilitiesCacheFile);
    DeleteFileIfExists(kAppInfoDataFile);
    hmi_capabilities_.reset();
  }

  void CreateFile(const std::string& file_name) {
    file_system::CreateFile(file_name);
    ASSERT_TRUE(file_system::FileExists(file_name));
  }

  void DeleteFileIfExists(const std::string& file_path) {
    if (file_system::FileExists(file_path)) {
      EXPECT_TRUE(::file_system::DeleteFile(file_path));
    }
  }

  void SetUpLanguageAndLightCapabilitiesExpectation() {
    ON_CALL(mock_app_mngr_, IsSOStructValid(_, _)).WillByDefault(Return(true));

    EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
                CommonLanguageFromString(_))
        .WillRepeatedly(Invoke(TestCommonLanguageFromString));
    EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
                CommonLightNameFromString(_))
        .WillRepeatedly(Invoke(TestCommonLightNameFromString));
  }

  std::shared_ptr<HMICapabilitiesImpl> hmi_capabilities_;
  NiceMock<MockApplicationManager> mock_app_mngr_;
  NiceMock<event_engine_test::MockEventDispatcher> mock_event_dispatcher_;
  NiceMock<MockApplicationManagerSettings> mock_application_manager_settings_;
  resumption::LastStateWrapperPtr last_state_wrapper_;
  smart_objects::CSmartSchema schema_;
};

TEST_F(HMICapabilitiesTest,
       Init_CheckActiveLanguages_SuccesSetupDefaultLanguages) {
  SetUpLanguageAndLightCapabilitiesExpectation();
  hmi_capabilities_->Init(last_state_wrapper_);

  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            hmi_capabilities_->active_ui_language());
  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            hmi_capabilities_->active_vr_language());
  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            hmi_capabilities_->active_tts_language());
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckUILanguages_SuccessLoadAndConvert) {
  SetUpLanguageAndLightCapabilitiesExpectation();
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto ui_supported_languages_so =
      *(hmi_capabilities_->ui_supported_languages());

  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            static_cast<hmi_apis::Common_Language::eType>(
                ui_supported_languages_so[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::ES_MX,
            static_cast<hmi_apis::Common_Language::eType>(
                ui_supported_languages_so[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::FR_CA,
            static_cast<hmi_apis::Common_Language::eType>(
                ui_supported_languages_so[2].asInt()));
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckVRLanguages_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto vr_supported_languages_so =
      *(hmi_capabilities_->vr_supported_languages());

  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            static_cast<hmi_apis::Common_Language::eType>(
                vr_supported_languages_so[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::ES_MX,
            static_cast<hmi_apis::Common_Language::eType>(
                vr_supported_languages_so[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::FR_CA,
            static_cast<hmi_apis::Common_Language::eType>(
                vr_supported_languages_so[2].asInt()));
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckTTSSupportedLanguages_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto tts_supported_languages_so =
      *(hmi_capabilities_->tts_supported_languages());

  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            static_cast<hmi_apis::Common_Language::eType>(
                tts_supported_languages_so[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::ES_MX,
            static_cast<hmi_apis::Common_Language::eType>(
                tts_supported_languages_so[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::FR_CA,
            static_cast<hmi_apis::Common_Language::eType>(
                tts_supported_languages_so[2].asInt()));
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckSpeechCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto tts_capabilities_so = *(hmi_capabilities_->speech_capabilities());

  EXPECT_EQ(hmi_apis::Common_SpeechCapabilities::SC_TEXT,
            static_cast<hmi_apis::Common_SpeechCapabilities::eType>(
                tts_capabilities_so[0].asInt()));
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckUIButtonCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto buttons_capabilities_so =
      *(hmi_capabilities_->button_capabilities());

  // Count of buttons in json file
  const size_t btn_length = buttons_capabilities_so.length();
  EXPECT_EQ(16ull, btn_length);
  for (size_t index = 0; index < btn_length; ++index) {
    EXPECT_TRUE(
        (buttons_capabilities_so[index])
            .keyExists(rc_rpc_plugin::enums_value::kShortPressAvailable));
    EXPECT_TRUE(
        (buttons_capabilities_so[index])
            .keyExists(rc_rpc_plugin::enums_value::kLongPressAvailable));
    EXPECT_TRUE((buttons_capabilities_so[index])
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
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckDisplayType_SuccessLoadAndConvert) {
  SetUpLanguageAndLightCapabilitiesExpectation();
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto display_capabilities_so =
      *(hmi_capabilities_->display_capabilities());

  EXPECT_EQ(hmi_apis::Common_DisplayType::SDL_GENERIC,
            static_cast<hmi_apis::Common_DisplayType::eType>(
                display_capabilities_so[hmi_response::display_type].asInt()));

  EXPECT_EQ("GENERIC_DISPLAY",
            display_capabilities_so[hmi_response::display_name].asString());

  EXPECT_TRUE(display_capabilities_so["graphicSupported"].asBool());
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckImageFields_SuccessLoadAndConvert) {
  SetUpLanguageAndLightCapabilitiesExpectation();
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto display_capabilities_so =
      *(hmi_capabilities_->display_capabilities());

  ASSERT_TRUE((display_capabilities_so).keyExists(hmi_response::image_fields));
  const size_t img_len =
      display_capabilities_so[hmi_response::image_fields].length();
  EXPECT_NE(0ull, img_len);
  for (size_t index = 0; index < img_len; ++index) {
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
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckTextFields_SuccessLoadAndConvert) {
  SetUpLanguageAndLightCapabilitiesExpectation();
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto display_capabilities_so =
      *(hmi_capabilities_->display_capabilities());

  const size_t text_len =
      display_capabilities_so[hmi_response::text_fields].length();
  EXPECT_NE(0ull, text_len);
  for (size_t index = 0; index < text_len; ++index) {
    EXPECT_TRUE((display_capabilities_so[hmi_response::text_fields][index])
                    .keyExists(strings::name));
    EXPECT_TRUE((display_capabilities_so[hmi_response::text_fields][index])
                    .keyExists(strings::character_set));
  }
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckMediaClockFormats_SuccessLoadAndConvert) {
  SetUpLanguageAndLightCapabilitiesExpectation();
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto display_capabilities_so =
      *(hmi_capabilities_->display_capabilities());

  EXPECT_TRUE(
      (display_capabilities_so).keyExists(hmi_response::media_clock_formats));
  const size_t media_length =
      display_capabilities_so[hmi_response::media_clock_formats].length();
  EXPECT_NE(0ull, media_length);
  for (size_t i = 0; i < media_length; ++i) {
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

  // TemplatesAvailable parameter could be as empty array
  ASSERT_TRUE(
      display_capabilities_so.keyExists(hmi_response::templates_available));
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckAudioPassThru_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto audio_pass_thru_capabilities_so =
      *(hmi_capabilities_->audio_pass_thru_capabilities());

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
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckHmiZoneCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto hmi_zone_capabilities_so =
      *(hmi_capabilities_->hmi_zone_capabilities());

  EXPECT_EQ(hmi_apis::Common_HmiZoneCapabilities::FRONT,
            static_cast<hmi_apis::Common_HmiZoneCapabilities::eType>(
                hmi_zone_capabilities_so.asInt()));
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckSoftButtonCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto& soft_button_capabilities_so =
      *(hmi_capabilities_->soft_button_capabilities());

  const size_t soft_butons_length = soft_button_capabilities_so.length();

  ASSERT_TRUE(soft_butons_length > 0);

  for (size_t index = 0; index < soft_butons_length; ++index) {
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
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckPresetBankCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto preset_bank_so = *(hmi_capabilities_->preset_bank_capabilities());

  EXPECT_TRUE(
      preset_bank_so.keyExists(hmi_response::on_screen_presets_available));
  EXPECT_EQ(
      smart_objects::SmartType::SmartType_Boolean,
      preset_bank_so[hmi_response::on_screen_presets_available].getType());
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckVehicleType_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto vehicle_type_so = *(hmi_capabilities_->vehicle_type());

  EXPECT_EQ("SDL", vehicle_type_so["make"].asString());
  EXPECT_EQ("Generic", vehicle_type_so["model"].asString());
  EXPECT_EQ("2019", vehicle_type_so["modelYear"].asString());
  EXPECT_EQ("SE", vehicle_type_so["trim"].asString());
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckNavigationCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto navigation_capability_so =
      *(hmi_capabilities_->navigation_capability());

  EXPECT_TRUE(navigation_capability_so.keyExists("sendLocationEnabled"));
  EXPECT_TRUE(navigation_capability_so.keyExists("getWayPointsEnabled"));
  EXPECT_TRUE(navigation_capability_so["sendLocationEnabled"].asBool());
  EXPECT_TRUE(navigation_capability_so["getWayPointsEnabled"].asBool());

  // Since we have navigation capabilities, the feature should be supported
  EXPECT_TRUE(hmi_capabilities_->navigation_supported());
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckPhoneCapability_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto phone_capability_so = *(hmi_capabilities_->phone_capability());

  EXPECT_TRUE(phone_capability_so.keyExists("dialNumberEnabled"));
  EXPECT_TRUE(phone_capability_so["dialNumberEnabled"].asBool());

  EXPECT_TRUE(hmi_capabilities_->phone_call_supported());
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckVideoStreamingCapability_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto vs_capability_so =
      *(hmi_capabilities_->video_streaming_capability());

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
  const size_t supported_formats_len =
      vs_capability_so[strings::supported_formats].length();
  EXPECT_EQ(1ull, supported_formats_len);

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

  EXPECT_TRUE(hmi_capabilities_->video_streaming_supported());
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckRemoteControlCapabilites_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto rc_capability_so = *(hmi_capabilities_->rc_capability());

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
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckRadioControlCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto rc_capability_so = *(hmi_capabilities_->rc_capability());

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
}

TEST_F(
    HMICapabilitiesTest,
    LoadCapabilitiesFromFile_CheckRCButtonCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto rc_capability_so = *(hmi_capabilities_->rc_capability());

  ASSERT_TRUE(
      rc_capability_so.keyExists(rc_rpc_plugin::strings::kbuttonCapabilities));
  const auto& button_capabilities_so =
      rc_capability_so[rc_rpc_plugin::strings::kbuttonCapabilities];

  const size_t button_capabilities_length = button_capabilities_so.length();

  for (size_t index = 0; index < button_capabilities_length; ++index) {
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
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckAudioCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto rc_capability_so = *(hmi_capabilities_->rc_capability());

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
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckSeatCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto rc_capability_so = *(hmi_capabilities_->rc_capability());

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
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFile_CheckLightCapabilities_SuccessLoadAndConvert) {
  hmi_capabilities_->Init(last_state_wrapper_);
  const auto rc_capability_so = *(hmi_capabilities_->rc_capability());

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

  const size_t supported_lights_length = supported_lights.length();
  for (size_t index = 0; index < supported_lights_length; ++index) {
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

  const size_t sup_lights_length = supported_light_so.length();

  for (size_t index = 0; index < sup_lights_length; ++index) {
    EXPECT_TRUE(
        supported_light_so[index].keyExists(rc_rpc_plugin::strings::kName));
  }
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFileAndVerifyUnsupportedSystemCapabilities) {
  const std::string hmi_capabilities_file = "hmi_capabilities_sc1.json";
  ON_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_file));

  hmi_capabilities_->Init(last_state_wrapper_);

  // Check system capabilities; only phone capability is available
  EXPECT_FALSE(hmi_capabilities_->navigation_supported());
  EXPECT_TRUE(hmi_capabilities_->phone_call_supported());
  EXPECT_FALSE(hmi_capabilities_->video_streaming_supported());
  EXPECT_FALSE(hmi_capabilities_->rc_supported());

  // verify phone capability
  const smart_objects::SmartObject phone_capability_so =
      *(hmi_capabilities_->phone_capability());
  EXPECT_TRUE(phone_capability_so.keyExists("dialNumberEnabled"));
  EXPECT_TRUE(phone_capability_so["dialNumberEnabled"].asBool());
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFileAndVerifyEmptySystemCapabilities) {
  const std::string hmi_capabilities_file = "hmi_capabilities_sc2.json";
  ON_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_file));

  hmi_capabilities_->Init(last_state_wrapper_);

  // Check system capabilities; only navigation capability is valid, the other
  // two are empty
  EXPECT_TRUE(hmi_capabilities_->navigation_supported());
  EXPECT_FALSE(hmi_capabilities_->phone_call_supported());
  EXPECT_FALSE(hmi_capabilities_->video_streaming_supported());
  EXPECT_FALSE(hmi_capabilities_->rc_supported());

  // verify navigation capabilities
  smart_objects::SmartObject navigation_capability_so =
      *(hmi_capabilities_->navigation_capability());
  EXPECT_TRUE(navigation_capability_so.keyExists("sendLocationEnabled"));
  EXPECT_TRUE(navigation_capability_so.keyExists("getWayPointsEnabled"));
  EXPECT_TRUE(navigation_capability_so["sendLocationEnabled"].asBool());
  EXPECT_FALSE(navigation_capability_so["getWayPointsEnabled"].asBool());
}

TEST_F(HMICapabilitiesTest,
       LoadCapabilitiesFromFileAndVerifyOldAudioPassThruCapabilities) {
  const std::string hmi_capabilities_file = "hmi_capabilities_old_apt.json";
  ON_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_file));

  EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
              CommonLanguageFromString(_))
      .WillRepeatedly(Invoke(TestCommonLanguageFromString));

  hmi_capabilities_->Init(last_state_wrapper_);

  // with old audio pass thru format, the object is an array containing a single
  // object
  smart_objects::SmartObjectSPtr audio_pass_thru_capabilities_so =
      hmi_capabilities_->audio_pass_thru_capabilities();
  EXPECT_EQ(smart_objects::SmartType_Array,
            audio_pass_thru_capabilities_so->getType());
  EXPECT_EQ(1ull, audio_pass_thru_capabilities_so->length());
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
  ON_CALL(mock_app_mngr_, IsSOStructValid(_, _)).WillByDefault(Return(true));

  const int32_t image_type = 1;
  smart_objects::SmartObject sm_obj;
  sm_obj[hmi_response::image_capabilities][0] = image_type;
  hmi_capabilities_->set_display_capabilities(sm_obj);

  EXPECT_TRUE(hmi_capabilities_->VerifyImageType(image_type));

  const int32_t new_image_type = 2;
  EXPECT_FALSE(hmi_capabilities_->VerifyImageType(new_image_type));
}

TEST_F(HMICapabilitiesTest, SetVRCooperating) {
  hmi_capabilities_->set_is_vr_cooperating(true);
  EXPECT_TRUE(hmi_capabilities_->is_vr_cooperating());
}

TEST_F(HMICapabilitiesTest, SetTTSCooperating) {
  hmi_capabilities_->set_is_tts_cooperating(true);
  EXPECT_TRUE(hmi_capabilities_->is_tts_cooperating());
}

TEST_F(HMICapabilitiesTest, SetUICooperating) {
  hmi_capabilities_->set_is_ui_cooperating(true);
  EXPECT_TRUE(hmi_capabilities_->is_ui_cooperating());
}

TEST_F(HMICapabilitiesTest, SetIviCooperating) {
  hmi_capabilities_->set_is_ivi_cooperating(true);
  EXPECT_TRUE(hmi_capabilities_->is_ivi_cooperating());
}

TEST_F(
    HMICapabilitiesTest,
    UpdateCapabilitiesDependingOn_ccpuVersion_FromCacheForOld_RequestForNew) {
  const std::string ccpu_version = "4.1.3.B_EB355B";
  const std::string ccpu_version_new = "5.1.3.B_EB355B";
  const std::string hmi_capabilities_invalid_file =
      "hmi_capabilities_invalid_file.json";

  ON_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_invalid_file));

  EXPECT_CALL(mock_app_mngr_, SetHMICooperating(true));
  EXPECT_CALL(mock_app_mngr_, RequestForInterfacesAvailability()).Times(2);

  hmi_capabilities_->set_ccpu_version(ccpu_version);
  hmi_capabilities_->OnSoftwareVersionReceived(ccpu_version);

  EXPECT_EQ(ccpu_version, hmi_capabilities_->ccpu_version());

  hmi_capabilities_->OnSoftwareVersionReceived(ccpu_version_new);
  EXPECT_EQ(ccpu_version_new, hmi_capabilities_->ccpu_version());
}

TEST_F(HMICapabilitiesTest,
       UpdateCapabilitiesForNew_ccpuVersion_DeleteCacheFile) {
  MockApplicationManagerSettings mock_application_manager_settings;
  const std::string ccpu_version = "4.1.3.B_EB355B";
  const std::string ccpu_version_new = "5.1.3.B_EB355B";
  const std::string hmi_capabilities_invalid_file =
      "hmi_capabilities_invalid_file.json";

  CreateFile(kHmiCapabilitiesCacheFile);

  ON_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_invalid_file));

  hmi_capabilities_->set_ccpu_version(ccpu_version);
  hmi_capabilities_->OnSoftwareVersionReceived(ccpu_version_new);
  EXPECT_EQ(ccpu_version_new, hmi_capabilities_->ccpu_version());

  EXPECT_FALSE(file_system::FileExists(kHmiCapabilitiesCacheFile));
}

TEST_F(
    HMICapabilitiesTest,
    OnSoftwareVersionReceived_CcpuMatchNoPendingRequestsForCapability_SetHMICooperatingToTrue) {
  // If local ccpu_version matches with the received ccpu_version from the HMI,
  // and cache exists, SDL Core should check if hmi_cooperating is set to true
  // if yes - SDL should respond to all pending RAI requests, if they exist.
  // hmi_cooperting is set to true when no pending capability requests exist
  const std::string ccpu_version = "4.1.3.B_EB355B";

  hmi_capabilities_->set_ccpu_version(ccpu_version);

  EXPECT_CALL(mock_app_mngr_, SetHMICooperating(true));

  hmi_capabilities_->OnSoftwareVersionReceived(ccpu_version);
}

TEST_F(
    HMICapabilitiesTest,
    OnSoftwareVersionReceived_CcpuMatchHavePendingRequestsForCapability_NoSetHMICooperatingToTrue) {
  // If local ccpu_version matches with the received ccpu_version from the HMI,
  // and there is no cache, SDL Core should check if hmi_cooperating is set to
  // true if no - SDL should suspend all RAI responses (if any) and wait for HMI
  // responses with all required capabilities. The RAI responses (if any) could
  // be handled only after hmi_cooperating is set to true, it is set when all
  // capabilities responses are received from the HMI
  const std::string ccpu_version = "4.1.3.B_EB355B";

  // Init() is required to set pending capabilities requests to the HMI
  hmi_capabilities_->Init(last_state_wrapper_);
  hmi_capabilities_->set_ccpu_version(ccpu_version);

  EXPECT_CALL(mock_app_mngr_, SetHMICooperating(true)).Times(0);

  hmi_capabilities_->OnSoftwareVersionReceived(ccpu_version);
}

TEST_F(HMICapabilitiesTest,
       CacheFileNameNotSpecified_NoNeedToSave_ReturnSuccess) {
  const std::string hmi_capabilities_empty_file_name = "";
  const std::vector<std::string> sections_to_update{hmi_response::language};

  ON_CALL(mock_application_manager_settings_,
          hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_empty_file_name));

  EXPECT_TRUE(hmi_capabilities_->SaveCachedCapabilitiesToFile(
      hmi_interface::tts, sections_to_update, schema_));
}

TEST_F(HMICapabilitiesTest, SaveCachedCapabilitiesToFile_ParseFile_Failed) {
  const std::vector<std::string> sections_to_update{hmi_response::language};

  file_system::CreateFile(kHmiCapabilitiesCacheFile);
  EXPECT_TRUE(file_system::FileExists(kHmiCapabilitiesCacheFile));

  EXPECT_FALSE(hmi_capabilities_->SaveCachedCapabilitiesToFile(
      hmi_interface::tts, sections_to_update, schema_));
}

TEST_F(HMICapabilitiesTest,
       SaveCachedCapabilitiesToFile_ParsedFieldsSave_Success) {
  const std::vector<std::string> sections_to_update{
      hmi_response::display_capabilities};
  const std::string content_to_save = "{\"field\" : \"value\" }";

  CreateFile(kHmiCapabilitiesCacheFile);

  const std::vector<uint8_t> binary_data_to_save(content_to_save.begin(),
                                                 content_to_save.end());
  file_system::Write(kHmiCapabilitiesCacheFile, binary_data_to_save);

  EXPECT_TRUE(hmi_capabilities_->SaveCachedCapabilitiesToFile(
      hmi_interface::tts, sections_to_update, schema_));

  std::string content_after_update;
  EXPECT_TRUE(
      file_system::ReadFile(kHmiCapabilitiesCacheFile, content_after_update));
  EXPECT_NE(content_to_save, content_after_update);
}

TEST_F(
    HMICapabilitiesTest,
    SaveCachedCapabilitiesToFile_LanguageIsNotTheSameAsPersisted_SaveNewLanguageToCache) {
  SetUpLanguageAndLightCapabilitiesExpectation();
  const std::string new_language = "RU_RU";
  ON_CALL(*(MockMessageHelper::message_helper_mock()),
          CommonLanguageToString(_))
      .WillByDefault(Return(new_language));

  hmi_capabilities_->Init(last_state_wrapper_);
  hmi_capabilities_->set_active_tts_language(hmi_apis::Common_Language::RU_RU);
  const std::vector<std::string> sections_to_update{hmi_response::language};
  const std::string content_to_save = "{\"TTS\": {\"language\":\"EN_US\"}}";

  CreateFile(kHmiCapabilitiesCacheFile);
  const std::vector<uint8_t> binary_data_to_save(content_to_save.begin(),
                                                 content_to_save.end());
  file_system::Write(kHmiCapabilitiesCacheFile, binary_data_to_save);

  EXPECT_TRUE(hmi_capabilities_->SaveCachedCapabilitiesToFile(
      hmi_interface::tts, sections_to_update, schema_));

  std::string content_after_update;
  ASSERT_TRUE(
      file_system::ReadFile(kHmiCapabilitiesCacheFile, content_after_update));
  EXPECT_TRUE(content_after_update.find(new_language) != std::string::npos);
}

TEST_F(HMICapabilitiesTest, PrepareJsonValueForSaving_Success) {
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

  smart_objects::SmartObject audio_capabilities_so(
      smart_objects::SmartType_Array);
  audio_capabilities_so[0][strings::sampling_rate] =
      hmi_apis::Common_SamplingRate::RATE_44KHZ;
  audio_capabilities_so[0][strings::bits_per_sample] =
      hmi_apis::Common_BitsPerSample::RATE_8_BIT;
  audio_capabilities_so[0][strings::audio_type] =
      hmi_apis::Common_AudioType::PCM;

  const std::string content_to_save = "{\"field\" : \"value\" }";
  const std::vector<uint8_t> binary_data_to_save(content_to_save.begin(),
                                                 content_to_save.end());

  hmi_capabilities_->set_audio_pass_thru_capabilities(audio_capabilities_so);

  CreateFile(kHmiCapabilitiesCacheFile);
  file_system::Write(kHmiCapabilitiesCacheFile, binary_data_to_save);

  for (size_t i = 0; i < interfaces_name.size(); ++i) {
    EXPECT_TRUE(hmi_capabilities_->SaveCachedCapabilitiesToFile(
        interfaces_name[i], sections_to_update, schema_));
  }

  std::string content_after_update;
  EXPECT_TRUE(
      file_system::ReadFile(kHmiCapabilitiesCacheFile, content_after_update));

  Json::Value root_node;
  utils::JsonReader reader;
  ASSERT_TRUE(reader.parse(content_after_update, &root_node));

  for (size_t i = 0; i < interfaces_name.size(); ++i) {
    EXPECT_TRUE(static_cast<bool>(root_node[interfaces_name[i]]));
  }
  EXPECT_TRUE(
      root_node[hmi_interface::ui][strings::audio_pass_thru_capabilities]
          .isArray());
}

TEST_F(HMICapabilitiesTest,
       OnCapabilityInitialized_RespondToAllPendingRAIRequestsIfTheyHold) {
  SetUpLanguageAndLightCapabilitiesExpectation();

  std::vector<hmi_apis::FunctionID::eType> requests_required{
      hmi_apis::FunctionID::RC_GetCapabilities,
      hmi_apis::FunctionID::VR_GetLanguage,
      hmi_apis::FunctionID::VR_GetSupportedLanguages,
      hmi_apis::FunctionID::VR_GetCapabilities,
      hmi_apis::FunctionID::TTS_GetLanguage,
      hmi_apis::FunctionID::TTS_GetSupportedLanguages,
      hmi_apis::FunctionID::TTS_GetCapabilities,
      hmi_apis::FunctionID::Buttons_GetCapabilities,
      hmi_apis::FunctionID::VehicleInfo_GetVehicleType,
      hmi_apis::FunctionID::UI_GetCapabilities,
      hmi_apis::FunctionID::UI_GetLanguage,
      hmi_apis::FunctionID::UI_GetSupportedLanguages};

  // Contains only UI capabilities
  const std::string hmi_capabilities_file = "hmi_capabilities_sc2.json";
  ON_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_file));

  // Initializes the UI capabilities with the default values
  // Other interfaces are absent and appropriate requests should be sent
  hmi_capabilities_->Init(last_state_wrapper_);

  ON_CALL(mock_app_mngr_, IsHMICooperating()).WillByDefault(Return(false));

  // Sets isHMICooperating flag to true after all required capabilities are
  // received from HMI
  EXPECT_CALL(mock_app_mngr_, SetHMICooperating(true));

  for (const auto& request_id : requests_required) {
    hmi_capabilities_->UpdateRequestsRequiredForCapabilities(request_id);
  }
}

TEST_F(HMICapabilitiesTest,
       OnlyUICapabilitiesInCacheFile_RequestRequiredForOtherInterfaces) {
  SetUpLanguageAndLightCapabilitiesExpectation();

  std::vector<hmi_apis::FunctionID::eType> requests_required{
      hmi_apis::FunctionID::RC_GetCapabilities,
      hmi_apis::FunctionID::VR_GetLanguage,
      hmi_apis::FunctionID::VR_GetSupportedLanguages,
      hmi_apis::FunctionID::VR_GetCapabilities,
      hmi_apis::FunctionID::TTS_GetLanguage,
      hmi_apis::FunctionID::TTS_GetSupportedLanguages,
      hmi_apis::FunctionID::TTS_GetCapabilities,
      hmi_apis::FunctionID::Buttons_GetCapabilities,
      hmi_apis::FunctionID::VehicleInfo_GetVehicleType};

  const std::string hmi_capabilities_cache_file =
      "hmi_capabilities_cache_test.json";
  CreateFile(hmi_capabilities_cache_file);
  const std::string predefined_ui_capabilities =
      "{\"UI\" : {\"language\" : "
      "\"EN-US\",\"languages\":[],\"displayCapabilities\" : "
      "{},\"audioPassThruCapabilities\":[],\"pcmStreamCapabilities\" : "
      "{},\"hmiZoneCapabilities\": \"\",\"softButtonCapabilities\" : "
      "[],\"systemCapabilities\" : {}}}";

  const std::vector<uint8_t> binary_data_to_save(
      predefined_ui_capabilities.begin(), predefined_ui_capabilities.end());

  file_system::Write(hmi_capabilities_cache_file, binary_data_to_save);

  ON_CALL(mock_application_manager_settings_,
          hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_cache_file));

  hmi_capabilities_->Init(last_state_wrapper_);

  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::UI_GetLanguage));
  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::UI_GetSupportedLanguages));
  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::UI_GetCapabilities));

  for (const auto& item : requests_required) {
    EXPECT_TRUE(hmi_capabilities_->IsRequestsRequiredForCapabilities(item));
  }

  DeleteFileIfExists(hmi_capabilities_cache_file);
}

TEST_F(HMICapabilitiesTest,
       OnlyRCCapabilitiesInCacheFile_RequestRequiredForOtherInterfaces) {
  SetUpLanguageAndLightCapabilitiesExpectation();

  std::vector<hmi_apis::FunctionID::eType> requests_required{
      hmi_apis::FunctionID::UI_GetLanguage,
      hmi_apis::FunctionID::UI_GetSupportedLanguages,
      hmi_apis::FunctionID::UI_GetCapabilities,
      hmi_apis::FunctionID::VR_GetLanguage,
      hmi_apis::FunctionID::VR_GetSupportedLanguages,
      hmi_apis::FunctionID::VR_GetCapabilities,
      hmi_apis::FunctionID::TTS_GetLanguage,
      hmi_apis::FunctionID::TTS_GetSupportedLanguages,
      hmi_apis::FunctionID::TTS_GetCapabilities,
      hmi_apis::FunctionID::Buttons_GetCapabilities,
      hmi_apis::FunctionID::VehicleInfo_GetVehicleType};

  const std::string hmi_capabilities_cache_file =
      "hmi_capabilities_cache_test.json";
  CreateFile(hmi_capabilities_cache_file);
  const std::string predefined_rc_capabilities =
      "{\"RC\" : {\"remoteControlCapability\" : {\"buttonCapabilities\": "
      "[],\"climateControlCapabilities\": [],\"radioControlCapabilities\": "
      "[],\"audioControlCapabilities\": [],\"seatControlCapabilities\": "
      "[],\"lightControlCapabilities\": {},\"hmiSettingsControlCapabilities\": "
      "{}},\"seatLocationCapability\": {}}}}";

  const std::vector<uint8_t> binary_data_to_save(
      predefined_rc_capabilities.begin(), predefined_rc_capabilities.end());

  file_system::Write(hmi_capabilities_cache_file, binary_data_to_save);

  ON_CALL(mock_application_manager_settings_,
          hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_cache_file));

  hmi_capabilities_->Init(last_state_wrapper_);

  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::RC_GetCapabilities));

  for (const auto& item : requests_required) {
    EXPECT_TRUE(hmi_capabilities_->IsRequestsRequiredForCapabilities(item));
  }
}

TEST_F(HMICapabilitiesTest,
       OnlyVRCapabilitiesInCacheFile_RequestRequiredForOtherInterfaces) {
  SetUpLanguageAndLightCapabilitiesExpectation();

  std::vector<hmi_apis::FunctionID::eType> requests_required{
      hmi_apis::FunctionID::UI_GetLanguage,
      hmi_apis::FunctionID::UI_GetSupportedLanguages,
      hmi_apis::FunctionID::RC_GetCapabilities,
      hmi_apis::FunctionID::UI_GetCapabilities,
      hmi_apis::FunctionID::TTS_GetLanguage,
      hmi_apis::FunctionID::TTS_GetSupportedLanguages,
      hmi_apis::FunctionID::TTS_GetCapabilities,
      hmi_apis::FunctionID::Buttons_GetCapabilities,
      hmi_apis::FunctionID::VehicleInfo_GetVehicleType};

  const std::string hmi_capabilities_cache_file =
      "hmi_capabilities_cache_test.json";
  CreateFile(hmi_capabilities_cache_file);
  const std::string predefined_vr_capabilities =
      "{\"VR\": {\"vrCapabilities\": [],\"language\": \"\",\"languages\": []}}";

  const std::vector<uint8_t> binary_data_to_save(
      predefined_vr_capabilities.begin(), predefined_vr_capabilities.end());

  file_system::Write(hmi_capabilities_cache_file, binary_data_to_save);

  ON_CALL(mock_application_manager_settings_,
          hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_cache_file));

  hmi_capabilities_->Init(last_state_wrapper_);

  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::VR_GetLanguage));
  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::VR_GetSupportedLanguages));
  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::VR_GetCapabilities));

  for (const auto& item : requests_required) {
    EXPECT_TRUE(hmi_capabilities_->IsRequestsRequiredForCapabilities(item));
  }
}

TEST_F(HMICapabilitiesTest,
       OnlyTTSCapabilitiesInCacheFile_RequestRequiredForOtherInterfaces) {
  SetUpLanguageAndLightCapabilitiesExpectation();

  std::vector<hmi_apis::FunctionID::eType> requests_required{
      hmi_apis::FunctionID::UI_GetLanguage,
      hmi_apis::FunctionID::UI_GetSupportedLanguages,
      hmi_apis::FunctionID::UI_GetCapabilities,
      hmi_apis::FunctionID::RC_GetCapabilities,
      hmi_apis::FunctionID::VR_GetLanguage,
      hmi_apis::FunctionID::VR_GetSupportedLanguages,
      hmi_apis::FunctionID::VR_GetCapabilities,
      hmi_apis::FunctionID::Buttons_GetCapabilities,
      hmi_apis::FunctionID::VehicleInfo_GetVehicleType};

  const std::string hmi_capabilities_cache_file =
      "hmi_capabilities_cache_test.json";
  CreateFile(hmi_capabilities_cache_file);
  const std::string predefined_tts_capabilities =
      "{\"TTS\": {\"speechCapabilities\": [],\"prerecordedSpeechCapabilities\" "
      ": [],\"language\": \"\",\"languages\": "
      "[]}}";

  const std::vector<uint8_t> binary_data_to_save(
      predefined_tts_capabilities.begin(), predefined_tts_capabilities.end());

  file_system::Write(hmi_capabilities_cache_file, binary_data_to_save);

  ON_CALL(mock_application_manager_settings_,
          hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_cache_file));

  hmi_capabilities_->Init(last_state_wrapper_);

  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::TTS_GetLanguage));
  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::TTS_GetSupportedLanguages));
  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::TTS_GetCapabilities));

  for (const auto& item : requests_required) {
    EXPECT_TRUE(hmi_capabilities_->IsRequestsRequiredForCapabilities(item));
  }
}

TEST_F(HMICapabilitiesTest,
       OnlyButtonsSCapabilitiesInCacheFile_RequestRequiredForOtherInterfaces) {
  SetUpLanguageAndLightCapabilitiesExpectation();

  std::vector<hmi_apis::FunctionID::eType> requests_required{
      hmi_apis::FunctionID::UI_GetLanguage,
      hmi_apis::FunctionID::UI_GetSupportedLanguages,
      hmi_apis::FunctionID::UI_GetCapabilities,
      hmi_apis::FunctionID::RC_GetCapabilities,
      hmi_apis::FunctionID::TTS_GetLanguage,
      hmi_apis::FunctionID::TTS_GetSupportedLanguages,
      hmi_apis::FunctionID::TTS_GetCapabilities,
      hmi_apis::FunctionID::VR_GetLanguage,
      hmi_apis::FunctionID::VR_GetSupportedLanguages,
      hmi_apis::FunctionID::VR_GetCapabilities,
      hmi_apis::FunctionID::VehicleInfo_GetVehicleType};

  const std::string hmi_capabilities_cache_file =
      "hmi_capabilities_cache_test.json";
  CreateFile(hmi_capabilities_cache_file);
  const std::string predefined_buttons_capabilities =
      "{\"Buttons\": {\"capabilities\": [],\"presetBankCapabilities\": {}}}";

  const std::vector<uint8_t> binary_data_to_save(
      predefined_buttons_capabilities.begin(),
      predefined_buttons_capabilities.end());

  file_system::Write(hmi_capabilities_cache_file, binary_data_to_save);

  ON_CALL(mock_application_manager_settings_,
          hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_cache_file));

  hmi_capabilities_->Init(last_state_wrapper_);

  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::Buttons_GetCapabilities));

  for (const auto& item : requests_required) {
    EXPECT_TRUE(hmi_capabilities_->IsRequestsRequiredForCapabilities(item));
  }
}

TEST_F(HMICapabilitiesTest,
       OnlyVehicleInfoInCacheFile_RequestRequiredForOtherInterfaces) {
  SetUpLanguageAndLightCapabilitiesExpectation();

  std::vector<hmi_apis::FunctionID::eType> requests_required{
      hmi_apis::FunctionID::UI_GetLanguage,
      hmi_apis::FunctionID::UI_GetSupportedLanguages,
      hmi_apis::FunctionID::UI_GetCapabilities,
      hmi_apis::FunctionID::RC_GetCapabilities,
      hmi_apis::FunctionID::TTS_GetLanguage,
      hmi_apis::FunctionID::TTS_GetSupportedLanguages,
      hmi_apis::FunctionID::TTS_GetCapabilities,
      hmi_apis::FunctionID::VR_GetLanguage,
      hmi_apis::FunctionID::VR_GetSupportedLanguages,
      hmi_apis::FunctionID::VR_GetCapabilities,
      hmi_apis::FunctionID::Buttons_GetCapabilities};

  const std::string hmi_capabilities_cache_file =
      "hmi_capabilities_cache_test.json";
  CreateFile(hmi_capabilities_cache_file);
  const std::string predefined_vi_capabilities =
      "{\"VehicleInfo\": { \"vehicleType\" : {} }}";

  const std::vector<uint8_t> binary_data_to_save(
      predefined_vi_capabilities.begin(), predefined_vi_capabilities.end());

  file_system::Write(hmi_capabilities_cache_file, binary_data_to_save);

  ON_CALL(mock_application_manager_settings_,
          hmi_capabilities_cache_file_name())
      .WillByDefault(ReturnRef(hmi_capabilities_cache_file));

  hmi_capabilities_->Init(last_state_wrapper_);

  EXPECT_FALSE(hmi_capabilities_->IsRequestsRequiredForCapabilities(
      hmi_apis::FunctionID::VehicleInfo_GetVehicleType));

  for (const auto& item : requests_required) {
    EXPECT_TRUE(hmi_capabilities_->IsRequestsRequiredForCapabilities(item));
  }
}

TEST_F(
    HMICapabilitiesTest,
    ConvertJsonArrayToSoArray_ConvertPrerecordedSpeech_SuccessConvertFromStringToEnum) {
  SetUpLanguageAndLightCapabilitiesExpectation();

  CreateFile(kHmiCapabilitiesCacheFile);
  const std::string prerecordedSpeechCapabilities =
      "{ \"TTS\" :{"
      "\"prerecordedSpeechCapabilities\" :["
      "\"HELP_JINGLE\","
      "\"INITIAL_JINGLE\","
      "\"LISTEN_JINGLE\","
      "\"POSITIVE_JINGLE\","
      "\"NEGATIVE_JINGLE\"]}"
      "}";

  const std::vector<uint8_t> binary_data_to_save(
      prerecordedSpeechCapabilities.begin(),
      prerecordedSpeechCapabilities.end());
  file_system::Write(kHmiCapabilitiesCacheFile, binary_data_to_save);

  hmi_capabilities_->Init(last_state_wrapper_);

  const auto tts_capabilities_so = *(hmi_capabilities_->prerecorded_speech());

  EXPECT_EQ(hmi_apis::Common_PrerecordedSpeech::HELP_JINGLE,
            static_cast<hmi_apis::Common_PrerecordedSpeech::eType>(
                tts_capabilities_so[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_PrerecordedSpeech::INITIAL_JINGLE,
            static_cast<hmi_apis::Common_PrerecordedSpeech::eType>(
                tts_capabilities_so[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_PrerecordedSpeech::LISTEN_JINGLE,
            static_cast<hmi_apis::Common_PrerecordedSpeech::eType>(
                tts_capabilities_so[2].asInt()));
  EXPECT_EQ(hmi_apis::Common_PrerecordedSpeech::POSITIVE_JINGLE,
            static_cast<hmi_apis::Common_PrerecordedSpeech::eType>(
                tts_capabilities_so[3].asInt()));
  EXPECT_EQ(hmi_apis::Common_PrerecordedSpeech::NEGATIVE_JINGLE,
            static_cast<hmi_apis::Common_PrerecordedSpeech::eType>(
                tts_capabilities_so[4].asInt()));
}

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

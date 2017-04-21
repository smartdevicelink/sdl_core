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

#include "application_manager/hmi_capabilities.h"
#include "gtest/gtest.h"
#include "smart_objects/smart_object.h"
#include "application_manager/mock_message_helper.h"
#include "smart_objects/enum_schema_item.h"
#include "interfaces/HMI_API.h"
#include "utils/make_shared.h"
#include "application_manager/hmi_capabilities_for_testing.h"
#include "utils/file_system.h"
#include "application_manager/mock_application_manager.h"
#include "application_manager/mock_application_manager_settings.h"
#include "application_manager/mock_event_dispatcher.h"
#include "application_manager/state_controller.h"
#include "resumption/last_state.h"
#include "application_manager/resumption/resume_ctrl.h"

namespace test {
namespace components {
namespace application_manager_test {

using ::testing::_;
using ::testing::Return;
using ::testing::ReturnRef;
using ::testing::AtLeast;
using ::testing::Invoke;
using ::testing::InSequence;

using namespace application_manager;

class HMICapabilitiesTest : public ::testing::Test {
 protected:
  HMICapabilitiesTest()
      : last_state_("app_storage_folder", "app_info_data")
      , file_name_("hmi_capabilities.json") {}
  virtual void SetUp() OVERRIDE {
    EXPECT_CALL(app_mngr_, event_dispatcher())
        .WillOnce(ReturnRef(mock_event_dispatcher));
    EXPECT_CALL(app_mngr_, get_settings())
        .WillRepeatedly(ReturnRef(mock_application_manager_settings_));
    EXPECT_CALL(mock_application_manager_settings_,
                hmi_capabilities_file_name()).WillOnce(ReturnRef(file_name_));
    EXPECT_CALL(mock_event_dispatcher, add_observer(_, _, _)).Times(1);
    EXPECT_CALL(mock_event_dispatcher, remove_observer(_)).Times(1);
    EXPECT_CALL(mock_application_manager_settings_, launch_hmi())
        .WillOnce(Return(false));
    hmi_capabilities_test =
        utils::MakeShared<HMICapabilitiesForTesting>(app_mngr_);
    hmi_capabilities_test->Init(&last_state_);
  }

  void TearDown() OVERRIDE {
    hmi_capabilities_test.reset();
  }
  static void TearDownTestCase() {
    if (file_system::FileExists("./app_info_data")) {
      EXPECT_TRUE(::file_system::DeleteFile("./app_info_data"));
    }
  }

  void SetCooperating();
  MockApplicationManager app_mngr_;
  event_engine_test::MockEventDispatcher mock_event_dispatcher;
  resumption::LastState last_state_;
  MockApplicationManagerSettings mock_application_manager_settings_;
  utils::SharedPtr<HMICapabilitiesForTesting> hmi_capabilities_test;
  const std::string file_name_;
};

const char* const cstring_values_[] = {
    "EN_US", "ES_MX", "FR_CA", "DE_DE", "ES_ES", "EN_GB", "RU_RU", "TR_TR",
    "PL_PL", "FR_FR", "IT_IT", "SV_SE", "PT_PT", "NL_NL", "EN_AU", "ZH_CN",
    "ZH_TW", "JA_JP", "AR_SA", "KO_KR", "PT_BR", "CS_CZ", "DA_DK", "NO_NO",
    "NL_BE", "EL_GR", "HU_HU", "FI_FI", "SK_SK"};

const hmi_apis::Common_Language::eType enum_values_[] = {
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
    hmi_apis::Common_Language::SK_SK};

struct CStringComparator {
  bool operator()(const char* a, const char* b) {
    return strcmp(a, b) < 0;
  }
};

typedef std::map<const char*,
                 hmi_apis::Common_Language::eType,
                 CStringComparator> CStringToEnumMap;

CStringToEnumMap InitCStringToEnumMap() {
  size_t value = sizeof(cstring_values_) / sizeof(cstring_values_[0]);
  CStringToEnumMap result;
  for (size_t i = 0; i < value; ++i) {
    result[cstring_values_[i]] = enum_values_[i];
  }
  return result;
}

bool StringToEnum(const char* str, hmi_apis::Common_Language::eType& value) {
  size_t count_value = sizeof(cstring_values_) / sizeof(cstring_values_[0]);
  CStringToEnumMap result;
  for (size_t i = 0; i < count_value; ++i) {
    result[cstring_values_[i]] = enum_values_[i];
  }

  CStringToEnumMap::const_iterator it = result.find(str);
  if (it == result.end()) {
    return false;
  }
  value = it->second;
  return true;
}

hmi_apis::Common_Language::eType TestCommonLanguageFromString(
    const std::string& language) {
  hmi_apis::Common_Language::eType value;
  if (StringToEnum(language.c_str(), value)) {
    return value;
  }
  return hmi_apis::Common_Language::INVALID_ENUM;
}

TEST_F(HMICapabilitiesTest, LoadCapabilitiesFromFile) {
  const std::string hmi_capabilities_file = "hmi_capabilities.json";
  EXPECT_CALL(mock_application_manager_settings_, hmi_capabilities_file_name())
      .WillOnce(ReturnRef(hmi_capabilities_file));
  EXPECT_CALL(*(MockMessageHelper::message_helper_mock()),
              CommonLanguageFromString(_))
      .WillRepeatedly(Invoke(TestCommonLanguageFromString));

  if (file_system::FileExists("./app_info_data")) {
    EXPECT_TRUE(::file_system::DeleteFile("./app_info_data"));
  }
  EXPECT_TRUE(hmi_capabilities_test->LoadCapabilitiesFromFile());

  // Check active languages
  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            hmi_capabilities_test->active_ui_language());
  EXPECT_EQ(hmi_apis::Common_Language::ES_MX,
            hmi_capabilities_test->active_vr_language());
  EXPECT_EQ(hmi_apis::Common_Language::DE_DE,
            hmi_capabilities_test->active_tts_language());

  // Check UI languages
  const smart_objects::SmartObject ui_supported_languages =
      *(hmi_capabilities_test->ui_supported_languages());

  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            static_cast<hmi_apis::Common_Language::eType>(
                ui_supported_languages[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::ES_MX,
            static_cast<hmi_apis::Common_Language::eType>(
                ui_supported_languages[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::FR_CA,
            static_cast<hmi_apis::Common_Language::eType>(
                ui_supported_languages[2].asInt()));

  // Check VR languages
  const smart_objects::SmartObject vr_supported_languages =
      *(hmi_capabilities_test->vr_supported_languages());

  EXPECT_EQ(hmi_apis::Common_Language::AR_SA,
            static_cast<hmi_apis::Common_Language::eType>(
                vr_supported_languages[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::EN_US,
            static_cast<hmi_apis::Common_Language::eType>(
                vr_supported_languages[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::ES_MX,
            static_cast<hmi_apis::Common_Language::eType>(
                vr_supported_languages[2].asInt()));

  // Check TTS languages
  const smart_objects::SmartObject tts_supported_languages =
      *(hmi_capabilities_test->tts_supported_languages());

  EXPECT_EQ(hmi_apis::Common_Language::DA_DK,
            static_cast<hmi_apis::Common_Language::eType>(
                tts_supported_languages[0].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::CS_CZ,
            static_cast<hmi_apis::Common_Language::eType>(
                tts_supported_languages[1].asInt()));
  EXPECT_EQ(hmi_apis::Common_Language::KO_KR,
            static_cast<hmi_apis::Common_Language::eType>(
                tts_supported_languages[2].asInt()));

  // Check button capabilities
  const smart_objects::SmartObject buttons_capabilities_so =
      *(hmi_capabilities_test->button_capabilities());

  // Count of buttons in json file
  const uint32_t btn_length = buttons_capabilities_so.length();
  EXPECT_EQ(15u, btn_length);
  for (uint32_t i = 0; i < btn_length; ++i) {
    EXPECT_TRUE((buttons_capabilities_so[i]).keyExists(strings::name));
    EXPECT_TRUE((buttons_capabilities_so[i]).keyExists("shortPressAvailable"));
    EXPECT_TRUE((buttons_capabilities_so[i]).keyExists("longPressAvailable"));
    EXPECT_TRUE((buttons_capabilities_so[i]).keyExists("upDownAvailable"));
    EXPECT_TRUE(buttons_capabilities_so[i]["shortPressAvailable"].asBool());
    EXPECT_TRUE(buttons_capabilities_so[i]["longPressAvailable"].asBool());
    EXPECT_TRUE(buttons_capabilities_so[i]["upDownAvailable"].asBool());
  }
  const smart_objects::SmartObject display_capabilities_so =
      *(hmi_capabilities_test->display_capabilities());

  // Check display type
  EXPECT_EQ(hmi_apis::Common_DisplayType::GEN2_8_DMA,
            static_cast<hmi_apis::Common_DisplayType::eType>(
                display_capabilities_so[hmi_response::display_type].asInt()));

  EXPECT_TRUE(display_capabilities_so["graphicSupported"].asBool());

  // Check text fields
  const uint32_t text_len =
      display_capabilities_so[hmi_response::text_fields].length();
  EXPECT_NE(0u, text_len);
  for (uint32_t i = 0; i < text_len; ++i) {
    EXPECT_TRUE((display_capabilities_so[hmi_response::text_fields][i])
                    .keyExists(strings::name));
    EXPECT_TRUE((display_capabilities_so[hmi_response::text_fields][i])
                    .keyExists(strings::character_set));
  }

  // Check image fields
  EXPECT_TRUE((display_capabilities_so).keyExists(hmi_response::image_fields));
  const uint32_t img_len =
      display_capabilities_so[hmi_response::image_fields].length();
  EXPECT_NE(0u, img_len);
  for (uint32_t i = 0; i < img_len; ++i) {
    EXPECT_TRUE((display_capabilities_so[hmi_response::image_fields][i])
                    .keyExists(strings::name));
    EXPECT_TRUE((display_capabilities_so[hmi_response::image_fields][i])
                    .keyExists(strings::image_type_supported));
    if (display_capabilities_so[hmi_response::image_fields][i][strings::name] ==
        hmi_apis::Common_ImageFieldName::locationImage) {
      EXPECT_EQ(hmi_apis::Common_FileType::GRAPHIC_PNG,
                static_cast<hmi_apis::Common_FileType::eType>(
                    display_capabilities_so[hmi_response::image_fields][i]
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

  EXPECT_TRUE(
      (display_capabilities_so).keyExists(hmi_response::image_capabilities));
  EXPECT_EQ(hmi_apis::Common_ImageType::DYNAMIC,
            static_cast<hmi_apis::Common_ImageType::eType>(
                display_capabilities_so[hmi_response::image_capabilities][0]
                    .asInt()));
  EXPECT_EQ(hmi_apis::Common_ImageType::STATIC,
            static_cast<hmi_apis::Common_ImageType::eType>(
                display_capabilities_so[hmi_response::image_capabilities][1]
                    .asInt()));

  // Check audio pass thru
  const smart_objects::SmartObject audio_pass_thru_capabilities_so =
      *(hmi_capabilities_test->audio_pass_thru_capabilities());
  EXPECT_EQ(hmi_apis::Common_SamplingRate::RATE_44KHZ,
            static_cast<hmi_apis::Common_SamplingRate::eType>(
                audio_pass_thru_capabilities_so["samplingRate"].asInt()));
  EXPECT_EQ(hmi_apis::Common_BitsPerSample::RATE_8_BIT,
            static_cast<hmi_apis::Common_BitsPerSample::eType>(
                audio_pass_thru_capabilities_so["bitsPerSample"].asInt()));
  EXPECT_EQ(hmi_apis::Common_AudioType::PCM,
            static_cast<hmi_apis::Common_AudioType::eType>(
                audio_pass_thru_capabilities_so["audioType"].asInt()));

  // Check hmi zone capabilities
  const smart_objects::SmartObject hmi_zone_capabilities_so =
      *(hmi_capabilities_test->hmi_zone_capabilities());
  EXPECT_EQ(hmi_apis::Common_HmiZoneCapabilities::FRONT,
            static_cast<hmi_apis::Common_HmiZoneCapabilities::eType>(
                hmi_zone_capabilities_so.asInt()));

  const smart_objects::SmartObject soft_button_capabilities_so =
      *(hmi_capabilities_test->soft_button_capabilities());

  EXPECT_TRUE(soft_button_capabilities_so[0]["shortPressAvailable"].asBool());
  EXPECT_TRUE(soft_button_capabilities_so[0]["longPressAvailable"].asBool());
  EXPECT_TRUE(soft_button_capabilities_so[0]["upDownAvailable"].asBool());
  EXPECT_TRUE(soft_button_capabilities_so[0]["imageSupported"].asBool());

  const smart_objects::SmartObject preset_bank_so =
      *(hmi_capabilities_test->preset_bank_capabilities());
  EXPECT_TRUE(preset_bank_so["onScreenPresetsAvailable"].asBool());

  // Check vehicle type
  const smart_objects::SmartObject vehicle_type_so =
      *(hmi_capabilities_test->vehicle_type());
  EXPECT_TRUE(preset_bank_so["onScreenPresetsAvailable"].asBool());

  EXPECT_EQ("Ford", vehicle_type_so["make"].asString());
  EXPECT_EQ("Fiesta", vehicle_type_so["model"].asString());
  EXPECT_EQ("2013", vehicle_type_so["modelYear"].asString());
  EXPECT_EQ("SE", vehicle_type_so["trim"].asString());
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
              CreateModuleInfoSO(_, _)).WillRepeatedly(Return(test_so));
  EXPECT_CALL(app_mngr_, ManageHMICommand(_)).WillRepeatedly(Return(true));
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

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

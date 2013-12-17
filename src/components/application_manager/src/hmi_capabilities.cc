/**
 * Copyright (c) 2013, Ford Motor Company
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

#include <map>
#include "json/json.h"
#include "utils/file_system.h"
#include "interfaces/HMI_API.h"
#include "config_profile/profile.h"
#include "smart_objects/smart_object.h"
#include "application_manager/message_helper.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"

namespace application_manager {

std::map<std::string, hmi_apis::Common_Language::eType> languages_enum_values =
{
    {"EN_US", hmi_apis::Common_Language::EN_US},
    {"ES_MX", hmi_apis::Common_Language::ES_MX},
    {"FR_CA", hmi_apis::Common_Language::FR_CA},
    {"DE_DE", hmi_apis::Common_Language::DE_DE},
    {"ES_ES", hmi_apis::Common_Language::ES_ES},
    {"EN_GB", hmi_apis::Common_Language::EN_GB},
    {"TR_TR", hmi_apis::Common_Language::RU_RU},
    {"TR_TR", hmi_apis::Common_Language::TR_TR},
    {"PL_PL", hmi_apis::Common_Language::PL_PL},
    {"FR_FR", hmi_apis::Common_Language::FR_FR},
    {"IT_IT", hmi_apis::Common_Language::IT_IT},
    {"SV_SE", hmi_apis::Common_Language::SV_SE},
    {"PT_PT", hmi_apis::Common_Language::PT_PT},
    {"EN_AU", hmi_apis::Common_Language::NL_NL},
    {"EN_AU", hmi_apis::Common_Language::EN_AU},
    {"ZH_CN", hmi_apis::Common_Language::ZH_CN},
    {"ZH_TW", hmi_apis::Common_Language::ZH_TW},
    {"JA_JP", hmi_apis::Common_Language::JA_JP},
    {"AR_SA", hmi_apis::Common_Language::AR_SA},
    {"KO_KR", hmi_apis::Common_Language::KO_KR},
    {"PT_BR", hmi_apis::Common_Language::PT_BR},
    {"CS_CZ", hmi_apis::Common_Language::CS_CZ},
    {"DA_DK", hmi_apis::Common_Language::DA_DK},
    {"NO_NO", hmi_apis::Common_Language::NO_NO}
};

std::map<std::string, hmi_apis::Common_VrCapabilities::eType> vr_enum_capabilities =
{
    {"TEXT", hmi_apis::Common_VrCapabilities::VR_TEXT}
};

std::map<std::string, hmi_apis::Common_ButtonName::eType> button_enum_name =
{
    {"OK"             , hmi_apis::Common_ButtonName::OK},
    {"SEEKLEFT"       , hmi_apis::Common_ButtonName::SEEKLEFT},
    {"SEEKRIGHT"      , hmi_apis::Common_ButtonName::SEEKRIGHT},
    {"TUNEUP"         , hmi_apis::Common_ButtonName::TUNEUP},
    {"TUNEDOWN"       , hmi_apis::Common_ButtonName::TUNEDOWN},
    {"PRESET_0"       , hmi_apis::Common_ButtonName::PRESET_0},
    {"PRESET_1"       , hmi_apis::Common_ButtonName::PRESET_1},
    {"PRESET_2"       , hmi_apis::Common_ButtonName::PRESET_2},
    {"PRESET_3"       , hmi_apis::Common_ButtonName::PRESET_3},
    {"PRESET_4"       , hmi_apis::Common_ButtonName::PRESET_4},
    {"PRESET_5"       , hmi_apis::Common_ButtonName::PRESET_5},
    {"PRESET_6"       , hmi_apis::Common_ButtonName::PRESET_6},
    {"PRESET_7"       , hmi_apis::Common_ButtonName::PRESET_7},
    {"PRESET_8"       , hmi_apis::Common_ButtonName::PRESET_8},
    {"PRESET_9"       , hmi_apis::Common_ButtonName::PRESET_9},
    {"CUSTOM_BUTTON"  , hmi_apis::Common_ButtonName::CUSTOM_BUTTON},
    {"SEARCH"         , hmi_apis::Common_ButtonName::SEARCH},

};

std::map<std::string, hmi_apis::Common_TextFieldName::eType> text_fields_enum_name =
{
    {"mainField1", hmi_apis::Common_TextFieldName::mainField1},
    {"mainField2", hmi_apis::Common_TextFieldName::mainField2},
    {"mainField3", hmi_apis::Common_TextFieldName::mainField3},
    {"mainField4", hmi_apis::Common_TextFieldName::mainField4},
    {"statusBar" , hmi_apis::Common_TextFieldName::statusBar},
    {"mediaClock", hmi_apis::Common_TextFieldName::mediaClock},
    {"mediaTrack", hmi_apis::Common_TextFieldName::mediaTrack},
    {"alertText1", hmi_apis::Common_TextFieldName::alertText1},
    {"alertText2", hmi_apis::Common_TextFieldName::alertText2},
    {"alertText3", hmi_apis::Common_TextFieldName::alertText3},
    {"scrollableMessageBody" , hmi_apis::Common_TextFieldName::scrollableMessageBody},
    {"initialInteractionText", hmi_apis::Common_TextFieldName::initialInteractionText},
    {"navigationText1"       , hmi_apis::Common_TextFieldName::navigationText1},
    {"navigationText2"       , hmi_apis::Common_TextFieldName::navigationText2},
    {"ETA"                   , hmi_apis::Common_TextFieldName::ETA},
    {"totalDistance"         , hmi_apis::Common_TextFieldName::totalDistance},
    {"audioPassThruDisplayText1", hmi_apis::Common_TextFieldName::audioPassThruDisplayText1},
    {"audioPassThruDisplayText2", hmi_apis::Common_TextFieldName::audioPassThruDisplayText2},
    {"sliderHeader"     , hmi_apis::Common_TextFieldName::sliderHeader},
    {"sliderFooter"     , hmi_apis::Common_TextFieldName::sliderFooter},
    {"notificationText" , hmi_apis::Common_TextFieldName::notificationText},
    {"menuName"         , hmi_apis::Common_TextFieldName::menuName},
    {"secondaryText"    , hmi_apis::Common_TextFieldName::secondaryText},
    {"tertiaryText"     , hmi_apis::Common_TextFieldName::tertiaryText},
    {"timeToDestination", hmi_apis::Common_TextFieldName::timeToDestination},
    {"turnText"         , hmi_apis::Common_TextFieldName::turnText}
};

std::map<std::string, hmi_apis::Common_MediaClockFormat::eType> media_clock_enum_name =
{
    {"CLOCK1"    , hmi_apis::Common_MediaClockFormat::CLOCK1},
    {"CLOCK2"    , hmi_apis::Common_MediaClockFormat::CLOCK2},
    {"CLOCK3"    , hmi_apis::Common_MediaClockFormat::CLOCK3},
    {"CLOCKTEXT1", hmi_apis::Common_MediaClockFormat::CLOCKTEXT1},
    {"CLOCKTEXT2", hmi_apis::Common_MediaClockFormat::CLOCKTEXT2},
    {"CLOCKTEXT3", hmi_apis::Common_MediaClockFormat::CLOCKTEXT3},
    {"CLOCKTEXT4", hmi_apis::Common_MediaClockFormat::CLOCKTEXT4},
};

std::map<std::string, hmi_apis::Common_ImageType::eType> image_type_enum =
{
    {"STATIC" , hmi_apis::Common_ImageType::STATIC},
    {"DYNAMIC", hmi_apis::Common_ImageType::DYNAMIC}
};

std::map<std::string, hmi_apis::Common_SamplingRate::eType> sampling_rate_enum =
{
    {"8KHZ" , hmi_apis::Common_SamplingRate::RATE_8KHZ},
    {"16KHZ", hmi_apis::Common_SamplingRate::RATE_16KHZ},
    {"22KHZ", hmi_apis::Common_SamplingRate::RATE_22KHZ},
    {"44KHZ", hmi_apis::Common_SamplingRate::RATE_44KHZ}
};

std::map<std::string, hmi_apis::Common_BitsPerSample::eType> bit_per_sample_enum =
{
    {"RATE_8_BIT", hmi_apis::Common_BitsPerSample::RATE_8_BIT},
    {"RATE_16_BIT", hmi_apis::Common_BitsPerSample::RATE_16_BIT}
};

std::map<std::string, hmi_apis::Common_AudioType::eType> audio_type_enum =
{
    {"PCM", hmi_apis::Common_AudioType::PCM}
};

std::map<std::string, hmi_apis::Common_HmiZoneCapabilities::eType> hmi_zone_enum =
{
    {"FRONT", hmi_apis::Common_HmiZoneCapabilities::FRONT},
    {"BACK", hmi_apis::Common_HmiZoneCapabilities::BACK},
};



HMICapabilities::HMICapabilities(ApplicationManagerImpl* const app_mngr)
  : is_vr_cooperating_(false),
    is_tts_cooperating_(false),
    is_ui_cooperating_(false),
    is_navi_cooperating_(false),
    is_ivi_cooperating_(false),
    is_vr_ready_response_recieved_(false),
    is_tts_ready_response_recieved_(false),
    is_ui_ready_response_recieved_(false),
    is_navi_ready_response_recieved_(false),
    is_ivi_ready_response_recieved_(false),
    ui_language_(hmi_apis::Common_Language::INVALID_ENUM),
    vr_language_(hmi_apis::Common_Language::INVALID_ENUM),
    tts_language_(hmi_apis::Common_Language::INVALID_ENUM),
    vehicle_type_(NULL),
    attenuated_supported_(false),
    ui_supported_languages_(NULL),
    tts_supported_languages_(NULL),
    vr_supported_languages_(NULL),
    display_capabilities_(NULL),
    hmi_zone_capabilities_(NULL),
    soft_buttons_capabilities_(NULL),
    button_capabilities_(NULL),
    preset_bank_capabilities_(NULL),
    vr_capabilities_(NULL),
    speech_capabilities_(NULL),
    audio_pass_thru_capabilities_(NULL),
    app_mngr_(app_mngr){

  if (false == profile::Profile::instance()->launch_hmi()) {
    if (load_capabilities_from_file()) {
      is_vr_ready_response_recieved_ = true;
      is_tts_ready_response_recieved_ = true;
      is_ui_ready_response_recieved_ = true;
      is_navi_ready_response_recieved_ = true;
      is_ivi_ready_response_recieved_ = true;

      is_vr_cooperating_ = true;
      is_tts_cooperating_ = true;
      is_ui_cooperating_ = true;
      is_navi_cooperating_ = true;
      is_ivi_cooperating_ = true;
    }
  }
}

HMICapabilities::~HMICapabilities() {
  delete vehicle_type_;
  delete ui_supported_languages_;
  delete tts_supported_languages_;
  delete vr_supported_languages_;
  delete display_capabilities_;
  delete hmi_zone_capabilities_;
  delete soft_buttons_capabilities_;
  delete button_capabilities_;
  delete preset_bank_capabilities_;
  delete vr_capabilities_;
  delete speech_capabilities_;
  delete audio_pass_thru_capabilities_;
  app_mngr_ = NULL;
}

bool HMICapabilities::is_hmi_capabilities_initialized() const {
  bool result = true;

  if (is_vr_ready_response_recieved_ && is_tts_ready_response_recieved_
      && is_ui_ready_response_recieved_ && is_navi_ready_response_recieved_
      && is_ivi_ready_response_recieved_) {
    if (is_vr_cooperating_) {
      if ((!vr_supported_languages_) ||
          (hmi_apis::Common_Language::INVALID_ENUM == vr_language_)) {
        result = false;
      }
    }

    if (is_tts_cooperating_) {
      if ((!tts_supported_languages_) ||
          (hmi_apis::Common_Language::INVALID_ENUM == tts_language_)) {
        result = false;
      }
    }

    if (is_ui_cooperating_) {
      if ((!ui_supported_languages_)  ||
          (hmi_apis::Common_Language::INVALID_ENUM == ui_language_)) {
         result = false;
      }
    }

    if (is_ivi_cooperating_) {
      if (!vehicle_type_) {
        result = false;
      }
    }
  } else {
    result = false;
  }

  return result;
}

bool HMICapabilities::VerifyImageType(int image_type) const {
  if (!display_capabilities_) {
    return false;
  }

  if (display_capabilities_->keyExists(hmi_response::image_capabilities)) {
    const smart_objects::SmartObject& image_caps = display_capabilities_
        ->getElement(hmi_response::image_capabilities);
    for (int i = 0; i < image_caps.length(); ++i) {
      if (image_caps.getElement(i).asInt() == image_type) {
        return true;
      }
    }
  }

  return false;
}

void HMICapabilities::set_is_vr_cooperating(bool value) {
  is_vr_ready_response_recieved_ = true;
  is_vr_cooperating_ = value;
  if (is_vr_cooperating_) {
    utils::SharedPtr<smart_objects::SmartObject> get_language(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetLanguage));
    app_mngr_->ManageHMICommand(get_language);
    utils::SharedPtr<smart_objects::SmartObject> get_all_languages(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetSupportedLanguages));
    app_mngr_->ManageHMICommand(get_all_languages);
    utils::SharedPtr<smart_objects::SmartObject> get_capabilities(
      MessageHelper::CreateModuleInfoSO(hmi_apis::FunctionID::VR_GetCapabilities));
    app_mngr_->ManageHMICommand(get_capabilities);

    MessageHelper::SendHelpVrCommand();
  }
}

void HMICapabilities::set_is_tts_cooperating(bool value) {
  is_tts_ready_response_recieved_ = true;
  is_tts_cooperating_ = value;
  if (is_tts_cooperating_) {
    utils::SharedPtr<smart_objects::SmartObject> get_language(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::TTS_GetLanguage));
    app_mngr_->ManageHMICommand(get_language);
    utils::SharedPtr<smart_objects::SmartObject> get_all_languages(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::TTS_GetSupportedLanguages));
    app_mngr_->ManageHMICommand(get_all_languages);
    utils::SharedPtr<smart_objects::SmartObject> get_capabilities(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::TTS_GetCapabilities));
    app_mngr_->ManageHMICommand(get_capabilities);
  }
}

void HMICapabilities::set_is_ui_cooperating(bool value) {
  is_ui_ready_response_recieved_ = true;
  is_ui_cooperating_ = value;
  if (is_ui_cooperating_) {
    utils::SharedPtr<smart_objects::SmartObject> get_language(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::UI_GetLanguage));
    app_mngr_->ManageHMICommand(get_language);
    utils::SharedPtr<smart_objects::SmartObject> get_all_languages(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::UI_GetSupportedLanguages));
    app_mngr_->ManageHMICommand(get_all_languages);
    utils::SharedPtr<smart_objects::SmartObject> get_capabilities(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::UI_GetCapabilities));
    app_mngr_->ManageHMICommand(get_capabilities);
  }
}

void HMICapabilities::set_is_navi_cooperating(bool value) {
  is_navi_ready_response_recieved_ = true;
  is_navi_cooperating_ = value;
}

void HMICapabilities::set_is_ivi_cooperating(bool value) {
  is_ivi_ready_response_recieved_ = true;
  is_ivi_cooperating_ = value;
  if (is_ivi_cooperating_) {
    utils::SharedPtr<smart_objects::SmartObject> get_type(
      MessageHelper::CreateModuleInfoSO(
        hmi_apis::FunctionID::VehicleInfo_GetVehicleType));
    app_mngr_->ManageHMICommand(get_type);
  }
}

void HMICapabilities::set_attenuated_supported(bool state) {
  attenuated_supported_ = state;
}

void HMICapabilities::set_active_ui_language(
  const hmi_apis::Common_Language::eType& language) {
  ui_language_ = language;
}

void HMICapabilities::set_active_vr_language(
  const hmi_apis::Common_Language::eType& language) {
  vr_language_ = language;
}

void HMICapabilities::set_active_tts_language(
  const hmi_apis::Common_Language::eType& language) {
  tts_language_ = language;
}

void HMICapabilities::set_ui_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (ui_supported_languages_) {
    delete ui_supported_languages_;
  }
  ui_supported_languages_ = new smart_objects::SmartObject(supported_languages);
}

void HMICapabilities::set_tts_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (tts_supported_languages_) {
    delete tts_supported_languages_;
  }
  tts_supported_languages_ = new smart_objects::SmartObject(
      supported_languages);
}

void HMICapabilities::set_vr_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (vr_supported_languages_) {
    delete vr_supported_languages_;
  }
  vr_supported_languages_ = new smart_objects::SmartObject(supported_languages);
}

void HMICapabilities::set_display_capabilities(
    const smart_objects::SmartObject& display_capabilities) {
  if (display_capabilities_) {
    delete display_capabilities_;
  }
  display_capabilities_ = new smart_objects::SmartObject(display_capabilities);
}

void HMICapabilities::set_hmi_zone_capabilities(
    const smart_objects::SmartObject& hmi_zone_capabilities) {
  if (hmi_zone_capabilities_) {
    delete hmi_zone_capabilities_;
  }
  hmi_zone_capabilities_ = new smart_objects::SmartObject(
      hmi_zone_capabilities);
}

void HMICapabilities::set_soft_button_capabilities(
    const smart_objects::SmartObject& soft_button_capabilities) {
  if (soft_buttons_capabilities_) {
    delete soft_buttons_capabilities_;
  }
  soft_buttons_capabilities_ = new smart_objects::SmartObject(
      soft_button_capabilities);
}

void HMICapabilities::set_button_capabilities(
    const smart_objects::SmartObject& button_capabilities) {
  if (button_capabilities_) {
    delete button_capabilities_;
  }
  button_capabilities_ = new smart_objects::SmartObject(button_capabilities);
}

void HMICapabilities::set_vr_capabilities(
    const smart_objects::SmartObject& vr_capabilities) {
  if (vr_capabilities_) {
    delete vr_capabilities_;
  }
  vr_capabilities_ = new smart_objects::SmartObject(vr_capabilities);
}

void HMICapabilities::set_speech_capabilities(
    const smart_objects::SmartObject& speech_capabilities) {
  if (speech_capabilities_) {
    delete speech_capabilities_;
  }
  speech_capabilities_ = new smart_objects::SmartObject(speech_capabilities);
}

void HMICapabilities::set_audio_pass_thru_capabilities(
    const smart_objects::SmartObject& audio_pass_thru_capabilities) {
  if (audio_pass_thru_capabilities_) {
    delete audio_pass_thru_capabilities_;
  }
  audio_pass_thru_capabilities_ = new smart_objects::SmartObject(
      audio_pass_thru_capabilities);
}

void HMICapabilities::set_preset_bank_capabilities(
    const smart_objects::SmartObject& preset_bank_capabilities) {
  if (preset_bank_capabilities_) {
    delete preset_bank_capabilities_;
  }
  preset_bank_capabilities_ = new smart_objects::SmartObject(
      preset_bank_capabilities);
}

void HMICapabilities::set_vehicle_type(
  const smart_objects::SmartObject& vehicle_type) {
  if (vehicle_type_) {
    delete vehicle_type_;
  }
  vehicle_type_ = new smart_objects::SmartObject(vehicle_type);
}

bool HMICapabilities::load_capabilities_from_file() {
  std::string json_string;
  std::string file_name =
      profile::Profile::instance()->hmi_capabilities_file_name();

  if (!file_system::FileExists(file_name)) {
    return false;
  }

  if (!file_system::ReadFile(file_name, json_string)) {
    return false;
  }

  try {
    Json::Reader reader_;
    Json::Value  root_json;

    bool result = reader_.parse(json_string, root_json, false);
    if (!result) {
      return false;
    }

     // UI
     Json::Value ui = root_json.get("UI", "");
     set_active_ui_language(
         languages_enum_values.find(ui.get("language", "").asString())->second);

     Json::Value languages = ui.get("languages", "");
     smart_objects::SmartObject ui_languages =
         smart_objects::SmartObject(smart_objects::SmartType_Array);
     for (int i = 0; i < languages.size(); i++){
       ui_languages[i] =
           languages_enum_values.find(languages[i].asString())->second;
     }
     set_ui_supported_languages(ui_languages);

     Json::Value display_capabilities = ui.get("displayCapabilities", "");
     smart_objects::SmartObject display_capabilities_so =
              smart_objects::SmartObject(smart_objects::SmartType_Map);
     display_capabilities_so["displayType"] =
         display_capabilities.get("displayType", "").asString();

     display_capabilities_so["textFields"] =
         smart_objects::SmartObject(smart_objects::SmartType_Array);

     Json::Value text_fields = display_capabilities.get("textFields", "");
     for (int i = 0; i < text_fields.size(); i++) {

       // there is an issue with enum to string, therefore used string
       display_capabilities_so["textFields"][i]["name"] =
           text_fields_enum_name.find(text_fields[i].asString())->first;
     }

     display_capabilities_so["mediaClockFormats"] =
              smart_objects::SmartObject(smart_objects::SmartType_Array);
     Json::Value media_clock_format =
         display_capabilities.get("mediaClockFormats", "");
     for (int i = 0; i < media_clock_format.size(); i++) {
       display_capabilities_so["mediaClockFormats"][i] =
           media_clock_enum_name.find(media_clock_format[i].asString())->second;
     }
     display_capabilities_so["graphicSupported"] =
         display_capabilities.get("graphicSupported", "").asBool();

     Json::Value image_capabilities =
         display_capabilities.get("imageCapabilities", "");
     display_capabilities_so["imageCapabilities"] =
              smart_objects::SmartObject(smart_objects::SmartType_Array);
     for (int i = 0; i < image_capabilities.size(); i++) {
       display_capabilities_so["imageCapabilities"][i] =
           image_type_enum.find(image_capabilities[i].asString())->second;
     }
     set_display_capabilities(display_capabilities_so);

     Json::Value audio_capabilities = ui.get("audioPassThruCapabilities", "");
     smart_objects::SmartObject audio_capabilities_so =
              smart_objects::SmartObject(smart_objects::SmartType_Array);
     int i = 0;
     audio_capabilities_so[i] =
                   smart_objects::SmartObject(smart_objects::SmartType_Map);
     audio_capabilities_so[i]["samplingRate"] =
         sampling_rate_enum.find(
             audio_capabilities.get("samplingRate", "").asString())->second;
     audio_capabilities_so[i]["bitsPerSample"] =
         bit_per_sample_enum.find(
             audio_capabilities.get("bitsPerSample", "").asString())->second;
     audio_capabilities_so[i]["audioType"] =
         audio_type_enum.find(
             audio_capabilities.get("audioType", "").asString())->second;
     set_audio_pass_thru_capabilities(audio_capabilities_so);

     smart_objects::SmartObject hmi_zone_capabilities_so =
         smart_objects::SmartObject(smart_objects::SmartType_Array);
     int index = 0;
     hmi_zone_capabilities_so[index] =
         hmi_zone_enum.find(ui.get("hmiZoneCapabilities", "").asString())->second;
     set_hmi_zone_capabilities(hmi_zone_capabilities_so);

     Json::Value soft_button_capabilities = ui.get("softButtonCapabilities", "");
     smart_objects::SmartObject soft_button_capabilities_so =
         smart_objects::SmartObject(smart_objects::SmartType_Map);
     soft_button_capabilities_so["shortPressAvailable"] =
         soft_button_capabilities.get("shortPressAvailable", "").asBool();
     soft_button_capabilities_so["longPressAvailable"] =
         soft_button_capabilities.get("longPressAvailable", "").asBool();
     soft_button_capabilities_so["upDownAvailable"] =
         soft_button_capabilities.get("upDownAvailable", "").asBool();
     soft_button_capabilities_so["imageSupported"] =
         soft_button_capabilities.get("imageSupported", "").asBool();
     set_soft_button_capabilities(soft_button_capabilities_so);

     // VR
     Json::Value vr = root_json.get("VR", "");
     set_active_vr_language(
         languages_enum_values.find(vr.get("language", "").asString())->second);

     languages = vr.get("languages", "");
     smart_objects::SmartObject vr_languages =
         smart_objects::SmartObject(smart_objects::SmartType_Array);
     for (int i = 0; i < languages.size(); i++){
       vr_languages[i] =
           languages_enum_values.find(languages[i].asString())->second;
     }
     set_vr_supported_languages(ui_languages);

     Json::Value capabilities = vr.get("capabilities", "");
     smart_objects::SmartObject vr_capabilities =
         smart_objects::SmartObject(smart_objects::SmartType_Array);
     for (int i = 0; i < capabilities.size(); i++){
       vr_capabilities[i] =
           vr_enum_capabilities.find(capabilities[i].asString())->second;
     }
     set_vr_capabilities(vr_capabilities);


     // TTS
     Json::Value tts = root_json.get("TTS", "");
     set_active_tts_language(
         languages_enum_values.find(tts.get("language", "").asString())->second);

     languages = tts.get("languages", "");
     smart_objects::SmartObject tts_languages =
         smart_objects::SmartObject(smart_objects::SmartType_Array);
     for (int i = 0; i < languages.size(); i++){
       tts_languages[i] =
           languages_enum_values.find(languages[i].asString())->second;
     }
     set_tts_supported_languages(ui_languages);
     set_speech_capabilities(
         smart_objects::SmartObject(tts.get("capabilities", "").asString()));


     // Buttons
     Json::Value buttons = root_json.get("Buttons", "");
     Json::Value bt_capabilities = buttons.get("capabilities", "");
     smart_objects::SmartObject buttons_capabilities =
         smart_objects::SmartObject(smart_objects::SmartType_Array);
     for (int i = 0; i < bt_capabilities.size(); i++){
       smart_objects::SmartObject button =
                smart_objects::SmartObject(smart_objects::SmartType_Map);
       button["name"] =
           button_enum_name.find(bt_capabilities[i].get("name", "").asString())->second;
       button["shortPressAvailable"] =
           bt_capabilities[i].get("shortPressAvailable", "").asBool();
       button["longPressAvailable"] =
           bt_capabilities[i].get("longPressAvailable", "").asBool();
       button["upDownAvailable"] =
           bt_capabilities[i].get("upDownAvailable", "").asBool();

       buttons_capabilities[i] = button;
     }
     set_button_capabilities(buttons_capabilities);

     Json::Value presetBank = buttons.get("presetBankCapabilities", "");
     smart_objects::SmartObject preset_bank =
         smart_objects::SmartObject(smart_objects::SmartType_Map);
     preset_bank["onScreenPresetsAvailable"] =
         presetBank.get("onScreenPresetsAvailable", "").asBool();
     set_preset_bank_capabilities(preset_bank);


     // VehicleType
     Json::Value vehicle_info = root_json.get("VehicleInfo", "");
     smart_objects::SmartObject vehicle_type =
         smart_objects::SmartObject(smart_objects::SmartType_Map);
     vehicle_type["make"] = vehicle_info.get("make", "").asString();
     vehicle_type["model"] = vehicle_info.get("model", "").asString();
     vehicle_type["modelYear"] = vehicle_info.get("modelYear", "").asString();
     vehicle_type["trim"] = vehicle_info.get("trim", "").asString();
     set_vehicle_type(vehicle_type);

  } catch (...) {
    return false;
  }

  return true;
}

}  //  namespace application_manager

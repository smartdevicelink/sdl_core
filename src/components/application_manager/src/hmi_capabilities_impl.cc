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

#include <map>

#include "utils/logger.h"
#include "application_manager/hmi_capabilities_impl.h"
#include "application_manager/application_manager_impl.h"
#include "smart_objects/smart_object.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "config_profile/profile.h"
#include "formatters/CFormatterJsonBase.h"
#include "interfaces/HMI_API.h"
#include "utils/file_system.h"

namespace application_manager {
namespace Formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

namespace {
std::map<std::string, hmi_apis::Common_VrCapabilities::eType>
    vr_enum_capabilities;
std::map<std::string, hmi_apis::Common_SpeechCapabilities::eType>
    tts_enum_capabilities;
std::map<std::string, hmi_apis::Common_ButtonName::eType> button_enum_name;
std::map<std::string, hmi_apis::Common_TextFieldName::eType>
    text_fields_enum_name;
std::map<std::string, hmi_apis::Common_MediaClockFormat::eType>
    media_clock_enum_name;
std::map<std::string, hmi_apis::Common_ImageType::eType> image_type_enum;
std::map<std::string, hmi_apis::Common_SamplingRate::eType> sampling_rate_enum;
std::map<std::string, hmi_apis::Common_BitsPerSample::eType>
    bit_per_sample_enum;
std::map<std::string, hmi_apis::Common_AudioType::eType> audio_type_enum;
std::map<std::string, hmi_apis::Common_HmiZoneCapabilities::eType>
    hmi_zone_enum;
std::map<std::string, hmi_apis::Common_ImageFieldName::eType>
    image_field_name_enum;
std::map<std::string, hmi_apis::Common_FileType::eType> file_type_enum;
std::map<std::string, hmi_apis::Common_DisplayType::eType> display_type_enum;
std::map<std::string, hmi_apis::Common_CharacterSet::eType> character_set_enum;
std::map<std::string, hmi_apis::Common_VideoStreamingProtocol::eType>
    video_streaming_protocol_enum;
std::map<std::string, hmi_apis::Common_VideoStreamingCodec::eType>
    video_streaming_codec_enum;

void InitCapabilities() {
  vr_enum_capabilities.insert(std::make_pair(
      std::string("TEXT"), hmi_apis::Common_VrCapabilities::VR_TEXT));

  tts_enum_capabilities.insert(std::make_pair(
      std::string("TEXT"), hmi_apis::Common_SpeechCapabilities::SC_TEXT));
  tts_enum_capabilities.insert(
      std::make_pair(std::string("SAPI_PHONEMES"),
                     hmi_apis::Common_SpeechCapabilities::SAPI_PHONEMES));
  tts_enum_capabilities.insert(
      std::make_pair(std::string("LHPLUS_PHONEMES"),
                     hmi_apis::Common_SpeechCapabilities::LHPLUS_PHONEMES));
  tts_enum_capabilities.insert(
      std::make_pair(std::string("SAPI_PHONEMES"),
                     hmi_apis::Common_SpeechCapabilities::SAPI_PHONEMES));
  tts_enum_capabilities.insert(
      std::make_pair(std::string("PRE_RECORDED"),
                     hmi_apis::Common_SpeechCapabilities::PRE_RECORDED));
  tts_enum_capabilities.insert(std::make_pair(
      std::string("SILENCE"), hmi_apis::Common_SpeechCapabilities::SILENCE));

  button_enum_name.insert(
      std::make_pair(std::string("OK"), hmi_apis::Common_ButtonName::OK));
  button_enum_name.insert(std::make_pair(
      std::string("SEEKLEFT"), hmi_apis::Common_ButtonName::SEEKLEFT));
  button_enum_name.insert(std::make_pair(
      std::string("SEEKRIGHT"), hmi_apis::Common_ButtonName::SEEKRIGHT));
  button_enum_name.insert(std::make_pair(std::string("TUNEUP"),
                                         hmi_apis::Common_ButtonName::TUNEUP));
  button_enum_name.insert(std::make_pair(
      std::string("TUNEDOWN"), hmi_apis::Common_ButtonName::TUNEDOWN));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_0"), hmi_apis::Common_ButtonName::PRESET_0));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_1"), hmi_apis::Common_ButtonName::PRESET_1));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_2"), hmi_apis::Common_ButtonName::PRESET_2));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_3"), hmi_apis::Common_ButtonName::PRESET_3));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_4"), hmi_apis::Common_ButtonName::PRESET_4));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_5"), hmi_apis::Common_ButtonName::PRESET_5));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_6"), hmi_apis::Common_ButtonName::PRESET_6));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_7"), hmi_apis::Common_ButtonName::PRESET_7));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_8"), hmi_apis::Common_ButtonName::PRESET_8));
  button_enum_name.insert(std::make_pair(
      std::string("PRESET_9"), hmi_apis::Common_ButtonName::PRESET_9));
  button_enum_name.insert(
      std::make_pair(std::string("CUSTOM_BUTTON"),
                     hmi_apis::Common_ButtonName::CUSTOM_BUTTON));
  button_enum_name.insert(std::make_pair(std::string("SEARCH"),
                                         hmi_apis::Common_ButtonName::SEARCH));

  text_fields_enum_name.insert(std::make_pair(
      std::string("mainField1"), hmi_apis::Common_TextFieldName::mainField1));
  text_fields_enum_name.insert(std::make_pair(
      std::string("mainField2"), hmi_apis::Common_TextFieldName::mainField2));
  text_fields_enum_name.insert(std::make_pair(
      std::string("mainField3"), hmi_apis::Common_TextFieldName::mainField3));
  text_fields_enum_name.insert(std::make_pair(
      std::string("mainField4"), hmi_apis::Common_TextFieldName::mainField4));
  text_fields_enum_name.insert(std::make_pair(
      std::string("statusBar"), hmi_apis::Common_TextFieldName::statusBar));
  text_fields_enum_name.insert(std::make_pair(
      std::string("mediaClock"), hmi_apis::Common_TextFieldName::mediaClock));
  text_fields_enum_name.insert(std::make_pair(
      std::string("mediaTrack"), hmi_apis::Common_TextFieldName::mediaTrack));
  text_fields_enum_name.insert(std::make_pair(
      std::string("alertText1"), hmi_apis::Common_TextFieldName::alertText1));
  text_fields_enum_name.insert(std::make_pair(
      std::string("alertText2"), hmi_apis::Common_TextFieldName::alertText2));
  text_fields_enum_name.insert(std::make_pair(
      std::string("alertText3"), hmi_apis::Common_TextFieldName::alertText3));
  text_fields_enum_name.insert(
      std::make_pair(std::string("scrollableMessageBody"),
                     hmi_apis::Common_TextFieldName::scrollableMessageBody));
  text_fields_enum_name.insert(
      std::make_pair(std::string("initialInteractionText"),
                     hmi_apis::Common_TextFieldName::initialInteractionText));
  text_fields_enum_name.insert(
      std::make_pair(std::string("navigationText1"),
                     hmi_apis::Common_TextFieldName::navigationText1));
  text_fields_enum_name.insert(
      std::make_pair(std::string("navigationText2"),
                     hmi_apis::Common_TextFieldName::navigationText2));
  text_fields_enum_name.insert(
      std::make_pair(std::string("ETA"), hmi_apis::Common_TextFieldName::ETA));
  text_fields_enum_name.insert(
      std::make_pair(std::string("totalDistance"),
                     hmi_apis::Common_TextFieldName::totalDistance));
  text_fields_enum_name.insert(std::make_pair(
      std::string("audioPassThruDisplayText1"),
      hmi_apis::Common_TextFieldName::audioPassThruDisplayText1));
  text_fields_enum_name.insert(std::make_pair(
      std::string("audioPassThruDisplayText2"),
      hmi_apis::Common_TextFieldName::audioPassThruDisplayText2));
  text_fields_enum_name.insert(
      std::make_pair(std::string("sliderHeader"),
                     hmi_apis::Common_TextFieldName::sliderHeader));
  text_fields_enum_name.insert(
      std::make_pair(std::string("sliderFooter"),
                     hmi_apis::Common_TextFieldName::sliderFooter));
  text_fields_enum_name.insert(
      std::make_pair(std::string("navigationText"),
                     hmi_apis::Common_TextFieldName::navigationText));
  text_fields_enum_name.insert(
      std::make_pair(std::string("notificationText"),
                     hmi_apis::Common_TextFieldName::notificationText));
  text_fields_enum_name.insert(std::make_pair(
      std::string("menuName"), hmi_apis::Common_TextFieldName::menuName));
  text_fields_enum_name.insert(
      std::make_pair(std::string("secondaryText"),
                     hmi_apis::Common_TextFieldName::secondaryText));
  text_fields_enum_name.insert(
      std::make_pair(std::string("tertiaryText"),
                     hmi_apis::Common_TextFieldName::tertiaryText));
  text_fields_enum_name.insert(
      std::make_pair(std::string("timeToDestination"),
                     hmi_apis::Common_TextFieldName::timeToDestination));
  text_fields_enum_name.insert(
      std::make_pair(std::string("locationName"),
                     hmi_apis::Common_TextFieldName::locationName));
  text_fields_enum_name.insert(
      std::make_pair(std::string("locationDescription"),
                     hmi_apis::Common_TextFieldName::locationDescription));
  text_fields_enum_name.insert(std::make_pair(
      std::string("turnText"), hmi_apis::Common_TextFieldName::turnText));
  text_fields_enum_name.insert(
      std::make_pair(std::string("addressLines"),
                     hmi_apis::Common_TextFieldName::addressLines));
  text_fields_enum_name.insert(std::make_pair(
      std::string("phoneNumber"), hmi_apis::Common_TextFieldName::phoneNumber));
  text_fields_enum_name.insert(std::make_pair(
      std::string("turnText"), hmi_apis::Common_TextFieldName::turnText));
  text_fields_enum_name.insert(std::make_pair(
      std::string("menuTitle"), hmi_apis::Common_TextFieldName::menuTitle));

  media_clock_enum_name.insert(std::make_pair(
      std::string("CLOCK1"), hmi_apis::Common_MediaClockFormat::CLOCK1));
  media_clock_enum_name.insert(std::make_pair(
      std::string("CLOCK2"), hmi_apis::Common_MediaClockFormat::CLOCK2));
  media_clock_enum_name.insert(std::make_pair(
      std::string("CLOCK3"), hmi_apis::Common_MediaClockFormat::CLOCK3));
  media_clock_enum_name.insert(
      std::make_pair(std::string("CLOCKTEXT1"),
                     hmi_apis::Common_MediaClockFormat::CLOCKTEXT1));
  media_clock_enum_name.insert(
      std::make_pair(std::string("CLOCKTEXT2"),
                     hmi_apis::Common_MediaClockFormat::CLOCKTEXT2));
  media_clock_enum_name.insert(
      std::make_pair(std::string("CLOCKTEXT3"),
                     hmi_apis::Common_MediaClockFormat::CLOCKTEXT3));
  media_clock_enum_name.insert(
      std::make_pair(std::string("CLOCKTEXT4"),
                     hmi_apis::Common_MediaClockFormat::CLOCKTEXT4));

  image_type_enum.insert(std::make_pair(std::string("STATIC"),
                                        hmi_apis::Common_ImageType::STATIC));
  image_type_enum.insert(std::make_pair(std::string("DYNAMIC"),
                                        hmi_apis::Common_ImageType::DYNAMIC));

  sampling_rate_enum.insert(std::make_pair(
      std::string("8KHZ"), hmi_apis::Common_SamplingRate::RATE_8KHZ));
  sampling_rate_enum.insert(std::make_pair(
      std::string("16KHZ"), hmi_apis::Common_SamplingRate::RATE_16KHZ));
  sampling_rate_enum.insert(std::make_pair(
      std::string("22KHZ"), hmi_apis::Common_SamplingRate::RATE_22KHZ));
  sampling_rate_enum.insert(std::make_pair(
      std::string("44KHZ"), hmi_apis::Common_SamplingRate::RATE_44KHZ));

  bit_per_sample_enum.insert(std::make_pair(
      std::string("RATE_8_BIT"), hmi_apis::Common_BitsPerSample::RATE_8_BIT));
  bit_per_sample_enum.insert(std::make_pair(
      std::string("RATE_16_BIT"), hmi_apis::Common_BitsPerSample::RATE_16_BIT));

  audio_type_enum.insert(
      std::make_pair(std::string("PCM"), hmi_apis::Common_AudioType::PCM));

  hmi_zone_enum.insert(std::make_pair(
      std::string("FRONT"), hmi_apis::Common_HmiZoneCapabilities::FRONT));
  hmi_zone_enum.insert(std::make_pair(
      std::string("BACK"), hmi_apis::Common_HmiZoneCapabilities::BACK));

  image_field_name_enum.insert(
      std::make_pair(std::string("softButtonImage"),
                     hmi_apis::Common_ImageFieldName::softButtonImage));
  image_field_name_enum.insert(
      std::make_pair(std::string("choiceImage"),
                     hmi_apis::Common_ImageFieldName::choiceImage));
  image_field_name_enum.insert(
      std::make_pair(std::string("choiceSecondaryImage"),
                     hmi_apis::Common_ImageFieldName::choiceSecondaryImage));
  image_field_name_enum.insert(std::make_pair(
      std::string("vrHelpItem"), hmi_apis::Common_ImageFieldName::vrHelpItem));
  image_field_name_enum.insert(std::make_pair(
      std::string("turnIcon"), hmi_apis::Common_ImageFieldName::turnIcon));
  image_field_name_enum.insert(std::make_pair(
      std::string("menuIcon"), hmi_apis::Common_ImageFieldName::menuIcon));
  image_field_name_enum.insert(std::make_pair(
      std::string("cmdIcon"), hmi_apis::Common_ImageFieldName::cmdIcon));
  image_field_name_enum.insert(std::make_pair(
      std::string("appIcon"), hmi_apis::Common_ImageFieldName::appIcon));
  image_field_name_enum.insert(std::make_pair(
      std::string("graphic"), hmi_apis::Common_ImageFieldName::graphic));
  image_field_name_enum.insert(
      std::make_pair(std::string("showConstantTBTIcon"),
                     hmi_apis::Common_ImageFieldName::showConstantTBTIcon));
  image_field_name_enum.insert(std::make_pair(
      std::string("showConstantTBTNextTurnIcon"),
      hmi_apis::Common_ImageFieldName::showConstantTBTNextTurnIcon));
  image_field_name_enum.insert(
      std::make_pair(std::string("locationImage"),
                     hmi_apis::Common_ImageFieldName::locationImage));

  file_type_enum.insert(std::make_pair(std::string("GRAPHIC_BMP"),
                                       hmi_apis::Common_FileType::GRAPHIC_BMP));
  file_type_enum.insert(std::make_pair(
      std::string("GRAPHIC_JPEG"), hmi_apis::Common_FileType::GRAPHIC_JPEG));
  file_type_enum.insert(std::make_pair(std::string("GRAPHIC_PNG"),
                                       hmi_apis::Common_FileType::GRAPHIC_PNG));
  file_type_enum.insert(std::make_pair(std::string("AUDIO_WAVE"),
                                       hmi_apis::Common_FileType::AUDIO_WAVE));
  file_type_enum.insert(std::make_pair(std::string("AUDIO_MP3"),
                                       hmi_apis::Common_FileType::AUDIO_MP3));
  file_type_enum.insert(std::make_pair(std::string("AUDIO_AAC"),
                                       hmi_apis::Common_FileType::AUDIO_AAC));
  file_type_enum.insert(
      std::make_pair(std::string("BINARY"), hmi_apis::Common_FileType::BINARY));
  file_type_enum.insert(
      std::make_pair(std::string("JSON"), hmi_apis::Common_FileType::JSON));

  display_type_enum.insert(
      std::make_pair(std::string("CID"), hmi_apis::Common_DisplayType::CID));
  display_type_enum.insert(std::make_pair(std::string("TYPE2"),
                                          hmi_apis::Common_DisplayType::TYPE2));
  display_type_enum.insert(std::make_pair(std::string("TYPE5"),
                                          hmi_apis::Common_DisplayType::TYPE5));
  display_type_enum.insert(
      std::make_pair(std::string("NGN"), hmi_apis::Common_DisplayType::NGN));
  display_type_enum.insert(std::make_pair(
      std::string("GEN2_8_DMA"), hmi_apis::Common_DisplayType::GEN2_8_DMA));
  display_type_enum.insert(std::make_pair(
      std::string("GEN2_6_DMA"), hmi_apis::Common_DisplayType::GEN2_6_DMA));
  display_type_enum.insert(
      std::make_pair(std::string("MFD3"), hmi_apis::Common_DisplayType::MFD3));
  display_type_enum.insert(
      std::make_pair(std::string("MFD4"), hmi_apis::Common_DisplayType::MFD4));
  display_type_enum.insert(
      std::make_pair(std::string("MFD5"), hmi_apis::Common_DisplayType::MFD5));
  display_type_enum.insert(std::make_pair(
      std::string("GEN3_8_INCH"), hmi_apis::Common_DisplayType::GEN3_8_INCH));

  character_set_enum.insert(std::make_pair(
      std::string("TYPE2SET"), hmi_apis::Common_CharacterSet::TYPE2SET));
  character_set_enum.insert(std::make_pair(
      std::string("TYPE5SET"), hmi_apis::Common_CharacterSet::TYPE5SET));
  character_set_enum.insert(std::make_pair(
      std::string("CID1SET"), hmi_apis::Common_CharacterSet::CID1SET));
  character_set_enum.insert(std::make_pair(
      std::string("CID2SET"), hmi_apis::Common_CharacterSet::CID2SET));

  video_streaming_protocol_enum.insert(std::make_pair(
      std::string("RAW"), hmi_apis::Common_VideoStreamingProtocol::RAW));
  video_streaming_protocol_enum.insert(std::make_pair(
      std::string("RTP"), hmi_apis::Common_VideoStreamingProtocol::RTP));
  video_streaming_protocol_enum.insert(std::make_pair(
      std::string("RTSP"), hmi_apis::Common_VideoStreamingProtocol::RTSP));
  video_streaming_protocol_enum.insert(std::make_pair(
      std::string("RTMP"), hmi_apis::Common_VideoStreamingProtocol::RTMP));
  video_streaming_protocol_enum.insert(std::make_pair(
      std::string("WEBM"), hmi_apis::Common_VideoStreamingProtocol::WEBM));

  video_streaming_codec_enum.insert(std::make_pair(
      std::string("H264"), hmi_apis::Common_VideoStreamingCodec::H264));
  video_streaming_codec_enum.insert(std::make_pair(
      std::string("H265"), hmi_apis::Common_VideoStreamingCodec::H265));
  video_streaming_codec_enum.insert(std::make_pair(
      std::string("Theora"), hmi_apis::Common_VideoStreamingCodec::Theora));
  video_streaming_codec_enum.insert(std::make_pair(
      std::string("VP8"), hmi_apis::Common_VideoStreamingCodec::VP8));
  video_streaming_codec_enum.insert(std::make_pair(
      std::string("VP9"), hmi_apis::Common_VideoStreamingCodec::VP9));
}

}  // namespace

HMICapabilitiesImpl::HMICapabilitiesImpl(ApplicationManager& app_mngr)
    : is_vr_cooperating_(false)
    , is_tts_cooperating_(false)
    , is_ui_cooperating_(false)
    , is_navi_cooperating_(false)
    , is_ivi_cooperating_(false)
    , is_rc_cooperating_(false)
    , attenuated_supported_(false)
    , ui_language_(hmi_apis::Common_Language::INVALID_ENUM)
    , vr_language_(hmi_apis::Common_Language::INVALID_ENUM)
    , tts_language_(hmi_apis::Common_Language::INVALID_ENUM)
    , vehicle_type_(NULL)
    , ui_supported_languages_(NULL)
    , tts_supported_languages_(NULL)
    , vr_supported_languages_(NULL)
    , display_capabilities_(NULL)
    , hmi_zone_capabilities_(NULL)
    , soft_buttons_capabilities_(NULL)
    , button_capabilities_(NULL)
    , preset_bank_capabilities_(NULL)
    , vr_capabilities_(NULL)
    , speech_capabilities_(NULL)
    , audio_pass_thru_capabilities_(NULL)
    , pcm_stream_capabilities_(NULL)
    , prerecorded_speech_(NULL)
    , is_navigation_supported_(false)
    , is_phone_call_supported_(false)
    , is_video_streaming_supported_(false)
    , navigation_capability_(NULL)
    , phone_capability_(NULL)
    , video_streaming_capability_(NULL)
    , rc_capability_(NULL)
    , app_mngr_(app_mngr)
    , hmi_language_handler_(app_mngr) {
  InitCapabilities();
  if (false == app_mngr_.get_settings().launch_hmi()) {
    is_vr_cooperating_ = true;
    is_tts_cooperating_ = true;
    is_ui_cooperating_ = true;
    is_navi_cooperating_ = true;
    is_ivi_cooperating_ = true;
    is_rc_cooperating_ = true;
  }
}

HMICapabilitiesImpl::~HMICapabilitiesImpl() {
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
  delete pcm_stream_capabilities_;
  delete prerecorded_speech_;
  delete navigation_capability_;
  delete phone_capability_;
  delete video_streaming_capability_;
  delete rc_capability_;
}

bool HMICapabilitiesImpl::VerifyImageType(const int32_t image_type) const {
  if (!display_capabilities_) {
    return false;
  }

  if (display_capabilities_->keyExists(hmi_response::image_capabilities)) {
    const smart_objects::SmartObject& image_caps =
        display_capabilities_->getElement(hmi_response::image_capabilities);
    for (uint32_t i = 0; i < image_caps.length(); ++i) {
      if (image_caps.getElement(i).asInt() == image_type) {
        return true;
      }
    }
  }

  return false;
}

void HMICapabilitiesImpl::set_is_vr_cooperating(const bool value) {
  is_vr_cooperating_ = value;
}

void HMICapabilitiesImpl::set_is_tts_cooperating(const bool value) {
  is_tts_cooperating_ = value;
}

void HMICapabilitiesImpl::set_is_ui_cooperating(const bool value) {
  is_ui_cooperating_ = value;
}

void HMICapabilitiesImpl::set_is_navi_cooperating(const bool value) {
  is_navi_cooperating_ = value;
}

void HMICapabilitiesImpl::set_is_ivi_cooperating(const bool value) {
  is_ivi_cooperating_ = value;
}

void HMICapabilitiesImpl::set_is_rc_cooperating(const bool value) {
  is_rc_cooperating_ = value;
}

void HMICapabilitiesImpl::set_attenuated_supported(const bool state) {
  attenuated_supported_ = state;
}

void HMICapabilitiesImpl::set_active_ui_language(
    const hmi_apis::Common_Language::eType language) {
  ui_language_ = language;
  hmi_language_handler_.set_language_for(HMILanguageHandler::INTERFACE_UI,
                                         language);
}

void HMICapabilitiesImpl::set_active_vr_language(
    const hmi_apis::Common_Language::eType language) {
  vr_language_ = language;
  hmi_language_handler_.set_language_for(HMILanguageHandler::INTERFACE_VR,
                                         language);
}

void HMICapabilitiesImpl::set_active_tts_language(
    const hmi_apis::Common_Language::eType language) {
  tts_language_ = language;
  hmi_language_handler_.set_language_for(HMILanguageHandler::INTERFACE_TTS,
                                         language);
}

const hmi_apis::Common_Language::eType HMICapabilitiesImpl::active_ui_language()
    const {
  using namespace hmi_apis;
  const Common_Language::eType language =
      hmi_language_handler_.get_language_for(HMILanguageHandler::INTERFACE_UI);
  return Common_Language::INVALID_ENUM != language ? language : ui_language_;
}

const hmi_apis::Common_Language::eType HMICapabilitiesImpl::active_vr_language()
    const {
  using namespace hmi_apis;
  const Common_Language::eType language =
      hmi_language_handler_.get_language_for(HMILanguageHandler::INTERFACE_VR);
  return Common_Language::INVALID_ENUM != language ? language : vr_language_;
}

const hmi_apis::Common_Language::eType
HMICapabilitiesImpl::active_tts_language() const {
  using namespace hmi_apis;
  const Common_Language::eType language =
      hmi_language_handler_.get_language_for(HMILanguageHandler::INTERFACE_TTS);
  return Common_Language::INVALID_ENUM != language ? language : tts_language_;
}

void HMICapabilitiesImpl::set_ui_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (ui_supported_languages_) {
    delete ui_supported_languages_;
  }
  ui_supported_languages_ = new smart_objects::SmartObject(supported_languages);
}

void HMICapabilitiesImpl::set_tts_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (tts_supported_languages_) {
    delete tts_supported_languages_;
  }
  tts_supported_languages_ =
      new smart_objects::SmartObject(supported_languages);
}

void HMICapabilitiesImpl::set_vr_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  if (vr_supported_languages_) {
    delete vr_supported_languages_;
  }
  vr_supported_languages_ = new smart_objects::SmartObject(supported_languages);
}

void HMICapabilitiesImpl::set_display_capabilities(
    const smart_objects::SmartObject& display_capabilities) {
  if (display_capabilities_) {
    delete display_capabilities_;
  }
  display_capabilities_ = new smart_objects::SmartObject(display_capabilities);
}

void HMICapabilitiesImpl::set_hmi_zone_capabilities(
    const smart_objects::SmartObject& hmi_zone_capabilities) {
  if (hmi_zone_capabilities_) {
    delete hmi_zone_capabilities_;
  }
  hmi_zone_capabilities_ =
      new smart_objects::SmartObject(hmi_zone_capabilities);
}

void HMICapabilitiesImpl::set_soft_button_capabilities(
    const smart_objects::SmartObject& soft_button_capabilities) {
  if (soft_buttons_capabilities_) {
    delete soft_buttons_capabilities_;
  }
  soft_buttons_capabilities_ =
      new smart_objects::SmartObject(soft_button_capabilities);
}

void HMICapabilitiesImpl::set_button_capabilities(
    const smart_objects::SmartObject& button_capabilities) {
  if (button_capabilities_) {
    delete button_capabilities_;
  }
  button_capabilities_ = new smart_objects::SmartObject(button_capabilities);
}

void HMICapabilitiesImpl::set_vr_capabilities(
    const smart_objects::SmartObject& vr_capabilities) {
  if (vr_capabilities_) {
    delete vr_capabilities_;
  }
  vr_capabilities_ = new smart_objects::SmartObject(vr_capabilities);
}

void HMICapabilitiesImpl::set_speech_capabilities(
    const smart_objects::SmartObject& speech_capabilities) {
  if (speech_capabilities_) {
    delete speech_capabilities_;
  }
  speech_capabilities_ = new smart_objects::SmartObject(speech_capabilities);
}

void HMICapabilitiesImpl::set_audio_pass_thru_capabilities(
    const smart_objects::SmartObject& audio_pass_thru_capabilities) {
  if (audio_pass_thru_capabilities_) {
    delete audio_pass_thru_capabilities_;
  }
  audio_pass_thru_capabilities_ =
      new smart_objects::SmartObject(audio_pass_thru_capabilities);
}

void HMICapabilitiesImpl::set_pcm_stream_capabilities(
    const smart_objects::SmartObject& pcm_stream_capabilities) {
  if (pcm_stream_capabilities_) {
    delete pcm_stream_capabilities_;
  }
  pcm_stream_capabilities_ =
      new smart_objects::SmartObject(pcm_stream_capabilities);
}

void HMICapabilitiesImpl::set_preset_bank_capabilities(
    const smart_objects::SmartObject& preset_bank_capabilities) {
  if (preset_bank_capabilities_) {
    delete preset_bank_capabilities_;
  }
  preset_bank_capabilities_ =
      new smart_objects::SmartObject(preset_bank_capabilities);
}

void HMICapabilitiesImpl::set_vehicle_type(
    const smart_objects::SmartObject& vehicle_type) {
  if (vehicle_type_) {
    delete vehicle_type_;
  }
  vehicle_type_ = new smart_objects::SmartObject(vehicle_type);
}

void HMICapabilitiesImpl::set_prerecorded_speech(
    const smart_objects::SmartObject& prerecorded_speech) {
  if (prerecorded_speech_) {
    delete prerecorded_speech_;
    prerecorded_speech_ = NULL;
  }
  prerecorded_speech_ = new smart_objects::SmartObject(prerecorded_speech);
}

void HMICapabilitiesImpl::set_navigation_supported(const bool supported) {
  is_navigation_supported_ = supported;
}
void HMICapabilitiesImpl::set_phone_call_supported(const bool supported) {
  is_phone_call_supported_ = supported;
}

void HMICapabilitiesImpl::set_video_streaming_supported(const bool supported) {
  is_video_streaming_supported_ = supported;
}

void HMICapabilitiesImpl::set_navigation_capability(
    const smart_objects::SmartObject& navigation_capability) {
  if (navigation_capability_) {
    delete navigation_capability_;
  }
  navigation_capability_ =
      new smart_objects::SmartObject(navigation_capability);
}

void HMICapabilitiesImpl::set_phone_capability(
    const smart_objects::SmartObject& phone_capability) {
  if (phone_capability_) {
    delete phone_capability_;
  }
  phone_capability_ = new smart_objects::SmartObject(phone_capability);
}

void HMICapabilitiesImpl::set_video_streaming_capability(
    const smart_objects::SmartObject& video_streaming_capability) {
  if (video_streaming_capability_) {
    delete video_streaming_capability_;
  }
  video_streaming_capability_ =
      new smart_objects::SmartObject(video_streaming_capability);
}

void HMICapabilitiesImpl::set_rc_capability(
    const smart_objects::SmartObject& rc_capability) {
  if (rc_capability_) {
    delete rc_capability_;
  }
  rc_capability_ = new smart_objects::SmartObject(rc_capability);
}

void HMICapabilitiesImpl::Init(resumption::LastState* last_state) {
  hmi_language_handler_.Init(last_state);
  if (false == load_capabilities_from_file()) {
    LOG4CXX_ERROR(logger_, "file hmi_capabilities.json was not loaded");
  } else {
    LOG4CXX_INFO(logger_, "file hmi_capabilities.json was loaded");
  }
  hmi_language_handler_.set_default_capabilities_languages(
      ui_language_, vr_language_, tts_language_);
}

bool HMICapabilitiesImpl::is_ui_cooperating() const {
  return is_ui_cooperating_;
}

bool HMICapabilitiesImpl::is_vr_cooperating() const {
  return is_vr_cooperating_;
}

bool HMICapabilitiesImpl::is_tts_cooperating() const {
  return is_tts_cooperating_;
}

bool HMICapabilitiesImpl::is_navi_cooperating() const {
  return is_navi_cooperating_;
}

bool HMICapabilitiesImpl::is_ivi_cooperating() const {
  return is_ivi_cooperating_;
}

bool HMICapabilitiesImpl::is_rc_cooperating() const {
  return is_rc_cooperating_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::ui_supported_languages()
    const {
  return ui_supported_languages_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::vr_supported_languages()
    const {
  return vr_supported_languages_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::tts_supported_languages()
    const {
  return tts_supported_languages_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::display_capabilities()
    const {
  return display_capabilities_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::hmi_zone_capabilities()
    const {
  return hmi_zone_capabilities_;
}

const smart_objects::SmartObject*
HMICapabilitiesImpl::soft_button_capabilities() const {
  return soft_buttons_capabilities_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::button_capabilities()
    const {
  return button_capabilities_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::speech_capabilities()
    const {
  return speech_capabilities_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::vr_capabilities() const {
  return vr_capabilities_;
}

const smart_objects::SmartObject*
HMICapabilitiesImpl::audio_pass_thru_capabilities() const {
  return audio_pass_thru_capabilities_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::pcm_stream_capabilities()
    const {
  return pcm_stream_capabilities_;
}

const smart_objects::SmartObject*
HMICapabilitiesImpl::preset_bank_capabilities() const {
  return preset_bank_capabilities_;
}

bool HMICapabilitiesImpl::attenuated_supported() const {
  return attenuated_supported_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::vehicle_type() const {
  return vehicle_type_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::prerecorded_speech()
    const {
  return prerecorded_speech_;
}

bool HMICapabilitiesImpl::navigation_supported() const {
  return is_navigation_supported_;
}

bool HMICapabilitiesImpl::phone_call_supported() const {
  return is_phone_call_supported_;
}

bool HMICapabilitiesImpl::video_streaming_supported() const {
  return is_video_streaming_supported_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::navigation_capability()
    const {
  return navigation_capability_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::phone_capability()
    const {
  return phone_capability_;
}

const smart_objects::SmartObject*
HMICapabilitiesImpl::video_streaming_capability() const {
  return video_streaming_capability_;
}

const smart_objects::SmartObject* HMICapabilitiesImpl::rc_capability() const {
  return rc_capability_;
}

bool HMICapabilitiesImpl::load_capabilities_from_file() {
  std::string json_string;
  std::string file_name = app_mngr_.get_settings().hmi_capabilities_file_name();

  if (!file_system::FileExists(file_name)) {
    return false;
  }

  if (!file_system::ReadFile(file_name, json_string)) {
    return false;
  }

  try {
    Json::Reader reader_;
    Json::Value root_json;

    bool result = reader_.parse(json_string, root_json, false);
    if (!result) {
      return false;
    }
    // UI
    if (check_existing_json_member(root_json, "UI")) {
      Json::Value ui = root_json.get("UI", Json::Value::null);

      if (check_existing_json_member(ui, "language")) {
        const std::string lang = ui.get("language", "EN-US").asString();
        set_active_ui_language(MessageHelper::CommonLanguageFromString(lang));
      } else {
        set_active_ui_language(
            MessageHelper::CommonLanguageFromString("EN-US"));
      }

      if (check_existing_json_member(ui, "languages")) {
        smart_objects::SmartObject ui_languages_so(
            smart_objects::SmartType_Array);
        Json::Value languages_ui = ui.get("languages", "");
        convert_json_languages_to_obj(languages_ui, ui_languages_so);
        set_ui_supported_languages(ui_languages_so);
      }

      if (check_existing_json_member(ui, "displayCapabilities")) {
        smart_objects::SmartObject display_capabilities_so;
        Json::Value display_capabilities = ui.get("displayCapabilities", "");
        Formatters::CFormatterJsonBase::jsonValueToObj(display_capabilities,
                                                       display_capabilities_so);

        if (display_capabilities_so.keyExists(hmi_response::display_type)) {
          std::map<std::string,
                   hmi_apis::Common_DisplayType::eType>::const_iterator it =
              display_type_enum.find(
                  (display_capabilities_so[hmi_response::display_type])
                      .asString());
          display_capabilities_so.erase(hmi_response::display_type);
          if (display_type_enum.end() != it) {
            display_capabilities_so[hmi_response::display_type] = it->second;
          }
        }

        if (display_capabilities_so.keyExists(hmi_response::text_fields)) {
          const uint32_t kLen =
              display_capabilities_so[hmi_response::text_fields].length();

          for (uint32_t i = 0; i < kLen; ++i) {
            if ((display_capabilities_so[hmi_response::text_fields][i])
                    .keyExists(strings::name)) {
              std::map<std::string,
                       hmi_apis::Common_TextFieldName::eType>::const_iterator
                  it_text_field_name = text_fields_enum_name.find(
                      display_capabilities_so[hmi_response::text_fields][i]
                                             [strings::name].asString());
              display_capabilities_so[hmi_response::text_fields][i].erase(
                  strings::name);
              if (text_fields_enum_name.end() != it_text_field_name) {
                display_capabilities_so[hmi_response::text_fields][i]
                                       [strings::name] =
                                           it_text_field_name->second;
              }
            }
            if ((display_capabilities_so[hmi_response::text_fields][i])
                    .keyExists(strings::character_set)) {
              std::map<std::string,
                       hmi_apis::Common_CharacterSet::eType>::const_iterator
                  it_characte_set = character_set_enum.find(
                      display_capabilities_so[hmi_response::text_fields][i]
                                             [strings::character_set]
                                                 .asString());
              display_capabilities_so[hmi_response::text_fields][i].erase(
                  strings::character_set);
              if (character_set_enum.end() != it_characte_set) {
                display_capabilities_so[hmi_response::text_fields][i]
                                       [strings::character_set] =
                                           it_characte_set->second;
              }
            }
          }
        }

        if (display_capabilities_so.keyExists(hmi_response::image_fields)) {
          smart_objects::SmartObject& array_image_fields =
              display_capabilities_so[hmi_response::image_fields];
          for (uint32_t i = 0; i < array_image_fields.length(); ++i) {
            if (array_image_fields[i].keyExists(strings::name)) {
              std::map<std::string,
                       hmi_apis::Common_ImageFieldName::eType>::const_iterator
                  it = image_field_name_enum.find(
                      (array_image_fields[i][strings::name]).asString());
              array_image_fields[i].erase(strings::name);
              if (image_field_name_enum.end() != it) {
                array_image_fields[i][strings::name] = it->second;
              }
            }
            if (array_image_fields[i].keyExists(
                    strings::image_type_supported)) {
              smart_objects::SmartObject& image_type_supported_array =
                  array_image_fields[i][strings::image_type_supported];
              smart_objects::SmartObject image_type_supported_enum(
                  smart_objects::SmartType_Array);
              for (uint32_t k = 0, j = 0;
                   k < image_type_supported_array.length();
                   ++k) {
                std::map<std::string,
                         hmi_apis::Common_FileType::eType>::const_iterator it =
                    file_type_enum.find(
                        (image_type_supported_array[k]).asString());
                if (file_type_enum.end() != it) {
                  image_type_supported_enum[j++] = it->second;
                }
              }
              array_image_fields[i].erase(strings::image_type_supported);
              array_image_fields[i][strings::image_type_supported] =
                  image_type_supported_enum;
            }
          }
        }
        if (display_capabilities_so.keyExists(
                hmi_response::media_clock_formats)) {
          smart_objects::SmartObject& media_clock_formats_array =
              display_capabilities_so[hmi_response::media_clock_formats];
          smart_objects::SmartObject media_clock_formats_enum(
              smart_objects::SmartType_Array);
          for (uint32_t i = 0, j = 0; i < media_clock_formats_array.length();
               ++i) {
            std::map<std::string,
                     hmi_apis::Common_MediaClockFormat::eType>::const_iterator
                it = media_clock_enum_name.find(
                    (media_clock_formats_array[i]).asString());
            if (media_clock_enum_name.end() != it) {
              media_clock_formats_enum[j++] = it->second;
            }
          }
          display_capabilities_so.erase(hmi_response::media_clock_formats);
          display_capabilities_so[hmi_response::media_clock_formats] =
              media_clock_formats_enum;
        }

        if (display_capabilities_so.keyExists(
                hmi_response::image_capabilities)) {
          smart_objects::SmartObject& image_capabilities_array =
              display_capabilities_so[hmi_response::image_capabilities];
          smart_objects::SmartObject image_capabilities_enum(
              smart_objects::SmartType_Array);
          for (uint32_t i = 0, j = 0; i < image_capabilities_array.length();
               ++i) {
            std::map<std::string,
                     hmi_apis::Common_ImageType::eType>::const_iterator it =
                image_type_enum.find((image_capabilities_array[i]).asString());
            if (image_type_enum.end() != it) {
              image_capabilities_enum[j++] = it->second;
            }
          }
          display_capabilities_so.erase(hmi_response::image_capabilities);
          display_capabilities_so[hmi_response::image_capabilities] =
              image_capabilities_enum;
        }
        set_display_capabilities(display_capabilities_so);
      }

      if (check_existing_json_member(ui, "audioPassThruCapabilities")) {
        Json::Value audio_capabilities =
            ui.get("audioPassThruCapabilities", "");
        smart_objects::SmartObject audio_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        audio_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Map);
        if (check_existing_json_member(audio_capabilities, "samplingRate")) {
          audio_capabilities_so["samplingRate"] =
              sampling_rate_enum.find(audio_capabilities.get("samplingRate", "")
                                          .asString())->second;
        }
        if (check_existing_json_member(audio_capabilities, "bitsPerSample")) {
          audio_capabilities_so["bitsPerSample"] =
              bit_per_sample_enum.find(audio_capabilities.get("bitsPerSample",
                                                              "").asString())
                  ->second;
        }
        if (check_existing_json_member(audio_capabilities, "audioType")) {
          audio_capabilities_so["audioType"] =
              audio_type_enum.find(audio_capabilities.get("audioType", "")
                                       .asString())->second;
        }
        set_audio_pass_thru_capabilities(audio_capabilities_so);
      }

      if (check_existing_json_member(ui, "pcmStreamCapabilities")) {
        Json::Value pcm_capabilities = ui.get("pcmStreamCapabilities", "");
        smart_objects::SmartObject pcm_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Map);

        if (check_existing_json_member(pcm_capabilities, "samplingRate")) {
          pcm_capabilities_so["samplingRate"] =
              sampling_rate_enum.find(pcm_capabilities.get("samplingRate", "")
                                          .asString())->second;
        }
        if (check_existing_json_member(pcm_capabilities, "bitsPerSample")) {
          pcm_capabilities_so["bitsPerSample"] =
              bit_per_sample_enum.find(pcm_capabilities.get("bitsPerSample", "")
                                           .asString())->second;
        }
        if (check_existing_json_member(pcm_capabilities, "audioType")) {
          pcm_capabilities_so["audioType"] =
              audio_type_enum.find(pcm_capabilities.get("audioType", "")
                                       .asString())->second;
        }

        set_pcm_stream_capabilities(pcm_capabilities_so);
      }

      if (check_existing_json_member(ui, "hmiZoneCapabilities")) {
        smart_objects::SmartObject hmi_zone_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        hmi_zone_capabilities_so =
            hmi_zone_enum.find(ui.get("hmiZoneCapabilities", "").asString())
                ->second;
        set_hmi_zone_capabilities(hmi_zone_capabilities_so);
      }

      if (check_existing_json_member(ui, "softButtonCapabilities")) {
        Json::Value soft_button_capabilities =
            ui.get("softButtonCapabilities", "");
        smart_objects::SmartObject soft_button_capabilities_so;
        Formatters::CFormatterJsonBase::jsonValueToObj(
            soft_button_capabilities, soft_button_capabilities_so);
        set_soft_button_capabilities(soft_button_capabilities_so);
      }
      if (check_existing_json_member(ui, "systemCapabilities")) {
        Json::Value system_capabilities = ui.get("systemCapabilities", "");
        set_navigation_supported(false);
        if (check_existing_json_member(system_capabilities,
                                       "navigationCapability")) {
          Json::Value navigation_capability =
              system_capabilities.get("navigationCapability", "");
          smart_objects::SmartObject navigation_capability_so;
          Formatters::CFormatterJsonBase::jsonValueToObj(
              navigation_capability, navigation_capability_so);
          set_navigation_capability(navigation_capability_so);
          if (!navigation_capability_so.empty()) {
            set_navigation_supported(true);
          }
        }
        set_phone_call_supported(false);
        if (check_existing_json_member(system_capabilities,
                                       "phoneCapability")) {
          Json::Value phone_capability =
              system_capabilities.get("phoneCapability", "");
          smart_objects::SmartObject phone_capability_so;
          Formatters::CFormatterJsonBase::jsonValueToObj(phone_capability,
                                                         phone_capability_so);
          set_phone_capability(phone_capability_so);
          if (!phone_capability_so.empty()) {
            set_phone_call_supported(true);
          }
        }
        set_video_streaming_supported(false);
        if (check_existing_json_member(system_capabilities,
                                       "videoStreamingCapability")) {
          Json::Value vs_capability =
              system_capabilities.get("videoStreamingCapability", "");
          smart_objects::SmartObject vs_capability_so;
          Formatters::CFormatterJsonBase::jsonValueToObj(vs_capability,
                                                         vs_capability_so);

          if (vs_capability_so.keyExists("supportedFormats")) {
            smart_objects::SmartObject& supported_format_array =
                vs_capability_so["supportedFormats"];
            smart_objects::SmartObject converted_array(
                smart_objects::SmartType_Array);
            for (uint32_t i = 0, j = 0; i < supported_format_array.length();
                 i++) {
              if (!supported_format_array[i].keyExists("protocol") ||
                  !supported_format_array[i].keyExists("codec")) {
                continue;
              }

              std::map<std::string,
                       hmi_apis::Common_VideoStreamingProtocol::eType>::
                  const_iterator it_protocol =
                      video_streaming_protocol_enum.find(
                          supported_format_array[i]["protocol"].asString());

              std::map<std::string,
                       hmi_apis::Common_VideoStreamingCodec::eType>::
                  const_iterator it_codec = video_streaming_codec_enum.find(
                      supported_format_array[i]["codec"].asString());

              // format is valid only if both protocol and codec are converted
              // to enum values successfully
              if (it_protocol != video_streaming_protocol_enum.end() &&
                  it_codec != video_streaming_codec_enum.end()) {
                smart_objects::SmartObject format_so =
                    smart_objects::SmartObject(smart_objects::SmartType_Map);
                format_so["protocol"] = it_protocol->second;
                format_so["codec"] = it_codec->second;
                converted_array[j++] = format_so;
              }
            }
            vs_capability_so.erase("supportedFormats");
            vs_capability_so["supportedFormats"] = converted_array;
          }
          set_video_streaming_capability(vs_capability_so);
          if (!vs_capability_so.empty()) {
            set_video_streaming_supported(true);
          }
        }
        if (check_existing_json_member(system_capabilities,
                                       "remoteControlCapability")) {
          Json::Value rc_capability =
              system_capabilities.get("remoteControlCapability", "");
          smart_objects::SmartObject rc_capability_so;
          Formatters::CFormatterJsonBase::jsonValueToObj(rc_capability,
                                                         rc_capability_so);
          set_rc_capability(rc_capability_so);
        }
      }
    }  // UI end

    // VR
    if (check_existing_json_member(root_json, "VR")) {
      Json::Value vr = root_json.get("VR", "");
      if (check_existing_json_member(vr, "language")) {
        const std::string lang = vr.get("language", "EN-US").asString();
        set_active_vr_language(MessageHelper::CommonLanguageFromString(lang));
      } else {
        set_active_vr_language(
            MessageHelper::CommonLanguageFromString("EN-US"));
      }

      if (check_existing_json_member(vr, "languages")) {
        Json::Value languages_vr = vr.get("languages", "");
        smart_objects::SmartObject vr_languages_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        convert_json_languages_to_obj(languages_vr, vr_languages_so);
        set_vr_supported_languages(vr_languages_so);
      }

      if (check_existing_json_member(vr, "capabilities")) {
        Json::Value capabilities = vr.get("capabilities", "");
        smart_objects::SmartObject vr_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        for (uint32_t i = 0; i < capabilities.size(); ++i) {
          vr_capabilities_so[i] =
              vr_enum_capabilities.find(capabilities[i].asString())->second;
        }
        set_vr_capabilities(vr_capabilities_so);
      }
    }  // VR end

    // TTS
    if (check_existing_json_member(root_json, "TTS")) {
      Json::Value tts = root_json.get("TTS", "");

      if (check_existing_json_member(tts, "language")) {
        const std::string lang = tts.get("language", "EN-US").asString();
        set_active_tts_language(MessageHelper::CommonLanguageFromString(lang));
      } else {
        set_active_tts_language(
            MessageHelper::CommonLanguageFromString("EN-US"));
      }

      if (check_existing_json_member(tts, "languages")) {
        Json::Value languages_tts = tts.get("languages", "");
        smart_objects::SmartObject tts_languages_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        convert_json_languages_to_obj(languages_tts, tts_languages_so);
        set_tts_supported_languages(tts_languages_so);
      }

      if (check_existing_json_member(tts, "capabilities")) {
        Json::Value capabilities = tts.get("capabilities", "");
        smart_objects::SmartObject tts_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        for (uint32_t i = 0; i < capabilities.size(); ++i) {
          tts_capabilities_so[i] =
              tts_enum_capabilities.find(capabilities[i].asString())->second;
        }
        set_speech_capabilities(tts_capabilities_so);
      }
    }  // TTS end

    // Buttons
    if (check_existing_json_member(root_json, "Buttons")) {
      Json::Value buttons = root_json.get("Buttons", "");
      if (check_existing_json_member(buttons, "capabilities")) {
        Json::Value bt_capabilities = buttons.get("capabilities", "");
        smart_objects::SmartObject buttons_capabilities_so;
        Formatters::CFormatterJsonBase::jsonValueToObj(bt_capabilities,
                                                       buttons_capabilities_so);

        for (uint32_t i = 0; i < buttons_capabilities_so.length(); ++i) {
          if ((buttons_capabilities_so[i]).keyExists(strings::name)) {
            std::map<std::string,
                     hmi_apis::Common_ButtonName::eType>::const_iterator it =
                button_enum_name.find(
                    (buttons_capabilities_so[i][strings::name]).asString());
            buttons_capabilities_so[i].erase(strings::name);
            if (button_enum_name.end() != it) {
              buttons_capabilities_so[i][strings::name] = it->second;
            }
          }
        }
        set_button_capabilities(buttons_capabilities_so);
      }
      if (check_existing_json_member(buttons, "presetBankCapabilities")) {
        Json::Value presetBank = buttons.get("presetBankCapabilities", "");
        smart_objects::SmartObject preset_bank_so;
        Formatters::CFormatterJsonBase::jsonValueToObj(presetBank,
                                                       preset_bank_so);
        set_preset_bank_capabilities(preset_bank_so);
      }
    }  // Buttons end

    // VehicleType
    if (check_existing_json_member(root_json, "VehicleInfo")) {
      Json::Value vehicle_info = root_json.get("VehicleInfo", "");
      smart_objects::SmartObject vehicle_type_so;
      Formatters::CFormatterJsonBase::jsonValueToObj(vehicle_info,
                                                     vehicle_type_so);
      set_vehicle_type(vehicle_type_so);
    }  // VehicleType end
  } catch (...) {
    return false;
  }
  return true;
}

void HMICapabilitiesImpl::set_ccpu_version(const std::string& ccpu_version) {
  ccpu_version_ = ccpu_version;
}

const std::string& HMICapabilitiesImpl::ccpu_version() const {
  return ccpu_version_;
}

bool HMICapabilitiesImpl::check_existing_json_member(
    const Json::Value& json_member, const char* name_of_member) const {
  return json_member.isMember(name_of_member);
}

void HMICapabilitiesImpl::convert_json_languages_to_obj(
    const Json::Value& json_languages,
    smart_objects::SmartObject& languages) const {
  for (uint32_t i = 0, j = 0; i < json_languages.size(); ++i) {
    languages[j++] =
        MessageHelper::CommonLanguageFromString(json_languages[i].asString());
  }
}

HMILanguageHandler& HMICapabilitiesImpl::get_hmi_language_handler() {
  return hmi_language_handler_;
}

void HMICapabilitiesImpl::set_handle_response_for(
    const smart_objects::SmartObject& request) {
  hmi_language_handler_.set_handle_response_for(request);
}

}  //  namespace application_manager

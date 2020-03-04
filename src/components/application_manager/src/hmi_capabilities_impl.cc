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

#include <iostream>
#include "application_manager/application_manager.h"
#include "application_manager/hmi_capabilities_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_plugins/rc_rpc_plugin/include/rc_rpc_plugin/rc_module_constants.h"
#include "application_manager/smart_object_keys.h"
#include "config_profile/profile.h"
#include "formatters/CFormatterJsonBase.h"
#include "interfaces/HMI_API.h"
#include "smart_objects/enum_schema_item.h"
#include "smart_objects/smart_object.h"
#include "utils/file_system.h"
#include "utils/helpers.h"
#include "utils/logger.h"

namespace application_manager {
namespace formatters = ns_smart_device_link::ns_json_handler::formatters;

CREATE_LOGGERPTR_GLOBAL(logger_, "HMICapabilities")

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
  tts_enum_capabilities.insert(std::make_pair(
      std::string("FILE"), hmi_apis::Common_SpeechCapabilities::FILE));

  button_enum_name.insert(
      std::make_pair(std::string("OK"), hmi_apis::Common_ButtonName::OK));
  button_enum_name.insert(std::make_pair(
      std::string("PLAY_PAUSE"), hmi_apis::Common_ButtonName::PLAY_PAUSE));
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
  button_enum_name.insert(
      std::make_pair(std::string("NAV_CENTER_LOCATION"),
                     hmi_apis::Common_ButtonName::NAV_CENTER_LOCATION));
  button_enum_name.insert(std::make_pair(
      std::string("NAV_ZOOM_IN"), hmi_apis::Common_ButtonName::NAV_ZOOM_IN));
  button_enum_name.insert(std::make_pair(
      std::string("NAV_ZOOM_OUT"), hmi_apis::Common_ButtonName::NAV_ZOOM_OUT));
  button_enum_name.insert(std::make_pair(
      std::string("NAV_PAN_UP"), hmi_apis::Common_ButtonName::NAV_PAN_UP));
  button_enum_name.insert(
      std::make_pair(std::string("NAV_PAN_UP_RIGHT"),
                     hmi_apis::Common_ButtonName::NAV_PAN_UP_RIGHT));
  button_enum_name.insert(
      std::make_pair(std::string("NAV_PAN_RIGHT"),
                     hmi_apis::Common_ButtonName::NAV_PAN_RIGHT));
  button_enum_name.insert(
      std::make_pair(std::string("NAV_PAN_DOWN_RIGHT"),
                     hmi_apis::Common_ButtonName::NAV_PAN_DOWN_RIGHT));
  button_enum_name.insert(std::make_pair(
      std::string("NAV_PAN_DOWN"), hmi_apis::Common_ButtonName::NAV_PAN_DOWN));
  button_enum_name.insert(
      std::make_pair(std::string("NAV_PAN_DOWN_LEFT"),
                     hmi_apis::Common_ButtonName::NAV_PAN_DOWN_LEFT));
  button_enum_name.insert(std::make_pair(
      std::string("NAV_PAN_LEFT"), hmi_apis::Common_ButtonName::NAV_PAN_LEFT));
  button_enum_name.insert(
      std::make_pair(std::string("NAV_PAN_UP_LEFT"),
                     hmi_apis::Common_ButtonName::NAV_PAN_UP_LEFT));
  button_enum_name.insert(
      std::make_pair(std::string("NAV_TILT_TOGGLE"),
                     hmi_apis::Common_ButtonName::NAV_TILT_TOGGLE));
  button_enum_name.insert(
      std::make_pair(std::string("NAV_ROTATE_CLOCKWISE"),
                     hmi_apis::Common_ButtonName::NAV_ROTATE_CLOCKWISE));
  button_enum_name.insert(
      std::make_pair(std::string("NAV_ROTATE_COUNTERCLOCKWISE"),
                     hmi_apis::Common_ButtonName::NAV_ROTATE_COUNTERCLOCKWISE));
  button_enum_name.insert(
      std::make_pair(std::string("NAV_HEADING_TOGGLE"),
                     hmi_apis::Common_ButtonName::NAV_HEADING_TOGGLE));

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
      std::make_pair(std::string("secondaryGraphic"),
                     hmi_apis::Common_ImageFieldName::secondaryGraphic));
  image_field_name_enum.insert(
      std::make_pair(std::string("showConstantTBTIcon"),
                     hmi_apis::Common_ImageFieldName::showConstantTBTIcon));
  image_field_name_enum.insert(std::make_pair(
      std::string("showConstantTBTNextTurnIcon"),
      hmi_apis::Common_ImageFieldName::showConstantTBTNextTurnIcon));
  image_field_name_enum.insert(
      std::make_pair(std::string("locationImage"),
                     hmi_apis::Common_ImageFieldName::locationImage));
  image_field_name_enum.insert(std::make_pair(
      std::string("alertIcon"), hmi_apis::Common_ImageFieldName::alertIcon));

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
  display_type_enum.insert(std::make_pair(
      std::string("SDL_GENERIC"), hmi_apis::Common_DisplayType::SDL_GENERIC));

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

namespace {
/**
 * @brief Saves smart object content into the JSON node
 * @param field_name name of the field to save
 * @param schema reference to schema to unapply
 * @param object_to_save pointer to object to save
 * @param out_json_node JSON node for the output result
 */
void save_hmi_capability_field_to_json(
    const std::string& field_name,
    const smart_objects::CSmartSchema& schema,
    smart_objects::SmartObjectSPtr object_to_save,
    Json::Value& out_json_node) {
  if (!object_to_save) {
    return;
  }

  namespace Formatters = ns_smart_device_link::ns_json_handler::formatters;
  smart_objects::SmartObject formatted_object(smart_objects::SmartType_Map);
  formatted_object[strings::msg_params][field_name] = *object_to_save;
  formatted_object.setSchema(schema);
  formatted_object.getSchema().unapplySchema(
      formatted_object);  // converts enums back to strings

  Json::Value temp_value;
  Formatters::CFormatterJsonBase::objToJsonValue(formatted_object, temp_value);
  out_json_node[field_name] = temp_value[strings::msg_params][field_name];
}

}  //  namespace

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
    , is_rc_supported_(false)
    , navigation_capability_(NULL)
    , phone_capability_(NULL)
    , video_streaming_capability_(NULL)
    , rc_capability_(NULL)
    , seat_location_capability_(NULL)
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

HMICapabilitiesImpl::~HMICapabilitiesImpl() {}

bool HMICapabilitiesImpl::VerifyImageType(const int32_t image_type) const {
  auto capabilities = display_capabilities();
  if (!capabilities) {
    return false;
  }

  if (capabilities->keyExists(hmi_response::image_capabilities)) {
    const smart_objects::SmartObject& image_caps =
        capabilities->getElement(hmi_response::image_capabilities);
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
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(supported_languages);
  ui_supported_languages_.swap(new_value);
}

void HMICapabilitiesImpl::set_tts_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(supported_languages);
  tts_supported_languages_.swap(new_value);
}

void HMICapabilitiesImpl::set_vr_supported_languages(
    const smart_objects::SmartObject& supported_languages) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(supported_languages);
  vr_supported_languages_.swap(new_value);
}

void HMICapabilitiesImpl::set_display_capabilities(
    const smart_objects::SmartObject& display_capabilities) {
  if (app_mngr_.IsSOStructValid(
          hmi_apis::StructIdentifiers::Common_DisplayCapabilities,
          display_capabilities)) {
    auto new_value =
        std::make_shared<smart_objects::SmartObject>(display_capabilities);
    display_capabilities_.swap(new_value);
  }
}

void HMICapabilitiesImpl::set_system_display_capabilities(
    const smart_objects::SmartObject& display_capabilities) {
  system_display_capabilities_.reset(
      new smart_objects::SmartObject(display_capabilities));
}

void HMICapabilitiesImpl::set_hmi_zone_capabilities(
    const smart_objects::SmartObject& hmi_zone_capabilities) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(hmi_zone_capabilities);
  hmi_zone_capabilities_.swap(new_value);
}

void HMICapabilitiesImpl::set_soft_button_capabilities(
    const smart_objects::SmartObject& soft_button_capabilities) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(soft_button_capabilities);
  soft_buttons_capabilities_.swap(new_value);
}

void HMICapabilitiesImpl::set_button_capabilities(
    const smart_objects::SmartObject& button_capabilities) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(button_capabilities);
  button_capabilities_.swap(new_value);
}

void HMICapabilitiesImpl::set_vr_capabilities(
    const smart_objects::SmartObject& vr_capabilities) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(vr_capabilities);
  vr_capabilities_.swap(new_value);
}

void HMICapabilitiesImpl::set_speech_capabilities(
    const smart_objects::SmartObject& speech_capabilities) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(speech_capabilities);
  speech_capabilities_.swap(new_value);
}

void HMICapabilitiesImpl::set_audio_pass_thru_capabilities(
    const smart_objects::SmartObject& audio_pass_thru_capabilities) {
  auto new_value = std::make_shared<smart_objects::SmartObject>(
      audio_pass_thru_capabilities);
  audio_pass_thru_capabilities_.swap(new_value);
}

void HMICapabilitiesImpl::set_pcm_stream_capabilities(
    const smart_objects::SmartObject& pcm_stream_capabilities) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(pcm_stream_capabilities);
  pcm_stream_capabilities_.swap(new_value);
}

void HMICapabilitiesImpl::set_preset_bank_capabilities(
    const smart_objects::SmartObject& preset_bank_capabilities) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(preset_bank_capabilities);
  preset_bank_capabilities_.swap(new_value);
}

void HMICapabilitiesImpl::set_vehicle_type(
    const smart_objects::SmartObject& vehicle_type) {
  auto new_value = std::make_shared<smart_objects::SmartObject>(vehicle_type);
  vehicle_type_.swap(new_value);
}

void HMICapabilitiesImpl::set_prerecorded_speech(
    const smart_objects::SmartObject& prerecorded_speech) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(prerecorded_speech);
  prerecorded_speech_.swap(new_value);
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

void HMICapabilitiesImpl::set_rc_supported(const bool supported) {
  is_rc_supported_ = supported;
}

void HMICapabilitiesImpl::set_navigation_capability(
    const smart_objects::SmartObject& navigation_capability) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(navigation_capability);
  navigation_capability_.swap(new_value);
}

void HMICapabilitiesImpl::set_phone_capability(
    const smart_objects::SmartObject& phone_capability) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(phone_capability);
  phone_capability_.swap(new_value);
}

void HMICapabilitiesImpl::set_video_streaming_capability(
    const smart_objects::SmartObject& video_streaming_capability) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(video_streaming_capability);
  video_streaming_capability_.swap(new_value);
}

void HMICapabilitiesImpl::set_rc_capability(
    const smart_objects::SmartObject& rc_capability) {
  auto new_value = std::make_shared<smart_objects::SmartObject>(rc_capability);
  rc_capability_.swap(new_value);
}

void HMICapabilitiesImpl::set_seat_location_capability(
    const smart_objects::SmartObject& seat_location_capability) {
  auto new_value =
      std::make_shared<smart_objects::SmartObject>(seat_location_capability);
  seat_location_capability_.swap(new_value);
}

void HMICapabilitiesImpl::Init(
    resumption::LastStateWrapperPtr last_state_wrapper) {
  hmi_language_handler_.Init(last_state_wrapper);
  if (false == LoadCapabilitiesFromFile()) {
    LOG4CXX_ERROR(logger_, "file hmi_capabilities.json was not loaded");
  } else {
    LOG4CXX_INFO(logger_, "file hmi_capabilities.json was loaded");
  }
  hmi_language_handler_.set_default_capabilities_languages(
      ui_language_, vr_language_, tts_language_);
}

void HMICapabilitiesImpl::Init(resumption::LastState*) {}

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

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::ui_supported_languages() const {
  return ui_supported_languages_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::vr_supported_languages() const {
  return vr_supported_languages_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::tts_supported_languages() const {
  return tts_supported_languages_;
}

const smart_objects::SmartObjectSPtr HMICapabilitiesImpl::display_capabilities()
    const {
  return display_capabilities_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::system_display_capabilities() const {
  return system_display_capabilities_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::hmi_zone_capabilities() const {
  return hmi_zone_capabilities_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::soft_button_capabilities() const {
  return soft_buttons_capabilities_;
}

const smart_objects::SmartObjectSPtr HMICapabilitiesImpl::button_capabilities()
    const {
  return button_capabilities_;
}

const smart_objects::SmartObjectSPtr HMICapabilitiesImpl::speech_capabilities()
    const {
  return speech_capabilities_;
}

const smart_objects::SmartObjectSPtr HMICapabilitiesImpl::vr_capabilities()
    const {
  return vr_capabilities_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::audio_pass_thru_capabilities() const {
  return audio_pass_thru_capabilities_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::pcm_stream_capabilities() const {
  return pcm_stream_capabilities_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::preset_bank_capabilities() const {
  return preset_bank_capabilities_;
}

bool HMICapabilitiesImpl::attenuated_supported() const {
  return attenuated_supported_;
}

const smart_objects::SmartObjectSPtr HMICapabilitiesImpl::vehicle_type() const {
  return vehicle_type_;
}

const smart_objects::SmartObjectSPtr HMICapabilitiesImpl::prerecorded_speech()
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

bool HMICapabilitiesImpl::rc_supported() const {
  return is_rc_supported_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::navigation_capability() const {
  return navigation_capability_;
}

const smart_objects::SmartObjectSPtr HMICapabilitiesImpl::phone_capability()
    const {
  return phone_capability_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::video_streaming_capability() const {
  return video_streaming_capability_;
}

const smart_objects::SmartObjectSPtr HMICapabilitiesImpl::rc_capability()
    const {
  return rc_capability_;
}

const smart_objects::SmartObjectSPtr
HMICapabilitiesImpl::seat_location_capability() const {
  return seat_location_capability_;
}

/**
 * @brief Checks if JSON member exists
 * @param json_member reference to JSON structure to check
 * @param name_of_member name which we should be checked
 * @returns true if member exists, otherwise returns false
 */
bool JsonIsMemberSafe(const Json::Value& json_member,
                      const char* name_of_member) {
  return !json_member.isNull() && json_member.isMember(name_of_member);
}

/**
 * @brief Converts specified string to appropriate enum value
 * according to schema
 * @return converted enum value
 */
template <typename EnumType>
EnumType ConvertStringToEnum(const std::string& str) {
  using ns_smart_device_link::ns_smart_objects::EnumConversionHelper;
  EnumType value;
  if (EnumConversionHelper<EnumType>::StringToEnum(str, &value)) {
    return value;
  }

  return EnumType::INVALID_ENUM;
}

/**
 * @brief Converts the JSON array of string type into the SmartArray of enums
 * @param json_array JSON value containing array
 * @param out_so_array output SmartArray
 */
template <typename EnumType>
void ConvertJsonArrayToSoArray(const Json::Value& json_array,
                               smart_objects::SmartObject& out_so_array) {
  out_so_array =
      smart_objects::SmartObject(smart_objects::SmartType::SmartType_Array);
  for (uint32_t i = 0; i < json_array.size(); ++i) {
    out_so_array[i] = ConvertStringToEnum<EnumType>(json_array[i].asString());
  }
}

/**
 * @brief Helper class for obtaining proper capabilities JSON value considering
 * case if it were overriden by cache
 */
struct JsonCapabilitiesGetter {
 public:
  /**
   * @brief JsonCapabilitiesGetter constructor
   * @param json_main_node reference to the main JSON capabilities node
   * @param json_override_node reference to overriden JSON capabilities node
   */
  JsonCapabilitiesGetter(Json::Value& json_main_node,
                         Json::Value& json_override_node)
      : json_main_node_(json_main_node)
      , json_override_node_(json_override_node) {}

  Json::Value GetInterfaceJsonMember(const char* member_name) {
    if (JsonIsMemberSafe(json_override_node_, member_name)) {
      return GetOverrideJsonMember(member_name);
    }

    if (JsonIsMemberSafe(json_main_node_, member_name)) {
      return GetMainJsonMember(member_name);
    }

    return Json::Value::null;
  }

  /**
   * @brief GetJsonMember gets JSON value for a specified JSON member from
   * overriden JSON node if member exists, otherwise main JSON node will be
   * taken
   * @param member_name name of the JSON member to get
   * @return JSON value for specified member or Value::null if not found
   */
  Json::Value GetJsonMember(
      const char* member_name,
      hmi_apis::FunctionID::eType request_id,
      std::set<hmi_apis::FunctionID::eType>& interfaces_from_default) {
    if (JsonIsMemberSafe(json_override_node_, member_name)) {
      return GetOverrideJsonMember(member_name);
    }

    if (JsonIsMemberSafe(json_main_node_, member_name)) {
      interfaces_from_default.insert(request_id);
      return GetMainJsonMember(member_name);
    }

    return Json::Value::null;
  }

  /**
   * @brief GetMainJsonMember gets JSON value for a specified JSON member from
   * the main JSON node
   * @param member_name name of the JSON member to get
   * @return JSON value for specified member or Value::null if not found
   */
  Json::Value GetMainJsonMember(const char* member_name) {
    return json_main_node_.get(member_name, Json::Value::null);
  }

  /**
   * @brief GetOverrideJsonMember gets JSON value for a specified JSON member
   * from the overriden JSON node
   * @param member_name name of the JSON member to get
   * @return JSON value for specified member or Value::null if not found
   */
  Json::Value GetOverrideJsonMember(const char* member_name) {
    return json_override_node_.get(member_name, Json::Value::null);
  }

 private:
  Json::Value& json_main_node_;
  Json::Value& json_override_node_;
};

bool HMICapabilitiesImpl::LoadCapabilitiesFromFile() {
  std::string json_string;
  const std::string file_name =
      app_mngr_.get_settings().hmi_capabilities_file_name();

  if (!file_system::FileExists(file_name)) {
    return false;
  }

  if (!file_system::ReadFile(file_name, json_string)) {
    return false;
  }

  const std::string cache_file_name =
      app_mngr_.get_settings().hmi_capabilities_cache_file_name();
  Json::Value root_json_override;

  if (file_system::FileExists(cache_file_name)) {
    LOG4CXX_DEBUG(logger_,
                  "HMI capabilities cache was found: " << cache_file_name);

    std::string cache_json_string;
    if (!file_system::ReadFile(cache_file_name, cache_json_string)) {
      LOG4CXX_DEBUG(
          logger_,
          "Failed to read data from cache file. Cache will be ignored");
    }

    Json::CharReaderBuilder reader_builder;
    const std::unique_ptr<Json::CharReader> reader(
        reader_builder.newCharReader());
    JSONCPP_STRING err;
    const size_t json_len = cache_json_string.length();
    if (!reader->parse(cache_json_string.c_str(),
                       cache_json_string.c_str() + json_len,
                       &root_json_override,
                       &err)) {
      LOG4CXX_ERROR(
          logger_,
          "Cached JSON file is invalid." << err << "Deleting the file");
      file_system::DeleteFile(cache_file_name);
      root_json_override =
          Json::Value::null;  // Just to clear intermediate state of value
    }
  }

  try {
    Json::CharReaderBuilder reader_builder;
    const std::unique_ptr<Json::CharReader> reader_(
        reader_builder.newCharReader());
    JSONCPP_STRING err;
    Json::Value root_json;
    const size_t json_len = json_string.length();

    const bool result = reader_->parse(
        json_string.c_str(), json_string.c_str() + json_len, &root_json, &err);

    if (!result) {
      LOG4CXX_DEBUG(logger_, "Json parsing fails: " << err);
      return false;
    }

    JsonCapabilitiesGetter json_root_getter(root_json, root_json_override);
    // UI

    if (!json_root_getter.GetInterfaceJsonMember(hmi_interface::ui).isNull()) {
      auto ui_main_node = json_root_getter.GetMainJsonMember(hmi_interface::ui);
      auto ui_override_node =
          json_root_getter.GetOverrideJsonMember(hmi_interface::ui);
      JsonCapabilitiesGetter json_ui_getter(ui_main_node, ui_override_node);

      auto ui_language_node =
          json_ui_getter.GetJsonMember(hmi_response::language,
                                       hmi_apis::FunctionID::UI_GetLanguage,
                                       interfaces_from_default_);

      if (!ui_language_node.isNull()) {
        const std::string lang = ui_language_node.asString();
        set_active_ui_language(MessageHelper::CommonLanguageFromString(lang));
      }

      auto ui_languages_node = json_ui_getter.GetJsonMember(
          hmi_response::languages,
          hmi_apis::FunctionID::UI_GetSupportedLanguages,
          interfaces_from_default_);
      if (!ui_languages_node.isNull()) {
        smart_objects::SmartObject ui_languages_so(
            smart_objects::SmartType_Array);
        ConvertJsonArrayToSoArray<hmi_apis::Common_Language::eType>(
            ui_languages_node, ui_languages_so);
        set_ui_supported_languages(ui_languages_so);
      }

      auto ui_display_capabilities_node =
          json_ui_getter.GetJsonMember(hmi_response::display_capabilities,
                                       hmi_apis::FunctionID::UI_GetCapabilities,
                                       interfaces_from_default_);
      if (!ui_display_capabilities_node.isNull()) {
        smart_objects::SmartObject display_capabilities_so;
        formatters::CFormatterJsonBase::jsonValueToObj(
            ui_display_capabilities_node, display_capabilities_so);

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
                                             [strings::name]
                                                 .asString());
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

      auto ui_audio_pass_thru_capabilities_node =
          json_ui_getter.GetJsonMember(strings::audio_pass_thru_capabilities,
                                       hmi_apis::FunctionID::UI_GetCapabilities,
                                       interfaces_from_default_);
      if (!ui_audio_pass_thru_capabilities_node.isNull()) {
        smart_objects::SmartObject audio_capabilities_so(
            smart_objects::SmartType_Array);
        if (ui_audio_pass_thru_capabilities_node.type() == Json::arrayValue) {
          for (uint32_t i = 0; i < ui_audio_pass_thru_capabilities_node.size();
               i++) {
            convert_audio_capability_to_obj(
                ui_audio_pass_thru_capabilities_node[i],
                audio_capabilities_so[i]);
          }
        } else if (ui_audio_pass_thru_capabilities_node.type() ==
                   Json::objectValue) {
          convert_audio_capability_to_obj(ui_audio_pass_thru_capabilities_node,
                                          audio_capabilities_so[0]);
        }
        set_audio_pass_thru_capabilities(audio_capabilities_so);
      }

      // ilytvynenko ToDo: change to UI_GetCapabilities
      auto ui_pcm_stream_capabilities_node =
          json_ui_getter.GetJsonMember(strings::pcm_stream_capabilities,
                                       hmi_apis::FunctionID::INVALID_ENUM,
                                       interfaces_from_default_);
      if (!ui_pcm_stream_capabilities_node.isNull()) {
        smart_objects::SmartObject pcm_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Map);
        convert_audio_capability_to_obj(ui_pcm_stream_capabilities_node,
                                        pcm_capabilities_so);
        set_pcm_stream_capabilities(pcm_capabilities_so);
      }

      auto ui_hmi_zone_capabilities_node =
          json_ui_getter.GetJsonMember(hmi_response::hmi_zone_capabilities,
                                       hmi_apis::FunctionID::UI_GetCapabilities,
                                       interfaces_from_default_);
      if (!ui_hmi_zone_capabilities_node.isNull()) {
        smart_objects::SmartObject hmi_zone_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        hmi_zone_capabilities_so =
            hmi_zone_enum.find(ui_hmi_zone_capabilities_node.asString())
                ->second;
        set_hmi_zone_capabilities(hmi_zone_capabilities_so);
      }

      auto ui_soft_button_capabilities_node =
          json_ui_getter.GetJsonMember(hmi_response::soft_button_capabilities,
                                       hmi_apis::FunctionID::UI_GetCapabilities,
                                       interfaces_from_default_);
      if (!ui_soft_button_capabilities_node.isNull()) {
        smart_objects::SmartObject soft_button_capabilities_so;
        formatters::CFormatterJsonBase::jsonValueToObj(
            ui_soft_button_capabilities_node, soft_button_capabilities_so);
        set_soft_button_capabilities(soft_button_capabilities_so);
      }

      auto ui_system_capabilities_capabilities_node =
          json_ui_getter.GetJsonMember(strings::system_capabilities,
                                       hmi_apis::FunctionID::UI_GetCapabilities,
                                       interfaces_from_default_);
      if (!ui_system_capabilities_capabilities_node.isNull()) {
        if (JsonIsMemberSafe(ui_system_capabilities_capabilities_node,
                             strings::navigation_capability)) {
          Json::Value navigation_capability =
              ui_system_capabilities_capabilities_node.get(
                  strings::navigation_capability, "");
          smart_objects::SmartObject navigation_capability_so;
          formatters::CFormatterJsonBase::jsonValueToObj(
              navigation_capability, navigation_capability_so);
          set_navigation_capability(navigation_capability_so);
          if (!navigation_capability_so.empty()) {
            set_navigation_supported(true);
          }
        }
        if (JsonIsMemberSafe(ui_system_capabilities_capabilities_node,
                             strings::phone_capability)) {
          Json::Value phone_capability =
              ui_system_capabilities_capabilities_node.get(
                  strings::phone_capability, "");
          smart_objects::SmartObject phone_capability_so;
          formatters::CFormatterJsonBase::jsonValueToObj(phone_capability,
                                                         phone_capability_so);
          set_phone_capability(phone_capability_so);
          if (!phone_capability_so.empty()) {
            set_phone_call_supported(true);
          }
        }
        if (JsonIsMemberSafe(ui_system_capabilities_capabilities_node,
                             strings::video_streaming_capability)) {
          Json::Value vs_capability =
              ui_system_capabilities_capabilities_node.get(
                  strings::video_streaming_capability, "");
          smart_objects::SmartObject vs_capability_so;
          formatters::CFormatterJsonBase::jsonValueToObj(vs_capability,
                                                         vs_capability_so);

          if (vs_capability_so.keyExists(strings::supported_formats)) {
            smart_objects::SmartObject& supported_format_array =
                vs_capability_so[strings::supported_formats];
            smart_objects::SmartObject converted_array(
                smart_objects::SmartType_Array);
            for (uint32_t i = 0, j = 0; i < supported_format_array.length();
                 i++) {
              if (!supported_format_array[i].keyExists(strings::protocol) ||
                  !supported_format_array[i].keyExists(strings::codec)) {
                continue;
              }

              std::map<std::string,
                       hmi_apis::Common_VideoStreamingProtocol::eType>::
                  const_iterator it_protocol =
                      video_streaming_protocol_enum.find(
                          supported_format_array[i][strings::protocol]
                              .asString());

              std::map<std::string,
                       hmi_apis::Common_VideoStreamingCodec::eType>::
                  const_iterator it_codec = video_streaming_codec_enum.find(
                      supported_format_array[i][strings::codec].asString());

              // format is valid only if both protocol and codec are converted
              // to enum values successfully
              if (it_protocol != video_streaming_protocol_enum.end() &&
                  it_codec != video_streaming_codec_enum.end()) {
                smart_objects::SmartObject format_so =
                    smart_objects::SmartObject(smart_objects::SmartType_Map);
                format_so[strings::protocol] = it_protocol->second;
                format_so[strings::codec] = it_codec->second;
                converted_array[j++] = format_so;
              }
            }
            vs_capability_so.erase(strings::supported_formats);
            vs_capability_so[strings::supported_formats] = converted_array;
          }
          set_video_streaming_capability(vs_capability_so);
          if (!vs_capability_so.empty()) {
            set_video_streaming_supported(true);
          }
        }
        if (JsonIsMemberSafe(ui_system_capabilities_capabilities_node,
                             strings::rc_capability)) {
          Json::Value rc_capability =
              ui_system_capabilities_capabilities_node.get(
                  strings::rc_capability, "");
          smart_objects::SmartObject rc_capability_so;
          formatters::CFormatterJsonBase::jsonValueToObj(rc_capability,
                                                         rc_capability_so);
          if (rc_capability_so.keyExists(
                  rc_rpc_plugin::strings::klightControlCapabilities)) {
            if (rc_capability_so
                    [rc_rpc_plugin::strings::klightControlCapabilities]
                        .keyExists(rc_rpc_plugin::strings::kSupportedLights)) {
              auto& lights = rc_capability_so
                  [rc_rpc_plugin::strings::klightControlCapabilities]
                  [rc_rpc_plugin::strings::kSupportedLights];
              auto it = lights.asArray()->begin();
              for (; it != lights.asArray()->end(); ++it) {
                smart_objects::SmartObject& light_name_so =
                    (*it)[strings::name];
                auto light_name = MessageHelper::CommonLightNameFromString(
                    light_name_so.asString());
                light_name_so = light_name;
              }
            }
          }
          set_rc_capability(rc_capability_so);
          if (!rc_capability_so.empty()) {
            set_rc_supported(true);
          }
        }
        if (JsonIsMemberSafe(ui_system_capabilities_capabilities_node,
                             strings::seat_location_capability)) {
          Json::Value seat_location_capability =
              ui_system_capabilities_capabilities_node.get(
                  strings::seat_location_capability, "");
          smart_objects::SmartObject seat_location_capability_so;
          formatters::CFormatterJsonBase::jsonValueToObj(
              seat_location_capability, seat_location_capability_so);
          set_seat_location_capability(seat_location_capability_so);
        }
      }
    }  // UI end

    // VR
    if (!json_root_getter.GetInterfaceJsonMember(hmi_interface::vr).isNull()) {
      auto vr_main_node = json_root_getter.GetMainJsonMember(hmi_interface::vr);
      auto vr_override_node =
          json_root_getter.GetOverrideJsonMember(hmi_interface::vr);
      JsonCapabilitiesGetter json_vr_getter(vr_main_node, vr_override_node);

      auto vr_language_node =
          json_vr_getter.GetJsonMember(hmi_response::language,
                                       hmi_apis::FunctionID::VR_GetLanguage,
                                       interfaces_from_default_);
      if (!vr_language_node.isNull()) {
        const std::string lang = vr_language_node.asString();
        set_active_vr_language(MessageHelper::CommonLanguageFromString(lang));
      }

      auto vr_languages_node = json_vr_getter.GetJsonMember(
          hmi_response::languages,
          hmi_apis::FunctionID::VR_GetSupportedLanguages,
          interfaces_from_default_);
      if (!vr_languages_node.isNull()) {
        smart_objects::SmartObject vr_languages_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        ConvertJsonArrayToSoArray<hmi_apis::Common_Language::eType>(
            vr_languages_node, vr_languages_so);
        set_vr_supported_languages(vr_languages_so);
      }

      auto vr_capabilities_node =
          json_vr_getter.GetJsonMember(strings::vr_capabilities,
                                       hmi_apis::FunctionID::VR_GetCapabilities,
                                       interfaces_from_default_);
      if (!vr_capabilities_node.isNull()) {
        smart_objects::SmartObject vr_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        ConvertJsonArrayToSoArray<hmi_apis::Common_VrCapabilities::eType>(
            vr_capabilities_node, vr_capabilities_so);
        set_vr_capabilities(vr_capabilities_so);
      }
    }  // VR end

    // TTS
    if (!json_root_getter.GetInterfaceJsonMember(hmi_interface::tts).isNull()) {
      auto tts_main_node =
          json_root_getter.GetMainJsonMember(hmi_interface::tts);
      auto tts_override_node =
          json_root_getter.GetOverrideJsonMember(hmi_interface::tts);
      JsonCapabilitiesGetter json_tts_getter(tts_main_node, tts_override_node);

      auto tts_language_node =
          json_tts_getter.GetJsonMember(hmi_response::language,
                                        hmi_apis::FunctionID::TTS_GetLanguage,
                                        interfaces_from_default_);
      if (!tts_language_node.isNull()) {
        const std::string lang = tts_language_node.asString();
        set_active_tts_language(MessageHelper::CommonLanguageFromString(lang));
      }

      auto tts_languages_node = json_tts_getter.GetJsonMember(
          hmi_response::languages,
          hmi_apis::FunctionID::TTS_GetSupportedLanguages,
          interfaces_from_default_);
      if (!tts_languages_node.isNull()) {
        smart_objects::SmartObject tts_languages_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        ConvertJsonArrayToSoArray<hmi_apis::Common_Language::eType>(
            tts_languages_node, tts_languages_so);
        set_tts_supported_languages(tts_languages_so);
      }

      auto tts_speech_capabilities_node = json_tts_getter.GetJsonMember(
          hmi_response::speech_capabilities,
          hmi_apis::FunctionID::TTS_GetCapabilities,
          interfaces_from_default_);
      if (!tts_speech_capabilities_node.isNull()) {
        smart_objects::SmartObject tts_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        ConvertJsonArrayToSoArray<hmi_apis::Common_SpeechCapabilities::eType>(
            tts_speech_capabilities_node, tts_capabilities_so);
        set_speech_capabilities(tts_capabilities_so);
      }

      auto tts_prerecorded_speech_capabilities_node =
          json_tts_getter.GetJsonMember(
              hmi_response::prerecorded_speech_capabilities,
              hmi_apis::FunctionID::TTS_GetCapabilities,
              interfaces_from_default_);
      if (!tts_prerecorded_speech_capabilities_node.isNull()) {
        smart_objects::SmartObject tts_capabilities_so =
            smart_objects::SmartObject(smart_objects::SmartType_Array);
        ConvertJsonArrayToSoArray<hmi_apis::Common_SpeechCapabilities::eType>(
            tts_prerecorded_speech_capabilities_node, tts_capabilities_so);
        set_prerecorded_speech(tts_capabilities_so);
      }
    }  // TTS end

    // Buttons
    if (!json_root_getter.GetInterfaceJsonMember(hmi_interface::buttons)
             .isNull()) {
      auto buttons_main_node =
          json_root_getter.GetMainJsonMember(hmi_interface::buttons);
      auto buttons_override_node =
          json_root_getter.GetOverrideJsonMember(hmi_interface::buttons);
      JsonCapabilitiesGetter json_buttons_getter(buttons_main_node,
                                                 buttons_override_node);

      auto buttons_capabilities_node = json_buttons_getter.GetJsonMember(
          hmi_response::capabilities,
          hmi_apis::FunctionID::Buttons_GetCapabilities,
          interfaces_from_default_);
      if (!buttons_capabilities_node.isNull()) {
        smart_objects::SmartObject buttons_capabilities_so;
        formatters::CFormatterJsonBase::jsonValueToObj(
            buttons_capabilities_node, buttons_capabilities_so);

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

      auto buttons_preset_bank_capabilities_node =
          json_buttons_getter.GetMainJsonMember(
              hmi_response::preset_bank_capabilities);
      if (!buttons_preset_bank_capabilities_node.isNull()) {
        smart_objects::SmartObject preset_bank_so;
        formatters::CFormatterJsonBase::jsonValueToObj(
            buttons_preset_bank_capabilities_node, preset_bank_so);
        set_preset_bank_capabilities(preset_bank_so);
      }
    }  // Buttons end

    // VehicleType

    if (!json_root_getter.GetInterfaceJsonMember(hmi_interface::vehicle_info)
             .isNull()) {
      auto vehicle_info_main_node =
          json_root_getter.GetMainJsonMember(hmi_interface::vehicle_info);
      auto vehicle_info_override_node =
          json_root_getter.GetOverrideJsonMember(hmi_interface::vehicle_info);
      JsonCapabilitiesGetter json_vehicle_info_getter(
          vehicle_info_main_node, vehicle_info_override_node);
      auto vehicle_type_node = json_vehicle_info_getter.GetJsonMember(
          hmi_response::vehicle_type,
          hmi_apis::FunctionID::VehicleInfo_GetVehicleType,
          interfaces_from_default_);
      if (!vehicle_type_node.isNull()) {
        smart_objects::SmartObject vehicle_type_so;
        formatters::CFormatterJsonBase::jsonValueToObj(vehicle_type_node,
                                                       vehicle_type_so);
        set_vehicle_type(vehicle_type_so);
      }
    }  // VehicleType end
  } catch (...) {
    return false;
  }
  return true;
}

hmi_apis::Common_Language::eType
HMICapabilitiesImpl::GetActiveLanguageForInterface(
    const char* interface_name) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (strcmp(hmi_interface::ui, interface_name) == 0) {
    return active_ui_language();
  }
  if (strcmp(hmi_interface::vr, interface_name) == 0) {
    return active_vr_language();
  }
  if (strcmp(hmi_interface::tts, interface_name) == 0) {
    return active_tts_language();
  }
  return hmi_apis::Common_Language::INVALID_ENUM;
}

std::set<hmi_apis::FunctionID::eType>
HMICapabilitiesImpl::GetInterfacesFromDefault() const {
  return interfaces_from_default_;
}

bool HMICapabilitiesImpl::AllFieldsSaved(
    const Json::Value& root_node,
    const char* interface_name,
    const std::vector<std::string>& sections_to_check) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!JsonIsMemberSafe(root_node, interface_name)) {
    LOG4CXX_DEBUG(logger_,
                  interface_name
                      << " interface is not found. All fields should be saved");
    return false;
  }

  const auto& interface_node = root_node.get(interface_name, Json::Value::null);
  for (auto it = sections_to_check.begin(); it != sections_to_check.end();
       ++it) {
    const std::string section = (*it).c_str();
    if (!JsonIsMemberSafe(interface_node, section.c_str())) {
      LOG4CXX_DEBUG(logger_,
                    "Field " << *it << " should be saved into the file");
      return false;
    }

    if (hmi_response::language == section) {
      const auto active_language =
          GetActiveLanguageForInterface(interface_name);
      const auto json_language = interface_node[hmi_response::language];

      if (active_language !=
          MessageHelper::CommonLanguageFromString(json_language.asString())) {
        LOG4CXX_DEBUG(logger_,
                      "Active " << interface_name
                                << " language is not the same as the persisted "
                                   "one. Field should be overwritten");
        return false;
      }
    }
  }

  return true;
}

void HMICapabilitiesImpl::PrepareUiJsonValueForSaving(
    const std::vector<std::string>& sections_to_update,
    const smart_objects::CSmartSchema& schema,
    Json::Value& out_node) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (helpers::in_range(sections_to_update,
                        hmi_response::display_capabilities)) {
    save_hmi_capability_field_to_json(hmi_response::display_capabilities,
                                      schema,
                                      display_capabilities(),
                                      out_node);
  }

  if (helpers::in_range(sections_to_update,
                        hmi_response::hmi_zone_capabilities)) {
    save_hmi_capability_field_to_json(hmi_response::hmi_zone_capabilities,
                                      schema,
                                      hmi_zone_capabilities(),
                                      out_node);
  }

  if (helpers::in_range(sections_to_update,
                        hmi_response::soft_button_capabilities)) {
    save_hmi_capability_field_to_json(hmi_response::soft_button_capabilities,
                                      schema,
                                      soft_button_capabilities(),
                                      out_node);
  }

  if (helpers::in_range(sections_to_update,
                        strings::audio_pass_thru_capabilities)) {
    save_hmi_capability_field_to_json(strings::audio_pass_thru_capabilities,
                                      schema,
                                      audio_pass_thru_capabilities(),
                                      out_node);
  }

  if (helpers::in_range(sections_to_update, strings::hmi_capabilities)) {
    out_node[strings::hmi_capabilities][strings::navigation] =
        navigation_supported();
    out_node[strings::hmi_capabilities][strings::phone_call] =
        phone_call_supported();
    out_node[strings::hmi_capabilities][strings::video_streaming] =
        video_streaming_supported();
  }

  if (helpers::in_range(sections_to_update, strings::system_capabilities)) {
    save_hmi_capability_field_to_json(strings::navigation_capability,
                                      schema,
                                      navigation_capability(),
                                      out_node[strings::system_capabilities]);
    save_hmi_capability_field_to_json(strings::phone_capability,
                                      schema,
                                      phone_capability(),
                                      out_node[strings::system_capabilities]);
    save_hmi_capability_field_to_json(strings::video_streaming_capability,
                                      schema,
                                      video_streaming_capability(),
                                      out_node[strings::system_capabilities]);
    save_hmi_capability_field_to_json(strings::display_capabilities,
                                      schema,
                                      system_display_capabilities(),
                                      out_node[strings::system_capabilities]);
  }

  if (helpers::in_range(sections_to_update, hmi_response::language)) {
    out_node[hmi_response::language] =
        MessageHelper::CommonLanguageToString(active_ui_language());
  }

  if (helpers::in_range(sections_to_update, hmi_response::languages)) {
    save_hmi_capability_field_to_json(
        hmi_response::languages, schema, ui_supported_languages(), out_node);
  }
}

void HMICapabilitiesImpl::PrepareVrJsonValueForSaving(
    const std::vector<std::string>& sections_to_update,
    const smart_objects::CSmartSchema& schema,
    Json::Value& out_node) const {
  UNUSED(schema);
  LOG4CXX_AUTO_TRACE(logger_);
  if (helpers::in_range(sections_to_update, hmi_response::language)) {
    out_node[hmi_response::language] =
        MessageHelper::CommonLanguageToString(active_vr_language());
  }

  if (helpers::in_range(sections_to_update, hmi_response::languages)) {
    save_hmi_capability_field_to_json(
        hmi_response::languages, schema, vr_supported_languages(), out_node);
  }

  if (helpers::in_range(sections_to_update, strings::vr_capabilities)) {
    save_hmi_capability_field_to_json(
        strings::vr_capabilities, schema, vr_capabilities(), out_node);
  }
}

void HMICapabilitiesImpl::PrepareTtsJsonValueForSaving(
    const std::vector<std::string>& sections_to_update,
    const smart_objects::CSmartSchema& schema,
    Json::Value& out_node) const {
  UNUSED(schema);
  LOG4CXX_AUTO_TRACE(logger_);
  if (helpers::in_range(sections_to_update, hmi_response::language)) {
    out_node[hmi_response::language] =
        MessageHelper::CommonLanguageToString(active_tts_language());
  }

  if (helpers::in_range(sections_to_update, hmi_response::languages)) {
    save_hmi_capability_field_to_json(
        hmi_response::languages, schema, tts_supported_languages(), out_node);
  }

  if (helpers::in_range(sections_to_update,
                        hmi_response::speech_capabilities)) {
    save_hmi_capability_field_to_json(hmi_response::speech_capabilities,
                                      schema,
                                      speech_capabilities(),
                                      out_node);
  }
  if (helpers::in_range(sections_to_update,
                        hmi_response::prerecorded_speech_capabilities)) {
    save_hmi_capability_field_to_json(
        hmi_response::prerecorded_speech_capabilities,
        schema,
        prerecorded_speech(),
        out_node);
  }
}

void HMICapabilitiesImpl::PrepareButtonsJsonValueForSaving(
    const std::vector<std::string>& sections_to_update,
    const smart_objects::CSmartSchema& schema,
    Json::Value& out_node) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (helpers::in_range(sections_to_update,
                        hmi_response::button_capabilities)) {
    save_hmi_capability_field_to_json(
        hmi_response::capabilities, schema, button_capabilities(), out_node);
  }

  if (helpers::in_range(sections_to_update,
                        hmi_response::preset_bank_capabilities)) {
    save_hmi_capability_field_to_json(hmi_response::preset_bank_capabilities,
                                      schema,
                                      preset_bank_capabilities(),
                                      out_node);
  }
}

void HMICapabilitiesImpl::PrepareVehicleInfoJsonValueForSaving(
    const std::vector<std::string>& sections_to_update,
    const smart_objects::CSmartSchema& schema,
    Json::Value& out_node) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (helpers::in_range(sections_to_update, hmi_response::vehicle_type)) {
    save_hmi_capability_field_to_json(
        hmi_response::vehicle_type, schema, vehicle_type(), out_node);
  }
}

void HMICapabilitiesImpl::PrepareRCJsonValueForSaving(
    const std::vector<std::string>& sections_to_update,
    const smart_objects::CSmartSchema& schema,
    Json::Value& out_node) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (helpers::in_range(sections_to_update, strings::rc_capability)) {
    save_hmi_capability_field_to_json(
        strings::rc_capability, schema, rc_capability(), out_node);
  }

  if (helpers::in_range(sections_to_update,
                        strings::seat_location_capability)) {
    save_hmi_capability_field_to_json(strings::seat_location_capability,
                                      schema,
                                      seat_location_capability(),
                                      out_node);
  }
}

void HMICapabilitiesImpl::PrepareJsonValueForSaving(
    const char* interface_name,
    const std::vector<std::string>& sections_to_update,
    const smart_objects::CSmartSchema& schema,
    Json::Value& out_root_node) const {
  LOG4CXX_DEBUG(logger_,
                "Prepare " << interface_name << " sections for saving");

  if (out_root_node.isNull()) {
    out_root_node = Json::Value(Json::objectValue);
  }

  if (!out_root_node.isMember(interface_name)) {
    out_root_node[interface_name] = Json::Value(Json::objectValue);
  }

  Json::Value& interface_node = out_root_node[interface_name];
  if (strcmp(interface_name, hmi_interface::ui) == 0) {
    PrepareUiJsonValueForSaving(sections_to_update, schema, interface_node);
  }

  if (strcmp(interface_name, hmi_interface::vr) == 0) {
    PrepareVrJsonValueForSaving(sections_to_update, schema, interface_node);
  }

  if (strcmp(interface_name, hmi_interface::tts) == 0) {
    PrepareTtsJsonValueForSaving(sections_to_update, schema, interface_node);
  }

  if (strcmp(interface_name, hmi_interface::buttons) == 0) {
    PrepareButtonsJsonValueForSaving(
        sections_to_update, schema, interface_node);
  }

  if (strcmp(interface_name, hmi_interface::vehicle_info) == 0) {
    PrepareVehicleInfoJsonValueForSaving(
        sections_to_update, schema, interface_node);
  }

  if (strcmp(interface_name, hmi_interface::rc) == 0) {
    PrepareRCJsonValueForSaving(sections_to_update, schema, interface_node);
  }
}

bool HMICapabilitiesImpl::SaveCachedCapabilitiesToFile(
    const std::string& interface_name,
    const std::vector<std::string>& sections_to_update,
    const smart_objects::CSmartSchema& schema) {
  LOG4CXX_AUTO_TRACE(logger_);

  const std::string cache_file_name =
      app_mngr_.get_settings().hmi_capabilities_cache_file_name();
  if (cache_file_name.empty()) {
    LOG4CXX_DEBUG(logger_,
                  "Cache file name is not specified. No need to save cache");
    return true;
  }

  Json::Value root_node;
  if (file_system::FileExists(cache_file_name)) {
    LOG4CXX_DEBUG(logger_, "Cache file exists. Check for it's content");

    std::string file_content;
    if (!file_system::ReadFile(cache_file_name, file_content)) {
      LOG4CXX_ERROR(logger_, "Failed to read file content");
      return false;
    }

    Json::CharReaderBuilder builder;
    const std::unique_ptr<Json::CharReader> reader(builder.newCharReader());
    if (!reader->parse(file_content.c_str(),
                       file_content.c_str() + file_content.length(),
                       &root_node,
                       NULL)) {
      LOG4CXX_ERROR(logger_, "Can't parse the file. Skipping");
      return false;
    }

    if (AllFieldsSaved(root_node, interface_name.c_str(), sections_to_update)) {
      LOG4CXX_DEBUG(
          logger_,
          "All " << interface_name
                 << " fields are present in the file. No need to update");
      return true;
    }

    LOG4CXX_DEBUG(logger_, "Some fields in the cache file should be updated");
  }

  PrepareJsonValueForSaving(
      interface_name.c_str(), sections_to_update, schema, root_node);

  LOG4CXX_DEBUG(logger_, "Saving cache to file: " << cache_file_name);
  const std::string content_to_save = root_node.toStyledString();
  const std::vector<uint8_t> binary_data_to_save(content_to_save.begin(),
                                                 content_to_save.end());

  return file_system::Write(cache_file_name, binary_data_to_save);
}

void HMICapabilitiesImpl::DeleteCachedCapabilitiesFile() const {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string cache_file_name =
      app_mngr_.get_settings().hmi_capabilities_cache_file_name();
  if (cache_file_name.empty()) {
    LOG4CXX_DEBUG(logger_,
                  "Cache file name is not specified. Nothing to delete");
    return;
  }

  if (!file_system::FileExists(cache_file_name)) {
    LOG4CXX_DEBUG(logger_, "Cache file does not exist");
    return;
  }

  if (!file_system::DeleteFile(cache_file_name)) {
    LOG4CXX_ERROR(logger_, "Failed to delete cache file");
  }
}

void HMICapabilitiesImpl::set_ccpu_version(const std::string& ccpu_version) {
  ccpu_version_ = ccpu_version;
}

const std::string& HMICapabilitiesImpl::ccpu_version() const {
  return ccpu_version_;
}

void HMICapabilitiesImpl::convert_json_languages_to_obj(
    const Json::Value& json_languages,
    smart_objects::SmartObject& languages) const {
  for (uint32_t i = 0, j = 0; i < json_languages.size(); ++i) {
    languages[j++] =
        MessageHelper::CommonLanguageFromString(json_languages[i].asString());
  }
}

void HMICapabilitiesImpl::convert_audio_capability_to_obj(
    const Json::Value& capability,
    smart_objects::SmartObject& output_so) const {
  if (JsonIsMemberSafe(capability, "samplingRate")) {
    output_so[strings::sampling_rate] =
        sampling_rate_enum.find(capability.get("samplingRate", "").asString())
            ->second;
  }
  if (JsonIsMemberSafe(capability, "bitsPerSample")) {
    output_so[strings::bits_per_sample] =
        bit_per_sample_enum
            .find(capability.get("bitsPerSample", "").asString())
            ->second;
  }
  if (JsonIsMemberSafe(capability, "audioType")) {
    output_so[strings::audio_type] =
        audio_type_enum.find(capability.get("audioType", "").asString())
            ->second;
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

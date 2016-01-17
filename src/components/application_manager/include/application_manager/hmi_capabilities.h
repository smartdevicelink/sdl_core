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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_H_

#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "json/json.h"
#include "utils/macro.h"
#include "application_manager/hmi_language_handler.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;
}
}

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

namespace application_manager {
class ApplicationManagerImpl;

class HMICapabilities {

 public:

  /*
   * @ Class constructor
   *
   * @param app_mngr Application manager pointer
   */
  explicit HMICapabilities(ApplicationManagerImpl* const app_mngr);

  /*
   * @brief Class destructor
   *
   */
  virtual ~HMICapabilities();

  /**
   * @brief Checks if all HMI capabilities received
   *
   * @return TRUE if all information received, otherwise FALSE
   */
  bool is_hmi_capabilities_initialized() const;

  /*
   * @brief Checks is image type(Static/Dynamic) requested by
   * Mobile Device is supported on current HMI.
   * @param image_type recieved type of image from Enum.
   * @return Bool true if supported
   */
  bool VerifyImageType(int32_t image_type) const;

  /**
   * @brief Checks if all HMI capabilities received
   *
   * @return TRUE if all information received, otherwise FALSE
   */
  inline bool is_vr_cooperating() const;
  void set_is_vr_cooperating(bool value);

  inline bool is_tts_cooperating() const;
  void set_is_tts_cooperating(bool value);

  inline bool is_ui_cooperating() const;
  void set_is_ui_cooperating(bool value);

  inline bool is_navi_cooperating() const;
  void set_is_navi_cooperating(bool value);

  inline bool is_ivi_cooperating() const;
  void set_is_ivi_cooperating(bool value);

  /*
   * @brief Retrieves if mixing audio is supported by HMI
   * (ie recording TTS command and playing audio)
   *
   * @return Current state of the mixing audio flag
   */
  inline bool attenuated_supported() const;

  /*
   * @brief Sets state for mixing audio
   *
   * @param state New state to be set
   */
  void set_attenuated_supported(bool state);

  /*
   * @brief Retrieves currently active UI language
   *
   * @return Currently active UI language
   */
  const hmi_apis::Common_Language::eType active_ui_language() const;

  /*
   * @brief Sets currently active UI language
   *
   * @param language Currently active UI language
   */
  void set_active_ui_language(const hmi_apis::Common_Language::eType& language);

  /*
   * @brief Retrieves UI supported languages
   *
   * @return Currently supported UI languages
   */
  inline const smart_objects::SmartObject*
  ui_supported_languages() const;

  /*
   * @brief Sets supported UI languages
   *
   * @param supported_languages Supported UI languages
   */
  void set_ui_supported_languages(
      const smart_objects::SmartObject& supported_languages);

  /*
   * @brief Retrieves currently active VR language
   *
   * @return Currently active VR language
   */
  const hmi_apis::Common_Language::eType active_vr_language() const;

  /*
   * @brief Sets currently active VR language
   *
   * @param language Currently active VR language
   */
  void set_active_vr_language(const hmi_apis::Common_Language::eType& language);

  /*
   * @brief Retrieves VR supported languages
   *
   * @return Currently supported VR languages
   */
  inline const smart_objects::SmartObject*
  vr_supported_languages() const;

  /*
   * @brief Sets supported VR languages
   *
   * @param supported_languages Supported VR languages
   */
  void set_vr_supported_languages(
      const smart_objects::SmartObject& supported_languages);

  /*
   * @brief Retrieves currently active TTS language
   *
   * @return Currently active TTS language
   */
  const hmi_apis::Common_Language::eType active_tts_language() const;

  /*
   * @brief Sets currently active TTS language
   *
   * @param language Currently active TTS language
   */
  void set_active_tts_language(
    const hmi_apis::Common_Language::eType& language);

  /*
   * @brief Retrieves TTS  supported languages
   *
   * @return Currently supported TTS languages
   */
  inline const smart_objects::SmartObject*
  tts_supported_languages() const;

  /*
   * @brief Sets supported TTS languages
   *
   * @param supported_languages Supported TTS languages
   */
  void set_tts_supported_languages(
      const smart_objects::SmartObject& supported_languages);

  /*
   * @brief Retrieves information about the display capabilities
   *
   * @return Currently supported display capabilities
   */
  inline const smart_objects::SmartObject*
  display_capabilities() const;

  /*
   * @brief Sets supported display capabilities
   *
   * @param display_capabilities supported display capabilities
   */
  void set_display_capabilities(
      const smart_objects::SmartObject& display_capabilities);

  /*
   * @brief Retrieves information about the HMI zone capabilities
   *
   * @return Currently supported HMI zone capabilities
   */
  inline const smart_objects::SmartObject*
  hmi_zone_capabilities() const;

  /*
   * @brief Sets supported HMI zone capabilities
   *
   * @param hmi_zone_capabilities supported HMI zone capabilities
   */
  void set_hmi_zone_capabilities(
      const smart_objects::SmartObject& hmi_zone_capabilities);

  /*
   * @brief Retrieves information about the SoftButton's capabilities
   *
   * @return Currently supported SoftButton's capabilities
   */
  inline const smart_objects::SmartObject*
  soft_button_capabilities() const;

  /*
   * @brief Sets supported SoftButton's capabilities
   *
   * @param soft_button_capabilities supported SoftButton's capabilities
   */
  void set_soft_button_capabilities(
      const smart_objects::SmartObject& soft_button_capabilities);

  /*
   * @brief Retrieves information about the Button's capabilities
   *
   * @return Currently supported Button's capabilities
   */
  inline const smart_objects::SmartObject* button_capabilities() const;

  /*
   * @brief Sets supported Button's capabilities
   *
   * @param soft_button_capabilities supported Button's capabilities
   */
  void set_button_capabilities(
      const smart_objects::SmartObject& button_capabilities);

  /*
   * @brief Sets supported speech capabilities
   *
   * @param speech_capabilities supported speech capabilities
   */
  void set_speech_capabilities(
      const smart_objects::SmartObject& speech_capabilities);

  /*
   * @brief Retrieves information about the speech capabilities
   *
   * @return Currently supported speech capabilities
   */
  inline const smart_objects::SmartObject* speech_capabilities() const;

  /*
   * @brief Sets supported VR capabilities
   *
   * @param vr_capabilities supported VR capabilities
   */
  void set_vr_capabilities(const smart_objects::SmartObject& vr_capabilities);

  /*
   * @brief Retrieves information about the VR capabilities
   *
   * @return Currently supported VR capabilities
   */
  inline const smart_objects::SmartObject* vr_capabilities() const;

  /*
   * @brief Sets supported audio_pass_thru capabilities
   *
   * @param vr_capabilities supported audio_pass_thru capabilities
   */
  void set_audio_pass_thru_capabilities(
      const smart_objects::SmartObject& audio_pass_thru_capabilities);

  /*
   * @brief Retrieves information about the audio_pass_thru capabilities
   *
   * @return Currently supported audio_pass_thru capabilities
   */
  inline const smart_objects::SmartObject*
  audio_pass_thru_capabilities() const;

  /*
   * @brief Retrieves information about the preset bank capabilities
   *
   * @return Currently supported preset bank capabilities
   */
  inline const smart_objects::SmartObject* preset_bank_capabilities() const;

  /*
   * @brief Sets supported preset bank capabilities
   *
   * @param soft_button_capabilities supported preset bank capabilities
   */
  void set_preset_bank_capabilities(
      const smart_objects::SmartObject& preset_bank_capabilities);

  /*
   * @brief Sets vehicle information(make, model, modelYear)
   *
   * @param vehicle_type Cuurent vehicle information
   */
  void set_vehicle_type(const smart_objects::SmartObject& vehicle_type);

  /*
   * @brief Retrieves vehicle information(make, model, modelYear)
   *
   * @param vehicle_type Cuurent vehicle information
   */
  inline const smart_objects::SmartObject* vehicle_type() const;

  /*
   * @brief Retrieves information about the prerecorded speech
   *
   * @return Currently supported prerecorded speech
   */
  inline const smart_objects::SmartObject* prerecorded_speech() const;

  /*
   * @brief Sets supported prerecorded speech
   *
   * @param prerecorded_speech supported prerecorded speech
   */
  void set_prerecorded_speech(
       const smart_objects::SmartObject& prerecorded_speech);

  /*
   * @brief Interface used to store information if navigation
   * supported by the system
   *
   * @param supported Indicates if navigation supported by the system
   */
  void set_navigation_supported(bool supported);

  /*
   * @brief Retrieves information if navi supported by the system
   *
   * @return TRUE if it supported, otherwise FALSE
   */
  inline bool navigation_supported() const;

  /*
   * @brief Interface used to store information if phone call
   * supported by the system
   *
   * @param supported Indicates if navigation supported by the sustem
   */
  void set_phone_call_supported(bool supported);

  /*
   * @brief Retrieves information if phone call supported by the system
   *
   * @return TRUE if it supported, otherwise FALSE
   */
  inline bool phone_call_supported() const;

  /*
   * @brief Interface used to store information about software version of the target
   *
   * @param ccpu_version Received system/hmi software version
   */
  void set_ccpu_version(const std::string& ccpu_version);

  /*
   * @brief Returns software version of the target
   *
   * @return TRUE if it supported, otherwise FALSE
   */
  inline const std::string& ccpu_version() const;

 protected:

  /*
   * @brief Loads capabilities from local file in case SDL was launched
   * without HMI
   *
   * @return TRUE if capabilities loaded successfully, otherwise FALSE.
   */
  bool load_capabilities_from_file();

  /*
   * @brief function checks if json member exists
   *
   * @param json_member from file hmi_capabilities.json
   * @param name_of_member name which we should check
   *     hmi_capabilities.json
   *
   * @returns TRUE if member exists and returns FALSE if
   * member does not exist.
   */
  bool check_existing_json_member(
      const Json::Value& json_member, const char* name_of_member);

  /*
   * @brief function converts json object "languages" to smart object
   *
   * @param json_languages from file hmi_capabilities.json
   * @param languages - the converted object
   *
   */
  void convert_json_languages_to_obj(Json::Value& json_languages,
                                     smart_objects::SmartObject& languages);

 private:
  bool                             is_vr_cooperating_;
  bool                             is_tts_cooperating_;
  bool                             is_ui_cooperating_;
  bool                             is_navi_cooperating_;
  bool                             is_ivi_cooperating_;

  // to check if IsReady response for corresponding interface received
  bool                             is_vr_ready_response_recieved_;
  bool                             is_tts_ready_response_recieved_;
  bool                             is_ui_ready_response_recieved_;
  bool                             is_navi_ready_response_recieved_;
  bool                             is_ivi_ready_response_recieved_;

  bool                             attenuated_supported_;
  hmi_apis::Common_Language::eType ui_language_;
  hmi_apis::Common_Language::eType vr_language_;
  hmi_apis::Common_Language::eType tts_language_;
  smart_objects::SmartObject*      vehicle_type_;
  smart_objects::SmartObject*      ui_supported_languages_;
  smart_objects::SmartObject*      tts_supported_languages_;
  smart_objects::SmartObject*      vr_supported_languages_;
  smart_objects::SmartObject*      display_capabilities_;
  smart_objects::SmartObject*      hmi_zone_capabilities_;
  smart_objects::SmartObject*      soft_buttons_capabilities_;
  smart_objects::SmartObject*      button_capabilities_;
  smart_objects::SmartObject*      preset_bank_capabilities_;
  smart_objects::SmartObject*      vr_capabilities_;
  smart_objects::SmartObject*      speech_capabilities_;
  smart_objects::SmartObject*      audio_pass_thru_capabilities_;
  smart_objects::SmartObject*      prerecorded_speech_;
  std::string                      ccpu_version_;
  bool                             is_navigation_supported_;
  bool                             is_phone_call_supported_;

  ApplicationManagerImpl*          app_mngr_;
  HMILanguageHandler               hmi_language_handler_;

  DISALLOW_COPY_AND_ASSIGN(HMICapabilities);
};

bool HMICapabilities::is_ui_cooperating() const {
  return is_ui_cooperating_;
}

bool HMICapabilities::is_vr_cooperating() const {
  return is_vr_cooperating_;
}

bool HMICapabilities::is_tts_cooperating() const {
  return is_tts_cooperating_;
}

bool HMICapabilities::is_navi_cooperating() const {
  return is_navi_cooperating_;
}

bool HMICapabilities::is_ivi_cooperating() const {
  return is_ivi_cooperating_;
}

const smart_objects::SmartObject*
HMICapabilities::ui_supported_languages() const {
  return ui_supported_languages_;
}

const smart_objects::SmartObject*
HMICapabilities::vr_supported_languages() const {
  return vr_supported_languages_;
}

const smart_objects::SmartObject*
HMICapabilities::tts_supported_languages() const {
  return tts_supported_languages_;
}

const smart_objects::SmartObject*
HMICapabilities::display_capabilities() const {
  return display_capabilities_;
}

const smart_objects::SmartObject*
HMICapabilities::hmi_zone_capabilities() const {
  return hmi_zone_capabilities_;
}

const smart_objects::SmartObject*
HMICapabilities::soft_button_capabilities() const {
  return soft_buttons_capabilities_;
}

const smart_objects::SmartObject* HMICapabilities::button_capabilities() const {
  return button_capabilities_;
}

const smart_objects::SmartObject*
HMICapabilities::speech_capabilities() const {
  return speech_capabilities_;
}

const smart_objects::SmartObject* HMICapabilities::vr_capabilities() const {
  return vr_capabilities_;
}

const smart_objects::SmartObject*
HMICapabilities::audio_pass_thru_capabilities() const {
  return audio_pass_thru_capabilities_;
}

const smart_objects::SmartObject*
HMICapabilities::preset_bank_capabilities() const {
  return preset_bank_capabilities_;
}

bool HMICapabilities::attenuated_supported() const {
  return attenuated_supported_;
}

const smart_objects::SmartObject* HMICapabilities::vehicle_type() const {
  return vehicle_type_;
}

const smart_objects::SmartObject*
HMICapabilities::prerecorded_speech() const {
  return prerecorded_speech_;
}

const std::string& HMICapabilities::ccpu_version() const {
  return ccpu_version_;
}

bool HMICapabilities::navigation_supported() const {
  return is_navigation_supported_;
}

bool HMICapabilities::phone_call_supported() const {
  return is_phone_call_supported_;
}

}  //  namespace application_manager

#endif  //  SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_H_

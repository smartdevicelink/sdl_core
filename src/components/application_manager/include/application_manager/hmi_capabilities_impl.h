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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_IMPL_H_

#include "application_manager/hmi_capabilities.h"
#include "application_manager/hmi_language_handler.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "json/json.h"
#include "smart_objects/smart_object.h"
#include "utils/macro.h"

namespace resumption {
class LastState;
}  // namespace resumption

namespace application_manager {
class ApplicationManager;

class HMICapabilitiesImpl : public HMICapabilities {
 public:
  /*
   * @ Class constructor
   *
   * @param app_mngr Application manager pointer
   */
  explicit HMICapabilitiesImpl(ApplicationManager& app_mngr);

  /*
   * @brief Class destructor
   *
   */
  virtual ~HMICapabilitiesImpl();

  /*
   * @brief Checks is image type(Static/Dynamic) requested by
   * Mobile Device is supported on current HMI.
   * @param image_type recieved type of image from Enum.
   * @return Bool true if supported
   */
  bool VerifyImageType(const int32_t image_type) const OVERRIDE;

  /**
   * @brief Checks if all HMI capabilities received
   *
   * @return TRUE if all information received, otherwise FALSE
   */
  bool is_vr_cooperating() const OVERRIDE;
  void set_is_vr_cooperating(const bool value) OVERRIDE;

  bool is_tts_cooperating() const OVERRIDE;
  void set_is_tts_cooperating(const bool value) OVERRIDE;

  bool is_ui_cooperating() const OVERRIDE;
  void set_is_ui_cooperating(const bool value) OVERRIDE;

  bool is_navi_cooperating() const OVERRIDE;
  void set_is_navi_cooperating(const bool value) OVERRIDE;

  bool is_ivi_cooperating() const OVERRIDE;
  void set_is_ivi_cooperating(const bool value) OVERRIDE;

  bool is_rc_cooperating() const OVERRIDE;
  void set_is_rc_cooperating(const bool value) OVERRIDE;

  /*
   * @brief Interface used to store information about software version of the
   *target
   *
   * @param ccpu_version Received system/hmi software version
   */
  void set_ccpu_version(const std::string& ccpu_version) OVERRIDE;

  /*
   * @brief Returns software version of the target
   *
   * @return TRUE if it supported, otherwise FALSE
   */
  const std::string& ccpu_version() const OVERRIDE;

  /*
   * @brief Retrieves if mixing audio is supported by HMI
   * (ie recording TTS command and playing audio)
   *
   * @return Current state of the mixing audio flag
   */
  bool attenuated_supported() const OVERRIDE;

  /*
   * @brief Sets state for mixing audio
   *
   * @param state New state to be set
   */
  void set_attenuated_supported(const bool state) OVERRIDE;

  /*
   * @brief Retrieves currently active UI language
   *
   * @return Currently active UI language
   */
  const hmi_apis::Common_Language::eType active_ui_language() const OVERRIDE;

  /*
   * @brief Sets currently active UI language
   *
   * @param language Currently active UI language
   */
  void set_active_ui_language(
      const hmi_apis::Common_Language::eType language) OVERRIDE;

  /*
   * @brief Retrieves UI supported languages
   *
   * @return Currently supported UI languages
   */
  const smart_objects::SmartObject* ui_supported_languages() const OVERRIDE;

  /*
   * @brief Sets supported UI languages
   *
   * @param supported_languages Supported UI languages
   */
  void set_ui_supported_languages(
      const smart_objects::SmartObject& supported_languages) OVERRIDE;

  /*
   * @brief Retrieves currently active VR language
   *
   * @return Currently active VR language
   */
  const hmi_apis::Common_Language::eType active_vr_language() const OVERRIDE;

  /*
   * @brief Sets currently active VR language
   *
   * @param language Currently active VR language
   */
  void set_active_vr_language(
      const hmi_apis::Common_Language::eType language) OVERRIDE;

  /*
   * @brief Retrieves VR supported languages
   *
   * @return Currently supported VR languages
   */
  const smart_objects::SmartObject* vr_supported_languages() const OVERRIDE;

  /*
   * @brief Sets supported VR languages
   *
   * @param supported_languages Supported VR languages
   */
  void set_vr_supported_languages(
      const smart_objects::SmartObject& supported_languages) OVERRIDE;

  /*
   * @brief Retrieves currently active TTS language
   *
   * @return Currently active TTS language
   */
  const hmi_apis::Common_Language::eType active_tts_language() const OVERRIDE;

  /*
   * @brief Sets currently active TTS language
   *
   * @param language Currently active TTS language
   */
  void set_active_tts_language(
      const hmi_apis::Common_Language::eType language) OVERRIDE;

  /*
   * @brief Retrieves TTS  supported languages
   *
   * @return Currently supported TTS languages
   */
  const smart_objects::SmartObject* tts_supported_languages() const OVERRIDE;

  /*
   * @brief Sets supported TTS languages
   *
   * @param supported_languages Supported TTS languages
   */
  void set_tts_supported_languages(
      const smart_objects::SmartObject& supported_languages) OVERRIDE;

  /*
   * @brief Retrieves information about the display capabilities
   *
   * @return Currently supported display capabilities
   */
  const smart_objects::SmartObject* display_capabilities() const OVERRIDE;

  /*
   * @brief Sets supported display capabilities
   *
   * @param display_capabilities supported display capabilities
   */
  void set_display_capabilities(
      const smart_objects::SmartObject& display_capabilities) OVERRIDE;

  /*
   * @brief Retrieves information about the display capability
   * @return Currently supported display capability
   */
  const smart_objects::SmartObjectSPtr system_display_capabilities()
      const OVERRIDE;

  /*
   * @brief Sets supported display capability
   * @param display_capabilities supported display capability
   */
  void set_system_display_capabilities(
      const smart_objects::SmartObject& display_capabilities);

  /*
   * @brief Retrieves information about the HMI zone capabilities
   *
   * @return Currently supported HMI zone capabilities
   */
  const smart_objects::SmartObject* hmi_zone_capabilities() const OVERRIDE;

  /*
   * @brief Sets supported HMI zone capabilities
   *
   * @param hmi_zone_capabilities supported HMI zone capabilities
   */
  void set_hmi_zone_capabilities(
      const smart_objects::SmartObject& hmi_zone_capabilities) OVERRIDE;

  /*
   * @brief Retrieves information about the SoftButton's capabilities
   *
   * @return Currently supported SoftButton's capabilities
   */
  const smart_objects::SmartObject* soft_button_capabilities() const OVERRIDE;

  /*
   * @brief Sets supported SoftButton's capabilities
   *
   * @param soft_button_capabilities supported SoftButton's capabilities
   */
  void set_soft_button_capabilities(
      const smart_objects::SmartObject& soft_button_capabilities) OVERRIDE;

  /*
   * @brief Retrieves information about the Button's capabilities
   *
   * @return Currently supported Button's capabilities
   */
  const smart_objects::SmartObject* button_capabilities() const OVERRIDE;

  /*
   * @brief Sets supported Button's capabilities
   *
   * @param soft_button_capabilities supported Button's capabilities
   */
  void set_button_capabilities(
      const smart_objects::SmartObject& button_capabilities) OVERRIDE;

  /*
   * @brief Sets supported speech capabilities
   *
   * @param speech_capabilities supported speech capabilities
   */
  void set_speech_capabilities(
      const smart_objects::SmartObject& speech_capabilities) OVERRIDE;

  /*
   * @brief Retrieves information about the speech capabilities
   *
   * @return Currently supported speech capabilities
   */
  const smart_objects::SmartObject* speech_capabilities() const OVERRIDE;

  /*
   * @brief Sets supported VR capabilities
   *
   * @param vr_capabilities supported VR capabilities
   */
  void set_vr_capabilities(
      const smart_objects::SmartObject& vr_capabilities) OVERRIDE;

  /*
   * @brief Retrieves information about the VR capabilities
   *
   * @return Currently supported VR capabilities
   */
  const smart_objects::SmartObject* vr_capabilities() const OVERRIDE;

  /*
   * @brief Sets supported audio_pass_thru capabilities
   *
   * @param vr_capabilities supported audio_pass_thru capabilities
   */
  void set_audio_pass_thru_capabilities(
      const smart_objects::SmartObject& audio_pass_thru_capabilities) OVERRIDE;

  /*
   * @brief Retrieves information about the audio_pass_thru capabilities
   *
   * @return Currently supported audio_pass_thru capabilities
   */
  const smart_objects::SmartObject* audio_pass_thru_capabilities()
      const OVERRIDE;

  /*
   * @brief Sets supported pcm_stream capabilities
   *
   * @param supported pcm_stream capabilities
   */
  void set_pcm_stream_capabilities(
      const smart_objects::SmartObject& pcm_stream_capabilities) OVERRIDE;

  /*
   * @brief Retrieves information about the pcm_stream capabilities
   *
   * @return Currently supported pcm_streaming capabilities
   */
  const smart_objects::SmartObject* pcm_stream_capabilities() const OVERRIDE;

  /*
   * @brief Retrieves information about the preset bank capabilities
   *
   * @return Currently supported preset bank capabilities
   */
  const smart_objects::SmartObject* preset_bank_capabilities() const OVERRIDE;

  /*
   * @brief Sets supported preset bank capabilities
   *
   * @param soft_button_capabilities supported preset bank capabilities
   */
  void set_preset_bank_capabilities(
      const smart_objects::SmartObject& preset_bank_capabilities) OVERRIDE;

  /*
   * @brief Sets vehicle information(make, model, modelYear)
   *
   * @param vehicle_type Cuurent vehicle information
   */
  void set_vehicle_type(
      const smart_objects::SmartObject& vehicle_type) OVERRIDE;

  /*
   * @brief Retrieves vehicle information(make, model, modelYear)
   *
   * @param vehicle_type Cuurent vehicle information
   */
  const smart_objects::SmartObject* vehicle_type() const OVERRIDE;

  /*
   * @brief Retrieves information about the prerecorded speech
   *
   * @return Currently supported prerecorded speech
   */
  const smart_objects::SmartObject* prerecorded_speech() const OVERRIDE;

  /*
   * @brief Sets supported prerecorded speech
   *
   * @param prerecorded_speech supported prerecorded speech
   */
  void set_prerecorded_speech(
      const smart_objects::SmartObject& prerecorded_speech) OVERRIDE;

  /*
   * @brief Interface used to store information if navigation
   * supported by the system
   *
   * @param supported Indicates if navigation supported by the system
   */
  void set_navigation_supported(const bool supported) OVERRIDE;

  /*
   * @brief Retrieves information if navi supported by the system
   *
   * @return TRUE if it supported, otherwise FALSE
   */
  bool navigation_supported() const OVERRIDE;

  /*
   * @brief Interface used to store information if phone call
   * supported by the system
   *
   * @param supported Indicates if navigation supported by the sustem
   */
  void set_phone_call_supported(const bool supported) OVERRIDE;

  /*
   * @brief Retrieves information if phone call supported by the system
   *
   * @return TRUE if it supported, otherwise FALSE
   */
  bool phone_call_supported() const OVERRIDE;

  /*
   * @brief Interface to store whether HMI supports video streaming
   *
   * @param supported Indicates whether video streaming is supported by HMI
   */
  void set_video_streaming_supported(const bool supported) OVERRIDE;

  /*
   * @brief Retrieves whether HMI supports video streaming
   *
   * @return TRUE if it supported, otherwise FALSE
   */
  bool video_streaming_supported() const OVERRIDE;

  /*
   * @brief Interface to store whether HMI supports remote control
   *
   * @param supported Indicates whether video streaming is supported by HMI
   */
  void set_rc_supported(const bool supported) OVERRIDE;

  /*
   * @brief Retrieves whether HMI supports remote control
   *
   * @return TRUE if it supported, otherwise FALSE
   */
  bool rc_supported() const OVERRIDE;

  /*
   * @brief Retrieves whether HMI supports passed Menu Layout
   *
   * @return TRUE if it supported, otherwise FALSE
   */
  bool menu_layout_supported(
      mobile_apis::MenuLayout::eType layout) const OVERRIDE;

  /*
   * @brief Interface used to store information regarding
   * the navigation "System Capability"
   *
   * @param navigation_capability contains information related
   * to the navigation system capability.
   */
  void set_navigation_capability(
      const smart_objects::SmartObject& navigation_capability) OVERRIDE;

  /*
   * @brief Retrieves information regarding the navigation system capability
   *
   * @return NAVIGATION system capability
   */
  const smart_objects::SmartObject* navigation_capability() const OVERRIDE;

  /*
   * @brief Interface used to store information regarding
   * the phone "System Capability"
   *
   * @param phone_capability contains information related
   * to the phone system capability.
   */
  void set_phone_capability(
      const smart_objects::SmartObject& phone_capability) OVERRIDE;

  /*
   * @brief Retrieves information regarding the phone call system capability
   *
   * @return PHONE_CALL system capability
   */

  const smart_objects::SmartObject* phone_capability() const OVERRIDE;

  /*
   * @brief Sets HMI's video streaming related capability information
   *
   * @param video_streaming_capability the video streaming related capabilities
   */
  void set_video_streaming_capability(
      const smart_objects::SmartObject& video_streaming_capability) OVERRIDE;

  /*
   * @brief Retrieves HMI's video streaming related capabilities
   *
   * @return HMI's video streaming related capability information
   */
  const smart_objects::SmartObject* video_streaming_capability() const OVERRIDE;

  void set_rc_capability(
      const smart_objects::SmartObject& rc_capability) OVERRIDE;

  const smart_objects::SmartObject* rc_capability() const OVERRIDE;

  void Init(resumption::LastState* last_state) OVERRIDE;

  /*
   * @brief return component which follows for correctness of
   * languages
   * @return HMI language handler
   */
  HMILanguageHandler& get_hmi_language_handler() OVERRIDE;

  /**
   * @brief Trigger waiting for response
   * @param request Request object
   */
  void set_handle_response_for(
      const smart_objects::SmartObject& request) OVERRIDE;

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
  bool check_existing_json_member(const Json::Value& json_member,
                                  const char* name_of_member) const OVERRIDE;

  /*
   * @brief function converts json object "languages" to smart object
   *
   * @param json_languages from file hmi_capabilities.json
   * @param languages - the converted object
   *
   */
  void convert_json_languages_to_obj(
      const Json::Value& json_languages,
      smart_objects::SmartObject& languages) const OVERRIDE;

  /*
   * @brief function that converts a single entry of audio pass thru capability
   *        to smart object
   *
   * @param capability json object that represents a single entry of audio pass
   *        thru capability
   * @param output_so the converted object
   */
  void convert_audio_capability_to_obj(
      const Json::Value& capability,
      smart_objects::SmartObject& output_so) const OVERRIDE;

 private:
  bool is_vr_cooperating_;
  bool is_tts_cooperating_;
  bool is_ui_cooperating_;
  bool is_navi_cooperating_;
  bool is_ivi_cooperating_;
  bool is_rc_cooperating_;

  bool attenuated_supported_;
  hmi_apis::Common_Language::eType ui_language_;
  hmi_apis::Common_Language::eType vr_language_;
  hmi_apis::Common_Language::eType tts_language_;
  smart_objects::SmartObject* vehicle_type_;
  smart_objects::SmartObject* ui_supported_languages_;
  smart_objects::SmartObject* tts_supported_languages_;
  smart_objects::SmartObject* vr_supported_languages_;
  /*
   * display_capabilities_ is deprecated and replaced by
   * system_display_capabilities_. For backward compatibility
   * display_capabilities_ is not removed.
   */
  smart_objects::SmartObject* display_capabilities_;
  smart_objects::SmartObjectSPtr system_display_capabilities_;
  smart_objects::SmartObject* hmi_zone_capabilities_;
  smart_objects::SmartObject* soft_buttons_capabilities_;
  smart_objects::SmartObject* button_capabilities_;
  smart_objects::SmartObject* preset_bank_capabilities_;
  smart_objects::SmartObject* vr_capabilities_;
  smart_objects::SmartObject* speech_capabilities_;
  smart_objects::SmartObject* audio_pass_thru_capabilities_;
  smart_objects::SmartObject* pcm_stream_capabilities_;
  smart_objects::SmartObject* prerecorded_speech_;
  bool is_navigation_supported_;
  bool is_phone_call_supported_;
  bool is_video_streaming_supported_;
  bool is_rc_supported_;
  std::string ccpu_version_;
  smart_objects::SmartObject* navigation_capability_;
  smart_objects::SmartObject* phone_capability_;
  smart_objects::SmartObject* video_streaming_capability_;
  smart_objects::SmartObject* rc_capability_;

  ApplicationManager& app_mngr_;
  HMILanguageHandler hmi_language_handler_;

  DISALLOW_COPY_AND_ASSIGN(HMICapabilitiesImpl);
};

}  //  namespace application_manager

#endif  //  SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_IMPL_H_

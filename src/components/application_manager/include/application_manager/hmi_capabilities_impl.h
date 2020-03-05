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
#include "utils/macro.h"

namespace application_manager {
class ApplicationManager;

class HMICapabilitiesImpl : public HMICapabilities {
 public:
  /**
   * @ Class constructor
   * @param app_mngr Application manager pointer
   */
  explicit HMICapabilitiesImpl(ApplicationManager& app_mngr);

  /**
   * @brief Class destructor
   */
  virtual ~HMICapabilitiesImpl();

  bool VerifyImageType(const int32_t image_type) const OVERRIDE;

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

  void set_ccpu_version(const std::string& ccpu_version) OVERRIDE;

  const std::string& ccpu_version() const OVERRIDE;

  bool attenuated_supported() const OVERRIDE;

  void set_attenuated_supported(const bool state) OVERRIDE;

  const hmi_apis::Common_Language::eType active_ui_language() const OVERRIDE;

  void set_active_ui_language(
      const hmi_apis::Common_Language::eType language) OVERRIDE;

  const smart_objects::SmartObjectSPtr ui_supported_languages() const OVERRIDE;

  void set_ui_supported_languages(
      const smart_objects::SmartObject& supported_languages) OVERRIDE;

  const hmi_apis::Common_Language::eType active_vr_language() const OVERRIDE;

  void set_active_vr_language(
      const hmi_apis::Common_Language::eType language) OVERRIDE;

  const smart_objects::SmartObjectSPtr vr_supported_languages() const OVERRIDE;

  void set_vr_supported_languages(
      const smart_objects::SmartObject& supported_languages) OVERRIDE;

  const hmi_apis::Common_Language::eType active_tts_language() const OVERRIDE;

  void set_active_tts_language(
      const hmi_apis::Common_Language::eType language) OVERRIDE;

  const smart_objects::SmartObjectSPtr tts_supported_languages() const OVERRIDE;

  void set_tts_supported_languages(
      const smart_objects::SmartObject& supported_languages) OVERRIDE;

  const smart_objects::SmartObjectSPtr display_capabilities() const OVERRIDE;

  void set_display_capabilities(
      const smart_objects::SmartObject& display_capabilities) OVERRIDE;

  const smart_objects::SmartObjectSPtr system_display_capabilities()
      const OVERRIDE;

  void set_system_display_capabilities(
      const smart_objects::SmartObject& display_capabilities);

  const smart_objects::SmartObjectSPtr hmi_zone_capabilities() const OVERRIDE;

  void set_hmi_zone_capabilities(
      const smart_objects::SmartObject& hmi_zone_capabilities) OVERRIDE;

  const smart_objects::SmartObjectSPtr soft_button_capabilities()
      const OVERRIDE;

  void set_soft_button_capabilities(
      const smart_objects::SmartObject& soft_button_capabilities) OVERRIDE;

  const smart_objects::SmartObjectSPtr button_capabilities() const OVERRIDE;

  void set_button_capabilities(
      const smart_objects::SmartObject& button_capabilities) OVERRIDE;

  void set_speech_capabilities(
      const smart_objects::SmartObject& speech_capabilities) OVERRIDE;

  const smart_objects::SmartObjectSPtr speech_capabilities() const OVERRIDE;

  void set_vr_capabilities(
      const smart_objects::SmartObject& vr_capabilities) OVERRIDE;

  const smart_objects::SmartObjectSPtr vr_capabilities() const OVERRIDE;

  void set_audio_pass_thru_capabilities(
      const smart_objects::SmartObject& audio_pass_thru_capabilities) OVERRIDE;

  const smart_objects::SmartObjectSPtr audio_pass_thru_capabilities()
      const OVERRIDE;

  void set_pcm_stream_capabilities(
      const smart_objects::SmartObject& pcm_stream_capabilities) OVERRIDE;

  const smart_objects::SmartObjectSPtr pcm_stream_capabilities() const OVERRIDE;

  const smart_objects::SmartObjectSPtr preset_bank_capabilities()
      const OVERRIDE;

  void set_preset_bank_capabilities(
      const smart_objects::SmartObject& preset_bank_capabilities) OVERRIDE;

  void set_vehicle_type(
      const smart_objects::SmartObject& vehicle_type) OVERRIDE;

  const smart_objects::SmartObjectSPtr vehicle_type() const OVERRIDE;

  const smart_objects::SmartObjectSPtr prerecorded_speech() const OVERRIDE;

  void set_prerecorded_speech(
      const smart_objects::SmartObject& prerecorded_speech) OVERRIDE;

  void set_navigation_supported(const bool supported) OVERRIDE;

  bool navigation_supported() const OVERRIDE;

  void set_phone_call_supported(const bool supported) OVERRIDE;

  bool phone_call_supported() const OVERRIDE;

  void set_video_streaming_supported(const bool supported) OVERRIDE;

  bool video_streaming_supported() const OVERRIDE;

  void set_rc_supported(const bool supported) OVERRIDE;

  bool rc_supported() const OVERRIDE;

  void set_navigation_capability(
      const smart_objects::SmartObject& navigation_capability) OVERRIDE;

  const smart_objects::SmartObjectSPtr navigation_capability() const OVERRIDE;

  void set_phone_capability(
      const smart_objects::SmartObject& phone_capability) OVERRIDE;

  const smart_objects::SmartObjectSPtr phone_capability() const OVERRIDE;

  void set_video_streaming_capability(
      const smart_objects::SmartObject& video_streaming_capability) OVERRIDE;

  const smart_objects::SmartObjectSPtr video_streaming_capability()
      const OVERRIDE;

  void set_rc_capability(
      const smart_objects::SmartObject& rc_capability) OVERRIDE;

  const smart_objects::SmartObjectSPtr rc_capability() const OVERRIDE;

  void set_seat_location_capability(
      const smart_objects::SmartObject& seat_location_capability) OVERRIDE;

  const smart_objects::SmartObjectSPtr seat_location_capability()
      const OVERRIDE;

  void Init(resumption::LastStateWrapperPtr last_state_wrapper) OVERRIDE;

  DEPRECATED
  void Init(resumption::LastState* last_state) OVERRIDE;

  HMILanguageHandler& get_hmi_language_handler() OVERRIDE;

  void set_handle_response_for(
      const smart_objects::SmartObject& request) OVERRIDE;

  bool SaveCachedCapabilitiesToFile(
      const std::string& interface_name,
      const std::vector<std::string>& sections_to_update,
      const smart_objects::CSmartSchema& schema) OVERRIDE;

  bool DeleteCachedCapabilitiesFile() const OVERRIDE;

  std::set<hmi_apis::FunctionID::eType> GetDefaultInitializedCapabilities()
      const OVERRIDE;

 protected:
  /**
   * @brief Loads capabilities from local file in case SDL was launched
   * without HMI
   *
   * @return TRUE if capabilities loaded successfully, otherwise FALSE.
   */
  bool LoadCapabilitiesFromFile();

  /**
   * @brief function converts json object "languages" to smart object
   * @param json_languages from file hmi_capabilities.json
   * @param languages - the converted object
   *
   */
  void convert_json_languages_to_obj(
      const Json::Value& json_languages,
      smart_objects::SmartObject& languages) const;

  /**
   * @brief function that converts a single entry of audio pass thru capability
   *        to smart object
   * @param capability json object that represents a single entry of audio pass
   *        thru capability
   * @param output_so the converted object
   */
  void convert_audio_capability_to_obj(
      const Json::Value& capability,
      smart_objects::SmartObject& output_so) const;

 private:
  /**
   * @brief Checks are all updating fields are currently saved in the JSON
   * structure
   * @param root_node reference to root node of JSON structure
   * @param interface_name name of interface to check
   * @param sections_to_check reference to list of fields to check
   * @return true if all fields from the list are saved in the JSON structure,
   * otherwise returns false
   */
  bool AllFieldsSaved(const Json::Value& root_node,
                      const std::string& interface_name,
                      const std::vector<std::string>& sections_to_check) const;

  /**
   * @brief Gets the currently active language depending on interface
   * @param interface_name name of interface of currently active language
   * @return active language for specified interface
   *
   */
  hmi_apis::Common_Language::eType GetActiveLanguageForInterface(
      const std::string& interface_name) const;

  /**
   * @brief Prepares specified JSON structure according to sections which
   * should be updated
   * @param interface_name name of interface to prepare
   * @param sections_to_update reference to list of fields to update
   * @param schema reference to schema which should be unapplied before field
   * updating
   * @param out_root_node reference to JSON structure to update
   */
  void PrepareJsonValueForSaving(
      const char* interface_name,
      const std::vector<std::string>& sections_to_update,
      const smart_objects::CSmartSchema& schema,
      Json::Value& out_root_node) const;

  /**
   * @brief Prepares specified JSON structure for UI interface according to
   * sections which should be updated
   * @param sections_to_update reference to list of fields to update
   * @param schema reference to schema which should be unapplied before field
   * updating
   * @param out_node reference to JSON structure to update
   */
  void PrepareUiJsonValueForSaving(
      const std::vector<std::string>& sections_to_update,
      const smart_objects::CSmartSchema& schema,
      Json::Value& out_node) const;

  /**
   * @brief Prepares specified JSON structure for VR interface according to
   * sections which should be updated
   * @param sections_to_update reference to list of fields to update
   * @param schema reference to schema which should be unapplied before field
   * updating
   * @param out_node reference to JSON structure to update
   */
  void PrepareVrJsonValueForSaving(
      const std::vector<std::string>& sections_to_update,
      const smart_objects::CSmartSchema& schema,
      Json::Value& out_node) const;

  /**
   * @brief Prepares specified JSON structure for TTS interface according to
   * sections which should be updated
   * @param sections_to_update reference to list of fields to update
   * @param schema reference to schema which should be unapplied before field
   * updating
   * @param out_node reference to JSON structure to update
   */
  void PrepareTtsJsonValueForSaving(
      const std::vector<std::string>& sections_to_update,
      const smart_objects::CSmartSchema& schema,
      Json::Value& out_node) const;

  /**
   * @brief Prepares specified JSON structure for Buttons interface according to
   * sections which should be updated
   * @param sections_to_update reference to list of fields to update
   * @param schema reference to schema which should be unapplied before field
   * updating
   * @param out_node reference to JSON structure to update
   */
  void PrepareButtonsJsonValueForSaving(
      const std::vector<std::string>& sections_to_update,
      const smart_objects::CSmartSchema& schema,
      Json::Value& out_node) const;

  /**
   * @brief Prepares specified JSON structure for VehicleInfo interface
   * according to sections which should be updated
   * @param sections_to_update reference to list of fields to update
   * @param schema reference to schema which should be unapplied before field
   * updating
   * @param out_node reference to JSON structure to update
   */
  void PrepareVehicleInfoJsonValueForSaving(
      const std::vector<std::string>& sections_to_update,
      const smart_objects::CSmartSchema& schema,
      Json::Value& out_node) const;

  /**
   * @brief Prepares specified JSON structure for RC interface according to
   * sections which should be updated
   * @param sections_to_update reference to list of fields to update
   * @param schema reference to schema which should be unapplied before field
   * updating
   * @param out_node reference to JSON structure to update
   */
  void PrepareRCJsonValueForSaving(
      const std::vector<std::string>& sections_to_update,
      const smart_objects::CSmartSchema& schema,
      Json::Value& out_node) const;

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
  smart_objects::SmartObjectSPtr vehicle_type_;
  smart_objects::SmartObjectSPtr ui_supported_languages_;
  smart_objects::SmartObjectSPtr tts_supported_languages_;
  smart_objects::SmartObjectSPtr vr_supported_languages_;
  /*
   * display_capabilities_ is deprecated and replaced by
   * system_display_capabilities_. For backward compatibility
   * display_capabilities_ is not removed.
   */
  smart_objects::SmartObjectSPtr display_capabilities_;
  smart_objects::SmartObjectSPtr system_display_capabilities_;
  smart_objects::SmartObjectSPtr hmi_zone_capabilities_;
  smart_objects::SmartObjectSPtr soft_buttons_capabilities_;
  smart_objects::SmartObjectSPtr button_capabilities_;
  smart_objects::SmartObjectSPtr preset_bank_capabilities_;
  smart_objects::SmartObjectSPtr vr_capabilities_;
  smart_objects::SmartObjectSPtr speech_capabilities_;
  smart_objects::SmartObjectSPtr audio_pass_thru_capabilities_;
  smart_objects::SmartObjectSPtr pcm_stream_capabilities_;
  smart_objects::SmartObjectSPtr prerecorded_speech_;
  bool is_navigation_supported_;
  bool is_phone_call_supported_;
  bool is_video_streaming_supported_;
  bool is_rc_supported_;
  std::string ccpu_version_;
  smart_objects::SmartObjectSPtr navigation_capability_;
  smart_objects::SmartObjectSPtr phone_capability_;
  smart_objects::SmartObjectSPtr video_streaming_capability_;
  smart_objects::SmartObjectSPtr rc_capability_;
  smart_objects::SmartObjectSPtr seat_location_capability_;

  ApplicationManager& app_mngr_;
  HMILanguageHandler hmi_language_handler_;

  std::set<hmi_apis::FunctionID::eType> default_initialized_capabilities_;

  DISALLOW_COPY_AND_ASSIGN(HMICapabilitiesImpl);
};

}  //  namespace application_manager

#endif  //  SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_IMPL_H_

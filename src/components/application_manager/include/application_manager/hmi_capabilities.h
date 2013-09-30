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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_H_

#include "interfaces/MOBILE_API.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;
}
}

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

namespace application_manager {

struct HMICapabilities {
  HMICapabilities();
  virtual ~HMICapabilities();

  virtual bool is_vr_cooperating() const;
  virtual void set_is_vr_cooperating(bool value);

  virtual bool is_tts_cooperating() const;
  virtual void set_is_tts_cooperating(bool value);

  virtual bool is_ui_cooperating() const;
  virtual void set_is_ui_cooperating(bool value);

  virtual bool is_navi_cooperating() const;
  virtual void set_is_navi_cooperating(bool value);

  virtual bool is_ivi_cooperating() const;
  virtual void set_is_ivi_cooperating(bool value);

  /*
   * @brief Retrieves if mixing audio is supported by HMI
   * (ie recording TTS command and playing audio)
   *
   * @return Current state of the mixing audio flag
   */
  bool attenuated_supported() const;

  /*
   * @brief Sets state for mixing audio
   *
   * @param state New state to be set
   */
  void set_attenuated_supported(bool state);

  /*
   * @brief Retrieves UI supported languages
   *
   * @return Currently supported UI languages
   */
  inline const smart_objects::SmartObject*
  ui_supported_languages() const {
    return ui_supported_languages_;
  }

  /*
   * @brief Sets supported UI languages
   *
   * @param supported_languages Supported UI languages
   */
  void set_ui_supported_languages(
      const smart_objects::SmartObject& supported_languages);

  /*
   * @brief Retrieves TTS  supported languages
   *
   * @return Currently supported TTS languages
   */
  inline const smart_objects::SmartObject*
  tts_supported_languages() const {
    return tts_supported_languages_;
  }

  /*
   * @brief Sets supported TTS languages
   *
   * @param supported_languages Supported TTS languages
   */
  void set_tts_supported_languages(
      const smart_objects::SmartObject& supported_languages);

  /*
   * @brief Retrieves VR supported languages
   *
   * @return Currently supported VR languages
   */
  inline const smart_objects::SmartObject*
  vr_supported_languages() const {
    return vr_supported_languages_;
  }

  /*
   * @brief Sets supported VR languages
   *
   * @param supported_languages Supported VR languages
   */
  void set_vr_supported_languages(
      const smart_objects::SmartObject& supported_languages);

  /*
   * @brief Retrieves information about the display capabilities
   *
   * @return Currently supported display capabilities
   */
  inline const smart_objects::SmartObject*
  display_capabilities() const {
    return display_capabilities_;
  }

  /*
   * @brief Checks is image type(Static/Dynamic) requested by
   * Mobile Device is supported on current HMI.
   * @param image_type recieved type of image from Enum.
   * @return Bool true if supported
   */
  bool VerifyImageType(int image_type);

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
  hmi_zone_capabilities() const {
    return hmi_zone_capabilities_;
  }

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
  soft_button_capabilities() const {
    return soft_buttons_capabilities_;
  }

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
  inline const smart_objects::SmartObject* button_capabilities() const {
    return button_capabilities_;
  }

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
  inline const smart_objects::SmartObject* speech_capabilities() const {
    return speech_capabilities_;
  }

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
  inline const smart_objects::SmartObject* vr_capabilities() const {
    return vr_capabilities_;
  }

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
  audio_pass_thru_capabilities() const {
    return audio_pass_thru_capabilities_;
  }

  /*
   * @brief Retrieves information about the preset bank capabilities
   *
   * @return Currently supported preset bank capabilities
   */
  inline const smart_objects::SmartObject* preset_bank_capabilities() const {
    return preset_bank_capabilities_;
  }

  /*
   * @brief Sets supported preset bank capabilities
   *
   * @param soft_button_capabilities supported preset bank capabilities
   */
  void set_preset_bank_capabilities(
      const smart_objects::SmartObject& preset_bank_capabilities);

 protected:
  bool is_vr_cooperating_;
  bool is_tts_cooperating_;
  bool is_ui_cooperating_;
  bool is_navi_cooperating_;
  bool is_ivi_cooperating_;

  // to check if IsReady response for corresponding interface received
  bool is_vr_ready_response_recieved_;
  bool is_tts_ready_response_recieved_;
  bool is_ui_ready_response_recieved_;
  bool is_navi_ready_response_recieved_;
  bool is_ivi_ready_response_recieved_;

  bool attenuated_supported_;
  smart_objects::SmartObject* ui_supported_languages_;
  smart_objects::SmartObject* tts_supported_languages_;
  smart_objects::SmartObject* vr_supported_languages_;
  smart_objects::SmartObject* display_capabilities_;
  smart_objects::SmartObject* hmi_zone_capabilities_;
  smart_objects::SmartObject* soft_buttons_capabilities_;
  smart_objects::SmartObject* button_capabilities_;
  smart_objects::SmartObject* preset_bank_capabilities_;
  smart_objects::SmartObject* vr_capabilities_;
  smart_objects::SmartObject* speech_capabilities_;
  smart_objects::SmartObject* audio_pass_thru_capabilities_;
};
}  //  namespace application_manager

#endif  //  SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_CAPABILITIES_H_

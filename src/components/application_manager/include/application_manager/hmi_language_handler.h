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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_LANGUAGE_HANDLER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_LANGUAGE_HANDLER_H_

#include "application_manager/event_engine/event_observer.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

/**
 * Class is used to handle edge case with slow HMI responses for current
 * languages. Main idea is to cache values within some persistent storage and
 * respond to applications with these values. In case app registered before
 * responses are gotten and after responses were received their language(s)
 * mismatch to current on HMI - apps have to be unregistered.
 **/
class HMILanguageHandler: public event_engine::EventObserver {
public:
  /**
   * @brief Class constructor
   */
  HMILanguageHandler();

  /**
   * @brief Sets UI language value in persistent storage
   * @param language UI language
   */
  void set_ui_language(hmi_apis::Common_Language::eType language);

  /**
   * @brief Sets VR language value in persistent storage
   * @param language VR language
   */
  void set_vr_language(hmi_apis::Common_Language::eType language);

  /**
   * @brief Sets TTS language value in persistent storage
   * @param language TTS language
   */
  void set_tts_language(hmi_apis::Common_Language::eType language);

  /**
   * @brief Gets UI language value from persistent storage
   * @return UI language
   */
  hmi_apis::Common_Language::eType get_ui_language() const;

  /**
   * @brief Gets VR language from persistent storage
   * @return VR language
   */
  hmi_apis::Common_Language::eType get_vr_language() const;

  /**
   * @brief Gets TTS language value from persistent storage
   * @return TTS language
   */
  hmi_apis::Common_Language::eType get_tts_language() const;

  void on_event(const event_engine::Event& event) OVERRIDE;

  /**
   * @brief Trigger waiting for response
   * @param request Request object
   */
  void set_handle_response_for(
          const event_engine::smart_objects::SmartObject& request);

private:

  /**
   * @brief Verifies already registered apps for language mismatch with
   * current HMI language(s).
   */
  void VerifyRegisteredApps() const;

  bool is_ui_language_received_;
  bool is_vr_language_received_;
  bool is_tts_language_received_;
};

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_LANGUAGE_HANDLER_H_


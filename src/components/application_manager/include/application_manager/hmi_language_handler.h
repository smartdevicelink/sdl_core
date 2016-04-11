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
namespace resumption {
class LastState;
}
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
  typedef std::map<uint32_t, bool> Apps;

  /**
  * @brief System interfaces
  */
  enum Interface {
    INTERFACE_UI,
    INTERFACE_VR,
    INTERFACE_TTS
  };

  /**
   * @brief Class constructor
   */
  HMILanguageHandler();

  /**
   * @brief Sets language for interface
   * @param interface Interface
   * @param language Language
   */
  void set_language_for(Interface interface,
                        hmi_apis::Common_Language::eType language);

  /**
   * @brief Gets language for interface
   * @param interface Interface
   * @return Language
   */
  hmi_apis::Common_Language::eType get_language_for(Interface interface) const;

  void on_event(const event_engine::Event& event) OVERRIDE;

  /**
   * @brief Trigger waiting for response
   * @param request Request object
   */
  void set_handle_response_for(
          const event_engine::smart_objects::SmartObject& request);

  /**
   * @brief Sets default languages from HMI capabilities
   * @param ui UI language
   * @param vr VR language
   * @param tts TTS language
   */
  void set_default_capabilities_languages(hmi_apis::Common_Language::eType ui,
                                          hmi_apis::Common_Language::eType vr,
                                          hmi_apis::Common_Language::eType tts);
  void Init(resumption::LastState* value);

private:
  void SendOnLanguageChangeToMobile(uint32_t connection_key);

  /**
   * @brief Verifies languages gotten from HMI with persisted languages
   */
  void VerifyWithPersistedLanguages();

  /**
   * @brief Handles applications registered before actual HMI languages
   * have been received
   * @param app_id Application id
   */
  void HandleWrongLanguageApp(const Apps::value_type& app_id);

  /**
   * @brief Checks if application needs to be handled because of language(s)
   * mismatch
   * @param app Application
   */
  void CheckApplication(const Apps::value_type app);

  sync_primitives::Lock apps_lock_;

  /**
   * @brief Applications, which needs to be handled
   */
  Apps apps_;

  /**
   * @brief UI language persisted from previous ignition cycle
   */
  hmi_apis::Common_Language::eType persisted_ui_language_;

  /**
   * @brief VR language persisted from previous ignition cycle
   */
  hmi_apis::Common_Language::eType persisted_vr_language_;

  /**
   * @brief TTS language persisted from previous ignition cycle
   */
  hmi_apis::Common_Language::eType persisted_tts_language_;

  /**
   * @brief Default UI language from HMI capabilitites
   */
  hmi_apis::Common_Language::eType capabilities_ui_language_;

  /**
   * @brief Default VR language from HMI capabilitites
   */
  hmi_apis::Common_Language::eType capabilities_vr_language_;

  /**
   * @brief Default TTS language from HMI capabilitites
   */
  hmi_apis::Common_Language::eType capabilities_tts_language_;

  /**
   * @brief Indicates if current UI language has been received from HMI
   */
  bool is_ui_language_received_;

  /**
   * @brief Indicates if current VR language has been received from HMI
   */
  bool is_vr_language_received_;

  /**
   * @brief Indicates if current TTS language has been received from HMI
   */
  bool is_tts_language_received_;
  resumption::LastState* last_state_;
};

} // namespace application_manager

#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_HMI_LANGUAGE_HANDLER_H_


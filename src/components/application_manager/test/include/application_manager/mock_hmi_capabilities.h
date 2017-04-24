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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_HMI_CAPABILITIES_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_HMI_CAPABILITIES_H_

#include <stdint.h>

#include "application_manager/hmi_capabilities.h"
#include "gmock/gmock.h"

namespace test {
namespace components {
namespace application_manager_test {

class MockHMICapabilities : public ::application_manager::HMICapabilities {
 public:
  MOCK_METHOD2(convert_json_speech_capabilities_to_obj,
               void(Json::Value& json_speech,
                    smart_objects::SmartObject& speech_capabilities));

  MOCK_CONST_METHOD1(VerifyImageType, bool(const int32_t image_type));

  MOCK_CONST_METHOD0(is_vr_cooperating, bool());
  MOCK_METHOD1(set_is_vr_cooperating, void(const bool value));

  MOCK_CONST_METHOD0(is_tts_cooperating, bool());
  MOCK_METHOD1(set_is_tts_cooperating, void(const bool value));

  MOCK_CONST_METHOD0(is_ui_cooperating, bool());
  MOCK_METHOD1(set_is_ui_cooperating, void(const bool value));

  MOCK_CONST_METHOD0(is_navi_cooperating, bool());
  MOCK_METHOD1(set_is_navi_cooperating, void(const bool value));

  MOCK_CONST_METHOD0(is_ivi_cooperating, bool());
  MOCK_METHOD1(set_is_ivi_cooperating, void(const bool value));

  MOCK_CONST_METHOD0(attenuated_supported, bool());

  MOCK_METHOD1(set_attenuated_supported, void(const bool state));

  MOCK_CONST_METHOD0(active_ui_language,
                     const hmi_apis::Common_Language::eType());

  MOCK_METHOD1(set_active_ui_language,
               void(const hmi_apis::Common_Language::eType language));

  MOCK_CONST_METHOD0(ui_supported_languages,
                     const smart_objects::SmartObject*());
  MOCK_METHOD1(set_ui_supported_languages,
               void(const smart_objects::SmartObject& supported_languages));

  MOCK_CONST_METHOD0(active_vr_language,
                     const hmi_apis::Common_Language::eType());
  MOCK_METHOD1(set_active_vr_language,
               void(const hmi_apis::Common_Language::eType language));

  MOCK_CONST_METHOD0(vr_supported_languages,
                     const smart_objects::SmartObject*());
  MOCK_METHOD1(set_vr_supported_languages,
               void(const smart_objects::SmartObject& supported_languages));

  MOCK_CONST_METHOD0(active_tts_language,
                     const hmi_apis::Common_Language::eType());
  MOCK_METHOD1(set_active_tts_language,
               void(const hmi_apis::Common_Language::eType language));

  MOCK_CONST_METHOD0(tts_supported_languages,
                     const smart_objects::SmartObject*());
  MOCK_METHOD1(set_tts_supported_languages,
               void(const smart_objects::SmartObject& supported_languages));

  MOCK_CONST_METHOD0(display_capabilities, const smart_objects::SmartObject*());
  MOCK_METHOD1(set_display_capabilities,
               void(const smart_objects::SmartObject& display_capabilities));

  MOCK_CONST_METHOD0(hmi_zone_capabilities,
                     const smart_objects::SmartObject*());
  MOCK_METHOD1(set_hmi_zone_capabilities,
               void(const smart_objects::SmartObject& hmi_zone_capabilities));

  MOCK_CONST_METHOD0(soft_button_capabilities,
                     const smart_objects::SmartObject*());
  MOCK_METHOD1(
      set_soft_button_capabilities,
      void(const smart_objects::SmartObject& soft_button_capabilities));

  MOCK_CONST_METHOD0(button_capabilities, const smart_objects::SmartObject*());
  MOCK_METHOD1(set_button_capabilities,
               void(const smart_objects::SmartObject& button_capabilities));

  MOCK_CONST_METHOD0(speech_capabilities, const smart_objects::SmartObject*());
  MOCK_METHOD1(set_speech_capabilities,
               void(const smart_objects::SmartObject& speech_capabilities));

  MOCK_CONST_METHOD0(vr_capabilities, const smart_objects::SmartObject*());
  MOCK_METHOD1(set_vr_capabilities,
               void(const smart_objects::SmartObject& vr_capabilities));

  MOCK_CONST_METHOD0(audio_pass_thru_capabilities,
                     const smart_objects::SmartObject*());
  MOCK_METHOD1(
      set_audio_pass_thru_capabilities,
      void(const smart_objects::SmartObject& audio_pass_thru_capabilities));

  MOCK_CONST_METHOD0(pcm_stream_capabilities,
                     const smart_objects::SmartObject*());
  MOCK_METHOD1(set_pcm_stream_capabilities,
               void(const smart_objects::SmartObject& pcm_stream_capabilities));

  MOCK_CONST_METHOD0(preset_bank_capabilities,
                     const smart_objects::SmartObject*());
  MOCK_METHOD1(
      set_preset_bank_capabilities,
      void(const smart_objects::SmartObject& preset_bank_capabilities));

  MOCK_CONST_METHOD0(vehicle_type, const smart_objects::SmartObject*());
  MOCK_METHOD1(set_vehicle_type,
               void(const smart_objects::SmartObject& vehicle_type));

  MOCK_CONST_METHOD0(prerecorded_speech, const smart_objects::SmartObject*());
  MOCK_METHOD1(set_prerecorded_speech,
               void(const smart_objects::SmartObject& prerecorded_speech));

  MOCK_CONST_METHOD0(ui_hmi_capabilities, const smart_objects::SmartObject&());

  MOCK_METHOD1(set_ui_hmi_capabilities,
               void(const smart_objects::SmartObject& ui_hmi_capabilities));

  MOCK_CONST_METHOD0(navigation_supported, bool());
  MOCK_METHOD1(set_navigation_supported, void(const bool supported));

  MOCK_CONST_METHOD0(phone_call_supported, bool());
  MOCK_METHOD1(set_phone_call_supported, void(const bool supported));

  MOCK_METHOD1(Init, void(resumption::LastState* last_state));

  MOCK_CONST_METHOD0(ccpu_version, const std::string&());
  MOCK_METHOD1(set_ccpu_version, void(const std::string& ccpu_version));
  MOCK_METHOD0(get_hmi_language_handler,
               application_manager::HMILanguageHandler&());
  MOCK_METHOD1(set_handle_response_for,
               void(const smart_objects::SmartObject& request));

 protected:
  MOCK_CONST_METHOD2(check_existing_json_member,
                     bool(const Json::Value& json_member,
                          const char* name_of_member));

  MOCK_CONST_METHOD2(convert_json_languages_to_obj,
                     void(const Json::Value& json_languages,
                          smart_objects::SmartObject& languages));
};

}  // namespace application_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_TEST_INCLUDE_APPLICATION_MANAGER_MOCK_HMI_CAPABILITIES_H_

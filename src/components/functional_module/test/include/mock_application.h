/*
 * Copyright (c) 2015, Ford Motor Company
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
#ifndef SRC_COMPONENTS_FUNCTIONAL_MODULE_TEST_INCLUDE_MOCK_APPLICATION_H_
#define SRC_COMPONENTS_FUNCTIONAL_MODULE_TEST_INCLUDE_MOCK_APPLICATION_H_

#include "gmock/gmock.h"
#include "application_manager/application.h"
#include "application_manager/usage_statistics.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

class MockApplication : public Application {
 public:
  MOCK_CONST_METHOD0(active_message, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(curHash, const std::string&());
  MOCK_METHOD0(UpdateHash, void());
  MOCK_METHOD0(CloseActiveMessage, void());
  MOCK_CONST_METHOD0(IsFullscreen, bool());
  MOCK_METHOD0(MakeFullscreen, bool());
  MOCK_CONST_METHOD0(IsAudible, bool());
  MOCK_METHOD0(MakeNotAudible, void());
  MOCK_CONST_METHOD0(allowed_support_navigation, bool());
  MOCK_METHOD1(set_allowed_support_navigation, void(bool allow));
  MOCK_CONST_METHOD0(hmi_supports_navi_streaming, bool());
  MOCK_METHOD1(set_hmi_supports_navi_streaming, void(const bool& supports));
  MOCK_CONST_METHOD0(app_allowed, bool());
  MOCK_CONST_METHOD0(has_been_activated, bool());
  MOCK_CONST_METHOD0(version, const Version&());
  MOCK_METHOD1(set_hmi_application_id, void(uint32_t hmi_app_id));
  MOCK_CONST_METHOD0(hmi_app_id, uint32_t());
  MOCK_CONST_METHOD0(app_id, uint32_t());
  MOCK_CONST_METHOD0(name, const std::string&());
  MOCK_CONST_METHOD0(folder_name, const std::string());
  MOCK_CONST_METHOD0(is_media_application, bool());
  MOCK_CONST_METHOD0(hmi_level, const mobile_api::HMILevel::eType&());
  MOCK_CONST_METHOD0(put_file_in_none_count, const uint32_t());
  MOCK_CONST_METHOD0(delete_file_in_none_count, const uint32_t());
  MOCK_CONST_METHOD0(list_files_in_none_count, const uint32_t());
  MOCK_CONST_METHOD0(system_context, const mobile_api::SystemContext::eType&());
  MOCK_CONST_METHOD0(audio_streaming_state,
                     const mobile_api::AudioStreamingState::eType&());
  MOCK_CONST_METHOD0(app_icon_path, const std::string&());
  MOCK_CONST_METHOD0(device, connection_handler::DeviceHandle());
  MOCK_METHOD1(set_tts_speak_state, void(bool state_tts_speak));
  MOCK_METHOD0(tts_speak_state, bool());
  MOCK_METHOD1(set_tts_properties_in_none, void(bool active));
  MOCK_METHOD0(tts_properties_in_none, bool());
  MOCK_METHOD1(set_tts_properties_in_full, void(bool active));
  MOCK_METHOD0(tts_properties_in_full, bool());
  MOCK_METHOD1(set_version, void(const Version& version));
  MOCK_METHOD1(set_name, void(const std::string& name));
  MOCK_METHOD1(set_is_media_application, void(bool is_media));
  MOCK_METHOD1(set_hmi_level,
               void(const mobile_api::HMILevel::eType& hmi_level));
  MOCK_METHOD0(increment_put_file_in_none_count, void());
  MOCK_METHOD0(increment_delete_file_in_none_count, void());
  MOCK_METHOD0(increment_list_files_in_none_count, void());
  MOCK_METHOD1(set_system_context,
               void(const mobile_api::SystemContext::eType& system_context));
  MOCK_METHOD1(set_audio_streaming_state,
               void(const mobile_api::AudioStreamingState::eType& state));
  MOCK_METHOD1(set_app_icon_path, bool(const std::string& file_name));
  MOCK_METHOD1(set_app_allowed, void(const bool& allowed));
  MOCK_METHOD1(set_device, void(connection_handler::DeviceHandle device));
  MOCK_CONST_METHOD0(get_grammar_id, uint32_t());
  MOCK_METHOD1(set_grammar_id, void(uint32_t value));
  MOCK_METHOD1(set_protocol_version,
               void(const ProtocolVersion& protocol_version));
  MOCK_CONST_METHOD0(protocol_version, ProtocolVersion());
  MOCK_METHOD1(AddFile, bool(AppFile& file));
  MOCK_CONST_METHOD0(getAppFiles, const AppFilesMap&());
  MOCK_METHOD1(UpdateFile, bool(AppFile& file));
  MOCK_METHOD1(DeleteFile, bool(const std::string& file_name));
  MOCK_METHOD1(GetFile, const AppFile*(const std::string& file_name));
  MOCK_METHOD1(SubscribeToButton,
               bool(mobile_apis::ButtonName::eType btn_name));
  MOCK_METHOD1(IsSubscribedToButton,
               bool(mobile_apis::ButtonName::eType btn_name));
  MOCK_METHOD1(UnsubscribeFromButton,
               bool(mobile_apis::ButtonName::eType btn_name));
  MOCK_METHOD1(SubscribeToIVI, bool(uint32_t vehicle_info_type_));
  MOCK_METHOD1(IsSubscribedToIVI, bool(uint32_t vehicle_info_type_));
  MOCK_METHOD1(UnsubscribeFromIVI, bool(uint32_t vehicle_info_type_));
  MOCK_METHOD2(IsCommandLimitsExceeded,
               bool(mobile_apis::FunctionID::eType cmd_id,
                    TLimitSource source));
  MOCK_METHOD0(usage_report, UsageStatistics&());
  MOCK_METHOD2(SubscribeToSoftButtons,
               void(int32_t cmd_id, const SoftButtonID& softbuttons_id));
  MOCK_METHOD1(IsSubscribedToSoftButton, bool(const uint32_t softbutton_id));
  MOCK_METHOD1(UnsubscribeFromSoftButtons, void(int32_t cmd_id));
  MOCK_METHOD1(QueryInterface, AppExtensionPtr(AppExtensionUID uid));
  MOCK_METHOD1(AddExtension, bool(AppExtensionPtr extention));
  MOCK_METHOD1(RemoveExtension, bool(AppExtensionUID uid));
  MOCK_METHOD0(RemoveExtensions, void());
  MOCK_CONST_METHOD0(help_prompt, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(timeout_prompt, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(vr_help_title, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(vr_help, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(tbt_state, const mobile_api::TBTState::eType&());
  MOCK_CONST_METHOD0(show_command, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(tbt_show_command, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(SubscribedButtons,
                     const std::set<mobile_apis::ButtonName::eType>&());
  MOCK_CONST_METHOD0(SubscribesIVI,
                     const application_manager::VehicleInfoSubscriptions&());
  MOCK_CONST_METHOD0(keyboard_props, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(menu_title, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(menu_icon, const smart_objects::SmartObject*());
  MOCK_METHOD1(set_help_prompt,
               void(const smart_objects::SmartObject& help_prompt));
  MOCK_METHOD1(set_timeout_prompt,
               void(const smart_objects::SmartObject& timeout_prompt));
  MOCK_METHOD1(set_vr_help_title,
               void(const smart_objects::SmartObject& vr_help_title));
  MOCK_METHOD0(reset_vr_help_title, void());
  MOCK_METHOD1(set_vr_help, void(const smart_objects::SmartObject& vr_help));
  MOCK_METHOD0(reset_vr_help, void());
  MOCK_METHOD1(set_tbt_state,
               void(const mobile_api::TBTState::eType& tbt_state));
  MOCK_METHOD1(set_show_command,
               void(const smart_objects::SmartObject& show_command));
  MOCK_METHOD1(set_tbt_show_command,
               void(const smart_objects::SmartObject& tbt_show));
  MOCK_METHOD1(set_keyboard_props,
               void(const smart_objects::SmartObject& keyboard_props));
  MOCK_METHOD1(set_menu_title,
               void(const smart_objects::SmartObject& menu_title));
  MOCK_METHOD1(set_menu_icon,
               void(const smart_objects::SmartObject& menu_icon));
  MOCK_METHOD2(AddCommand,
               void(uint32_t cmd_id,
                    const smart_objects::SmartObject& command));
  MOCK_METHOD1(RemoveCommand, void(uint32_t cmd_id));
  MOCK_METHOD1(FindCommand, smart_objects::SmartObject*(uint32_t cmd_id));
  MOCK_METHOD2(AddSubMenu,
               void(uint32_t menu_id, const smart_objects::SmartObject& menu));
  MOCK_METHOD1(RemoveSubMenu, void(uint32_t menu_id));
  MOCK_CONST_METHOD1(FindSubMenu,
                     smart_objects::SmartObject*(uint32_t menu_id));
  MOCK_METHOD1(IsSubMenuNameAlreadyExist, bool(const std::string& name));
  MOCK_METHOD2(AddChoiceSet,
               void(uint32_t choice_set_id,
                    const smart_objects::SmartObject& choice_set));
  MOCK_METHOD1(RemoveChoiceSet, void(uint32_t choice_set_id));
  MOCK_METHOD1(FindChoiceSet,
               smart_objects::SmartObject*(uint32_t choice_set_id));
  MOCK_METHOD2(AddPerformInteractionChoiceSet,
               void(uint32_t choice_set_id,
                    const smart_objects::SmartObject& choice_set));
  MOCK_METHOD0(DeletePerformInteractionChoiceSetMap, void());
  MOCK_CONST_METHOD0(performinteraction_choice_set_map,
                     DataAccessor<PerformChoiceSetMap>());
  MOCK_CONST_METHOD1(FindPerformInteractionChoiceSet,
                     smart_objects::SmartObject*(uint32_t choice_set_id));
  MOCK_CONST_METHOD0(commands_map, DataAccessor<CommandsMap>());
  MOCK_CONST_METHOD0(sub_menu_map, DataAccessor<SubMenuMap>());
  MOCK_CONST_METHOD0(choice_set_map, DataAccessor<ChoiceSetMap>());
  MOCK_METHOD1(set_perform_interaction_active, void(uint32_t active));
  MOCK_CONST_METHOD0(is_perform_interaction_active, uint32_t());
  MOCK_METHOD1(set_perform_interaction_ui_corrid, void(uint32_t choice));
  MOCK_CONST_METHOD0(perform_interaction_ui_corrid, uint32_t());
  MOCK_METHOD1(set_perform_interaction_mode, void(int32_t mode));
  MOCK_CONST_METHOD0(perform_interaction_mode, int32_t());
  MOCK_METHOD1(set_reset_global_properties_active, void(bool active));
  MOCK_CONST_METHOD0(is_reset_global_properties_active, bool());
  MOCK_CONST_METHOD0(app_types, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(vr_synonyms, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(mobile_app_id, std::string());
  MOCK_CONST_METHOD0(tts_name, const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(ngn_media_screen_name,
                     const smart_objects::SmartObject*());
  MOCK_CONST_METHOD0(language, const mobile_api::Language::eType&());
  MOCK_CONST_METHOD0(ui_language, const mobile_api::Language::eType&());
  MOCK_METHOD1(set_app_types,
               void(const smart_objects::SmartObject& app_types));
  MOCK_METHOD1(set_vr_synonyms,
               void(const smart_objects::SmartObject& vr_synonyms));
  MOCK_METHOD1(set_mobile_app_id,
               void(const smart_objects::SmartObject& mobile_app_id));
  MOCK_METHOD1(set_tts_name, void(const smart_objects::SmartObject& tts_name));
  MOCK_METHOD1(set_ngn_media_screen_name,
               void(const smart_objects::SmartObject& ngn_name));
  MOCK_METHOD1(set_language, void(const mobile_api::Language::eType& language));
  MOCK_METHOD1(set_ui_language,
               void(const mobile_api::Language::eType& ui_language));
  MOCK_METHOD1(load_global_properties,
               void(const smart_objects::SmartObject& so));
  MOCK_METHOD1(set_mobile_app_id, void(const std::string& mobile_app_id));
  MOCK_METHOD0(ChangeSupportingAppHMIType, void());
  MOCK_CONST_METHOD0(is_navi, bool());
  MOCK_METHOD1(set_is_navi, void(bool allow));
  MOCK_CONST_METHOD0(hmi_supports_navi_video_streaming, bool());
  MOCK_METHOD1(set_hmi_supports_navi_video_streaming, void(bool supports));
  MOCK_CONST_METHOD0(hmi_supports_navi_audio_streaming, bool());
  MOCK_METHOD1(set_hmi_supports_navi_audio_streaming, void(bool supports));
  MOCK_CONST_METHOD0(is_voice_communication_supported, bool());
  MOCK_METHOD1(set_voice_communication_supported, void(bool));
  MOCK_METHOD1(set_activated, bool(bool is_active));
  MOCK_CONST_METHOD0(is_foreground, bool());
  MOCK_METHOD1(set_foreground, void(bool is_foreground));
  MOCK_CONST_METHOD0(IsAudioApplication, bool());
  MOCK_CONST_METHOD0(video_stream_retry_active, bool());
  MOCK_METHOD1(set_video_stream_retry_active, void(bool active));
  MOCK_CONST_METHOD0(audio_stream_retry_active, bool());
  MOCK_METHOD1(set_audio_stream_retry_active, void(bool active));
  MOCK_METHOD0(OnVideoStreamRetry, void());
  MOCK_METHOD0(OnAudioStreamRetry, void());
  MOCK_METHOD1(SubscribeToInteriorVehicleData,
               bool(smart_objects::SmartObject module));
  MOCK_METHOD1(IsSubscribedToInteriorVehicleData,
               bool(smart_objects::SmartObject module));
  MOCK_METHOD1(UnsubscribeFromInteriorVehicleData,
               bool(smart_objects::SmartObject module));
};

}  // namespace application_manager

#endif  // SRC_COMPONENTS_FUNCTIONAL_MODULE_TEST_INCLUDE_MOCK_APPLICATION_H_

/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SMART_OBJECT_KEYS_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SMART_OBJECT_KEYS_H_

namespace application_manager {

namespace strings {

const char params[] = "params";
const char message_type[] = "message_type";
const char correlation_id[] = "correlation_id";
const char function_id[] = "function_id";
const char protocol_version[] = "protocol_version";
const char protocol_type[] = "protocol_type";
const char connection_key[] = "connection_key";
const char error[] = "error";
const char error_msg[] = "message";
const char default_app_id[] = "default";


const char msg_params[] = "msg_params";
const char info[] = "info";
const char app_id[] = "appID";
const char hmi_app_id[] = "hmiAppID";
const char device_mac[] = "deviceMAC";
const char url[] = "url";
const char cmd_icon[] = "cmdIcon";
const char result_code[] = "resultCode";
const char success[] = "success";
const char sync_msg_version[] = "syncMsgVersion";
const char major_version[] = "majorVersion";
const char minor_version[] = "minorVersion";
const char app_name[] = "appName";
const char ngn_media_screen_app_name[] = "ngnMediaScreenAppName";
const char vr_synonyms[] = "vrSynonyms";
const char uses_vehicle_data[] = "usesVehicleData";
const char is_media_application[] = "isMediaApplication";
const char language_desired[] = "languageDesired";
const char auto_activated_id[] = "autoActivateID";
const char app_type[] = "appType";
const char app_hmi_type[] = "appHMIType";
const char tts_name[] = "ttsName";
const char binary_data[] = "binary_data";
const char timeout_prompt[] = "timeoutPrompt";
const char timeout[] = "timeout";
const char vr_help_title[] = "vrHelpTitle";
const char vr_help[] = "vrHelp";
const char main_field_1[] = "mainField1";
const char main_field_2[] = "mainField2";
const char main_field_3[] = "mainField3";
const char main_field_4[] = "mainField4";
const char eta[] = "eta";
const char time_to_destination[] = "timeToDestination";
const char total_distance[] = "totalDistance";
const char alignment[] = "alignment";
const char graphic[] = "graphic";
const char secondary_graphic[] = "secondaryGraphic";
const char status_bar[] = "statusBar";
const char media_clock[] = "mediaClock";
const char media_track[] = "mediaTrack";
const char properties[] = "properties";
const char cmd_id[] = "cmdID";
const char menu_params[] = "menuParams";
const char menu_title[] = "menuTitle";
const char menu_icon[] = "menuIcon";
const char keyboard_properties[] = "keyboardProperties";
const char vr_commands[] = "vrCommands";
const char position[] = "position";
const char num_ticks[] = "numTicks";
const char slider_footer[] = "sliderFooter";
const char menu_id[] = "menuID";
const char menu_name[] = "menuName";
const char interaction_choice_set_id[] = "interactionChoiceSetID";
const char interaction_choice_set_id_list[] = "interactionChoiceSetIDList";
const char choice_set[] = "choiceSet";
const char choice_id[] = "choiceID";
const char grammar_id[] = "grammarID";
const char navigation_text_1[] = "navigationText1";
const char navigation_text_2[] = "navigationText2";
const char alert_text1[] = "alertText1";
const char alert_text2[] = "alertText2";
const char alert_text3[] = "alertText3";
const char tts_chunks[] = "ttsChunks";
const char initial_prompt[] = "initialPrompt";
const char initial_text[] = "initialText";
const char duration[] = "duration";
const char progress_indicator[] = "progressIndicator";
const char alert_type[] = "alertType";
const char play_tone[] = "playTone";
const char soft_buttons[] = "softButtons";
const char soft_button_id[] = "softButtonID";
const char custom_presets[] = "customPresets";
const char audio_pass_display_text1[] = "audioPassThruDisplayText1";
const char audio_pass_display_text2[] = "audioPassThruDisplayText2";
const char max_duration[] = "maxDuration";
const char sampling_rate[] = "samplingRate";
const char bits_per_sample[] = "bitsPerSample";
const char audio_type[] = "audioType";
const char mute_audio[] = "muteAudio";
const char button_name[] = "buttonName";
const char button_event_mode[] = "buttonEventMode";
const char button_press_mode[] = "buttonPressMode";
const char custom_button_id[] = "customButtonID";
const char data_type[] = "dataType";
const char turn_list[] = "turnList";
const char turn_icon[] = "turnIcon";
const char next_turn_icon[] = "nextTurnIcon";
const char value[] = "value";
const char hmi_display_language[] = "hmiDisplayLanguage";
const char language[] = "language";
const char data[] = "data";
const char start_time[] = "startTime";
const char end_time[] = "endTime";
const char hours[] = "hours";
const char minutes[] = "minutes";
const char seconds [] = "seconds";
const char update_mode[] = "updateMode";
const char trigger_source[] = "triggerSource";
const char hmi_level[] = "hmiLevel";
const char activate_app_hmi_level[] = "level";
const char audio_streaming_state[] = "audioStreamingState";
const char system_context[] = "systemContext";
const char speech_capabilities[] = "speechCapabilities";
const char vr_capabilities[] = "vrCapabilities";
const char audio_pass_thru_capabilities[] = "audioPassThruCapabilities";
// PutFile
const char sync_file_name[] = "syncFileName";
const char file_name[] = "fileName";
const char file_type[] = "fileType";
const char file_size[] = "fileSize";
const char request_type[] = "requestType";
const char persistent_file[] = "persistentFile";
const char file_data[] = "fileData";
const char space_available[] = "spaceAvailable";
const char image_type[] = "imageType";
const char image[] = "image";
const char type[] = "type";
const char system_file[] = "systemFile";
const char offset[] = "offset";
const char length[] = "length";
const char secondary_image[] = "secondaryImage";
const char filenames[] = "filenames";

const char hmi_display_language_desired[] = "hmiDisplayLanguageDesired";
const char ecu_name[] = "ecuName";
const char dtc_mask[] = "dtcMask";
const char did_location[] = "didLocation";
const char app_list[] = "appList";
const char device_list[] = "deviceList";
const char device_info[] = "deviceInfo";
const char name[] = "name";
const char id[] = "id";
const char isSDLAllowed[] = "isSDLAllowed";
const char application[] = "application";
const char applications[] = "applications";
const char icon[] = "icon";
const char device_name[] = "deviceName";
const char reason[] = "reason";
const char available[] = "available";
const char text[] = "text";
const char character_set[] = "characterSet";
const char secondary_text[] = "secondaryText";
const char tertiary_text[] = "tertiaryText";
const char hardware[] = "hardware";
const char firmware_rev[] = "firmwareRev";
const char os[] = "os";
const char os_version[] = "osVersion";
const char carrier[] = "carrier";
const char slider_header[] = "sliderHeader";

// duplicate names from hmi_request
const char limited_character_list[] = "limitedCharacterList";
const char auto_complete_text[] = "autoCompleteText";
const char navigation_text[] = "navigationText";

// vehicle info
const char gps[] = "gps";
const char speed[] = "speed";
const char rpm[] = "rpm";
const char fuel_level[] = "fuelLevel";
const char fuel_level_state[] = "fuelLevel_State";
const char instant_fuel_consumption[] = "instantFuelConsumption";
const char external_temp[] = "externalTemperature";
const char vin[] = "vin";
const char prndl[] = "prndl";
const char tire_pressure[] = "tirePressure";
const char odometer[] = "odometer";
const char belt_status[] = "beltStatus";
const char body_information[] = "bodyInformation";
const char device_status[] = "deviceStatus";
const char driver_braking[] = "driverBraking";
const char wiper_status[] = "wiperStatus";
const char head_lamp_status[] = "headLampStatus";
const char engine_torque[] = "engineTorque";
const char acc_pedal_pos[] = "accPedalPosition";
const char steering_wheel_angle[] = "steeringWheelAngle";
const char e_call_info[] = "eCallInfo";
const char airbag_status[] = "airbagStatus";
const char emergency_event[] = "emergencyEvent";
const char cluster_mode_status[] = "clusterModeStatus";
const char my_key[] = "myKey";
const char help_prompt[] = "helpPrompt";
const char scroll_message_body[] = "scrollableMessageBody";
const char data_result[] = "dataResult";
const char dtc_list[] = "dtcList";
const char interaction_mode[] = "interactionMode";
const char slider_position[] = "sliderPosition";
const char system_action[] = "systemAction";
const char prerecorded_speech[] = "prerecordedSpeech";
const char supported_diag_modes[] = "supportedDiagModes";
const char priority[] = "priority";

//resuming
const char application_commands[] = "applicationCommands";
const char application_submenus[] = "applicationSubMenus";
const char application_choise_sets[] = "applicationChoiceSets";
const char application_global_properties[] = "globalProperties";
const char application_vehicle_info[] = "vehicleInfo";
const char application_buttons[] = "buttons";
const char application_subscribtions[] = "subscribtions";
const char application_files[] = "applicationFiles";
const char application_show[] = "applicationShow";
const char resumption[] = "resumption";
const char resume_vr_grammars[] = "resumeVrGrammars";

const char ign_off_count[] = "ign_off_count";
const char connection_info[] = "connection_info";
const char is_download_complete[] = "is_download_complete";

const char hash_id[] = "hashID";
const char time_stamp[] = "timeStamp";
const char manual_text_entry[] = "manualTextEntry";
const char image_type_supported[] = "imageTypeSupported";
const char unexpected_disconnect[] = "unexpectedDisconnect";
const char location_name[] = "locationName";
const char location_description[] = "locationDescription";
const char address_lines[] = "addressLines";
const char phone_number[] = "phoneNumber";
const char location_image[] = "locationImage";
}  // namespace strings

namespace mobile_notification {
const char state[] = "state";
const char syncp_timeout[] = "Timeout";
const char syncp_url[] = "URL";
}  // namespace mobile_notification

namespace hmi_levels {
const char kFull[] = "FULL";
const char kLimited[] = "LIMITED";
const char kBackground[] = "BACKGROUND";
const char kNone[] = "NONE";
}

namespace hmi_request {
const char parent_id[] = "parentID";
const char field_name[] = "fieldName";
const char field_text[] = "fieldText";
const char alert_strings[] = "alertStrings";
const char duration[] = "duration";
const char soft_buttons[] = "softButtons";
const char tts_chunks[] = "ttsChunks";
const char speak_type[] = "speakType";
const char audio_pass_display_texts[] = "audioPassThruDisplayTexts";
const char max_duration[] = "maxDuration";
const char reason[] = "reason";
const char message_text[] = "messageText";
const char initial_text[] = "initialText";
const char navi_texts[] = "navigationTexts";
const char navi_text[] = "navigationText";
const char show_strings[] = "showStrings";
const char interaction_layout[] = "interactionLayout";
const char menu_title[] = "menuTitle";
const char menu_icon[] = "menuIcon";
const char keyboard_properties[] = "keyboardProperties";
const char method_name[] = "methodName";
const char keyboard_layout[] = "keyboardLayout";
const char limited_character_list[] = "limitedCharacterList";
const char auto_complete_text[] = "autoCompleteText";
const char file[] = "file";
const char retry[] = "retry";
const char service[] = "service";
}  // namespace hmi_request

namespace hmi_response {
const char code[] = "code";
const char message[] = "message";
const char method[] = "method";
const char try_again_time[] = "tryAgainTime";
const char custom_button_id[] = "customButtonID";
const char button_name[] = "name";
const char button_mode[] = "mode";
const char attenuated_supported[] = "attenuatedSupported";
const char languages[] = "languages";
const char language[] = "language";
const char display_capabilities[] = "displayCapabilities";
const char hmi_zone_capabilities[] = "hmiZoneCapabilities";
const char soft_button_capabilities[] = "softButtonCapabilities";
const char image_supported[] = "imageSupported";
const char button_capabilities[] = "buttonCapabilities";
const char capabilities[] = "capabilities";
const char speech_capabilities[] = "speechCapabilities";
const char prerecorded_speech_capabilities[] = "prerecordedSpeechCapabilities";
const char preset_bank_capabilities[] = "presetBankCapabilities";
const char allowed[] = "allowed";
const char vehicle_type[] = "vehicleType";
const char did_result[] = "didResult";
const char result_code[] = "resultCode";
const char dtc[] = "dtc";
const char ecu_header[] = "ecuHeader";
const char image_capabilities[] = "imageCapabilities";
const char display_type[] = "displayType";
const char text_fields[] = "textFields";
const char media_clock_formats[] = "mediaClockFormats";
const char graphic_supported[] = "graphicSupported";
const char image_fields[] = "imageFields";
const char templates_available[] = "templatesAvailable";
const char screen_params[] = "screenParams";
const char num_custom_presets_available[] = "numCustomPresetsAvailable";
const char urls[] = "urls";
const char policy_app_id[] = "policyAppId";
}  // namespace hmi_response

namespace hmi_notification {
const char prndl[] = "prndl";
const char file_name[] = "file_name";
const char system_context[] = "systemContext";
const char state[] = "state";
const char result[] = "result";
const char statistic_type[] = "statisticType";
const char error[] = "error";
const char policyfile[] = "policyfile";
const char is_active[] = "isActive";

}  // namespace hmi_notification

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SMART_OBJECT_KEYS_H_

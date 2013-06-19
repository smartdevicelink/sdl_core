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
const char connection_key[] = "connection_key";

const char msg_params[] = "msg_params";
const char info[] = "info";
const char app_id[] = "appId";
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
const char app_type[] = "AppType";
const char tts_name[] = "ttsName";
const char binary_data[] = "binary_data";
const char help_promt[] = "helpPrompt";
const char timeout_promt[] = "timeoutPrompt";
const char vr_help_title[] = "vrHelpTitle";
const char vr_help[] = "vrHelp";
const char main_field_1[] = "mainField1";
const char main_field_2[] = "mainField2";
const char alignment[] = "alignment";
const char status_bar[] = "statusBar";
const char media_clock[] = "media_clock";
const char media_track[] = "media_track";
const char properties[] = "properties";
const char cmd_id[] = "cmdID";
const char menu_params[] = "menuParams";
const char vr_commands[] = "vrCommands";
const char menu_id[] = "menuID";
const char menu_name[] = "menuName";
const char interaction_choice_set_id[] = "interactionChoiceSetID";
const char choice_set[] = "choiceSet";
const char alert_text1[] = "alertText1";
const char alert_text2[] = "alertText2";
const char alert_text3[] = "alertText3";
const char tts_chunks[] = "ttsChunks";
const char duration[] = "duration";
const char play_tone[] = "playTone";
const char soft_buttons[] = "softButtons";
const char audio_pass_display_text1[] = "audioPassThruDisplayText1";
const char audio_pass_display_text2[] = "audioPassThruDisplayText2";
const char max_duration[] = "maxDuration";
const char sampling_rate[] = "samplingRate";
const char bits_per_sample[] = "bitsPerSample";
const char audio_type[] = "audioType";
const char button_name[] = "buttonName";
const char button_event_mode[] = "buttonEventMode";
const char button_press_mode[] = "buttonPressMode";
const char custom_button_id[] = "customButtonID";
const char data_type[] = "dataType";
const char turn_list[] = "turnList";
const char turn_icon[] = "turnIcon";
const char value[] = "value";
const char hmi_display_language[] = "hmiDisplayLanguage";
const char language[] = "language";
const char data[] = "data";
const char trigger_source[] = "triggerSource";
const char prndl[] = "prndl";
const char hmi_level[] = "hmiLevel";
const char audio_streaming_state[] = "audioStreamingState";
const char system_context[] = "systemContext";
// PutFile
const char sync_file_name[] = "syncFileName";
const char file_type[] = "fileType";
const char persistent_file[] = "persistentFile";
const char file_data[] = "fileData";
const char space_available[] = "spaceAvailable";

const char filenames[] = "filenames";

const char hmi_display_language_desired[] = "hmiDisplayLanguageDesired";
const char ecu_name[] = "ecuName";
const char did_location[] = "didLocation";
const char app_list[] = "appList";
const char device_list[] = "deviceList";
const char device_info[] = "deviceInfo";
const char name[] = "name";
const char id[] = "id";
}  // namespace strings

namespace hmi_request {
  const char field_name[] = "fieldName";
  const char field_text[] = "fieldText";
  const char alert_strings[] = "alertStrings";
  const char duration[] = "duration";
  const char soft_buttons[] = "softButtons";
  const char tts_chunks[] = "ttsChunks";
  const char audio_pass_display_texts[] = "audioPassThruDisplayTexts";
  const char max_duration[] = "maxDuration";
}  // namespace hmi_request

namespace hmi_response {
  const char code[] = "code";
  const char method[] = "method";
  const char try_again_time[] = "tryAgainTime";
  const char custom_button_id[] = "customButtonID";
  const char button_name[] = "name";
  const char button_mode[] = "mode";
}  // namespace hmi_response

namespace hmi_notification {
  const char prndl[] = "prndl";
  const char file_name[] = "file_name";
  const char system_context[] = "systemContext";
}  // namespace hmi_notification

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SMART_OBJECT_KEYS_H_

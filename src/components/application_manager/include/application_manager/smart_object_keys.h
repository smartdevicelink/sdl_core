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

const char hmi_display_language_desired[] = "hmiDisplayLanguageDesired";

}  // namespace strings

namespace hmi_response {
  const char code[] = "code";
  const char method[] = "method";
}

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_SMART_OBJECT_KEYS_H_

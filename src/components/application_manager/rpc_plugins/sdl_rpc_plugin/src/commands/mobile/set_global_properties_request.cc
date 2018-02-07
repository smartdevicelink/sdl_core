/*
 Copyright (c) 2015, Ford Motor Company
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

#include <string.h>
#include <algorithm>
#include "sdl_rpc_plugin/commands/mobile/set_global_properties_request.h"

#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "utils/helpers.h"

namespace application_manager {

namespace commands {

SetGlobalPropertiesRequest::SetGlobalPropertiesRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager)
    , is_ui_send_(false)
    , is_tts_send_(false)
    , is_ui_received_(false)
    , is_tts_received_(false)
    , ui_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , tts_result_(hmi_apis::Common_Result::INVALID_ENUM) {}

SetGlobalPropertiesRequest::~SetGlobalPropertiesRequest() {}

void SetGlobalPropertiesRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "No application associated with connection key "
                      << connection_key());
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (!ValidateConditionalMandatoryParameters(msg_params)) {
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "There are no parameters present in request.");
    return;
  }

  mobile_apis::Result::eType verification_result = mobile_apis::Result::SUCCESS;

  if ((*message_)[strings::msg_params].keyExists(strings::menu_icon)) {
    verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::menu_icon],
        app,
        application_manager_);
    if (mobile_apis::Result::SUCCESS != verification_result) {
      LOG4CXX_ERROR(
          logger_, "MessageHelper::VerifyImage return " << verification_result);
      SendResponse(false, verification_result);
      return;
    }
  }
  // Check for image file(s) in vrHelpItem
  if ((*message_)[strings::msg_params].keyExists(strings::vr_help)) {
    if (mobile_apis::Result::SUCCESS !=
        MessageHelper::VerifyImageVrHelpItems(
            (*message_)[strings::msg_params][strings::vr_help],
            app,
            application_manager_)) {
      LOG4CXX_ERROR(logger_, "MessageHelper::VerifyImage return INVALID_DATA!");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_ERROR(logger_, "White spaces found");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  // if application waits for sending ttsGlobalProperties need to remove this
  // application from tts_global_properties_app_list_
  application_manager_.RemoveAppFromTTSGlobalPropertiesList(connection_key());
  bool is_help_prompt_present = msg_params.keyExists(strings::help_prompt);
  bool is_timeout_prompt_present =
      msg_params.keyExists(strings::timeout_prompt);
  bool is_vr_help_title_present = msg_params.keyExists(strings::vr_help_title);
  bool is_vr_help_present = msg_params.keyExists(strings::vr_help);

  // check VR params
  if (is_vr_help_title_present ^ is_vr_help_present) {
    LOG4CXX_ERROR(logger_,
                  "Reject because of vr_help or vr_help_title only provided");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  /* Need to set flags before sending request to HMI
   * for correct processing this flags in method on_event */
  if (is_help_prompt_present || is_timeout_prompt_present) {
    is_tts_send_ = true;
  }
  if (is_vr_help_title_present && is_vr_help_present) {
    LOG4CXX_DEBUG(logger_, "VRHelp params presents");

    if (!CheckVrHelpItemsOrder(msg_params[strings::vr_help])) {
      LOG4CXX_ERROR(logger_,
                    "VR Help Items contains nonsequential positions"
                        << " (e.g. [1,2,4]) or not started from 1");
      SendResponse(false, mobile_apis::Result::REJECTED);
      return;
    }

    smart_objects::SmartObject params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    PrepareUIRequestVRHelpData(app, msg_params, params);
    PrepareUIRequestMenuAndKeyboardData(app, msg_params, params);

    params[strings::app_id] = app->app_id();
    SendUIRequest(params, true);
  } else {
    LOG4CXX_DEBUG(logger_, "VRHelp params does not present");
    DCHECK_OR_RETURN_VOID(!is_vr_help_title_present && !is_vr_help_present);

    smart_objects::SmartObject params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    if (ValidateVRHelpTitle(app->vr_help_title())) {
      LOG4CXX_DEBUG(logger_, "App already contains VRHelp data");
    } else {
      if (!PrepareUIRequestDefaultVRHelpData(app, params)) {
        LOG4CXX_ERROR(logger_, "default VRHElp data could not be generated");
        SendResponse(false, mobile_apis::Result::INVALID_DATA);
        return;
      }
    }
    PrepareUIRequestMenuAndKeyboardData(app, msg_params, params);

    // Preparing data
    if (params.empty()) {
      LOG4CXX_DEBUG(logger_, "No UI info provided");
    } else {
      params[strings::app_id] = app->app_id();
      SendUIRequest(params, true);
    }
  }

  // check TTS params
  if (is_help_prompt_present || is_timeout_prompt_present) {
    LOG4CXX_DEBUG(logger_, "TTS params presents");
    smart_objects::SmartObject params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);

    if (is_help_prompt_present) {
      app->set_help_prompt(msg_params.getElement(strings::help_prompt));
      params[strings::help_prompt] = (*app->help_prompt());
    }

    if (is_timeout_prompt_present) {
      app->set_timeout_prompt(msg_params.getElement(strings::timeout_prompt));
      params[strings::timeout_prompt] = (*app->timeout_prompt());
    }

    params[strings::app_id] = app->app_id();
    SendTTSRequest(params, true);
  }
}

bool SetGlobalPropertiesRequest::CheckVrHelpItemsOrder(
    const smart_objects::SmartObject& vr_help) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN(vr_help.getType() == smart_objects::SmartType_Array, false);
  const size_t vr_help_length = vr_help.length();
  DCHECK_OR_RETURN(vr_help_length > 0, false);

  for (size_t j = 0; j < vr_help_length; ++j) {
    const size_t position =
        vr_help.getElement(j).getElement(strings::position).asUInt();
    // Elements shall start from 1 and increment one by one
    if (position != (j + 1)) {
      LOG4CXX_ERROR(logger_,
                    "VR help items order is wrong"
                        << " at " << j << ", position value:" << position);
      return false;
    }
  }
  return true;
}

void SetGlobalPropertiesRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_SetGlobalProperties: {
      LOG4CXX_INFO(logger_, "Received UI_SetGlobalProperties event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      is_ui_received_ = true;
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, ui_response_info_);
      break;
    }
    case hmi_apis::FunctionID::TTS_SetGlobalProperties: {
      LOG4CXX_INFO(logger_, "Received TTS_SetGlobalProperties event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
      is_tts_received_ = true;
      tts_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, tts_response_info_);
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }

  if (IsPendingResponseExist()) {
    LOG4CXX_DEBUG(logger_, "Continue waiting for response");
    return;
  }
  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;
  std::string response_info;
  const bool result = PrepareResponseParameters(result_code, response_info);

  // TODO{ALeshin} APPLINK-15858. connection_key removed during SendResponse
  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  SendResponse(result,
               result_code,
               response_info.empty() ? NULL : response_info.c_str(),
               &(message[strings::msg_params]));
}

bool SetGlobalPropertiesRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

bool SetGlobalPropertiesRequest::PrepareResponseParameters(
    mobile_apis::Result::eType& result_code, std::string& info) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  ResponseInfo ui_properties_info(
      ui_result_, HmiInterfaces::HMI_INTERFACE_UI, application_manager_);

  ResponseInfo tts_properties_info(
      tts_result_, HmiInterfaces::HMI_INTERFACE_TTS, application_manager_);
  const bool result =
      PrepareResultForMobileResponse(ui_properties_info, tts_properties_info);
  if (result &&
      (HmiInterfaces::STATE_AVAILABLE == tts_properties_info.interface_state) &&
      (tts_properties_info.is_unsupported_resource)) {
    result_code = mobile_apis::Result::WARNINGS;
    tts_response_info_ = "Unsupported phoneme type sent in a prompt";
    info = MergeInfos(tts_properties_info,
                      tts_response_info_,
                      ui_properties_info,
                      ui_response_info_);
    return result;
  }
  result_code =
      PrepareResultCodeForResponse(ui_properties_info, tts_properties_info);
  info = MergeInfos(tts_properties_info,
                    tts_response_info_,
                    ui_properties_info,
                    ui_response_info_);
  return result;
}

bool SetGlobalPropertiesRequest::ValidateVRHelpTitle(
    const smart_objects::SmartObject* const vr_help_so_ptr) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (vr_help_so_ptr) {
    const std::string& vr_help = vr_help_so_ptr->asString();
    LOG4CXX_TRACE(logger_, "App contains vr_help_title: \"" << vr_help << '"');
    return !vr_help.empty();
  }
  return false;
}

void SetGlobalPropertiesRequest::PrepareUIRequestVRHelpData(
    const ApplicationSharedPtr app,
    const smart_objects::SmartObject& msg_params,
    smart_objects::SmartObject& out_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);

  app->set_vr_help_title(msg_params.getElement(strings::vr_help_title));
  app->set_vr_help(msg_params.getElement(strings::vr_help));

  out_params[strings::vr_help_title] = (*app->vr_help_title());
  out_params[strings::vr_help] = (*app->vr_help());
}

bool SetGlobalPropertiesRequest::PrepareUIRequestDefaultVRHelpData(
    const ApplicationSharedPtr app, smart_objects::SmartObject& out_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN(app, false);

  LOG4CXX_DEBUG(logger_, "Generate default VRHelp data");
  const DataAccessor<CommandsMap> accessor = app->commands_map();
  const CommandsMap& cmdMap = accessor.GetData();

  int32_t index = 0;
  smart_objects::SmartObject vr_help_items;
  for (CommandsMap::const_iterator command_it = cmdMap.begin();
       cmdMap.end() != command_it;
       ++command_it) {
    const smart_objects::SmartObject& command = *command_it->second;
    if (!command.keyExists(strings::vr_commands)) {
      LOG4CXX_ERROR(logger_, "VR synonyms are empty");
      return false;
    }
    // use only first
    vr_help_items[index][strings::position] = (index + 1);
    vr_help_items[index++][strings::text] =
        (*command_it->second)[strings::vr_commands][0];
  }

  app->set_vr_help_title(smart_objects::SmartObject(app->name()));

  out_params[strings::vr_help_title] = (*app->vr_help_title());
  if (vr_help_items.length() > 0) {
    app->set_vr_help(vr_help_items);
    out_params[strings::vr_help] = (*app->vr_help());
  }
  return true;
}

void SetGlobalPropertiesRequest::PrepareUIRequestMenuAndKeyboardData(
    const ApplicationSharedPtr app,
    const smart_objects::SmartObject& msg_params,
    smart_objects::SmartObject& out_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(app);

  const bool is_menu_title_present =
      msg_params.keyExists(hmi_request::menu_title);
  const bool is_menu_icon_present =
      msg_params.keyExists(hmi_request::menu_icon);
  const bool is_keyboard_props_present =
      msg_params.keyExists(hmi_request::keyboard_properties);

  if (is_menu_title_present) {
    out_params[hmi_request::menu_title] =
        msg_params[hmi_request::menu_title].asString();
    app->set_menu_title(msg_params[hmi_request::menu_title]);
  }
  if (is_menu_icon_present) {
    out_params[hmi_request::menu_icon] = msg_params[hmi_request::menu_icon];
    app->set_menu_icon(msg_params[hmi_request::menu_icon]);
  }
  if (is_keyboard_props_present) {
    out_params[hmi_request::keyboard_properties] =
        msg_params[hmi_request::keyboard_properties];
    app->set_keyboard_props(msg_params[hmi_request::keyboard_properties]);
  }
}

void SetGlobalPropertiesRequest::SendTTSRequest(
    const smart_objects::SmartObject& params, bool use_events) {
  LOG4CXX_AUTO_TRACE(logger_);
  is_tts_send_ = true;
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
  SendHMIRequest(
      hmi_apis::FunctionID::TTS_SetGlobalProperties, &params, use_events);
}

void SetGlobalPropertiesRequest::SendUIRequest(
    const smart_objects::SmartObject& params, bool use_events) {
  LOG4CXX_AUTO_TRACE(logger_);
  is_ui_send_ = true;
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(
      hmi_apis::FunctionID::UI_SetGlobalProperties, &params, use_events);
}

bool SetGlobalPropertiesRequest::IsPendingResponseExist() {
  return is_ui_send_ != is_ui_received_ || is_tts_send_ != is_tts_received_;
}

bool SetGlobalPropertiesRequest::ValidateConditionalMandatoryParameters(
    const smart_objects::SmartObject& params) {
  LOG4CXX_AUTO_TRACE(logger_);
  return params.keyExists(strings::help_prompt) ||
         params.keyExists(strings::timeout_prompt) ||
         params.keyExists(strings::vr_help_title) ||
         params.keyExists(strings::vr_help) ||
         params.keyExists(strings::menu_title) ||
         params.keyExists(strings::menu_icon) ||
         params.keyExists(strings::keyboard_properties);
}

bool SetGlobalPropertiesRequest::IsWhiteSpaceExist() {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str;

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (msg_params.keyExists(strings::help_prompt)) {
    const smart_objects::SmartArray* hp_array =
        msg_params[strings::help_prompt].asArray();

    smart_objects::SmartArray::const_iterator it_hp = hp_array->begin();
    smart_objects::SmartArray::const_iterator it_hp_end = hp_array->end();

    for (; it_hp != it_hp_end; ++it_hp) {
      str = (*it_hp)[strings::text].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid help_prompt syntax check failed");
        return true;
      }
    }
  }

  if (msg_params.keyExists(strings::timeout_prompt)) {
    const smart_objects::SmartArray* tp_array =
        msg_params[strings::timeout_prompt].asArray();

    smart_objects::SmartArray::const_iterator it_tp = tp_array->begin();
    smart_objects::SmartArray::const_iterator it_tp_end = tp_array->end();

    for (; it_tp != it_tp_end; ++it_tp) {
      str = (*it_tp)[strings::text].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid timeout_prompt syntax check failed");
        return true;
      }
    }
  }

  if (msg_params.keyExists(strings::vr_help)) {
    const smart_objects::SmartArray* vh_array =
        msg_params[strings::vr_help].asArray();

    smart_objects::SmartArray::const_iterator it_vh = vh_array->begin();
    smart_objects::SmartArray::const_iterator it_vh_end = vh_array->end();

    for (; it_vh != it_vh_end; ++it_vh) {
      str = (*it_vh)[strings::text].asCharArray();
      if (!CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid vr_help text syntax check failed");
        return true;
      }

      if ((*it_vh).keyExists(strings::image)) {
        str = (*it_vh)[strings::image][strings::value].asCharArray();
        if (!CheckSyntax(str)) {
          LOG4CXX_ERROR(logger_,
                        "Invalid vr_help image value syntax check failed");
          return true;
        }
      }  // if image exists
    }    // for - vh_array iteration
  }

  if (msg_params.keyExists(strings::menu_icon)) {
    str = msg_params[strings::menu_icon][strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid menu_icon value syntax check failed");
      return true;
    }
  }

  if (msg_params.keyExists(strings::vr_help_title)) {
    str = msg_params[strings::vr_help_title].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid vr_help_title value syntax check failed");
      return true;
    }
  }

  if (msg_params.keyExists(strings::menu_title)) {
    str = msg_params[strings::menu_title].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid menu_title value syntax check failed");
      return true;
    }
  }

  if (msg_params.keyExists(strings::keyboard_properties)) {
    if (msg_params[strings::keyboard_properties].keyExists(
            strings::limited_character_list)) {
      const smart_objects::SmartArray* lcl_array =
          msg_params[strings::keyboard_properties]
                    [strings::limited_character_list].asArray();

      smart_objects::SmartArray::const_iterator it_lcl = lcl_array->begin();
      smart_objects::SmartArray::const_iterator it_lcl_end = lcl_array->end();

      for (; it_lcl != it_lcl_end; ++it_lcl) {
        str = (*it_lcl).asCharArray();
        if (!CheckSyntax(str)) {
          LOG4CXX_ERROR(logger_,
                        "Invalid keyboard_properties "
                        "limited_character_list syntax check failed");
          return true;
        }
      }
    }

    if (msg_params[strings::keyboard_properties].keyExists(
            strings::auto_complete_text)) {
      str =
          msg_params[strings::keyboard_properties][strings::auto_complete_text]
              .asCharArray();

      if (!CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_,
                      "Invalid keyboard_properties "
                      "auto_complete_text syntax check failed");
        return true;
      }
    }
  }
  return false;
}

}  // namespace commands
}  // namespace application_manager

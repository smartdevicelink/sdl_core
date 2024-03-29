/*
 Copyright (c) 2018, Ford Motor Company
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

#include "sdl_rpc_plugin/commands/mobile/set_global_properties_request.h"
#include <string.h>
#include <algorithm>
#include <numeric>
#include "application_manager/resumption/resume_ctrl.h"

#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

#include "utils/helpers.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

namespace {
bool IsResultCodeWarning(const app_mngr::commands::ResponseInfo& first,
                         const app_mngr::commands::ResponseInfo& second,
                         const app_mngr::commands::ResponseInfo& third) {
  const bool first_is_ok_second_is_warn =
      (first.is_ok || first.is_not_used) &&
      (hmi_apis::Common_Result::WARNINGS == second.result_code) &&
      (hmi_apis::Common_Result::WARNINGS == third.result_code);

  const bool final_warnings =
      hmi_apis::Common_Result::WARNINGS == first.result_code &&
      ((hmi_apis::Common_Result::WARNINGS == second.result_code) &&
       (hmi_apis::Common_Result::WARNINGS == third.result_code));

  return first_is_ok_second_is_warn || final_warnings;
}
}  // namespace

SetGlobalPropertiesRequest::SetGlobalPropertiesRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler)
    , is_ui_send_(false)
    , is_tts_send_(false)
    , is_rc_send_(false)
    , is_ui_received_(false)
    , is_tts_received_(false)
    , is_rc_received_(false)
    , ui_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , tts_result_(hmi_apis::Common_Result::INVALID_ENUM)
    , rc_result_(hmi_apis::Common_Result::INVALID_ENUM) {}

SetGlobalPropertiesRequest::~SetGlobalPropertiesRequest() {}

void SetGlobalPropertiesRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("No application associated with connection key "
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

  if (msg_params.keyExists(strings::menu_icon)) {
    mobile_apis::Result::eType verification_result = MessageHelper::VerifyImage(
        msg_params[strings::menu_icon], app, application_manager_);
    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      SDL_LOG_ERROR("MessageHelper::VerifyImage return "
                    << verification_result);
      SendResponse(false, verification_result);
      return;
    }
  }
  // Check for image file(s) in vrHelpItem
  if (msg_params.keyExists(strings::vr_help)) {
    if (mobile_apis::Result::INVALID_DATA ==
        MessageHelper::VerifyImageVrHelpItems(
            msg_params[strings::vr_help], app, application_manager_)) {
      SDL_LOG_ERROR("MessageHelper::VerifyImage return INVALID_DATA!");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
  }

  if (msg_params.keyExists(strings::keyboard_properties)) {
    if (!msg_params[strings::keyboard_properties].keyExists(
            strings::auto_complete_list) &&
        msg_params[strings::keyboard_properties].keyExists(
            strings::auto_complete_text)) {
      SDL_LOG_ERROR("Replacing deprecated autoCompleteText property");
      msg_params[strings::keyboard_properties][strings::auto_complete_list][0] =
          msg_params[strings::keyboard_properties][strings::auto_complete_text]
              .asString();
    }
    msg_params[strings::keyboard_properties].erase(strings::auto_complete_text);
  }

  if (IsWhiteSpaceExist()) {
    SDL_LOG_ERROR("White spaces found");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (!ValidateCustomKeys()) {
    SDL_LOG_ERROR(
        "Number of customizable keys exceeds the maximum number for this "
        "layout");
    SendResponse(
        false,
        mobile_apis::Result::INVALID_DATA,
        "customKeys exceeds the number of customizable keys in this Layout");
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
    SDL_LOG_ERROR("Reject because of vr_help or vr_help_title only provided");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  smart_objects::SmartObject params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  if (msg_params.keyExists(strings::menu_layout)) {
    auto menu_layout = static_cast<mobile_apis::MenuLayout::eType>(
        msg_params[strings::menu_layout].asUInt());
    if (app->menu_layout_supported(menu_layout)) {
      params[strings::menu_layout] = msg_params[strings::menu_layout];
      app->set_menu_layout(msg_params[strings::menu_layout]);
    } else {
      is_menu_layout_available_ = false;
    }
  }

  /* Need to set flags before sending request to HMI
   * for correct processing this flags in method on_event */
  if (is_help_prompt_present || is_timeout_prompt_present) {
    is_tts_send_ = true;
  }
  if (is_vr_help_title_present && is_vr_help_present) {
    SDL_LOG_DEBUG("VRHelp params presents");

    if (!CheckVrHelpItemsOrder(msg_params[strings::vr_help])) {
      SDL_LOG_ERROR("VR Help Items contains nonsequential positions"
                    << " (e.g. [1,2,4]) or not started from 1");
      SendResponse(false, mobile_apis::Result::REJECTED);
      return;
    }

    PrepareUIRequestVRHelpData(app, msg_params, params);
    PrepareUIRequestMenuAndKeyboardData(app, msg_params, params);

    params[strings::app_id] = app->app_id();
    SendUIRequest(params, true);

    auto& help_prompt_manager = app->help_prompt_manager();
    help_prompt_manager.OnSetGlobalPropertiesReceived(params, false);
  } else {
    SDL_LOG_DEBUG("VRHelp params does not present");
    DCHECK_OR_RETURN_VOID(!is_vr_help_title_present && !is_vr_help_present);

    PrepareUIRequestMenuAndKeyboardData(app, msg_params, params);

    // Preparing data
    if (params.empty()) {
      SDL_LOG_DEBUG("No UI info provided");
    } else {
      params[strings::app_id] = app->app_id();
      SendUIRequest(params, true);
    }
  }

  // Check RC params
  const bool is_user_location_present =
      msg_params.keyExists(strings::user_location);
  if (is_user_location_present) {
    if (msg_params[strings::user_location].empty()) {
      SendResponse(
          false, mobile_apis::Result::INVALID_DATA, "UserLocation is empty");
      return;
    }
    SDL_LOG_DEBUG("Userlocation params presents");
    const auto& user_location = msg_params[strings::user_location];
    app->set_user_location(user_location);

    auto on_global_properties_updated = [app](
                                            plugin_manager::RPCPlugin& plugin) {
      plugin.OnApplicationEvent(plugin_manager::kGlobalPropertiesUpdated, app);
    };

    application_manager_.GetPluginManager().ForEachPlugin(
        on_global_properties_updated);

    smart_objects::SmartObject rc_request_params =
        smart_objects::SmartObject(smart_objects::SmartType_Map);
    rc_request_params[strings::app_id] = app->app_id();
    rc_request_params[strings::user_location] = user_location;
    SendRCRequest(rc_request_params, true);
  }

  // check TTS params
  if (is_help_prompt_present || is_timeout_prompt_present) {
    SDL_LOG_DEBUG("TTS params presents");
    auto tts_params = smart_objects::SmartObject(smart_objects::SmartType_Map);

    std::vector<std::string> invalid_params;
    if (is_help_prompt_present) {
      smart_objects::SmartObject& help_prompt =
          (*message_)[strings::msg_params][strings::help_prompt];
      mobile_apis::Result::eType verification_result =
          MessageHelper::VerifyTtsFiles(help_prompt, app, application_manager_);

      if (mobile_apis::Result::FILE_NOT_FOUND == verification_result) {
        SDL_LOG_ERROR("MessageHelper::VerifyTtsFiles return "
                      << verification_result);
        invalid_params.push_back("help_prompt");
      } else {
        app->set_help_prompt(help_prompt);
        tts_params[strings::help_prompt] = (*app->help_prompt());
      }
    }

    if (is_timeout_prompt_present) {
      smart_objects::SmartObject& timeout_prompt =
          (*message_)[strings::msg_params][strings::timeout_prompt];
      mobile_apis::Result::eType verification_result =
          MessageHelper::VerifyTtsFiles(
              timeout_prompt, app, application_manager_);

      if (mobile_apis::Result::FILE_NOT_FOUND == verification_result) {
        SDL_LOG_ERROR("MessageHelper::VerifyTtsFiles return "
                      << verification_result);
        invalid_params.push_back("timeout_prompt");
      } else {
        app->set_timeout_prompt(timeout_prompt);
        tts_params[strings::timeout_prompt] = (*app->timeout_prompt());
      }
    }

    if (!invalid_params.empty()) {
      std::string params_list = std::accumulate(
          std::begin(invalid_params),
          std::end(invalid_params),
          std::string(""),
          [](std::string& first, const std::string& second) {
            return first.empty() ? second : first + ", " + second;
          });
      const std::string info =
          "One or more files needed for " + params_list + " are not present";
      SendResponse(false, mobile_apis::Result::FILE_NOT_FOUND, info.c_str());
      return;
    }

    tts_params[strings::app_id] = app->app_id();
    SendTTSRequest(tts_params, true);

    auto& help_prompt_manager = app->help_prompt_manager();
    help_prompt_manager.OnSetGlobalPropertiesReceived(tts_params, false);
  } else if (!is_ui_send_ && !is_rc_send_) {
    std::string response_info = "There are no parameters present in request.";
    if (!is_menu_layout_available_) {
      response_info += " The MenuLayout specified is unsupported.";
    }
    SendResponse(
        false, mobile_apis::Result::INVALID_DATA, response_info.c_str());
  }
}

bool SetGlobalPropertiesRequest::CheckVrHelpItemsOrder(
    const smart_objects::SmartObject& vr_help) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN(vr_help.getType() == smart_objects::SmartType_Array, false);
  const size_t vr_help_length = vr_help.length();
  DCHECK_OR_RETURN(vr_help_length > 0, false);

  for (size_t j = 0; j < vr_help_length; ++j) {
    const size_t position =
        vr_help.getElement(j).getElement(strings::position).asUInt();
    // Elements shall start from 1 and increment one by one
    if (position != (j + 1)) {
      SDL_LOG_ERROR("VR help items order is wrong"
                    << " at " << j << ", position value:" << position);
      return false;
    }
  }
  return true;
}

void SetGlobalPropertiesRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  using namespace helpers;
  const smart_objects::SmartObject& message = event.smart_object();

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_SetGlobalProperties: {
      SDL_LOG_DEBUG("Received UI_SetGlobalProperties event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      is_ui_received_ = true;
      ui_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, ui_response_info_);
      if (application.use_count() != 0) {
        auto& help_prompt_manager = application->help_prompt_manager();
        help_prompt_manager.OnSetGlobalPropertiesReceived(message, true);
      }
      break;
    }
    case hmi_apis::FunctionID::TTS_SetGlobalProperties: {
      SDL_LOG_DEBUG("Received TTS_SetGlobalProperties event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
      is_tts_received_ = true;
      tts_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, tts_response_info_);
      if (application.use_count() != 0) {
        auto& help_prompt_manager = application->help_prompt_manager();
        help_prompt_manager.OnSetGlobalPropertiesReceived(message, true);
      }
      break;
    }
    case hmi_apis::FunctionID::RC_SetGlobalProperties: {
      SDL_LOG_DEBUG("Received RC_SetGlobalProperties event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_RC);
      is_rc_received_ = true;
      rc_result_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      GetInfo(message, rc_response_info_);
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      return;
    }
  }

  if (IsPendingResponseExist()) {
    SDL_LOG_DEBUG("Continue waiting for response");
    return;
  }
  mobile_apis::Result::eType result_code = mobile_apis::Result::INVALID_ENUM;
  std::string response_info;
  const bool result = PrepareResponseParameters(result_code, response_info);

  if (result && !is_menu_layout_available_) {
    response_info =
        "The MenuLayout specified is unsupported, the "
        "default MenuLayout will be used." +
        response_info;
    SendResponse(result,
                 mobile_apis::Result::WARNINGS,
                 response_info.c_str(),
                 &(message[strings::msg_params]));
  } else {
    SendResponse(result,
                 result_code,
                 response_info.empty() ? NULL : response_info.c_str(),
                 &(message[strings::msg_params]));
  }
}

bool SetGlobalPropertiesRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

void SetGlobalPropertiesRequest::OnTimeOut() {
  SDL_LOG_AUTO_TRACE();

  RequestFromMobileImpl::OnTimeOut();

  auto& resume_ctrl = application_manager_.resume_controller();

  resume_ctrl.HandleOnTimeOut(
      correlation_id(),
      static_cast<hmi_apis::FunctionID::eType>(function_id()));
}

bool SetGlobalPropertiesRequest::PrepareResponseParameters(
    mobile_apis::Result::eType& result_code, std::string& info) {
  SDL_LOG_AUTO_TRACE();
  using namespace helpers;

  app_mngr::commands::ResponseInfo ui_properties_info(
      ui_result_, HmiInterfaces::HMI_INTERFACE_UI, application_manager_);

  app_mngr::commands::ResponseInfo tts_properties_info(
      tts_result_, HmiInterfaces::HMI_INTERFACE_TTS, application_manager_);

  app_mngr::commands::ResponseInfo rc_properties_info(
      rc_result_, HmiInterfaces::HMI_INTERFACE_RC, application_manager_);

  bool result = false;

  if (!is_rc_send_) {
    result = RequestFromMobileImpl::PrepareResultForMobileResponse(
        ui_properties_info, tts_properties_info);
  } else {
    result = PrepareResultForMobileResponse(
        ui_properties_info, tts_properties_info, rc_properties_info);
  }
  if (result &&
      (HmiInterfaces::STATE_AVAILABLE == tts_properties_info.interface_state) &&
      (tts_properties_info.is_unsupported_resource)) {
    result_code = mobile_apis::Result::WARNINGS;
    tts_response_info_ = "Unsupported phoneme type sent in a prompt";
    info = MergeInfos(tts_properties_info,
                      tts_response_info_,
                      ui_properties_info,
                      ui_response_info_,
                      rc_properties_info,
                      rc_response_info_);
    return result;
  }

  if (!is_rc_send_) {
    result_code = RequestFromMobileImpl::PrepareResultCodeForResponse(
        ui_properties_info, tts_properties_info);
  } else {
    result_code = PrepareResultCodeForResponse(
        ui_properties_info, tts_properties_info, rc_properties_info);
  }
  info = MergeInfos(tts_properties_info,
                    tts_response_info_,
                    ui_properties_info,
                    ui_response_info_,
                    rc_properties_info,
                    rc_response_info_);
  return result;
}

bool SetGlobalPropertiesRequest::PrepareResultForMobileResponse(
    const app_mngr::commands::ResponseInfo& first,
    const app_mngr::commands::ResponseInfo& second,
    const app_mngr::commands::ResponseInfo& third) const {
  SDL_LOG_AUTO_TRACE();

  app_mngr::commands::ResponseInfo both_info;
  std::vector<hmi_apis::Common_Result::eType> success_result_codes{
      hmi_apis::Common_Result::SUCCESS,
      hmi_apis::Common_Result::WARNINGS,
      hmi_apis::Common_Result::WRONG_LANGUAGE,
      hmi_apis::Common_Result::RETRY,
      hmi_apis::Common_Result::SAVED};

  both_info.is_ok =
      helpers::in_range(success_result_codes, first.result_code) &&
      helpers::in_range(success_result_codes, second.result_code);

  both_info.is_not_used =
      (hmi_apis::Common_Result::INVALID_ENUM == first.result_code) ||
      (hmi_apis::Common_Result::INVALID_ENUM == second.result_code);

  both_info.is_unsupported_resource =
      (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == first.result_code) ||
      (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == second.result_code);

  const bool final_result =
      RequestFromMobileImpl::CheckResultCode(both_info, third) ||
      RequestFromMobileImpl::CheckResultCode(third, both_info);

  return final_result;
}

mobile_apis::Result::eType
SetGlobalPropertiesRequest::PrepareResultCodeForResponse(
    const app_mngr::commands::ResponseInfo& first,
    const app_mngr::commands::ResponseInfo& second,
    const app_mngr::commands::ResponseInfo& third) {
  SDL_LOG_AUTO_TRACE();
  if (IsResultCodeUnsupported(first, second, third) ||
      IsResultCodeUnsupported(second, third, first) ||
      IsResultCodeUnsupported(third, first, second)) {
    return mobile_apis::Result::UNSUPPORTED_RESOURCE;
  }
  if (IsResultCodeWarning(first, second, third) ||
      IsResultCodeWarning(second, third, first) ||
      IsResultCodeWarning(third, first, second)) {
    return mobile_apis::Result::WARNINGS;
  }
  // If response contains erroneous result code SDL need return erroneous
  // result code.
  auto first_result = hmi_apis::Common_Result::INVALID_ENUM;
  auto second_result = hmi_apis::Common_Result::INVALID_ENUM;
  auto third_result = hmi_apis::Common_Result::INVALID_ENUM;

  if (!first.is_unsupported_resource) {
    first_result = first.result_code;
  }
  if (!second.is_unsupported_resource) {
    second_result = second.result_code;
  }
  if (!third.is_unsupported_resource) {
    third_result = third.result_code;
  }

  hmi_apis::Common_Result::eType intermediate_result =
      std::max(first_result, second_result);
  mobile_apis::Result::eType result_code = MessageHelper::HMIToMobileResult(
      std::max(intermediate_result, third_result));

  return result_code;
}

bool SetGlobalPropertiesRequest::IsResultCodeUnsupported(
    const application_manager::commands::ResponseInfo& first,
    const application_manager::commands::ResponseInfo& second,
    const application_manager::commands::ResponseInfo& third) const {
  const bool first_ok_second_or_third_unsupported =
      (first.is_ok || first.is_not_used) &&
      (second.is_unsupported_resource || third.is_unsupported_resource);
  const bool final_unsupported = first.is_unsupported_resource &&
                                 second.is_unsupported_resource &&
                                 third.is_unsupported_resource;

  return first_ok_second_or_third_unsupported || final_unsupported;
}

std::string SetGlobalPropertiesRequest::MergeInfos(
    const app_mngr::commands::ResponseInfo& first_info,
    const std::string& first_str,
    const app_mngr::commands::ResponseInfo& second_info,
    const std::string& second_str,
    const app_mngr::commands::ResponseInfo& third_info,
    const std::string& third_str) {
  if ((first_info.interface_state == HmiInterfaces::STATE_NOT_AVAILABLE) &&

      ((second_info.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
       !second_str.empty()) &&
      ((third_info.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
       !third_str.empty())) {
    return second_str;
  }

  if ((second_info.interface_state == HmiInterfaces::STATE_NOT_AVAILABLE) &&
      ((first_info.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
       !first_str.empty()) &&
      ((third_info.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
       !third_str.empty())) {
    return first_str;
  }

  if ((third_info.interface_state == HmiInterfaces::STATE_NOT_AVAILABLE) &&
      ((first_info.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
       !first_str.empty()) &&
      ((second_info.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE) &&
       !second_str.empty())) {
    return third_str;
  }

  std::string intermediate_result =
      app_mngr::commands::MergeInfos(first_str, second_str);
  return app_mngr::commands::MergeInfos(intermediate_result, third_str);
}

void SetGlobalPropertiesRequest::PrepareUIRequestVRHelpData(
    const ApplicationSharedPtr app,
    const smart_objects::SmartObject& msg_params,
    smart_objects::SmartObject& out_params) {
  SDL_LOG_AUTO_TRACE();
  DCHECK_OR_RETURN_VOID(app);

  app->set_vr_help_title(msg_params.getElement(strings::vr_help_title));
  app->set_vr_help(msg_params.getElement(strings::vr_help));

  out_params[strings::vr_help_title] = (*app->vr_help_title());
  out_params[strings::vr_help] = (*app->vr_help());
}

void SetGlobalPropertiesRequest::PrepareUIRequestMenuAndKeyboardData(
    const ApplicationSharedPtr app,
    const smart_objects::SmartObject& msg_params,
    smart_objects::SmartObject& out_params) {
  SDL_LOG_AUTO_TRACE();
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
    smart_objects::SmartObject cached_keyboard_props(
        msg_params[hmi_request::keyboard_properties]);

    if (cached_keyboard_props.keyExists(hmi_request::auto_complete_list)) {
      auto auto_complete_list =
          cached_keyboard_props[hmi_request::auto_complete_list].asArray();
      if (auto_complete_list && auto_complete_list->empty()) {
        cached_keyboard_props.erase(hmi_request::auto_complete_list);
      }
    }

    auto saved_keyboard_props = app->keyboard_props();
    if (!saved_keyboard_props) {
      app->set_keyboard_props(cached_keyboard_props);
      return;
    }

    if (!msg_params[hmi_request::keyboard_properties].keyExists(
            hmi_request::keyboard_layout) &&
        saved_keyboard_props->keyExists(hmi_request::keyboard_layout)) {
      cached_keyboard_props[hmi_request::keyboard_layout] =
          static_cast<hmi_apis::Common_KeyboardLayout::eType>(
              (*saved_keyboard_props)[hmi_request::keyboard_layout].asInt());
    }

    if (!msg_params[hmi_request::keyboard_properties].keyExists(
            hmi_response::language) &&
        saved_keyboard_props->keyExists(hmi_response::language)) {
      cached_keyboard_props[hmi_response::language] =
          static_cast<hmi_apis::Common_Language::eType>(
              (*saved_keyboard_props)[hmi_response::language].asInt());
    }

    if (!msg_params[hmi_request::keyboard_properties].keyExists(
            hmi_request::auto_complete_list) &&
        saved_keyboard_props->keyExists(hmi_request::auto_complete_list)) {
      cached_keyboard_props[hmi_request::auto_complete_list] =
          (*saved_keyboard_props)[hmi_request::auto_complete_list];
    }

    if (!msg_params[hmi_request::keyboard_properties].keyExists(
            hmi_request::mask_input_characters) &&
        saved_keyboard_props->keyExists(hmi_request::mask_input_characters)) {
      cached_keyboard_props[hmi_request::mask_input_characters] =
          (*saved_keyboard_props)[hmi_request::mask_input_characters];
    }
    app->set_keyboard_props(cached_keyboard_props);
  }
}

void SetGlobalPropertiesRequest::SendTTSRequest(
    const smart_objects::SmartObject& params, bool use_events) {
  SDL_LOG_AUTO_TRACE();
  is_tts_send_ = true;
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_TTS);
  SendHMIRequest(
      hmi_apis::FunctionID::TTS_SetGlobalProperties, &params, use_events);
}

void SetGlobalPropertiesRequest::SendUIRequest(
    const smart_objects::SmartObject& params, bool use_events) {
  SDL_LOG_AUTO_TRACE();
  is_ui_send_ = true;
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(
      hmi_apis::FunctionID::UI_SetGlobalProperties, &params, use_events);
}

void SetGlobalPropertiesRequest::SendRCRequest(
    const ns_smart_device_link::ns_smart_objects::SmartObject& params,
    bool use_events) {
  SDL_LOG_AUTO_TRACE();
  is_rc_send_ = true;
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_RC);
  SendHMIRequest(
      hmi_apis::FunctionID::RC_SetGlobalProperties, &params, use_events);
}

bool SetGlobalPropertiesRequest::IsPendingResponseExist() {
  return is_ui_send_ != is_ui_received_ || is_tts_send_ != is_tts_received_ ||
         is_rc_send_ != is_rc_received_;
}

bool SetGlobalPropertiesRequest::ValidateConditionalMandatoryParameters(
    const smart_objects::SmartObject& params) {
  SDL_LOG_AUTO_TRACE();
  return params.keyExists(strings::help_prompt) ||
         params.keyExists(strings::timeout_prompt) ||
         params.keyExists(strings::vr_help_title) ||
         params.keyExists(strings::vr_help) ||
         params.keyExists(strings::menu_title) ||
         params.keyExists(strings::menu_icon) ||
         params.keyExists(strings::keyboard_properties) ||
         params.keyExists(strings::menu_layout) ||
         params.keyExists(strings::user_location);
}

bool SetGlobalPropertiesRequest::IsWhiteSpaceExist() {
  SDL_LOG_AUTO_TRACE();
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
        SDL_LOG_ERROR("Invalid help_prompt syntax check failed");
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
        SDL_LOG_ERROR("Invalid timeout_prompt syntax check failed");
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
        SDL_LOG_ERROR("Invalid vr_help text syntax check failed");
        return true;
      }

      if ((*it_vh).keyExists(strings::image)) {
        str = (*it_vh)[strings::image][strings::value].asCharArray();
        if (!CheckSyntax(str)) {
          SDL_LOG_ERROR("Invalid vr_help image value syntax check failed");
          return true;
        }
      }  // if image exists
    }    // for - vh_array iteration
  }

  if (msg_params.keyExists(strings::menu_icon)) {
    str = msg_params[strings::menu_icon][strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid menu_icon value syntax check failed");
      return true;
    }
  }

  if (msg_params.keyExists(strings::vr_help_title)) {
    str = msg_params[strings::vr_help_title].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid vr_help_title value syntax check failed");
      return true;
    }
  }

  if (msg_params.keyExists(strings::menu_title)) {
    str = msg_params[strings::menu_title].asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid menu_title value syntax check failed");
      return true;
    }
  }

  if (msg_params.keyExists(strings::keyboard_properties)) {
    if (msg_params[strings::keyboard_properties].keyExists(
            strings::limited_character_list)) {
      const smart_objects::SmartArray* lcl_array =
          msg_params[strings::keyboard_properties]
                    [strings::limited_character_list]
                        .asArray();

      smart_objects::SmartArray::const_iterator it_lcl = lcl_array->begin();
      smart_objects::SmartArray::const_iterator it_lcl_end = lcl_array->end();

      for (; it_lcl != it_lcl_end; ++it_lcl) {
        str = (*it_lcl).asCharArray();
        if (!CheckSyntax(str)) {
          SDL_LOG_ERROR(
              "Invalid keyboard_properties "
              "limited_character_list syntax check failed");
          return true;
        }
      }
    }

    if (msg_params[strings::keyboard_properties].keyExists(
            strings::auto_complete_list)) {
      const smart_objects::SmartArray* acl_array =
          msg_params[strings::keyboard_properties][strings::auto_complete_list]
              .asArray();

      smart_objects::SmartArray::const_iterator it = acl_array->begin();

      for (; it != acl_array->end(); ++it) {
        str = it->asCharArray();
        if (!CheckSyntax(str)) {
          SDL_LOG_ERROR(
              "Invalid keyboard_properties "
              "auto_complete_list syntax check failed");
          return true;
        }
      }
    }

    if (msg_params[strings::keyboard_properties].keyExists(
            hmi_request::custom_keys)) {
      const smart_objects::SmartArray* custom_keys_array =
          msg_params[strings::keyboard_properties][hmi_request::custom_keys]
              .asArray();

      for (auto keys : (*custom_keys_array)) {
        if (!CheckSyntax(keys.asCharArray())) {
          SDL_LOG_ERROR(
              "Invalid keyboard_properties "
              "custom_keys syntax check failed");
          return true;
        }
      }
    }
  }

  return false;
}

hmi_apis::Common_KeyboardLayout::eType
SetGlobalPropertiesRequest::GetKeyboardLayout() const {
  SDL_LOG_AUTO_TRACE();

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];
  if (msg_params[strings::keyboard_properties].keyExists(
          hmi_request::keyboard_layout)) {
    return static_cast<hmi_apis::Common_KeyboardLayout::eType>(
        msg_params[strings::keyboard_properties][hmi_request::keyboard_layout]
            .asInt());
  }

  ApplicationSharedPtr app = application_manager_.application(connection_key());
  auto saved_keyboard_props = app->keyboard_props();
  if (saved_keyboard_props) {
    if (saved_keyboard_props->keyExists(hmi_request::keyboard_layout)) {
      return static_cast<hmi_apis::Common_KeyboardLayout::eType>(
          (*saved_keyboard_props)[hmi_request::keyboard_layout].asInt());
    }
  }

  return hmi_apis::Common_KeyboardLayout::QWERTY;
}

uint32_t SetGlobalPropertiesRequest::GetAllowedNumberOfConfigurableKeys()
    const {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());
  auto display_capabilities = app->display_capabilities();
  if (!display_capabilities) {
    SDL_LOG_WARN("Display capabilities are not available");
    return 0;
  }

  auto* window_capabilities =
      (*display_capabilities)[0][strings::window_capabilities].asArray();

  if (!window_capabilities) {
    SDL_LOG_WARN("Window capabilities are not available");
    return 0;
  }

  if (!(*window_capabilities)[0].keyExists(
          hmi_response::keyboard_capabilities)) {
    SDL_LOG_WARN("Keyboard capabilities are not available");
    return 0;
  }

  if (!(*window_capabilities)[0][hmi_response::keyboard_capabilities].keyExists(
          hmi_response::supported_keyboards)) {
    SDL_LOG_WARN("Data about supported keyboards is not available");
    return 0;
  }

  auto supported_keyboards =
      (*window_capabilities)[0][hmi_response::keyboard_capabilities]
                            [hmi_response::supported_keyboards]
                                .asArray();

  const auto requested_layout = GetKeyboardLayout();
  for (auto keyboard : (*supported_keyboards)) {
    if (requested_layout ==
        static_cast<hmi_apis::Common_KeyboardLayout::eType>(
            keyboard[hmi_request::keyboard_layout].asInt())) {
      return keyboard[hmi_response::num_configurable_keys].asUInt();
    }
  }

  return 0;
}

bool SetGlobalPropertiesRequest::ValidateCustomKeys() const {
  SDL_LOG_AUTO_TRACE();

  const smart_objects::SmartObject& msg_params =
      (*message_)[strings::msg_params];

  if (!msg_params.keyExists(strings::keyboard_properties)) {
    SDL_LOG_WARN("Keyboard properties are not available");
    return true;
  }

  if (!msg_params[strings::keyboard_properties].keyExists(
          hmi_request::custom_keys)) {
    SDL_LOG_WARN("Customizable keys are not available");
    return true;
  }

  auto custom_keys_array =
      msg_params[strings::keyboard_properties][hmi_request::custom_keys]
          .asArray();
  if (custom_keys_array) {
    uint32_t requested_key_count = custom_keys_array->size();
    uint32_t allowed_key_count = GetAllowedNumberOfConfigurableKeys();

    if (requested_key_count > allowed_key_count) {
      return false;
    }
  }

  return true;
}
}  // namespace commands
}  // namespace sdl_rpc_plugin

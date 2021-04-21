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
#include "sdl_rpc_plugin/commands/mobile/show_request.h"
#include <string.h>

#include "application_manager/application.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler.h"
#include "utils/file_system.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

ShowRequest::ShowRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler)
    , core_result_code_(mobile_apis::Result::INVALID_ENUM)
    , current_window_id_(mobile_apis::PredefinedWindows::DEFAULT_WINDOW)
    , template_config_(smart_objects::SmartType::SmartType_Null)
    , layout_change_required_(false)
    , dcs_change_required_(false)
    , ncs_change_required_(false) {}

ShowRequest::~ShowRequest() {}

void ShowRequest::HandleMetadata(const char* field_id,
                                 int32_t field_index,
                                 smart_objects::SmartObject& msg_params) {
  smart_objects::SmartObject& metadata_tags =
      (*message_)[strings::msg_params][strings::metadata_tags];

  if (metadata_tags.keyExists(field_id)) {
    if (field_index != -1) {
      msg_params[hmi_request::show_strings][field_index]
                [hmi_request::field_types] =
                    smart_objects::SmartObject(smart_objects::SmartType_Array);

      const size_t num_tags = metadata_tags[field_id].length();
      for (size_t i = 0; i < num_tags; ++i) {
        const int32_t current_tag = metadata_tags[field_id][i].asInt();
        msg_params[hmi_request::show_strings][field_index]
                  [hmi_request::field_types][i] = current_tag;
      }
    } else {
      SDL_LOG_INFO("metadata tag provided with no item for "
                   << field_id << ", ignoring with warning");
      // tag provided with no item, ignore with warning
      if (mobile_apis::Result::INVALID_ENUM == core_result_code_) {
        core_result_code_ = mobile_apis::Result::WARNINGS;
        core_response_info_ =
            "Metadata tag was provided for a field with no data.";
      }
    }
  } else {
    SDL_LOG_INFO("No metadata tagging provided for field: " << field_id);
  }
}

bool ShowRequest::CheckTemplateConfigurationForApp(
    application_manager::Application& app) {
  SDL_LOG_AUTO_TRACE();

  if ((*message_)[strings::msg_params].keyExists(strings::window_id)) {
    current_window_id_ =
        (*message_)[strings::msg_params][strings::window_id].asInt();
  }

  const auto set_window_layout = [&app, this]() -> bool {
    const auto new_template_layout =
        template_config_[strings::template_layout].asString();
    const auto old_template_layout = app.window_layout(current_window_id_);
    SDL_LOG_DEBUG("New layout: " << new_template_layout);
    SDL_LOG_DEBUG("Old layout: " << old_template_layout);

    const bool layouts_equal = (new_template_layout == old_template_layout);

    if (!new_template_layout.empty() && !layouts_equal) {
      // Template switched, hence allow any color change
      SDL_LOG_DEBUG("Show Request: Setting new Layout: " << new_template_layout
                                                         << " for window ID: "
                                                         << current_window_id_);
      layout_change_required_ = true;
      return true;
    }
    SDL_LOG_DEBUG("Show Request: No Layout Change");
    return false;
  };

  const auto set_day_color_scheme = [&app, this]() -> bool {
    if (!template_config_.keyExists(strings::day_color_scheme)) {
      return false;
    }
    if (app.day_color_scheme(current_window_id_).getType() !=
            smart_objects::SmartType_Null &&
        template_config_[strings::day_color_scheme] !=
            app.day_color_scheme(current_window_id_)) {
      // Color scheme param exists and has been previously set,
      // hence do not allow color change
      SDL_LOG_DEBUG("Day Color Scheme change is rejected");
      return false;
    }
    SDL_LOG_DEBUG("Day Color Scheme change is allowed");
    dcs_change_required_ = true;

    return true;
  };

  const auto set_night_color_scheme = [&app, this]() -> bool {
    if (!template_config_.keyExists(strings::night_color_scheme)) {
      return false;
    }
    if (app.night_color_scheme(current_window_id_).getType() !=
            smart_objects::SmartType_Null &&
        template_config_[strings::night_color_scheme] !=
            app.night_color_scheme(current_window_id_)) {
      // Color scheme param exists and has been previously set,
      // hence do not allow color change
      SDL_LOG_DEBUG("Night Color Scheme change is rejected");
      return false;
    }
    SDL_LOG_DEBUG("Night Color Scheme Change is allowed");
    ncs_change_required_ = true;

    return true;
  };

  const bool set_layout_result = set_window_layout();

  if (set_layout_result) {
    set_day_color_scheme();
    set_night_color_scheme();
    return true;
  }

  if (!template_config_.keyExists(strings::night_color_scheme) &&
      !template_config_.keyExists(strings::day_color_scheme)) {
    // In case current layout was not changed and day and night color
    // schemes are absent in mobile message SDL has to forward message
    // to HMI with the only layout even it was not changed
    return true;
  }

  const bool set_schemes_result =
      (set_day_color_scheme() && set_night_color_scheme());

  return set_schemes_result;
}

void ShowRequest::ApplyTemplateConfigurationForApp(
    mobile_apis::Result::eType result, application_manager::Application& app) {
  SDL_LOG_AUTO_TRACE();
  if (helpers::Compare<mobile_apis::Result::eType, helpers::EQ, helpers::ONE>(
          result,
          mobile_apis::Result::SUCCESS,
          mobile_apis::Result::WARNINGS)) {
    if (layout_change_required_) {
      const std::string new_layout =
          template_config_[strings::template_layout].asString();
      SDL_LOG_DEBUG("New layout : " << new_layout << " is applied");
      app.set_window_layout(current_window_id_, new_layout);

      if (template_config_.keyExists(strings::day_color_scheme)) {
        app.set_day_color_scheme(current_window_id_,
                                 template_config_[strings::day_color_scheme]);
      }

      if (template_config_.keyExists(strings::night_color_scheme)) {
        app.set_night_color_scheme(
            current_window_id_, template_config_[strings::night_color_scheme]);
      }

      return;
    }

    if (dcs_change_required_) {
      SDL_LOG_DEBUG("New day color scheme is applied");
      app.set_day_color_scheme(current_window_id_,
                               template_config_[strings::day_color_scheme]);
    }

    if (ncs_change_required_) {
      SDL_LOG_DEBUG("New night color scheme is applied");
      app.set_night_color_scheme(current_window_id_,
                                 template_config_[strings::night_color_scheme]);
    }
  }
}

void ShowRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }
  // SDLAQ-CRS-494, VC3.1
  if ((*message_)[strings::msg_params].empty()) {
    SDL_LOG_ERROR(strings::msg_params << " is empty.");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  if (!CheckStringsOfShowRequest()) {
    SDL_LOG_ERROR("Incorrect characters in string");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  // ProcessSoftButtons checks strings on the contents incorrect character

  mobile_apis::Result::eType processing_result = mobile_apis::Result::SUCCESS;
  if (((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) &&
      ((*message_)[strings::msg_params][strings::soft_buttons].length() > 0)) {
    processing_result =
        MessageHelper::ProcessSoftButtons((*message_)[strings::msg_params],
                                          app,
                                          policy_handler_,
                                          application_manager_);
  }

  if (mobile_apis::Result::SUCCESS != processing_result) {
    SDL_LOG_ERROR("Processing of soft buttons failed.");
    SendResponse(false, processing_result);
    return;
  }

  mobile_apis::Result::eType verification_result = mobile_apis::Result::SUCCESS;
  if (((*message_)[strings::msg_params].keyExists(strings::graphic)) &&
      ((*message_)[strings::msg_params][strings::graphic][strings::value]
           .asString())
          .length()) {
    verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::graphic],
        app,
        application_manager_);
    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      SDL_LOG_ERROR("Image verification failed.");
      SendResponse(false, verification_result);
      return;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::secondary_graphic)) {
    verification_result = MessageHelper::VerifyImage(
        (*message_)[strings::msg_params][strings::secondary_graphic],
        app,
        application_manager_);
    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      SDL_LOG_ERROR("Image verification failed.");
      SendResponse(false, verification_result);
      return;
    }
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();

  msg_params[hmi_request::show_strings] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);

  int32_t index = 0;
  int32_t main_field_1_index = -1;
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_1)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mainField1);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_1];
    main_field_1_index = index;
    ++index;
  }

  int32_t main_field_2_index = -1;
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_2)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mainField2);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_2];
    main_field_2_index = index;
    ++index;
  }

  int32_t main_field_3_index = -1;
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_3)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mainField3);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_3];
    main_field_3_index = index;
    ++index;
  }

  int32_t main_field_4_index = -1;
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_4)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mainField4);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::main_field_4];
    main_field_4_index = index;
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::metadata_tags)) {
    HandleMetadata(strings::main_field_1, main_field_1_index, msg_params);
    HandleMetadata(strings::main_field_2, main_field_2_index, msg_params);
    HandleMetadata(strings::main_field_3, main_field_3_index, msg_params);
    HandleMetadata(strings::main_field_4, main_field_4_index, msg_params);
  }

  if ((*message_)[strings::msg_params].keyExists(strings::template_title)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::templateTitle);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::template_title];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::media_clock)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mediaClock);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::media_clock];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::media_track)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::mediaTrack);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::media_track];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::status_bar)) {
    msg_params[hmi_request::show_strings][index][hmi_request::field_name] =
        static_cast<int32_t>(hmi_apis::Common_TextFieldName::statusBar);
    msg_params[hmi_request::show_strings][index][hmi_request::field_text] =
        (*message_)[strings::msg_params][strings::status_bar];
    ++index;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::alignment)) {
    msg_params[strings::alignment] =
        (*message_)[strings::msg_params][strings::alignment];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::graphic)) {
    msg_params[strings::graphic] =
        (*message_)[strings::msg_params][strings::graphic];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::secondary_graphic)) {
    msg_params[strings::secondary_graphic] =
        (*message_)[strings::msg_params][strings::secondary_graphic];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::soft_buttons)) {
    msg_params[strings::soft_buttons] =
        (*message_)[strings::msg_params][strings::soft_buttons];
    if ((*message_)[strings::msg_params][strings::soft_buttons].length() == 0) {
      app->UnsubscribeFromSoftButtons(function_id());
    } else {
      MessageHelper::SubscribeApplicationToSoftButton(
          (*message_)[strings::msg_params], app, function_id(), window_id());
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::custom_presets)) {
    msg_params[strings::custom_presets] =
        (*message_)[strings::msg_params][strings::custom_presets];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::window_id)) {
    const auto window_id =
        (*message_)[strings::msg_params][strings::window_id].asInt();
    if (!app->WindowIdExists(window_id)) {
      SDL_LOG_ERROR("Window with id #" << window_id << " does not exist");
      SendResponse(false, mobile_apis::Result::INVALID_ID);
      return;
    }
    msg_params[strings::window_id] = window_id;
  }

  if ((*message_)[strings::msg_params].keyExists(
          strings::template_configuration)) {
    template_config_ =
        (*message_)[strings::msg_params][strings::template_configuration];
    const bool result = CheckTemplateConfigurationForApp(*app);
    if (!result) {
      const char* info(
          "Color schemes can not be changed without a new template set");
      SendResponse(false, mobile_apis::Result::REJECTED, info);
      return;
    }
    msg_params[strings::template_configuration] =
        (*message_)[strings::msg_params][strings::template_configuration];
  }

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(hmi_apis::FunctionID::UI_Show, &msg_params, true);

  app_mngr::commands::MessageSharedPtr persistentData =
      std::make_shared<smart_objects::SmartObject>(msg_params);
  app->set_show_command(*persistentData);
}

void ShowRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();
  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_Show: {
      SDL_LOG_DEBUG("Received UI_Show event.");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      std::string response_info;
      hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_UI);
      GetInfo(message, response_info);
      if (hmi_apis::Common_Result::WARNINGS == result_code &&
          message[strings::params].keyExists(hmi_response::message)) {
        response_info =
            message[strings::params][hmi_response::message].asString();
      }
      mobile_apis::Result::eType converted_result_code =
          MessageHelper::HMIToMobileResult(result_code);

      ApplyTemplateConfigurationForApp(converted_result_code, *app);

      if (mobile_apis::Result::SUCCESS == converted_result_code &&
          mobile_apis::Result::INVALID_ENUM != core_result_code_) {
        converted_result_code = core_result_code_;
        response_info = core_response_info_;
      }
      SendResponse(result,
                   converted_result_code,
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      break;
    }
  }
}

bool ShowRequest::CheckStringsOfShowRequest() {
  SDL_LOG_AUTO_TRACE();
  const char* str;

  if ((*message_)[strings::msg_params].keyExists(strings::main_field_4)) {
    str = (*message_)[strings::msg_params][strings::main_field_4].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid main_field_4 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_3)) {
    str = (*message_)[strings::msg_params][strings::main_field_3].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid main_field_3 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_2)) {
    str = (*message_)[strings::msg_params][strings::main_field_2].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid main_field_2 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::main_field_1)) {
    str = (*message_)[strings::msg_params][strings::main_field_1].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid main_field_1 syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::template_title)) {
    str =
        (*message_)[strings::msg_params][strings::template_title].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid templateTitle syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::status_bar)) {
    str = (*message_)[strings::msg_params][strings::status_bar].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid status_bar syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::media_clock)) {
    str = (*message_)[strings::msg_params][strings::media_clock].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid media_clock syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::media_track)) {
    str = (*message_)[strings::msg_params][strings::media_track].asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid media_track syntax check failed");
      return false;
    }
  }
  if ((*message_)[strings::msg_params].keyExists(strings::custom_presets)) {
    smart_objects::SmartObject& custom_presets_array =
        (*message_)[strings::msg_params][strings::custom_presets];
    for (size_t i = 0; i < custom_presets_array.length(); ++i) {
      str = custom_presets_array[i].asCharArray();
      if (!CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid custom_presets syntax check failed");
        return false;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::graphic)) {
    str = (*message_)[strings::msg_params][strings::graphic][strings::value]
              .asCharArray();
    if (strlen(str) && !CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid graphic value syntax check failed");
      return false;
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::secondary_graphic)) {
    str = (*message_)[strings::msg_params][strings::secondary_graphic]
                     [strings::value]
                         .asCharArray();
    if (!CheckSyntax(str)) {
      SDL_LOG_ERROR("Invalid secondary_graphic value syntax check failed");
      return false;
    }
  }
  return true;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

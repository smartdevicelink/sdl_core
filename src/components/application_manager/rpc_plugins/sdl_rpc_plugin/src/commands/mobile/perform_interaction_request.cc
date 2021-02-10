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

#include "sdl_rpc_plugin/commands/mobile/perform_interaction_request.h"

#include <string.h>

#include <numeric>
#include <string>

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "utils/custom_string.h"
#include "utils/file_system.h"
#include "utils/gen_hash.h"
#include "utils/helpers.h"

namespace {
const std::int32_t INVALID_CHOICE_ID = -1;
}

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

namespace custom_str = utils::custom_string;

uint32_t PerformInteractionRequest::pi_requests_count_ = 0;

PerformInteractionRequest::PerformInteractionRequest(
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
    , interaction_mode_(mobile_apis::InteractionMode::INVALID_ENUM)
    , ui_choice_id_received_(INVALID_CHOICE_ID)
    , vr_choice_id_received_(INVALID_CHOICE_ID)
    , ui_response_received_(false)
    , vr_response_received_(false)
    , app_pi_was_active_before_(false)
    , vr_result_code_(hmi_apis::Common_Result::INVALID_ENUM)
    , ui_result_code_(hmi_apis::Common_Result::INVALID_ENUM) {
  response_msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  subscribe_on_event(hmi_apis::FunctionID::UI_OnResetTimeout);
  subscribe_on_event(hmi_apis::FunctionID::VR_OnCommand);
  subscribe_on_event(hmi_apis::FunctionID::Buttons_OnButtonPress);
}

PerformInteractionRequest::~PerformInteractionRequest() {}

bool PerformInteractionRequest::Init() {
  /* Timeout in milliseconds.
     If omitted a standard value of 10000 milliseconds is used.*/
  const auto& msg_params = (*message_)[strings::msg_params];
  uint32_t request_timeout = msg_params[strings::timeout].asUInt();

  interaction_mode_ = static_cast<mobile_apis::InteractionMode::eType>(
      msg_params[strings::interaction_mode].asInt());

  if (mobile_apis::InteractionMode::BOTH == interaction_mode_ ||
      mobile_apis::InteractionMode::MANUAL_ONLY == interaction_mode_) {
    const uint32_t increase_value = 2;
    default_timeout_ += request_timeout * increase_value;
  } else {
    default_timeout_ += request_timeout;
  }
  return true;
}

void PerformInteractionRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (app->is_perform_interaction_active()) {
    SDL_LOG_DEBUG("Application has active PerformInteraction");
    app_pi_was_active_before_ = true;
  }

  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];
  mobile_apis::LayoutMode::eType interaction_layout =
      mobile_apis::LayoutMode::INVALID_ENUM;

  if (msg_params.keyExists(hmi_request::interaction_layout)) {
    interaction_layout = static_cast<mobile_apis::LayoutMode::eType>(
        msg_params[hmi_request::interaction_layout].asInt());
  }

  if ((mobile_apis::InteractionMode::VR_ONLY == interaction_mode_) &&
      (mobile_apis::LayoutMode::KEYBOARD == interaction_layout)) {
    SDL_LOG_ERROR(
        "PerformInteraction contains InteractionMode"
        "=VR_ONLY and interactionLayout=KEYBOARD");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  const size_t choice_set_id_list_length =
      msg_params[strings::interaction_choice_set_id_list].length();

  if (0 == choice_set_id_list_length) {
    if (mobile_apis::LayoutMode::KEYBOARD == interaction_layout) {
      if (mobile_apis::InteractionMode::BOTH == interaction_mode_) {
        SDL_LOG_ERROR(
            "interactionChoiceSetIDList is empty,"
            " InteractionMode=BOTH and"
            " interactionLayout=KEYBOARD");
        SendResponse(false, mobile_apis::Result::INVALID_DATA);
        return;
      }
    } else {
      SDL_LOG_ERROR(
          "interactionChoiceSetIDList is empty"
          " and interactionLayout!=KEYBOARD");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
  }

  if (!CheckChoiceIDFromRequest(
          app,
          choice_set_id_list_length,
          msg_params[strings::interaction_choice_set_id_list])) {
    SDL_LOG_ERROR(
        "PerformInteraction has choice sets with "
        "duplicated IDs or application does not have choice sets");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  if (msg_params.keyExists(strings::vr_help)) {
    if (mobile_apis::Result::INVALID_DATA ==
        MessageHelper::VerifyImageVrHelpItems(
            msg_params[strings::vr_help], app, application_manager_)) {
      SDL_LOG_ERROR("Verification of " << strings::vr_help << " failed.");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
  }

  if (IsWhiteSpaceExist()) {
    SDL_LOG_ERROR("Incoming perform interaction has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  switch (interaction_mode_) {
    case mobile_apis::InteractionMode::BOTH: {
      SDL_LOG_DEBUG("Interaction Mode: BOTH");
      if (!CheckChoiceSetVRSynonyms(app) || !CheckVrHelpItemPositions(app) ||
          !CheckChoiceSetListVRCommands(app)) {
        return;
      }
      break;
    }
    case mobile_apis::InteractionMode::MANUAL_ONLY: {
      SDL_LOG_DEBUG("Interaction Mode: MANUAL_ONLY");
      if (!CheckChoiceSetVRSynonyms(app) || !CheckVrHelpItemPositions(app)) {
        return;
      }
      break;
    }
    case mobile_apis::InteractionMode::VR_ONLY: {
      SDL_LOG_DEBUG("Interaction Mode: VR_ONLY");
      if (!CheckChoiceSetVRSynonyms(app) || !CheckVrHelpItemPositions(app) ||
          !CheckChoiceSetListVRCommands(app)) {
        return;
      }
      break;
    }
    default: {
      SDL_LOG_ERROR("Unknown interaction mode");
      return;
    }
  }

  app->set_perform_interaction_mode(static_cast<int32_t>(interaction_mode_));
  app->set_perform_interaction_active(true);
  app->set_perform_interaction_layout(interaction_layout);
  // increment amount of active requests
  ++pi_requests_count_;
  SendVRPerformInteractionRequest(app);
  SendUIPerformInteractionRequest(app);
}

void PerformInteractionRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_OnResetTimeout: {
      SDL_LOG_DEBUG("Received UI_OnResetTimeout event");
      application_manager_.updateRequestTimeout(
          connection_key(), correlation_id(), default_timeout());
      break;
    }
    case hmi_apis::FunctionID::UI_PerformInteraction: {
      SDL_LOG_DEBUG("Received UI_PerformInteraction event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      ui_response_received_ = true;

      unsubscribe_from_event(hmi_apis::FunctionID::UI_PerformInteraction);
      ui_result_code_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asUInt());
      GetInfo(message, ui_info_);
      ProcessUIResponse(event.smart_object());
      break;
    }
    case hmi_apis::FunctionID::VR_PerformInteraction: {
      SDL_LOG_DEBUG("Received VR_PerformInteraction");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VR);
      vr_response_received_ = true;

      unsubscribe_from_event(hmi_apis::FunctionID::VR_PerformInteraction);
      vr_result_code_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asUInt());
      GetInfo(message, vr_info_);
      if (ProcessVRResponse(event.smart_object())) {
        return;
      }
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event" << event.id());
      break;
    }
  }

  if (!HasHMIResponsesToWait()) {
    SDL_LOG_DEBUG("Send response in interaction mode "
                  << static_cast<int32_t>(interaction_mode_));
    if (SetChoiceIdToResponseMsgParams(response_msg_params)) {
      SendBothModeResponse(response_msg_params);
    } else {
      DisablePerformInteraction();
      SendResponse(false,
                   mobile_apis::Result::GENERIC_ERROR,
                   "Received two different choice IDs");
    }
  }
}

void PerformInteractionRequest::onTimeOut() {
  SDL_LOG_AUTO_TRACE();

  switch (interaction_mode_) {
    case mobile_apis::InteractionMode::BOTH: {
      SDL_LOG_DEBUG("Interaction Mode: BOTH");
      if (true == vr_response_received_) {
        unsubscribe_from_event(hmi_apis::FunctionID::UI_PerformInteraction);
        DisablePerformInteraction();
        CommandRequestImpl::onTimeOut();
      } else {
        application_manager_.updateRequestTimeout(
            connection_key(), correlation_id(), default_timeout_);
      }
      break;
    }
    case mobile_apis::InteractionMode::VR_ONLY: {
      SDL_LOG_DEBUG("Interaction Mode: VR_ONLY");
      unsubscribe_from_event(hmi_apis::FunctionID::UI_PerformInteraction);
      DisablePerformInteraction();
      CommandRequestImpl::onTimeOut();
      break;
    }
    case mobile_apis::InteractionMode::MANUAL_ONLY: {
      SDL_LOG_DEBUG("InteractionMode: MANUAL_ONLY");
      unsubscribe_from_event(hmi_apis::FunctionID::UI_PerformInteraction);
      DisablePerformInteraction();
      CommandRequestImpl::onTimeOut();
      break;
    }
    default: {
      SDL_LOG_ERROR("INVALID ENUM");
      return;
    }
  };
}

bool PerformInteractionRequest::ProcessVRResponse(
    const smart_objects::SmartObject& message) {
  SDL_LOG_AUTO_TRACE();
  using namespace hmi_apis;
  using namespace mobile_apis;
  using namespace smart_objects;
  using namespace helpers;

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("NULL pointer");
    return false;
  }

  const bool is_vr_aborted_timeout = Compare<Common_Result::eType, EQ, ONE>(
      vr_result_code_, Common_Result::ABORTED, Common_Result::TIMED_OUT);

  if (is_vr_aborted_timeout) {
    SDL_LOG_DEBUG("VR response aborted");
    if (InteractionMode::VR_ONLY == interaction_mode_) {
      SDL_LOG_DEBUG("Aborted or Timeout Send Close Popup");
      TerminatePerformInteraction();
      SendResponse(false, MessageHelper::HMIToMobileResult(vr_result_code_));
      return true;
    }
    SDL_LOG_DEBUG("Update timeout for UI");
    application_manager_.updateRequestTimeout(
        connection_key(), correlation_id(), default_timeout_);
    return false;
  }

  if (!ui_response_received_ &&
      InteractionMode::MANUAL_ONLY != interaction_mode_) {
    SendClosePopupRequestToHMI();
  }

  const SmartObject& hmi_msg_params = message[strings::msg_params];
  if (hmi_msg_params.keyExists(strings::choice_id)) {
    const int choice_id = hmi_msg_params[strings::choice_id].asInt();
    if (!CheckChoiceIDFromResponse(app, choice_id)) {
      SDL_LOG_ERROR("Wrong choiceID was received from HMI");
      TerminatePerformInteraction();
      SendResponse(
          false, Result::GENERIC_ERROR, "Wrong choiceID was received from HMI");
      return true;
    }
    vr_choice_id_received_ = choice_id;
  }

  if (mobile_apis::InteractionMode::BOTH == interaction_mode_ ||
      mobile_apis::InteractionMode::MANUAL_ONLY == interaction_mode_) {
    SDL_LOG_DEBUG("Update timeout for UI");
    application_manager_.updateRequestTimeout(
        connection_key(), correlation_id(), default_timeout_);
  }

  const bool is_vr_result_success = Compare<Common_Result::eType, EQ, ONE>(
      vr_result_code_, Common_Result::SUCCESS, Common_Result::WARNINGS);

  if (is_vr_result_success &&
      InteractionMode::MANUAL_ONLY == interaction_mode_) {
    SDL_LOG_DEBUG("VR response is successfull in MANUAL_ONLY mode "
                  << "Wait for UI response");
    // in case MANUAL_ONLY mode VR.PI SUCCESS just return
    return false;
  }

  return false;
}

void PerformInteractionRequest::ProcessUIResponse(
    const smart_objects::SmartObject& message) {
  SDL_LOG_AUTO_TRACE();
  using namespace helpers;
  using namespace smart_objects;

  ApplicationSharedPtr app = application_manager_.application(connection_key());
  if (!app) {
    SDL_LOG_ERROR("NULL pointer");
    return;
  }

  HmiInterfaces::InterfaceState ui_interface_state =
      application_manager_.hmi_interfaces().GetInterfaceState(
          HmiInterfaces::HMI_INTERFACE_UI);

  bool result = Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
      ui_result_code_,
      hmi_apis::Common_Result::SUCCESS,
      hmi_apis::Common_Result::WARNINGS);

  result = result ||
           (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE == ui_result_code_ &&
            HmiInterfaces::STATE_NOT_AVAILABLE != ui_interface_state);

  const bool is_pi_warning = Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
      ui_result_code_, hmi_apis::Common_Result::WARNINGS);

  const bool is_pi_unsupported =
      Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
          ui_result_code_, hmi_apis::Common_Result::UNSUPPORTED_RESOURCE);

  if (result) {
    const smart_objects::SmartObject& hmi_msg_params =
        message[strings::msg_params];
    if (is_pi_unsupported) {
      ui_result_code_ = hmi_apis::Common_Result::UNSUPPORTED_RESOURCE;
      ui_info_ = message[strings::msg_params][strings::info].asString();
    } else if (is_pi_warning) {
      ui_result_code_ = hmi_apis::Common_Result::WARNINGS;
      ui_info_ = message[strings::msg_params][strings::info].asString();
    }

    // result code must be GENERIC_ERROR in case wrong choice_id
    if (hmi_msg_params.keyExists(strings::choice_id)) {
      const std::int32_t ui_choice_id =
          static_cast<std::int32_t>(hmi_msg_params[strings::choice_id].asInt());

      if (!CheckChoiceIDFromResponse(app, ui_choice_id)) {
        ui_result_code_ = hmi_apis::Common_Result::GENERIC_ERROR;
        ui_info_ = "Wrong choiceID was received from HMI";
      } else {
        ui_choice_id_received_ = ui_choice_id;
      }
    } else if (hmi_msg_params.keyExists(strings::manual_text_entry)) {
      ui_text_entry_received_ =
          hmi_msg_params[strings::manual_text_entry].asString();
    }
  }
}

void PerformInteractionRequest::SendUIPerformInteractionRequest(
    application_manager::ApplicationSharedPtr const app) {
  SDL_LOG_AUTO_TRACE();
  smart_objects::SmartObject& choice_set_id_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  if ((*message_)[strings::msg_params].keyExists(strings::cancel_id)) {
    msg_params[strings::cancel_id] =
        (*message_)[strings::msg_params][strings::cancel_id].asInt();
  }

  mobile_apis::InteractionMode::eType mode =
      static_cast<mobile_apis::InteractionMode::eType>(
          (*message_)[strings::msg_params][strings::interaction_mode].asInt());

  if (mobile_apis::InteractionMode::VR_ONLY != mode) {
    msg_params[hmi_request::initial_text][hmi_request::field_name] =
        static_cast<int32_t>(
            hmi_apis::Common_TextFieldName::initialInteractionText);
    msg_params[hmi_request::initial_text][hmi_request::field_text] =
        (*message_)[strings::msg_params][hmi_request::initial_text];
  }
  bool is_vr_help_item = false;
  if (mobile_apis::InteractionMode::MANUAL_ONLY != mode) {
    msg_params[strings::vr_help_title] =
        (*message_)[strings::msg_params][strings::initial_text].asString();
    if ((*message_)[strings::msg_params].keyExists(strings::vr_help)) {
      is_vr_help_item = true;
      msg_params[strings::vr_help] =
          (*message_)[strings::msg_params][strings::vr_help];
    }
  }

  msg_params[strings::timeout] =
      (*message_)[strings::msg_params][strings::timeout].asUInt();
  msg_params[strings::app_id] = app->app_id();
  if (mobile_apis::InteractionMode::VR_ONLY != mode) {
    msg_params[strings::choice_set] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
  }
  int32_t index_array_of_vr_help = 0;
  for (size_t i = 0; i < choice_set_id_list.length(); ++i) {
    smart_objects::SmartObject choice_set =
        app->FindChoiceSet(choice_set_id_list[i].asInt());
    if (smart_objects::SmartType_Null != choice_set.getType()) {
      // save perform interaction choice set
      app->AddPerformInteractionChoiceSet(
          correlation_id(), choice_set_id_list[i].asInt(), choice_set);
      for (size_t j = 0; j < choice_set[strings::choice_set].length(); ++j) {
        if (mobile_apis::InteractionMode::VR_ONLY != mode) {
          size_t index = msg_params[strings::choice_set].length();
          msg_params[strings::choice_set][index] =
              choice_set[strings::choice_set][j];
          // vrCommands should be added via VR.AddCommand only
          msg_params[strings::choice_set][index].erase(strings::vr_commands);
        }
        if (mobile_apis::InteractionMode::MANUAL_ONLY != mode &&
            !is_vr_help_item) {
          smart_objects::SmartObject& vr_commands =
              choice_set[strings::choice_set][j][strings::vr_commands];
          if (0 < vr_commands.length()) {
            // copy only first synonym
            smart_objects::SmartObject item(smart_objects::SmartType_Map);
            item[strings::text] = vr_commands[0].asString();
            item[strings::position] = index_array_of_vr_help + 1;
            msg_params[strings::vr_help][index_array_of_vr_help++] = item;
          }
        }
      }
    }
  }
  if ((*message_)[strings::msg_params].keyExists(
          hmi_request::interaction_layout) &&
      mobile_apis::InteractionMode::VR_ONLY != mode) {
    msg_params[hmi_request::interaction_layout] =
        (*message_)[strings::msg_params][hmi_request::interaction_layout]
            .asInt();
  }
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(
      hmi_apis::FunctionID::UI_PerformInteraction, &msg_params, true);
}

void PerformInteractionRequest::SendVRPerformInteractionRequest(
    application_manager::ApplicationSharedPtr const app) {
  SDL_LOG_AUTO_TRACE();

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  if ((*message_)[strings::msg_params].keyExists(strings::cancel_id)) {
    msg_params[strings::cancel_id] =
        (*message_)[strings::msg_params][strings::cancel_id].asInt();
  }

  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  if (mobile_apis::InteractionMode::MANUAL_ONLY != interaction_mode_) {
    msg_params[strings::grammar_id] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);

    int32_t grammar_id_index = 0;
    for (uint32_t i = 0; i < choice_list.length(); ++i) {
      const auto choice_id = choice_list[i].asInt();
      smart_objects::SmartObject choice_set = app->FindChoiceSet(choice_id);
      if (smart_objects::SmartType_Null == choice_set.getType()) {
        SDL_LOG_WARN("Couldn't found choiceset : " << choice_id);
        continue;
      }
      msg_params[strings::grammar_id][grammar_id_index++] =
          choice_set[strings::grammar_id].asUInt();
    }
  }

  std::vector<std::string> invalid_params;
  if ((*message_)[strings::msg_params].keyExists(strings::help_prompt)) {
    smart_objects::SmartObject& help_prompt =
        (*message_)[strings::msg_params][strings::help_prompt];
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyTtsFiles(help_prompt, app, application_manager_);

    if (mobile_apis::Result::FILE_NOT_FOUND == verification_result) {
      SDL_LOG_WARN("MessageHelper::VerifyTtsFiles return "
                   << verification_result);
      invalid_params.push_back("help_prompt");
    } else {
      msg_params[strings::help_prompt] = help_prompt;
    }
  } else {
    if (choice_list.length() != 0) {
      msg_params[strings::help_prompt] =
          smart_objects::SmartObject(smart_objects::SmartType_Array);
    }
    int32_t index = 0;
    for (uint32_t i = 0; i < choice_list.length(); ++i) {
      smart_objects::SmartObject choice_set =
          app->FindChoiceSet(choice_list[i].asInt());

      if (smart_objects::SmartType_Null != choice_set.getType()) {
        for (uint32_t j = 0; j < choice_set[strings::choice_set].length();
             ++j) {
          smart_objects::SmartObject& vr_commands =
              choice_set[strings::choice_set][j][strings::vr_commands];
          if (0 < vr_commands.length()) {
            // copy only first synonym
            smart_objects::SmartObject item(smart_objects::SmartType_Map);
            // Since there is no custom data from application side, SDL should
            // construct prompt and append delimiter to each item
            item[strings::type] = hmi_apis::Common_SpeechCapabilities::SC_TEXT;
            item[strings::text] =
                vr_commands[0].asString() +
                application_manager_.get_settings().tts_delimiter();
            msg_params[strings::help_prompt][index++] = item;
          }
        }
      } else {
        SDL_LOG_ERROR("Can't found choiceSet!");
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::timeout_prompt)) {
    smart_objects::SmartObject& timeout_prompt =
        (*message_)[strings::msg_params][strings::timeout_prompt];
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyTtsFiles(
            timeout_prompt, app, application_manager_);

    if (mobile_apis::Result::FILE_NOT_FOUND == verification_result) {
      SDL_LOG_WARN("MessageHelper::VerifyTtsFiles return "
                   << verification_result);
      invalid_params.push_back("timeout_prompt");
    } else {
      msg_params[strings::timeout_prompt] = timeout_prompt;
    }
  } else {
    if (msg_params.keyExists(strings::help_prompt)) {
      msg_params[strings::timeout_prompt] = msg_params[strings::help_prompt];
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::initial_prompt)) {
    smart_objects::SmartObject& initial_prompt =
        (*message_)[strings::msg_params][strings::initial_prompt];
    mobile_apis::Result::eType verification_result =
        MessageHelper::VerifyTtsFiles(
            initial_prompt, app, application_manager_);

    if (mobile_apis::Result::FILE_NOT_FOUND == verification_result) {
      SDL_LOG_WARN("MessageHelper::VerifyTtsFiles return "
                   << verification_result);
      invalid_params.push_back("initial_prompt");
    } else {
      msg_params[strings::initial_prompt] = initial_prompt;
    }
  }

  if (!invalid_params.empty()) {
    const std::string params_list =
        std::accumulate(std::begin(invalid_params),
                        std::end(invalid_params),
                        std::string(""),
                        [](std::string& first, std::string& second) {
                          return first.empty() ? second : first + ", " + second;
                        });
    const std::string info =
        "One or more files needed for " + params_list + " are not present";
    SendResponse(false, mobile_apis::Result::FILE_NOT_FOUND, info.c_str());
    return;
  }

  msg_params[strings::timeout] =
      (*message_)[strings::msg_params][strings::timeout].asUInt();
  ;
  msg_params[strings::app_id] = app->app_id();
  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_VR);
  SendHMIRequest(
      hmi_apis::FunctionID::VR_PerformInteraction, &msg_params, true);
}

bool PerformInteractionRequest::CheckChoiceSetVRSynonyms(
    application_manager::ApplicationSharedPtr const app) {
  SDL_LOG_AUTO_TRACE();

  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  for (size_t i = 0; i < choice_list.length(); ++i) {
    // choice_set contains SmartObject msg_params
    smart_objects::SmartObject i_choice_set =
        app->FindChoiceSet(choice_list[i].asInt());

    for (size_t j = 0; j < choice_list.length(); ++j) {
      smart_objects::SmartObject j_choice_set =
          app->FindChoiceSet(choice_list[j].asInt());

      if (i == j) {
        // skip check the same element
        continue;
      }

      if ((smart_objects::SmartType_Null == i_choice_set.getType()) ||
          (smart_objects::SmartType_Null == j_choice_set.getType())) {
        SDL_LOG_ERROR("Invalid ID");
        SendResponse(false, mobile_apis::Result::INVALID_ID);
        return false;
      }

      size_t ii = 0;
      size_t jj = 0;
      for (; ii < i_choice_set[strings::choice_set].length(); ++ii) {
        for (; jj < j_choice_set[strings::choice_set].length(); ++jj) {
          if (!(i_choice_set[strings::choice_set][ii].keyExists(
                    strings::vr_commands) &&
                j_choice_set[strings::choice_set][jj].keyExists(
                    strings::vr_commands))) {
            SDL_LOG_DEBUG(
                "One or both sets has missing vr commands, skipping "
                "synonym check");
            return true;
          }
          // choice_set pointer contains SmartObject msg_params
          smart_objects::SmartObject& ii_vr_commands =
              i_choice_set[strings::choice_set][ii][strings::vr_commands];

          smart_objects::SmartObject& jj_vr_commands =
              j_choice_set[strings::choice_set][jj][strings::vr_commands];

          for (size_t iii = 0; iii < ii_vr_commands.length(); ++iii) {
            for (size_t jjj = 0; jjj < jj_vr_commands.length(); ++jjj) {
              const custom_str::CustomString& vr_cmd_i =
                  ii_vr_commands[iii].asCustomString();
              const custom_str::CustomString& vr_cmd_j =
                  jj_vr_commands[jjj].asCustomString();
              if (vr_cmd_i.CompareIgnoreCase(vr_cmd_j)) {
                SDL_LOG_ERROR("Choice set has duplicated VR synonym");
                SendResponse(false,
                             mobile_apis::Result::DUPLICATE_NAME,
                             "Choice set has duplicated VR synonym");
                return false;
              }
            }
          }
        }
      }
    }
  }
  return true;
}

bool PerformInteractionRequest::CheckVrHelpItemPositions(
    application_manager::ApplicationSharedPtr const app) {
  SDL_LOG_AUTO_TRACE();

  if (!(*message_)[strings::msg_params].keyExists(strings::vr_help)) {
    SDL_LOG_DEBUG(strings::vr_help << " is omitted.");
    return true;
  }

  smart_objects::SmartObject& vr_help =
      (*message_)[strings::msg_params][strings::vr_help];

  int32_t position = 1;
  for (size_t i = 0; i < vr_help.length(); ++i) {
    if (position != vr_help[i][strings::position].asInt()) {
      SDL_LOG_ERROR("Non-sequential vrHelp item position");
      SendResponse(false,
                   mobile_apis::Result::REJECTED,
                   "Non-sequential vrHelp item position");
      return false;
    }
    ++position;
  }
  return true;
}

void PerformInteractionRequest::DisablePerformInteraction() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());
  if (!app) {
    SDL_LOG_ERROR("NULL pointer");
    return;
  }

  if (app->is_perform_interaction_active()) {
    // decrease amount of active requests
    --pi_requests_count_;
    if (!pi_requests_count_) {
      app->set_perform_interaction_active(false);
      app->set_perform_interaction_mode(-1);
    }
  }
  app->DeletePerformInteractionChoiceSet(correlation_id());
}

bool PerformInteractionRequest::IsWhiteSpaceExist() {
  SDL_LOG_AUTO_TRACE();

  const char* str =
      (*message_)[strings::msg_params][strings::initial_text].asCharArray();
  if (!CheckSyntax(str)) {
    SDL_LOG_ERROR("Invalid initial_text syntax check failed");
    return true;
  }

  if ((*message_)[strings::msg_params].keyExists(strings::initial_prompt)) {
    const smart_objects::SmartArray* ip_array =
        (*message_)[strings::msg_params][strings::initial_prompt].asArray();

    smart_objects::SmartArray::const_iterator it_ip = ip_array->begin();
    smart_objects::SmartArray::const_iterator it_ip_end = ip_array->end();

    for (; it_ip != it_ip_end; ++it_ip) {
      str = (*it_ip)[strings::text].asCharArray();
      if (strlen(str) && !CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid initial_prompt syntax check failed");
        return true;
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::help_prompt)) {
    const smart_objects::SmartArray* hp_array =
        (*message_)[strings::msg_params][strings::help_prompt].asArray();

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

  if ((*message_)[strings::msg_params].keyExists(strings::timeout_prompt)) {
    const smart_objects::SmartArray* tp_array =
        (*message_)[strings::msg_params][strings::timeout_prompt].asArray();

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

  if ((*message_)[strings::msg_params].keyExists(strings::vr_help)) {
    const smart_objects::SmartArray* vh_array =
        (*message_)[strings::msg_params][strings::vr_help].asArray();

    smart_objects::SmartArray::const_iterator it_vh = vh_array->begin();
    smart_objects::SmartArray::const_iterator it_vh_end = vh_array->end();

    for (; it_vh != it_vh_end; ++it_vh) {
      str = (*it_vh)[strings::text].asCharArray();
      if (!CheckSyntax(str)) {
        SDL_LOG_ERROR("Invalid vr_help syntax check failed");
        return true;
      }

      if ((*it_vh).keyExists(strings::image)) {
        str = (*it_vh)[strings::image][strings::value].asCharArray();
        if (!CheckSyntax(str)) {
          SDL_LOG_ERROR("Invalid vr_help image value syntax check failed");
          return true;
        }
      }
    }
  }
  return false;
}

void PerformInteractionRequest::TerminatePerformInteraction() {
  SDL_LOG_AUTO_TRACE();

  SendClosePopupRequestToHMI();
  DisablePerformInteraction();
}

void PerformInteractionRequest::SendClosePopupRequestToHMI() {
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[hmi_request::method_name] = "UI.PerformInteraction";
  SendHMIRequest(hmi_apis::FunctionID::UI_ClosePopUp, &msg_params);
}

bool PerformInteractionRequest::CheckChoiceIDFromResponse(
    ApplicationSharedPtr app, const int32_t choice_id) {
  SDL_LOG_AUTO_TRACE();
  const DataAccessor<PerformChoiceSetMap> accessor =
      app->performinteraction_choice_set_map();
  const PerformChoiceSetMap& choice_set_map = accessor.GetData();

  PerformChoiceSetMap::const_iterator choice_set_map_it =
      choice_set_map.find(correlation_id());
  if (choice_set_map.end() != choice_set_map_it) {
    const PerformChoice& choice = choice_set_map_it->second;
    PerformChoice::const_iterator it = choice.begin();
    for (; choice.end() != it; ++it) {
      if ((*it->second).keyExists(strings::choice_set)) {
        const smart_objects::SmartObject& choice_set =
            (*it->second).getElement(strings::choice_set);
        for (size_t j = 0; j < choice_set.length(); ++j) {
          if (choice_id ==
              choice_set.getElement(j).getElement(strings::choice_id).asInt()) {
            return true;
          }
        }
      }
    }
  }
  return false;
}

bool PerformInteractionRequest::CheckChoiceSetListVRCommands(
    ApplicationSharedPtr app) {
  SDL_LOG_AUTO_TRACE();

  const smart_objects::SmartObject& choice_set_id_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  for (size_t i = 0; i < choice_set_id_list.length(); ++i) {
    auto choice_set = app->FindChoiceSet(choice_set_id_list[i].asInt());

    // this should never ever happen since this was already checked
    if (smart_objects::SmartType_Null == choice_set.getType()) {
      SDL_LOG_ERROR(
          "Couldn't find choiceset_id = " << choice_set_id_list[i].asInt());
      SendResponse(false, mobile_apis::Result::INVALID_ID);
      return false;
    }

    const smart_objects::SmartObject& choices_list =
        choice_set[strings::choice_set];
    auto vr_status = MessageHelper::CheckChoiceSetVRCommands(choices_list);

    // if not all choices have vr commands
    if (vr_status != MessageHelper::ChoiceSetVRCommandsStatus::ALL) {
      SDL_LOG_ERROR(
          "PerformInteraction has choice sets with "
          "missing vrCommands, not in MANUAL_ONLY mode");
      SendResponse(false,
                   mobile_apis::Result::INVALID_DATA,
                   "Some choices don't contain VR commands.");
      return false;
    }
  }
  return true;
}

bool PerformInteractionRequest::CheckChoiceIDFromRequest(
    ApplicationSharedPtr app,
    const size_t choice_set_id_list_length,
    const smart_objects::SmartObject& choice_set_id_list) const {
  SDL_LOG_AUTO_TRACE();

  std::set<uint32_t> choice_id_set;
  std::pair<std::set<uint32_t>::iterator, bool> ins_res;

  for (size_t i = 0; i < choice_set_id_list_length; ++i) {
    auto choice_set = app->FindChoiceSet(choice_set_id_list[i].asInt());
    if (smart_objects::SmartType_Null == choice_set.getType()) {
      SDL_LOG_ERROR(
          "Couldn't find choiceset_id = " << choice_set_id_list[i].asInt());
      return false;
    }

    size_t choice_list_length = choice_set[strings::choice_set].length();
    const smart_objects::SmartObject& choices_list =
        choice_set[strings::choice_set];
    for (size_t k = 0; k < choice_list_length; ++k) {
      ins_res =
          choice_id_set.insert(choices_list[k][strings::choice_id].asInt());
      if (!ins_res.second) {
        SDL_LOG_ERROR("choice with ID "
                      << choices_list[k][strings::choice_id].asInt()
                      << " already exists");
        return false;
      }
    }
  }
  return true;
}

const bool PerformInteractionRequest::HasHMIResponsesToWait() const {
  SDL_LOG_AUTO_TRACE();
  return !ui_response_received_ || !vr_response_received_;
}

void PerformInteractionRequest::SendBothModeResponse(
    const smart_objects::SmartObject& msg_param) {
  SDL_LOG_AUTO_TRACE();

  app_mngr::commands::ResponseInfo ui_perform_info(
      ui_result_code_, HmiInterfaces::HMI_INTERFACE_UI, application_manager_);
  app_mngr::commands::ResponseInfo vr_perform_info(
      vr_result_code_, HmiInterfaces::HMI_INTERFACE_VR, application_manager_);
  const bool result =
      PrepareResultForMobileResponse(ui_perform_info, vr_perform_info);
  mobile_apis::Result::eType perform_interaction_result_code =
      PrepareResultCodeForResponse(ui_perform_info, vr_perform_info);
  const smart_objects::SmartObject* response_params =
      msg_param.empty() ? NULL : &msg_param;
  std::string info = app_mngr::commands::MergeInfos(
      ui_perform_info, ui_info_, vr_perform_info, vr_info_);

  DisablePerformInteraction();

  SendResponse(result,
               perform_interaction_result_code,
               info.empty() ? NULL : info.c_str(),
               response_params);
}

mobile_apis::Result::eType
PerformInteractionRequest::PrepareResultCodeForResponse(
    const app_mngr::commands::ResponseInfo& ui_response,
    const app_mngr::commands::ResponseInfo& vr_response) {
  SDL_LOG_DEBUG(
      "InteractionMode = " << static_cast<int32_t>(interaction_mode_));

  auto mobile_vr_result_code =
      MessageHelper::HMIToMobileResult(vr_result_code_);
  auto mobile_ui_result_code =
      MessageHelper::HMIToMobileResult(ui_result_code_);

  if (mobile_apis::Result::eType::WARNINGS == mobile_vr_result_code) {
    if (mobile_apis::Result::eType::SUCCESS == mobile_ui_result_code) {
      return mobile_apis::Result::eType::WARNINGS;
    }
  } else if (mobile_apis::Result::eType::WARNINGS == mobile_ui_result_code) {
    if (mobile_apis::Result::eType::SUCCESS == mobile_vr_result_code) {
      return mobile_apis::Result::eType::WARNINGS;
    }
  }

  if (mobile_apis::InteractionMode::VR_ONLY == interaction_mode_) {
    return mobile_vr_result_code;
  }
  if (mobile_apis::InteractionMode::MANUAL_ONLY == interaction_mode_) {
    return mobile_ui_result_code;
  }

  if (INVALID_CHOICE_ID != vr_choice_id_received_) {
    return mobile_vr_result_code;
  }
  if (INVALID_CHOICE_ID != ui_choice_id_received_ ||
      !ui_text_entry_received_.empty()) {
    return mobile_ui_result_code;
  }

  return CommandRequestImpl::PrepareResultCodeForResponse(ui_response,
                                                          vr_response);
}

bool PerformInteractionRequest::PrepareResultForMobileResponse(
    app_mngr::commands::ResponseInfo& ui_response,
    app_mngr::commands::ResponseInfo& vr_response) const {
  bool vr_choice_received = INVALID_CHOICE_ID != vr_choice_id_received_;
  bool ui_choice_received = INVALID_CHOICE_ID != ui_choice_id_received_ ||
                            !ui_text_entry_received_.empty();

  bool vr_response_success =
      vr_response.is_ok ||
      (vr_response.is_unsupported_resource &&
       vr_response.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE &&
       vr_choice_received);
  bool ui_response_success =
      ui_response.is_ok ||
      (ui_response.is_unsupported_resource &&
       ui_response.interface_state != HmiInterfaces::STATE_NOT_AVAILABLE &&
       ui_choice_received);

  if (mobile_apis::InteractionMode::VR_ONLY == interaction_mode_) {
    return vr_response_success;
  }
  if (mobile_apis::InteractionMode::MANUAL_ONLY == interaction_mode_) {
    return ui_response_success;
  }

  return (vr_response_success || ui_response_success);
}

bool PerformInteractionRequest::SetChoiceIdToResponseMsgParams(
    ns_smart_device_link::ns_smart_objects::SmartObject& msg_param) const {
  SDL_LOG_AUTO_TRACE();

  const bool ui_choice_id_valid = INVALID_CHOICE_ID != ui_choice_id_received_;
  const bool vr_choice_id_valid = INVALID_CHOICE_ID != vr_choice_id_received_;
  if (ui_choice_id_valid && vr_choice_id_valid &&
      ui_choice_id_received_ != vr_choice_id_received_) {
    return false;
  }

  if (!ui_text_entry_received_.empty()) {
    msg_param[strings::trigger_source] =
        mobile_apis::TriggerSource::TS_KEYBOARD;
    msg_param[strings::manual_text_entry] = ui_text_entry_received_;
    return true;
  }

  switch (interaction_mode_) {
    case mobile_apis::InteractionMode::eType::MANUAL_ONLY:
      if (ui_choice_id_valid) {
        msg_param[strings::trigger_source] =
            mobile_apis::TriggerSource::TS_MENU;
        msg_param[strings::choice_id] = ui_choice_id_received_;
      }
    case mobile_apis::InteractionMode::eType::VR_ONLY:
      if (vr_choice_id_valid) {
        msg_param[strings::trigger_source] = mobile_apis::TriggerSource::TS_VR;
        msg_param[strings::choice_id] = vr_choice_id_received_;
      }
    default:
      if (ui_choice_id_valid) {
        msg_param[strings::trigger_source] =
            mobile_apis::TriggerSource::TS_MENU;
        msg_param[strings::choice_id] = ui_choice_id_received_;
      } else if (vr_choice_id_valid) {
        msg_param[strings::trigger_source] = mobile_apis::TriggerSource::TS_VR;
        msg_param[strings::choice_id] = vr_choice_id_received_;
      }
  }

  return true;
}

}  // namespace commands
}  // namespace sdl_rpc_plugin

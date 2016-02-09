/*

 Copyright (c) 2016, Ford Motor Company
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
#include <string>
#include "application_manager/commands/mobile/perform_interaction_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "utils/file_system.h"
#include "utils/helpers.h"
#include "utils/custom_string.h"

namespace application_manager {

namespace commands {

namespace custom_str = utils::custom_string;

uint32_t PerformInteractionRequest::pi_requests_count_ = 0;

PerformInteractionRequest::PerformInteractionRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message),
      interaction_mode_(mobile_apis::InteractionMode::INVALID_ENUM),
      ui_response_recived_(false),
      vr_response_recived_(false),
      ui_result_(false),
      vr_result_(false),
      app_pi_was_active_before_(false),
      vr_resultCode_(mobile_apis::Result::INVALID_ENUM),
      ui_resultCode_(mobile_apis::Result::INVALID_ENUM) {
  subscribe_on_event(hmi_apis::FunctionID::UI_OnResetTimeout);
  subscribe_on_event(hmi_apis::FunctionID::VR_OnCommand);
  subscribe_on_event(hmi_apis::FunctionID::Buttons_OnButtonPress);
}

PerformInteractionRequest::~PerformInteractionRequest() {}

bool PerformInteractionRequest::Init() {
  /* Timeout in milliseconds.
     If omitted a standard value of 10000 milliseconds is used.*/
  if ((*message_)[strings::msg_params].keyExists(strings::timeout)) {
    default_timeout_ =
        (*message_)[strings::msg_params][strings::timeout].asUInt();
  }

  interaction_mode_ = static_cast<mobile_apis::InteractionMode::eType>(
      (*message_)[strings::msg_params][strings::interaction_mode].asInt());

  if (mobile_apis::InteractionMode::BOTH == interaction_mode_ ||
      mobile_apis::InteractionMode::MANUAL_ONLY == interaction_mode_) {
    default_timeout_ *= 2;
  }
  return true;
}

void PerformInteractionRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (app->is_perform_interaction_active()) {
    LOG4CXX_DEBUG(logger_, "Application has active PerformInteraction");
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
    LOG4CXX_ERROR(logger_,
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
        LOG4CXX_ERROR(logger_,
                      "interactionChoiceSetIDList is empty,"
                      " InteractionMode=BOTH and"
                      " interactionLayout=KEYBOARD");
        SendResponse(false, mobile_apis::Result::INVALID_DATA);
        return;
      }
    } else {
      LOG4CXX_ERROR(logger_,
                    "interactionChoiceSetIDList is empty"
                    " and interactionLayout!=KEYBOARD");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
  }

  if (choice_set_id_list_length &&
      (!CheckChoiceIDFromRequest(
          app, choice_set_id_list_length,
          msg_params[strings::interaction_choice_set_id_list]))) {

    LOG4CXX_ERROR(logger_,"PerformInteraction has choice sets with "
        "duplicated IDs or application does not have choice sets");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  if (msg_params.keyExists(strings::vr_help)) {
    if (mobile_apis::Result::SUCCESS !=
        MessageHelper::VerifyImageVrHelpItems(msg_params[strings::vr_help],
                                              app)) {
      LOG4CXX_ERROR(logger_,
                    "Verification of " << strings::vr_help << " failed.");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
  }

  if (IsWhiteSpaceExist()) {
    LOG4CXX_ERROR(logger_,
                  "Incoming perform interaction has contains \t\n \\t \\n");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  switch (interaction_mode_) {
    case mobile_apis::InteractionMode::BOTH: {
      LOG4CXX_DEBUG(logger_, "Interaction Mode: BOTH");
      if (!CheckChoiceSetVRSynonyms(app) || !CheckChoiceSetMenuNames(app) ||
          !CheckVrHelpItemPositions(app)) {
        return;
      }
      break;
    }
    case mobile_apis::InteractionMode::MANUAL_ONLY: {
      LOG4CXX_DEBUG(logger_, "Interaction Mode: MANUAL_ONLY");
      if (!CheckChoiceSetVRSynonyms(app) || !CheckChoiceSetMenuNames(app) ||
          !CheckVrHelpItemPositions(app)) {
        return;
      }
      break;
    }
    case mobile_apis::InteractionMode::VR_ONLY: {
      LOG4CXX_DEBUG(logger_, "Interaction Mode: VR_ONLY");
      if (!CheckChoiceSetVRSynonyms(app) || !CheckVrHelpItemPositions(app)) {
        return;
      }
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown interaction mode");
      return;
    }
  }

  app->set_perform_interaction_mode(static_cast<int32_t>(interaction_mode_));
  app->set_perform_interaction_active(true);
  // increment amount of active requests
  ++pi_requests_count_;
  SendVRPerformInteractionRequest(app);
  SendUIPerformInteractionRequest(app);
}

void PerformInteractionRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();
  smart_objects::SmartObject msg_param =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_OnResetTimeout: {
      LOG4CXX_DEBUG(logger_, "Received UI_OnResetTimeout event");
      ApplicationManagerImpl::instance()->updateRequestTimeout(
          connection_key(), correlation_id(), default_timeout());
      break;
    }
    case hmi_apis::FunctionID::UI_PerformInteraction: {
      LOG4CXX_DEBUG(logger_, "Received UI_PerformInteraction event");
      ui_response_recived_ = true;
      unsubscribe_from_event(hmi_apis::FunctionID::UI_PerformInteraction);
      ui_resultCode_ = GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asUInt()));
      ProcessPerformInteractionResponse(event.smart_object(), msg_param);
      break;
    }
    case hmi_apis::FunctionID::VR_PerformInteraction: {
      LOG4CXX_DEBUG(logger_, "Received VR_PerformInteraction");
      vr_response_recived_ = true;
      unsubscribe_from_event(hmi_apis::FunctionID::VR_PerformInteraction);
      vr_resultCode_ = GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asUInt()));
      ProcessVRResponse(event.smart_object(), msg_param);
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      break;
    }
  }

  if (mobile_apis::InteractionMode::BOTH == interaction_mode_ &&
      !HasHMIResponsesToWait()) {
    LOG4CXX_DEBUG(logger_, "Send response in BOTH iteraction mode");
    SendBothModeResponse(msg_param);
  }
}

void PerformInteractionRequest::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);

  switch (interaction_mode_) {
    case mobile_apis::InteractionMode::BOTH: {
      if (true == vr_response_recived_) {
        unsubscribe_from_event(hmi_apis::FunctionID::UI_PerformInteraction);
        DisablePerformInteraction();
        CommandRequestImpl::onTimeOut();
      } else {
        ApplicationManagerImpl::instance()->updateRequestTimeout(
            connection_key(), correlation_id(), default_timeout());
      }
      break;
    }
    case mobile_apis::InteractionMode::VR_ONLY: {
      ApplicationManagerImpl::instance()->updateRequestTimeout(
          connection_key(), correlation_id(), default_timeout());
      break;
    }
    case mobile_apis::InteractionMode::MANUAL_ONLY: {
      unsubscribe_from_event(hmi_apis::FunctionID::UI_PerformInteraction);
      DisablePerformInteraction();
      CommandRequestImpl::onTimeOut();
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "INVALID ENUM");
      return;
    }
  };
}

void PerformInteractionRequest::ProcessVRResponse(
    const smart_objects::SmartObject& message,
    smart_objects::SmartObject& msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  using namespace mobile_apis::Result;
  using namespace smart_objects;
  using namespace helpers;

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  CheckResponseResultCode();
  msg_params[strings::trigger_source] =
      static_cast<int32_t>(TriggerSource::TS_VR);

  const bool is_vr_aborted_timeout =
      Compare<Result::eType, EQ, ONE>(vr_resultCode_,
                                      ABORTED,
                                      TIMED_OUT);

  if (is_vr_aborted_timeout) {
    LOG4CXX_DEBUG(logger_, "VR response aborted");
    if (InteractionMode::VR_ONLY == interaction_mode_) {
      LOG4CXX_DEBUG(logger_, "Aborted or Timeout Send Close Popup");
      TerminatePerformInteraction();
      SendResponse(false, vr_resultCode_);
      return;
    }
    LOG4CXX_DEBUG(logger_, "Update timeout for UI");
    ApplicationManagerImpl::instance()->updateRequestTimeout(
        connection_key(), correlation_id(), default_timeout());
    return;
  }

  if (SUCCESS == vr_resultCode_ &&
      InteractionMode::MANUAL_ONLY == interaction_mode_) {
    LOG4CXX_DEBUG(logger_,
                  "VR response SUCCESS in MANUAL_ONLY mode "
                      << "Wait for UI response");
    // in case MANUAL_ONLY mode VR.PI SUCCESS just return
    return;
  }

  const SmartObject& hmi_msg_params = message[strings::msg_params];
  if (hmi_msg_params.keyExists(strings::choice_id)) {
    const int choise_id = hmi_msg_params[strings::choice_id].asInt();
    if (!CheckChoiceIDFromResponse(app, choise_id)) {
      LOG4CXX_ERROR(logger_, "Wrong choiceID was received from HMI");
      TerminatePerformInteraction();
      SendResponse(
          false, GENERIC_ERROR, "Wrong choiceID was received from HMI");
      return;
    }
    msg_params[strings::choice_id] = choise_id;
  }

  vr_result_ = true;

  if (mobile_apis::InteractionMode::BOTH == interaction_mode_ &&
      mobile_apis::Result::SUCCESS != vr_resultCode_) {
    LOG4CXX_DEBUG(logger_, "VR response isn't SUCCESS in BOTH mode");
    return;
  }

  LOG4CXX_DEBUG(logger_, "VR response consider to be SUCCESS");
  TerminatePerformInteraction();
  SendResponse(vr_result_, SUCCESS, NULL, &msg_params);
}

void PerformInteractionRequest::ProcessPerformInteractionResponse(
    const smart_objects::SmartObject& message,
    smart_objects::SmartObject& msg_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;
  using namespace smart_objects;

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());
  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  ui_result_ =
      Compare<mobile_api::Result::eType, EQ, ONE>(
        ui_resultCode_,
        mobile_apis::Result::SUCCESS,
        mobile_apis::Result::WARNINGS,
        mobile_apis::Result::UNSUPPORTED_RESOURCE);

  const bool is_pi_warning =
      Compare<mobile_api::Result::eType, EQ, ONE>(
        ui_resultCode_,
        mobile_apis::Result::WARNINGS);

  const bool is_pi_unsupported =
      Compare<mobile_api::Result::eType, EQ, ONE>(
        ui_resultCode_,
        mobile_apis::Result::UNSUPPORTED_RESOURCE);

  std::string info;

  if (ui_result_) {
    if (is_pi_warning) {
      ui_resultCode_ = mobile_apis::Result::WARNINGS;
      info = "Unsupported phoneme type was sent in an item";
      if (message.keyExists(strings::params)
          && message[strings::params].keyExists(strings::data)) {
        msg_params = message[strings::params][strings::data];
      }
    } else if (is_pi_unsupported) {
      ui_resultCode_ = mobile_apis::Result::UNSUPPORTED_RESOURCE;
      info = "Unsupported phoneme type was sent in an item";
    } else if (message.keyExists(strings::msg_params)) {
      msg_params = message[strings::msg_params];
    }
    // result code must be GENERIC_ERROR in case wrong choice_id
    if (msg_params.keyExists(strings::choice_id)) {
      if (!CheckChoiceIDFromResponse(app,
                                     msg_params[strings::choice_id].asInt())) {
        ui_resultCode_ = mobile_apis::Result::GENERIC_ERROR;
        info = "Wrong choiceID was received from HMI";
      } else {
        msg_params[strings::trigger_source] =
            mobile_apis::TriggerSource::TS_MENU;
      }
    } else if (msg_params.keyExists(strings::manual_text_entry)) {
      msg_params[strings::trigger_source] =
          mobile_apis::TriggerSource::TS_KEYBOARD;
      if (msg_params[strings::manual_text_entry].empty()) {
        msg_params.erase(strings::manual_text_entry);
      }
    }
  }

  const SmartObject* response_params =
      msg_params.empty()
      ? NULL
      : &msg_params;

  if (mobile_apis::InteractionMode::BOTH != interaction_mode_) {
    DisablePerformInteraction();
    SendResponse(ui_result_, ui_resultCode_, info.c_str(), response_params);
  }
}

void PerformInteractionRequest::SendUIPerformInteractionRequest(
    application_manager::ApplicationSharedPtr const app) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject& choice_set_id_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

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

  if (mobile_apis::InteractionMode::BOTH == mode ||
      mobile_apis::InteractionMode::MANUAL_ONLY == mode) {
    msg_params[strings::timeout] = default_timeout_ / 2;
  } else {
    msg_params[strings::timeout] = default_timeout_;
  }
  msg_params[strings::app_id] = app->app_id();
  if (mobile_apis::InteractionMode::VR_ONLY != mode) {
    msg_params[strings::choice_set] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);
  }
  int32_t index_array_of_vr_help = 0;
  for (size_t i = 0; i < choice_set_id_list.length(); ++i) {
    smart_objects::SmartObject* choice_set =
        app->FindChoiceSet(choice_set_id_list[i].asInt());
    if (choice_set) {
      // save perform interaction choice set
      app->AddPerformInteractionChoiceSet(
          correlation_id(), choice_set_id_list[i].asInt(), *choice_set);
      for (size_t j = 0; j < (*choice_set)[strings::choice_set].length(); ++j) {
        if (mobile_apis::InteractionMode::VR_ONLY != mode) {
          size_t index = msg_params[strings::choice_set].length();
          msg_params[strings::choice_set][index] =
              (*choice_set)[strings::choice_set][j];
          // vrCommands should be added via VR.AddCommand only
          msg_params[strings::choice_set][index].erase(strings::vr_commands);
        }
        if (mobile_apis::InteractionMode::MANUAL_ONLY != mode &&
            !is_vr_help_item) {
          smart_objects::SmartObject& vr_commands =
              (*choice_set)[strings::choice_set][j][strings::vr_commands];
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
  SendHMIRequest(
      hmi_apis::FunctionID::UI_PerformInteraction, &msg_params, true);
}

void PerformInteractionRequest::SendVRPerformInteractionRequest(
    application_manager::ApplicationSharedPtr const app) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  if (mobile_apis::InteractionMode::MANUAL_ONLY != interaction_mode_) {
    msg_params[strings::grammar_id] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);

    int32_t grammar_id_index = 0;
    for (uint32_t i = 0; i < choice_list.length(); ++i) {
      smart_objects::SmartObject* choice_set =
          app->FindChoiceSet(choice_list[i].asInt());
      if (!choice_set) {
        LOG4CXX_WARN(logger_, "Couldn't found choiset");
        continue;
      }
      msg_params[strings::grammar_id][grammar_id_index++] =
          (*choice_set)[strings::grammar_id].asUInt();
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::help_prompt)) {
    msg_params[strings::help_prompt] =
        (*message_)[strings::msg_params][strings::help_prompt];
  } else {
    if (choice_list.length() != 0) {
      msg_params[strings::help_prompt] =
          smart_objects::SmartObject(smart_objects::SmartType_Array);
    }
    int32_t index = 0;
    for (uint32_t i = 0; i < choice_list.length(); ++i) {
      smart_objects::SmartObject* choice_set =
          app->FindChoiceSet(choice_list[i].asInt());

      if (choice_set) {
        for (uint32_t j = 0; j < (*choice_set)[strings::choice_set].length();
             ++j) {
          smart_objects::SmartObject& vr_commands =
              (*choice_set)[strings::choice_set][j][strings::vr_commands];
          if (0 < vr_commands.length()) {
            // copy only first synonym
            smart_objects::SmartObject item(smart_objects::SmartType_Map);
            // Since there is no custom data from application side, SDL should
            // construct prompt and append delimiter to each item
            item[strings::type] = hmi_apis::Common_SpeechCapabilities::SC_TEXT;
            item[strings::text] = vr_commands[0].asString() +
                                  profile::Profile::instance()->tts_delimiter();
            msg_params[strings::help_prompt][index++] = item;
          }
        }
      } else {
        LOG4CXX_ERROR(logger_, "Can't found choiceSet!");
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::timeout_prompt)) {
    msg_params[strings::timeout_prompt] =
        (*message_)[strings::msg_params][strings::timeout_prompt];
  } else {
    if (msg_params.keyExists(strings::help_prompt)) {
      msg_params[strings::timeout_prompt] = msg_params[strings::help_prompt];
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::initial_prompt)) {
    msg_params[strings::initial_prompt] =
        (*message_)[strings::msg_params][strings::initial_prompt];
  }

  mobile_apis::InteractionMode::eType mode =
      static_cast<mobile_apis::InteractionMode::eType>(
          (*message_)[strings::msg_params][strings::interaction_mode].asInt());

  if (mobile_apis::InteractionMode::BOTH == mode ||
      mobile_apis::InteractionMode::MANUAL_ONLY == mode) {
    msg_params[strings::timeout] = default_timeout_ / 2;
  } else {
    msg_params[strings::timeout] = default_timeout_;
  }
  msg_params[strings::app_id] = app->app_id();
  SendHMIRequest(
      hmi_apis::FunctionID::VR_PerformInteraction, &msg_params, true);
}

bool PerformInteractionRequest::CheckChoiceSetMenuNames(
    application_manager::ApplicationSharedPtr const app) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  for (size_t i = 0; i < choice_list.length(); ++i) {
    // choice_set contains SmartObject msg_params
    smart_objects::SmartObject* i_choice_set =
        app->FindChoiceSet(choice_list[i].asInt());

    for (size_t j = 0; j < choice_list.length(); ++j) {
      smart_objects::SmartObject* j_choice_set =
          app->FindChoiceSet(choice_list[j].asInt());

      if (i == j) {
        // skip check the same element
        continue;
      }

      if (!i_choice_set || !j_choice_set) {
        LOG4CXX_ERROR(logger_, "Invalid ID");
        SendResponse(false, mobile_apis::Result::INVALID_ID);
        return false;
      }

      size_t ii = 0;
      size_t jj = 0;
      for (; ii < (*i_choice_set)[strings::choice_set].length(); ++ii) {
        for (; jj < (*j_choice_set)[strings::choice_set].length(); ++jj) {
          const std::string& ii_menu_name =
              (*i_choice_set)[strings::choice_set][ii][strings::menu_name]
                  .asString();
          const std::string& jj_menu_name =
              (*j_choice_set)[strings::choice_set][jj][strings::menu_name]
                  .asString();

          if (ii_menu_name == jj_menu_name) {
            LOG4CXX_ERROR(logger_, "Choice set has duplicated menu name");
            SendResponse(false,
                         mobile_apis::Result::DUPLICATE_NAME,
                         "Choice set has duplicated menu name");
            return false;
          }
        }
      }
    }
  }

  return true;
}

bool PerformInteractionRequest::CheckChoiceSetVRSynonyms(
    application_manager::ApplicationSharedPtr const app) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  for (size_t i = 0; i < choice_list.length(); ++i) {
    // choice_set contains SmartObject msg_params
    smart_objects::SmartObject* i_choice_set =
        app->FindChoiceSet(choice_list[i].asInt());

    for (size_t j = 0; j < choice_list.length(); ++j) {
      smart_objects::SmartObject* j_choice_set =
          app->FindChoiceSet(choice_list[j].asInt());

      if (i == j) {
        // skip check the same element
        continue;
      }

      if ((!i_choice_set) || (!j_choice_set)) {
        LOG4CXX_ERROR(logger_, "Invalid ID");
        SendResponse(false, mobile_apis::Result::INVALID_ID);
        return false;
      }

      size_t ii = 0;
      size_t jj = 0;
      for (; ii < (*i_choice_set)[strings::choice_set].length(); ++ii) {
        for (; jj < (*j_choice_set)[strings::choice_set].length(); ++jj) {
          // choice_set pointer contains SmartObject msg_params
          smart_objects::SmartObject& ii_vr_commands =
              (*i_choice_set)[strings::choice_set][ii][strings::vr_commands];

          smart_objects::SmartObject& jj_vr_commands =
              (*j_choice_set)[strings::choice_set][jj][strings::vr_commands];

          for (size_t iii = 0; iii < ii_vr_commands.length(); ++iii) {
            for (size_t jjj = 0; jjj < jj_vr_commands.length(); ++jjj) {
              const custom_str::CustomString& vr_cmd_i =
                  ii_vr_commands[iii].asCustomString();
              const custom_str::CustomString& vr_cmd_j =
                  jj_vr_commands[jjj].asCustomString();
              if (vr_cmd_i.CompareIgnoreCase(vr_cmd_j)) {
                LOG4CXX_ERROR(logger_, "Choice set has duplicated VR synonym");
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
  LOG4CXX_AUTO_TRACE(logger_);

  if (!(*message_)[strings::msg_params].keyExists(strings::vr_help)) {
    LOG4CXX_DEBUG(logger_, strings::vr_help << " is omitted.");
    return true;
  }

  smart_objects::SmartObject& vr_help =
      (*message_)[strings::msg_params][strings::vr_help];

  int32_t position = 1;
  for (size_t i = 0; i < vr_help.length(); ++i) {
    if (position != vr_help[i][strings::position].asInt()) {
      LOG4CXX_ERROR(logger_, "Non-sequential vrHelp item position");
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
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());
  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
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
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  str = (*message_)[strings::msg_params][strings::initial_text].asCharArray();
  if (!CheckSyntax(str)) {
    LOG4CXX_ERROR(logger_, "Invalid initial_text syntax check failed");
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
        LOG4CXX_ERROR(logger_, "Invalid initial_prompt syntax check failed");
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
        LOG4CXX_ERROR(logger_, "Invalid help_prompt syntax check failed");
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
        LOG4CXX_ERROR(logger_, "Invalid timeout_prompt syntax check failed");
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
        LOG4CXX_ERROR(logger_, "Invalid vr_help syntax check failed");
        return true;
      }

      if ((*it_vh).keyExists(strings::image)) {
        str = (*it_vh)[strings::image][strings::value].asCharArray();
        if (!CheckSyntax(str)) {
          LOG4CXX_ERROR(logger_,
                        "Invalid vr_help image value syntax check failed");
          return true;
        }
      }
    }
  }
  return false;
}

void PerformInteractionRequest::TerminatePerformInteraction() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[hmi_request::method_name] = "UI.PerformInteraction";
  SendHMIRequest(hmi_apis::FunctionID::UI_ClosePopUp, &msg_params);
  DisablePerformInteraction();
}

bool PerformInteractionRequest::CheckChoiceIDFromResponse(
    ApplicationSharedPtr app, int32_t choice_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  const DataAccessor<PerformChoiceSetMap> accessor =
      app->performinteraction_choice_set_map();
  const PerformChoiceSetMap& choice_set_map = accessor.GetData();

  PerformChoiceSetMap::const_iterator choice_set_map_it =
      choice_set_map.find(correlation_id());
  if (choice_set_map.end() != choice_set_map_it) {
    const PerformChoice& choice = choice_set_map_it->second;
    PerformChoice::const_iterator it = choice.begin();
    for (; choice.end() != it; ++it) {
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
  return false;
}

bool PerformInteractionRequest::CheckChoiceIDFromRequest(
    ApplicationSharedPtr app,
    const size_t choice_set_id_list_length,
    const smart_objects::SmartObject& choice_set_id_list) const {
  LOG4CXX_AUTO_TRACE(logger_);

  size_t choice_list_length = 0;
  std::set<uint32_t> choice_id_set;
  smart_objects::SmartObject* choice_set = 0;
  std::pair<std::set<uint32_t>::iterator, bool> ins_res;

  for (size_t i = 0; i < choice_set_id_list_length; ++i) {
    choice_set = app->FindChoiceSet(choice_set_id_list[i].asInt());
    if (!choice_set) {
      LOG4CXX_ERROR(logger_, "Couldn't find choiset_id = "
                    <<choice_set_id_list[i].asInt());
      return false;
    }
    choice_list_length = (*choice_set)[strings::choice_set].length();
    const smart_objects::SmartObject& choices_list =
        (*choice_set)[strings::choice_set];
    for (size_t k = 0; k < choice_list_length; ++k) {
      ins_res = choice_id_set.insert(choices_list[k][strings::choice_id].asInt());
      if (!ins_res.second) {
        LOG4CXX_ERROR(logger_, "Choise with ID "
                      << choices_list[k][strings::choice_id].asInt()
                      << " already exists");
        return false;
      }
    }
  }
  return true;
}

const bool PerformInteractionRequest::HasHMIResponsesToWait() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return !ui_response_recived_ || !vr_response_recived_;
}

void PerformInteractionRequest::CheckResponseResultCode() {
  LOG4CXX_AUTO_TRACE(logger_);
  mobile_apis::Result::eType resultCode = mobile_apis::Result::INVALID_ENUM;
  bool result = false;
  if (mobile_apis::Result::GENERIC_ERROR == vr_resultCode_) {
    LOG4CXX_DEBUG(logger_, "VR response GENERIC_ERROR");
    resultCode = mobile_apis::Result::GENERIC_ERROR;
  } else if (mobile_apis::Result::REJECTED == vr_resultCode_) {
    LOG4CXX_DEBUG(logger_, "VR had been rejected.");
    resultCode = mobile_apis::Result::REJECTED;
  } else if (mobile_apis::Result::WARNINGS == vr_resultCode_ ||
      mobile_apis::Result::UNSUPPORTED_REQUEST == vr_resultCode_) {
    LOG4CXX_DEBUG(logger_, "VR response WARNINGS");
    resultCode = mobile_api::Result::WARNINGS;
    result = true;
  }

  if (mobile_apis::Result::INVALID_ENUM != resultCode) {
    TerminatePerformInteraction();
    SendResponse(result, resultCode);
  }
}

void PerformInteractionRequest::SendBothModeResponse(
    const smart_objects::SmartObject& msg_param) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis::Result;

  bool result = ui_result_ || vr_result_;
  mobile_apis::Result::eType perform_interaction_result_code = ui_resultCode_;

  if (UNSUPPORTED_RESOURCE == vr_resultCode_ &&
      UNSUPPORTED_RESOURCE != ui_resultCode_) {
    perform_interaction_result_code = vr_resultCode_;
  } else if (UNSUPPORTED_RESOURCE == vr_resultCode_ &&
      UNSUPPORTED_RESOURCE == ui_resultCode_) {
    result = false;
  }

  const bool is_error_code = (SUCCESS != perform_interaction_result_code ||
      WARNINGS != perform_interaction_result_code);

  if (vr_resultCode_ == ui_resultCode_ && is_error_code) {
    result = false;
  }

  const smart_objects::SmartObject* response_params =
      msg_param.empty()
      ? NULL
      : &msg_param;

  TerminatePerformInteraction();
  SendResponse(result, perform_interaction_result_code, NULL, response_params);
}

}  // namespace commands

}  // namespace application_manager

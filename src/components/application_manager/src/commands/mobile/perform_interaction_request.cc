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

namespace application_manager {

namespace commands {

PerformInteractionRequest::PerformInteractionRequest(
  const MessageSharedPtr& message)
: CommandRequestImpl(message),
  timer_(this, &PerformInteractionRequest::onTimer),
  tts_perform_interaction_code_(mobile_apis::Result::INVALID_ENUM) {

  subscribe_on_event(hmi_apis::FunctionID::UI_OnResetTimeout);
  subscribe_on_event(hmi_apis::FunctionID::VR_OnCommand);
  subscribe_on_event(hmi_apis::FunctionID::Buttons_OnButtonPress);
  subscribe_on_event(
      hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered);
}

PerformInteractionRequest::~PerformInteractionRequest() {
}

void PerformInteractionRequest::onTimer() {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::onTimer");
}

bool PerformInteractionRequest::Init() {

  /* Timeout in milliseconds.
     If omitted a standard value of 10000 milliseconds is used.*/
  if ((*message_)[strings::msg_params].keyExists(strings::timeout)) {
    default_timeout_ =
        (*message_)[strings::msg_params][strings::timeout].asUInt();
  }
  mobile_apis::InteractionMode::eType mode =
      static_cast<mobile_apis::InteractionMode::eType>(
      (*message_)[strings::msg_params][strings::interaction_mode].asInt());

  if (mobile_apis::InteractionMode::BOTH == mode ||
      mobile_apis::InteractionMode::MANUAL_ONLY == mode) {
      default_timeout_ *= 2;
    }
  return true;
}

void PerformInteractionRequest::Run() {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::Run");

  // timer_.start(2);

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (0 != app->is_perform_interaction_active()) {
    LOG4CXX_INFO(logger_, "Another perform interaction is running!");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  mobile_apis::Result::eType verification_result =
      MessageHelper::VerifyImageFiles((*message_)[strings::msg_params], app);

  if ((mobile_apis::Result::SUCCESS != verification_result) &&
      (mobile_apis::Result::UNSUPPORTED_RESOURCE != verification_result)) {
    LOG4CXX_ERROR_EXT(
        logger_,
        "MessageHelper::VerifyImageFiles return " << verification_result);
    SendResponse(false, verification_result);
    return;
  }

  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  for (size_t i = 0; i < choice_list.length(); ++i) {
    if (!app->FindChoiceSet(choice_list[i].asInt())) {
      LOG4CXX_ERROR(logger_, "Invalid ID");
      SendResponse(false, mobile_apis::Result::INVALID_ID);
      return;
    }
  }

  uint32_t correlation_id =
      (*message_)[strings::params][strings::correlation_id].asUInt();

  int32_t mode =
      (*message_)[strings::msg_params][strings::interaction_mode].asInt();

  app->set_perform_interaction_mode(mode);

  mobile_apis::InteractionMode::eType interaction_mode =
      static_cast<mobile_apis::InteractionMode::eType>(mode);

  switch (interaction_mode) {
    case mobile_apis::InteractionMode::BOTH: {
      LOG4CXX_INFO(logger_, "Interaction Mode: BOTH");
      if (!CheckChoiceSetVRSynonyms(app)) {
        return;
      }

      if (!CheckChoiceSetMenuNames(app)) {
        return;
      }

      if (!CheckVrHelpItemPositions(app)) {
        return;
      }

      app->set_perform_interaction_active(correlation_id);
      SendTTSPerformInteractionRequest(app);
      SendUIPerformInteractionRequest(app);
      break;
    }
    case mobile_apis::InteractionMode::MANUAL_ONLY: {
      LOG4CXX_INFO(logger_, "Interaction Mode: MANUAL_ONLY");
      if (!CheckChoiceSetMenuNames(app)) {
        return;
      }

      app->set_perform_interaction_active(correlation_id);
      SendTTSPerformInteractionRequest(app);
      SendUIPerformInteractionRequest(app);
      break;
    }
    case mobile_apis::InteractionMode::VR_ONLY: {
      LOG4CXX_INFO(logger_, "Interaction Mode: VR_ONLY");
      if (!CheckChoiceSetVRSynonyms(app)) {
        return;
      }

      if (!CheckVrHelpItemPositions(app)) {
        return;
      }

      // TODO(DK): need to implement timeout
      app->set_perform_interaction_active(correlation_id);
      SendTTSPerformInteractionRequest(app);
      SendUIPerformInteractionRequest(app);
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown interaction mode");
      return;
    }
  }

  // TODO(DK): need to implement timeout TTS speak request.
}

void PerformInteractionRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::on_event");

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_OnResetTimeout: {
      LOG4CXX_INFO(logger_, "Received UI_OnResetTimeout event");
      ApplicationManagerImpl::instance()->updateRequestTimeout(connection_key(),
        correlation_id(),
      default_timeout());
      break;
    }
    case hmi_apis::FunctionID::VR_OnCommand: {
      LOG4CXX_INFO(logger_, "Received VR_OnCommand event");
      ProcessVRNotification(event.smart_object());
      break;
    }
    case hmi_apis::FunctionID::UI_PerformInteraction: {
      LOG4CXX_INFO(logger_, "Received UI_PerformInteraction event");
      ProcessPerformInteractionResponse(event.smart_object());
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnAppUnregistered: {
      LOG4CXX_INFO(logger_, "Received OnAppUnregistered event");
      ProcessAppUnregisteredNotification(event.smart_object());
      break;
    }
    case hmi_apis::FunctionID::TTS_PerformInteraction: {
      LOG4CXX_INFO(logger_, "Received TTS_PerformInteraction");
      tts_perform_interaction_code_ = static_cast<mobile_apis::Result::eType>(
          event.smart_object()[strings::params][hmi_response::code].asInt());
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      break;
    }
  }
}

void PerformInteractionRequest::onTimeOut() {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::onTimeOut");

  // Unsubscribe from event on UIPerformInteractionResponse to
  // avoid of double execution of SendVrDeleteCommand()
  unsubscribe_from_event(hmi_apis::FunctionID::UI_PerformInteraction);
  DisablePerformInteraction();
  CommandRequestImpl::onTimeOut();
}


void PerformInteractionRequest::ProcessVRNotification(
    const smart_objects::SmartObject& message) {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::ProcessVRNotification");
  const uint32_t app_id = message[strings::msg_params][strings::app_id]
                                .asUInt();
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(app_id);
  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }
  int32_t cmd_id = message[strings::msg_params][strings::cmd_id].asInt();
  const PerformChoiceSetMap& choice_set_map = app
      ->performinteraction_choice_set_map();
  bool choice_id_chosen = false;
  LOG4CXX_INFO(logger_, "If command was choice id");
  for (PerformChoiceSetMap::const_iterator it = choice_set_map.begin();
      choice_set_map.end() != it; ++it) {
    const smart_objects::SmartObject& choice_set = (*it->second).getElement(
        strings::choice_set);
    for (size_t j = 0; j < choice_set.length(); ++j) {
      if (cmd_id ==
          choice_set.getElement(j).getElement(strings::choice_id).asInt()) {
        choice_id_chosen = true;
        break;
      }
    }
  }
  if (choice_id_chosen) {
    LOG4CXX_INFO(logger_, "Command was choice id!");
    smart_objects::SmartObject c_p_request_so = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    c_p_request_so[hmi_request::method_name] = "UI.PerformInteraction";
    SendHMIRequest(hmi_apis::FunctionID::UI_ClosePopUp, &(c_p_request_so));
    DisablePerformInteraction();

    (*message_)[strings::params][strings::function_id] =
        static_cast<int32_t>(mobile_apis::FunctionID::PerformInteractionID);
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params[strings::choice_id] = cmd_id;
    msg_params[strings::trigger_source] =
        static_cast<int32_t>(mobile_apis::TriggerSource::TS_VR);
    SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &(msg_params));

  } else {
    LOG4CXX_INFO(logger_, "Sending OnCommand notification");
    smart_objects::SmartObject* notification_so =
        new smart_objects::SmartObject(smart_objects::SmartType_Map);
    if (!notification_so) {
      LOG4CXX_ERROR(
          logger_,
          "Failed to allocate memory for perform interaction response.");
      return;
    }
    smart_objects::SmartObject& notification = *notification_so;
    notification = message;
    notification[strings::params][strings::function_id] =
        static_cast<int32_t>(mobile_apis::FunctionID::eType::OnCommandID);
    notification[strings::msg_params][strings::trigger_source] =
        static_cast<int32_t>(mobile_apis::TriggerSource::TS_VR);
    ApplicationManagerImpl::instance()->ManageMobileCommand(notification_so);
  }
}

void PerformInteractionRequest::ProcessAppUnregisteredNotification
  (const smart_objects::SmartObject& message) {
  LOG4CXX_INFO(logger_,
               "PerformInteractionRequest::ProcessAppUnregisteredNotification");
  const uint32_t app_id = connection_key();
  if (app_id == message[strings::msg_params][strings::app_id].asUInt()) {
    DisablePerformInteraction();
  } else {
    LOG4CXX_INFO(logger_, "Notification was sent from another application");
  }
}

void PerformInteractionRequest::SendVrDeleteCommand(
    application_manager::ApplicationSharedPtr const app) {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::SendVrDeleteCommand");

  const PerformChoiceSetMap& choice_set_map =
      app->performinteraction_choice_set_map();

  PerformChoiceSetMap::const_iterator it = choice_set_map.begin();
  for (; choice_set_map.end() != it; ++it) {
    const smart_objects::SmartObject& choice_set = (*it->second).getElement(
        strings::choice_set);
    for (size_t j = 0; j < choice_set.length(); ++j) {
      smart_objects::SmartObject msg_params = smart_objects::SmartObject(
          smart_objects::SmartType_Map);
      msg_params[strings::app_id] = app->app_id();
      msg_params[strings::cmd_id] = choice_set.getElement(j).getElement(
          strings::choice_id);
      SendHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, &msg_params);
    }
  }
}

void PerformInteractionRequest::ProcessPerformInteractionResponse(
    const smart_objects::SmartObject& message) {
  LOG4CXX_INFO(logger_,
               "PerformInteractionRequest::ProcessPerformInteractionResponse");

  DisablePerformInteraction();

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params = message[strings::msg_params];

  bool result = false;
  int32_t hmi_response_code =
      message[strings::params][hmi_response::code].asInt();
  if (hmi_apis::Common_Result::SUCCESS ==
      hmi_apis::Common_Result::eType(hmi_response_code)) {
    if (message[strings::msg_params].keyExists(strings::manual_text_entry)) {
      msg_params[strings::trigger_source] = mobile_apis::TriggerSource::TS_KEYBOARD;
    } else {
      msg_params[strings::trigger_source] = mobile_apis::TriggerSource::TS_MENU;
    }
    result = true;
  }

  const char* return_info = NULL;
  mobile_apis::Result::eType result_code =
      static_cast<mobile_apis::Result::eType>(hmi_response_code);
  if (result) {
    if (hmi_apis::Common_Result::UNSUPPORTED_RESOURCE ==
        hmi_apis::Common_Result::eType(hmi_response_code)) {
      result_code = mobile_apis::Result::WARNINGS;
      return_info = std::string(
          "Unsupported phoneme type sent in any item").c_str();
    }
  }

  SendResponse(result, result_code, return_info, &(msg_params));
}



void PerformInteractionRequest::SendUIPerformInteractionRequest(
    application_manager::ApplicationSharedPtr const app) {
  smart_objects::SmartObject& choice_set_id_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  mobile_apis::InteractionMode::eType mode  =
      static_cast<mobile_apis::InteractionMode::eType>(
      (*message_)[strings::msg_params][strings::interaction_mode].asInt());

  if (mobile_apis::InteractionMode::VR_ONLY != mode) {
    msg_params[hmi_request::initial_text][hmi_request::field_name] =
        static_cast<int32_t>(application_manager::TextFieldName::
                         INITIAL_INTERACTION_TEXT);
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
    msg_params[strings::timeout] = default_timeout_/2;
  } else {
    msg_params[strings::timeout] = default_timeout_;
  }
  msg_params[strings::app_id] = app->app_id();
  if (mobile_apis::InteractionMode::VR_ONLY != mode) {
    msg_params[strings::choice_set] = smart_objects::SmartObject(
      smart_objects::SmartType_Array);
  }
  int32_t index_array_of_vr_help = 0;
  for (size_t i = 0; i < choice_set_id_list.length(); ++i) {
    smart_objects::SmartObject* choice_set = app->FindChoiceSet(
        choice_set_id_list[i].asInt());
    if (choice_set) {
      // save perform interaction choice set
      app->AddPerformInteractionChoiceSet(choice_set_id_list[i].asInt(),
                                          *choice_set);
      for (size_t j = 0; j < (*choice_set)[strings::choice_set].length(); ++j) {
        if (mobile_apis::InteractionMode::VR_ONLY != mode) {
          size_t index = msg_params[strings::choice_set].length();
          msg_params[strings::choice_set][index] =
              (*choice_set)[strings::choice_set][j];
          // vrCommands should be added via VR.AddCommand only
          msg_params[strings::choice_set][index].erase(strings::vr_commands);
        }
        if (mobile_apis::InteractionMode::MANUAL_ONLY !=
            mode && !is_vr_help_item) {
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
  if ((*message_)[strings::msg_params]
                  .keyExists(hmi_request::interaction_layout)
        && mobile_apis::InteractionMode::VR_ONLY != mode) {
    msg_params[hmi_request::interaction_layout] =
        (*message_)[strings::msg_params][hmi_request::interaction_layout].
        asInt();
  }
  CreateUIPerformInteraction(msg_params, app);
}

void PerformInteractionRequest::CreateUIPerformInteraction(
    const smart_objects::SmartObject& msg_params,
    application_manager::ApplicationSharedPtr const app) {
  SendHMIRequest(hmi_apis::FunctionID::UI_PerformInteraction,
                     &msg_params, true);
}

void PerformInteractionRequest::SendTTSPerformInteractionRequest(
    application_manager::ApplicationSharedPtr const app) {
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  if ((*message_)[strings::msg_params].keyExists(strings::help_prompt)) {

    msg_params[strings::help_prompt] =
        (*message_)[strings::msg_params][strings::help_prompt];

    DeleteParameterFromTTSChunk(&msg_params[strings::help_prompt]);
  } else {

    smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

    msg_params[strings::help_prompt] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);

    int32_t index = 0;
    int32_t grammar_id_index = 0;

    msg_params[strings::grammar_id] = smart_objects::SmartObject(smart_objects::SmartType_Array);
    for (uint32_t i = 0; i < choice_list.length(); ++i) {
      smart_objects::SmartObject* choice_set =
          app->FindChoiceSet(choice_list[i].asInt());

      if (choice_set) {
        for (uint32_t j = 0;
            j < (*choice_set)[strings::choice_set].length();
            ++j) {
          smart_objects::SmartObject& vr_commands =
              (*choice_set)[strings::choice_set][j][strings::vr_commands];
          if (0 < vr_commands.length()) {
            // copy only first synonym
            smart_objects::SmartObject item(smart_objects::SmartType_Map);
            item[strings::text] = vr_commands[0].asString();
            msg_params[strings::help_prompt][index++] = item;
          }
        }
         msg_params[strings::grammar_id][grammar_id_index++]= (*choice_set)[strings::grammar_id];
      } else {
        LOG4CXX_ERROR(logger_, "Can't found choiset!")
      }
    }
  }

  if ((*message_)[strings::msg_params].keyExists(strings::timeout_prompt)) {
    msg_params[strings::timeout_prompt] =
            (*message_)[strings::msg_params][strings::timeout_prompt];

    DeleteParameterFromTTSChunk(&msg_params[strings::timeout_prompt]);
  } else {
    msg_params[strings::timeout_prompt] = msg_params[strings::help_prompt];
  }

  if ((*message_)[strings::msg_params].keyExists(strings::initial_prompt)) {
      msg_params[strings::initial_prompt] =
          (*message_)[strings::msg_params][strings::initial_prompt];

      DeleteParameterFromTTSChunk(&msg_params[strings::initial_prompt]);
  }

  mobile_apis::InteractionMode::eType mode =
      static_cast<mobile_apis::InteractionMode::eType>(
        (*message_)[strings::msg_params][strings::interaction_mode].asInt());

  if (mobile_apis::InteractionMode::BOTH == mode ||
      mobile_apis::InteractionMode::MANUAL_ONLY == mode) {
    msg_params[strings::timeout] = default_timeout_/2;
  } else {
    msg_params[strings::timeout] = default_timeout_;
  }

  SendHMIRequest(hmi_apis::FunctionID::TTS_PerformInteraction, &msg_params,
                 true);
}

void PerformInteractionRequest::DeleteParameterFromTTSChunk
(smart_objects::SmartObject* array_tts_chunk) {
  int32_t length = array_tts_chunk->length();
  for (int32_t i = 0; i < length; ++i) {
    array_tts_chunk[i].erase(strings::type);
  }
}

bool PerformInteractionRequest::CheckChoiceSetMenuNames(
    application_manager::ApplicationSharedPtr const app) {
  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  for (size_t i = 0; i < choice_list.length(); ++i) {
    // choice_set contains SmartObject msg_params
    smart_objects::SmartObject* i_choice_set = app->FindChoiceSet(
        choice_list[i].asInt());

    for (size_t j = 0; j < choice_list.length(); ++j) {
      smart_objects::SmartObject* j_choice_set = app->FindChoiceSet(
          choice_list[j].asInt());

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
          std::string ii_menu_name =
              (*i_choice_set)[strings::choice_set][ii][strings::menu_name]
                  .asString();
          std::string jj_menu_name =
              (*j_choice_set)[strings::choice_set][jj][strings::menu_name]
                  .asString();

          if (ii_menu_name == jj_menu_name) {
            LOG4CXX_ERROR(logger_, "Choice set has duplicated menu name");
            SendResponse(false, mobile_apis::Result::DUPLICATE_NAME,
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
  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  for (size_t i = 0; i < choice_list.length(); ++i) {
    // choice_set contains SmartObject msg_params
    smart_objects::SmartObject* i_choice_set = app->FindChoiceSet(
        choice_list[i].asInt());

    for (size_t j = 0; j < choice_list.length(); ++j) {
      smart_objects::SmartObject* j_choice_set = app->FindChoiceSet(
          choice_list[j].asInt());

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
              std::string vr_cmd_i = ii_vr_commands[iii].asString();
              std::string vr_cmd_j = jj_vr_commands[jjj].asString();
              if (0 == strcasecmp(vr_cmd_i.c_str(), vr_cmd_j.c_str())) {
                LOG4CXX_ERROR(logger_, "Choice set has duplicated VR synonym");
                SendResponse(false, mobile_apis::Result::DUPLICATE_NAME,
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

  if (!(*message_)[strings::msg_params].keyExists(strings::vr_help)) {
    LOG4CXX_INFO(logger_, ""
        "PerformInteractionRequest::CheckVrHelpItemPositions vr_help omitted");
    return true;
  }

  smart_objects::SmartObject& vr_help =
      (*message_)[strings::msg_params][strings::vr_help];

  int32_t position = 1;
  for (size_t i = 0; i < vr_help.length(); ++i) {
    if (position != vr_help[i][strings::position].asInt()) {
      LOG4CXX_ERROR(logger_, "Non-sequential vrHelp item position");
      SendResponse(false, mobile_apis::Result::REJECTED,
                   "Non-sequential vrHelp item position");
      return false;
    }
    ++position;
  }
  return true;
}

void PerformInteractionRequest::DisablePerformInteraction() {
  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(connection_key());
  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  if (app->is_perform_interaction_active()) {
    app->set_perform_interaction_active(0);
    app->set_perform_interaction_mode(-1);
    app->DeletePerformInteractionChoiceSetMap();
  }
}

}  // namespace commands

}  // namespace application_manager

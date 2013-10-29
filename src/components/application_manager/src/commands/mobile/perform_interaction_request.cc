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
  is_keyboard_trigger_source_(false),
  trigger_source_(mobile_apis::TriggerSource::INVALID_ENUM){

  subscribe_on_event(hmi_apis::FunctionID::VR_OnCommand);
  subscribe_on_event(hmi_apis::FunctionID::Buttons_OnButtonPress);
}

PerformInteractionRequest::~PerformInteractionRequest() {
}

void PerformInteractionRequest::onTimer() const{
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::onTimer");
}

bool PerformInteractionRequest::Init() {

  /* Timeout in milliseconds.
     If omitted a standard value of 10000 milliseconds is used.*/
  if ((*message_)[strings::msg_params].keyExists(strings::timeout)) {
    default_timeout_ =
        (*message_)[strings::msg_params][strings::timeout].asUInt();
  } else {
    default_timeout_ = 10000;
  }

  return true;
}

void PerformInteractionRequest::Run() {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::Run");

  //timer_.start(2);

  Application* app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key]);

  if (NULL == app) {
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

  if (mobile_apis::Result::SUCCESS != verification_result) {
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

  unsigned int correlation_id =
      (*message_)[strings::params][strings::correlation_id].asUInt();

  int mode =
      (*message_)[strings::msg_params][strings::interaction_mode].asInt();
  app->set_perform_interaction_mode(mode);

  switch (mode) {
    case InteractionMode::BOTH: {
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
      SendVRAddCommandRequest(app);
      SendUIPerformInteractionRequest(app);
      break;
    }
    case InteractionMode::MANUAL_ONLY: {
      LOG4CXX_INFO(logger_, "Interaction Mode: MANUAL_ONLY");
      if (!CheckChoiceSetMenuNames(app)) {
        return;
      }

      app->set_perform_interaction_active(correlation_id);
      SendUIPerformInteractionRequest(app);
      break;
    }
    case InteractionMode::VR_ONLY: {
      LOG4CXX_INFO(logger_, "Interaction Mode: VR_ONLY");
      if (!CheckChoiceSetVRSynonyms(app)) {
        return;
      }

      if (!CheckVrHelpItemPositions(app)) {
        return;
      }

      // TODO(DK): need to implement timeout
      app->set_perform_interaction_active(correlation_id);
      SendVRAddCommandRequest(app);
      SendUIShowVRHelpRequest(app);
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown interaction mode");
      return;
    }
  }

  SendTTSSpeakRequest(app);

  // TODO(DK): need to implement timeout TTS speak request.
}

void PerformInteractionRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::on_event");

  switch (event.id()) {
    case hmi_apis::FunctionID::VR_OnCommand: {
      LOG4CXX_INFO(logger_, "Received VR_OnCommand event");
      ProcessMessageFromVR(event.smart_object());
      break;
    }
    case hmi_apis::FunctionID::Buttons_OnButtonPress: {
      LOG4CXX_INFO(logger_, "Recived Buttons_OnButtonPress");
      is_keyboard_trigger_source_ = true;
      break;
    }
    case hmi_apis::FunctionID::UI_PerformInteraction: {
      LOG4CXX_INFO(logger_,"Received UI_PerformInteraction event");
      if(is_keyboard_trigger_source_) {
        trigger_source_ = mobile_apis::TriggerSource::TS_KEYBOARD;
      } else {
        trigger_source_ = mobile_apis::TriggerSource::TS_MENU;
      }
      ProcessPerformInteractionResponse(event.smart_object());
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_,"Received unknown event" << event.id());
      break;
    }
  }
}

//called when vrnotification comes
void PerformInteractionRequest::ProcessMessageFromVR(
    const smart_objects::SmartObject& message) {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::ProcesMessageFromVR");
  const unsigned int app_id = message[strings::msg_params][strings::app_id]
                                .asUInt();
  Application* app = ApplicationManagerImpl::instance()->application(app_id);
  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }
  int cmd_id = message[strings::msg_params][strings::cmd_id].asInt();
  const PerformChoiceSetMap& choice_set_map = app
      ->performinteraction_choice_set_map();
  bool choice_id_chosen = false;
  LOG4CXX_INFO(logger_, "If command was choice id");
  for (PerformChoiceSetMap::const_iterator it = choice_set_map.begin();
         choice_set_map.end() != it; ++it) {
      const smart_objects::SmartObject& choice_set = (*it->second).getElement(
            strings::choice_set);

      for (size_t j = 0; j < choice_set.length(); ++j) {
        if (cmd_id
            == choice_set.getElement(j).getElement(strings::choice_id).asInt()) {
          choice_id_chosen = true;
          break;
        }
      }
    }
    if (choice_id_chosen) {
      LOG4CXX_INFO(logger_, "Command was choice id!");
      SendVrDeleteCommand (app);
      smart_objects::SmartObject c_p_request_so = smart_objects::SmartObject(
                smart_objects::SmartType_Map);
      SendHMIRequest(hmi_apis::FunctionID::UI_ClosePopUp, &(c_p_request_so));
      app->set_perform_interaction_mode(-1);
      app->DeletePerformInteractionChoiceSetMap();
      app->set_perform_interaction_active(0);

      (*message_)[strings::params][strings::function_id] =
          mobile_apis::FunctionID::PerformInteractionID;
      smart_objects::SmartObject msg_params = smart_objects::SmartObject(
          smart_objects::SmartType_Map);
      msg_params[strings::trigger_source] =
        mobile_apis::TriggerSource::TS_VR;
      SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &(msg_params));

    } else {
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
            mobile_apis::FunctionID::eType::OnCommandID;
      notification[strings::msg_params][strings::trigger_source] =
          mobile_apis::TriggerSource::TS_VR;
      ApplicationManagerImpl::instance()->ManageMobileCommand(notification_so);
    }
}

void PerformInteractionRequest::SendVrDeleteCommand (Application* const app) {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::SendVrDeleteCommand");
  const PerformChoiceSetMap& choice_set_map = app
      ->performinteraction_choice_set_map();
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
  Application* app = ApplicationManagerImpl::instance()->application(
        (*message_)[strings::params][strings::connection_key]);
    if (NULL == app) {
      LOG4CXX_ERROR(logger_, "NULL pointer");
      return;
    }
    if (app->is_perform_interaction_active()) {
      if (mobile_apis::InteractionMode::MANUAL_ONLY
          != app->perform_interaction_mode()) {
        SendVrDeleteCommand (app);
      }
      app->set_perform_interaction_mode(-1);
      app->DeletePerformInteractionChoiceSetMap();
      app->set_perform_interaction_active(0);
    }
    (*message_)[strings::params][strings::function_id] =
            mobile_apis::FunctionID::PerformInteractionID;
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params = message[strings::msg_params];
    bool result_code = false;
    const hmi_apis::Common_Result::eType code =
        static_cast<hmi_apis::Common_Result::eType>(
            message[strings::params][hmi_response::code].asInt());
    if (hmi_apis::Common_Result::SUCCESS == code) {
      msg_params[strings::trigger_source] = trigger_source_;
      result_code = true;
    }
    SendResponse(result_code, static_cast<mobile_apis::Result::eType>(code),
                   NULL, &(msg_params));
}

void PerformInteractionRequest::SendVRAddCommandRequest(
    Application* const app) {
  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  if (InteractionMode::VR_ONLY
      == (*message_)[strings::msg_params][strings::interaction_mode].asInt()) {
    // TODO(DK): We need subscribe perform interaction with on command notification
    /*CreateHMIRequest(hmi_apis::FunctionID::UI_PerformInteraction,
     smart_objects::SmartObject(smart_objects::SmartType_Map), true, 1);*/
  }

  for (size_t i = 0; i < choice_list.length(); ++i) {
    smart_objects::SmartObject* choice_set = app->FindChoiceSet(
        choice_list[i].asInt());

    if (choice_set) {
      if (InteractionMode::VR_ONLY
          == (*message_)[strings::msg_params]
                         [strings::interaction_mode].asInt()) {
        // save perform interaction choice set
        app->AddPerformInteractionChoiceSet(choice_list[i].asInt(),
                                            *choice_set);
      }

      for (size_t j = 0; j < (*choice_set)[strings::choice_set].length(); ++j) {
        smart_objects::SmartObject msg_params = smart_objects::SmartObject(
            smart_objects::SmartType_Map);
        msg_params[strings::app_id] = app->app_id();
        msg_params[strings::cmd_id] =
            (*choice_set)[strings::choice_set][j][strings::choice_id];
        msg_params[strings::vr_commands] = smart_objects::SmartObject(
            smart_objects::SmartType_Array);
        msg_params[strings::vr_commands] =
            (*choice_set)[strings::choice_set][j][strings::vr_commands];

        CreateHMIRequest(hmi_apis::FunctionID::VR_AddCommand, msg_params,
                         false);
      }
    }
  }
}

void PerformInteractionRequest::SendUIPerformInteractionRequest(
    Application* const app) {
  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[hmi_request::initial_text][hmi_request::field_name] =
      TextFieldName::INITIAL_INTERACTION_TEXT;
  msg_params[hmi_request::initial_text][hmi_request::field_text] =
      (*message_)[strings::msg_params][hmi_request::initial_text];

  msg_params[strings::timeout] = default_timeout_;
  msg_params[strings::app_id] = app->app_id();

  msg_params[strings::choice_set] = smart_objects::SmartObject(
      smart_objects::SmartType_Array);

  for (size_t i = 0; i < choice_list.length(); ++i) {
    smart_objects::SmartObject* choice_set = app->FindChoiceSet(
        choice_list[i].asInt());
    if (choice_set) {
      // save perform interaction choice set
      app->AddPerformInteractionChoiceSet(choice_list[i].asInt(), *choice_set);
      for (size_t j = 0; j < (*choice_set)[strings::choice_set].length(); ++j) {
        int index = msg_params[strings::choice_set].length();
        msg_params[strings::choice_set][index] =
            (*choice_set)[strings::choice_set][j];
      }
    }
  }
  if((*message_)[strings::msg_params].
        keyExists(hmi_request::interaction_layout)) {
    msg_params[hmi_request::interaction_layout] =
        (*message_)[strings::msg_params][hmi_request::interaction_layout].
        asInt();
  }
  CreateUIPerformInteraction(msg_params, app);
}

void PerformInteractionRequest::CreateUIPerformInteraction(
    const smart_objects::SmartObject& msg_params, Application* const app) {
  SendHMIRequest(hmi_apis::FunctionID::UI_PerformInteraction,
                     &msg_params, true);
}

void PerformInteractionRequest::SendTTSSpeakRequest(Application* const app) {
  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[strings::tts_chunks] =
      (*message_)[strings::msg_params][strings::initial_prompt];
  msg_params[strings::app_id] = app->app_id();

  CreateHMIRequest(hmi_apis::FunctionID::TTS_Speak, msg_params, false);
}

void PerformInteractionRequest::SendUIShowVRHelpRequest(
    Application* const app) {
  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();
  msg_params[strings::vr_help_title] =
      (*message_)[strings::msg_params][strings::initial_text].asString();

  if ((*message_)[strings::msg_params].keyExists(strings::vr_help)) {
    msg_params[strings::vr_help] =
        (*message_)[strings::msg_params][strings::vr_help];
  } else {
    // copy choice set VR synonyms
    int index = 0;
    for (int i = 0; i < choice_list.length(); ++i) {
      smart_objects::SmartObject* choice_set = app->FindChoiceSet(
          choice_list[i].asInt());
      if (choice_set) {
        for (int j = 0; j < (*choice_set)[strings::choice_set].length(); ++j) {
          smart_objects::SmartObject& vr_commands =
              (*choice_set)[strings::choice_set][j][strings::vr_commands];
          if (0 < vr_commands.length()) {
            // copy only first synonym
            smart_objects::SmartObject item(smart_objects::SmartType_Map);
            item[strings::text] = vr_commands[0].asString();
            item[strings::position] = index;
            msg_params[strings::vr_help][index++] = item;
          }
        }
      }
    }
  }

  CreateHMIRequest(hmi_apis::FunctionID::UI_ShowVrHelp, msg_params, false);
}

bool PerformInteractionRequest::CheckChoiceSetMenuNames(
    Application* const app) {
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
    Application* const app) {
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
    Application* const app) {

  if (!(*message_)[strings::msg_params].keyExists(strings::vr_help)) {
    LOG4CXX_INFO(logger_, ""
        "PerformInteractionRequest::CheckVrHelpItemPositions vr_help omitted");
    return true;
  }

  smart_objects::SmartObject& vr_help =
      (*message_)[strings::msg_params][strings::vr_help];

  int position = 1;
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

}  // namespace commands

}  // namespace application_manager

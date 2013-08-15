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

#include <string>
#include <string.h>
#include "application_manager/commands/mobile/perform_interaction_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

PerformInteractionRequest::PerformInteractionRequest(
  const MessageSharedPtr& message): CommandRequestImpl(message) {
}

PerformInteractionRequest::~PerformInteractionRequest() {
}

void PerformInteractionRequest::Run() {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::Run");

  Application* app =
    ApplicationManagerImpl::instance()->
    application((*message_)[strings::params][strings::connection_key]);

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (!MessageHelper::VerifyImageFiles((*message_)[strings::msg_params], app)) {
    LOG4CXX_ERROR_EXT(logger_, "INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
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

  switch ((*message_)[strings::msg_params][strings::interaction_mode].asInt()) {
    case InteractionMode::BOTH: {
      if (!CheckChoiceSetVRSynonyms(app)) {
        return;
      }

      if (!CheckChoiceSetMenuNames(app)) {
        return;
      }

      app->set_perform_interaction_active(true);
      SendVRAddCommandRequest(app);
      SendUIPerformInteractionRequest(app);
      break;
    }
    case InteractionMode::MANUAL_ONLY: {
      if (!CheckChoiceSetMenuNames(app)) {
        return;
      }

      app->set_perform_interaction_active(true);
      SendUIPerformInteractionRequest(app);
      break;
    }
    case InteractionMode::VR_ONLY: {
      if (!CheckChoiceSetVRSynonyms(app)) {
        return;
      }

      // TODO (DK): need to implement timeout
      app->set_perform_interaction_active(true);
      SendUIShowVRHelpRequest(app);
      SendVRAddCommandRequest(app);
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Unknown interaction mode");
      return;
    }
  }

  SendTTSSpeakRequest(app);

  // TODO (DK): need to implement timeout TTS speak request.
}

void PerformInteractionRequest::SendVRAddCommandRequest(
    Application* const app) {
  smart_objects::SmartObject& choice_list =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  for (size_t i = 0; i < choice_list.length(); ++i) {
    smart_objects::SmartObject* choice_set =
      app->FindChoiceSet(choice_list[i].asInt());

    if (choice_set) {
      for (size_t j = 0; j < (*choice_set)[strings::choice_set].length(); ++j) {
        smart_objects::SmartObject msg_params =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
        msg_params[strings::app_id] = app->app_id();
        msg_params[strings::cmd_id] =
          (*choice_set)[strings::choice_set][j][strings::choice_id];
        msg_params[strings::vr_commands] =
          smart_objects::SmartObject(smart_objects::SmartType_Array);
        msg_params[strings::vr_commands] =
          (*choice_set)[strings::choice_set][j][strings::vr_commands];

        CreateHMIRequest(hmi_apis::FunctionID::VR_AddCommand,
                         msg_params, false);
      }
    }
  }
}

void PerformInteractionRequest::SendUIPerformInteractionRequest(
  Application* const app) {
  smart_objects::SmartObject& choice_list =
    (*message_)[strings::msg_params][strings::interaction_choice_set_id_list];

  smart_objects::SmartObject msg_params =
    smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[hmi_request::initial_text][hmi_request::field_name] =
    TextFieldName::INITIAL_INTERACTION_TEXT;
  msg_params[hmi_request::initial_text][hmi_request::field_text] =
    (*message_)[strings::msg_params][hmi_request::initial_text];

  if ((*message_)[strings::msg_params].keyExists(strings::timeout)) {
    msg_params[strings::timeout] =
        (*message_)[strings::msg_params][strings::timeout];
  }
  else {
    msg_params[strings::timeout] = 10000;
  }

  msg_params[strings::app_id] = app->app_id();

  msg_params[strings::choice_set] =
    smart_objects::SmartObject(smart_objects::SmartType_Array);

  for (size_t i = 0; i < choice_list.length(); ++i) {
    smart_objects::SmartObject* choice_set =
      app->FindChoiceSet(choice_list[i].asInt());
    if (choice_set) {
      // save perform interaction choice set
      app->AddPerformInteractionChoiceSet(choice_list[i].asInt(), *choice_set);
      for (size_t j = 0; j < (*choice_set)[strings::choice_set].length(); ++j) {
        int index = msg_params[strings::choice_set].length();
        msg_params[strings::choice_set][index] =
            (*choice_set)[strings::choice_set][j];
        std::string file_path = file_system::FullPath(app->name());
        file_path += "/";
        file_path += msg_params[strings::choice_set][index]
            [strings::image][strings::value].asString();
        msg_params[strings::choice_set][index][strings::image][strings::value] =
            file_path;
      }
    }
  }

  CreateHMIRequest(hmi_apis::FunctionID::UI_PerformInteraction,
                   msg_params, true, 1);
}

void PerformInteractionRequest::SendTTSSpeakRequest(Application* const app) {
  smart_objects::SmartObject msg_params =
    smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::tts_chunks] =
      (*message_)[strings::msg_params][strings::initial_prompt];
  msg_params[strings::app_id] = app->app_id();

  CreateHMIRequest(hmi_apis::FunctionID::TTS_Speak, msg_params, false);
}

void PerformInteractionRequest::SendUIShowVRHelpRequest(
    Application* const app) {
  /*
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::vr_help_title] = (*app->vr_help_title());
  msg_params[strings::vr_help] = (*app->vr_help());
  msg_params[strings::app_id] = app->app_id();

  CreateHMIRequest(hmi_apis::FunctionID::UI_ShowVrHelp, msg_params, false);
  */
}

bool PerformInteractionRequest::CheckChoiceSetMenuNames(
    Application* const app) {
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
          std::string ii_menu_name = (*i_choice_set)[strings::choice_set][ii]
              [strings::menu_name].asString();
          std::string jj_menu_name = (*j_choice_set)[strings::choice_set][jj]
              [strings::menu_name].asString();

          if (ii_menu_name == jj_menu_name) {
            LOG4CXX_ERROR(logger_,"Choice set has duplicated menu name");
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

}  // namespace commands

}  // namespace application_manager

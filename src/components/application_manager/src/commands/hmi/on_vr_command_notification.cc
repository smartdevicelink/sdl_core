/**
 * Copyright (c) 2013, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/hmi/on_vr_command_notification.h"
#include "application_manager/application_manager_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"

namespace application_manager {

namespace commands {

OnVRCommandNotification::OnVRCommandNotification(
  const MessageSharedPtr& message)
  : NotificationFromHMI(message) {
}

OnVRCommandNotification::~OnVRCommandNotification() {
}

void OnVRCommandNotification::Run() {
  LOG4CXX_INFO(logger_, "OnVRCommandNotification::Run");

  const unsigned int cmd_id = (*message_)[strings::msg_params][strings::cmd_id]
                              .asUInt();

  // Check if this is one of standart VR commands (i.e. "Help")
  unsigned int max_cmd_id = profile::Profile::instance()->max_cmd_id();
  if (cmd_id > max_cmd_id) {
    if (max_cmd_id + 1 == cmd_id) {
      Application* active_app = ApplicationManagerImpl::instance()
                                ->active_application();
      MessageHelper::SendShowVrHelpToHMI(active_app);
    } else {
      Application* app = ApplicationManagerImpl::instance()->application(
                           cmd_id - max_cmd_id);
      if (app) {
        ApplicationManagerImpl::instance()->ActivateApplication(app);
        MessageHelper::SendActivateAppToHMI(app);
      }
    }
    return;
  }

  const unsigned int app_id = (*message_)[strings::msg_params][strings::app_id]
                              .asUInt();
  Application* app = ApplicationManagerImpl::instance()->application(app_id);

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  /* check if perform interaction is active
   * if it is active we should sent to HMI DeleteCommand request
   * and PerformInterActionResponse to mobile
   */
  if (0 != app->is_perform_interaction_active()
      && PerformInteractionHandling(app)) {
  } else {
    (*message_)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::eType::OnCommandID;

    (*message_)[strings::msg_params][strings::trigger_source] =
      mobile_apis::TriggerSource::TS_VR;
    SendNotificationToMobile(message_);
  }
}

bool OnVRCommandNotification::PerformInteractionHandling(
  Application* const app) {
  LOG4CXX_INFO(logger_, "OnVRCommandNotification::PerformInteractionHandling");
  int cmd_id = (*message_)[strings::msg_params][strings::cmd_id].asInt();

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

    // send response
    smart_objects::SmartObject* p_i_response_so =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);
    if (!p_i_response_so) {
      LOG4CXX_ERROR(
        logger_,
        "Failed to allocate memory for perform interaction response.");
    }
    smart_objects::SmartObject& p_i_response = *p_i_response_so;
    p_i_response[strings::params][strings::function_id] =
      mobile_apis::FunctionID::PerformInteractionID;
    p_i_response[strings::params][strings::message_type] =
      mobile_apis::messageType::response;
    p_i_response[strings::params][strings::connection_key] = app->app_id();
    p_i_response[strings::params][strings::correlation_id] = app
        ->is_perform_interaction_active();
    app->set_perform_interaction_active(0);
    p_i_response[strings::params][hmi_response::code] =
      hmi_apis::Common_Result::SUCCESS;
    p_i_response[strings::msg_params][strings::choice_id] = cmd_id;
    p_i_response[strings::msg_params][strings::trigger_source] =
      mobile_apis::TriggerSource::TS_VR;
    ApplicationManagerImpl::instance()->ManageMobileCommand(p_i_response_so);

    MessageChaining* chain =
      ApplicationManagerImpl::instance()->GetMessageChain(
        app->perform_interaction_ui_corrid());
    if (chain) {
      unsigned int mobile_cor_id = chain->correlation_id();
      ApplicationManagerImpl::instance()->DecreaseMessageChain(
        app->perform_interaction_ui_corrid(), mobile_cor_id);
      app->set_perform_interaction_ui_corrid(0);
    }

    for (PerformChoiceSetMap::const_iterator it = choice_set_map.begin();
         choice_set_map.end() != it; ++it) {
      const smart_objects::SmartObject& choice_set = (*it->second).getElement(
            strings::choice_set);

      for (size_t j = 0; j < choice_set.length(); ++j) {
        smart_objects::SmartObject msg_params = smart_objects::SmartObject(
            smart_objects::SmartType_Map);
        msg_params[strings::app_id] = app->app_id();
        msg_params[strings::cmd_id] = choice_set.getElement(j).getElement(
                                        strings::choice_id);

        CreateHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, msg_params);
      }
    }

    app->DeletePerformInteractionChoiceSetMap();

    CreateHMIRequest(hmi_apis::FunctionID::UI_ClosePopUp,
                     smart_objects::SmartObject(smart_objects::SmartType_Map));
    app->set_perform_interaction_mode(-1);
  }

  return choice_id_chosen;
}

}  // namespace commands

}  // namespace application_manager


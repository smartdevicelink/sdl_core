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
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "application_manager/message_helper.h"
#include "config_profile/profile.h"

namespace application_manager {

namespace commands {

OnVRCommandNotification::OnVRCommandNotification(
  const MessageSharedPtr& message): NotificationFromHMI(message) {
}

OnVRCommandNotification::~OnVRCommandNotification() {
}

void OnVRCommandNotification::Run() {
  LOG4CXX_INFO(logger_, "OnVRCommandNotification::Run");

  const unsigned int cmd_id =
    (*message_)[strings::msg_params][strings::cmd_id].asUInt();

  // Check if this is one of standart VR commands (i.e. "Help")
  unsigned int max_cmd_id = profile::Profile::instance()->max_cmd_id();
  if (cmd_id > max_cmd_id) {
    if (max_cmd_id + 1 == cmd_id) {
      Application* active_app =
        ApplicationManagerImpl::instance()->active_application();
      MessageHelper::SendShowVrHelpToHMI(active_app);
    } else {
      Application* app = ApplicationManagerImpl::instance()->application(
                           cmd_id - max_cmd_id);
      if (app) {
        ApplicationManagerImpl::instance()->ActivateApplication(app);
      }
    }
    return;
  }

  const unsigned int app_id =
    (*message_)[strings::msg_params][strings::app_id].asUInt();
  Application* app = ApplicationManagerImpl::instance()->
                     application(app_id);

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  /* check if perform interaction is active
   * if it is active we should sent to HMI DeleteCommand request
   * and PerformInterActionResponse to mobile
   */
  if (app->is_perform_interaction_active()) {
    app->set_perform_interaction_choice(
        (*message_)[strings::msg_params][strings::cmd_id].asInt());
    const PerformChoiceSetMap& choice_set_map =
      app->GetPerformInteractionChoiceSetMap();

    PerformChoiceSetMap::const_iterator it = choice_set_map.begin();
    for (; choice_set_map.end() != it; ++it) {
      const smart_objects::SmartObject& choice_set =
        (*it->second).getElement(strings::choice_set);

      for (size_t j = 0; j < choice_set.length(); ++j) {
        smart_objects::SmartObject msg_params =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
        msg_params[strings::app_id] = app->app_id();
        msg_params[strings::cmd_id] =
          choice_set.getElement(j).getElement(strings::choice_id);

        CreateHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, msg_params);
      }
    }

    CreateHMIRequest(hmi_apis::FunctionID::UI_ClosePopUp,
                     smart_objects::SmartObject(smart_objects::SmartType_Map));

  } else {
    (*message_)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::eType::OnCommandID;

    (*message_)[strings::msg_params][strings::trigger_source] =
      mobile_apis::TriggerSource::TS_VR;
    SendNotificationToMobile(message_);
  }
}

}  // namespace commands

}  // namespace application_manager


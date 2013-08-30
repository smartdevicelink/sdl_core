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
#include "application_manager/commands/hmi/ui_perform_interaction_response.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_chaining.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

UIPerformInteractionResponse::UIPerformInteractionResponse(
  const MessageSharedPtr& message): ResponseFromHMI(message) {
}

UIPerformInteractionResponse::~UIPerformInteractionResponse() {
}

void UIPerformInteractionResponse::Run() {
  LOG4CXX_INFO(logger_, "UIPerformInteractionResponse::Run");

  const unsigned int correlation_id =
    (*message_)[strings::params][strings::correlation_id].asUInt();

  MessageChaining* msg_chain =
    ApplicationManagerImpl::instance()->GetMessageChain(correlation_id);

  if (NULL == msg_chain) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  /* store received response code for to check it
   * in corresponding Mobile response
   */
  const hmi_apis::Common_Result::eType code =
    static_cast<hmi_apis::Common_Result::eType>(
      (*message_)[strings::params][hmi_response::code].asInt());

  msg_chain->set_ui_response_result(code);

  const int connection_key =  msg_chain->connection_key();
  Application* app = ApplicationManagerImpl::instance()->
                     application(connection_key);

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  if (app->is_perform_interaction_active()) {
    const PerformChoiceSetMap& choice_set_map =
      app->GetPerformInteractionChoiceSetMap();

    if (mobile_apis::InteractionMode::MANUAL_ONLY !=
        app->perform_interaction_mode()) {
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
    }
    app->set_perform_interaction_mode(-1);
    app->DeletePerformInteractionChoiceSetMap();
    app->set_perform_interaction_active(0);

    if (hmi_apis::Common_Result::ABORTED ==
        (*message_)[strings::params][hmi_response::code].asInt()) {
    }
  }

  (*message_)[strings::msg_params][strings::trigger_source] =
    mobile_apis::TriggerSource::TS_MENU;

  // prepare SmartObject for mobile factory
  (*message_)[strings::params][strings::function_id] =
    mobile_apis::FunctionID::PerformInteractionID;

  SendResponseToMobile(message_);
}

}  // namespace commands

}  // namespace application_manager

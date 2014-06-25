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

#include "application_manager/commands/mobile/delete_interaction_choice_set_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "application_manager/message_helper.h"
namespace application_manager {

namespace commands {

DeleteInteractionChoiceSetRequest::DeleteInteractionChoiceSetRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

DeleteInteractionChoiceSetRequest::~DeleteInteractionChoiceSetRequest() {
}

void DeleteInteractionChoiceSetRequest::Run() {
  LOG4CXX_INFO(logger_, "DeleteInteractionChoiceSetRequest::Run");

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    LOG4CXX_ERROR_EXT(logger_, "No application associated with session key");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const int32_t choise_set_id =
      (*message_)[strings::msg_params]
                  [strings::interaction_choice_set_id].asInt();

  if (!app->FindChoiceSet(choise_set_id)) {
    LOG4CXX_ERROR_EXT(logger_, "INVALID_ID");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  if (ChoiceSetInUse(app)) {
    LOG4CXX_ERROR_EXT(logger_, "Choice set currently in use");
    SendResponse(false, mobile_apis::Result::IN_USE);
    return;
  }
  SendVrDeleteCommand(app);

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);

  msg_params[strings::interaction_choice_set_id] = choise_set_id;
  msg_params[strings::app_id] = app->app_id();

  app->RemoveChoiceSet(choise_set_id);

  SendResponse(true, mobile_apis::Result::SUCCESS);
  /*CreateHMIRequest(hmi_apis::FunctionID::UI_DeleteInteractionChoiceSet,
   msg_params, true);*/
}

bool DeleteInteractionChoiceSetRequest::ChoiceSetInUse(ApplicationConstSharedPtr app) {
  if (app->is_perform_interaction_active()) {
    // retrieve stored choice sets for perform interaction
    const PerformChoiceSetMap& choice_set_map = app
        ->performinteraction_choice_set_map();

    PerformChoiceSetMap::const_iterator it = choice_set_map.begin();
    for (; choice_set_map.end() != it; ++it) {
      if (it->first
          == (*message_)[strings::msg_params]
                         [strings::interaction_choice_set_id].asUInt()) {
        LOG4CXX_ERROR_EXT(logger_,
                          "DeleteInteractionChoiceSetRequest::ChoiceSetInUse");
        return true;
      }
    }
  }
  return false;
}

void DeleteInteractionChoiceSetRequest::SendVrDeleteCommand(
    application_manager::ApplicationSharedPtr app) {
  LOG4CXX_INFO(logger_, "PerformInteractionRequest::SendVrDeleteCommand");

  smart_objects::SmartObject* choice_set =
                              app->FindChoiceSet((*message_)[strings::msg_params]
                               [strings::interaction_choice_set_id].asInt());

  if (choice_set) {
    smart_objects::SmartObject msg_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);
    msg_params[strings::app_id] = app->app_id();
    msg_params[strings::type] = hmi_apis::Common_VRCommandType::Choice;
    msg_params[strings::grammar_id] = (*choice_set)[strings::grammar_id];
    choice_set = &((*choice_set)[strings::choice_set]);
    for (uint32_t i = 0; i < (*choice_set).length() ; ++i) {
      msg_params[strings::cmd_id] = (*choice_set)[i][strings::choice_id];
      SendHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, &msg_params);
    }
  }
}

}  // namespace commands

}  // namespace application_manager

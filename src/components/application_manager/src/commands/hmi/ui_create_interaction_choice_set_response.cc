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
#include "application_manager/commands/hmi/ui_create_interaction_choice_set_response.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_chaining.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

UICreateInteractionChoiceSetResponse::UICreateInteractionChoiceSetResponse(
    const MessageSharedPtr& message): ResponseFromHMI(message) {
}

UICreateInteractionChoiceSetResponse::~UICreateInteractionChoiceSetResponse() {
}

void UICreateInteractionChoiceSetResponse::Run() {
  LOG4CXX_INFO(logger_, "UICreateInteractionChoiceSetResponse::Run");

  const uint64_t correlation_id =
      (*message_)[strings::params][strings::correlation_id].asUint64();

  MessageChaining* msg_chain =
    ApplicationManagerImpl::instance()->GetMessageChain(correlation_id);

  if (NULL == msg_chain) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  smart_objects::SmartObject data =
    msg_chain->data();

  /* store received response code for to check it
   * in corresponding Mobile response
   */
  const hmi_apis::Common_Result::eType code =
    static_cast<hmi_apis::Common_Result::eType>(
      (*message_)[strings::params][hmi_response::code].asInt());

  msg_chain->set_ui_response_result(code);

  int app_id = (*message_)[strings::params][strings::connection_key];
  ApplicationImpl* app = static_cast<ApplicationImpl*>(
                           ApplicationManagerImpl::instance()->
                           application(app_id));

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }

  const int choice_set_id =
      data[strings::msg_params][strings::interaction_choice_set_id].asInt();

  if (hmi_apis::Common_Result::SUCCESS == code) {
    app->AddChoiceSet(choice_set_id, data[strings::msg_params]);
  }

  smart_objects::SmartObject& choice_set =
      data[strings::msg_params][strings::choice_set];

  app->AddChoiceSetVRCommands(choice_set_id, data[strings::msg_params]);

  // prepare SmartObject for mobile factory
  (*message_)[strings::params][strings::function_id] =
    mobile_apis::FunctionID::CreateInteractionChoiceSetID;

  SendResponseToMobile(message_);
}

}  // namespace commands

}  // namespace application_manager

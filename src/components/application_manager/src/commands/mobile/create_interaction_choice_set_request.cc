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

#include "application_manager/commands/mobile/create_interaction_choice_set_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

CreateInteractionChoiceSetRequest::CreateInteractionChoiceSetRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

CreateInteractionChoiceSetRequest::~CreateInteractionChoiceSetRequest() {
}

void CreateInteractionChoiceSetRequest::Run() {
  LOG4CXX_INFO(logger_, "CreateInteractionChoiceSetRequest::Run");

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[strings::params][strings::connection_key]));

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }


  const int choise_set_id =
  (*message_)[strings::msg_params][strings::interaction_choice_set_id].asInt();

  if (app->FindChoiceSet(choise_set_id)) {
    LOG4CXX_ERROR(logger_, "Invalid ID");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  if ((false == CheckChoiceSetMenuNames()) ||
      (false == CheckChoiceSetVRSynonyms())) {
    SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
    return;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params = (*message_)[strings::msg_params];
  msg_params[strings::app_id] = app->app_id();

  SendResponse(true, mobile_apis::Result::SUCCESS);
}

bool CreateInteractionChoiceSetRequest::CheckChoiceSetMenuNames() {
  smart_objects::SmartObject& choice_set =
      (*message_)[strings::msg_params][strings::choice_set];

  for (size_t i = 0; i < choice_set.length(); ++i) {
    for (size_t j = 0; j < choice_set.length(); ++j) {
      if (i == j) {
        // skip check the same element
        continue;
      }
      if (choice_set[i][strings::menu_name].asString() ==
           choice_set[j][strings::menu_name].asString()) {
        LOG4CXX_ERROR(logger_, "Incoming choiceset has duplicated menu name " <<
                      choice_set[i][strings::menu_name].asString() << " " <<
                      choice_set[j][strings::menu_name].asString());
        return false;
      }
    }
  }
  return true;
}

bool CreateInteractionChoiceSetRequest::CheckChoiceSetVRSynonyms() {
  smart_objects::SmartObject& choice_set =
      (*message_)[strings::msg_params][strings::choice_set];

  for (size_t i = 0; i < choice_set.length(); ++i) {
    for (size_t j = 0; j < choice_set.length(); ++j) {
      for (size_t ii = 0; ii < choice_set[i][strings::vr_commands].length();
          ++ii) {
        for (size_t jj = 0; jj < choice_set[j][strings::vr_commands].length();
            ++jj) {
          if ((i == j) && (ii == jj)) {
            // skip check the same element
            continue;
          }
          if (choice_set[i][strings::vr_commands][ii].asString() ==
              choice_set[j][strings::vr_commands][jj].asString()) {
            LOG4CXX_ERROR(logger_, "Choice set has duplicated VR synonym " <<
                          choice_set[i][strings::vr_commands][ii].asString() <<
                          " " <<
                          choice_set[j][strings::vr_commands][jj].asString());
            return false;
          }
        }
      }
    }
  }
  return true;
}

}  // namespace commands

}  // namespace application_manager

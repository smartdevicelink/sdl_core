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
#include "application_manager/commands/mobile/create_interaction_choice_set_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

CreateInteractionChoiceSetRequest::CreateInteractionChoiceSetRequest(
    const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

CreateInteractionChoiceSetRequest::~CreateInteractionChoiceSetRequest() {
}

void CreateInteractionChoiceSetRequest::Run() {
  LOG4CXX_INFO(logger_, "CreateInteractionChoiceSetRequest::Run");

  Application* app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::params][strings::connection_key]);

  if (NULL == app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
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

  const int choise_set_id =
      (*message_)[strings::msg_params]
                  [strings::interaction_choice_set_id].asInt();

  if (app->FindChoiceSet(choise_set_id) || false == CheckChoiceID(app)) {
    LOG4CXX_ERROR(logger_, "Invalid ID");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  if ((false == CheckChoiceSetMenuNames())
      || (false == CheckChoiceSetVRSynonyms())) {
    SendResponse(false, mobile_apis::Result::DUPLICATE_NAME);
    return;
  }

  const int choice_set_id =
      (*message_)[strings::msg_params]
                  [strings::interaction_choice_set_id].asInt();

  app->AddChoiceSet(choice_set_id, (*message_)[strings::msg_params]);

  SendResponse(true, mobile_apis::Result::SUCCESS);
}

bool CreateInteractionChoiceSetRequest::CheckChoiceSetMenuNames() {
  LOG4CXX_INFO(logger_,
               "CreateInteractionChoiceSetRequest::CheckChoiceSetMenuNames");
  smart_objects::SmartObject& choice_set =
      (*message_)[strings::msg_params][strings::choice_set];

  for (size_t i = 0; i < choice_set.length(); ++i) {
    for (size_t j = i + 1; j < choice_set.length(); ++j) {
      if (choice_set[i][strings::menu_name].asString()
          == choice_set[j][strings::menu_name].asString()) {
        LOG4CXX_ERROR(
            logger_,
            "Incoming choiceset has duplicated menu name "
                << choice_set[i][strings::menu_name].asString() << " "
                << choice_set[j][strings::menu_name].asString());
        return false;
      }
    }
  }
  return true;
}

bool CreateInteractionChoiceSetRequest::CheckChoiceSetVRSynonyms() {
  LOG4CXX_INFO(logger_,
               "CreateInteractionChoiceSetRequest::CheckChoiceSetVRSynonyms");
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
          std::string vr_cmd_i = choice_set[i][strings::vr_commands][ii]
              .asString();
          std::string vr_cmd_j = choice_set[j][strings::vr_commands][jj]
              .asString();
          if (0 == strcasecmp(vr_cmd_i.c_str(), vr_cmd_j.c_str())) {
            LOG4CXX_ERROR(
                logger_,
                "Choice set has duplicated VR synonym " << vr_cmd_i << " and "
                    << vr_cmd_j);
            return false;
          }
        }
      }
    }
  }
  return true;
}

bool CreateInteractionChoiceSetRequest::CheckChoiceID(const Application* app) {
  LOG4CXX_INFO(logger_, "CreateInteractionChoiceSetRequest::CheckChoiceID");
  smart_objects::SmartObject& choice_set =
      (*message_)[strings::msg_params][strings::choice_set];

  // check inside incoming request
  for (size_t i = 0; i < choice_set.length(); ++i) {
    for (size_t j = i + 1; j < choice_set.length(); ++j) {
      if (choice_set[i][strings::choice_id].asInt()
          == choice_set[j][strings::choice_id].asInt()) {
        LOG4CXX_ERROR(logger_, "Incoming choice set has duplicated choice ID");
        return false;
      }
    }
  }

  // check inside SDL application
  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return false;
  }

  const ChoiceSetMap& choice_set_map = app->choice_set_map();
  ChoiceSetMap::const_iterator it = choice_set_map.begin();

  for (; choice_set_map.end() != it; ++it) {
    for (size_t i = 0; i < (*it->second)[strings::choice_set].length(); ++i) {
      for (size_t j = 0; j < choice_set.length(); ++j) {
        if ((*it->second)[strings::choice_set][i][strings::choice_id].asInt()
            == choice_set[j][strings::choice_id].asInt()) {
          LOG4CXX_ERROR(logger_, "Incoming choice ID already exist");
          return false;
        }
      }
    }
  }

  return true;
}

}  // namespace commands

}  // namespace application_manager

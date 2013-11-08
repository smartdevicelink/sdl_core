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
#include <algorithm>
#include <vector>
#include "application_manager/commands/mobile/create_interaction_choice_set_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "smart_objects/smart_object.h"
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

  mobile_apis::Result::eType  result_ = mobile_apis::Result::SUCCESS;
  if (mobile_apis::Result::SUCCESS != verification_result) {
    if (mobile_apis::Result::INVALID_DATA == verification_result) {
      LOG4CXX_ERROR(logger_, "VerifyImageFiles INVALID_DATA!");
      SendResponse(false, verification_result);
      return;
    }
    if (mobile_apis::Result::UNSUPPORTED_RESOURCE == verification_result) {
      LOG4CXX_ERROR(logger_, "VerifyImageFiles UNSUPPORTED_RESOURCE!");
      result_ = verification_result;
    }
  }

  const int choice_set_id = (*message_)[strings::msg_params]
      [strings::interaction_choice_set_id].asInt();

  if (app->FindChoiceSet(choice_set_id)) {
    LOG4CXX_ERROR(logger_, "Invalid ID");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  mobile_apis::Result::eType result = CheckChoiceSet(app);
  if (mobile_apis::Result::SUCCESS != result) {
    SendResponse(false, result);
    return;
  }

  app->AddChoiceSet(choice_set_id, (*message_)[strings::msg_params]);

  SendResponse(true, result_);
}

mobile_apis::Result::eType CreateInteractionChoiceSetRequest::CheckChoiceSet(
    const Application* app) {
  LOG4CXX_INFO(logger_, "CreateInteractionChoiceSetRequest::CheckChoiceSet");

  smart_objects::SmartObject& choice_set =
      (*message_)[strings::msg_params][strings::choice_set];
  size_t length = choice_set.length();

  for (size_t i = 0; i < length; ++i) {
    for (size_t j = i + 1; j < length; ++j) {

      /* Checks if incoming choice set doesn't has similar menu names. */
      if (choice_set[i][strings::menu_name].asString()
          == choice_set[j][strings::menu_name].asString()) {
        LOG4CXX_ERROR(logger_, "Incoming choice set has duplicated menu name "
                      << choice_set[i][strings::menu_name].asString() << " "
                      << choice_set[j][strings::menu_name].asString());
        return mobile_apis::Result::DUPLICATE_NAME;
      }

      /* Checks if incoming choiceSet doesn't have duplicated choice ID's */
      if (choice_set[i][strings::choice_id].asInt()
          == choice_set[j][strings::choice_id].asInt()) {
        LOG4CXX_ERROR(logger_, "Incoming choice set has duplicated choice ID");
        return mobile_apis::Result::INVALID_ID;
      }

      if (!compareSynonyms(choice_set[i], choice_set[j])) {
        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }

    /* Choice ID doesn't exist in application choiceSet map */
    const ChoiceSetMap& choice_set_map = app->choice_set_map();
    ChoiceSetMap::const_iterator it = choice_set_map.begin();
    for (; choice_set_map.end() != it; ++it) {
      for (size_t i = 0; i < (*it->second)[strings::choice_set].length(); ++i) {
        if ((*it->second)[strings::choice_set][i][strings::choice_id].asInt()
            == choice_set[i][strings::choice_id].asInt()) {
          LOG4CXX_ERROR(logger_, "Incoming choice ID already exist");
          return mobile_apis::Result::INVALID_ID;
        }
      }
    }
  }

  return  mobile_apis::Result::SUCCESS;
}

bool CreateInteractionChoiceSetRequest::compareSynonyms(
    const NsSmartDeviceLink::NsSmartObjects::SmartObject& choice1,
    const NsSmartDeviceLink::NsSmartObjects::SmartObject& choice2) {

  if (choice1[strings::choice_id].asInt() ==
      choice2[strings::choice_id].asInt()) {
    return false;
  }

  smart_objects::SmartArray* vr_cmds_1 =
      choice1[strings::vr_commands].asArray();
  DCHECK(vr_cmds_1);
  smart_objects::SmartArray* vr_cmds_2 =
      choice2[strings::vr_commands].asArray();
  DCHECK(vr_cmds_2);

  smart_objects::SmartArray::iterator it;
  it = std::find_first_of(vr_cmds_1->begin(), vr_cmds_1->end(),
                          vr_cmds_2->begin(), vr_cmds_2->end(),
                          CreateInteractionChoiceSetRequest::compareStr);


  if (it != vr_cmds_1->end()) {
    LOG4CXX_INFO(logger_, "Incoming choice set has duplicated VR synonyms "
                 << it->asString());
    return false;
  }

  return true;
}

bool CreateInteractionChoiceSetRequest::compareStr(
    const NsSmartDeviceLink::NsSmartObjects::SmartObject& str1,
    const NsSmartDeviceLink::NsSmartObjects::SmartObject& str2) {

  return 0 == strcasecmp(str1.asString().c_str(), str2.asString().c_str());
}


}  // namespace commands

}  // namespace application_manager

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

namespace application_manager {

namespace commands {

CreateInteractionChoiceSetRequest::CreateInteractionChoiceSetRequest(
  const MessageSharedPtr& message)
  : CommandRequestImpl(message),
    expected_chs_count_(0),
    received_chs_count_(0),
    error_from_hmi_(false) {
}

CreateInteractionChoiceSetRequest::~CreateInteractionChoiceSetRequest() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void CreateInteractionChoiceSetRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
                (*message_)[strings::params][strings::connection_key].asUInt());

  if (!app) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }
  for (uint32_t i = 0;
       i < (*message_)[strings::msg_params][strings::choice_set].length();
       ++i) {
    Result::eType verification_result_image = Result::SUCCESS;
    Result::eType verification_result_secondary_image = Result::SUCCESS;
    if ((*message_)[strings::msg_params]
        [strings::choice_set][i].keyExists(strings::image)) {
      verification_result_image = MessageHelper::VerifyImage(
                           (*message_)[strings::msg_params][strings::choice_set]
                           [i][strings::image], app);
    }
    if ((*message_)[strings::msg_params]
        [strings::choice_set][i].keyExists(strings::secondary_image)) {
      verification_result_secondary_image = MessageHelper::VerifyImage(
                           (*message_)[strings::msg_params][strings::choice_set]
                           [i][strings::secondary_image], app);
    }
    if (verification_result_image == Result::INVALID_DATA ||
        verification_result_secondary_image == Result::INVALID_DATA) {
      LOG4CXX_ERROR(logger_, "VerifyImage INVALID_DATA!");
        SendResponse(false, Result::INVALID_DATA);
      return;
    }
  }

  choice_set_id_ = (*message_)[strings::msg_params]
                   [strings::interaction_choice_set_id].asInt();

  if (app->FindChoiceSet(choice_set_id_)) {
    LOG4CXX_ERROR(logger_, "Invalid ID");
    SendResponse(false, Result::INVALID_ID);
    return;
  }

  Result::eType result = CheckChoiceSet(app);
  if (Result::SUCCESS != result) {
    SendResponse(false, result);
    return;
  }
  uint32_t grammar_id = ApplicationManagerImpl::instance()->GenerateGrammarID();
  (*message_)[strings::msg_params][strings::grammar_id] = grammar_id;
  app->AddChoiceSet(choice_set_id_, (*message_)[strings::msg_params]);
  SendVRAddCommandRequests(app);
}

mobile_apis::Result::eType CreateInteractionChoiceSetRequest::CheckChoiceSet(
  ApplicationConstSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);

  const smart_objects::SmartArray* new_choice_set_array =
    (*message_)[strings::msg_params][strings::choice_set].asArray();

  smart_objects::SmartArray::const_iterator it_array =
    new_choice_set_array->begin();

  smart_objects::SmartArray::const_iterator it_array_end =
    new_choice_set_array->end();

  // Self check of new choice set for params coincidence
  for (; it_array != it_array_end; ++it_array) {
    const smart_objects::SmartArray* vr_array =
      (*it_array)[strings::vr_commands].asArray();

    CoincidencePredicateChoiceID c((*it_array)[strings::choice_id].asInt());
    if (1 != std::count_if(
          new_choice_set_array->begin(),
          new_choice_set_array->end(), c)) {
      LOG4CXX_ERROR(logger_, "Incoming choice set has duplicate IDs.");
      return mobile_apis::Result::INVALID_ID;
    }

    // Check new choice set params along with already registered choice sets
    const DataAccessor<ChoiceSetMap> accessor = app->choice_set_map();
    const ChoiceSetMap& app_choice_set_map = accessor.GetData();
    ChoiceSetMap::const_iterator it = app_choice_set_map.begin();
    ChoiceSetMap::const_iterator itEnd = app_choice_set_map.end();
    for (; it != itEnd; ++it) {
      const smart_objects::SmartObject* app_choice_set = it->second;
      if (NULL != app_choice_set) {
        const smart_objects::SmartArray* curr_choice_set =
          (*app_choice_set)[strings::choice_set].asArray();

        if (0 != std::count_if(
              curr_choice_set->begin(),
              curr_choice_set->end(),
              c)) {
          LOG4CXX_ERROR(logger_, "Incoming choice ID already exists.");
          return mobile_apis::Result::INVALID_ID;
        }
      }
    }

    CoincidencePredicateMenuName m((*it_array)[strings::menu_name].asString());
    if (1 != std::count_if(
          new_choice_set_array->begin(),
          new_choice_set_array->end(),
          m)) {
      LOG4CXX_ERROR(logger_, "Incoming choice set has duplicate menu names.");
      return mobile_apis::Result::DUPLICATE_NAME;
    }

    // Check coincidence inside the current choice

    smart_objects::SmartArray::const_iterator it_vr = vr_array->begin();
    smart_objects::SmartArray::const_iterator it_vr_end = vr_array->end();

    for (; it_vr != it_vr_end; ++it_vr) {
      CoincidencePredicateVRCommands v((*it_vr));
      if (1 != std::count_if(vr_array->begin(), vr_array->end(), v)) {
        LOG4CXX_ERROR(logger_,
                      "Incoming choice set has duplicate VR command(s)");

        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }

    // Check along with VR commands in other choices in the new set
    smart_objects::SmartArray::const_iterator it_same_array =
      new_choice_set_array->begin();

    smart_objects::SmartArray::const_iterator it_same_array_end =
      new_choice_set_array->end();

    for (; it_same_array != it_same_array_end; ++it_same_array) {
      // Skip check for itself
      if ((*it_array)[strings::choice_id] ==
          (*it_same_array)[strings::choice_id]) {
        continue;
      }

      if (compareSynonyms((*it_array), (*it_same_array))) {
        LOG4CXX_ERROR(logger_,
                      "Incoming choice set has duplicate VR command(s).");

        return mobile_apis::Result::DUPLICATE_NAME;
      }
    }

    if (IsWhiteSpaceExist((*it_array))) {
      LOG4CXX_ERROR(logger_,
                    "Incoming choice set has contains \t\n \\t \\n");
      return mobile_apis::Result::INVALID_DATA;
    }
  }

  return mobile_apis::Result::SUCCESS;
}

bool CreateInteractionChoiceSetRequest::compareSynonyms(
  const NsSmartDeviceLink::NsSmartObjects::SmartObject& choice1,
  const NsSmartDeviceLink::NsSmartObjects::SmartObject& choice2) {
  smart_objects::SmartArray* vr_cmds_1 =
    choice1[strings::vr_commands].asArray();
  DCHECK(vr_cmds_1 != NULL);
  smart_objects::SmartArray* vr_cmds_2 =
    choice2[strings::vr_commands].asArray();
  DCHECK(vr_cmds_2 != NULL);

  smart_objects::SmartArray::iterator it;
  it = std::find_first_of(vr_cmds_1->begin(), vr_cmds_1->end(),
                          vr_cmds_2->begin(), vr_cmds_2->end(),
                          CreateInteractionChoiceSetRequest::compareStr);


  if (it != vr_cmds_1->end()) {
    LOG4CXX_INFO(logger_, "Incoming choice set has duplicated VR synonyms "
                 << it->asString());
    return true;
  }

  return false;
}

bool CreateInteractionChoiceSetRequest::compareStr(
  const NsSmartDeviceLink::NsSmartObjects::SmartObject& str1,
  const NsSmartDeviceLink::NsSmartObjects::SmartObject& str2) {
  return 0 == strcasecmp(str1.asCharArray(), str2.asCharArray());
}

bool CreateInteractionChoiceSetRequest::IsWhiteSpaceExist(
  const smart_objects::SmartObject& choice_set) {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  str = choice_set[strings::menu_name].asCharArray();
  if (!CheckSyntax(str)) {
    LOG4CXX_ERROR(logger_, "Invalid menu_name syntax check failed");
    return true;
  }

  if (choice_set.keyExists(strings::secondary_text)) {
    str = choice_set[strings::secondary_text].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid secondary_text syntax check failed");
      return true;
    }
  }

  if (choice_set.keyExists(strings::tertiary_text)) {
    str = choice_set[strings::tertiary_text].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid tertiary_text syntax check failed");
      return true;
    }
  }

  if (choice_set.keyExists(strings::vr_commands)) {
    const size_t len =
      choice_set[strings::vr_commands].length();

    for (size_t i = 0; i < len; ++i) {
      str = choice_set[strings::vr_commands][i].asCharArray();
      if (!CheckSyntax(str)) {
        LOG4CXX_ERROR(logger_, "Invalid vr_commands syntax check failed");
        return true;
      }
    }
  }

  if (choice_set.keyExists(strings::image)) {
    str = choice_set[strings::image][strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid image value syntax check failed");
      return true;
    }
  }

  if (choice_set.keyExists(strings::secondary_image)) {
    str = choice_set[strings::secondary_image][strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_, "Invalid secondary_image value. "
                             "Syntax check failed");
      return true;
    }
  }
  return false;
}

void CreateInteractionChoiceSetRequest::SendVRAddCommandRequests(
  application_manager::ApplicationSharedPtr const app) {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject& choice_set = (*message_)[strings::msg_params];
  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params[strings::type] = hmi_apis::Common_VRCommandType::Choice;
  msg_params[strings::app_id] = app->app_id();
  msg_params[strings::grammar_id] =  choice_set[strings::grammar_id];
  const uint32_t choice_count = choice_set[strings::choice_set].length();
  SetAllowedToTerminate(false);

  expected_chs_count_ = choice_count;
  size_t chs_num = 0;
  for (; chs_num < choice_count; ++chs_num) {
    {
      sync_primitives::AutoLock error_lock(error_from_hmi_lock_);
      if (error_from_hmi_) {
        LOG4CXX_WARN(logger_, "Error from HMI received. Stop sending VRCommands");
        break;
      }
    }

    msg_params[strings::cmd_id] =
      choice_set[strings::choice_set][chs_num][strings::choice_id];
    msg_params[strings::vr_commands] = smart_objects::SmartObject(
                                         smart_objects::SmartType_Array);
    msg_params[strings::vr_commands] =
      choice_set[strings::choice_set][chs_num][strings::vr_commands];

    sync_primitives::AutoLock commands_lock(vr_commands_lock_);
    const uint32_t vr_cmd_id = msg_params[strings::cmd_id].asUInt();
    const uint32_t vr_corr_id =
        SendHMIRequest(hmi_apis::FunctionID::VR_AddCommand, &msg_params, true);

    VRCommandInfo vr_command(vr_cmd_id);
    sent_commands_map_[vr_corr_id] = vr_command;
    LOG4CXX_DEBUG(logger_, "VR_command sent corr_id "
                  << vr_corr_id << " cmd_id " << vr_corr_id);
  }
  expected_chs_count_ = chs_num;
  LOG4CXX_DEBUG(logger_, "expected_chs_count_ = " << expected_chs_count_);
}

void CreateInteractionChoiceSetRequest::on_event(
    const event_engine::Event& event) {
  using namespace hmi_apis;
  LOG4CXX_AUTO_TRACE(logger_);

  const smart_objects::SmartObject& message = event.smart_object();
  if (event.id() == hmi_apis::FunctionID::VR_AddCommand) {
    received_chs_count_++;
    LOG4CXX_DEBUG(logger_, "Got VR.AddCommand response, there are "
                  << expected_chs_count_ - received_chs_count_
                  << " more to wait.");

    uint32_t corr_id = static_cast<uint32_t>(message[strings::params]
        [strings::correlation_id].asUInt());
    SentCommandsMap::iterator it = sent_commands_map_.find(corr_id);
    if (sent_commands_map_.end() == it) {
      LOG4CXX_WARN(logger_, "HMI response for unknown VR command received");
      return;
    }

    Common_Result::eType  vr_result_ = static_cast<Common_Result::eType>(
        message[strings::params][hmi_response::code].asInt());
    if (Common_Result::SUCCESS == vr_result_) {
      VRCommandInfo& vr_command = it->second;
      vr_command.succesful_response_received_ = true;
    } else {
      LOG4CXX_DEBUG(logger_, "Hmi response is not Success: " << vr_result_
                    << ". Stop sending VRAddCommand requests");
      sync_primitives::AutoLock error_lock(error_from_hmi_lock_);
      if (!error_from_hmi_) {
        error_from_hmi_ = true;
        SendResponse(false, GetMobileResultCode(vr_result_));
      }
    }

    // update request timeout for case we send many VR add command requests
    // and HMI has no time to send responses for all of them
    LOG4CXX_DEBUG(logger_, "expected_chs_count_ = " << expected_chs_count_
                  << "received_chs_count_ = " << received_chs_count_);
    if (received_chs_count_ < expected_chs_count_) {
      sync_primitives::AutoLock timeout_lock_(is_timed_out_lock_);
      if (!is_timed_out_) {
        ApplicationManagerImpl::instance()->updateRequestTimeout(
            connection_key(), correlation_id(), default_timeout());
      }
    } else {
      OnAllHMIResponsesReceived();
    }
  }
}

void CreateInteractionChoiceSetRequest::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);

  sync_primitives::AutoLock error_lock(error_from_hmi_lock_);
  if (!error_from_hmi_) {
    SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
  }

  // We have to keep request alive until receive all responses from HMI
  // according to SDLAQ-CRS-2976
  sync_primitives::AutoLock timeout_lock_(is_timed_out_lock_);
  is_timed_out_ = true;
  ApplicationManagerImpl::instance()->updateRequestTimeout(
      connection_key(), correlation_id(), 0);
}

void CreateInteractionChoiceSetRequest::DeleteChoices() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr application =
    ApplicationManagerImpl::instance()->application(connection_key());
  if (!application) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }
  application->RemoveChoiceSet(choice_set_id_);

  smart_objects::SmartObject msg_param(smart_objects::SmartType_Map);
  msg_param[strings::app_id] = application->app_id();

  sync_primitives::AutoLock commands_lock(vr_commands_lock_);
  SentCommandsMap::const_iterator it = sent_commands_map_.begin();
  for (; it != sent_commands_map_.end(); ++it) {
    const VRCommandInfo& vr_command_info = it->second;
    if (vr_command_info.succesful_response_received_) {
      msg_param[strings::cmd_id] = vr_command_info.cmd_id_;
      SendHMIRequest(hmi_apis::FunctionID::VR_DeleteCommand, &msg_param);
    } else {
      LOG4CXX_WARN(
          logger_, "Succesfull response has not been received for cmd_id =  "
          << vr_command_info.cmd_id_);
    }
  }
  sent_commands_map_.clear();
}

void CreateInteractionChoiceSetRequest::OnAllHMIResponsesReceived() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!error_from_hmi_) {
    SendResponse(true, mobile_apis::Result::SUCCESS);

    ApplicationSharedPtr application =
        ApplicationManagerImpl::instance()->application(connection_key());
    if (!application) {
      LOG4CXX_ERROR(logger_, "NULL pointer");
      return;
    }
    application->UpdateHash();
  } else {
    DeleteChoices();
  }
  ApplicationManagerImpl::instance()->TerminateRequest(connection_key(),
                                                       correlation_id());
}

}  // namespace commands

}  // namespace application_manager

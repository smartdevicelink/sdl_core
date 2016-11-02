/*

 Copyright (c) 2016, Ford Motor Company
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
#include <cstring>
#include <algorithm>
#include <vector>
#include "application_manager/commands/mobile/create_interaction_choice_set_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "utils/gen_hash.h"
#include "utils/helpers.h"
#include "utils/stl_utils.h"

namespace application_manager {

namespace commands {

/**
 * @brief The UniqueParamChecker class
 * helps to check, whether incoming string
 * is unique compared to previous ones, or not.
 * Also checks syntax correctness.
 */
class UniqueParamChecker {
 public:
  UniqueParamChecker(const char* const name) : name_(name) {}

  mobile_apis::Result::eType operator()(const std::string& param) {
    if (!utils::InsertIntoSet(Djb2HashFromString(param), hash_set_)) {
      error_msg_ = "Choise with " + name_ + " " + param + " already exists";
      return mobile_apis::Result::DUPLICATE_NAME;
    }
    if (!CommandRequestImpl::CheckSyntax(param)) {
      error_msg_ = "Invalid syntax of " + name_ + " value. Syntax check failed";
      return mobile_apis::Result::INVALID_DATA;
    }

    return mobile_apis::Result::SUCCESS;
  }

  const std::string error_msg() const {
    return error_msg_;
  }

 private:
  std::string error_msg_;
  std::set<int32_t> hash_set_;
  const std::string name_;
};

CreateInteractionChoiceSetRequest::CreateInteractionChoiceSetRequest(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager)
    , expected_chs_count_(0)
    , received_chs_count_(0)
    , error_from_hmi_(false)
    , vr_commands_lock_(true) {}

CreateInteractionChoiceSetRequest::~CreateInteractionChoiceSetRequest() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void CreateInteractionChoiceSetRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace mobile_apis;
  ApplicationSharedPtr app = application_manager_.application(connection_key());

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
    if ((*message_)[strings::msg_params][strings::choice_set][i].keyExists(
            strings::image)) {
      verification_result_image = MessageHelper::VerifyImage(
          (*message_)[strings::msg_params][strings::choice_set][i]
                     [strings::image],
          app,
          application_manager_);
    }
    if ((*message_)[strings::msg_params][strings::choice_set][i].keyExists(
            strings::secondary_image)) {
      verification_result_secondary_image = MessageHelper::VerifyImage(
          (*message_)[strings::msg_params][strings::choice_set][i]
                     [strings::secondary_image],
          app,
          application_manager_);
    }
    if (verification_result_image == Result::INVALID_DATA ||
        verification_result_secondary_image == Result::INVALID_DATA) {
      LOG4CXX_ERROR(logger_, "Image verification failed.");
      SendResponse(false, Result::INVALID_DATA);
      return;
    }
  }

  choice_set_id_ =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id]
          .asInt();

  if (app->FindChoiceSet(choice_set_id_)) {
    LOG4CXX_ERROR(logger_,
                  "Choice set with id " << choice_set_id_ << " is not found.");
    SendResponse(false, Result::INVALID_ID);
    return;
  }

  Result::eType result = CheckChoiceSet();
  if (Result::SUCCESS != result) {
    SendResponse(false, result);
    return;
  }
  uint32_t grammar_id = application_manager_.GenerateGrammarID();
  (*message_)[strings::msg_params][strings::grammar_id] = grammar_id;
  SendVRAddCommandRequests(app);
  app->AddChoiceSet(choice_set_id_, (*message_)[strings::msg_params]);
}

mobile_apis::Result::eType CreateInteractionChoiceSetRequest::CheckChoiceSet() {
  using smart_objects::SmartObject;
  using smart_objects::SmartArray;
  LOG4CXX_AUTO_TRACE(logger_);

  std::set<uint32_t> choice_id_set;
  UniqueParamChecker choice_menu_name_checker(strings::menu_name);
  UniqueParamChecker choice_vr_command_checker(strings::vr_commands);

  const SmartArray* choice_set =
      (*message_)[strings::msg_params][strings::choice_set].asArray();

  SmartArray::const_iterator choice_set_it = choice_set->begin();

  for (; choice_set->end() != choice_set_it; ++choice_set_it) {
    const uint32_t choice_id = (*choice_set_it)[strings::choice_id].asUInt();
    if (!utils::InsertIntoSet(choice_id, choice_id_set)) {
      LOG4CXX_ERROR(logger_,
                    "Choise with ID " << choice_id << " already exists");
      return mobile_apis::Result::INVALID_ID;
    }

    mobile_apis::Result::eType result_code = mobile_apis::Result::SUCCESS;

    const std::string choice_menu_name =
        (*choice_set_it)[strings::menu_name].asString();
    result_code = choice_menu_name_checker(choice_menu_name);
    if (mobile_apis::Result::SUCCESS != result_code) {
      LOG4CXX_ERROR(logger_, choice_menu_name_checker.error_msg());
      return result_code;
    }

    const SmartObject& vr_commands_array =
        (*choice_set_it)[strings::vr_commands];

    for (size_t i = 0; i < vr_commands_array.length(); ++i) {
      const std::string choice_vr_command = vr_commands_array[i].asString();
      result_code = choice_vr_command_checker(choice_vr_command);
      if (mobile_apis::Result::SUCCESS != result_code) {
        LOG4CXX_ERROR(logger_, choice_vr_command_checker.error_msg());
        return result_code;
      }
    }

    if (IsWhiteSpaceExist(*choice_set_it)) {
      LOG4CXX_ERROR(logger_, "Incoming choice set has contains \\t\\n \\t \\n");
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
  it = std::find_first_of(vr_cmds_1->begin(),
                          vr_cmds_1->end(),
                          vr_cmds_2->begin(),
                          vr_cmds_2->end(),
                          CreateInteractionChoiceSetRequest::compareStr);

  if (it != vr_cmds_1->end()) {
    LOG4CXX_INFO(logger_,
                 "Incoming choice set has duplicated VR synonyms "
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
    const smart_objects::SmartObject& choice) {
  LOG4CXX_AUTO_TRACE(logger_);
  const char* str = NULL;

  if (choice.keyExists(strings::secondary_text)) {
    str = choice[strings::secondary_text].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(
          logger_,
          "Invalid syntax of secondary_text value. Syntax check failed");
      return true;
    }
  }

  if (choice.keyExists(strings::tertiary_text)) {
    str = choice[strings::tertiary_text].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(
          logger_,
          "Invalid syntax of tertiary_text value. Syntax check failed");
      return true;
    }
  }

  if (choice.keyExists(strings::image)) {
    str = choice[strings::image][strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(logger_,
                    "Invalid syntax of image value. Syntax check failed");
      return true;
    }
  }

  if (choice.keyExists(strings::secondary_image)) {
    str = choice[strings::secondary_image][strings::value].asCharArray();
    if (!CheckSyntax(str)) {
      LOG4CXX_ERROR(
          logger_,
          "Invalid syntax of secondary_image value. Syntax check failed");
      return true;
    }
  }
  return false;
}

bool InsertVrCommands(const smart_objects::SmartObject& vr_commands,
                      std::set<int32_t>& vr_commands_hash) {
  using smart_objects::SmartArray;
  const smart_objects::SmartArray* vr_commands_array = vr_commands.asArray();

  SmartArray::const_iterator vr_it = vr_commands_array->begin();
  const SmartArray::const_iterator vr_end = vr_commands_array->end();

  bool result = true;
  for (; vr_end != vr_it; ++vr_it) {
    const std::string vr_command = (*vr_it).asString();
    result = utils::InsertIntoSet(utils::Djb2HashFromString(vr_command),
                                  vr_commands_hash) &&
             result;
  }
  return result;
}

std::set<int32_t> CollectVRCommands(
    DataAccessor<application_manager::ChoiceSetMap> data_accessor) {
  using namespace application_manager;
  std::set<int32_t> vr_commands_set;
  const ChoiceSetMap& choice_set_map = data_accessor.GetData();

  ChoiceSetMap::const_iterator choice_set_it = choice_set_map.begin();
  const ChoiceSetMap::const_iterator choice_set_end = choice_set_map.end();
  for (; choice_set_it != choice_set_end; ++choice_set_it) {
    InsertVrCommands((*choice_set_it->second)[strings::vr_commands],
                     vr_commands_set);
  }
  return vr_commands_set;
}

void CreateInteractionChoiceSetRequest::SendVRAddCommandRequests(
    application_manager::ApplicationSharedPtr const app) {
  using smart_objects::SmartObject;
  LOG4CXX_AUTO_TRACE(logger_);

  SmartObject& choice_set = (*message_)[strings::msg_params];
  SmartObject msg_params = SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::type] = hmi_apis::Common_VRCommandType::Choice;
  msg_params[strings::app_id] = app->app_id();
  msg_params[strings::grammar_id] = choice_set[strings::grammar_id];
  const uint32_t choice_count = choice_set[strings::choice_set].length();
  SetAllowedToTerminate(false);

  std::set<int32_t> vr_commands_hash(CollectVRCommands(app->choice_set_map()));

  expected_chs_count_ = choice_count;
  size_t chs_num = 0;
  for (; chs_num < choice_count; ++chs_num) {
    {
      sync_primitives::AutoLock error_lock(error_from_hmi_lock_);
      if (error_from_hmi_) {
        LOG4CXX_WARN(logger_,
                     "Error from HMI received. Stop sending VRCommands");
        break;
      }
    }

    const SmartObject& choice = choice_set[strings::choice_set][chs_num];

    if (!choice[strings::vr_commands].asArray() ||
        choice[strings::vr_commands].empty()) {
      --expected_chs_count_;
      LOG4CXX_DEBUG(logger_,
                    "Choice with cmd_id "
                        << choice[strings::cmd_id].asString()
                        << "was skipped because it have no vr_commands");
      continue;
    }

    if (!InsertVrCommands(choice[strings::vr_commands], vr_commands_hash)) {
      --expected_chs_count_;
      LOG4CXX_DEBUG(
          logger_,
          "Choice with cmd_id "
              << choice[strings::cmd_id].asString()
              << "was skipped because some of vr_commands were already sent to "
                 "HMI. vr_commands: "
              << choice[strings::vr_commands].asString());

      continue;
    }

    msg_params[strings::cmd_id] = choice[strings::choice_id];
    msg_params[strings::vr_commands] = choice[strings::vr_commands];

    sync_primitives::AutoLock commands_lock(vr_commands_lock_);
    const uint32_t vr_cmd_id = msg_params[strings::cmd_id].asUInt();
    const uint32_t vr_corr_id =
        SendHMIRequest(hmi_apis::FunctionID::VR_AddCommand, &msg_params, true);

    VRCommandInfo vr_command(vr_cmd_id);
    sent_commands_map_[vr_corr_id] = vr_command;
    LOG4CXX_DEBUG(logger_,
                  "VR_command sent corr_id " << vr_corr_id << " cmd_id "
                                             << vr_corr_id);
  }
  LOG4CXX_DEBUG(logger_, "expected_chs_count_ = " << expected_chs_count_);
}

void CreateInteractionChoiceSetRequest::ProcessHmiError(
    const hmi_apis::Common_Result::eType vr_result) {
  LOG4CXX_DEBUG(logger_,
                "Hmi response is not Success: "
                    << vr_result << ". Stop sending VRAddCommand requests");
  if (!error_from_hmi_) {
    error_from_hmi_ = true;
    std::string info =
        vr_result == hmi_apis::Common_Result::UNSUPPORTED_RESOURCE
            ? "VR is not supported by system"
            : "";
    SendResponse(false, GetMobileResultCode(vr_result), info.c_str());
  }
}

bool CreateInteractionChoiceSetRequest::ProcessSuccesfulHMIResponse(
    const uint32_t corr_id) {
  SentCommandsMap::iterator it = sent_commands_map_.find(corr_id);
  if (sent_commands_map_.end() == it) {
    LOG4CXX_WARN(logger_, "HMI response for unknown VR command received");
    return false;
  }
  VRCommandInfo& vr_command = it->second;
  vr_command.succesful_response_received_ = true;
  return true;
}

void CreateInteractionChoiceSetRequest::CountReceivedVRResponses() {
  received_chs_count_++;
  LOG4CXX_DEBUG(logger_,
                "Got VR.AddCommand response, there are "
                    << expected_chs_count_ - received_chs_count_
                    << " more to wait.");
  if (received_chs_count_ < expected_chs_count_) {
    application_manager_.updateRequestTimeout(
        connection_key(), correlation_id(), default_timeout());
    LOG4CXX_DEBUG(logger_, "Timeout for request was updated");
  } else {
    OnAllHMIResponsesReceived();
  }
}

void CreateInteractionChoiceSetRequest::on_event(
    const event_engine::Event& event) {
  using namespace hmi_apis;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);

  const smart_objects::SmartObject& message = event.smart_object();
  const Common_Result::eType result = static_cast<Common_Result::eType>(
      message[strings::params][hmi_response::code].asInt());
  const bool is_no_error = Compare<Common_Result::eType, EQ, ONE>(
      result, Common_Result::SUCCESS, Common_Result::WARNINGS);
  uint32_t corr_id = static_cast<uint32_t>(
      message[strings::params][strings::correlation_id].asUInt());
  if (event.id() == hmi_apis::FunctionID::VR_AddCommand) {
    {
      sync_primitives::AutoLock commands_lock(vr_commands_lock_);
      if (is_no_error) {
        if (!ProcessSuccesfulHMIResponse(corr_id)) {
          return;
        }
      } else {
        ProcessHmiError(result);
      }
    }
    CountReceivedVRResponses();
  }
}

void CreateInteractionChoiceSetRequest::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!error_from_hmi_) {
    SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
  }
  DeleteChoices();

  // We have to keep request alive until receive all responses from HMI
  // according to SDLAQ-CRS-2976
  sync_primitives::AutoLock timeout_lock_(is_timed_out_lock_);
  is_timed_out_ = true;
  application_manager_.TerminateRequest(
      connection_key(), correlation_id(), function_id());
}

void CreateInteractionChoiceSetRequest::DeleteChoices() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());
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
      LOG4CXX_WARN(logger_,
                   "succesful response has not been received for cmd_id =  "
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
        application_manager_.application(connection_key());
    if (!application) {
      LOG4CXX_ERROR(logger_, "NULL pointer");
      return;
    }
    application->UpdateHash();
  } else {
    DeleteChoices();
  }

  application_manager_.TerminateRequest(
      connection_key(), correlation_id(), function_id());
}

}  // namespace commands

}  // namespace application_manager

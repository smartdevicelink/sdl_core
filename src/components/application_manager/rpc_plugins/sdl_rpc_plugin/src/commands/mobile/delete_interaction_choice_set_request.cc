/*

 Copyright (c) 2018, Ford Motor Company
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

#include "sdl_rpc_plugin/commands/mobile/delete_interaction_choice_set_request.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

DeleteInteractionChoiceSetRequest::DeleteInteractionChoiceSetRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler)
    , response_result_code_(hmi_apis::Common_Result::eType::INVALID_ENUM)
    , response_result_(false) {}

DeleteInteractionChoiceSetRequest::~DeleteInteractionChoiceSetRequest() {}

void DeleteInteractionChoiceSetRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(
        logger_,
        "No application associated with connection key " << connection_key());
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const int32_t choice_set_id =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id]
          .asInt();

  if (!app->FindChoiceSet(choice_set_id)) {
    LOG4CXX_ERROR(logger_,
                  "Choice set with id " << choice_set_id << " is not found.");
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  if (ChoiceSetInUse(app)) {
    LOG4CXX_ERROR(logger_, "Choice set currently in use.");
    SendResponse(false, mobile_apis::Result::IN_USE);
    return;
  }
  SendVrDeleteCommand(app);
}

bool DeleteInteractionChoiceSetRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

void DeleteInteractionChoiceSetRequest::on_event(
    const event_engine::Event& event) {
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);

  if (event.id() == hmi_apis::FunctionID::VR_DeleteCommand) {
    {
      sync_primitives::AutoLock auto_lock(requests_lock_);
      const smart_objects::SmartObject& message = event.smart_object();
      response_result_code_ = static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());
      const std::uint32_t correlation_id = static_cast<uint32_t>(
          message[strings::params][strings::correlation_id].asUInt());

      auto found_request = sent_requests_.find(correlation_id);
      if (sent_requests_.end() == found_request) {
        LOG4CXX_WARN(logger_,
                     "Request with " << *found_request
                                     << " correlation_id is not found.");
        return;
      }
      sent_requests_.erase(found_request);

      if (sent_requests_.empty()) {
        SendDeleteInteractionChoiceSetResponse();
      }
    }
  }
}

void DeleteInteractionChoiceSetRequest::onTimeOut() {
  LOG4CXX_AUTO_TRACE(logger_);
  SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
}

bool DeleteInteractionChoiceSetRequest::ChoiceSetInUse(
    ApplicationConstSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!app->is_perform_interaction_active()) {
    return false;
  }
  const DataAccessor<PerformChoiceSetMap> accessor =
      app->performinteraction_choice_set_map();
  const PerformChoiceSetMap& choice_set_map = accessor.GetData();

  const uint32_t choice_set_id =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id]
          .asUInt();

  PerformChoiceSetMap::const_iterator it = choice_set_map.begin();
  for (; choice_set_map.end() != it; ++it) {
    const PerformChoice& choice = it->second;
    PerformChoice::const_iterator choice_it = choice.begin();
    for (; choice.end() != choice_it; ++choice_it) {
      if (choice_it->first == choice_set_id) {
        LOG4CXX_ERROR(logger_,
                      "Choice set with id " << choice_set_id << " is in use.");
        return true;
      }
    }
  }
  return true;
}

void DeleteInteractionChoiceSetRequest::SendVrDeleteCommand(
    application_manager::ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);

  const uint32_t choice_set_id =
      (*message_)[strings::msg_params][strings::interaction_choice_set_id]
          .asUInt();

  smart_objects::SmartObject* choice_set = app->FindChoiceSet(choice_set_id);

  if (!choice_set) {
    LOG4CXX_ERROR(logger_,
                  "Choice set with id " << choice_set_id << " is not found.");
    return;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();
  msg_params[strings::type] = hmi_apis::Common_VRCommandType::Choice;
  msg_params[strings::grammar_id] = (*choice_set)[strings::grammar_id];
  choice_set = &((*choice_set)[strings::choice_set]);
  {
    sync_primitives::AutoLock auto_lock(requests_lock_);
    for (uint32_t i = 0; i < (*choice_set).length(); ++i) {
      msg_params[strings::cmd_id] = (*choice_set)[i][strings::choice_id];
      const uint32_t delte_cmd_hmi_corr_id = SendHMIRequest(
          hmi_apis::FunctionID::VR_DeleteCommand, &msg_params, true);
      sent_requests_.insert(delte_cmd_hmi_corr_id);
    }
  }
}

void DeleteInteractionChoiceSetRequest::
    SendDeleteInteractionChoiceSetResponse() {
  const mobile_apis::Result::eType result_code =
      MessageHelper::HMIToMobileResult(response_result_code_);
  response_result_ =
      helpers::Compare<mobile_apis::Result::eType, helpers::EQ, helpers::ONE>(
          result_code,
          mobile_apis::Result::SUCCESS,
          mobile_apis::Result::WARNINGS);
  if (response_result_) {
    ApplicationSharedPtr app =
        application_manager_.application(connection_key());
    if (!app) {
      LOG4CXX_ERROR(logger_,
                    "Application with connection key " << connection_key()
                                                       << " did not find.");
      return;
    }
    const uint32_t choice_set_id =
        (*message_)[strings::msg_params][strings::interaction_choice_set_id]
            .asUInt();
    app->RemoveChoiceSet(choice_set_id);
  }

  LOG4CXX_DEBUG(logger_,
                "Response sent. Result code: " << result_code
                                               << " sussess: " << std::boolalpha
                                               << result_code);
  SendResponse(response_result_, result_code);
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

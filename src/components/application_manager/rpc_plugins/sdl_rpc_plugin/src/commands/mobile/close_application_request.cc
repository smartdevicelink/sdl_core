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

#include "sdl_rpc_plugin/commands/mobile/close_application_request.h"
#include "application_manager/rpc_service.h"
#include "application_manager/message_helper.h"
#include "utils/include/utils/helpers.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

CloseApplicationRequest::CloseApplicationRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

CloseApplicationRequest::~CloseApplicationRequest() {}

void CloseApplicationRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  auto application = application_manager_.application(connection_key());

  if (!application) {
    LOG4CXX_ERROR(logger_, "No such application registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const auto app_hmi_level = application->hmi_level();

  if (mobile_apis::HMILevel::HMI_NONE == app_hmi_level) {
    LOG4CXX_DEBUG(logger_,
                  "Application with id " << connection_key()
                                         << " is already in hmi_level NONE.");
    SendResponse(false, mobile_apis::Result::IGNORED);
    return;
  }

  const bool sent_success =
      SendBCActivateApp(application, hmi_apis::Common_HMILevel::NONE, true);

  if (!sent_success) {
    LOG4CXX_ERROR(logger_, "Unable to send BC.ActivateApp");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
  }
}

void CloseApplicationRequest::on_event(const event_engine::Event& event) {
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);
  const auto message = event.smart_object();

  if (hmi_apis::FunctionID::BasicCommunication_ActivateApp != event.id()) {
    LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
    return;
  }

  const auto hmi_result = static_cast<hmi_apis::Common_Result::eType>(
      message[strings::params][hmi_response::code].asInt());

  const auto mobile_result = MessageHelper::HMIToMobileResult(hmi_result);
  const bool success = Compare<hmi_apis::Common_Result::eType, EQ, ONE>(
      hmi_result,
      hmi_apis::Common_Result::SUCCESS,
      hmi_apis::Common_Result::WARNINGS);

  if (success) {
    auto application = application_manager_.application(connection_key());
    if (!application) {
      LOG4CXX_DEBUG(logger_, "No such application registered");
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }
    application->set_hmi_level(mobile_apis::HMILevel::HMI_NONE);
    LOG4CXX_DEBUG(logger_,
                  "Application with id " << application->app_id()
                                         << " has switched to HMI_level NONE.");
  } else {
    LOG4CXX_WARN(logger_, "Application hasn't switched to HMI_level NONE.");
  }
  SendResponse(success, mobile_result);
}

bool CloseApplicationRequest::SendBCActivateApp(
    ApplicationConstSharedPtr app,
    const hmi_apis::Common_HMILevel::eType hmi_level,
    const bool send_policy_priority) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto bc_activate_app_request = MessageHelper::GetBCActivateAppRequestToHMI(
      app,
      application_manager_.connection_handler().get_session_observer(),
      application_manager_.GetPolicyHandler(),
      hmi_level,
      send_policy_priority,
      application_manager_);
  if (!bc_activate_app_request) {
    LOG4CXX_ERROR(logger_, "Unable to create BC.ActivateAppRequest");
    return false;
  }

  if (!application_manager_.GetRPCService().ManageHMICommand(
          bc_activate_app_request)) {
    LOG4CXX_ERROR(logger_, "Unable to send BC.ActivateAppRequest");
    return false;
  }
  const auto activate_app_corr_id =
      (*bc_activate_app_request)[strings::params][strings::correlation_id]
          .asUInt();
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_ActivateApp,
                     activate_app_corr_id);
  return true;
}

}  // namespace commands
}  // namespace sdl_rpc_plugin

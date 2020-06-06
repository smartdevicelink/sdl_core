/*
 Copyright (c) 2019, Ford Motor Company
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

#include "sdl_rpc_plugin/commands/mobile/delete_window_request.h"

#include <algorithm>

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

DeleteWindowRequest::DeleteWindowRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

DeleteWindowRequest::~DeleteWindowRequest() {}

bool DeleteWindowRequest::CheckWindowId(
    app_mngr::ApplicationSharedPtr app) const {
  const WindowID window_id =
      (*message_)[strings::msg_params][strings::window_id].asInt();
  if (mobile_apis::PredefinedWindows::DEFAULT_WINDOW == window_id) {
    LOG4CXX_ERROR(logger_, "Main application window can't be deleted");
    return false;
  }

  if (!app->WindowIdExists(window_id)) {
    LOG4CXX_ERROR(logger_,
                  "Window with id #" << window_id << " does not exist");
    return false;
  }

  return true;
}

app_mngr::WindowID DeleteWindowRequest::window_id() const {
  return mobile_apis::PredefinedWindows::DEFAULT_WINDOW;
}

void DeleteWindowRequest::DeleteWindow(
    app_mngr::ApplicationSharedPtr app) const {
  const WindowID window_id =
      (*message_)[strings::msg_params][strings::window_id].asInt();
  app->RemoveWindowInfo(window_id);
  app->RemoveHMIState(window_id, HmiState::StateID::STATE_ID_REGULAR);
  app->remove_window_capability(window_id);
}

void DeleteWindowRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (!CheckWindowId(application)) {
    SendResponse(false, mobile_apis::Result::INVALID_ID);
    return;
  }

  smart_objects::SmartObject msg_params = (*message_)[strings::msg_params];
  msg_params[strings::app_id] = application->hmi_app_id();

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(hmi_apis::FunctionID::UI_DeleteWindow, &msg_params, true);
}

void DeleteWindowRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (hmi_apis::FunctionID::UI_DeleteWindow != event.id()) {
    LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
    return;
  }

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  LOG4CXX_INFO(logger_, "Received DeleteWindow event");
  EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);

  const smart_objects::SmartObject& response_message = event.smart_object();
  const auto result_code = CommandRequestImpl::GetMobileResultCode(
      static_cast<hmi_apis::Common_Result::eType>(
          response_message[strings::params][hmi_response::code].asInt()));

  const bool is_success = IsMobileResultSuccess(result_code);
  std::string response_info;
  GetInfo(response_message, response_info);

  if (!is_success) {
    LOG4CXX_ERROR(logger_, "DeleteWindow request has failed on HMI side");
    SendResponse(is_success,
                 result_code,
                 response_info.empty() ? nullptr : response_info.c_str());
    return;
  }

  DeleteWindow(application);

  SendResponse(is_success,
               result_code,
               response_info.empty() ? nullptr : response_info.c_str());
}

bool DeleteWindowRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {

CloseApplicationRequest::CloseApplicationRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
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
  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (app.use_count() == 0) {
    LOG4CXX_ERROR(logger_, "Application does not exist");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  smart_objects::SmartObject msg_params;
  msg_params[strings::app_id] = connection_key();
  SendHMIRequest(hmi_apis::FunctionID::BasicCommunication_CloseApplication,
                 &msg_params,
                 true);
}

void CloseApplicationRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();

  auto msg_params = message[strings::msg_params];

  hmi_apis::Common_Result::eType hmi_result =
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt());

  mobile_apis::Result::eType result =
      MessageHelper::HMIToMobileResult(hmi_result);
  bool success = PrepareResultForMobileResponse(
      hmi_result, HmiInterfaces::HMI_INTERFACE_BasicCommunication);
  if (success) {
    ApplicationSharedPtr app =
        application_manager_.application(connection_key());

    if (app.use_count() == 0) {
      LOG4CXX_ERROR(logger_, "Application does not exist");
      SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
      return;
    }

    auto on_app_exit = [app](plugin_manager::RPCPlugin& plugin) {
      plugin.OnApplicationEvent(plugin_manager::kApplicationExit, app);
    };

    application_manager_.GetPluginManager().ForEachPlugin(on_app_exit);
    application_manager_.state_controller().SetRegularState(
        app,
        mobile_apis::HMILevel::HMI_NONE,
        mobile_apis::AudioStreamingState::NOT_AUDIBLE,
        mobile_apis::VideoStreamingState::NOT_STREAMABLE,
        false);
  }

  const char* info = msg_params.keyExists(strings::info)
                         ? msg_params[strings::info].asCharArray()
                         : NULL;

  SendResponse(success, result, info, &msg_params);
}

}  // namespace commands
}  // namespace sdl_rpc_plugin

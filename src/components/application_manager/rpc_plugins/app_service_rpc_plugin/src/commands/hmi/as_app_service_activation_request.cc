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

#include "app_service_rpc_plugin/commands/hmi/as_app_service_activation_request.h"

#include "application_manager/app_service_manager.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

ASAppServiceActivationRequest::ASAppServiceActivationRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromHMI(message,
                     application_manager,
                     rpc_service,
                     hmi_capabilities,
                     policy_handler) {}

ASAppServiceActivationRequest::~ASAppServiceActivationRequest() {}

void ASAppServiceActivationRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  AppServiceManager& service_manager =
      application_manager_.GetAppServiceManager();
  smart_objects::SmartObject params = (*message_)[strings::msg_params];
  smart_objects::SmartObject response_params(smart_objects::SmartType_Map);
  std::string service_id = params[strings::service_id].asString();
  response_params[strings::service_id] = service_id;
  if (params[strings::activate].asBool()) {
    response_params[strings::activate] =
        service_manager.ActivateAppService(service_id);
  } else {
    service_manager.DeactivateAppService(service_id);
    response_params[strings::activate] = false;
  }

  if (params.keyExists(strings::set_as_default)) {
    if (params[strings::set_as_default].asBool()) {
      response_params[strings::set_as_default] =
          service_manager.SetDefaultService(service_id);
    } else {
      service_manager.RemoveDefaultService(service_id);
      response_params[strings::set_as_default] = false;
    }
  }

  SendResponse(true,
               (*message_)[strings::params][strings::correlation_id].asUInt(),
               hmi_apis::FunctionID::AppService_AppServiceActivation,
               hmi_apis::Common_Result::SUCCESS,
               &response_params);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

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

#include "app_service_rpc_plugin/commands/mobile/perform_app_service_interaction_request.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

PerformAppServiceInteractionRequest::PerformAppServiceInteractionRequest(
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

PerformAppServiceInteractionRequest::~PerformAppServiceInteractionRequest() {}

void PerformAppServiceInteractionRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  auto app = application_manager_.application(connection_key());
  if (!app) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];
  std::string service_id = msg_params[strings::service_id].asString();
  auto service =
      application_manager_.GetAppServiceManager().FindServiceByID(service_id);
  if (!service) {
    SendResponse(false,
                 mobile_apis::Result::INVALID_ID,
                 "The requested service ID does not exist");
    return;
  }

  bool request_service_active = false;
  if (msg_params.keyExists(strings::request_service_active)) {
    request_service_active =
        msg_params[strings::request_service_active].asBool();
    msg_params.erase(strings::request_service_active);
  }

  // Only activate service if it is not already active
  bool activate_service = request_service_active &&
                          !service->record[strings::service_active].asBool();
  if (activate_service) {
    if (app->is_foreground()) {
      // App is in foreground, we can just activate the service
      application_manager_.GetAppServiceManager().ActivateAppService(
          service_id);
    } else {
      // App is not in foreground, we need to prompt the user to activate the
      // service
      smart_objects::SmartObject request_params;
      request_params[strings::service_id] = service_id;
      SendHMIRequest(hmi_apis::FunctionID::AppService_GetActiveServiceConsent,
                     &request_params,
                     true);
      return;
    }
  }

  SendProviderRequest(
      mobile_apis::FunctionID::PerformAppServiceInteractionID,
      hmi_apis::FunctionID::AppService_PerformAppServiceInteraction,
      &(*message_),
      true);
}

void PerformAppServiceInteractionRequest::on_event(
    const event_engine::Event& event) {
  LOG4CXX_DEBUG(logger_, "HMI PerformAppServiceInteraction on_event");
  const smart_objects::SmartObject& event_message = event.smart_object();

  auto msg_params = event_message[strings::msg_params];

  const char* info = msg_params.keyExists(strings::info)
                         ? msg_params[strings::info].asCharArray()
                         : NULL;
  hmi_apis::Common_Result::eType hmi_result =
      static_cast<hmi_apis::Common_Result::eType>(
          event_message[strings::params][hmi_response::code].asInt());
  mobile_apis::Result::eType result =
      MessageHelper::HMIToMobileResult(hmi_result);
  bool success = PrepareResultForMobileResponse(
      hmi_result, HmiInterfaces::HMI_INTERFACE_AppService);

  switch (event.id()) {
    case hmi_apis::FunctionID::AppService_PerformAppServiceInteraction:
      SendResponse(success, result, info, &msg_params);
      break;
    case hmi_apis::FunctionID::AppService_GetActiveServiceConsent:
      if (msg_params[strings::activate].asBool()) {
        // User agreed to activate service, we can now send the provider request
        application_manager_.GetAppServiceManager().ActivateAppService(
            (*message_)[strings::msg_params][strings::service_id].asString());
        SendProviderRequest(
            mobile_apis::FunctionID::PerformAppServiceInteractionID,
            hmi_apis::FunctionID::AppService_PerformAppServiceInteraction,
            &(*message_),
            true);
      } else if (mobile_apis::Result::SUCCESS == result) {
        // Request was successful, but user denied the request to activate the
        // service
        SendResponse(false,
                     mobile_apis::Result::USER_DISALLOWED,
                     "Request to activate service was denied by driver");
      } else {
        SendResponse(success, result, info);
      }
      break;
    default:
      break;
  }
}

void PerformAppServiceInteractionRequest::on_event(
    const event_engine::MobileEvent& event) {
  LOG4CXX_DEBUG(logger_, "Mobile PerformAppServiceInteraction on_event");
  const smart_objects::SmartObject& event_message = event.smart_object();

  auto msg_params = event_message[strings::msg_params];

  const char* info = msg_params.keyExists(strings::info)
                         ? msg_params[strings::info].asCharArray()
                         : NULL;
  mobile_apis::Result::eType result = static_cast<mobile_apis::Result::eType>(
      msg_params[strings::result_code].asInt());
  bool success = IsMobileResultSuccess(result);

  SendResponse(success, result, info, &msg_params);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

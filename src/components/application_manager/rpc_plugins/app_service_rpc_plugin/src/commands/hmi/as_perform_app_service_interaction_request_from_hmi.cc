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

#include "app_service_rpc_plugin/commands/hmi/as_perform_app_service_interaction_request_from_hmi.h"

#include "application_manager/app_service_manager.h"
#include "application_manager/message_helper.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

ASPerformAppServiceInteractionRequestFromHMI::
    ASPerformAppServiceInteractionRequestFromHMI(
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

ASPerformAppServiceInteractionRequestFromHMI::
    ~ASPerformAppServiceInteractionRequestFromHMI() {}

void ASPerformAppServiceInteractionRequestFromHMI::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];
  std::string hmi_origin_id =
      application_manager_.get_settings().hmi_origin_id();
  if (!msg_params.keyExists(strings::origin_app)) {
    if (hmi_origin_id.empty()) {
      smart_objects::SmartObject response_params;
      response_params[strings::info] =
          "No HMI origin ID to use for interaction passthrough";
      SendResponse(
          false,
          correlation_id(),
          hmi_apis::FunctionID::AppService_PerformAppServiceInteraction,
          hmi_apis::Common_Result::INVALID_DATA,
          &response_params,
          application_manager::commands::Command::SOURCE_SDL_TO_HMI);
      return;
    }
    msg_params[strings::origin_app] = hmi_origin_id;
  }

  std::string service_id = msg_params[strings::service_id].asString();
  auto service =
      application_manager_.GetAppServiceManager().FindServiceByID(service_id);
  if (!service) {
    smart_objects::SmartObject response_params;
    response_params[strings::info] = "The requested service ID does not exist";
    SendResponse(false,
                 correlation_id(),
                 hmi_apis::FunctionID::AppService_PerformAppServiceInteraction,
                 hmi_apis::Common_Result::INVALID_ID,
                 &response_params,
                 application_manager::commands::Command::SOURCE_SDL_TO_HMI);
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
    application_manager_.GetAppServiceManager().ActivateAppService(service_id);
  }

  SendProviderRequest(
      mobile_apis::FunctionID::PerformAppServiceInteractionID,
      hmi_apis::FunctionID::AppService_PerformAppServiceInteraction,
      &(*message_),
      true);
}

void ASPerformAppServiceInteractionRequestFromHMI::on_event(
    const event_engine::Event& event) {
  const smart_objects::SmartObject& event_message = event.smart_object();

  auto msg_params = event_message[strings::msg_params];

  hmi_apis::Common_Result::eType result =
      static_cast<hmi_apis::Common_Result::eType>(
          event_message[strings::params][hmi_response::code].asInt());
  bool success =
      IsHMIResultSuccess(result, HmiInterfaces::HMI_INTERFACE_AppService);
  SendResponse(success,
               correlation_id(),
               hmi_apis::FunctionID::AppService_PerformAppServiceInteraction,
               result,
               &msg_params,
               application_manager::commands::Command::SOURCE_SDL_TO_HMI);
}

void ASPerformAppServiceInteractionRequestFromHMI::on_event(
    const event_engine::MobileEvent& event) {
  const smart_objects::SmartObject& event_message = event.smart_object();

  auto msg_params = event_message[strings::msg_params];

  mobile_apis::Result::eType mobile_result =
      static_cast<mobile_apis::Result::eType>(
          msg_params[strings::result_code].asInt());
  hmi_apis::Common_Result::eType result =
      MessageHelper::MobileToHMIResult(mobile_result);
  bool success = IsMobileResultSuccess(mobile_result);
  SendResponse(success,
               correlation_id(),
               hmi_apis::FunctionID::AppService_PerformAppServiceInteraction,
               result,
               &msg_params,
               application_manager::commands::Command::SOURCE_SDL_TO_HMI);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

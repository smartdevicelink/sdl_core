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

#include "app_service_rpc_plugin/commands/hmi/as_get_app_service_data_request_from_hmi.h"
#include "application_manager/application_impl.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/enum_schema_item.h"

#include "application_manager/message_helper.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

ASGetAppServiceDataRequestFromHMI::ASGetAppServiceDataRequestFromHMI(
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

ASGetAppServiceDataRequestFromHMI::~ASGetAppServiceDataRequestFromHMI() {}

void ASGetAppServiceDataRequestFromHMI::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  // Todo enum or string?
  hmi_apis::Common_AppServiceType::eType service_type =
      static_cast<hmi_apis::Common_AppServiceType::eType>(
          (*message_)[strings::msg_params][strings::service_type].asUInt());

  std::string service_type_str = std::string();
  smart_objects::EnumConversionHelper<
      hmi_apis::Common_AppServiceType::eType>::EnumToString(service_type,
                                                            &service_type_str);
  LOG4CXX_DEBUG(logger_, "Get Service Type: " << service_type_str);

  SendProviderRequest(mobile_apis::FunctionID::GetAppServiceDataID,
                      hmi_apis::FunctionID::AppService_GetAppServiceData,
                      &(*message_),
                      true);
}

void ASGetAppServiceDataRequestFromHMI::on_event(
    const event_engine::Event& event) {
  const smart_objects::SmartObject& event_message = event.smart_object();

  auto msg_params = event_message[strings::msg_params];
  SendResponse(true,
               correlation_id(),
               hmi_apis::FunctionID::AppService_GetAppServiceData,
               hmi_apis::Common_Result::SUCCESS,
               &msg_params,
               application_manager::commands::Command::SOURCE_TO_HMI);
}

void ASGetAppServiceDataRequestFromHMI::on_event(
    const event_engine::MobileEvent& event) {
  const smart_objects::SmartObject& event_message = event.smart_object();

  auto msg_params = event_message[strings::msg_params];
  SendResponse(true,
               correlation_id(),
               hmi_apis::FunctionID::AppService_GetAppServiceData,
               hmi_apis::Common_Result::SUCCESS,
               &msg_params,
               application_manager::commands::Command::SOURCE_TO_HMI);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

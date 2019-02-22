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

#include "app_service_rpc_plugin/commands/mobile/get_app_service_data_request.h"
#include "app_service_rpc_plugin/app_service_app_extension.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/enum_schema_item.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

GetAppServiceDataRequest::GetAppServiceDataRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler)
    , plugin_(NULL) {
  auto plugin = (application_manager.GetPluginManager().FindPluginToProcess(
      mobile_apis::FunctionID::GetAppServiceDataID,
      app_mngr::commands::Command::CommandSource::SOURCE_MOBILE));
  if (plugin) {
    plugin_ = dynamic_cast<AppServiceRpcPlugin*>(&(*plugin));
  }
}

GetAppServiceDataRequest::~GetAppServiceDataRequest() {}

void GetAppServiceDataRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  std::string service_type =
      (*message_)[strings::msg_params][strings::service_type].asString();

  bool subscribe = false;
  if ((*message_)[strings::msg_params].keyExists(strings::subscribe)) {
    subscribe = (*message_)[strings::msg_params][strings::subscribe].asBool();
  }

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (subscribe) {
    auto& ext = AppServiceAppExtension::ExtractASExtension(*app);
    ext.SubscribeToAppService(service_type);
  }

  SendProviderRequest(mobile_apis::FunctionID::GetAppServiceDataID,
                      hmi_apis::FunctionID::AppService_GetAppServiceData,
                      &(*message_),
                      true);
}

void GetAppServiceDataRequest::on_event(
    const event_engine::MobileEvent& event) {
  const smart_objects::SmartObject& event_message = event.smart_object();

  auto msg_params = event_message[strings::msg_params];

  mobile_apis::Result::eType result = static_cast<mobile_apis::Result::eType>(
      msg_params[strings::result_code].asInt());
  bool success = IsMobileResultSuccess(result);
  SendResponse(success, result, NULL, &msg_params);
}

void GetAppServiceDataRequest::on_event(const event_engine::Event& event) {
  const smart_objects::SmartObject& event_message = event.smart_object();

  auto msg_params = event_message[strings::msg_params];

  hmi_apis::Common_Result::eType hmi_result =
      static_cast<hmi_apis::Common_Result::eType>(
          event_message[strings::params][hmi_response::code].asInt());

  mobile_apis::Result::eType result =
      MessageHelper::HMIToMobileResult(hmi_result);
  bool success = PrepareResultForMobileResponse(
      hmi_result, HmiInterfaces::HMI_INTERFACE_AppService);

  SendResponse(success, result, NULL, &msg_params);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

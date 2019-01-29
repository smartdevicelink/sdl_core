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

#include "app_service_rpc_plugin/commands/mobile/publish_app_service_request.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"
#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

PublishAppServiceRequest::PublishAppServiceRequest(
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
      mobile_apis::FunctionID::PublishAppServiceID,
      app_mngr::commands::Command::CommandSource::SOURCE_MOBILE));
  if (plugin) {
    plugin_ = dynamic_cast<AppServiceRpcPlugin*>(&(*plugin));
  }
}

PublishAppServiceRequest::~PublishAppServiceRequest() {}

bool PublishAppServiceRequest::ValidateManifest(
    smart_objects::SmartObject& manifest) {
  if (manifest.keyExists(strings::uri_scheme)) {
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(manifest[strings::uri_scheme].asString(), value)) {
      SendResponse(false,
                   mobile_apis::Result::INVALID_DATA,
                   "Provided uriScheme was not valid JSON");
      return false;
    }
  }
  return true;
}

void PublishAppServiceRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Received a PublishAppService");
  MessageHelper::PrintSmartObject(*message_);

  smart_objects::SmartObject response_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject manifest =
      (*message_)[strings::msg_params][strings::app_service_manifest];
  if (!ValidateManifest(manifest)) {
    return;
  }
  smart_objects::SmartObject service_record =
      application_manager_.GetAppServiceManager().PublishAppService(manifest);

  response_params[strings::app_service_record] = service_record;

  ApplicationSharedPtr app = application_manager_.application(connection_key());
  auto& ext =
      sdl_rpc_plugin::SystemCapabilityAppExtension::ExtractExtension(*app);
  ext.subscribeTo(mobile_apis::SystemCapabilityType::APP_SERVICES);

  SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

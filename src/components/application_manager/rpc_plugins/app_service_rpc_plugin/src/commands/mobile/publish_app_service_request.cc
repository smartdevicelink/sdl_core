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

#include "application_manager/app_service_manager.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"
#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

PublishAppServiceRequest::PublishAppServiceRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler) {}

PublishAppServiceRequest::~PublishAppServiceRequest() {}

void PublishAppServiceRequest::Run() {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Received a PublishAppService " << connection_key());
  MessageHelper::PrintSmartObject(*message_);

  smart_objects::SmartObject response_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject manifest =
      (*message_)[strings::msg_params][strings::app_service_manifest];

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  std::string requested_service_name = "";

  if ((*message_)[strings::msg_params][strings::app_service_manifest].keyExists(
          strings::service_name)) {
    requested_service_name =
        (*message_)[strings::msg_params][strings::app_service_manifest]
                   [strings::service_name]
                       .asString();
  }

  std::string requested_service_type =
      (*message_)[strings::msg_params][strings::app_service_manifest]
                 [strings::service_type]
                     .asString();

  smart_objects::SmartArray* requested_handled_rpcs = NULL;
  if ((*message_)[strings::msg_params][strings::app_service_manifest].keyExists(
          strings::handled_rpcs)) {
    requested_handled_rpcs =
        (*message_)[strings::msg_params][strings::app_service_manifest]
                   [strings::handled_rpcs]
                       .asArray();
  }

  bool result =
      policy_handler_.CheckAppServiceParameters(app->policy_app_id(),
                                                requested_service_name,
                                                requested_service_type,
                                                requested_handled_rpcs);

  if (!result) {
    SendResponse(false,
                 mobile_apis::Result::DISALLOWED,
                 "Service disallowed by policies");
    return;
  }

  auto& ext =
      sdl_rpc_plugin::SystemCapabilityAppExtension::ExtractExtension(*app);
  ext.SubscribeTo(mobile_apis::SystemCapabilityType::APP_SERVICES);

  smart_objects::SmartObject service_record =
      application_manager_.GetAppServiceManager().PublishAppService(
          manifest, true, connection_key());

  if (service_record.empty()) {
    SendResponse(
        false, mobile_apis::Result::REJECTED, "Failed to publish service");
    return;
  }

  response_params[strings::app_service_record] = service_record;

  SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

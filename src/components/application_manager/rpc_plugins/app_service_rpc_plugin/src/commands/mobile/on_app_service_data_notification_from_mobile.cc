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

#include "app_service_rpc_plugin/commands/mobile/on_app_service_data_notification_from_mobile.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

OnAppServiceDataNotificationFromMobile::OnAppServiceDataNotificationFromMobile(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandNotificationFromMobileImpl(message,
                                        application_manager,
                                        rpc_service,
                                        hmi_capabilities,
                                        policy_handler)
    , plugin_(NULL) {
  auto plugin = application_manager.GetPluginManager().FindPluginToProcess(
      mobile_apis::FunctionID::OnAppServiceDataID,
      app_mngr::commands::Command::CommandSource::SOURCE_MOBILE);
  if (plugin) {
    plugin_ = dynamic_cast<AppServiceRpcPlugin*>(&(*plugin));
  }
}

OnAppServiceDataNotificationFromMobile::
    ~OnAppServiceDataNotificationFromMobile() {}

void OnAppServiceDataNotificationFromMobile::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Received an OnAppServiceData");
  MessageHelper::PrintSmartObject(*message_);

  // Todo: possible revision make service type string.
  mobile_apis::AppServiceType::eType service_type =
      static_cast<mobile_apis::AppServiceType::eType>(
          (*message_)[strings::msg_params][strings::app_service_manifest]
                     [strings::service_type].asUInt());

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  bool result = policy_handler_.CheckAppServiceParameters(
      app->policy_app_id(), std::string(), service_type, NULL);

  if (!result) {
    LOG4CXX_DEBUG(logger_,
                  "Incorrect service type received in "
                  "OnAppServiceDataNotificationFromMobile");
    return;
  }

  SendNotificationToConsumers(
      hmi_apis::FunctionID::eType::AppService_OnAppServiceData);
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

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

#include "app_service_rpc_plugin/commands/mobile/on_app_service_data_notification.h"
#include "app_service_rpc_plugin/app_service_app_extension.h"
#include "application_manager/application_impl.h"
#include "application_manager/helpers/application_helper.h"
#include "application_manager/message_helper.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"

#include "smart_objects/enum_schema_item.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

OnAppServiceDataNotification::OnAppServiceDataNotification(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandNotificationImpl(message,
                              application_manager,
                              rpc_service,
                              hmi_capabilities,
                              policy_handler) {}

OnAppServiceDataNotification::~OnAppServiceDataNotification() {}

void OnAppServiceDataNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Sending OnAppServiceData to consumer");
  MessageHelper::PrintSmartObject(*message_);

  std::string service_type =
      (*message_)[strings::msg_params][strings::service_type].asString();

  auto subscribed_to_app_service_predicate =
      [service_type](const ApplicationSharedPtr app) {
        DCHECK_OR_RETURN(app, false);
        auto& ext = AppServiceAppExtension::ExtractASExtension(*app);
        LOG4CXX_DEBUG(logger_, "Check subscription for type: " << service_type);
        return ext.IsSubscribedToAppService(service_type);
      };

  const std::vector<ApplicationSharedPtr>& applications =
      application_manager::FindAllApps(application_manager_.applications(),
                                       subscribed_to_app_service_predicate);

  std::vector<ApplicationSharedPtr>::const_iterator app_it =
      applications.begin();

  for (; applications.end() != app_it; ++app_it) {
    const ApplicationSharedPtr app = *app_it;
    if (!app) {
      LOG4CXX_ERROR(logger_, "NULL pointer");
      continue;
    }
    LOG4CXX_DEBUG(logger_,
                  "Sending OnAppServiceDataNotification to mobile connection: "
                      << app->app_id());
    (*message_)[app_mngr::strings::params][app_mngr::strings::connection_key] =
        app->app_id();
    SendNotification();
  }
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

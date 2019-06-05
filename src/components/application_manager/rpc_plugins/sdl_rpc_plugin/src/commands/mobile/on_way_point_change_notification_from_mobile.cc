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

#include "app_service_rpc_plugin/commands/mobile/on_way_point_change_notification_from_mobile.h"
#include "application_manager/app_service_manager.h"
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"

namespace app_service_rpc_plugin {
using namespace application_manager;
namespace commands {

OnWayPointChangeNotificationFromMobile::OnWayPointChangeNotificationFromMobile(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandNotificationFromMobileImpl(message,
                                        application_manager,
                                        rpc_service,
                                        hmi_capabilities,
                                        policy_handler) {}

OnWayPointChangeNotificationFromMobile::
    ~OnWayPointChangeNotificationFromMobile() {}

void OnWayPointChangeNotificationFromMobile::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  auto services =
      application_manager_.GetAppServiceManager().GetActiveServices();
  AppService* app_service = NULL;
  for (AppService service : services) {
    if (service.mobile_service && service.connection_key == connection_key()) {
      for (size_t i = 0; i < handled_rpcs.length(); i++) {
        if (handled_rpcs[i].asInt() ==
            (int32_t)mobile_apis::FunctionID::GetWayPointsID) {
          app_service = &service;
          break;
        }
      }
    }

    if (app_service) {
      break;
    }
  }

  if (!app_service) {
    LOG4CXX_ERROR(logger_,
                  "Disallowing OnWayPointChange from mobile application: "
                      << connection_key());
    return;
  }

  SendNotificationToMobile();
}

}  // namespace commands
}  // namespace app_service_rpc_plugin

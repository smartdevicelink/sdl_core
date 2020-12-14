/*
 Copyright (c) 2020, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

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

#include "sdl_rpc_plugin/commands/mobile/on_way_point_change_notification_from_mobile.h"

#include "application_manager/app_service_manager.h"
#include "application_manager/message.h"
#include "application_manager/message_helper.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

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
  SDL_LOG_AUTO_TRACE();

  (*message_)[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);
  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (app.use_count() == 0) {
    SDL_LOG_ERROR(
        "OnWayPointChangeNotificationFromMobile application doesn't exist");
    return;
  }

  auto service =
      application_manager_.GetAppServiceManager().FindWayPointsHandler();
  if (!service || !service->mobile_service ||
      service->connection_key != connection_key()) {
    SDL_LOG_ERROR("Application is not active NAVIGATION ASP");
    return;
  }

  application_manager_.SaveWayPointsMessage(message_, connection_key());

  (*message_)[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);
  rpc_service_.ManageMobileCommand(message_, SOURCE_SDL);
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

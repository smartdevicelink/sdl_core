/*
 Copyright (c) 2018, Ford Motor Company
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

#include "sdl_rpc_plugin/commands/mobile/subscribe_way_points_request.h"

#include "application_manager/app_service_manager.h"
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

SubscribeWayPointsRequest::SubscribeWayPointsRequest(
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

SubscribeWayPointsRequest::~SubscribeWayPointsRequest() {}

void SubscribeWayPointsRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("An application with connection key "
                  << connection_key() << " is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (application_manager_.IsAppSubscribedForWayPoints(*app)) {
    SendResponse(false, mobile_apis::Result::IGNORED);
    return;
  }

  if (application_manager_.IsSubscribedToHMIWayPoints()) {
    application_manager_.SubscribeAppForWayPoints(app, false);
    SendResponse(true, mobile_apis::Result::SUCCESS);
    return;
  }

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
  SendHMIRequest(
      hmi_apis::FunctionID::Navigation_SubscribeWayPoints, NULL, true);
}

void SubscribeWayPointsRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  ApplicationSharedPtr app = application_manager_.application(connection_key());
  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_SubscribeWayPoints: {
      SDL_LOG_INFO("Received Navigation_SubscribeWayPoints event");
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_Navigation);
      hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      std::string response_info;
      GetInfo(message, response_info);
      bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_Navigation);
      if (result) {
        application_manager_.SubscribeAppForWayPoints(app, true);
      } else if (application_manager_.GetAppServiceManager()
                     .FindWayPointsHandler() != nullptr) {
        application_manager_.SubscribeAppForWayPoints(app, false);
        result = true;
        result_code = hmi_apis::Common_Result::WARNINGS;
      }
      SendResponse(result,
                   MessageHelper::HMIToMobileResult(result_code),
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      break;
    }
  }
}

void SubscribeWayPointsRequest::onTimeOut() {
  SDL_LOG_AUTO_TRACE();
  if (application_manager_.GetAppServiceManager().FindWayPointsHandler() !=
      nullptr) {
    ApplicationSharedPtr app =
        application_manager_.application(connection_key());
    application_manager_.SubscribeAppForWayPoints(app, false);
    SendResponse(true,
                 mobile_apis::Result::WARNINGS,
                 "HMI request timeout expired, waypoints are available through "
                 "NAVIGATION service");
  } else {
    SendResponse(
        false, mobile_apis::Result::GENERIC_ERROR, "Request timeout expired");
  }
}

bool SubscribeWayPointsRequest::Init() {
  hash_update_mode_ = HashUpdateMode::kDoHashUpdate;
  return true;
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

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

#include "sdl_rpc_plugin/commands/mobile/get_system_capability_request.h"

#include <set>
#include "application_manager/app_service_manager.h"
#include "application_manager/message_helper.h"
#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

GetSystemCapabilityRequest::GetSystemCapabilityRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler) {}

GetSystemCapabilityRequest::~GetSystemCapabilityRequest() {}

void GetSystemCapabilityRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if ((*message_)[strings::msg_params].empty()) {
    SDL_LOG_ERROR(strings::msg_params << " is empty.");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject response_params(smart_objects::SmartType_Map);
  mobile_apis::SystemCapabilityType::eType response_type =
      static_cast<mobile_apis::SystemCapabilityType::eType>(
          (*message_)[strings::msg_params][strings::system_capability_type]
              .asInt());
  response_params[strings::system_capability][strings::system_capability_type] =
      response_type;

  const HMICapabilities& hmi_capabilities = hmi_capabilities_;

  switch (response_type) {
    case mobile_apis::SystemCapabilityType::NAVIGATION: {
      smart_objects::SmartObject nav_capability(smart_objects::SmartType_Map);
      bool has_nav_capability = false;
      if (hmi_capabilities.navigation_capability()) {
        has_nav_capability = true;
        nav_capability = *hmi_capabilities.navigation_capability();
      }

      has_nav_capability = application_manager_.GetAppServiceManager()
                               .UpdateNavigationCapabilities(nav_capability) ||
                           has_nav_capability;

      if (has_nav_capability) {
        response_params[strings::system_capability]
                       [strings::navigation_capability] = nav_capability;
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::PHONE_CALL: {
      if (hmi_capabilities.phone_capability()) {
        response_params[strings::system_capability][strings::phone_capability] =
            *hmi_capabilities.phone_capability();
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::REMOTE_CONTROL: {
      if (!app->is_remote_control_supported()) {
        SendResponse(false, mobile_apis::Result::DISALLOWED);
        return;
      }
      if (!hmi_capabilities.is_rc_cooperating()) {
        SendResponse(false, mobile_apis::Result::UNSUPPORTED_RESOURCE);
        return;
      }
      if (hmi_capabilities.rc_capability()) {
        response_params[strings::system_capability][strings::rc_capability] =
            *hmi_capabilities.rc_capability();
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::SEAT_LOCATION: {
      if (hmi_capabilities.seat_location_capability()) {
        response_params[strings::system_capability]
                       [strings::seat_location_capability] =
                           *hmi_capabilities.seat_location_capability();
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::VIDEO_STREAMING:
      if (hmi_capabilities.video_streaming_capability()) {
        response_params[strings::system_capability]
                       [strings::video_streaming_capability] =
                           *hmi_capabilities.video_streaming_capability();
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    case mobile_apis::SystemCapabilityType::APP_SERVICES: {
      auto all_services =
          application_manager_.GetAppServiceManager().GetAllServiceRecords();
      response_params[strings::system_capability]
                     [strings::app_services_capabilities] =
                         MessageHelper::CreateAppServiceCapabilities(
                             all_services);
      break;
    }
    case mobile_apis::SystemCapabilityType::DRIVER_DISTRACTION:
      if (hmi_capabilities.driver_distraction_capability()) {
        response_params[strings::system_capability]
                       [strings::driver_distraction_capability] =
                           *hmi_capabilities.driver_distraction_capability();
      } else {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        return;
      }
      break;
    case mobile_apis::SystemCapabilityType::DISPLAYS: {
      auto capabilities = hmi_capabilities.system_display_capabilities();
      if (app->display_capabilities()) {
        capabilities = app->display_capabilities();
      }

      if (!capabilities) {
        SendResponse(false, mobile_apis::Result::DATA_NOT_AVAILABLE);
        SDL_LOG_INFO("system_display_capabilities are not available");
        return;
      }

      response_params[strings::system_capability]
                     [strings::display_capabilities] = *capabilities;
      break;
    }
    default:  // Return unsupported resource
      SendResponse(false, mobile_apis::Result::UNSUPPORTED_RESOURCE);
      return;
  }

  const char* info = nullptr;
  // Ignore subscription/unsubscription for DISPLAYS type
  if (mobile_apis::SystemCapabilityType::DISPLAYS != response_type) {
    if ((*message_)[app_mngr::strings::msg_params].keyExists(
            strings::subscribe)) {
      auto& ext = SystemCapabilityAppExtension::ExtractExtension(*app);
      if ((*message_)[app_mngr::strings::msg_params][strings::subscribe]
              .asBool() == true) {
        SDL_LOG_DEBUG("Subscribe to system capability: "
                      << response_type << " for app_id: " << app->app_id());
        ext.SubscribeTo(response_type);
      } else {
        SDL_LOG_DEBUG("Unsubscribe from system capability: "
                      << response_type << " for app_id: " << app->app_id());
        ext.UnsubscribeFrom(response_type);
      }
    }
  } else {
    if ((*message_)[app_mngr::strings::msg_params].keyExists(
            strings::subscribe)) {
      info =
          "Subscribe parameter is ignored. Auto Subscription/Unsubscription is "
          "used for DISPLAY capability type.";
    }
  }

  SendResponse(true, mobile_apis::Result::SUCCESS, info, &response_params);
}

void GetSystemCapabilityRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_INFO("GetSystemCapabilityRequest on_event");
}

}  // namespace commands
}  // namespace sdl_rpc_plugin

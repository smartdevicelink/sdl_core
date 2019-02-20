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
#include "sdl_rpc_plugin/extensions/system_capability_app_extension.h"
#include "application_manager/message_helper.h"
#include <set>

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

GetSystemCapabilityRequest::GetSystemCapabilityRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

GetSystemCapabilityRequest::~GetSystemCapabilityRequest() {}

void GetSystemCapabilityRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if ((*message_)[strings::msg_params].empty()) {
    LOG4CXX_ERROR(logger_, strings::msg_params << " is empty.");
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
      if (hmi_capabilities.navigation_capability()) {
        response_params[strings::system_capability]
                       [strings::navigation_capability] =
                           *hmi_capabilities.navigation_capability();
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
          application_manager_.GetAppServiceManager().GetAllServices();
      response_params
          [strings::system_capability][strings::app_services_capabilities] =
              MessageHelper::CreateAppServiceCapabilities(all_services);

      break;
    }
    default:  // Return unsupported resource
      SendResponse(false, mobile_apis::Result::UNSUPPORTED_RESOURCE);
      return;
  }

  if ((*message_)[app_mngr::strings::msg_params].keyExists(
          strings::subscribe)) {
    auto& ext = SystemCapabilityAppExtension::ExtractExtension(*app);
    if ((*message_)[app_mngr::strings::msg_params][strings::subscribe]
            .asBool() == true) {
      LOG4CXX_DEBUG(logger_,
                    "GETSYSCAP: Subscribe to system capability - "
                        << response_type);
      ext.SubscribeTo(response_type);
    } else {
      LOG4CXX_DEBUG(logger_,
                    "GETSYSCAP: Unsubscribe from system capability - "
                        << response_type);
      ext.UnsubscribeFrom(response_type);
    }
  }

  SendResponse(true, mobile_apis::Result::SUCCESS, NULL, &response_params);
}

void GetSystemCapabilityRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "GetSystemCapabilityRequest on_event");
}

}  // namespace commands
}  // namespace application_manager

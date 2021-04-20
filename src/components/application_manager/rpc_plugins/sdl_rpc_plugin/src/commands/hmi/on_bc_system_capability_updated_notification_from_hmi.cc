/*
 * Copyright (c) 2019, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "sdl_rpc_plugin/commands/hmi/on_bc_system_capability_updated_notification_from_hmi.h"

#include "application_manager/display_capabilities_builder.h"
#include "application_manager/message_helper.h"
#include "extensions/system_capability_app_extension.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

OnBCSystemCapabilityUpdatedNotificationFromHMI::
    OnBCSystemCapabilityUpdatedNotificationFromHMI(
        const application_manager::commands::MessageSharedPtr& message,
        ApplicationManager& application_manager,
        rpc_service::RPCService& rpc_service,
        HMICapabilities& hmi_capabilities,
        policy::PolicyHandlerInterface& policy_handle)
    : NotificationFromHMI(message,
                          application_manager,
                          rpc_service,
                          hmi_capabilities,
                          policy_handle) {}

OnBCSystemCapabilityUpdatedNotificationFromHMI::
    ~OnBCSystemCapabilityUpdatedNotificationFromHMI() {}

OnBCSystemCapabilityUpdatedNotificationFromHMI::
    ProcessSystemDisplayCapabilitiesResult
    OnBCSystemCapabilityUpdatedNotificationFromHMI::
        ProcessSystemDisplayCapabilities(
            const smart_objects::SmartObject& display_capabilities) {
  SDL_LOG_AUTO_TRACE();

  if (!(*message_)[strings::msg_params].keyExists(strings::app_id)) {
    SDL_LOG_DEBUG("Updating general display capabilities");
    hmi_capabilities_.set_system_display_capabilities(display_capabilities);
    return ProcessSystemDisplayCapabilitiesResult::SUCCESS;
  }

  const auto app_id =
      (*message_)[strings::msg_params][strings::app_id].asUInt();
  auto app = application_manager_.application(app_id);
  if (!app) {
    SDL_LOG_ERROR("Application with app_id " << app_id << " is not registered");
    return ProcessSystemDisplayCapabilitiesResult::FAIL;
  }

  SDL_LOG_DEBUG("Updating display capabilities for app " << app_id);
  app->set_display_capabilities(display_capabilities);

  RemoveAppIdFromNotification();
  auto& builder = app->display_capabilities_builder();
  if (builder.IsWaitingForWindowCapabilities(display_capabilities)) {
    SDL_LOG_DEBUG("Application is waiting for capabilities");
    builder.UpdateDisplayCapabilities(display_capabilities);
    return ProcessSystemDisplayCapabilitiesResult::CAPABILITIES_CACHED;
  }

  return ProcessSystemDisplayCapabilitiesResult::SUCCESS;
}

void OnBCSystemCapabilityUpdatedNotificationFromHMI::
    RemoveAppIdFromNotification() {
  (*message_)[strings::params][strings::connection_key] =
      (*message_)[strings::msg_params][strings::app_id];
  (*message_)[strings::msg_params].erase(strings::app_id);
}

bool OnBCSystemCapabilityUpdatedNotificationFromHMI::
    ProcessVideoStreamingCapability(
        const smart_objects::SmartObject& system_capability) {
  if (!system_capability.keyExists(strings::video_streaming_capability)) {
    SDL_LOG_WARN(
        "VideoStreamingCapability is absent in the notification. "
        "Notification Will be ignored");
    return false;
  }
  if (!(*message_)[strings::msg_params].keyExists(strings::app_id)) {
    SDL_LOG_WARN(
        "Notification doesn't contain an application id. Will "
        "be ignored");
    return false;
  }

  const auto app_id =
      (*message_)[strings::msg_params][strings::app_id].asUInt();

  auto app = application_manager_.application(app_id);
  if (!app) {
    SDL_LOG_WARN("Application with app_id: "
                 << app_id
                 << " isn't registered. Notification will be ignored");
    return false;
  }

  auto& system_capability_extension =
      SystemCapabilityAppExtension::ExtractExtension(*app);

  if (!system_capability_extension.IsSubscribedTo(
          mobile_apis::SystemCapabilityType::VIDEO_STREAMING)) {
    SDL_LOG_WARN("The Application with app_id: "
                 << app_id
                 << " isn't subscribed to the VIDEO_STREAMING system "
                    "capability type. Notification will be ignored");
    return false;
  }
  return true;
}

void OnBCSystemCapabilityUpdatedNotificationFromHMI::Run() {
  SDL_LOG_AUTO_TRACE();

  // Prepare SmartObject for mobile factory
  (*message_)[strings::params][strings::function_id] = static_cast<int32_t>(
      mobile_apis::FunctionID::OnSystemCapabilityUpdatedID);

  const auto& system_capability =
      (*message_)[strings::msg_params][strings::system_capability];

  const auto system_capability_type =
      static_cast<mobile_apis::SystemCapabilityType::eType>(
          system_capability[strings::system_capability_type].asInt());

  switch (system_capability_type) {
    case mobile_apis::SystemCapabilityType::DISPLAYS: {
      if (system_capability.keyExists(strings::display_capabilities)) {
        const auto result = ProcessSystemDisplayCapabilities(
            system_capability[strings::display_capabilities]);
        if (ProcessSystemDisplayCapabilitiesResult::FAIL == result) {
          SDL_LOG_ERROR(
              "Failed to process display capabilities. Notification will "
              "be ignored");
          return;
        } else if (ProcessSystemDisplayCapabilitiesResult::
                       CAPABILITIES_CACHED == result) {
          SDL_LOG_TRACE("Capabilities are being cached for resuming app");
          return;
        }
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::REMOTE_CONTROL: {
      if (system_capability.keyExists(strings::rc_capability)) {
        SDL_LOG_DEBUG("Updating RC Capabilities");
        hmi_capabilities_.set_rc_capability(
            system_capability[strings::rc_capability]);
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::VIDEO_STREAMING: {
      if (!ProcessVideoStreamingCapability(system_capability)) {
        return;
      }
      RemoveAppIdFromNotification();
      break;
    }
    default: { SDL_LOG_ERROR("Unknown system capability type received"); }
  }

  SendNotificationToMobile(message_);
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

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
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

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
  LOG4CXX_AUTO_TRACE(logger_);

  if (!(*message_)[strings::msg_params].keyExists(strings::app_id)) {
    LOG4CXX_DEBUG(logger_, "Updating general display capabilities");
    hmi_capabilities_.set_system_display_capabilities(display_capabilities);
    return ProcessSystemDisplayCapabilitiesResult::SUCCESS;
  }

  const auto app_id =
      (*message_)[strings::msg_params][strings::app_id].asUInt();
  auto app = application_manager_.application(app_id);
  if (!app) {
    LOG4CXX_ERROR(logger_,
                  "Application with app_id " << app_id << " is not registered");
    return ProcessSystemDisplayCapabilitiesResult::FAIL;
  }

  LOG4CXX_DEBUG(logger_, "Updating display capabilities for app " << app_id);
  app->set_display_capabilities(display_capabilities);

  // Remove app_id from notification to mobile
  (*message_)[strings::params][strings::connection_key] =
      (*message_)[strings::msg_params][strings::app_id];
  (*message_)[strings::msg_params].erase(strings::app_id);

  auto& builder = app->display_capabilities_builder();
  if (builder.IsWaitingForWindowCapabilities(display_capabilities)) {
    LOG4CXX_DEBUG(logger_, "Application is waiting for capabilities");
    builder.UpdateDisplayCapabilities(display_capabilities);
    return ProcessSystemDisplayCapabilitiesResult::CAPABILITIES_CACHED;
  }

  return ProcessSystemDisplayCapabilitiesResult::SUCCESS;
}

void OnBCSystemCapabilityUpdatedNotificationFromHMI::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  // Prepare SmartObject for mobile factory
  (*message_)[strings::params][strings::function_id] = static_cast<int32_t>(
      mobile_apis::FunctionID::OnSystemCapabilityUpdatedID);

  const auto& system_capability =
      (*message_)[strings::msg_params][strings::system_capability];

  switch (system_capability[strings::system_capability_type].asInt()) {
    case mobile_apis::SystemCapabilityType::DISPLAYS: {
      if (system_capability.keyExists(strings::display_capabilities)) {
        const auto result = ProcessSystemDisplayCapabilities(
            system_capability[strings::display_capabilities]);
        if (ProcessSystemDisplayCapabilitiesResult::FAIL == result) {
          LOG4CXX_ERROR(
              logger_,
              "Failed to process display capabilities. Notification will "
              "be ignored");
          return;
        } else if (ProcessSystemDisplayCapabilitiesResult::
                       CAPABILITIES_CACHED == result) {
          LOG4CXX_TRACE(logger_,
                        "Capabilities are being cached for resuming app");
          return;
        }
      }
      break;
    }
    case mobile_apis::SystemCapabilityType::REMOTE_CONTROL: {
      if (system_capability.keyExists(strings::rc_capability)) {
        LOG4CXX_DEBUG(logger_, "Updating RC Capabilities");
        hmi_capabilities_.set_rc_capability(
            system_capability[strings::rc_capability]);
      }
      break;
    }
  }

  SendNotificationToMobile(message_);
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

/*
 * Copyright (c) 2018, Ford Motor Company
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

#include <set>
#include "sdl_rpc_plugin/commands/hmi/on_driver_distraction_notification.h"

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

namespace hmi {

namespace {
struct OnDriverDistractionProcessor {
  OnDriverDistractionProcessor(
      OnDriverDistractionNotification& command,
      smart_objects::SmartObjectSPtr on_driver_distraction_so,
      ApplicationManager& application_manager)
      : command_(command)
      , on_driver_distraction_so_(on_driver_distraction_so)
      , application_manager_(application_manager)
      , stringified_function_id_(MessageHelper::StringifiedFunctionID(
            mobile_api::FunctionID::OnDriverDistractionID)) {}

  void operator()(ApplicationSharedPtr application) {
    if (application) {
      (*on_driver_distraction_so_)[strings::params][strings::connection_key] =
          application->app_id();
      const RPCParams params;
      policy::CheckPermissionResult result;
      application_manager_.GetPolicyHandler().CheckPermissions(
          application, stringified_function_id_, params, result);
      if (result.hmi_level_permitted != policy::kRpcAllowed) {
        MobileMessageQueue messages;
        application->SwapMobileMessageQueue(messages);
        messages.erase(
            std::remove_if(
                messages.begin(),
                messages.end(),
                [this](smart_objects::SmartObjectSPtr message) {
                  return (*message)[strings::params][strings::function_id]
                             .asString() == stringified_function_id_;
                }),
            messages.end());
        application->PushMobileMessage(on_driver_distraction_so_);
        return;
      }
      command_.SendNotificationToMobile(on_driver_distraction_so_);
    }
  }

 private:
  OnDriverDistractionNotification& command_;
  smart_objects::SmartObjectSPtr on_driver_distraction_so_;
  ApplicationManager& application_manager_;
  std::string stringified_function_id_;
};
}

OnDriverDistractionNotification::OnDriverDistractionNotification(
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

OnDriverDistractionNotification::~OnDriverDistractionNotification() {}

void OnDriverDistractionNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  const auto state =
      static_cast<hmi_apis::Common_DriverDistractionState::eType>(
          (*message_)[strings::msg_params][hmi_notification::state].asInt());
  application_manager_.set_driver_distraction_state(state);

  auto on_driver_distraction = std::make_shared<smart_objects::SmartObject>();
  if (!on_driver_distraction) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    return;
  }
  (*on_driver_distraction)[strings::params][strings::function_id] =
      mobile_api::FunctionID::OnDriverDistractionID;
  (*on_driver_distraction)[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);
  (*on_driver_distraction)[strings::msg_params][mobile_notification::state] =
      state;

  const auto lock_screen_dismissal =
      application_manager_.GetPolicyHandler().LockScreenDismissalEnabledState();

  if (lock_screen_dismissal &&
      hmi_apis::Common_DriverDistractionState::DD_ON == state) {
    (*on_driver_distraction)
        [strings::msg_params]
        [mobile_notification::lock_screen_dismissal_enabled] =
            *lock_screen_dismissal;
  }

  const ApplicationSet applications =
      application_manager_.applications().GetData();

  OnDriverDistractionProcessor processor(
      *this, on_driver_distraction, application_manager_);
  std::for_each(applications.begin(), applications.end(), processor);
}

}  // namespace hmi

}  // namespace commands

}  // namespace application_manager

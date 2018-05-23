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

#include "sdl_rpc_plugin/commands/mobile/on_hmi_status_notification.h"

#include "application_manager/message_helper.h"
#include "application_manager/message.h"
#include "interfaces/MOBILE_API.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {

OnHMIStatusNotification::OnHMIStatusNotification(
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

OnHMIStatusNotification::~OnHMIStatusNotification() {}

void OnHMIStatusNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  (*message_)[strings::params][strings::message_type] =
      static_cast<int32_t>(application_manager::MessageType::kNotification);
  ApplicationSharedPtr app = application_manager_.application(connection_key());
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "OnHMIStatusNotification application doesn't exist");
    return;
  }

  // If the response has no hmi level, return and don't send the notification
  if (!(*message_)[strings::msg_params].keyExists(strings::hmi_level)) {
    // our notification clearly isn't well-formed
    LOG4CXX_ERROR(logger_, "OnHMIStatusNotification has no hmiLevel field");
    return;
  }

  // NOTE c++ maps default-construct on the [] operator, so if there is no
  // hmiLevel field this will create one that is invalid
  mobile_apis::HMILevel::eType hmi_level =
      static_cast<mobile_apis::HMILevel::eType>(
          (*message_)[strings::msg_params][strings::hmi_level].asInt());
  if ((mobile_apis::HMILevel::HMI_BACKGROUND == hmi_level) ||
      (mobile_apis::HMILevel::HMI_NONE == hmi_level)) {
    if (!(app->tts_properties_in_none())) {
      app->set_tts_properties_in_none(true);
      LOG4CXX_INFO(logger_,
                   "OnHMIStatusNotification::Send TTS GlobalProperties"
                   " with empty array to HMI");
      MessageHelper::SendTTSGlobalProperties(app, false, application_manager_);
    }
  } else if ((mobile_apis::HMILevel::HMI_FULL == hmi_level) ||
             (mobile_apis::HMILevel::HMI_LIMITED == hmi_level)) {
    if (!(app->tts_properties_in_full())) {
      app->set_tts_properties_in_full(true);
      LOG4CXX_INFO(logger_,
                   "OnHMIStatusNotification AddAppToTTSGlobalPropertiesList");
      application_manager_.AddAppToTTSGlobalPropertiesList(app->app_id());
    }
  }
  SendNotification();
}

}  // namespace commands

}  // namespace application_manager

/*

 Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/mobile/on_hmi_status_notification_from_mobile.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/message.h"
#include "interfaces/MOBILE_API.h"

namespace application_manager {
namespace commands {

bool OnHMIStatusNotificationFromMobile::is_apps_requested_ = false;

OnHMIStatusNotificationFromMobile::OnHMIStatusNotificationFromMobile(
    const MessageSharedPtr& message)
    : CommandNotificationFromMobileImpl(message) {
}

OnHMIStatusNotificationFromMobile::~OnHMIStatusNotificationFromMobile() {
}

void OnHMIStatusNotificationFromMobile::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  (*message_)[strings::params][strings::message_type] = static_cast<int32_t> (
      application_manager::MessageType::kNotification);
  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
        connection_key());
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_,
                  "OnHMIStatusNotificationFromMobile application doesn't exist");
    return;
  }

  // In case if this notification will be received from mobile side, it will
  // mean, that app is in foreground on mobile. This should trigger remote
  // apps list query for SDL 4.0 app
  if (is_apps_requested_) {
    LOG4CXX_DEBUG(logger_, "Remote apps list had been requested already.");
    return;
  }
  if (ProtocolVersion::kV4 == app->protocol_version()) {
    MessageHelper::SendQueryApps(connection_key());
    is_apps_requested_ = true;
  }
}

}  // namespace commands

}  // namespace application_manager

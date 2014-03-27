/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/hmi/on_button_event_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "utils/logger.h"

namespace {
  const int hook_array[] =
  { hmi_apis::Common_ButtonName::TUNEUP,
    hmi_apis::Common_ButtonName::SEEKLEFT,
    hmi_apis::Common_ButtonName::TUNEDOWN,
    hmi_apis::Common_ButtonName::SEEKRIGHT,
    hmi_apis::Common_ButtonName::OK};
  const std::vector<int> hook_buttons (
      hook_array, hook_array + sizeof(hook_array) / sizeof(hook_array[0]));
  std::vector<int>::const_iterator hook_iterator(hook_buttons.begin());
}
namespace application_manager {

namespace commands {

namespace hmi {

OnButtonEventNotification::OnButtonEventNotification(
    const MessageSharedPtr& message) : NotificationFromHMI(message) {
}

void OnButtonEventNotification::Run() {
  LOG4CXX_INFO(logger_, "OnButtonEventNotification::Run");

  (*message_)[strings::params][strings::function_id] =
      static_cast<int32_t>(mobile_apis::FunctionID::OnButtonEventID);

  SendNotificationToMobile(message_);

  const int button_mode =  (*message_)[strings::msg_params][hmi_response::button_mode].asInt();
  if(hmi_apis::Common_ButtonEventMode::BUTTONUP == button_mode) {
    const int button_name =  (*message_)[strings::msg_params][hmi_response::button_name].asInt();
    if(*hook_iterator != button_name) {
      hook_iterator = hook_buttons.begin();
    }
    else {
      ++hook_iterator;
      if(hook_iterator == hook_buttons.end()) {
        LOG4CXX_WARN(logger_,
                     "Certificate recall will be called for FULL HMI applications!");
        const std::set<ApplicationSharedPtr> applications =
            ApplicationManagerImpl::instance()->applications();
        std::set<ApplicationSharedPtr>::iterator it = applications.begin();
        for (;applications.end() != it; ++it) {
          ApplicationSharedPtr app = (*it);
          if(mobile_api::HMILevel::HMI_FULL == app->hmi_level() ) {
            LOG4CXX_WARN(logger_, "Shall call CertificateRecall - not implemented!");
            // TODO (EZamakhov) : add app policy update after merge
            break;
          }
        }
      }
    }
  }
}

}  // namespace hmi

}  // namespace commands

}  // namespace application_manager


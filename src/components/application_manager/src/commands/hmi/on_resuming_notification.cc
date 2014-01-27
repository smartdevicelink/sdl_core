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

#include "application_manager/commands/hmi/on_resuming_notification.h"

#include "application_manager/application_manager_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"
#include "application_manager/commands/hmi/response_from_hmi.h"
#include "application_manager/message_helper.h"
namespace application_manager {

namespace commands {

OnResumingNotification::OnResumingNotification(const MessageSharedPtr& message)
    : NotificationFromHMI(message) {
}

OnResumingNotification::~OnResumingNotification() {
}

void OnResumingNotification::Run() {
  LOG4CXX_INFO(logger_, "OnResumingNotification::Run");
  const hmi_apis::Common_Result::eType result =
      static_cast<hmi_apis::Common_Result::eType>(
          (*message_)[strings::params][hmi_notification::result].asInt());

  if (hmi_apis::Common_Result::SUCCESS == result) {
    int32_t correlation_id = (*message_)[strings::params][strings::app_id].asInt();
    const uint32_t app_id = ApplicationManagerImpl::instance()->
        application_id(correlation_id);
    if (!app_id) {
      LOG4CXX_ERROR(logger_, "Error app_id = "<<app_id);
      return;
    }
    Application* application = ApplicationManagerImpl::instance()->application(app_id);
    if (application) {
      if (ApplicationManagerImpl::instance()->RestoreApplicationHMILevel(application)) {
        if (application->hmi_level() == mobile_apis::HMILevel::HMI_FULL) {
          MessageHelper::SendActivateAppToHMI(app_id);
        }
        MessageHelper::SendHMIStatusNotification(*application);
      }
    } else {
      LOG4CXX_ERROR(logger_, "Application cannot be resumed");
    }
  } else {
    LOG4CXX_ERROR(logger_, "Error result code"<<code);
  }
}
}  // namespace commands

}  // namespace application_manager


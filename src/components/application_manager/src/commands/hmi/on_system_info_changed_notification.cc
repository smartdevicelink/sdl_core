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

#include "application_manager/commands/hmi/on_system_info_changed_notification.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

OnSystemInfoChangedNotification::OnSystemInfoChangedNotification(
    const MessageSharedPtr& message)
    : NotificationFromHMI(message) {
}

OnSystemInfoChangedNotification::~OnSystemInfoChangedNotification() {
}

void OnSystemInfoChangedNotification::Run() {
  LOG4CXX_INFO(logger_, "OnSystemInfoChangedNotification::Run");
  uint32_t lang_code =
      (*message_)[strings::msg_params][strings::language].asUInt();
  const std::string language =
      application_manager::MessageHelper::CommonLanguageToString(
        static_cast<hmi_apis::Common_Language::eType>(lang_code));

  policy::PolicyHandler::instance()->OnSystemInfoChanged(language);
}

}  // namespace commands

}  // namespace application_manager



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

#include "application_manager/commands/mobile/on_command_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"

namespace application_manager {

namespace commands {

OnCommandNotification::OnCommandNotification(const MessageSharedPtr& message)
    : CommandNotificationImpl(message) {
}

OnCommandNotification::~OnCommandNotification() {
}

void OnCommandNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(
      (*message_)[strings::msg_params][strings::app_id].asInt());

  if (!app) {
    LOG4CXX_ERROR(logger_, "No application associated with session key");
    return;
  }

  const uint32_t cmd_id = (*message_)[strings::msg_params][strings::cmd_id]
      .asUInt();

  if (!app->FindCommand(cmd_id)) {
    LOG4CXX_ERROR(logger_,
                      " No applications found for the command " << cmd_id);
    return;
  }

  (*message_)[strings::params][strings::connection_key] = app->app_id();
  // remove app_id from notification
  if ((*message_)[strings::msg_params].keyExists(strings::app_id)) {
    (*message_)[strings::msg_params].erase(strings::app_id);
  }

  SendNotification();
}

}  // namespace commands

}  // namespace application_manager

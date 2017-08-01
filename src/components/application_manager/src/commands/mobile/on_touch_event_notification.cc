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

#include "application_manager/commands/mobile/on_touch_event_notification.h"

#include "application_manager/application_impl.h"

namespace application_manager {

namespace commands {

namespace mobile {

OnTouchEventNotification::OnTouchEventNotification(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandNotificationImpl(message, application_manager) {}

OnTouchEventNotification::~OnTouchEventNotification() {}

void OnTouchEventNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  const std::vector<ApplicationSharedPtr>& applications_with_navi =
      application_manager_.applications_with_navi();

  const std::vector<ApplicationSharedPtr>& projection_applications =
      application_manager_.applications_with_mobile_projection();

  std::vector<ApplicationSharedPtr>::const_iterator nav_it =
      applications_with_navi.begin();

  for (; applications_with_navi.end() != nav_it; ++nav_it) {
    ApplicationSharedPtr app = *nav_it;
    if (app->IsFullscreen()) {
      (*message_)[strings::params][strings::connection_key] = app->app_id();
      SendNotification();
    }
  }

  std::vector<ApplicationSharedPtr>::const_iterator projection_it =
      projection_applications.begin();

  for (; projection_applications.end() != projection_it; ++projection_it) {
    ApplicationSharedPtr projection_app = *projection_it;
    if (projection_app->IsFullscreen()) {
      (*message_)[strings::params][strings::connection_key] =
          projection_app->app_id();
      SendNotification();
    }
  }
}

}  // namespace mobile

}  // namespace commands

}  // namespace application_manager

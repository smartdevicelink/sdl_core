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

#ifdef MODIFY_FUNCTION_SIGN
#include <global_first.h>
#endif
#include "application_manager/commands/hmi/on_search_applist_notification.h"
#include "application_manager/application_manager_impl.h"

namespace application_manager {

namespace commands {

OnSearchAppListNotification::OnSearchAppListNotification(
  const MessageSharedPtr& message): NotificationFromHMI(message) {
}

OnSearchAppListNotification::~OnSearchAppListNotification() {
}

void OnSearchAppListNotification::Run() {
  LOG4CXX_INFO(logger_, "OnSearchAppListNotification::Run");
  smart_objects::SmartObject* notification = new smart_objects::SmartObject;
  if (!notification) {
    // TODO(VS): please add logger.
    return;
  }
  smart_objects::SmartObject& message = *notification;

  message[strings::params][strings::function_id] =
    static_cast<int32_t>(hmi_apis::FunctionID::BasicCommunication_OnFindAppList);

  message[strings::params][strings::message_type] =
    static_cast<int32_t>(application_manager::MessageType::kNotification);
  int index = 0;

  ApplicationManagerImpl::ApplicationListAccessor accessor;
  const ApplicationManagerImpl::ApplictionSet applications = accessor.applications();

  ApplicationManagerImpl::ApplictionSetConstIt it = applications.begin();
  for (; it != applications.end(); ++it){
    message[strings::msg_params][strings::applications][index][strings::app_name] =
      (*it)->name();

    const smart_objects::SmartObject* ngn_media_screen_name =
      (*it)->ngn_media_screen_name();

    if (ngn_media_screen_name) {
      message[strings::msg_params][strings::applications][index]
      [strings::ngn_media_screen_app_name] = *ngn_media_screen_name;
    }

    message[strings::msg_params][strings::applications][index][strings::icon] =
      (*it)->app_icon_path();

    std::string dev_name = ApplicationManagerImpl::instance()->GetDeviceName(
                            (*it)->device());
    message[strings::msg_params][strings::applications][index][strings::device_name] =
      dev_name;

    message[strings::msg_params][strings::applications][index][strings::app_id] =
      (*it)->app_id();

    message[strings::msg_params][strings::applications][index][strings::hmi_display_language_desired] =
      static_cast<int32_t>((*it)->ui_language());

    message[strings::msg_params][strings::applications][index][strings::is_media_application] =
      (*it)->is_media_application();

    const smart_objects::SmartObject* app_type = (*it)->app_types();

    if (app_type) {
      message[strings::msg_params][strings::applications][index][strings::app_type] =
        *app_type;
    }
    ++index;
  }
  
  DCHECK(ApplicationManagerImpl::instance()->ManageHMICommand(notification));
}

}  // namespace commands

}  // namespace application_manager


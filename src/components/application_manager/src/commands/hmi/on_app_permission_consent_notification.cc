/**
 * Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/commands/hmi/on_app_permission_consent_notification.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/policies/policy_handler.h"

namespace application_manager {

namespace commands {

OnAppPermissionConsent::OnAppPermissionConsent(const MessageSharedPtr& message)
    : NotificationFromHMI(message) {
}

OnAppPermissionConsent::~OnAppPermissionConsent() {
}

void OnAppPermissionConsent::Run() {
  LOG4CXX_INFO(logger_, "OnAppPermissionConsent::Run");
  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];

  policy::PermissionConsent permission_consent;
  // If user defined group permissions for specific app
  if (msg_params.keyExists(strings::app_id)) {
    permission_consent.policy_app_id = msg_params[strings::app_id].asString();

    typedef std::set<Application*> ApplicationList;
    const ApplicationList app_list =
        application_manager::ApplicationManagerImpl::instance()->applications();

    ApplicationList::const_iterator it_app_list = app_list.begin();
    ApplicationList::const_iterator it_app_list_end = app_list.end();
    for (; it_app_list != it_app_list_end; ++it_app_list) {
      if (permission_consent.policy_app_id.compare(
            (*it_app_list)->mobile_app_id()->asString()) == 0) {
        policy::DeviceParams device_params;
        application_manager::MessageHelper::GetDeviceInfoForHandle(
              (*it_app_list)->device(),
              &device_params);

        permission_consent.device_id = device_params.device_mac_address;
      }
    }
  }

  smart_objects::SmartArray* user_consent =
      msg_params["consentedFunctions"].asArray();

  smart_objects::SmartArray::const_iterator it = user_consent->begin();
  smart_objects::SmartArray::const_iterator it_end = user_consent->end();
  for (; it != it_end; ++it) {
    policy::FunctionalGroupPermission permissions;
    permissions.group_id = (*it)["id"].asInt();
    permissions.group_name = (*it)["name"].asString();
    permissions.is_allowed = (*it)["allowed"].asBool();
    permission_consent.group_permissions.push_back(permissions);
  }

  permission_consent.consent_source = msg_params["source"].asString();

  policy::PolicyHandler::instance()->OnAppPermissionConsent(permission_consent);
}

}  // namespace commands

}  // namespace application_manager

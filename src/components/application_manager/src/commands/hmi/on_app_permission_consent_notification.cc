
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
#include "application_manager/application_manager.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/message_helper.h"
#include <algorithm>
#include <functional>
#include <string>
#include "policy/policy_types.h"
#include "smart_objects/smart_object.h"

namespace {

/**
 * @brief Converts SmartObject data to group permission status and appends to
 * collection
 */
struct PermissionsAppender
    : public std::unary_function<void,
                                 const smart_objects::SmartArray::value_type&> {
  PermissionsAppender(policy::PermissionConsent& consents)
      : allowed_key_(application_manager::hmi_response::allowed)
      , consents_(consents) {}
  void operator()(const smart_objects::SmartArray::value_type& item) const {
    using namespace policy;
    using namespace application_manager;

    FunctionalGroupPermission permissions;

    permissions.group_id = static_cast<int32_t>(item[strings::id].asInt());
    permissions.group_alias = item[strings::name].asString();

    if (item.keyExists(allowed_key_)) {
      permissions.state =
          item[allowed_key_].asBool() ? kGroupAllowed : kGroupDisallowed;
    }

    consents_.group_permissions.push_back(permissions);
  }

 private:
  const std::string allowed_key_;
  policy::PermissionConsent& consents_;
};

/**
 * @brief Converts SmartObject data to customer connectivity status item and
 * appends to collection
 */
struct CCSStatusAppender
    : std::unary_function<void, const smart_objects::SmartArray::value_type&> {
  CCSStatusAppender(policy::CCSStatus& ccs_status) : ccs_status_(ccs_status) {}
  void operator()(const smart_objects::SmartArray::value_type& item) const {
    using namespace policy;
    using namespace hmi_apis;
    using namespace application_manager;

    CCSStatusItem status_item(
        static_cast<uint32_t>(item[strings::entity_type].asUInt()),
        static_cast<uint32_t>(item[strings::entity_id].asUInt()),
        static_cast<Common_EntityStatus::eType>(
            item[strings::status].asUInt()) == Common_EntityStatus::ON
            ? policy::kStatusOn
            : policy::kStatusOff);

    ccs_status_.insert(status_item);
  }

 private:
  policy::CCSStatus& ccs_status_;
};

}  // namespace

namespace application_manager {

namespace commands {

OnAppPermissionConsentNotification::OnAppPermissionConsentNotification(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : NotificationFromHMI(message, application_manager) {}

OnAppPermissionConsentNotification::~OnAppPermissionConsentNotification() {}

void OnAppPermissionConsentNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject& msg_params = (*message_)[strings::msg_params];

  uint32_t connection_key = 0;

  policy::PermissionConsent permission_consent;

  PermissionConsent permission_consent;
  if (msg_params.keyExists(strings::consented_functions)) {
    const SmartArray* user_consents =
        msg_params[strings::consented_functions].asArray();

  if (msg_params.keyExists("consentedFunctions")) {
    smart_objects::SmartArray* user_consent =
        msg_params["consentedFunctions"].asArray();

    smart_objects::SmartArray::const_iterator it = user_consent->begin();
    smart_objects::SmartArray::const_iterator it_end = user_consent->end();
    for (; it != it_end; ++it) {
      policy::FunctionalGroupPermission permissions;
      permissions.group_id = (*it)["id"].asInt();
      permissions.group_alias = (*it)["name"].asString();
      if ((*it).keyExists("allowed")) {
        permissions.state = (*it)["allowed"].asBool()
                                ? policy::kGroupAllowed
                                : policy::kGroupDisallowed;
      } else {
        permissions.state = policy::kGroupUndefined;
      }

      permission_consent.group_permissions.push_back(permissions);
    }

    permission_consent.consent_source = msg_params[strings::source].asString();
  }

  CCSStatus ccs_status;
  if (msg_params.keyExists(strings::ccs_status)) {
    const SmartArray* system_ccs_status =
        msg_params[strings::ccs_status].asArray();

    application_manager_.GetPolicyHandler().OnAppPermissionConsent(
        connection_key, permission_consent);
  }
}

}  // namespace commands

}  // namespace application_manager

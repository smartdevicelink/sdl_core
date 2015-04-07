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

#include <vector>
#include <algorithm>
#include "application_manager/core_service.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/policies/policy_handler.h"
#include "json/json.h"

namespace application_manager {

namespace {
struct AppExtensionPredicate {
  AppExtensionUID uid;
      bool operator() (const ApplicationSharedPtr app) {
        return app? app->QueryInterface(uid).valid() : false;
      }
};
}

CoreService::CoreService() {
}

CoreService::~CoreService() {
}

mobile_apis::Result::eType CoreService::CheckPolicyPermissions(MessagePtr msg) {
  ApplicationSharedPtr app = GetApplication(msg->connection_key());
  if (!app) {
    return mobile_apis::Result::eType::APPLICATION_NOT_REGISTERED;
  }

  CommandParametersPermissions params;
  const mobile_apis::Result::eType ret = ApplicationManagerImpl::instance()
      ->CheckPolicyPermissions(app->mobile_app_id()->asString(),
                               app->hmi_level(), msg->function_name(), &params);

  if (ret != mobile_apis::Result::eType::SUCCESS) {
    return ret;
  }

  FilterParameters(msg, params);
  if (!AreParametersAllowed(msg, params)) {
    return mobile_apis::Result::eType::DISALLOWED;
  }

  return ret;
}

TypeAccess CoreService::CheckAccess(const ApplicationId& app_id,
                                    const PluginFunctionID& function_id,
                                    const SeatLocation& seat,
                                    const SeatLocation& zone) {
  ApplicationSharedPtr app = GetApplication(app_id);
  if (app) {
    return policy::PolicyHandler::instance()->CheckAccess(
        app->mobile_app_id()->asString(), function_id, seat, zone);
  }
  return kNone;
}

void CoreService::SetAccess(const ApplicationId& app_id,
                            const std::string& group_name,
                            const SeatLocation& zone,
                            bool allowed) {
  ApplicationSharedPtr app = GetApplication(app_id);
  if (app) {
    policy::PolicyHandler::instance()->SetAccess(
        app->mobile_app_id()->asString(), group_name, zone, allowed);
  }
}

void CoreService::ResetAccess(const ApplicationId& app_id) {
  ApplicationSharedPtr app = GetApplication(app_id);
  if (app) {
    policy::PolicyHandler::instance()->ResetAccess(
        app->mobile_app_id()->asString());
  }
}

void CoreService::ResetAccess(const std::string& group_name,
                              const SeatLocation& zone) {
  policy::PolicyHandler::instance()->ResetAccess(group_name, zone);
}

void CoreService::SetPrimaryDevice(const std::string& dev_id) {
  policy::PolicyHandler::instance()->SetPrimaryDevice(dev_id);
}

void CoreService::SetRemoteControl(bool enabled) {
  policy::PolicyHandler::instance()->SetRemoteControl(enabled);
}

ApplicationSharedPtr CoreService::GetApplication(ApplicationId app_id) {
  return ApplicationManagerImpl::instance()->application(app_id);
}

void CoreService::SendMessageToHMI(const MessagePtr& message) {
  ApplicationManagerImpl::instance()->PostMessageToHMIQueque(message);
}

void CoreService::SendMessageToMobile(const MessagePtr& message) {
  ApplicationManagerImpl::instance()->PostMessageToMobileQueque(message);
}

uint32_t CoreService::GetNextCorrelationID() {
  return ApplicationManagerImpl::instance()->GetNextHMICorrelationID();
}

std::vector<ApplicationSharedPtr> CoreService::GetApplications(AppExtensionUID uid) {
  ApplicationManagerImpl::ApplicationListAccessor accessor;
  AppExtensionPredicate predicate;
  predicate.uid = uid;
  return accessor.FindAll(predicate);
}

void CoreService::SubscribeToHMINotification(
    const std::string& hmi_notification) {
  if (!hmi_notification.empty()) {
    ApplicationManagerImpl::instance()->SubscribeToHMINotification(
        hmi_notification);
  }
}

void CoreService::FilterParameters(MessagePtr msg,
                                   const CommandParametersPermissions& params) {
  // TODO(KKolodiy): may be need to implement filter parameters
  // see application_manager::command::CommandRequestImpl::RemoveDisallowedParameters
}

bool CoreService::AreParametersAllowed(
    MessagePtr msg, const CommandParametersPermissions& params) {
  Json::Reader reader;
  Json::Value json;
  bool ret = reader.parse(msg->json_message(), json);
  if (ret) {
    return CheckParams(json.get(strings::params, Json::Value(Json::nullValue)),
                       params.allowed_params);
  }
  return false;
}

bool CoreService::CheckParams(const Json::Value& object,
                              const std::vector<std::string>& allowed_params) {
  if (!object.isObject()) {
    return true;
  }
  for (Json::Value::iterator i = object.begin(); i != object.end(); ++i) {
    std::string name = i.memberName();
    const Json::Value& value = *i;
    bool allowed = IsAllowed(name, allowed_params)
        && CheckParams(value, allowed_params);
    if (!allowed) {
      return false;
    }
  }
  return true;
}

bool CoreService::IsAllowed(const std::string& name,
                            const std::vector<std::string>& allowed_params) {
  return std::find(allowed_params.begin(), allowed_params.end(), name)
      != allowed_params.end();
}

}  // namespace application_manager

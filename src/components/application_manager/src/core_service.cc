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

#include "application_manager/core_service.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/policies/policy_handler.h"

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
  const mobile_apis::Result::eType check_result =
      ApplicationManagerImpl::instance()->CheckPolicyPermissions(
          app->mobile_app_id()->asString(), app->hmi_level(),
          msg->function_name(), &params);

  if (check_result == mobile_apis::Result::eType::SUCCESS) {
    FilterParameters(msg, params);
  }

  return check_result;
}

TypeAccess CoreService::CheckAccess(
    ApplicationId app_id, const PluginFunctionID& function_id,
    const std::string& seat) {
  ApplicationSharedPtr app = GetApplication(app_id);
  if (app) {
    return policy::PolicyHandler::instance()->CheckAccess(
        app->mobile_app_id()->asString(), function_id, seat);
  }
  return kNone;
}

void CoreService::SetAccess(ApplicationId app_id,
                            const PluginFunctionID& function_id) {
  ApplicationSharedPtr app = GetApplication(app_id);
  if (app) {
    policy::PolicyHandler::instance()->SetAccess(
        app->mobile_app_id()->asString(), function_id);
  }
}

void CoreService::RemoveAccess(const PluginFunctionID& function_id) {
  policy::PolicyHandler::instance()->RemoveAccess(function_id);
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
  // TODO(KKolodiy): need to implement filter parameters
  // see application_manager::command::CommandRequestImpl::RemoveDisallowedParameters
}

}  // namespace application_manager

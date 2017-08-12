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
#include "application_manager/message_helper.h"
#include "json/json.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace {
struct AppExtensionPredicate {
  AppExtensionUID uid;
  bool operator()(const ApplicationSharedPtr app) {
    return app ? app->QueryInterface(uid).valid() : false;
  }
};
}

CoreService::CoreService(ApplicationManager& application_manager)
    : application_manager_(application_manager) {}

CoreService::~CoreService() {}

mobile_apis::Result::eType CoreService::CheckPolicyPermissions(MessagePtr msg) {
  ApplicationSharedPtr app = GetApplication(msg->connection_key());
  if (!app) {
    return mobile_apis::Result::eType::APPLICATION_NOT_REGISTERED;
  }

#ifdef SDL_REMOTE_CONTROL
  const RPCParams rpc_params;
  CommandParametersPermissions params;
  const mobile_apis::Result::eType ret =
      application_manager_.CheckPolicyPermissions(
          app, msg->function_name(), rpc_params, &params);

  if (ret != mobile_apis::Result::eType::SUCCESS) {
    return ret;
  }

  if (!AreParametersAllowed(msg, params)) {
    return mobile_apis::Result::eType::DISALLOWED;
  }

  return ret;
#else
  return mobile_apis::Result::eType::SUCCESS;
#endif  // SDL_REMOTE_CONTROL
}

TypeAccess CoreService::CheckAccess(const ApplicationId& app_id,
                                    const std::string& module,
                                    const std::string& rpc,
                                    const std::vector<std::string>& params) {
#ifdef SDL_REMOTE_CONTROL
  ApplicationSharedPtr app = GetApplication(app_id);
  if (app) {
    std::string device_handle = MessageHelper::GetDeviceMacAddressForHandle(
        app->device(), application_manager_);
    return application_manager_.GetPolicyHandler().CheckAccess(
        device_handle, app->policy_app_id(), module, rpc, params);
  }
#endif  // SDL_REMOTE_CONTROL
  return kNone;
}

bool CoreService::CheckModule(const ApplicationId& app_id,
                              const std::string& module) {
#ifdef SDL_REMOTE_CONTROL
  ApplicationSharedPtr app = GetApplication(app_id);
  if (app) {
    return application_manager_.GetPolicyHandler().CheckModule(
        app->policy_app_id(), module);
  }
#endif  // SDL_REMOTE_CONTROL
  return false;
}

void CoreService::SetAccess(const ApplicationId& app_id,
                            const std::string& module,
                            bool allowed) {
#ifdef SDL_REMOTE_CONTROL
  ApplicationSharedPtr app = GetApplication(app_id);
  if (app) {
    std::string device_handle = MessageHelper::GetDeviceMacAddressForHandle(
        app->device(), application_manager_);
    application_manager_.GetPolicyHandler().SetAccess(
        device_handle, app->policy_app_id(), module, allowed);
  }
#endif  // SDL_REMOTE_CONTROL
}

void CoreService::ResetAccess(const ApplicationId& app_id) {
#ifdef SDL_REMOTE_CONTROL
  ApplicationSharedPtr app = GetApplication(app_id);
  if (app) {
    std::string device_handle = MessageHelper::GetDeviceMacAddressForHandle(
        app->device(), application_manager_);
    application_manager_.GetPolicyHandler().ResetAccess(device_handle,
                                                        app->policy_app_id());
  }
#endif  // SDL_REMOTE_CONTROL
}

void CoreService::ResetAccess(const std::string& module) {
#ifdef SDL_REMOTE_CONTROL
  application_manager_.GetPolicyHandler().ResetAccess(module);
#endif  // SDL_REMOTE_CONTROL
}

uint32_t CoreService::GetDeviceHandlerById(const std::string& device_id) {
  uint32_t device_handle = 0;
  application_manager_.connection_handler().GetDeviceID(device_id,
                                                        &device_handle);
  return device_handle;
}

void CoreService::SetPrimaryDevice(const uint32_t dev_id) {
#ifdef SDL_REMOTE_CONTROL
  std::string device_handle =
      MessageHelper::GetDeviceMacAddressForHandle(dev_id, application_manager_);
  application_manager_.GetPolicyHandler().SetPrimaryDevice(device_handle);
#endif  // SDL_REMOTE_CONTROL
}

void CoreService::ResetPrimaryDevice() {
#ifdef SDL_REMOTE_CONTROL
  application_manager_.GetPolicyHandler().ResetPrimaryDevice();
#endif  // SDL_REMOTE_CONTROL
}

uint32_t CoreService::PrimaryDevice() const {
#ifdef SDL_REMOTE_CONTROL
  return application_manager_.GetPolicyHandler().PrimaryDevice();
#endif  // SDL_REMOTE_CONTROL
  return 0;
}

void CoreService::SetRemoteControl(bool enabled) {
#ifdef SDL_REMOTE_CONTROL
  application_manager_.GetPolicyHandler().SetRemoteControl(enabled);
#endif  // SDL_REMOTE_CONTROL
}

bool CoreService::IsRemoteControlAllowed() const {
#ifdef SDL_REMOTE_CONTROL
  return application_manager_.GetPolicyHandler().GetRemoteControl();
#endif  // SDL_REMOTE_CONTROL
  return false;
}

bool CoreService::IsRemoteControlApplication(ApplicationSharedPtr app) const {
#ifdef SDL_REMOTE_CONTROL
  return application_manager_.GetPolicyHandler().CheckHMIType(
      app->policy_app_id(),
      mobile_apis::AppHMIType::eType::REMOTE_CONTROL,
      app->app_types());
#endif  // SDL_REMOTE_CONTROL
  return false;
}

bool CoreService::IsInterfaceAvailable(
    const HmiInterfaces::InterfaceID interface) const {
#ifdef SDL_REMOTE_CONTROL
  HmiInterfaces& hmi_interfaces = application_manager_.hmi_interfaces();
  const HmiInterfaces::InterfaceState state =
      hmi_interfaces.GetInterfaceState(interface);
  return HmiInterfaces::STATE_NOT_AVAILABLE != state;
#endif  // SDL_REMOTE_CONTROL
  return false;
}

void CoreService::RemoveHMIFakeParameters(
    application_manager::MessagePtr& message) {
  application_manager_.RemoveHMIFakeParameters(message);
}

ApplicationSharedPtr CoreService::GetApplication(ApplicationId app_id) {
  return application_manager_.application(app_id);
}

void CoreService::SendMessageToHMI(const MessagePtr& message) {
  application_manager_.SendPostMessageToHMI(message);
}

void CoreService::SendMessageToMobile(const MessagePtr& message) {
  application_manager_.SendPostMessageToMobile(message);
}

uint32_t CoreService::GetNextCorrelationID() {
  return application_manager_.GetNextHMICorrelationID();
}

std::vector<ApplicationSharedPtr> CoreService::GetApplications(
    AppExtensionUID uid) {
  ApplicationSet accessor = application_manager_.applications().GetData();
  AppExtensionPredicate predicate;
  predicate.uid = uid;

  std::vector<ApplicationSharedPtr> result;
  ApplicationSetConstIt it =
      std::find_if(accessor.begin(), accessor.end(), predicate);
  while (it != accessor.end()) {
    result.push_back(*it);
    it = std::find_if(++it, accessor.end(), predicate);
  }
  return result;
}

void CoreService::SubscribeToHMINotification(
    const std::string& hmi_notification) {
  if (!hmi_notification.empty()) {
    application_manager_.SubscribeToHMINotification(hmi_notification);
  }
}

void CoreService::ChangeNotifyHMILevel(ApplicationSharedPtr app,
                                       mobile_apis::HMILevel::eType level) {
  application_manager_.ChangeAppsHMILevel(app->app_id(), level);
  MessageHelper::SendHMIStatusNotification(*app, application_manager_);
}

const smart_objects::SmartObject* CoreService::GetRCCapabilities() const {
  return application_manager_.hmi_capabilities().rc_capability();
}

void CoreService::NotifyHMIAboutHMILevel(ApplicationSharedPtr app,
                                         mobile_apis::HMILevel::eType level) {
  if (app->hmi_level() != mobile_apis::HMILevel::eType::HMI_FULL) {
    MessageHelper::SendActivateAppToHMI(
        app->app_id(),
        application_manager_,
        static_cast<hmi_apis::Common_HMILevel::eType>(level),
        true);
  }
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
                              const RPCParams& allowed_params) {
  if (!object.isObject()) {
    return true;
  }
  for (Json::Value::iterator i = object.begin(); i != object.end(); ++i) {
    std::string name = i.memberName();
    if (!IsAllowed(name, allowed_params)) {
      return false;
    }
  }
  return true;
}

bool CoreService::IsAllowed(const std::string& name,
                            const RPCParams& allowed_params) {
  return std::find(allowed_params.begin(), allowed_params.end(), name) !=
         allowed_params.end();
}

bool CoreService::GetModuleTypes(const std::string& policy_app_id,
                                 std::vector<std::string>* modules) const {
#ifdef SDL_REMOTE_CONTROL
  return application_manager_.GetPolicyHandler().GetModuleTypes(policy_app_id,
                                                                modules);
#endif  // SDL_REMOTE_CONTROL
  return false;
}

MessageValidationResult CoreService::ValidateMessageBySchema(
    const Message& message) {
  const MessageValidationResult result =
      application_manager_.ValidateMessageBySchema(message);
  LOG4CXX_DEBUG(logger_, "Validation result : " << result);
  return result;
}

}  // namespace application_manager

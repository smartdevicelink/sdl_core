/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the copyright holders nor the names of their contributors
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

#include "rc_rpc_plugin/resource_allocation_manager_impl.h"
#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "smart_objects/enum_schema_item.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "application_manager/message_helper.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "json/json.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"

namespace rc_rpc_plugin {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

ResourceAllocationManagerImpl::ResourceAllocationManagerImpl(
    application_manager::ApplicationManager& app_mngr,
    application_manager::rpc_service::RPCService& rpc_service)
    : current_access_mode_(hmi_apis::Common_RCAccessMode::AUTO_ALLOW)
    , app_mngr_(app_mngr)
    , rpc_service_(rpc_service)
    , is_rc_enabled_(true) {}

ResourceAllocationManagerImpl::~ResourceAllocationManagerImpl() {}

AcquireResult::eType ResourceAllocationManagerImpl::AcquireResource(
    const std::string& module_type, const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  const application_manager::ApplicationSharedPtr acquiring_app =
      app_mngr_.application(app_id);
  if (!acquiring_app) {
    LOG4CXX_WARN(logger_, "App with app_id: " << app_id << "does not exist!");
    return AcquireResult::IN_USE;
  }

  sync_primitives::AutoLock lock(allocated_resources_lock_);
  const AllocatedResources::const_iterator allocated_it =
      allocated_resources_.find(module_type);
  if (allocated_resources_.end() == allocated_it) {
    SetResourceAquired(module_type, app_id);
    LOG4CXX_DEBUG(logger_,
                  "Resource is not acquired yet. "
                      << "App: " << app_id << " is allowed to acquire "
                      << module_type);
    return AcquireResult::ALLOWED;
  }

  if (app_id == allocated_resources_[module_type]) {
    LOG4CXX_DEBUG(logger_,
                  "App: " << app_id << " is already acquired resource "
                          << module_type);
    return AcquireResult::ALLOWED;
  }

  if (IsModuleTypeRejected(module_type, app_id)) {
    LOG4CXX_DEBUG(logger_,
                  "Driver disallowed app: " << app_id << " to acquire "
                                            << module_type);
    return AcquireResult::REJECTED;
  }

  const mobile_apis::HMILevel::eType acquiring_app_hmi_level =
      acquiring_app->hmi_level();

  if (mobile_apis::HMILevel::HMI_FULL != acquiring_app_hmi_level) {
    LOG4CXX_DEBUG(logger_,
                  "Aquiring resources is not allowed in HMI level: "
                      << acquiring_app_hmi_level << ". App: " << app_id
                      << " is disallowed to acquire " << module_type);
    return AcquireResult::REJECTED;
  }

  switch (current_access_mode_) {
    case hmi_apis::Common_RCAccessMode::AUTO_DENY: {
      LOG4CXX_DEBUG(logger_,
                    "Current access_mode is AUTO_DENY. "
                        << "App: " << app_id << " is disallowed to acquire "
                        << module_type);
      return AcquireResult::IN_USE;
    }
    case hmi_apis::Common_RCAccessMode::ASK_DRIVER: {
      LOG4CXX_DEBUG(logger_,
                    "Current access_mode is ASK_DRIVER. "
                    "Driver confirmation is required for app: "
                        << app_id << " to acquire " << module_type);
      return AcquireResult::ASK_DRIVER;
    }
    case hmi_apis::Common_RCAccessMode::AUTO_ALLOW: {
      LOG4CXX_DEBUG(logger_,
                    "Current access_mode is AUTO_ALLOW. "
                        << "App: " << app_id << " is allowed to acquire "
                        << module_type);

      SetResourceAquired(module_type, app_id);
      return AcquireResult::ALLOWED;
    }
    default: { DCHECK_OR_RETURN(false, AcquireResult::IN_USE); }
  }
}

void ResourceAllocationManagerImpl::ReleaseResource(
    const std::string& module_type, const uint32_t application_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Release " << module_type << " by " << application_id);
  SetResourceFree(module_type, application_id);
}

void ResourceAllocationManagerImpl::ProcessApplicationPolicyUpdate() {
  LOG4CXX_AUTO_TRACE(logger_);
  Apps app_list = RCRPCPlugin::GetRCApplications(app_mngr_);

  Apps::const_iterator app = app_list.begin();
  for (; app_list.end() != app; ++app) {
    application_manager::ApplicationSharedPtr app_ptr = *app;
    const uint32_t application_id = app_ptr->app_id();
    Resources acquired_modules = GetAcquiredResources(application_id);
    std::sort(acquired_modules.begin(), acquired_modules.end());

    Resources allowed_modules;
    app_mngr_.GetPolicyHandler().GetModuleTypes((*app)->policy_app_id(),
                                                &allowed_modules);
    std::sort(allowed_modules.begin(), allowed_modules.end());

    LOG4CXX_DEBUG(logger_,
                  "Acquired modules: " << acquired_modules.size()
                                       << " , allowed modules: "
                                       << allowed_modules.size());

    Resources disallowed_modules;
    std::set_difference(acquired_modules.begin(),
                        acquired_modules.end(),
                        allowed_modules.begin(),
                        allowed_modules.end(),
                        std::back_inserter(disallowed_modules));

    RCAppExtensionPtr rc_extention = GetApplicationExtention(app_ptr);
    Resources::const_iterator module = disallowed_modules.begin();
    for (; disallowed_modules.end() != module; ++module) {
      ReleaseResource(*module, application_id);
      if (rc_extention) {
        rc_extention->UnsubscribeFromInteriorVehicleData(*module);
      }
      SendOnRCStatusNotifications(NotificationTrigger::MODULE_ALLOCATION);
    }
  }
}

RCAppExtensionPtr ResourceAllocationManagerImpl::GetApplicationExtention(
    application_manager::ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  RCAppExtensionPtr rc_app_extension;
  if (!application) {
    return rc_app_extension;
  }

  application_manager::AppExtensionPtr app_extension =
      application->QueryInterface(RCRPCPlugin::kRCPluginID);
  if (!app_extension) {
    return rc_app_extension;
  }

  rc_app_extension =
      application_manager::AppExtensionPtr::static_pointer_cast<RCAppExtension>(
          app_extension);

  return rc_app_extension;
}

void ResourceAllocationManagerImpl::RemoveAppsSubscriptions(const Apps& apps) {
  LOG4CXX_AUTO_TRACE(logger_);
  Apps::const_iterator app = apps.begin();
  for (; apps.end() != app; ++app) {
    application_manager::ApplicationSharedPtr app_ptr = *app;
    if (!app_ptr) {
      continue;
    }
    RCAppExtensionPtr rc_extention = GetApplicationExtention(app_ptr);
    if (rc_extention) {
      rc_extention->UnsubscribeFromInteriorVehicleData();
    }
  }
}

template <typename EnumType>
EnumType StringToEnum(const std::string& str) {
  using smart_objects::EnumConversionHelper;
  EnumType val;
  EnumConversionHelper<EnumType>::StringToEnum(str, &val);
  return val;
}

void ConstructOnRCStatusNotificationParams(
    smart_objects::SmartObject& msg_params,
    const std::map<std::string, uint32_t>& allocated_resources,
    const std::vector<std::string>& supported_resources,
    const uint32_t app_id,
    const bool is_rc_enabled) {
  namespace strings = application_manager::strings;
  namespace message_params = rc_rpc_plugin::message_params;
  using smart_objects::SmartObject;
  using smart_objects::SmartType_Map;
  using smart_objects::SmartType_Array;
  LOG4CXX_AUTO_TRACE(logger_);
  SmartObject allocated_modules = SmartObject(SmartType_Array);
  SmartObject free_modules = SmartObject(SmartType_Array);
  if (is_rc_enabled) {
    auto modules_inserter = [](SmartObject& result_modules) {
      return [&result_modules](const std::string& module_name) {
        smart_objects::SmartObject module_data =
            SmartObject(smart_objects::SmartType_Map);
        auto module_type =
            StringToEnum<mobile_apis::ModuleType::eType>(module_name);
        module_data[message_params::kModuleType] = module_type;
        result_modules.asArray()->push_back(module_data);
      };
    };
    for (const auto& module : allocated_resources) {
      if (module.second == app_id) {
        modules_inserter(allocated_modules)(module.first);
      }
    }
    for (auto& module : supported_resources) {
      if (allocated_resources.find(module) == allocated_resources.end()) {
        modules_inserter(free_modules)(module);
      }
    }
  }
  msg_params[message_params::kAllocatedModules] = allocated_modules;
  msg_params[message_params::kFreeModules] = free_modules;
}

smart_objects::SmartObjectSPtr
ResourceAllocationManagerImpl::CreateOnRCStatusNotificationToMobile(
    const application_manager::ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using application_manager::MessageHelper;
  auto msg_to_mobile = MessageHelper::CreateNotification(
      mobile_apis::FunctionID::OnRCStatusID, app->app_id());
  auto& msg_params = (*msg_to_mobile)[application_manager::strings::msg_params];
  ConstructOnRCStatusNotificationParams(msg_params,
                                        allocated_resources_,
                                        all_supported_modules(),
                                        app->app_id(),
                                        is_rc_enabled());
  return msg_to_mobile;
}

smart_objects::SmartObjectSPtr
ResourceAllocationManagerImpl::CreateOnRCStatusNotificationToHmi(
    const application_manager::ApplicationSharedPtr app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using application_manager::MessageHelper;
  auto msg_to_hmi =
      MessageHelper::CreateHMINotification(hmi_apis::FunctionID::RC_OnRCStatus);
  auto& msg_params = (*msg_to_hmi)[application_manager::strings::msg_params];
  ConstructOnRCStatusNotificationParams(msg_params,
                                        allocated_resources_,
                                        all_supported_modules(),
                                        app->app_id(),
                                        is_rc_enabled());
  msg_params[application_manager::strings::app_id] = app->hmi_app_id();
  return msg_to_hmi;
}

void ResourceAllocationManagerImpl::SetResourceAquired(
    const std::string& module_type, const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  allocated_resources_[module_type] = app_id;
  SendOnRCStatusNotifications(NotificationTrigger::MODULE_ALLOCATION);
}

void ResourceAllocationManagerImpl::SendOnRCStatusNotifications(
    NotificationTrigger::eType event) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto rc_apps = RCRPCPlugin::GetRCApplications(app_mngr_);
  for (const auto& rc_app : rc_apps) {
    smart_objects::SmartObjectSPtr msg_to_mobile;
    smart_objects::SmartObjectSPtr msg_to_hmi;
    switch (event) {
      case NotificationTrigger::APP_REGISTRATION:
        msg_to_mobile = CreateOnRCStatusNotificationToMobile(rc_app);
        (*msg_to_mobile)[application_manager::strings::msg_params]
                        [message_params::kAllowed] = is_rc_enabled();
        rpc_service_.SendMessageToMobile(msg_to_mobile);
        if (is_rc_enabled()) {
          msg_to_hmi = CreateOnRCStatusNotificationToHmi(rc_app);
          rpc_service_.SendMessageToHMI(msg_to_hmi);
        }
        break;
      case NotificationTrigger::MODULE_ALLOCATION:
        msg_to_mobile = CreateOnRCStatusNotificationToMobile(rc_app);
        rpc_service_.SendMessageToMobile(msg_to_mobile);
        msg_to_hmi = CreateOnRCStatusNotificationToHmi(rc_app);
        rpc_service_.SendMessageToHMI(msg_to_hmi);
        break;
      case NotificationTrigger::RC_STATE_CHANGING:
        msg_to_mobile = CreateOnRCStatusNotificationToMobile(rc_app);
        (*msg_to_mobile)[application_manager::strings::msg_params]
                        [message_params::kAllowed] = is_rc_enabled();
        rpc_service_.SendMessageToMobile(msg_to_mobile);
        break;
    }
  }
}

bool ResourceAllocationManagerImpl::is_rc_enabled() const {
  return is_rc_enabled_;
}

void ResourceAllocationManagerImpl::set_rc_enabled(const bool value) {
  is_rc_enabled_ = value;
  SendOnRCStatusNotifications(NotificationTrigger::RC_STATE_CHANGING);
}

void ResourceAllocationManagerImpl::SetResourceFree(
    const std::string& module_type, const uint32_t app_id) {
  AllocatedResources::const_iterator allocation =
      allocated_resources_.find(module_type);
  if (allocated_resources_.end() == allocation) {
    LOG4CXX_DEBUG(logger_, "Resource " << module_type << " is not allocated.");
    return;
  }
  if (app_id != allocation->second) {
    LOG4CXX_ERROR(logger_,
                  "Resource " << module_type
                              << " is allocated by different application "
                              << allocation->second);
  }
  allocated_resources_.erase(allocation);
  LOG4CXX_DEBUG(logger_, "Resource " << module_type << " is released.");
}

std::vector<std::string>
ResourceAllocationManagerImpl::all_supported_modules() {
  std::vector<std::string> result;
  result.push_back(enums_value::kClimate);
  result.push_back(enums_value::kRadio);
  return result;
}

std::vector<std::string> ResourceAllocationManagerImpl::GetAcquiredResources(
    const uint32_t application_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  Resources allocated_resources;
  AllocatedResources::const_iterator allocation = allocated_resources_.begin();
  for (; allocated_resources_.end() != allocation; ++allocation) {
    if (application_id == allocation->second) {
      allocated_resources.push_back(allocation->first);
    }
  }

  LOG4CXX_DEBUG(logger_,
                "Application " << application_id << " acquired "
                               << allocated_resources.size()
                               << " resource(s).");

  return allocated_resources;
}

void ResourceAllocationManagerImpl::SetResourceState(
    const std::string& module_type,
    const uint32_t app_id,
    const ResourceState::eType state) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "Setting state for " << module_type << " by app_id " << app_id
                                     << " to state " << state);
  {
    sync_primitives::AutoLock lock(allocated_resources_lock_);
    const AllocatedResources::const_iterator allocated_it =
        allocated_resources_.find(module_type);

    const bool acquired = allocated_resources_.end() != allocated_it;
    if (acquired) {
      LOG4CXX_DEBUG(logger_,
                    "Resource " << module_type << " is already acquired."
                                << " Owner application id is "
                                << allocated_it->second
                                << " Changing application id is " << app_id);
    } else {
      LOG4CXX_DEBUG(logger_,
                    "Resource " << module_type << " is not acquired yet");
    }
  }

  sync_primitives::AutoLock lock(resources_state_lock_);
  resources_state_[module_type] = state;
  LOG4CXX_DEBUG(logger_, "Resource " << module_type << " got state " << state);
}

bool ResourceAllocationManagerImpl::IsResourceFree(
    const std::string& module_type) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(resources_state_lock_);
  const ResourcesState::const_iterator resource =
      resources_state_.find(module_type);

  if (resources_state_.end() == resource) {
    LOG4CXX_DEBUG(logger_, "Resource " << module_type << " is free.");
    return true;
  }

  LOG4CXX_DEBUG(logger_,
                "Resource " << module_type << " state is " << resource->second);

  return ResourceState::FREE == resource->second;
}

void ResourceAllocationManagerImpl::SetAccessMode(
    const hmi_apis::Common_RCAccessMode::eType access_mode) {
  if (hmi_apis::Common_RCAccessMode::ASK_DRIVER != access_mode) {
    sync_primitives::AutoLock lock(rejected_resources_for_application_lock_);
    rejected_resources_for_application_.clear();
  }
  current_access_mode_ = access_mode;
}

hmi_apis::Common_RCAccessMode::eType
ResourceAllocationManagerImpl::GetAccessMode() const {
  return current_access_mode_;
}

void ResourceAllocationManagerImpl::ForceAcquireResource(
    const std::string& module_type, const uint32_t app_id) {
  LOG4CXX_DEBUG(logger_, "Force " << app_id << " acquiring " << module_type);
  sync_primitives::AutoLock lock(allocated_resources_lock_);
  SetResourceAquired(module_type, app_id);
}

bool ResourceAllocationManagerImpl::IsModuleTypeRejected(
    const std::string& module_type, const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(rejected_resources_for_application_lock_);
  RejectedResources::iterator it =
      rejected_resources_for_application_.find(app_id);

  if (rejected_resources_for_application_.end() == it) {
    return false;
  }

  const std::vector<std::string>& list_of_rejected_resources =
      rejected_resources_for_application_[app_id];

  return helpers::in_range(list_of_rejected_resources, module_type);
}

void ResourceAllocationManagerImpl::OnDriverDisallowed(
    const std::string& module_type, const uint32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(rejected_resources_for_application_lock_);
  auto it = rejected_resources_for_application_.find(app_id);

  if (rejected_resources_for_application_.end() == it) {
    rejected_resources_for_application_[app_id] = std::vector<std::string>();
  }
  std::vector<std::string>& list_of_rejected_resources =
      rejected_resources_for_application_[app_id];
  list_of_rejected_resources.push_back(module_type);
}

void ResourceAllocationManagerImpl::OnApplicationEvent(
    application_manager::plugin_manager::ApplicationEvent event,
    application_manager::ApplicationSharedPtr application) {
  using application_manager::plugin_manager::ApplicationEvent;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "Event " << event << " came for " << application->app_id());

  if (ApplicationEvent::kApplicationExit == event ||
      ApplicationEvent::kApplicationUnregistered == event) {
    Resources acquired_modules = GetAcquiredResources(application->app_id());
    Resources::const_iterator module = acquired_modules.begin();
    for (; acquired_modules.end() != module; ++module) {
      ReleaseResource(*module, application->app_id());
    }
    if (!acquired_modules.empty()) {
      SendOnRCStatusNotifications(NotificationTrigger::MODULE_ALLOCATION);
    }
    Apps app_list;
    app_list.push_back(application);
    RemoveAppsSubscriptions(app_list);
  }
}

void ResourceAllocationManagerImpl::OnPolicyEvent(
    application_manager::plugin_manager::PolicyEvent event) {
  using application_manager::plugin_manager::PolicyEvent;
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Event " << event);

  if (PolicyEvent::kApplicationPolicyUpdated == event) {
    ProcessApplicationPolicyUpdate();
    return;
  }

  if (PolicyEvent::kApplicationsDisabled == event) {
    ResetAllAllocations();
    Apps app_list = RCRPCPlugin::GetRCApplications(app_mngr_);
    RemoveAppsSubscriptions(app_list);
    return;
  }
}

void ResourceAllocationManagerImpl::ResetAllAllocations() {
  LOG4CXX_AUTO_TRACE(logger_);
  {
    sync_primitives::AutoLock lock(resources_state_lock_);
    resources_state_.clear();
  }
  {
    sync_primitives::AutoLock lock(allocated_resources_lock_);
    allocated_resources_.clear();
  }
  {
    sync_primitives::AutoLock lock(rejected_resources_for_application_lock_);
    rejected_resources_for_application_.clear();
  }
}

}  // namespace rc_rpc_plugin

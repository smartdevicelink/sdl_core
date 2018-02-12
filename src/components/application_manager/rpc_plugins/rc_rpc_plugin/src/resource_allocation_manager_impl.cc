#include "rc_rpc_plugin/resource_allocation_manager_impl.h"
#include "application_manager/application.h"
#include "application_manager/application_manager.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"

#include "application_manager/message_helper.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "json/json.h"
#include "utils/helpers.h"
#include "utils/make_shared.h"

//#include "remote_control/message_helper.h"
//#include "remote_control/rc_app_extension.h"

namespace rc_rpc_plugin {

CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule")

ResourceAllocationManagerImpl::ResourceAllocationManagerImpl(
    application_manager::ApplicationManager& app_mngr)
    : current_access_mode_(hmi_apis::Common_RCAccessMode::AUTO_ALLOW)
    , app_mngr_(app_mngr) {}

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
    allocated_resources_[module_type] = app_id;
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
    LOG4CXX_DEBUG(
        logger_,
        "Aquiring resources is not allowed in HMI level: "
            << application_manager::MessageHelper::StringifiedHMILevel(
                   acquiring_app_hmi_level) << ". App: " << app_id
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

      allocated_resources_[module_type] = app_id;
      return AcquireResult::ALLOWED;
    }
    default: { DCHECK_OR_RETURN(false, AcquireResult::IN_USE); }
  }
}

void ResourceAllocationManagerImpl::ReleaseResource(
    const std::string& module_type, const uint32_t application_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Release " << module_type << " by " << application_id);
  AllocatedResources::const_iterator allocation =
      allocated_resources_.find(module_type);
  if (allocated_resources_.end() == allocation) {
    LOG4CXX_DEBUG(logger_, "Resource " << module_type << " is not allocated.");
    return;
  }

  if (application_id != allocation->second) {
    LOG4CXX_DEBUG(logger_,
                  "Resource " << module_type
                              << " is allocated by different application "
                              << allocation->second);
    return;
  }

  allocated_resources_.erase(allocation);
  LOG4CXX_DEBUG(logger_, "Resource " << module_type << " is released.");
  return;
}

void ResourceAllocationManagerImpl::ProcessApplicationPolicyUpdate() {
  typedef std::vector<application_manager::ApplicationSharedPtr> Apps;
  Apps app_list = GetRCApplications();

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
        rc_extention->UnsubscribeFromInteriorVehicleData(Json::Value(*module));
      }
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
  allocated_resources_[module_type] = app_id;
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
    Apps app_list = GetRCApplications();
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

ResourceAllocationManagerImpl::Apps
ResourceAllocationManagerImpl::GetRCApplications() {
  LOG4CXX_AUTO_TRACE(logger_);
  using application_manager::ApplicationSharedPtr;
  using application_manager::ApplicationSet;
  ApplicationSet accessor = app_mngr_.applications().GetData();

  auto predicate = [](const ApplicationSharedPtr& app) {
    auto uid = RCRPCPlugin::kRCPluginID;
    return app ? app->QueryInterface(uid).valid() : false;
  };

  auto it = std::find_if(accessor.begin(), accessor.end(), predicate);

  std::vector<ApplicationSharedPtr> result;
  while (it != accessor.end()) {
    result.push_back(*it);
    it = std::find_if(++it, accessor.end(), predicate);
  }
  return result;
}

}  // namespace rc_rpc_plugin

#include "rc_rpc_plugin/interior_data_manager_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"

namespace rc_rpc_plugin {
CREATE_LOGGERPTR_GLOBAL(logger_, "RemoteControlModule");

InteriorDataManagerImpl::InteriorDataManagerImpl(
    RCRPCPlugin& rc_plugin,
    InteriorDataCache& cache,
    application_manager::ApplicationManager& app_mngr,
    application_manager::rpc_service::RPCService& rpc_service)
    : rc_plugin_(rc_plugin)
    , cache_(cache)
    , app_mngr_(app_mngr)
    , rpc_service_(rpc_service) {}

void InteriorDataManagerImpl::OnPolicyEvent(plugins::PolicyEvent event) {
  UpdateHMISubscriptionsOnPolicyUpdated();
}

void InteriorDataManagerImpl::OnApplicationEvent(
    plugins::ApplicationEvent event,
    app_mngr::ApplicationSharedPtr application) {
  if (plugins::ApplicationEvent::kApplicationUnregistered == event ||
      plugins::ApplicationEvent::kApplicationExit == event) {
    UpdateHMISubscriptionsOnAppUnregistered(*application);
  }
}

void InteriorDataManagerImpl::OnDisablingRC() {
  LOG4CXX_AUTO_TRACE(logger_);
  auto existing_subscription = AppsSubscribedModules();
  std::set<ModuleUid> subscribed_modules;
  for (auto& pair : existing_subscription) {
    auto& app = pair.first;
    auto rc_extension = RCHelpers::GetRCExtension(*app);
    for (const auto& module : pair.second) {
      subscribed_modules.insert(module);
      rc_extension->UnsubscribeFromInteriorVehicleData(module);
    }
  }
  for (auto& module : subscribed_modules) {
    LOG4CXX_TRACE(logger_,
                  "unsubscribe from module type: " << module.first
                                                   << " id: " << module.second);
    UnsubscribeFromInteriorVehicleData(module);
  }
}

void InteriorDataManagerImpl::StoreRequestToHMITime(const ModuleUid& module) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(requests_to_hmi_history_lock_);
  requests_to_hmi_history_[module].push_back(date_time::getCurrentTime());
}

bool InteriorDataManagerImpl::CheckRequestsToHMIFrequency(
    const ModuleUid& module) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(requests_to_hmi_history_lock_);
  ClearOldRequestsToHMIHistory();
  const auto& history = requests_to_hmi_history_[module];
  const auto limit =
      app_mngr_.get_settings().get_interior_vehicle_data_frequency().first;
  return history.size() < limit;
}

void InteriorDataManagerImpl::UpdateHMISubscriptionsOnPolicyUpdated() {
  auto apps_allowed_module_types =
      RCHelpers::GetApplicationsAllowedModuleTypes(app_mngr_);
  auto apps_subscribed_module_types = AppsSubscribedModuleTypes();
  InteriorDataManagerImpl::AppsModuleTypes apps_disallowed_modules;
  for (auto& pair : apps_subscribed_module_types) {
    auto& allowed = apps_allowed_module_types[pair.first];
    auto& subscribed = pair.second;
    std::vector<std::string> disallowed_modules;
    std::set_difference(subscribed.begin(),
                        subscribed.end(),
                        allowed.begin(),
                        allowed.end(),
                        std::back_inserter(disallowed_modules));

    std::sort(disallowed_modules.begin(), disallowed_modules.end());

    auto unique_result =
        std::unique(disallowed_modules.begin(), disallowed_modules.end());

    disallowed_modules.erase(unique_result, disallowed_modules.end());
    apps_disallowed_modules[pair.first] = disallowed_modules;
  }

  for (auto& pair : apps_disallowed_modules) {
    auto& app = pair.first;
    auto rc_extension = RCHelpers::GetRCExtension(*app);
    for (const auto& module_type : pair.second) {
      rc_extension->UnsubscribeFromInteriorVehicleDataOfType(module_type);
      auto apps_subscribed =
          RCHelpers::AppsSubscribedToModuleType(app_mngr_, module_type);
      if (apps_subscribed.empty()) {
        UnsubscribeFromInteriorVehicleDataOfType(module_type);
      }
    }
  }
}

void InteriorDataManagerImpl::UpdateHMISubscriptionsOnAppUnregistered(
    application_manager::Application& app) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto rc_extension = RCHelpers::GetRCExtension(app);
  auto subscribed_data = rc_extension->InteriorVehicleDataSubscriptions();
  rc_extension->UnsubscribeFromInteriorVehicleData();
  for (auto& data : subscribed_data) {
    auto apps_subscribed = RCHelpers::AppsSubscribedToModule(app_mngr_, data);
    if (apps_subscribed.empty()) {
      UnsubscribeFromInteriorVehicleData(data);
    }
    if (apps_subscribed.size() == 1 &&
        apps_subscribed.front()->hmi_app_id() == app.hmi_app_id()) {
      UnsubscribeFromInteriorVehicleData(data);
    }
  }
}

void InteriorDataManagerImpl::UnsubscribeFromInteriorVehicleData(
    const ModuleUid& module) {
  cache_.Remove(module);
  auto unsubscribe_request = RCHelpers::CreateUnsubscribeRequestToHMI(
      module, app_mngr_.GetNextHMICorrelationID());
  LOG4CXX_DEBUG(logger_,
                "Send Unsubscribe from module type: " << module.first << " id: "
                                                      << module.second);
  rpc_service_.ManageHMICommand(unsubscribe_request);
}

void InteriorDataManagerImpl::UnsubscribeFromInteriorVehicleDataOfType(
    const std::string& module_type) {
  const auto& modules = cache_.GetCachedModulesByType(module_type);

  for (const auto& module : modules) {
    cache_.Remove(module);
    auto unsubscribe_request = RCHelpers::CreateUnsubscribeRequestToHMI(
        module, app_mngr_.GetNextHMICorrelationID());
    LOG4CXX_DEBUG(logger_,
                  "Send Unsubscribe from module type: "
                      << module.first << " id: " << module.second);
    rpc_service_.ManageHMICommand(unsubscribe_request);
  }
}

void InteriorDataManagerImpl::ClearOldRequestsToHMIHistory() {
  auto limit =
      app_mngr_.get_settings().get_interior_vehicle_data_frequency().second;
  uint32_t time_frame = limit * date_time::MILLISECONDS_IN_SECOND;
  auto lest_that_time_frame_ago = [time_frame](date_time::TimeDuration time) {
    auto span = date_time::calculateTimeSpan(time);
    return span < time_frame;
  };
  for (auto& it : requests_to_hmi_history_) {
    auto& history = it.second;
    auto first_actual =
        std::find_if(history.begin(), history.end(), lest_that_time_frame_ago);
    history.erase(history.begin(), first_actual);
  }
}

InteriorDataManagerImpl::AppsModules
InteriorDataManagerImpl::AppsSubscribedModules() {
  auto apps_list = RCRPCPlugin::GetRCApplications(app_mngr_);
  InteriorDataManagerImpl::AppsModules result;
  for (auto& app_ptr : apps_list) {
    const auto rc_extension = RCHelpers::GetRCExtension(*app_ptr);
    auto app_subscriptions = rc_extension->InteriorVehicleDataSubscriptions();
    result[app_ptr] = std::vector<ModuleUid>(app_subscriptions.size());
    std::copy(app_subscriptions.begin(),
              app_subscriptions.end(),
              result[app_ptr].begin());
  }
  return result;
}

InteriorDataManagerImpl::AppsModuleTypes
InteriorDataManagerImpl::AppsSubscribedModuleTypes() {
  auto apps_list = RCRPCPlugin::GetRCApplications(app_mngr_);
  RCHelpers::AppsModuleTypes result;
  for (auto& app_ptr : apps_list) {
    const auto rc_extension = RCHelpers::GetRCExtension(*app_ptr);
    auto app_subscriptions = rc_extension->InteriorVehicleDataSubscriptions();
    std::vector<std::string> app_module_types;

    for (auto& app_subscription : app_subscriptions) {
      app_module_types.push_back(app_subscription.first);
    }

    std::sort(app_module_types.begin(), app_module_types.end());
    result[app_ptr] = app_module_types;
  }
  return result;
}
}  // namespace rc_rpc_plugin

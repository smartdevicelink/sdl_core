#include "rc_rpc_plugin/interior_data_manager_impl.h"
#include "rc_rpc_plugin/rc_helpers.h"
#include "rc_rpc_plugin/rc_rpc_plugin.h"
#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"

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
  if (plugins::ApplicationEvent::kApplicationUnregistered == event) {
    UpdateHMISubscriptionsOnAppUnregistered(*application);
  }
}

void InteriorDataManagerImpl::OnDisablingRC() {
  LOG4CXX_AUTO_TRACE(logger_);
  auto existing_subscription = AppsSubscribedModules();
  std::set<std::string> subscribed_modules;
  for (auto& pair : existing_subscription) {
    auto& app = pair.first;
    auto rc_extention = RCHelpers::GetRCExtension(*app);
    for (const auto& module : pair.second) {
      subscribed_modules.insert(module);
      rc_extention->UnsubscribeFromInteriorVehicleData(module);
    }
  }
  for (auto& module : subscribed_modules) {
    LOG4CXX_TRACE(logger_, "unsubscribe " << module);
    UnsubscribeFromInteriorVehicleData(module);
  }
}

void InteriorDataManagerImpl::UpdateHMISubscriptionsOnPolicyUpdated() {
  auto apps_allowed_modules =
      RCHelpers::GetApplicaitonsAllowedModules(app_mngr_);
  auto apps_subscribed_modules = AppsSubscribedModules();
  InteriorDataManagerImpl::AppsModules apps_disallowed_modules;
  for (auto& pair : apps_subscribed_modules) {
    auto& allowed = apps_allowed_modules[pair.first];
    auto& subscribed = pair.second;
    std::vector<std::string> disallowed_modules;
    std::set_difference(subscribed.begin(),
                        subscribed.end(),
                        allowed.begin(),
                        allowed.end(),
                        std::back_inserter(disallowed_modules));
    apps_disallowed_modules[pair.first] = disallowed_modules;
  }

  for (auto& pair : apps_disallowed_modules) {
    auto& app = pair.first;
    auto rc_extention = RCHelpers::GetRCExtension(*app);
    for (const auto& module : pair.second) {
      rc_extention->UnsubscribeFromInteriorVehicleData(module);
      auto apps_subscribed =
          RCHelpers::AppsSubscribedToModuleType(module, app_mngr_);
      if (apps_subscribed.empty()) {
        UnsubscribeFromInteriorVehicleData(module);
      }
    }
  }
}

void InteriorDataManagerImpl::UpdateHMISubscriptionsOnAppUnregistered(
    application_manager::Application& app) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto rc_extension = RCHelpers::GetRCExtension(app);
  auto subscribed_data = rc_extension->InteriorVehicleDataSubscriptions();
  for (auto& data : subscribed_data) {
    auto apps_subscribed = RCHelpers::AppsSubscribedTo(app_mngr_, data);
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
    const std::string& module_type) {
  auto unsubscribe_request = RCHelpers::CreateUnsubscribeRequestToHMI(
      module_type, app_mngr_.GetNextHMICorrelationID());
  LOG4CXX_DEBUG(logger_, "Send Unsubscribe from " << module_type);
  rpc_service_.ManageHMICommand(unsubscribe_request);
}

InteriorDataManagerImpl::AppsModules
InteriorDataManagerImpl::AppsSubscribedModules() {
  auto apps_list = RCRPCPlugin::GetRCApplications(app_mngr_);
  InteriorDataManagerImpl::AppsModules result;
  for (auto& app_ptr : apps_list) {
    const auto rc_extension = RCHelpers::GetRCExtension(*app_ptr);
    auto app_subscriptions = rc_extension->InteriorVehicleDataSubscriptions();
    result[app_ptr] = std::vector<std::string>(app_subscriptions.size());
    std::copy(app_subscriptions.begin(),
              app_subscriptions.end(),
              result[app_ptr].begin());
  }
  return result;
}
}  // namespace rc_rpc_plugin

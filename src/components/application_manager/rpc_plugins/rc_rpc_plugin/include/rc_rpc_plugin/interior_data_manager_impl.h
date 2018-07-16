#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_MANAGER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_MANAGER_IMPL_H_
#include "rc_rpc_plugin/interior_data_manager.h"
namespace application_manager {
class ApplicationManager;
namespace rpc_service {
class RPCService;
}
}
namespace rc_rpc_plugin {

class InteriorDataCache;
class RCRPCPlugin;

class InteriorDataManagerImpl : public InteriorDataManager {
 public:
  InteriorDataManagerImpl(
      RCRPCPlugin& rc_plugin,
      InteriorDataCache& cache,
      application_manager::ApplicationManager& app_mngr,
      application_manager::rpc_service::RPCService& rpc_service);

  void OnPolicyEvent(app_mngr::plugin_manager::PolicyEvent event) OVERRIDE;

  void OnApplicationEvent(plugins::ApplicationEvent event,
                          app_mngr::ApplicationSharedPtr application) OVERRIDE;

  void OnDisablingRC() OVERRIDE;

 private:
  void UpdateHMISubscriptionsOnPolicyUpdated();

  void UpdateHMISubscriptionsOnAppUnregistered(
      application_manager::Application& app);

  void UnsubscribeFromInteriorVehicleData(const std::string& module_type);
  typedef std::map<application_manager::ApplicationSharedPtr,
                   std::vector<std::string> > AppsModules;
  AppsModules AppsSubscribedModules();
  RCRPCPlugin& rc_plugin_;
  InteriorDataCache& cache_;
  application_manager::ApplicationManager& app_mngr_;
  application_manager::rpc_service::RPCService& rpc_service_;
};

}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_INTERIOR_DATA_MANAGER_IMPL_H_

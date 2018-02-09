#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_RPC_PLUGIN_MANAGER_IMPL_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_RPC_PLUGIN_MANAGER_IMPL_H
#include "application_manager/plugin_manager/rpc_plugin_manager.h"
#include "application_manager/application_manager.h"
#include "application_manager/rpc_service.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "utils/optional.h"

namespace application_manager {
namespace plugin_manager {

class RPCPluginManagerImpl : public RPCPluginManager {
  // RPCPluginManager interface
 public:
  RPCPluginManagerImpl(ApplicationManager& app_manager,
                       rpc_service::RPCService& rpc_service,
                       HMICapabilities& hmi_capabilities,
                       policy::PolicyHandlerInterface& policy_handler);

  uint32_t LoadPlugins(const std::string& plugins_path) OVERRIDE;
  std::vector<RPCPluginPtr>& GetPlugins() OVERRIDE;
  utils::Optional<RPCPlugin> FindPluginToProcess(
      const int32_t function_id,
      const commands::Command::CommandSource message_source) OVERRIDE;

 private:
  std::vector<RPCPluginPtr> loaded_plugins_;
  ApplicationManager& app_manager_;
  rpc_service::RPCService& rpc_service_;
  HMICapabilities& hmi_capabilities_;
  policy::PolicyHandlerInterface& policy_handler_;

  // RPCPluginManager interface
 public:
  void ForEachPlugin(std::function<void(RPCPlugin&)> functor) OVERRIDE;
};
}  // namespace plugin_manager
}  // namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_RPC_PLUGIN_MANAGER_IMPL_H

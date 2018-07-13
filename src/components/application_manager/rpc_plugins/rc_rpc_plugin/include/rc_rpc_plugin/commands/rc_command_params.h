#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_RC_COMMAND_PARAMS_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_RC_COMMAND_PARAMS_H

namespace application_manager {
class ApplicationManager;
namespace rpc_service {
class RPCService;
}
class HMICapabilities;
}

namespace policy {
class PolicyHandlerInterface;
}

namespace rc_rpc_plugin {

class ResourceAllocationManager;
class InteriorDataCache;

struct RCCommandParams {
  application_manager::ApplicationManager& application_manager_;
  application_manager::rpc_service::RPCService& rpc_service_;
  application_manager::HMICapabilities& hmi_capabilities_;
  policy::PolicyHandlerInterface& policy_handler_;
  rc_rpc_plugin::ResourceAllocationManager& resource_allocation_manager_;
  rc_rpc_plugin::InteriorDataCache& interior_data_cache_;
};
}
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_RC_COMMAND_PARAMS_H

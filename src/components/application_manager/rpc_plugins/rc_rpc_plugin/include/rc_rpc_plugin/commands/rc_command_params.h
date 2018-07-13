#ifndef RC_COMMAND_PARAMS_H
#define RC_COMMAND_PARAMS_H

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
  //    const application_manager::commands::MessageSharedPtr& message_;
  application_manager::ApplicationManager& application_manager_;
  application_manager::rpc_service::RPCService& rpc_service_;
  application_manager::HMICapabilities& hmi_capabilities_;
  policy::PolicyHandlerInterface& policy_handler_;
  rc_rpc_plugin::ResourceAllocationManager& resource_allocation_manager_;
  rc_rpc_plugin::InteriorDataCache& interior_data_cache_;
};
}
#endif  // RC_COMMAND_PARAMS_H

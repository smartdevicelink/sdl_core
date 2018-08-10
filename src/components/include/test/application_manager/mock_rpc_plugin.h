#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_MOCK_RPC_PLUGIN_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_MOCK_RPC_PLUGIN_H

#include "gmock/gmock.h"
#include "application_manager/plugin_manager/rpc_plugin.h"

namespace application_manager {
class CommandFactory;
namespace plugin_manager {

class MockRPCPlugin : public RPCPlugin {
 public:
  MOCK_METHOD4(Init,
               bool(ApplicationManager& app_manager,
                    rpc_service::RPCService& rpc_service,
                    HMICapabilities& hmi_capabilities,
                    policy::PolicyHandlerInterface& policy_handler));
  MOCK_METHOD2(IsAbleToProcess,
               bool(const int32_t function_id,
                    const commands::Command::CommandSource message_source));
  MOCK_METHOD0(PluginName, std::string());
  MOCK_METHOD0(GetCommandFactory, CommandFactory&());
  MOCK_METHOD1(OnPolicyEvent, void(PolicyEvent event));
  MOCK_METHOD2(OnApplicationEvent,
               void(ApplicationEvent event,
                    application_manager::ApplicationSharedPtr application));
};
}  // namespace plugin_manager
}  // namespace application_manager
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_PLUGIN_MANAGER_MOCK_RPC_PLUGIN_H

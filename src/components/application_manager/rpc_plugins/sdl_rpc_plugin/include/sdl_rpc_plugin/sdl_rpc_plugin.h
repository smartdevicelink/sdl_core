#ifndef SDL_RPC_PLUGIN_H
#define SDL_RPC_PLUGIN_H
#include "application_manager/plugin_manager/rpc_plugin.h"
#include "application_manager/command_factory.h"

namespace sdl_rpc_plugin {
namespace plugins = application_manager::plugin_manager;
class SDLRPCPlugin : public plugins::RPCPlugin {
  // RPCPlugin interface
 public:
  bool Init(application_manager::ApplicationManager& app_manager,
            application_manager::rpc_service::RPCService& rpc_service,
            application_manager::HMICapabilities& hmi_capabilities,
            policy::PolicyHandlerInterface& policy_handler) OVERRIDE;

  bool IsAbleToProcess(
      const int32_t function_id,
      const application_manager::commands::Command::CommandSource
          message_source) OVERRIDE;
  std::string PluginName() OVERRIDE;

  application_manager::CommandFactory& GetCommandFactory() OVERRIDE;
  void OnPolicyEvent(
      application_manager::plugin_manager::PolicyEvent event) OVERRIDE;
  void OnApplicationEvent(
      application_manager::plugin_manager::ApplicationEvent event,
      application_manager::ApplicationSharedPtr application) OVERRIDE;

 private:
  std::unique_ptr<application_manager::CommandFactory> command_factory_;
};
}

extern "C" application_manager::plugin_manager::RPCPlugin* Create();

#endif  // SDL_RPC_PLUGIN_H

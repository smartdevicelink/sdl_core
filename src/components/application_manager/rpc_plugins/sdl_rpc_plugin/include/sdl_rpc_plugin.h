#ifndef SDL_RPC_PLUGIN_H
#define SDL_RPC_PLUGIN_H
#include "application_manager/plugin_manager/rpc_plugin.h"
namespace sdl_rpc_plugin {
namespace plugins = application_manager::plugin_manager;
class SDLRPCPlugin : plugins::RPCPlugin {
  // RPCPlugin interface
 public:
  bool Init() OVERRIDE;
  bool IsAbleToProcess(
      const int32_t function_id,
      const application_manager::commands::Command::CommandOrigin
          message_source) OVERRIDE;
  std::string PluginName() OVERRIDE;
  application_manager::plugin_manager::CommandFactory& GetCommandFactory()
      override;
  void OnPolicyEvent(
      application_manager::plugin_manager::PolicyEvent event) OVERRIDE;
  void OnApplicationEvent(
      application_manager::plugin_manager::ApplicationEvent event,
      application_manager::ApplicationSharedPtr application) OVERRIDE;
};
}

#endif  // SDL_RPC_PLUGIN_H

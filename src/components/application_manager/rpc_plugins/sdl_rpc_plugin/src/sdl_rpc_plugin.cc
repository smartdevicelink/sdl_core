#include "sdl_rpc_plugin/sdl_rpc_plugin.h"
#include "sdl_rpc_plugin/sdl_command_factory.h"

namespace sdl_rpc_plugin {
namespace plugins = application_manager::plugin_manager;

bool SDLRPCPlugin::Init(
    application_manager::ApplicationManager& app_manager,
    application_manager::rpc_service::RPCService& rpc_service,
    application_manager::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler) {
  command_factory_.reset(new sdl_rpc_plugin::SDLCommandFactory(
      app_manager, rpc_service, hmi_capabilities, policy_handler));
  return true;
}

bool SDLRPCPlugin::IsAbleToProcess(
    const int32_t function_id,
    const application_manager::commands::Command::CommandSource
        message_source) {
  return true;
}

std::string SDLRPCPlugin::PluginName() {
  return "SDL RPC Plugin";
}

application_manager::CommandFactory& SDLRPCPlugin::GetCommandFactory() {
  return *command_factory_;
}

void SDLRPCPlugin::OnPolicyEvent(
    application_manager::plugin_manager::PolicyEvent event) {}

void SDLRPCPlugin::OnApplicationEvent(
    application_manager::plugin_manager::ApplicationEvent event,
    application_manager::ApplicationSharedPtr application) {}

}  // namespace sdl_rpc_plugin

extern "C" application_manager::plugin_manager::RPCPlugin* Create() {
  return new sdl_rpc_plugin::SDLRPCPlugin();
}

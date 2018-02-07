#include "sdl_rpc_plugin/sdl_rpc_plugin.h"

namespace sdl_rpc_plugin {
namespace plugins = application_manager::plugin_manager;
bool SDLRPCPlugin::Init() {
  return true;
}

bool SDLRPCPlugin::IsAbleToProcess(
    const int32_t function_id,
    const application_manager::commands::Command::CommandOrigin
        message_source) {
  return true;
}

std::string SDLRPCPlugin::PluginName() {
  return "SDL RPC Plugin";
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

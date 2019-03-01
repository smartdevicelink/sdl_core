#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_GET_CLOUD_APP_PROPERTIES_RESPONSE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_GET_CLOUD_APP_PROPERTIES_RESPONSE_H_

#include "application_manager/commands/command_response_impl.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

class GetCloudAppPropertiesResponse
    : public app_mngr::commands::CommandResponseImpl {
 public:
  GetCloudAppPropertiesResponse(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler);
  virtual ~GetCloudAppPropertiesResponse();
  virtual void Run();

 private:
  DISALLOW_COPY_AND_ASSIGN(GetCloudAppPropertiesResponse);

};  // GetCloudAppPropertiesResponse

}  // namespace commands
}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_GET_CLOUD_APP_PROPERTIES_RESPONSE_H_

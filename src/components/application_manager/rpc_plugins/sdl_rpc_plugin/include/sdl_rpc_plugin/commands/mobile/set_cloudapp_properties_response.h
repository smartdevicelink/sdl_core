#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SET_CLOUDAPP_PROPERTIES_RESPONSE_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SET_CLOUDAPP_PROPERTIES_RESPONSE_H_

#include "application_manager/commands/command_response_impl.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;
namespace commands {

class SetCloudAppPropertiesResponse : public app_mngr::commands::CommandResponseImpl{
public:
    SetCloudAppPropertiesResponse(const app_mngr::commands::MessageSharedPtr& message,
               app_mngr::ApplicationManager& application_manager,
               app_mngr::rpc_service::RPCService& rpc_service,
               app_mngr::HMICapabilities& hmi_capabilities,
               policy::PolicyHandlerInterface& policy_handler);


    virtual ~SetCloudAppPropertiesResponse();

    virtual void Run();

private:
  DISALLOW_COPY_AND_ASSIGN(SetCloudAppPropertiesResponse);
    
}; //SetCloudAppPropertiesResponse
} // namespace commands
} // namespace sdl_rpc_plugin

#endif //SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_SET_CLOUDAPP_PROPERTIES_RESPONSE_H_
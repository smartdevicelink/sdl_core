#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_GET_CLOUD_APP_PROPERTIES_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_GET_CLOUD_APP_PROPERTIES_REQUEST_H_

#include "application_manager/commands/request_from_mobile_impl.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

class GetCloudAppPropertiesRequest
    : public app_mngr::commands::RequestFromMobileImpl {
 public:
  GetCloudAppPropertiesRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler);
  virtual ~GetCloudAppPropertiesRequest();
  virtual void Run();
  virtual void on_event(const app_mngr::event_engine::Event& event);

 private:
  DISALLOW_COPY_AND_ASSIGN(GetCloudAppPropertiesRequest);
};  // GetCloudAppPropertiesRequest

}  // namespace commands
}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_GET_CLOUD_APP_PROPERTIES_REQUEST_H_

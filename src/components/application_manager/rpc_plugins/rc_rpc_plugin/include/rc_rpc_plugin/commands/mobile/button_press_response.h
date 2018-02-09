#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_BUTTON_PRESS_RESPONSE_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_BUTTON_PRESS_RESPONSE_H

#include "application_manager/commands/command_response_impl.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class ButtonPressResponse
    : public application_manager::commands::CommandResponseImpl {
 public:
  ButtonPressResponse(const app_mngr::commands::MessageSharedPtr& message,
                      app_mngr::ApplicationManager& application_manager);
  bool Init() OVERRIDE;
  void Run() OVERRIDE;
  void on_event(const application_manager::event_engine::Event& event);
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_BUTTON_PRESS_RESPONSE_H

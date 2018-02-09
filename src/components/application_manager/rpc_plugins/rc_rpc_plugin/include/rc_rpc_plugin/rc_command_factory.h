#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_COMMAND_FACTORY_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_COMMAND_FACTORY_H

#include <memory>
#include "application_manager/command_factory.h"
#include "application_manager/application_manager.h"
#include "application_manager/command_factory.h"
#include "application_manager/rpc_service.h"
#include "application_manager/hmi_capabilities.h"
#include "application_manager/policies/policy_handler_interface.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;
class RCCommandFactory : public application_manager::CommandFactory {
 public:
  RCCommandFactory(app_mngr::ApplicationManager& app_manager,
                   app_mngr::rpc_service::RPCService& rpc_service,
                   app_mngr::HMICapabilities& hmi_capabilities,
                   policy::PolicyHandlerInterface& policy_handler);
  application_manager::CommandSharedPtr CreateCommand(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::commands::Command::CommandSource source) OVERRIDE;

 private:
  application_manager::CommandSharedPtr CreateMobileCommand(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::commands::Command::CommandSource source);

  application_manager::CommandSharedPtr CreateHMICommand(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::commands::Command::CommandSource source);

  app_mngr::ApplicationManager& app_manager_;
  app_mngr::rpc_service::RPCService& rpc_service_;
  app_mngr::HMICapabilities& hmi_capabilities_;
  policy::PolicyHandlerInterface& policy_handler_;
};
}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_COMMAND_FACTORY_H

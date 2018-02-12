#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_BUTTON_PRESS_REQUEST_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_BUTTON_PRESS_REQUEST_H

#include "rc_rpc_plugin/commands/rc_command_request.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class ButtonPressRequest : public RCCommandRequest {
 public:
  ButtonPressRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      rc_rpc_plugin::ResourceAllocationManager& resource_allocation_manager);

  /**
   * @brief Execute command
   * send HMI request if message contains appropriate
   * button name and module type
   * otherwise sends negative sesponse to mobile
   */
  void Execute() FINAL;

  /**
   * @brief AcquireResource Tries to acquire specific resource
   * @param message Incoming message containg the resource name
   * @return Acquire result
   */
  AcquireResult::eType AcquireResource(
      const app_mngr::commands::MessageSharedPtr& message) FINAL;

  /**
   * @brief IsResourceFree check resource state
   * @param module_type Resource name
   * @return True if free, otherwise - false
   */
  bool IsResourceFree(const std::string& module_type) const FINAL;

  /**
   * @brief SetResourceState changes state of resource
   * @param state State to set for resource
   */
  void SetResourceState(const std::string& module_type,
                        const ResourceState::eType state) FINAL;

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const app_mngr::event_engine::Event& event) FINAL;

  /**
   * @brief ButtonPressRequest class destructor
   */
  virtual ~ButtonPressRequest();
};

}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_BUTTON_PRESS_REQUEST_H

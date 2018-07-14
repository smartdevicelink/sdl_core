/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_HMI_RC_ON_REMOTE_CONTROL_SETTINGS_NOTIFICATION_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_HMI_RC_ON_REMOTE_CONTROL_SETTINGS_NOTIFICATION_H_

#include "application_manager/commands/notification_from_hmi.h"
#include "rc_rpc_plugin/commands/rc_command_request.h"
#include "rc_rpc_plugin/interior_data_cache.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;
class InteriorDataManager;

namespace commands {
class RCOnRemoteControlSettingsNotification
    : public application_manager::commands::NotificationFromHMI {
 public:
  /**
   * @brief RCOnRemoteControlSettingsNotification class constructor
   * @param message MessageSharedPtr
   * @param application_manager ApplicationManager
   * @param rpc_service RPCService
   * @param hmi_capabilities HMICapabilities
   * @param policy_handle PolicyHandlerInterface
   * @param resource_allocation_manager ResourceAllocationManager
   **/
  RCOnRemoteControlSettingsNotification(
      const application_manager::commands::MessageSharedPtr& message,
      const RCCommandParams& params);
  /**
   * @brief Execute command
   **/
  void Run() OVERRIDE;

  ~RCOnRemoteControlSettingsNotification();

 private:
  ResourceAllocationManager& resource_allocation_manager_;
  InteriorDataManager& interior_data_manager_;
  /**
   * @brief Disalows RC functionality for all RC apps
   * All registered apps will be unsubsribed from OnInteriorVehicleData
   * notifications
   */
  void DisallowRCFunctionality();
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_HMI_RC_ON_REMOTE_CONTROL_SETTINGS_NOTIFICATION_H_

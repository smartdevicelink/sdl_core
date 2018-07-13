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

 Neither the name of the copyright holders nor the names of their contributors
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_ON_INTERIOR_VEHICLE_DATA_NOTIFICATION_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_ON_INTERIOR_VEHICLE_DATA_NOTIFICATION_H_

#include <string>
#include "application_manager/commands/command_notification_impl.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"
#include "rc_rpc_plugin/interior_data_cache.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class OnInteriorVehicleDataNotification
    : public application_manager::commands::CommandNotificationImpl {
 public:
  /**
   * @brief Constructor of OnInteriorVehicleDataNotification
   * @param message MessageSharedPtr
   * @param application_manager ApplicationManager
   * @param rpc_service RPCService
   * @param hmi_capabilities HMICapabilities
   * @param policy_handle PolicyHandlerInterface
   * @param resource_allocation_manager ResourceAllocationManager
   * @param interior_data_cache InteriorDataCache
   */
  OnInteriorVehicleDataNotification(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler,
      ResourceAllocationManager& resource_allocation_manager,
      InteriorDataCache& interior_data_cache);

  /**
   * @brief Run OnInteriorVehicleDataNotification command
   */
  void Run() OVERRIDE;

  std::string ModuleType();

  /**
   * @brief OnInteriorVehicleDataNotification class destructor
   */
  ~OnInteriorVehicleDataNotification();

 private:
  InteriorDataCache& interior_data_cache_;

  /**
   * @brief Method of adding data to cache
   * @param module_type std::string
   */
  void AddDataToCache(const std::string& module_type);
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_ON_INTERIOR_VEHICLE_DATA_NOTIFICATION_H_

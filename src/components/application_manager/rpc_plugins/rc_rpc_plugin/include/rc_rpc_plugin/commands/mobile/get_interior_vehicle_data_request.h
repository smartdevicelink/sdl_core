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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_GET_INTERIOR_VEHICLE_DATA_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_GET_INTERIOR_VEHICLE_DATA_REQUEST_H_

#include "rc_rpc_plugin/commands/rc_command_request.h"
#include <string>

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class GetInteriorVehicleDataRequest : public RCCommandRequest {
 public:
  /**
   * @brief Constructor of GetInteriorVehicleDataRequest
   * @param message MessageSharedPtr
   * @param application_manager ApplicationManager
   * @param rpc_service RPCService
   * @param hmi_capabilities HMICapabilities
   * @param policy_handle PolicyHandlerInterface
   * @param resource_allocation_manager ResourceAllocationManager
   * @param interior_data_cache InteriorDataCache
   */
  GetInteriorVehicleDataRequest(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handle,
      ResourceAllocationManager& resource_allocation_manager,
      InteriorDataCache& interior_data_cache);
  /**
   * @brief Execute command
   */
  void Execute() FINAL;

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const app_mngr::event_engine::Event& event) FINAL;

  /**
   * @brief GetInteriorVehicleDataRequest class destructor
   */
  ~GetInteriorVehicleDataRequest();

 private:
  std::vector<application_manager::ApplicationSharedPtr>
  AppsSubscribedToModuleType(const std::string& module_type);

  /**
   * @brief Check if app wants to proceed with already setup subscription
   * @param request_params request parameters to check
   * @return true if app already subscribed(unsubsribed) for module type but
   * wants to subscribe(unsubscribe) for the same module again
   * otherwise - false
   */
  bool HasRequestExcessiveSubscription();

  /**
    * @brief Handle subscription to vehicle data
    * @param hmi_response json message with response from HMI
    */
  void ProccessSubscription(
      const NsSmartDeviceLink::NsSmartObjects::SmartObject& hmi_response);

  /**
   * @brief Cuts off subscribe parameter
   * @param request_params request parameters to handle
   */
  void RemoveExcessiveSubscription();

  /**
   * @brief Get the module type
   * @return module type - std::string
   */
  std::string ModuleType() FINAL;
  bool excessive_subscription_occured_;
  uint32_t max_request_in_time_frame_;

  /**
   * @brief Process the capabilities
   * @return bool - result of processing
   */
  bool ProcessCapabilities();

  /**
   * @brief Process the responce from cache to mobile
   * @param app ApplicationSharedPtr
   */
  void ProcessResponseToMobileFromCache(app_mngr::ApplicationSharedPtr app);

  /**
   * @brief Check rate limits. When the requests in time frame more then
   * allowed, send REJECTED responce to mobile
   * @return bool - result of check
   */
  bool CheckRateLimits();

  /**
   * @brief Check is app should be unsubscribed
   * @param bool - result of check
   */
  bool AppShouldBeUnsubscribed();

  /**
   * @brief Check if the subscribed on module app is the last
   * @param bool - result of check
   */
  bool TheLastAppShouldBeUnsubscribed(app_mngr::ApplicationSharedPtr app);
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_GET_INTERIOR_VEHICLE_DATA_REQUEST_H_

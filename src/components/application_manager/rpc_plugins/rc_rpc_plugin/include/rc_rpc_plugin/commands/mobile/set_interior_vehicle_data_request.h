/*
 * Copyright (c) 2018, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_SET_INTERIOR_VEHICLE_DATA_REQUEST_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_SET_INTERIOR_VEHICLE_DATA_REQUEST_H

#include "rc_rpc_plugin/commands/rc_command_request.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class SetInteriorVehicleDataRequest : public RCCommandRequest {
 public:
  SetInteriorVehicleDataRequest(
      const application_manager::commands::MessageSharedPtr& message,
      const RCCommandParams& params);

  /**
   * @brief Execute command
   */
  void Execute() FINAL;

  /**
   * @brief AcquireResource proxy AcquireResource to Resource allocation manager
   * @param message message of requires contatin module types
   * @return result of acauiring resources
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
   * @brief Method that check if READ_ONLY parameters present
   * @param request_params params from received message
   * @return true if present , false - otherwise
   */
  bool AreReadOnlyParamsPresent(const smart_objects::SmartObject& module_data);

  /**
   * @brief Method that check if all request parameters are READ_ONLY
   * @param request_params params from received message
   * @return true if all are read only , false - otherwise
   */
  bool AreAllParamsReadOnly(const smart_objects::SmartObject& module_data);

  /**
   * @brief Method that cuts-off READ_ONLY parameters
   * @param request_params params to handle
   */
  void CutOffReadOnlyParams(smart_objects::SmartObject& module_data);

  std::string ModuleType() FINAL;

  /**
   * @brief SetInteriorVehicleDataRequest class destructor
   */
  ~SetInteriorVehicleDataRequest();

 private:
  /**
   * @brief ControlData
   * @param module_data received params
   * @return value of module data depending on module type
   */
  const smart_objects::SmartObject& ControlData(
      const smart_objects::SmartObject& module_data);
};
}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_COMMANDS_SET_INTERIOR_VEHICLE_DATA_REQUEST_H

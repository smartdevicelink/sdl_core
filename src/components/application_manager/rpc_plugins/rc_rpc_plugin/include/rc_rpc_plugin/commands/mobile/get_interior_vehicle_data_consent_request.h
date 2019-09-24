/*
 * Copyright (c) 2019, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_RC_GET_INTERIOR_VEHICLE_DATA_CONSENT_REQUEST_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_RC_GET_INTERIOR_VEHICLE_DATA_CONSENT_REQUEST_H

#include <string>

#include "rc_rpc_plugin/commands/rc_command_params.h"
#include "rc_rpc_plugin/commands/rc_command_request.h"
#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

class GetInteriorVehicleDataConsentRequest
    : public rc_rpc_plugin::commands::RCCommandRequest {
 public:
  /**
   * @brief GetInteriorVehicleDataConsentRequest constructor
   * @param message smart pointer with SmartObject
   * @param params structure that contains references to
   * parameters used in remote сontrol commands
   */
  GetInteriorVehicleDataConsentRequest(
      const application_manager::commands::MessageSharedPtr& message,
      const RCCommandParams& params);

  /**
   * @brief Execute command
   */
  void Execute() FINAL;

  void on_event(const app_mngr::event_engine::Event& event) FINAL;

  std::string ModuleType() const FINAL;

  /**
   * @brief For this RPC this method isn't correct, because SDL receives array
   * of module_ids instead of only one module_id. This method returns empty
   * string.
   */
  std::string ModuleId() const FINAL;

  ~GetInteriorVehicleDataConsentRequest();

 private:
  /**
   * @brief Saves ModuleId consents (saved before moduleids + received moduleId
   * consents + date_of_consent) to file.
   *
   * @param msg_params Message params from response message as SmartObject.
   */
  bool SaveModuleIdConsents(std::string& info_out,
                            const smart_objects::SmartObject& msg_params);

  /**
   * @brief Gets calculated vehicle data consent and puts it into provided smart
   * object
   * @param out_response output smart object
   * @return true in case all required consent information was provided in
   * output smart object, otherwise returns false
   */
  bool GetCalculatedVehicleDataConsent(
      smart_objects::SmartObject& out_response) const;

  /**
   * @brief Checks whether the user_location is covered by the module's
   * serviceArea
   * @param module_uid module key(module_type + module_id)
   * @return true if the user_location is covered by the module's serviceArea or
   * the user is the driver (or if seatLocationCapabilities don't exists)
   * otherwise false
   */
  bool IsUserLocationValid(const ModuleUid& module_uid) const;

  /**
   * @brief Checks whether user should have access to module using the specific
   * accessMode's rules
   * @param module_uid module key(moudle_type + module_id)
   * @param access_mode current HMI accessMode
   * @return consent enum value
   */
  rc_rpc_types::ModuleConsent GetModuleConsentByAccessMode(
      const ModuleUid& module_uid,
      const hmi_apis::Common_RCAccessMode::eType access_mode) const;

  smart_objects::SmartObject hmi_request_consents_;
};

}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_RC_GET_INTERIOR_VEHICLE_DATA_CONSENT_REQUEST_H

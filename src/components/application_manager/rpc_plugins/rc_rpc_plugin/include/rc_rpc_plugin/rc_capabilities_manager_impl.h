/*
 Copyright (c) 2019, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_CAPABILITIES_MANAGER_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_CAPABILITIES_MANAGER_IMPL_H_
#include "application_manager/application_manager.h"
#include "rc_rpc_plugin/rc_capabilities_manager.h"

namespace rc_rpc_plugin {
class RCCapabilitiesManagerImpl : public RCCapabilitiesManager {
 public:
  RCCapabilitiesManagerImpl(
      application_manager::HMICapabilities& hmi_capabilities);

  bool CheckIfButtonExistInRCCaps(
      const mobile_apis::ButtonName::eType button) FINAL;

  bool CheckButtonName(const std::string& module_type,
                       const std::string& button_name) FINAL;

  bool CheckIfModuleTypeExistInCapabilities(
      const std::string& module_type) FINAL;

  ModuleCapability GetModuleDataCapabilities(
      const smart_objects::SmartObject& module_data) FINAL;

  const smart_objects::SmartObject& ControlData(
      const smart_objects::SmartObject& module_data,
      const std::string& module_type) FINAL;

  bool AreReadOnlyParamsPresent(
      const smart_objects::SmartObject& module_data,
      const std::string& module_type,
      ModuleCapability& module_data_capabilities) FINAL;

  bool AreAllParamsReadOnly(const smart_objects::SmartObject& module_data,
                            const std::string& module_type) FINAL;

 private:
  const std::map<std::string, std::string> GetLightCapabilitiesMapping();

  const std::map<std::string, std::string> GetModuleDataToCapabilitiesMapping();

  /**
   * @brief Check whether the exists light data related to correspondent
   * capabilities
   * @param smart object of capabilities
   * @param smart object of control_data
   * @return pair of state and capability status - ModuleCapability
   */
  ModuleCapability GetControlDataCapabilities(
      const smart_objects::SmartObject& capabilities,
      const smart_objects::SmartObject& control_data);

  /**
   * @brief Check whether the parameter exist in capabilities
   * @param smart object of capabilities
   * @param mapping - map of module data and capabilities
   * @param request_parameter - string
   * @param switched_off_result - ref of mobile_apis::Result
   * @return success if parameter exist in capabilities missedParam otherwise
   */
  capabilitiesStatus GetItemCapability(
      const smart_objects::SmartObject& capabilities,
      const std::map<std::string, std::string>& mapping,
      const std::string& request_parameter,
      const mobile_apis::Result::eType& switched_off_result);

  /**
   * @brief Check whether the cpabilities for light allowed
   * @param smart object of capabilities
   * @param smart object of control_data
   * @return pair of state and capability status - ModuleCapability
   */
  ModuleCapability GetLightDataCapabilities(
      const smart_objects::SmartObject& capabilities,
      const smart_objects::SmartObject& control_data);

  /**
   * @brief Check whether the light name exists in capabilities
   * @param smart object of capabilities_status
   * @param smart object of light_data
   * @return pair of state and capability status - ModuleCapability
   */
  ModuleCapability GetLightNameCapabilities(
      const smart_objects::SmartObject& capabilities_status,
      const smart_objects::SmartObject& light_data);

  ModuleCapability GetRadioBandByCapabilities(
      const smart_objects::SmartObject& capabilities_status,
      const smart_objects::SmartObject& request_parameter);

  bool CheckReadOnlyParamsForAudio(
      const smart_objects::SmartObject& module_type_params);

  bool CheckReadOnlyParamsForLight(
      const smart_objects::SmartObject& module_type_params);

  application_manager::HMICapabilities& hmi_capabilities_;
};

}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_CAPABILITIES_MANAGER_IMPL_H_

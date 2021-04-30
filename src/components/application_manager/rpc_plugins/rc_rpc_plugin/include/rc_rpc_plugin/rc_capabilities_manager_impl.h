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
      const mobile_apis::ButtonName::eType button) const FINAL;

  bool CheckButtonName(const std::string& module_type,
                       const std::string& button_name) const FINAL;

  bool CheckIfModuleExistsInCapabilities(const ModuleUid& module) const FINAL;

  ModuleTypeCapability GetModuleDataCapabilities(
      const smart_objects::SmartObject& module_data,
      const std::string& module_id) const FINAL;

  const smart_objects::SmartObject& ControlDataForType(
      const smart_objects::SmartObject& module_data,
      const std::string& module_type) const FINAL;

  bool AreReadOnlyParamsPresent(
      const smart_objects::SmartObject& module_data,
      const std::string& module_type,
      ModuleTypeCapability& module_data_capabilities) const FINAL;

  bool AreAllParamsReadOnly(const smart_objects::SmartObject& module_data,
                            const std::string& module_type) const FINAL;

  const std::string GetDefaultModuleIdFromCapabilities(
      const std::string& module_type) const FINAL;

  const std::vector<ModuleUid> GetResources() const FINAL;

  Grid GetModuleServiceArea(const ModuleUid& module) const FINAL;

  bool IsMultipleAccessAllowed(const ModuleUid& module) const FINAL;

  const Grid GetDriverLocationFromSeatLocationCapability() const FINAL;

  bool IsSeatLocationCapabilityProvided() const FINAL;

  const std::string GetModuleIdForSeatLocation(
      mobile_apis::SupportedSeat::eType id) const FINAL;

 private:
  const std::map<std::string, std::string> GetLightCapabilitiesMapping() const;

  const std::map<std::string, std::string> GetModuleDataToCapabilitiesMapping()
      const;

  /**
   * @brief Check whether the exists light data related to correspondent
   * capabilities
   * @param capabilities smart object of capabilities
   * @param control_data smart object of control_data
   * @return pair of state and capability status - ModuleTypeCapability
   */
  ModuleTypeCapability GetControlDataCapabilities(
      const smart_objects::SmartObject& capabilities,
      const smart_objects::SmartObject& control_data) const;

  /**
   * @brief Check whether the parameter exist in capabilities
   * @param capabilities smart object of capabilities
   * @param mapping - map of module data and capabilities
   * @param request_parameter - string
   * @param switched_off_result - ref of mobile_apis::Result
   * @return success if the parameter exists in capabilities,
   * otherwise - missedParam
   */
  capabilitiesStatus GetItemCapability(
      const smart_objects::SmartObject& capabilities,
      const std::map<std::string, std::string>& mapping,
      const std::string& request_parameter,
      const mobile_apis::Result::eType& switched_off_result) const;

  /**
   * @brief Check whether the capabilities for light are allowed
   * @param capabilities smart object of capabilities
   * @param control_data smart object of control_data
   * @return pair of state and capability status - ModuleTypeCapability
   */
  ModuleTypeCapability GetLightDataCapabilities(
      const smart_objects::SmartObject& capabilities,
      const smart_objects::SmartObject& control_data) const;

  /**
   * @brief Check whether the light name exists in capabilities
   * @param capabilities_status smart object of capabilities_status
   * @param light_data smart object of light_data
   * @return pair of state and capability status - ModuleTypeCapability
   */
  ModuleTypeCapability GetLightNameCapabilities(
      const smart_objects::SmartObject& capabilities_status,
      const smart_objects::SmartObject& light_data) const;

  ModuleTypeCapability GetRadioBandByCapabilities(
      const smart_objects::SmartObject& capabilities_status,
      const smart_objects::SmartObject& request_parameter) const;

  bool CheckReadOnlyParamsForAudio(
      const smart_objects::SmartObject& module_type_params) const;

  bool CheckReadOnlyParamsForLight(
      const smart_objects::SmartObject& module_type_params) const;

  smart_objects::SmartObject GetCapabilitiesByModuleIdFromArray(
      const smart_objects::SmartObject& module_data_capabilities,
      const std::string& module_id) const;

  const bool CheckModuleIdWithCapabilities(
      const smart_objects::SmartObject& rc_capabilities,
      const ModuleUid& module) const;

  const bool CheckModuleIdWithCapabilitiesArrays(
      const smart_objects::SmartObject& control_capabilities,
      const std::string& module_id) const;

  const bool CheckModuleIdWithCapabilitiesStructure(
      const smart_objects::SmartObject& control_capabilities,
      const std::string& module_id) const;

  const std::string GetDefaultModuleIdFromCapabilitiesStructure(
      const smart_objects::SmartObject& control_capabilities,
      const std::string& module_type) const;

  const std::string GetDefaultModuleIdFromCapabilitiesArray(
      const smart_objects::SmartObject& control_capabilities,
      const std::string& module_type) const;

  const std::vector<std::string> GetCapabilitiesList() const;

  const std::function<std::string(const std::string& control_cap)>
  GetCapabilitiesToModuleTypeMapping() const;

  void GetResourcesFromCapabilitiesStructure(
      const smart_objects::SmartObject& control_capabilities,
      const std::string& capabitity_key,
      std::vector<ModuleUid>& out_resources) const;

  void GetResourcesFromCapabilitiesArray(
      const smart_objects::SmartObject& control_capabilities,
      const std::string& capability_key,
      std::vector<ModuleUid>& out_resources) const;

  Grid GetModuleServiceAreaFromControlCapability(
      const smart_objects::SmartObject& control_capabilities) const;

  Grid GetModuleLocationFromControlCapability(
      const smart_objects::SmartObject& control_capabilities) const;

  bool IsMultipleAccessAllowedInControlCaps(
      const smart_objects::SmartObject& control_capabilities) const;

  Grid GetWholeVehicleArea() const;

  application_manager::HMICapabilities& hmi_capabilities_;
};

}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_CAPABILITIES_MANAGER_IMPL_H_

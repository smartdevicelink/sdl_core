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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_CAPABILITIES_MANAGER_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_CAPABILITIES_MANAGER_H_
#include "application_manager/application_manager.h"
#include "rc_rpc_plugin/resource_allocation_manager.h"

namespace rc_rpc_plugin {

enum capabilitiesStatus {
  kInvalidStatus,
  kSuccess,
  kMissedLightName,
  kMissedParam,
  kReadOnly
};
typedef std::pair<std::string, capabilitiesStatus> ModuleTypeCapability;

class RCCapabilitiesManager {
 public:
  /**
   * @brief CheckIfButtonExistInRCCaps checks if the
   * specified button exists in rc_capabilities
   * @param button button name
   * @return true if present, otherwise - false
   */
  virtual bool CheckIfButtonExistInRCCaps(
      const mobile_apis::ButtonName::eType button) const = 0;

  /**
   * @brief CheckButtonName checks if the button
   * corresponds to the module_type
   * @param module_type resource name
   * @param button_name button name
   * @return true if they match, otherwise - false
   */
  virtual bool CheckButtonName(const std::string& module_type,
                               const std::string& button_name) const = 0;

  /**
   * @brief CheckIfModuleExistsInCapabilities checks
   * if the specified module exists in rc_capabilities
   * @param module moduleType + moduleId
   * @return true if exists, otherwise - false
   */
  virtual bool CheckIfModuleExistsInCapabilities(
      const ModuleUid& module) const = 0;

  /**
   * @brief GetModuleDataCapabilities checks whether rc module data
   * capabilities are presented
   * @param module_data smart object of module_data
   * @param module_id module id
   * @return pair of state and capability status - ModuleCapability
   */
  virtual ModuleTypeCapability GetModuleDataCapabilities(
      const smart_objects::SmartObject& module_data,
      const std::string& module_id) const = 0;

  /**
   * @brief ControlData retrieves control data
   * for specified module type
   * @param module_data smart object of module_data
   * @param module_type resource name
   * @return smart object of control data
   */
  virtual const smart_objects::SmartObject& ControlDataForType(
      const smart_objects::SmartObject& module_data,
      const std::string& module_type) const = 0;

  /**
   * @brief AreReadOnlyParamsPresent checks if there are
   * any read only params in the module data
   * @param module_data smart object of module_data
   * @param module_type resource name
   * @param module_data_capabilities smart object of capabilities
   * @return true if read only params exist, otherwise - false
   */
  virtual bool AreReadOnlyParamsPresent(
      const smart_objects::SmartObject& module_data,
      const std::string& module_type,
      ModuleTypeCapability& module_data_capabilities) const = 0;

  /**
   * @brief AreAllParamsReadOnly checks that all params from module
   * data are read only
   * @param module_data smart object of module_data
   * @param module_type resource name
   * @return true if all params are read only, otherwise - false
   */
  virtual bool AreAllParamsReadOnly(
      const smart_objects::SmartObject& module_data,
      const std::string& module_type) const = 0;

  /**
   * @brief GetDefaultModuleIdFromCapabilities returns the moduleId
   * from the first item published by xyz-ControlCapabilities
   * @param module_type resource name
   * @return default module id from HMI capabilities
   */
  virtual const std::string GetDefaultModuleIdFromCapabilities(
      const std::string& module_type) const = 0;

  /**
   * @brief GetResources get list of ModuleUids(moduleType + moduleId)
   * of all known resources
   * @return vector contains all known moduleUids
   */
  virtual const std::vector<ModuleUid> GetResources() const = 0;

  /**
   * @brief GetModuleServiceArea retrieves service area of module,
   * if module serviceArea is missed in capabilities, serviceArea is
   * assumed to be the same as location, if location
   * is missed in capabilities, it is assumed that serviceArea covers
   * the whole area of vehicle
   * @param module module type + module id
   * @return Grid of module service area
   */
  virtual Grid GetModuleServiceArea(const ModuleUid& module) const = 0;

  /**
   * @brief IsMultipleAccessAllowed checks if multiple access allowed
   * for requested module
   * @param module module type + module id
   * @return true if allowed, otherwise - false
   */
  virtual bool IsMultipleAccessAllowed(const ModuleUid& module) const = 0;

  /**
   * @brief GetDriverLocationFromSeatLocationCapability retrieves the driver's
   * location from seat location capabilities
   * @return Grid of driver's location
   */
  virtual const Grid GetDriverLocationFromSeatLocationCapability() const = 0;

  /**
   * @brief IsSeatLocationCapabilityProvided checks whether all necessary
   * parameters are provided in seat location capabilities
   * @return true if provided, otherwise - false
   */
  virtual bool IsSeatLocationCapabilityProvided() const = 0;

  /**
   * @brief GetModuleIdForSeatLocation returns the moduleId if request from
   * mobile contains the id parameter and doesn't contain the moduleId
   * parameter. If the vehicle support at least one seat control, seat control
   * capability includes at least two items. SupportedSeat id shall be converted
   * to the moduleId according to capabilities: 1st item from capabilities for
   * driver’s seat. 2nd item for front passenger’s seat.
   * @param id supported seat id
   */
  virtual const std::string GetModuleIdForSeatLocation(
      mobile_apis::SupportedSeat::eType id) const = 0;

  virtual ~RCCapabilitiesManager() {}
};

}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_CAPABILITIES_MANAGER_H_

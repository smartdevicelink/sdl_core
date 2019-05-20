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

namespace rc_rpc_plugin {

enum capabilitiesStatus { success, missedLightName, missedParam, readOnly };
typedef std::pair<std::string, capabilitiesStatus> ModuleCapability;

class RCCapabilitiesManager {
 public:
  /**
   * @brief CheckIfButtonExistInRCCaps checks if the
   * specified button exists in rc_capabilities
   * @param button button name
   * @return true if button exists, otherwise - false
   */
  virtual bool CheckIfButtonExistInRCCaps(
      const mobile_apis::ButtonName::eType button) = 0;

  /**
   * @brief CheckButtonName checks if the button
   * corresponds to the module_type
   * @param module_type resource name
   * @param button_name button name
   * @return true if they match, otherwise - false
   */
  virtual bool CheckButtonName(const std::string& module_type,
                               const std::string& button_name) = 0;

  /**
   * @brief CheckIfModuleTypeExistInCapabilities checks
   * if the specified module type exists in rc_capabilities
   * @param module_typere source name
   * @return true if exists, otherwise - false
   */
  virtual bool CheckIfModuleTypeExistInCapabilities(
      const std::string& module_type) = 0;

  /**
   * @brief GetModuleDataCapabilities checks whether rc module data
   * capabilities are presented
   * @param smart object of rc_capabilities
   * @param smart object of module_data
   * @return pair of state and capability status - ModuleCapability
   */
  virtual ModuleCapability GetModuleDataCapabilities(
      const smart_objects::SmartObject& module_data) = 0;

  /**
   * @brief ControlData retrieves control data
   * for specified module type
   * @param module_data smart object of module_data
   * @param module_type resource name
   * @return smart object of control data
   */
  virtual const smart_objects::SmartObject& ControlData(
      const smart_objects::SmartObject& module_data,
      const std::string& module_type) = 0;

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
      ModuleCapability& module_data_capabilities) = 0;

  /**
   * @brief AreAllParamsReadOnly checks that all params from module
   * data are read only
   * @param module_data smart object of module_data
   * @param module_type resource name
   * @return true if all params are read only, otherwise - false
   */
  virtual bool AreAllParamsReadOnly(
      const smart_objects::SmartObject& module_data,
      const std::string& module_type) = 0;

  virtual ~RCCapabilitiesManager() {}
};

}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_CAPABILITIES_MANAGER_H_

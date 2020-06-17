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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_HELPERS_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_HELPERS_H_

#include <map>
#include <string>
#include "application_manager/application.h"
#include "rc_rpc_plugin/rc_app_extension.h"
#include "rc_rpc_plugin/rc_rpc_types.h"

namespace rc_rpc_plugin {
class RCRPCPlugin;

/**
 * @brief The RCHelpers class contains frequently used static data
 * structures related strictly to RC
 * Converters, mapping, factory functions
 */
class RCHelpers {
 public:
  /**
   * @brief GetModuleTypeToDataMapping get mapping of module type enum naming to
   * actual module data filed name
   * @return module mapping from enum naming to filed name
   */
  static const std::function<std::string(const std::string& module_type)>
  GetModuleTypeToDataMapping();

  /**
   * @brief GetModuleTypeToDataMapping get mapping of module type enum naming to
   * actual module capabilities key
   * @return module mapping from enum naming to capabilities key
   */
  static const std::function<std::string(const std::string& module_type)>
  GetModuleTypeToCapabilitiesMapping();

  /**
   * @brief GetModuleTypesList get list of all known module types
   * @return vector contains all known module types
   */
  static const std::vector<std::string> GetModuleTypesList();

  /**
   * @brief GetRCExtension extract RC extension from application
   * @param app application to extract extension
   * @return rc extension of app is rc applicaiton, otherwise return emty shared
   * pointer.
   */
  static RCAppExtensionPtr GetRCExtension(
      application_manager::Application& app);

  static smart_objects::SmartObjectSPtr CreateUnsubscribeRequestToHMI(
      const ModuleUid& module, const uint32_t correlation_id);

  static std::vector<application_manager::ApplicationSharedPtr>
  AppsSubscribedToModule(application_manager::ApplicationManager& app_mngr,
                         const ModuleUid& module);

  static std::vector<application_manager::ApplicationSharedPtr>
  AppsSubscribedToModuleType(application_manager::ApplicationManager& app_mngr,
                             const std::string& module_type);

  typedef std::map<application_manager::ApplicationSharedPtr,
                   std::vector<std::string> >
      AppsModuleTypes;

  static AppsModuleTypes GetApplicationsAllowedModuleTypes(
      application_manager::ApplicationManager& app_mngr);

  typedef std::map<std::string, mobile_apis::ButtonName::eType> ButtonsMap;

  static const std::vector<std::string> buttons_climate();

  static const std::vector<std::string> buttons_radio();

  static const ButtonsMap buttons_map();

  static std::vector<std::string> GetModuleReadOnlyParams(
      const std::string& module_type);

  /**
   * @brief Combines module ids and alloweds for these ids and fills vector with
   * ModuleConsents
   * @param module_type Module type as string
   * @param module_ids Modules ids which needed consents from driver
   * @param allowed Consents for modules
   */
  static rc_rpc_types::ModuleIdConsentVector FillModuleConsents(
      const std::string& module_type,
      const std::vector<std::string>& module_ids,
      const std::vector<bool> allowed);

  /**
   * @brief Retrieves module ids from SmartObject.
   * @param moduleIds Smartobject which contains collection of module ids
   * @return Collection of module ids
   */
  static std::vector<std::string> RetrieveModuleIds(
      const smart_objects::SmartObject& moduleIds);

  /**
   * @brief Retrieves module ids consents from SmartObject.
   * @param moduleIds Smartobject which contains collection of module ids
   * consents
   * @return Collection of module ids consents.
   */
  static std::vector<bool> RetrieveModuleConsents(
      const smart_objects::SmartObject& consents);

  /**
   * @brief RemoveRedundantGPSDataFromVIDataMsg removes redundant GPS data
   * params from interior vehicle data response message if one contains radio
   * station location data
   * @param msg_params Params of the interior vehicle data response message
   */
  static void RemoveRedundantGPSDataFromIVDataMsg(
      smart_objects::SmartObject& msg_params);

  /**
   * @brief MergeModuleData all keys and values from first parameter and
   * update and append keys and values from the second
   * @param data1 - initial data
   * @param data2 - updated data
   * @return updated data1 with data2 keys and values
   */
  static smart_objects::SmartObject MergeModuleData(
      const smart_objects::SmartObject& data1,
      const smart_objects::SmartObject& data2);

  /**
   * @brief MergeArray merge two arrays if their elements contain an `id`
   * parameter
   * @param data1 - initial data
   * @param data2 - updated data
   * @return updated data1 with any values in data2 if the arrays can be merged,
   * otherwise data2
   */
  static smart_objects::SmartObject MergeArray(
      const smart_objects::SmartObject& data1,
      const smart_objects::SmartObject& data2);
};

}  // namespace rc_rpc_plugin
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_RC_HELPERS_H_

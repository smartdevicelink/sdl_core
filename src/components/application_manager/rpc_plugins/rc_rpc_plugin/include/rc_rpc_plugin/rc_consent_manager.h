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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_CONSENT_MANAGER_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_CONSENT_MANAGER_H

#include <string>

#include "rc_rpc_types.h"

namespace ns_smart_device_link {
namespace ns_smart_objects {
class SmartObject;
}  // namespace ns_smart_objects
}  // namespace ns_smart_device_link

namespace rc_rpc_plugin {

class RCConsentManager {
 public:
  /**
   * @brief Saves consents (is_consented + date_of_consent) for module resources
   * (module_type + module_id) for specified application
   * @param policy_app_id Application id which module consents should be saved
   * @param mac_address Device mac address
   * @param module_consents - Module resources consents
   */
  virtual void SaveModuleConsents(
      const std::string& policy_app_id,
      const std::string& mac_address,
      const rc_rpc_types::ModuleIdConsentVector& module_consents) = 0;

  /**
   * @brief Retrieves saved consent for specified module resource (module_type
   * + module_id)
   * @param app_id Application which related to specified module resource
   * @param mac_address Device mac address
   * @param module_id Module resource (module_type + module_id)
   * @return Module consent state
   */
  virtual rc_rpc_plugin::rc_rpc_types::ModuleConsent GetModuleConsent(
      const std::string& app_id,
      const std::string& mac_address,
      const rc_rpc_types::ModuleUid& module_id) const = 0;

  /**
   * @brief Remove modules consents from LastState if they exist
   */
  virtual void RemoveAllConsents() = 0;

  /**
   * @brief Remove all expired module consents from LastState
   * if they exist
   */
  virtual void RemoveExpiredConsents() = 0;

  virtual ~RCConsentManager() {}
};

}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_CONSENT_MANAGER_H

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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_CONSENT_MANAGER_IMPL_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_CONSENT_MANAGER_IMPL_H

#include "rc_consent_manager.h"
#include "resumption/last_state.h"
#include "utils/lock.h"

namespace Json {
class Value;
}  // namespace Json

namespace rc_rpc_plugin {

class RCConsentManagerImpl : public RCConsentManager {
 public:
  RCConsentManagerImpl(
      resumption::LastState& last_state,
      application_manager::ApplicationManager& application_manager,
      const uint32_t period_of_consent_expired);

  void SaveModuleConsents(
      const std::string& policy_app_id,
      const std::string& mac_address,
      const rc_rpc_types::ModuleIdConsentVector& module_consents) OVERRIDE;

  rc_rpc_types::ModuleConsent GetModuleConsent(
      const std::string& app_id,
      const std::string& mac_address,
      const rc_rpc_types::ModuleUid& module_id) const OVERRIDE;

  void RemoveExpiredConsents() OVERRIDE;

  void RemoveAllConsents() OVERRIDE;

  ~RCConsentManagerImpl() OVERRIDE;

 private:
  /**
   * @brief Saves module consents for specified application
   * @param app_module_consents Application consents for which new
   * consents should be added or existing ones should be overwritten
   * @param consent_to_save module consents which should be saved
   */
  void SaveAppModuleConsent(
      Json::Value& app_module_consents,
      const rc_rpc_types::ModuleIdConsent& consent_to_save);

  /**
   * @brief Checks if module consent is expired
   * @param module_consent Module consent
   * @return ModuleConsentState
   */
  rc_rpc_types::ModuleConsentState CheckModuleConsentState(
      const Json::Value& module_consent) const;

  /**
   * @brief Removes devices expired consents
   * @param device Device section in AppConsensts section of LastState in Json
   */
  void RemoveDeviceExpiredConsents(Json::Value& device);

  /**
   * @brief Remove expired module consents which are related to specified
   * application
   * @param app_consents Application consents
   */
  void RemoveAppExpiredConsents(Json::Value& app_consents);

  /**
   * @brief Remove expired module consents which are related to specified
   * module type
   * @param module_consents Module consents wich should be checked for expired
   * consents
   */
  void RemoveModuleExpiredConsents(Json::Value& module_consents);

  /**
   * @brief Get Remote Control section of LastState.
   * In case the section is absent, will be appended a new empty section.
   * @return Remote Control section of LastState in Json
   */
  Json::Value& GetRemoteControlDataOrAppend() const;

  /**
   * @brief Gets Device applications section for specified device mac adress
   * In case the section is absent, will be appended a new empty section.
   * @param mac_aress Device MAC adress
   * @return Device applications section of LastState in Json
   */
  Json::Value& GetDeviceApplicationsOrAppend(
      const std::string& mac_address) const;

  /**
   * @brief Get AppConsentsList section of LastState for specified application.
   * In case the consent list is absent, will be appended a new empty section.
   * @param policy_app_id Application policy ID
   * @param mac_address Device MAC address
   * @return AppConsentsList of LastState in Json
   */
  Json::Value& GetAppConsentsListOrAppend(const std::string& policy_app_id,
                                          const std::string& mac_address) const;

  /**
   * @brief Get Application consents section of Remote Control section of
   * LastState with all consents for all applications. In case if this section
   * is absent, will be created a new empty section
   * @return AppConsents section of RemoteControl section of LastState in Jason
   */
  Json::Value& GetAppsConsentsOrAppend() const;

  /**
   * @brief Get all module resource consents for specified application and
   * module type. In case if section with specified module type consents is
   * absent, will be created a new empty section
   * @param policy_app_id Application id which contains specified module type
   * @param mac_adress MAC address of mobile device that needs user consent for
   * acquiring resource
   * @param module_type Module type with consents
   */
  Json::Value& GetModuleTypeConsentsOrAppend(
      const std::string& policy_app_id,
      const std::string& mac_address,
      const std::string& module_type) const;

 private:
  application_manager::ApplicationManager& app_manager_;
  resumption::LastState& last_state_;
  const uint32_t period_of_consent_expired_;
  mutable sync_primitives::Lock dictionary_control_lock_;
  mutable sync_primitives::Lock remote_control_lock_;
  mutable sync_primitives::Lock device_applications_lock_;
  mutable sync_primitives::Lock applications_lock_;
  mutable sync_primitives::Lock app_consents_lock_;
  mutable sync_primitives::Lock module_consents_lock_;
};

}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_CONSENT_MANAGER_IMPL_H

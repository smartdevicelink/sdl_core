/*
 Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_PT_EXT_REPRESENTATION_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_PT_EXT_REPRESENTATION_H_

#include "policy/pt_representation.h"

namespace policy {

enum StatisticsType {
  S_NONE = 0,
  S_IAP_BUFFER_FULL,
  S_SYNC_OUT_OF_MEMORY,
  S_SYNC_REBOOTS,
  S_MINS_HMI_FULL,
  S_MINS_HMI_LIMITED,
  S_MINS_HMI_BACKGROUND,
  S_MINS_HMI_NONE,
  S_RFCOM_LIMIT_REACHED,
  S_USER_SELECTIONS,
  S_REJECTIONS_SYNC_OUT_OF_MEMORY,
  S_REJECTIONS_NICKNAME_MISMATCH,
  S_REJECTIONS_DUPLICATE_NAME,
  S_REJECTED_RPC_CALLS,
  S_RPCS_IN_HMI_NONE,
  S_REMOVALS_MISBEHAVED,
  S_RUN_ATTEMPTS_WHILE_REVOKED
};

enum LanguageType {
  L_NONE = 0,
  L_GUI,
  L_VUI
};

class PTExtRepresentation : public virtual PTRepresentation {
  public:
    virtual ~PTExtRepresentation() {
    }

    /**
     * @brief Is application allowed to send notifications while in
     * Backgound or limited mode.
     * @param app_id Application id
     * @return bool Allowed/disallowed.
     */
    virtual bool CanAppKeepContext(const std::string& app_id) = 0;

    /**
     * @brief Is application allowed to move foreground at will?
     * @param app_id Application id
     * @return bool Allowed/disallowed.
     */
    virtual bool CanAppStealFocus(const std::string& app_id) = 0;

    /**
     * @brief Get default_hmi for given application
     * @param policy_app_id Unique application id
     * @param default_hmi Default HMI level for application or empty, if value was
     * not set
     * @return true, if succedeed, otherwise - false
     */
    virtual bool GetDefaultHMI(const std::string& policy_app_id,
                               std::string* default_hmi) = 0;

    /**
     * @brief Reset user consent for device data and applications permissions
     * @return
     */
    virtual bool ResetUserConsent() = 0;

    /**
     * @brief Reset user consent for device data
     * @return
     */
    virtual bool ResetDeviceConsents() = 0;

    /**
     * @brief Reset user consent for applications permissions
     * @return
     */
    virtual bool ResetAppConsents() = 0;

    /**
     * @brief Get user permissions for device data usage
     * @param device_id Generated or obtained id of device
     * @param consented_groups Groups consented by user
     * @param disallowed_groups Groups not consented by user
     * @return true, if query was successfull, otherwise - false
     */
    virtual bool GetUserPermissionsForDevice(
      const std::string& device_id, StringArray* consented_groups = NULL,
      StringArray* disallowed_groups = NULL) = 0;

    /**
     * @brief Gets list of groups permissions from policy table
     * @param device_id Unique device id, which hosts specific application
     * @param policy_app_id Unique application id
     * @param group_types Group list sorted by permission status
     * @return true, if query was successfull, otherwise - false
     */
    virtual bool GetPermissionsForApp(
      const std::string& device_id,
      const std::string& policy_app_id,
      FunctionalIdType* group_types) = 0;

    /**
     * @brief Get device groups and preconsented groups from policies section
     * @param groups List of groups to be consented for device usage
     * @param preconsented_groups List of preconsented groups for device usage
     * @return true, if query was successful, otherwise - false
     */
    virtual bool GetDeviceGroupsFromPolicies(
      policy_table::Strings* groups = NULL,
      policy_table::Strings* preconsented_groups = NULL) = 0;

    /**
     * @brief Record information about mobile device in Policy Table.
     * @param device_id Generated or obtained id of device
     * @return bool Success of operation
     */
    virtual bool SetDeviceData(const std::string& device_id,
                               const std::string& hardware = "",
                               const std::string& firmware = "",
                               const std::string& os = "",
                               const std::string& os_version = "",
                               const std::string& carrier = "",
                               const uint32_t number_of_ports = 0,
                               const std::string& connection_type = "") = 0;

    /**
     * @brief Sets user consent for particular mobile device,
     * i.e. to use device for exchanging of Policy Table.
     * @return bool Success of operation
     */
    virtual bool SetUserPermissionsForDevice(
      const std::string& device_id, const StringArray& consented_groups =
        StringArray(),
      const StringArray& disallowed_gropus = StringArray()) = 0;

    /**
     * @brief Update Application Policies as reaction
     * on User allowing/disallowing device this app is running on.
     */
    virtual bool ReactOnUserDevConsentForApp(
      const std::string& app_id,
      bool is_device_allowed) = 0;

    /**
     * @brief Set user consent on functional groups
     * @param permissions User consent on functional group
     * @return true, if operation succedeed, otherwise - false
     */
    virtual bool SetUserPermissionsForApp(
      const PermissionConsent& permissions) = 0;

    /**
     * @brief Counter for statistics information: adds 1 to existing number.
     * @param type Type of statistics (errors, mins in mode etc)
     * @return bool Success of operation
     */
    virtual bool IncreaseStatisticsData(StatisticsType type) = 0;

    /**
     * @brief Records information about what language
     * application tried to register with.
     * @param app_id Id of application
     * @param type - language for UI/VR
     * @param language Language
     * @return bool Success of operation
     */
    virtual bool SetAppRegistrationLanguage(const std::string& app_id,
                                            LanguageType type,
                                            const std::string& language) = 0;

    /**
     * @brief Records information about head unit system to PT
     * @return bool Success of operation
     */
    virtual bool SetMetaInfo(const std::string& ccpu_version,
                             const std::string& wers_country_code,
                             const std::string& language) = 0;

    /**
     * @brief Checks, if specific head unit is present in PT
     * @return boot Suceess, if present, otherwise - false
     */
    virtual bool IsMetaInfoPresent() = 0;

    /**
     * @brief Kms pass since last successfull PT update
     */
    virtual int GetKmFromSuccessfulExchange() = 0;

    /**
     * @brief Days pass since last successfull PT update
     */
    virtual int GetDayFromScsExchange() = 0;

    /**
     * @brief Ignition cycles pass since last successfull PT update
     */
    virtual int GetIgnitionsFromScsExchange() = 0;

    /**
     * @brief Set current system language
     * @param language System language
     * @return true, if succedeed, otherwise - false
     */
    virtual bool SetSystemLanguage(const std::string& language) = 0;

    /**
     * Increments global counter
     * @param type type of counter
     */
    virtual void Increment(const std::string& type) const = 0;

    /**
     * Increments counter of application
     * @param app_id id application
     * @param type type of counter
     */
    virtual void Increment(const std::string& app_id,
                           const std::string& type) const = 0;

    /**
     * Sets value of application information
     * @param app_id id application
     * @param type type of information
     * @param value value of information
     */
    virtual void Set(const std::string& app_id, const std::string& type,
                     const std::string& value) const = 0;

    /**
     * Adds value to stopwatch of application
     * @param app_id id application
     * @param type type of stopwatch
     * @param seconds value for adding in seconds
     */
    virtual void Add(const std::string& app_id, const std::string& type,
                     int seconds) const = 0;

    virtual bool CountUnconsentedGroups(const std::string& policy_app_id,
                                        const std::string& device_id,
                                        int* result) const = 0;

    /**
     * @brief Gets functional group names and user_consent_prompts, if any
     * @param Array to be filled with group ids, names and functional prompts
     * @return true, if succeeded, otherwise - false
     */
    // TODO(AOleynik): Possibly, we can get rid of this method. Check this.
    virtual bool GetFunctionalGroupNames(policy::FunctionalGroupNames& names) = 0;

    /**
         * @brief Set app policy to pre_DataConsented policy
         * @param app_id Policy ID of application to be changed
         * @return true, if succeeded, otherwise - false
         */
    virtual bool SetPredataPolicy(const std::string& app_id) = 0;

    /**
     * @brief Updates application policy to either pre_DataConsented or not
     * @param app_id Policy Id of application to be checked
     * @param is_pre_data True of False to setting app policy to be pre_DataConsented
     * @return true, if succeeded, otherwise - false
     */
    virtual bool SetIsPredata(const std::string& app_id, bool is_pre_data) = 0;

    /**
     * @brief Removes unpaired devices
     * @return true if success
     */
    virtual bool CleanupUnpairedDevices(const DeviceIds& device_ids) const = 0;

    /**
     * Sets flag of unpaired device
     * @param device_id Unique device id
     * @param unpaired True, if unpaired, otherwise - false
     * @return true if success
     */
    virtual bool SetUnpairedDevice(const std::string& device_id,
                                   bool unpaired) const = 0;

    /**
     * Gets list of unpaired devices
     * @param device_ids output list
     * @return true if success
     */
    virtual bool UnpairedDevicesList(DeviceIds* device_ids) const = 0;

    /**
     * @brief Remove application consent for particular group
     * @param policy_app_id Unique application id
     * @param functional_group_name Functional group name, which consents should
     * be removed
     * @return true, in case of success, otherwise - false
     */
    virtual bool RemoveAppConsentForGroup(
      const std::string& policy_app_id,
      const std::string& functional_group_name) const = 0;
};
}  //  namespace policy

#endif  //  SRC_COMPONENTS_POLICY_INCLUDE_POLICY_PT_EXT_REPRESENTATION_H_

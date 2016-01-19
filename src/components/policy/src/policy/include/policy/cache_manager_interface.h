/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_CACHE_MANAGER_INTERFACE_H_
#define SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_CACHE_MANAGER_INTERFACE_H_

#include <string>
#include <vector>

#include "utils/shared_ptr.h"
#include "usage_statistics/counter.h"
#include "policy/policy_types.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {

class CacheManagerInterface {
 public:
  virtual ~CacheManagerInterface() {
  }

  /**
   * @brief Check if specified RPC for specified application
   * has permission to be executed in specified HMI Level
   * and also its permitted params.
   * @param app_id Id of application provided during registration
   * @param hmi_level Current HMI Level of application
   * @param rpc Name of RPC
   * @return CheckPermissionResult containing flag if HMI Level is allowed
   * and list of allowed params.
   */
  virtual void CheckPermissions(const PTString& app_id,
                                const PTString& hmi_level, const PTString& rpc,
                                CheckPermissionResult& result) = 0;

  /**
   * @brief Returns true if Policy Table was not updated yet
   * from preloaded pt file.
   */
  virtual bool IsPTPreloaded() = 0;

  /**
   * Gets number of ignition cycles before next update policy table
   * @return number of ignition cycles
   */
  virtual int IgnitionCyclesBeforeExchange() = 0;

  /**
   * Gets value in kilometers before next update policy table
   * @param current value in kilometers from the odometers
   * @return value in kilometers
   */
  virtual int KilometersBeforeExchange(int current) = 0;

  /**
   * @brief Sets counter value that passed for recieved successful PT UPdate
   */
  virtual bool SetCountersPassedForSuccessfulUpdate(Counters counter,
                                                    int value) = 0;

  /**
   * Gets value in days before next update policy table
   * @param current value in days after epoch
   * @return value in days
   */
  virtual int DaysBeforeExchange(int current) = 0;

  /**
   * @brief Increment number of ignition cycles since last exchange by 1
   */
  virtual void IncrementIgnitionCycles() = 0;

  /**
   * @brief Reset number of ignition cycles since last exchange to 0
   */
  virtual void ResetIgnitionCycles() = 0;

  /**
   * @brief Returns timeout to wait for a response of PT update
   * @return value in seconds
   */
  virtual int TimeoutResponse() = 0;

  /**
   * @brief Returns number of seconds between each try of sending PTS
   * @param seconds Return value: array of 5 elements
   * @return bool Success of operation
   */
  virtual bool SecondsBetweenRetries(std::vector<int> &seconds) = 0;

  /**
   * @brief Get information about vehicle
   */
  virtual const VehicleInfo GetVehicleInfo() const = 0;

  /**
   * @brief Allows to update 'vin' field in module_meta table.
   *
   * @param new 'vin' value.
   *
   * @return true in case when data has been successfully updated,
   * false otherwise.
   */
  virtual bool SetVINValue(const std::string& value) = 0;

  /**
   * @brief Get message text for displaying/pronouncing for user
   * dependent on language and context.
   * @param msg_codes Context of message (Driver distraction, Grant permission etc)
   * @param language Language of the message
   * @return Array of appropriate messages parameters
   */
  virtual std::vector<UserFriendlyMessage> GetUserFriendlyMsg(
      const std::vector<std::string>& msg_codes,
      const std::string& language) = 0;

  /**
   * @brief Get list of URLs related to particular service
   * @param service_type If URLs for specific service are preset,
   * return them otherwise default URLs.
   */
  virtual void GetServiceUrls(const std::string& service_type,
                              EndpointUrls& end_points) = 0;

  /**
   * @brief GetLockScreenIcon allows to obtain lock screen icon url;
   *
   * @return url which point to the resourse where lock screen icon could be obtained.
   */
  virtual std::string GetLockScreenIconUrl() const = 0;

  /**
   * @brief Get allowed number of notifications
   * depending on application priority.
   * @param priority Priority of application
   */
  virtual policy_table::NumberOfNotificationsType GetNotificationsNumber(const std::string& priority) = 0;

  /**
   * @brief Get priority for given application
   * @param policy_app_id Unique application id
   * @param priority Priority for application or empty, if value was not set
   * @return true, if succedeed, otherwise - false
   */
  virtual bool GetPriority(const std::string& policy_app_id,
                           std::string& priority) = 0;

  /**
   * @brief Initialized Policy Table (load)
   * @return bool Success of operation
   */
  virtual bool Init(const std::string& file_name) = 0;

  /**
   * @brief Get snapshot of Policy Table
   * including app_policies, functional_groups,
   * device_info, statistics, excluding user messages
   * @return Generated structure for obtaining Json string.
   */
  virtual utils::SharedPtr<policy_table::Table> GenerateSnapshot() = 0;

  /**
   * Applies policy table to the current table
   * @param update_pt policy table
   * @return true if successfully
   */
  virtual bool ApplyUpdate(const policy_table::Table& update_pt) = 0;

  /**
   * @brief Gets list of appHMIType associated with mobile appID
   * @param container of appHMIType
   */
  virtual void GetHMIAppTypeAfterUpdate(std::map<std::string, StringArray>& app_hmi_types) = 0;

  /**
   * Gets flag updateRequired
   * @return true if update is required
   */
  virtual bool UpdateRequired() const = 0;

  /**
   * @brief Saves flag updateRequired
   * @param status update status if true then update required.
   */
  virtual void SaveUpdateRequired(bool status) = 0;

  /**
   * @brief GetInitialAppData Retrieves data from app_policies
   * about app on its registration
   * @param app_id id of registered app.
   * All outputs are filled in only if not null
   * @param nicknames Synonyms for application
   * @param app_hmi_types app_types Section on HMI where app can
   * appear (Navigation, Phone etc)
   * @return true in case initial application data was obtained successfuly.
   */
  virtual bool GetInitialAppData(const std::string& app_id,
                                 StringArray& nicknames,
                                 StringArray& app_hmi_types) = 0;

  /**
   * Checks if the application is revoked
   * @param app_id application id
   * @return true if application is revoked
   */
  virtual bool IsApplicationRevoked(const std::string& app_id) const = 0;

  /**
   * @brief Get functional groupings from DB
   * @param groups Known functional groupings
   * @return true, if succeeded, otherwise - false
   */
  virtual bool GetFunctionalGroupings(
      policy_table::FunctionalGroupings& groups) = 0;

  /**
   * Checks if the application is represented in policy table
   * @param app_id application id
   * @return true if application is represented in policy table
   */
  virtual bool IsApplicationRepresented(const std::string& app_id) const = 0;

  /**
   * Checks if the application has default policy
   * @param app_id application id
   * @return true if application has default policy
   */
  virtual bool IsDefaultPolicy(const std::string& app_id) = 0;

  /**
   * @brief SetIsDefault Sets is_default flag for application
   * @param app_id app specific application
   * @return  true in case opperation was done successfully.
   */
  virtual bool SetIsDefault(const std::string& app_id) = 0;

  /**
   * Checks if the application has pre_data policy
   * @param app_id application id
   * @return true if application has pre_data policy
   */
  virtual bool IsPredataPolicy(const std::string& app_id) = 0;

  /**
   * Sets default policy for application
   * @param app_id application id
   * @return true if success
   */
  virtual bool SetDefaultPolicy(const std::string& app_id) = 0;

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
  virtual bool GetDefaultHMI(const std::string& app_id,
                             std::string &default_hmi) = 0;

  /**
   * @brief Reset user consent for device data and applications permissions
   * @return
   */
  virtual bool ResetUserConsent() = 0;

  /**
   * @brief Get user permissions for device data usage
   * @param device_id Generated or obtained id of device
   * @param consented_groups Groups consented by user
   * @param disallowed_groups Groups not consented by user
   * @return true, if query was successfull, otherwise - false
   */
  virtual bool GetUserPermissionsForDevice(const std::string& device_id,
                                           StringArray &consented_groups,
                                           StringArray &disallowed_groups) = 0;

  /**
   * @brief Gets list of groups permissions from policy table
   * @param device_id Unique device id, which hosts specific application
   * @param policy_app_id Unique application id
   * @param group_types Group list sorted by permission status
   * @return true, if query was successfull, otherwise - false
   */
  virtual bool GetPermissionsForApp(const std::string& device_id,
                                    const std::string& app_id,
                                    FunctionalIdType &group_types) = 0;

  /**
   * @brief Get device groups and preconsented groups from policies section
   * @param groups List of groups to be consented for device usage
   * @param preconsented_groups List of preconsented groups for device usage
   * @return true, if query was successful, otherwise - false
   */
  virtual bool GetDeviceGroupsFromPolicies(
      rpc::policy_table_interface_base::Strings &groups,
      rpc::policy_table_interface_base::Strings &preconsented_groups) = 0;

  /**
   * @brief Add's information about mobile device in Policy Table.
   * @param device_id Generated or obtained id of device
   * @param connection_type device connection type
   * @return bool Success of operation
   */
  virtual bool AddDevice(const std::string& device_id,
                         const std::string& connection_type) = 0;

  /**
   * @brief Record information about mobile device in Policy Table.
   * @param device_id Generated or obtained id of device
   * @return bool Success of operation
   */
  virtual bool SetDeviceData(const std::string& device_id,
                             const std::string& hardware,
                             const std::string& firmware, const std::string& os,
                             const std::string& os_version,
                             const std::string& carrier,
                             const uint32_t number_of_ports,
                             const std::string& connection_type) = 0;

  /**
   * @brief Sets user consent for particular mobile device,
   * i.e. to use device for exchanging of Policy Table.
   * @return bool Success of operation
   */
  virtual bool SetUserPermissionsForDevice(
      const std::string& device_id, const StringArray& consented_groups,
      const StringArray& disallowed_groups) = 0;

  /**
   * @brief Update Application Policies as reaction
   * on User allowing/disallowing device this app is running on.
   */
  virtual bool ReactOnUserDevConsentForApp(const std::string& app_id,
                                           bool is_device_allowed) = 0;

  /**
   * @brief Set user consent on functional groups
   * @param permissions User consent on functional group
   * @return true, if operation succedeed, otherwise - false
   */
  virtual bool SetUserPermissionsForApp(
      const PermissionConsent& permissions) = 0;

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
  virtual bool IsMetaInfoPresent() const = 0;

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
  virtual void Increment(usage_statistics::GlobalCounterId type) = 0;

  /**
   * Increments counter of application
   * @param app_id id application
   * @param type type of counter
   */
  virtual void Increment(const std::string& app_id,
                         usage_statistics::AppCounterId type) = 0;

  /**
   * Sets value of application information
   * @param app_id id application
   * @param type type of information
   * @param value value of information
   */
  virtual void Set(const std::string& app_id, usage_statistics::AppInfoId type,
                   const std::string& value) = 0;

  /**
   * Adds value to stopwatch of application
   * @param app_id id application
   * @param type type of stopwatch
   * @param seconds value for adding in seconds
   */
  virtual void Add(const std::string& app_id,
                   usage_statistics::AppStopwatchId type, int seconds) = 0;

  /**
   * @brief CountUnconsentedGroups allows to obtain the count of unconsented
   * groups for specific application.
   * @param policy_app_id application id.
   * @param device_id device id.
   * @param the count of unconsented groups
   */
  virtual int CountUnconsentedGroups(const std::string& policy_app_id,
                                      const std::string& device_id) = 0;

  /**
   * @brief Gets functional group names and user_consent_prompts, if any
   * @param Array to be filled with group ids, names and functional prompts
   * @return true, if succeeded, otherwise - false
   */
  virtual bool GetFunctionalGroupNames(FunctionalGroupNames& names) = 0;

  /**
   * @brief GetAllAppGroups allows to obtain all groups for certain application.
   * @param app_id specific application id.
   * @param all_group_ids parameter to fill.
   */
  virtual void GetAllAppGroups(const std::string& app_id,
                               FunctionalGroupIDs& all_group_ids) = 0;
  /**
   * @brief GetPreConsentedGroups allows to obtain all pre-consented groups for
   * specific application.
   * @param app_id specific application id.
   * @param preconsented_groups parameter to fill.
   */
  virtual void GetPreConsentedGroups(
      const std::string &app_id, FunctionalGroupIDs& preconsented_groups) = 0;
  /**
   * @brief GetConsentedGroups allows to obtain list of allowed and disallowed
   * groups for specific application on certain device.
   * @param device_id certain device
   * @param app_id application id.
   * @param allowed_groups list of allowed groups
   * @param disallowed_groups list of disallowed groups
   */
  virtual void GetConsentedGroups(const std::string &device_id,
                                  const std::string &app_id,
                                  FunctionalGroupIDs& allowed_groups,
                                  FunctionalGroupIDs& disallowed_groups) = 0;

  /**
   * @brief GetUnconsentedGroups allows to obtain list of allowed and disallowed
   * groups for specific application on certain device.
   * @param device_id certain device
   * @param policy_app_id application id.
   * @param unconsented_groups list of unconsented groups.
   */
  virtual void GetUnconsentedGroups(const std::string& device_id,
                                    const std::string& policy_app_id,
                                    FunctionalGroupIDs& unconsented_groups) = 0;

  virtual void RemoveAppConsentForGroup(const std::string& app_id,
                                        const std::string& group_name) = 0;

  /**
   * @brief Set app policy to pre_DataConsented policy
   * @param app_id Policy ID of application to be changed
   * @return true, if succeeded, otherwise - false
   */
  virtual bool SetPredataPolicy(const std::string& app_id) = 0;

  /**
   * @brief Removes unpaired devices
   * @return true if success
   */
  virtual bool CleanupUnpairedDevices() = 0;

  /**
   * Sets flag of unpaired device
   * @param device_id Unique device id
   * @param unpaired True, if should be marked as unpaired, otherwise - false
   * @return true if success
   */
  virtual bool SetUnpairedDevice(const std::string& device_id, bool unpaired = true) = 0;

  /**
   * Resets Policy Table
   * @param file_name Path to preloaded PT file
   * @return true if successfully
   */
  virtual bool ResetPT(const std::string& file_name) = 0;

  /**
   * @brief LoadFromBackup allows to load policy into the cache from backup.
   * @return true in case operation was successful.
   */
  virtual bool LoadFromBackup() = 0;

  /**
   * @brief LoadFromFile allows to load policy cache from preloaded table.
   * @param file_name preloaded
   * @param table object which will be filled during file parsing.
   * @return true in case file was successfuly loaded, false otherwise.
   */
  virtual bool LoadFromFile(const std::string& file_name, policy_table::Table& table) = 0;

  /**
   * @brief Backup allows to save cache onto hard drive.
   */
  virtual void Backup() = 0;

  /**
   * Returns heart beat timeout
   * @param app_id application id
   * @return if timeout was set then value in milliseconds greater zero
   * otherwise heart beat for specific application isn't set
   */
  virtual uint32_t HeartBeatTimeout(const std::string& app_id) const = 0;

  /**
   * @brief Resets all calculated permissions in cache
   */
  virtual void ResetCalculatedPermissions() = 0;

  /**
   * @brief Adds calculated permissions for specific app on particular device
   * into cache
   * @param device_id Device id
   * @param policy_app_id Application id
   * @param permissions Calculated permissions
   */
  virtual void AddCalculatedPermissions(
      const std::string& device_id,
      const std::string& policy_app_id,
      const policy::Permissions& permissions) = 0;

  /**
   * @brief Checks if permissions calculated for specific app on particular
   * device
   * @param device_id Device id
   * @param policy_app_id Application id
   * @param permission Permissions to be filled, in case of presence in cache
   * @return true if present, otherwise false
   */
  virtual bool IsPermissionsCalculated(
      const std::string& device_id,
      const std::string& policy_app_id,
      policy::Permissions& permission) = 0;

  /**
   * @brief Gets request types for application
   * @param policy_app_id Unique application id
   * @param request_types Request types of application
   */
  virtual void GetAppRequestTypes(
      const std::string& policy_app_id,
      std::vector<std::string>& request_types) const = 0;

    /**
     * @brief GetCertificate allows to obtain certificate in order to
     * make secure connection
     *
     * @return The certificate in PKCS#7.
     */
    virtual std::string GetCertificate() const = 0;

#ifdef BUILD_TESTS
  /**
   * @brief GetPT allows to obtain SharedPtr to PT.
   * Used ONLY in Unit tests
   * @return SharedPTR to PT
   *
   */
  virtual utils::SharedPtr<policy_table::Table> GetPT() const  = 0;
#endif
};

typedef utils::SharedPtr<CacheManagerInterface> CacheManagerInterfaceSPtr;

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_SRC_POLICY_INCLUDE_POLICY_CACHE_MANAGER_INTERFACE_H_

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

#ifndef SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_CACHE_MANAGER_H_
#define SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_CACHE_MANAGER_H_

#include <map>

#include "utils/shared_ptr.h"
#include "policy/pt_representation.h"
#include "policy/pt_ext_representation.h"
#include "policy/usage_statistics/statistics_manager.h"
#include "policy/cache_manager_interface.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

#include "utils/lock.h"
#include "utils/conditional_variable.h"
#include "policy/policy_types.h"

namespace policy {
class PolicySettings;

class CacheManager : public CacheManagerInterface {
 public:
  CacheManager();
  ~CacheManager();

  const policy_table::Strings& GetGroups(const PTString& app_id);

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
  virtual void CheckPermissions(const policy_table::Strings& groups,
                                const PTString& hmi_level,
                                const PTString& rpc,
                                CheckPermissionResult& result);

  /**
   * @brief Returns true if Policy Table was not updated yet
   * from preloaded pt file.
   */
  virtual bool IsPTPreloaded();

  /**
   * Gets number of ignition cycles before next update policy table
   * @return number of ignition cycles
   */
  virtual int IgnitionCyclesBeforeExchange();

  /**
   * Gets value in kilometers before next update policy table
   * @param current value in kilometers from the odometers
   * @return value in kilometers
   */
  virtual int KilometersBeforeExchange(int current);

  /**
   * @brief Sets counter value that passed for recieved successful PT UPdate
   */
  virtual bool SetCountersPassedForSuccessfulUpdate(Counters counter,
                                                    int value);

  /**
   * Gets value in days before next update policy table
   * @param current value in days after epoch
   * @return value in days
   */
  virtual int DaysBeforeExchange(int current);

  /**
   * @brief Increment number of ignition cycles since last exchange by 1
   */
  virtual void IncrementIgnitionCycles();

  /**
   * @brief Reset number of ignition cycles since last exchange to 0
   */
  virtual void ResetIgnitionCycles();

  /**
   * @brief Returns timeout to wait for a response of PT update
   * @return value in msec
   */
  virtual int TimeoutResponse();

  /**
   * @brief Returns number of seconds between each try of sending PTS
   * @param seconds Return value: array of 5 elements
   * @return bool Success of operation
   */
  virtual bool SecondsBetweenRetries(std::vector<int>& seconds);

  /**
   * @brief Get information about vehicle
   */
  virtual const VehicleInfo GetVehicleInfo() const;

  /**
   * @brief Allows to update 'vin' field in module_meta table.
   *
   * @param new 'vin' value.
   *
   * @return true in case when data has been successfully updated,
   * false otherwise.
   */
  bool SetVINValue(const std::string& value);

  /**
   * @brief Get message text for displaying/pronouncing for user
   * dependent on language and context.
   * @param msg_codes Context of message (Driver distraction, Grant permission
   * etc)
   * @param language Language of the message
   * @return Array of appropriate messages parameters
   */
  std::vector<UserFriendlyMessage> GetUserFriendlyMsg(
      const std::vector<std::string>& msg_codes, const std::string& language);

  /**
   * @brief Get list of URLs related to particular service
   * @param service_type If URLs for specific service are preset,
   * return them otherwise default URLs.
   */
  virtual void GetUpdateUrls(const std::string& service_type,
                             EndpointUrls& out_end_points);

  virtual void GetUpdateUrls(const uint32_t service_type,
                             EndpointUrls& out_end_points);
  /**
   * @brief GetLockScreenIcon allows to obtain lock screen icon url;
   *
   * @return url which point to the resourse where lock screen icon could be
   *obtained.
   */
  virtual std::string GetLockScreenIconUrl() const;

  /**
   * @brief Get allowed number of notifications
   * depending on application priority.
   * @param priority Priority of application
   */
  virtual rpc::policy_table_interface_base::NumberOfNotificationsType
  GetNotificationsNumber(const std::string& priority);

  /**
   * @brief Get priority for given application
   * @param policy_app_id Unique application id
   * @param priority Priority for application or empty, if value was not set
   * @return true, if succedeed, otherwise - false
   */
  virtual bool GetPriority(const std::string& policy_app_id,
                           std::string& priority) const OVERRIDE;

  /**
   * @brief Initialized Policy Table (load)
   * @return bool Success of operation
   */
  bool Init(const std::string& file_name, const PolicySettings* settings);

  /**
   * @brief Get snapshot of Policy Table
   * including app_policies, functional_groups,
   * device_info, statistics, excluding user messages
   * @return Generated structure for obtaining Json string.
   */
  virtual utils::SharedPtr<policy_table::Table> GenerateSnapshot();

  /**
   * Applies policy table to the current table
   * @param update_pt policy table
   * @return true if successfully
   */
  bool ApplyUpdate(const policy_table::Table& update_pt);

  /**
   * @brief Gets list of appHMIType associated with mobile appID
   * @param container of appHMIType
   */
  virtual void GetHMIAppTypeAfterUpdate(
      std::map<std::string, StringArray>& app_hmi_types);

  /**
   * @brief AppHasHMIType checks whether app has been registered with certain
   *HMI type.
   *
   * @return true in case app contains certain HMI type, false otherwise.
   */
  virtual bool AppHasHMIType(const std::string& application_id,
                             policy_table::AppHMIType hmi_type) const OVERRIDE;

  /**
   * Gets flag updateRequired
   * @return true if update is required
   */
  bool UpdateRequired() const;

  /**
   * @brief Saves flag updateRequired
   * @param status update status if true then update required.
   */
  void SaveUpdateRequired(bool status);

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
  bool GetInitialAppData(const std::string& app_id,
                         StringArray& nicknames,
                         StringArray& app_hmi_types);

  /**
   * Checks if the application is revoked
   * @param app_id application id
   * @return true if application is revoked
   */
  bool IsApplicationRevoked(const std::string& app_id) const;

  /**
   * @brief Get functional groupings from DB
   * @param groups Known functional groupings
   * @return true, if succeeded, otherwise - false
   */
  bool GetFunctionalGroupings(policy_table::FunctionalGroupings& groups);

  /**
   * Checks if the application is represented in policy table
   * @param app_id application id
   * @return true if application is represented in policy table
   */
  bool IsApplicationRepresented(const std::string& app_id) const;

  /**
   * Checks if the application has default policy
   * @param app_id application id
   * @return true if application has default policy
   */
  bool IsDefaultPolicy(const std::string& app_id) const OVERRIDE;

  /**
   * @brief Sets decrypted certificate in policy table
   * @param certificate content of certificate
   */
  void SetDecryptedCertificate(const std::string& certificate) const OVERRIDE;

  /**
   * @brief SetIsDefault Sets is_default flag for application
   * @param app_id app specific application
   * @return  true in case opperation was done successfully.
   */
  bool SetIsDefault(const std::string& app_id);

  /**
   * Checks if the application has pre_data policy
   * @param app_id application id
   * @return true if application has pre_data policy
   */
  bool IsPredataPolicy(const std::string& app_id) const OVERRIDE;

  /**
   * Sets default policy for application
   * @param app_id application id
   * @return true if success
   */
  bool SetDefaultPolicy(const std::string& app_id);

  /**
   * @brief Is application allowed to send notifications while in
   * Backgound or limited mode.
   * @param app_id Application id
   * @return bool Allowed/disallowed.
   */
  bool CanAppKeepContext(const std::string& app_id) const OVERRIDE;

  /**
   * @brief Is application allowed to move foreground at will?
   * @param app_id Application id
   * @return bool Allowed/disallowed.
   */
  bool CanAppStealFocus(const std::string& app_id) const;

  /**
   * @brief Gets default_hmi for given application
   * @param policy_app_id Unique application id
   * @param default_hmi Default HMI level for application or empty, if value was
   * not set
   * @return true, if succedeed, otherwise - false
   */
  bool GetDefaultHMI(const std::string& app_id, std::string& default_hmi) const;

  /**
   * Gets HMI types from specific policy
   * @param app_id ID application
   * @return list of HMI types
   */
  const policy_table::AppHMITypes* GetHMITypes(const std::string& app_id);

  /**
   * @brief Reset user consent for device data and applications permissions
   * @return
   */
  bool ResetUserConsent();

  /**
   * @brief Get user permissions for device data usage
   * @param device_id Generated or obtained id of device
   * @param consented_groups Groups consented by user
   * @param disallowed_groups Groups not consented by user
   * @return true, if query was successfull, otherwise - false
   */
  bool GetUserPermissionsForDevice(const std::string& device_id,
                                   StringArray& consented_groups,
                                   StringArray& disallowed_groups) const;

  /**
   * @brief Gets list of groups permissions from policy table
   * @param device_id Unique device id, which hosts specific application
   * @param policy_app_id Unique application id
   * @param group_types Group list sorted by permission status
   * @return true, if query was successfull, otherwise - false
   */
  bool GetPermissionsForApp(const std::string& device_id,
                            const std::string& app_id,
                            FunctionalIdType& group_types);

  /**
   * @brief Get device groups and preconsented groups from policies section
   * @param groups List of groups to be consented for device usage
   * @param preconsented_groups List of preconsented groups for device usage
   * @return true, if query was successful, otherwise - false
   */
  bool GetDeviceGroupsFromPolicies(
      rpc::policy_table_interface_base::Strings& groups,
      rpc::policy_table_interface_base::Strings& preconsented_groups) const;

  /**
   * @brief Add's information about mobile device in Policy Table.
   * @param device_id Generated or obtained id of device
   * @param connection_type device connection type
   * @return bool Success of operation
   */
  bool AddDevice(const std::string& device_id,
                 const std::string& connection_type);

  /**
   * @brief Record information about mobile device in Policy Table.
   * @param device_id Generated or obtained id of device
   * @return bool Success of operation
   */
  bool SetDeviceData(const std::string& device_id,
                     const std::string& hardware = "",
                     const std::string& firmware = "",
                     const std::string& os = "",
                     const std::string& os_version = "",
                     const std::string& carrier = "",
                     const uint32_t number_of_ports = 0,
                     const std::string& connection_type = "");

  /**
   * @brief Sets user consent for particular mobile device,
   * i.e. to use device for exchanging of Policy Table.
   * @return bool Success of operation
   */
  bool SetUserPermissionsForDevice(
      const std::string& device_id,
      const StringArray& consented_groups = StringArray(),
      const StringArray& disallowed_groups = StringArray());

  /**
   * @brief Update Application Policies as reaction
   * on User allowing/disallowing device this app is running on.
   */
  bool ReactOnUserDevConsentForApp(const std::string& app_id,
                                   bool is_device_allowed);

  /**
   * @brief Set user consent on functional groups
   * @param permissions User consent on functional group
   * @return true, if operation succedeed, otherwise - false
   */
  bool SetUserPermissionsForApp(const PermissionConsent& permissions);

  /**
   * @brief Records information about head unit system to PT
   * @return bool Success of operation
   */
  bool SetMetaInfo(const std::string& ccpu_version,
                   const std::string& wers_country_code,
                   const std::string& language);

  /**
   * @brief Checks, if specific head unit is present in PT
   * @return boot Suceess, if present, otherwise - false
   */
  bool IsMetaInfoPresent() const;

  /**
   * @brief Set current system language
   * @param language System language
   * @return true, if succedeed, otherwise - false
   */
  bool SetSystemLanguage(const std::string& language);

  /**
   * Increments global counter
   * @param type type of counter
   */
  void Increment(usage_statistics::GlobalCounterId type);

  /**
   * Increments counter of application
   * @param app_id id application
   * @param type type of counter
   */
  void Increment(const std::string& app_id,
                 usage_statistics::AppCounterId type);

  /**
   * Sets value of application information
   * @param app_id id application
   * @param type type of information
   * @param value value of information
   */
  void Set(const std::string& app_id,
           usage_statistics::AppInfoId type,
           const std::string& value);

  /**
   * Adds value to stopwatch of application
   * @param app_id id application
   * @param type type of stopwatch
   * @param seconds value for adding in seconds
   */
  void Add(const std::string& app_id,
           usage_statistics::AppStopwatchId type,
           int seconds);

  /**
   * @brief CountUnconsentedGroups allows to obtain the count of unconsented
   * groups for specific application.
   * @param policy_app_id application id.
   * @param device_id device id.
   * @return the count of unconsented groups
   */
  int CountUnconsentedGroups(const std::string& policy_app_id,
                             const std::string& device_id);

  /**
   * @brief Gets functional group names and user_consent_prompts, if any
   * @param Array to be filled with group ids, names and functional prompts
   * @return true, if succeeded, otherwise - false
   */
  bool GetFunctionalGroupNames(FunctionalGroupNames& names);

  /**
   * @brief GetAllAppGroups allows to obtain all groups for certain application.
   * @param app_id specific application id.
   * @param all_group_ids parameter to fill.
   */
  void GetAllAppGroups(const std::string& app_id,
                       FunctionalGroupIDs& all_group_ids);
  /**
   * @brief GetPreConsentedGroups allows to obtain all pre-consented groups for
   * specific application.
   * @param app_id specific application id.
   * @param preconsented_groups parameter to fill.
   */
  void GetPreConsentedGroups(const std::string& app_id,
                             FunctionalGroupIDs& preconsented_groups);
  /**
   * @brief GetConsentedGroups allows to obtain list of allowed and disallowed
   * groups for specific application on certain device.
   * @param device_id certain device
   * @param app_id application id.
   * @param allowed_groups list of allowed groups
   * @param disallowed_groups list of disallowed groups
   */
  void GetConsentedGroups(const std::string& device_id,
                          const std::string& app_id,
                          FunctionalGroupIDs& allowed_groups,
                          FunctionalGroupIDs& disallowed_groups);

  /**
   * @brief GetUnconsentedGroups allows to obtain list of allowed and disallowed
   * groups for specific application on certain device.
   * @param device_id certain device
   * @param policy_app_id application id.
   * @param unconsented_groups list of unconsented groups.
   */
  void GetUnconsentedGroups(const std::string& device_id,
                            const std::string& policy_app_id,
                            FunctionalGroupIDs& unconsented_groups);

  void RemoveAppConsentForGroup(const std::string& app_id,
                                const std::string& group_name);

  /**
   * @brief Set app policy to pre_DataConsented policy
   * @param app_id Policy ID of application to be changed
   * @return true, if succeeded, otherwise - false
   */
  bool SetPredataPolicy(const std::string& app_id);

  /**
   * @brief Removes unpaired devices
   * @return true if success
   */
  bool CleanupUnpairedDevices();

  /**
   * Sets flag of unpaired device
   * @param device_id Unique device id
   * @param unpaired True, if should be marked as unpaired, otherwise - false
   * @return true if success
   */
  bool SetUnpairedDevice(const std::string& device_id, bool unpaired = true);

  /**
   * Resets Policy Table
   * @param file_name Path to preloaded PT file
   * @return true if successfully
   */
  bool ResetPT(const std::string& file_name);

  /**
   * @brief LoadFromBackup allows to load policy into the cache from backup.
   * @return true in case operation was successful.
   */
  bool LoadFromBackup();

  /**
   * @brief LoadFromFile allows to load policy cache from preloaded table.
   * @param file_name preloaded
   * @return
   */
  bool LoadFromFile(const std::string& file_name, policy_table::Table& table);

  /**
   * @brief Backup allows to save cache onto hard drive.
   */
  void Backup();

  /**
   * Returns heart beat timeout
   * @param app_id application id
   * @return if timeout was set then value in milliseconds greater zero
   * otherwise heart beat for specific application isn't set
   */
  uint32_t HeartBeatTimeout(const std::string& app_id) const;

  /**
   * @brief Allows to generate hash from the specified string.
   * The djb2 algorithm uses for hash generation.
   * @param str_to_hash - the string from which hash should be generated.
   * @return integer hash for the specified string.
   */
  static int32_t GenerateHash(const std::string& str_to_hash);

  /**
   * @brief Gets request types for application
   * @param policy_app_id Unique application id
   * @param request_types Request types of application
   */
  void GetAppRequestTypes(const std::string& policy_app_id,
                          std::vector<std::string>& request_types) const;

  /**
   * @brief GetCertificate allows to obtain certificate in order to
   * make secure connection
   *
   * @return The certificate in PKCS#7.
   */
  virtual std::string GetCertificate() const OVERRIDE;

  /**
   * @brief MergePreloadPT allows to load policy table from certain JSON file,
   * and then decide if merge is needed. The merge is needed in case when
   * preload
   * JSON date is different than current database.
   *
   * @param file_name the preloaded policy table JSON file.
   * @return false in case of invalid preloaded_pt
   */
  bool MergePreloadPT(const std::string& file_name);

  /**
   * @brief MergeMC allows to merge ModuleConfig section by definite rules.
   *
   * The rules are:
   * 1. Add new fields (known to PoliciesManager) & sub-sections if such are
   * present in the updated Preloaded PT
   * 2. "vehicle_make", “model”, “year” – leave the fields & values as they were
   * in the database
   * 3. For all other fields – overwrite the values with the new ones from
   *preloaded PT.
   *
   * @param new_pt the policy table loaded from updated preload JSON file.
   *
   * @param pt the exists database.
   */
  void MergeMC(const policy_table::PolicyTable& new_pt,
               policy_table::PolicyTable& pt);

  /**
   * @brief MergeFG allows to merge FunctionalGroupings sections by definite
   *rules.
   *
   * The rules are:
   * 1. If functional_group_name exists in both database (LocalPT) and updated
   * PreloadedPT -> PoliciesManager must overwrite it (that is, replace such
   * functional_group_name in the database by the one from Pre-PT).
   * 2. If functional_group_name exists in updated PreloadedPT and does not
   * exist in database (LocalPT), PoliciesManager must add such group to the
   *database.
   * 3. If functional_group_name does not exist in updated PreloadedPT and
   * exists in the database (LocalPT), PoliciesManager must leave such group in
   * the database without changes.
   *
   * @param new_pt the policy table loaded from updated preload JSON file.
   *
   * @param pt the exists database.
   */
  void MergeFG(const policy_table::PolicyTable& new_pt,
               policy_table::PolicyTable& pt);

  /**
   * @brief MergeAP Allows to merge ApplicationPolicies section by definite
   *relues.
   * The rules are:
   * 1. Leave “<appID>” sub-sections as they were in the database (fields &
   *their values).
   * 2. Over-write "default", "device", "pre_DataConsent" subsections.
   *
   * @param new_pt the policy table loaded from updated preload JSON file.
   *
   * @param pt the exists database.
   */
  void MergeAP(const policy_table::PolicyTable& new_pt,
               policy_table::PolicyTable& pt);

  /**
   * @brief MergeCFM allows to merge ConsumerFriendlyMessages section by
   *definite rules.
   *
   * The rules are:
   * 1. If friendly_message_name exists in both database (LocalPT) and updated
   * Preloaded PT -> PoliciesManager must overwrite it.
   * 2. If friendly_message_name exists in updated Preloaded PT and does not
   * exist in database (LocalPT), PoliciesManager must add such
   * friendly_message_name to the database (LocalPT).
   * 3. If friendly_message_name does not exist in updated Preloaded PT and
   * exists in the database (LocalPT), PoliciesManager must leave such
   * friendly_message_name in the database without changes.
   *
   * @param new_pt the policy table loaded from updated preload JSON file.
   *
   * @param pt the exists database
   */
  void MergeCFM(const policy_table::PolicyTable& new_pt,
                policy_table::PolicyTable& pt);

  const PolicySettings& get_settings() const;

  utils::SharedPtr<policy_table::Table> pt() const {
    return pt_;
  }

  /**
   * @brief OnDeviceSwitching Processes existing policy permissions for devices
   * switching transport
   * @param device_id_from Device ID original
   * @param device_id_to Device ID new
   */
  void OnDeviceSwitching(const std::string& device_id_from,
                         const std::string& device_id_to) OVERRIDE;

 private:
  std::string currentDateTime();
  struct AppHMITypeToString {
    std::string operator()(rpc::Enum<policy_table::AppHMIType> value) {
      return std::string(policy_table::EnumToJsonString(value));
    }
  };

  void GetGroupNameByHashID(const int32_t group_id, std::string& group_name);
  void FillDeviceSpecificData();
  bool AppExists(const std::string& app_id) const;
  long ConvertSecondsToMinute(int seconds);

  /**
   * @brief Checks snapshot initialization and initializes to default values, if
   * necessary
   */
  void CheckSnapshotInitialization();

  void PersistData();

  /**
   * @brief Transform to lower case all non default application names in
   * applications policies section
   * @param pt polict rable for update
   */
  void MakeLowerCaseAppNames(policy_table::Table& pt) const;

  void ResetCalculatedPermissions();

  void AddCalculatedPermissions(const std::string& device_id,
                                const std::string& policy_app_id,
                                const policy::Permissions& permissions);

  bool IsPermissionsCalculated(const std::string& device_id,
                               const std::string& policy_app_id,
                               policy::Permissions& permission);

 private:
  utils::SharedPtr<policy_table::Table> pt_;
  utils::SharedPtr<policy_table::Table> snapshot_;
  utils::SharedPtr<PTRepresentation> backup_;
  bool update_required;
  typedef std::set<std::string> UnpairedDevices;
  UnpairedDevices is_unpaired_;

  mutable sync_primitives::Lock cache_lock_;
  sync_primitives::Lock unpaired_lock_;

  typedef std::map<std::string, Permissions> AppCalculatedPermissions;
  typedef std::map<std::string, AppCalculatedPermissions> CalculatedPermissions;
  CalculatedPermissions calculated_permissions_;
  sync_primitives::Lock calculated_permissions_lock_;

  class BackgroundBackuper : public threads::ThreadDelegate {
    friend class CacheManager;

   public:
    BackgroundBackuper(CacheManager* cache_manager);
    ~BackgroundBackuper();
    virtual void threadMain();
    virtual void exitThreadMain();
    void DoBackup();

   private:
    void InternalBackup();
    CacheManager* cache_manager_;
    sync_primitives::ConditionalVariable backup_notifier_;
    volatile bool stop_flag_;
    volatile bool new_data_available_;

    sync_primitives::Lock need_backup_lock_;
    DISALLOW_COPY_AND_ASSIGN(BackgroundBackuper);
  };
  threads::Thread* backup_thread_;
  sync_primitives::Lock backuper_locker_;
  BackgroundBackuper* backuper_;
  const PolicySettings* settings_;

#ifdef BUILD_TESTS
  friend class AccessRemoteImpl;
  FRIEND_TEST(AccessRemoteImplTest, CheckModuleType);
  FRIEND_TEST(AccessRemoteImplTest, EnableDisable);
  FRIEND_TEST(AccessRemoteImplTest, GetGroups);
#endif  // BUILD_TESTS
};
}  // namespace policy
#endif  // SRC_COMPONENTS_POLICY_POLICY_REGULAR_INCLUDE_POLICY_CACHE_MANAGER_H_

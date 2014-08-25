#ifndef CACHE_MANAGER_H
#define CACHE_MANAGER_H

#include "utils/shared_ptr.h"
#include "policy/pt_representation.h"
#include "policy/pt_ext_representation.h"
#include "utils/lock.h"

#include <map>
namespace policy {
class CacheManager {
  public:
    CacheManager();

    bool UpdateRequired() const;
    void SaveUpdateRequired(bool status);
    bool IsApplicationRevoked(const std::string app_id);

    virtual CheckPermissionResult CheckPermissions(const PTString& app_id,
                                                   const PTString& hmi_level,
                                                   const PTString& rpc);

    virtual bool IsPTPreloaded();
    virtual int IgnitionCyclesBeforeExchange();
    virtual int KilometersBeforeExchange(int current);
    virtual bool SetCountersPassedForSuccessfulUpdate(int kilometers,
                                                      int days_after_epoch);
    virtual int DaysBeforeExchange(int current);
    virtual void IncrementIgnitionCycles();
    virtual void ResetIgnitionCycles();
    virtual int TimeoutResponse();
    virtual bool SecondsBetweenRetries(std::vector<int> &seconds);

    virtual VehicleData GetVehicleData();

    virtual EndpointUrls GetUpdateUrls(int service_type);

    virtual int GetNotificationsNumber(const std::string& priority);
    virtual bool GetPriority(const std::string& policy_app_id,
                             std::string& priority);

    virtual utils::SharedPtr<policy_table::Table> GenerateSnapshot() const;
    bool Save(const policy_table::Table& table);

    bool GetInitialAppData(const std::string& app_id,
                           StringArray& nicknames,
                           StringArray& app_hmi_types);

    bool GetFunctionalGroupings(policy_table::FunctionalGroupings& groups);

    bool CountUnconsentedGroups(const std::string& policy_app_id,
                                const std::string& device_id,
                                int& result);

    bool CanAppKeepContext(const std::string& app_id);
    bool CanAppStealFocus(const std::string& app_id);
    bool GetDefaultHMI(const std::string& app_id,
                       std::string &default_hmi);
    bool ResetUserConsent();
    bool GetUserPermissionsForDevice(const std::string& device_id,
                                     StringArray &consented_groups,
                                     StringArray &disallowed_groups);

    bool GetPermissionsForApp(const std::string& device_id, const std::string& app_id,
      FunctionalIdType &group_types);

    bool GetDeviceGroupsFromPolicies(
        rpc::policy_table_interface_base::Strings &groups,
        rpc::policy_table_interface_base::Strings &preconsented_groups);

    bool SetDeviceData(const std::string& device_id,
                       const std::string& hardware = "",
                       const std::string& firmware = "",
                       const std::string& os = "",
                       const std::string& os_version = "",
                       const std::string& carrier = "",
                       const uint32_t number_of_ports = 0,
                       const std::string& connection_type = "");

    bool SetUserPermissionsForDevice(
        const std::string& device_id,
        const StringArray& consented_groups = StringArray(),
        const StringArray& disallowed_groups = StringArray());

    bool ReactOnUserDevConsentForApp(const std::string& app_id,
                                     bool is_device_allowed);

    bool SetUserPermissionsForApp(const PermissionConsent& permissions);

    std::vector<UserFriendlyMessage> GetUserFriendlyMsg(
      const std::vector<std::string>& msg_codes, const std::string& language);

    bool IncreaseStatisticsData(StatisticsType type) {
      return true;
    }
    bool SetAppRegistrationLanguage(const std::string& app_id, LanguageType type,
                                    const std::string& language) {
      return true;
    }

    bool SetMetaInfo(const std::string& ccpu_version,
                     const std::string& wers_country_code,
                     const std::string& language);

    bool IsMetaInfoPresent() const;

    bool SetSystemLanguage(const std::string& language);

    bool GetFunctionalGroupNames(FunctionalGroupNames& names);
    bool CleanupUnpairedDevices(const DeviceIds& device_ids);

    void Increment(const std::string& type);
    void Increment(const std::string& app_id, const std::string& type) const;
    void Set(const std::string& app_id, const std::string& type,
             const std::string& value) const;
    void Add(const std::string& app_id, const std::string& type,
             int seconds) const;
    bool SetDefaultPolicy(const std::string& app_id);
    bool IsDefaultPolicy(const std::string& app_id);
    bool SetIsDefault(const std::string& app_id, bool is_default);
    bool SetPredataPolicy(const std::string& app_id);
    bool SetIsPredata(const std::string& app_id, bool is_pre_data);
    bool IsPredataPolicy(const std::string& app_id);
    bool SetUnpairedDevice(const std::string& device_id);
    bool UnpairedDevicesList(DeviceIds& device_ids);
    bool SetVINValue(const std::string& value);

    bool IsApplicationRepresented(const std::string& app_id) const;

    void SetPT(utils::SharedPtr<policy_table::Table> table);


    bool Init(const std::string& file_name);
    bool LoadFromBackup();
    bool LoadFromFile(const std::string& file_name);
    bool ResetPT(const std::string& file_name);

    void GetAllAppGroups(const std::string& app_id,
                         FunctionalGroupIDs& all_group_ids);

    void GetPreConsentedGroups(const std::string &app_id,
                               FunctionalGroupIDs& preconsented_groups);

    void GetConsentedGroups(const std::string &device_id,
                            const std::string &app_id,
                            FunctionalGroupIDs& allowed_groups,
                            FunctionalGroupIDs& disallowed_groups);

    bool ApplyUpdate(const policy_table::Table& update_pt);

    void Backup();
  private:

    struct AppHMITypeToString {
        std::string operator()(rpc::Enum<policy_table::AppHMIType> value) {
          return std::string(
                policy_table::EnumToJsonString(value));
        }
    };

    void GetGroupNameByHashID(const uint32_t group_id, std::string& group_name);
    void FillDeviceSpecificData();
    void FillAppSpecificData();
    void CopyInternalParams(const std::string &from, const std::string &to);
    bool AppExists(const std::string& app_id) const;
    unsigned long GenerateHash(const std::string& str_to_hash);
    utils::SharedPtr<policy_table::Table> pt;


    utils::SharedPtr<PTRepresentation> backup_;
    utils::SharedPtr<PTExtRepresentation> ex_backup_;

    bool update_required;
    std::map<std::string, bool> is_revoked;
    std::map<std::string, bool> is_default;
    std::map<std::string, bool> is_predata;
    std::map<std::string, bool> is_unpaired;

    sync_primitives::Lock cache_lock_;
};
} // policy

#endif // CACHE_MANAGER_H

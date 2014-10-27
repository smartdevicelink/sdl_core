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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_EXT_REPRESENTATION_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_EXT_REPRESENTATION_H_

#include <string>
#include "policy/sql_pt_representation.h"
#include "policy/pt_ext_representation.h"

namespace policy {

class SQLPTExtRepresentation : public SQLPTRepresentation,
  public PTExtRepresentation {
  public:
    bool CanAppKeepContext(const std::string& app_id);
    bool CanAppStealFocus(const std::string& app_id);
    bool GetDefaultHMI(const std::string& policy_app_id,
                       std::string* default_hmi);
    bool ResetUserConsent();
    bool ResetDeviceConsents();
    bool ResetAppConsents();
    bool GetUserPermissionsForDevice(const std::string& device_id,
                                     StringArray* consented_groups = NULL,
                                     StringArray* disallowed_groups = NULL);

    bool GetPermissionsForApp(
      const std::string& device_id, const std::string& policy_app_id,
      FunctionalIdType* group_types);

    bool GetDeviceGroupsFromPolicies(policy_table::Strings* groups = NULL,
                                     policy_table::Strings* preconsented_groups =
                                       NULL);
    bool SetDeviceData(const std::string& device_id,
                       const std::string& hardware = "",
                       const std::string& firmware = "",
                       const std::string& os = "",
                       const std::string& os_version = "",
                       const std::string& carrier = "",
                       const uint32_t number_of_ports = 0,
                       const std::string& connection_type = "");
    bool SetUserPermissionsForDevice(const std::string& device_id,
                                     const StringArray& consented_groups =
                                       StringArray(),
                                     const StringArray& disallowed_groups =
                                       StringArray());

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

    bool IsMetaInfoPresent();

    bool SetSystemLanguage(const std::string& language);

    int GetKmFromSuccessfulExchange() {
      return true;
    }
    int GetDayFromScsExchange() {
      return true;
    }
    int GetIgnitionsFromScsExchange() {
      return true;
    }

    bool GetFunctionalGroupNames(FunctionalGroupNames& names);
    bool CleanupUnpairedDevices(const DeviceIds& device_ids) const;

    void Increment(const std::string& type) const;
    void Increment(const std::string& app_id, const std::string& type) const;
    void Set(const std::string& app_id, const std::string& type,
             const std::string& value) const;
    void Add(const std::string& app_id, const std::string& type,
             int seconds) const;
    bool SetDefaultPolicy(const std::string& app_id);
    bool SetPredataPolicy(const std::string& app_id);
    bool SetIsPredata(const std::string& app_id, bool is_pre_data);
    bool IsPredataPolicy(const std::string& app_id) const;
    bool SetUnpairedDevice(const std::string& device_id) const;
    bool UnpairedDevicesList(DeviceIds* device_ids) const;
    bool RemoveAppConsentForGroup(
        const std::string& policy_app_id,
        const std::string& functional_group_name) const;

    virtual bool SetVINValue(const std::string& value);

  private:
    void GatherModuleMeta(policy_table::ModuleMeta* meta) const;
    void GatherPreconsentedGroup(const std::string& app_id,
                                 policy_table::Strings* groups) const;
    bool GatherUsageAndErrorCounts(
      policy_table::UsageAndErrorCounts* counts) const;
    bool GatherAppLevels(policy_table::AppLevels* apps) const;
    void GatherDeviceData(policy_table::DeviceData* data) const;
    void GatherConsentGroup(const std::string& device_id,
                            policy_table::UserConsentRecords* records) const;
    bool GatherApplicationPolicies(policy_table::ApplicationPolicies* apps) const;
    bool SaveDeviceData(const policy_table::DeviceData& devices);
    bool GatherConsumerFriendlyMessages(
        policy_table::ConsumerFriendlyMessages* messages) const;
    bool SaveConsentGroup(const std::string& device_id,
                          const policy_table::UserConsentRecords& records);
    bool SaveApplicationPolicies(const policy_table::ApplicationPolicies& apps);
    bool SaveSpecificAppPolicy(
        const policy_table::ApplicationPolicies::value_type& app);
    bool SavePreconsentedGroup(const std::string& app_id,
                               const policy_table::Strings& groups);
    bool SaveMessageString(const std::string& type, const std::string& lang,
                           const policy_table::MessageString& strings);

    virtual bool SaveUsageAndErrorCounts(
        const policy_table::UsageAndErrorCounts& counts);

    bool IsExistAppLevel(const std::string& app_id) const;

    bool GetAllAppGroups(const std::string& policy_app_id,
                         FunctionalGroupIDs& all_groups);

    bool GetConsentedGroups(const std::string& policy_app_id,
                            const std::string& device_id,
                            FunctionalGroupIDs& allowed_groups,
                            FunctionalGroupIDs& disallowed_groups);

    bool GetPreconsentedGroups(const std::string& policy_app_id,
                               FunctionalGroupIDs& preconsented_groups);

    void FillFunctionalGroupPermissions(
      FunctionalGroupIDs& ids, FunctionalGroupNames& names,
      GroupConsent state,
      std::vector<FunctionalGroupPermission>& permissions);
    bool CountUnconsentedGroups(const std::string& policy_app_id,
                                const std::string& device_id,
                                int* result) const;

    /**
     * @brief Checks, if there is message present with requested language in PT
     * @param message Message name
     * @param language Required message language
     * @return True, if message with requested language is present, otherwise -
     * false
     */
    bool IsMsgLanguagePresent(const std::string& message,
                              const std::string& language);
};

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_SQL_PT_EXT_REPRESENTATION_H_

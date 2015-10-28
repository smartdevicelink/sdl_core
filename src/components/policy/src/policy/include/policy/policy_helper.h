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

#ifndef SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_HELPER_H_
#define SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_HELPER_H_

#include "functions.h"
#include "utils/shared_ptr.h"
#include "policy/policy_types.h"

namespace policy {
class PolicyManagerImpl;

const std::string kAllowedKey = "allowed";
const std::string kUserDisallowedKey = "userDisallowed";
const std::string kUndefinedKey = "undefined";

namespace policy_table = rpc::policy_table_interface_base;

typedef policy_table::Strings::const_iterator StringsConstItr;
typedef policy_table::ApplicationPolicies::const_iterator AppPoliciesConstItr;
typedef policy_table::HmiLevels::const_iterator HMILevelsConstItr;
typedef policy_table::Parameters::const_iterator ParametersConstItr;
typedef policy_table::FunctionalGroupings::const_iterator FuncGroupConstItr;

typedef policy_table::ApplicationPolicies::value_type AppPoliciesValueType;
typedef policy_table::Rpc::value_type RpcValueType;
typedef policy_table::Strings::value_type StringsValueType;

/*
 * @brief Helper struct to compare functional group names
 */
struct CompareGroupName {
    explicit CompareGroupName(const StringsValueType& group_name);
    bool operator()(const StringsValueType& group_name_to_compare) const;
  private:
    const StringsValueType& group_name_;
};

/*
 * @brief Used for compare of policies parameters mapped with specific
 * application ids
 */
bool operator!=(const policy_table::ApplicationParams& first,
                const policy_table::ApplicationParams& second);

/*
 * @brief Helper struct for checking changes of application policies, which
 * come with update along with current data snapshot
 * In case of policies changed for some application, current data will be
 * updated and notification will be sent to application
 */
struct CheckAppPolicy {
    CheckAppPolicy(PolicyManagerImpl* pm,
                   const utils::SharedPtr<policy_table::Table> update,
                   const utils::SharedPtr<policy_table::Table> snapshot);
    bool operator()(const AppPoliciesValueType& app_policy);
  private:
    enum PermissionsCheckResult {
        RESULT_NO_CHANGES,
        RESULT_APP_REVOKED,
        RESULT_NICKNAME_MISMATCH,
        RESULT_PERMISSIONS_REVOKED,
        RESULT_CONSENT_NEEDED,
        RESULT_CONSENT_NOT_REQIURED,
        RESULT_PERMISSIONS_REVOKED_AND_CONSENT_NEEDED,
        RESULT_REQUEST_TYPE_CHANGED
    };

    void SetPendingPermissions(const AppPoliciesValueType& app_policy,
                                PermissionsCheckResult result) const;
    PermissionsCheckResult CheckPermissionsChanges(
            const AppPoliciesValueType &app_policy) const;
    bool HasRevokedGroups(const AppPoliciesValueType& app_policy,
                          policy_table::Strings* revoked_groups = NULL) const;
    bool HasNewGroups(const AppPoliciesValueType& app_policy,
                      policy_table::Strings* new_groups = NULL) const;
    bool HasConsentNeededGroups(const AppPoliciesValueType& app_policy) const;
    std::vector<FunctionalGroupPermission> GetRevokedGroups(
            const AppPoliciesValueType& app_policy) const;
    void RemoveRevokedConsents(
            const AppPoliciesValueType& app_policy,
            const std::vector<FunctionalGroupPermission>& revoked_groups) const;
    bool IsKnownAppication(const std::string& application_id) const;
    void NotifySystem(const AppPoliciesValueType& app_policy) const;
    void SendPermissionsToApp(const AppPoliciesValueType& app_policy) const;
    bool IsAppRevoked(const AppPoliciesValueType& app_policy) const;
    bool NicknamesMatch(const AppPoliciesValueType& app_policy) const;
    /**
     * @brief Allows to check if appropriate group requires any consent.
     * @param group_name the group for which consent will be checked.
     * @return true if consent is required, false otherwise.
     */
    bool IsConsentRequired(const std::string& app_id,
                           const std::string& group_name) const;
    bool IsRequestTypeChanged(const AppPoliciesValueType& app_policy) const;

  private:
    PolicyManagerImpl* pm_;
    const utils::SharedPtr<policy_table::Table> update_;
    const utils::SharedPtr<policy_table::Table> snapshot_;
};

/*
 * @brief Fill permissions data with merged rpc permissions for hmi levels and
 * parameters
 */
struct FillNotificationData {
    FillNotificationData(Permissions& data, GroupConsent group_state,
                         GroupConsent undefined_group_consent);
    bool operator()(const RpcValueType& rpc);
    void UpdateHMILevels(const policy_table::HmiLevels& in_hmi,
                         std::set<HMILevel>& out_hmi);
    void UpdateParameters(const policy_table::Parameters& in_parameters,
                          std::set<Parameter>& out_parameter);
  private:
    void ExcludeSame();
    void ExcludeSameHMILevels(std::set<HMILevel>& source,
                              const std::set<HMILevel>& target);
    void ExcludeSameParameters(std::set<Parameter>& source,
                               const std::set<Parameter>& target);
    void InitRpcKeys(const std::string& rpc_name);
    std::string current_key_;
    Permissions& data_;
};

/*
 * @brief Checks for functional group presence and pass it to helper struct,
 * which fills permissions data according to group consent
 */
struct ProcessFunctionalGroup {
    ProcessFunctionalGroup(
      const policy_table::FunctionalGroupings& fg,
      const std::vector<FunctionalGroupPermission>& group_permissions,
      Permissions& data,
      GroupConsent undefined_group_consent = GroupConsent::kGroupDisallowed);
    bool operator()(const StringsValueType& group_name);
  private:
    GroupConsent GetGroupState(const std::string& group_name);
    const policy_table::FunctionalGroupings& fg_;
    const std::vector<FunctionalGroupPermission>& group_permissions_;
    Permissions& data_;
    GroupConsent undefined_group_consent_;
};

struct FunctionalGroupInserter {
    FunctionalGroupInserter(const policy_table::Strings& preconsented_groups,
                            PermissionsList& list);
    void operator()(const StringsValueType& group_name);
  private:
    PermissionsList& list_;
    const policy_table::Strings& preconsented_;
};

/**
 * @brief Fills FunctionalGroupPermissions with provided params
 * @param ids Functional group ids from DB
 * @param names Group names and user_consent_prompt
 * @param state User consent for group
 * @param permissions Struct to be filled with provided params
 */
void FillFunctionalGroupPermissions(
  FunctionalGroupIDs& ids,
  FunctionalGroupNames& names,
  GroupConsent state,
  std::vector<FunctionalGroupPermission>& permissions);

/**
 * @brief Checks, if application is predefined, e.g. "default", i.e. which is
 * must be present in policy table
 * @param app Application struct
 * @return true, if app is predefined, otherwise - false
 */
bool IsPredefinedApp(const AppPoliciesValueType& app);

/**
 * @brief Excludes same values
 * @param from Source, which should be checked
 * @param what Target, which should be excluded from source
 * @return Values without excluded
 */
FunctionalGroupIDs ExcludeSame(const FunctionalGroupIDs& from,
                               const FunctionalGroupIDs& what);

/**
 * @brief Merges all values without same values
 * @param first First source of values
 * @param second Second source of values
 * @return Merged values w/o same values
 */
FunctionalGroupIDs Merge(const FunctionalGroupIDs& first,
                         const FunctionalGroupIDs& second);

/**
 * @brief Finds same values
 * @param first First source of values
 * @param second Second source of values
 * @return Same values set, if any found
 */
FunctionalGroupIDs FindSame(const FunctionalGroupIDs& first,
                            const FunctionalGroupIDs& second);

/**
 * @brief Unwrap application policies from predefined values to specific policy
 * values, i.e. if application has "default", it will be assigned default
 * policies
 * @param app_policies Application policies to unwrap
 * @return true, if succeded, otherwise - false
 */
bool UnwrapAppPolicies(policy_table::ApplicationPolicies& app_policies);

}

#endif // SRC_COMPONENTS_POLICY_INCLUDE_POLICY_POLICY_HELPER_H_

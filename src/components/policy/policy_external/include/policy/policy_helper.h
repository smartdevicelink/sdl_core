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

#ifndef SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_POLICY_HELPER_H_
#define SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_POLICY_HELPER_H_

#include "policy/policy_table/functions.h"

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
typedef policy_table::Rpc::const_iterator RpcConstItr;
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

/**
 * @brief Helper struct for checking changes of application policies, which
 * come with update along with current data snapshot
 * @param pm Pointer to PolicyManager instance
 * @param update Shared pointer to policy table update received
 * @param snapshot Shared pointer to current policy table copy
 * @param out_results Collection of check result
 */
struct CheckAppPolicy {
  CheckAppPolicy(PolicyManagerImpl* pm,
                 const std::shared_ptr<policy_table::Table> update,
                 const std::shared_ptr<policy_table::Table> snapshot,
                 CheckAppPolicyResults& out_results);

  bool operator()(const AppPoliciesValueType& app_policy);

 private:
  /**
   * @brief Sets pending values to be notified to the system (depends on HMI
   * level of application) with SDL.ActivateApp or OnAppPermissionsChanged
   * notification
   * @param app_policy Reference to updated application policy
   * @param result Result of check of updated policy
   */
  void SetPendingPermissions(const AppPoliciesValueType& app_policy,
                             PermissionsCheckResult result) const;
  /**
   * @brief Analyzes updated application policy whether any changes received. If
   * yes - provides appropriate result code
   * @param app_policy Reference to updated application policy
   * @return Result code according to changes in updated policy
   */
  PermissionsCheckResult CheckPermissionsChanges(
      const AppPoliciesValueType& app_policy) const;
  /**
   * @brief Checks whether updated policy has groups revoked, i.e. absent in
   * compare to current one
   * @param app_policy Reference to updated policy
   * @param revoked_groups List of revoked groups if any
   * @return True if there are revoked groups, otherwise - false
   */
  bool HasRevokedGroups(const AppPoliciesValueType& app_policy,
                        policy_table::Strings* revoked_groups = NULL) const;
  /**
   * @brief Checks whether updated application policy has new group in compare
   * to current one
   * @param app_policy Reference to updated application policy
   * @param new_groups List of new groups if any
   * @return True if new groups found, otherwise - false
   */
  bool HasNewGroups(const AppPoliciesValueType& app_policy,
                    policy_table::Strings* new_groups = NULL) const;
  /**
   * @brief Checks whether updated policy has groups which require user consent
   * @param app_policy Reference to updated application policy
   * @return True if has groups requiring user consents, otherwise - false
   */
  bool HasConsentNeededGroups(const AppPoliciesValueType& app_policy) const;
  /**
   * @brief Gets revoked groups parameters from current policies
   * @param app_policy Reference to updated application policy
   * @return List of revoked groups with their parameters
   */
  std::vector<FunctionalGroupPermission> GetRevokedGroups(
      const AppPoliciesValueType& app_policy) const;
  /**
   * @brief Removes consents for revoked groups of application
   * @param app_policy Reference to updated application policy
   * @param revoked_groups List of revoked groups with parameters to remove
   * consents if any exists
   */
  void RemoveRevokedConsents(
      const AppPoliciesValueType& app_policy,
      const std::vector<FunctionalGroupPermission>& revoked_groups) const;
  /**
   * @brief Checks whether application is present in current policy table,
   * since update can be processed only for application known by policy while
   * it sent update request
   * @param application_id Application id
   * @return True if application is known, otherwise - false
   */
  bool IsKnownAppication(const std::string& application_id) const;
  /**
   * @brief Checks whether application is revoked by backend, i.e. has 'null'
   * for policies parameters
   * @param app_policy Reference to updated application policy
   * @return True if application is revoked, otherwise - false
   */
  bool IsAppRevoked(const AppPoliciesValueType& app_policy) const;
  /**
   * @brief Checks whether there such application is registered and has correct
   * nickname
   * @param app_policy Reference to updated application policy
   * @return  True if there is nickname mismatch, otherwise - false
   */
  bool NicknamesMatch(const AppPoliciesValueType& app_policy) const;
  /**
   * @brief Adds result of check of current application policy with updated one
   * @param app_id Application id
   * @param result Result value
   */
  void AddResult(const std::string& app_id, PermissionsCheckResult result);
  /**
   * @brief Allows to check if appropriate group requires any consent.
   * @param group_name the group for which consent will be checked.
   * @return true if consent is required, false otherwise.
   */
  bool IsConsentRequired(const std::string& app_id,
                         const std::string& group_name) const;
  /**
   * @brief Checks whether RequestTypes of application have been changed by
   * udpated
   * @param app_policy Reference to updated application policy
   * @return True if changed, otherwise - false
   */
  bool IsRequestTypeChanged(const AppPoliciesValueType& app_policy) const;

  /**
   * @brief Checks whether App RequestSubTypes have been changed by
   * udpated
   * @param app_policy Reference to updated application policy
   * @return True if changed, otherwise - false
   */
  bool IsRequestSubTypeChanged(const AppPoliciesValueType& app_policy) const;

 private:
  PolicyManagerImpl* pm_;
  const std::shared_ptr<policy_table::Table> update_;
  const std::shared_ptr<policy_table::Table> snapshot_;
  CheckAppPolicyResults& out_results_;
};

/**
 * @brief Helper struct for filling actions to be done for processed application
 * using CheckAppPolicyResults data as a source
 */
struct FillActionsForAppPolicies {
  FillActionsForAppPolicies(
      ApplicationsPoliciesActions& actions,
      const policy_table::ApplicationPolicies& app_policies)
      : actions_(actions), app_policies_(app_policies) {}

  void operator()(const policy::CheckAppPolicyResults::value_type& value);

 private:
  ApplicationsPoliciesActions& actions_;
  const policy_table::ApplicationPolicies& app_policies_;
};

/*
 * @brief Fill permissions data with merged rpc permissions for hmi levels and
 * parameters
 */
struct FillNotificationData {
  /**
 * @brief Constructor
 * @param data Output structure with filled data
 * @param group_state Consent of the group processed by instance
 * @param undefined_group_consent Defines how to treat 'undefined' consent
 * @param does_require_user_consent Specifies whether processed group requires
 * user consent
 */
  FillNotificationData(Permissions& data,
                       GroupConsent group_state,
                       GroupConsent undefined_group_consent,
                       bool does_require_user_consent);
  bool operator()(const RpcValueType& rpc);
  void UpdateHMILevels(const policy_table::HmiLevels& in_hmi,
                       HMIPermissions& out_hmi);
  void UpdateParameters(const policy_table::Parameters& in_parameters,
                        ParameterPermissions& out_parameter);

 private:
  void ExcludeSame(RpcPermissions& rpc);
  void ExcludeSameHMILevels(std::set<HMILevel>& source,
                            const std::set<HMILevel>& target);
  void ExcludeSameParameters(std::set<Parameter>& source,
                             const std::set<Parameter>& target);
  void InitRpcKeys(const std::string& rpc_name);
  bool RpcParametersEmpty(RpcPermissions& rpc);

  /**
   * @brief Checks if specific section in specified permissions is empty
   * @param permissions reference to the permissions structure
   * @param section reference to the section name
   * @return true if specified section in permissions is empty otherwise returns
   * false
   */
  bool IsSectionEmpty(const ParameterPermissions& permissions,
                      const std::string& section) const;

  /**
   * @brief Checks if at least one parameter is allowed for the specified
   * permissions
   * @param permissions reference to the permissions structure
   * @return true if at least one parameter is allowed for the specified
   * permissions otherwise returns false
   */
  bool IsSomeParameterAllowed(const ParameterPermissions& permissions) const;

  std::string current_key_;
  Permissions& data_;
  const bool does_require_user_consent_;
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

}  // namespace policy

#endif  // SRC_COMPONENTS_POLICY_POLICY_EXTERNAL_INCLUDE_POLICY_POLICY_HELPER_H_

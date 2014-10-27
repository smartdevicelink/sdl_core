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

#include <algorithm>
#include <sstream>
#include <string.h>
#include "utils/logger.h"
#include "policy/policy_helper.h"
#include "policy/policy_manager_impl.h"

namespace policy {

namespace {

CREATE_LOGGERPTR_GLOBAL(logger_, "PolicyManagerImpl")

bool Match(const StringsValueType& first_name,
           const StringsValueType& second_name) {
  const std::string& first = first_name;
  const std::string& second = second_name;
  return (strcasecmp(first.c_str(), second.c_str()) == 0);
}

bool Compare(const StringsValueType& first, const StringsValueType& second) {
  const std::string& first_str = first;
  const std::string& second_str = second;
  return (strcasecmp(first_str.c_str(), second_str.c_str()) < 0);
}

}

CompareGroupName::CompareGroupName(const StringsValueType& group_name)
  : group_name_(group_name) {
}

bool CompareGroupName::operator()(
  const StringsValueType& group_name_to_compare) const {
  const std::string gn_ = group_name_;
  const std::string gn_compare = group_name_to_compare;
  return !(strcasecmp(gn_.c_str(), gn_compare.c_str()));
}

bool operator!=(const policy_table::ApplicationParams& first,
                const policy_table::ApplicationParams& second) {
  // TODO(AOleynik): Add comparing of Ford-specific and other parameters
  if (first.groups.size() != second.groups.size()) {
    return true;
  }
  StringsConstItr it_first = first.groups.begin();
  StringsConstItr it_first_end = first.groups.end();
  StringsConstItr it_second = second.groups.begin();
  StringsConstItr it_second_end = second.groups.end();
  for (; it_first != it_first_end; ++it_first) {
    CompareGroupName gp(*it_first);
    StringsConstItr it = std::find_if(it_second, it_second_end, gp);
    if (it_first_end == it) {
      return true;
    }
  }
  return false;
}

CheckAppPolicy::CheckAppPolicy(
    PolicyManagerImpl* pm,
    const utils::SharedPtr<policy_table::Table> update,
    const utils::SharedPtr<policy_table::Table> snapshot)
  : pm_(pm),
    update_(update),
    snapshot_(snapshot) {
}

bool CheckAppPolicy::HasSameGroups(const AppPoliciesValueType& app_policy,
                                   AppPermissions* perms) const {
  const std::string app_id = app_policy.first;
  AppPoliciesConstItr it = snapshot_->policy_table.app_policies.find(app_id);

  if (app_policy.second.is_string()) {
    return (it->second.is_string() &&
            app_policy.second.get_string().compare(it->second.get_string())
            == 0);
  }

  policy_table::Strings groups_new = app_policy.second.groups;
  std::sort(groups_new.begin(), groups_new.end(), Compare);

  policy_table::Strings groups_curr = (*it).second.groups;
  std::sort(groups_curr.begin(), groups_curr.end(), Compare);

  StringsConstItr it_groups_new = groups_new.begin();
  StringsConstItr it_groups_new_end = groups_new.end();

  StringsConstItr it_groups_curr = groups_curr.begin();
  StringsConstItr it_groups_curr_end = groups_curr.end();

  StringsConstItr new_it = it_groups_new;
  StringsConstItr old_it = it_groups_curr;

  std::pair<StringsConstItr, StringsConstItr> diff;

  while (it_groups_new_end != new_it && it_groups_curr_end != old_it) {
    size_t size = ((it_groups_new_end - new_it) > (it_groups_curr_end - old_it))
                  ? it_groups_curr_end - old_it : it_groups_new_end - new_it;
    diff = std::mismatch(old_it, old_it + size, new_it, Match);
    if (it_groups_curr_end == diff.first || it_groups_new_end == diff.second) {
      new_it = diff.second;
      old_it = diff.first;
      break;
    }
    if (Compare(*diff.first, *diff.second) &&
        IsConsentRequired(*(diff.first))) {
      perms->isAppPermissionsRevoked = true;
      FunctionalGroupPermission group;
      group.group_name = *(diff.first);
      perms->appRevokedPermissions.push_back(group);
      old_it = ++diff.first;
      new_it = diff.second;
    } else {
      // according to the SDLAQ-CRS-2757 we have to set
      // appPermissionsConsentNeeded should not be set to true
      // in case if this group is auto-allowed
      perms->appPermissionsConsentNeeded = IsConsentRequired(*new_it);
      old_it = diff.first;
      new_it = ++diff.second;
    }
  }

  for (StringsConstItr it = old_it; it != it_groups_curr_end; ++it) {
    if (!IsConsentRequired(*it)) {
      continue;
    }
    perms->isAppPermissionsRevoked = true;
    FunctionalGroupPermission group;
    group.group_name = *it;
    perms->appRevokedPermissions.push_back(group);
  }

  if (it_groups_new_end != new_it) {
    perms->appPermissionsConsentNeeded = true;
  }

  if (perms->isAppPermissionsRevoked) {

      std::vector<policy::FunctionalGroupPermission>::const_iterator it =
          perms->appRevokedPermissions.begin();
      std::vector<policy::FunctionalGroupPermission>::const_iterator it_end =
          perms->appRevokedPermissions.end();
      for (;it != it_end; ++it) {
       pm_->RemoveAppConsentForGroup(perms->application_id, it->group_name);
    }
  }


  return !(perms->appRevokedPermissions.size() > 0
           || perms->appPermissionsConsentNeeded);
}

bool CheckAppPolicy::IsNewAppication(const std::string& application_id) const {
  const policy_table::ApplicationPolicies& current_policies =
      snapshot_->policy_table.app_policies;
  AppPoliciesConstItr it_app_policies_curr = current_policies.begin();
  AppPoliciesConstItr it_app_policies_curr_end = current_policies.end();

  for (; it_app_policies_curr != it_app_policies_curr_end;
       ++it_app_policies_curr) {

    // Find necessary application in current snapshot
    const std::string application_id_curr = (*it_app_policies_curr).first;
    if (application_id == application_id_curr) {
      return false;
    }
  }
  return true;
}

void CheckAppPolicy::SendNotification(
  const AppPoliciesValueType& app_policy) const {
  // Collecting all available rpcs and their parameters from updated
  // policies and fill notification data struct
  Permissions notification_data;

  // Get current user permissions for groups from DB
  std::vector<FunctionalGroupPermission> group_permissons;
  // Get current device_id from application id
  const std::string device_id = pm_->GetCurrentDeviceId(app_policy.first);
  if (device_id.empty()) {
    LOG4CXX_WARN(logger_, "Couldn't find device info for application id "
                 "'" << app_policy.first << "'");
    return;
  }
  pm_->GetPermissionsForApp(device_id, app_policy.first, group_permissons);

  pm_->PrepareNotificationData(update_->policy_table.functional_groupings,
                               app_policy.second.groups,
                               group_permissons, notification_data);

  const std::string app_id = app_policy.first;
  LOG4CXX_INFO(logger_, "Send notification for application_id:" << app_id);
  // Default_hmi is Ford-specific and should not be used with basic policy
  const std::string default_hmi;
  pm_->listener()->OnPermissionsUpdated(app_id, notification_data, default_hmi);
}

void CheckAppPolicy::SendOnPendingPermissions(
  const AppPoliciesValueType& app_policy, AppPermissions permissions) const {
  // TODO(AOleynik): Exclude default group(s)
  if (permissions.appPermissionsConsentNeeded) {
  }
  // TODO(AOleynik): Seems, it is unused part?
  if (permissions.isAppPermissionsRevoked) {
    pm_->app_permissions_diff_.insert(
      std::make_pair(app_policy.first, permissions));
    pm_->listener()->OnPendingPermissionChange(app_policy.first);
  }
}

bool CheckAppPolicy::IsAppRevoked(
  const AppPoliciesValueType& app_policy) const {
  // Application params are not initialized = application revoked
  // i.e. "123":null
  return app_policy.second.is_null();
}

bool CheckAppPolicy::NicknamesMatch(
    const std::string app_id,
    const AppPoliciesValueType& app_policy) const {
  std::string app_name = pm_->listener()->GetAppName(app_id);
  if (!app_name.empty() &&
      app_policy.second.nicknames &&
      !app_policy.second.nicknames->empty()) {
    for (policy_table::Strings::const_iterator it =
         app_policy.second.nicknames->begin();
         app_policy.second.nicknames->end() != it; ++it) {
      std::string temp = *it;
      if (temp.compare(app_name) == 0) {
        return true;
      }
    }
    return false;
  }
  return true;
}

bool CheckAppPolicy::operator()(const AppPoliciesValueType& app_policy) {
  policy_table::ApplicationPolicies& current_policies =
      snapshot_->policy_table.app_policies;

  const std::string app_id = app_policy.first;

  AppPermissions permissions_diff(app_id);
  permissions_diff.priority = policy_table::EnumToJsonString(
                                app_policy.second.priority);

  // Check revocation
  if (!IsPredefinedApp(app_policy) && IsAppRevoked(app_policy)) {
    permissions_diff.appRevoked = true;
    pm_->app_permissions_diff_.insert(std::make_pair(app_id, permissions_diff));
    pm_->listener()->OnAppRevoked(app_id);
    policy_table::ApplicationPolicies::iterator it = current_policies.find(
          app_id);
    // Update snapshot with new policies for application
    if (it != current_policies.end()) {
      // Update
      (*it).second = app_policy.second;
      it->second.set_to_null();
    } else {
      // Add
      current_policies[app_policy.first] = app_policy.second;
    }
    return true;
  }

  // TODO(PV): do we really need this check?
  if (IsNewAppication(app_id)) {
    // Update snapshot with policies for new application
    current_policies[app_id] = app_policy.second;
    SendNotification(app_policy);
    SendOnPendingPermissions(app_policy, permissions_diff);
    return true;
  }

  if (!IsPredefinedApp(app_policy) && !NicknamesMatch(app_id, app_policy)) {
    permissions_diff.appUnauthorized = true;
    pm_->app_permissions_diff_.insert(std::make_pair(app_id, permissions_diff));
    pm_->listener()->OnPendingPermissionChange(app_policy.first);
    policy_table::ApplicationPolicies::iterator it = current_policies.find(
          app_id);
    // Update snapshot with new policies for application
    if (it != current_policies.end()) {
      (*it).second = app_policy.second;
      it->second.set_to_null();
    }
    return true;
  }

  if (HasSameGroups(app_policy, &permissions_diff)) {
    LOG4CXX_INFO(logger_, "Permissions for application:" << app_id <<
                 " wasn't changed.");
    return true;
  }

  LOG4CXX_INFO(logger_, "Permissions for application:" << app_id <<
               " have been changed.");

  policy_table::ApplicationPolicies::iterator it = current_policies.find(
        app_id);
  // Update snapshot with new policies for application
  (*it).second = app_policy.second;

  // Don't sent notification for predefined apps (e.g. default, device etc.)
  if (!IsPredefinedApp(app_policy)) {
    SendNotification(app_policy);
    SendOnPendingPermissions(app_policy, permissions_diff);
  }
  return true;
}

bool CheckAppPolicy::IsConsentRequired(const std::string& group_name) const {
  const policy_table::FunctionalGroupings& functional_groupings =
      snapshot_->policy_table.functional_groupings;
  FuncGroupConstItr it = functional_groupings.find(group_name);
  if (functional_groupings.end() == it) {
    return false;
  }

  return it->second.user_consent_prompt.is_initialized();
}

FillNotificationData::FillNotificationData(Permissions& data,
    GroupConsent group_state, GroupConsent undefined_group_consent)
  : data_(data) {
  switch (group_state) {
    case kGroupAllowed:
      current_key_ = kAllowedKey;
      break;
    case kGroupUndefined:
      if (kGroupUndefined == undefined_group_consent) {
        current_key_ = kUndefinedKey;
        break;
      }
      current_key_ = kGroupAllowed == undefined_group_consent?
                     kAllowedKey:
                     kUserDisallowedKey;
      break;
    default:
      current_key_ = kUserDisallowedKey;
      break;
  }
}

bool FillNotificationData::operator()(const RpcValueType& rpc) {
  Permissions::iterator it = data_.find(rpc.first);
  // If rpc is present already - update its permissions
  if (data_.end() != it) {
    UpdateHMILevels(rpc.second.hmi_levels,
                    (*it).second.hmi_permissions[current_key_]);
    // TODO(IKozyrenko): Check logic if optional container is missing
    UpdateParameters(*rpc.second.parameters,
                     (*it).second.parameter_permissions[current_key_]);
    ExcludeSame();
  } else {
    // Init mandatory keys, since they should be present irrespectively of
    // values presence
    InitRpcKeys(rpc.first);
    // If rpc is not present - add its permissions
    UpdateHMILevels(rpc.second.hmi_levels,
                    data_[rpc.first].hmi_permissions[current_key_]);
    // TODO(IKozyrenko): Check logic if optional container is missing
    UpdateParameters(*rpc.second.parameters,
                     data_[rpc.first].parameter_permissions[current_key_]);
    ExcludeSame();
  }
  return true;
}

void FillNotificationData::UpdateHMILevels(
  const policy_table::HmiLevels& in_hmi, std::set<HMILevel>& out_hmi) {
  HMILevelsConstItr it_hmi_levels = in_hmi.begin();
  HMILevelsConstItr it_hmi_levels_end = in_hmi.end();

  for (; it_hmi_levels != it_hmi_levels_end; ++it_hmi_levels) {
    out_hmi.insert(policy_table::EnumToJsonString(*it_hmi_levels));
  }
}

void FillNotificationData::UpdateParameters(
  const policy_table::Parameters& in_parameters,
  std::set<Parameter>& out_parameter) {
  ParametersConstItr it_parameters = in_parameters.begin();
  ParametersConstItr it_parameters_end = in_parameters.end();

  for (; it_parameters != it_parameters_end; ++it_parameters) {
    out_parameter.insert(policy_table::EnumToJsonString(*it_parameters));
  }
}

void FillNotificationData::ExcludeSame() {
  Permissions::iterator it = data_.begin();
  Permissions::const_iterator it_end = data_.end();
  // Groups
  for (; it != it_end; ++it) {
    HMIPermissions& rpc_hmi_permissions = (*it).second.hmi_permissions;
    HMIPermissions::const_iterator it_hmi_allowed =
        (*it).second.hmi_permissions.find(kAllowedKey);
    HMIPermissions::const_iterator it_hmi_undefined =
        (*it).second.hmi_permissions.find(kUndefinedKey);
    HMIPermissions::const_iterator it_hmi_user_disallowed =
        (*it).second.hmi_permissions.find(kUserDisallowedKey);

    ParameterPermissions& rpc_parameter_permissions =
            (*it).second.parameter_permissions;
    ParameterPermissions::const_iterator it_parameter_allowed =
        (*it).second.parameter_permissions.find(kAllowedKey);
    ParameterPermissions::const_iterator it_parameter_undefined =
        (*it).second.parameter_permissions.find(kUndefinedKey);
    ParameterPermissions::const_iterator it_parameter_user_disallowed =
        (*it).second.parameter_permissions.find(kUserDisallowedKey);

    // First, remove disallowed from other types
    if (rpc_hmi_permissions.end() != it_hmi_user_disallowed) {
      if (rpc_hmi_permissions.end() != it_hmi_allowed) {
        ExcludeSameHMILevels(rpc_hmi_permissions[kAllowedKey],
                    rpc_hmi_permissions[kUserDisallowedKey]);
      }
      if (rpc_hmi_permissions.end() != it_hmi_undefined) {
        ExcludeSameHMILevels(rpc_hmi_permissions[kUndefinedKey],
                    rpc_hmi_permissions[kUserDisallowedKey]);
      }
    }

    if (rpc_parameter_permissions.end() != it_parameter_user_disallowed) {
      if (rpc_parameter_permissions.end() != it_parameter_allowed) {
        ExcludeSameParameters(rpc_parameter_permissions[kAllowedKey],
                              rpc_parameter_permissions[kUserDisallowedKey]);
      }
      if (rpc_parameter_permissions.end() != it_parameter_undefined) {
        ExcludeSameParameters(rpc_parameter_permissions[kUndefinedKey],
                              rpc_parameter_permissions[kUserDisallowedKey]);
      }
    }

    // Then, remove undefined from allowed
    if (rpc_hmi_permissions.end() != it_hmi_undefined) {
      if (rpc_hmi_permissions.end() != it_hmi_allowed) {
        ExcludeSameHMILevels(rpc_hmi_permissions[kAllowedKey],
                    rpc_hmi_permissions[kUndefinedKey]);
      }
    }

    if (rpc_parameter_permissions.end() != it_parameter_undefined) {
      if (rpc_parameter_permissions.end() != it_parameter_allowed) {
        ExcludeSameParameters(rpc_parameter_permissions[kAllowedKey],
                              rpc_parameter_permissions[kUndefinedKey]);
      }
    }
  }
}

void FillNotificationData::ExcludeSameHMILevels(
    std::set<HMILevel>& source,
    const std::set<HMILevel>& target) {
  std::set<HMILevel> diff_hmi;

  std::set_difference(source.begin(), source.end(),
                      target.begin(), target.end(),
                      std::inserter(diff_hmi, diff_hmi.begin()));

  source = diff_hmi;
}

void FillNotificationData::ExcludeSameParameters(
    std::set<Parameter>& source,
    const std::set<Parameter>& target) {
  std::set<Parameter> diff_parameter;

  std::set_difference(source.begin(), source.end(),
                      target.begin(), target.end(),
                      std::inserter(diff_parameter, diff_parameter.begin()));

  source = diff_parameter;
}

void FillNotificationData::InitRpcKeys(const std::string& rpc_name) {
  data_[rpc_name].hmi_permissions[kAllowedKey];
  data_[rpc_name].hmi_permissions[kUserDisallowedKey];
  data_[rpc_name].parameter_permissions[kAllowedKey];
  data_[rpc_name].parameter_permissions[kUserDisallowedKey];
}

ProcessFunctionalGroup::ProcessFunctionalGroup(
  const policy_table::FunctionalGroupings& fg,
  const std::vector<FunctionalGroupPermission>& group_permissions,
  Permissions& data,
  GroupConsent undefined_group_consent)
  : fg_(fg),
    group_permissions_(group_permissions),
    data_(data),
    undefined_group_consent_(undefined_group_consent) {
}

bool ProcessFunctionalGroup::operator()(const StringsValueType& group_name) {
  const std::string group_name_str = group_name;
  FuncGroupConstItr it = fg_.find(group_name_str);

  if (fg_.end() != it) {
    const policy_table::Rpc& rpcs = (*it).second.rpcs;
    FillNotificationData filler(data_, GetGroupState(group_name_str),
                                undefined_group_consent_);
    std::for_each(rpcs.begin(), rpcs.end(), filler);
  }
  return true;
}

GroupConsent ProcessFunctionalGroup::GetGroupState(
  const std::string& group_name) {
  std::vector<FunctionalGroupPermission>::const_iterator it =
    group_permissions_.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end =
    group_permissions_.end();
  for (; it != it_end; ++it) {
    if (group_name == (*it).group_name) {
      return (*it).state;
    }
  }
  return kGroupUndefined;
}

FunctionalGroupInserter::FunctionalGroupInserter(
  const policy_table::Strings& preconsented_groups, PermissionsList& list)
  : list_(list),
    preconsented_(preconsented_groups) {
}

void FunctionalGroupInserter::operator()(const StringsValueType& group_name) {
  CompareGroupName name(group_name);
  if (std::find_if(preconsented_.begin(), preconsented_.end(), name)
      == preconsented_.end()) {
    list_.push_back(group_name);
  }
}

void FillFunctionalGroupPermissions(
    FunctionalGroupIDs& ids,
    FunctionalGroupNames& names,
    GroupConsent state,
    std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_INFO(logger_, "FillFunctionalGroupPermissions" );
  FunctionalGroupIDs::const_iterator it = ids.begin();
  FunctionalGroupIDs::const_iterator it_end = ids.end();
  for (; it != it_end; ++it) {
    FunctionalGroupPermission current_group;
    current_group.group_id = *it;
    current_group.group_alias = names[*it].first;
    current_group.group_name = names[*it].second;
    current_group.state = state;
    permissions.push_back(current_group);
  }
}

bool IsPredefinedApp(const AppPoliciesValueType& app) {
  return app.first == kDefaultId || app.first == kPreDataConsentId ||
         app.first == kDeviceId;
}

FunctionalGroupIDs ExcludeSame(const FunctionalGroupIDs& from,
                               const FunctionalGroupIDs& what) {
  LOG4CXX_INFO(logger_, "Exclude same groups" );
  FunctionalGroupIDs from_copy(from);
  FunctionalGroupIDs what_copy(what);

  std::sort(from_copy.begin(), from_copy.end());
  std::sort(what_copy.begin(), what_copy.end());

  FunctionalGroupIDs no_same;
  std::set_difference(from_copy.begin(), from_copy.end(),
                      what_copy.begin(),what_copy.end(),
                      std::back_inserter(no_same));

  no_same.resize(std::distance(no_same.begin(),
                               std::unique(no_same.begin(), no_same.end())));

  return no_same;
}

FunctionalGroupIDs Merge(const FunctionalGroupIDs& first,
                         const FunctionalGroupIDs& second) {
  LOG4CXX_INFO(logger_, "Merge groups" );
  FunctionalGroupIDs first_copy(first);
  FunctionalGroupIDs second_copy(second);

  std::sort(first_copy.begin(), first_copy.end());
  std::sort(second_copy.begin(), second_copy.end());

  FunctionalGroupIDs merged;
  std::set_union(first_copy.begin(), first_copy.end(),
                 second_copy.begin(), second_copy.end(),
                 std::back_inserter(merged));

  merged.resize(std::distance(merged.begin(),
                              std::unique(merged.begin(), merged.end())));

  return merged;
}

FunctionalGroupIDs FindSame(const FunctionalGroupIDs& first,
                            const FunctionalGroupIDs& second) {
  LOG4CXX_INFO(logger_, "Find same groups");
  FunctionalGroupIDs first_copy(first);
  FunctionalGroupIDs second_copy(second);

  std::sort(first_copy.begin(), first_copy.end());
  std::sort(second_copy.begin(), second_copy.end());

  FunctionalGroupIDs same;
  std::set_intersection(first_copy.begin(), first_copy.end(),
                        second_copy.begin(), second_copy.end(),
                        std::back_inserter(same));

  same.resize(std::distance(same.begin(),
                            std::unique(same.begin(), same.end())));

  return same;
}

bool UnwrapAppPolicies(policy_table::ApplicationPolicies& app_policies) {
  policy_table::ApplicationPolicies::iterator it = app_policies.begin();
  policy_table::ApplicationPolicies::iterator it_default = app_policies.
                                                           find(kDefaultId);
  for (; app_policies.end() != it; ++it) {
    // Set default policies for app, if there is record like "123":"default"
    if (kDefaultId.compare((*it).second.get_string()) == 0) {
      if (it != app_policies.end()) {
        (*it).second = (*it_default).second;
        it->second.set_to_string(kDefaultId);
      } else {
        LOG4CXX_ERROR(logger_, "There is no default application policy was "
                      "found in PTU.");
        return false;
      }
    }
  }

  return true;
}

}

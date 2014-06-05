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

#define EXTENDED_POLICY 1

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
  PolicyManagerImpl* pm, const utils::SharedPtr<policy_table::Table> update)
  : pm_(pm),
    update_(update) {
}

bool CheckAppPolicy::HasSameGroups(const AppPoliciesValueType& app_policy,
                                   AppPermissions* perms) const {
  const std::string app_id = app_policy.first;
  AppPoliciesConstItr it = pm_->policy_table_snapshot_->policy_table
                           .app_policies.find(app_id);
  if (app_policy.second.is_string()) {
    return (it->second.is_string() &&
            app_policy.second.get_string().compare(it->second.get_string()) == 0);
  } else {
    if (it->second.is_string()) {
      return false;
    }
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
    size_t size = ((it_groups_new_end - new_it) > (it_groups_curr_end - old_it)) ? it_groups_curr_end - old_it : it_groups_new_end - new_it;
    diff = std::mismatch(old_it, old_it + size, new_it, Match);
    if (it_groups_curr_end == diff.first || it_groups_new_end == diff.second) {
      new_it = diff.second;
      old_it = diff.first;
      break;
    }
    if (Compare(*diff.first, *diff.second)) {
      perms->isAppPermissionsRevoked = true;
      perms->appRevokedPermissions.push_back(*(diff.first));
      old_it = ++diff.first;
      new_it = diff.second;
    } else {
      perms->appPermissionsConsentNeeded = true;
      old_it = diff.first;
      new_it = ++diff.second;
    }
  }

  for (StringsConstItr it = old_it; it != it_groups_curr_end; ++it) {
    perms->isAppPermissionsRevoked = true;
    perms->appRevokedPermissions.push_back(*it);
  }

  if (it_groups_new_end != new_it) {
    perms->appPermissionsConsentNeeded = true;
  }

  return !(perms->appRevokedPermissions.size() > 0
           || perms->appPermissionsConsentNeeded);
}

bool CheckAppPolicy::IsNewAppication(const std::string& application_id) const {
  const policy_table::ApplicationPolicies& current_policies = pm_
      ->policy_table_snapshot_->policy_table.app_policies;
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
  pm_->GetUserPermissionsForApp(device_id, app_policy.first, group_permissons);

  pm_->PrepareNotificationData(update_->policy_table.functional_groupings,
                               app_policy.second.groups,
                               group_permissons, notification_data);

  const std::string app_id = app_policy.first;
  LOG4CXX_INFO(logger_, "Send notification for application_id:" << app_id);
#if defined (EXTENDED_POLICY)
  pm_->listener()->OnPermissionsUpdated(app_id, notification_data,
       policy_table::EnumToJsonString(app_policy.second.default_hmi));
#else
  // Default_hmi is Ford-specific and should not be used with basic policy
  const std::string default_hmi;
  pm_->listener()->OnPermissionsUpdated(app_id, notification_data, default_hmi);
#endif
}

void CheckAppPolicy::SendOnPendingPermissions(
  const AppPoliciesValueType& app_policy, AppPermissions permissions) const {
  // TODO(AOleynik): Exclude default group(s)
  if (permissions.appPermissionsConsentNeeded) {
#if defined(EXTENDED_POLICY)
    const policy_table::Strings& groups = app_policy.second.groups;
    // TODO(IKozyrenko): Check logic if optional container is missing
    const policy_table::Strings& preconsented_groups = *app_policy.second
        .preconsented_groups;

    // TODO(KKolodiy): Use consent_groups to filtrate groups
    PermissionsList list_of_permissions;
    std::for_each(
      groups.begin(), groups.end(),
      FunctionalGroupInserter(preconsented_groups, list_of_permissions));
    // TODO(PV): logic has changed.
    if (!list_of_permissions.empty()) {
      pm_->app_permissions_diff_.insert(
        std::make_pair(app_policy.first, permissions));
      pm_->listener()->OnPendingPermissionChange(app_policy.first);
      return;
    }
#endif
  }
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

bool CheckAppPolicy::NicknamesMatch(const std::string app_id,
                                    const AppPoliciesValueType& app_policy) const {
  std::string app_name = pm_->listener()->GetAppName(app_id);
  if (!app_name.empty() && app_policy.second.nicknames && !app_policy.second.nicknames->empty()) {
    for (policy_table::Strings::const_iterator it = app_policy.second.nicknames->begin();
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
  policy_table::ApplicationPolicies& current_policies = pm_
      ->policy_table_snapshot_->policy_table.app_policies;

  const std::string app_id = app_policy.first;

  AppPermissions permissions_diff(app_id);
#if defined (EXTENDED_POLICY)
  permissions_diff.priority = policy_table::EnumToJsonString(
                                app_policy.second.priority);
#endif

  // Check revocation for any numeric application ids
  if (atoi(app_id.c_str()) && IsAppRevoked(app_policy)) {
    permissions_diff.appRevoked = true;
    pm_->app_permissions_diff_.insert(std::make_pair(app_id, permissions_diff));
    pm_->listener()->OnAppRevoked(app_id);
    policy_table::ApplicationPolicies::iterator it = current_policies.find(
          app_id);
    // Update snapshot with new policies for application
    if (it != current_policies.end()) {
      (*it).second = app_policy.second;
      it->second.set_to_null();
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

  if (!NicknamesMatch(app_id, app_policy)) {
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
    LOG4CXX_INFO(
      logger_,
      "Permissions for application:" << app_id << " wasn't changed.");
    return true;
  }

  policy_table::ApplicationPolicies::iterator it = current_policies.find(
        app_id);
  // Update snapshot with new policies for application
  (*it).second = app_policy.second;

  // Don't sent notification for "default"
  if (kDefaultId != app_id && kDeviceId != app_id) {
    SendNotification(app_policy);
    SendOnPendingPermissions(app_policy, permissions_diff);
  }

  // if 'device' was update with new/other func groups => user consent
  // for device should be cleared.
  if (kDeviceId == app_id) {
#if defined (EXTENDED_POLICY)
    // TODO(AOleynik): Check design for more convenient access to policy ext data
    PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(pm_->policy_table_
                                  .pt_data().get());
    if (pt_ext) {
      if (!pt_ext->ResetDeviceConsents()) {
        return false;
      }
    }
#endif
  }

  return true;
}

FillNotificationData::FillNotificationData(Permissions& data,
                                           GroupConsent group_state)
  : data_(data) {
  switch (group_state) {
  case kGroupAllowed:
    current_key_ = kAllowedKey;
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
    ExcludeDisAllowed();
  } else {
    // If rpc is not present - add its permissions
    UpdateHMILevels(rpc.second.hmi_levels,
                    data_[rpc.first].hmi_permissions[current_key_]);
    // TODO(IKozyrenko): Check logic if optional container is missing
    UpdateParameters(*rpc.second.parameters,
                     data_[rpc.first].parameter_permissions[current_key_]);
    ExcludeDisAllowed();
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

void FillNotificationData::ExcludeDisAllowed() {
  // Search through filled data and remove permission from allowed, if it was
  // found in disallowed section
  Permissions::iterator it = data_.begin();
  Permissions::const_iterator it_end = data_.end();
  // Groups
  for (; it != it_end; ++it) {
    std::set<HMILevel>& allowed_hmi_level = (*it).second.hmi_permissions[kAllowedKey];
    std::set<HMILevel>& disallowed_hmi_level =
        (*it).second.hmi_permissions[kUserDisallowedKey];
    std::set<HMILevel> diff_hmi;

    std::set_difference(allowed_hmi_level.begin(), allowed_hmi_level.end(),
                        disallowed_hmi_level.begin(), disallowed_hmi_level.end(),
                        std::inserter(diff_hmi, diff_hmi.begin()));
    // Leave levels, which are not present in disallowed
    allowed_hmi_level = diff_hmi;

    std::set<Parameter>& allowed_parameters =
        (*it).second.parameter_permissions[kAllowedKey];
    std::set<Parameter>& disallowed_parameters =
        (*it).second.parameter_permissions[kUserDisallowedKey];

    std::set<Parameter> diff_params;

    std::set_difference(allowed_parameters.begin(), allowed_parameters.end(),
                        disallowed_parameters.begin(), disallowed_parameters.end(),
                        std::inserter(diff_params, diff_params.begin()));

    // Leave parameters, which are not present in disallowed
    allowed_parameters = diff_params;
  }
}

ProcessFunctionalGroup::ProcessFunctionalGroup(
    const policy_table::FunctionalGroupings& fg,
    const std::vector<FunctionalGroupPermission>& group_permissions,
    Permissions& data)
  : fg_(fg),
    group_permissions_(group_permissions),
    data_(data) {
}

bool ProcessFunctionalGroup::operator()(const StringsValueType& group_name) {
  const std::string group_name_str = group_name;
  FuncGroupConstItr it = fg_.find(group_name_str);

  if (fg_.end() != it) {
    const policy_table::Rpc& rpcs = (*it).second.rpcs;
    FillNotificationData filler(data_, GetGroupState(group_name_str));
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

void FillFunctionalGroupPermissions(FunctionalGroupIDs& ids,
    FunctionalGroupNames& names,
    GroupConsent state,
    std::vector<FunctionalGroupPermission>& permissions) {
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

}

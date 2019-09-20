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

#include "policy/policy_helper.h"
#include <string.h>
#include <algorithm>
#include <sstream>
#include "policy/policy_manager_impl.h"
#include "utils/custom_string.h"
#include "utils/logger.h"

namespace policy {

namespace custom_str = utils::custom_string;

namespace {

CREATE_LOGGERPTR_GLOBAL(logger_, "Policy")

bool CompareStrings(const StringsValueType& first,
                    const StringsValueType& second) {
  const std::string& first_str = first;
  const std::string& second_str = second;
  return (strcasecmp(first_str.c_str(), second_str.c_str()) < 0);
}

struct CheckGroupName {
  CheckGroupName(const policy::StringsValueType& value) : value_(value) {}

  bool operator()(const FunctionalGroupNames::value_type& value) {
    return value.second.second == std::string(value_);
  }

 private:
  const policy::StringsValueType& value_;
};

struct CopyAttributes {
  CopyAttributes(const FunctionalGroupNames& groups_attributes,
                 std::vector<FunctionalGroupPermission>& groups_permissions)
      : groups_attributes_(groups_attributes)
      , groups_permissions_(groups_permissions) {}

  bool operator()(const policy::StringsValueType& value) {
    CheckGroupName checker(value);
    FunctionalGroupNames::const_iterator it = std::find_if(
        groups_attributes_.begin(), groups_attributes_.end(), checker);
    if (groups_attributes_.end() == it) {
      return false;
    }
    FunctionalGroupPermission group;
    group.group_name = it->second.second;
    group.group_alias = it->second.first;
    group.group_id = it->first;
    groups_permissions_.push_back(group);
    return true;
  }

 private:
  const FunctionalGroupNames& groups_attributes_;
  std::vector<FunctionalGroupPermission>& groups_permissions_;
};
}  // namespace

CompareGroupName::CompareGroupName(const StringsValueType& group_name)
    : group_name_(group_name) {}

bool CompareGroupName::operator()(
    const StringsValueType& group_name_to_compare) const {
  const std::string gn_ = group_name_;
  const std::string gn_compare = group_name_to_compare;
  return !(strcasecmp(gn_.c_str(), gn_compare.c_str()));
}

bool operator!=(const policy_table::ApplicationParams& first,
                const policy_table::ApplicationParams& second) {
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
    if (it_second_end == it) {
      return true;
    }
  }
  return false;
}

CheckAppPolicy::CheckAppPolicy(
    PolicyManagerImpl* pm,
    const std::shared_ptr<policy_table::Table> update,
    const std::shared_ptr<policy_table::Table> snapshot,
    CheckAppPolicyResults& out_results)
    : pm_(pm)
    , update_(update)
    , snapshot_(snapshot)
    , out_results_(out_results) {}

bool policy::CheckAppPolicy::HasRevokedGroups(
    const policy::AppPoliciesValueType& app_policy,
    policy_table::Strings* revoked_groups) const {
  AppPoliciesConstItr it =
      snapshot_->policy_table.app_policies_section.apps.find(app_policy.first);

  policy_table::Strings groups_new = app_policy.second.groups;
  std::sort(groups_new.begin(), groups_new.end(), CompareStrings);

  policy_table::Strings groups_curr = (*it).second.groups;
  std::sort(groups_curr.begin(), groups_curr.end(), CompareStrings);

  StringsConstItr it_groups_new = groups_new.begin();
  StringsConstItr it_groups_new_end = groups_new.end();

  StringsConstItr it_groups_curr = groups_curr.begin();
  StringsConstItr it_groups_curr_end = groups_curr.end();

  policy_table::Strings revoked_group_list;
  std::set_difference(it_groups_curr,
                      it_groups_curr_end,
                      it_groups_new,
                      it_groups_new_end,
                      std::back_inserter(revoked_group_list),
                      CompareStrings);

  if (revoked_groups) {
    *revoked_groups = revoked_group_list;
  }

  return !revoked_group_list.empty();
}

bool policy::CheckAppPolicy::HasUpdatedGroups(
    const policy::AppPoliciesValueType& app_policy,
    policy_table::Strings* updated_groups) const {
  AppPoliciesConstItr it =
      snapshot_->policy_table.app_policies_section.apps.find(app_policy.first);

  policy_table::Strings groups_new = app_policy.second.groups;
  std::sort(groups_new.begin(), groups_new.end(), CompareStrings);

  policy_table::Strings groups_curr = (*it).second.groups;
  std::sort(groups_curr.begin(), groups_curr.end(), CompareStrings);

  policy_table::Strings intersection_list;
  std::set_intersection(groups_new.begin(),
                        groups_new.end(),
                        groups_curr.begin(),
                        groups_curr.end(),
                        std::back_inserter(intersection_list));

  auto IsGroupContentUpdated =
      [this](const StringsValueType& groupName) -> bool {
    const auto& func_group_from_update =
        update_->policy_table.functional_groupings.find(groupName);
    const auto& func_group_from_snapshot =
        snapshot_->policy_table.functional_groupings.find(groupName);

    const auto& update_fg_rpcs = func_group_from_update->second.rpcs;
    const auto& snapshot_fg_rpcs = func_group_from_snapshot->second.rpcs;

    if (update_fg_rpcs.is_null() || snapshot_fg_rpcs.is_null()) {
      return !(update_fg_rpcs.is_null() && snapshot_fg_rpcs.is_null());
    }

    if (update_fg_rpcs.size() != snapshot_fg_rpcs.size()) {
      return true;
    }

    for (const auto& rpc : update_fg_rpcs) {
      const auto& old_rpc = snapshot_fg_rpcs.find(rpc.first);
      if (snapshot_fg_rpcs.end() == old_rpc) {
        return true;
      }

      const bool hmi_levels_same =
          old_rpc->second.hmi_levels == rpc.second.hmi_levels;
      const bool parameters_same =
          *(old_rpc->second.parameters) == *(rpc.second.parameters);

      if (!hmi_levels_same || !parameters_same) {
        return true;
      }
    }

    return false;
  };

  policy_table::Strings updated_group_list;
  for (const auto& item : intersection_list) {
    if (IsGroupContentUpdated(item)) {
      updated_group_list.push_back(item);
    }
  }

  if (updated_groups) {
    *updated_groups = updated_group_list;
  }

  return !updated_group_list.empty();
}

bool policy::CheckAppPolicy::HasNewGroups(
    const policy::AppPoliciesValueType& app_policy,
    policy_table::Strings* new_groups) const {
  AppPoliciesConstItr it =
      snapshot_->policy_table.app_policies_section.apps.find(app_policy.first);

  policy_table::Strings groups_new = app_policy.second.groups;
  std::sort(groups_new.begin(), groups_new.end(), CompareStrings);

  policy_table::Strings groups_curr = (*it).second.groups;
  std::sort(groups_curr.begin(), groups_curr.end(), CompareStrings);

  StringsConstItr it_groups_new = groups_new.begin();
  StringsConstItr it_groups_new_end = groups_new.end();

  StringsConstItr it_groups_curr = groups_curr.begin();
  StringsConstItr it_groups_curr_end = groups_curr.end();

  policy_table::Strings new_group_list;
  std::set_difference(it_groups_new,
                      it_groups_new_end,
                      it_groups_curr,
                      it_groups_curr_end,
                      std::back_inserter(new_group_list),
                      CompareStrings);

  if (new_groups) {
    *new_groups = new_group_list;
  }

  return !new_group_list.empty();
}

bool policy::CheckAppPolicy::HasConsentNeededGroups(
    const policy::AppPoliciesValueType& app_policy) const {
  policy_table::Strings new_groups;
  policy_table::Strings updated_groups;
  if (!HasNewGroups(app_policy, &new_groups) &&
      !HasUpdatedGroups(app_policy, &updated_groups)) {
    return false;
  }

  policy_table::Strings groups_to_check_consent;
  std::set_union(new_groups.begin(),
                 new_groups.end(),
                 updated_groups.begin(),
                 updated_groups.end(),
                 std::back_inserter(groups_to_check_consent));

  StringsConstItr it = groups_to_check_consent.begin();
  StringsConstItr it_end = groups_to_check_consent.end();
  for (; it != it_end; ++it) {
    if (IsConsentRequired(app_policy.first, *it)) {
      return true;
    }
  }

  return false;
}

std::vector<FunctionalGroupPermission> policy::CheckAppPolicy::GetRevokedGroups(
    const policy::AppPoliciesValueType& app_policy) const {
  policy_table::Strings revoked_groups_names;
  if (!HasRevokedGroups(app_policy, &revoked_groups_names)) {
    return std::vector<FunctionalGroupPermission>();
  }

  FunctionalGroupNames groups_attributes;
  if (!pm_->cache_->GetFunctionalGroupNames(groups_attributes)) {
    LOG4CXX_WARN(logger_, "Can't get functional group names");
    return std::vector<FunctionalGroupPermission>();
  }

  std::vector<FunctionalGroupPermission> revoked_groups_permissions;
  CopyAttributes copier(groups_attributes, revoked_groups_permissions);
  std::for_each(
      revoked_groups_names.begin(), revoked_groups_names.end(), copier);

  return revoked_groups_permissions;
}

void policy::CheckAppPolicy::RemoveRevokedConsents(
    const AppPoliciesValueType& app_policy,
    const std::vector<FunctionalGroupPermission>& revoked_groups) const {
  std::vector<policy::FunctionalGroupPermission>::const_iterator it =
      revoked_groups.begin();
  std::vector<policy::FunctionalGroupPermission>::const_iterator it_end =
      revoked_groups.end();
  for (; it != it_end; ++it) {
    pm_->RemoveAppConsentForGroup(app_policy.first, it->group_name);
  }
}

bool CheckAppPolicy::IsKnownAppication(
    const std::string& application_id) const {
  const policy_table::ApplicationPolicies& current_policies =
      snapshot_->policy_table.app_policies_section.apps;

  return !(current_policies.end() == current_policies.find(application_id));
}

void policy::CheckAppPolicy::NotifySystem(
    const policy::AppPoliciesValueType& app_policy) const {
  auto& listener = *pm_->listener();
  const auto devices_ids = listener.GetDevicesIds(app_policy.first);
  if (devices_ids.empty()) {
    LOG4CXX_WARN(
        logger_,
        "Couldn't find device info for application id: " << app_policy.first);
    return;
  }

  for (const auto& device_id : devices_ids) {
    listener.OnPendingPermissionChange(device_id, app_policy.first);
  }
}

bool CheckAppPolicy::IsAppRevoked(
    const AppPoliciesValueType& app_policy) const {
  LOG4CXX_AUTO_TRACE(logger_);
  // Application params are not initialized = application revoked
  // i.e. "123":null
  return app_policy.second.is_null();
}

bool CheckAppPolicy::NicknamesMatch(
    const AppPoliciesValueType& app_policy) const {
  const std::string& app_id = app_policy.first;
  const custom_str::CustomString app_name = pm_->listener()->GetAppName(app_id);
  if (!app_name.empty() && app_policy.second.nicknames &&
      !app_policy.second.nicknames->empty()) {
    for (policy_table::Strings::const_iterator it =
             app_policy.second.nicknames->begin();
         app_policy.second.nicknames->end() != it;
         ++it) {
      std::string temp = *it;
      if (app_name.CompareIgnoreCase(temp.c_str())) {
        return true;
      }
    }
    return false;
  }
  return true;
}

void CheckAppPolicy::AddResult(const std::string& app_id,
                               PermissionsCheckResult result) {
  LOG4CXX_AUTO_TRACE(logger_);
  const auto item = std::make_pair(app_id, result);
  out_results_.insert(item);
}

bool CheckAppPolicy::operator()(const AppPoliciesValueType& app_policy) {
  const std::string app_id = app_policy.first;

  if (!IsKnownAppication(app_id)) {
    LOG4CXX_WARN(logger_,
                 "Application:" << app_id << " is not present in snapshot.");
    return true;
  }

  if (!IsPredefinedApp(app_policy) && IsAppRevoked(app_policy)) {
    SetPendingPermissions(app_policy, RESULT_APP_REVOKED);
    AddResult(app_id, RESULT_APP_REVOKED);
    return true;
  }

  if (!IsPredefinedApp(app_policy) && !NicknamesMatch(app_policy)) {
    SetPendingPermissions(app_policy, RESULT_NICKNAME_MISMATCH);
    AddResult(app_id, RESULT_NICKNAME_MISMATCH);
    return true;
  }

  PermissionsCheckResult result = CheckPermissionsChanges(app_policy);
  if (!IsPredefinedApp(app_policy)) {
    const bool is_request_type_changed = IsRequestTypeChanged(app_policy);
    const bool is_request_subtype_changed = IsRequestSubTypeChanged(app_policy);

    if (is_request_type_changed) {
      SetPendingPermissions(app_policy, RESULT_REQUEST_TYPE_CHANGED);
      AddResult(app_id, RESULT_REQUEST_TYPE_CHANGED);
    }
    if (is_request_subtype_changed) {
      SetPendingPermissions(app_policy, RESULT_REQUEST_SUBTYPE_CHANGED);
      AddResult(app_id, RESULT_REQUEST_SUBTYPE_CHANGED);
    }
  }
  if (RESULT_NO_CHANGES == result) {
    LOG4CXX_INFO(
        logger_,
        "Permissions for application:" << app_id << " wasn't changed.");
    AddResult(app_id, RESULT_NO_CHANGES);
    return true;
  }

  LOG4CXX_INFO(
      logger_,
      "Permissions for application:" << app_id << " have been changed.");

  if (IsPredefinedApp(app_policy)) {
    const auto& snapshot_app_policy_begin =
        snapshot_->policy_table.app_policies_section.apps.begin();
    const auto& snapshot_app_policy_end =
        snapshot_->policy_table.app_policies_section.apps.end();

    auto find_app = [&app_id](AppPoliciesValueType& app) {
      return app_id == app.second.get_string();
    };

    const auto& app = std::find_if(
        snapshot_app_policy_begin, snapshot_app_policy_end, find_app);

    if ((snapshot_app_policy_end != app) &&
        (RESULT_CONSENT_NOT_REQUIRED != result)) {
      SetPendingPermissions(*app, result);
      AddResult(app_id, RESULT_CONSENT_NEEDED);
    }
    return true;
  }

  SetPendingPermissions(app_policy, result);
  if (RESULT_CONSENT_NOT_REQUIRED != result) {
    AddResult(app_id, RESULT_CONSENT_NEEDED);
    return true;
  }

  AddResult(app_id, result);
  return true;
}

void policy::CheckAppPolicy::SetPendingPermissions(
    const AppPoliciesValueType& app_policy,
    PermissionsCheckResult result) const {
  const std::string app_id = app_policy.first;
  AppPermissions permissions_diff(app_id);
  permissions_diff.priority =
      policy_table::EnumToJsonString(app_policy.second.priority);

  switch (result) {
    case RESULT_APP_REVOKED:
      permissions_diff.appRevoked = true;
      break;
    case RESULT_NICKNAME_MISMATCH:
      permissions_diff.appUnauthorized = true;
      break;
    case RESULT_PERMISSIONS_REVOKED:
      permissions_diff.isAppPermissionsRevoked = true;
      permissions_diff.appRevokedPermissions = GetRevokedGroups(app_policy);
      RemoveRevokedConsents(app_policy, permissions_diff.appRevokedPermissions);
      break;
    case RESULT_CONSENT_NEEDED:
      permissions_diff.appPermissionsConsentNeeded = true;
      break;
    case RESULT_PERMISSIONS_REVOKED_AND_CONSENT_NEEDED:
      permissions_diff.isAppPermissionsRevoked = true;
      permissions_diff.appPermissionsConsentNeeded = true;
      permissions_diff.appRevokedPermissions = GetRevokedGroups(app_policy);
      RemoveRevokedConsents(app_policy, permissions_diff.appRevokedPermissions);
      break;
    case RESULT_REQUEST_TYPE_CHANGED:
      permissions_diff.priority.clear();
      permissions_diff.requestTypeChanged = true;

      // Getting Request Types from PTU (not from cache)
      for (const auto& request_type : *app_policy.second.RequestType) {
        permissions_diff.requestType.push_back(EnumToJsonString(request_type));
      }
      break;
    case RESULT_REQUEST_SUBTYPE_CHANGED:
      permissions_diff.priority.clear();
      permissions_diff.requestSubTypeChanged = true;
      // Getting Request SubTypes from PTU (not from cache)
      for (const auto& request_subtype : *app_policy.second.RequestSubType) {
        permissions_diff.requestSubType.push_back(request_subtype);
      }
      break;
    default:
      return;
  }
  pm_->app_permissions_diff_lock_.Acquire();
  pm_->app_permissions_diff_.insert(std::make_pair(app_id, permissions_diff));
  pm_->app_permissions_diff_lock_.Release();
}

policy::PermissionsCheckResult policy::CheckAppPolicy::CheckPermissionsChanges(
    const policy::AppPoliciesValueType& app_policy) const {
  const bool has_revoked_groups = HasRevokedGroups(app_policy);

  const bool has_consent_needed_groups = HasConsentNeededGroups(app_policy);

  const bool has_new_groups = HasNewGroups(app_policy);

  const bool has_updated_groups = HasUpdatedGroups(app_policy);

  const bool encryption_required_flag_changed =
      IsEncryptionRequiredFlagChanged(app_policy);

  if (has_revoked_groups && has_consent_needed_groups) {
    return RESULT_PERMISSIONS_REVOKED_AND_CONSENT_NEEDED;
  } else if (has_revoked_groups) {
    return RESULT_PERMISSIONS_REVOKED;
  } else if (has_consent_needed_groups) {
    return RESULT_CONSENT_NEEDED;
  } else if (has_new_groups || has_updated_groups) {
    return RESULT_CONSENT_NOT_REQUIRED;
  } else if (encryption_required_flag_changed) {
    return RESULT_ENCRYPTION_REQUIRED_FLAG_CHANGED;
  }

  return RESULT_NO_CHANGES;
}

bool CheckAppPolicy::IsConsentRequired(const std::string& app_id,
                                       const std::string& group_name) const {
  const policy_table::FunctionalGroupings& functional_groupings =
      snapshot_->policy_table.functional_groupings;

  FuncGroupConstItr it = functional_groupings.find(group_name);

  if (functional_groupings.end() == it) {
    return false;
  }

  bool is_preconsented = false;
  return it->second.user_consent_prompt.is_initialized() && !is_preconsented;
}

bool CheckAppPolicy::IsRequestTypeChanged(
    const AppPoliciesValueType& app_policy) const {
  policy::AppPoliciesConstItr it =
      snapshot_->policy_table.app_policies_section.apps.find(app_policy.first);
  if (it == snapshot_->policy_table.app_policies_section.apps.end()) {
    if (!app_policy.second.RequestType->empty()) {
      return true;
    }
    return false;
  }
  if (it->second.RequestType->size() != app_policy.second.RequestType->size()) {
    return true;
  }
  policy_table::RequestTypes diff;
  std::set_difference(it->second.RequestType->begin(),
                      it->second.RequestType->end(),
                      app_policy.second.RequestType->begin(),
                      app_policy.second.RequestType->end(),
                      std::back_inserter(diff));
  return diff.size();
}

bool CheckAppPolicy::IsRequestSubTypeChanged(
    const AppPoliciesValueType& app_policy) const {
  policy::AppPoliciesConstItr it =
      snapshot_->policy_table.app_policies_section.apps.find(app_policy.first);

  if (it == snapshot_->policy_table.app_policies_section.apps.end()) {
    if (!app_policy.second.RequestSubType->empty()) {
      return true;
    }
    return false;
  }

  if (it->second.RequestSubType->size() !=
      app_policy.second.RequestSubType->size()) {
    return true;
  }

  policy_table::RequestSubTypes diff;
  std::set_difference(it->second.RequestSubType->begin(),
                      it->second.RequestSubType->end(),
                      app_policy.second.RequestSubType->begin(),
                      app_policy.second.RequestSubType->end(),
                      std::back_inserter(diff));
  return diff.size();
}

bool CheckAppPolicy::IsEncryptionRequiredFlagChanged(
    const AppPoliciesValueType& app_policy) const {
  auto get_app_encryption_needed =
      [](const std::string& policy_app_id,
         policy_table::ApplicationPolicies& policies)
      -> rpc::Optional<rpc::Boolean> {
    auto it = policies.find(policy_app_id);
    if (policies.end() == it) {
      LOG4CXX_WARN(logger_, "App is not present in policies" << policy_app_id);
      return rpc::Optional<rpc::Boolean>(false);
    }
    return it->second.encryption_required;
  };

  auto get_app_groups =
      [](const std::string& policy_app_id,
         policy_table::ApplicationPolicies& policies) -> policy_table::Strings {
    policy_table::Strings result;
    auto it = policies.find(policy_app_id);
    if (policies.end() == it) {
      LOG4CXX_WARN(logger_, "App is not present in policies" << policy_app_id);
      return result;
    }
    auto& groups = it->second.groups;
    std::copy(groups.begin(), groups.end(), std::back_inserter(result));
    return result;
  };

  auto get_app_rpcs = [](const std::string group_name,
                         const FunctionalGroupings& groups)
      -> rpc::Optional<policy_table::Rpcs> {
    auto it = groups.find(group_name);
    if (it == groups.end()) {
      return rpc::Optional<policy_table::Rpcs>();
    }
    return rpc::Optional<policy_table::Rpcs>(it->second);
  };

  const auto snapshot_groups = get_app_groups(
      app_policy.first, snapshot_->policy_table.app_policies_section.apps);
  const auto update_groups = get_app_groups(
      app_policy.first, update_->policy_table.app_policies_section.apps);

  auto get_resulting_encryption_required_flag_for_app_groups =
      [this, &get_app_rpcs](
          const rpc::policy_table_interface_base::Strings& app_groups,
          const std::shared_ptr<rpc::policy_table_interface_base::Table> pt) {
        for (const auto& group : app_groups) {
          const auto rpcs =
              get_app_rpcs(group, pt->policy_table.functional_groupings);
          if (*rpcs->encryption_required) {
            return true;
          }
        }

        return false;
      };

  auto group_res_en_flag_changed =
      [this, &get_resulting_encryption_required_flag_for_app_groups](
          const rpc::policy_table_interface_base::Strings& snapshot_groups,
          const rpc::policy_table_interface_base::Strings& update_groups) {
        return get_resulting_encryption_required_flag_for_app_groups(
                   snapshot_groups, snapshot_) !=
               get_resulting_encryption_required_flag_for_app_groups(
                   update_groups, update_);
      };

  const auto snapshot_app_encryption_needed = get_app_encryption_needed(
      app_policy.first, snapshot_->policy_table.app_policies_section.apps);
  const auto update_app_encryption_needed = get_app_encryption_needed(
      app_policy.first, update_->policy_table.app_policies_section.apps);

  const bool app_encryption_needed_changed =
      (snapshot_app_encryption_needed.is_initialized() !=
       update_app_encryption_needed.is_initialized()) ||
      (*snapshot_app_encryption_needed != *update_app_encryption_needed);

  if ((!update_app_encryption_needed.is_initialized() ||
       *update_app_encryption_needed) &&
      group_res_en_flag_changed(snapshot_groups, update_groups)) {
    return true;
  }

  return app_encryption_needed_changed;
}

void FillActionsForAppPolicies::operator()(
    const policy::CheckAppPolicyResults::value_type& value) {
  const std::string app_id = value.first;
  const auto app_policy = app_policies_.find(app_id);

  if (app_policies_.end() == app_policy) {
    return;
  }

  if (IsPredefinedApp(*app_policy)) {
    return;
  }

  switch (value.second) {
    case RESULT_APP_REVOKED:
    case RESULT_NICKNAME_MISMATCH:
      actions_[app_id].is_notify_system = true;
      return;
    case RESULT_CONSENT_NEEDED:
    case RESULT_PERMISSIONS_REVOKED_AND_CONSENT_NEEDED:
      actions_[app_id].is_consent_needed = true;
      break;
    case RESULT_CONSENT_NOT_REQUIRED:
    case RESULT_PERMISSIONS_REVOKED:
    case RESULT_REQUEST_TYPE_CHANGED:
    case RESULT_REQUEST_SUBTYPE_CHANGED:
    case RESULT_ENCRYPTION_REQUIRED_FLAG_CHANGED:
      break;
    case RESULT_NO_CHANGES:
    default:
      return;
  }

  actions_[app_id].is_notify_system = true;
  actions_[app_id].is_send_permissions_to_app = true;
}

FillNotificationData::FillNotificationData(Permissions& data,
                                           GroupConsent group_state,
                                           GroupConsent undefined_group_consent)
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
      current_key_ = kGroupAllowed == undefined_group_consent
                         ? kAllowedKey
                         : kUserDisallowedKey;
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
    out_parameter.insert(*it_parameters);
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
    std::set<HMILevel>& source, const std::set<HMILevel>& target) {
  std::set<HMILevel> diff_hmi;

  std::set_difference(source.begin(),
                      source.end(),
                      target.begin(),
                      target.end(),
                      std::inserter(diff_hmi, diff_hmi.begin()));

  source = diff_hmi;
}

void FillNotificationData::ExcludeSameParameters(
    std::set<Parameter>& source, const std::set<Parameter>& target) {
  std::set<Parameter> diff_parameter;

  std::set_difference(source.begin(),
                      source.end(),
                      target.begin(),
                      target.end(),
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
    : fg_(fg)
    , group_permissions_(group_permissions)
    , data_(data)
    , undefined_group_consent_(undefined_group_consent) {}

bool ProcessFunctionalGroup::operator()(const StringsValueType& group_name) {
  const std::string group_name_str = group_name;
  FuncGroupConstItr it = fg_.find(group_name_str);

  if (fg_.end() != it) {
    const policy_table::Rpc& rpcs = (*it).second.rpcs;
    FillNotificationData filler(
        data_, GetGroupState(group_name_str), undefined_group_consent_);
    std::for_each(rpcs.begin(), rpcs.end(), filler);
    FillEncryptionFlagForRpcs(rpcs, (*it).second.encryption_required);
  }
  return true;
}

void ProcessFunctionalGroup::FillEncryptionFlagForRpcs(
    const policy_table::Rpc& rpcs,
    const EncryptionRequired encryption_required) {
  auto update_encryption_required = [](EncryptionRequired& current,
                                       const EncryptionRequired& incoming) {
    if (!incoming.is_initialized()) {
      return;
    }
    if (current.is_initialized() && *current) {
      return;
    }
    current = incoming;
  };

  for (const auto& rpc : rpcs) {
    auto& item = data_[rpc.first];
    update_encryption_required(item.require_encryption, encryption_required);
  }
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
    : list_(list), preconsented_(preconsented_groups) {}

void FunctionalGroupInserter::operator()(const StringsValueType& group_name) {
  CompareGroupName name(group_name);
  if (std::find_if(preconsented_.begin(), preconsented_.end(), name) ==
      preconsented_.end()) {
    list_.push_back(group_name);
  }
}

void FillFunctionalGroupPermissions(
    FunctionalGroupIDs& ids,
    FunctionalGroupNames& names,
    GroupConsent state,
    std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
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
  LOG4CXX_AUTO_TRACE(logger_);
  FunctionalGroupIDs from_copy(from);
  FunctionalGroupIDs what_copy(what);

  std::sort(from_copy.begin(), from_copy.end());
  std::sort(what_copy.begin(), what_copy.end());

  FunctionalGroupIDs no_same;
  std::set_difference(from_copy.begin(),
                      from_copy.end(),
                      what_copy.begin(),
                      what_copy.end(),
                      std::back_inserter(no_same));

  no_same.resize(std::distance(no_same.begin(),
                               std::unique(no_same.begin(), no_same.end())));

  return no_same;
}

FunctionalGroupIDs Merge(const FunctionalGroupIDs& first,
                         const FunctionalGroupIDs& second) {
  LOG4CXX_AUTO_TRACE(logger_);
  FunctionalGroupIDs first_copy(first);
  FunctionalGroupIDs second_copy(second);

  std::sort(first_copy.begin(), first_copy.end());
  std::sort(second_copy.begin(), second_copy.end());

  FunctionalGroupIDs merged;
  std::set_union(first_copy.begin(),
                 first_copy.end(),
                 second_copy.begin(),
                 second_copy.end(),
                 std::back_inserter(merged));

  merged.resize(
      std::distance(merged.begin(), std::unique(merged.begin(), merged.end())));

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
  std::set_intersection(first_copy.begin(),
                        first_copy.end(),
                        second_copy.begin(),
                        second_copy.end(),
                        std::back_inserter(same));

  same.resize(
      std::distance(same.begin(), std::unique(same.begin(), same.end())));

  return same;
}

bool UnwrapAppPolicies(policy_table::ApplicationPolicies& app_policies) {
  policy_table::ApplicationPolicies::iterator it = app_policies.begin();
  policy_table::ApplicationPolicies::iterator it_default =
      app_policies.find(kDefaultId);
  for (; app_policies.end() != it; ++it) {
    // Set default policies for app, if there is record like "123":"default"
    if (kDefaultId.compare((*it).second.get_string()) == 0) {
      if (it != app_policies.end()) {
        (*it).second = (*it_default).second;
        it->second.set_to_string(kDefaultId);
      } else {
        LOG4CXX_ERROR(logger_,
                      "There is no default application policy was "
                      "found in PTU.");
        return false;
      }
    }
  }

  return true;
}
}  // namespace policy

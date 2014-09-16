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
#include <set>
#include <iterator>
#include "json/reader.h"
#include "json/writer.h"
#include "policy/policy_table.h"
#include "policy/pt_representation.h"
#include "policy/policy_manager_impl.h"
#include "policy/policy_helper.h"
#include "utils/file_system.h"
#include "utils/logger.h"

policy::PolicyManager* CreateManager() {
  return new policy::PolicyManagerImpl();
}

namespace {

struct CheckGroupName {
  CheckGroupName(const std::string& name)
    : name_(name) {
  }

  bool operator()(const policy::FunctionalGroupPermission& value) {
    return value.group_name == name_;
  }

private:
  const std::string& name_;
};

struct CopyPermissions{
  CopyPermissions(const std::vector<policy::FunctionalGroupPermission>& groups)
    : groups_(groups) {
  }

bool operator()(policy::FunctionalGroupPermission& value) {
  CheckGroupName checker(value.group_name);
  std::vector<policy::FunctionalGroupPermission>::const_iterator it =
      std::find_if(groups_.begin(), groups_.end(), checker);
  if (groups_.end() == it) {
    return false;
  }
  value.group_alias = it->group_alias;
  value.group_id = it->group_id;
  value.state = it->state;
  return true;
}

private:
  const std::vector<policy::FunctionalGroupPermission>& groups_;
};

}

namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "PolicyManagerImpl")

#if defined(__QNXNTO__) and defined(GCOV_ENABLED)
bool CheckGcov() {
  LOG4CXX_ERROR(log4cxx::Logger::getLogger("appMain"),
                "Attention! This application was built with unsupported "
                "configuration (gcov + QNX). Use it at your own risk.");
  return true;
}
bool check_gcov = CheckGcov();
#endif

PolicyManagerImpl::PolicyManagerImpl()
  : PolicyManager(),
    listener_(NULL),
    retry_sequence_timeout_(60),
    retry_sequence_index_(0) {
}

void PolicyManagerImpl::ResetDefaultPT(const PolicyTable& policy_table) {
  policy_table_ = policy_table;
  update_status_manager_.OnResetDefaultPT(cache.UpdateRequired());

  retry_sequence_index_ = 0;
  RefreshRetrySequence();
}

void PolicyManagerImpl::set_listener(PolicyListener* listener) {
  listener_ = listener;
  update_status_manager_.set_listener(listener);
}

PolicyManagerImpl::~PolicyManagerImpl() {
  LOG4CXX_INFO(logger_, "Destroying policy manager.");
    const bool update_required =
        policy::StatusUpToDate != update_status_manager_.GetUpdateStatus()
        ? true : false;
    cache.SaveUpdateRequired(update_required);
    cache.Backup();
}

utils::SharedPtr<policy_table::Table> PolicyManagerImpl::Parse(
  const BinaryMessage& pt_content) {
  std::string json(pt_content.begin(), pt_content.end());
  Json::Value value;
  Json::Reader reader;
  if (reader.parse(json.c_str(), value)) {
    return new policy_table::Table(&value);
  } else {
    return utils::SharedPtr<policy_table::Table>();
  }
}

bool PolicyManagerImpl::LoadPT(const std::string& file,
                               const BinaryMessage& pt_content) {
  LOG4CXX_INFO(logger_, "LoadPT of size " << pt_content.size());

  // Parse message into table struct
  utils::SharedPtr<policy_table::Table> pt_update = Parse(pt_content);
  if (!pt_update) {
    LOG4CXX_WARN(logger_, "Parsed table pointer is 0.");
    update_status_manager_.OnWrongUpdateReceived();
    return false;
  }
  pt_update->SetPolicyTableType(policy_table::PT_UPDATE);

#ifdef EXTENDED_POLICY
  file_system::DeleteFile(file);
#endif  

  if (!pt_update->is_valid()) {
    rpc::ValidationReport report("policy_table");
    pt_update->ReportErrors(&report);
    LOG4CXX_WARN(logger_, "Parsed table is not valid " <<
                 rpc::PrettyFormat(report));

    update_status_manager_.OnWrongUpdateReceived();

    return false;
  }

  update_status_manager_.OnValidUpdateReceived();

  sync_primitives::AutoLock lock(apps_registration_lock_);

  // Get current DB data, since it could be updated during awaiting of PTU
  utils::SharedPtr<policy_table::Table> policy_table_snapshot =
      cache.GenerateSnapshot();
  if (!policy_table_snapshot) {
    LOG4CXX_ERROR(logger_, "Failed to create snapshot of policy table");
    return false;
  }

  // Replace predefined policies with its actual setting, e.g. "123":"default"
  // to actual values of default section
  UnwrapAppPolicies(pt_update->policy_table.app_policies);

  // Check and update permissions for applications, send notifications
  CheckPermissionsChanges(pt_update, policy_table_snapshot);

  // Replace current data with updated


  if (!cache.ApplyUpdate(*pt_update)) {
    LOG4CXX_WARN(logger_, "Unsuccessful save of updated policy table.");
    return false;
  }

  // If there was a user request for policy table update, it should be started
  // right after current update is finished
  if(listener_) {
    RefreshRetrySequence();
    listener_->OnUserRequestedUpdateCheckRequired();
    return true;
  }

  // Removing last app request from update requests
  RemoveAppFromUpdateList();

  // TODO(AOleynik): Check, if there is updated info present for apps in list
  // and skip update in this case for given app
  if (!update_requests_list_.empty()) {
    if (listener_) {
      listener_->OnPTExchangeNeeded();
    }
  } else {
    LOG4CXX_INFO(logger_, "Update request queue is empty.");
  }

  RefreshRetrySequence();

  return true;
}

void PolicyManagerImpl::CheckPermissionsChanges(
    const utils::SharedPtr<policy_table::Table> pt_update,
    const utils::SharedPtr<policy_table::Table> snapshot) {
  LOG4CXX_INFO(logger_, "Checking incoming permissions.");

  std::for_each(pt_update->policy_table.app_policies.begin(),
                pt_update->policy_table.app_policies.end(),
                CheckAppPolicy(this, pt_update, snapshot));
}

void PolicyManagerImpl::PrepareNotificationData(
  const policy_table::FunctionalGroupings& groups,
  const policy_table::Strings& group_names,
  const std::vector<FunctionalGroupPermission>& group_permission,
  Permissions& notification_data) {

  LOG4CXX_INFO(logger_, "Preparing data for notification.");
  ProcessFunctionalGroup processor(groups, group_permission, notification_data);
  std::for_each(group_names.begin(), group_names.end(), processor);
}

void PolicyManagerImpl::AddAppToUpdateList(const std::string& application_id) {
  sync_primitives::AutoLock lock(update_request_list_lock_);
  LOG4CXX_INFO(logger_,
               "Adding application " << application_id << " to update list");
  // Add application id only once
  std::list<std::string>::const_iterator it = std::find(
        update_requests_list_.begin(), update_requests_list_.end(),
        application_id);
  if (it == update_requests_list_.end()) {
    update_requests_list_.push_back(application_id);
  }
}

void PolicyManagerImpl::RemoveAppFromUpdateList() {
  sync_primitives::AutoLock lock(update_request_list_lock_);
  if (update_requests_list_.empty()) {
    return;
  }
  LOG4CXX_INFO(
    logger_,
    "Removing application " << update_requests_list_.front() << " from update list");
  update_requests_list_.pop_front();
}

std::string PolicyManagerImpl::GetUpdateUrl(int service_type) {
  LOG4CXX_INFO(logger_, "PolicyManagerImpl::GetUpdateUrl");
  EndpointUrls urls = cache.GetUpdateUrls(service_type);

  static uint32_t index = 0;
  std::string url;

  if (!urls.empty() && index >= urls.size()) {
    index = 0;
  }
  url = urls[index].url.empty() ? "" :urls[index].url[0];

  ++index;
  return url;
}

EndpointUrls PolicyManagerImpl::GetUpdateUrls(int service_type) {
  LOG4CXX_INFO(logger_, "PolicyManagerImpl::GetUpdateUrls");
  return cache.GetUpdateUrls(service_type);
}

BinaryMessageSptr PolicyManagerImpl::RequestPTUpdate() {
  LOG4CXX_INFO(logger_, "Creating PT Snapshot");  
  utils::SharedPtr<policy_table::Table> policy_table_snapshot =
      cache.GenerateSnapshot();
  if (!policy_table_snapshot) {
    LOG4CXX_ERROR(logger_, "Failed to create snapshot of policy table");
    return NULL;
  }

  // Temporary workaround
  *(policy_table_snapshot->policy_table.module_config.preloaded_pt) = false;

#ifdef EXTENDED_POLICY
    cache.UnpairedDevicesList(unpaired_device_ids_);
#endif  // EXTENDED_POLICY
  policy_table_snapshot->SetPolicyTableType(policy_table::PT_SNAPSHOT);
  /*if (false == policy_table_snapshot->is_valid()) {
    LOG4CXX_INFO(
          logger_, "Policy snappshot is not valid");
    rpc::ValidationReport report("policy_table");
    policy_table_snapshot->ReportErrors(&report);
    LOG4CXX_INFO(logger_,
                 "Errors: " << rpc::PrettyFormat(report));
  }*/

  Json::Value value = policy_table_snapshot->ToJsonValue();
  Json::FastWriter writer;
  std::string message_string = writer.write(value);
  return new BinaryMessage(message_string.begin(), message_string.end());
}

void PolicyManagerImpl::CheckPermissions(const PTString& app_id,
                                         const PTString& hmi_level,
                                          const PTString& rpc,
                                          CheckPermissionResult& result) {
  LOG4CXX_INFO(
    logger_,
    "CheckPermissions for " << app_id << " and rpc " << rpc << " for "
    << hmi_level << " level.");

#ifdef EXTENDED_POLICY
  const std::string device_id = GetCurrentDeviceId(app_id);
  // Get actual application group permission according to user consents
  std::vector<FunctionalGroupPermission> app_group_permissions;
  GetPermissionsForApp(device_id, app_id, app_group_permissions);

  // Fill struct with known groups RPCs
  policy_table::FunctionalGroupings functional_groupings;
  cache.GetFunctionalGroupings(functional_groupings);

  policy_table::Strings app_groups;
  std::vector<FunctionalGroupPermission>::const_iterator it =
    app_group_permissions.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end =
    app_group_permissions.end();
  for (; it != it_end; ++it) {
    app_groups.push_back((*it).group_name);
  }

  Permissions rpc_permissions;
  // Undefined groups (without user consent) disallowed by default, since
  // OnPermissionsChange notification has no "undefined" section
  // For RPC permission checking undefinded group will be treated as separate
  // type
  ProcessFunctionalGroup processor(functional_groupings,
                                   app_group_permissions,
                                   rpc_permissions,
                                   GroupConsent::kGroupUndefined);
  std::for_each(app_groups.begin(), app_groups.end(), processor);

  if (rpc_permissions.end() == rpc_permissions.find(rpc)) {
    // RPC not found in list == disallowed by backend
    result.hmi_level_permitted = kRpcDisallowed;
  }

  // Check HMI level
  if (rpc_permissions[rpc].hmi_permissions[kAllowedKey].end() !=
      rpc_permissions[rpc].hmi_permissions[kAllowedKey].find(hmi_level)) {
    // RPC found in allowed == allowed by backend and user
    result.hmi_level_permitted = kRpcAllowed;
  } else if (rpc_permissions[rpc].hmi_permissions[kUndefinedKey].end() !=
      rpc_permissions[rpc].hmi_permissions[kUndefinedKey].find(hmi_level)) {
    // RPC found in undefined == allowed by backend, but not consented yet by
    // user
    result.hmi_level_permitted = kRpcDisallowed;
  } else if (rpc_permissions[rpc].hmi_permissions[kUserDisallowedKey].end() !=
      rpc_permissions[rpc].hmi_permissions[kUserDisallowedKey].find(hmi_level)) {
    // RPC found in allowed == allowed by backend, but disallowed by user
    result.hmi_level_permitted = kRpcUserDisallowed;
  }

  // Add parameters of RPC, if any
  std::copy(rpc_permissions[rpc].parameter_permissions[kAllowedKey].begin(),
            rpc_permissions[rpc].parameter_permissions[kAllowedKey].end(),
            std::back_inserter(result.list_of_allowed_params));

  std::copy(
        rpc_permissions[rpc].parameter_permissions[kUserDisallowedKey].begin(),
        rpc_permissions[rpc].parameter_permissions[kUserDisallowedKey].end(),
        std::back_inserter(result.list_of_disallowed_params));

  std::copy(rpc_permissions[rpc].parameter_permissions[kUndefinedKey].begin(),
            rpc_permissions[rpc].parameter_permissions[kUndefinedKey].end(),
            std::back_inserter(result.list_of_undefined_params));
#else
  cache.CheckPermissions(app_id, hmi_level, rpc, result);
#endif
}

bool PolicyManagerImpl::ResetUserConsent() {
  bool result = true;
#ifdef EXTENDED_POLICY
  result = cache.ResetUserConsent();
#endif // EXTENDED_POLICY

  return result;
}

void PolicyManagerImpl::SendNotificationOnPermissionsUpdated(
  const std::string& application_id) {
  const std::string device_id = GetCurrentDeviceId(application_id);
  if (device_id.empty()) {
    LOG4CXX_WARN(logger_, "Couldn't find device info for application id "
                 "'" << application_id << "'");
    return;
  }

  std::vector<FunctionalGroupPermission> app_group_permissions;
  GetPermissionsForApp(device_id, application_id, app_group_permissions);

  policy_table::FunctionalGroupings functional_groupings;
  cache.GetFunctionalGroupings(functional_groupings);

  policy_table::Strings app_groups;
  std::vector<FunctionalGroupPermission>::const_iterator it =
    app_group_permissions.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end =
    app_group_permissions.end();
  for (; it != it_end; ++it) {
    app_groups.push_back((*it).group_name);
  }

  Permissions notification_data;
  PrepareNotificationData(functional_groupings, app_groups, app_group_permissions,
                          notification_data);

  LOG4CXX_INFO(logger_, "Send notification for application_id:" << application_id);
  std::string default_hmi;
 #ifdef EXTENDED_POLICY
  cache.GetDefaultHMI(application_id, default_hmi);
#else
  default_hmi = "NONE";
#endif
  listener()->OnPermissionsUpdated(application_id, notification_data,
                                   default_hmi);
}

bool PolicyManagerImpl::CleanupUnpairedDevices() {
  LOG4CXX_INFO(logger_, "CleanupUnpairedDevices");
#ifdef EXTENDED_POLICY
  return cache.CleanupUnpairedDevices(unpaired_device_ids_);
#else  // EXTENDED_POLICY
  // For SDL-specific it doesn't matter
  return true;
#endif  // EXTENDED_POLICY
}

DeviceConsent PolicyManagerImpl::GetUserConsentForDevice(
  const std::string& device_id) {
  LOG4CXX_INFO(logger_, "GetUserConsentForDevice");
#ifdef EXTENDED_POLICY
  // Get device permission groups from app_policies section, which hadn't been
  // preconsented
  policy_table::Strings groups;
  policy_table::Strings preconsented_groups;
  if (!cache.GetDeviceGroupsFromPolicies(groups, preconsented_groups)) {
    LOG4CXX_WARN(logger_, "Can't get device groups from policies.");
    return kDeviceDisallowed;
  }

  StringArray list_of_permissions;
  std::for_each(
    groups.begin(), groups.end(),
    FunctionalGroupInserter(preconsented_groups, list_of_permissions));

  // Check device permission groups for user consent in device_data section
  if (!list_of_permissions.empty()) {
    StringArray consented_groups;
    StringArray disallowed_groups;
    if (!cache.GetUserPermissionsForDevice(device_id, consented_groups,
        disallowed_groups)) {
      return kDeviceDisallowed;
    }

    if (consented_groups.empty() && disallowed_groups.empty()) {
      return kDeviceHasNoConsent;
    }

    std::sort(list_of_permissions.begin(), list_of_permissions.end());
    std::sort(consented_groups.begin(), consented_groups.end());

    StringArray to_be_consented_by_user;
    std::set_difference(
      list_of_permissions.begin(),
      list_of_permissions.end(),
      consented_groups.begin(),
      consented_groups.end(),
      std::back_inserter(to_be_consented_by_user));
    if (to_be_consented_by_user.empty()) {
      return kDeviceAllowed;
    }

    return kDeviceDisallowed;
  }

  return kDeviceAllowed;
#else
  return kDeviceAllowed;
#endif
}

void PolicyManagerImpl::SetUserConsentForDevice(const std::string& device_id,
    bool is_allowed) {
  LOG4CXX_INFO(logger_, "SetUserConsentForDevice");
  DeviceConsent current_consent = GetUserConsentForDevice(device_id);
  bool is_current_device_allowed =
      DeviceConsent::kDeviceAllowed == current_consent ? true : false;
  if (DeviceConsent::kDeviceHasNoConsent != current_consent &&
      is_current_device_allowed == is_allowed) {
    const std::string consent = is_allowed ? "allowed" : "disallowed";
    LOG4CXX_INFO(logger_, "Device is already " << consent << ".");
    return;
  }
#ifdef EXTENDED_POLICY
  // Get device permission groups from app_policies section, which hadn't been
  // preconsented
  policy_table::Strings groups;
  policy_table::Strings preconsented_groups;
  if (!cache.GetDeviceGroupsFromPolicies(groups, preconsented_groups)) {
    LOG4CXX_WARN(logger_, "Can't get device groups from policies.");
    return;
  }

  StringArray list_of_permissions;
  std::for_each(
    groups.begin(), groups.end(),
    FunctionalGroupInserter(preconsented_groups, list_of_permissions));

  if (list_of_permissions.empty()) {
    return;
  }

  StringArray consented_groups;
  StringArray disallowed_groups;

  // Supposed only one group for device date consent
  if (is_allowed) {
    consented_groups.push_back(list_of_permissions[0]);
  } else {
    disallowed_groups.push_back(list_of_permissions[0]);
  }

  if (!cache.SetUserPermissionsForDevice(device_id, consented_groups,
      disallowed_groups)) {
    LOG4CXX_WARN(logger_, "Can't set user consent for device");
    return;
  }

  if (listener_) {
    listener_->OnDeviceConsentChanged(device_id, is_allowed);
  } else {
    LOG4CXX_WARN(logger_, "Event listener is not initialized. "
                 "Can't call OnDeviceConsentChanged");
  }
#endif
}

bool PolicyManagerImpl::ReactOnUserDevConsentForApp(const std::string app_id,
    bool is_device_allowed) {
#ifdef EXTENDED_POLICY
  // TODO(AOleynik): Move out logic from PT representation to policy manager
  return cache.ReactOnUserDevConsentForApp(app_id, is_device_allowed);
#endif
  return true;
}

bool PolicyManagerImpl::GetInitialAppData(const std::string& application_id,
    StringArray* nicknames,
    StringArray* app_hmi_types) {
  LOG4CXX_INFO(logger_, "GetInitialAppData");
  const bool result = nicknames && app_hmi_types;
  if (result) {
    cache.GetInitialAppData(application_id, *nicknames, *app_hmi_types);
  }
  return result;
}

void PolicyManagerImpl::SetDeviceInfo(const std::string& device_id,
                                      const DeviceInfo& device_info) {
  LOG4CXX_INFO(logger_, "SetDeviceInfo");
#ifdef EXTENDED_POLICY
  if (!cache.SetDeviceData(device_id, device_info.hardware,
                             device_info.firmware_rev, device_info.os,
                             device_info.os_ver, device_info.carrier,
                             device_info.max_number_rfcom_ports,
                             device_info.connection_type)) {
    LOG4CXX_WARN(logger_, "Can't set device data.");
  }
#endif
}

PermissionConsent PolicyManagerImpl::EnsureCorrectPermissionConsent(
    const PermissionConsent& permissions_to_check) {
  std::vector<FunctionalGroupPermission> current_user_consents;
  GetUserConsentForApp(permissions_to_check.device_id,
                       permissions_to_check.policy_app_id,
                       current_user_consents);

  PermissionConsent permissions_to_set;
  permissions_to_set.device_id = permissions_to_check.device_id;
  permissions_to_set.policy_app_id = permissions_to_check.policy_app_id;
  permissions_to_set.consent_source = permissions_to_check.consent_source;

  std::vector<FunctionalGroupPermission>::const_iterator it =
      permissions_to_check.group_permissions.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end =
      permissions_to_check.group_permissions.end();

  for (;it != it_end; ++it) {
    std::vector<FunctionalGroupPermission>::const_iterator it_curr =
        current_user_consents.begin();
    std::vector<FunctionalGroupPermission>::const_iterator it_curr_end =
        current_user_consents.end();

    for (; it_curr != it_curr_end; ++it_curr) {
      if (it->group_alias == it_curr->group_alias &&
          it->group_id == it_curr->group_id) {
        permissions_to_set.group_permissions.push_back(*it);
      }
    }
  }

  return permissions_to_set;
}

void PolicyManagerImpl::SetUserConsentForApp(
    const PermissionConsent& permissions) {
  LOG4CXX_INFO(logger_, "SetUserConsentForApp");
#ifdef EXTENDED_POLICY
  PermissionConsent verified_permissions =
      EnsureCorrectPermissionConsent(permissions);

  if (!cache.SetUserPermissionsForApp(verified_permissions)) {
    LOG4CXX_WARN(logger_, "Can't set user permissions for application.");
  }
  // Send OnPermissionChange notification, since consents were changed
  std::vector<FunctionalGroupPermission> app_group_permissons;
  GetPermissionsForApp(verified_permissions.device_id,
                       verified_permissions.policy_app_id,
                       app_group_permissons);

  // Get current functional groups from DB with RPC permissions
  policy_table::FunctionalGroupings functional_groups;
  cache.GetFunctionalGroupings(functional_groups);

  // Get list of groups assigned to application
  policy_table::Strings app_groups;
  std::vector<FunctionalGroupPermission>::const_iterator it =
    app_group_permissons.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end =
    app_group_permissons.end();
  for (; it != it_end; ++it) {
    app_groups.push_back((*it).group_name);
  }

  // Fill notification data according to group permissions
  Permissions notification_data;
  PrepareNotificationData(functional_groups, app_groups,
                          app_group_permissons, notification_data);

  std::string default_hmi;
  cache.GetDefaultHMI(verified_permissions.policy_app_id, default_hmi);

  listener()->OnPermissionsUpdated(verified_permissions.policy_app_id,
                                   notification_data,
                                   default_hmi);
#endif
}

bool PolicyManagerImpl::GetDefaultHmi(const std::string& policy_app_id,
                                      std::string* default_hmi) {
  LOG4CXX_INFO(logger_, "GetDefaultHmi");
#ifdef EXTENDED_POLICY
  return cache.GetDefaultHMI(policy_app_id, *default_hmi);
#else
  return false;
#endif
}

bool PolicyManagerImpl::GetPriority(const std::string& policy_app_id,
                                    std::string* priority) {
  LOG4CXX_INFO(logger_, "GetPriority");
  if (!priority) {
    LOG4CXX_WARN(logger_, "Input priority parameter is null.");
    return false;
  }

  return cache.GetPriority(policy_app_id, *priority);
}

std::vector<UserFriendlyMessage> PolicyManagerImpl::GetUserFriendlyMessages(
  const std::vector<std::string>& message_code, const std::string& language) {
  return cache.GetUserFriendlyMsg(message_code, language);
}

void PolicyManagerImpl::GetUserConsentForApp(
  const std::string& device_id, const std::string& policy_app_id,
  std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_INFO(logger_, "GetUserConsentForApp");

  FunctionalIdType group_types;
  if (!cache.GetPermissionsForApp(device_id, policy_app_id,
                                        group_types)) {
    LOG4CXX_WARN(logger_, "Can't get user permissions for app "
                 << policy_app_id);
    return;
  }

  // Functional groups w/o alias ("user_consent_prompt") considered as
  // automatically allowed and it could not be changed by user
  FunctionalGroupNames group_names;
  if (!cache.GetFunctionalGroupNames(group_names)) {
    LOG4CXX_WARN(logger_, "Can't get functional group names");
    return;
  }

  FunctionalGroupNames::const_iterator it = group_names.begin();
  FunctionalGroupNames::const_iterator it_end = group_names.end();
  FunctionalGroupIDs auto_allowed_groups;
  for (;it != it_end; ++it) {
    if (it->second.first.empty()) {
      auto_allowed_groups.push_back(it->first);
    }
  }

#ifdef EXTENDED_POLICY
  FunctionalGroupIDs all_groups = group_types[kTypeGeneral];
  FunctionalGroupIDs preconsented_groups = group_types[kTypePreconsented];
  FunctionalGroupIDs consent_allowed_groups = group_types[kTypeAllowed];
  FunctionalGroupIDs consent_disallowed_groups = group_types[kTypeDisallowed];
  FunctionalGroupIDs default_groups = group_types[kTypeDefault];
  FunctionalGroupIDs predataconsented_groups =
      group_types[kTypePreDataConsented];
  FunctionalGroupIDs device_groups = group_types[kTypeDevice];

  // Sorting groups by consent
  FunctionalGroupIDs preconsented_wo_auto =
      ExcludeSame(preconsented_groups, auto_allowed_groups);

  FunctionalGroupIDs preconsented_wo_disallowed_auto =
      ExcludeSame(preconsented_wo_auto, consent_disallowed_groups);

  FunctionalGroupIDs allowed_groups = Merge(consent_allowed_groups,
                                            preconsented_wo_disallowed_auto);

  FunctionalGroupIDs merged_stage_1 = Merge(default_groups,
                                            predataconsented_groups);

  FunctionalGroupIDs merged_stage_2 = Merge(merged_stage_1,
                                            device_groups);

  FunctionalGroupIDs merged_stage_3 = Merge(merged_stage_2,
                                            auto_allowed_groups);

  FunctionalGroupIDs excluded_stage_1 = ExcludeSame(all_groups,
                                                    merged_stage_3);

  FunctionalGroupIDs excluded_stage_2 =
      ExcludeSame(excluded_stage_1, consent_disallowed_groups);

  FunctionalGroupIDs undefined_consent = ExcludeSame(excluded_stage_2,
                                                     allowed_groups);

  // Fill result
  FillFunctionalGroupPermissions(undefined_consent, group_names,
                                 kGroupUndefined, permissions);
  FillFunctionalGroupPermissions(allowed_groups, group_names,
                                 kGroupAllowed, permissions);
  FillFunctionalGroupPermissions(consent_disallowed_groups, group_names,
                                 kGroupDisallowed, permissions);
#else
  // For basic policy
  FunctionalGroupIDs all_groups = group_types[kTypeGeneral];
  FunctionalGroupIDs default_groups = group_types[kTypeDefault];
  FunctionalGroupIDs predataconsented_groups =
      group_types[kTypePreDataConsented];

  FunctionalGroupIDs allowed_groups;
  FunctionalGroupIDs no_auto = ExcludeSame(all_groups, auto_allowed_groups);

  if (cache.IsDefaultPolicy(policy_app_id)) {
    allowed_groups = ExcludeSame(no_auto, default_groups);
  } else if (cache.IsPredataPolicy(policy_app_id)) {
    allowed_groups = ExcludeSame(no_auto, predataconsented_groups);
  }
  FillFunctionalGroupPermissions(allowed_groups, group_names,
                                 kGroupAllowed, permissions);
#endif
}

void PolicyManagerImpl::GetPermissionsForApp(
  const std::string& device_id, const std::string& policy_app_id,
  std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_INFO(logger_, "GetPermissionsForApp");
  std::string app_id_to_check = policy_app_id;  

  bool allowed_by_default = false;
  if (cache.IsDefaultPolicy(policy_app_id)) {
    app_id_to_check = kDefaultId;
    allowed_by_default = true;
  } else if (cache.IsPredataPolicy(policy_app_id)) {
    app_id_to_check = kPreDataConsentId;
    allowed_by_default = true;
  }

  FunctionalIdType group_types;
  if (!cache.GetPermissionsForApp(device_id, app_id_to_check,
                                        group_types)) {
    LOG4CXX_WARN(logger_, "Can't get user permissions for app "
                 << policy_app_id);
    return;
  }

  // Functional groups w/o alias ("user_consent_prompt") considered as
  // automatically allowed and it could not be changed by user
  FunctionalGroupNames group_names;
  if (!cache.GetFunctionalGroupNames(group_names)) {
    LOG4CXX_WARN(logger_, "Can't get functional group names");
    return;
  }

  // The "default" and "pre_DataConsent" are auto-allowed groups
  // So, check if application in the one of these mode.
  if (allowed_by_default) {
    LOG4CXX_INFO(logger_, "Get auto allowe groups");
    GroupType type = (kDefaultId == app_id_to_check ?
          kTypeDefault : kTypePreDataConsented);

    FillFunctionalGroupPermissions(group_types[type], group_names,
                                   kGroupAllowed, permissions);
  } else {

    // The code bellow allows to process application which
    // has specific permissions(not default and pre_DataConsent).

    // All groups for specific application
    FunctionalGroupIDs all_groups = group_types[kTypeGeneral];

#ifdef EXTENDED_POLICY
    // Groups assigned by the user for specific application
    FunctionalGroupIDs allowed_groups = group_types[kTypeAllowed];

    // Groups disallowed by the user for specific application
    FunctionalGroupIDs common_disallowed = group_types[kTypeDisallowed];

    // Groups that allowed by default but can be changed by the user
    FunctionalGroupIDs preconsented_groups = group_types[kTypePreconsented];

    // Pull common groups from allowed and preconsented parts.
    FunctionalGroupIDs allowed_preconsented = Merge(allowed_groups,
                                              preconsented_groups);

    // Get all groups that we suppose are allowed.
    FunctionalGroupIDs all_allowed = Merge(allowed_preconsented,
                                           all_groups);

    // In case when same groups exists in disallowed and allowed tables,
    // disallowed one have priority over allowed. So we have to remove
    // all disallowed groups from allowed table.
    FunctionalGroupIDs common_allowed = ExcludeSame(all_allowed,
                                                    common_disallowed);

    // Remove all disallowed groups from application specific groups.
    FunctionalGroupIDs no_disallowed = ExcludeSame(all_groups,
                                                   common_disallowed);

    // Undefined groups are groups that have no allowed or disallowed type.
    FunctionalGroupIDs undefined_consent = ExcludeSame(no_disallowed,
                                                       common_allowed);

    // Fill result
    FillFunctionalGroupPermissions(undefined_consent, group_names,
                                   kGroupUndefined, permissions);
    FillFunctionalGroupPermissions(common_allowed, group_names,
                                   kGroupAllowed, permissions);
    FillFunctionalGroupPermissions(common_disallowed, group_names,
                                   kGroupDisallowed, permissions);
#else
    // In case of GENIVI all groups are allowed
    FunctionalGroupIDs common_allowed = all_groups;
    FillFunctionalGroupPermissions(common_allowed, group_names,
                                   kGroupAllowed, permissions);
#endif
  }
  return;
}

std::string& PolicyManagerImpl::GetCurrentDeviceId(
  const std::string& policy_app_id) {
  LOG4CXX_INFO(logger_, "GetDeviceInfo");
  last_device_id_ =
    listener()->OnCurrentDeviceIdUpdateRequired(policy_app_id);
  return last_device_id_;
}

void PolicyManagerImpl::SetSystemLanguage(const std::string& language) {
#ifdef EXTENDED_POLICY
  cache.SetSystemLanguage(language);
#endif
}

void PolicyManagerImpl::SetSystemInfo(const std::string& ccpu_version,
                                      const std::string& wers_country_code,
                                      const std::string& language) {
  LOG4CXX_INFO(logger_, "SetSystemInfo");
#ifdef EXTENDED_POLICY
  cache.SetMetaInfo(ccpu_version, wers_country_code, language);
#endif
}

void PolicyManagerImpl::OnSystemReady() {
  // Update policy table for the first time with system information
  if (cache.IsPTPreloaded()) {
    listener()->OnSystemInfoUpdateRequired();
    return;
  }
#ifdef EXTENDED_POLICY
  if (cache.IsMetaInfoPresent()) {
    listener()->OnSystemInfoUpdateRequired();
  }
#endif
}

uint32_t PolicyManagerImpl::GetNotificationsNumber(
    const std::string& priority) {
  LOG4CXX_INFO(logger_, "GetNotificationsNumber");
  return cache.GetNotificationsNumber(priority);
}

bool PolicyManagerImpl::ExceededIgnitionCycles() {
  return 0 == cache.IgnitionCyclesBeforeExchange();
}

bool PolicyManagerImpl::ExceededDays(int days) {
  return 0 == cache.DaysBeforeExchange(days);
}

bool PolicyManagerImpl::ExceededKilometers(int kilometers) {
  return 0 == cache.KilometersBeforeExchange(kilometers);
}

void PolicyManagerImpl::IncrementIgnitionCycles() {
  cache.IncrementIgnitionCycles();
}

PolicyTableStatus PolicyManagerImpl::GetPolicyTableStatus() {
  return update_status_manager_.GetUpdateStatus();
}

int PolicyManagerImpl::NextRetryTimeout() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  LOG4CXX_DEBUG(logger_, "Index: " << retry_sequence_index_);
  int next = 0;
  if (!retry_sequence_seconds_.empty()
      && retry_sequence_index_ < retry_sequence_seconds_.size()) {
    next = retry_sequence_seconds_[retry_sequence_index_];
    ++retry_sequence_index_;
  }
  return next;
}

void PolicyManagerImpl::RefreshRetrySequence() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  retry_sequence_timeout_ = cache.TimeoutResponse();
  retry_sequence_seconds_.clear();
  cache.SecondsBetweenRetries(retry_sequence_seconds_);
}

void PolicyManagerImpl::ResetRetrySequence() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  retry_sequence_index_ = 0;
  update_status_manager_.OnResetRetrySequence();
}

int PolicyManagerImpl::TimeoutExchange() {
  return retry_sequence_timeout_;
}

const std::vector<int> PolicyManagerImpl::RetrySequenceDelaysSeconds() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  return retry_sequence_seconds_;
}

void PolicyManagerImpl::OnExceededTimeout() {
  update_status_manager_.OnUpdateTimeoutOccurs();
}

void PolicyManagerImpl::OnUpdateStarted() {
  int update_timeout = TimeoutExchange();
  LOG4CXX_INFO(logger_, "Update timeout will be set to: " << update_timeout);
  update_status_manager_.OnUpdateSentOut(update_timeout);
}

void PolicyManagerImpl::PTUpdatedAt(int kilometers, int days_after_epoch) {
  LOG4CXX_INFO(logger_, "PTUpdatedAt");
  LOG4CXX_INFO(logger_,
               "Kilometers: " << kilometers << " Days: " << days_after_epoch);
  cache.SetCountersPassedForSuccessfulUpdate(
    kilometers, days_after_epoch);
  cache.ResetIgnitionCycles();
}

void PolicyManagerImpl::Increment(usage_statistics::GlobalCounterId type) {
  std::string counter;
  switch (type) {
    case usage_statistics::IAP_BUFFER_FULL:
      counter = "count_of_iap_buffer_full";
      break;
    case usage_statistics::SYNC_OUT_OF_MEMORY:
      counter = "count_sync_out_of_memory";
      break;
    case usage_statistics::SYNC_REBOOTS:
      counter = "count_of_sync_reboots";
      break;
    default:
      LOG4CXX_INFO(logger_, "Type global counter is unknown");
      return;
  }
#ifdef EXTENDED_POLICY
  cache.Increment(counter);
#endif
}

void PolicyManagerImpl::Increment(const std::string& app_id,
                                  usage_statistics::AppCounterId type) {
  std::string counter;
  switch (type) {
    case usage_statistics::USER_SELECTIONS:
      counter = "count_of_user_selections";
      break;
    case usage_statistics::REJECTIONS_SYNC_OUT_OF_MEMORY:
      counter = "count_of_rejections_sync_out_of_memory";
      break;
    case usage_statistics::REJECTIONS_NICKNAME_MISMATCH:
      counter = "count_of_rejections_nickname_mismatch";
      break;
    case usage_statistics::REJECTIONS_DUPLICATE_NAME:
      counter = "count_of_rejections_duplicate_name";
      break;
    case usage_statistics::REJECTED_RPC_CALLS:
      counter = "count_of_rejected_rpcs_calls";
      break;
    case usage_statistics::RPCS_IN_HMI_NONE:
      counter = "count_of_rpcs_sent_in_hmi_none";
      break;
    case usage_statistics::REMOVALS_MISBEHAVED:
      counter = "count_of_removals_for_bad_behavior";
      break;
    case usage_statistics::RUN_ATTEMPTS_WHILE_REVOKED:
      counter = "count_of_run_attempts_while_revoked";
      break;
    default:
      LOG4CXX_INFO(logger_, "Type app counter is unknown");
      return;
  }
#ifdef EXTENDED_POLICY
  sync_primitives::AutoLock locker(statistics_lock_);
  cache.Increment(app_id, counter);
#endif
}

void PolicyManagerImpl::Set(const std::string& app_id,
                            usage_statistics::AppInfoId type,
                            const std::string& value) {
  std::string info;
  switch (type) {
    case usage_statistics::LANGUAGE_GUI:
      info = "app_registration_language_gui";
      break;
    case usage_statistics::LANGUAGE_VUI:
      info = "app_registration_language_vui";
      break;
    default:
      LOG4CXX_INFO(logger_, "Type app info is unknown");
      return;
  }

#ifdef EXTENDED_POLICY
  sync_primitives::AutoLock locker(statistics_lock_);
  cache.Set(app_id, info, value);
#endif
}

void PolicyManagerImpl::Add(const std::string& app_id,
                            usage_statistics::AppStopwatchId type,
                            int32_t timespan_seconds) {
#ifdef EXTENDED_POLICY
  std::string stopwatch;
  switch (type) {
    // TODO(KKolodiy): rename fields in database
    case usage_statistics::SECONDS_HMI_FULL:
      stopwatch = "minutes_in_hmi_full";
      break;
    case usage_statistics::SECONDS_HMI_LIMITED:
      stopwatch = "minutes_in_hmi_limited";
      break;
    case usage_statistics::SECONDS_HMI_BACKGROUND:
      stopwatch = "minutes_in_hmi_background";
      break;
    case usage_statistics::SECONDS_HMI_NONE:
      stopwatch = "minutes_in_hmi_none";
      break;
    default:
      LOG4CXX_INFO(logger_, "Type app stopwatch is unknown");
      return;
  }

  sync_primitives::AutoLock locker(statistics_lock_);
  cache.Add(app_id, stopwatch, timespan_seconds);
#endif
}

bool PolicyManagerImpl::IsApplicationRevoked(const std::string& app_id) const {
  return const_cast<PolicyManagerImpl*>(this)->cache.IsApplicationRevoked(app_id);
}

int PolicyManagerImpl::IsConsentNeeded(const std::string& app_id) {
#ifdef EXTENDED_POLICY
  const std::string device_id = GetCurrentDeviceId(app_id);
  int count = 0;
  if (cache.CountUnconsentedGroups(app_id, device_id, count)) {
    return count;
  }
#endif
  return 0;
}

void PolicyManagerImpl::SetVINValue(const std::string& value) {
#ifdef EXTENDED_POLICY
  cache.SetVINValue(value);
#endif // EXTENDED_POLICY
}

AppPermissions PolicyManagerImpl::GetAppPermissionsChanges(
    const std::string& device_id,
    const std::string& policy_app_id) {
  typedef std::map<std::string, AppPermissions>::iterator PermissionsIt;
  PermissionsIt app_id_diff = app_permissions_diff_.find(policy_app_id);
  AppPermissions permissions(policy_app_id);
  if (app_permissions_diff_.end() != app_id_diff) {
    // At this point we're able to know the device id for which user consents
    // could be evaluated
    std::vector<FunctionalGroupPermission> groups;
    GetUserConsentForApp(device_id, policy_app_id, groups);
    CopyPermissions copier(groups);
    std::for_each(app_id_diff->second.appRevokedPermissions.begin(),
                  app_id_diff->second.appRevokedPermissions.end(),
                  copier);
    permissions = app_id_diff->second;
  } else {
    permissions.appPermissionsConsentNeeded = IsConsentNeeded(policy_app_id);
    permissions.appRevoked = IsApplicationRevoked(policy_app_id);
    GetPriority(permissions.application_id, &permissions.priority);
  }
  return permissions;
}

void PolicyManagerImpl::RemovePendingPermissionChanges(
  const std::string& app_id) {
  app_permissions_diff_.erase(app_id);
}

bool PolicyManagerImpl::CanAppKeepContext(const std::string& app_id) {
#ifdef EXTENDED_POLICY
  return cache.CanAppKeepContext(app_id);
#endif // EXTENDED_POLICY
  return false;
}

bool PolicyManagerImpl::CanAppStealFocus(const std::string& app_id) {
#ifdef EXTENDED_POLICY
  cache.CanAppStealFocus(app_id);
#endif // EXTENDED_POLICY
  return false;
}

void PolicyManagerImpl::MarkUnpairedDevice(const std::string& device_id) {
#ifdef EXTENDED_POLICY
    cache.SetUnpairedDevice(device_id);
    SetUserConsentForDevice(device_id, false);
#endif  // EXTENDED_POLICY
}

void PolicyManagerImpl::AddApplication(const std::string& application_id) {
  LOG4CXX_INFO(logger_, "AddApplication");
  const std::string device_id = GetCurrentDeviceId(application_id);
  DeviceConsent device_consent = GetUserConsentForDevice(device_id);
  sync_primitives::AutoLock lock(apps_registration_lock_);

  if (IsNewApplication(application_id)) {
    AddNewApplication(application_id, device_consent);
    AddAppToUpdateList(application_id);
    if (PolicyTableStatus::StatusUpToDate == GetPolicyTableStatus()) {
      update_status_manager_.OnNewApplicationAdded();
    }
  } else {
    PromoteExistedApplication(application_id, device_consent);
  }
  SendNotificationOnPermissionsUpdated(application_id);
}

void PolicyManagerImpl::AddNewApplication(const std::string& application_id,
                                          DeviceConsent device_consent) {
  LOG4CXX_INFO(logger_, "PolicyManagerImpl::AddNewApplication");

  LOG4CXX_INFO(
    logger_,
    "Setting default permissions for application id: " << application_id);
#ifdef EXTENDED_POLICY
  if (kDeviceHasNoConsent == device_consent ||
      kDeviceDisallowed == device_consent) {
    cache.SetPredataPolicy(application_id);
  } else {
    cache.SetDefaultPolicy(application_id);
  }
#else
  cache.SetDefaultPolicy(application_id);
#endif
}

void PolicyManagerImpl::PromoteExistedApplication(
    const std::string& application_id, DeviceConsent device_consent) {

  if (kDeviceHasNoConsent != device_consent
      && cache.IsPredataPolicy(application_id)) {
    // If device consent changed to allowed during application being
    // disconnected, app permissions should be changed also
    if (kDeviceAllowed == device_consent) {
      cache.SetDefaultPolicy(application_id);
    }
  }
}

bool PolicyManagerImpl::IsNewApplication(
    const std::string& application_id) const {
  return false == cache.IsApplicationRepresented(application_id);
}

bool PolicyManagerImpl::ResetPT(const std::string& file_name) {
  const bool result = cache.ResetPT(file_name);
  if (result) {
    RefreshRetrySequence();
  }
  return result;
}

bool PolicyManagerImpl::InitPT(const std::string& file_name) {
  const bool ret = cache.Init(file_name);
  if (ret) {
    RefreshRetrySequence();
    update_status_manager_.OnPolicyInit(cache.UpdateRequired());
  }
  return ret;
}

}  //  namespace policy


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
#include "policy/policy_manager_impl.h"

#include <algorithm>
#include <set>
#include <queue>
#include <iterator>
#include <limits>
#include "json/reader.h"
#include "json/writer.h"
#include "policy/policy_table.h"
#include "policy/pt_representation.h"
#include "policy/policy_helper.h"
#include "utils/file_system.h"
#include "utils/logger.h"
#include "utils/date_time.h"
#include "policy/cache_manager.h"
#include "policy/update_status_manager.h"
#include "config_profile/profile.h"
#include "utils/make_shared.h"

policy::PolicyManager* CreateManager() {
  return new policy::PolicyManagerImpl();
}
void DeleteManager(policy::PolicyManager* pm) {
  delete pm;
}

namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "Policy")

PolicyManagerImpl::PolicyManagerImpl()
    : PolicyManager()
    , listener_(NULL)
    , cache_(new CacheManager)
    , retry_sequence_timeout_(60)
    , retry_sequence_index_(0)
    , ignition_check(true) {}

PolicyManagerImpl::PolicyManagerImpl(bool in_memory)
    : PolicyManager()
    , listener_(NULL)
    , cache_(new CacheManager(in_memory))
    , retry_sequence_timeout_(60)
    , retry_sequence_index_(0)
    , ignition_check(true) {}

void PolicyManagerImpl::set_listener(PolicyListener* listener) {
  listener_ = listener;
  update_status_manager_.set_listener(listener);
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

void PolicyManagerImpl::CheckTriggers() {
  LOG4CXX_AUTO_TRACE(logger_);
  const bool exceed_ignition_cycles = ExceededIgnitionCycles();
  const bool exceed_days = ExceededDays();

  LOG4CXX_DEBUG(
      logger_,
      "\nDays exceeded: " << std::boolalpha << exceed_days
                          << "\nIgnition cycles exceeded: " << std::boolalpha
                          << exceed_ignition_cycles);

  if (exceed_ignition_cycles || exceed_days) {
    update_status_manager_.ScheduleUpdate();
  }
}

bool PolicyManagerImpl::LoadPT(const std::string& file,
                               const BinaryMessage& pt_content) {
  LOG4CXX_INFO(logger_, "LoadPT of size " << pt_content.size());

  // Parse message into table struct
  utils::SharedPtr<policy_table::Table> pt_update = Parse(pt_content);
  if (!pt_update) {
    LOG4CXX_WARN(logger_, "Parsed table pointer is NULL.");
    update_status_manager_.OnWrongUpdateReceived();
    return false;
  }

  file_system::DeleteFile(file);

  if (!IsPTValid(pt_update, policy_table::PT_UPDATE)) {
    update_status_manager_.OnWrongUpdateReceived();
    return false;
  }

  update_status_manager_.OnValidUpdateReceived();
  cache_->SaveUpdateRequired(false);

  {
    sync_primitives::AutoLock lock(apps_registration_lock_);

    // Get current DB data, since it could be updated during awaiting of PTU
    utils::SharedPtr<policy_table::Table> policy_table_snapshot =
        cache_->GenerateSnapshot();
    if (!policy_table_snapshot) {
      LOG4CXX_ERROR(logger_, "Failed to create snapshot of policy table");
      return false;
    }

    // Checking of difference between PTU and current policy state
    // Must to be done before PTU applying since it is possible, that functional
    // groups, which had been present before are absent in PTU and will be
    // removed after update. So in case of revoked groups system has to know
    // names and ids of revoked groups before they will be removed.
    CheckPermissionsChanges(pt_update, policy_table_snapshot);

    // Replace current data with updated
    if (!cache_->ApplyUpdate(*pt_update)) {
      LOG4CXX_WARN(logger_, "Unsuccessful save of updated policy table.");
      return false;
    }

    listener_->OnCertificateUpdated(
        *(pt_update->policy_table.module_config.certificate));

    std::map<std::string, StringArray> app_hmi_types;
    cache_->GetHMIAppTypeAfterUpdate(app_hmi_types);
    if (!app_hmi_types.empty()) {
      LOG4CXX_INFO(logger_, "app_hmi_types is full calling OnUpdateHMIAppType");
      listener_->OnUpdateHMIAppType(app_hmi_types);
    } else {
      LOG4CXX_INFO(logger_, "app_hmi_types empty");
    }
  }

  // If there was a user request for policy table update, it should be started
  // right after current update is finished
  if (update_status_manager_.IsUpdateRequired()) {
    StartPTExchange();
    return true;
  }

  RefreshRetrySequence();
  return true;
}

std::string PolicyManagerImpl::GetLockScreenIconUrl() const {
  return cache_->GetLockScreenIconUrl();
}

void PolicyManagerImpl::CheckPermissionsChanges(
    const utils::SharedPtr<policy_table::Table> pt_update,
    const utils::SharedPtr<policy_table::Table> snapshot) {
  LOG4CXX_INFO(logger_, "Checking incoming permissions.");

  // Replace predefined policies with its actual setting, e.g. "123":"default"
  // to actual values of default section
  UnwrapAppPolicies(pt_update->policy_table.app_policies_section.apps);

  std::for_each(pt_update->policy_table.app_policies_section.apps.begin(),
                pt_update->policy_table.app_policies_section.apps.end(),
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

void PolicyManagerImpl::GetUpdateUrls(const std::string& service_type,
                                      EndpointUrls& out_end_points) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->GetUpdateUrls(service_type, out_end_points);
}
void PolicyManagerImpl::GetUpdateUrls(const uint32_t service_type,
                                      EndpointUrls& out_end_points) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->GetUpdateUrls(service_type, out_end_points);
}

void PolicyManagerImpl::RequestPTUpdate() {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::SharedPtr<policy_table::Table> policy_table_snapshot =
      cache_->GenerateSnapshot();
  if (!policy_table_snapshot) {
    LOG4CXX_ERROR(logger_, "Failed to create snapshot of policy table");
    return;
  }

  if (IsPTValid(policy_table_snapshot, policy_table::PT_SNAPSHOT)) {
    Json::Value value = policy_table_snapshot->ToJsonValue();
    Json::FastWriter writer;
    std::string message_string = writer.write(value);

    LOG4CXX_DEBUG(logger_, "Snapshot contents is : " << message_string);

    BinaryMessage update(message_string.begin(), message_string.end());

    listener_->OnSnapshotCreated(
        update, RetrySequenceDelaysSeconds(), TimeoutExchangeMSec());
  } else {
    LOG4CXX_ERROR(logger_, "Invalid Policy table snapshot - PTUpdate failed");
  }
}

void PolicyManagerImpl::StartPTExchange() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (ignition_check) {
    CheckTriggers();
    ignition_check = false;
  }

  if (update_status_manager_.IsAppsSearchInProgress() &&
      update_status_manager_.IsUpdateRequired()) {
    LOG4CXX_INFO(logger_,
                 "Starting exchange skipped, since applications "
                 "search is in progress.");
    return;
  }

  if (update_status_manager_.IsUpdatePending()) {
    update_status_manager_.ScheduleUpdate();
    LOG4CXX_INFO(logger_,
                 "Starting exchange skipped, since another exchange "
                 "is in progress.");
    return;
  }
  LOG4CXX_INFO(logger_, "Policy want to  call RequestPTUpdate");
  if (listener_ && listener_->CanUpdate()) {
    LOG4CXX_INFO(logger_, "Listener CanUpdate");
    if (update_status_manager_.IsUpdateRequired()) {
      LOG4CXX_INFO(logger_, "IsUpdateRequired");
      RequestPTUpdate();
    }
  }
}

void PolicyManagerImpl::OnAppsSearchStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
  update_status_manager_.OnAppsSearchStarted();
}

void PolicyManagerImpl::OnAppsSearchCompleted() {
  LOG4CXX_AUTO_TRACE(logger_);
  update_status_manager_.OnAppsSearchCompleted();
  if (update_status_manager_.IsUpdateRequired()) {
    StartPTExchange();
  }
}

const std::vector<std::string> PolicyManagerImpl::GetAppRequestTypes(
    const std::string policy_app_id) const {
  std::vector<std::string> request_types;
  if (kDeviceDisallowed ==
      cache_->GetDeviceConsent(GetCurrentDeviceId(policy_app_id))) {
    cache_->GetAppRequestTypes(kPreDataConsentId, request_types);
  } else {
    cache_->GetAppRequestTypes(policy_app_id, request_types);
  }
  return request_types;
}

const VehicleInfo PolicyManagerImpl::GetVehicleInfo() const {
  return cache_->GetVehicleInfo();
}

void PolicyManagerImpl::CheckPermissions(const PTString& app_id,
                                         const PTString& hmi_level,
                                         const PTString& rpc,
                                         const RPCParams& rpc_params,
                                         CheckPermissionResult& result) {
  LOG4CXX_INFO(logger_,
               "CheckPermissions for " << app_id << " and rpc " << rpc
                                       << " for " << hmi_level << " level.");

  const std::string device_id = GetCurrentDeviceId(app_id);

  Permissions rpc_permissions;

  // Check, if there are calculated permission present in cache
  if (!cache_->IsPermissionsCalculated(device_id, app_id, rpc_permissions)) {
    LOG4CXX_DEBUG(logger_,
                  "IsPermissionsCalculated for device: "
                      << device_id << " and app: " << app_id
                      << " returns false");
    // Get actual application group permission according to user consents
    std::vector<FunctionalGroupPermission> app_group_permissions;
    GetPermissionsForApp(device_id, app_id, app_group_permissions);

    // Fill struct with known groups RPCs
    policy_table::FunctionalGroupings functional_groupings;
    cache_->GetFunctionalGroupings(functional_groupings);

    std::vector<FunctionalGroupPermission>::const_iterator it =
        app_group_permissions.begin();
    std::vector<FunctionalGroupPermission>::const_iterator it_end =
        app_group_permissions.end();
    policy_table::Strings app_groups;
    for (; it != it_end; ++it) {
      app_groups.push_back((*it).group_name);
    }

    // Undefined groups (without user consent) disallowed by default, since
    // OnPermissionsChange notification has no "undefined" section
    // For RPC permission checking undefinded group will be treated as separate
    // type
    ProcessFunctionalGroup processor(functional_groupings,
                                     app_group_permissions,
                                     rpc_permissions,
                                     GroupConsent::kGroupUndefined);
    std::for_each(app_groups.begin(), app_groups.end(), processor);

    cache_->AddCalculatedPermissions(device_id, app_id, rpc_permissions);
  } else {
    LOG4CXX_DEBUG(logger_,
                  "IsPermissionsCalculated for device: "
                      << device_id << " and app: " << app_id
                      << " returns true");
  }

  const bool known_rpc = rpc_permissions.end() != rpc_permissions.find(rpc);
  LOG4CXX_INFO(logger_, "Is known rpc " << known_rpc);
  if (!known_rpc) {
    // RPC not found in list == disallowed by backend
    result.hmi_level_permitted = kRpcDisallowed;
    return;
  }

  // Check HMI level
  if (rpc_permissions[rpc].hmi_permissions[kAllowedKey].end() !=
      rpc_permissions[rpc].hmi_permissions[kAllowedKey].find(hmi_level)) {
    // RPC found in allowed == allowed by backend and user
    result.hmi_level_permitted = kRpcAllowed;
  } else if (rpc_permissions[rpc].hmi_permissions[kUndefinedKey].end() !=
             rpc_permissions[rpc].hmi_permissions[kUndefinedKey].find(
                 hmi_level)) {
    // RPC found in undefined == allowed by backend, but not consented yet by
    // user
    result.hmi_level_permitted = kRpcDisallowed;
  } else if (rpc_permissions[rpc].hmi_permissions[kUserDisallowedKey].end() !=
             rpc_permissions[rpc].hmi_permissions[kUserDisallowedKey].find(
                 hmi_level)) {
    // RPC found in allowed == allowed by backend, but disallowed by user
    result.hmi_level_permitted = kRpcUserDisallowed;
  } else {
    LOG4CXX_DEBUG(logger_,
                  "HMI level " << hmi_level << " wasn't found "
                               << " for rpc " << rpc << " and appID "
                               << app_id);
    return;
  }

  if (kRpcAllowed != result.hmi_level_permitted) {
    LOG4CXX_DEBUG(logger_, "RPC is not allowed. Stop parameters processing.");
    result.list_of_allowed_params =
        rpc_permissions[rpc].parameter_permissions[kAllowedKey];

    result.list_of_disallowed_params =
        rpc_permissions[rpc].parameter_permissions[kUserDisallowedKey];

    result.list_of_undefined_params =
        rpc_permissions[rpc].parameter_permissions[kUndefinedKey];
    return;
  }

  // Considered that items disallowed by user take priority over system (policy)
  // permissions, so that flag is processed first
  if (rpc_permissions[rpc]
          .parameter_permissions.any_parameter_disallowed_by_user) {
    LOG4CXX_DEBUG(logger_, "All parameters are disallowed by user.");
    result.list_of_disallowed_params = rpc_params;
    result.hmi_level_permitted = kRpcUserDisallowed;
    return;
  }

  if (rpc_permissions[rpc]
          .parameter_permissions.any_parameter_disallowed_by_policy) {
    LOG4CXX_DEBUG(logger_, "All parameters are disallowed by policy.");
    result.list_of_undefined_params = rpc_params;
    result.hmi_level_permitted = kRpcDisallowed;
    return;
  }

  if (rpc_permissions[rpc].parameter_permissions.any_parameter_allowed) {
    LOG4CXX_DEBUG(logger_, "All parameters are allowed.");
    result.list_of_allowed_params = rpc_params;
    return;
  }

  result.list_of_allowed_params =
      rpc_permissions[rpc].parameter_permissions[kAllowedKey];

  result.list_of_disallowed_params =
      rpc_permissions[rpc].parameter_permissions[kUserDisallowedKey];

  result.list_of_undefined_params =
      rpc_permissions[rpc].parameter_permissions[kUndefinedKey];

  // In case of some parameters of RPC are missing in current policy table
  // they will be considered as disallowed by policy itself, not by user.
  // Undefined parameters contain parameters present in policy table, but which
  // have not been allowed or disallowed explicitly by user, so missing params
  // are being added to undefined.
  RPCParams::const_iterator parameter = rpc_params.begin();
  RPCParams::const_iterator end = rpc_params.end();
  for (; end != parameter; ++parameter) {
    if (!result.HasParameter(*parameter)) {
      LOG4CXX_DEBUG(logger_,
                    "Parameter " << *parameter << " is unknown."
                                                  " Adding to undefined list.");
      result.list_of_undefined_params.insert(*parameter);
    }
  }

  if (result.DisallowedInclude(rpc_params)) {
    LOG4CXX_DEBUG(logger_, "All parameters are disallowed.");
    result.hmi_level_permitted = kRpcUserDisallowed;
  } else if (!result.IsAnyAllowed(rpc_params)) {
    LOG4CXX_DEBUG(logger_, "There are no parameters allowed.");
    result.hmi_level_permitted = kRpcDisallowed;
  }

  if (cache_->IsApplicationRevoked(app_id)) {
    // SDL must be able to notify mobile side with its status after app has
    // been revoked by backend
    if ("OnHMIStatus" == rpc && "NONE" == hmi_level) {
      result.hmi_level_permitted = kRpcAllowed;
    } else {
      result.hmi_level_permitted = kRpcDisallowed;
    }
    return;
  }
}

bool PolicyManagerImpl::ResetUserConsent() {
  bool result = true;
  result = cache_->ResetUserConsent();

  return result;
}

void PolicyManagerImpl::SendNotificationOnPermissionsUpdated(
    const std::string& application_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string device_id = GetCurrentDeviceId(application_id);
  if (device_id.empty()) {
    LOG4CXX_WARN(logger_,
                 "Couldn't find device info for application id "
                 "'" << application_id << "'");
    return;
  }

  std::vector<FunctionalGroupPermission> app_group_permissions;
  GetPermissionsForApp(device_id, application_id, app_group_permissions);

  policy_table::FunctionalGroupings functional_groupings;
  cache_->GetFunctionalGroupings(functional_groupings);

  policy_table::Strings app_groups;
  std::vector<FunctionalGroupPermission>::const_iterator it =
      app_group_permissions.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end =
      app_group_permissions.end();
  for (; it != it_end; ++it) {
    app_groups.push_back((*it).group_name);
  }

  Permissions notification_data;
  PrepareNotificationData(functional_groupings,
                          app_groups,
                          app_group_permissions,
                          notification_data);

  LOG4CXX_INFO(logger_,
               "Send notification for application_id:" << application_id);

  std::string default_hmi;
  GetDefaultHmi(application_id, &default_hmi);

  listener()->OnPermissionsUpdated(
      application_id, notification_data, default_hmi);
}

bool PolicyManagerImpl::CleanupUnpairedDevices() {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->CleanupUnpairedDevices();
}

DeviceConsent PolicyManagerImpl::GetUserConsentForDevice(
    const std::string& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetDeviceConsent(device_id);
}

void PolicyManagerImpl::SetUserConsentForDevice(const std::string& device_id,
                                                const bool is_allowed) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Device :" << device_id);
  cache_->SetDeviceConsent(device_id, is_allowed);
  if (listener_) {
    listener_->OnDeviceConsentChanged(device_id, is_allowed);
  } else {
    LOG4CXX_WARN(logger_,
                 "Event listener is not initialized. "
                 "Can't call OnDeviceConsentChanged");
  }
  if (is_allowed) {
    update_status_manager_.OnDeviceConsented();
  }
  StartPTExchange();
}

bool PolicyManagerImpl::ReactOnUserDevConsentForApp(
    const std::string& app_id, const bool is_device_allowed) {
  std::vector<std::string> current_request_types = GetAppRequestTypes(app_id);
  std::string current_priority, new_priority;
  GetPriority(app_id, &current_priority);

  bool result = cache_->ReactOnUserDevConsentForApp(app_id, is_device_allowed);

  std::vector<std::string> new_request_types = GetAppRequestTypes(app_id);
  GetPriority(app_id, &new_priority);
  std::sort(current_request_types.begin(), current_request_types.end());
  std::sort(new_request_types.begin(), new_request_types.end());

  std::vector<std::string> diff;
  std::set_symmetric_difference(current_request_types.begin(),
                                current_request_types.end(),
                                new_request_types.begin(),
                                new_request_types.end(),
                                std::back_inserter(diff));

  AppPermissions permissions(app_id);

  if (!diff.empty()) {
    permissions.requestType = new_request_types;
    permissions.requestTypeChanged = true;
  }

  if ((!current_priority.empty()) && (!new_priority.empty()) &&
      (current_priority != new_priority)) {
    permissions.priority = new_priority;
  }

  if (permissions.requestTypeChanged || (!permissions.priority.empty())) {
    listener_->SendOnAppPermissionsChanged(permissions, app_id);
  }
  return result;
}

bool PolicyManagerImpl::GetInitialAppData(const std::string& application_id,
                                          StringArray* nicknames,
                                          StringArray* app_hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  const bool result = nicknames && app_hmi_types;
  if (result) {
    cache_->GetInitialAppData(application_id, *nicknames, *app_hmi_types);
  }
  return result;
}

void PolicyManagerImpl::AddDevice(const std::string& device_id,
                                  const std::string& connection_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Device: " << device_id);
  if (!cache_->AddDevice(device_id, connection_type)) {
    LOG4CXX_WARN(logger_, "Can't add device.");
  }
}

void PolicyManagerImpl::SetDeviceInfo(const std::string& device_id,
                                      const DeviceInfo& device_info) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Device :" << device_id);
  if (!cache_->SetDeviceData(device_id,
                             device_info.hardware,
                             device_info.firmware_rev,
                             device_info.os,
                             device_info.os_ver,
                             device_info.carrier,
                             device_info.max_number_rfcom_ports,
                             device_info.connection_type)) {
    LOG4CXX_WARN(logger_, "Can't set device data.");
  }
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

  for (; it != it_end; ++it) {
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

void PolicyManagerImpl::CheckPendingPermissionsChanges(
    const std::string& policy_app_id,
    const std::vector<FunctionalGroupPermission>& current_permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(app_permissions_diff_lock_);
  std::map<std::string, AppPermissions>::iterator it_pending =
      app_permissions_diff_.find(policy_app_id);
  if (app_permissions_diff_.end() == it_pending) {
    LOG4CXX_WARN(
        logger_,
        "No pending permissions had been found for appID: " << policy_app_id);
    return;
  }

  LOG4CXX_DEBUG(
      logger_,
      "Pending permissions had been found for appID: " << policy_app_id);

  // Change appPermissionsConsentNeeded depending on unconsented groups
  // presence
  std::vector<policy::FunctionalGroupPermission>::const_iterator it_groups =
      current_permissions.begin();
  std::vector<policy::FunctionalGroupPermission>::const_iterator it_end_groups =
      current_permissions.end();

  for (; it_groups != it_end_groups; ++it_groups) {
    if (policy::kGroupUndefined == it_groups->state) {
      LOG4CXX_DEBUG(
          logger_,
          "Unconsented groups still present for appID: " << policy_app_id);
      it_pending->second.appPermissionsConsentNeeded = true;
      return;
    }
  }

  LOG4CXX_DEBUG(
      logger_,
      "Unconsented groups not present anymore for appID: " << policy_app_id);
  it_pending->second.appPermissionsConsentNeeded = false;
  return;
}

void PolicyManagerImpl::SetUserConsentForApp(
    const PermissionConsent& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->ResetCalculatedPermissions();
  PermissionConsent verified_permissions =
      EnsureCorrectPermissionConsent(permissions);

  if (!cache_->SetUserPermissionsForApp(verified_permissions)) {
    LOG4CXX_WARN(logger_, "Can't set user permissions for application.");
  }

  // Send OnPermissionChange notification, since consents were changed
  std::vector<FunctionalGroupPermission> app_group_permissons;
  GetPermissionsForApp(verified_permissions.device_id,
                       verified_permissions.policy_app_id,
                       app_group_permissons);

  // Change pending permissions isConsentNeeded state, if no unconsented
  // groups left
  CheckPendingPermissionsChanges(permissions.policy_app_id,
                                 app_group_permissons);

  // Get current functional groups from DB with RPC permissions
  policy_table::FunctionalGroupings functional_groups;
  cache_->GetFunctionalGroupings(functional_groups);

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
  PrepareNotificationData(
      functional_groups, app_groups, app_group_permissons, notification_data);

  listener()->OnPermissionsUpdated(verified_permissions.policy_app_id,
                                   notification_data);
}

bool PolicyManagerImpl::GetDefaultHmi(const std::string& policy_app_id,
                                      std::string* default_hmi) const {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string device_id = GetCurrentDeviceId(policy_app_id);
  DeviceConsent device_consent = GetUserConsentForDevice(device_id);
  const std::string app_id = policy::kDeviceAllowed != device_consent
                                 ? kPreDataConsentId
                                 : policy_app_id;
  return cache_->GetDefaultHMI(app_id, *default_hmi);
}

bool PolicyManagerImpl::GetPriority(const std::string& policy_app_id,
                                    std::string* priority) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!priority) {
    LOG4CXX_WARN(logger_, "Input priority parameter is null.");
    return false;
  }

  return cache_->GetPriority(policy_app_id, *priority);
}

std::vector<UserFriendlyMessage> PolicyManagerImpl::GetUserFriendlyMessages(
    const std::vector<std::string>& message_code,
    const std::string& language,
    const std::string& active_hmi_language) {
  return cache_->GetUserFriendlyMsg(
      message_code, language, active_hmi_language);
}

void PolicyManagerImpl::GetUserConsentForApp(
    const std::string& device_id,
    const std::string& policy_app_id,
    std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);

  FunctionalIdType group_types;
  if (!cache_->GetPermissionsForApp(device_id, policy_app_id, group_types)) {
    LOG4CXX_WARN(logger_,
                 "Can't get user permissions for app " << policy_app_id);
    return;
  }

  // Functional groups w/o alias ("user_consent_prompt") considered as
  // automatically allowed and it could not be changed by user
  FunctionalGroupNames group_names;
  if (!cache_->GetFunctionalGroupNames(group_names)) {
    LOG4CXX_WARN(logger_, "Can't get functional group names");
    return;
  }

  FunctionalGroupNames::const_iterator it = group_names.begin();
  FunctionalGroupNames::const_iterator it_end = group_names.end();
  FunctionalGroupIDs auto_allowed_groups;
  for (; it != it_end; ++it) {
    if (it->second.first.empty()) {
      auto_allowed_groups.push_back(it->first);
    }
  }

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

  FunctionalGroupIDs allowed_groups =
      Merge(consent_allowed_groups, preconsented_wo_disallowed_auto);

  FunctionalGroupIDs merged_stage_1 =
      Merge(default_groups, predataconsented_groups);

  FunctionalGroupIDs merged_stage_2 = Merge(merged_stage_1, device_groups);

  FunctionalGroupIDs merged_stage_3 =
      Merge(merged_stage_2, auto_allowed_groups);

  FunctionalGroupIDs excluded_stage_1 = ExcludeSame(all_groups, merged_stage_3);

  FunctionalGroupIDs excluded_stage_2 =
      ExcludeSame(excluded_stage_1, consent_disallowed_groups);

  FunctionalGroupIDs undefined_consent =
      ExcludeSame(excluded_stage_2, allowed_groups);

  // Fill result
  FillFunctionalGroupPermissions(
      undefined_consent, group_names, kGroupUndefined, permissions);
  FillFunctionalGroupPermissions(
      allowed_groups, group_names, kGroupAllowed, permissions);
  FillFunctionalGroupPermissions(
      consent_disallowed_groups, group_names, kGroupDisallowed, permissions);
}

void PolicyManagerImpl::GetPermissionsForApp(
    const std::string& device_id,
    const std::string& policy_app_id,
    std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string app_id_to_check = policy_app_id;

  bool allowed_by_default = false;
  if (cache_->IsDefaultPolicy(policy_app_id)) {
    app_id_to_check = kDefaultId;
    allowed_by_default = true;
  } else if (cache_->IsPredataPolicy(policy_app_id) ||
             policy::kDeviceDisallowed == GetUserConsentForDevice(device_id)) {
    app_id_to_check = kPreDataConsentId;
    allowed_by_default = true;
  }

  FunctionalIdType group_types;
  if (!cache_->GetPermissionsForApp(device_id, app_id_to_check, group_types)) {
    LOG4CXX_WARN(logger_,
                 "Can't get user permissions for app " << policy_app_id);
    return;
  }

  // Functional groups w/o alias ("user_consent_prompt") considered as
  // automatically allowed and it could not be changed by user
  FunctionalGroupNames group_names;
  if (!cache_->GetFunctionalGroupNames(group_names)) {
    LOG4CXX_WARN(logger_, "Can't get functional group names");
    return;
  }

  // The "default" and "pre_DataConsent" are auto-allowed groups
  // So, check if application in the one of these mode.
  if (allowed_by_default) {
    LOG4CXX_INFO(logger_, "Get auto allowed groups");
    GroupType type =
        (kDefaultId == app_id_to_check ? kTypeDefault : kTypePreDataConsented);

    FillFunctionalGroupPermissions(
        group_types[type], group_names, kGroupAllowed, permissions);
  } else {
    // The code bellow allows to process application which
    // has specific permissions(not default and pre_DataConsent).

    // All groups for specific application
    FunctionalGroupIDs all_groups = group_types[kTypeGeneral];

    // Groups assigned by the user for specific application
    FunctionalGroupIDs allowed_groups = group_types[kTypeAllowed];

    // Groups disallowed by the user for specific application
    FunctionalGroupIDs common_disallowed = group_types[kTypeDisallowed];

    // Groups that allowed by default but can be changed by the user
    FunctionalGroupIDs preconsented_groups = group_types[kTypePreconsented];

    // Groups which has user consent promt but there is no any consnets now.
    FunctionalGroupIDs unconsented_groups = group_types[kTypeUnconsented];

    // Pull common groups from allowed and preconsented parts.
    FunctionalGroupIDs allowed_preconsented =
        Merge(allowed_groups, preconsented_groups);

    // Get all groups that we suppose are allowed.
    FunctionalGroupIDs all_allowed = Merge(allowed_preconsented, all_groups);

    // In case when same groups exists in disallowed and allowed tables,
    // disallowed one have priority over allowed. So we have to remove
    // all disallowed groups from allowed table.
    FunctionalGroupIDs common_allowed =
        ExcludeSame(all_allowed, common_disallowed);
    FunctionalGroupIDs consent_disallowed =
        ExcludeSame(unconsented_groups, preconsented_groups);

    // Disallowed groups are contain all directly disallowed,
    // plus unconsented minus preconsented.
    FunctionalGroupIDs all_disallowed =
        Merge(common_disallowed, consent_disallowed);

    // Fill result
    FillFunctionalGroupPermissions(
        consent_disallowed, group_names, kGroupUndefined, permissions);
    FillFunctionalGroupPermissions(
        common_allowed, group_names, kGroupAllowed, permissions);
    FillFunctionalGroupPermissions(
        all_disallowed, group_names, kGroupDisallowed, permissions);
  }
  return;
}

std::string& PolicyManagerImpl::GetCurrentDeviceId(
    const std::string& policy_app_id) const {
  LOG4CXX_INFO(logger_, "GetDeviceInfo");
  last_device_id_ = listener()->OnCurrentDeviceIdUpdateRequired(policy_app_id);
  return last_device_id_;
}

void PolicyManagerImpl::SetSystemLanguage(const std::string& language) {
  cache_->SetSystemLanguage(language);
}

void PolicyManagerImpl::SetSystemInfo(const std::string& ccpu_version,
                                      const std::string& wers_country_code,
                                      const std::string& language) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->SetMetaInfo(ccpu_version, wers_country_code, language);
}

void PolicyManagerImpl::OnSystemReady() {
  // Update policy table for the first time with system information
  if (!cache_->IsMetaInfoPresent()) {
    listener()->OnSystemInfoUpdateRequired();
  }
}

uint32_t PolicyManagerImpl::GetNotificationsNumber(
    const std::string& priority) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetNotificationsNumber(priority);
}

bool PolicyManagerImpl::ExceededIgnitionCycles() {
  return 0 == cache_->IgnitionCyclesBeforeExchange();
}

bool PolicyManagerImpl::IsPTValid(
    utils::SharedPtr<policy_table::Table> policy_table,
    policy_table::PolicyTableType type) const {
  policy_table->SetPolicyTableType(type);
  if (!policy_table->is_valid()) {
    LOG4CXX_ERROR(logger_, "Policy table is not valid.");
    rpc::ValidationReport report("policy_table");
    policy_table->ReportErrors(&report);
    LOG4CXX_DEBUG(logger_, "Errors: " << rpc::PrettyFormat(report));
    return false;
  }
  return true;
}

const PolicySettings& PolicyManagerImpl::get_settings() const {
  DCHECK(settings_);
  return *settings_;
}

bool PolicyManagerImpl::ExceededDays() {
  LOG4CXX_AUTO_TRACE(logger_);

  TimevalStruct current_time = date_time::DateTime::getCurrentTime();
  const int kSecondsInDay = 60 * 60 * 24;
  const int days = current_time.tv_sec / kSecondsInDay;

  DCHECK(std::numeric_limits<uint16_t>::max() >= days);

  if (std::numeric_limits<uint16_t>::max() <= days) {
    LOG4CXX_WARN(logger_, "The days since epoch exceeds maximum value.");
    return false;
  }
  return 0 == cache_->DaysBeforeExchange(static_cast<uint16_t>(days));
}

void PolicyManagerImpl::KmsChanged(int kilometers) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (0 == cache_->KilometersBeforeExchange(kilometers)) {
    LOG4CXX_INFO(logger_, "Enough kilometers passed to send for PT update.");
    update_status_manager_.ScheduleUpdate();
    StartPTExchange();
    PTUpdatedAt(KILOMETERS, kilometers);
  }
}

void PolicyManagerImpl::IncrementIgnitionCycles() {
  cache_->IncrementIgnitionCycles();
}

std::string PolicyManagerImpl::ForcePTExchange() {
  update_status_manager_.ScheduleUpdate();
  StartPTExchange();
  return update_status_manager_.StringifiedUpdateStatus();
}

std::string PolicyManagerImpl::GetPolicyTableStatus() const {
  return update_status_manager_.StringifiedUpdateStatus();
}

int PolicyManagerImpl::NextRetryTimeout() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  LOG4CXX_DEBUG(logger_, "Index: " << retry_sequence_index_);
  int next = 0;
  if (!retry_sequence_seconds_.empty() &&
      retry_sequence_index_ < retry_sequence_seconds_.size()) {
    next = retry_sequence_seconds_[retry_sequence_index_];
    ++retry_sequence_index_;
  }
  return next;
}

void PolicyManagerImpl::RefreshRetrySequence() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  retry_sequence_timeout_ = cache_->TimeoutResponse();
  retry_sequence_seconds_.clear();
  cache_->SecondsBetweenRetries(retry_sequence_seconds_);
}

void PolicyManagerImpl::ResetRetrySequence() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  retry_sequence_index_ = 0;
  update_status_manager_.OnResetRetrySequence();
}

uint32_t PolicyManagerImpl::TimeoutExchangeMSec() {
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
  uint32_t update_timeout = TimeoutExchangeMSec();
  LOG4CXX_DEBUG(logger_,
                "Update timeout will be set to (milisec): " << update_timeout);
  update_status_manager_.OnUpdateSentOut(update_timeout);
  cache_->SaveUpdateRequired(true);
}

void PolicyManagerImpl::PTUpdatedAt(Counters counter, int value) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->SetCountersPassedForSuccessfulUpdate(counter, value);
  cache_->ResetIgnitionCycles();
}

void PolicyManagerImpl::Increment(usage_statistics::GlobalCounterId type) {
  LOG4CXX_INFO(logger_, "Increment without app id");
  cache_->Increment(type);
}

void PolicyManagerImpl::Increment(const std::string& app_id,
                                  usage_statistics::AppCounterId type) {
  LOG4CXX_DEBUG(logger_, "Increment " << app_id << " AppCounter: " << type);
  cache_->Increment(app_id, type);
}

void PolicyManagerImpl::Set(const std::string& app_id,
                            usage_statistics::AppInfoId type,
                            const std::string& value) {
  LOG4CXX_INFO(logger_, "Set " << app_id);
  cache_->Set(app_id, type, value);
}

void PolicyManagerImpl::Add(const std::string& app_id,
                            usage_statistics::AppStopwatchId type,
                            int32_t timespan_seconds) {
  LOG4CXX_INFO(logger_, "Add " << app_id);
  cache_->Add(app_id, type, timespan_seconds);
}

bool PolicyManagerImpl::IsApplicationRevoked(const std::string& app_id) const {
  return cache_->IsApplicationRevoked(app_id);
}

bool PolicyManagerImpl::IsConsentNeeded(const std::string& app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string device_id = GetCurrentDeviceId(app_id);
  int count = cache_->CountUnconsentedGroups(app_id, device_id);
  LOG4CXX_DEBUG(logger_, "There are: " << count << " unconsented groups.");
  return count != 0;
}

void PolicyManagerImpl::SetVINValue(const std::string& value) {
  cache_->SetVINValue(value);
}

AppPermissions PolicyManagerImpl::GetAppPermissionsChanges(
    const std::string& policy_app_id) {
  typedef std::map<std::string, AppPermissions>::iterator PermissionsIt;
  PermissionsIt app_id_diff = app_permissions_diff_.find(policy_app_id);
  AppPermissions permissions(policy_app_id);
  if (app_permissions_diff_.end() != app_id_diff) {
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

bool PolicyManagerImpl::CanAppKeepContext(const std::string& app_id) const {
  return cache_->CanAppKeepContext(app_id);
}

bool PolicyManagerImpl::CanAppStealFocus(const std::string& app_id) const {
  return cache_->CanAppStealFocus(app_id);
}

void PolicyManagerImpl::MarkUnpairedDevice(const std::string& device_id) {
  if (!cache_->SetUnpairedDevice(device_id)) {
    LOG4CXX_DEBUG(logger_, "Could not set unpaired flag for " << device_id);
    return;
  }
  SetUserConsentForDevice(device_id, false);
}

void PolicyManagerImpl::OnAppRegisteredOnMobile(
    const std::string& application_id) {
  StartPTExchange();
  SendNotificationOnPermissionsUpdated(application_id);
}

const MetaInfo PolicyManagerImpl::GetMetaInfo() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetMetaInfo();
}

std::string PolicyManagerImpl::RetrieveCertificate() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetCertificate();
}

bool PolicyManagerImpl::HasCertificate() const {
  return !cache_->GetCertificate().empty();
}

void PolicyManagerImpl::SetDecryptedCertificate(
    const std::string& certificate) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->SetDecryptedCertificate(certificate);
}

/**
 * @brief The CallStatusChange class notify update manager aboun new application
 */
class CallStatusChange : public utils::Callable {
 public:
  CallStatusChange(UpdateStatusManager& upd_manager,
                   const DeviceConsent& device_consent)
      : upd_manager_(upd_manager), device_consent_(device_consent) {}

  // Callable interface
  void operator()() const {
    upd_manager_.OnNewApplicationAdded(device_consent_);
  }

 private:
  UpdateStatusManager& upd_manager_;
  const DeviceConsent device_consent_;
};

StatusNotifier PolicyManagerImpl::AddApplication(
    const std::string& application_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string device_id = GetCurrentDeviceId(application_id);
  DeviceConsent device_consent = GetUserConsentForDevice(device_id);
  sync_primitives::AutoLock lock(apps_registration_lock_);

  if (IsNewApplication(application_id)) {
    AddNewApplication(application_id, device_consent);
    return utils::MakeShared<CallStatusChange>(update_status_manager_,
                                               device_consent);
  } else {
    PromoteExistedApplication(application_id, device_consent);
    return utils::MakeShared<utils::CallNothing>();
  }
}

void PolicyManagerImpl::RemoveAppConsentForGroup(
    const std::string& app_id, const std::string& group_name) {
  cache_->RemoveAppConsentForGroup(app_id, group_name);
}

bool PolicyManagerImpl::IsPredataPolicy(
    const std::string& policy_app_id) const {
  LOG4CXX_INFO(logger_, "IsPredataApp");
  return cache_->IsPredataPolicy(policy_app_id);
}

void PolicyManagerImpl::AddNewApplication(const std::string& application_id,
                                          DeviceConsent device_consent) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (kDeviceHasNoConsent == device_consent ||
      kDeviceDisallowed == device_consent) {
    LOG4CXX_INFO(logger_,
                 "Setting "
                     << policy::kPreDataConsentId
                     << " permissions for application id: " << application_id);
    cache_->SetPredataPolicy(application_id);
  } else {
    LOG4CXX_INFO(logger_,
                 "Setting "
                     << policy::kDefaultId
                     << " permissions for application id: " << application_id);
    cache_->SetDefaultPolicy(application_id);
  }
}

void PolicyManagerImpl::PromoteExistedApplication(
    const std::string& application_id, DeviceConsent device_consent) {
  // If device consent changed to allowed during application being
  // disconnected, app permissions should be changed also
  if (kDeviceAllowed == device_consent &&
      cache_->IsPredataPolicy(application_id)) {
    cache_->SetDefaultPolicy(application_id);
  }
  if (HasCertificate()) {
    LOG4CXX_DEBUG(logger_, "Certificate exits, no update required.");
    return;
  }

  if (cache_->AppHasHMIType(application_id, policy_table::AHT_NAVIGATION)) {
    update_status_manager_.ScheduleUpdate();
  }
}

bool PolicyManagerImpl::IsNewApplication(
    const std::string& application_id) const {
  return false == cache_->IsApplicationRepresented(application_id);
}

bool PolicyManagerImpl::ResetPT(const std::string& file_name) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->ResetCalculatedPermissions();
  const bool result = cache_->ResetPT(file_name);
  if (result) {
    RefreshRetrySequence();
  }
  return result;
}

bool PolicyManagerImpl::CheckAppStorageFolder() const {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string app_storage_folder = get_settings().app_storage_folder();
  LOG4CXX_DEBUG(logger_, "AppStorageFolder " << app_storage_folder);
  if (!file_system::DirectoryExists(app_storage_folder)) {
    LOG4CXX_WARN(logger_,
                 "Storage directory doesn't exist " << app_storage_folder);
    return false;
  }
  if (!(file_system::IsWritingAllowed(app_storage_folder) &&
        file_system::IsReadingAllowed(app_storage_folder))) {
    LOG4CXX_WARN(logger_,
                 "Storage directory doesn't have read/write permissions "
                     << app_storage_folder);
    return false;
  }
  return true;
}

bool PolicyManagerImpl::InitPT(const std::string& file_name,
                               const PolicySettings* settings) {
  LOG4CXX_AUTO_TRACE(logger_);
  settings_ = settings;
  if (!CheckAppStorageFolder()) {
    LOG4CXX_ERROR(logger_, "Can not read/write into AppStorageFolder");
    return false;
  }
  const bool ret = cache_->Init(file_name, settings);
  if (ret) {
    RefreshRetrySequence();
    update_status_manager_.OnPolicyInit(cache_->UpdateRequired());
  }
  return ret;
}

uint32_t PolicyManagerImpl::HeartBeatTimeout(const std::string& app_id) const {
  return cache_->HeartBeatTimeout(app_id);
}

void PolicyManagerImpl::SaveUpdateStatusRequired(bool is_update_needed) {
  cache_->SaveUpdateRequired(is_update_needed);
}

void PolicyManagerImpl::set_cache_manager(
    CacheManagerInterface* cache_manager) {
  cache_ = cache_manager;
}

}  //  namespace policy

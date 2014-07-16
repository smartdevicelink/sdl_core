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
void CheckPreloadedGroups(policy_table::ApplicationParams& app_param) {

}
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
    exchange_in_progress_(false),
    update_required_(false),
    exchange_pending_(false),
    retry_sequence_timeout_(60),
    retry_sequence_index_(0),
    last_update_status_(policy::StatusUnknown) {
}

void PolicyManagerImpl::ResetDefaultPT(const PolicyTable& policy_table) {
  policy_table_ = policy_table;
  exchange_in_progress_ = false;
  update_required_ = policy_table_.pt_data()->UpdateRequired();
  exchange_pending_ = false;
  retry_sequence_index_ = 0;
  last_update_status_ = policy::StatusUnknown;
  RefreshRetrySequence();
}

void PolicyManagerImpl::set_listener(PolicyListener* listener) {
  listener_ = listener;
}

PolicyManagerImpl::~PolicyManagerImpl() {
  LOG4CXX_INFO(logger_, "Destroying policy manager.");
  policy_table_.pt_data()->SaveUpdateRequired(update_required_);
}

bool PolicyManagerImpl::LoadPTFromFile(const std::string& file_name) {
  LOG4CXX_INFO(logger_, "LoadPTFromFile: " << file_name);

  BinaryMessage json_string;
  bool final_result = false;
  final_result = file_system::ReadBinaryFile(file_name, json_string);
  if (!final_result) {
    LOG4CXX_WARN(logger_, "Failed to read pt file.");
    utils::SharedPtr<policy_table::Table> table = new policy_table::Table();
    return final_result;
  }
  utils::SharedPtr<policy_table::Table> table = Parse(json_string);
  if (!table) {
    LOG4CXX_WARN(logger_, "Failed to parse policy table");
    utils::SharedPtr<policy_table::Table> table = new policy_table::Table();
    return false;
  }
  final_result = final_result && policy_table_.pt_data()->Save(*table);
  LOG4CXX_INFO(
    logger_,
    "Loading from file was " << (final_result ? "successful" : "unsuccessful"));

  // Initial setting of snapshot data
  if (!policy_table_snapshot_) {
    policy_table_snapshot_ = policy_table_.pt_data()->GenerateSnapshot();
    if (!policy_table_snapshot_) {
      LOG4CXX_WARN(logger_,
                   "Failed to create initial snapshot of policy table");
      return final_result;
    }
  }

  RefreshRetrySequence();
  return final_result;
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
  LOG4CXX_INFO(logger_, "LoadPTFromString of size " << pt_content.size());

  // Parse message into table struct
  utils::SharedPtr<policy_table::Table> pt_update = Parse(pt_content);

  if (!pt_update) {
    LOG4CXX_WARN(logger_, "Parsed table pointer is 0.");
    return false;
  }

#if defined (EXTENDED_POLICY)
  file_system::DeleteFile(file);
#endif

  set_exchange_in_progress(false);

  if (!pt_update->is_valid()) {
    rpc::ValidationReport report("policy_table");
    pt_update->ReportErrors(&report);
    LOG4CXX_WARN(logger_, "Parsed table is not valid " <<
                 rpc::PrettyFormat(report));
    return false;
  }

  // Check and update permissions for applications, send notifications
  CheckPermissionsChanges(pt_update);

  // Replace current data with updated
  policy_table_snapshot_->policy_table.functional_groupings = pt_update
      ->policy_table.functional_groupings;

  policy_table_snapshot_->policy_table.module_config = pt_update->policy_table
      .module_config;

  bool is_message_part_updated = pt_update->policy_table
                                 .consumer_friendly_messages.is_initialized();

  if (is_message_part_updated) {
    policy_table_snapshot_->policy_table.consumer_friendly_messages->messages =
      pt_update->policy_table.consumer_friendly_messages->messages;
  }

  // Save data to DB
  if (!policy_table_.pt_data()->Save(*policy_table_snapshot_)) {
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
  const utils::SharedPtr<policy_table::Table> pt_update) {
  LOG4CXX_INFO(logger_, "Checking incoming permissions.");

  std::for_each(pt_update->policy_table.app_policies.begin(),
                pt_update->policy_table.app_policies.end(),
                CheckAppPolicy(this, pt_update));
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

void PolicyManagerImpl::set_exchange_in_progress(bool value) {
  sync_primitives::AutoLock lock(exchange_in_progress_lock_);
  LOG4CXX_INFO(logger_,
               "Exchange in progress value is:" << std::boolalpha << value);
  exchange_in_progress_ = value;
  CheckUpdateStatus();
}

void PolicyManagerImpl::set_exchange_pending(bool value) {
  sync_primitives::AutoLock lock(exchange_pending_lock_);
  LOG4CXX_INFO(logger_,
               "Exchange pending value is:" << std::boolalpha << value);
  exchange_pending_ = value;
  CheckUpdateStatus();
}

void PolicyManagerImpl::set_update_required(bool value) {
  sync_primitives::AutoLock lock(update_required_lock_);
  LOG4CXX_INFO(logger_, "Update required value is:" << std::boolalpha << value);
  update_required_ = value;
  CheckUpdateStatus();
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
  EndpointUrls urls = policy_table_.pt_data()->GetUpdateUrls(service_type);

  static uint32_t index = 0;
  std::string url;
  if (index < urls.size()) {
    url = urls[index].url;
  } else if (!urls.empty()) {
    index = 0;
    url = urls[index].url;
  }
  ++index;

  return url;
}

EndpointUrls PolicyManagerImpl::GetUpdateUrls(int service_type) {
  LOG4CXX_INFO(logger_, "PolicyManagerImpl::GetUpdateUrls");
  return policy_table_.pt_data()->GetUpdateUrls(service_type);
}

BinaryMessageSptr PolicyManagerImpl::RequestPTUpdate() {
  LOG4CXX_INFO(logger_, "Creating PT Snapshot");  
  policy_table_snapshot_ = policy_table_.pt_data()->GenerateSnapshot();
  if (!policy_table_snapshot_) {
    LOG4CXX_ERROR(logger_, "Failed to create snapshot of policy table");
    return NULL;
  }

  policy_table::ApplicationPolicies& apps = policy_table_snapshot_->
                                            policy_table.app_policies;
  std::map<std::string,
      rpc::Stringifyable<rpc::Nullable<
      rpc::policy_table_interface_base::ApplicationParams>>>::iterator it =
      apps.begin();
  // TODO(AOleynik): Should we check here for 'device' also?
  for (; apps.end() != it; ++it) {
    if (policy_table_.pt_data()->IsDefaultPolicy(it->first)) {
      it->second.set_to_string(kDefaultId);
      continue;
    }
    if (policy_table_.pt_data()->IsPredataPolicy(it->first)) {
      it->second.set_to_string(kPreDataConsentId);
      continue;
    }
  }

  set_exchange_in_progress(true);
  if (!exchange_pending_) {
    set_update_required(false);
  }
  set_exchange_pending(false);

#ifdef EXTENDED_POLICY
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {
    pt_ext->UnpairedDevicesList(&unpaired_device_ids_);
  }
#endif  // EXTENDED_POLICY

  Json::Value value = policy_table_snapshot_->ToJsonValue();
  Json::FastWriter writer;
  std::string message_string = writer.write(value);
  return new BinaryMessage(message_string.begin(), message_string.end());
}

CheckPermissionResult PolicyManagerImpl::CheckPermissions(
  const PTString& app_id, const PTString& hmi_level, const PTString& rpc) {
  LOG4CXX_INFO(
    logger_,
    "CheckPermissions for " << app_id << " and rpc " << rpc << " for "
    << hmi_level << " level.");

#if defined (EXTENDED_POLICY)
  const std::string device_id = GetCurrentDeviceId(app_id);
  // Get actual application group permission according to user consents
  std::vector<FunctionalGroupPermission> app_group_permissions;
  GetPermissionsForApp(device_id, app_id, app_group_permissions);

  // Fill struct with known groups RPCs
  policy_table::FunctionalGroupings functional_groupings;
  policy_table_.pt_data()->GetFunctionalGroupings(functional_groupings);

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

  CheckPermissionResult result;
  if (rpc_permissions.end() == rpc_permissions.find(rpc)) {
    // RPC not found in list == disallowed by backend
    result.hmi_level_permitted = kRpcDisallowed;
    return result;
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
  result.list_of_allowed_params = new std::vector<PTString>();
  std::copy(rpc_permissions[rpc].parameter_permissions[kAllowedKey].begin(),
            rpc_permissions[rpc].parameter_permissions[kAllowedKey].end(),
            std::back_inserter(*result.list_of_allowed_params));

  result.list_of_disallowed_params = new std::vector<PTString>();
  std::copy(
        rpc_permissions[rpc].parameter_permissions[kUserDisallowedKey].begin(),
        rpc_permissions[rpc].parameter_permissions[kUserDisallowedKey].end(),
        std::back_inserter(*result.list_of_disallowed_params));

  result.list_of_undefined_params = new std::vector<PTString>();
  std::copy(rpc_permissions[rpc].parameter_permissions[kUndefinedKey].begin(),
            rpc_permissions[rpc].parameter_permissions[kUndefinedKey].end(),
            std::back_inserter(*result.list_of_undefined_params));

  return result;
#else
  return policy_table_.pt_data()->CheckPermissions(app_id, hmi_level, rpc);
#endif
}

bool PolicyManagerImpl::ResetUserConsent() {
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {
    return pt_ext->ResetUserConsent();
  }
  return false;
#else
  return true;
#endif
}

void PolicyManagerImpl::CheckAppPolicyState(const std::string& application_id) {
  LOG4CXX_INFO(logger_, "CheckAppPolicyState");
  const std::string device_id = GetCurrentDeviceId(application_id);
  DeviceConsent device_consent  = GetUserConsentForDevice(device_id);
  if (!policy_table_.pt_data()->IsApplicationRepresented(application_id)) {
    LOG4CXX_INFO(
      logger_,
      "Setting default permissions for application id: " << application_id);
#if defined (EXTENDED_POLICY)
    if (kDeviceHasNoConsent == device_consent ||
        kDeviceDisallowed == device_consent) {
      PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(
                                      policy_table_.pt_data().get());
      if (!pt_ext) {
        LOG4CXX_WARN(logger_, "Can't cleanup unpaired devices.");
        return;
      }
      pt_ext->SetPredataPolicy(application_id);
    } else {
      policy_table_.pt_data()->SetDefaultPolicy(application_id);
    }
#else
    policy_table_.pt_data()->SetDefaultPolicy(application_id);
#endif
  } else if (kDeviceHasNoConsent != device_consent
            && policy_table_.pt_data()->IsPredataPolicy(application_id)) {
      // If device consent changed to allowed during application being
      // disconnected, app permissions should be changed also
      if (kDeviceAllowed == device_consent) {
        policy_table_.pt_data()->SetDefaultPolicy(application_id);
      }
      SendNotificationOnPermissionsUpdated(application_id);
      return;
  } else if (!policy_table_.pt_data()->IsDefaultPolicy(application_id)) {
    SendNotificationOnPermissionsUpdated(application_id);
    return;
  }

  AddAppToUpdateList(application_id);

  if (PolicyTableStatus::StatusUpToDate == GetPolicyTableStatus()) {
    set_update_required(true);
  } else {
    set_exchange_pending(true);
  }

  SendNotificationOnPermissionsUpdated(application_id);
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
  policy_table_.pt_data()->GetFunctionalGroupings(functional_groupings);

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
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (!pt_ext) {
    LOG4CXX_WARN(logger_, "Can't get default hmi level for " << application_id);
    return;
  }

  pt_ext->GetDefaultHMI(application_id, &default_hmi);
#else
  default_hmi = "NONE";
#endif
  listener()->OnPermissionsUpdated(application_id, notification_data,
                                   default_hmi);
}

bool PolicyManagerImpl::CleanupUnpairedDevices() {
  LOG4CXX_INFO(logger_, "CleanupUnpairedDevices");
#ifdef EXTENDED_POLICY
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (!pt_ext) {
    LOG4CXX_WARN(logger_, "Can't cleanup unpaired devices.");
    return false;
  }

  return pt_ext->CleanupUnpairedDevices(unpaired_device_ids_);
#else  // EXTENDED_POLICY
  // For SDL-specific it doesn't matter
  return true;
#endif  // EXTENDED_POLICY
}

PolicyTableStatus PolicyManagerImpl::GetPolicyTableStatus() {
  if (!exchange_in_progress_ && !exchange_pending_ && !update_required_) {
    return PolicyTableStatus::StatusUpToDate;
  }

  if (update_required_ && !exchange_in_progress_ && !exchange_pending_) {
    return PolicyTableStatus::StatusUpdateRequired;
  }

  return PolicyTableStatus::StatusUpdatePending;
}

DeviceConsent PolicyManagerImpl::GetUserConsentForDevice(
  const std::string& device_id) {
  LOG4CXX_INFO(logger_, "GetUserConsentForDevice");
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (!pt_ext) {
    LOG4CXX_WARN(logger_, "Can't get user consent for device");
    return kDeviceDisallowed;
  }

  // Get device permission groups from app_policies section, which hadn't been
  // preconsented
  policy_table::Strings groups;
  policy_table::Strings preconsented_groups;
  if (!pt_ext->GetDeviceGroupsFromPolicies(&groups, &preconsented_groups)) {
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
    if (!pt_ext->GetUserPermissionsForDevice(device_id, &consented_groups,
        &disallowed_groups)) {
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
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (!pt_ext) {
    LOG4CXX_WARN(logger_, "Can't set user consent for device");
    return;
  }

  // Get device permission groups from app_policies section, which hadn't been
  // preconsented
  policy_table::Strings groups;
  policy_table::Strings preconsented_groups;
  if (!pt_ext->GetDeviceGroupsFromPolicies(&groups, &preconsented_groups)) {
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

  if (!pt_ext->SetUserPermissionsForDevice(device_id, consented_groups,
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
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (!pt_ext) {
    LOG4CXX_WARN(logger_, "Can't set user consent for device");
    return false;
  }
  // TODO(AOleynik): Move out logic from PT representation to policy manager
  return pt_ext->ReactOnUserDevConsentForApp(app_id, is_device_allowed);
#endif
  return true;
}

bool PolicyManagerImpl::GetInitialAppData(const std::string& application_id,
    StringArray* nicknames,
    StringArray* app_hmi_types) {
  LOG4CXX_INFO(logger_, "GetInitialAppData");
  return policy_table_.pt_data()->GetInitialAppData(application_id, nicknames,
         app_hmi_types);
}

void PolicyManagerImpl::SetDeviceInfo(const std::string& device_id,
                                      const DeviceInfo& device_info) {
  LOG4CXX_INFO(logger_, "SetDeviceInfo");
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {
    if (!pt_ext->SetDeviceData(device_id, device_info.hardware,
                               device_info.firmware_rev, device_info.os,
                               device_info.os_ver, device_info.carrier,
                               device_info.max_number_rfcom_ports)) {
      LOG4CXX_WARN(logger_, "Can't set device data.");
    }
  }
#endif
}

void PolicyManagerImpl::EnsureCorrectPermissionConsent(const FunctionalGroupNames& group_names, PermissionConsent& permissions)
{
    std::vector<FunctionalGroupPermission>::iterator group_perm_iter =
            permissions.group_permissions.begin();

    std::vector<FunctionalGroupPermission>::iterator group_perm_iter_end =
            permissions.group_permissions.end();

    for(; group_perm_iter != group_perm_iter_end; ++group_perm_iter) {

      const std::uint32_t id = (*group_perm_iter).group_id;
      FunctionalGroupNames::const_iterator group_name_iter = group_names.find(id);

      if (group_names.end() == group_name_iter) {
        LOG4CXX_WARN(logger_, "Can't change user consent for unexisted function."
                     << "\t\nid: " << id
                     << "\t\nalias: " << (*group_perm_iter).group_alias
                     << "\t\ngroup name: " << (*group_perm_iter).group_name);
      }
      // check if group_alias is not empty string
      // which means it has user_consent_promt ability
      else if ((*group_name_iter).second.first.empty()) {
        LOG4CXX_WARN(logger_, "Specified function is not in user consent group."
                      << "\t\nid: " << id
                      << "\t\nalias: " << (*group_perm_iter).group_alias
                      << "\t\ngroup name: " << (*group_perm_iter).group_name);

        permissions.group_permissions.erase(group_perm_iter);
      }
    }
}

void PolicyManagerImpl::SetUserConsentForApp(
  PermissionConsent& permissions) {
  LOG4CXX_INFO(logger_, "SetUserConsentForApp");
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {

    FunctionalGroupNames group_names;
    if (!pt_ext->GetFunctionalGroupNames(group_names)) {
      LOG4CXX_WARN(logger_, "Can't get functional group names");
      return;
    }

    EnsureCorrectPermissionConsent(group_names, permissions);

    // TODO(AOleynik): Change device id to appropriate value (MAC with SHA-256)
    // in parameters
    if (!pt_ext->SetUserPermissionsForApp(permissions)) {
      LOG4CXX_WARN(logger_, "Can't set user permissions for application.");
    }
    // Send OnPermissionChange notification, since consents were changed
    std::vector<FunctionalGroupPermission> app_group_permissons;
    GetPermissionsForApp(permissions.device_id,
                             permissions.policy_app_id,
                             app_group_permissons);

    // Get current functional groups from DB with RPC permissions
    policy_table::FunctionalGroupings functional_groups;
    policy_table_.pt_data()->GetFunctionalGroupings(functional_groups);

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
    pt_ext->GetDefaultHMI(permissions.policy_app_id, &default_hmi);

    listener()->OnPermissionsUpdated(permissions.policy_app_id,
                                     notification_data,
                                     default_hmi);
  }
#endif
}

bool PolicyManagerImpl::GetDefaultHmi(const std::string& policy_app_id,
                                      std::string* default_hmi) {
  LOG4CXX_INFO(logger_, "GetDefaultHmi");
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (!pt_ext) {
    LOG4CXX_WARN(logger_, "Can't get default hmi level.");
    return false;
  }

  return pt_ext->GetDefaultHMI(policy_app_id, default_hmi);
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
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (!pt_ext) {
    LOG4CXX_WARN(logger_, "Can't get priority.");
    return false;
  }

  return pt_ext->GetPriority(policy_app_id, priority);
#else
  priority->clear();
  return true;
#endif
}

std::vector<UserFriendlyMessage> PolicyManagerImpl::GetUserFriendlyMessages(
  const std::vector<std::string>& message_code, const std::string& language) {
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  // For extended policy
  if (pt_ext) {
    return pt_ext->GetUserFriendlyMsg(message_code, language);
  }
#endif
  // For basic policy
  return policy_table_.pt_data()->GetUserFriendlyMsg(message_code, language);
}

void PolicyManagerImpl::GetUserConsentForApp(
  const std::string& device_id, const std::string& policy_app_id,
  std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_INFO(logger_, "GetUserConsentForApp");
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {
    FunctionalIdType group_types;
    if (!pt_ext->GetPermissionsForApp(device_id, policy_app_id,
                                          &group_types)) {
      LOG4CXX_WARN(logger_, "Can't get user permissions for app "
                   << policy_app_id);
      return;
    }

    // Functional groups w/o alias ("user_consent_prompt") considered as
    // automatically allowed and it could not be changed by user
    FunctionalGroupNames group_names;
    if (!pt_ext->GetFunctionalGroupNames(group_names)) {
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
  }
#else
  // For basic policy
  permissions = std::vector<FunctionalGroupPermission>();
#endif
}

void PolicyManagerImpl::GetPermissionsForApp(
  const std::string& device_id, const std::string& policy_app_id,
  std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_INFO(logger_, "GetPermissionsForApp");
#if defined (EXTENDED_POLICY)
  std::string app_id_to_check = policy_app_id;  

  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  // For extended policy
  if (pt_ext) {
    if (pt_ext->IsDefaultPolicy(policy_app_id)) {
      app_id_to_check = kDefaultId;
    } else if (pt_ext->IsPredataPolicy(policy_app_id)) {
      app_id_to_check = kPreDataConsentId;
    }

    FunctionalIdType group_types;
    if (!pt_ext->GetPermissionsForApp(device_id, app_id_to_check,
                                          &group_types)) {
      LOG4CXX_WARN(logger_, "Can't get user permissions for app "
                   << policy_app_id);
      return;
    }

    // Functional groups w/o alias ("user_consent_prompt") considered as
    // automatically allowed and it could not be changed by user
    FunctionalGroupNames group_names;
    if (!pt_ext->GetFunctionalGroupNames(group_names)) {
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

    FunctionalGroupIDs all_groups = group_types[kTypeGeneral];
    FunctionalGroupIDs allowed_groups;
    // If application is limited to default only related groups ara allowed
    // If application is limited to pre_Dataconsent only related groups are
    // allowed
    if (kDefaultId == app_id_to_check) {
      allowed_groups = group_types[kTypeDefault];
    } else if (kPreDataConsentId == app_id_to_check) {
      allowed_groups = group_types[kTypePreDataConsented];
    } else {
      allowed_groups = group_types[kTypeAllowed];
    }

    FunctionalGroupIDs disallowed_groups = group_types[kTypeDisallowed];
    FunctionalGroupIDs preconsented_groups = group_types[kTypePreconsented];
    // If application is limited to pre_DataConsent - no default groups
    // permissions should be used
    FunctionalGroupIDs default_groups =
      app_id_to_check == kPreDataConsentId ?
      FunctionalGroupIDs() : group_types[kTypeDefault];

    // Find common disallowed groups
    FunctionalGroupIDs common_disallowed = disallowed_groups;

    // Find common allowed groups
    FunctionalGroupIDs allowed_preconsented = Merge(allowed_groups,
                                              preconsented_groups);

    FunctionalGroupIDs allowed_preconsented_auto = Merge(allowed_preconsented,
                                                         auto_allowed_groups);
    // Default groups always allowed
    FunctionalGroupIDs related_defaults = FindSame(all_groups, default_groups);

    FunctionalGroupIDs all_allowed = Merge(allowed_preconsented_auto,
                                           related_defaults);

    FunctionalGroupIDs common_allowed = ExcludeSame(all_allowed,
                                                    common_disallowed);

    // Find groups with undefinded consent
    FunctionalGroupIDs no_disallowed = ExcludeSame(all_groups,
                                                   common_disallowed);
    FunctionalGroupIDs undefined_consent = ExcludeSame(no_disallowed,
                                                       common_allowed);

    // Fill result
    FillFunctionalGroupPermissions(undefined_consent, group_names,
                                   kGroupUndefined, permissions);
    FillFunctionalGroupPermissions(common_allowed, group_names,
                                   kGroupAllowed, permissions);
    FillFunctionalGroupPermissions(common_disallowed, group_names,
                                   kGroupDisallowed, permissions);

    return;
  }
#else
  // For basic policy
  permissions = std::vector<FunctionalGroupPermission>();
#endif
}

std::string& PolicyManagerImpl::GetCurrentDeviceId(
  const std::string& policy_app_id) {
  LOG4CXX_INFO(logger_, "GetDeviceInfo");
  last_device_id_ =
    listener()->OnCurrentDeviceIdUpdateRequired(policy_app_id);
  return last_device_id_;
}

void PolicyManagerImpl::SetSystemLanguage(const std::string& language) {
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {
    pt_ext->SetSystemLanguage(language);
    return;
  }
#endif
}

void PolicyManagerImpl::SetSystemInfo(const std::string& ccpu_version,
                                      const std::string& wers_country_code,
                                      const std::string& language) {
  LOG4CXX_INFO(logger_, "SetSystemInfo");
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {
    pt_ext->SetMetaInfo(ccpu_version, wers_country_code, language);
    return;
  }
#endif
}

void PolicyManagerImpl::OnSystemReady() {
  // Update policy table for the first time with system information
  if (policy_table_.pt_data()->IsPTPreloaded()) {
    listener()->OnSystemInfoUpdateRequired();
    return;
  }
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext && !pt_ext->IsMetaInfoPresent()) {
    listener()->OnSystemInfoUpdateRequired();
  }
#endif
}

uint32_t PolicyManagerImpl::GetNotificationsNumber(
    const std::string& priority) {
  LOG4CXX_INFO(logger_, "GetNotificationsNumber");
  return policy_table_.pt_data()->GetNotificationsNumber(priority);
}

bool PolicyManagerImpl::ExceededIgnitionCycles() {
  return policy_table_.pt_data()->IgnitionCyclesBeforeExchange() == 0;
}

bool PolicyManagerImpl::ExceededDays(int days) {
  return policy_table_.pt_data()->DaysBeforeExchange(days) == 0;
}

bool PolicyManagerImpl::ExceededKilometers(int kilometers) {
  return policy_table_.pt_data()->KilometersBeforeExchange(kilometers) == 0;
}

void PolicyManagerImpl::IncrementIgnitionCycles() {
  policy_table_.pt_data()->IncrementIgnitionCycles();
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
  retry_sequence_timeout_ = policy_table_.pt_data()->TimeoutResponse();
  retry_sequence_seconds_.clear();
  policy_table_.pt_data()->SecondsBetweenRetries(&retry_sequence_seconds_);
}

void PolicyManagerImpl::ResetRetrySequence() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  retry_sequence_index_ = 0;

  if (exchange_in_progress_) {
    set_exchange_pending(true);
  }
  set_update_required(true);
}

int PolicyManagerImpl::TimeoutExchange() {
  return retry_sequence_timeout_;
}

const std::vector<int> PolicyManagerImpl::RetrySequenceDelaysSeconds() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  return retry_sequence_seconds_;
}

void PolicyManagerImpl::OnExceededTimeout() {
  set_exchange_in_progress(false);
}

void PolicyManagerImpl::PTUpdatedAt(int kilometers, int days_after_epoch) {
  LOG4CXX_INFO(logger_, "PTUpdatedAt");
  LOG4CXX_INFO(logger_,
               "Kilometers: " << kilometers << " Days: " << days_after_epoch);
  policy_table_.pt_data()->SetCountersPassedForSuccessfulUpdate(
    kilometers, days_after_epoch);
  policy_table_.pt_data()->ResetIgnitionCycles();
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
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  pt_ext->Increment(counter);
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
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  sync_primitives::AutoLock locker(statistics_lock_);
  pt_ext->Increment(app_id, counter);
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

#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  sync_primitives::AutoLock locker(statistics_lock_);
  pt_ext->Set(app_id, info, value);
#endif
}

void PolicyManagerImpl::Add(const std::string& app_id,
                            usage_statistics::AppStopwatchId type,
                            int32_t timespan_seconds) {
#if defined (EXTENDED_POLICY)
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
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  sync_primitives::AutoLock locker(statistics_lock_);
  pt_ext->Add(app_id, stopwatch, timespan_seconds);
#endif
}

bool PolicyManagerImpl::IsApplicationRevoked(const std::string& app_id) const {
  return policy_table_.pt_data()->IsApplicationRevoked(app_id);
}

int PolicyManagerImpl::IsConsentNeeded(const std::string& app_id) {
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());

  const std::string device_id = GetCurrentDeviceId(app_id);

  int count = 0;
  if (pt_ext->CountUnconsentedGroups(app_id, device_id, &count)) {
    return count;
  } else {
    return 0;
  }
#endif
  return 0;
}

void PolicyManagerImpl::CheckUpdateStatus() {
  LOG4CXX_INFO(logger_, "CheckUpdateStatus");
  policy::PolicyTableStatus status = GetPolicyTableStatus();
  if (last_update_status_ != status) {
    listener_->OnUpdateStatusChanged(status);
  }
  last_update_status_ = status;
}

AppPermissions PolicyManagerImpl::GetAppPermissionsChanges(
  const std::string& app_id) {
  typedef std::map<std::string, AppPermissions>::const_iterator PermissionsIt;
  PermissionsIt app_id_diff = app_permissions_diff_.find(app_id);
  AppPermissions permissions(app_id);
  if (app_permissions_diff_.end() != app_id_diff) {
    permissions = app_id_diff->second;
  } else {
    permissions.appPermissionsConsentNeeded = IsConsentNeeded(app_id);
    permissions.appRevoked = IsApplicationRevoked(app_id);
    GetPriority(permissions.application_id, &permissions.priority);
  }
  return permissions;
}

void PolicyManagerImpl::RemovePendingPermissionChanges(
  const std::string& app_id) {
  app_permissions_diff_.erase(app_id);
}

bool PolicyManagerImpl::CanAppKeepContext(const std::string& app_id) {
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {
    return pt_ext->CanAppKeepContext(app_id);
  }
  return false;
#else
  return false;
#endif
}

bool PolicyManagerImpl::CanAppStealFocus(const std::string& app_id) {
#if defined (EXTENDED_POLICY)
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {
    return pt_ext->CanAppStealFocus(app_id);
  }
  return false;
#else
  return false;
#endif
}

void PolicyManagerImpl::MarkUnpairedDevice(const std::string& device_id) {
#ifdef EXTENDED_POLICY
  PTExtRepresentation* pt_ext = dynamic_cast<PTExtRepresentation*>(policy_table_
                                .pt_data().get());
  if (pt_ext) {
    pt_ext->SetUnpairedDevice(device_id);
    SetUserConsentForDevice(device_id, false);
  }
#endif  // EXTENDED_POLICY
}

bool PolicyManagerImpl::ResetPT(const std::string& file_name) {
  return policy_table_.pt_data()->Clear() && LoadPTFromFile(file_name);
}

bool PolicyManagerImpl::InitPT(const std::string& file_name) {
  bool ret = false;
  InitResult init_result = policy_table_.pt_data()->Init();
  switch (init_result) {
    case InitResult::EXISTS: {
      LOG4CXX_INFO(logger_, "Policy Table exists, was loaded correctly.");
      ret = true;
    } break;
    case InitResult::SUCCESS: {
      LOG4CXX_INFO(logger_, "Policy Table was inited successfully");
      ret = LoadPTFromFile(file_name);
    } break;
    case InitResult::FAIL:
    default: {
      LOG4CXX_ERROR(logger_, "Failed to init policy table.");
      ret = false;
    }
  }
  RefreshRetrySequence();
  set_update_required(policy_table_.pt_data()->UpdateRequired());
  return ret;
}

}  //  namespace policy


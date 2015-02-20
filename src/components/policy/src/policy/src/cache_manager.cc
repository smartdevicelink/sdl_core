/*
 * Copyright (c) 2014, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "policy/cache_manager.h"

#include <algorithm>
#include <functional>
#include <ctime>
#include <cmath>

#include "utils/file_system.h"
#include "json/reader.h"
#include "json/features.h"
#include "utils/logger.h"

#  include "policy/sql_pt_representation.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "CacheManager")

#define CACHE_MANAGER_CHECK(return_value) {\
  if (!pt_) {\
    LOG4CXX_WARN(logger_, "The cache manager is not initialized");\
    return return_value;\
  }\
}

#define CACHE_MANAGER_CHECK_VOID() {\
  if (!pt_) {\
    LOG4CXX_WARN(logger_, "The cache manager is not initialized");\
    return;\
  }\
}


CacheManager::CacheManager()
  : CacheManagerInterface(),
    backup_(
                     new SQLPTRepresentation()
    ),
    update_required(false) {

  LOG4CXX_AUTO_TRACE(logger_);
  backuper_ = new BackgroundBackuper(this);
  backup_thread_ = threads::CreateThread("Backup thread", backuper_);
  backup_thread_->start();
}

CacheManager::~CacheManager() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(backuper_locker_);
  backup_thread_->join();
  delete backup_thread_->delegate();
  threads::DeleteThread(backup_thread_);
}

bool CacheManager::CanAppKeepContext(const std::string &app_id) {
  CACHE_MANAGER_CHECK(false);
  bool result = true;
  return result;
}

uint16_t CacheManager::HeartBeatTimeout(const std::string &app_id) const {
  CACHE_MANAGER_CHECK(0);
  uint16_t result = 0;
  if (AppExists(app_id)) {
    if (pt_->policy_table.app_policies[app_id].heart_beat_timeout_ms
        .is_initialized()) {
      result = *(pt_->policy_table.app_policies[app_id].heart_beat_timeout_ms);
    }
  }
  return result;
}

bool CacheManager::CanAppStealFocus(const std::string &app_id) {
  CACHE_MANAGER_CHECK(false);
  bool result = true;
  return result;
}

bool CacheManager::GetDefaultHMI(const std::string &app_id,
                                 std::string& default_hmi) {
  CACHE_MANAGER_CHECK(false);
  bool result = true;

  return result;
}

bool CacheManager::ResetUserConsent() {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock lock (cache_lock_);
  return true;
}


bool CacheManager::GetUserPermissionsForDevice(const std::string &device_id,
                                               StringArray& consented_groups,
                                               StringArray& disallowed_groups) {

  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  return true;
}

void CacheManager::GetAllAppGroups(const std::string& app_id,
                                   FunctionalGroupIDs& all_group_ids) {

  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
  policy_table::ApplicationPolicies::const_iterator app_params_iter =
      pt_->policy_table.app_policies.find(app_id);

  if (pt_->policy_table.app_policies.end() != app_params_iter) {
    policy_table::Strings::const_iterator iter =
        (*app_params_iter).second.groups.begin();
    policy_table::Strings::const_iterator iter_end =
        (*app_params_iter).second.groups.end();

    for (; iter != iter_end; ++iter) {
      const uint32_t group_id =
          static_cast<uint32_t> ((GenerateHash(*iter)));
      all_group_ids.push_back(group_id);
    }
  }
}

void CacheManager::GetPreConsentedGroups(const std::string &app_id,
                                         FunctionalGroupIDs& preconsented_groups) {

  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::GetConsentedGroups(const std::string &device_id,
                                      const std::string &app_id,
                                      FunctionalGroupIDs& allowed_groups,
                                      FunctionalGroupIDs& disallowed_groups) {

  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::GetUnconsentedGroups(const std::string& device_id,
                                        const std::string& policy_app_id,
                                        FunctionalGroupIDs& unconsented_groups) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::RemoveAppConsentForGroup(const std::string& app_id,
                                            const std::string& group_name) {
  CACHE_MANAGER_CHECK_VOID();
}

bool CacheManager::ApplyUpdate(const policy_table::Table& update_pt) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  pt_->policy_table.functional_groupings =
      update_pt.policy_table.functional_groupings;

  policy_table::ApplicationPolicies::const_iterator iter =
      update_pt.policy_table.app_policies.begin();
  policy_table::ApplicationPolicies::const_iterator iter_end =
      update_pt.policy_table.app_policies.end();

  for (;iter != iter_end; ++iter) {
    if (iter->second.is_null()) {
      pt_->policy_table.app_policies[iter->first].set_to_null();
      pt_->policy_table.app_policies[iter->first].set_to_string("");
    } else {
      pt_->policy_table.app_policies[iter->first] = iter->second;
    }
  }

  if (update_pt.policy_table.consumer_friendly_messages.is_initialized()) {
    pt_->policy_table.consumer_friendly_messages =
        update_pt.policy_table.consumer_friendly_messages;
  }
  ResetCalculatedPermissions();
  Backup();
  return true;
}

void CacheManager::GetHMIAppTypeAfterUpdate(std::map<std::string, StringArray>& app_hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
  policy_table::ApplicationPolicies::const_iterator policy_iter_begin =
      pt_->policy_table.app_policies.begin();
  policy_table::ApplicationPolicies::const_iterator policy_iter_end =
      pt_->policy_table.app_policies.end();
  std::vector<std::string> transform_app_hmi_types;
  for(; policy_iter_begin != policy_iter_end; ++policy_iter_begin) {
    const policy_table::ApplicationParams& app_params = (*policy_iter_begin).second;
    if(app_params.AppHMIType.is_initialized()) {
      if(!(transform_app_hmi_types.empty())) {
        transform_app_hmi_types.clear();
      }
      std::transform(app_params.AppHMIType->begin(), app_params.AppHMIType->end(),
                     std::back_inserter(transform_app_hmi_types), AppHMITypeToString());
      app_hmi_types[(*policy_iter_begin).first] = transform_app_hmi_types;
    }
  }
}

void CacheManager::Backup() {
  sync_primitives::AutoLock lock(backuper_locker_);
  DCHECK(backuper_);
  backuper_->DoBackup();
}

std::string CacheManager::currentDateTime() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  // ISO_8601 format is expected, e.g. “2000-01-01T12:18:53Z”
  strftime(buf, sizeof(buf), "%Y-%m-%dT%XZ", &tstruct);
  return buf;
}

bool CacheManager::GetPermissionsForApp(const std::string &device_id,
                                        const std::string &app_id,
                                        FunctionalIdType& group_types) {

  LOG4CXX_AUTO_TRACE(logger_);
  GetAllAppGroups(app_id, group_types[kTypeGeneral]);
  GetAllAppGroups(kDefaultId, group_types[kTypeDefault]);
  GetAllAppGroups(kPreDataConsentId, group_types[kTypePreDataConsented]);
  return true;
}

bool CacheManager::GetDeviceGroupsFromPolicies(
  policy_table::Strings& groups,
  policy_table::Strings& preconsented_groups) {

  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  return true;
}

bool CacheManager::SetDeviceData(const std::string &device_id,
                                 const std::string &hardware,
                                 const std::string &firmware,
                                 const std::string &os,
                                 const std::string &os_version,
                                 const std::string &carrier,
                                 const uint32_t number_of_ports,
                                 const std::string &connection_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  sync_primitives::AutoLock auto_lock(cache_lock_);
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::SetUserPermissionsForDevice(
    const std::string &device_id,
    const StringArray &consented_groups,
    const StringArray &disallowed_groups) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::ReactOnUserDevConsentForApp(const std::string &app_id,
                                               bool is_device_allowed) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  bool result = true;
  Backup();
  return result;
}

void CacheManager::GetGroupNameByHashID(const int32_t group_id,
                                        std::string& group_name) {

  CACHE_MANAGER_CHECK_VOID();
  policy_table::FunctionalGroupings::const_iterator fg_iter =
      pt_->policy_table.functional_groupings.begin();
  policy_table::FunctionalGroupings::const_iterator fg_iter_end =
      pt_->policy_table.functional_groupings.end();

  for (; fg_iter != fg_iter_end; ++fg_iter) {
    const int32_t id = GenerateHash((*fg_iter).first);
    if (group_id == id) {
      group_name = (*fg_iter).first;
    }
  }
}

bool CacheManager::SetUserPermissionsForApp(
    const PermissionConsent &permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::UpdateRequired() const {
  return update_required;
}

void CacheManager::SaveUpdateRequired(bool status) {
  update_required = status;
  Backup();
}

bool CacheManager::IsApplicationRevoked(const std::string& app_id) const {
  CACHE_MANAGER_CHECK(false);
  bool is_revoked = false;
  if (pt_->policy_table.app_policies.end() !=
      pt_->policy_table.app_policies.find(app_id)) {
    is_revoked = pt_->policy_table.app_policies[app_id].is_null();
  }

  return is_revoked;
}

void CacheManager::CheckPermissions(const PTString &app_id,
                                    const PTString &hmi_level,
                                    const PTString &rpc,
                                    CheckPermissionResult &result) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();

  if (pt_->policy_table.app_policies.end() ==
      pt_->policy_table.app_policies.find(app_id)) {
    LOG4CXX_ERROR(logger_, "Application id " << app_id
                  << " was not found in policy DB.");
    return;
  }

  policy_table::Strings::const_iterator app_groups_iter =
      pt_->policy_table.app_policies[app_id].groups.begin();

  policy_table::Strings::const_iterator app_groups_iter_end =
      pt_->policy_table.app_policies[app_id].groups.end();

  policy_table::FunctionalGroupings::const_iterator concrete_group;

  for (; app_groups_iter != app_groups_iter_end; ++app_groups_iter) {
    concrete_group = pt_->policy_table.functional_groupings.find(*app_groups_iter);
    if (pt_->policy_table.functional_groupings.end() != concrete_group) {
      const policy_table::Rpcs& rpcs = concrete_group->second;

      policy_table::Rpc::const_iterator rpc_iter = rpcs.rpcs.find(rpc);
      if (rpcs.rpcs.end() != rpc_iter) {
        policy_table::RpcParameters rpc_param = rpc_iter->second;

        policy_table::HmiLevel hmi_level_e;
        policy_table::EnumFromJsonString(hmi_level, &hmi_level_e);

        policy_table::HmiLevels::const_iterator hmi_iter =
            std::find(rpc_param.hmi_levels.begin(), rpc_param.hmi_levels.end(),
                  hmi_level_e);

        if (rpc_param.hmi_levels.end() != hmi_iter) {
          result.hmi_level_permitted = PermitResult::kRpcAllowed;

          policy_table::Parameters::const_iterator params_iter =
              rpc_param.parameters->begin();
          policy_table::Parameters::const_iterator params_iter_end =
              rpc_param.parameters->end();

          for (;params_iter != params_iter_end; ++params_iter) {
            result.list_of_allowed_params.push_back(
                  policy_table::EnumToJsonString(*params_iter));
          }
        }
      }
    }
  }
}

bool CacheManager::IsPTPreloaded() {
  CACHE_MANAGER_CHECK(false);
  return *pt_->policy_table.module_config.preloaded_pt;
}

int CacheManager::IgnitionCyclesBeforeExchange() {
  CACHE_MANAGER_CHECK(0);
  const uint8_t limit = std::max(
        static_cast<int>(
          pt_->policy_table.module_config.exchange_after_x_ignition_cycles), 0);
  LOG4CXX_DEBUG(logger_, "IgnitionCyclesBeforeExchange limit:" << limit);
  uint8_t current = 0;


  return std::max(limit - current, 0);
}

int CacheManager::KilometersBeforeExchange(int current) {
  CACHE_MANAGER_CHECK(0);
  const int limit = std::max(
        static_cast<int>(
          pt_->policy_table.module_config.exchange_after_x_kilometers), 0);
  LOG4CXX_DEBUG(logger_, "KilometersBeforeExchange limit:" << limit);
  int last = 0;


  const int actual = std::max((current - last), 0);
  LOG4CXX_DEBUG(logger_, "KilometersBeforeExchange actual:" << actual);
  return std::max(limit - actual, 0);
}

bool CacheManager::SetCountersPassedForSuccessfulUpdate(int kilometers,
                                                        int days_after_epoch) {
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

int CacheManager::DaysBeforeExchange(int current) {
  CACHE_MANAGER_CHECK(0);
  const uint8_t limit = std::max(
        static_cast<int>(
          pt_->policy_table.module_config.exchange_after_x_days), 0);
  LOG4CXX_DEBUG(logger_, "DaysBeforeExchange limit:" << limit);
  uint8_t last = 0;


  const uint8_t actaul = std::max((current - last), 0);
  LOG4CXX_DEBUG(logger_, "DaysBeforeExchange actual:" << actaul);
  return std::max(limit - actaul, 0);
}

void CacheManager::IncrementIgnitionCycles() {
  CACHE_MANAGER_CHECK_VOID();
  Backup();
}

void CacheManager::ResetIgnitionCycles() {
  CACHE_MANAGER_CHECK_VOID();
  Backup();
}

int CacheManager::TimeoutResponse() {
  CACHE_MANAGER_CHECK(0);
  return pt_->policy_table.module_config.timeout_after_x_seconds;
}

bool CacheManager::SecondsBetweenRetries(std::vector<int>& seconds) {
  CACHE_MANAGER_CHECK(false);
  rpc::policy_table_interface_base::SecondsBetweenRetries::iterator iter =
    pt_->policy_table.module_config.seconds_between_retries.begin();
  rpc::policy_table_interface_base::SecondsBetweenRetries::iterator iter_end =
    pt_->policy_table.module_config.seconds_between_retries.end();

  const std::size_t size =
      pt_->policy_table.module_config.seconds_between_retries.size();
  seconds.reserve(size);
  for (;iter != iter_end; ++iter) {
    seconds.push_back(*iter);
  }
  return true;
}

VehicleData CacheManager::GetVehicleData() {
  // TODO(AGaliuzov): maybe should be removed.
  return VehicleData();
}

std::vector<UserFriendlyMessage> CacheManager::GetUserFriendlyMsg(
    const std::vector<std::string> &msg_codes, const std::string &language) {

  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<UserFriendlyMessage> result;
  CACHE_MANAGER_CHECK(result);

  const std::string fallback_language = "en-us";
  std::vector<std::string>::const_iterator it = msg_codes.begin();
  std::vector<std::string>::const_iterator it_end = msg_codes.end();
  for (; it != it_end; ++it) {

    UserFriendlyMessage msg;
    msg.message_code = *it;
    result.push_back(msg);
  }
  return result;
}

void CacheManager::GetUpdateUrls(int service_type,
                                 EndpointUrls& end_points) {
  LOG4CXX_AUTO_TRACE(logger_);

  CACHE_MANAGER_CHECK_VOID();
  char buff[32];
  sprintf(buff, "%x", service_type);

  std::string serv_type(buff);
  // TODO: remove this workaround
  if (service_type <= 0x9) {
    serv_type.insert(0,"0x0", 3);
  } else {
    serv_type.insert(0,"0x", 2);
  }

  policy_table::ServiceEndpoints::const_iterator iter =
      pt_->policy_table.module_config.endpoints.find(serv_type);

  if (pt_->policy_table.module_config.endpoints.end() != iter) {
    policy_table::URLList::const_iterator url_list_iter = (*iter).second.begin();
    policy_table::URLList::const_iterator url_list_iter_end = (*iter).second.end();
    for (; url_list_iter != url_list_iter_end; ++url_list_iter) {
      EndpointData data;
      data.app_id = (*url_list_iter).first;
      std::copy((*url_list_iter).second.begin(), (*url_list_iter).second.end(),
              std::back_inserter(data.url));

      end_points.push_back(data);
    }
  }
}

int CacheManager::GetNotificationsNumber(const std::string &priority) {
  CACHE_MANAGER_CHECK(0);
  typedef rpc::policy_table_interface_base::NumberOfNotificationsPerMinute NNPM;

  const NNPM& nnpm =
      pt_->policy_table.module_config.notifications_per_minute_by_priority;

  NNPM::const_iterator priority_iter = nnpm.find(priority);

  const int result = (nnpm.end() != priority_iter ? (*priority_iter).second : 0);
  return result;
}

bool CacheManager::GetPriority(const std::string &policy_app_id,
                               std::string &priority) {
  CACHE_MANAGER_CHECK(false);
  const policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies;

  policy_table::ApplicationPolicies::const_iterator policy_iter = policies.find(policy_app_id);
  const bool app_id_exists = policies.end() != policy_iter;
  if (app_id_exists) {
    priority = EnumToJsonString((*policy_iter).second.priority);
  }

  return app_id_exists;
}

void CacheManager::CheckSnapshotInitialization() {
  CACHE_MANAGER_CHECK_VOID();
  if (!snapshot_) {
    LOG4CXX_ERROR(logger_, "Snapshot pointer is not initialized");
    return;
  }

  *(snapshot_->policy_table.module_config.preloaded_pt) = false;
}

void CacheManager::PersistData() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (backup_.valid()) {
    if (pt_.valid()) {

      cache_lock_.Acquire();
      policy_table::Table copy_pt(*pt_);
      cache_lock_.Release();

      backup_->Save(copy_pt);
      backup_->SaveUpdateRequired(update_required);

      policy_table::ApplicationPolicies::const_iterator app_policy_iter =
          copy_pt.policy_table.app_policies.begin();
      policy_table::ApplicationPolicies::const_iterator app_policy_iter_end =
          copy_pt.policy_table.app_policies.end();

      bool is_revoked = false;
      bool is_default_policy;
      bool is_predata_policy;

      for (; app_policy_iter != app_policy_iter_end; ++app_policy_iter) {

        const std::string app_id = (*app_policy_iter).first;

        if (copy_pt.policy_table.app_policies.end() !=
            copy_pt.policy_table.app_policies.find(app_id)) {
          is_revoked = copy_pt.policy_table.app_policies[app_id].is_null();
        }

        is_default_policy = copy_pt.policy_table.app_policies.end() !=
            copy_pt.policy_table.app_policies.find(app_id) &&
            policy::kDefaultId ==
            copy_pt.policy_table.app_policies[app_id].get_string();

        // TODO(AOleynik): Remove this field from DB
        is_predata_policy = copy_pt.policy_table.app_policies.end() !=
            copy_pt.policy_table.app_policies.find(app_id) &&
            policy::kPreDataConsentId ==
            copy_pt.policy_table.app_policies[app_id].get_string();

        backup_->SaveApplicationCustomData(app_id,
                                           is_revoked,
                                           is_default_policy,
                                           is_predata_policy);
        is_revoked = false;
      }

  // In case of extended policy the meta info should be backuped as well.
      backup_->WriteDb();
    }
  }
}

void CacheManager::ResetCalculatedPermissions() {
  LOG4CXX_TRACE(logger_, "ResetCalculatedPermissions");
  sync_primitives::AutoLock lock(calculated_permissions_lock_);
  calculated_permissions_.clear();
}

void CacheManager::AddCalculatedPermissions(
    const std::string& device_id,
    const std::string& policy_app_id,
    const Permissions& permissions) {
  LOG4CXX_DEBUG(logger_, "AddCalculatedPermissions for device: " << device_id
               << " and app: " << policy_app_id);
  sync_primitives::AutoLock lock(calculated_permissions_lock_);
  calculated_permissions_[device_id][policy_app_id] = permissions;
}

bool CacheManager::IsPermissionsCalculated(
    const std::string& device_id,
    const std::string& policy_app_id,
    Permissions& permission) {
  LOG4CXX_DEBUG(logger_, "IsPermissionsCalculated for device: " << device_id
               << " and app: " << policy_app_id);
  sync_primitives::AutoLock lock(calculated_permissions_lock_);
  CalculatedPermissions::const_iterator it =
      calculated_permissions_.find(device_id);

  if (calculated_permissions_.end() == it) {
    return false;
  }

  AppCalculatedPermissions::const_iterator app_it =
      (*it).second.find(policy_app_id);
  if ((*it).second.end() == app_it) {
    return false;
  } else {
    permission = (*app_it).second;
    return true;
  }
  return false;
}

std::string CacheManager::RemoteAppsUrl() const {
  return "Not implemented";
}

utils::SharedPtr<policy_table::Table>
CacheManager::GenerateSnapshot() {
  CACHE_MANAGER_CHECK(snapshot_);
  snapshot_ = new policy_table::Table();
  snapshot_->policy_table = pt_->policy_table;
  CheckSnapshotInitialization();
  return snapshot_;
}

bool CacheManager::GetInitialAppData(const std::string& app_id,
                                     StringArray& nicknames,
                                     StringArray& app_hmi_types) {

  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::const_iterator policy_iter =
      pt_->policy_table.app_policies.find(app_id);

  if (pt_->policy_table.app_policies.end() != policy_iter) {
    const policy_table::ApplicationParams& app_params = (*policy_iter).second;

    std::copy(app_params.nicknames->begin(), app_params.nicknames->end(),
              std::back_inserter(nicknames));

    std::transform(app_params.AppHMIType->begin(), app_params.AppHMIType->end(),
                   std::back_inserter(app_hmi_types), AppHMITypeToString());
  }
  return true;
}

bool CacheManager::GetFunctionalGroupings(
    policy_table::FunctionalGroupings& groups) {

  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  const policy_table::FunctionalGroupings& f_groupings =
    pt_->policy_table.functional_groupings;

  groups.insert(f_groupings.begin(), f_groupings.end());
  return true;
}

int CacheManager::CountUnconsentedGroups(const std::string& policy_app_id,
                                          const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  LOG4CXX_DEBUG(logger_, "Application id: " << policy_app_id);
  int result = 0;
  return result;
}

bool CacheManager::SetMetaInfo(const std::string &ccpu_version,
                               const std::string &wers_country_code,
                               const std::string &language) {
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::IsMetaInfoPresent() const {
  CACHE_MANAGER_CHECK(false);
  bool result = true;
  return result;
}

bool CacheManager::SetSystemLanguage(const std::string &language) {
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::GetFunctionalGroupNames(FunctionalGroupNames &names) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  rpc::policy_table_interface_base::FunctionalGroupings::iterator iter =
      pt_->policy_table.functional_groupings.begin();
  rpc::policy_table_interface_base::FunctionalGroupings::iterator iter_end =
      pt_->policy_table.functional_groupings.end();

  for (; iter != iter_end; ++iter) {
    const int32_t id = GenerateHash((*iter).first);
    std::pair<std::string, std::string> value =
        std::make_pair( *(*iter).second.user_consent_prompt, (*iter).first);

    names.insert(std::pair<uint32_t, std::pair<std::string, std::string> >(id, value));
  }
  return true;
}

bool CacheManager::CleanupUnpairedDevices() {
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

void CacheManager::Increment(usage_statistics::GlobalCounterId type) {
  CACHE_MANAGER_CHECK_VOID();
  Backup();
}

void CacheManager::Increment(const std::string &app_id,
                             usage_statistics::AppCounterId type) {
  CACHE_MANAGER_CHECK_VOID();
  Backup();
}

void CacheManager::Set(const std::string &app_id,
                       usage_statistics::AppInfoId type,
                       const std::string &value) {
  CACHE_MANAGER_CHECK_VOID();
  Backup();
}

void CacheManager::Add(const std::string &app_id,
                       usage_statistics::AppStopwatchId type,
                       int seconds) {
  CACHE_MANAGER_CHECK_VOID();
  Backup();
}

long CacheManager::ConvertSecondsToMinute(int seconds) {
  const float seconds_in_minute = 60.0;
  return std::round(seconds / seconds_in_minute);
}

bool CacheManager::SetDefaultPolicy(const std::string &app_id) {
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies.find(kDefaultId);
  if (pt_->policy_table.app_policies.end() != iter) {
    pt_->policy_table.app_policies[app_id] =
        pt_->policy_table.app_policies[kDefaultId];

    SetIsDefault(app_id);
  }
  Backup();
  return true;
}

bool CacheManager::IsDefaultPolicy(const std::string& app_id) {
  CACHE_MANAGER_CHECK(false);
  const bool result =
      pt_->policy_table.app_policies.end() !=
      pt_->policy_table.app_policies.find(app_id) &&
      policy::kDefaultId ==
      pt_->policy_table.app_policies[app_id].get_string();

  return result;
}

bool CacheManager::SetIsDefault(const std::string& app_id) {
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies.find(app_id);
  if (pt_->policy_table.app_policies.end() != iter) {
    pt_->policy_table.app_policies[app_id].set_to_string(kDefaultId);
  }
  return true;
}

bool CacheManager::SetPredataPolicy(const std::string &app_id) {
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies.find(kPreDataConsentId);

  if (pt_->policy_table.app_policies.end() == iter) {
    LOG4CXX_ERROR(logger_, "Could not set " << kPreDataConsentId
                  << " permissions for app " << app_id);
    return false;
  }

  pt_->policy_table.app_policies[app_id] =
      pt_->policy_table.app_policies[kPreDataConsentId];

  pt_->policy_table.app_policies[app_id].set_to_string(kPreDataConsentId);

  Backup();
  return true;
}

bool CacheManager::IsPredataPolicy(const std::string &app_id) {
  // TODO(AOleynik): Maybe change for comparison with pre_DataConsent
  // permissions or check string value from get_string()
  policy_table::ApplicationParams& pre_data_app =
    pt_->policy_table.app_policies[kPreDataConsentId];
  policy_table::ApplicationParams& specific_app =
    pt_->policy_table.app_policies[app_id];

  policy_table::Strings res;
  std::set_intersection(pre_data_app.groups.begin(),
                        pre_data_app.groups.end(),
                        specific_app.groups.begin(),
                        specific_app.groups.end(),
                        std::back_inserter(res));

  return !res.empty();
}

bool CacheManager::SetUnpairedDevice(const std::string &device_id,
                bool unpaired) {
  const bool result =
      pt_->policy_table.device_data->end() !=
      pt_->policy_table.device_data->find(device_id);
  if (!result) {
    LOG4CXX_DEBUG(logger_, "Couldn't set unpaired flag for device id "
                  << device_id << " , since it wasn't found.");
    return false;
  }

  sync_primitives::AutoLock lock(unpaired_lock_);
  if (unpaired) {
    is_unpaired_.insert(device_id);
    LOG4CXX_DEBUG(logger_, "Unpaired flag was set for device id " << device_id);
  } else {
    is_unpaired_.erase(device_id);
    LOG4CXX_DEBUG(logger_, "Unpaired flag was removed for device id " << device_id);
  }
  return result;
}

bool CacheManager::SetVINValue(const std::string& value) {
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::IsApplicationRepresented(const std::string& app_id) const {
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies.find(app_id);
  return pt_->policy_table.app_policies.end() != iter;
}

bool CacheManager::Init(const std::string& file_name) {
  LOG4CXX_AUTO_TRACE(logger_);

  InitResult init_result = backup_->Init();

  bool result = true;
  switch (init_result) {
    case InitResult::EXISTS: {
      LOG4CXX_INFO(logger_, "Policy Table exists, was loaded correctly.");
      result = LoadFromBackup();
    } break;
    case InitResult::SUCCESS: {
      LOG4CXX_INFO(logger_, "Policy Table was inited successfully");
      result = LoadFromFile(file_name);
    } break;
    default: {
      result = false;
      LOG4CXX_ERROR(logger_, "Failed to init policy table.");
    } break;
  }

  return result;
}

void CacheManager::FillDeviceSpecificData() {
}

bool CacheManager::LoadFromBackup() {
  sync_primitives::AutoLock lock(cache_lock_);
  pt_ = backup_->GenerateSnapshot();
  update_required = backup_->UpdateRequired();

  FillDeviceSpecificData();

  return true;
}

bool CacheManager::LoadFromFile(const std::string& file_name) {
  LOG4CXX_AUTO_TRACE(logger_);
  BinaryMessage json_string;
  if (!file_system::ReadBinaryFile(file_name, json_string)) {
    LOG4CXX_FATAL(logger_, "Failed to read pt file.");
    return false;
  }

  Json::Value value;
  Json::Reader reader(Json::Features::strictMode());
  std::string json(json_string.begin(), json_string.end());
  if (!reader.parse(json.c_str(), value)) {
    LOG4CXX_FATAL(
        logger_,
        "Preloaded PT is corrupted: " << reader.getFormattedErrorMessages());
    return false;
  }

  LOG4CXX_TRACE(logger_, "Start create PT");
  sync_primitives::AutoLock locker(cache_lock_);
  backup_->Clear();

  pt_ = new policy_table::Table(&value);
  if (pt_->is_valid()) {
    if (backup_->Save(*pt_)) {
      backup_->WriteDb();
      return true;
    } else {
      LOG4CXX_FATAL(logger_, "Failed to save PT");
      return false;
    }
  } else {
    rpc::ValidationReport report("policy_table");
    pt_->ReportErrors(&report);
    LOG4CXX_FATAL(logger_,
                  "Parsed table is not valid " << rpc::PrettyFormat(report));
    return false;
  }
}

bool CacheManager::ResetPT(const std::string& file_name) {
  bool result = true;
  Backup();
  return result;
}

bool CacheManager::AppExists(const std::string &app_id) const {
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::iterator policy_iter =
      pt_->policy_table.app_policies.find(app_id);
  return pt_->policy_table.app_policies.end() != policy_iter;
}

int32_t CacheManager::GenerateHash(const std::string& str_to_hash) {

  uint32_t hash = 5381U;
  std::string::const_iterator it = str_to_hash.begin();
  std::string::const_iterator it_end = str_to_hash.end();

  for (;it != it_end; ++it) {
       hash = ((hash << 5) + hash) + (*it);
  }

  // Reset sign bit in case it has been set.
  // This is needed to avoid overflow for signed int.
  const int32_t result = hash & 0x7FFFFFFF;
  return result;
}

CacheManager::BackgroundBackuper::BackgroundBackuper(CacheManager* cache_manager)
  : cache_manager_(cache_manager),
    stop_flag_(false),
    new_data_available_(false) {
  LOG4CXX_AUTO_TRACE(logger_);
}

CacheManager::BackgroundBackuper::~BackgroundBackuper() {
  LOG4CXX_AUTO_TRACE(logger_);
}

void CacheManager::BackgroundBackuper::InternalBackup() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(cache_manager_);

  while (new_data_available_) {
    new_data_available_ = false;
    LOG4CXX_DEBUG(logger_, "DoBackup");
    cache_manager_->PersistData();
  }
}

void CacheManager::BackgroundBackuper::threadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(need_backup_lock_);
  while (!stop_flag_) {
    need_backup_lock_.Release();
    InternalBackup();
    need_backup_lock_.Acquire();
    if (new_data_available_ || stop_flag_) {
      continue;
    }
    LOG4CXX_DEBUG(logger_, "Wait for a next backup");
    backup_notifier_.Wait(need_backup_lock_);
  }
}

void CacheManager::BackgroundBackuper::exitThreadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(need_backup_lock_);
  stop_flag_ = true;
  backup_notifier_.NotifyOne();
}

void CacheManager::BackgroundBackuper::DoBackup() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(need_backup_lock_);
  new_data_available_ = true;
  backup_notifier_.NotifyOne();
}

}  // namespace policy

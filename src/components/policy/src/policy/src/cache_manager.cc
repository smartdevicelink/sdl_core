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

// EXTENDED_POLICY
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
// EXTENDED_POLICY
                     new SQLPTRepresentation()
    ),
    update_required(false) {

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
  return true;
}


bool CacheManager::GetUserPermissionsForDevice(const std::string &device_id,
                                               StringArray& consented_groups,
                                               StringArray& disallowed_groups) {

  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

void CacheManager::GetAllAppGroups(const std::string& app_id,
                                   FunctionalGroupIDs& all_group_ids) {

  LOG4CXX_TRACE_ENTER(logger_);
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
  LOG4CXX_TRACE_EXIT(logger_);
}

void CacheManager::GetPreConsentedGroups(const std::string &app_id,
                                         FunctionalGroupIDs& preconsented_groups) {

  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK_VOID();
  LOG4CXX_TRACE_EXIT(logger_);
}

void CacheManager::GetConsentedGroups(const std::string &device_id,
                                      const std::string &app_id,
                                      FunctionalGroupIDs& allowed_groups,
                                      FunctionalGroupIDs& disallowed_groups) {

  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK_VOID();
  LOG4CXX_TRACE_EXIT(logger_);
}

void CacheManager::GetUnconsentedGroups(const std::string& device_id,
                                        const std::string& policy_app_id,
                                        FunctionalGroupIDs& unconsented_groups) {
  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK_VOID();
  LOG4CXX_TRACE_EXIT(logger_);
}

void CacheManager::RemoveAppConsentForGroup(const std::string& app_id,
                                            const std::string& group_name) {
  CACHE_MANAGER_CHECK_VOID();
}

bool CacheManager::ApplyUpdate(const policy_table::Table& update_pt) {
  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  pt_->policy_table.functional_groupings =
      update_pt.policy_table.functional_groupings;

  policy_table::ApplicationPolicies::const_iterator iter =
      update_pt.policy_table.app_policies.begin();
  policy_table::ApplicationPolicies::const_iterator iter_end =
      update_pt.policy_table.app_policies.end();

  for (;iter != iter_end; ++iter) {
    pt_->policy_table.app_policies[iter->first] = iter->second;
  }

  pt_->policy_table.module_config = update_pt.policy_table.module_config;

  if (update_pt.policy_table.consumer_friendly_messages.is_initialized()) {
    pt_->policy_table.consumer_friendly_messages =
        update_pt.policy_table.consumer_friendly_messages;
  }

  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

void CacheManager::Backup() {
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  if (backup_.valid()) {
    if (pt_.valid()) {
      backup_->Save(*pt_);
      backup_->SaveUpdateRequired(update_required);


      policy_table::ApplicationPolicies::const_iterator app_policy_iter =
          pt_->policy_table.app_policies.begin();
      policy_table::ApplicationPolicies::const_iterator app_policy_iter_end =
          pt_->policy_table.app_policies.end();

      for (; app_policy_iter != app_policy_iter_end; ++app_policy_iter) {

        const std::string app_id = (*app_policy_iter).first;
        backup_->SaveApplicationCustomData(app_id,
                                          IsApplicationRevoked(app_id),
                                          IsDefaultPolicy(app_id),
                                          is_predata_[app_id]);
      }

  // In case of extended policy the meta info should be backuped as well.
    }
  }
}

std::string CacheManager::currentDateTime() {
  time_t     now = time(0);
  struct tm  tstruct;
  char       buf[80];
  tstruct = *localtime(&now);
  strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
  return buf;
}

bool CacheManager::GetPermissionsForApp(const std::string &device_id,
                                        const std::string &app_id,
                                        FunctionalIdType& group_types) {

  LOG4CXX_TRACE_ENTER(logger_);
  GetAllAppGroups(app_id, group_types[kTypeGeneral]);
  GetAllAppGroups(kDefaultId, group_types[kTypeDefault]);
  GetAllAppGroups(kPreDataConsentId, group_types[kTypePreDataConsented]);
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool CacheManager::GetDeviceGroupsFromPolicies(
  policy_table::Strings& groups,
  policy_table::Strings& preconsented_groups) {

  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  LOG4CXX_TRACE_EXIT(logger_);
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

  sync_primitives::AutoLock auto_lock(cache_lock_);
  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool CacheManager::SetUserPermissionsForDevice(
    const std::string &device_id,
    const StringArray &consented_groups,
    const StringArray &disallowed_groups) {
  sync_primitives::AutoLock auto_lock(cache_lock_);
  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool CacheManager::ReactOnUserDevConsentForApp(const std::string &app_id,
                                               bool is_device_allowed) {
  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  bool result = true;
  LOG4CXX_TRACE_EXIT(logger_);
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
  sync_primitives::AutoLock auto_lock(cache_lock_);
  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool CacheManager::UpdateRequired() const {
  return update_required;
}

void CacheManager::SaveUpdateRequired(bool status) {

  update_required = status;
}

bool CacheManager::IsApplicationRevoked(const std::string& app_id) {
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
  LOG4CXX_TRACE_ENTER(logger_);
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
  LOG4CXX_TRACE_EXIT(logger_);
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

  uint8_t current = 0;


  return std::max(limit - current, 0);
}

int CacheManager::KilometersBeforeExchange(int current) {
  CACHE_MANAGER_CHECK(0);
  const uint8_t limit = std::max(
        static_cast<int>(
          pt_->policy_table.module_config.exchange_after_x_kilometers), 0);
  uint8_t last = 0;


  const uint8_t actual = std::max((current - last), 0);
  return std::max(limit - actual, 0);
}

bool CacheManager::SetCountersPassedForSuccessfulUpdate(int kilometers,
                                                        int days_after_epoch) {
  CACHE_MANAGER_CHECK(false);
  return true;
}

int CacheManager::DaysBeforeExchange(int current) {
  CACHE_MANAGER_CHECK(0);
  const uint8_t limit = std::max(
        static_cast<int>(
          pt_->policy_table.module_config.exchange_after_x_days), 0);
  uint8_t last = 0;


  const uint8_t actaul = std::max((current - last), 0);
  return std::max(limit - actaul, 0);
}

void CacheManager::IncrementIgnitionCycles() {
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::ResetIgnitionCycles() {
  CACHE_MANAGER_CHECK_VOID();
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

  LOG4CXX_TRACE_ENTER(logger_);
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
  LOG4CXX_TRACE_EXIT(logger_);
  return result;
}

EndpointUrls CacheManager::GetUpdateUrls(int service_type) {
  LOG4CXX_TRACE_ENTER(logger_);

  EndpointUrls result;
  CACHE_MANAGER_CHECK(result);
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

      result.push_back(data);
    }
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return result;
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

utils::SharedPtr<policy_table::Table>
CacheManager::GenerateSnapshot() {
  CACHE_MANAGER_CHECK(snapshot_);
  snapshot_ = utils::SharedPtr<policy_table::Table>(new policy_table::Table(pt_->policy_table));
  CheckSnapshotInitialization();
  return snapshot_;
}

bool CacheManager::GetInitialAppData(const std::string& app_id,
                                     StringArray& nicknames,
                                     StringArray& app_hmi_types) {

  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::const_iterator policy_iter =
      pt_->policy_table.app_policies.find(app_id);

  if (pt_->policy_table.app_policies.end() != policy_iter) {
    const policy_table::ApplicationParams& app_params = (*policy_iter).second;

    std::copy(app_params.nicknames->begin(), app_params.nicknames->end(),
              std::back_inserter(nicknames));

    std::transform(app_params.AppHMIType->begin(), app_params.AppHMIType->end(),
                   std::back_inserter(nicknames), AppHMITypeToString());
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool CacheManager::GetFunctionalGroupings(
    policy_table::FunctionalGroupings& groups) {

  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  const policy_table::FunctionalGroupings& f_groupings =
    pt_->policy_table.functional_groupings;

  groups.insert(f_groupings.begin(), f_groupings.end());
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool CacheManager::CountUnconsentedGroups(const std::string& policy_app_id,
                                          const std::string& device_id,
                                          int& result) {
  LOG4CXX_TRACE_ENTER(logger_);
  CACHE_MANAGER_CHECK(false);
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool CacheManager::SetMetaInfo(const std::string &ccpu_version,
                               const std::string &wers_country_code,
                               const std::string &language) {
  CACHE_MANAGER_CHECK(false);
  return true;
}

bool CacheManager::IsMetaInfoPresent() const {
  CACHE_MANAGER_CHECK(false);
  bool result = true;
  return result;
}

bool CacheManager::SetSystemLanguage(const std::string &language) {
  CACHE_MANAGER_CHECK(false);
  return true;
}

bool CacheManager::GetFunctionalGroupNames(FunctionalGroupNames &names) {
  LOG4CXX_TRACE_ENTER(logger_);
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
  LOG4CXX_TRACE_EXIT(logger_);
  return true;
}

bool CacheManager::CleanupUnpairedDevices(const DeviceIds &device_ids) {
  CACHE_MANAGER_CHECK(false);
  return true;
}

void CacheManager::Increment(usage_statistics::GlobalCounterId type) {
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::Increment(const std::string &app_id,
                             usage_statistics::AppCounterId type) {
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::Set(const std::string &app_id,
                       usage_statistics::AppInfoId type,
                       const std::string &value) {
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::Add(const std::string &app_id,
                       usage_statistics::AppStopwatchId type,
                       int seconds) {
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::CopyInternalParams(const std::string &from,
                                      const std::string& to) {
  is_predata_[to] = is_predata_[from];
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

    CopyInternalParams(kDefaultId, app_id);
    SetIsDefault(app_id, true);
    SetIsPredata(app_id, false);

    pt_->policy_table.app_policies[app_id].set_to_string(kDefaultId);
  }
  return true;
}

bool CacheManager::IsDefaultPolicy(const std::string& app_id) {
  CACHE_MANAGER_CHECK(false);
  const bool result =
      pt_->policy_table.app_policies.end() !=
      pt_->policy_table.app_policies.find(app_id) &&
      !pt_->policy_table.app_policies[app_id].get_string().empty();

  return result;
}

bool CacheManager::SetIsDefault(const std::string& app_id,
                                bool is_default) {
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies.find(app_id);
  if (pt_->policy_table.app_policies.end() != iter) {
    if (is_default) {
      pt_->policy_table.app_policies[app_id].set_to_string("default");
    } else {
      pt_->policy_table.app_policies[app_id].set_to_string("");
    }
  }
  return true;
}

bool CacheManager::SetPredataPolicy(const std::string &app_id) {
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies.find(kPreDataConsentId);

  if (pt_->policy_table.app_policies.end() != iter) {
    pt_->policy_table.app_policies[app_id] =
        pt_->policy_table.app_policies[kPreDataConsentId];

    CopyInternalParams(kPreDataConsentId, app_id);

    SetIsPredata(app_id, true);
    SetIsDefault(app_id, false);
  }
  return true;
}

bool CacheManager::SetIsPredata(const std::string &app_id, bool is_pre_data) {
  is_predata_[app_id] = is_pre_data;
  return true;
}

bool CacheManager::IsPredataPolicy(const std::string &app_id) {
  const bool result =
      (is_predata_.end() != is_predata_.find(app_id)) && is_predata_[app_id];
  return result;
}

bool CacheManager::SetUnpairedDevice(const std::string &device_id) {

  const bool result = is_unpaired_.end() != is_unpaired_.find(device_id);
  if (result) {
    is_unpaired_[device_id] = true;
  }
  return result;
}

bool CacheManager::UnpairedDevicesList(DeviceIds& device_ids) {

  std::map<std::string, bool>::const_iterator iter = is_unpaired_.begin();
  std::map<std::string, bool>::const_iterator iter_end = is_unpaired_.end();

  for (; iter != iter_end; ++iter) {
    if ((*iter).second) {
      device_ids.push_back((*iter).first);
    }
  }
  return true;
}

bool CacheManager::SetVINValue(const std::string& value) {
  CACHE_MANAGER_CHECK(false);
  return true;
}

bool CacheManager::IsApplicationRepresented(const std::string& app_id) const {
  CACHE_MANAGER_CHECK(false);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies.find(app_id);
  return pt_->policy_table.app_policies.end() != iter;
}

bool CacheManager::Init(const std::string& file_name) {
  LOG4CXX_INFO(logger_, "CacheManager::Init");

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

void CacheManager::FillAppSpecificData() {
  CACHE_MANAGER_CHECK_VOID();
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies.begin();
  policy_table::ApplicationPolicies::const_iterator iter_end =
      pt_->policy_table.app_policies.begin();

  for (; iter != iter_end; ++iter) {
    const std::string& app_name = (*iter).first;

    is_predata_.insert(std::make_pair(app_name, backup_->IsPredataPolicy(app_name)));
  }
}

void CacheManager::FillDeviceSpecificData() {
}

bool CacheManager::LoadFromBackup() {
  pt_ = backup_->GenerateSnapshot();
  update_required = backup_->UpdateRequired();

  FillAppSpecificData();
  FillDeviceSpecificData();

  return true;
}

bool CacheManager::LoadFromFile(const std::string& file_name) {

  LOG4CXX_INFO(logger_, "CacheManager::LoadFromFile");
  BinaryMessage json_string;
  bool final_result = false;
  final_result = file_system::ReadBinaryFile(file_name, json_string);
  if (!final_result) {
    LOG4CXX_WARN(logger_, "Failed to read pt file.");
    return final_result;
  }

  Json::Value value;
  Json::Reader reader(Json::Features::strictMode());
  std::string json(json_string.begin(), json_string.end());
  bool ok = reader.parse(json.c_str(), value);
  if (ok) {
    pt_ = new policy_table::Table(&value);
  } else {
    LOG4CXX_WARN(logger_, reader.getFormattedErrorMessages());
  }

  if (!pt_) {
    LOG4CXX_WARN(logger_, "Failed to parse policy table");
    return false;
  }

  if (!pt_->is_valid()) {
    rpc::ValidationReport report("policy_table");
    pt_->ReportErrors(&report);
    LOG4CXX_WARN(logger_, "Parsed table is not valid " <<
                 rpc::PrettyFormat(report));
  }

  final_result = backup_->Save(*pt_);
  LOG4CXX_INFO(
    logger_,
    "Loading from file was " << (final_result ? "successful" : "unsuccessful"));
  return final_result;
}

bool CacheManager::ResetPT(const std::string& file_name) {
  bool result = true;
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

}

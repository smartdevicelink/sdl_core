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
#include <sstream>

#include "utils/file_system.h"
#include "utils/helpers.h"
#include "json/reader.h"
#include "json/features.h"
#include "json/writer.h"
#include "utils/logger.h"
#include "utils/date_time.h"
#include "utils/gen_hash.h"
#include "utils/macro.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

#include "policy/sql_pt_representation.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "Policy")

#define CACHE_MANAGER_CHECK(return_value)                            \
  {                                                                  \
    if (!pt_) {                                                      \
      LOG4CXX_WARN(logger_, "The cache manager is not initialized"); \
      return return_value;                                           \
    }                                                                \
  }

#define CACHE_MANAGER_CHECK_VOID()                                   \
  {                                                                  \
    if (!pt_) {                                                      \
      LOG4CXX_WARN(logger_, "The cache manager is not initialized"); \
      return;                                                        \
    }                                                                \
  }

struct LanguageFinder {
  LanguageFinder(const std::string& language) : language_(language) {}
  bool operator()(const policy_table::Languages::value_type& lang) const {
    return !strcasecmp(language_.c_str(), lang.first.c_str());
  }

 private:
  const std::string& language_;
};

struct PolicyTableUpdater {
  PolicyTableUpdater(const policy_table::ApplicationParams& default_params)
      : default_params_(default_params) {}

  void operator()(policy_table::ApplicationPolicies::value_type& pt_value) {
    if (policy::kDefaultId == pt_value.second.get_string()) {
      pt_value.second = default_params_;
      pt_value.second.set_to_string(policy::kDefaultId);
    }
  }

 private:
  const policy_table::ApplicationParams& default_params_;
};

CacheManager::CacheManager()
    : CacheManagerInterface()
    , pt_(new policy_table::Table)
    , backup_(new SQLPTRepresentation())
    , update_required(false)
    , settings_(nullptr) {
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

const policy_table::Strings& CacheManager::GetGroups(const PTString& app_id) {
  sync_primitives::AutoLock auto_lock(cache_lock_);
  return pt_->policy_table.app_policies_section.apps[app_id].groups;
}

bool CacheManager::CanAppKeepContext(const std::string& app_id) const {
  CACHE_MANAGER_CHECK(false);
  bool result = true;
  return result;
}

uint32_t CacheManager::HeartBeatTimeout(const std::string& app_id) const {
  CACHE_MANAGER_CHECK(0);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  uint32_t result = 0;
  if (!AppExists(app_id)) {
    return result;
  }

  const policy_table::ApplicationPolicies::mapped_type& app =
      pt_->policy_table.app_policies_section.apps[app_id];
  if (app.heart_beat_timeout_ms.is_initialized()) {
    result = *(app.heart_beat_timeout_ms);
  }

  return result;
}

const policy_table::AppHMITypes* CacheManager::GetHMITypes(
    const std::string& app_id) {
  sync_primitives::AutoLock auto_lock(cache_lock_);
  const policy_table::ApplicationPolicies& apps =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::const_iterator i = apps.find(app_id);
  if (i != apps.end()) {
    const policy_table::AppHMITypes& app_hmi_types = *i->second.AppHMIType;
    if (app_hmi_types.is_initialized()) {
      return &app_hmi_types;
    }
  }
  return NULL;
}

bool CacheManager::CanAppStealFocus(const std::string& app_id) const {
  CACHE_MANAGER_CHECK(false);
  bool result = true;
  return result;
}

bool CacheManager::GetDefaultHMI(const std::string& app_id,
                                 std::string& default_hmi) const {
  CACHE_MANAGER_CHECK(false);
  default_hmi = "NONE";
  return true;
}

bool CacheManager::ResetUserConsent() {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock lock(cache_lock_);
  return true;
}

bool CacheManager::GetUserPermissionsForDevice(
    const std::string& device_id,
    StringArray& consented_groups,
    StringArray& disallowed_groups) const {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  return true;
}

void CacheManager::GetAllAppGroups(const std::string& app_id,
                                   FunctionalGroupIDs& all_group_ids) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();

  if (kDeviceId == app_id) {
    LOG4CXX_INFO(logger_, "Devices doesn't have groups");
    return;
  }

  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::const_iterator app_params_iter =
      pt_->policy_table.app_policies_section.apps.find(app_id);

  if (pt_->policy_table.app_policies_section.apps.end() != app_params_iter) {
    policy_table::Strings::const_iterator iter =
        (*app_params_iter).second.groups.begin();
    policy_table::Strings::const_iterator iter_end =
        (*app_params_iter).second.groups.end();

    for (; iter != iter_end; ++iter) {
      const uint32_t group_id = static_cast<uint32_t>((GenerateHash(*iter)));
      all_group_ids.push_back(group_id);
    }
  }
}

void CacheManager::GetPreConsentedGroups(
    const std::string& app_id, FunctionalGroupIDs& preconsented_groups) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::GetConsentedGroups(const std::string& device_id,
                                      const std::string& app_id,
                                      FunctionalGroupIDs& allowed_groups,
                                      FunctionalGroupIDs& disallowed_groups) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::GetUnconsentedGroups(
    const std::string& device_id,
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
      update_pt.policy_table.app_policies_section.apps.begin();
  policy_table::ApplicationPolicies::const_iterator iter_end =
      update_pt.policy_table.app_policies_section.apps.end();

  for (; iter != iter_end; ++iter) {
    if (iter->second.is_null()) {
      pt_->policy_table.app_policies_section.apps[iter->first] =
          policy_table::ApplicationParams();
      pt_->policy_table.app_policies_section.apps[iter->first].set_to_null();
      pt_->policy_table.app_policies_section.apps[iter->first].set_to_string(
          "");
    } else {
      pt_->policy_table.app_policies_section.apps[iter->first] = iter->second;
      if (kDefaultId == iter->first) {
        std::for_each(pt_->policy_table.app_policies_section.apps.begin(),
                      pt_->policy_table.app_policies_section.apps.end(),
                      PolicyTableUpdater(iter->second));
      }
    }
  }

  pt_->policy_table.app_policies_section.device =
      update_pt.policy_table.app_policies_section.device;

  pt_->policy_table.module_config.SafeCopyFrom(
      update_pt.policy_table.module_config);

  pt_->policy_table.consumer_friendly_messages.assign_if_valid(
      update_pt.policy_table.consumer_friendly_messages);

  ResetCalculatedPermissions();
  Backup();
  return true;
}

void CacheManager::GetHMIAppTypeAfterUpdate(
    std::map<std::string, StringArray>& app_hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::const_iterator policy_iter_begin =
      pt_->policy_table.app_policies_section.apps.begin();
  policy_table::ApplicationPolicies::const_iterator policy_iter_end =
      pt_->policy_table.app_policies_section.apps.end();
  std::vector<std::string> transform_app_hmi_types;
  for (; policy_iter_begin != policy_iter_end; ++policy_iter_begin) {
    const policy_table::ApplicationParams& app_params =
        (*policy_iter_begin).second;
    if (app_params.AppHMIType.is_initialized()) {
      if (!(transform_app_hmi_types.empty())) {
        transform_app_hmi_types.clear();
      }
      std::transform(app_params.AppHMIType->begin(),
                     app_params.AppHMIType->end(),
                     std::back_inserter(transform_app_hmi_types),
                     AppHMITypeToString());
      app_hmi_types[(*policy_iter_begin).first] = transform_app_hmi_types;
    }
  }
}

bool CacheManager::AppHasHMIType(const std::string& application_id,
                                 policy_table::AppHMIType hmi_type) const {
  sync_primitives::AutoLock auto_lock(cache_lock_);
  const policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;

  policy_table::ApplicationPolicies::const_iterator policy_iter =
      policies.find(application_id);

  if (policy_iter == policies.end()) {
    return false;
  }

  if (policy_iter->second.AppHMIType.is_initialized()) {
    return helpers::in_range(*(policy_iter->second.AppHMIType),
                             rpc::Enum<policy_table::AppHMIType>(hmi_type));
  }

  return false;
}

void CacheManager::Backup() {
  sync_primitives::AutoLock lock(backuper_locker_);
  DCHECK(backuper_);
  backuper_->DoBackup();
}

std::string CacheManager::currentDateTime() {
  time_t now = time(0);
  struct tm tstruct;
  char buf[80];
  tstruct = *localtime(&now);
  // ISO_8601 format is expected, e.g. “2000-01-01T12:18:53Z”
  strftime(buf, sizeof(buf), "%Y-%m-%dT%XZ", &tstruct);
  return buf;
}

bool CacheManager::GetPermissionsForApp(const std::string& device_id,
                                        const std::string& app_id,
                                        FunctionalIdType& group_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  GetAllAppGroups(app_id, group_types[kTypeGeneral]);
  GetAllAppGroups(kDefaultId, group_types[kTypeDefault]);
  GetAllAppGroups(kPreDataConsentId, group_types[kTypePreDataConsented]);
  return true;
}

bool CacheManager::GetDeviceGroupsFromPolicies(
    policy_table::Strings& groups,
    policy_table::Strings& preconsented_groups) const {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  return true;
}

bool CacheManager::AddDevice(const std::string& device_id,
                             const std::string& connection_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  sync_primitives::AutoLock auto_lock(cache_lock_);
  CACHE_MANAGER_CHECK(false);
  policy_table::DeviceParams& params =
      (*(pt_->policy_table.device_data))[device_id];

  // Open SDL stored just device id in policy
  UNUSED(params);

  // We have to set preloaded flag as false in policy table on adding new
  // information (SDLAQ-CRS-2365). It can happens only after device addition.
  *pt_->policy_table.module_config.preloaded_pt = false;

  Backup();
  return true;
}

bool CacheManager::SetDeviceData(const std::string& device_id,
                                 const std::string& hardware,
                                 const std::string& firmware,
                                 const std::string& os,
                                 const std::string& os_version,
                                 const std::string& carrier,
                                 const uint32_t number_of_ports,
                                 const std::string& connection_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  sync_primitives::AutoLock auto_lock(cache_lock_);
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::SetUserPermissionsForDevice(
    const std::string& device_id,
    const StringArray& consented_groups,
    const StringArray& disallowed_groups) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::ReactOnUserDevConsentForApp(const std::string& app_id,
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
  sync_primitives::AutoLock auto_lock(cache_lock_);
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
    const PermissionConsent& permissions) {
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
  sync_primitives::AutoLock auto_lock(cache_lock_);
  bool is_revoked = false;
  if (pt_->policy_table.app_policies_section.apps.end() !=
      pt_->policy_table.app_policies_section.apps.find(app_id)) {
    is_revoked = pt_->policy_table.app_policies_section.apps[app_id].is_null();
  }

  return is_revoked;
}

void CacheManager::CheckPermissions(const policy_table::Strings& groups,
                                    const PTString& hmi_level,
                                    const PTString& rpc,
                                    CheckPermissionResult& result) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);

  policy_table::Strings::const_iterator app_groups_iter = groups.begin();
  policy_table::Strings::const_iterator app_groups_iter_end = groups.end();

  policy_table::FunctionalGroupings::const_iterator concrete_group;

  for (; app_groups_iter != app_groups_iter_end; ++app_groups_iter) {
    concrete_group =
        pt_->policy_table.functional_groupings.find(*app_groups_iter);
    if (pt_->policy_table.functional_groupings.end() != concrete_group) {
      const policy_table::Rpcs& rpcs = concrete_group->second;

      policy_table::Rpc::const_iterator rpc_iter = rpcs.rpcs.find(rpc);
      if (rpcs.rpcs.end() != rpc_iter) {
        policy_table::RpcParameters rpc_param = rpc_iter->second;

        if (rpc_param.parameters.is_initialized() &&
            rpc_param.parameters->empty()) {
          // If "parameters" field exist in PT section of incoming RPC but empty
          // all  params considered as DISALLOWED
          result.hmi_level_permitted = kRpcDisallowed;
          return;
        }
        policy_table::HmiLevel hmi_level_e;
        policy_table::EnumFromJsonString(hmi_level, &hmi_level_e);

        policy_table::HmiLevels::const_iterator hmi_iter =
            std::find(rpc_param.hmi_levels.begin(),
                      rpc_param.hmi_levels.end(),
                      hmi_level_e);

        if (rpc_param.hmi_levels.end() != hmi_iter) {
          result.hmi_level_permitted = PermitResult::kRpcAllowed;

          policy_table::Parameters::const_iterator params_iter =
              rpc_param.parameters->begin();
          policy_table::Parameters::const_iterator params_iter_end =
              rpc_param.parameters->end();

          for (; params_iter != params_iter_end; ++params_iter) {
            result.list_of_allowed_params.insert(
                policy_table::EnumToJsonString(*params_iter));
          }
        }
      }
    }
  }
}

bool CacheManager::IsPTPreloaded() {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  return *pt_->policy_table.module_config.preloaded_pt;
}

int CacheManager::IgnitionCyclesBeforeExchange() {
  CACHE_MANAGER_CHECK(0);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  const uint8_t limit = std::max(
      static_cast<int>(
          pt_->policy_table.module_config.exchange_after_x_ignition_cycles),
      0);
  LOG4CXX_DEBUG(logger_, "IgnitionCyclesBeforeExchange limit:" << limit);
  uint8_t current = 0;

  const int last_exch = static_cast<int>(
      *pt_->policy_table.module_meta->ignition_cycles_since_last_exchange);
  current = std::max(last_exch, 0);
  LOG4CXX_DEBUG(
      logger_,
      "IgnitionCyclesBeforeExchange current:" << static_cast<int>(current));

  return std::max(limit - current, 0);
}

int CacheManager::KilometersBeforeExchange(int current) {
  CACHE_MANAGER_CHECK(0);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  const int limit =
      std::max(static_cast<int>(
                   pt_->policy_table.module_config.exchange_after_x_kilometers),
               0);
  LOG4CXX_DEBUG(logger_, "KilometersBeforeExchange limit:" << limit);
  int last = 0;

  const int odo_val = static_cast<int>(
      *pt_->policy_table.module_meta->pt_exchanged_at_odometer_x);
  last = std::max(odo_val, 0);
  LOG4CXX_DEBUG(logger_, "KilometersBeforeExchange last:" << last);

  const int actual = std::max((current - last), 0);
  LOG4CXX_DEBUG(logger_, "KilometersBeforeExchange actual:" << actual);
  return std::max(limit - actual, 0);
}

bool CacheManager::SetCountersPassedForSuccessfulUpdate(
    policy::Counters counter, int value) {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  switch (counter) {
    case KILOMETERS:
      *pt_->policy_table.module_meta->pt_exchanged_at_odometer_x = value;
      LOG4CXX_DEBUG(logger_,
                    "SetCountersPassedForSuccessfulUpdate km:" << value);
      break;
    case DAYS_AFTER_EPOCH:
      *pt_->policy_table.module_meta->pt_exchanged_x_days_after_epoch = value;
      LOG4CXX_DEBUG(
          logger_,
          "SetCountersPassedForSuccessfulUpdate days after epoch:" << value);
      break;
    default:
      LOG4CXX_ERROR(logger_,
                    "Unknown counter was requested to set: " << counter);
      return false;
  }

  Backup();
  return true;
}

int CacheManager::DaysBeforeExchange(int current) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(0);
  sync_primitives::AutoLock auto_lock(cache_lock_);

  const rpc::Optional<rpc::Integer<uint16_t, 0, 65535> >& days_after_epoch =
      (pt_->policy_table.module_meta->pt_exchanged_x_days_after_epoch);

  if (!days_after_epoch->is_initialized()) {
    return -1;
  }

  const uint8_t limit = pt_->policy_table.module_config.exchange_after_x_days;
  LOG4CXX_DEBUG(logger_,
                "Exchange after: " << static_cast<int>(limit) << " days");

  LOG4CXX_DEBUG(logger_, "Epoch since last update: " << *days_after_epoch);

  const uint16_t actual =
      std::max(static_cast<uint16_t>(current - *days_after_epoch), uint16_t(0));
  LOG4CXX_DEBUG(logger_, "The days since last update: " << actual);

  return std::max(limit - actual, 0);
}

void CacheManager::IncrementIgnitionCycles() {
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  const int ign_val = static_cast<int>(
      *pt_->policy_table.module_meta->ignition_cycles_since_last_exchange);
  (*pt_->policy_table.module_meta->ignition_cycles_since_last_exchange) =
      ign_val + 1;
  LOG4CXX_DEBUG(logger_, "IncrementIgnitionCycles ignitions:" << ign_val);
  Backup();
}

void CacheManager::ResetIgnitionCycles() {
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  (*pt_->policy_table.module_meta->ignition_cycles_since_last_exchange) = 0;
  Backup();
}

int CacheManager::TimeoutResponse() {
  CACHE_MANAGER_CHECK(0);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  return pt_->policy_table.module_config.timeout_after_x_seconds *
         date_time::MILLISECONDS_IN_SECOND;
}

bool CacheManager::SecondsBetweenRetries(std::vector<int>& seconds) {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  rpc::policy_table_interface_base::SecondsBetweenRetries::iterator iter =
      pt_->policy_table.module_config.seconds_between_retries.begin();
  rpc::policy_table_interface_base::SecondsBetweenRetries::iterator iter_end =
      pt_->policy_table.module_config.seconds_between_retries.end();

  const std::size_t size =
      pt_->policy_table.module_config.seconds_between_retries.size();
  seconds.reserve(size);
  for (; iter != iter_end; ++iter) {
    seconds.push_back(*iter);
  }
  return true;
}

const policy::VehicleInfo CacheManager::GetVehicleInfo() const {
  CACHE_MANAGER_CHECK(VehicleInfo());
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ModuleConfig& module_config = pt_->policy_table.module_config;
  VehicleInfo vehicle_info;
  vehicle_info.vehicle_make = *module_config.vehicle_make;
  vehicle_info.vehicle_model = *module_config.vehicle_model;
  vehicle_info.vehicle_year = *module_config.vehicle_year;
  LOG4CXX_DEBUG(
      logger_,
      "Vehicle info (make, model, year):" << vehicle_info.vehicle_make << ","
                                          << vehicle_info.vehicle_model << ","
                                          << vehicle_info.vehicle_year);
  return vehicle_info;
}

const utils::OptionalVal<bool> CacheManager::LockScreenDismissalEnabledState()
    const {
  utils::OptionalVal<bool> empty(utils::OptionalVal<bool>::EMPTY);
  CACHE_MANAGER_CHECK(empty);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ModuleConfig& module_config = pt_->policy_table.module_config;
  if (module_config.lock_screen_dismissal_enabled.is_initialized()) {
    return utils::OptionalVal<bool>(
        *module_config.lock_screen_dismissal_enabled);
  }
  return empty;
}

std::vector<UserFriendlyMessage> CacheManager::GetUserFriendlyMsg(
    const std::vector<std::string>& msg_codes, const std::string& language) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<UserFriendlyMessage> result;
  CACHE_MANAGER_CHECK(result);

  std::vector<std::string>::const_iterator it = msg_codes.begin();
  std::vector<std::string>::const_iterator it_end = msg_codes.end();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  for (; it != it_end; ++it) {
    policy_table::MessageLanguages msg_languages =
        (*pt_->policy_table.consumer_friendly_messages->messages)[*it];

    policy_table::MessageString message_string;

    // If message has no records with required language, fallback language
    // should be used instead.
    LanguageFinder finder(language);
    policy_table::Languages::const_iterator it_language = std::find_if(
        msg_languages.languages.begin(), msg_languages.languages.end(), finder);

    if (msg_languages.languages.end() == it_language) {
      LOG4CXX_WARN(logger_,
                   "Language "
                       << language
                       << " haven't been found for message code: " << *it);

      LanguageFinder fallback_language_finder("en-us");

      policy_table::Languages::const_iterator it_fallback_language =
          std::find_if(msg_languages.languages.begin(),
                       msg_languages.languages.end(),
                       fallback_language_finder);

      if (msg_languages.languages.end() == it_fallback_language) {
        LOG4CXX_ERROR(logger_,
                      "No fallback language found for message code: " << *it);
        continue;
      }

      message_string = it_fallback_language->second;
    } else {
      message_string = it_language->second;
    }

    UserFriendlyMessage msg;
    msg.message_code = *it;
    result.push_back(msg);
  }
  return result;
}

void CacheManager::GetUpdateUrls(const uint32_t service_type,
                                 EndpointUrls& out_end_points) {
  std::stringstream service_type_stream;
  service_type_stream << (service_type <= 9 ? "0x0" : "0x") << service_type;

  const std::string service_type_str = service_type_stream.str();
  GetUpdateUrls(service_type_str, out_end_points);
}

void CacheManager::GetUpdateUrls(const std::string& service_type,
                                 EndpointUrls& out_end_points) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();

  LOG4CXX_DEBUG(logger_, "Search service value is: " << service_type);

  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ServiceEndpoints::const_iterator iter =
      pt_->policy_table.module_config.endpoints.find(service_type);

  if (pt_->policy_table.module_config.endpoints.end() != iter) {
    policy_table::URLList::const_iterator url_list_iter =
        (*iter).second.begin();
    policy_table::URLList::const_iterator url_list_iter_end =
        (*iter).second.end();
    for (; url_list_iter != url_list_iter_end; ++url_list_iter) {
      EndpointData data;
      data.app_id = (*url_list_iter).first;
      std::copy((*url_list_iter).second.begin(),
                (*url_list_iter).second.end(),
                std::back_inserter(data.url));

      out_end_points.push_back(data);
    }
  }
}

std::string CacheManager::GetLockScreenIconUrl() const {
  if (backup_) {
    return backup_->GetLockScreenIconUrl();
  }
  return std::string("");
}

rpc::policy_table_interface_base::NumberOfNotificationsType
CacheManager::GetNotificationsNumber(const std::string& priority) {
  CACHE_MANAGER_CHECK(0);
  typedef rpc::policy_table_interface_base::NumberOfNotificationsPerMinute NNPM;

  sync_primitives::AutoLock auto_lock(cache_lock_);
  const NNPM& nnpm =
      pt_->policy_table.module_config.notifications_per_minute_by_priority;

  NNPM::const_iterator priority_iter = nnpm.find(priority);

  const rpc::policy_table_interface_base::NumberOfNotificationsType result =
      (nnpm.end() != priority_iter ? (*priority_iter).second : 0u);
  return result;
}

bool CacheManager::GetPriority(const std::string& policy_app_id,
                               std::string& priority) const {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  if (kDeviceId == policy_app_id) {
    priority = EnumToJsonString(
        pt_->policy_table.app_policies_section.device.priority);
    return true;
  }

  const policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;

  policy_table::ApplicationPolicies::const_iterator policy_iter =
      policies.find(policy_app_id);
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

  *(snapshot_->policy_table.module_config.full_app_id_supported) =
      get_settings().use_full_app_id();

  // SDL must not send certificate in snapshot
  snapshot_->policy_table.module_config.certificate =
      rpc::Optional<rpc::String<0, 65535> >();

  rpc::Optional<policy_table::ModuleMeta>& module_meta =
      snapshot_->policy_table.module_meta;
  if (!module_meta->pt_exchanged_at_odometer_x->is_initialized()) {
    *(module_meta->pt_exchanged_at_odometer_x) = 0;
  }
  if (!module_meta->ignition_cycles_since_last_exchange->is_initialized()) {
    *(module_meta->ignition_cycles_since_last_exchange) = 0;
  }
  if (!module_meta->pt_exchanged_x_days_after_epoch->is_initialized()) {
    *(module_meta->pt_exchanged_x_days_after_epoch) = 0;
  }

  /* consumer_friendly_messages are required for the snapshot;
   * consumer_friendly_messages->version is required always, but
   * consumer_friendly_messages->messages must be omitted in PTS */
  if (snapshot_->policy_table.consumer_friendly_messages->is_initialized()) {
    snapshot_->policy_table.consumer_friendly_messages->messages =
        rpc::Optional<policy_table::Messages>();
  } else {
    LOG4CXX_WARN(logger_,
                 "policy_table.consumer_friendly_messages is not initialized");
  }

  /* policy_table.usage_and_error_counts are required for PTS and
   * policy_table.usage_and_error_counts->app_level is optional */
  rpc::Optional<policy_table::UsageAndErrorCounts>& usage_and_error_counts =
      snapshot_->policy_table.usage_and_error_counts;

  if (usage_and_error_counts->app_level->is_initialized()) {
    policy_table::AppLevels::iterator it =
        usage_and_error_counts->app_level->begin();
    policy_table::AppLevels::const_iterator it_end =
        usage_and_error_counts->app_level->end();
    for (; it != it_end; ++it) {
      if (!(*it).second.minutes_in_hmi_full.is_initialized()) {
        (*it).second.minutes_in_hmi_full = 0;
      }

      if (!(*it).second.app_registration_language_gui.is_initialized()) {
        (*it).second.app_registration_language_gui = "unknown";
      }

      if (!(*it).second.app_registration_language_vui.is_initialized()) {
        (*it).second.app_registration_language_vui = "unknown";
      }

      if (!(*it).second.minutes_in_hmi_limited.is_initialized()) {
        (*it).second.minutes_in_hmi_limited = 0;
      }

      if (!(*it).second.minutes_in_hmi_background.is_initialized()) {
        (*it).second.minutes_in_hmi_background = 0;
      }

      if (!(*it).second.minutes_in_hmi_none.is_initialized()) {
        (*it).second.minutes_in_hmi_none = 0;
      }

      if (!(*it).second.count_of_user_selections.is_initialized()) {
        (*it).second.count_of_user_selections = 0;
      }

      if (!(*it)
               .second.count_of_rejections_sync_out_of_memory
               .is_initialized()) {
        (*it).second.count_of_rejections_sync_out_of_memory = 0;
      }

      if (!(*it)
               .second.count_of_rejections_nickname_mismatch.is_initialized()) {
        (*it).second.count_of_rejections_nickname_mismatch = 0;
      }

      if (!(*it).second.count_of_rejections_duplicate_name.is_initialized()) {
        (*it).second.count_of_rejections_duplicate_name = 0;
      }

      if (!(*it).second.count_of_rejected_rpc_calls.is_initialized()) {
        (*it).second.count_of_rejected_rpc_calls = 0;
      }

      if (!(*it).second.count_of_rpcs_sent_in_hmi_none.is_initialized()) {
        (*it).second.count_of_rpcs_sent_in_hmi_none = 0;
      }

      if (!(*it).second.count_of_removals_for_bad_behavior.is_initialized()) {
        (*it).second.count_of_removals_for_bad_behavior = 0;
      }

      if (!(*it).second.count_of_tls_errors.is_initialized()) {
        (*it).second.count_of_tls_errors = 0;
      }

      if (!(*it).second.count_of_run_attempts_while_revoked.is_initialized()) {
        (*it).second.count_of_run_attempts_while_revoked = 0;
      }
    }
  } else {
    LOG4CXX_WARN(logger_, "app_level is not initialized");
  }
}

void CacheManager::PersistData() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (backup_.use_count() != 0) {
    if (pt_.use_count() != 0) {
      // Comma expression is used to hold the lock only during the constructor
      // call
      policy_table::Table copy_pt(
          (sync_primitives::AutoLock(cache_lock_), *pt_));

      backup_->Save(copy_pt);
      backup_->SaveUpdateRequired(update_required);

      policy_table::ApplicationPolicies::const_iterator app_policy_iter =
          copy_pt.policy_table.app_policies_section.apps.begin();
      policy_table::ApplicationPolicies::const_iterator app_policy_iter_end =
          copy_pt.policy_table.app_policies_section.apps.end();

      bool is_revoked = false;
      bool is_default_policy;
      bool is_predata_policy;

      for (; app_policy_iter != app_policy_iter_end; ++app_policy_iter) {
        const std::string app_id = (*app_policy_iter).first;

        if (copy_pt.policy_table.app_policies_section.apps.end() !=
            copy_pt.policy_table.app_policies_section.apps.find(app_id)) {
          is_revoked =
              copy_pt.policy_table.app_policies_section.apps[app_id].is_null();
        }

        is_default_policy =
            copy_pt.policy_table.app_policies_section.apps.end() !=
                copy_pt.policy_table.app_policies_section.apps.find(app_id) &&
            policy::kDefaultId ==
                copy_pt.policy_table.app_policies_section.apps[app_id]
                    .get_string();

        // TODO(AOleynik): Remove this field from DB
        is_predata_policy =
            copy_pt.policy_table.app_policies_section.apps.end() !=
                copy_pt.policy_table.app_policies_section.apps.find(app_id) &&
            policy::kPreDataConsentId ==
                copy_pt.policy_table.app_policies_section.apps[app_id]
                    .get_string();

        backup_->SaveApplicationCustomData(
            app_id, is_revoked, is_default_policy, is_predata_policy);
        is_revoked = false;
      }

      // In case of extended policy the meta info should be backuped as well.
      backup_->WriteDb();
    }
  }
}

void CacheManager::ResetCalculatedPermissions() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(calculated_permissions_lock_);
  calculated_permissions_.clear();
}

void CacheManager::AddCalculatedPermissions(const std::string& device_id,
                                            const std::string& policy_app_id,
                                            const Permissions& permissions) {
  LOG4CXX_DEBUG(logger_,
                "AddCalculatedPermissions for device: "
                    << device_id << " and app: " << policy_app_id);
  sync_primitives::AutoLock lock(calculated_permissions_lock_);
  calculated_permissions_[device_id][policy_app_id] = permissions;
}

bool CacheManager::IsPermissionsCalculated(const std::string& device_id,
                                           const std::string& policy_app_id,
                                           Permissions& permission) {
  LOG4CXX_DEBUG(logger_,
                "IsPermissionsCalculated for device: "
                    << device_id << " and app: " << policy_app_id);
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

std::shared_ptr<policy_table::Table> CacheManager::GenerateSnapshot() {
  CACHE_MANAGER_CHECK(snapshot_);

  snapshot_ = std::make_shared<policy_table::Table>();

  // Copy all members of policy table except messages in consumer friendly
  // messages
  sync_primitives::AutoLock auto_lock(cache_lock_);
  snapshot_->policy_table.app_policies_section =
      pt_->policy_table.app_policies_section;
  snapshot_->policy_table.functional_groupings =
      pt_->policy_table.functional_groupings;
  snapshot_->policy_table.consumer_friendly_messages->version =
      pt_->policy_table.consumer_friendly_messages->version;
  snapshot_->policy_table.consumer_friendly_messages->mark_initialized();
  snapshot_->policy_table.module_config = pt_->policy_table.module_config;
  snapshot_->policy_table.module_meta = pt_->policy_table.module_meta;
  snapshot_->policy_table.usage_and_error_counts =
      pt_->policy_table.usage_and_error_counts;
  snapshot_->policy_table.device_data = pt_->policy_table.device_data;

  // Set policy table type to Snapshot
  snapshot_->SetPolicyTableType(
      rpc::policy_table_interface_base::PolicyTableType::PT_SNAPSHOT);

  CheckSnapshotInitialization();
  return snapshot_;
}

bool CacheManager::GetInitialAppData(const std::string& app_id,
                                     StringArray& nicknames,
                                     StringArray& app_hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::const_iterator policy_iter =
      pt_->policy_table.app_policies_section.apps.find(app_id);

  if (pt_->policy_table.app_policies_section.apps.end() != policy_iter) {
    const policy_table::ApplicationParams& app_params = (*policy_iter).second;

    std::copy(app_params.nicknames->begin(),
              app_params.nicknames->end(),
              std::back_inserter(nicknames));

    std::transform(app_params.AppHMIType->begin(),
                   app_params.AppHMIType->end(),
                   std::back_inserter(app_hmi_types),
                   AppHMITypeToString());
  }
  return true;
}

bool CacheManager::GetFunctionalGroupings(
    policy_table::FunctionalGroupings& groups) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
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

bool CacheManager::SetMetaInfo(const std::string& ccpu_version,
                               const std::string& wers_country_code,
                               const std::string& language) {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);

  // We have to set preloaded flag as false in policy table on any response
  // of GetSystemInfo (SDLAQ-CRS-2365)
  *pt_->policy_table.module_config.preloaded_pt = false;

  Backup();
  return true;
}

bool CacheManager::IsMetaInfoPresent() const {
  CACHE_MANAGER_CHECK(false);
  bool result = true;
  return result;
}

bool CacheManager::SetSystemLanguage(const std::string& language) {
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::GetFunctionalGroupNames(FunctionalGroupNames& names) {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  rpc::policy_table_interface_base::FunctionalGroupings::iterator iter =
      pt_->policy_table.functional_groupings.begin();
  rpc::policy_table_interface_base::FunctionalGroupings::iterator iter_end =
      pt_->policy_table.functional_groupings.end();

  for (; iter != iter_end; ++iter) {
    const int32_t id = GenerateHash((*iter).first);
    std::pair<std::string, std::string> value =
        std::make_pair(*(*iter).second.user_consent_prompt, (*iter).first);

    names.insert(
        std::pair<uint32_t, std::pair<std::string, std::string> >(id, value));
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

void CacheManager::Increment(const std::string& app_id,
                             usage_statistics::AppCounterId type) {
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock lock(cache_lock_);
  switch (type) {
    case usage_statistics::USER_SELECTIONS:
      ++(*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
            .count_of_user_selections;
      break;
    case usage_statistics::REJECTIONS_SYNC_OUT_OF_MEMORY:
      ++(*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
            .count_of_rejections_sync_out_of_memory;
      break;
    case usage_statistics::REJECTIONS_NICKNAME_MISMATCH:
      ++(*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
            .count_of_rejections_nickname_mismatch;
      break;
    case usage_statistics::REJECTIONS_DUPLICATE_NAME:
      ++(*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
            .count_of_rejections_duplicate_name;
      break;
    case usage_statistics::REJECTED_RPC_CALLS:
      ++(*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
            .count_of_rejected_rpc_calls;
      break;
    case usage_statistics::RPCS_IN_HMI_NONE:
      ++(*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
            .count_of_rpcs_sent_in_hmi_none;
      break;
    case usage_statistics::REMOVALS_MISBEHAVED:
      ++(*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
            .count_of_removals_for_bad_behavior;
      break;
    case usage_statistics::RUN_ATTEMPTS_WHILE_REVOKED:
      ++(*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
            .count_of_run_attempts_while_revoked;
      break;
    case usage_statistics::COUNT_OF_TLS_ERRORS:
      ++(*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
            .count_of_tls_errors;
      break;
    default:
      LOG4CXX_WARN(logger_, "Type app counter is unknown");
      return;
  }
  Backup();
}

void CacheManager::Set(const std::string& app_id,
                       usage_statistics::AppInfoId type,
                       const std::string& value) {
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock lock(cache_lock_);
  switch (type) {
    case usage_statistics::LANGUAGE_GUI:
      (*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
          .app_registration_language_gui = value;
      break;
    case usage_statistics::LANGUAGE_VUI:
      (*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
          .app_registration_language_vui = value;
      break;
    default:
      LOG4CXX_WARN(logger_, "Type app info is unknown");
      return;
  }
  Backup();
}

void CacheManager::Add(const std::string& app_id,
                       usage_statistics::AppStopwatchId type,
                       int seconds) {
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock lock(cache_lock_);
  const int minutes = ConvertSecondsToMinute(seconds);
  switch (type) {
    case usage_statistics::SECONDS_HMI_FULL:
      (*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
          .minutes_in_hmi_full += minutes;
      break;
    case usage_statistics::SECONDS_HMI_LIMITED:
      (*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
          .minutes_in_hmi_limited += minutes;
      break;
    case usage_statistics::SECONDS_HMI_BACKGROUND:
      (*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
          .minutes_in_hmi_background += minutes;
      break;
    case usage_statistics::SECONDS_HMI_NONE:
      (*pt_->policy_table.usage_and_error_counts->app_level)[app_id]
          .minutes_in_hmi_none += minutes;
      break;
    default:
      LOG4CXX_WARN(logger_, "Type app stopwatch is unknown");
      return;
  }
  Backup();
}

long CacheManager::ConvertSecondsToMinute(int seconds) {
  const float seconds_in_minute = 60.0;
  return std::round(seconds / seconds_in_minute);
}

bool CacheManager::SetDefaultPolicy(const std::string& app_id) {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies_section.apps.find(kDefaultId);
  if (pt_->policy_table.app_policies_section.apps.end() != iter) {
    pt_->policy_table.app_policies_section.apps[app_id] =
        pt_->policy_table.app_policies_section.apps[kDefaultId];

    SetIsDefault(app_id);
  }
  Backup();
  return true;
}

bool CacheManager::IsDefaultPolicy(const std::string& app_id) const {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  const bool result =
      pt_->policy_table.app_policies_section.apps.end() !=
          pt_->policy_table.app_policies_section.apps.find(app_id) &&
      policy::kDefaultId ==
          pt_->policy_table.app_policies_section.apps[app_id].get_string();

  return result;
}

bool CacheManager::SetIsDefault(const std::string& app_id) {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies_section.apps.find(app_id);
  if (pt_->policy_table.app_policies_section.apps.end() != iter) {
    pt_->policy_table.app_policies_section.apps[app_id].set_to_string(
        kDefaultId);
  }
  return true;
}

bool CacheManager::SetPredataPolicy(const std::string& app_id) {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies_section.apps.find(kPreDataConsentId);

  if (pt_->policy_table.app_policies_section.apps.end() == iter) {
    LOG4CXX_ERROR(logger_,
                  "Could not set " << kPreDataConsentId
                                   << " permissions for app " << app_id);
    return false;
  }

  pt_->policy_table.app_policies_section.apps[app_id] =
      pt_->policy_table.app_policies_section.apps[kPreDataConsentId];

  pt_->policy_table.app_policies_section.apps[app_id].set_to_string(
      kPreDataConsentId);

  Backup();
  return true;
}

bool CacheManager::IsPredataPolicy(const std::string& app_id) const {
  // TODO(AOleynik): Maybe change for comparison with pre_DataConsent
  // permissions or check string value from get_string()
  sync_primitives::AutoLock auto_lock(cache_lock_);
  if (!IsApplicationRepresented(app_id)) {
    return false;
  }

  policy_table::ApplicationPolicies& apps =
      pt_->policy_table.app_policies_section.apps;
  const policy_table::ApplicationPolicies::mapped_type& pre_data_app =
      apps[kPreDataConsentId];
  const policy_table::ApplicationPolicies::mapped_type& specific_app =
      apps[app_id];

  policy_table::Strings res;
  std::set_intersection(pre_data_app.groups.begin(),
                        pre_data_app.groups.end(),
                        specific_app.groups.begin(),
                        specific_app.groups.end(),
                        std::back_inserter(res));

  const bool is_marked_as_predata =
      (kPreDataConsentId == specific_app.get_string());

  return !res.empty() && is_marked_as_predata;
}

bool CacheManager::SetUnpairedDevice(const std::string& device_id,
                                     bool unpaired) {
  sync_primitives::AutoLock auto_lock(cache_lock_);
  const bool result = pt_->policy_table.device_data->end() !=
                      pt_->policy_table.device_data->find(device_id);
  if (!result) {
    LOG4CXX_DEBUG(logger_,
                  "Couldn't set unpaired flag for device id "
                      << device_id << " , since it wasn't found.");
    return false;
  }

  sync_primitives::AutoLock lock(unpaired_lock_);
  if (unpaired) {
    is_unpaired_.insert(device_id);
    LOG4CXX_DEBUG(logger_, "Unpaired flag was set for device id " << device_id);
  } else {
    is_unpaired_.erase(device_id);
    LOG4CXX_DEBUG(logger_,
                  "Unpaired flag was removed for device id " << device_id);
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
  if (kDeviceId == app_id) {
    return true;
  }
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::const_iterator iter =
      pt_->policy_table.app_policies_section.apps.find(app_id);
  return pt_->policy_table.app_policies_section.apps.end() != iter;
}

bool CacheManager::Init(const std::string& file_name,
                        const PolicySettings* settings) {
  LOG4CXX_AUTO_TRACE(logger_);
  settings_ = settings;
  InitResult init_result = backup_->Init(settings);

  bool result = true;
  switch (init_result) {
    case InitResult::EXISTS: {
      LOG4CXX_INFO(logger_, "Policy Table exists, was loaded correctly.");
      result = LoadFromBackup();
      if (result) {
        if (!backup_->IsDBVersionActual()) {
          LOG4CXX_INFO(logger_, "DB version is NOT actual");
          if (!backup_->RefreshDB()) {
            LOG4CXX_ERROR(logger_, "RefreshDB() failed");
            return false;
          }
          backup_->UpdateDBVersion();
          Backup();
        }
        if (!MergePreloadPT(file_name)) {
          result = false;
        }
      }
    } break;
    case InitResult::SUCCESS: {
      LOG4CXX_INFO(logger_, "Policy Table was inited successfully");

      result = LoadFromFile(file_name, *pt_);

      std::shared_ptr<policy_table::Table> snapshot = GenerateSnapshot();
      result &= snapshot->is_valid();
      LOG4CXX_DEBUG(logger_,
                    "Check if snapshot is valid: " << std::boolalpha << result);
      if (!result) {
        rpc::ValidationReport report("policy_table");
        snapshot->ReportErrors(&report);
        return result;
      }

      backup_->UpdateDBVersion();
      Backup();
    } break;
    default: {
      result = false;
      LOG4CXX_ERROR(logger_, "Failed to init policy table.");
    } break;
  }

  return result;
}

void CacheManager::FillDeviceSpecificData() {}

bool CacheManager::LoadFromBackup() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(cache_lock_);
  pt_ = backup_->GenerateSnapshot();
  update_required = backup_->UpdateRequired();
  LOG4CXX_DEBUG(logger_,
                "Update required flag from backup: " << std::boolalpha
                                                     << update_required);
  FillDeviceSpecificData();

  return true;
}

void CacheManager::MakeLowerCaseAppNames(policy_table::Table& pt) const {
  policy_table::ApplicationPolicies& apps =
      pt.policy_table.app_policies_section.apps;
  for (policy_table::ApplicationPolicies::iterator iter = apps.begin();
       iter != apps.end();) {
    std::string key = iter->first;
    if (key == kDefaultId || key == kPreDataConsentId || key == kDeviceId) {
      ++iter;
      continue;
    }

    std::transform(key.begin(), key.end(), key.begin(), ::tolower);
    if (key.compare(iter->first) != 0) {
      std::swap(apps[key], iter->second);
      iter = apps.erase(iter);
    } else {
      ++iter;
    }
  }
}

bool CacheManager::LoadFromFile(const std::string& file_name,
                                policy_table::Table& table) {
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

  table = policy_table::Table(&value);

  Json::StyledWriter s_writer;
  LOG4CXX_DEBUG(logger_, "PT out:");
  LOG4CXX_DEBUG(logger_, s_writer.write(table.ToJsonValue()));

  MakeLowerCaseAppNames(table);

  if (!table.is_valid()) {
    rpc::ValidationReport report("policy_table");
    table.ReportErrors(&report);
    LOG4CXX_FATAL(logger_,
                  "Parsed table is not valid " << rpc::PrettyFormat(report));
    return false;
  }

  return true;
}

bool CacheManager::ResetPT(const std::string& file_name) {
  bool result = true;
  Backup();
  return result;
}

bool CacheManager::AppExists(const std::string& app_id) const {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::iterator policy_iter =
      pt_->policy_table.app_policies_section.apps.find(app_id);
  return pt_->policy_table.app_policies_section.apps.end() != policy_iter;
}

int32_t CacheManager::GenerateHash(const std::string& str_to_hash) {
  uint32_t hash = 5381U;
  std::string::const_iterator it = str_to_hash.begin();
  std::string::const_iterator it_end = str_to_hash.end();

  for (; it != it_end; ++it) {
    hash = ((hash << 5) + hash) + (*it);
  }

  // Reset sign bit in case it has been set.
  // This is needed to avoid overflow for signed int.
  const int32_t result = hash & 0x7FFFFFFF;
  return result;
}

RequestType::State CacheManager::GetAppRequestTypesState(
    const std::string& policy_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::iterator app_policies_iter =
      pt_->policy_table.app_policies_section.apps.find(policy_app_id);
  if (pt_->policy_table.app_policies_section.apps.end() == app_policies_iter) {
    LOG4CXX_DEBUG(logger_,
                  "Can't find request types for app_id " << policy_app_id);
    return RequestType::State::UNAVAILABLE;
  }
  const policy_table::RequestTypes& request_types =
      *app_policies_iter->second.RequestType;
  if (!request_types.is_initialized()) {
    LOG4CXX_DEBUG(logger_,
                  "Request types for " << policy_app_id << " are OMITTED");
    return RequestType::State::OMITTED;
  }
  if (request_types.empty()) {
    LOG4CXX_DEBUG(logger_,
                  "Request types for " << policy_app_id << " are EMPTY");
    return RequestType::State::EMPTY;
  }
  return RequestType::State::AVAILABLE;
}

void CacheManager::GetAppRequestTypes(
    const std::string& policy_app_id,
    std::vector<std::string>& request_types) const {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  if (kDeviceId == policy_app_id) {
    LOG4CXX_DEBUG(logger_,
                  "Request types not applicable for app_id " << kDeviceId);
    return;
  }
  policy_table::ApplicationPolicies::iterator policy_iter =
      pt_->policy_table.app_policies_section.apps.find(policy_app_id);
  if (pt_->policy_table.app_policies_section.apps.end() == policy_iter) {
    LOG4CXX_DEBUG(logger_,
                  "Can't find request types for app_id " << policy_app_id);
    return;
  }

  for (const auto& request_type : *policy_iter->second.RequestType) {
    request_types.push_back(EnumToJsonString(request_type));
  }
  return;
}

RequestSubType::State CacheManager::GetAppRequestSubTypesState(
    const std::string& policy_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::iterator app_policies_iter =
      pt_->policy_table.app_policies_section.apps.find(policy_app_id);
  if (pt_->policy_table.app_policies_section.apps.end() == app_policies_iter) {
    LOG4CXX_DEBUG(logger_,
                  "Can't find request subtypes for app_id " << policy_app_id);
    return RequestSubType::State::UNAVAILABLE;
  }
  const policy_table::RequestSubTypes& request_subtypes =
      *app_policies_iter->second.RequestSubType;
  if (!request_subtypes.is_initialized()) {
    LOG4CXX_DEBUG(logger_,
                  "Request subtypes for " << policy_app_id << " are OMITTED");
    return RequestSubType::State::OMITTED;
  }
  if (request_subtypes.empty()) {
    LOG4CXX_DEBUG(logger_,
                  "Request subtypes for " << policy_app_id << " are EMPTY");
    return RequestSubType::State::EMPTY;
  }
  return RequestSubType::State::AVAILABLE;
}

void CacheManager::GetAppRequestSubTypes(
    const std::string& policy_app_id,
    std::vector<std::string>& request_types) const {
  LOG4CXX_AUTO_TRACE(logger_);
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  if (kDeviceId == policy_app_id) {
    LOG4CXX_DEBUG(logger_,
                  "Request subtypes not applicable for app_id " << kDeviceId);
    return;
  }
  policy_table::ApplicationPolicies::iterator policy_iter =
      pt_->policy_table.app_policies_section.apps.find(policy_app_id);
  if (pt_->policy_table.app_policies_section.apps.end() == policy_iter) {
    LOG4CXX_DEBUG(logger_,
                  "Can't find request subtypes for app_id " << policy_app_id);
    return;
  }

  for (const auto& request_subtype : *policy_iter->second.RequestSubType) {
    request_types.push_back(request_subtype);
  }
  return;
}

std::string CacheManager::GetCertificate() const {
  CACHE_MANAGER_CHECK(std::string(""));
  sync_primitives::AutoLock auto_lock(cache_lock_);
  if (pt_->policy_table.module_config.certificate.is_initialized()) {
    return *pt_->policy_table.module_config.certificate;
  }
  return std::string("");
}

bool CacheManager::MergePreloadPT(const std::string& file_name) {
  LOG4CXX_AUTO_TRACE(logger_);
  policy_table::Table table;
  if (!LoadFromFile(file_name, table)) {
    LOG4CXX_DEBUG(logger_, "Unable to load preloaded PT.");
    return false;
  }

  sync_primitives::AutoLock lock(cache_lock_);
  policy_table::PolicyTable& current = pt_->policy_table;
  policy_table::PolicyTable& new_table = table.policy_table;
  const std::string date_current = *current.module_config.preloaded_date;
  const std::string date_new = *new_table.module_config.preloaded_date;
  if (date_current != date_new) {
    MergeMC(new_table, current);
    MergeFG(new_table, current);
    MergeAP(new_table, current);
    MergeCFM(new_table, current);
    Backup();
  }
  return true;
}

void CacheManager::MergeMC(const policy_table::PolicyTable& new_pt,
                           policy_table::PolicyTable& pt) {
  LOG4CXX_AUTO_TRACE(logger_);
  policy_table::ModuleConfig copy(pt.module_config);

  pt.module_config = new_pt.module_config;
  pt.module_config.vehicle_make = copy.vehicle_make;
  pt.module_config.vehicle_year = copy.vehicle_year;
  pt.module_config.vehicle_model = copy.vehicle_model;
}

void CacheManager::MergeFG(const policy_table::PolicyTable& new_pt,
                           policy_table::PolicyTable& pt) {
  LOG4CXX_AUTO_TRACE(logger_);
  policy_table::FunctionalGroupings::const_iterator it =
      new_pt.functional_groupings.begin();

  for (; it != new_pt.functional_groupings.end(); ++it) {
    LOG4CXX_DEBUG(logger_, "Merge functional group: " << it->first);
    pt.functional_groupings[it->first] = it->second;
  }
}

void CacheManager::MergeAP(const policy_table::PolicyTable& new_pt,
                           policy_table::PolicyTable& pt) {
  LOG4CXX_AUTO_TRACE(logger_);
  pt.app_policies_section.device = const_cast<policy_table::PolicyTable&>(
                                       new_pt).app_policies_section.device;

  pt.app_policies_section.apps[kDefaultId] =
      const_cast<policy_table::PolicyTable&>(new_pt)
          .app_policies_section.apps[kDefaultId];

  pt.app_policies_section.apps[kPreDataConsentId] =
      const_cast<policy_table::PolicyTable&>(new_pt)
          .app_policies_section.apps[kPreDataConsentId];
}

void CacheManager::MergeCFM(const policy_table::PolicyTable& new_pt,
                            policy_table::PolicyTable& pt) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (new_pt.consumer_friendly_messages.is_initialized()) {
    if (!pt.consumer_friendly_messages.is_initialized()) {
      pt.consumer_friendly_messages = new_pt.consumer_friendly_messages;
    } else {
      policy_table::Messages::const_iterator it =
          new_pt.consumer_friendly_messages->messages->begin();

      pt.consumer_friendly_messages->version =
          new_pt.consumer_friendly_messages->version;
      for (; it != new_pt.consumer_friendly_messages->messages->end(); ++it) {
        LOG4CXX_DEBUG(logger_, "Merge CFM: " << it->first);
        if (!(pt.consumer_friendly_messages->messages.is_initialized())) {
          LOG4CXX_DEBUG(logger_, "CFM not initialized.");
        }
        (*pt.consumer_friendly_messages->messages)[it->first] = it->second;
      }
    }
  }
}

const PolicySettings& CacheManager::get_settings() const {
  DCHECK(settings_);

  return *settings_;
}

void CacheManager::OnDeviceSwitching(const std::string& device_id_from,
                                     const std::string& device_id_to) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_INFO(logger_, "Implementation does not support user consents.");
}

CacheManager::BackgroundBackuper::BackgroundBackuper(
    CacheManager* cache_manager)
    : cache_manager_(cache_manager)
    , stop_flag_(false)
    , new_data_available_(false) {
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
    {
      sync_primitives::AutoUnlock unlock(need_backup_lock_);
      InternalBackup();
    }
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

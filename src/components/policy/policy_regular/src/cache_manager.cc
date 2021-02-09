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
#include <boost/algorithm/string.hpp>
#include <cmath>
#include <ctime>
#include <functional>
#include <sstream>

#include "interfaces/MOBILE_API.h"
#include "json/json_features.h"
#include "json/reader.h"
#include "json/writer.h"
#include "smart_objects/enum_schema_item.h"
#include "utils/date_time.h"
#include "utils/file_system.h"
#include "utils/gen_hash.h"
#include "utils/helpers.h"
#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

#include "policy/policy_helper.h"
#include "policy/sql_pt_representation.h"

namespace policy_table = rpc::policy_table_interface_base;

namespace policy {

SDL_CREATE_LOG_VARIABLE("Policy")

#define CACHE_MANAGER_CHECK(return_value)                   \
  {                                                         \
    if (!pt_) {                                             \
      SDL_LOG_WARN("The cache manager is not initialized"); \
      return return_value;                                  \
    }                                                       \
  }

#define CACHE_MANAGER_CHECK_VOID()                          \
  {                                                         \
    if (!pt_) {                                             \
      SDL_LOG_WARN("The cache manager is not initialized"); \
      return;                                               \
    }                                                       \
  }

struct LanguageFinder {
  explicit LanguageFinder(const std::string& language) : language_(language) {}
  bool operator()(const policy_table::Languages::value_type& lang) const {
    return !strcasecmp(language_.c_str(), lang.first.c_str());
  }

 private:
  const std::string& language_;
};

struct PolicyTableUpdater {
  explicit PolicyTableUpdater(
      const policy_table::ApplicationParams& default_params)
      : default_params_(default_params) {}

  void operator()(
      policy_table::ApplicationPolicies::value_type& pt_value) const {
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
    , removed_custom_vd_items_()
    , settings_(nullptr) {
  SDL_LOG_AUTO_TRACE();
  backuper_ = new BackgroundBackuper(this);
  backup_thread_ = threads::CreateThread("Backup thread", backuper_);
  backup_thread_->Start();
}

CacheManager::~CacheManager() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(backuper_locker_);
  backup_thread_->Stop(threads::Thread::kThreadSoftStop);
  delete backup_thread_->GetDelegate();
  threads::DeleteThread(backup_thread_);
}

const policy_table::Strings& CacheManager::GetGroups(const PTString& app_id) {
  sync_primitives::AutoLock auto_lock(cache_lock_);
  return pt_->policy_table.app_policies_section.apps[app_id].groups;
}

const policy_table::Strings CacheManager::GetPolicyAppIDs() const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(cache_lock_);

  const auto apps = pt_->policy_table.app_policies_section.apps;

  policy_table::Strings policy_app_ids;
  for (const auto& app : apps) {
    policy_app_ids.push_back(app.first);
  }

  return policy_app_ids;
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
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK(false);
  return true;
}

void CacheManager::GetAllAppGroups(const std::string& app_id,
                                   FunctionalGroupIDs& all_group_ids) {
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK_VOID();

  if (kDeviceId == app_id) {
    SDL_LOG_INFO("Devices doesn't have groups");
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
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::GetConsentedGroups(const std::string& device_id,
                                      const std::string& app_id,
                                      FunctionalGroupIDs& allowed_groups,
                                      FunctionalGroupIDs& disallowed_groups) {
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::GetUnconsentedGroups(
    const std::string& device_id,
    const std::string& policy_app_id,
    FunctionalGroupIDs& unconsented_groups) {
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK_VOID();
}

void CacheManager::RemoveAppConsentForGroup(const std::string& app_id,
                                            const std::string& group_name) {
  CACHE_MANAGER_CHECK_VOID();
}

bool CacheManager::ApplyUpdate(const policy_table::Table& update_pt) {
  SDL_LOG_AUTO_TRACE();
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

  pt_->policy_table.module_config.endpoint_properties =
      update_pt.policy_table.module_config.endpoint_properties;

  // Apply update for vehicle data
  if (update_pt.policy_table.vehicle_data.is_initialized()) {
    policy_table::VehicleDataItems custom_items_before_apply;
    if (pt_->policy_table.vehicle_data->schema_items.is_initialized()) {
      custom_items_before_apply =
          CollectCustomVDItems(*pt_->policy_table.vehicle_data->schema_items);
    }

    if (!update_pt.policy_table.vehicle_data->schema_items.is_initialized() ||
        update_pt.policy_table.vehicle_data->schema_items->empty()) {
      pt_->policy_table.vehicle_data->schema_items =
          rpc::Optional<policy_table::VehicleDataItems>();
    } else {
      policy_table::VehicleDataItems custom_items = CollectCustomVDItems(
          *update_pt.policy_table.vehicle_data->schema_items);

      pt_->policy_table.vehicle_data->schema_version =
          update_pt.policy_table.vehicle_data->schema_version;
      pt_->policy_table.vehicle_data->schema_items =
          rpc::Optional<policy_table::VehicleDataItems>(custom_items);
    }

    policy_table::VehicleDataItems custom_items_after_apply =
        *pt_->policy_table.vehicle_data->schema_items;
    const auto& items_diff = CalculateCustomVdItemsDiff(
        custom_items_before_apply, custom_items_after_apply);
    SetRemovedCustomVdItems(items_diff);
  }

  ResetCalculatedPermissions();
  Backup();
  return true;
}

policy_table::VehicleDataItems CacheManager::CollectCustomVDItems(
    const policy_table::VehicleDataItems& vd_items) {
  policy_table::VehicleDataItems result_items;
  for (auto& item : vd_items) {
    const std::string i_name = "VEHICLEDATA_" + std::string(item.name);
    const std::string vd_name = boost::to_upper_copy<std::string>(i_name);
    const bool is_rpc_spec =
        policy_table::EnumSchemaItemFactory::IsRPCSpecVehicleDataType(vd_name);
    if (!is_rpc_spec) {
      result_items.push_back(item);
    }
  }
  return result_items;
}

void CacheManager::GetHMIAppTypeAfterUpdate(
    std::map<std::string, StringArray>& app_hmi_types) {
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_AUTO_TRACE();
  GetAllAppGroups(app_id, group_types[kTypeGeneral]);
  GetAllAppGroups(kDefaultId, group_types[kTypeDefault]);
  GetAllAppGroups(kPreDataConsentId, group_types[kTypePreDataConsented]);
  return true;
}

bool CacheManager::GetDeviceGroupsFromPolicies(
    policy_table::Strings& groups,
    policy_table::Strings& preconsented_groups) const {
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK(false);
  return true;
}

bool CacheManager::AddDevice(const std::string& device_id,
                             const std::string& connection_type) {
  SDL_LOG_AUTO_TRACE();

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
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoLock auto_lock(cache_lock_);
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::SetUserPermissionsForDevice(
    const std::string& device_id,
    const StringArray& consented_groups,
    const StringArray& disallowed_groups) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  CACHE_MANAGER_CHECK(false);
  Backup();
  return true;
}

bool CacheManager::ReactOnUserDevConsentForApp(const std::string& app_id,
                                               bool is_device_allowed) {
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_AUTO_TRACE();
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

          for (const auto& param : *rpc_param.parameters) {
            result.list_of_allowed_params.insert(std::string(param));
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
  SDL_LOG_DEBUG("IgnitionCyclesBeforeExchange limit:" << limit);
  uint8_t current = 0;

  const int last_exch = static_cast<int>(
      *pt_->policy_table.module_meta->ignition_cycles_since_last_exchange);
  current = std::max(last_exch, 0);
  SDL_LOG_DEBUG(
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
  SDL_LOG_DEBUG("KilometersBeforeExchange limit:" << limit);
  int last = 0;

  const int odo_val = static_cast<int>(
      *pt_->policy_table.module_meta->pt_exchanged_at_odometer_x);
  last = std::max(odo_val, 0);
  SDL_LOG_DEBUG("KilometersBeforeExchange last:" << last);

  const int actual = std::max((current - last), 0);
  SDL_LOG_DEBUG("KilometersBeforeExchange actual:" << actual);
  return std::max(limit - actual, 0);
}

bool CacheManager::SetCountersPassedForSuccessfulUpdate(
    policy::Counters counter, int value) {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  switch (counter) {
    case KILOMETERS:
      *pt_->policy_table.module_meta->pt_exchanged_at_odometer_x = value;
      SDL_LOG_DEBUG("SetCountersPassedForSuccessfulUpdate km:" << value);
      break;
    case DAYS_AFTER_EPOCH:
      *pt_->policy_table.module_meta->pt_exchanged_x_days_after_epoch = value;
      SDL_LOG_DEBUG(
          "SetCountersPassedForSuccessfulUpdate days after epoch:" << value);
      break;
    default:
      SDL_LOG_ERROR("Unknown counter was requested to set: " << counter);
      return false;
  }

  Backup();
  return true;
}

int CacheManager::DaysBeforeExchange(int current) {
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK(0);
  sync_primitives::AutoLock auto_lock(cache_lock_);

  const rpc::Optional<rpc::Integer<uint16_t, 0, 65535> >& days_after_epoch =
      (pt_->policy_table.module_meta->pt_exchanged_x_days_after_epoch);

  if (!days_after_epoch->is_initialized()) {
    return -1;
  }

  const uint8_t limit = pt_->policy_table.module_config.exchange_after_x_days;
  SDL_LOG_DEBUG("Exchange after: " << static_cast<int>(limit) << " days");

  SDL_LOG_DEBUG("Epoch since last update: " << *days_after_epoch);

  const uint16_t actual =
      std::max(static_cast<uint16_t>(current - *days_after_epoch), uint16_t(0));
  SDL_LOG_DEBUG("The days since last update: " << actual);

  return std::max(limit - actual, 0);
}

void CacheManager::IncrementIgnitionCycles() {
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  const int ign_val = static_cast<int>(
      *pt_->policy_table.module_meta->ignition_cycles_since_last_exchange);
  (*pt_->policy_table.module_meta->ignition_cycles_since_last_exchange) =
      ign_val + 1;
  SDL_LOG_DEBUG("IncrementIgnitionCycles ignitions:" << ign_val);
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

const std::vector<policy_table::VehicleDataItem>
CacheManager::GetVehicleDataItems() const {
  CACHE_MANAGER_CHECK(std::vector<policy_table::VehicleDataItem>());
  sync_primitives::AutoLock auto_lock(cache_lock_);
  if (pt_->policy_table.vehicle_data.is_initialized() &&
      pt_->policy_table.vehicle_data->schema_items.is_initialized()) {
    return *(pt_->policy_table.vehicle_data->schema_items);
  }

  return std::vector<policy_table::VehicleDataItem>();
}

std::vector<policy_table::VehicleDataItem>
CacheManager::GetRemovedVehicleDataItems() const {
  CACHE_MANAGER_CHECK(std::vector<policy_table::VehicleDataItem>());
  return removed_custom_vd_items_;
}

Json::Value CacheManager::GetPolicyTableData() const {
  return pt_->policy_table.ToJsonValue();
}

void CacheManager::GetEnabledCloudApps(
    std::vector<std::string>& enabled_apps) const {
#if !defined(CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT)
  enabled_apps.clear();
  return;
#else
  const policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  for (policy_table::ApplicationPolicies::const_iterator it = policies.begin();
       it != policies.end();
       ++it) {
    auto app_policy = (*it).second;
    if (app_policy.enabled.is_initialized() && *app_policy.enabled) {
      enabled_apps.push_back((*it).first);
    }
  }
#endif  // CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT
}

std::vector<std::string> CacheManager::GetEnabledLocalApps() const {
#if !defined(WEBSOCKET_SERVER_TRANSPORT_SUPPORT)
  return std::vector<std::string>();
#else
  std::vector<std::string> enabled_apps;
  const policy_table::ApplicationPolicies& app_policies =
      pt_->policy_table.app_policies_section.apps;
  for (const auto& app_policies_item : app_policies) {
    const auto app_policy = app_policies_item.second;
    // Local (WebEngine) applications
    // should not have "endpoint" field
    if (app_policy.endpoint.is_initialized()) {
      continue;
    }
    if (app_policy.enabled.is_initialized() && *app_policy.enabled) {
      enabled_apps.push_back(app_policies_item.first);
    }
  }
  return enabled_apps;
#endif  // WEBSOCKET_SERVER_TRANSPORT_SUPPORT
}

bool CacheManager::GetAppProperties(const std::string& policy_app_id,
                                    AppProperties& out_app_properties) const {
  const policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::const_iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter) {
    auto app_policy = (*policy_iter).second;
    out_app_properties.endpoint = app_policy.endpoint.is_initialized()
                                      ? *app_policy.endpoint
                                      : std::string();
    out_app_properties.auth_token = app_policy.auth_token.is_initialized()
                                        ? *app_policy.auth_token
                                        : std::string();
    out_app_properties.transport_type =
        app_policy.cloud_transport_type.is_initialized()
            ? *app_policy.cloud_transport_type
            : std::string();
    out_app_properties.certificate = app_policy.certificate.is_initialized()
                                         ? *app_policy.certificate
                                         : std::string();
    out_app_properties.hybrid_app_preference =
        app_policy.hybrid_app_preference.is_initialized()
            ? EnumToJsonString(*app_policy.hybrid_app_preference)
            : std::string();
    out_app_properties.enabled =
        app_policy.enabled.is_initialized() && *app_policy.enabled;
    return true;
  }
  return false;
}

void CacheManager::InitCloudApp(const std::string& policy_app_id) {
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);

  policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::const_iterator default_iter =
      policies.find(kDefaultId);
  policy_table::ApplicationPolicies::const_iterator app_iter =
      policies.find(policy_app_id);

  if (default_iter != policies.end()) {
    if (app_iter == policies.end()) {
      policies[policy_app_id] = policies[kDefaultId];
    }
  }
  // Add cloud app specific policies

  Backup();
}

void CacheManager::SetCloudAppEnabled(const std::string& policy_app_id,
                                      const bool enabled) {
  policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter) {
    *(*policy_iter).second.enabled = enabled;
  }
}

void CacheManager::SetAppAuthToken(const std::string& policy_app_id,
                                   const std::string& auth_token) {
  policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter) {
    *(*policy_iter).second.auth_token = auth_token;
  }
}

void CacheManager::SetAppCloudTransportType(
    const std::string& policy_app_id, const std::string& cloud_transport_type) {
  policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter) {
    *(*policy_iter).second.cloud_transport_type = cloud_transport_type;
  }
}

void CacheManager::SetAppEndpoint(const std::string& policy_app_id,
                                  const std::string& endpoint) {
  policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter) {
    *(*policy_iter).second.endpoint = endpoint;
  }
}

void CacheManager::SetAppNicknames(const std::string& policy_app_id,
                                   const StringArray& nicknames) {
  policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter) {
    (*(*policy_iter).second.nicknames) = nicknames;
  }
}

void CacheManager::SetHybridAppPreference(
    const std::string& policy_app_id,
    const std::string& hybrid_app_preference) {
  policy_table::HybridAppPreference value;
  bool valid = EnumFromJsonString(hybrid_app_preference, &value);
  policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter && valid) {
    *(*policy_iter).second.hybrid_app_preference = value;
  }
}

void CacheManager::GetAppServiceParameters(
    const std::string& policy_app_id,
    policy_table::AppServiceParameters* app_service_parameters) const {
  SDL_LOG_AUTO_TRACE();
  const policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::const_iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter) {
    auto app_policy = (*policy_iter).second;
    if (app_policy.app_service_parameters.is_initialized()) {
      *app_service_parameters = *(app_policy.app_service_parameters);
    }
  }
}

bool CacheManager::UnknownRPCPassthroughAllowed(
    const std::string& policy_app_id) const {
  const policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::const_iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter) {
    const auto app_policy = (*policy_iter).second;
    if (app_policy.allow_unknown_rpc_passthrough.is_initialized()) {
      return *(app_policy.allow_unknown_rpc_passthrough);
    }
  }
  return false;
}

const boost::optional<bool> CacheManager::LockScreenDismissalEnabledState()
    const {
  SDL_LOG_AUTO_TRACE();
  boost::optional<bool> empty;
  CACHE_MANAGER_CHECK(empty);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ModuleConfig& module_config = pt_->policy_table.module_config;
  if (module_config.lock_screen_dismissal_enabled.is_initialized()) {
    SDL_LOG_TRACE("state = " << *module_config.lock_screen_dismissal_enabled);
    return boost::optional<bool>(*module_config.lock_screen_dismissal_enabled);
  }
  SDL_LOG_TRACE("state = empty");
  return empty;
}

const boost::optional<std::string>
CacheManager::LockScreenDismissalWarningMessage(
    const std::string& language) const {
  SDL_LOG_AUTO_TRACE();
  boost::optional<std::string> empty;
  CACHE_MANAGER_CHECK(empty);

  const std::string lock_screen_dismissal_warning_message =
      "LockScreenDismissalWarning";
  sync_primitives::AutoLock auto_lock(cache_lock_);

  std::vector<std::string> msg_codes{lock_screen_dismissal_warning_message};

  const auto messages = GetUserFriendlyMsg(msg_codes, language);

  if (messages.empty() || messages[0].text_body.empty()) {
    return empty;
  }

  return boost::optional<std::string>(messages[0].text_body);
}

std::vector<UserFriendlyMessage> CacheManager::GetUserFriendlyMsg(
    const std::vector<std::string>& msg_codes,
    const std::string& language) const {
  SDL_LOG_AUTO_TRACE();
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
      SDL_LOG_WARN("Language "
                   << language
                   << " haven't been found for message code: " << *it);

      LanguageFinder fallback_language_finder("en-us");

      policy_table::Languages::const_iterator it_fallback_language =
          std::find_if(msg_languages.languages.begin(),
                       msg_languages.languages.end(),
                       fallback_language_finder);

      if (msg_languages.languages.end() == it_fallback_language) {
        SDL_LOG_ERROR("No fallback language found for message code: " << *it);
        continue;
      }

      message_string = it_fallback_language->second;
    } else {
      message_string = it_language->second;
    }

    UserFriendlyMessage msg;
    msg.message_code = *it;
    msg.tts = *message_string.tts;
    msg.label = *message_string.label;
    msg.line1 = *message_string.line1;
    msg.line2 = *message_string.line2;
    msg.text_body = *message_string.textBody;

    result.push_back(msg);
  }
  return result;
}

void CacheManager::GetUpdateUrls(const uint32_t service_type,
                                 EndpointUrls& out_end_points) const {
  auto find_hexademical =
      [service_type](policy_table::ServiceEndpoints::value_type end_point) {
        uint32_t decimal;
        std::istringstream(end_point.first) >> std::hex >> decimal;
        return end_point.first.compare(0, 2, "0x") == 0 &&
               decimal == service_type;
      };
  auto& end_points = pt_->policy_table.module_config.endpoints;
  const auto end_point =
      std::find_if(end_points.begin(), end_points.end(), find_hexademical);
  if (end_point != end_points.end()) {
    GetUpdateUrls(end_point->first, out_end_points);
  }
}

void CacheManager::GetUpdateUrls(const std::string& service_type,
                                 EndpointUrls& out_end_points) const {
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK_VOID();

  SDL_LOG_DEBUG("Search service value is: " << service_type);

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

std::string CacheManager::GetIconUrl(const std::string& policy_app_id) const {
  CACHE_MANAGER_CHECK(std::string());
  std::string url;
  const policy_table::ApplicationPolicies& policies =
      pt_->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::const_iterator policy_iter =
      policies.find(policy_app_id);
  if (policies.end() != policy_iter) {
    auto app_policy = (*policy_iter).second;
    url = app_policy.icon_url.is_initialized() ? *app_policy.icon_url
                                               : std::string();
  }
  return url;
}

rpc::policy_table_interface_base::NumberOfNotificationsType
CacheManager::GetNotificationsNumber(const std::string& priority,
                                     const bool is_subtle) {
  CACHE_MANAGER_CHECK(0);

  sync_primitives::AutoLock auto_lock(cache_lock_);
  const auto& module_config = pt_->policy_table.module_config;
  const auto& nnpm =
      is_subtle && module_config.subtle_notifications_per_minute_by_priority
                       .is_initialized()
          ? *module_config.subtle_notifications_per_minute_by_priority
          : module_config.notifications_per_minute_by_priority;

  auto priority_iter = nnpm.find(priority);

  const uint32_t result =
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
  const bool app_id_exists =
      policies.end() != policy_iter && !IsApplicationRevoked(policy_app_id);
  if (app_id_exists) {
    priority = EnumToJsonString((*policy_iter).second.priority);
  }

  return app_id_exists;
}

void CacheManager::CheckSnapshotInitialization() {
  CACHE_MANAGER_CHECK_VOID();

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
    SDL_LOG_WARN("policy_table.consumer_friendly_messages is not initialized");
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
    SDL_LOG_WARN("app_level is not initialized");
  }
}

policy_table::VehicleDataItems CacheManager::CalculateCustomVdItemsDiff(
    const policy_table::VehicleDataItems& items_before,
    const policy_table::VehicleDataItems& items_after) const {
  SDL_LOG_AUTO_TRACE();
  if (items_before.empty()) {
    SDL_LOG_DEBUG("No custom VD items found in policy");
    return policy_table::VehicleDataItems();
  }

  if (items_after.empty()) {
    SDL_LOG_DEBUG("All custom VD items were removed after policy update");
    return items_before;
  }

  policy_table::VehicleDataItems removed_items;
  for (auto& item_to_search : items_before) {
    auto item_predicate =
        [&item_to_search](const policy_table::VehicleDataItem& item_to_check) {
          return item_to_search.name == item_to_check.name;
        };

    auto it =
        std::find_if(items_after.begin(), items_after.end(), item_predicate);
    if (items_after.end() == it) {
      removed_items.push_back(item_to_search);
    }
  }

  SDL_LOG_DEBUG("Found " << removed_items.size() << " removed VD items");
  return removed_items;
}

void CacheManager::SetRemovedCustomVdItems(
    const policy_table::VehicleDataItems& removed_items) {
  SDL_LOG_AUTO_TRACE();
  removed_custom_vd_items_ = removed_items;
}

void CacheManager::PersistData() {
  SDL_LOG_AUTO_TRACE();
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

      for (; app_policy_iter != app_policy_iter_end; ++app_policy_iter) {
        const std::string app_id = (*app_policy_iter).first;

        if (copy_pt.policy_table.app_policies_section.apps.end() !=
            copy_pt.policy_table.app_policies_section.apps.find(app_id)) {
          is_revoked =
              copy_pt.policy_table.app_policies_section.apps[app_id].is_null();
        }

        bool is_default_policy =
            copy_pt.policy_table.app_policies_section.apps.end() !=
                copy_pt.policy_table.app_policies_section.apps.find(app_id) &&
            policy::kDefaultId ==
                copy_pt.policy_table.app_policies_section.apps[app_id]
                    .get_string();

        // TODO(AOleynik): Remove this field from DB
        bool is_predata_policy =
            copy_pt.policy_table.app_policies_section.apps.end() !=
                copy_pt.policy_table.app_policies_section.apps.find(app_id) &&
            policy::kPreDataConsentId ==
                copy_pt.policy_table.app_policies_section.apps[app_id]
                    .get_string();

        backup_->SaveApplicationCustomData(
            app_id, is_revoked, is_default_policy, is_predata_policy);
        is_revoked = false;
      }

      backup_->SetMetaInfo(*(*copy_pt.policy_table.module_meta).ccpu_version);

      // In case of extended policy the meta info should be backuped as well.
      backup_->WriteDb();
    }
  }
}

void CacheManager::ResetCalculatedPermissions() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(calculated_permissions_lock_);
  calculated_permissions_.clear();
}

void CacheManager::AddCalculatedPermissions(const std::string& device_id,
                                            const std::string& policy_app_id,
                                            const Permissions& permissions) {
  SDL_LOG_DEBUG("AddCalculatedPermissions for device: "
                << device_id << " and app: " << policy_app_id);
  sync_primitives::AutoLock lock(calculated_permissions_lock_);
  calculated_permissions_[device_id][policy_app_id] = permissions;
}

bool CacheManager::IsPermissionsCalculated(const std::string& device_id,
                                           const std::string& policy_app_id,
                                           Permissions& permission) {
  SDL_LOG_DEBUG("IsPermissionsCalculated for device: "
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
  snapshot_->policy_table.usage_and_error_counts->app_level =
      pt_->policy_table.usage_and_error_counts->app_level;
  snapshot_->policy_table.usage_and_error_counts->mark_initialized();
  snapshot_->policy_table.usage_and_error_counts->app_level->mark_initialized();
  snapshot_->policy_table.device_data = pt_->policy_table.device_data;

  if (pt_->policy_table.vehicle_data.is_initialized()) {
    snapshot_->policy_table.vehicle_data =
        rpc::Optional<policy_table::VehicleData>();
    snapshot_->policy_table.vehicle_data->mark_initialized();
    snapshot_->policy_table.vehicle_data->schema_version =
        pt_->policy_table.vehicle_data->schema_version;
  }

  // Set policy table type to Snapshot
  snapshot_->SetPolicyTableType(
      rpc::policy_table_interface_base::PolicyTableType::PT_SNAPSHOT);

  CheckSnapshotInitialization();
  return snapshot_;
}

bool CacheManager::GetInitialAppData(const std::string& app_id,
                                     StringArray& nicknames,
                                     StringArray& app_hmi_types) {
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  const policy_table::FunctionalGroupings& f_groupings =
      pt_->policy_table.functional_groupings;

  groups.insert(f_groupings.begin(), f_groupings.end());
  return true;
}

int CacheManager::CountUnconsentedGroups(const std::string& policy_app_id,
                                         const std::string& device_id) {
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK(false);
  SDL_LOG_DEBUG("Application id: " << policy_app_id);
  int result = 0;
  return result;
}

void CacheManager::SetPreloadedPtFlag(const bool is_preloaded) {
  SDL_LOG_AUTO_TRACE();
  *(pt_->policy_table.module_config.preloaded_pt) = is_preloaded;
  Backup();
}

bool CacheManager::SetMetaInfo(const std::string& ccpu_version,
                               const std::string& wers_country_code,
                               const std::string& language) {
  CACHE_MANAGER_CHECK(false);
  sync_primitives::AutoLock auto_lock(cache_lock_);
  rpc::Optional<policy_table::ModuleMeta>& module_meta =
      pt_->policy_table.module_meta;
  *(module_meta->ccpu_version) = ccpu_version;
  // We have to set preloaded flag as false in policy table on any response
  // of GetSystemInfo (SDLAQ-CRS-2365)
  *(pt_->policy_table.module_config.preloaded_pt) = false;
  Backup();
  return true;
}

std::string CacheManager::GetCCPUVersionFromPT() const {
  SDL_LOG_AUTO_TRACE();
  rpc::Optional<policy_table::ModuleMeta>& module_meta =
      pt_->policy_table.module_meta;
  return *(module_meta->ccpu_version);
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
  SDL_LOG_AUTO_TRACE();
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
      SDL_LOG_WARN("Type app counter is unknown");
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
      SDL_LOG_WARN("Type app info is unknown");
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
      SDL_LOG_WARN("Type app stopwatch is unknown");
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
    SDL_LOG_ERROR("Could not set " << kPreDataConsentId
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
    SDL_LOG_DEBUG("Couldn't set unpaired flag for device id "
                  << device_id << " , since it wasn't found.");
    return false;
  }

  sync_primitives::AutoLock lock(unpaired_lock_);
  if (unpaired) {
    is_unpaired_.insert(device_id);
    SDL_LOG_DEBUG("Unpaired flag was set for device id " << device_id);
  } else {
    is_unpaired_.erase(device_id);
    SDL_LOG_DEBUG("Unpaired flag was removed for device id " << device_id);
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
  SDL_LOG_AUTO_TRACE();
  settings_ = settings;
  InitResult init_result = backup_->Init(settings);

  bool result = true;
  switch (init_result) {
    case InitResult::EXISTS: {
      SDL_LOG_INFO("Policy Table exists, was loaded correctly.");
      result = LoadFromBackup();
      if (result) {
        if (!backup_->IsDBVersionActual()) {
          SDL_LOG_INFO("DB version is NOT actual");
          if (!backup_->RefreshDB()) {
            SDL_LOG_ERROR("RefreshDB() failed");
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
      SDL_LOG_INFO("Policy Table was inited successfully");

      result = LoadFromFile(file_name, *pt_);

      std::shared_ptr<policy_table::Table> snapshot = GenerateSnapshot();
      result &= snapshot->is_valid();
      SDL_LOG_DEBUG("Check if snapshot is valid: " << std::boolalpha << result);
      if (!result) {
        rpc::ValidationReport report("policy_table");
        snapshot->ReportErrors(&report);
        SDL_LOG_DEBUG("Validation report: " << rpc::PrettyFormat(report));
        return result;
      }

      if (!UnwrapAppPolicies(pt_->policy_table.app_policies_section.apps)) {
        SDL_LOG_ERROR("Cannot unwrap application policies");
      }

      backup_->UpdateDBVersion();
      Backup();
    } break;
    default: {
      result = false;
      SDL_LOG_ERROR("Failed to init policy table.");
    } break;
  }

  return result;
}

void CacheManager::FillDeviceSpecificData() {}

bool CacheManager::LoadFromBackup() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(cache_lock_);
  pt_ = backup_->GenerateSnapshot();
  update_required = backup_->UpdateRequired();
  SDL_LOG_DEBUG("Update required flag from backup: " << std::boolalpha
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
  SDL_LOG_AUTO_TRACE();
  BinaryMessage json_string;
  if (!file_system::ReadBinaryFile(file_name, json_string)) {
    SDL_LOG_FATAL("Failed to read pt file.");
    return false;
  }

  Json::CharReaderBuilder reader_builder;
  Json::CharReaderBuilder::strictMode(&reader_builder.settings_);
  auto reader =
      std::unique_ptr<Json::CharReader>(reader_builder.newCharReader());
  Json::Value value;
  JSONCPP_STRING err;
  std::string json(json_string.begin(), json_string.end());
  const size_t json_len = json.length();
  if (!reader->parse(json.c_str(), json.c_str() + json_len, &value, &err)) {
    SDL_LOG_FATAL("Preloaded PT is corrupted: " << err);
    return false;
  }

  SDL_LOG_TRACE("Start create PT");
  sync_primitives::AutoLock locker(cache_lock_);

  table = policy_table::Table(&value);

  Json::StreamWriterBuilder writer_builder;
  SDL_LOG_DEBUG("PT out:");
  SDL_LOG_DEBUG(Json::writeString(writer_builder, table.ToJsonValue()));

  MakeLowerCaseAppNames(table);

  if (!table.is_valid()) {
    rpc::ValidationReport report("policy_table");
    table.ReportErrors(&report);
    SDL_LOG_FATAL("Parsed table is not valid " << rpc::PrettyFormat(report));
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
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::iterator app_policies_iter =
      pt_->policy_table.app_policies_section.apps.find(policy_app_id);
  if (pt_->policy_table.app_policies_section.apps.end() == app_policies_iter) {
    SDL_LOG_DEBUG("Can't find request types for app_id " << policy_app_id);
    return RequestType::State::UNAVAILABLE;
  }
  const policy_table::RequestTypes& request_types =
      *app_policies_iter->second.RequestType;
  if (!request_types.is_initialized()) {
    SDL_LOG_DEBUG("Request types for " << policy_app_id << " are OMITTED");
    return RequestType::State::OMITTED;
  }
  if (request_types.empty()) {
    SDL_LOG_DEBUG("Request types for " << policy_app_id << " are EMPTY");
    return RequestType::State::EMPTY;
  }
  return RequestType::State::AVAILABLE;
}

void CacheManager::GetAppRequestTypes(
    const std::string& policy_app_id,
    std::vector<std::string>& request_types) const {
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  if (kDeviceId == policy_app_id) {
    SDL_LOG_DEBUG("Request types not applicable for app_id " << kDeviceId);
    return;
  }
  policy_table::ApplicationPolicies::iterator policy_iter =
      pt_->policy_table.app_policies_section.apps.find(policy_app_id);
  if (pt_->policy_table.app_policies_section.apps.end() == policy_iter) {
    SDL_LOG_DEBUG("Can't find request types for app_id " << policy_app_id);
    return;
  }

  for (const auto& request_type : *policy_iter->second.RequestType) {
    request_types.push_back(EnumToJsonString(request_type));
  }
  return;
}

RequestSubType::State CacheManager::GetAppRequestSubTypesState(
    const std::string& policy_app_id) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  policy_table::ApplicationPolicies::iterator app_policies_iter =
      pt_->policy_table.app_policies_section.apps.find(policy_app_id);
  if (pt_->policy_table.app_policies_section.apps.end() == app_policies_iter) {
    SDL_LOG_DEBUG("Can't find request subtypes for app_id " << policy_app_id);
    return RequestSubType::State::UNAVAILABLE;
  }
  const policy_table::RequestSubTypes& request_subtypes =
      *app_policies_iter->second.RequestSubType;
  if (!request_subtypes.is_initialized()) {
    SDL_LOG_DEBUG("Request subtypes for " << policy_app_id << " are OMITTED");
    return RequestSubType::State::OMITTED;
  }
  if (request_subtypes.empty()) {
    SDL_LOG_DEBUG("Request subtypes for " << policy_app_id << " are EMPTY");
    return RequestSubType::State::EMPTY;
  }
  return RequestSubType::State::AVAILABLE;
}

void CacheManager::GetAppRequestSubTypes(
    const std::string& policy_app_id,
    std::vector<std::string>& request_types) const {
  SDL_LOG_AUTO_TRACE();
  CACHE_MANAGER_CHECK_VOID();
  sync_primitives::AutoLock auto_lock(cache_lock_);
  if (kDeviceId == policy_app_id) {
    SDL_LOG_DEBUG("Request subtypes not applicable for app_id " << kDeviceId);
    return;
  }
  policy_table::ApplicationPolicies::iterator policy_iter =
      pt_->policy_table.app_policies_section.apps.find(policy_app_id);
  if (pt_->policy_table.app_policies_section.apps.end() == policy_iter) {
    SDL_LOG_DEBUG("Can't find request subtypes for app_id " << policy_app_id);
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
  SDL_LOG_AUTO_TRACE();
  policy_table::Table table;
  if (!LoadFromFile(file_name, table)) {
    SDL_LOG_DEBUG("Unable to load preloaded PT.");
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
    MergeVD(new_table, current);
    Backup();
  }
  return true;
}

void CacheManager::MergeMC(const policy_table::PolicyTable& new_pt,
                           policy_table::PolicyTable& pt) {
  SDL_LOG_AUTO_TRACE();
  policy_table::ModuleConfig copy(pt.module_config);

  pt.module_config = new_pt.module_config;
  pt.module_config.vehicle_make = copy.vehicle_make;
  pt.module_config.vehicle_year = copy.vehicle_year;
  pt.module_config.vehicle_model = copy.vehicle_model;
}

void CacheManager::MergeFG(const policy_table::PolicyTable& new_pt,
                           policy_table::PolicyTable& pt) {
  SDL_LOG_AUTO_TRACE();
  policy_table::FunctionalGroupings::const_iterator it =
      new_pt.functional_groupings.begin();

  for (; it != new_pt.functional_groupings.end(); ++it) {
    SDL_LOG_DEBUG("Merge functional group: " << it->first);
    pt.functional_groupings[it->first] = it->second;
  }
}

void CacheManager::MergeAP(const policy_table::PolicyTable& new_pt,
                           policy_table::PolicyTable& pt) {
  SDL_LOG_AUTO_TRACE();
  pt.app_policies_section.device =
      const_cast<policy_table::PolicyTable&>(new_pt)
          .app_policies_section.device;

  pt.app_policies_section.apps[kDefaultId] =
      const_cast<policy_table::PolicyTable&>(new_pt)
          .app_policies_section.apps[kDefaultId];

  pt.app_policies_section.apps[kPreDataConsentId] =
      const_cast<policy_table::PolicyTable&>(new_pt)
          .app_policies_section.apps[kPreDataConsentId];
}

void CacheManager::MergeCFM(const policy_table::PolicyTable& new_pt,
                            policy_table::PolicyTable& pt) {
  SDL_LOG_AUTO_TRACE();
  if (new_pt.consumer_friendly_messages.is_initialized()) {
    if (!pt.consumer_friendly_messages.is_initialized()) {
      pt.consumer_friendly_messages = new_pt.consumer_friendly_messages;
    } else {
      policy_table::Messages::const_iterator it =
          new_pt.consumer_friendly_messages->messages->begin();

      pt.consumer_friendly_messages->version =
          new_pt.consumer_friendly_messages->version;
      for (; it != new_pt.consumer_friendly_messages->messages->end(); ++it) {
        SDL_LOG_DEBUG("Merge CFM: " << it->first);
        if (!(pt.consumer_friendly_messages->messages.is_initialized())) {
          SDL_LOG_DEBUG("CFM not initialized.");
        }
        (*pt.consumer_friendly_messages->messages)[it->first] = it->second;
      }
    }
  }
}

void CacheManager::MergeVD(const policy_table::PolicyTable& new_pt,
                           policy_table::PolicyTable& pt) {
  SDL_LOG_AUTO_TRACE();
  pt.vehicle_data.assign_if_valid(new_pt.vehicle_data);
}

const PolicySettings& CacheManager::get_settings() const {
  DCHECK(settings_);

  return *settings_;
}

void CacheManager::OnDeviceSwitching(const std::string& device_id_from,
                                     const std::string& device_id_to) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_INFO("Implementation does not support user consents.");
}

CacheManager::BackgroundBackuper::BackgroundBackuper(
    CacheManager* cache_manager)
    : cache_manager_(cache_manager)
    , stop_flag_(false)
    , new_data_available_(false) {
  SDL_LOG_AUTO_TRACE();
}

CacheManager::BackgroundBackuper::~BackgroundBackuper() {
  SDL_LOG_AUTO_TRACE();
}

void CacheManager::BackgroundBackuper::InternalBackup() {
  SDL_LOG_AUTO_TRACE();
  DCHECK(cache_manager_);

  while (new_data_available_) {
    new_data_available_ = false;
    SDL_LOG_DEBUG("DoBackup");
    cache_manager_->PersistData();
  }
}

void CacheManager::BackgroundBackuper::threadMain() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(need_backup_lock_);
  while (!stop_flag_) {
    {
      sync_primitives::AutoUnlock unlock(need_backup_lock_);
      InternalBackup();
    }
    if (new_data_available_ || stop_flag_) {
      continue;
    }
    SDL_LOG_DEBUG("Wait for a next backup");
    backup_notifier_.Wait(need_backup_lock_);
  }
}

void CacheManager::BackgroundBackuper::exitThreadMain() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(need_backup_lock_);
  stop_flag_ = true;
  backup_notifier_.NotifyOne();
}

void CacheManager::BackgroundBackuper::DoBackup() {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(need_backup_lock_);
  new_data_available_ = true;
  backup_notifier_.NotifyOne();
}

EncryptionRequired CacheManager::GetAppEncryptionRequiredFlag(
    const std::string& application) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(cache_lock_);

  return pt_->policy_table.app_policies_section.apps[application]
      .encryption_required;
}

EncryptionRequired CacheManager::GetFunctionalGroupingEncryptionRequiredFlag(
    const std::string& functional_group) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(cache_lock_);

  const auto& functional_groupings = pt_->policy_table.functional_groupings;

  const auto& grouping_itr = functional_groupings.find(functional_group);
  if (grouping_itr == functional_groupings.end()) {
    SDL_LOG_WARN("Group " << functional_group << " not found");
    return EncryptionRequired(rpc::Boolean(false));
  }

  return (*grouping_itr).second.encryption_required;
}

void CacheManager::GetApplicationParams(
    const std::string& application_name,
    policy_table::ApplicationParams& application_params) const {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock auto_lock(cache_lock_);

  const auto apps = pt_->policy_table.app_policies_section.apps;
  const auto it = apps.find(application_name);
  if (apps.end() == it) {
    SDL_LOG_WARN("Application " << application_name << " was not found");
    return;
  }

  application_params = (*it).second;
}

}  // namespace policy

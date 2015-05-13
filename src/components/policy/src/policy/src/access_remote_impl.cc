/*
 * Copyright (c) 2015, Ford Motor Company
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
#include "policy/access_remote_impl.h"

#include <algorithm>
#include <iterator>

#include "policy/cache_manager.h"
#include "utils/logger.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "PolicyManagerImpl")

using policy_table::DeviceData;
using policy_table::FunctionalGroupings;
using rpc::policy_table_interface_base::EnumFromJsonString;

namespace policy {

struct IsPrimary {
  bool operator ()(const DeviceData::value_type& item) const {
    return *item.second.primary == true;
  }
};

struct SetPrimary {
 private:
  bool value_;
 public:
  explicit SetPrimary(bool value)
      : value_(value) {
  }
  void operator ()(DeviceData::value_type& item) const {
    *item.second.primary = value_;
  }
};

struct Erase {
 private:
  const Subject& who_;
 public:
  explicit Erase(const Subject& who)
      : who_(who) {
  }
  void operator ()(AccessRemoteImpl::AccessControlList::value_type& row) const {
    row.second.erase(who_);
  }
};

struct IsTypeAccess {
 private:
  const TypeAccess& type_;
 public:
  explicit IsTypeAccess(const TypeAccess& type)
      : type_(type) {
  }
  bool operator ()(
      const AccessRemoteImpl::AccessControlRow::value_type& item) const {
    return item.second == type_;
  }
};

Match::Match(const FunctionalGroupings& groups, const PTString& rpc,
             const RemoteControlParams& params)
    : groups_(groups),
      rpc_(rpc),
      params_(params) {
}

bool Match::operator ()(const PTString& item) const {
  const FunctionalGroupings::const_iterator i = groups_.find(item);
  if (i == groups_.end()) {
    return false;
  }
  const policy_table::Rpc::const_iterator j = i->second.rpcs.find(rpc_);
  if (j == i->second.rpcs.end()) {
    return false;
  }
  const policy_table::Parameters& params = *j->second.parameters;
  if (params_.size() != params.size()) {
    return false;
  }
  for (RemoteControlParams::const_iterator j = params_.begin();
      j != params_.end(); ++j) {
    policy_table::Parameter value;
    if (!EnumFromJsonString(*j, &value)) {
      return false;
    }
    if (std::find(params.begin(), params.end(), value) == params.end()) {
      return false;
    }
  }
  return true;
}

struct ToHMIType {
  policy_table::AppHMITypes::value_type operator ()(int item) const {
    policy_table::AppHMIType type = static_cast<policy_table::AppHMIType>(item);
    if (!IsValidEnum(type)) {
      LOG4CXX_WARN(logger_, "HMI types isn't known " << item);
      type = policy_table::AHT_DEFAULT;
    }
    return policy_table::AppHMITypes::value_type(type);
  }
};

AccessRemoteImpl::AccessRemoteImpl()
    : cache_(new CacheManager()),
      primary_device_(),
      enabled_(true),
      country_consent_(true),
      acl_() {
}

AccessRemoteImpl::AccessRemoteImpl(utils::SharedPtr<CacheManager> cache)
    : cache_(cache),
      primary_device_(),
      enabled_(true),
      country_consent_(true),
      acl_() {
}

void AccessRemoteImpl::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(cache_->pt_);

  policy_table::ModuleConfig& config = cache_->pt_->policy_table.module_config;
  country_consent_ = !config.country_consent_passengersRC.is_initialized()
      || *config.country_consent_passengersRC;
  bool enabled = !config.user_consent_passengerRC.is_initialized()
      || *config.user_consent_passengerRC;
  set_enabled(enabled);

  const DeviceData& devices = *cache_->pt_->policy_table.device_data;
  DeviceData::const_iterator d = std::find_if(devices.begin(), devices.end(),
                                              IsPrimary());
  if (d != devices.end()) {
    LOG4CXX_TRACE(logger_, "Primary device is set");
    primary_device_ = d->first;
  }
}

bool AccessRemoteImpl::IsPrimaryDevice(const PTString& dev_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return primary_device_ == dev_id;
}

bool AccessRemoteImpl::IsPassengerZone(const SeatLocation& seat,
                                       const SeatLocation& zone) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return seat == zone;
}

TypeAccess AccessRemoteImpl::Check(const Subject& who,
                                   const Object& what) const {
  LOG4CXX_AUTO_TRACE(logger_);
  TypeAccess ret = TypeAccess::kDisallowed;
  AccessControlList::const_iterator i = acl_.find(what);
  if (i != acl_.end()) {
    const AccessControlRow& row = i->second;
    AccessControlRow::const_iterator j = row.find(who);
    if (j != row.end()) {
      // who has permissions
      LOG4CXX_TRACE(
          logger_,
          "Subject " << who << " has permissions " << ret << " to object " << what);
      ret = j->second;
    } else {
      // Look for somebody is who controls this object
      j = std::find_if(row.begin(), row.end(),
                       IsTypeAccess(TypeAccess::kAllowed));
      if (j != row.end()) {
        // Someone controls this object
        LOG4CXX_TRACE(logger_, "Someone controls " << what);
        ret = TypeAccess::kDisallowed;
      } else {
        // Nobody controls this object
        LOG4CXX_TRACE(logger_, "Nobody controls " << what);
        ret = TypeAccess::kManual;
      }
    }
  } else {
    // Nobody controls this object
    LOG4CXX_TRACE(logger_, "Nobody controls " << what);
    ret = TypeAccess::kManual;
  }
  return ret;
}

void AccessRemoteImpl::Allow(const Subject& who, const Object& what) {
  LOG4CXX_AUTO_TRACE(logger_);
  acl_[what][who] = TypeAccess::kAllowed;
}

void AccessRemoteImpl::Deny(const Subject& who, const Object& what) {
  LOG4CXX_AUTO_TRACE(logger_);
  acl_[what][who] = TypeAccess::kDisallowed;
}

void AccessRemoteImpl::Reset(const Subject& who) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::for_each(acl_.begin(), acl_.end(), Erase(who));
}

void AccessRemoteImpl::Reset(const Object& what) {
  LOG4CXX_AUTO_TRACE(logger_);
  acl_.erase(what);
}

void AccessRemoteImpl::SetPrimaryDevice(const PTString& dev_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  primary_device_ = dev_id;
  DeviceData& devices = *cache_->pt_->policy_table.device_data;
  std::for_each(devices.begin(), devices.end(), SetPrimary(false));
  *devices[dev_id].primary = true;
  cache_->Backup();
}

void AccessRemoteImpl::Enable() {
  LOG4CXX_AUTO_TRACE(logger_);
  set_enabled(true);
}

void AccessRemoteImpl::Disable() {
  LOG4CXX_AUTO_TRACE(logger_);
  set_enabled(false);
}

void AccessRemoteImpl::set_enabled(bool value) {
  enabled_ = country_consent_ && value;
  *cache_->pt_->policy_table.module_config.user_consent_passengerRC = value;
  cache_->Backup();
}

bool AccessRemoteImpl::IsEnabled() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return enabled_;
}

PTString AccessRemoteImpl::FindGroup(const Subject& who, const PTString& rpc,
                                     const RemoteControlParams& params) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!cache_->IsApplicationRepresented(who.app_id)) {
    return "";
  }

  const policy_table::Strings& groups =
      *cache_->pt_->policy_table.app_policies[who.app_id].groups_non_primaryRC;
  const FunctionalGroupings& all_groups = cache_->pt_->policy_table
      .functional_groupings;
  policy_table::Strings::const_iterator i = std::find_if(
      groups.begin(), groups.end(), Match(all_groups, rpc, params));

  if (i != groups.end()) {
    return *i;
  }
  return "";
}

void AccessRemoteImpl::SetDefaultHmiTypes(const std::string& app_id,
                                          const std::vector<int>& hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  HMIList::mapped_type types;
  std::transform(hmi_types.begin(), hmi_types.end(), std::back_inserter(types),
                 ToHMIType());
  std::vector<int>::const_iterator i;
  HMIList::mapped_type::const_iterator j;
  for (i = hmi_types.begin(), j = types.begin();
      i != hmi_types.end(), j != types.end(); ++i, ++j) {
    LOG4CXX_DEBUG(logger_, "DEFHMI: " << *i << " - " << *j);
  }
  hmi_types_[app_id] = types;
}

const policy_table::AppHMITypes& AccessRemoteImpl::HmiTypes(
    const std::string& app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (cache_->IsDefaultPolicy(app_id)) {
    return hmi_types_[app_id];
  } else {
    return *cache_->pt_->policy_table.app_policies[app_id].AppHMIType;
  }
}

const policy_table::Strings& AccessRemoteImpl::GetGroups(
    const PTString& device_id, const PTString& app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  const policy_table::AppHMITypes& hmi_types = HmiTypes(app_id);
  bool reverse = std::find(hmi_types.begin(), hmi_types.end(),
                           policy_table::AHT_REMOTE_CONTROL) != hmi_types.end();

  if (reverse) {
    return
        IsPrimaryDevice(device_id) ?
            *cache_->pt_->policy_table.app_policies[app_id].groups_primaryRC :
            *cache_->pt_->policy_table.app_policies[app_id].groups_non_primaryRC;
  }
  return cache_->GetGroups(app_id);
}

}
// namespace policy

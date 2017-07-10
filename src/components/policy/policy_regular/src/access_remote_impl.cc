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

struct Erase {
 private:
  const Subject& who_;

 public:
  explicit Erase(const Subject& who) : who_(who) {}
  void operator()(AccessRemoteImpl::AccessControlList::value_type& row) const {
    row.second.erase(who_);
  }
};

struct IsTypeAccess {
 private:
  const TypeAccess& type_;

 public:
  explicit IsTypeAccess(const TypeAccess& type) : type_(type) {}
  bool operator()(
      const AccessRemoteImpl::AccessControlRow::value_type& item) const {
    return item.second == type_;
  }
};

struct ToHMIType {
  policy_table::AppHMITypes::value_type operator()(int item) const {
    policy_table::AppHMIType type = static_cast<policy_table::AppHMIType>(item);
    if (!IsValidEnum(type)) {
      LOG4CXX_WARN(logger_, "HMI type isn't known " << item);
      type = policy_table::AHT_DEFAULT;
    }
    LOG4CXX_DEBUG(logger_,
                  "HMI type: " << item << " - " << EnumToJsonString(type));
    return policy_table::AppHMITypes::value_type(type);
  }
};

struct Contained {
 private:
  const policy_table::Strings& params_;

 public:
  explicit Contained(const policy_table::Strings& params) : params_(params) {}
  bool operator()(const RemoteControlParams::value_type& item) const {
    return std::find_if(params_.begin(), params_.end(), CompareString(item)) !=
           params_.end();
  }
  struct CompareString {
   private:
    const RemoteControlParams::value_type& value_;

   public:
    explicit CompareString(const RemoteControlParams::value_type& value)
        : value_(value) {}
    bool operator()(const policy_table::Strings::value_type& item) const {
      return value_ == static_cast<std::string>(item);
    }
  };
};

struct ToModuleType {
  std::string operator()(policy_table::ModuleTypes::value_type item) const {
    policy_table::ModuleType type = static_cast<policy_table::ModuleType>(item);
    return EnumToJsonString(type);
  }
};

AccessRemoteImpl::AccessRemoteImpl()
    : cache_(new CacheManager()), primary_device_(), enabled_(true), acl_() {}

AccessRemoteImpl::AccessRemoteImpl(utils::SharedPtr<CacheManager> cache)
    : cache_(cache), primary_device_(), enabled_(true), acl_() {}

void AccessRemoteImpl::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(cache_->pt());

  policy_table::ModuleConfig& config = cache_->pt()->policy_table.module_config;
  enabled_ = country_consent() &&
             (!config.user_consent_passengersRC.is_initialized() ||
              *config.user_consent_passengersRC);
}

bool AccessRemoteImpl::IsPrimaryDevice(const PTString& dev_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return primary_device_ == dev_id;
}

TypeAccess AccessRemoteImpl::Check(const Subject& who,
                                   const Object& what) const {
  LOG4CXX_AUTO_TRACE(logger_);
  AccessControlList::const_iterator i = acl_.find(what);
  if (i != acl_.end()) {
    const AccessControlRow& row = i->second;
    AccessControlRow::const_iterator j = row.find(who);
    if (j != row.end()) {
      // who has permissions
      TypeAccess ret = j->second;
      LOG4CXX_TRACE(logger_,
                    "Subject " << who << " has permissions " << ret
                               << " to object " << what);
      return ret;
    }
  }
  return TypeAccess::kManual;
}

bool AccessRemoteImpl::CheckModuleType(const PTString& app_id,
                                       policy_table::ModuleType module) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!cache_->IsApplicationRepresented(app_id)) {
    return false;
  }

  const policy_table::ApplicationParams& app =
      cache_->pt()->policy_table.app_policies_section.apps[app_id];
  if (!app.moduleType.is_initialized()) {
    return false;
  }

  const policy_table::ModuleTypes& modules = *app.moduleType;
  if (modules.empty()) {
    return true;
  }

  return std::find(modules.begin(), modules.end(), module) != modules.end();
}

bool AccessRemoteImpl::IsAllowed(const policy_table::AccessModules& modules,
                                 const std::string& module_name,
                                 const std::string& rpc_name,
                                 RemoteControlParams* input) const {
  LOG4CXX_AUTO_TRACE(logger_);
  policy_table::AccessModules::const_iterator i = modules.find(module_name);
  if (i == modules.end()) {
    LOG4CXX_DEBUG(logger_, "Module " << module_name << " wasn't found");
    return false;
  }

  const policy_table::RemoteRpcs& rpcs = i->second;
  if (rpcs.empty()) {
    return true;
  }
  policy_table::RemoteRpcs::const_iterator j = rpcs.find(rpc_name);
  if (j != rpcs.end()) {
    const policy_table::Strings& parameters = j->second;
    return CompareParameters(parameters, input);
  }
  LOG4CXX_DEBUG(logger_, "RPC " << rpc_name << " wasn't found");
  return false;
}

bool AccessRemoteImpl::CompareParameters(
    const policy_table::Strings& parameters, RemoteControlParams* input) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (parameters.empty()) {
    return true;
  }

  if (input->empty()) {
    LOG4CXX_DEBUG(logger_, "Input is empty");
    return false;
  }

  input->erase(
      std::remove_if(input->begin(), input->end(), Contained(parameters)),
      input->end());
  return input->empty();
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

void AccessRemoteImpl::Reset() {
  acl_.clear();
}

void AccessRemoteImpl::SetPrimaryDevice(const PTString& dev_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  primary_device_ = dev_id;
}

PTString AccessRemoteImpl::PrimaryDevice() const {
  return primary_device_;
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
  enabled_ = country_consent() && value;
  *cache_->pt()->policy_table.module_config.user_consent_passengersRC = value;
  cache_->Backup();
}

bool AccessRemoteImpl::country_consent() const {
  policy_table::ModuleConfig& config = cache_->pt()->policy_table.module_config;
  return !config.country_consent_passengersRC.is_initialized() ||
         *config.country_consent_passengersRC;
}

bool AccessRemoteImpl::IsEnabled() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return enabled_;
}

void AccessRemoteImpl::SetDefaultHmiTypes(const Subject& who,
                                          const std::vector<int>& hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  HMIList::mapped_type types;
  std::transform(hmi_types.begin(),
                 hmi_types.end(),
                 std::back_inserter(types),
                 ToHMIType());
  hmi_types_[who] = types;
}

const policy_table::AppHMITypes& AccessRemoteImpl::HmiTypes(
    const Subject& who) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (cache_->IsDefaultPolicy(who.app_id)) {
    return hmi_types_[who];
  } else {
    return *cache_->pt()
                ->policy_table.app_policies_section.apps[who.app_id]
                .AppHMIType;
  }
}

const policy_table::Strings& AccessRemoteImpl::GetGroups(const Subject& who) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (IsAppReverse(who)) {
    if (IsPrimaryDevice(who.dev_id)) {
      return *cache_->pt()
                  ->policy_table.app_policies_section.apps[who.app_id]
                  .groups_primaryRC;
    } else if (IsEnabled()) {
      return *cache_->pt()
                  ->policy_table.app_policies_section.apps[who.app_id]
                  .groups_nonPrimaryRC;
    } else {
      return cache_->GetGroups(kPreConsentPassengersRC);
    }
  }
  return cache_->GetGroups(who.app_id);
}

bool AccessRemoteImpl::IsAppReverse(const Subject& who) {
  LOG4CXX_AUTO_TRACE(logger_);
  const policy_table::AppHMITypes& hmi_types = HmiTypes(who);
  return std::find(hmi_types.begin(),
                   hmi_types.end(),
                   policy_table::AHT_REMOTE_CONTROL) != hmi_types.end();
}

bool AccessRemoteImpl::GetPermissionsForApp(const std::string& device_id,
                                            const std::string& app_id,
                                            FunctionalIdType& group_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  GetGroupsIds(device_id, app_id, group_types[kTypeGeneral]);
  GetGroupsIds(device_id, kDefaultId, group_types[kTypeDefault]);
  GetGroupsIds(
      device_id, kPreDataConsentId, group_types[kTypePreDataConsented]);
  return true;
}

std::ostream& operator<<(std::ostream& output,
                         const FunctionalGroupIDs& types) {
  std::copy(types.begin(),
            types.end(),
            std::ostream_iterator<FunctionalGroupIDs::value_type>(output, " "));
  return output;
}

void AccessRemoteImpl::GetGroupsIds(const std::string& device_id,
                                    const std::string& app_id,
                                    FunctionalGroupIDs& groups_ids) {
  Subject who = {device_id, app_id};
  const policy_table::Strings& groups = GetGroups(who);
  groups_ids.resize(groups.size());
  std::transform(groups.begin(),
                 groups.end(),
                 groups_ids.begin(),
                 &CacheManager::GenerateHash);
  LOG4CXX_DEBUG(logger_, "Groups Ids: " << groups_ids);
}

bool AccessRemoteImpl::GetModuleTypes(const std::string& application_id,
                                      std::vector<std::string>* modules) {
  DCHECK(modules);
  policy_table::ApplicationPolicies& apps =
      cache_->pt()->policy_table.app_policies_section.apps;
  policy_table::ApplicationPolicies::iterator i = apps.find(application_id);
  if (i == apps.end()) {
    return false;
  }
  rpc::Optional<policy_table::ModuleTypes> moduleTypes = i->second.moduleType;
  if (!moduleTypes.is_initialized()) {
    return false;
  }
  std::transform(moduleTypes->begin(),
                 moduleTypes->end(),
                 std::back_inserter(*modules),
                 ToModuleType());
  return true;
}

}  // namespace policy

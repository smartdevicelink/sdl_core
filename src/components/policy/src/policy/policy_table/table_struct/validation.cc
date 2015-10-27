#include <algorithm>
#include "./types.h"

namespace rpc {
namespace policy_table_interface_base {
bool ApplicationParams::Validate() const {
  return ValidateModuleTypes();
}
bool ApplicationParams::ValidateModuleTypes() const {
  // moduleType is optional so see Optional<T>::is_valid()
  bool is_initialized = moduleType->is_initialized();
  if (!is_initialized) {
    // valid if not initialized
    return true;
  }
  bool is_valid = moduleType->is_valid();
  if (is_valid) {
    return true;
  }

  struct IsInvalid {
    bool operator()(Enum<ModuleType> item) const {
      return !item.is_valid();
    }
  };
  // cut invalid items
  moduleType->erase(std::remove_if(moduleType->begin(), moduleType->end(),
                                   IsInvalid()),
              moduleType->end());
  bool empty = moduleType->empty();
  if (empty) {
    // set non initialized value
    ModuleTypes non_initialized;
    moduleType = Optional<ModuleTypes>(non_initialized);
  }
  return true;
}

bool RpcParameters::Validate() const {
  return true;
}
bool Rpcs::Validate() const {
  return true;
}

bool ModuleConfig::Validate() const {

  if (PT_PRELOADED == GetPolicyTableType()) {

    if (vehicle_make.is_initialized()) {
      return false;
    }
    if (vehicle_year.is_initialized()) {
      return false;
    }
    if (vehicle_model.is_initialized()) {
      return false;
    }
  }
  return true;
}

bool MessageString::Validate() const {
  return true;
}

bool MessageLanguages::Validate() const {
  if (PT_SNAPSHOT == GetPolicyTableType()) {
    return false;
  }
  return true;
}

bool ConsumerFriendlyMessages::Validate() const {
  if (PT_SNAPSHOT == GetPolicyTableType()) {
    return false;
  }
  return true;
}

bool ModuleMeta::Validate() const {
  return true;
}

bool AppLevel::Validate() const {
  if (PT_PRELOADED == GetPolicyTableType() ||
      PT_UPDATE == GetPolicyTableType()) {
    return false;
  }
  return true;
}

bool UsageAndErrorCounts::Validate() const {
  if (PT_PRELOADED == GetPolicyTableType() ||
      PT_UPDATE == GetPolicyTableType()) {
   return false;
  }
  return true;
}

bool DeviceParams::Validate() const {
  return true;
}
bool PolicyTable::Validate() const {
  for (ApplicationPolicies::const_iterator it = app_policies.begin();
       app_policies.end() != it; ++it) {
    if (kDeviceApp == it->first) {
      if (it->second.nicknames.is_initialized()) {
        initialization_state__ = kUninitialized;
        return false;
      }
      continue;
    }
  }
  if (PT_PRELOADED == GetPolicyTableType() ||
      PT_UPDATE == GetPolicyTableType()) {
    if (device_data.is_initialized()) {
      return false;
    }
  }
  return true;
}
bool Table::Validate() const {
  return true;
}

bool InteriorZone::Validate() const {
  for (AccessModules::const_iterator i = auto_allow.begin();
      i != auto_allow.end(); ++i) {
    ModuleType module;
    if (!EnumFromJsonString(i->first, &module)) {
      return false;
    }
  }
  for (AccessModules::const_iterator i = driver_allow.begin();
      i != driver_allow.end(); ++i) {
    ModuleType module;
    if (!EnumFromJsonString(i->first, &module)) {
      return false;
    }
  }
  return true;
}

bool Equipment::Validate() const {
  return true;
}
}  // namespace policy_table_interface_base
}  // namespace rpc


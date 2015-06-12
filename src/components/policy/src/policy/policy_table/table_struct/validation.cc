// This file is generated, do not edit
#include "./types.h"

namespace rpc {
namespace policy_table_interface_base {
bool PolicyBase::Validate() const {
  return true;
}
bool ApplicationPoliciesSection::Validate() const {
  return true;
}
bool ApplicationParams::Validate() const {
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
}  // namespace policy_table_interface_base
}  // namespace rpc


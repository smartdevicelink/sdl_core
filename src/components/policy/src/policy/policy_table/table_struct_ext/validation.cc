// This file is generated, do not edit
#include "./types.h"

namespace rpc {
namespace policy_table_interface_base {
bool ApplicationParams::Validate() const {
  if (is_initialized()) {
    if (preconsented_groups.is_initialized()) {
      const Strings& all = groups;
      const Strings& preconsented = *preconsented_groups;
      if (preconsented.size() > all.size()) {
        return false;
      }
    }
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
  return true;
}
bool ModuleMeta::Validate() const {
  if (GetPolicyTableType() == PT_UPDATE ||
      GetPolicyTableType() == PT_PRELOADED) {
    return false;
  }
  return true;
}
bool AppLevel::Validate() const {
  if (PT_PRELOADED == GetPolicyTableType() ||
      PT_SNAPSHOT == GetPolicyTableType()) {
    return false;
  }
  return true;
}
bool UsageAndErrorCounts::Validate() const {
  if (PT_PRELOADED == GetPolicyTableType() ||
      PT_SNAPSHOT == GetPolicyTableType()) {
   return false;
  }
  return true;
}
bool ConsentRecords::Validate() const {
  return true;
}
bool DeviceParams::Validate() const {
  return true;
}
bool PolicyTable::Validate() const {
  for (ApplicationPolicies::const_iterator it = app_policies.begin();
       app_policies.end() != it; ++it) {
    // TODO(AOleynik): checks below are disabled until clarification with
    // customer, see APPLINK-8149
    if (kDeviceApp == it->first) {
      if (/*it->second.AppHMIType.is_initialized()
          || it->second.memory_kb.is_initialized()
          || it->second.heart_beat_timeout_ms.is_initialized()
          ||*/ it->second.nicknames.is_initialized()) {
        initialization_state__ = kUninitialized;
        return false;
      }
      continue;
    }
  }
  if (PT_PRELOADED == GetPolicyTableType() ||
      PT_SNAPSHOT == GetPolicyTableType()) {
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


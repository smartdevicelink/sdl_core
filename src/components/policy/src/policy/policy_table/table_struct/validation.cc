#include <algorithm>
#include <cctype>
#include <cstdlib>
#include <locale>
#include "./types.h"
#include "utils/macro.h"

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

bool InteriorZone::ValidateParameters(ModuleType module,
                                      const Strings& parameters) const {
  const std::string *begin = 0;
  const std::string *end = 0;
  switch (module) {
    case MT_RADIO:
      begin = kRadioParameters;
      end = kRadioParameters + length_radio;
      break;
    case MT_CLIMATE:
      begin = kClimateParameters;
      end = kClimateParameters + length_climate;
      break;
  }
  DCHECK(begin);
  DCHECK(end);
  for (Strings::const_iterator i = parameters.begin();
      i != parameters.end(); ++i) {
    std::string name = *i;
    bool found = std::find(begin, end, name) != end;
    if (!found) {
      return false;
    }
  }
  return true;
}

bool InteriorZone::ValidateRemoteRpcs(ModuleType module,
                                      const RemoteRpcs& rpcs) const {
  for (RemoteRpcs::const_iterator i = rpcs.begin();
      i != rpcs.end(); ++i) {
    const std::string& name = i->first;
    const Strings& parameters = i->second;
    const std::string *begin = kRemoteRpcs;
    const std::string *end = kRemoteRpcs + length;
    bool found = std::find(begin, end, name) != end;
    if (!found || !ValidateParameters(module, parameters)) {
      return false;
    }
  }
  return true;
}

bool InteriorZone::ValidateAllow(const AccessModules& modules) const {
  for (AccessModules::const_iterator i = modules.begin();
      i != modules.end(); ++i) {
    const std::string& name = i->first;
    const RemoteRpcs& rpcs = i->second;
    ModuleType module;
    if (!EnumFromJsonString(name, &module)
        || !ValidateRemoteRpcs(module, rpcs)) {
      return false;
    }
  }
  return true;
}

bool InteriorZone::Validate() const {
  return ValidateAllow(auto_allow) && ValidateAllow(driver_allow);
}

namespace {
struct IsDeniedChar {
  bool operator() (wchar_t c) {
    return c != '_' && !std::isalnum(c, std::locale(""));
  }
};
}  // namespace

bool Equipment::ValidateNameZone(const std::string& name) const {
  if (name.empty()) {
    return false;
  }
  std::vector<wchar_t> wchars(name.length() + 1, L'\0');
  std::string current_locale = setlocale(LC_ALL, NULL);
  setlocale(LC_ALL, "");
  int n = mbstowcs(&(wchars.front()), name.c_str(), name.length());
  setlocale(LC_ALL, current_locale.c_str());
  if (n != -1) {
    std::vector<wchar_t>::iterator real_end = wchars.begin() + n;
    return std::find_if(wchars.begin(), real_end, IsDeniedChar()) == real_end;
  }
  return false;
}

bool Equipment::Validate() const {
  for (Zones::const_iterator i = zones.begin(); i != zones.end(); ++i) {
    if (!ValidateNameZone(i->first)) {
      return false;
    }
  }
  return true;
}
}  // namespace policy_table_interface_base
}  // namespace rpc


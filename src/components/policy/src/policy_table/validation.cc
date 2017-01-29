#include <algorithm>
#include "policy/policy_table/types.h"
#include "utils/logger.h"

namespace {
bool IsTypeInvalid(
    rpc::Enum<rpc::policy_table_interface_base::RequestType> request) {
  return !request.is_valid();
}
}

namespace rpc {
namespace policy_table_interface_base {

CREATE_LOGGERPTR_GLOBAL(logger_, "Policy")

void RemoveInvalidTypes(RequestTypes& types) {
  types.erase(std::remove_if(types.begin(), types.end(), &IsTypeInvalid),
              types.end());
}

bool PolicyBase::Validate() const {
  return true;
}

bool ApplicationPoliciesSection::Validate() const {
  ApplicationPolicies::iterator it_default_policy = apps.find(kDefaultApp);
  ApplicationPolicies::iterator it_pre_data_policy =
      apps.find(kPreDataConsentApp);

  // Default and PreData policies are mandatory
  if (apps.end() == it_default_policy || apps.end() == it_pre_data_policy) {
    LOG4CXX_ERROR(logger_, "Default or preData policy is not present.");
    return false;
  }

  // Device policy is mandatory
  if (!device.is_initialized()) {
    LOG4CXX_ERROR(logger_, "Device policy is not present.");
    return false;
  }

  PolicyTableType pt_type = GetPolicyTableType();
  if (PT_PRELOADED != pt_type && PT_UPDATE != pt_type) {
    return true;
  }

  if (!it_default_policy->second.RequestType.is_valid()) {
    LOG4CXX_WARN(logger_,
                 "Default policy RequestTypes are not valid. Will be cleaned.");
    RemoveInvalidTypes(*it_default_policy->second.RequestType);
    // If preloaded does not have valid default types - validation fails
    // Otherwise default will be empty, i.e. all types allowed
    if (PT_PRELOADED == pt_type) {
      if (it_default_policy->second.RequestType->empty()) {
        LOG4CXX_ERROR(
            logger_,
            "Default policy RequestTypes empty after clean-up. Exiting.");
        return false;
      }
    }
  }

  ApplicationPolicies::iterator iter = apps.begin();
  ApplicationPolicies::iterator end_iter = apps.end();

  while (iter != end_iter) {
    ApplicationParams& app_params = (*iter).second;
    bool is_request_type_ommited = !app_params.RequestType.is_initialized();
    bool is_request_type_valid = app_params.RequestType.is_valid();
    bool is_request_type_empty = app_params.RequestType->empty();

    if (PT_PRELOADED == pt_type) {
      if (!is_request_type_valid) {
        LOG4CXX_WARN(logger_,
                     "App policy RequestTypes are not valid. Will be cleaned.");
        RemoveInvalidTypes(*app_params.RequestType);
        if (app_params.RequestType->empty()) {
          LOG4CXX_ERROR(
              logger_,
              "App policy RequestTypes empty after clean-up. Exiting.");
          return false;
        }
      }
    } else {
      if (is_request_type_ommited) {
        LOG4CXX_WARN(logger_,
                     "App policy RequestTypes ommited."
                     " Will be replaced with default.");
        app_params.RequestType = apps[kDefaultApp].RequestType;
        ++iter;
        continue;
      }
      if (!is_request_type_valid) {
        LOG4CXX_WARN(logger_,
                     "App policy RequestTypes are invalid. Will be cleaned.");
        RemoveInvalidTypes(*app_params.RequestType);
        if (app_params.RequestType->empty()) {
          LOG4CXX_WARN(logger_,
                       "App policy RequestTypes empty after clean-up."
                       " Will be replaced with default.");
          app_params.RequestType = apps[kDefaultApp].RequestType;
          ++iter;
          continue;
        }
      }
      if (is_request_type_empty) {
        LOG4CXX_WARN(logger_, "App policy RequestTypes empty.");
      }
    }
    ++iter;
  }

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
  /* According to requirements consumer_friendly_messages are optional for PTU
     and required for PTP and PTS. So, they are allowed always */
  if (PT_SNAPSHOT == GetPolicyTableType() && messages.is_initialized()) {
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

#ifdef SDL_REMOTE_CONTROL

bool InteriorZone::ValidateParameters(ModuleType module,
                                      const Strings& parameters) const {
  const std::string* begin = 0;
  const std::string* end = 0;
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
  for (Strings::const_iterator i = parameters.begin(); i != parameters.end();
       ++i) {
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
  for (RemoteRpcs::const_iterator i = rpcs.begin(); i != rpcs.end(); ++i) {
    const std::string& name = i->first;
    const Strings& parameters = i->second;
    const std::string* begin = kRemoteRpcs;
    const std::string* end = kRemoteRpcs + length;
    bool found = std::find(begin, end, name) != end;
    if (!found || !ValidateParameters(module, parameters)) {
      return false;
    }
  }
  return true;
}

bool InteriorZone::ValidateAllow(const AccessModules& modules) const {
  for (AccessModules::const_iterator i = modules.begin(); i != modules.end();
       ++i) {
    const std::string& name = i->first;
    const RemoteRpcs& rpcs = i->second;
    ModuleType module;
    if (!EnumFromJsonString(name, &module) ||
        !ValidateRemoteRpcs(module, rpcs)) {
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
  bool operator()(wchar_t c) {
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
#endif
}  // namespace policy_table_interface_base
}  // namespace rpc

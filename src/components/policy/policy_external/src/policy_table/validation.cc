#include <iostream>
#include <algorithm>
#include "policy/policy_table/types.h"
#include "utils/logger.h"
#include "utils/helpers.h"

namespace {
bool IsPredefinedApplication(const std::string& app_id) {
  using namespace rpc::policy_table_interface_base;
  return kPreDataConsentApp == app_id || kDefaultApp == app_id;
}
}

namespace rpc {
namespace policy_table_interface_base {

CREATE_LOGGERPTR_GLOBAL(logger_, "Policy")

bool VerifyPredefinedApp(ApplicationPolicies::value_type& app_policies) {
  const std::string& app_id = app_policies.first;
  if (!IsPredefinedApplication(app_id)) {
    return true;
  }

  RequestTypes& predefined_request_types = *app_policies.second.RequestType;

  if (!predefined_request_types.is_valid()) {
    LOG4CXX_WARN(logger_,
                 app_id << " policy invalid RequestTypes will be cleaned.");
    predefined_request_types.CleanUp();
    if (PT_PRELOADED == app_policies.second.GetPolicyTableType() &&
        predefined_request_types.is_cleaned_up()) {
      LOG4CXX_ERROR(
          logger_,
          app_id << " policy RequestTypes is empty after clean-up. Exiting.");
      return false;
    }

    LOG4CXX_WARN(logger_, app_id << " request types have cleaned up.");
  }
  return true;
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

  if (!VerifyPredefinedApp(*it_default_policy)) {
    return false;
  }

  if (!VerifyPredefinedApp(*it_pre_data_policy)) {
    return false;
  }

  ApplicationPolicies::iterator iter = apps.begin();
  ApplicationPolicies::iterator end_iter = apps.end();

  while (iter != end_iter) {
    const std::string app_id = iter->first;
    if (IsPredefinedApplication(app_id)) {
      ++iter;
      continue;
    }

    RequestTypes& app_request_types = *iter->second.RequestType;

    if (app_request_types.is_omitted()) {
      LOG4CXX_WARN(logger_,
                   "RequestTypes omitted for "
                       << app_id << " Will be replaced with default.");
      app_request_types = *apps[kDefaultApp].RequestType;
      ++iter;
      continue;
    }

    if (!app_request_types.is_valid()) {
      LOG4CXX_WARN(logger_,
                   "Invalid RequestTypes for " << app_id
                                               << " Will be cleaned up.");
      app_request_types.CleanUp();
      if (app_request_types.is_cleaned_up()) {
        if (PT_PRELOADED == pt_type) {
          LOG4CXX_ERROR(logger_,
                        "RequestTypes empty after clean-up for "
                            << app_id << " Exiting.");
          return false;
        }

        LOG4CXX_WARN(logger_,
                     "RequestTypes empty after clean-up for "
                         << app_id << " Will be replaced with default.");

        app_request_types = *apps[kDefaultApp].RequestType;
      }

      LOG4CXX_DEBUG(logger_, "Clean up for " << app_id << " is done.");

      ++iter;
      continue;
    }

    if (app_request_types.is_empty()) {
      LOG4CXX_WARN(logger_, "RequestTypes is empty for " << app_id);
    }

    ++iter;
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
  switch (GetPolicyTableType()) {
    case PT_PRELOADED: {
      if (helpers::Compare<bool, helpers::EQ, helpers::ONE>(
              true,
              vehicle_make.is_initialized(),
              vehicle_year.is_initialized(),
              vehicle_model.is_initialized())) {
        return false;
      }
      break;
    }
    case PT_UPDATE: {
      if (preloaded_pt->is_initialized()) {
        return false;
      }
      if (preloaded_date->is_initialized()) {
        return false;
      }
      break;
    }
    default:
      break;
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
bool ConsentRecords::Validate() const {
  if (PT_SNAPSHOT != GetPolicyTableType()) {
    return !external_consent_status_groups->is_initialized();
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
  moduleType->erase(
      std::remove_if(moduleType->begin(), moduleType->end(), IsInvalid()),
      moduleType->end());
  bool empty = moduleType->empty();
  if (empty) {
    // set non initialized value
    ModuleTypes non_initialized;
    moduleType = Optional<ModuleTypes>(non_initialized);
  }
  return true;
}
#endif  // SDL_REMOTE_CONTROL

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
#ifdef SDL_REMOTE_CONTROL
  return ValidateModuleTypes();
#else   // SDL_REMOTE_CONTROL
  return true;
#endif  // SDL_REMOTE_CONTROL
}

}  // namespace policy_table_interface_base
}  // namespace rpc

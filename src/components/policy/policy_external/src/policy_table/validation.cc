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
  // Check for empty "groups" sub-sections
  if (groups.empty()) {
    return false;
  }
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
  PolicyTableType policy_table_type = GetPolicyTableType();

  if (PT_PRELOADED == policy_table_type || PT_UPDATE == policy_table_type) {
    if (device_data.is_initialized()) {
      return false;
    }
  }

  if (PT_PRELOADED == policy_table_type || PT_SNAPSHOT == policy_table_type) {
    // Check upper bound of each "groups" sub section in the app policies
    const FunctionalGroupings::size_type functional_groupings_count =
        functional_groupings.size();
    for (ApplicationPolicies::const_iterator app_policiies_it =
             app_policies_section.apps.begin();
         app_policies_section.apps.end() != app_policiies_it;
         ++app_policiies_it) {
      if (app_policiies_it->second.groups.size() > functional_groupings_count) {
        return false;
      }
    }
  }

  return true;
}

bool Table::Validate() const {
  return true;
}
}  // namespace policy_table_interface_base
}  // namespace rpc

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
    bool is_request_type_omitted = !app_params.RequestType.is_initialized();
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
      if (is_request_type_omitted) {
        LOG4CXX_WARN(logger_,
                     "App policy RequestTypes omitted."
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
  // Check for empty "groups" sub-sections
  if (groups.empty()) {
    return false;
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

  for (ServiceEndpoints::const_iterator it_endpoints = endpoints.begin();
       it_endpoints != endpoints.end();
       ++it_endpoints) {
    const URLList& endpoint_list = it_endpoints->second;
    if (endpoint_list.end() == endpoint_list.find(kDefaultApp)) {
      LOG4CXX_ERROR(logger_,
                    "Endpoint " << it_endpoints->first
                                << "does not contain default group");
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
  const PolicyTableType policy_table_type = GetPolicyTableType();

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

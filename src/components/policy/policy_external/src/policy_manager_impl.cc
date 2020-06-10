/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */
#include "policy/policy_manager_impl.h"

#include <algorithm>
#include <functional>
#include <iterator>
#include <limits>
#include <queue>
#include <set>
#include <vector>
#include "config_profile/profile.h"
#include "json/writer.h"
#include "policy/cache_manager.h"
#include "policy/policy_helper.h"
#include "policy/policy_table.h"
#include "policy/pt_representation.h"
#include "policy/update_status_manager.h"
#include "utils/date_time.h"
#include "utils/file_system.h"
#include "utils/jsoncpp_reader_wrapper.h"
#include "utils/logger.h"

#include "policy/access_remote.h"
#include "policy/access_remote_impl.h"
#include "utils/timer_task_impl.h"

namespace {

/**
 * @brief Extracts group name from group permission structure
 */
struct GroupNamesAppender
    : public std::unary_function<void,
                                 const policy::FunctionalGroupPermission&> {
  GroupNamesAppender(policy_table::Strings& names) : names_(names) {}

  void operator()(const policy::FunctionalGroupPermission& value) {
    names_.push_back(value.group_name);
  }

 private:
  policy_table::Strings& names_;
};

/**
 * @brief Updates permission state of input group permission value in case
 * group name is found within allowed or disallowed groups lists considering
 * current priorities of consents
 * Also collects matched groups names to separate collection for futher
 * processing
 */
struct ConsentsUpdater
    : public std::unary_function<void, policy::FunctionalGroupPermission&> {
  ConsentsUpdater(const policy::GroupsNames& allowed,
                  const policy::GroupsNames& disallowed,
                  std::vector<policy::FunctionalGroupPermission>&
                      out_external_consent_matches,
                  const policy::ConsentPriorityType prio)
      : allowed_(allowed)
      , disallowed_(disallowed)
      , out_external_consent_matches_(out_external_consent_matches)
      , prio_(prio) {}

  void operator()(policy::FunctionalGroupPermission& value) {
    if (helpers::in_range(disallowed_, value.group_name)) {
      policy::FunctionalGroupPermission external_consent = value;
      external_consent.state = policy::kGroupDisallowed;
      out_external_consent_matches_.push_back(external_consent);

      if (IsAllowedToChangedUserConsent(value.state)) {
        value.state = policy::kGroupDisallowed;
      }
      return;
    }

    if (helpers::in_range(allowed_, value.group_name)) {
      policy::FunctionalGroupPermission external_consent = value;
      external_consent.state = policy::kGroupAllowed;
      out_external_consent_matches_.push_back(external_consent);

      if (IsAllowedToChangedUserConsent(value.state)) {
        value.state = policy::kGroupAllowed;
      }
    }
  }

 private:
  bool IsAllowedToChangedUserConsent(
      policy::GroupConsent current_consent) const {
    if (policy::GroupConsent::kGroupUndefined == current_consent) {
      return true;
    }

    return policy::ConsentPriorityType::kUserConsentPrio != prio_;
  }

  const policy::GroupsNames& allowed_;
  const policy::GroupsNames& disallowed_;
  std::vector<policy::FunctionalGroupPermission>& out_external_consent_matches_;
  const policy::ConsentPriorityType prio_;
};

/**
 * @brief Checks whether ExternalConsent entity status is the same as name of
 * group
 * container where entity has been found in. In case of match group is added to
 * 'disallowed' list, otherwise - to 'allowed' one.
 * E.g. if entity has "ON" status and is found in
 * 'disallowed_by_external_consent_entities_on' it will be added to
 * 'disallowed'. If it has
 * been found in 'disallowed_by_external_consent_entities_off' than group is
 * added to
 * 'allowed' list.
 */
struct GroupChecker
    : std::unary_function<
          void,
          policy::GroupsByExternalConsentStatus::mapped_type::value_type> {
  GroupChecker(const policy::EntityStatus entity_status,
               policy::GroupsNames& out_allowed,
               policy::GroupsNames& out_disallowed)
      : entity_status_(entity_status)
      , out_allowed_(out_allowed)
      , out_disallowed_(out_disallowed) {}

  void operator()(
      const policy::GroupsByExternalConsentStatus::mapped_type::value_type
          value) {
    using namespace policy;

    const std::string group_name = value.first;

    if ((value.second && (kStatusOn == entity_status_)) ||
        (!value.second && (kStatusOff == entity_status_))) {
      out_disallowed_.insert(group_name);
    } else {
      out_allowed_.insert(group_name);
    }
  }

 private:
  const policy::EntityStatus entity_status_;
  policy::GroupsNames& out_allowed_;
  policy::GroupsNames& out_disallowed_;
};

/**
 * @brief Sorts groups for 'allowed' and 'disallowed' by ExternalConsent
 * entities statuses.
 * Wraps GroupChecker logic.
 */
struct GroupSorter
    : std::unary_function<
          void,
          const policy::GroupsByExternalConsentStatus::value_type&> {
  GroupSorter(policy::GroupsNames& out_allowed,
              policy::GroupsNames& out_disallowed)
      : out_allowed_(out_allowed), out_disallowed_(out_disallowed) {}

  void operator()(
      const policy::GroupsByExternalConsentStatus::value_type& value) {
    GroupChecker checker(value.first.status_, out_allowed_, out_disallowed_);
    std::for_each(value.second.begin(), value.second.end(), checker);
  }

 private:
  policy::GroupsNames& out_allowed_;
  policy::GroupsNames& out_disallowed_;
};

}  // namespace

namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "Policy")

PolicyManagerImpl::PolicyManagerImpl()
    : PolicyManager()
    , listener_(NULL)
    , cache_(new CacheManager)
    , access_remote_(
          new AccessRemoteImpl(std::static_pointer_cast<CacheManager>(cache_)))
    , retry_sequence_timeout_(60)
    , retry_sequence_index_(0)
    , applications_pending_ptu_count_(0)
    , ignition_check(true)
    , retry_sequence_url_(0, 0, "")
    , ptu_requested_(false)
    , is_ptu_in_progress_(false) {}

PolicyManagerImpl::PolicyManagerImpl(bool in_memory)
    : PolicyManager()
    , listener_(NULL)
    , cache_(new CacheManager(in_memory))
    , access_remote_(
          new AccessRemoteImpl(std::static_pointer_cast<CacheManager>(cache_)))
    , retry_sequence_timeout_(60)
    , retry_sequence_index_(0)
    , applications_pending_ptu_count_(0)
    , ignition_check(true)
    , retry_sequence_url_(0, 0, "")
    , send_on_update_sent_out_(false)
    , trigger_ptu_(false)
    , ptu_requested_(false)
    , is_ptu_in_progress_(false) {}

void PolicyManagerImpl::set_listener(PolicyListener* listener) {
  listener_ = listener;
  update_status_manager_.set_listener(listener);
}

std::shared_ptr<policy_table::Table> PolicyManagerImpl::Parse(
    const BinaryMessage& pt_content) {
  std::string json(pt_content.begin(), pt_content.end());
  utils::JsonReader reader;
  Json::Value value;

  if (reader.parse(json, &value)) {
    return std::make_shared<policy_table::Table>(&value);
  } else {
    return std::make_shared<policy_table::Table>();
  }
}

void PolicyManagerImpl::CheckTriggers() {
  LOG4CXX_AUTO_TRACE(logger_);
  const bool exceed_ignition_cycles = ExceededIgnitionCycles();
  const bool exceed_days = ExceededDays();

  LOG4CXX_DEBUG(
      logger_,
      "\nDays exceeded: " << std::boolalpha << exceed_days
                          << "\nIgnition cycles exceeded: " << std::boolalpha
                          << exceed_ignition_cycles);

  if (exceed_ignition_cycles || exceed_days) {
    update_status_manager_.ScheduleUpdate();
  }
}

std::string PolicyManagerImpl::GetIconUrl(
    const std::string& policy_app_id) const {
  return cache_->GetIconUrl(policy_app_id);
}

/**
 * @brief FilterInvalidFunctions filter functions that are absent in schema
 * @param rpcs list of functions to filter
 */
void FilterInvalidFunctions(policy_table::Rpc& rpcs) {
  policy_table::Rpc valid_rpcs;
  for (const auto& rpc : rpcs) {
    const std::string& rpc_name = rpc.first;
    policy_table::FunctionID function_id;
    if (policy_table::EnumFromJsonString(rpc_name, &function_id)) {
      valid_rpcs.insert(rpc);
    }
  }
  rpcs.swap(valid_rpcs);
}

/**
 * @brief FilterInvalidRPCParameters filter parameters that not present in
 * schema
 * @param rpc_parameters parameters to filter
 */
void FilterInvalidRPCParameters(
    policy_table::RpcParameters& rpc_parameters,
    const std::vector<policy_table::VehicleDataItem>& vehicle_data_items) {
  policy_table::HmiLevels valid_hmi_levels;
  for (const auto& hmi_level : rpc_parameters.hmi_levels) {
    if (hmi_level.is_valid()) {
      valid_hmi_levels.push_back(hmi_level);
    }
  }
  rpc_parameters.hmi_levels.swap(valid_hmi_levels);

  auto ParamExists =
      [&vehicle_data_items](const rpc::String<0, 255>& param_name) {
        policy_table::Parameter parameter_enum;
        if (policy_table::EnumFromJsonString(param_name, &parameter_enum)) {
          return true;
        }

        // In case when this collection is empty that means collection is not
        // initialized
        if (vehicle_data_items.empty()) {
          return false;
        }

        for (const auto& vdi : vehicle_data_items) {
          if (param_name == vdi.name) {
            return true;
          }
        }
        return false;
      };

  policy_table::Parameters valid_params;
  const policy_table::Parameters& params = *(rpc_parameters.parameters);
  for (const auto& param : params) {
    if (param.is_valid() && ParamExists(param)) {
      valid_params.push_back(param);
    }
  }
  rpc_parameters.parameters->swap(valid_params);
}

/**
 * @brief FilterInvalidPriorityValues filter notification priorities that are
 * not present in schema
 * @param notifications priorities to filter
 */
void FilterInvalidPriorityValues(
    policy_table::NumberOfNotificationsPerMinute& notifications) {
  policy_table::NumberOfNotificationsPerMinute valid_notifications;
  for (const auto& notification : notifications) {
    policy_table::Priority priority;
    if (policy_table::EnumFromJsonString(notification.first, &priority)) {
      valid_notifications.insert(notification);
    }
  }
  notifications.swap(valid_notifications);
}

/**
 * @brief FilterInvalidApplicationParameters filter app params that are not
 * present in schema
 * @param app_params object of app policy params to filter
 */
void FilterInvalidApplicationParameters(
    policy_table::ApplicationParams& app_params) {
  // Filter AppHMIType array
  policy_table::AppHMITypes valid_app_hmi_types;
  const policy_table::AppHMITypes& app_hmi_types = *(app_params.AppHMIType);
  for (const auto& app_hmi_type : app_hmi_types) {
    if (app_hmi_type.is_valid()) {
      valid_app_hmi_types.push_back(app_hmi_type);
    }
  }
  app_params.AppHMIType->swap(valid_app_hmi_types);

  // Filter RquestTypes array
  policy_table::RequestTypes valid_request_types;
  const policy_table::RequestTypes& request_types = *(app_params.RequestType);
  for (const auto& request_type : request_types) {
    if (request_type.is_valid()) {
      valid_request_types.push_back(request_type);
    }
  }
  if (valid_request_types.empty() && !request_types.empty()) {
    // An empty RequestType array will allow all request types. No valid
    // parameters are in the filtered array, so assign an uninitialized value to
    // for array to be "omitted"
    *(app_params.RequestType) = policy_table::RequestTypes();
  } else {
    app_params.RequestType->swap(valid_request_types);
  }

  // Filter moduleType array
  policy_table::ModuleTypes valid_module_types;
  const policy_table::ModuleTypes& module_types = *(app_params.moduleType);
  for (const auto& module_type : module_types) {
    if (module_type.is_valid()) {
      valid_module_types.push_back(module_type);
    }
  }
  if (valid_module_types.empty() && !module_types.empty()) {
    // An empty moduleType array will allow all request types. No valid
    // parameters are in the filtered array, so assign an uninitialized value to
    // for array to be "omitted"
    *(app_params.moduleType) = policy_table::ModuleTypes();
  } else {
    app_params.moduleType->swap(valid_module_types);
  }

  // Filter default_hmi
  if (!app_params.default_hmi.is_valid()) {
    app_params.default_hmi = policy_table::HmiLevel();
  }

  // Filter priority
  if (!app_params.priority.is_valid()) {
    app_params.priority = policy_table::Priority();
  }
}

/**
 * @brief FilterPolicyTable filter values that not present in schema
 * @param pt policy table to filter
 */
void FilterPolicyTable(
    policy_table::PolicyTable& pt,
    const std::vector<policy_table::VehicleDataItem>& current_vd_items) {
  policy_table::ModuleConfig& module_config = pt.module_config;
  if (module_config.is_initialized() &&
      module_config.notifications_per_minute_by_priority.is_initialized()) {
    FilterInvalidPriorityValues(
        module_config.notifications_per_minute_by_priority);
  }

  if (pt.app_policies_section.is_initialized()) {
    policy_table::ApplicationPolicies& apps = pt.app_policies_section.apps;
    for (auto& app_policy : apps) {
      FilterInvalidApplicationParameters(app_policy.second);
    }
  }

  for (auto& group : pt.functional_groupings) {
    policy_table::Rpc& rpcs = group.second.rpcs;
    FilterInvalidFunctions(rpcs);

    policy_table::VehicleDataItems vehicle_data_items;

    if (!pt.vehicle_data->struct_empty()) {
      vehicle_data_items =
          pt.vehicle_data.is_initialized() &&
                  pt.vehicle_data->schema_items.is_initialized()
              ? *pt.vehicle_data->schema_items
              : current_vd_items;
    }

    for (auto& func : rpcs) {
      FilterInvalidRPCParameters(func.second, vehicle_data_items);
    }
  }
}

PolicyManager::PtProcessingResult PolicyManagerImpl::LoadPT(
    const std::string& file, const BinaryMessage& pt_content) {
  LOG4CXX_INFO(logger_, "LoadPT of size " << pt_content.size());
  LOG4CXX_DEBUG(
      logger_,
      "PTU content is: " << std::string(pt_content.begin(), pt_content.end()));

  // Parse message into table struct
  std::shared_ptr<policy_table::Table> pt_update = Parse(pt_content);
  if (!pt_update) {
    LOG4CXX_WARN(logger_, "Parsed table pointer is NULL.");
    return PtProcessingResult::kWrongPtReceived;
  }

  file_system::DeleteFile(file);
  auto current_vd_items = GetVehicleDataItems();

  FilterPolicyTable(pt_update->policy_table, current_vd_items);
  if (!IsPTValid(pt_update, policy_table::PT_UPDATE)) {
    LOG4CXX_WARN(logger_, "Received policy table update is not valid");
    return PtProcessingResult::kWrongPtReceived;
  }

  cache_->SaveUpdateRequired(false);
  sync_primitives::AutoLock lock(apps_registration_lock_);

  // Get current DB data, since it could be updated during awaiting of PTU
  auto policy_table_snapshot = cache_->GenerateSnapshot();
  if (!policy_table_snapshot) {
    LOG4CXX_ERROR(
        logger_,
        "Failed to create snapshot of policy table, trying another exchange");
    return PtProcessingResult::kNewPtRequired;
  }

  // Checking of difference between PTU and current policy state
  // Must to be done before PTU applying since it is possible, that functional
  // groups, which had been present before are absent in PTU and will be
  // removed after update. So in case of revoked groups system has to know
  // names and ids of revoked groups before they will be removed.
  CheckAppPolicyResults results =
      CheckPermissionsChanges(pt_update, policy_table_snapshot);

  // Replace current data with updated
  if (!cache_->ApplyUpdate(*pt_update)) {
    LOG4CXX_WARN(
        logger_,
        "Unsuccessful save of updated policy table, trying another exchange");
    return PtProcessingResult::kNewPtRequired;
  }

  ExternalConsentStatus status = cache_->GetExternalConsentStatus();
  GroupsByExternalConsentStatus groups_by_status =
      cache_->GetGroupsWithSameEntities(status);

  ProcessExternalConsentStatusUpdate(
      groups_by_status, ConsentProcessingPolicy::kExternalConsentBased);

  ProcessAppPolicyCheckResults(
      results, pt_update->policy_table.app_policies_section.apps);

  CheckPermissionsChangesAfterUpdate(*pt_update, *policy_table_snapshot);

  listener_->OnCertificateUpdated(
      *(pt_update->policy_table.module_config.certificate));

  std::map<std::string, StringArray> app_hmi_types;
  cache_->GetHMIAppTypeAfterUpdate(app_hmi_types);
  if (!app_hmi_types.empty()) {
    LOG4CXX_INFO(logger_, "app_hmi_types is full calling OnUpdateHMIAppType");
    listener_->OnUpdateHMIAppType(app_hmi_types);
  } else {
    LOG4CXX_INFO(logger_, "app_hmi_types empty");
  }

  std::vector<std::string> enabled_apps;
  cache_->GetEnabledCloudApps(enabled_apps);
  for (auto it = enabled_apps.begin(); it != enabled_apps.end(); ++it) {
    SendAuthTokenUpdated(*it);
  }

  return PtProcessingResult::kSuccess;
}

void PolicyManagerImpl::OnPTUFinished(const PtProcessingResult ptu_result) {
  LOG4CXX_AUTO_TRACE(logger_);

  ptu_requested_ = false;
  if (PtProcessingResult::kWrongPtReceived == ptu_result) {
    LOG4CXX_DEBUG(logger_, "Wrong PT was received");
    update_status_manager_.OnWrongUpdateReceived();
    return;
  }

  update_status_manager_.OnValidUpdateReceived();

  if (HasApplicationForPTU()) {
    update_status_manager_.OnExistedApplicationAdded(true);
  } else if (PtProcessingResult::kNewPtRequired == ptu_result) {
    LOG4CXX_DEBUG(logger_, "New PTU interation is required");
    ForcePTExchange();
    return;
  }

  ResumePendingAppPolicyActions();

  // If there was a user request for policy table update, it should be started
  // right after current update is finished
  if (update_status_manager_.IsUpdateRequired()) {
    LOG4CXX_DEBUG(logger_,
                  "PTU was successful and new PTU iteration was scheduled");
    StartPTExchange();
    return;
  }

  RefreshRetrySequence();
}

CheckAppPolicyResults PolicyManagerImpl::CheckPermissionsChanges(
    const std::shared_ptr<policy_table::Table> pt_update,
    const std::shared_ptr<policy_table::Table> snapshot) {
  LOG4CXX_AUTO_TRACE(logger_);

  // Replace predefined policies with its actual setting, e.g. "123":"default"
  // to actual values of default section
  UnwrapAppPolicies(pt_update->policy_table.app_policies_section.apps);

  CheckAppPolicyResults out_results;
  std::for_each(pt_update->policy_table.app_policies_section.apps.begin(),
                pt_update->policy_table.app_policies_section.apps.end(),
                CheckAppPolicy(this, pt_update, snapshot, out_results));

  return out_results;
}

void PolicyManagerImpl::CheckPermissionsChangesAfterUpdate(
    const policy_table::Table& update, const policy_table::Table& snapshot) {
  const auto new_lock_screen_dismissal_enabled =
      update.policy_table.module_config.lock_screen_dismissal_enabled;
  const auto old_lock_screen_dismissal_enabled =
      snapshot.policy_table.module_config.lock_screen_dismissal_enabled;
  if (new_lock_screen_dismissal_enabled != old_lock_screen_dismissal_enabled) {
    listener()->OnLockScreenDismissalStateChanged();
  }
}

void PolicyManagerImpl::ProcessAppPolicyCheckResults(
    const CheckAppPolicyResults& results,
    const policy_table::ApplicationPolicies& app_policies) {
  ApplicationsPoliciesActions actions_for_apps_policies;
  FillActionsForAppPolicies filler(actions_for_apps_policies, app_policies);

  std::for_each(results.begin(), results.end(), filler);

  ProcessActionsForAppPolicies(actions_for_apps_policies, app_policies);
}

void PolicyManagerImpl::ProcessActionsForAppPolicies(
    const ApplicationsPoliciesActions& actions,
    const policy_table::ApplicationPolicies& app_policies) {
  notify_system_list_.clear();
  send_permissions_list_.clear();

  for (const auto& action : actions) {
    const auto& app_policy = app_policies.find(action.first);
    if (app_policies.end() == app_policy) {
      continue;
    }

    if (action.second.app_properties_changed) {
      app_properties_changed_list_.push_back(app_policy->first);
    }

    const auto devices_ids = listener()->GetDevicesIds(app_policy->first);
    for (const auto& device_id : devices_ids) {
      if (action.second.is_consent_needed) {
        // Post-check after ExternalConsent consent changes
        const auto& policy_app_id = app_policy->first;
        if (!IsConsentNeeded(device_id, policy_app_id)) {
          sync_primitives::AutoLock lock(app_permissions_diff_lock_);

          const auto& app_id_diff = app_permissions_diff_.find(policy_app_id);

          if (app_permissions_diff_.end() != app_id_diff) {
            app_id_diff->second.appPermissionsConsentNeeded = false;
          }
        }
      }

      if (action.second.is_notify_system) {
        notify_system_list_.push_back(std::make_pair(device_id, *app_policy));
      }

      if (action.second.is_send_permissions_to_app) {
        send_permissions_list_.push_back(
            std::make_pair(device_id, *app_policy));
      }
    }
  }
}

void PolicyManagerImpl::PrepareNotificationData(
    const policy_table::FunctionalGroupings& groups,
    const policy_table::Strings& group_names,
    const std::vector<FunctionalGroupPermission>& group_permission,
    Permissions& notification_data) {
  LOG4CXX_INFO(logger_, "Preparing data for notification.");
  ProcessFunctionalGroup processor(groups, group_permission, notification_data);
  std::for_each(group_names.begin(), group_names.end(), processor);
}

std::string PolicyManagerImpl::GetUpdateUrl(int service_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  EndpointUrls urls;
  GetUpdateUrls(service_type, urls);

  std::string url;
  if (!urls.empty()) {
    static uint32_t index = 0;

    if (index >= urls.size()) {
      index = 0;
    }
    url = urls[index].url.empty() ? "" : urls[index].url[0];

    ++index;
  } else {
    LOG4CXX_ERROR(logger_, "The endpoint entry is empty");
  }
  return url;
}

void PolicyManagerImpl::GetUpdateUrls(const std::string& service_type,
                                      EndpointUrls& out_end_points) const {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->GetUpdateUrls(service_type, out_end_points);
}
void PolicyManagerImpl::GetUpdateUrls(const uint32_t service_type,
                                      EndpointUrls& out_end_points) const {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->GetUpdateUrls(service_type, out_end_points);
}

void PolicyManagerImpl::RequestPTUpdate() {
  LOG4CXX_AUTO_TRACE(logger_);
  std::shared_ptr<policy_table::Table> policy_table_snapshot =
      cache_->GenerateSnapshot();
  if (!policy_table_snapshot) {
    LOG4CXX_ERROR(logger_, "Failed to create snapshot of policy table");
    return;
  }

  if (IsPTValid(policy_table_snapshot, policy_table::PT_SNAPSHOT)) {
    Json::Value value = policy_table_snapshot->ToJsonValue();
    Json::StreamWriterBuilder writer_builder;
    writer_builder["indentation"] = "";
    std::string message_string = Json::writeString(writer_builder, value);

    LOG4CXX_DEBUG(logger_, "Snapshot contents is : " << message_string);

    BinaryMessage update(message_string.begin(), message_string.end());
    ptu_requested_ = true;
    listener_->OnSnapshotCreated(
        update, RetrySequenceDelaysSeconds(), TimeoutExchangeMSec());
  } else {
    LOG4CXX_ERROR(logger_, "Invalid Policy table snapshot - PTUpdate failed");
  }
}

void PolicyManagerImpl::StartPTExchange() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (ignition_check) {
    CheckTriggers();
    ignition_check = false;
  }

  if (update_status_manager_.IsAppsSearchInProgress() &&
      update_status_manager_.IsUpdateRequired()) {
    LOG4CXX_INFO(logger_,
                 "Starting exchange skipped, since applications "
                 "search is in progress.");
    return;
  }

  if (update_status_manager_.IsUpdatePending()) {
    if (trigger_ptu_) {
      update_status_manager_.ScheduleUpdate();
    }
    LOG4CXX_INFO(logger_,
                 "Starting exchange skipped, since another exchange "
                 "is in progress.");
    return;
  }
  LOG4CXX_INFO(logger_, "Policy want to  call RequestPTUpdate");
  if (listener_) {
    LOG4CXX_INFO(logger_, "Listener CanUpdate");
    if (update_status_manager_.IsUpdateRequired()) {
      update_status_manager_.PendingUpdate();
      LOG4CXX_INFO(logger_, "IsUpdateRequired");
      RequestPTUpdate();
    }
  }
}

void PolicyManagerImpl::OnAppsSearchStarted() {
  LOG4CXX_AUTO_TRACE(logger_);
  update_status_manager_.OnAppsSearchStarted();
}

void PolicyManagerImpl::OnAppsSearchCompleted(const bool trigger_ptu) {
  LOG4CXX_AUTO_TRACE(logger_);
  update_status_manager_.OnAppsSearchCompleted();

  trigger_ptu_ = trigger_ptu;

  if (update_status_manager_.IsUpdateRequired()) {
    StartPTExchange();
  }
}

void PolicyManagerImpl::UpdatePTUReadyAppsCount(const uint32_t new_app_count) {
  applications_pending_ptu_count_ = new_app_count;
}

const std::vector<std::string> PolicyManagerImpl::GetAppRequestTypes(
    const transport_manager::DeviceHandle& device_handle,
    const std::string policy_app_id) const {
  std::vector<std::string> request_types;
  if (kDeviceAllowed != cache_->GetDeviceConsent(
                            GetCurrentDeviceId(device_handle, policy_app_id))) {
    cache_->GetAppRequestTypes(kPreDataConsentId, request_types);
  } else {
    cache_->GetAppRequestTypes(policy_app_id, request_types);
  }
  return request_types;
}

RequestType::State PolicyManagerImpl::GetAppRequestTypesState(
    const std::string& policy_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetAppRequestTypesState(policy_app_id);
}

RequestSubType::State PolicyManagerImpl::GetAppRequestSubTypesState(
    const std::string& policy_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetAppRequestSubTypesState(policy_app_id);
}

const std::vector<std::string> PolicyManagerImpl::GetAppRequestSubTypes(
    const std::string& policy_app_id) const {
  std::vector<std::string> request_subtypes;
  cache_->GetAppRequestSubTypes(policy_app_id, request_subtypes);
  return request_subtypes;
}

const std::vector<policy_table::VehicleDataItem>
PolicyManagerImpl::GetVehicleDataItems() const {
  return cache_->GetVehicleDataItems();
}

std::vector<policy_table::VehicleDataItem>
PolicyManagerImpl::GetRemovedVehicleDataItems() const {
  return cache_->GetRemovedVehicleDataItems();
}

Json::Value PolicyManagerImpl::GetPolicyTableData() const {
  return cache_->GetPolicyTableData();
}

void PolicyManagerImpl::GetEnabledCloudApps(
    std::vector<std::string>& enabled_apps) const {
  cache_->GetEnabledCloudApps(enabled_apps);
}

std::vector<std::string> PolicyManagerImpl::GetEnabledLocalApps() const {
  return cache_->GetEnabledLocalApps();
}

bool PolicyManagerImpl::GetAppProperties(
    const std::string& policy_app_id, AppProperties& out_app_properties) const {
  return cache_->GetAppProperties(policy_app_id, out_app_properties);
}

void PolicyManagerImpl::InitCloudApp(const std::string& policy_app_id) {
  cache_->InitCloudApp(policy_app_id);
}

void PolicyManagerImpl::SetCloudAppEnabled(const std::string& policy_app_id,
                                           const bool enabled) {
  cache_->SetCloudAppEnabled(policy_app_id, enabled);
}

void PolicyManagerImpl::SetAppAuthToken(const std::string& policy_app_id,
                                        const std::string& auth_token) {
  cache_->SetAppAuthToken(policy_app_id, auth_token);
}

void PolicyManagerImpl::SetAppCloudTransportType(
    const std::string& policy_app_id, const std::string& cloud_transport_type) {
  cache_->SetAppCloudTransportType(policy_app_id, cloud_transport_type);
}

void PolicyManagerImpl::SetAppEndpoint(const std::string& policy_app_id,
                                       const std::string& endpoint) {
  cache_->SetAppEndpoint(policy_app_id, endpoint);
}

void PolicyManagerImpl::SetAppNicknames(const std::string& policy_app_id,
                                        const StringArray& nicknames) {
  cache_->SetAppNicknames(policy_app_id, nicknames);
}

void PolicyManagerImpl::SetHybridAppPreference(
    const std::string& policy_app_id,
    const std::string& hybrid_app_preference) {
  cache_->SetHybridAppPreference(policy_app_id, hybrid_app_preference);
}

void PolicyManagerImpl::GetAppServiceParameters(
    const std::string& policy_app_id,
    policy_table::AppServiceParameters* app_service_parameters) const {
  cache_->GetAppServiceParameters(policy_app_id, app_service_parameters);
}

bool PolicyManagerImpl::UnknownRPCPassthroughAllowed(
    const std::string& policy_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->UnknownRPCPassthroughAllowed(policy_app_id);
}

void PolicyManagerImpl::CheckPermissions(const PTString& device_id,
                                         const PTString& app_id,
                                         const PTString& hmi_level,
                                         const PTString& rpc,
                                         const RPCParams& rpc_params,
                                         CheckPermissionResult& result) {
  LOG4CXX_INFO(logger_,
               "CheckPermissions for " << app_id << " and rpc " << rpc
                                       << " for " << hmi_level << " level.");

  Permissions rpc_permissions;

  // Check, if there are calculated permission present in cache
  if (!cache_->IsPermissionsCalculated(device_id, app_id, rpc_permissions)) {
    LOG4CXX_DEBUG(logger_,
                  "IsPermissionsCalculated for device: "
                      << device_id << " and app: " << app_id
                      << " returns false");
    // Get actual application group permission according to user consents
    std::vector<FunctionalGroupPermission> app_group_permissions;
    GetPermissionsForApp(device_id, app_id, app_group_permissions);

    // Fill struct with known groups RPCs
    policy_table::FunctionalGroupings functional_groupings;
    cache_->GetFunctionalGroupings(functional_groupings);

    policy_table::Strings app_groups = GetGroupsNames(app_group_permissions);

    // Undefined groups (without user consent) disallowed by default, since
    // OnPermissionsChange notification has no "undefined" section
    // For RPC permission checking undefinded group will be treated as separate
    // type
    ProcessFunctionalGroup processor(functional_groupings,
                                     app_group_permissions,
                                     rpc_permissions,
                                     GroupConsent::kGroupUndefined);
    std::for_each(app_groups.begin(), app_groups.end(), processor);

    cache_->AddCalculatedPermissions(device_id, app_id, rpc_permissions);
  } else {
    LOG4CXX_DEBUG(logger_,
                  "IsPermissionsCalculated for device: "
                      << device_id << " and app: " << app_id
                      << " returns true");
  }

  if (cache_->IsApplicationRevoked(app_id)) {
    // SDL must be able to notify mobile side with its status after app has
    // been revoked by backend
    if ("OnHMIStatus" == rpc && "NONE" == hmi_level) {
      result.hmi_level_permitted = kRpcAllowed;
    } else {
      result.hmi_level_permitted = kRpcDisallowed;
    }
    return;
  }

  const bool known_rpc = rpc_permissions.end() != rpc_permissions.find(rpc);
  LOG4CXX_DEBUG(logger_, "Is known rpc " << (known_rpc ? "true" : "false"));
  if (!known_rpc) {
    // RPC not found in list == disallowed by backend
    result.hmi_level_permitted = kRpcDisallowed;
    return;
  }

  // Check HMI level
  if (rpc_permissions[rpc].hmi_permissions[kAllowedKey].end() !=
      rpc_permissions[rpc].hmi_permissions[kAllowedKey].find(hmi_level)) {
    // RPC found in allowed == allowed by backend and user
    result.hmi_level_permitted = kRpcAllowed;
  } else if (rpc_permissions[rpc].hmi_permissions[kUndefinedKey].end() !=
             rpc_permissions[rpc].hmi_permissions[kUndefinedKey].find(
                 hmi_level)) {
    // RPC found in undefined == allowed by backend, but not consented yet by
    // user
    result.hmi_level_permitted = kRpcDisallowed;
  } else if (rpc_permissions[rpc].hmi_permissions[kUserDisallowedKey].end() !=
             rpc_permissions[rpc].hmi_permissions[kUserDisallowedKey].find(
                 hmi_level)) {
    // RPC found in allowed == allowed by backend, but disallowed by user
    LOG4CXX_DEBUG(
        logger_,
        "RPC found in allowed == allowed by backend, but disallowed by user");
    result.hmi_level_permitted = kRpcUserDisallowed;
  } else {
    LOG4CXX_DEBUG(logger_,
                  "HMI level " << hmi_level << " wasn't found "
                               << " for rpc " << rpc << " and appID "
                               << app_id);
    return;
  }

  if (kRpcAllowed != result.hmi_level_permitted) {
    LOG4CXX_DEBUG(logger_, "RPC is not allowed. Stop parameters processing.");
    result.list_of_allowed_params =
        rpc_permissions[rpc].parameter_permissions[kAllowedKey];

    result.list_of_disallowed_params =
        rpc_permissions[rpc].parameter_permissions[kUserDisallowedKey];

    result.list_of_undefined_params =
        rpc_permissions[rpc].parameter_permissions[kUndefinedKey];
    return;
  }

  // Considered that items disallowed by user take priority over system (policy)
  // permissions, so that flag is processed first
  if (rpc_permissions[rpc]
          .parameter_permissions.any_parameter_disallowed_by_user) {
    LOG4CXX_DEBUG(logger_, "All parameters are disallowed by user.");
    result.list_of_disallowed_params = rpc_params;
    result.hmi_level_permitted = kRpcUserDisallowed;
    return;
  }

  if (rpc_permissions[rpc]
          .parameter_permissions.any_parameter_disallowed_by_policy) {
    LOG4CXX_DEBUG(logger_, "All parameters are disallowed by policy.");
    result.list_of_undefined_params = rpc_params;
    result.hmi_level_permitted = kRpcDisallowed;
    return;
  }

  if (rpc_permissions[rpc].parameter_permissions.any_parameter_allowed) {
    LOG4CXX_DEBUG(logger_, "All parameters are allowed.");
    result.list_of_allowed_params = rpc_params;
    return;
  }

  result.list_of_allowed_params =
      rpc_permissions[rpc].parameter_permissions[kAllowedKey];

  result.list_of_disallowed_params =
      rpc_permissions[rpc].parameter_permissions[kUserDisallowedKey];

  result.list_of_undefined_params =
      rpc_permissions[rpc].parameter_permissions[kUndefinedKey];

  // In case of some parameters of RPC are missing in current policy table
  // they will be considered as disallowed by policy itself, not by user.
  // Undefined parameters contain parameters present in policy table, but which
  // have not been allowed or disallowed explicitly by user, so missing params
  // are being added to undefined.
  RPCParams::const_iterator parameter = rpc_params.begin();
  RPCParams::const_iterator end = rpc_params.end();
  for (; end != parameter; ++parameter) {
    if (!result.HasParameter(*parameter)) {
      LOG4CXX_DEBUG(logger_,
                    "Parameter " << *parameter
                                 << " is unknown."
                                    " Adding to undefined list.");
      result.list_of_undefined_params.insert(*parameter);
    }
  }

  if (result.DisallowedInclude(rpc_params)) {
    LOG4CXX_DEBUG(logger_, "All parameters are disallowed.");
    result.hmi_level_permitted = kRpcUserDisallowed;
  } else if (!result.IsAnyAllowed(rpc_params)) {
    LOG4CXX_DEBUG(logger_, "There are no parameters allowed.");

    if (!result.list_of_undefined_params.empty()) {
      result.hmi_level_permitted = kRpcAllowed;
    } else {
      result.hmi_level_permitted = kRpcDisallowed;
    }
  }
}

bool PolicyManagerImpl::ResetUserConsent() {
  return cache_->ResetUserConsent();
}

policy_table::Strings PolicyManagerImpl::GetGroupsNames(
    const std::vector<FunctionalGroupPermission>& app_group_permissions) const {
  policy_table::Strings app_groups;
  GroupNamesAppender appender(app_groups);
  std::for_each(
      app_group_permissions.begin(), app_group_permissions.end(), appender);

  return app_groups;
}

void PolicyManagerImpl::SendNotificationOnPermissionsUpdated(
    const std::string& device_id, const std::string& application_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (device_id.empty()) {
    LOG4CXX_WARN(logger_,
                 "Couldn't find device info for application id "
                 "'" << application_id
                     << "'");
    return;
  }

  std::vector<FunctionalGroupPermission> app_group_permissions;
  GetPermissionsForApp(device_id, application_id, app_group_permissions);

  policy_table::FunctionalGroupings functional_groupings;
  cache_->GetFunctionalGroupings(functional_groupings);

  policy_table::Strings app_groups = GetGroupsNames(app_group_permissions);

  Permissions notification_data;
  PrepareNotificationData(functional_groupings,
                          app_groups,
                          app_group_permissions,
                          notification_data);

  LOG4CXX_INFO(logger_,
               "Send notification for application_id:" << application_id);

  const ApplicationOnDevice who = {device_id, application_id};
  if (access_remote_->IsAppRemoteControl(who)) {
    listener()->OnPermissionsUpdated(
        device_id, application_id, notification_data);
    return;
  }

  std::string default_hmi;
  GetDefaultHmi(device_id, application_id, &default_hmi);

  listener()->OnPermissionsUpdated(
      device_id, application_id, notification_data, default_hmi);
}

bool PolicyManagerImpl::CleanupUnpairedDevices() {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->CleanupUnpairedDevices();
}

DeviceConsent PolicyManagerImpl::GetUserConsentForDevice(
    const std::string& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetDeviceConsent(device_id);
}

void PolicyManagerImpl::SetUserConsentForDevice(const std::string& device_id,
                                                const bool is_allowed) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Device :" << device_id);
  cache_->SetDeviceConsent(device_id, is_allowed);
  if (listener_) {
    listener_->OnDeviceConsentChanged(device_id, is_allowed);
  } else {
    LOG4CXX_WARN(logger_,
                 "Event listener is not initialized. "
                 "Can't call OnDeviceConsentChanged");
  }
  if (is_allowed && listener_->CanUpdate()) {
    update_status_manager_.OnDeviceConsented();
  }
  StartPTExchange();
}

bool PolicyManagerImpl::ReactOnUserDevConsentForApp(
    const transport_manager::DeviceHandle& device_handle,
    const std::string& app_id,
    const bool is_device_allowed) {
  std::vector<std::string> current_request_types =
      GetAppRequestTypes(device_handle, app_id);
  std::string current_priority, new_priority;
  GetPriority(app_id, &current_priority);

  bool result = cache_->ReactOnUserDevConsentForApp(app_id, is_device_allowed);

  std::vector<std::string> new_request_types =
      GetAppRequestTypes(device_handle, app_id);
  GetPriority(app_id, &new_priority);
  std::sort(current_request_types.begin(), current_request_types.end());
  std::sort(new_request_types.begin(), new_request_types.end());

  std::vector<std::string> diff;
  std::set_symmetric_difference(current_request_types.begin(),
                                current_request_types.end(),
                                new_request_types.begin(),
                                new_request_types.end(),
                                std::back_inserter(diff));

  AppPermissions permissions(app_id);

  if (!diff.empty()) {
    permissions.requestType = new_request_types;
    permissions.requestTypeChanged = true;
  }

  if ((!current_priority.empty()) && (!new_priority.empty()) &&
      (current_priority != new_priority)) {
    permissions.priority = new_priority;
  }

  if (permissions.requestTypeChanged || (!permissions.priority.empty())) {
    const auto& device_id = GetCurrentDeviceId(device_handle, app_id);
    listener_->SendOnAppPermissionsChanged(permissions, device_id, app_id);
  }
  return result;
}

bool PolicyManagerImpl::GetInitialAppData(const std::string& application_id,
                                          StringArray* nicknames,
                                          StringArray* app_hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  const bool result = nicknames && app_hmi_types;
  if (result) {
    cache_->GetInitialAppData(application_id, *nicknames, *app_hmi_types);
  }
  return result;
}

void PolicyManagerImpl::AddDevice(const std::string& device_id,
                                  const std::string& connection_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Device: " << device_id);
  if (!cache_->AddDevice(device_id, connection_type)) {
    LOG4CXX_WARN(logger_, "Can't add device.");
  }
}

void PolicyManagerImpl::OnLocalAppAdded() {
  LOG4CXX_AUTO_TRACE(logger_);
  update_status_manager_.ScheduleUpdate();
  StartPTExchange();
}

void PolicyManagerImpl::SetDeviceInfo(const std::string& device_id,
                                      const DeviceInfo& device_info) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Device :" << device_id);
  if (!cache_->SetDeviceData(device_id,
                             device_info.hardware,
                             device_info.firmware_rev,
                             device_info.os,
                             device_info.os_ver,
                             device_info.carrier,
                             device_info.max_number_rfcom_ports,
                             device_info.connection_type)) {
    LOG4CXX_WARN(logger_, "Can't set device data.");
  }
}

PermissionConsent PolicyManagerImpl::EnsureCorrectPermissionConsent(
    const PermissionConsent& permissions_to_check) {
  std::vector<FunctionalGroupPermission> current_user_consents;
  GetUserConsentForApp(permissions_to_check.device_id,
                       permissions_to_check.policy_app_id,
                       current_user_consents);

  PermissionConsent permissions_to_set;
  permissions_to_set.device_id = permissions_to_check.device_id;
  permissions_to_set.policy_app_id = permissions_to_check.policy_app_id;
  permissions_to_set.consent_source = permissions_to_check.consent_source;

  std::vector<FunctionalGroupPermission>::const_iterator it =
      permissions_to_check.group_permissions.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end =
      permissions_to_check.group_permissions.end();

  for (; it != it_end; ++it) {
    std::vector<FunctionalGroupPermission>::const_iterator it_curr =
        current_user_consents.begin();
    std::vector<FunctionalGroupPermission>::const_iterator it_curr_end =
        current_user_consents.end();

    for (; it_curr != it_curr_end; ++it_curr) {
      if (it->group_alias == it_curr->group_alias &&
          it->group_id == it_curr->group_id) {
        permissions_to_set.group_permissions.push_back(*it);
      }
    }
  }

  return permissions_to_set;
}

void PolicyManagerImpl::CheckPendingPermissionsChanges(
    const std::string& policy_app_id,
    const std::vector<FunctionalGroupPermission>& current_permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(app_permissions_diff_lock_);
  PendingPermissions::iterator it_pending =
      app_permissions_diff_.find(policy_app_id);
  if (app_permissions_diff_.end() == it_pending) {
    LOG4CXX_WARN(
        logger_,
        "No pending permissions had been found for appID: " << policy_app_id);
    return;
  }

  LOG4CXX_DEBUG(
      logger_,
      "Pending permissions had been found for appID: " << policy_app_id);

  // Change appPermissionsConsentNeeded depending on unconsented groups
  // presence
  std::vector<policy::FunctionalGroupPermission>::const_iterator it_groups =
      current_permissions.begin();
  std::vector<policy::FunctionalGroupPermission>::const_iterator it_end_groups =
      current_permissions.end();

  for (; it_groups != it_end_groups; ++it_groups) {
    if (policy::kGroupUndefined == it_groups->state) {
      LOG4CXX_DEBUG(
          logger_,
          "Unconsented groups still present for appID: " << policy_app_id);
      it_pending->second.appPermissionsConsentNeeded = true;
      return;
    }
  }

  LOG4CXX_DEBUG(
      logger_,
      "Unconsented groups not present anymore for appID: " << policy_app_id);
  it_pending->second.appPermissionsConsentNeeded = false;
  return;
}

void PolicyManagerImpl::NotifyPermissionsChanges(
    const std::string& device_id,
    const std::string& policy_app_id,
    const std::vector<FunctionalGroupPermission>& app_group_permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  // Get current functional groups from DB with RPC permissions
  policy_table::FunctionalGroupings functional_groups;
  cache_->GetFunctionalGroupings(functional_groups);

  // Get list of groups assigned to application
  policy_table::Strings app_groups = GetGroupsNames(app_group_permissions);

  // Fill notification data according to group permissions
  Permissions notification_data;
  PrepareNotificationData(
      functional_groups, app_groups, app_group_permissions, notification_data);

  listener()->OnPermissionsUpdated(device_id, policy_app_id, notification_data);
}

void PolicyManagerImpl::SetUserConsentForApp(
    const PermissionConsent& permissions, const NotificationMode mode) {
  LOG4CXX_AUTO_TRACE(logger_);

  cache_->ResetCalculatedPermissions();
  PermissionConsent verified_permissions =
      EnsureCorrectPermissionConsent(permissions);

  bool app_permissions_changed = false;
  if (!cache_->SetUserPermissionsForApp(verified_permissions,
                                        &app_permissions_changed)) {
    LOG4CXX_WARN(logger_, "Can't set user permissions for application.");
    return;
  }

  if (kSilentMode == mode) {
    LOG4CXX_WARN(logger_,
                 "Silent mode is enabled. Application won't be informed.");
    return;
  }

  if (!app_permissions_changed) {
    LOG4CXX_WARN(logger_,
                 "Application already has same consents. "
                 "Notificaton won't be sent.");
    return;
  }

  std::vector<FunctionalGroupPermission> updated_app_group_permissons;
  GetPermissionsForApp(verified_permissions.device_id,
                       verified_permissions.policy_app_id,
                       updated_app_group_permissons);

  CheckPendingPermissionsChanges(verified_permissions.policy_app_id,
                                 updated_app_group_permissons);

  NotifyPermissionsChanges(verified_permissions.device_id,
                           verified_permissions.policy_app_id,
                           updated_app_group_permissons);
}

bool PolicyManagerImpl::IsAllowedRetryCountExceeded() const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);

  return retry_sequence_index_ > retry_sequence_seconds_.size();
}

void PolicyManagerImpl::IncrementRetryIndex() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);

  if (!is_ptu_in_progress_) {
    LOG4CXX_TRACE(logger_,
                  "First PTU iteration, skipping incrementing retry index");
    is_ptu_in_progress_ = true;
    return;
  }

  ++retry_sequence_index_;
  LOG4CXX_DEBUG(logger_,
                "current retry_sequence_index_ is: " << retry_sequence_index_);
}

void PolicyManagerImpl::RetrySequenceFailed() {
  LOG4CXX_AUTO_TRACE(logger_);

  listener_->OnPTUFinished(false);
  ResetRetrySequence(ResetRetryCountType::kResetWithStatusUpdate);

  ptu_requested_ = false;
  if (HasApplicationForPTU()) {
    update_status_manager_.OnExistedApplicationAdded(true);
    StartPTExchange();
  }
}

void PolicyManagerImpl::ResetTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (update_status_manager_.IsUpdatePending()) {
    uint32_t update_timeout = TimeoutExchangeMSec();
    update_status_manager_.ResetTimeout(update_timeout);
  }
}

void PolicyManagerImpl::OnSystemRequestReceived() {
  LOG4CXX_AUTO_TRACE(logger_);
  IncrementRetryIndex();
}

bool PolicyManagerImpl::GetDefaultHmi(const std::string& device_id,
                                      const std::string& policy_app_id,
                                      std::string* default_hmi) const {
  LOG4CXX_AUTO_TRACE(logger_);
  DeviceConsent device_consent = GetUserConsentForDevice(device_id);
  const std::string app_id = policy::kDeviceAllowed != device_consent
                                 ? kPreDataConsentId
                                 : policy_app_id;
  return cache_->GetDefaultHMI(app_id, *default_hmi);
}

bool PolicyManagerImpl::GetPriority(const std::string& policy_app_id,
                                    std::string* priority) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!priority) {
    LOG4CXX_WARN(logger_, "Input priority parameter is null.");
    return false;
  }

  return cache_->GetPriority(policy_app_id, *priority);
}

std::vector<UserFriendlyMessage> PolicyManagerImpl::GetUserFriendlyMessages(
    const std::vector<std::string>& message_code,
    const std::string& language,
    const std::string& active_hmi_language) {
  return cache_->GetUserFriendlyMsg(
      message_code, language, active_hmi_language);
}

void PolicyManagerImpl::GetUserConsentForApp(
    const std::string& device_id,
    const std::string& policy_app_id,
    std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);

  FunctionalIdType group_types;
  if (!cache_->GetPermissionsForApp(device_id, policy_app_id, group_types)) {
    LOG4CXX_WARN(logger_,
                 "Can't get user permissions for app " << policy_app_id);
    return;
  }

  // Functional groups w/o alias ("user_consent_prompt") considered as
  // automatically allowed and it could not be changed by user
  FunctionalGroupNames group_names;
  if (!cache_->GetFunctionalGroupNames(group_names)) {
    LOG4CXX_WARN(logger_, "Can't get functional group names");
    return;
  }

  FunctionalGroupNames::const_iterator it = group_names.begin();
  FunctionalGroupNames::const_iterator it_end = group_names.end();
  FunctionalGroupIDs auto_allowed_groups;
  for (; it != it_end; ++it) {
    if (it->second.first.empty()) {
      auto_allowed_groups.push_back(it->first);
    }
  }

  FunctionalGroupIDs all_groups = group_types[kTypeGeneral];
  FunctionalGroupIDs preconsented_groups = group_types[kTypePreconsented];
  FunctionalGroupIDs consent_allowed_groups = group_types[kTypeAllowed];
  FunctionalGroupIDs consent_disallowed_groups = group_types[kTypeDisallowed];

  // Sorting groups by consent
  FunctionalGroupIDs preconsented_wo_auto =
      ExcludeSame(preconsented_groups, auto_allowed_groups);

  FunctionalGroupIDs preconsented_wo_disallowed_auto =
      ExcludeSame(preconsented_wo_auto, consent_disallowed_groups);

  FunctionalGroupIDs allowed_groups =
      Merge(consent_allowed_groups, preconsented_wo_disallowed_auto);

  FunctionalGroupIDs excluded_stage_1 =
      ExcludeSame(all_groups, auto_allowed_groups);

  FunctionalGroupIDs excluded_stage_2 =
      ExcludeSame(excluded_stage_1, consent_disallowed_groups);

  FunctionalGroupIDs undefined_consent =
      ExcludeSame(excluded_stage_2, allowed_groups);

  // Fill result
  FillFunctionalGroupPermissions(
      undefined_consent, group_names, kGroupUndefined, permissions);
  FillFunctionalGroupPermissions(
      allowed_groups, group_names, kGroupAllowed, permissions);
  FillFunctionalGroupPermissions(
      consent_disallowed_groups, group_names, kGroupDisallowed, permissions);
}

void PolicyManagerImpl::GetPermissionsForApp(
    const std::string& device_id,
    const std::string& policy_app_id,
    std::vector<FunctionalGroupPermission>& permissions) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string app_id_to_check = policy_app_id;

  if (!cache_->IsApplicationRepresented(policy_app_id)) {
    LOG4CXX_WARN(logger_,
                 "Application with id " << policy_app_id
                                        << " is not found within known apps.");
    return;
  }

  bool allowed_by_default = false;
  const auto device_consent = GetUserConsentForDevice(device_id);
  if ((policy::kDeviceAllowed == device_consent) &&
      cache_->IsDefaultPolicy(policy_app_id)) {
    app_id_to_check = kDefaultId;
    allowed_by_default = true;
  } else if (cache_->IsPredataPolicy(policy_app_id) ||
             policy::kDeviceAllowed != device_consent) {
    app_id_to_check = kPreDataConsentId;
    allowed_by_default = true;
  }

  FunctionalIdType group_types;

  const bool ret =
      cache_->GetPermissionsForApp(device_id, app_id_to_check, group_types);

  if (!ret) {
    LOG4CXX_WARN(logger_,
                 "Can't get user permissions for app " << policy_app_id);
    return;
  }

  // Functional groups w/o alias ("user_consent_prompt") considered as
  // automatically allowed and it could not be changed by user
  FunctionalGroupNames group_names;
  if (!cache_->GetFunctionalGroupNames(group_names)) {
    LOG4CXX_WARN(logger_, "Can't get functional group names");
    return;
  }

  // The "default" and "pre_DataConsent" are auto-allowed groups
  // So, check if application in the one of these mode.
  if (allowed_by_default) {
    LOG4CXX_INFO(logger_, "Get auto allowed groups");
    GroupType type =
        (kDefaultId == app_id_to_check ? kTypeDefault : kTypePreDataConsented);

    FillFunctionalGroupPermissions(
        group_types[type], group_names, kGroupAllowed, permissions);
  } else {
    // The code bellow allows to process application which
    // has specific permissions(not default and pre_DataConsent).

    // All groups for specific application
    FunctionalGroupIDs all_groups = group_types[kTypeGeneral];

    // Groups assigned by the user for specific application
    FunctionalGroupIDs allowed_groups = group_types[kTypeAllowed];

    // Groups disallowed by the user for specific application
    FunctionalGroupIDs common_disallowed = group_types[kTypeDisallowed];

    // Groups that allowed by default but can be changed by the user
    FunctionalGroupIDs preconsented_groups = group_types[kTypePreconsented];

    // Groups which has user consent promt but there is no any consnets now.
    FunctionalGroupIDs unconsented_groups = group_types[kTypeUnconsented];

    // Pull common groups from allowed and preconsented parts.
    FunctionalGroupIDs allowed_preconsented =
        Merge(allowed_groups, preconsented_groups);

    // Get all groups that we suppose are allowed.
    FunctionalGroupIDs all_allowed = Merge(allowed_preconsented, all_groups);

    // In case when same groups exists in disallowed and allowed tables,
    // disallowed one have priority over allowed. So we have to remove
    // all disallowed groups from allowed table.
    FunctionalGroupIDs common_allowed =
        ExcludeSame(all_allowed, common_disallowed);
    FunctionalGroupIDs consent_disallowed =
        ExcludeSame(unconsented_groups, preconsented_groups);

    // Disallowed groups are contain all directly disallowed,
    // plus unconsented minus preconsented.
    FunctionalGroupIDs all_disallowed =
        Merge(common_disallowed, consent_disallowed);

    // Fill result
    FillFunctionalGroupPermissions(
        consent_disallowed, group_names, kGroupUndefined, permissions);
    FillFunctionalGroupPermissions(
        common_allowed, group_names, kGroupAllowed, permissions);
    FillFunctionalGroupPermissions(
        all_disallowed, group_names, kGroupDisallowed, permissions);
  }
  return;
}

std::string& PolicyManagerImpl::GetCurrentDeviceId(
    const transport_manager::DeviceHandle& device_handle,
    const std::string& policy_app_id) const {
  LOG4CXX_INFO(logger_, "GetDeviceInfo");
  last_device_id_ =
      listener()->OnCurrentDeviceIdUpdateRequired(device_handle, policy_app_id);
  return last_device_id_;
}

void PolicyManagerImpl::SetSystemLanguage(const std::string& language) {
  cache_->SetSystemLanguage(language);
}

void PolicyManagerImpl::SetSystemInfo(const std::string& ccpu_version,
                                      const std::string& wers_country_code,
                                      const std::string& language) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->SetMetaInfo(ccpu_version, wers_country_code, language);
}

uint32_t PolicyManagerImpl::GetNotificationsNumber(
    const std::string& priority) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetNotificationsNumber(priority);
}

bool PolicyManagerImpl::ExceededIgnitionCycles() {
  return 0 == cache_->IgnitionCyclesBeforeExchange();
}

bool PolicyManagerImpl::IsPTValid(
    std::shared_ptr<policy_table::Table> policy_table,
    policy_table::PolicyTableType type) const {
  policy_table->SetPolicyTableType(type);
  if (!policy_table->is_valid()) {
    LOG4CXX_ERROR(logger_, "Policy table is not valid.");
    rpc::ValidationReport report("policy_table");
    policy_table->ReportErrors(&report);
    LOG4CXX_DEBUG(logger_, "Errors: " << rpc::PrettyFormat(report));
    return false;
  }
  return true;
}

bool PolicyManagerImpl::HasApplicationForPTU() const {
  return applications_pending_ptu_count_ > 0;
}

const PolicySettings& PolicyManagerImpl::get_settings() const {
  DCHECK(settings_);
  return *settings_;
}

void PolicyManagerImpl::UpdateAppConsentWithExternalConsent(
    const std::string& device_id,
    const std::string& application_id,
    const GroupsNames& allowed_groups,
    const GroupsNames& disallowed_groups,
    const ConsentProcessingPolicy processing_policy) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (allowed_groups.empty() && disallowed_groups.empty()) {
    LOG4CXX_DEBUG(logger_,
                  "Allowed and disallowed groups are empty, skipping update by "
                  "external user consent.");
    return;
  }

  std::vector<FunctionalGroupPermission> current_permissions;
  GetUserConsentForApp(device_id, application_id, current_permissions);

  ConsentPriorityType prio = ConsentPriorityType::kExternalConsentPrio;
  if (ConsentProcessingPolicy::kTimestampBased == processing_policy) {
    prio = cache_->GetConsentsPriority(device_id, application_id);
  }

  std::vector<FunctionalGroupPermission> external_consent_groups_matches;
  ConsentsUpdater updater(
      allowed_groups, disallowed_groups, external_consent_groups_matches, prio);
  std::for_each(
      current_permissions.begin(), current_permissions.end(), updater);

  const std::string source = "GUI";

  PermissionConsent updated_user_permissions;
  updated_user_permissions.group_permissions = current_permissions;
  updated_user_permissions.device_id = device_id;
  updated_user_permissions.policy_app_id = application_id;
  updated_user_permissions.consent_source = source;

  // Need to check to which app to send notification since maybe app registered
  // from different device
  SetUserConsentForApp(updated_user_permissions,
                       PolicyManager::kNotifyApplicationMode);

  PermissionConsent updated_external_consent_permissions;
  updated_external_consent_permissions.group_permissions =
      external_consent_groups_matches;
  updated_external_consent_permissions.device_id = device_id;
  updated_external_consent_permissions.policy_app_id = application_id;
  updated_user_permissions.consent_source = source;

  cache_->SetExternalConsentForApp(updated_external_consent_permissions);
}

void PolicyManagerImpl::SendOnAppPropertiesChangeNotification(
    const std::string& policy_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  listener_->SendOnAppPropertiesChangeNotification(policy_app_id);
}

void PolicyManagerImpl::ResumePendingAppPolicyActions() {
  LOG4CXX_AUTO_TRACE(logger_);

  for (auto& notify_system_params : notify_system_list_) {
    NotifySystem(notify_system_params.first, notify_system_params.second);
  }
  notify_system_list_.clear();

  for (auto& send_permissions_params : send_permissions_list_) {
    SendPermissionsToApp(send_permissions_params.first,
                         send_permissions_params.second);
  }

  for (auto& app : app_properties_changed_list_) {
    SendOnAppPropertiesChangeNotification(app);
  }
  send_permissions_list_.clear();
}

void PolicyManagerImpl::NotifySystem(
    const std::string& device_id,
    const PolicyManagerImpl::AppPoliciesValueType& app_policy) const {
  listener()->OnPendingPermissionChange(device_id, app_policy.first);
}

void PolicyManagerImpl::SendPermissionsToApp(
    const std::string& device_id,
    const PolicyManagerImpl::AppPoliciesValueType& app_policy) {
  const std::string app_id = app_policy.first;

  if (device_id.empty()) {
    LOG4CXX_WARN(logger_,
                 "Couldn't find device info for application id: " << app_id);
    return;
  }
  std::vector<FunctionalGroupPermission> group_permissons;
  GetPermissionsForApp(device_id, app_id, group_permissons);

  Permissions notification_data;

  // Need to get rid of this call
  std::shared_ptr<policy_table::Table> policy_table_snapshot =
      cache_->GenerateSnapshot();

  PrepareNotificationData(
      policy_table_snapshot->policy_table.functional_groupings,
      app_policy.second.groups,
      group_permissons,
      notification_data);

  LOG4CXX_INFO(logger_, "Send notification for application_id: " << app_id);
  listener()->OnPermissionsUpdated(
      device_id,
      app_id,
      notification_data,
      policy_table::EnumToJsonString(app_policy.second.default_hmi));
}

void PolicyManagerImpl::ProcessExternalConsentStatusUpdate(
    const GroupsByExternalConsentStatus& groups_by_status,
    const ConsentProcessingPolicy processing_policy) {
  GroupsNames allowed_groups;
  GroupsNames disallowed_groups;
  CalculateGroupsConsentFromExternalConsent(
      groups_by_status, allowed_groups, disallowed_groups);

  std::map<std::string, std::string> known_links =
      cache_->GetKnownLinksFromPT();
  std::map<std::string, std::string> registered_links;
  listener_->GetRegisteredLinks(registered_links);

  std::map<std::string, std::string> all_known;
  std::merge(known_links.begin(),
             known_links.end(),
             registered_links.begin(),
             registered_links.end(),
             std::inserter(all_known, all_known.begin()));

  std::map<std::string, std::string>::const_iterator it_links =
      all_known.begin();
  for (; all_known.end() != it_links; ++it_links) {
    UpdateAppConsentWithExternalConsent(it_links->first,
                                        it_links->second,
                                        allowed_groups,
                                        disallowed_groups,
                                        processing_policy);
  }
}

bool ConsentStatusComparatorFunc(const ExternalConsentStatusItem& i1,
                                 const ExternalConsentStatusItem& i2) {
  return (i1.entity_id_ < i2.entity_id_) ||
         (i1.entity_type_ < i2.entity_type_) || (i1.status_ < i2.status_);
}

bool PolicyManagerImpl::IsNeedToUpdateExternalConsentStatus(
    const ExternalConsentStatus& new_status) const {
  LOG4CXX_AUTO_TRACE(logger_);
  typedef std::vector<ExternalConsentStatusItem> ItemV;
  const ExternalConsentStatus existing_status =
      cache_->GetExternalConsentEntities();

  ItemV new_status_v(new_status.begin(), new_status.end());
  ItemV existing_status_v(existing_status.begin(), existing_status.end());

  ItemV difference_v;
  difference_v.resize(new_status_v.size() + existing_status_v.size());

  ItemV::iterator ci = difference_v.begin();
  ci = std::set_difference(new_status_v.begin(),
                           new_status_v.end(),
                           existing_status_v.begin(),
                           existing_status_v.end(),
                           difference_v.begin(),
                           ConsentStatusComparatorFunc);
  difference_v.resize(ci - difference_v.begin());

  return !difference_v.empty();
}

bool PolicyManagerImpl::SetExternalConsentStatus(
    const ExternalConsentStatus& status) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (status.empty()) {
    LOG4CXX_INFO(logger_, "External consent status is empty, skipping update.");
    return false;
  }

  if (!cache_->SetExternalConsentStatus(status)) {
    LOG4CXX_WARN(logger_, "Can't set external user consent status.");
    return false;
  }

  GroupsByExternalConsentStatus groups_by_status =
      cache_->GetGroupsWithSameEntities(status);
  ProcessExternalConsentStatusUpdate(
      groups_by_status, ConsentProcessingPolicy::kExternalConsentBased);

  return true;
}

ExternalConsentStatus PolicyManagerImpl::GetExternalConsentStatus() {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetExternalConsentStatus();
}

void PolicyManagerImpl::CalculateGroupsConsentFromExternalConsent(
    const GroupsByExternalConsentStatus& groups_by_external_consent,
    GroupsNames& out_allowed_groups,
    GroupsNames& out_disallowed_groups) const {
  LOG4CXX_AUTO_TRACE(logger_);
  GroupSorter sorter(out_allowed_groups, out_disallowed_groups);
  std::for_each(groups_by_external_consent.begin(),
                groups_by_external_consent.end(),
                sorter);

  GroupsNames filtered_allowed_groups;
  std::set_difference(
      out_allowed_groups.begin(),
      out_allowed_groups.end(),
      out_disallowed_groups.begin(),
      out_disallowed_groups.end(),
      std::inserter(filtered_allowed_groups, filtered_allowed_groups.begin()));

  out_allowed_groups = filtered_allowed_groups;
}

bool PolicyManagerImpl::ExceededDays() {
  LOG4CXX_AUTO_TRACE(logger_);

  date_time::TimeDuration current_time = date_time::getCurrentTime();
  const int kSecondsInDay = 60 * 60 * 24;
  const int days = date_time::getSecs(current_time) / kSecondsInDay;

  DCHECK(std::numeric_limits<uint16_t>::max() >= days);

  if (std::numeric_limits<uint16_t>::max() <= days) {
    LOG4CXX_WARN(logger_, "The days since epoch exceeds maximum value.");
    return false;
  }
  return 0 == cache_->DaysBeforeExchange(static_cast<uint16_t>(days));
}

void PolicyManagerImpl::KmsChanged(int kilometers) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (0 == cache_->KilometersBeforeExchange(kilometers)) {
    LOG4CXX_INFO(logger_, "Enough kilometers passed to send for PT update.");
    update_status_manager_.ScheduleUpdate();
    StartPTExchange();
    PTUpdatedAt(KILOMETERS, kilometers);
  }
}

const boost::optional<bool> PolicyManagerImpl::LockScreenDismissalEnabledState()
    const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->LockScreenDismissalEnabledState();
}

const boost::optional<std::string>
PolicyManagerImpl::LockScreenDismissalWarningMessage(
    const std::string& language) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->LockScreenDismissalWarningMessage(language);
}

void PolicyManagerImpl::IncrementIgnitionCycles() {
  cache_->IncrementIgnitionCycles();
}

std::string PolicyManagerImpl::ForcePTExchange() {
  update_status_manager_.ScheduleUpdate();
  StartPTExchange();
  return update_status_manager_.StringifiedUpdateStatus();
}

void policy::PolicyManagerImpl::StopRetrySequence() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (cache_->UpdateRequired()) {
    ResetRetrySequence(ResetRetryCountType::kResetWithStatusUpdate);
  }
}

std::string PolicyManagerImpl::ForcePTExchangeAtUserRequest() {
  LOG4CXX_AUTO_TRACE(logger_);
  update_status_manager_.ScheduleManualUpdate();
  StartPTExchange();
  return update_status_manager_.StringifiedUpdateStatus();
}

std::string PolicyManagerImpl::GetPolicyTableStatus() const {
  return update_status_manager_.StringifiedUpdateStatus();
}

int PolicyManagerImpl::NextRetryTimeout() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  LOG4CXX_DEBUG(logger_, "Index: " << retry_sequence_index_);
  int next = 0;

  if (!retry_sequence_seconds_.empty() &&
      retry_sequence_index_ < retry_sequence_seconds_.size()) {
    next = retry_sequence_seconds_[retry_sequence_index_] *
           date_time::MILLISECONDS_IN_SECOND;
  }
  return next;
}

void PolicyManagerImpl::RefreshRetrySequence() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  retry_sequence_timeout_ = cache_->TimeoutResponse();
  retry_sequence_seconds_.clear();
  cache_->SecondsBetweenRetries(retry_sequence_seconds_);
}

void PolicyManagerImpl::ResetRetrySequence(
    const ResetRetryCountType reset_type) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  retry_sequence_index_ = 0;
  is_ptu_in_progress_ = false;
  if (ResetRetryCountType::kResetWithStatusUpdate == reset_type) {
    update_status_manager_.OnResetRetrySequence();
  }
}

uint32_t PolicyManagerImpl::TimeoutExchangeMSec() {
  return retry_sequence_timeout_;
}

const std::vector<int> PolicyManagerImpl::RetrySequenceDelaysSeconds() {
  sync_primitives::AutoLock auto_lock(retry_sequence_lock_);
  return retry_sequence_seconds_;
}

void PolicyManagerImpl::OnExceededTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);

  update_status_manager_.OnUpdateTimeoutOccurs();
}

void PolicyManagerImpl::OnUpdateStarted() {
  uint32_t update_timeout = TimeoutExchangeMSec();
  LOG4CXX_DEBUG(logger_,
                "Update timeout will be set to (milisec): " << update_timeout);
  send_on_update_sent_out_ =
      policy::kUpdating != update_status_manager_.StringifiedUpdateStatus();

  if (send_on_update_sent_out_) {
    update_status_manager_.OnUpdateSentOut(update_timeout);
  }
  cache_->SaveUpdateRequired(true);
}

void PolicyManagerImpl::PTUpdatedAt(Counters counter, int value) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->SetCountersPassedForSuccessfulUpdate(counter, value);
  cache_->ResetIgnitionCycles();
}

void PolicyManagerImpl::Increment(usage_statistics::GlobalCounterId type) {
  LOG4CXX_INFO(logger_, "Increment without app id");
  cache_->Increment(type);
}

void PolicyManagerImpl::Increment(const std::string& app_id,
                                  usage_statistics::AppCounterId type) {
  LOG4CXX_DEBUG(logger_, "Increment " << app_id << " AppCounter: " << type);
  cache_->Increment(app_id, type);
}

void PolicyManagerImpl::Set(const std::string& app_id,
                            usage_statistics::AppInfoId type,
                            const std::string& value) {
  LOG4CXX_INFO(logger_, "Set " << app_id);
  cache_->Set(app_id, type, value);
}

void PolicyManagerImpl::Add(const std::string& app_id,
                            usage_statistics::AppStopwatchId type,
                            int32_t timespan_seconds) {
  LOG4CXX_INFO(logger_, "Add " << app_id);
  cache_->Add(app_id, type, timespan_seconds);
}

bool PolicyManagerImpl::IsApplicationRevoked(const std::string& app_id) const {
  return cache_->IsApplicationRevoked(app_id);
}

bool PolicyManagerImpl::IsConsentNeeded(const std::string& device_id,
                                        const std::string& app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  const int count = cache_->CountUnconsentedGroups(app_id, device_id);
  LOG4CXX_DEBUG(logger_, "There are: " << count << " unconsented groups.");
  return count != 0;
}

void PolicyManagerImpl::SetVINValue(const std::string& value) {
  cache_->SetVINValue(value);
}

AppPermissions PolicyManagerImpl::GetAppPermissionsChanges(
    const std::string& device_id, const std::string& policy_app_id) {
  PendingPermissions::iterator app_id_diff =
      app_permissions_diff_.find(policy_app_id);

  AppPermissions permissions(policy_app_id);

  if (app_permissions_diff_.end() != app_id_diff) {
    permissions = app_id_diff->second;
  } else {
    permissions.appPermissionsConsentNeeded =
        IsConsentNeeded(device_id, policy_app_id);
    permissions.appRevoked = IsApplicationRevoked(policy_app_id);
    permissions.isSDLAllowed = false;
    GetPriority(permissions.application_id, &permissions.priority);
  }
  return permissions;
}

void PolicyManagerImpl::RemovePendingPermissionChanges(
    const std::string& app_id) {
  app_permissions_diff_.erase(app_id);
}

bool PolicyManagerImpl::CanAppKeepContext(const std::string& app_id) const {
  return cache_->CanAppKeepContext(app_id);
}

bool PolicyManagerImpl::CanAppStealFocus(const std::string& app_id) const {
  return cache_->CanAppStealFocus(app_id);
}

void PolicyManagerImpl::MarkUnpairedDevice(const std::string& device_id) {
  if (!cache_->SetUnpairedDevice(device_id)) {
    LOG4CXX_DEBUG(logger_, "Could not set unpaired flag for " << device_id);
    return;
  }
  SetUserConsentForDevice(device_id, false);
}

void PolicyManagerImpl::OnAppRegisteredOnMobile(
    const std::string& device_id, const std::string& application_id) {
  if (!is_ptu_in_progress_) {
    StartPTExchange();
  }
  SendNotificationOnPermissionsUpdated(device_id, application_id);
}

void PolicyManagerImpl::OnDeviceSwitching(const std::string& device_id_from,
                                          const std::string& device_id_to) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->OnDeviceSwitching(device_id_from, device_id_to);
}

const MetaInfo PolicyManagerImpl::GetMetaInfo() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetMetaInfo();
}

std::string PolicyManagerImpl::RetrieveCertificate() const {
  LOG4CXX_AUTO_TRACE(logger_);
  return cache_->GetCertificate();
}

bool PolicyManagerImpl::HasCertificate() const {
  return !cache_->GetCertificate().empty();
}

void PolicyManagerImpl::SetDecryptedCertificate(
    const std::string& certificate) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->SetDecryptedCertificate(certificate);
}

AppIdURL PolicyManagerImpl::GetNextUpdateUrl(const EndpointUrls& urls) {
  LOG4CXX_AUTO_TRACE(logger_);

  const AppIdURL next_app_url = RetrySequenceUrl(retry_sequence_url_, urls);

  retry_sequence_url_.url_idx_ = next_app_url.second + 1;
  retry_sequence_url_.app_idx_ = next_app_url.first;
  retry_sequence_url_.policy_app_id_ = urls[next_app_url.first].app_id;

  return next_app_url;
}

AppIdURL PolicyManagerImpl::RetrySequenceUrl(const struct RetrySequenceURL& rs,
                                             const EndpointUrls& urls) const {
  uint32_t url_idx = rs.url_idx_;
  uint32_t app_idx = rs.app_idx_;
  const std::string& app_id = rs.policy_app_id_;

  if (urls.size() <= app_idx) {
    // Index of current application doesn't exist any more due to app(s)
    // unregistration
    url_idx = 0;
    app_idx = 0;
  } else if (urls[app_idx].app_id != app_id) {
    // Index of current application points to another one due to app(s)
    // registration/unregistration
    url_idx = 0;
  } else if (url_idx >= urls[app_idx].url.size()) {
    // Index of current application is OK, but all of its URL are sent,
    // move to the next application
    url_idx = 0;
    if (++app_idx >= urls.size()) {
      app_idx = 0;
    }
  }
  const AppIdURL next_app_url = std::make_pair(app_idx, url_idx);

  return next_app_url;
}

/**
 * @brief The CallStatusChange class notify update manager aboun new application
 */
class CallStatusChange : public utils::Callable {
 public:
  CallStatusChange(UpdateStatusManager& upd_manager,
                   const DeviceConsent& device_consent)
      : upd_manager_(upd_manager), device_consent_(device_consent) {}

  // Callable interface
  void operator()() const {
    upd_manager_.OnNewApplicationAdded(device_consent_);
  }

 private:
  UpdateStatusManager& upd_manager_;
  const DeviceConsent device_consent_;
};

StatusNotifier PolicyManagerImpl::AddApplication(
    const std::string& device_id,
    const std::string& application_id,
    const rpc::policy_table_interface_base::AppHmiTypes& hmi_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  auto device_consent = GetUserConsentForDevice(device_id);
  LOG4CXX_DEBUG(logger_,
                "check_device_id: " << device_id << " check_device_consent: "
                                    << device_consent);
  sync_primitives::AutoLock lock(apps_registration_lock_);
  if (IsNewApplication(application_id)) {
    LOG4CXX_DEBUG(logger_, "Adding new application");
    AddNewApplication(device_id, application_id, device_consent);
    return std::make_shared<CallStatusChange>(update_status_manager_,
                                              device_consent);
  }
  LOG4CXX_DEBUG(logger_, "Promote existed application");
  PromoteExistedApplication(device_id, application_id, device_consent);
  if (!ptu_requested_) {
    update_status_manager_.OnExistedApplicationAdded(cache_->UpdateRequired());
  }
  return std::make_shared<utils::CallNothing>();
}

void PolicyManagerImpl::RemoveAppConsentForGroup(
    const std::string& app_id, const std::string& group_name) {
  cache_->RemoveAppConsentForGroup(app_id, group_name);
}

bool PolicyManagerImpl::IsPredataPolicy(
    const std::string& policy_app_id) const {
  LOG4CXX_INFO(logger_, "IsPredataApp");
  return cache_->IsPredataPolicy(policy_app_id);
}

void PolicyManagerImpl::ProcessExternalConsentStatusForApp(
    const std::string& device_id,
    const std::string& application_id,
    const ConsentProcessingPolicy processing_policy) {
  ExternalConsentStatus status = cache_->GetExternalConsentStatus();
  GroupsByExternalConsentStatus groups_by_status =
      cache_->GetGroupsWithSameEntities(status);

  GroupsNames allowed_groups;
  GroupsNames disallowed_groups;
  CalculateGroupsConsentFromExternalConsent(
      groups_by_status, allowed_groups, disallowed_groups);

  LOG4CXX_DEBUG(logger_, "check device_id: " << device_id);
  UpdateAppConsentWithExternalConsent(device_id,
                                      application_id,
                                      allowed_groups,
                                      disallowed_groups,
                                      processing_policy);
}

void PolicyManagerImpl::AddNewApplication(const std::string& device_id,
                                          const std::string& application_id,
                                          DeviceConsent device_consent) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (kDeviceHasNoConsent == device_consent ||
      kDeviceDisallowed == device_consent) {
    LOG4CXX_INFO(logger_,
                 "Setting "
                     << policy::kPreDataConsentId
                     << " permissions for application id: " << application_id);
    cache_->SetPredataPolicy(application_id);
  } else {
    LOG4CXX_INFO(logger_,
                 "Setting "
                     << policy::kDefaultId
                     << " permissions for application id: " << application_id);
    cache_->SetDefaultPolicy(application_id);
  }

  ProcessExternalConsentStatusForApp(
      device_id,
      application_id,
      ConsentProcessingPolicy::kExternalConsentBased);
}

void PolicyManagerImpl::PromoteExistedApplication(
    const std::string& device_id,
    const std::string& application_id,
    DeviceConsent device_consent) {
  // If device consent changed to allowed during application being
  // disconnected, app permissions should be changed also
  LOG4CXX_DEBUG(logger_,
                "kDeviceAllowed == device_consent: "
                    << (kDeviceAllowed == device_consent)
                    << " device_consent: " << device_consent);
  if (kDeviceAllowed == device_consent &&
      cache_->IsPredataPolicy(application_id)) {
    LOG4CXX_INFO(logger_,
                 "Setting "
                     << policy::kDefaultId
                     << " permissions for application id: " << application_id);
    cache_->SetDefaultPolicy(application_id);
  }
  ProcessExternalConsentStatusForApp(
      device_id, application_id, ConsentProcessingPolicy::kTimestampBased);
}

bool PolicyManagerImpl::IsNewApplication(
    const std::string& application_id) const {
  return false == cache_->IsApplicationRepresented(application_id);
}

bool PolicyManagerImpl::ResetPT(const std::string& file_name) {
  LOG4CXX_AUTO_TRACE(logger_);
  cache_->ResetCalculatedPermissions();
  const bool result = cache_->ResetPT(file_name);
  if (result) {
    RefreshRetrySequence();
  }
  return result;
}

bool PolicyManagerImpl::CheckAppStorageFolder() const {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string app_storage_folder = get_settings().app_storage_folder();
  LOG4CXX_DEBUG(logger_, "AppStorageFolder " << app_storage_folder);
  if (!file_system::DirectoryExists(app_storage_folder)) {
    LOG4CXX_WARN(logger_,
                 "Storage directory doesn't exist " << app_storage_folder);
    return false;
  }
  if (!(file_system::IsWritingAllowed(app_storage_folder) &&
        file_system::IsReadingAllowed(app_storage_folder))) {
    LOG4CXX_WARN(logger_,
                 "Storage directory doesn't have read/write permissions "
                     << app_storage_folder);
    return false;
  }
  return true;
}

bool PolicyManagerImpl::InitPT(const std::string& file_name,
                               const PolicySettings* settings) {
  LOG4CXX_AUTO_TRACE(logger_);
  settings_ = settings;
  if (!CheckAppStorageFolder()) {
    LOG4CXX_ERROR(logger_, "Can not read/write into AppStorageFolder");
    return false;
  }
  const bool ret = cache_->Init(file_name, settings);
  if (ret) {
    RefreshRetrySequence();
    std::vector<std::string> enabled_cloud_apps;
    cache_->GetEnabledCloudApps(enabled_cloud_apps);
    for (auto app : enabled_cloud_apps) {
      SendAuthTokenUpdated(app);
    }
  }
  return ret;
}

uint32_t PolicyManagerImpl::HeartBeatTimeout(const std::string& app_id) const {
  return cache_->HeartBeatTimeout(app_id);
}

void PolicyManagerImpl::SaveUpdateStatusRequired(bool is_update_needed) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!is_update_needed) {
    ResetRetrySequence(ResetRetryCountType::kResetInternally);
  }
  cache_->SaveUpdateRequired(is_update_needed);
}

void PolicyManagerImpl::set_cache_manager(
    CacheManagerInterface* cache_manager) {
  cache_ = std::shared_ptr<CacheManagerInterface>(cache_manager);
}

std::ostream& operator<<(std::ostream& output,
                         const policy_table::Strings& groups) {
  for (policy_table::Strings::const_iterator i = groups.begin();
       i != groups.end();
       ++i) {
    output << static_cast<std::string>(*i) << " ";
  }
  return output;
}

void PolicyManagerImpl::SetDefaultHmiTypes(
    const transport_manager::DeviceHandle& device_handle,
    const std::string& application_id,
    const std::vector<int>& hmi_types) {
  LOG4CXX_INFO(logger_, "SetDefaultHmiTypes");
  const auto device_id = GetCurrentDeviceId(device_handle, application_id);
  ApplicationOnDevice who = {device_id, application_id};
  access_remote_->SetDefaultHmiTypes(who, hmi_types);
}

struct HMITypeToInt {
  int operator()(const policy_table::AppHMITypes::value_type item) {
    return policy_table::AppHMIType(item);
  }
};

bool PolicyManagerImpl::GetHMITypes(const std::string& application_id,
                                    std::vector<int>* app_types) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (cache_->IsDefaultPolicy(application_id)) {
    return false;
  }
  const policy_table::AppHMITypes* hmi_types =
      cache_->GetHMITypes(application_id);
  if (hmi_types) {
    std::transform(hmi_types->begin(),
                   hmi_types->end(),
                   std::back_inserter(*app_types),
                   HMITypeToInt());
  }
  return hmi_types;
}

bool PolicyManagerImpl::CheckModule(const PTString& app_id,
                                    const PTString& module) {
  LOG4CXX_AUTO_TRACE(logger_);
  policy_table::ModuleType module_type;
  return EnumFromJsonString(module, &module_type) &&
         access_remote_->CheckModuleType(app_id, module_type);
}

void PolicyManagerImpl::SendHMILevelChanged(const ApplicationOnDevice& who) {
  std::string default_hmi("NONE");
  if (GetDefaultHmi(who.dev_id, who.app_id, &default_hmi)) {
    listener()->OnUpdateHMIStatus(who.dev_id, who.app_id, default_hmi);
  } else {
    LOG4CXX_WARN(
        logger_,
        "Couldn't get default HMI level for application " << who.app_id);
  }
}

void PolicyManagerImpl::GetPermissions(const std::string device_id,
                                       const std::string application_id,
                                       Permissions* data) {
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK(data);
  std::vector<FunctionalGroupPermission> app_group_permissions;
  GetPermissionsForApp(device_id, application_id, app_group_permissions);

  policy_table::FunctionalGroupings functional_groupings;
  cache_->GetFunctionalGroupings(functional_groupings);

  policy_table::Strings app_groups;
  std::vector<FunctionalGroupPermission>::const_iterator it =
      app_group_permissions.begin();
  std::vector<FunctionalGroupPermission>::const_iterator it_end =
      app_group_permissions.end();
  for (; it != it_end; ++it) {
    app_groups.push_back((*it).group_name);
  }

  PrepareNotificationData(
      functional_groupings, app_groups, app_group_permissions, *data);
}

void PolicyManagerImpl::SendAppPermissionsChanged(
    const std::string& device_id, const std::string& application_id) {
  Permissions notification_data;
  GetPermissions(device_id, application_id, &notification_data);
  listener()->OnPermissionsUpdated(
      device_id, application_id, notification_data);
}

void PolicyManagerImpl::SendAuthTokenUpdated(const std::string policy_app_id) {
  AppProperties app_properties;
  cache_->GetAppProperties(policy_app_id, app_properties);
  listener_->OnAuthTokenUpdated(policy_app_id, app_properties.auth_token);
}

void PolicyManagerImpl::OnPrimaryGroupsChanged(
    const std::string& application_id) {
  const std::vector<std::string> devices =
      listener()->GetDevicesIds(application_id);
  for (std::vector<std::string>::const_iterator i = devices.begin();
       i != devices.end();
       ++i) {
    const ApplicationOnDevice who = {*i, application_id};
    if (access_remote_->IsAppRemoteControl(who)) {
      SendAppPermissionsChanged(who.dev_id, who.app_id);
    }
  }
}

bool PolicyManagerImpl::GetModuleTypes(
    const std::string& application_id,
    std::vector<std::string>* modules) const {
  return access_remote_->GetModuleTypes(application_id, modules);
}

void PolicyManagerImpl::set_access_remote(
    std::shared_ptr<AccessRemote> access_remote) {
  access_remote_ = access_remote;
}

bool PolicyManagerImpl::AppNeedEncryption(
    const std::string& policy_app_id) const {
  const auto encryption_required =
      cache_->GetAppEncryptionRequiredFlag(policy_app_id);

  return encryption_required.is_initialized() ? *encryption_required : true;
}

const rpc::Optional<rpc::Boolean> PolicyManagerImpl::GetAppEncryptionRequired(
    const std::string& policy_app_id) const {
  return cache_->GetAppEncryptionRequiredFlag(policy_app_id);
}

const std::vector<std::string> PolicyManagerImpl::GetFunctionalGroupsForApp(
    const std::string& policy_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace rpc::policy_table_interface_base;
  DCHECK(kDeviceId != policy_app_id);

  std::vector<std::string> function_groups;

  ApplicationParams app_params;
  cache_->GetApplicationParams(policy_app_id, app_params);

  const auto& policy_function_group = app_params.groups;

  for (const auto& group : policy_function_group) {
    function_groups.push_back(group);
  }

  return function_groups;
}

const std::vector<std::string> PolicyManagerImpl::GetApplicationPolicyIDs()
    const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<std::string> policy_app_ids;

  const auto apps = cache_->GetPolicyAppIDs();

  for (const auto& app : apps) {
    policy_app_ids.push_back(app);
  }

  return policy_app_ids;
}

bool PolicyManagerImpl::FunctionGroupNeedEncryption(
    const std::string& policy_group) const {
  LOG4CXX_AUTO_TRACE(logger_);
  FunctionalGroupings functional_groupings;
  cache_->GetFunctionalGroupings(functional_groupings);

  const auto& grouping_itr = functional_groupings.find(policy_group);

  if (grouping_itr == functional_groupings.end()) {
    LOG4CXX_WARN(logger_, "Group " << policy_group << " not found");
    return false;
  }

  const auto& grouping = (*grouping_itr).second;

  return grouping.encryption_required.is_initialized()
             ? *grouping.encryption_required
             : false;
}

const std::string PolicyManagerImpl::GetPolicyFunctionName(
    const uint32_t function_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return policy_table::EnumToJsonString(
      static_cast<policy_table::FunctionID>(function_id));
}

const std::vector<std::string> PolicyManagerImpl::GetRPCsForFunctionGroup(
    const std::string& group) const {
  std::vector<std::string> rpcs_for_group;

  FunctionalGroupings functional_groupings;
  cache_->GetFunctionalGroupings(functional_groupings);

  const auto& rpcs = functional_groupings[group].rpcs;

  for (const auto& rpc : rpcs) {
    rpcs_for_group.push_back(rpc.first);
  }

  return rpcs_for_group;
}

}  //  namespace policy

__attribute__((visibility("default"))) policy::PolicyManager* CreateManager() {
  return new policy::PolicyManagerImpl();
}

__attribute__((visibility("default"))) void DeleteManager(
    policy::PolicyManager* pm) {
  delete pm;
  DELETE_THREAD_LOGGER(policy::logger_);
}

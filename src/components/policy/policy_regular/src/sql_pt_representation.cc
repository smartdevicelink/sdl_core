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

#include <sstream>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <unistd.h>

#include "utils/logger.h"
#include "utils/date_time.h"
#include "utils/file_system.h"
#include "utils/gen_hash.h"
#include "policy/sql_pt_representation.h"
#include "policy/sql_wrapper.h"
#include "policy/sql_pt_queries.h"
#include "policy/policy_helper.h"
#include "policy/cache_manager.h"
#include "config_profile/profile.h"

namespace policy {

CREATE_LOGGERPTR_GLOBAL(logger_, "Policy")

namespace {
template <typename T, typename K>
void InsertUnique(K value, T* array) {
  uint32_t i = 0;
  for (; i < array->size() && array->at(i) != value; ++i) {
    continue;
  }
  if (array->size() == i) {
    array->push_back(value);
  }
}
}  //  namespace

const std::string SQLPTRepresentation::kDatabaseName = "policy";

SQLPTRepresentation::SQLPTRepresentation()
    : db_(new utils::dbms::SQLDatabase(kDatabaseName)) {}

SQLPTRepresentation::~SQLPTRepresentation() {
  db_->Close();
  delete db_;
}

void SQLPTRepresentation::CheckPermissions(const PTString& app_id,
                                           const PTString& hmi_level,
                                           const PTString& rpc,
                                           CheckPermissionResult& result) {
  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(sql_pt::kSelectRpc)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect select statement from rpcs"
                     << query.LastError().text());
    return;
  }
  query.Bind(0, app_id);
  query.Bind(1, hmi_level);
  query.Bind(2, rpc);

  bool ret = query.Next();
  result.hmi_level_permitted = ret ? kRpcAllowed : kRpcDisallowed;
  LOG4CXX_INFO(logger_,
               "Level is " << (result.hmi_level_permitted == kRpcAllowed
                                   ? "permitted"
                                   : "not permitted"));
  std::string parameter;
  while (ret) {
    if (!query.IsNull(0)) {
      parameter = query.GetString(0);
      result.list_of_allowed_params.insert(parameter);
    }
    ret = query.Next();
  }
}

bool SQLPTRepresentation::IsPTPreloaded() {
  utils::dbms::SQLQuery query(db());
  return query.Prepare(sql_pt::kSelectPreloaded) && query.Next();
}

int SQLPTRepresentation::IgnitionCyclesBeforeExchange() {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectIgnitionCycles) || !query.Exec()) {
    LOG4CXX_WARN(logger_, "Can not select ignition cycles");
    return 0;
  }
  int limit = query.GetInteger(0);
  int current = query.GetInteger(1);

  if (limit < 0 || current < 0 || current > limit) {
    return 0;
  }

  return limit - current;
}

int SQLPTRepresentation::KilometersBeforeExchange(int current) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectKilometers) || !query.Exec()) {
    LOG4CXX_WARN(logger_, "Can not select kilometers");
    return 0;
  }
  int limit = query.GetInteger(0);
  int last = query.GetInteger(1);

  if (limit < 0 || last < 0 || current < 0 || current < last ||
      limit < (current - last)) {
    return 0;
  }

  return limit - (current - last);
}

bool SQLPTRepresentation::SetCountersPassedForSuccessfulUpdate(
    int kilometers, int days_after_epoch) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateCountersSuccessfulUpdate)) {
    LOG4CXX_WARN(logger_,
                 "Wrong update query for counters on successful update.");
    return false;
  }
  query.Bind(0, kilometers);
  query.Bind(1, days_after_epoch);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed to update counters on successful update.");
    return false;
  }
  return true;
}

int SQLPTRepresentation::DaysBeforeExchange(int current) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectDays) || !query.Exec()) {
    LOG4CXX_WARN(logger_, "Can not select days");
    return 0;
  }
  int limit = query.GetInteger(0);
  int last = query.GetInteger(1);

  if (0 == last) {
    return limit;
  }

  if (limit < 0 || last < 0 || current < 0 || current < last ||
      limit < (current - last)) {
    return 0;
  }

  return limit - (current - last);
}

int SQLPTRepresentation::TimeoutResponse() {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectTimeoutResponse) || !query.Exec()) {
    LOG4CXX_INFO(logger_, "Can not select timeout response for retry sequence");
    const int defaultTimeout = 30 * date_time::DateTime::MILLISECONDS_IN_SECOND;
    return defaultTimeout;
  }
  return query.GetInteger(0) * date_time::DateTime::MILLISECONDS_IN_SECOND;
}

bool SQLPTRepresentation::SecondsBetweenRetries(std::vector<int>* seconds) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectSecondsBetweenRetries)) {
    LOG4CXX_INFO(logger_,
                 "Incorrect select statement from seconds between retries");
    return false;
  }
  while (query.Next()) {
    seconds->push_back(query.GetInteger(0));
  }
  return true;
}

std::vector<UserFriendlyMessage> SQLPTRepresentation::GetUserFriendlyMsg(
    const std::vector<std::string>& msg_codes, const std::string& language) {
  std::vector<UserFriendlyMessage> result;
  std::vector<std::string>::const_iterator it = msg_codes.begin();
  std::vector<std::string>::const_iterator it_end = msg_codes.end();
  for (; it != it_end; ++it) {
    UserFriendlyMessage msg;
    msg.message_code = *it;
    result.push_back(msg);
  }
  return result;
}

EndpointUrls SQLPTRepresentation::GetUpdateUrls(int service_type) {
  LOG4CXX_INFO(logger_,
               "SQLPTRepresentation::GetUpdateUrls for " << service_type);
  utils::dbms::SQLQuery query(db());
  EndpointUrls ret;
  if (query.Prepare(sql_pt::kSelectEndpoint)) {
    query.Bind(0, service_type);
    while (query.Next()) {
      EndpointData data;

      data.url.push_back(query.GetString(0));
      if (!query.IsNull(1)) {
        data.app_id = query.GetString(1);
      }
      ret.push_back(data);
    }
  } else {
    LOG4CXX_WARN(logger_, "Invalid select endpoints statement.");
  }
  return ret;
}

std::string SQLPTRepresentation::GetLockScreenIconUrl() const {
  utils::dbms::SQLQuery query(db());
  std::string ret;
  if (query.Prepare(sql_pt::kSelectLockScreenIcon)) {
    query.Bind(0, std::string("lock_screen_icon_url"));
    query.Bind(1, std::string("default"));

    if (!query.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect select from notifications by priority.");
      return ret;
    }

    if (!query.IsNull(0)) {
      ret = query.GetString(0);
    }

  } else {
    LOG4CXX_WARN(logger_, "Invalid select endpoints statement.");
  }
  return ret;
}

int SQLPTRepresentation::GetNotificationsNumber(const std::string& priority) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectNotificationsPerPriority)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect select statement for priority "
                 "notification number.");
    return 0;
  }
  query.Bind(0, priority);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect select from notifications by priority.");
    return 0;
  }

  if (!query.IsNull(0)) {
    return query.GetInteger(0);
  }

  return 0;
}

bool SQLPTRepresentation::GetPriority(const std::string& policy_app_id,
                                      std::string* priority) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (NULL == priority) {
    LOG4CXX_WARN(logger_, "Input priority parameter is null.");
    return false;
  }
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectPriority)) {
    LOG4CXX_INFO(logger_, "Incorrect statement for priority.");
    return false;
  }

  query.Bind(0, policy_app_id);

  if (!query.Exec()) {
    LOG4CXX_INFO(logger_, "Error during select priority.");
    return false;
  }

  if (query.IsNull(0)) {
    priority->clear();
    return true;
  }

  priority->assign(query.GetString(0));

  return true;
}

InitResult SQLPTRepresentation::Init(const PolicySettings* settings) {
  settings_ = settings;
  LOG4CXX_AUTO_TRACE(logger_);
#ifdef BUILD_TESTS
  open_counter_ = 0;
#endif  // BUILD_TESTS
  std::string path = get_settings().app_storage_folder();
  if (!path.empty()) {
    db_->set_path(path + "/");
  }
  if (!db_->Open()) {
    LOG4CXX_ERROR(logger_, "Failed opening database.");
    LOG4CXX_INFO(logger_, "Starting opening retries.");
    const uint16_t attempts = get_settings().attempts_to_open_policy_db();
    LOG4CXX_DEBUG(logger_, "Total attempts number is: " << attempts);
    bool is_opened = false;
    const uint16_t open_attempt_timeout_ms =
        get_settings().open_attempt_timeout_ms();
    const useconds_t sleep_interval_mcsec = open_attempt_timeout_ms * 1000;
    LOG4CXX_DEBUG(logger_,
                  "Open attempt timeout(ms) is: " << open_attempt_timeout_ms);
    for (int i = 0; i < attempts; ++i) {
      usleep(sleep_interval_mcsec);
      LOG4CXX_INFO(logger_, "Attempt: " << i + 1);
#ifdef BUILD_TESTS
      ++open_counter_;
#endif  // BUILD_TESTS
      if (db_->Open()) {
        LOG4CXX_INFO(logger_, "Database opened.");
        is_opened = true;
        break;
      }
    }
    if (!is_opened) {
      LOG4CXX_ERROR(logger_,
                    "Open retry sequence failed. Tried "
                        << attempts << " attempts with "
                        << open_attempt_timeout_ms
                        << " open timeout(ms) for each.");
      return InitResult::FAIL;
    }
  }
#ifndef __QNX__
  if (!db_->IsReadWrite()) {
    LOG4CXX_ERROR(logger_, "There are no read/write permissions for database");
    return InitResult::FAIL;
  }

#endif  // __QNX__
  utils::dbms::SQLQuery check_pages(db());
  if (!check_pages.Prepare(sql_pt::kCheckPgNumber) || !check_pages.Next()) {
    LOG4CXX_WARN(logger_, "Incorrect pragma for page counting.");
  } else {
    if (0 < check_pages.GetInteger(0)) {
      utils::dbms::SQLQuery db_check(db());
      if (!db_check.Prepare(sql_pt::kCheckDBIntegrity)) {
        LOG4CXX_WARN(logger_, "Incorrect pragma for integrity check.");
      } else {
        while (db_check.Next()) {
          if (db_check.GetString(0).compare("ok") == 0) {
            utils::dbms::SQLQuery check_first_run(db());
            if (check_first_run.Prepare(sql_pt::kIsFirstRun) &&
                check_first_run.Next()) {
              LOG4CXX_INFO(logger_,
                           "Selecting is first run "
                               << check_first_run.GetBoolean(0));
              if (check_first_run.GetBoolean(0)) {
                utils::dbms::SQLQuery set_not_first_run(db());
                set_not_first_run.Exec(sql_pt::kSetNotFirstRun);
                return InitResult::SUCCESS;
              }
            } else {
              LOG4CXX_WARN(logger_, "Incorrect select is first run");
            }
            return InitResult::EXISTS;
          } else {
            LOG4CXX_ERROR(logger_,
                          "Existing policy table representation is invlaid.");
            // TODO(PV): add handle
            return InitResult::FAIL;
          }
        }
      }
    }
  }
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kCreateSchema)) {
    LOG4CXX_ERROR(
        logger_,
        "Failed creating schema of database: " << query.LastError().text());
    return InitResult::FAIL;
  }
  if (!query.Exec(sql_pt::kInsertInitData)) {
    LOG4CXX_ERROR(
        logger_,
        "Failed insert init data to database: " << query.LastError().text());
    return InitResult::FAIL;
  }
  return InitResult::SUCCESS;
}

bool SQLPTRepresentation::Close() {
  db_->Close();
  return db_->LastError().number() == utils::dbms::OK;
}

const VehicleInfo SQLPTRepresentation::GetVehicleInfo() const {
  policy_table::ModuleConfig module_config;
  GatherModuleConfig(&module_config);
  VehicleInfo vehicle_info;
  vehicle_info.vehicle_make = *module_config.vehicle_make;
  vehicle_info.vehicle_model = *module_config.vehicle_model;
  vehicle_info.vehicle_year = *module_config.vehicle_year;
  return vehicle_info;
}

bool SQLPTRepresentation::Drop() {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDropSchema)) {
    LOG4CXX_WARN(logger_,
                 "Failed dropping database: " << query.LastError().text());
    return false;
  }
  return true;
}

void SQLPTRepresentation::WriteDb() {
  db_->Backup();
}

bool SQLPTRepresentation::Clear() {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDeleteData)) {
    LOG4CXX_ERROR(logger_,
                  "Failed clearing database: " << query.LastError().text());
    return false;
  }
  if (!query.Exec(sql_pt::kInsertInitData)) {
    LOG4CXX_ERROR(
        logger_,
        "Failed insert init data to database: " << query.LastError().text());
    return false;
  }
  return true;
}

bool SQLPTRepresentation::RefreshDB() {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDropSchema)) {
    LOG4CXX_WARN(logger_,
                 "Failed dropping database: " << query.LastError().text());
    return false;
  }
  if (!query.Exec(sql_pt::kCreateSchema)) {
    LOG4CXX_ERROR(
        logger_,
        "Failed creating schema of database: " << query.LastError().text());
    return false;
  }
  if (!query.Exec(sql_pt::kInsertInitData)) {
    LOG4CXX_ERROR(
        logger_,
        "Failed insert init data to database: " << query.LastError().text());
    return false;
  }
  return true;
}

utils::SharedPtr<policy_table::Table> SQLPTRepresentation::GenerateSnapshot()
    const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::SharedPtr<policy_table::Table> table = new policy_table::Table();
  GatherModuleMeta(&*table->policy_table.module_meta);
  GatherModuleConfig(&table->policy_table.module_config);
  GatherUsageAndErrorCounts(&*table->policy_table.usage_and_error_counts);
  GatherDeviceData(&*table->policy_table.device_data);
  GatherFunctionalGroupings(&table->policy_table.functional_groupings);
  GatherConsumerFriendlyMessages(
      &*table->policy_table.consumer_friendly_messages);
  GatherApplicationPoliciesSection(&table->policy_table.app_policies_section);
  return table;
}

void SQLPTRepresentation::GatherModuleMeta(
    policy_table::ModuleMeta* meta) const {
  LOG4CXX_INFO(logger_, "Gather Module Meta Info");
  utils::dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt::kSelectModuleMeta) && query.Next()) {
    *meta->pt_exchanged_at_odometer_x = query.GetInteger(0);
    *meta->pt_exchanged_x_days_after_epoch = query.GetInteger(1);
    *meta->ignition_cycles_since_last_exchange = query.GetInteger(2);
  }
}

void SQLPTRepresentation::GatherModuleConfig(
    policy_table::ModuleConfig* config) const {
  LOG4CXX_INFO(logger_, "Gather Configuration Info");
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectModuleConfig) || !query.Next()) {
    LOG4CXX_WARN(logger_, "Incorrect select statement for module config");
  } else {
    *config->preloaded_pt = query.GetBoolean(0);
    config->exchange_after_x_ignition_cycles = query.GetInteger(1);
    config->exchange_after_x_kilometers = query.GetInteger(2);
    config->exchange_after_x_days = query.GetInteger(3);
    config->timeout_after_x_seconds = query.GetInteger(4);
    *config->certificate = query.GetString(5);
    *config->vehicle_make = query.GetString(6);
    *config->vehicle_model = query.GetString(7);
    *config->vehicle_year = query.GetString(8);
    *config->preloaded_date = query.GetString(9);
  }

  utils::dbms::SQLQuery endpoints(db());
  if (!endpoints.Prepare(sql_pt::kSelectEndpoints)) {
    LOG4CXX_WARN(logger_, "Incorrect select statement for endpoints");
  } else {
    while (endpoints.Next()) {
      const std::string& url = endpoints.GetString(0);
      const std::string& service = endpoints.GetString(1);
      const std::string& app_id = endpoints.GetString(2);
      config->endpoints[service][app_id].push_back(url);
    }
  }

  utils::dbms::SQLQuery notifications(db());
  if (!notifications.Prepare(sql_pt::kSelectNotificationsPerMin)) {
    LOG4CXX_WARN(logger_, "Incorrect select statement for notifications");
  } else {
    while (notifications.Next()) {
      config->notifications_per_minute_by_priority[notifications.GetString(0)] =
          notifications.GetInteger(1);
    }
  }
  utils::dbms::SQLQuery seconds(db());
  if (!seconds.Prepare(sql_pt::kSelectSecondsBetweenRetries)) {
    LOG4CXX_INFO(logger_,
                 "Incorrect select statement from seconds between retries");
  } else {
    while (seconds.Next()) {
      config->seconds_between_retries.push_back(seconds.GetInteger(0));
    }
  }
}

bool SQLPTRepresentation::GatherUsageAndErrorCounts(
    policy_table::UsageAndErrorCounts* counts) const {
  LOG4CXX_INFO(logger_, "Gather Usage and Error Counts.");
  utils::dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt::kSelectAppLevels)) {
    policy_table::AppLevel app_level;
    app_level.mark_initialized();
    while (query.Next()) {
      app_level.count_of_tls_errors = query.GetInteger(1);
      const std::string app_id = query.GetString(0);
      (*counts->app_level)[app_id] = app_level;
    }
    return true;
  }
  return false;
}

void SQLPTRepresentation::GatherDeviceData(
    policy_table::DeviceData* data) const {
  LOG4CXX_INFO(logger_, "Gather device data.");
  data->mark_initialized();

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt::kSelectDeviceData)) {
    policy_table::DeviceParams device_data;
    device_data.mark_initialized();
    while (query.Next()) {
      const std::string device_id = query.GetString(0);
      (*data)[device_id] = device_data;
    }
  }
}

bool SQLPTRepresentation::GatherFunctionalGroupings(
    policy_table::FunctionalGroupings* groups) const {
  LOG4CXX_INFO(logger_, "Gather Functional Groupings info");
  utils::dbms::SQLQuery func_group(db());
  if (!func_group.Prepare(sql_pt::kSelectFunctionalGroups)) {
    LOG4CXX_WARN(logger_, "Incorrect select from functional_groupings");
    return false;
  }
  utils::dbms::SQLQuery rpcs(db());
  if (!rpcs.Prepare(sql_pt::kSelectAllRpcs)) {
    LOG4CXX_WARN(logger_, "Incorrect select all from rpc");
    return false;
  }
  while (func_group.Next()) {
    policy_table::Rpcs rpcs_tbl;
    if (!func_group.IsNull(2)) {
      *rpcs_tbl.user_consent_prompt = func_group.GetString(2);
    }
    int func_id = func_group.GetInteger(0);
    rpcs.Bind(0, func_id);
    while (rpcs.Next()) {
      if (!rpcs.IsNull(1)) {
        policy_table::HmiLevel level;
        if (policy_table::EnumFromJsonString(rpcs.GetString(1), &level)) {
          InsertUnique(level, &rpcs_tbl.rpcs[rpcs.GetString(0)].hmi_levels);
        }
      }
      if (!rpcs.IsNull(2)) {
        policy_table::Parameter param;
        if (policy_table::EnumFromJsonString(rpcs.GetString(2), &param)) {
          InsertUnique(param, &(*rpcs_tbl.rpcs[rpcs.GetString(0)].parameters));
        }
      }
    }
    if (!rpcs_tbl.rpcs.is_initialized()) {
      rpcs_tbl.rpcs.set_to_null();
    }
    rpcs.Reset();
    (*groups)[func_group.GetString(1)] = rpcs_tbl;
  }
  return true;
}

bool SQLPTRepresentation::GatherConsumerFriendlyMessages(
    policy_table::ConsumerFriendlyMessages* messages) const {
  LOG4CXX_INFO(logger_, "Gather Consumer Friendly Messages");
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectUserMsgsVersion) || !query.Next()) {
    LOG4CXX_WARN(logger_, "Incorrect select from consumer_friendly_messages");
    return false;
  }

  messages->version = query.GetString(0);

  if (query.Prepare(sql_pt::kCollectFriendlyMsg)) {
    while (query.Next()) {
      UserFriendlyMessage msg;
      msg.message_code = query.GetString(7);
      std::string language = query.GetString(6);

      (*messages->messages)[msg.message_code].languages[language];
    }
  } else {
    LOG4CXX_WARN(logger_, "Incorrect statement for select friendly messages.");
  }

  return true;
}

bool SQLPTRepresentation::GatherApplicationPoliciesSection(
    policy_table::ApplicationPoliciesSection* policies) const {
  LOG4CXX_INFO(logger_, "Gather applications policies");
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectAppPolicies)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app_policies");
    return false;
  }

  while (query.Next()) {
    rpc::Nullable<policy_table::ApplicationParams> params;
    const std::string& app_id = query.GetString(0);
    if (IsApplicationRevoked(app_id)) {
      params.set_to_null();
      (*policies).apps[app_id] = params;
      continue;
    }
    if (IsDefaultPolicy(app_id)) {
      (*policies).apps[app_id].set_to_string(kDefaultId);
    }
    if (IsPredataPolicy(app_id)) {
      (*policies).apps[app_id].set_to_string(kPreDataConsentId);
    }
    if (kDeviceId == app_id) {
      // Priority is only SDL-specific item for device
      policy_table::Priority priority;
      policy_table::EnumFromJsonString(query.GetString(1), &priority);
      (*policies).device.priority = priority;
      continue;
    }
    policy_table::Priority priority;
    policy_table::EnumFromJsonString(query.GetString(1), &priority);
    params.priority = priority;

    *params.memory_kb = query.GetInteger(2);

    *params.heart_beat_timeout_ms = query.GetUInteger(3);
    if (!query.IsNull(3)) {
      *params.certificate = query.GetString(4);
    }
    if (!GatherAppGroup(app_id, &params.groups)) {
      return false;
    }
    if (!GatherNickName(app_id, &*params.nicknames)) {
      return false;
    }
    if (!GatherAppType(app_id, &*params.AppHMIType)) {
      return false;
    }
    if (!GatherRequestType(app_id, &*params.RequestType)) {
      return false;
    }

    (*policies).apps[app_id] = params;
  }
  return true;
}

bool SQLPTRepresentation::Save(const policy_table::Table& table) {
  LOG4CXX_AUTO_TRACE(logger_);
  db_->BeginTransaction();
  if (!SaveFunctionalGroupings(table.policy_table.functional_groupings)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveApplicationPoliciesSection(
          table.policy_table.app_policies_section)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveModuleConfig(table.policy_table.module_config)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveConsumerFriendlyMessages(
          *table.policy_table.consumer_friendly_messages)) {
    db_->RollbackTransaction();
    return false;
  }

  if (!SaveDeviceData(*table.policy_table.device_data)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveUsageAndErrorCounts(*table.policy_table.usage_and_error_counts)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!SaveModuleMeta(*table.policy_table.module_meta)) {
    db_->RollbackTransaction();
    return false;
  }
  db_->CommitTransaction();
  return true;
}

bool SQLPTRepresentation::SaveFunctionalGroupings(
    const policy_table::FunctionalGroupings& groups) {
  utils::dbms::SQLQuery query_delete(db());
  if (!query_delete.Exec(sql_pt::kDeleteRpc)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from rpc.");
    return false;
  }

  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDeleteFunctionalGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from seconds between retries.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertFunctionalGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for functional groups");
    return false;
  }

  policy_table::FunctionalGroupings::const_iterator it;

  for (it = groups.begin(); it != groups.end(); ++it) {
    // Since we uses this id in other tables, we have to be sure
    // that id for certain group will be same in case when
    // we drop records from the table and add them again.
    // That's why we use hash as a primary key insted of
    // simple auto incremental index.
    const long int id = abs(CacheManager::GenerateHash(it->first));
    // SQLite's Bind doesn support 'long' type
    // So we need to explicitly cast it to int64_t
    // to avoid ambiguity.
    query.Bind(0, static_cast<int64_t>(id));
    query.Bind(1, it->first);
    it->second.user_consent_prompt.is_initialized()
        ? query.Bind(2, *(it->second.user_consent_prompt))
        : query.Bind(2);

    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into functional groups");
      return false;
    }

    if (!SaveRpcs(query.LastInsertId(), it->second.rpcs)) {
      return false;
    }
  }
  return true;
}

bool SQLPTRepresentation::SaveRpcs(int64_t group_id,
                                   const policy_table::Rpc& rpcs) {
  utils::dbms::SQLQuery query(db());
  utils::dbms::SQLQuery query_parameter(db());
  if (!query.Prepare(sql_pt::kInsertRpc) ||
      !query_parameter.Prepare(sql_pt::kInsertRpcWithParameter)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for rpc");
    return false;
  }

  policy_table::Rpc::const_iterator it;
  for (it = rpcs.begin(); it != rpcs.end(); ++it) {
    const policy_table::HmiLevels& hmi_levels = it->second.hmi_levels;
    // TODO(IKozyrenko): Check logic if optional container is missing
    const policy_table::Parameters& parameters = *it->second.parameters;
    policy_table::HmiLevels::const_iterator hmi_it;
    policy_table::Parameters::const_iterator ps_it;
    for (hmi_it = hmi_levels.begin(); hmi_it != hmi_levels.end(); ++hmi_it) {
      if (!parameters.empty()) {
        for (ps_it = parameters.begin(); ps_it != parameters.end(); ++ps_it) {
          query_parameter.Bind(0, it->first);
          query_parameter.Bind(
              1, std::string(policy_table::EnumToJsonString(*hmi_it)));
          query_parameter.Bind(
              2, std::string(policy_table::EnumToJsonString(*ps_it)));
          query_parameter.Bind(3, group_id);
          if (!query_parameter.Exec() || !query_parameter.Reset()) {
            LOG4CXX_WARN(logger_, "Incorrect insert into rpc with parameter");
            return false;
          }
        }
      } else {
        query.Bind(0, it->first);
        query.Bind(1, std::string(policy_table::EnumToJsonString(*hmi_it)));
        query.Bind(2, group_id);
        if (!query.Exec() || !query.Reset()) {
          LOG4CXX_WARN(logger_, "Incorrect insert into rpc");
          return false;
        }
      }
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveApplicationPoliciesSection(
    const policy_table::ApplicationPoliciesSection& policies) {
  utils::dbms::SQLQuery query_delete(db());
  if (!query_delete.Exec(sql_pt::kDeleteAppGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from app_group.");
    return false;
  }
  if (!query_delete.Exec(sql_pt::kDeleteApplication)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from application.");
    return false;
  }

  if (!query_delete.Exec(sql_pt::kDeleteRequestType)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from request type.");
    return false;
  }

  // All predefined apps (e.g. default, pre_DataConsent) should be saved first,
  // otherwise another app with the predefined permissions can get incorrect
  // permissions
  policy_table::ApplicationPolicies::const_iterator it_default =
      policies.apps.find(kDefaultId);
  if (policies.apps.end() != it_default) {
    if (!SaveSpecificAppPolicy(*it_default)) {
      return false;
    }
  }
  policy_table::ApplicationPolicies::const_iterator it_pre_data_consented =
      policies.apps.find(kPreDataConsentId);
  if (policies.apps.end() != it_pre_data_consented) {
    if (!SaveSpecificAppPolicy(*it_pre_data_consented)) {
      return false;
    }
  }

  if (!SaveDevicePolicy(policies.device)) {
    return false;
  }

  policy_table::ApplicationPolicies::const_iterator it;
  for (it = policies.apps.begin(); it != policies.apps.end(); ++it) {
    // Skip saving of predefined app, since they should be saved before
    if (IsPredefinedApp(*it)) {
      continue;
    }
    if (!SaveSpecificAppPolicy(*it)) {
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveSpecificAppPolicy(
    const policy_table::ApplicationPolicies::value_type& app) {
  utils::dbms::SQLQuery app_query(db());
  if (!app_query.Prepare(sql_pt::kInsertApplication)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect insert statement into application (device).");
    return false;
  }

  app_query.Bind(0, app.first);
  app_query.Bind(
      1, std::string(policy_table::EnumToJsonString(app.second.priority)));
  app_query.Bind(2, app.second.is_null());
  app_query.Bind(3, *app.second.memory_kb);
  app_query.Bind(4, static_cast<int64_t>(*app.second.heart_beat_timeout_ms));
  app.second.certificate.is_initialized()
      ? app_query.Bind(5, *app.second.certificate)
      : app_query.Bind(5);
  if (!app_query.Exec() || !app_query.Reset()) {
    LOG4CXX_WARN(logger_, "Incorrect insert into application.");
    return false;
  }

  if (app.second.is_string()) {
    if (kDefaultId.compare(app.second.get_string()) == 0) {
      if (!SetDefaultPolicy(app.first)) {
        return false;
      }
      // Stop saving other params, since predefined permissions already set
      return true;
    }
  }

  if (!SaveAppGroup(app.first, app.second.groups)) {
    return false;
  }
  if (!SaveNickname(app.first, *app.second.nicknames)) {
    return false;
  }
  if (!SaveAppType(app.first, *app.second.AppHMIType)) {
    return false;
  }

  if (!SaveRequestType(app.first, *app.second.RequestType)) {
    return false;
  }

  return true;
}

bool policy::SQLPTRepresentation::SaveDevicePolicy(
    const policy_table::DevicePolicy& device) {
  utils::dbms::SQLQuery app_query(db());
  if (!app_query.Prepare(sql_pt::kInsertApplication)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement into application.");
    return false;
  }

  app_query.Bind(0, kDeviceId);
  app_query.Bind(1,
                 std::string(policy_table::EnumToJsonString(device.priority)));
  app_query.Bind(2, false);
  app_query.Bind(3, 0);
  app_query.Bind(4, 0);
  app_query.Bind(5);

  if (!app_query.Exec() || !app_query.Reset()) {
    LOG4CXX_WARN(logger_, "Incorrect insert into application.");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveAppGroup(
    const std::string& app_id, const policy_table::Strings& app_groups) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertAppGroup)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for app group");
    return false;
  }
  LOG4CXX_INFO(logger_, "SaveAppGroup");
  policy_table::Strings::const_iterator it;
  for (it = app_groups.begin(); it != app_groups.end(); ++it) {
    std::string ssss = *it;
    LOG4CXX_INFO(logger_, "Group: " << ssss);
    query.Bind(0, app_id);
    query.Bind(1, *it);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_,
                   "Incorrect insert into app group."
                       << query.LastError().text());
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveNickname(const std::string& app_id,
                                       const policy_table::Strings& nicknames) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertNickname)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for nickname");
    return false;
  }

  policy_table::Strings::const_iterator it;
  for (it = nicknames.begin(); it != nicknames.end(); ++it) {
    query.Bind(0, app_id);
    query.Bind(1, *it);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into nickname.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveAppType(const std::string& app_id,
                                      const policy_table::AppHMITypes& types) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertAppType)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for app type");
    return false;
  }

  policy_table::AppHMITypes::const_iterator it;
  for (it = types.begin(); it != types.end(); ++it) {
    query.Bind(0, app_id);
    query.Bind(1, std::string(policy_table::EnumToJsonString(*it)));
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into app type.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveRequestType(
    const std::string& app_id, const policy_table::RequestTypes& types) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertRequestType)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for request types.");
    return false;
  }

  policy_table::RequestTypes::const_iterator it;
  for (it = types.begin(); it != types.end(); ++it) {
    query.Bind(0, app_id);
    query.Bind(1, std::string(policy_table::EnumToJsonString(*it)));
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into request types.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveModuleMeta(const policy_table::ModuleMeta& meta) {
  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(sql_pt::kSaveModuleMeta)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for module_meta.");
    return false;
  }
  const int64_t odometer = *(meta.pt_exchanged_at_odometer_x);

  query.Bind(0, odometer);
  query.Bind(1, *(meta.pt_exchanged_x_days_after_epoch));
  query.Bind(2, *(meta.ignition_cycles_since_last_exchange));

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect update for module_meta.");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveModuleConfig(
    const policy_table::ModuleConfig& config) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateModuleConfig)) {
    LOG4CXX_WARN(logger_, "Incorrect update statement for module config");
    return false;
  }

  bool is_preloaded =
      config.preloaded_pt.is_initialized() && *config.preloaded_pt;

  query.Bind(0, is_preloaded);
  query.Bind(1, config.exchange_after_x_ignition_cycles);
  query.Bind(2, config.exchange_after_x_kilometers);
  query.Bind(3, config.exchange_after_x_days);
  query.Bind(4, config.timeout_after_x_seconds);
  query.Bind(5, (*config.certificate));
  config.vehicle_make.is_initialized() ? query.Bind(6, *(config.vehicle_make))
                                       : query.Bind(6);
  config.vehicle_model.is_initialized() ? query.Bind(7, *(config.vehicle_model))
                                        : query.Bind(7);
  config.vehicle_year.is_initialized() ? query.Bind(8, *(config.vehicle_year))
                                       : query.Bind(8);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect update module config");
    return false;
  }

  if (!SaveSecondsBetweenRetries(config.seconds_between_retries)) {
    return false;
  }

  if (!SaveNumberOfNotificationsPerMinute(
          config.notifications_per_minute_by_priority)) {
    return false;
  }

  if (!SaveServiceEndpoints(config.endpoints)) {
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveServiceEndpoints(
    const policy_table::ServiceEndpoints& endpoints) {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDeleteEndpoint)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from endpoint.");
    return false;
  }

  if (!query.Prepare(sql_pt::kInsertEndpoint)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for endpoint");
    return false;
  }

  policy_table::ServiceEndpoints::const_iterator it;
  for (it = endpoints.begin(); it != endpoints.end(); ++it) {
    const policy_table::URLList& apps = it->second;
    policy_table::URLList::const_iterator app_it;
    for (app_it = apps.begin(); app_it != apps.end(); ++app_it) {
      const policy_table::URL& urls = app_it->second;
      policy_table::URL::const_iterator url_it;
      for (url_it = urls.begin(); url_it != urls.end(); ++url_it) {
        query.Bind(0, it->first);
        query.Bind(1, *url_it);
        query.Bind(2, app_it->first);
        if (!query.Exec() || !query.Reset()) {
          LOG4CXX_WARN(logger_, "Incorrect insert into endpoint");
          return false;
        }
      }
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveConsumerFriendlyMessages(
    const policy_table::ConsumerFriendlyMessages& messages) {
  LOG4CXX_AUTO_TRACE(logger_);

  // According CRS-2419  If there is no “consumer_friendly_messages” key,
  // the current local consumer_friendly_messages section shall be maintained in
  // the policy table. So it won't be changed/updated
  if (messages.messages.is_initialized()) {
    utils::dbms::SQLQuery query(db());
    if (!query.Exec(sql_pt::kDeleteMessageString)) {
      LOG4CXX_WARN(logger_, "Incorrect delete from message.");
      return false;
    }

    if (query.Prepare(sql_pt::kUpdateVersion)) {
      query.Bind(0, messages.version);
      if (!query.Exec()) {
        LOG4CXX_WARN(logger_, "Incorrect update into version.");
        return false;
      }
    } else {
      LOG4CXX_WARN(logger_, "Incorrect update statement for version.");
      return false;
    }

    policy_table::Messages::const_iterator it;
    for (it = messages.messages->begin(); it != messages.messages->end();
         ++it) {
      if (!SaveMessageType(it->first)) {
        return false;
      }
      const policy_table::Languages& langs = it->second.languages;
      policy_table::Languages::const_iterator lang_it;
      for (lang_it = langs.begin(); lang_it != langs.end(); ++lang_it) {
        if (!SaveLanguage(lang_it->first)) {
          return false;
        }
        if (!SaveMessageString(it->first, lang_it->first, lang_it->second)) {
          return false;
        }
      }
    }
  } else {
    LOG4CXX_INFO(logger_, "Messages list is empty");
  }

  return true;
}

bool SQLPTRepresentation::SaveMessageType(const std::string& type) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertMessageType)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for message type.");
    return false;
  }

  query.Bind(0, type);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect insert into message type.");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveLanguage(const std::string& code) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertLanguage)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for language.");
    return false;
  }

  query.Bind(0, code);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Incorrect insert into language.");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveMessageString(
    const std::string& type,
    const std::string& lang,
    const policy_table::MessageString& strings) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertMessageString)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for message.");
    return false;
  }

  query.Bind(0, lang);
  query.Bind(1, type);

  if (!query.Exec() || !query.Reset()) {
    LOG4CXX_WARN(logger_, "Incorrect insert into message.");
    return false;
  }
  return true;
}

bool SQLPTRepresentation::SaveSecondsBetweenRetries(
    const policy_table::SecondsBetweenRetries& seconds) {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDeleteSecondsBetweenRetries)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from seconds between retries.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertSecondsBetweenRetry)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect insert statement for seconds between retries.");
    return false;
  }

  for (uint32_t i = 0; i < seconds.size(); ++i) {
    query.Bind(0, static_cast<int>(i));
    query.Bind(1, seconds[i]);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into seconds between retries.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveNumberOfNotificationsPerMinute(
    const policy_table::NumberOfNotificationsPerMinute& notifications) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertNotificationsByPriority)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect insert statement for notifications by priority.");
    return false;
  }

  policy_table::NumberOfNotificationsPerMinute::const_iterator it;
  for (it = notifications.begin(); it != notifications.end(); ++it) {
    query.Bind(0, it->first);
    query.Bind(1, it->second);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into notifications by priority.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveDeviceData(
    const policy_table::DeviceData& devices) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertDeviceData)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for device data.");
    return false;
  }

  policy_table::DeviceData::const_iterator it;
  for (it = devices.begin(); it != devices.end(); ++it) {
    query.Bind(0, it->first);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into device data.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveUsageAndErrorCounts(
    const policy_table::UsageAndErrorCounts& counts) {
  const_cast<policy_table::UsageAndErrorCounts&>(counts).mark_initialized();
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDeleteAppLevel)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from app level.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertAppLevel)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement for app level.");
    return false;
  }

  policy_table::AppLevels::const_iterator it;
  const policy_table::AppLevels& app_levels = *counts.app_level;
  const_cast<policy_table::AppLevels&>(*counts.app_level).mark_initialized();
  for (it = app_levels.begin(); it != app_levels.end(); ++it) {
    query.Bind(0, it->first);
    query.Bind(1, it->second.count_of_tls_errors);
    if (!query.Exec() || !query.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insert into app level.");
      return false;
    }
  }
  return true;
}

void SQLPTRepresentation::IncrementIgnitionCycles() {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kIncrementIgnitionCycles)) {
    LOG4CXX_WARN(logger_, "Failed incrementing ignition cycles");
  }
}

void SQLPTRepresentation::ResetIgnitionCycles() {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kResetIgnitionCycles)) {
    LOG4CXX_WARN(logger_, "Failed to reset ignition cycles number.");
  }
}

bool SQLPTRepresentation::UpdateRequired() const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectFlagUpdateRequired) || !query.Exec()) {
    LOG4CXX_WARN(logger_,
                 "Failed select update required flag from module meta");
    return false;
  }
  return query.GetBoolean(0);
}

void SQLPTRepresentation::SaveUpdateRequired(bool value) {
  utils::dbms::SQLQuery query(db());
  // TODO(AOleynik): Quick fix, will be reworked
  if (!query.Prepare(/*sql_pt::kUpdateFlagUpdateRequired*/
                     "UPDATE `module_meta` SET `flag_update_required` = ?")) {
    LOG4CXX_WARN(logger_,
                 "Incorrect update into module meta (update_required): "
                     << strerror(errno));
    return;
  }
  query.Bind(0, value);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed update module meta (update_required)");
  }
}

bool SQLPTRepresentation::GetInitialAppData(const std::string& app_id,
                                            StringArray* nicknames,
                                            StringArray* app_types) {
  LOG4CXX_INFO(logger_, "Getting initial application data.");
  utils::dbms::SQLQuery app_names(db());
  if (!app_names.Prepare(sql_pt::kSelectNicknames)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app nicknames");
    return false;
  }
  utils::dbms::SQLQuery app_hmi_types(db());
  if (!app_hmi_types.Prepare(sql_pt::kSelectAppTypes)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app types");
    return false;
  }
  app_names.Bind(0, app_id);
  while (app_names.Next()) {
    nicknames->push_back(app_names.GetString(0));
  }
  app_names.Reset();
  app_hmi_types.Bind(0, app_id);
  while (app_hmi_types.Next()) {
    app_types->push_back(app_hmi_types.GetString(0));
  }
  app_hmi_types.Reset();
  return true;
}

bool SQLPTRepresentation::GetFunctionalGroupings(
    policy_table::FunctionalGroupings& groups) {
  LOG4CXX_AUTO_TRACE(logger_);
  return GatherFunctionalGroupings(&groups);
}

bool SQLPTRepresentation::GatherAppType(
    const std::string& app_id, policy_table::AppHMITypes* app_types) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectAppTypes)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app types");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    policy_table::AppHMIType type;
    if (!policy_table::EnumFromJsonString(query.GetString(0), &type)) {
      return false;
    }
    app_types->push_back(type);
  }
  return true;
}

bool SQLPTRepresentation::GatherRequestType(
    const std::string& app_id,
    policy_table::RequestTypes* request_types) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectRequestTypes)) {
    LOG4CXX_WARN(logger_, "Incorrect select from request types.");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    policy_table::RequestType type;
    if (!policy_table::EnumFromJsonString(query.GetString(0), &type)) {
      return false;
    }
    request_types->push_back(type);
  }
  return true;
}

bool SQLPTRepresentation::GatherNickName(
    const std::string& app_id, policy_table::Strings* nicknames) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectNicknames)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app nicknames");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    nicknames->push_back(query.GetString(0));
  }
  return true;
}

bool SQLPTRepresentation::GatherAppGroup(
    const std::string& app_id, policy_table::Strings* app_groups) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectAppGroups)) {
    LOG4CXX_WARN(logger_, "Incorrect select from app groups");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    app_groups->push_back(query.GetString(0));
  }
  return true;
}

bool SQLPTRepresentation::SaveApplicationCustomData(const std::string& app_id,
                                                    bool is_revoked,
                                                    bool is_default,
                                                    bool is_predata) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateApplicationCustomData)) {
    LOG4CXX_WARN(logger_, "Incorrect update in application");
    return false;
  }

  query.Bind(0, is_revoked);
  query.Bind(1, is_default);
  query.Bind(2, is_predata);
  query.Bind(3, app_id);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed update in application");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::IsApplicationRevoked(
    const std::string& app_id) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectApplicationRevoked)) {
    LOG4CXX_WARN(logger_, "Incorrect select from is_revoked of application");
  }

  query.Bind(0, app_id);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed select is_revoked of application");
    return false;
  }
  return query.IsNull(0) ? false : query.GetBoolean(0);
}
bool SQLPTRepresentation::IsApplicationRepresented(
    const std::string& app_id) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectApplicationRepresented)) {
    LOG4CXX_WARN(logger_, "Incorrect select application by id");
    return false;
  }

  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed select application by id");
    return false;
  }
  return query.GetInteger(0) != 0;
}

bool SQLPTRepresentation::IsDefaultPolicy(const std::string& app_id) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectApplicationIsDefault)) {
    LOG4CXX_WARN(logger_, "Incorrect select application by id");
    return false;
  }

  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed select application by id");
    return false;
  }
  return query.IsNull(0) ? false : query.GetBoolean(0);
}

bool SQLPTRepresentation::IsPredataPolicy(const std::string& app_id) const {
  return false;
}

bool SQLPTRepresentation::SetDefaultPolicy(const std::string& app_id) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kDeleteAppGroupByApplicationId)) {
    LOG4CXX_ERROR(logger_, "Incorrect statement to delete from app_group.");
    return false;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    LOG4CXX_ERROR(logger_, "Failed deleting from app_group.");
    return false;
  }

  if (!CopyApplication(kDefaultId, app_id)) {
    return false;
  }

  SetPreloaded(false);

  policy_table::Strings default_groups;
  if (!GatherAppGroup(kDefaultId, &default_groups) ||
      !SaveAppGroup(app_id, default_groups)) {
    return false;
  }
  policy_table::RequestTypes request_types;
  if (!GatherRequestType(kDefaultId, &request_types) ||
      !SaveRequestType(app_id, request_types)) {
    return false;
  }
  policy_table::AppHMITypes app_types;
  if (!GatherAppType(kDefaultId, &app_types) ||
      !SaveAppType(app_id, app_types)) {
    return false;
  }
  return SetIsDefault(app_id, true);
}

bool SQLPTRepresentation::SetIsDefault(const std::string& app_id,
                                       bool is_default) const {
  LOG4CXX_TRACE(logger_, "Set flag is_default of application");
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateIsDefault)) {
    LOG4CXX_WARN(logger_, "Incorect statement for updating is_default");
    return false;
  }

  query.Bind(0, is_default);
  query.Bind(1, app_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed update is_default");
    return false;
  }
  return true;
}

void SQLPTRepresentation::RemoveDB() const {
  file_system::DeleteFile(db_->get_path());
}

bool SQLPTRepresentation::IsDBVersionActual() const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectDBVersion) || !query.Exec()) {
    LOG4CXX_ERROR(logger_,
                  "Failed to get DB version: " << query.LastError().text());
    return false;
  }

  const int32_t saved_db_version = query.GetInteger(0);
  const int32_t current_db_version = GetDBVersion();
  LOG4CXX_DEBUG(logger_,
                "Saved DB version is: " << saved_db_version
                                        << ". Current DB vesion is: "
                                        << current_db_version);

  return current_db_version == saved_db_version;
}

bool SQLPTRepresentation::UpdateDBVersion() const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateDBVersion)) {
    LOG4CXX_ERROR(logger_,
                  "Incorrect DB version query: " << query.LastError().text());
    return false;
  }

  const int32_t db_version = GetDBVersion();
  LOG4CXX_DEBUG(logger_, "DB version will be updated to: " << db_version);
  query.Bind(0, db_version);

  if (!query.Exec()) {
    LOG4CXX_ERROR(logger_,
                  "DB version getting failed: " << query.LastError().text());
    return false;
  }

  return true;
}

const int32_t SQLPTRepresentation::GetDBVersion() const {
  return utils::Djb2HashFromString(sql_pt::kCreateSchema);
}

utils::dbms::SQLDatabase* SQLPTRepresentation::db() const {
#ifdef __QNX__
  utils::dbms::SQLDatabase* db = new utils::dbms::SQLDatabase(kDatabaseName);
  db->Open();
  return db;
#else
  return db_;
#endif
}

bool SQLPTRepresentation::CopyApplication(const std::string& source,
                                          const std::string& destination) {
  utils::dbms::SQLQuery source_app(db());
  if (!source_app.Prepare(sql_pt::kSelectApplicationFull)) {
    LOG4CXX_WARN(logger_, "Incorrect select statement from application.");
    return false;
  }
  source_app.Bind(0, source);
  if (!source_app.Exec()) {
    LOG4CXX_WARN(logger_, "Failed selecting from application.");
    return false;
  }

  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertApplicationFull)) {
    LOG4CXX_WARN(logger_, "Incorrect insert statement into application.");
    return false;
  }
  query.Bind(0, destination);
  source_app.IsNull(0) ? query.Bind(1)
                       : query.Bind(1, source_app.GetBoolean(0));
  source_app.IsNull(1) ? query.Bind(2)
                       : query.Bind(2, source_app.GetBoolean(1));
  source_app.IsNull(2) ? query.Bind(3) : query.Bind(3, source_app.GetString(2));
  source_app.IsNull(3) ? query.Bind(4) : query.Bind(4, source_app.GetString(3));
  source_app.IsNull(4) ? query.Bind(5)
                       : query.Bind(5, source_app.GetBoolean(4));
  source_app.IsNull(5) ? query.Bind(6)
                       : query.Bind(6, source_app.GetBoolean(5));
  source_app.IsNull(6) ? query.Bind(7)
                       : query.Bind(7, source_app.GetBoolean(6));
  query.Bind(8, source_app.GetInteger(7));
  query.Bind(9, source_app.GetInteger(8));
  source_app.IsNull(9) ? query.Bind(10)
                       : query.Bind(10, source_app.GetString(9));
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed inserting into application.");
    return false;
  }
  return true;
}

void SQLPTRepresentation::SetPreloaded(bool value) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdatePreloaded)) {
    LOG4CXX_WARN(logger_, "Incorrect statement of updating preloaded.");
    return;
  }

  query.Bind(0, value);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Failed updating preloaded.");
    return;
  }
}

bool SQLPTRepresentation::SetVINValue(const std::string& value) {
  return true;
}

}  // namespace policy

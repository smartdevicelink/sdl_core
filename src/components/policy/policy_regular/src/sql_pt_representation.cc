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

#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <sstream>

#include "config_profile/profile.h"
#include "policy/cache_manager.h"
#include "policy/policy_helper.h"
#include "policy/sql_pt_queries.h"
#include "policy/sql_pt_representation.h"
#include "policy/sql_wrapper.h"
#include "utils/date_time.h"
#include "utils/file_system.h"
#include "utils/gen_hash.h"
#include "utils/logger.h"

namespace policy {

SDL_CREATE_LOG_VARIABLE("Policy")

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
    : db_(new utils::dbms::SQLDatabase(kDatabaseName)) {
  is_in_memory = false;
}

SQLPTRepresentation::SQLPTRepresentation(bool in_memory) {
  is_in_memory = in_memory;
  if (in_memory) {
    db_ = new utils::dbms::SQLDatabase();
  } else {
    db_ = new utils::dbms::SQLDatabase(kDatabaseName);
  }
}

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
    SDL_LOG_WARN("Incorrect select statement from rpcs"
                 << query.LastError().text());
    return;
  }
  query.Bind(0, app_id);
  query.Bind(1, hmi_level);
  query.Bind(2, rpc);

  bool ret = query.Next();
  result.hmi_level_permitted = ret ? kRpcAllowed : kRpcDisallowed;
  SDL_LOG_INFO("Level is " << (result.hmi_level_permitted == kRpcAllowed
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
    SDL_LOG_WARN("Can not select ignition cycles");
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
    SDL_LOG_WARN("Can not select kilometers");
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
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateCountersSuccessfulUpdate)) {
    SDL_LOG_WARN("Wrong update query for counters on successful update.");
    return false;
  }
  query.Bind(0, kilometers);
  query.Bind(1, days_after_epoch);
  if (!query.Exec()) {
    SDL_LOG_WARN("Failed to update counters on successful update.");
    return false;
  }
  return true;
}

int SQLPTRepresentation::DaysBeforeExchange(int current) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectDays) || !query.Exec()) {
    SDL_LOG_WARN("Can not select days");
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
    SDL_LOG_INFO("Can not select timeout response for retry sequence");
    const int defaultTimeout = 30 * date_time::MILLISECONDS_IN_SECOND;
    return defaultTimeout;
  }
  return query.GetInteger(0) * date_time::MILLISECONDS_IN_SECOND;
}

bool SQLPTRepresentation::SecondsBetweenRetries(std::vector<int>* seconds) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectSecondsBetweenRetries)) {
    SDL_LOG_INFO("Incorrect select statement from seconds between retries");
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
  SDL_LOG_INFO("SQLPTRepresentation::GetUpdateUrls for " << service_type);
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
    SDL_LOG_WARN("Invalid select endpoints statement.");
  }
  return ret;
}

int SQLPTRepresentation::GetNotificationsNumber(const std::string& priority) {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectNotificationsPerPriority)) {
    SDL_LOG_WARN(
        "Incorrect select statement for priority "
        "notification number.");
    return 0;
  }
  query.Bind(0, priority);
  if (!query.Exec()) {
    SDL_LOG_WARN("Incorrect select from notifications by priority.");
    return 0;
  }

  if (!query.IsNull(0)) {
    return query.GetInteger(0);
  }

  return 0;
}

bool SQLPTRepresentation::GetPriority(const std::string& policy_app_id,
                                      std::string* priority) {
  SDL_LOG_AUTO_TRACE();
  if (NULL == priority) {
    SDL_LOG_WARN("Input priority parameter is null.");
    return false;
  }
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectPriority)) {
    SDL_LOG_INFO("Incorrect statement for priority.");
    return false;
  }

  query.Bind(0, policy_app_id);

  if (!query.Exec()) {
    SDL_LOG_INFO("Error during select priority.");
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
  SDL_LOG_AUTO_TRACE();
#ifdef BUILD_TESTS
  open_counter_ = 0;
#endif  // BUILD_TESTS

  if (!is_in_memory) {
    const std::string& path = get_settings().app_storage_folder();
    if (!path.empty()) {
      db_->set_path(path + "/");
    }
  }

  if (!db_->Open()) {
    SDL_LOG_ERROR("Failed opening database.");
    SDL_LOG_INFO("Starting opening retries.");
    const uint16_t attempts = get_settings().attempts_to_open_policy_db();
    SDL_LOG_DEBUG("Total attempts number is: " << attempts);
    bool is_opened = false;
    const uint16_t open_attempt_timeout_ms =
        get_settings().open_attempt_timeout_ms();
    const useconds_t sleep_interval_mcsec = open_attempt_timeout_ms * 1000;
    SDL_LOG_DEBUG("Open attempt timeout(ms) is: " << open_attempt_timeout_ms);
    for (int i = 0; i < attempts; ++i) {
      usleep(sleep_interval_mcsec);
      SDL_LOG_INFO("Attempt: " << i + 1);
#ifdef BUILD_TESTS
      ++open_counter_;
#endif  // BUILD_TESTS
      if (db_->Open()) {
        SDL_LOG_INFO("Database opened.");
        is_opened = true;
        break;
      }
    }
    if (!is_opened) {
      SDL_LOG_ERROR("Open retry sequence failed. Tried "
                    << attempts << " attempts with " << open_attempt_timeout_ms
                    << " open timeout(ms) for each.");
      return InitResult::FAIL;
    }
  }

  if (!db_->IsReadWrite()) {
    SDL_LOG_ERROR("There are no read/write permissions for database");
    return InitResult::FAIL;
  }

  utils::dbms::SQLQuery check_pages(db());
  if (!check_pages.Prepare(sql_pt::kCheckPgNumber) || !check_pages.Next()) {
    SDL_LOG_WARN("Incorrect pragma for page counting.");
  } else {
    if (0 < check_pages.GetInteger(0)) {
      utils::dbms::SQLQuery db_check(db());
      if (!db_check.Prepare(sql_pt::kCheckDBIntegrity)) {
        SDL_LOG_WARN("Incorrect pragma for integrity check.");
      } else {
        while (db_check.Next()) {
          if (db_check.GetString(0).compare("ok") == 0) {
            utils::dbms::SQLQuery check_first_run(db());
            if (check_first_run.Prepare(sql_pt::kIsFirstRun) &&
                check_first_run.Next()) {
              SDL_LOG_INFO("Selecting is first run "
                           << check_first_run.GetBoolean(0));
              if (check_first_run.GetBoolean(0)) {
                utils::dbms::SQLQuery set_not_first_run(db());
                set_not_first_run.Exec(sql_pt::kSetNotFirstRun);
                return InitResult::SUCCESS;
              }
            } else {
              SDL_LOG_WARN("Incorrect select is first run");
            }
            return InitResult::EXISTS;
          } else {
            SDL_LOG_ERROR("Existing policy table representation is invlaid.");
            // TODO(PV): add handle
            return InitResult::FAIL;
          }
        }
      }
    }
  }
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kCreateSchema)) {
    SDL_LOG_ERROR(
        "Failed creating schema of database: " << query.LastError().text());
    return InitResult::FAIL;
  }
  if (!query.Exec(sql_pt::kInsertInitData)) {
    SDL_LOG_ERROR(
        "Failed insert init data to database: " << query.LastError().text());
    return InitResult::FAIL;
  }
  return InitResult::SUCCESS;
}

bool SQLPTRepresentation::Close() {
  db_->Close();
  return db_->LastError().number() == utils::dbms::OK;
}

bool SQLPTRepresentation::Drop() {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDropSchema)) {
    SDL_LOG_WARN("Failed dropping database: " << query.LastError().text());
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
    SDL_LOG_ERROR("Failed clearing database: " << query.LastError().text());
    return false;
  }
  if (!query.Exec(sql_pt::kInsertInitData)) {
    SDL_LOG_ERROR(
        "Failed insert init data to database: " << query.LastError().text());
    return false;
  }
  return true;
}

bool SQLPTRepresentation::RefreshDB() {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDropSchema)) {
    SDL_LOG_WARN("Failed dropping database: " << query.LastError().text());
    return false;
  }
  if (!query.Exec(sql_pt::kCreateSchema)) {
    SDL_LOG_ERROR(
        "Failed creating schema of database: " << query.LastError().text());
    return false;
  }
  if (!query.Exec(sql_pt::kInsertInitData)) {
    SDL_LOG_ERROR(
        "Failed insert init data to database: " << query.LastError().text());
    return false;
  }
  return true;
}

std::shared_ptr<policy_table::Table> SQLPTRepresentation::GenerateSnapshot()
    const {
  SDL_LOG_AUTO_TRACE();
  auto table = std::make_shared<policy_table::Table>();
  GatherModuleMeta(&*table->policy_table.module_meta);
  GatherModuleConfig(&table->policy_table.module_config);
  GatherUsageAndErrorCounts(&*table->policy_table.usage_and_error_counts);
  GatherDeviceData(&*table->policy_table.device_data);
  GatherFunctionalGroupings(&table->policy_table.functional_groupings);
  GatherConsumerFriendlyMessages(
      &*table->policy_table.consumer_friendly_messages);
  GatherApplicationPoliciesSection(&table->policy_table.app_policies_section);
  GatherVehicleData(&*table->policy_table.vehicle_data);
  if (!table->policy_table.vehicle_data.is_initialized()) {
    rpc::Optional<rpc::String<0, 100> > null_version;
    table->policy_table.vehicle_data->schema_version = null_version;
  }
  return table;
}

void SQLPTRepresentation::GatherModuleMeta(
    policy_table::ModuleMeta* meta) const {
  SDL_LOG_INFO("Gather Module Meta Info");
  utils::dbms::SQLQuery query(db());
  if (query.Prepare(sql_pt::kSelectModuleMeta) && query.Next()) {
    *meta->ccpu_version = query.GetString(0);
    *meta->hardware_version = query.GetString(1);
    *meta->pt_exchanged_at_odometer_x = query.GetInteger(2);
    *meta->pt_exchanged_x_days_after_epoch = query.GetInteger(3);
    *meta->ignition_cycles_since_last_exchange = query.GetInteger(4);
  }
}

void SQLPTRepresentation::GatherModuleConfig(
    policy_table::ModuleConfig* config) const {
  SDL_LOG_INFO("Gather Configuration Info");
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectModuleConfig) || !query.Next()) {
    SDL_LOG_WARN("Incorrect select statement for module config");
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
    if (!query.IsNull(9)) {
      *config->lock_screen_dismissal_enabled = query.GetBoolean(9);
    }
  }

  utils::dbms::SQLQuery endpoints(db());
  if (!endpoints.Prepare(sql_pt::kSelectEndpoints)) {
    SDL_LOG_WARN("Incorrect select statement for endpoints");
  } else {
    while (endpoints.Next()) {
      const std::string& url = endpoints.GetString(0);
      const std::string& service = endpoints.GetString(1);
      const std::string& app_id = endpoints.GetString(2);
      config->endpoints[service][app_id].push_back(url);
    }
  }

  utils::dbms::SQLQuery endpoint_properties(db());
  if (!endpoint_properties.Prepare(sql_pt::kSelectEndpointProperties)) {
    SDL_LOG_ERROR("Incorrect statement for Endpoint properties");
  } else {
    while (endpoint_properties.Next()) {
      const std::string& service = endpoint_properties.GetString(0);
      auto& ep_properties = (*config->endpoint_properties);
      if (!endpoint_properties.IsNull(1)) {
        const std::string& version = endpoint_properties.GetString(1);
        *ep_properties[service].version = version;
      } else {
        ep_properties[service].version = rpc::Optional<rpc::String<0, 100> >();
      }
    }
  }

  utils::dbms::SQLQuery notifications(db());
  if (!notifications.Prepare(sql_pt::kSelectNotificationsPerMin)) {
    SDL_LOG_WARN("Incorrect select statement for notifications");
  } else {
    while (notifications.Next()) {
      config->notifications_per_minute_by_priority[notifications.GetString(0)] =
          notifications.GetInteger(1);
    }
  }
  utils::dbms::SQLQuery subtle_notifications(db());
  if (!subtle_notifications.Prepare(sql_pt::kSelectSubtleNotificationsPerMin)) {
    SDL_LOG_WARN("Incorrect select statement for subtle notifications");
  } else {
    while (subtle_notifications.Next()) {
      (*config->subtle_notifications_per_minute_by_priority)
          [subtle_notifications.GetString(0)] =
              subtle_notifications.GetInteger(1);
    }
  }
  utils::dbms::SQLQuery seconds(db());
  if (!seconds.Prepare(sql_pt::kSelectSecondsBetweenRetries)) {
    SDL_LOG_INFO("Incorrect select statement from seconds between retries");
  } else {
    while (seconds.Next()) {
      config->seconds_between_retries.push_back(seconds.GetInteger(0));
    }
  }
}

bool SQLPTRepresentation::GatherUsageAndErrorCounts(
    policy_table::UsageAndErrorCounts* counts) const {
  SDL_LOG_INFO("Gather Usage and Error Counts.");
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
  SDL_LOG_INFO("Gather device data.");
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
  SDL_LOG_INFO("Gather Functional Groupings info");
  utils::dbms::SQLQuery functional_group(db());
  if (!functional_group.Prepare(sql_pt::kSelectFunctionalGroups)) {
    SDL_LOG_WARN("Incorrect select from functional_groupings");
    return false;
  }
  utils::dbms::SQLQuery rpcs(db());
  if (!rpcs.Prepare(sql_pt::kSelectAllRpcs)) {
    SDL_LOG_WARN("Incorrect select all from rpc");
    return false;
  }

  while (functional_group.Next()) {
    policy_table::Rpcs rpcs_structure;

    if (!functional_group.IsNull(2)) {
      *rpcs_structure.user_consent_prompt = functional_group.GetString(2);
    }

    if (!functional_group.IsNull(3)) {
      *rpcs_structure.encryption_required = functional_group.GetBoolean(3);
    }

    const int group_id = functional_group.GetInteger(0);

    // Collecting RPCs with their HMI levels and parameters (if any)
    rpcs.Bind(0, group_id);
    while (rpcs.Next()) {
      if (!rpcs.IsNull(1)) {
        policy_table::HmiLevel level;
        if (policy_table::EnumFromJsonString(rpcs.GetString(1), &level)) {
          InsertUnique(level,
                       &rpcs_structure.rpcs[rpcs.GetString(0)].hmi_levels);
        }
      }
      if (!rpcs.IsNull(2)) {
        std::string param = rpcs.GetString(2);
        // EMPTY is a special mark to specify that 'parameters' section is
        // present, but has no parameters. It is not valid parameter value.
        if (policy_table::EnumToJsonString(policy_table::P_EMPTY) == param) {
          (*rpcs_structure.rpcs[rpcs.GetString(0)].parameters)
              .mark_initialized();
          continue;
        }

        InsertUnique(param,
                     &(*rpcs_structure.rpcs[rpcs.GetString(0)].parameters));
      }
    }

    rpcs.Reset();

    if (!rpcs_structure.rpcs.is_initialized()) {
      rpcs_structure.rpcs.set_to_null();
    }
    policy_table::Rpcs& group_rpcs_structure =
        (*groups)[functional_group.GetString(1)];
    group_rpcs_structure = rpcs_structure;
  }
  return true;
}

bool SQLPTRepresentation::GatherConsumerFriendlyMessages(
    policy_table::ConsumerFriendlyMessages* messages) const {
  SDL_LOG_INFO("Gather Consumer Friendly Messages");
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectUserMsgsVersion) || !query.Next()) {
    SDL_LOG_WARN("Incorrect select from consumer_friendly_messages");
    return false;
  }

  messages->version = query.GetString(0);

  if (query.Prepare(sql_pt::kCollectFriendlyMsg)) {
    while (query.Next()) {
      UserFriendlyMessage msg;

      msg.tts = query.GetString(1);
      msg.label = query.GetString(2);
      msg.line1 = query.GetString(3);
      msg.line2 = query.GetString(4);
      msg.text_body = query.GetString(5);
      msg.message_code = query.GetString(7);

      std::string language = query.GetString(6);
      policy_table::Languages& languages =
          (*messages->messages)[msg.message_code].languages;
      policy_table::MessageString& specific_message = languages[language];
      if (!msg.tts.empty()) {
        *(specific_message).tts = msg.tts;
      }
      if (!msg.label.empty()) {
        *(specific_message).label = msg.label;
      }
      if (!msg.line1.empty()) {
        *(specific_message).line1 = msg.line1;
      }
      if (!msg.line2.empty()) {
        *(specific_message).line2 = msg.line2;
      }
      if (!msg.text_body.empty()) {
        *(specific_message).textBody = msg.text_body;
      }
    }
  } else {
    SDL_LOG_WARN("Incorrect statement for select friendly messages.");
  }

  return true;
}

bool SQLPTRepresentation::SetMetaInfo(const std::string& ccpu_version) {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateMetaParams)) {
    SDL_LOG_WARN("Incorrect statement for insert to module meta.");
    return false;
  }

  query.Bind(0, ccpu_version);

  if (!query.Exec() || !query.Reset()) {
    SDL_LOG_WARN("Incorrect insert to module meta.");
    return false;
  }
  return true;
}

void SQLPTRepresentation::SetHardwareVersion(
    const std::string& hardware_version) {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateMetaHardwareVersion)) {
    SDL_LOG_WARN("Incorrect statement for insert to module meta.");
    return;
  }

  query.Bind(0, hardware_version);

  if (!query.Exec()) {
    SDL_LOG_WARN("Incorrect insert to module meta.");
  }
}

bool SQLPTRepresentation::GatherApplicationPoliciesSection(
    policy_table::ApplicationPoliciesSection* policies) const {
  SDL_LOG_INFO("Gather applications policies");
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectAppPolicies)) {
    SDL_LOG_WARN("Incorrect select from app_policies");
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

    *params.heart_beat_timeout_ms = query.GetUInteger(2);
    if (!query.IsNull(3)) {
      *params.certificate = query.GetString(3);
    }

    // Read cloud app properties
    policy_table::HybridAppPreference hap;
    bool valid = policy_table::EnumFromJsonString(query.GetString(4), &hap);
    if (valid) {
      *params.hybrid_app_preference = hap;
    }
    *params.endpoint = query.GetString(5);
    if (!query.IsNull(6)) {
      *params.enabled = query.GetBoolean(6);
    }
    *params.auth_token = query.GetString(7);
    *params.cloud_transport_type = query.GetString(8);
    *params.icon_url = query.GetString(9);
    *params.allow_unknown_rpc_passthrough = query.GetBoolean(10);

    if (!query.IsNull(11)) {
      *params.encryption_required = query.GetBoolean(11);
    }

    const auto& gather_app_id = ((*policies).apps[app_id].is_string())
                                    ? (*policies).apps[app_id].get_string()
                                    : app_id;
    // Data should be gathered from db by  "default" key if application has
    // default policies

    if (!GatherAppGroup(gather_app_id, &params.groups)) {
      return false;
    }
    bool denied = false;
    if (!GatherRemoteControlDenied(gather_app_id, &denied)) {
      return false;
    }
    if (!denied) {
      if (!GatherModuleType(gather_app_id, &*params.moduleType)) {
        return false;
      }
    }
    if (!GatherNickName(gather_app_id, &*params.nicknames)) {
      return false;
    }
    if (!GatherAppType(gather_app_id, &*params.AppHMIType)) {
      return false;
    }
    if (!GatherRequestType(gather_app_id, &*params.RequestType)) {
      return false;
    }

    if (!GatherRequestSubType(gather_app_id, &*params.RequestSubType)) {
      return false;
    }

    if (!GatherAppServiceParameters(gather_app_id,
                                    &*params.app_service_parameters)) {
      return false;
    }

    (*policies).apps[app_id] = params;
  }
  return true;
}

bool SQLPTRepresentation::GatherVehicleData(
    policy_table::VehicleData* vehicle_data) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectVehicleDataSchemaVersion) ||
      !query.Next()) {
    SDL_LOG_ERROR("Incorrect statement for vehicle data schema version");
    return false;
  }
  *vehicle_data->schema_version = query.GetString(0);

  vehicle_data->mark_initialized();
  return GatherVehicleDataItems(&*vehicle_data->schema_items);
}

bool SQLPTRepresentation::GatherVehicleDataItems(
    policy_table::VehicleDataItems* vehicle_data_items) const {
  SDL_LOG_AUTO_TRACE();
  auto parameterized_vdi = SelectCompositeVehicleDataItems();
  if (!parameterized_vdi.is_initialized()) {
    return false;
  }

  auto non_parameterized_vdi = SelectPrimitiveVehicleDataItems();
  if (!non_parameterized_vdi.is_initialized()) {
    return false;
  }

  vehicle_data_items->insert(vehicle_data_items->end(),
                             parameterized_vdi.begin(),
                             parameterized_vdi.end());
  vehicle_data_items->insert(vehicle_data_items->end(),
                             non_parameterized_vdi.begin(),
                             non_parameterized_vdi.end());

  return true;
}

bool SQLPTRepresentation::Save(const policy_table::Table& table) {
  SDL_LOG_AUTO_TRACE();
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
  if (!SaveVehicleData(*table.policy_table.vehicle_data)) {
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
    SDL_LOG_WARN("Incorrect delete from rpc.");
    return false;
  }

  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDeleteFunctionalGroup)) {
    SDL_LOG_WARN("Incorrect delete from seconds between retries.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertFunctionalGroup)) {
    SDL_LOG_WARN("Incorrect insert statement for functional groups");
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
    it->second.encryption_required.is_initialized()
        ? query.Bind(3, *(it->second.encryption_required))
        : query.Bind(3);
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into functional groups");
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
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  utils::dbms::SQLQuery query_parameter(db());
  if (!query.Prepare(sql_pt::kInsertRpc) ||
      !query_parameter.Prepare(sql_pt::kInsertRpcWithParameter)) {
    SDL_LOG_WARN("Incorrect insert statement for rpc");
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
          query_parameter.Bind(2, std::string(*ps_it));
          query_parameter.Bind(3, group_id);
          if (!query_parameter.Exec() || !query_parameter.Reset()) {
            SDL_LOG_WARN("Incorrect insert into rpc with parameter");
            return false;
          }
        }
      } else if (parameters.is_initialized()) {
        query_parameter.Bind(0, it->first);
        query_parameter.Bind(
            1, std::string(policy_table::EnumToJsonString(*hmi_it)));
        query_parameter.Bind(
            2,
            std::string(policy_table::EnumToJsonString(policy_table::P_EMPTY)));
        query_parameter.Bind(3, group_id);
        if (!query_parameter.Exec() || !query_parameter.Reset()) {
          SDL_LOG_WARN("Incorrect insert into rpc with parameter");
          return false;
        }
      } else {
        query.Bind(0, it->first);
        query.Bind(1, std::string(policy_table::EnumToJsonString(*hmi_it)));
        query.Bind(2, group_id);
        if (!query.Exec() || !query.Reset()) {
          SDL_LOG_WARN("Incorrect insert into rpc");
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
    SDL_LOG_WARN("Incorrect delete from app_group.");
    return false;
  }
  if (!query_delete.Exec(sql_pt::kDeleteModuleTypes)) {
    SDL_LOG_WARN("Incorrect delete from module_type.");
    return false;
  }
  if (!query_delete.Exec(sql_pt::kDeleteApplication)) {
    SDL_LOG_WARN("Incorrect delete from application.");
    return false;
  }

  if (!query_delete.Exec(sql_pt::kDeleteRequestType)) {
    SDL_LOG_WARN("Incorrect delete from request type.");
    return false;
  }

  if (!query_delete.Exec(sql_pt::kDeleteRequestSubType)) {
    SDL_LOG_WARN("Incorrect delete from request subtype.");
    return false;
  }

  if (!query_delete.Exec(sql_pt::kDeleteAppServiceHandledRpcs)) {
    SDL_LOG_WARN("Incorrect delete from handled rpcs.");
    return false;
  }

  if (!query_delete.Exec(sql_pt::kDeleteAppServiceNames)) {
    SDL_LOG_WARN("Incorrect delete from service names.");
    return false;
  }

  if (!query_delete.Exec(sql_pt::kDeleteAppServiceTypes)) {
    SDL_LOG_WARN("Incorrect delete from handled service types.");
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
    SDL_LOG_WARN("Incorrect insert statement into application (device).");
    return false;
  }

  app_query.Bind(0, app.first);
  app_query.Bind(
      1, std::string(policy_table::EnumToJsonString(app.second.priority)));
  app_query.Bind(2, app.second.is_null());
  app_query.Bind(3, static_cast<int64_t>(*app.second.heart_beat_timeout_ms));
  app.second.certificate.is_initialized()
      ? app_query.Bind(4, *app.second.certificate)
      : app_query.Bind(4);
  app.second.hybrid_app_preference.is_initialized()
      ? app_query.Bind(5,
                       std::string(policy_table::EnumToJsonString(
                           *app.second.hybrid_app_preference)))
      : app_query.Bind(5);
  app.second.endpoint.is_initialized() ? app_query.Bind(6, *app.second.endpoint)
                                       : app_query.Bind(6);
  app.second.enabled.is_initialized() ? app_query.Bind(7, *app.second.enabled)
                                      : app_query.Bind(7);
  app.second.auth_token.is_initialized()
      ? app_query.Bind(8, *app.second.auth_token)
      : app_query.Bind(8);
  app.second.cloud_transport_type.is_initialized()
      ? app_query.Bind(9, *app.second.cloud_transport_type)
      : app_query.Bind(9);
  app.second.icon_url.is_initialized()
      ? app_query.Bind(10, *app.second.icon_url)
      : app_query.Bind(10);
  app.second.allow_unknown_rpc_passthrough.is_initialized()
      ? app_query.Bind(11, *app.second.allow_unknown_rpc_passthrough)
      : app_query.Bind(11);

  app.second.encryption_required.is_initialized()
      ? app_query.Bind(12, *app.second.encryption_required)
      : app_query.Bind(12);

  if (!app_query.Exec() || !app_query.Reset()) {
    SDL_LOG_WARN("Incorrect insert into application.");
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
  bool denied = !app.second.moduleType->is_initialized();
  if (!SaveRemoteControlDenied(app.first, denied) ||
      !SaveModuleType(app.first, *app.second.moduleType)) {
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

  if (!SaveRequestSubType(app.first, *app.second.RequestSubType)) {
    return false;
  }

  if (!SaveAppServiceParameters(app.first,
                                *app.second.app_service_parameters)) {
    return false;
  }

  return true;
}

bool policy::SQLPTRepresentation::SaveDevicePolicy(
    const policy_table::DevicePolicy& device) {
  utils::dbms::SQLQuery app_query(db());
  if (!app_query.Prepare(sql_pt::kInsertApplication)) {
    SDL_LOG_WARN("Incorrect insert statement into application.");
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
    SDL_LOG_WARN("Incorrect insert into application.");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveAppGroup(
    const std::string& app_id, const policy_table::Strings& app_groups) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertAppGroup)) {
    SDL_LOG_WARN("Incorrect insert statement for app group");
    return false;
  }
  SDL_LOG_INFO("SaveAppGroup");
  policy_table::Strings::const_iterator it;
  for (it = app_groups.begin(); it != app_groups.end(); ++it) {
    std::string ssss = *it;
    SDL_LOG_INFO("Group: " << ssss);
    query.Bind(0, app_id);
    query.Bind(1, *it);
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into app group."
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
    SDL_LOG_WARN("Incorrect insert statement for nickname");
    return false;
  }

  policy_table::Strings::const_iterator it;
  for (it = nicknames.begin(); it != nicknames.end(); ++it) {
    query.Bind(0, app_id);
    query.Bind(1, *it);
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into nickname.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveAppType(const std::string& app_id,
                                      const policy_table::AppHMITypes& types) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertAppType)) {
    SDL_LOG_WARN("Incorrect insert statement for app type");
    return false;
  }

  policy_table::AppHMITypes::const_iterator it;
  for (it = types.begin(); it != types.end(); ++it) {
    query.Bind(0, app_id);
    query.Bind(1, std::string(policy_table::EnumToJsonString(*it)));
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into app type.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveRequestType(
    const std::string& app_id, const policy_table::RequestTypes& types) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertRequestType)) {
    SDL_LOG_WARN("Incorrect insert statement for request types.");
    return false;
  }

  policy_table::RequestTypes::const_iterator it;
  if (!types.empty()) {
    SDL_LOG_WARN("Request types not empty.");
    for (it = types.begin(); it != types.end(); ++it) {
      query.Bind(0, app_id);
      query.Bind(1, std::string(policy_table::EnumToJsonString(*it)));
      if (!query.Exec() || !query.Reset()) {
        SDL_LOG_WARN("Incorrect insert into request types.");
        return false;
      }
    }
  } else if (types.is_initialized()) {
    SDL_LOG_WARN("Request types empty.");
    query.Bind(0, app_id);
    query.Bind(1,
               std::string(policy_table::EnumToJsonString(
                   policy_table::RequestType::RT_EMPTY)));
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into request types.");
      return false;
    }
  }
  return true;
}

bool SQLPTRepresentation::SaveRequestSubType(
    const std::string& app_id,
    const policy_table::RequestSubTypes& request_subtypes) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertRequestSubType)) {
    SDL_LOG_WARN("Incorrect insert statement for request subtypes.");
    return false;
  }

  policy_table::Strings::const_iterator it;
  if (!request_subtypes.empty()) {
    SDL_LOG_TRACE("Request subtypes are not empty.");
    for (it = request_subtypes.begin(); it != request_subtypes.end(); ++it) {
      query.Bind(0, app_id);
      query.Bind(1, *it);
      if (!query.Exec() || !query.Reset()) {
        SDL_LOG_WARN("Incorrect insert into request subtypes.");
        return false;
      }
    }
  } else if (request_subtypes.is_initialized()) {
    SDL_LOG_WARN("Request subtypes empty.");
    query.Bind(0, app_id);
    query.Bind(1,
               std::string(policy_table::EnumToJsonString(
                   policy_table::RequestType::RT_EMPTY)));
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into request subtypes.");
      return false;
    }
  }
  return true;
}

bool SQLPTRepresentation::SaveAppServiceParameters(
    const std::string& app_id,
    const policy_table::AppServiceParameters& app_service_parameters) {
  SDL_LOG_INFO("Save app service parameters");
  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(sql_pt::kInsertAppServiceTypes)) {
    SDL_LOG_WARN("Incorrect insert statement for app service types");
    return false;
  }
  policy_table::AppServiceParameters::const_iterator it;
  for (it = app_service_parameters.begin(); it != app_service_parameters.end();
       ++it) {
    // Create service type id from hashing app_id and service_type
    std::string str_to_hash = std::string(app_id + it->first);
    const long int id = abs(CacheManager::GenerateHash(str_to_hash));
    query.Bind(0, static_cast<int64_t>(id));
    query.Bind(1, it->first);
    query.Bind(2, app_id);
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Insert execute failed for into app service types");
      return false;
    }

    // Insert app names array into db
    utils::dbms::SQLQuery service_name_query(db());
    if (!service_name_query.Prepare(sql_pt::kInsertAppServiceNames)) {
      SDL_LOG_WARN("Incorrect insert statement for app service names");
      return false;
    }

    auto app_service_names = it->second.service_names;

    if (app_service_names.is_initialized() && app_service_names->empty()) {
      // App service names is an empty array
      SDL_LOG_DEBUG("App Service Names is Empty Array");
      service_name_query.Bind(0, static_cast<int64_t>(id));
      service_name_query.Bind(1);
      if (!service_name_query.Exec() || !service_name_query.Reset()) {
        SDL_LOG_WARN("Incorrect insert into empty app service names");
        return false;
      }
    } else {
      policy_table::AppServiceNames::const_iterator names_it;
      for (names_it = app_service_names->begin();
           names_it != app_service_names->end();
           ++names_it) {
        service_name_query.Bind(0, static_cast<int64_t>(id));
        service_name_query.Bind(1, *names_it);
        if (!service_name_query.Exec() || !service_name_query.Reset()) {
          SDL_LOG_WARN("Incorrect insert into app service names");
          return false;
        }
      }
    }

    // Insert handled rpcs array into db
    utils::dbms::SQLQuery handled_rpcs_query(db());
    if (!handled_rpcs_query.Prepare(sql_pt::kInsertAppServiceHandledRpcs)) {
      SDL_LOG_WARN("Incorrect insert statement for app service handled rpcs");
      return false;
    }

    auto handled_rpcs = it->second.handled_rpcs;
    policy_table::AppServiceHandledRpcs::const_iterator rpc_it;
    for (rpc_it = handled_rpcs.begin(); rpc_it != handled_rpcs.end();
         ++rpc_it) {
      handled_rpcs_query.Bind(0, static_cast<int64_t>(id));
      handled_rpcs_query.Bind(1, static_cast<int32_t>(rpc_it->function_id));
      if (!handled_rpcs_query.Exec() || !handled_rpcs_query.Reset()) {
        SDL_LOG_WARN("Incorrect insert into app service handled rpcs");
        return false;
      }
    }
  }
  return true;
}

bool SQLPTRepresentation::SaveModuleMeta(const policy_table::ModuleMeta& meta) {
  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(sql_pt::kSaveModuleMeta)) {
    SDL_LOG_WARN("Incorrect insert statement for module_meta.");
    return false;
  }
  const int64_t odometer = *(meta.pt_exchanged_at_odometer_x);

  query.Bind(0, odometer);
  query.Bind(1, *(meta.pt_exchanged_x_days_after_epoch));
  query.Bind(2, *(meta.ignition_cycles_since_last_exchange));

  if (!query.Exec()) {
    SDL_LOG_WARN("Incorrect update for module_meta.");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveModuleConfig(
    const policy_table::ModuleConfig& config) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateModuleConfig)) {
    SDL_LOG_WARN("Incorrect update statement for module config");
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
  config.lock_screen_dismissal_enabled.is_initialized()
      ? query.Bind(9, *(config.lock_screen_dismissal_enabled))
      : query.Bind(9);
  if (!query.Exec()) {
    SDL_LOG_WARN("Incorrect update module config");
    return false;
  }

  if (!SaveSecondsBetweenRetries(config.seconds_between_retries)) {
    return false;
  }

  if (!SaveNumberOfNotificationsPerMinute(
          config.notifications_per_minute_by_priority)) {
    return false;
  }

  if (!SaveNumberOfSubtleNotificationsPerMinute(
          *config.subtle_notifications_per_minute_by_priority)) {
    return false;
  }

  if (!SaveServiceEndpoints(config.endpoints)) {
    return false;
  }

  if (!SaveServiceEndpointProperties(*config.endpoint_properties)) {
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveServiceEndpoints(
    const policy_table::ServiceEndpoints& endpoints) {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDeleteEndpoint)) {
    SDL_LOG_WARN("Incorrect delete from endpoint.");
    return false;
  }

  if (!query.Prepare(sql_pt::kInsertEndpoint)) {
    SDL_LOG_WARN("Incorrect insert statement for endpoint");
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
          SDL_LOG_WARN("Incorrect insert into endpoint");
          return false;
        }
      }
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveServiceEndpointProperties(
    const policy_table::ServiceEndpointProperties& endpoint_properties) {
  utils::dbms::SQLQuery query(db());

  if (endpoint_properties.is_initialized() && endpoint_properties.empty()) {
    bool delete_query_exec_result =
        query.Exec(sql_pt::kDeleteEndpointProperties);

    if (!delete_query_exec_result) {
      SDL_LOG_WARN("Failed to delete endpoint properties from DB.");
      return false;
    }
    return true;
  }

  if (!query.Prepare(sql_pt::kInsertEndpointVersion)) {
    SDL_LOG_WARN(
        "Incorrect insert of endpoint property to endpoint_properties.");
    return false;
  }

  for (auto& endpoint_property : endpoint_properties) {
    query.Bind(0, endpoint_property.first);
    endpoint_property.second.version.is_initialized()
        ? query.Bind(1, *endpoint_property.second.version)
        : query.Bind(1);

    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN(
          "Failed to insert endpoint property into endpoint_properties.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveConsumerFriendlyMessages(
    const policy_table::ConsumerFriendlyMessages& messages) {
  SDL_LOG_AUTO_TRACE();

  // According CRS-2419  If there is no “consumer_friendly_messages” key,
  // the current local consumer_friendly_messages section shall be maintained in
  // the policy table. So it won't be changed/updated
  if (!messages.messages.is_initialized()) {
    SDL_LOG_INFO("ConsumerFriendlyMessages messages list is empty");
    return true;
  }

  utils::dbms::SQLQuery query(db());
  bool delete_query_exec_result = true;
  delete_query_exec_result = query.Exec(sql_pt::kDeleteMessageString);

  if (!delete_query_exec_result) {
    SDL_LOG_WARN("Failed to delete messages from DB.");
    return false;
  }

  if (!query.Prepare(sql_pt::kUpdateVersion)) {
    SDL_LOG_WARN("Invalid update messages version statement.");
    return false;
  }

  query.Bind(0, messages.version);
  if (!query.Exec()) {
    SDL_LOG_WARN("Failed to update messages version number in DB.");
    return false;
  }

  policy_table::Messages::const_iterator it;
  for (it = messages.messages->begin(); it != messages.messages->end(); ++it) {
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

  return true;
}

bool SQLPTRepresentation::SaveMessageType(const std::string& type) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertMessageType)) {
    SDL_LOG_WARN("Incorrect insert statement for message type.");
    return false;
  }

  query.Bind(0, type);
  if (!query.Exec()) {
    SDL_LOG_WARN("Incorrect insert into message type.");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::SaveLanguage(const std::string& code) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertLanguage)) {
    SDL_LOG_WARN("Incorrect insert statement for language.");
    return false;
  }

  query.Bind(0, code);
  if (!query.Exec()) {
    SDL_LOG_WARN("Incorrect insert into language.");
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
    SDL_LOG_WARN("Incorrect insert statement for message.");
    return false;
  }

  query.Bind(0, lang);
  query.Bind(1, type);
  if (strings.tts.is_valid() && strings.tts.is_initialized()) {
    query.Bind(2, (*strings.tts));
  }

  if (strings.label.is_valid() && strings.label.is_initialized()) {
    query.Bind(3, (*strings.label));
  }

  if (strings.line1.is_valid() && strings.line1.is_initialized()) {
    query.Bind(4, *(strings.line1));
  }

  if (strings.line2.is_valid() && strings.line2.is_initialized()) {
    query.Bind(5, (*strings.line2));
  }

  if (strings.textBody.is_valid() && strings.textBody.is_initialized()) {
    query.Bind(6, (*strings.textBody));
  }

  if (!query.Exec() || !query.Reset()) {
    SDL_LOG_WARN("Incorrect insert into message.");
    return false;
  }
  return true;
}

bool SQLPTRepresentation::SaveSecondsBetweenRetries(
    const policy_table::SecondsBetweenRetries& seconds) {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kDeleteSecondsBetweenRetries)) {
    SDL_LOG_WARN("Incorrect delete from seconds between retries.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertSecondsBetweenRetry)) {
    SDL_LOG_WARN("Incorrect insert statement for seconds between retries.");
    return false;
  }

  for (uint32_t i = 0; i < seconds.size(); ++i) {
    query.Bind(0, static_cast<int>(i));
    query.Bind(1, seconds[i]);
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into seconds between retries.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveNumberOfNotificationsPerMinute(
    const policy_table::NumberOfNotificationsPerMinute& notifications) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertNotificationsByPriority)) {
    SDL_LOG_WARN("Incorrect insert statement for notifications by priority.");
    return false;
  }

  policy_table::NumberOfNotificationsPerMinute::const_iterator it;
  for (it = notifications.begin(); it != notifications.end(); ++it) {
    query.Bind(0, it->first);
    query.Bind(1, it->second);
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into notifications by priority.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveNumberOfSubtleNotificationsPerMinute(
    const policy_table::NumberOfNotificationsPerMinute& notifications) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertSubtleNotificationsByPriority)) {
    SDL_LOG_WARN("Incorrect insert statement for notifications by priority.");
    return false;
  }

  policy_table::NumberOfNotificationsPerMinute::const_iterator it;
  for (it = notifications.begin(); it != notifications.end(); ++it) {
    query.Bind(0, it->first);
    query.Bind(1, it->second);
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into notifications by priority.");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::SaveDeviceData(
    const policy_table::DeviceData& devices) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertDeviceData)) {
    SDL_LOG_WARN("Incorrect insert statement for device data.");
    return false;
  }

  policy_table::DeviceData::const_iterator it;
  for (it = devices.begin(); it != devices.end(); ++it) {
    query.Bind(0, it->first);
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into device data.");
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
    SDL_LOG_WARN("Incorrect delete from app level.");
    return false;
  }
  if (!query.Prepare(sql_pt::kInsertAppLevel)) {
    SDL_LOG_WARN("Incorrect insert statement for app level.");
    return false;
  }

  policy_table::AppLevels::const_iterator it;
  const policy_table::AppLevels& app_levels = *counts.app_level;
  const_cast<policy_table::AppLevels&>(*counts.app_level).mark_initialized();
  for (it = app_levels.begin(); it != app_levels.end(); ++it) {
    query.Bind(0, it->first);
    query.Bind(1, it->second.count_of_tls_errors);
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into app level.");
      return false;
    }
  }
  return true;
}

bool SQLPTRepresentation::SaveVehicleData(
    const policy_table::VehicleData& vehicle_data) {
  SDL_LOG_AUTO_TRACE();
  if (vehicle_data.is_initialized() &&
      vehicle_data.schema_version.is_initialized()) {
    utils::dbms::SQLQuery query(db());
    if (!query.Prepare(sql_pt::kInsertVehicleDataSchemaVersion)) {
      SDL_LOG_WARN("Incorrect insert of schema_version to vehicle_data.");
      return false;
    }
    query.Bind(0, (*vehicle_data.schema_version));
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Failed to insert schema_version to vehicle_data.");
      return false;
    }
  }

  auto vehicle_data_items = vehicle_data.is_initialized() &&
                                    vehicle_data.schema_items.is_initialized()
                                ? *(vehicle_data.schema_items)
                                : policy_table::VehicleDataItems();

  return SaveVehicleDataItems(vehicle_data_items);
}

bool SQLPTRepresentation::SaveVehicleDataItems(
    const policy_table::VehicleDataItems& vehicle_data_items) {
  SDL_LOG_AUTO_TRACE();
  DeleteVehicleDataItems();
  for (const auto& item : vehicle_data_items) {
    if (!InsertVehicleDataItem(item)) {
      return false;
    }
  }
  return true;
}

void SQLPTRepresentation::IncrementIgnitionCycles() {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kIncrementIgnitionCycles)) {
    SDL_LOG_WARN("Failed incrementing ignition cycles");
  }
}

void SQLPTRepresentation::ResetIgnitionCycles() {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(sql_pt::kResetIgnitionCycles)) {
    SDL_LOG_WARN("Failed to reset ignition cycles number.");
  }
}

bool SQLPTRepresentation::UpdateRequired() const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectFlagUpdateRequired) || !query.Exec()) {
    SDL_LOG_WARN("Failed select update required flag from module meta");
    return false;
  }
  return query.GetBoolean(0);
}

void SQLPTRepresentation::SaveUpdateRequired(bool value) {
  utils::dbms::SQLQuery query(db());
  // TODO(AOleynik): Quick fix, will be reworked
  if (!query.Prepare(/*sql_pt::kUpdateFlagUpdateRequired*/
                     "UPDATE `module_meta` SET `flag_update_required` = ?")) {
    SDL_LOG_WARN("Incorrect update into module meta (update_required): "
                 << strerror(errno));
    return;
  }
  query.Bind(0, value);
  if (!query.Exec()) {
    SDL_LOG_WARN("Failed update module meta (update_required)");
  }
}

bool SQLPTRepresentation::GetInitialAppData(const std::string& app_id,
                                            StringArray* nicknames,
                                            StringArray* app_types) {
  SDL_LOG_INFO("Getting initial application data.");
  utils::dbms::SQLQuery app_names(db());
  if (!app_names.Prepare(sql_pt::kSelectNicknames)) {
    SDL_LOG_WARN("Incorrect select from app nicknames");
    return false;
  }
  utils::dbms::SQLQuery app_hmi_types(db());
  if (!app_hmi_types.Prepare(sql_pt::kSelectAppTypes)) {
    SDL_LOG_WARN("Incorrect select from app types");
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
  SDL_LOG_AUTO_TRACE();
  return GatherFunctionalGroupings(&groups);
}

bool SQLPTRepresentation::GatherAppType(
    const std::string& app_id, policy_table::AppHMITypes* app_types) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectAppTypes)) {
    SDL_LOG_WARN("Incorrect select from app types");
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
    SDL_LOG_WARN("Incorrect select from request types.");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    policy_table::RequestType type;
    if (!policy_table::EnumFromJsonString(query.GetString(0), &type)) {
      return false;
    }
    if (policy_table::RequestType::RT_EMPTY == type) {
      request_types->mark_initialized();
      continue;
    }
    request_types->push_back(type);
  }
  return true;
}

bool SQLPTRepresentation::GatherRequestSubType(
    const std::string& app_id,
    policy_table::RequestSubTypes* request_subtypes) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectRequestSubTypes)) {
    SDL_LOG_WARN("Incorrect select from request subtypes.");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    const std::string request_subtype = query.GetString(0);
    if (policy_table::EnumToJsonString(policy_table::RT_EMPTY) ==
        request_subtype) {
      request_subtypes->mark_initialized();
      continue;
    }
    request_subtypes->push_back(request_subtype);
  }
  return true;
}

bool SQLPTRepresentation::GatherAppServiceParameters(
    const std::string& app_id,
    policy_table::AppServiceParameters* app_service_parameters) const {
  SDL_LOG_INFO("Gather app service info");
  utils::dbms::SQLQuery service_type_query(db());
  if (!service_type_query.Prepare(sql_pt::kSelectAppServiceTypes)) {
    SDL_LOG_WARN("Incorrect select from service_types");
    return false;
  }

  utils::dbms::SQLQuery service_name_query(db());
  if (!service_name_query.Prepare(sql_pt::kSelectAppServiceNames)) {
    SDL_LOG_WARN("Incorrect select all from app_service_names");
    return false;
  }

  utils::dbms::SQLQuery handled_rpcs_query(db());
  if (!handled_rpcs_query.Prepare(sql_pt::kSelectAppServiceHandledRpcs)) {
    SDL_LOG_WARN("Incorrect select all from app_service_handled_rpcs");
    return false;
  }

  service_type_query.Bind(0, app_id);
  while (service_type_query.Next()) {
    const int service_type_id = service_type_query.GetInteger(0);
    std::string service_type = service_type_query.GetString(1);
    SDL_LOG_WARN("Load service type from pt: " << service_type);
    (*app_service_parameters)[service_type] = policy_table::AppServiceInfo();

    service_name_query.Bind(0, service_type_id);
    while (service_name_query.Next()) {
      SDL_LOG_DEBUG("Loading service name");
      (*app_service_parameters)[service_type].service_names->push_back(
          service_name_query.GetString(0));
      (*app_service_parameters)[service_type].service_names->mark_initialized();
    }

    handled_rpcs_query.Bind(0, service_type_id);
    while (handled_rpcs_query.Next()) {
      policy_table::AppServiceHandledRpc handled_rpc;
      handled_rpc.function_id = handled_rpcs_query.GetInteger(0);
      (*app_service_parameters)[service_type].handled_rpcs.push_back(
          handled_rpc);
    }

    if (!service_name_query.Reset()) {
      SDL_LOG_ERROR("Could not reset service_name query");
      return false;
    }
    if (!handled_rpcs_query.Reset()) {
      SDL_LOG_ERROR("Could not reset handled_rpcs query");
      return false;
    }
  }

  return true;
}

bool SQLPTRepresentation::GatherNickName(
    const std::string& app_id, policy_table::Strings* nicknames) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectNicknames)) {
    SDL_LOG_WARN("Incorrect select from app nicknames");
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
    SDL_LOG_WARN("Incorrect select from app groups");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    app_groups->push_back(query.GetString(0));
  }
  return true;
}

bool SQLPTRepresentation::GatherRemoteControlDenied(const std::string& app_id,
                                                    bool* denied) const {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectRemoteControlDenied)) {
    SDL_LOG_WARN("Incorrect select remote control flag");
    return false;
  }
  query.Bind(0, app_id);
  if (query.Next()) {
    *denied = query.GetBoolean(0);
  } else {
    return false;
  }
  return true;
}

bool SQLPTRepresentation::GatherModuleType(
    const std::string& app_id, policy_table::ModuleTypes* app_types) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectModuleTypes)) {
    SDL_LOG_WARN("Incorrect select from app types");
    return false;
  }

  query.Bind(0, app_id);
  while (query.Next()) {
    policy_table::ModuleType type;
    if (!policy_table::EnumFromJsonString(query.GetString(0), &type)) {
      return false;
    }
    if (policy_table::ModuleType::MT_EMPTY == type) {
      app_types->mark_initialized();
      continue;
    }
    app_types->push_back(type);
  }
  return true;
}

bool SQLPTRepresentation::SaveRemoteControlDenied(const std::string& app_id,
                                                  bool deny) {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateRemoteControlDenied)) {
    SDL_LOG_WARN("Incorrect update statement for remote control flag");
    return false;
  }
  SDL_LOG_DEBUG("App: " << app_id << std::boolalpha << " - " << deny);
  query.Bind(0, deny);
  query.Bind(1, app_id);
  if (!query.Exec()) {
    SDL_LOG_WARN("Incorrect update remote control flag.");
    return false;
  }
  return true;
}

bool SQLPTRepresentation::SaveModuleType(
    const std::string& app_id, const policy_table::ModuleTypes& types) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertModuleType)) {
    SDL_LOG_WARN("Incorrect insert statement for module type");
    return false;
  }

  policy_table::ModuleTypes::const_iterator it;
  if (!types.empty()) {
    for (it = types.begin(); it != types.end(); ++it) {
      query.Bind(0, app_id);
      std::string module(policy_table::EnumToJsonString(*it));
      query.Bind(1, module);
      SDL_LOG_DEBUG("Module(app: " << app_id << ", type: " << module << ")");
      if (!query.Exec() || !query.Reset()) {
        SDL_LOG_WARN("Incorrect insert into module type.");
        return false;
      }
    }
  } else if (types.is_initialized()) {
    query.Bind(0, app_id);
    query.Bind(1,
               std::string(policy_table::EnumToJsonString(
                   policy_table::ModuleType::MT_EMPTY)));
    if (!query.Exec() || !query.Reset()) {
      SDL_LOG_WARN("Incorrect insert into module types.");
      return false;
    }
  } else {
    SDL_LOG_WARN("Module Type omitted.");
  }
  return true;
}

bool SQLPTRepresentation::SaveAccessModule(
    TypeAccess access, const policy_table::AccessModules& modules) {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertAccessModule)) {
    SDL_LOG_WARN("Incorrect insert statement for access module");
    return false;
  }

  policy_table::AccessModules::const_iterator i;
  for (i = modules.begin(); i != modules.end(); ++i) {
    const std::string& name = i->first;
    const policy_table::RemoteRpcs& rpcs = i->second;
    query.Bind(0, name);
    query.Bind(1, access);
    if (!query.Exec()) {
      SDL_LOG_WARN("Incorrect insert into access module.");
      return false;
    }
    int id = query.LastInsertId();
    if (!query.Reset()) {
      SDL_LOG_WARN("Couldn't reset query access module.");
      return false;
    }
    if (!SaveRemoteRpc(id, rpcs)) {
      return false;
    }
  }
  return true;
}

bool SQLPTRepresentation::GatherAccessModule(
    TypeAccess access, policy_table::AccessModules* modules) const {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectAccessModules)) {
    SDL_LOG_WARN("Incorrect select from access module");
    return false;
  }

  query.Bind(0, access);
  while (query.Next()) {
    int id = query.GetInteger(0);
    std::string name = query.GetString(1);
    policy_table::RemoteRpcs rpcs;
    if (!GatherRemoteRpc(id, &rpcs)) {
      return false;
    }
    modules->insert(std::make_pair(name, rpcs));
  }
  return true;
}

bool SQLPTRepresentation::SaveRemoteRpc(int module_id,
                                        const policy_table::RemoteRpcs& rpcs) {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertRemoteRpc)) {
    SDL_LOG_WARN("Incorrect insert statement for remote rpc");
    return false;
  }
  policy_table::RemoteRpcs::const_iterator i;
  for (i = rpcs.begin(); i != rpcs.end(); ++i) {
    const std::string& name = i->first;
    const policy_table::Strings& params = i->second;
    policy_table::Strings::const_iterator j;
    if (params.empty()) {
      query.Bind(0, module_id);
      query.Bind(1, name);
      query.Bind(2);
      if (!query.Exec() || !query.Reset()) {
        SDL_LOG_WARN("Incorrect insert into remote rpc.");
        return false;
      }
    } else {
      for (j = params.begin(); j != params.end(); ++j) {
        const std::string& param = *j;
        query.Bind(0, module_id);
        query.Bind(1, name);
        query.Bind(2, param);
        if (!query.Exec() || !query.Reset()) {
          SDL_LOG_WARN("Incorrect insert into remote rpc.");
          return false;
        }
      }
    }
  }
  return true;
}

bool SQLPTRepresentation::GatherRemoteRpc(
    int module_id, policy_table::RemoteRpcs* rpcs) const {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectRemoteRpcs)) {
    SDL_LOG_WARN("Incorrect select from remote rpc");
    return false;
  }

  query.Bind(0, module_id);
  while (query.Next()) {
    std::string name = query.GetString(0);
    if (!query.IsNull(1)) {
      std::string parameter = query.GetString(1);
      (*rpcs)[name].push_back(parameter);
    } else {
      rpcs->insert(std::make_pair(name, policy_table::Strings()));
    }
  }
  return true;
}

bool SQLPTRepresentation::SaveApplicationCustomData(const std::string& app_id,
                                                    bool is_revoked,
                                                    bool is_default,
                                                    bool is_predata) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateApplicationCustomData)) {
    SDL_LOG_WARN("Incorrect update in application");
    return false;
  }

  query.Bind(0, is_revoked);
  query.Bind(1, is_default);
  query.Bind(2, is_predata);
  query.Bind(3, app_id);

  if (!query.Exec()) {
    SDL_LOG_WARN("Failed update in application");
    return false;
  }

  return true;
}

bool SQLPTRepresentation::IsApplicationRevoked(
    const std::string& app_id) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectApplicationRevoked)) {
    SDL_LOG_WARN("Incorrect select from is_revoked of application");
  }

  query.Bind(0, app_id);

  if (!query.Exec()) {
    SDL_LOG_WARN("Failed select is_revoked of application");
    return false;
  }
  return query.IsNull(0) ? false : query.GetBoolean(0);
}
bool SQLPTRepresentation::IsApplicationRepresented(
    const std::string& app_id) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectApplicationRepresented)) {
    SDL_LOG_WARN("Incorrect select application by id");
    return false;
  }

  query.Bind(0, app_id);
  if (!query.Exec()) {
    SDL_LOG_WARN("Failed select application by id");
    return false;
  }
  return query.GetInteger(0) != 0;
}

bool SQLPTRepresentation::IsDefaultPolicy(const std::string& app_id) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectApplicationIsDefault)) {
    SDL_LOG_WARN("Incorrect select application by id");
    return false;
  }

  query.Bind(0, app_id);
  if (!query.Exec()) {
    SDL_LOG_WARN("Failed select application by id");
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
    SDL_LOG_ERROR("Incorrect statement to delete from app_group.");
    return false;
  }
  query.Bind(0, app_id);
  if (!query.Exec()) {
    SDL_LOG_ERROR("Failed deleting from app_group.");
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

  policy_table::Strings request_subtypes;
  if (!GatherRequestSubType(kDefaultId, &request_subtypes) ||
      !SaveRequestSubType(app_id, request_subtypes)) {
    return false;
  }

  policy_table::AppHMITypes app_types;
  if (!GatherAppType(kDefaultId, &app_types) ||
      !SaveAppType(app_id, app_types)) {
    return false;
  }

  policy_table::AppServiceParameters app_service_parameters;
  if (!GatherAppServiceParameters(kDefaultId, &app_service_parameters) ||
      !SaveAppServiceParameters(app_id, app_service_parameters)) {
    return false;
  }

  return SetIsDefault(app_id, true);
}

bool SQLPTRepresentation::SetIsDefault(const std::string& app_id,
                                       bool is_default) const {
  SDL_LOG_TRACE("Set flag is_default of application");
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateIsDefault)) {
    SDL_LOG_WARN("Incorect statement for updating is_default");
    return false;
  }

  query.Bind(0, is_default);
  query.Bind(1, app_id);
  if (!query.Exec()) {
    SDL_LOG_WARN("Failed update is_default");
    return false;
  }
  return true;
}

void SQLPTRepresentation::RemoveDB() const {
  file_system::DeleteFile(db_->get_path());
}

bool SQLPTRepresentation::IsDBVersionActual() const {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectDBVersion) || !query.Exec()) {
    SDL_LOG_ERROR("Failed to get DB version: " << query.LastError().text());
    return false;
  }

  const int32_t saved_db_version = query.GetInteger(0);
  const int32_t current_db_version = GetDBVersion();
  SDL_LOG_DEBUG("Saved DB version is: " << saved_db_version
                                        << ". Current DB vesion is: "
                                        << current_db_version);

  return current_db_version == saved_db_version;
}

bool SQLPTRepresentation::UpdateDBVersion() const {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdateDBVersion)) {
    SDL_LOG_ERROR("Incorrect DB version query: " << query.LastError().text());
    return false;
  }

  const int32_t db_version = GetDBVersion();
  SDL_LOG_DEBUG("DB version will be updated to: " << db_version);
  query.Bind(0, db_version);

  if (!query.Exec()) {
    SDL_LOG_ERROR("DB version getting failed: " << query.LastError().text());
    return false;
  }

  return true;
}

const int32_t SQLPTRepresentation::GetDBVersion() const {
  return utils::Djb2HashFromString(sql_pt::kCreateSchema);
}

utils::dbms::SQLDatabase* SQLPTRepresentation::db() const {
  return db_;
}

bool SQLPTRepresentation::CopyApplication(const std::string& source,
                                          const std::string& destination) {
  utils::dbms::SQLQuery source_app(db());
  if (!source_app.Prepare(sql_pt::kSelectApplicationFull)) {
    SDL_LOG_WARN("Incorrect select statement from application.");
    return false;
  }
  source_app.Bind(0, source);
  if (!source_app.Exec()) {
    SDL_LOG_WARN("Failed selecting from application.");
    return false;
  }

  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kInsertApplicationFull)) {
    SDL_LOG_WARN("Incorrect insert statement into application.");
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
  source_app.IsNull(10) ? query.Bind(11)
                        : query.Bind(11, source_app.GetString(10));
  source_app.IsNull(11) ? query.Bind(12)
                        : query.Bind(12, source_app.GetString(11));
  source_app.IsNull(12) ? query.Bind(13)
                        : query.Bind(13, source_app.GetBoolean(12));
  source_app.IsNull(13) ? query.Bind(14)
                        : query.Bind(14, source_app.GetString(13));
  source_app.IsNull(14) ? query.Bind(15)
                        : query.Bind(15, source_app.GetString(14));
  source_app.IsNull(15) ? query.Bind(16)
                        : query.Bind(16, source_app.GetString(15));
  source_app.IsNull(16) ? query.Bind(17)
                        : query.Bind(17, source_app.GetBoolean(16));
  source_app.IsNull(17) ? query.Bind(18)
                        : query.Bind(18, source_app.GetBoolean(17));

  if (!query.Exec()) {
    SDL_LOG_WARN("Failed inserting into application.");
    return false;
  }
  return true;
}

void SQLPTRepresentation::SetPreloaded(bool value) {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kUpdatePreloaded)) {
    SDL_LOG_WARN("Incorrect statement of updating preloaded.");
    return;
  }

  query.Bind(0, value);
  if (!query.Exec()) {
    SDL_LOG_WARN("Failed updating preloaded.");
    return;
  }
}

bool SQLPTRepresentation::SetVINValue(const std::string& value) {
  return true;
}

bool SQLPTRepresentation::VehicleDataItemExists(
    const policy_table::VehicleDataItem& vehicle_data_item) const {
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(sql_pt::kSelectVehicleDataItemWithVersion)) {
    SDL_LOG_ERROR("Incorrent select statement for vehicle data item. "
                  << query.LastError().text());
    return false;
  }

  query.Bind(0, vehicle_data_item.name);
  query.Bind(1, vehicle_data_item.key);
  vehicle_data_item.since.is_initialized()
      ? query.Bind(2, std::string(*vehicle_data_item.since))
      : query.Bind(2);
  vehicle_data_item.until.is_initialized()
      ? query.Bind(3, std::string(*vehicle_data_item.until))
      : query.Bind(3);

  if (!query.Exec()) {
    SDL_LOG_ERROR("Failed to retrieve vehicle data item: "
                  << std::string(vehicle_data_item.key)
                  << ". Error: " << query.LastError().text());
    return false;
  }
  return !query.IsNull(0);
}

policy_table::VehicleDataItems SQLPTRepresentation::GetVehicleDataItem(
    const std::string& name, const std::string& key) const {
  policy_table::VehicleDataItems result;
  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(sql_pt::kSelectVehicleDataItem)) {
    SDL_LOG_ERROR("Incorrent select statement of vehicle data item. "
                  << query.LastError().text());
    return result;
  }

  query.Bind(0, name);
  query.Bind(1, key);

  while (query.Next()) {
    auto vdi = PopulateVDIFromQuery(query);
    if (!vdi.is_initialized()) {
      return policy_table::VehicleDataItems();
    }

    utils::dbms::SQLQuery param_query(db());
    if (policy_table::VehicleDataItem::kStruct ==
        static_cast<std::string>(vdi.type)) {
      if (!param_query.Prepare(sql_pt::kSelectVehicleDataItemParams)) {
        SDL_LOG_ERROR("Incorrent select statement of vehicle data item. "
                      << param_query.LastError().text());
        return policy_table::VehicleDataItems();
      }

      param_query.Bind(0, name);
      param_query.Bind(1, key);

      while (param_query.Next()) {
        const auto param_with_diff_versions = GetVehicleDataItem(
            param_query.GetString(2), param_query.GetString(3));
        if (!param_with_diff_versions.is_initialized()) {
          return policy_table::VehicleDataItems();
        }
        for (const auto& param : param_with_diff_versions) {
          vdi.params->push_back(param);
        }
      }
    }

    result.push_back(vdi);
  }
  if (!result.empty()) {
    result.mark_initialized();
  }

  return result;
}

policy_table::VehicleDataItem SQLPTRepresentation::PopulateVDIFromQuery(
    const utils::dbms::SQLQuery& query) const {
  policy_table::VehicleDataItem result;

  result.name = query.GetString(0);
  result.type = query.GetString(1);
  result.key = query.GetString(2);
  result.mandatory = query.GetBoolean(3);

  if (!query.IsNull(4)) {
    *result.array = query.GetBoolean(4);
  }
  if (!query.IsNull(5)) {
    *result.since = query.GetString(5);
  }
  if (!query.IsNull(6)) {
    *result.until = query.GetString(6);
  }
  if (!query.IsNull(7)) {
    *result.removed = query.GetBoolean(7);
  }
  if (!query.IsNull(8)) {
    *result.deprecated = query.GetBoolean(8);
  }
  if (!query.IsNull(9)) {
    *result.defvalue = query.GetString(9);
  }
  if (!query.IsNull(10)) {
    *result.minvalue = query.GetInteger(10);
  }
  if (!query.IsNull(11)) {
    *result.maxvalue = query.GetInteger(11);
  }
  if (!query.IsNull(12)) {
    *result.minsize = query.GetUInteger(12);
  }
  if (!query.IsNull(13)) {
    *result.maxsize = query.GetUInteger(13);
  }
  if (!query.IsNull(14)) {
    *result.minlength = query.GetUInteger(14);
  }
  if (!query.IsNull(15)) {
    *result.maxlength = query.GetUInteger(15);
  }
  result.params->mark_initialized();

  result.mark_initialized();
  return result;
}

bool SQLPTRepresentation::InsertVehicleDataItem(
    const policy_table::VehicleDataItem& vehicle_data_item) {
  SDL_LOG_AUTO_TRACE();
  utils::dbms::SQLQuery query(db());

  if (!vehicle_data_item.is_initialized() || !vehicle_data_item.is_valid()) {
    SDL_LOG_ERROR("Vehicle data item is not initialized.");
    return false;
  }

  if (VehicleDataItemExists(vehicle_data_item)) {
    SDL_LOG_INFO(static_cast<std::string>(vehicle_data_item.key)
                 << " is already stored.");
    return true;
  }

  if (!query.Prepare(sql_pt::kInsertVehicleDataItem)) {
    SDL_LOG_ERROR("Incorrent select statement of vehicle data item. "
                  << query.LastError().text());
    return false;
  }

  query.Bind(0, vehicle_data_item.name);
  query.Bind(1, vehicle_data_item.type);
  query.Bind(2, vehicle_data_item.key);
  query.Bind(3, vehicle_data_item.mandatory);
  vehicle_data_item.array.is_initialized()
      ? query.Bind(4, *vehicle_data_item.array)
      : query.Bind(4);
  vehicle_data_item.since.is_initialized()
      ? query.Bind(5, *vehicle_data_item.since)
      : query.Bind(5);
  vehicle_data_item.until.is_initialized()
      ? query.Bind(6, *vehicle_data_item.until)
      : query.Bind(6);
  vehicle_data_item.removed.is_initialized()
      ? query.Bind(7, *vehicle_data_item.removed)
      : query.Bind(7);
  vehicle_data_item.deprecated.is_initialized()
      ? query.Bind(8, *vehicle_data_item.deprecated)
      : query.Bind(8);
  vehicle_data_item.defvalue.is_initialized()
      ? query.Bind(9, *vehicle_data_item.defvalue)
      : query.Bind(9);
  vehicle_data_item.minvalue.is_initialized()
      ? query.Bind(10, *vehicle_data_item.minvalue)
      : query.Bind(10);
  vehicle_data_item.maxvalue.is_initialized()
      ? query.Bind(11, *vehicle_data_item.maxvalue)
      : query.Bind(11);
  vehicle_data_item.minsize.is_initialized()
      ? query.Bind(12, static_cast<int64_t>(*vehicle_data_item.minsize))
      : query.Bind(12);
  vehicle_data_item.maxsize.is_initialized()
      ? query.Bind(13, static_cast<int64_t>(*vehicle_data_item.maxsize))
      : query.Bind(13);
  vehicle_data_item.minlength.is_initialized()
      ? query.Bind(14, static_cast<int64_t>(*vehicle_data_item.minlength))
      : query.Bind(14);
  vehicle_data_item.maxlength.is_initialized()
      ? query.Bind(15, static_cast<int64_t>(*vehicle_data_item.maxlength))
      : query.Bind(15);

  if (!query.Exec() || !query.Reset()) {
    SDL_LOG_ERROR("Failed to insert vehicle data item: "
                  << static_cast<std::string>(vehicle_data_item.key)
                  << ". Error: " << query.LastError().text());
    return false;
  }

  if (vehicle_data_item.params->is_initialized()) {
    std::map<std::string, std::string> stored_vehicle_data_item_params;
    for (const auto& param : *(vehicle_data_item.params)) {
      if (!InsertVehicleDataItem(param)) {
        return false;
      }

      if (stored_vehicle_data_item_params.end() !=
          stored_vehicle_data_item_params.find(param.name)) {
        SDL_LOG_DEBUG("Parameter already stored.");
        continue;
      };

      if (!query.Prepare(sql_pt::kInsertVehicleDataItemParams)) {
        SDL_LOG_ERROR("Incorrent select statement of vehicle data item. "
                      << query.LastError().text());
        return false;
      }

      query.Bind(0, vehicle_data_item.name);
      query.Bind(1, vehicle_data_item.key);
      query.Bind(2, param.name);
      query.Bind(3, param.key);

      if (!query.Exec() || !query.Reset()) {
        SDL_LOG_ERROR(
            "Failed to insert to vehicle data item relations helper table: "
            << static_cast<std::string>(param.key)
            << ". Error: " << query.LastError().text());
        return false;
      }
      stored_vehicle_data_item_params[param.name] = param.key;
    }
  }

  return true;
}

policy_table::VehicleDataItems
SQLPTRepresentation::SelectCompositeVehicleDataItems() const {
  utils::dbms::SQLQuery query(db());
  SDL_LOG_AUTO_TRACE();
  if (!query.Prepare(sql_pt::kSelectCompositeVehicleDataItemsKey)) {
    SDL_LOG_ERROR("Incorrect statement for parameterized vehicle data items");
    return policy_table::VehicleDataItems();
  }

  policy_table::VehicleDataItems result;
  result.mark_initialized();

  while (query.Next()) {
    const auto vdi = GetVehicleDataItem(query.GetString(0), query.GetString(1));
    if (!vdi.is_initialized()) {
      return policy_table::VehicleDataItems();
    }
    std::copy(vdi.begin(), vdi.end(), std::back_inserter(result));
  }

  return result;
}

policy_table::VehicleDataItems
SQLPTRepresentation::SelectPrimitiveVehicleDataItems() const {
  utils::dbms::SQLQuery query(db());
  SDL_LOG_AUTO_TRACE();
  if (!query.Prepare(sql_pt::kSelectPrimitiveVehicleDataItems)) {
    SDL_LOG_ERROR(
        "Incorrect statement for non parameterized vehicle data items");
    return policy_table::VehicleDataItems();
  }

  auto result = policy_table::VehicleDataItems();
  result.mark_initialized();

  while (query.Next()) {
    const auto vdi = PopulateVDIFromQuery(query);
    if (!vdi.is_initialized()) {
      return policy_table::VehicleDataItems();
    }
    result.push_back(vdi);
  }
  return result;
}

bool SQLPTRepresentation::DeleteVehicleDataItems() const {
  utils::dbms::SQLQuery query(db());
  SDL_LOG_AUTO_TRACE();

  if (!query.Exec(sql_pt::kDeleteVehicleDataItems)) {
    SDL_LOG_ERROR("Failed clearing database: " << query.LastError().text());
    return false;
  }

  if (!query.Exec(sql_pt::kDeleteVehicleDataItemParams)) {
    SDL_LOG_ERROR("Failed clearing database: " << query.LastError().text());
    return false;
  }

  return true;
}

}  // namespace policy

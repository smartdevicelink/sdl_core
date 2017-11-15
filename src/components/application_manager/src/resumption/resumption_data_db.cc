/*
 * Copyright (c) 2015, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */
#include <string>
#include <unistd.h>

#include "application_manager/application_manager_impl.h"
#include "application_manager/resumption/resumption_data_db.h"
#include "application_manager/resumption/resumption_sql_queries.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/message_helper.h"
#include "utils/helpers.h"
#include "utils/gen_hash.h"
#include "utils/scope_guard.h"
#include "application_manager/application_manager_settings.h"

namespace {
const std::string kDatabaseName = "resumption";
}

namespace resumption {
CREATE_LOGGERPTR_GLOBAL(logger_, "Resumption")

ResumptionDataDB::ResumptionDataDB(
    DbStorage db_storage,
    const application_manager::ApplicationManager& application_manager)
    : ResumptionData(application_manager) {
  if (db_storage == In_File_Storage) {
    db_ = new utils::dbms::SQLDatabase(kDatabaseName);
#ifndef __QNX__
    std::string path = application_manager_.get_settings().app_storage_folder();
    if (!path.empty()) {
      db_->set_path(path + "/");
    }
  } else if (db_storage == In_Memory_Storage) {
    db_ = new utils::dbms::SQLDatabase();
#endif  // __QNX__
  } else {
    LOG4CXX_AUTO_TRACE(logger_);
    LOG4CXX_ERROR(logger_, "Get not existed type of database storage");
  }
}

ResumptionDataDB::~ResumptionDataDB() {
  db_->Close();
  delete db_;
}

bool ResumptionDataDB::Init() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!db_->Open()) {
    LOG4CXX_ERROR(logger_, "Failed opening database.");
    LOG4CXX_INFO(logger_, "Starting opening retries.");
    const uint16_t attempts =
        application_manager_.get_settings().attempts_to_open_resumption_db();
    LOG4CXX_DEBUG(logger_, "Total attempts number is: " << attempts);
    bool is_opened = false;
    const uint16_t open_attempt_timeout_ms =
        application_manager_.get_settings()
            .open_attempt_timeout_ms_resumption_db();
    const useconds_t sleep_interval_mcsec = open_attempt_timeout_ms * 1000;
    LOG4CXX_DEBUG(logger_,
                  "Open attempt timeout(ms) is: " << open_attempt_timeout_ms);
    for (int i = 0; i < attempts; ++i) {
      usleep(sleep_interval_mcsec);
      LOG4CXX_INFO(logger_, "Attempt: " << i + 1);
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
      return false;
    }
  }
#ifndef __QNX__
  if (!db_->IsReadWrite()) {
    LOG4CXX_ERROR(logger_, "There are no read/write permissions for database");
    return false;
  }
#endif  // __QNX__
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(kCreateSchema)) {
    LOG4CXX_ERROR(
        logger_,
        "Failed creating schema of database: " << query.LastError().text());
    return false;
  }
  utils::dbms::SQLQuery query_checks_resumption(db());
  if (!query_checks_resumption.Prepare(kChecksResumptionData) ||
      !query_checks_resumption.Exec()) {
    LOG4CXX_ERROR(logger_,
                  "Failed verification or execution query kChecksResumptionData"
                      << query_checks_resumption.LastError().text());
    return false;
  }
  if (0 == query_checks_resumption.GetInteger(0)) {
    utils::dbms::SQLQuery query_insert_resumption(db());
    if (!query_insert_resumption.Prepare(kInsertInitData) ||
        !query_insert_resumption.Exec()) {
      LOG4CXX_ERROR(logger_,
                    "Failed insert init data to database: "
                        << query_insert_resumption.LastError().text());
      return false;
    }
  }
  return true;
}

void ResumptionDataDB::SaveApplication(
    app_mngr::ApplicationSharedPtr application) {
  using namespace app_mngr;
  using namespace mobile_api;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(application);
  bool application_exist = false;
  const std::string& policy_app_id = application->policy_app_id();
  const std::string& device_mac = application->mac_address();
  LOG4CXX_INFO(logger_,
               "app_id : " << application->app_id() << " policy_app_id : "
                           << policy_app_id << " device_id : " << device_mac);

  if (!CheckExistenceApplication(
          policy_app_id, device_mac, application_exist)) {
    LOG4CXX_ERROR(logger_, "Problem with access to DB");
    return;
  }

  if (application->is_application_data_changed()) {
    if (application_exist) {
      if (!DeleteSavedApplication(policy_app_id, device_mac)) {
        LOG4CXX_ERROR(logger_, "Deleting of application data is not finished");
        return;
      }
    }

    if (!SaveApplicationToDB(application, policy_app_id, device_mac)) {
      LOG4CXX_ERROR(logger_, "Saving of application data is not finished");
      return;
    }
    LOG4CXX_INFO(logger_, "All data from application were saved successfully");
    application->set_is_application_data_changed(false);
  } else {
    if (application_exist) {
      if (!UpdateApplicationData(application, policy_app_id, device_mac)) {
        LOG4CXX_ERROR(logger_, "Updating application data is failed");
        return;
      }
      LOG4CXX_INFO(logger_, "Application data were updated successfully");
    } else {
      if (Compare<HMILevel::eType, EQ, ONE>(application->hmi_level(),
                                            HMILevel::HMI_FULL,
                                            HMILevel::HMI_LIMITED)) {
        if (!InsertApplicationData(application, policy_app_id, device_mac)) {
          LOG4CXX_ERROR(logger_, "Saving data of application is failed");
          return;
        }
      }
    }
  }
  WriteDb();
}

bool ResumptionDataDB::IsHMIApplicationIdExist(uint32_t hmi_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  return CheckExistenceHMIId(hmi_app_id);
}

uint32_t ResumptionDataDB::GetHMIApplicationID(
    const std::string& policy_app_id, const std::string& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t hmi_app_id = 0;
  SelectHMIId(policy_app_id, device_id, hmi_app_id);
  return hmi_app_id;
}

void ResumptionDataDB::OnSuspend() {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query_update_suspend_data(db());
  utils::dbms::SQLQuery query_update_last_ign_off_time(db());
  /*
    application_lifes - contains amount of ignition cycles during which
    db stores data of application
   */
  const int application_lifes = 3;

  if (DeleteAppWithIgnCount(application_lifes)) {
    LOG4CXX_INFO(logger_,
                 "Saved application with ign_off_count = " << application_lifes
                                                           << " was deleted");
  } else {
    LOG4CXX_WARN(logger_, "Problem with removing applications");
  }

  if (query_update_suspend_data.Prepare(kUpdateSuspendData)) {
    if (query_update_suspend_data.Exec()) {
      LOG4CXX_INFO(logger_,
                   "Data ign_off_count and suspend_count were updated");
    }
  }

  if (query_update_last_ign_off_time.Prepare(KUpdateLastIgnOffTime)) {
    query_update_last_ign_off_time.Bind(0, static_cast<int64_t>(time(NULL)));
    if (query_update_last_ign_off_time.Exec()) {
      LOG4CXX_INFO(logger_, "Data last_ign_off_time was updated");
    }
  }
}

bool ResumptionDataDB::DeleteAppWithIgnCount(const int application_lifes) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery select_apps_for_removing(db());
  utils::dbms::SQLQuery count_app(db());

  if (!select_apps_for_removing.Prepare(kSelectApplicationsIgnOffCount) ||
      !count_app.Prepare(kCountApplicationsIgnOff)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification query select_apps_for_removing or"
                 " query count_app");
    return false;
  }
  /* Positions of binding data for "query count_app" :
    field "ign_off_count" from table "application" = 0*/
  count_app.Bind(0, application_lifes);
  if (!count_app.Exec() || !count_app.GetInteger(0)) {
    LOG4CXX_WARN(logger_, "Problem with execution or count app=0");
    return false;
  }
  std::string policy_app_id;
  std::string device_id;
  /* Positions of binding data for "select_apps_for_removing" :
     field "ign_off_count" from table "application" = 0*/
  select_apps_for_removing.Bind(0, application_lifes);
  while (select_apps_for_removing.Next()) {
    device_id = select_apps_for_removing.GetString(0);
    policy_app_id = select_apps_for_removing.GetString(1);
    if (!DeleteSavedApplication(policy_app_id, device_id)) {
      LOG4CXX_WARN(logger_, "Problem with removing application data");
      return false;
    }
  }
  LOG4CXX_WARN(logger_, "Applications data were removed successfully");
  WriteDb();
  return true;
}

bool ResumptionDataDB::GetHashId(const std::string& policy_app_id,
                                 const std::string& device_id,
                                 std::string& hash_id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  return SelectHashId(policy_app_id, device_id, hash_id);
}

void ResumptionDataDB::OnAwake() {
  LOG4CXX_AUTO_TRACE(logger_);

  UpdateDataOnAwake();
}

bool ResumptionDataDB::GetSavedApplication(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  bool application_exist = false;

  if (!CheckExistenceApplication(policy_app_id, device_id, application_exist) ||
      !application_exist) {
    LOG4CXX_ERROR(logger_,
                  "Problem with access to DB or application does not exists");
    return false;
  }

  if (!SelectDataFromAppTable(policy_app_id, device_id, saved_app)) {
    LOG4CXX_ERROR(logger_,
                  "Problem with restoring of data from application table");
    return false;
  }

  if (!SelectFilesData(policy_app_id, device_id, saved_app)) {
    LOG4CXX_ERROR(logger_, "Problem with restoring of files data");
    return false;
  }

  if (!SelectSubmenuData(policy_app_id, device_id, saved_app)) {
    LOG4CXX_ERROR(logger_, "Problem with restoring of submenu data");
    return false;
  }

  if (!SelectCommandData(policy_app_id, device_id, saved_app)) {
    LOG4CXX_ERROR(logger_, "Problem with restoring of command data");
    return false;
  }

  if (!SelectSubscriptionsData(policy_app_id, device_id, saved_app)) {
    LOG4CXX_ERROR(logger_, "Problem with restoring of subscriptions data");
    return false;
  }

  if (!SelectChoiceSetData(policy_app_id, device_id, saved_app)) {
    LOG4CXX_ERROR(logger_, "Problem with restoring of choice set data");
    return false;
  }

  if (!SelectGlobalPropertiesData(policy_app_id, device_id, saved_app)) {
    LOG4CXX_ERROR(logger_, "Problem with restoring of global properties data");
    return false;
  }
  LOG4CXX_INFO(logger_,
               "Application data were successfully fetched from data base");
  return true;
}

bool ResumptionDataDB::RemoveApplicationFromSaved(
    const std::string& policy_app_id, const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool application_exist = false;
  if (!CheckExistenceApplication(policy_app_id, device_id, application_exist) ||
      !application_exist) {
    LOG4CXX_ERROR(logger_,
                  "Problem with access to DB or application does not"
                  " exist");
    return false;
  }
  bool result = false;
  if (DeleteSavedApplication(policy_app_id, device_id)) {
    WriteDb();
    result = true;
  }
  return result;
}

uint32_t ResumptionDataDB::GetIgnOffTime() const {
  LOG4CXX_AUTO_TRACE(logger_);

  return SelectIgnOffTime();
}

ssize_t ResumptionDataDB::IsApplicationSaved(
    const std::string& policy_app_id, const std::string& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  bool application_exist = false;
  if (CheckExistenceApplication(policy_app_id, device_id, application_exist) &&
      application_exist) {
    LOG4CXX_INFO(logger_, "Application exists in stored data");
    return 0;
  }
  return -1;
}

void ResumptionDataDB::GetDataForLoadResumeData(
    smart_objects::SmartObject& saved_data) const {
  LOG4CXX_AUTO_TRACE(logger_);
  SelectDataForLoadResumeData(saved_data);
}

bool ResumptionDataDB::SelectHMILevel(const std::string& policy_app_id,
                                      const std::string& device_id,
                                      int& hmi_level) const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query_count(db());
  utils::dbms::SQLQuery query_select(db());
  if (query_count.Prepare(kSelectCountHMILevel) &&
      query_select.Prepare(kSelectHMILevel)) {
    /* Positions of binding data for "query_count" and "query_select" :
       field "deviceID" from table "application" = 0
       field "appID" from table "application" = 1 */
    query_count.Bind(0, device_id);
    query_count.Bind(1, policy_app_id);
    query_select.Bind(0, device_id);
    query_select.Bind(1, policy_app_id);
    /* Position of data in "query_select" :
       field "hmiLevel" from table "application" = 0 */
    if (query_count.Exec() && query_count.GetInteger(0) &&
        query_select.Exec()) {
      hmi_level = query_select.GetInteger(0);
      return true;
    }
  }
  return false;
}

bool ResumptionDataDB::CheckExistenceHMIId(uint32_t hmi_app_id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kCheckHMIId)) {
    query.Bind(0, static_cast<int64_t>(hmi_app_id));
    if (query.Exec() && (query.GetInteger(0))) {
      LOG4CXX_INFO(logger_, "Saved data has HMI appID = " << hmi_app_id);
      return true;
    }
  }
  LOG4CXX_FATAL(logger_,
                "HMI appID = " << hmi_app_id << " doesn't exist in saved data");
  return false;
}

void ResumptionDataDB::SelectHMIId(const std::string& policy_app_id,
                                   const std::string& device_id,
                                   uint32_t& hmi_id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query_select(db());
  utils::dbms::SQLQuery query_check(db());
  /* Positions of binding data for "query_select" and "query_check" :
     field "deviceID" from table "application" = 0
     field "appID" from table "application" = 1 */
  if (query_select.Prepare(kSelectHMIId) &&
      query_check.Prepare(kSelectCountHMIId)) {
    query_select.Bind(0, device_id);
    query_select.Bind(1, policy_app_id);
    query_check.Bind(0, device_id);
    query_check.Bind(1, policy_app_id);
    /* Position of data in "query_select" :
       field "hmiAppID" from table "application" = 0 */
    if (query_check.Exec() && query_check.GetInteger(0) &&
        query_select.Exec()) {
      hmi_id = query_select.GetUInteger(0);
      LOG4CXX_INFO(logger_, "HMI appID = " << hmi_id);
      return;
    }
  }
  LOG4CXX_FATAL(logger_,
                "Saved data doesn't have application with "
                "device id = "
                    << device_id << " and policy appID = " << policy_app_id);
}

bool ResumptionDataDB::SelectHashId(const std::string& policy_app_id,
                                    const std::string& device_id,
                                    std::string& hash_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery count(db());
  utils::dbms::SQLQuery select_hash(db());
  if (!select_hash.Prepare(kSelectHashId) || !count.Prepare(kCountHashId)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification count query or"
                 " select_hash query");
    return false;
  }
  /* Positions of binding data for "count" and "select_hash" :
     field "deviceID" from table "application" = 0
     field "appID" from table "application" = 1 */
  count.Bind(0, device_id);
  count.Bind(1, policy_app_id);
  select_hash.Bind(0, device_id);
  select_hash.Bind(1, policy_app_id);
  /* Position of data in "select_hash" :
     field "hashID" from table "application" = 0 */
  if (count.Exec() && count.GetInteger(0) && select_hash.Exec()) {
    hash_id = select_hash.GetString(0);
    LOG4CXX_INFO(logger_, "Saved hash ID = " << hash_id);
    return true;
  }
  LOG4CXX_WARN(logger_,
               "Saved data doesn't have application with "
               "device id = "
                   << device_id << " and policy appID = " << policy_app_id
                   << "or hashID");
  return false;
}

uint32_t ResumptionDataDB::SelectIgnOffTime() const {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t ignOffTime = 0;
  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kSelectIgnOffTime)) {
    if (query.Exec()) {
      ignOffTime = query.GetUInteger(0);
      LOG4CXX_INFO(logger_, "Last ign off time = " << ignOffTime);
      return ignOffTime;
    }
  }
  LOG4CXX_ERROR(logger_, "Problem with prepare query");
  return ignOffTime;
}

bool ResumptionDataDB::CheckExistenceApplication(
    const std::string& policy_app_id,
    const std::string& device_id,
    bool& application_exist) const {
  LOG4CXX_AUTO_TRACE(logger_);
  bool result = false;
  utils::dbms::SQLQuery query(db());
  /* Positions of binding data for "query":
     field "deviceID" from table "application" = 0
     field "appID" from table "application" = 1 */
  if (query.Prepare(kCheckApplication)) {
    query.Bind(0, device_id);
    query.Bind(1, policy_app_id);
    result = query.Exec();
  }
  /* Position of data in "query" :
     amount of application = 0 */
  if (result && query.GetInteger(0)) {
    LOG4CXX_INFO(logger_,
                 "Saved data has application with policy appID = "
                     << policy_app_id << " and deviceID = " << device_id);
    application_exist = true;
  } else if (result) {
    LOG4CXX_INFO(logger_, "Saved data does not contain application");
    application_exist = false;
  } else {
    LOG4CXX_ERROR(logger_, "Problem with access DB");
  }
  return result;
}

void ResumptionDataDB::SelectDataForLoadResumeData(
    smart_objects::SmartObject& saved_data) const {
  using namespace app_mngr;
  using namespace smart_objects;
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery select_data(db());
  utils::dbms::SQLQuery count_application(db());
  if (!select_data.Prepare(kSelectDataForLoadResumeData) ||
      !count_application.Prepare(kCountApplications)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification select_data query"
                 " or count application");
    return;
  }

  if (!count_application.Exec() || !count_application.GetInteger(0)) {
    LOG4CXX_WARN(logger_,
                 "Problem with execution count_application query"
                 " or appliction table does not contain data");
    return;
  }
  SmartObject so_array_data(SmartType_Array);
  uint32_t i = 0;
  /* Position of data in "select_data" :
     field "hmiLevel" from table "application" = 0
     field "ign_off_count" from table "application" = 1
     field "timeStamp" from table "application" = 2
     field "appID" from table "application" = 3
     field "deviceID" from table "application" = 4 */
  while (select_data.Next()) {
    SmartObject so_obj(SmartType_Map);
    so_obj[strings::hmi_level] = select_data.GetInteger(0);
    so_obj[strings::ign_off_count] = select_data.GetInteger(1);
    so_obj[strings::time_stamp] = select_data.GetUInteger(2);
    so_obj[strings::app_id] = select_data.GetString(3);
    so_obj[strings::device_id] = select_data.GetString(4);
    so_array_data[i++] = so_obj;
  }
  saved_data = so_array_data;
}

void ResumptionDataDB::UpdateHmiLevel(const std::string& policy_app_id,
                                      const std::string& device_id,
                                      mobile_apis::HMILevel::eType hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  /* Positions of binding data for "query":
     field "hmiLevel" from table "application" = 0
     field "deviceID" from table "application" = 1
     field "appID" from table "application" = 2 */
  if (query.Prepare(kUpdateHMILevel)) {
    query.Bind(0, hmi_level);
    query.Bind(1, device_id);
    query.Bind(2, policy_app_id);
    if (query.Exec()) {
      LOG4CXX_INFO(logger_,
                   "Saved data has application with policy appID = "
                       << policy_app_id << " and deviceID = " << device_id
                       << " has new HMI level = " << hmi_level);
      WriteDb();
    }
  }
}

void ResumptionDataDB::Persist() {
  WriteDb();
}

bool ResumptionDataDB::RefreshDB() const {
  utils::dbms::SQLQuery query(db());
  if (!query.Exec(resumption::kDropSchema)) {
    LOG4CXX_WARN(logger_,
                 "Failed dropping database: " << query.LastError().text());
    return false;
  }
  if (!query.Exec(resumption::kCreateSchema)) {
    LOG4CXX_ERROR(
        logger_,
        "Failed creating schema of database: " << query.LastError().text());
    return false;
  }
  if (!query.Exec(resumption::kInsertInitData)) {
    LOG4CXX_ERROR(
        logger_,
        "Failed insert init data to database: " << query.LastError().text());
    return false;
  }
  return true;
}

bool ResumptionDataDB::GetAllData(smart_objects::SmartObject& data) const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(resumption::kSelectAllApps)) {
    LOG4CXX_ERROR(logger_, "Can't get applications data from DB.");
    return false;
  }

  data = smart_objects::SmartObject(smart_objects::SmartType_Array);

  uint32_t index = 0;
  std::string app_id;
  std::string device_id;
  while (query.Next()) {
    app_id = query.GetString(0);
    device_id = query.GetString(1);
    if (GetSavedApplication(app_id, device_id, data[index])) {
      ++index;
    }
  }
  return true;
}

bool ResumptionDataDB::SaveAllData(const smart_objects::SmartObject& data) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (smart_objects::SmartType_Array != data.getType()) {
    LOG4CXX_ERROR(logger_, "Unexpected type for resumption data.");
    return false;
  }
  const smart_objects::SmartArray* apps = data.asArray();
  smart_objects::SmartArray::const_iterator it_apps = apps->begin();
  for (; apps->end() != it_apps; ++it_apps) {
    if (!SaveApplicationToDB((*it_apps),
                             (*it_apps)["appID"].asString(),
                             (*it_apps)["deviceID"].asString())) {
      return false;
    }
  }
  return true;
}

bool ResumptionDataDB::IsDBVersionActual() const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(resumption::kSelectDBVersion) || !query.Exec()) {
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

bool ResumptionDataDB::UpdateDBVersion() const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(resumption::kUpdateDBVersion)) {
    LOG4CXX_ERROR(
        logger_,
        "Incorrect DB version update query: " << query.LastError().text());
    return false;
  }

  query.Bind(0, GetDBVersion());

  if (!query.Exec()) {
    LOG4CXX_ERROR(logger_,
                  "DB version update failed: " << query.LastError().text());
    return false;
  }

  return true;
}

bool ResumptionDataDB::DropAppDataResumption(const std::string& device_id,
                                             const std::string& app_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::ScopeGuard guard =
      utils::MakeObjGuard(*db_, &utils::dbms::SQLDatabase::RollbackTransaction);

  db_->BeginTransaction();
  if (!DeleteSavedFiles(app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedSubMenu(app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedSubscriptions(app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedCommands(app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedChoiceSet(app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedGlobalProperties(app_id, device_id)) {
    return false;
  }
  if (!UpdateGrammarID(app_id, device_id, 0)) {
    return false;
  }
  db_->CommitTransaction();

  guard.Dismiss();
  return true;
}

const int32_t ResumptionDataDB::GetDBVersion() const {
  return utils::Djb2HashFromString(resumption::kCreateSchema);
}

bool ResumptionDataDB::SelectFilesData(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(
          count_item, kSelectCountFiles, policy_app_id, device_id)) {
    return false;
  }

  saved_app[strings::application_files] = SmartObject(SmartType_Array);

  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain files data");
    return true;
  }
  utils::dbms::SQLQuery select_files(db());
  if (!PrepareSelectQuery(
          select_files, policy_app_id, device_id, kSelectFiles)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_files");
    return false;
  }
  saved_app[strings::application_files] = SmartObject(SmartType_Array);
  /* Position of data in "select_files" :
     field "fileType" from table "file" = 0
     field "is_download_complete" from table "file" = 1
     field "persistentFile" from table "file" = 2
     field "syncFileName" from table "file" = 3*/
  uint32_t i = 0;
  while (select_files.Next()) {
    SmartObject array_item(SmartType_Map);
    array_item[strings::file_type] = select_files.GetInteger(0);
    array_item[strings::is_download_complete] = select_files.GetBoolean(1);
    array_item[strings::persistent_file] = select_files.GetBoolean(2);
    array_item[strings::sync_file_name] = select_files.GetString(3);
    saved_app[strings::application_files][i++] = array_item;
  }
  LOG4CXX_INFO(logger_, "File data was restored successfully");
  return true;
}

bool ResumptionDataDB::SelectSubmenuData(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(
          count_item, kSelectCountSubMenu, policy_app_id, device_id)) {
    return false;
  }

  saved_app[strings::application_submenus] = SmartObject(SmartType_Array);

  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain submenu data");
    return true;
  }
  utils::dbms::SQLQuery select_sub_menu(db());
  if (!PrepareSelectQuery(
          select_sub_menu, policy_app_id, device_id, kSelectSubMenu)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_sub_menu");
    return false;
  }
  saved_app[strings::application_submenus] = SmartObject(SmartType_Array);
  /* Position of data in "select_sub_menu" :
     field "menuID" from table "subMenu" = 0
     field "menuName" from table "subMenu" = 1
     field "position" from table "subMenu" = 2*/
  uint32_t i = 0;
  while (select_sub_menu.Next()) {
    SmartObject array_item(SmartType_Map);
    array_item[strings::menu_id] = select_sub_menu.GetInteger(0);
    array_item[strings::menu_name] = select_sub_menu.GetString(1);
    if (!(select_sub_menu.IsNull(2))) {
      array_item[strings::position] = select_sub_menu.GetInteger(2);
    }
    saved_app[strings::application_submenus][i++] = array_item;
  }
  LOG4CXX_INFO(logger_, "Sub menu data was restored successfully");
  return true;
}

bool ResumptionDataDB::SelectCommandData(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(
          count_item, kSelectCountCommands, policy_app_id, device_id)) {
    return false;
  }

  saved_app[strings::application_commands] = SmartObject(SmartType_Array);

  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain commands data");
    return true;
  }

  utils::dbms::SQLQuery select_commands(db());
  if (!PrepareSelectQuery(
          select_commands, policy_app_id, device_id, kSelectCommands)) {
    return false;
  }
  int64_t command_key = 0;
  int32_t command_idx = -1;
  size_t vr_cmd_idx = 0;
  bool vr_command_exist = false;
  /* Position of data in "select_commands" :
     field "idcommand" from table "command" = 0
     field "cmdID" from table "command" = 1
     field "menuName" from table "command" = 2
     field "parentID" from table "command" = 3
     field "position" from table "command" = 4
     field "value" from table "image" = 5
     field "imageType" from table "image" = 6
     field "vrCommand" from table "vrCommandsArray" = 7*/
  while (select_commands.Next()) {
    if (command_key != select_commands.GetLongInt(0)) {
      ++command_idx;
      saved_app[strings::application_commands][command_idx] =
          SmartObject(SmartType_Map);
      SmartObject& so_item =
          saved_app[strings::application_commands][command_idx];
      so_item[strings::cmd_id] = select_commands.GetInteger(1);
      SmartObject menu_params(SmartType_Map);
      SmartObject cmd_icon(SmartType_Map);
      if (!(select_commands.IsNull(2))) {
        menu_params[strings::menu_name] = select_commands.GetString(2);
      }
      if (!(select_commands.IsNull(3))) {
        menu_params[hmi_request::parent_id] = select_commands.GetInteger(3);
      }
      if (!(select_commands.IsNull(4))) {
        menu_params[strings::position] = select_commands.GetInteger(4);
      }
      if (!menu_params.empty()) {
        so_item[strings::menu_params] = menu_params;
      }
      if (!(select_commands.IsNull(5))) {
        cmd_icon[strings::value] = select_commands.GetString(5);
      }
      if (!(select_commands.IsNull(6))) {
        cmd_icon[strings::image_type] = select_commands.GetInteger(6);
      }
      if (!cmd_icon.empty()) {
        so_item[strings::cmd_icon] = cmd_icon;
      }
      if (!(select_commands.IsNull(7))) {
        vr_command_exist = true;
        so_item[strings::vr_commands] = SmartObject(SmartType_Array);
      } else {
        vr_command_exist = false;
      }
      vr_cmd_idx = 0;
      command_key = select_commands.GetLongInt(0);
    }
    if (vr_command_exist) {
      saved_app[strings::application_commands][command_idx]
               [strings::vr_commands][vr_cmd_idx++] =
                   select_commands.GetString(7);
    }
  }
  LOG4CXX_INFO(logger_, "Commands were restored from DB successfully");
  return true;
}

bool ResumptionDataDB::SelectSubscriptionsData(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(
          count_item, kSelectCountSubscriptions, policy_app_id, device_id)) {
    return false;
  }

  saved_app[strings::application_subscribtions] = SmartObject(SmartType_Map);

  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain subscriptions data");
    return true;
  }
  utils::dbms::SQLQuery select_subscriptions(db());
  if (!PrepareSelectQuery(select_subscriptions,
                          policy_app_id,
                          device_id,
                          kSelectSubscriptions)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_subscriptions");
    return false;
  }
  SmartObject application_buttons(SmartType_Array);
  SmartObject application_vehicle_info(SmartType_Array);
  size_t buttons_idx = 0;
  size_t vi_idx = 0;
  /* Position of data in "select_subscriptions" :
     field "vehicleValue" from table "applicationSubscribtionsArray" = 0
     field "ButtonNameValue" from table "applicationSubscribtionsArray" = 1*/
  while (select_subscriptions.Next()) {
    if (!select_subscriptions.IsNull(0)) {
      application_vehicle_info[vi_idx++] = select_subscriptions.GetInteger(0);
    }
    if (!select_subscriptions.IsNull(1)) {
      application_buttons[buttons_idx++] = select_subscriptions.GetInteger(1);
    }
  }
  if (!application_buttons.empty()) {
    saved_app[strings::application_subscribtions]
             [strings::application_buttons] = application_buttons;
  }

  if (!application_vehicle_info.empty()) {
    saved_app[strings::application_subscribtions]
             [strings::application_vehicle_info] = application_vehicle_info;
  }
  LOG4CXX_INFO(logger_, "Subscriptions were restored from DB successfully");
  return true;
}

bool ResumptionDataDB::SelectChoiceSetData(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(
          count_item, kSelectCountChoiceSet, policy_app_id, device_id)) {
    return false;
  }

  saved_app[strings::application_choice_sets] = SmartObject(SmartType_Array);

  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain choice set data");
    return true;
  }
  utils::dbms::SQLQuery select_choice_set(db());
  if (!PrepareSelectQuery(
          select_choice_set, policy_app_id, device_id, kSelectChoiceSets)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_choice_set");
    return false;
  }

  int64_t application_choice_set_key = 0;
  int64_t choice_key = 0;
  int32_t choice_set_idx = -1;
  int32_t choice_idx = -1;
  size_t vr_cmd_idx = 0;
  /* Position of data in "select_choice_set" :
     field "idapplicationChoiceSet" from table "applicationChoiceSet" = 0
     field "grammarID" from table "applicationChoiceSet" = 1
     field "interactionChoiceSetID" from table "applicationChoiceSet" = 2
     field "idchoice" from table "choice" = 3
     field "choiceID" from table "choice" = 4
     field "menuName" from table "choice" = 5
     field "secondaryText" from table "choice" = 6
     field "tertiaryText" from table "choice" = 7
     field "idimage" from table "choice" = 8
     field "idsecondaryImage" from table "choice" = 9
     field "vrCommand" from table "vrCommandsArray" = 10*/
  while (select_choice_set.Next()) {
    if (application_choice_set_key != select_choice_set.GetLongInt(0)) {
      ++choice_set_idx;
      saved_app[strings::application_choice_sets][choice_set_idx] =
          SmartObject(SmartType_Map);
      SmartObject& choice_set_item =
          saved_app[strings::application_choice_sets][choice_set_idx];
      choice_set_item[strings::grammar_id] = select_choice_set.GetInteger(1);
      choice_set_item[strings::interaction_choice_set_id] =
          select_choice_set.GetInteger(2);
      saved_app[strings::application_choice_sets][choice_set_idx]
               [strings::choice_set] = SmartObject(SmartType_Array);
      application_choice_set_key = select_choice_set.GetLongInt(0);
      choice_idx = -1;
    }
    if (choice_key != select_choice_set.GetLongInt(3)) {
      ++choice_idx;
      choice_key = select_choice_set.GetLongInt(3);

      saved_app[strings::application_choice_sets][choice_set_idx]
               [strings::choice_set][choice_idx] = SmartObject(SmartType_Map);
      SmartObject& choice_item =
          saved_app[strings::application_choice_sets][choice_set_idx]
                   [strings::choice_set][choice_idx];
      choice_item[strings::choice_id] = select_choice_set.GetUInteger(4);
      choice_item[strings::menu_name] = select_choice_set.GetString(5);
      if (!(select_choice_set.IsNull(6))) {
        choice_item[strings::secondary_text] = select_choice_set.GetString(6);
      }
      if (!(select_choice_set.IsNull(7))) {
        choice_item[strings::tertiary_text] = select_choice_set.GetString(7);
      }
      if (!(select_choice_set.IsNull(8))) {
        SmartObject image(SmartType_Map);
        if (!SelectImageData(select_choice_set.GetLongInt(8), image)) {
          return false;
        }
        choice_item[strings::image] = image;
      }
      if (!(select_choice_set.IsNull(9))) {
        SmartObject secondary_image(SmartType_Map);
        if (!SelectImageData(select_choice_set.GetLongInt(9),
                             secondary_image)) {
          return false;
        }
        choice_item[strings::secondary_image] = secondary_image;
      }
      choice_item[strings::vr_commands] = SmartObject(SmartType_Array);
      vr_cmd_idx = 0;
    }
    saved_app[strings::application_choice_sets][choice_set_idx]
             [strings::choice_set][choice_idx][strings::vr_commands]
             [vr_cmd_idx++] = select_choice_set.GetString(10);
  }

  LOG4CXX_INFO(logger_, "Choice sets were restored from DB successfully");
  return true;
}

bool ResumptionDataDB::SelectGlobalPropertiesData(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(
          count_item, kSelectCountGlobalProperties, policy_app_id, device_id)) {
    return false;
  }

  saved_app[strings::application_global_properties] =
      SmartObject(SmartType_Map);

  if (0 == count_item) {
    LOG4CXX_INFO(logger_,
                 "Application does not contain global properties data");
    return true;
  }
  utils::dbms::SQLQuery select_globalproperties(db());
  if (!PrepareSelectQuery(select_globalproperties,
                          policy_app_id,
                          device_id,
                          kSelectGlobalProperties)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_globalproperties");
    return false;
  }
  saved_app[strings::application_global_properties] =
      SmartObject(SmartType_Map);
  SmartObject& global_properties =
      saved_app[strings::application_global_properties];
  SmartObject keyboard_properties(SmartType_Map);
  SmartObject help_prompt(SmartType_Array);
  SmartObject timeout_prompt(SmartType_Array);
  size_t help_prompt_idx = 0;
  size_t timeout_prompt_idx = 0;
  int64_t global_properties_key = 0;
  /* Position of data in "select_globalproperties" :
     field "idglobalProperties" from table "globalProperties" = 0
     field "vrHelpTitle" from table "globalProperties" = 1
     field "menuTitle" from table "globalProperties" = 2
     field "idmenuIcon" from table "globalProperties" = 3
     field "language" from table "globalProperties" = 4
     field "keyboardLayout" from table "globalProperties" = 5
     field "keypressMode" from table "globalProperties" = 6
     field "autoCompleteText" from table "globalProperties" = 7
     field "idhelpPrompt" from table "helpTimeoutPromptArray" = 8
     field "idtimeoutPrompt" from table "helpTimeoutPromptArray" = 9*/
  while (select_globalproperties.Next()) {
    if (global_properties_key != select_globalproperties.GetLongInt(0)) {
      global_properties_key = select_globalproperties.GetLongInt(0);
      if (!select_globalproperties.IsNull(1)) {
        global_properties[strings::vr_help_title] =
            select_globalproperties.GetString(1);
      }
      if (!select_globalproperties.IsNull(2)) {
        global_properties[strings::menu_title] =
            select_globalproperties.GetString(2);
      }
      if (!select_globalproperties.IsNull(3)) {
        SmartObject image(SmartType_Map);
        if (!SelectImageData(select_globalproperties.GetLongInt(3), image)) {
          return false;
        }
        global_properties[strings::menu_icon] = image;
      }
      if (!select_globalproperties.IsNull(4)) {
        keyboard_properties[strings::language] =
            select_globalproperties.GetInteger(4);
      }
      if (!select_globalproperties.IsNull(5)) {
        keyboard_properties[hmi_request::keyboard_layout] =
            select_globalproperties.GetInteger(5);
      }
      if (!select_globalproperties.IsNull(6)) {
        keyboard_properties[strings::key_press_mode] =
            select_globalproperties.GetInteger(6);
      }
      if (!select_globalproperties.IsNull(7)) {
        keyboard_properties[strings::auto_complete_text] =
            select_globalproperties.GetString(7);
      }
    }
    if (!select_globalproperties.IsNull(8)) {
      SmartObject tts_chunk(SmartType_Map);
      if (!SelectTTSChunkData(select_globalproperties.GetLongInt(8),
                              tts_chunk)) {
        return false;
      }
      help_prompt[help_prompt_idx++] = tts_chunk;
    }
    if (!select_globalproperties.IsNull(9)) {
      SmartObject tts_chunk(SmartType_Map);
      if (!SelectTTSChunkData(select_globalproperties.GetLongInt(9),
                              tts_chunk)) {
        return false;
      }
      timeout_prompt[timeout_prompt_idx++] = tts_chunk;
    }
  }
  if (help_prompt_idx != 0) {
    global_properties[strings::help_prompt] = help_prompt;
  }
  if (timeout_prompt_idx != 0) {
    global_properties[strings::timeout_prompt] = timeout_prompt;
  }
  if (!keyboard_properties.empty()) {
    global_properties[strings::keyboard_properties] = keyboard_properties;
    if (!SelectCharactersData(
            global_properties_key,
            global_properties[strings::keyboard_properties])) {
      return false;
    }
  }
  if (!SelectVrHelpItemsData(global_properties_key, global_properties)) {
    return false;
  }
  return true;
}

bool ResumptionDataDB::SelectVrHelpItemsData(
    int64_t global_properties_key,
    smart_objects::SmartObject& global_properties) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  utils::dbms::SQLQuery checks_vrhelp_item(db());
  if (!checks_vrhelp_item.Prepare(kChecksVrHelpItem)) {
    LOG4CXX_WARN(logger_, "Problem with verification checks_vrhelp_item query");
    return false;
  }
  checks_vrhelp_item.Bind(0, global_properties_key);
  if (!checks_vrhelp_item.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution checks_vrhelp_item query");
    return false;
  }
  if (0 == checks_vrhelp_item.GetInteger(0)) {
    LOG4CXX_INFO(logger_, "Global properties doesn't contain vr help item");
    return true;
  }
  utils::dbms::SQLQuery select_vrhelp_item(db());
  if (!select_vrhelp_item.Prepare(kSelectVrHelpItem)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_vrhelp_item query");
    return false;
  }
  global_properties[strings::vr_help] = SmartObject(SmartType_Array);
  SmartObject& vr_help_items = global_properties[strings::vr_help];
  select_vrhelp_item.Bind(0, global_properties_key);
  size_t vr_help_item_idx = 0;
  /* Position of data in "select_vrhelp_item" :
     field "text" from table "vrHelpItem" = 0
     field "position" from table "vrHelpItem" = 1
     field "imageType" from table "image" = 2
     field "value" from table "image" = 3*/
  while (select_vrhelp_item.Next()) {
    SmartObject item(SmartType_Map);
    item[strings::text] = select_vrhelp_item.GetString(0);
    item[strings::position] = select_vrhelp_item.GetInteger(1);
    if (!select_vrhelp_item.IsNull(2) && !select_vrhelp_item.IsNull(3)) {
      SmartObject image(SmartType_Map);
      image[strings::image_type] = select_vrhelp_item.GetInteger(2);
      image[strings::value] = select_vrhelp_item.GetString(3);
      item[strings::image] = image;
    }
    vr_help_items[vr_help_item_idx++] = item;
  }
  LOG4CXX_INFO(logger_, "VR Help items were restored successfully");
  return true;
}

bool ResumptionDataDB::SelectCharactersData(
    int64_t global_properties_key,
    smart_objects::SmartObject& keyboard_properties) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  utils::dbms::SQLQuery checks_characters(db());
  if (!checks_characters.Prepare(kChecksCharacter)) {
    LOG4CXX_WARN(logger_, "Problem with verification checks_characters query");
    return false;
  }
  checks_characters.Bind(0, global_properties_key);
  if (!checks_characters.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution checks_characters query");
    return false;
  }
  if (0 == checks_characters.GetInteger(0)) {
    LOG4CXX_INFO(
        logger_,
        "Keyboard properties doesn't contain table limited character list");
    return true;
  }
  utils::dbms::SQLQuery select_characters(db());
  if (!select_characters.Prepare(kSelectCharacter)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_characters query");
    return false;
  }

  keyboard_properties[strings::limited_character_list] =
      SmartObject(SmartType_Array);
  SmartObject& characters =
      keyboard_properties[strings::limited_character_list];
  select_characters.Bind(0, global_properties_key);
  size_t characters_idx = 0;
  /* Position of data in "select_characters" :
     field "limitedCharacterList" from table "tableLimitedCharacterList" = 0*/
  while (select_characters.Next()) {
    characters[characters_idx++] = select_characters.GetString(0);
  }
  return true;
}

bool ResumptionDataDB::SelectImageData(
    int64_t image_key, smart_objects::SmartObject& image) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery select_image(db());

  if (!select_image.Prepare(kSelectImage)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_image query");
    return false;
  }
  select_image.Bind(0, image_key);
  if (!select_image.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution select_image query");
    return false;
  }
  /* Position of data in "select_image" :
     field "imageType" from table "image" = 0
     field "value" from table "image" = 1*/
  image[strings::image_type] = select_image.GetInteger(0);
  image[strings::value] = select_image.GetString(1);
  return true;
}

bool ResumptionDataDB::SelectTTSChunkData(
    int64_t tts_chunk_key, smart_objects::SmartObject& tts_chunk) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery select_tts_chunk(db());

  if (!select_tts_chunk.Prepare(kSelectTTSChunk)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_tts_chunk query");
    return false;
  }
  select_tts_chunk.Bind(0, tts_chunk_key);
  if (!select_tts_chunk.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution select_tts_chunk query");
    return false;
  }
  /* Position of data in "select_tts_chunk" :
     field "text" from table "TTSChunk" = 0
     field "type" from table "TTSChunk" = 1*/
  tts_chunk[strings::text] = select_tts_chunk.GetString(0);
  tts_chunk[strings::type] = select_tts_chunk.GetInteger(1);
  return true;
}

bool ResumptionDataDB::SelectDataFromAppTable(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(kSelectAppTable)) {
    LOG4CXX_WARN(logger_, "Problem with verification kSelectAppTable query");
    return false;
  }
  query.Bind(0, policy_app_id);
  query.Bind(1, device_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution kSelectAppTable query");
    return false;
  }

  //  Position of data in "query" :
  //  field "appID" from table "application" = 0
  //  field "connection_key" from table "application" = 1
  //  field "grammarID" from table "application" = 2
  //  field "hashID" from table "application" = 3
  //  field "hmiAppID" from table "application" = 4
  //  field "hmiLevel" from table "application" = 5
  //  field "ign_off_count" from table "application" = 6
  //  field "timeStamp" from table "application" = 7
  //  field "deviceID" from table "application" = 8
  //  field "isMediaApplication" from table "application" = 9
  //  field "IsSubscribedForWayPoints" from table "application" = 10
  uint32_t connection_key = query.GetUInteger(1);

  saved_app[strings::app_id] = query.GetString(0);
  saved_app[strings::connection_key] = connection_key;
  uint32_t grammarID = query.GetUInteger(2);
  if (grammarID) {
    saved_app[strings::grammar_id] = grammarID;
  }
  saved_app[strings::hash_id] = query.GetString(3);
  saved_app[strings::hmi_app_id] = query.GetUInteger(4);
  saved_app[strings::hmi_level] = query.GetInteger(5);
  saved_app[strings::ign_off_count] = query.GetInteger(6);
  saved_app[strings::time_stamp] = query.GetUInteger(7);
  saved_app[strings::device_id] = query.GetString(8);
  saved_app[strings::is_media_application] = query.GetBoolean(9);
  saved_app[strings::subscribed_for_way_points] = query.GetBoolean(10);

  LOG4CXX_INFO(logger_,
               "Data from application table was restored successfully");
  return true;
}

bool ResumptionDataDB::SelectCountFromArray(
    uint32_t& count_item,
    const std::string& text_query,
    const std::string& policy_app_id,
    const std::string& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if (!query.Prepare(text_query)) {
    LOG4CXX_WARN(logger_, "Problem with verification query");
    return false;
  }
  query.Bind(0, policy_app_id);
  query.Bind(1, device_id);
  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution query");
    return false;
  }
  count_item = query.GetInteger(0);
  LOG4CXX_INFO(logger_, "count_item=" << count_item);
  return true;
}

bool ResumptionDataDB::DeleteSavedApplication(const std::string& policy_app_id,
                                              const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::ScopeGuard guard =
      utils::MakeObjGuard(*db_, &utils::dbms::SQLDatabase::RollbackTransaction);

  db_->BeginTransaction();
  if (!DeleteSavedFiles(policy_app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedSubMenu(policy_app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedSubscriptions(policy_app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedCommands(policy_app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedChoiceSet(policy_app_id, device_id)) {
    return false;
  }
  if (!DeleteSavedGlobalProperties(policy_app_id, device_id)) {
    return false;
  }
  if (!DeleteDataFromApplicationTable(policy_app_id, device_id)) {
    return false;
  }
  db_->CommitTransaction();

  guard.Dismiss();
  return true;
}

bool ResumptionDataDB::DeleteSavedFiles(const std::string& policy_app_id,
                                        const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!ExecQueryToDeleteData(policy_app_id, device_id, kDeleteFile)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from file.");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteApplicationFilesArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from applicationFilesArray.");
    return false;
  }
  return true;
}

bool ResumptionDataDB::DeleteSavedSubMenu(const std::string& policy_app_id,
                                          const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecQueryToDeleteData(policy_app_id, device_id, kDeleteSubMenu)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from subMenu.");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteApplicationSubMenuArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from applicationSubMenuArray.");
    return false;
  }
  return true;
}

bool ResumptionDataDB::DeleteSavedSubscriptions(
    const std::string& policy_app_id, const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteApplicationSubscribtionsArray)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete from applicationSubscribtionsArray.");
    return false;
  }
  return true;
}

bool ResumptionDataDB::DeleteSavedCommands(const std::string& policy_app_id,
                                           const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteImageFromCommands)) {
    LOG4CXX_WARN(logger_, "Incorrect delete image from commands.");
    return false;
  }

  if (!ExecQueryToDeleteData(policy_app_id, device_id, kDeleteVrCommands)) {
    LOG4CXX_WARN(logger_, "Incorrect delete vrcommands from commands.");
    return false;
  }

  if (!ExecQueryToDeleteData(policy_app_id, device_id, kDeleteCommands)) {
    LOG4CXX_WARN(logger_, "Incorrect delete commands.");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteApplicationCommandsArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete applicationCommandsArray.");
    return false;
  }

  return true;
}

bool ResumptionDataDB::DeleteSavedChoiceSet(const std::string& policy_app_id,
                                            const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecUnionQueryToDeleteData(
          policy_app_id, device_id, kDeleteImageFromChoiceSet)) {
    LOG4CXX_WARN(logger_, "Incorrect delete image from choice set");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteVrCommandsFromChoiceSet)) {
    LOG4CXX_WARN(logger_, "Incorrect delete vrCommands from choice set");
    return false;
  }

  if (!ExecQueryToDeleteData(policy_app_id, device_id, kDeleteChoice)) {
    LOG4CXX_WARN(logger_, "Incorrect delete choiceSet");
    return false;
  }

  if (!ExecQueryToDeleteData(policy_app_id, device_id, kDeleteChoiceArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from choiceArray");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteApplicationChoiceSet)) {
    LOG4CXX_WARN(logger_, "Incorrect delete applicationChoiceSet");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteApplicationChoiceSetArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from ApplicationChoiceSetArray");
    return false;
  }

  return true;
}

bool ResumptionDataDB::DeleteSavedGlobalProperties(
    const std::string& policy_app_id, const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecUnionQueryToDeleteData(
          policy_app_id, device_id, kDeleteImageFromGlobalProperties)) {
    LOG4CXX_WARN(logger_, "Incorrect delete image from globalProperties");
    return false;
  }

  if (!ExecQueryToDeleteData(policy_app_id, device_id, kDeletevrHelpItem)) {
    LOG4CXX_WARN(logger_, "Incorrect delete vrHelpItem");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeletevrHelpItemArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete vrHelpItemArray");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteTableLimitedCharacterList)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from tableLimitedCharacterList");
    return false;
  }

  if (!ExecQueryToDeleteData(policy_app_id, device_id, kDeleteCharacterArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from characterArray");
    return false;
  }

  if (!ExecUnionQueryToDeleteData(policy_app_id, device_id, kDeleteTTSChunk)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from TTSChunk");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteHelpTimeoutPromptArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from HelpTimeoutPromptArray");
    return false;
  }

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteGlobalProperties)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from GlobalProperties");
    return false;
  }

  return true;
}

bool ResumptionDataDB::DeleteDataFromApplicationTable(
    const std::string& policy_app_id, const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecQueryToDeleteData(
          policy_app_id, device_id, kDeleteFromApplicationTable)) {
    LOG4CXX_WARN(logger_, "Incorrect delete data from application table");
    return false;
  }

  return true;
}

bool ResumptionDataDB::ExecQueryToDeleteData(const std::string& policy_app_id,
                                             const std::string& device_id,
                                             const std::string& text_query) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  bool result = query.Prepare(text_query);
  if (result) {
    query.Bind(0, policy_app_id);
    query.Bind(1, device_id);
    result = query.Exec();
  }
  return result;
}

bool ResumptionDataDB::ExecUnionQueryToDeleteData(
    const std::string& policy_app_id,
    const std::string& device_id,
    const std::string& text_query) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  bool result = query.Prepare(text_query);
  if (result) {
    query.Bind(0, policy_app_id);
    query.Bind(1, device_id);
    query.Bind(2, policy_app_id);
    query.Bind(3, device_id);
    result = query.Exec();
  }
  return result;
}

bool ResumptionDataDB::ExecInsertImage(
    int64_t& image_primary_key, const smart_objects::SmartObject& image) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery count_image_query(db());
  utils::dbms::SQLQuery query(db());
  uint16_t count_image = 0;
  bool result = count_image_query.Prepare(kSelectCountImage);
  if (result) {
    count_image_query.Bind(0, image[strings::value].asString());
    result = count_image_query.Exec();
    if (result) {
      count_image = count_image_query.GetInteger(0);
    }
  }
  if (!result) {
    LOG4CXX_WARN(logger_,
                 "Problem with preparing or execution count_image_query.");
    return false;
  }
  if (count_image) {
    result = query.Prepare(kSelectPrimaryKeyImage);
    if (result) {
      query.Bind(0, image[strings::value].asString());
      result = query.Exec();
      if (result) {
        image_primary_key = query.GetLongInt(0);
      }
    }
    if (!result) {
      LOG4CXX_WARN(logger_,
                   "Problem with preparing or execution "
                   "query for select primary key of image");
    }
  } else {
    result = query.Prepare(kInsertImage);
    if (result) {
      query.Bind(0, image[strings::image_type].asInt());
      query.Bind(1, image[strings::value].asString());
      result = query.Exec();
      if (result) {
        image_primary_key = query.LastInsertId();
      }
    }
    if (!result) {
      LOG4CXX_WARN(logger_,
                   "Problem with preparing or execution "
                   "query for insert image to image table");
    }
  }
  return result;
}

bool ResumptionDataDB::ExecInsertChoice(
    int64_t choice_set_key,
    const smart_objects::SmartObject& choice_array) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery insert_choice(db());

  if (!insert_choice.Prepare(kInsertChoice)) {
    LOG4CXX_WARN(logger_, "Incorrect preparation insert_choice query");
    return false;
  }
  /* Positions of binding data for "insert_choice":
     field "choiceID" from table "choice" = 0
     field "menuName" from table "choice" = 1
     field "secondaryText" from table "choice" = 2
     field "tertiaryText" from table "choice" = 3
     field "idimage" from table "choice" = 4
     field "idsecondaryImage" from table "choice" = 5*/
  int64_t image_primary_key = 0;
  int64_t choice_primary_key = 0;
  size_t length_choice_array = choice_array.length();
  for (size_t i = 0; i < length_choice_array; ++i) {
    insert_choice.Bind(0, (choice_array[i][strings::choice_id]).asInt());
    insert_choice.Bind(1, (choice_array[i][strings::menu_name]).asString());

    CustomBind(strings::secondary_text, choice_array[i], insert_choice, 2);
    CustomBind(strings::tertiary_text, choice_array[i], insert_choice, 3);

    if (choice_array[i].keyExists(strings::image)) {
      if (!ExecInsertImage(image_primary_key,
                           choice_array[i][strings::image])) {
        LOG4CXX_WARN(logger_, "Problem with insert image to choice");
        return false;
      }
      insert_choice.Bind(4, image_primary_key);
    } else {
      insert_choice.Bind(4);
    }
    if (choice_array[i].keyExists(strings::secondary_image)) {
      if (!ExecInsertImage(image_primary_key,
                           choice_array[i][strings::secondary_image])) {
        LOG4CXX_WARN(logger_, "Problem with insert secondary_image to choice");
        return false;
      }
      insert_choice.Bind(5, image_primary_key);
    } else {
      insert_choice.Bind(5);
    }
    if (!insert_choice.Exec()) {
      LOG4CXX_WARN(logger_, "Problem with execution insert_choice query");
      return false;
    }
    choice_primary_key = insert_choice.LastInsertId();

    if ((!ExecInsertVrCommands(choice_primary_key,
                               choice_array[i][strings::vr_commands],
                               kVRCommandFromChoice)) ||
        !insert_choice.Reset()) {
      LOG4CXX_WARN(logger_, "problemm with add vr commands to choice");
      return false;
    }

    if (!ExecInsertDataToArray(
            choice_set_key, choice_primary_key, kInsertChoiceArray)) {
      LOG4CXX_INFO(logger_, "Problem with insertion data to choiceArray table");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Choice data were saved to DB successfully");
  return true;
}

bool ResumptionDataDB::ExecInsertVrCommands(
    const int64_t primary_key,
    const smart_objects::SmartObject& vr_commands_array,
    AccessoryVRCommand value) const {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery insert_vr_command(db());

  if (!insert_vr_command.Prepare(kInsertVrCommand)) {
    LOG4CXX_WARN(logger_, "Incorrect preparation insert_vr_command query");
    return false;
  }
  size_t length_vr_commands = vr_commands_array.length();

  /* Positions of binding data for "insert_vr_command":
     field "vrCommand" from table "vrCommandsArray" = 0
     field "idcommand" from table "vrCommandsArray" = 1
     field "idchoice" from table "vrCommandsArray" = 2*/
  for (size_t i = 0; i < length_vr_commands; ++i) {
    insert_vr_command.Bind(0, vr_commands_array[i].asString());
    if (AccessoryVRCommand::kVRCommandFromCommand == value) {
      insert_vr_command.Bind(1, primary_key);
      insert_vr_command.Bind(2);
    } else if (AccessoryVRCommand::kVRCommandFromChoice == value) {
      insert_vr_command.Bind(1);
      insert_vr_command.Bind(2, primary_key);
    }
    if (!insert_vr_command.Exec() || !insert_vr_command.Reset()) {
      LOG4CXX_WARN(logger_, "Problem with insert vr_command to DB");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Insertion of Vr command were executed successfully");
  return true;
}

bool ResumptionDataDB::ExecInsertDataToArray(
    int64_t first_primary_key,
    int64_t second_primary_key,
    const std::string& text_query) const {
  LOG4CXX_AUTO_TRACE(logger_);
  bool result;
  utils::dbms::SQLQuery query_insert_array(db());
  result = query_insert_array.Prepare(text_query);
  if (result) {
    query_insert_array.Bind(0, first_primary_key);
    query_insert_array.Bind(1, second_primary_key);
    result = query_insert_array.Exec();
  }
  return result;
}

bool ResumptionDataDB::SaveApplicationToDB(
    app_mngr::ApplicationSharedPtr application,
    const std::string& policy_app_id,
    const std::string& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  int64_t application_primary_key = 0;
  int64_t global_properties_key = 0;
  db_->BeginTransaction();
  if (!InsertGlobalPropertiesData(GetApplicationGlobalProperties(application),
                                  global_properties_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert globalProperties data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  ApplicationParams app(application);
  if (!InsertApplicationData(app,
                             policy_app_id,
                             device_id,
                             &application_primary_key,
                             global_properties_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert application data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertFilesData(GetApplicationFiles(application),
                       application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert file data to DB.");
    db_->RollbackTransaction();
    return false;
  }

  if (!InsertSubMenuData(GetApplicationSubMenus(application),
                         application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert submenu data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertCommandsData(GetApplicationCommands(application),
                          application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert commands data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertSubscriptionsData(GetApplicationSubscriptions(application),
                               application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert subscribtions data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertChoiceSetData(GetApplicationInteractionChoiseSets(application),
                           application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert choiceset data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  db_->CommitTransaction();
  return true;
}

bool ResumptionDataDB::SaveApplicationToDB(
    const smart_objects::SmartObject& application,
    const std::string& policy_app_id,
    const std::string& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  int64_t application_primary_key = 0;
  int64_t global_properties_key = 0;
  db_->BeginTransaction();
  if (!InsertGlobalPropertiesData(application["globalProperties"],
                                  global_properties_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert globalProperties data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertApplicationData(application,
                             policy_app_id,
                             device_id,
                             &application_primary_key,
                             global_properties_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert application data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertFilesData(application["applicationFiles"],
                       application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert file data to DB.");
    db_->RollbackTransaction();
    return false;
  }

  if (!InsertSubMenuData(application["applicationSubMenus"],
                         application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert submenu data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertCommandsData(application["applicationCommands"],
                          application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert commands data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertSubscriptionsData(application["subscribtions"],
                               application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert subscribtions data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertChoiceSetData(application["applicationChoiceSets"],
                           application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert choiceset data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  db_->CommitTransaction();
  return true;
}

bool ResumptionDataDB::InsertFilesData(const smart_objects::SmartObject& files,
                                       int64_t application_primary_key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  const size_t length_files_array = files.length();
  if (0 == length_files_array) {
    LOG4CXX_INFO(logger_, "Application doesn't contain files");
    return true;
  }

  utils::dbms::SQLQuery query_insert_file(db());

  if (!query_insert_file.Prepare(kInsertToFile)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification queries for insertion files");
    return false;
  }
  /* Positions of binding data for "query_insert_file":
     field "fileType" from table "file" = 0
     field "is_download_complete" from table "file" = 1
     field "persistentFile" from table "file" = 2
     field "syncFileName" from table "file" = 3*/
  for (size_t i = 0; i < length_files_array; ++i) {
    query_insert_file.Bind(0, (files[i][strings::file_type]).asInt());
    query_insert_file.Bind(1,
                           (files[i][strings::is_download_complete]).asBool());
    query_insert_file.Bind(2, (files[i][strings::persistent_file]).asBool());
    query_insert_file.Bind(3, (files[i][strings::sync_file_name]).asString());

    if (!query_insert_file.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion of files data");
      return false;
    }

    if ((!ExecInsertDataToArray(application_primary_key,
                                query_insert_file.LastInsertId(),
                                kInsertToApplicationFilesArray)) ||
        !query_insert_file.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion to application files array");
      return false;
    }
  }

  LOG4CXX_INFO(logger_, "Files data were inserted successfully to DB");
  return true;
}

bool ResumptionDataDB::InsertSubMenuData(
    const smart_objects::SmartObject& submenus,
    int64_t application_primary_key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  const size_t length_submenu_array = submenus.length();
  if (0 == length_submenu_array) {
    LOG4CXX_INFO(logger_, "Application doesn't contain submenu");
    return true;
  }
  utils::dbms::SQLQuery query_insert_submenu(db());

  if (!query_insert_submenu.Prepare(kInsertToSubMenu)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification queries for insertion submenu");
    return false;
  }
  /* Positions of binding data for "query_insert_submenu":
     field "menuID" from table "submenu" = 0
     field "menuName" from table "submenu" = 1
     field "position" from table "submenu" = 2*/
  for (size_t i = 0; i < length_submenu_array; ++i) {
    query_insert_submenu.Bind(0, (submenus[i][strings::menu_id]).asInt());
    query_insert_submenu.Bind(1, (submenus[i][strings::menu_name]).asString());
    CustomBind(strings::position, submenus[i], query_insert_submenu, 2);

    if (!query_insert_submenu.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion of submenu data");
      return false;
    }

    if ((!ExecInsertDataToArray(application_primary_key,
                                query_insert_submenu.LastInsertId(),
                                kInsertToApplicationSubMenuArray)) ||
        !query_insert_submenu.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion to application submenu array");
      return false;
    }
  }

  LOG4CXX_INFO(logger_, "Data about submenu were inserted successfully to DB");
  return true;
}

bool ResumptionDataDB::InsertCommandsData(
    const smart_objects::SmartObject& commands,
    int64_t application_primary_key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  const size_t length_command_array = commands.length();
  if (0 == length_command_array) {
    LOG4CXX_INFO(logger_, "Application doesn't contain command");
    return true;
  }
  utils::dbms::SQLQuery query_insert_command(db());
  int64_t image_primary_key = 0;
  int64_t command_primary_key = 0;

  if (!query_insert_command.Prepare(kInsertToCommand)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification queries for insertion commands");
    return false;
  }
  /* Positions of binding data for "query_insert_command":
     field "cmdID" from table "command" = 0
     field "idimage" from table "command" = 1
     field "menuName" from table "command" = 2
     field "parentID" from table "command" = 3
     field "position" from table "command" = 4*/
  for (size_t i = 0; i < length_command_array; ++i) {
    query_insert_command.Bind(0, commands[i][strings::cmd_id].asInt());
    if (commands[i].keyExists(strings::cmd_icon)) {
      if (!ExecInsertImage(image_primary_key, commands[i][strings::cmd_icon])) {
        LOG4CXX_WARN(logger_, "Problem with insert command image to DB");
        return false;
      }
      query_insert_command.Bind(1, image_primary_key);
    } else {
      query_insert_command.Bind(1);
    }

    if (commands[i].keyExists(strings::menu_params)) {
      const SmartObject& menu_params = commands[i][strings::menu_params];
      query_insert_command.Bind(2, menu_params[strings::menu_name].asString());

      CustomBind(hmi_request::parent_id, menu_params, query_insert_command, 3);
      CustomBind(strings::position, menu_params, query_insert_command, 4);
    } else {
      query_insert_command.Bind(2);
      query_insert_command.Bind(3);
      query_insert_command.Bind(4);
    }
    if (!query_insert_command.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion of command data to DB");
      return false;
    }
    command_primary_key = query_insert_command.LastInsertId();
    if (commands[i].keyExists(strings::vr_commands)) {
      if (!ExecInsertVrCommands(command_primary_key,
                                commands[i][strings::vr_commands],
                                kVRCommandFromCommand)) {
        return false;
      }
    }
    if ((!ExecInsertDataToArray(application_primary_key,
                                command_primary_key,
                                kInsertApplicationCommandArray)) ||
        !query_insert_command.Reset()) {
      LOG4CXX_WARN(logger_,
                   "Incorrect insertion to application commands array");
      return false;
    }
  }
  return true;
}

bool ResumptionDataDB::InsertSubscriptionsData(
    const smart_objects::SmartObject& subscriptions,
    int64_t application_primary_key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;

  if (subscriptions.empty()) {
    LOG4CXX_INFO(logger_, "Application doesn't contain subscriptions");
    return true;
  }
  const SmartObject& btn_sub = subscriptions[strings::application_buttons];
  const SmartObject& vi_sub = subscriptions[strings::application_vehicle_info];
  size_t btn_sub_length = btn_sub.length();
  size_t vi_sub_length = vi_sub.length();
  size_t max_length =
      (btn_sub_length > vi_sub_length) ? btn_sub_length : vi_sub_length;

  utils::dbms::SQLQuery insert_subscriptions(db());
  if (!insert_subscriptions.Prepare(kInsertSubscriptions)) {
    LOG4CXX_WARN(
        logger_,
        "Problem with verification queries for insertion subscriptions");
    return false;
  }
  /* Positions of binding data for "insert_subscriptions":
       field "idApplication" from table "applicationSubscribtionsArray" = 0
       field "vehicleValue" from table "applicationSubscribtionsArray" = 1
       field "ButtonNameValue" from table "applicationSubscribtionsArray" = 2*/
  for (size_t i = 0; i < max_length; ++i) {
    insert_subscriptions.Bind(0, application_primary_key);
    if (i < vi_sub_length) {
      insert_subscriptions.Bind(1, vi_sub[i].asInt());
    } else {
      insert_subscriptions.Bind(1);
    }
    if (i < btn_sub_length) {
      insert_subscriptions.Bind(2, btn_sub[i].asInt());
    } else {
      insert_subscriptions.Bind(2);
    }
    if (!insert_subscriptions.Exec() || !insert_subscriptions.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion of buttons to subscriptions");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Subscriptions data were saved successfully");
  return true;
}

bool ResumptionDataDB::InsertChoiceSetData(
    const smart_objects::SmartObject& choicesets,
    int64_t application_primary_key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;

  if (choicesets.empty()) {
    LOG4CXX_INFO(logger_, "Application doesn't contain choiceSet");
    return true;
  }
  int64_t choice_set_key = 0;
  size_t length_choceset_array = choicesets.length();
  for (size_t i = 0; i < length_choceset_array; ++i) {
    if (!ExecInsertApplicationChoiceSet(choice_set_key, choicesets[i])) {
      return false;
    }

    if (!ExecInsertChoice(choice_set_key, choicesets[i][strings::choice_set])) {
      return false;
    }

    if (!ExecInsertDataToArray(choice_set_key,
                               application_primary_key,
                               kInsertApplicationChoiceSetArray)) {
      LOG4CXX_WARN(logger_,
                   "Problem with insertion data to"
                   " applicationChoiceSetArray table");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Choice set data were saved to DB successfully");
  return true;
}

bool ResumptionDataDB::ExecInsertApplicationChoiceSet(
    int64_t& choice_set_primary_key,
    const smart_objects::SmartObject& choiceset) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;

  utils::dbms::SQLQuery insert_application_choice_set(db());
  if (!insert_application_choice_set.Prepare(kInsertApplicationChoiceSet)) {
    LOG4CXX_WARN(logger_,
                 "Problem with preparation insert "
                 "application choice set query");
    return false;
  }
  /* Positions of binding data for "insert_application_choice_set":
     field "grammarID" from table "applicationChoiceSet" = 0
     field "interactionChoiceSetID" from table "applicationChoiceSet" = 1*/
  insert_application_choice_set.Bind(
      0, static_cast<int64_t>(choiceset[strings::grammar_id].asUInt()));
  insert_application_choice_set.Bind(
      1, choiceset[strings::interaction_choice_set_id].asInt());

  if (!insert_application_choice_set.Exec()) {
    LOG4CXX_WARN(logger_,
                 "Problem with execution insert application choice set query");
    return false;
  }
  choice_set_primary_key = insert_application_choice_set.LastInsertId();
  LOG4CXX_INFO(logger_, "Application choice data were saved successfully");
  return true;
}

bool ResumptionDataDB::InsertGlobalPropertiesData(
    const smart_objects::SmartObject& global_properties,
    int64_t& global_properties_key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  SmartMap::iterator it_begin = global_properties.map_begin();
  SmartMap::iterator it_end = global_properties.map_end();
  bool data_exists = false;
  while (it_begin != it_end) {
    if (SmartType::SmartType_Null != ((it_begin->second).getType())) {
      LOG4CXX_INFO(logger_, "Global properties contains - " << it_begin->first);
      data_exists = true;
      break;
    }
    ++it_begin;
  }
  if (!data_exists) {
    LOG4CXX_INFO(logger_, "Application doesn't contain global properties");
    return true;
  }

  utils::dbms::SQLQuery insert_global_properties(db());
  if (!insert_global_properties.Prepare(kInsertGlobalProperties)) {
    LOG4CXX_WARN(logger_,
                 "Problem with preparation query "
                 "insert_global_properties");
    return false;
  }
  /* Positions of binding data for "insert_global_properties":
     field "vrHelpTitle" from table "globalProperties" = 0
     field "menuTitle" from table "globalProperties" = 1
     field "idmenuIcon" from table "globalProperties" = 2
     field "language" from table "globalProperties" = 3
     field "keyboardLayout" from table "globalProperties" = 4
     field "keypressMode" from table "globalProperties" = 5
     field "autoCompleteText" from table "globalProperties" = 6*/

  CustomBind(
      strings::vr_help_title, global_properties, insert_global_properties, 0);
  CustomBind(
      strings::menu_title, global_properties, insert_global_properties, 1);

  if (SmartType::SmartType_Null ==
      global_properties[strings::menu_icon].getType()) {
    insert_global_properties.Bind(2);
  } else {
    int64_t image_key = 0;
    if (ExecInsertImage(image_key, global_properties[strings::menu_icon])) {
      insert_global_properties.Bind(2, image_key);
    } else {
      LOG4CXX_WARN(logger_, "Problem with insert image to global properties");
      return false;
    }
  }

  if (SmartType::SmartType_Null ==
      global_properties[strings::keyboard_properties].getType()) {
    insert_global_properties.Bind(3);
    insert_global_properties.Bind(4);
    insert_global_properties.Bind(5);
    insert_global_properties.Bind(6);
  } else {
    const SmartObject& kb_prop =
        global_properties[strings::keyboard_properties];

    CustomBind(strings::language, kb_prop, insert_global_properties, 3);
    CustomBind(
        hmi_request::keyboard_layout, kb_prop, insert_global_properties, 4);
    CustomBind(strings::key_press_mode, kb_prop, insert_global_properties, 5);
    CustomBind(
        strings::auto_complete_text, kb_prop, insert_global_properties, 6);
  }
  if (!insert_global_properties.Exec()) {
    LOG4CXX_WARN(logger_,
                 "Problem with insert data to global properties table");
    return false;
  }

  global_properties_key = insert_global_properties.LastInsertId();
  if ((SmartType::SmartType_Null !=
       global_properties[strings::keyboard_properties].getType()) &&
      (global_properties[strings::keyboard_properties].keyExists(
          strings::limited_character_list))) {
    if (!ExecInsertLimitedCharacters(
            global_properties_key,
            global_properties[strings::keyboard_properties]
                             [strings::limited_character_list])) {
      LOG4CXX_WARN(logger_,
                   "Problem with insert data to limited_character table");
      return false;
    }
  }

  if (SmartType::SmartType_Null !=
      global_properties[strings::vr_help].getType()) {
    if (!ExecInsertVRHelpItem(global_properties_key,
                              global_properties[strings::vr_help])) {
      LOG4CXX_WARN(logger_, "Problem with insert data to vrHelpItem table");
      return false;
    }
  }

  if (!ExecInsertHelpTimeoutArray(global_properties, global_properties_key)) {
    LOG4CXX_WARN(logger_,
                 "Problem with insert data to HelpTimeoutPromptArray table");
    return false;
  }

  LOG4CXX_INFO(logger_, "Global properties data were saved successfully");
  return true;
}

bool ResumptionDataDB::ExecInsertHelpTimeoutArray(
    const smart_objects::SmartObject& global_properties,
    int64_t global_properties_key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  using namespace smart_objects;
  size_t timeout_prompt_length = 0;
  size_t help_prompt_length = 0;

  if (SmartType::SmartType_Null !=
      global_properties[strings::help_prompt].getType()) {
    help_prompt_length = global_properties[strings::help_prompt].length();
  }

  if (SmartType::SmartType_Null !=
      global_properties[strings::timeout_prompt].getType()) {
    timeout_prompt_length = global_properties[strings::timeout_prompt].length();
  }
  if (0 == timeout_prompt_length && 0 == help_prompt_length) {
    LOG4CXX_INFO(logger_,
                 "Application doesn't HelpPrompt and timoutPrompt data");
    return true;
  }

  utils::dbms::SQLQuery insert_help_prompt_array(db());

  if (!insert_help_prompt_array.Prepare(kInsertHelpTimeoutPromptArray)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification query insert_help_prompt_array");
    return false;
  }
  int64_t tts_chunk_key = 0;

  size_t max_length = (timeout_prompt_length > help_prompt_length)
                          ? timeout_prompt_length
                          : help_prompt_length;
  /* Positions of binding data for "insert_help_prompt_array":
     field "idglobalProperties" from table "helpTimeoutPromptArray" = 0
     field "idtimeoutPrompt" from table "helpTimeoutPromptArray" = 1
     field "idhelpPrompt" from table "helpTimeoutPromptArray" = 2*/
  for (size_t i = 0; i < max_length; ++i) {
    insert_help_prompt_array.Bind(0, global_properties_key);
    if (i < timeout_prompt_length) {
      if (!ExecInsertTTSChunks(global_properties[strings::timeout_prompt][i],
                               tts_chunk_key)) {
        LOG4CXX_WARN(logger_,
                     "Problem with insertion timeoutPrompt's ttsChunk");
        return false;
      }
      insert_help_prompt_array.Bind(1, tts_chunk_key);
    } else {
      insert_help_prompt_array.Bind(1);
    }

    if (i < help_prompt_length) {
      if (!ExecInsertTTSChunks(global_properties[strings::help_prompt][i],
                               tts_chunk_key)) {
        LOG4CXX_WARN(logger_, "Problem with insertion helpPrompt's ttsChunk");
        return false;
      }
      insert_help_prompt_array.Bind(2, tts_chunk_key);
    } else {
      insert_help_prompt_array.Bind(2);
    }
    if (!insert_help_prompt_array.Exec() || !insert_help_prompt_array.Reset()) {
      LOG4CXX_WARN(
          logger_,
          "Problem with execution or resetting insert_help_prompt_array query");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Data were saved to helpTimeoutPromptArray table");
  return true;
}

bool ResumptionDataDB::ExecInsertTTSChunks(
    const smart_objects::SmartObject& tts_chunk, int64_t& tts_chunk_key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery insert_tts_chunk(db());
  if (!insert_tts_chunk.Prepare(kInsertTTSChunk)) {
    LOG4CXX_WARN(logger_, "Problem with verification insert_tts_chunk query");
    return false;
  }
  /* Positions of binding data for "insert_tts_chunk":
     field "type" from table "TTSChunk" = 0
     field "text" from table "TTSChunk" = 1*/
  insert_tts_chunk.Bind(0, tts_chunk[strings::type].asInt());
  insert_tts_chunk.Bind(1, tts_chunk[strings::text].asString());
  if (!insert_tts_chunk.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution insert_tts_chunk query");
    return false;
  }
  tts_chunk_key = insert_tts_chunk.LastInsertId();
  LOG4CXX_WARN(logger_, "TTSChunk was saved successfully");
  return true;
}

bool ResumptionDataDB::ExecInsertLimitedCharacters(
    int64_t global_properties_key,
    const smart_objects::SmartObject& characters_array) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery insert_characters(db());
  if (!insert_characters.Prepare(kInsertTableLimitedCharacter)) {
    LOG4CXX_WARN(logger_,
                 "Problem with preparation query "
                 "insert_characters");
    return false;
  }
  size_t length_characters_array = characters_array.length();
  /* Positions of binding data for "insert_characters":
     field "limitedCharacterList" from table "tableLimitedCharacterList" = 0*/
  for (size_t i = 0; i < length_characters_array; ++i) {
    insert_characters.Bind(0, characters_array[i].asString());

    if (!insert_characters.Exec()) {
      LOG4CXX_WARN(logger_,
                   "Problem with insert data to limited_character table");
      return false;
    }
    if ((!ExecInsertDataToArray(global_properties_key,
                                insert_characters.LastInsertId(),
                                kInsertCharacterArray)) ||
        (!insert_characters.Reset())) {
      LOG4CXX_WARN(logger_, "Problem with insert data to characterArray table");
      return false;
    }
  }
  LOG4CXX_INFO(logger_,
               "Data were saved successfully to limited_character table");
  return true;
}

bool ResumptionDataDB::ExecInsertVRHelpItem(
    int64_t global_properties_key,
    const smart_objects::SmartObject& vrhelp_array) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery insert_vrhelp_item(db());
  if (!insert_vrhelp_item.Prepare(kInsertVRHelpItem)) {
    LOG4CXX_WARN(logger_, "Problem with preparation query insert_vrhelp_item");
    return false;
  }
  int64_t image_primary_key = 0;
  size_t length_vrhelp_array = vrhelp_array.length();
  /* Positions of binding data for "insert_vrhelp_item":
     field "text" from table "vrHelpItem" = 0
     field "position" from table "vrHelpItem" = 1
     field "idimage" from table "vrHelpItem" = 2*/
  for (size_t i = 0; i < length_vrhelp_array; ++i) {
    insert_vrhelp_item.Bind(0, vrhelp_array[i][strings::text].asString());
    insert_vrhelp_item.Bind(1, vrhelp_array[i][strings::position].asInt());
    if (vrhelp_array[i].keyExists(strings::image)) {
      if (!ExecInsertImage(image_primary_key,
                           vrhelp_array[i][strings::image])) {
        LOG4CXX_INFO(logger_, "Problem with insert image to vrHelpItem table");
        return false;
      }
      insert_vrhelp_item.Bind(2, image_primary_key);
    } else {
      insert_vrhelp_item.Bind(2);
    }

    if (!insert_vrhelp_item.Exec()) {
      LOG4CXX_INFO(logger_, "Problem with insert data vrHelpItem table");
      return false;
    }

    if ((!ExecInsertDataToArray(global_properties_key,
                                insert_vrhelp_item.LastInsertId(),
                                kInsertVRHelpItemArray)) ||
        (!insert_vrhelp_item.Reset())) {
      LOG4CXX_WARN(logger_,
                   "Problem with insert data to vrHelpItemArray table");
      return false;
    }
  }
  LOG4CXX_INFO(logger_,
               "Data were saved successfully to vrHelpItem array table");
  return true;
}

bool ResumptionDataDB::InsertApplicationData(
    app_mngr::ApplicationSharedPtr application,
    const std::string& policy_app_id,
    const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationParams app(application);
  return InsertApplicationData(app, policy_app_id, device_id, NULL, 0);
}

bool ResumptionDataDB::InsertApplicationData(
    const ApplicationParams& application,
    const std::string& policy_app_id,
    const std::string& device_id,
    int64_t* application_primary_key,
    int64_t global_properties_key) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery query(db());

  if (!application.m_is_valid) {
    LOG4CXX_ERROR(logger_, "Invalid application params passed.");
    return false;
  }

  const std::string hash = application.m_hash;
  const int64_t grammar_id = application.m_grammar_id;
  const int64_t time_stamp = static_cast<int64_t>(time(NULL));
  const int64_t connection_key = application.m_connection_key;
  const int64_t hmi_app_id = application.m_hmi_app_id;
  const mobile_apis::HMILevel::eType hmi_level = application.m_hmi_level;
  bool is_media_application = application.m_is_media_application;
  bool is_subscribed_for_way_points =
      application_manager_.IsAppSubscribedForWayPoints(application.app_ptr);

  if (!query.Prepare(kInsertApplication)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification query "
                 "for insert to table application");
    return false;
  }

  /* Positions of binding data for "query":
     field "connection_key" from table "application" = 0
     field "grammarID" from table "application" = 1
     field "hashID" from table "application" = 2
     field "hmiAppID" from table "application" = 3
     field "hmiLevel" from table "application" = 4
     field "ign_off_count" from table "application" = 5
     field "timeStamp" from table "application" = 6
     field "idglobalProperties" from table "application" = 7
     field "isMediaApplication" from table "application" = 8
     field "appID" from table "application" = 9
     field "deviceID" from table "application" = 10
     field "isSubscribedForWayPoints" from table "application" = 11*/
  query.Bind(0, connection_key);
  query.Bind(1, grammar_id);
  query.Bind(2, hash);
  query.Bind(3, hmi_app_id);
  query.Bind(4, static_cast<int32_t>(hmi_level));
  query.Bind(5, 0);
  query.Bind(6, time_stamp);
  if (global_properties_key) {
    query.Bind(7, global_properties_key);
  } else {
    query.Bind(7);
  }
  query.Bind(8, is_media_application);
  query.Bind(9, policy_app_id);
  query.Bind(10, device_id);
  query.Bind(11, is_subscribed_for_way_points);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution query");
    return false;
  }
  if (NULL != application_primary_key) {
    *application_primary_key = query.LastInsertId();
  }
  LOG4CXX_INFO(logger_, "Data were saved successfully to application table");
  return true;
}

void ResumptionDataDB::CustomBind(const std::string& key,
                                  const smart_objects::SmartObject& so,
                                  utils::dbms::SQLQuery& query,
                                  const int pos) const {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace smart_objects;
  if (so.keyExists(key) && SmartType::SmartType_Null != so[key].getType()) {
    switch (so[key].getType()) {
      case SmartType::SmartType_Integer: {
        query.Bind(pos, so[key].asInt());
        break;
      }
      case SmartType::SmartType_String: {
        query.Bind(pos, so[key].asString());
        break;
      }
      default: {
        LOG4CXX_WARN(logger_, "Incorrect type");
        break;
      }
    }
  } else {
    query.Bind(pos);
  }
}

bool ResumptionDataDB::PrepareSelectQuery(utils::dbms::SQLQuery& query,
                                          const std::string& policy_app_id,
                                          const std::string& device_id,
                                          const std::string& text_query) const {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!query.Prepare(text_query)) {
    LOG4CXX_WARN(logger_, "Problem with verification query");
    return false;
  }
  query.Bind(0, policy_app_id);
  query.Bind(1, device_id);
  return true;
}

void ResumptionDataDB::UpdateDataOnAwake() {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kUpdateIgnOffCount)) {
    if (query.Exec()) {
      LOG4CXX_INFO(logger_,
                   "Values of ignition off counts were updated successfully");
      WriteDb();
    }
  }
}

bool ResumptionDataDB::UpdateApplicationData(
    app_mngr::ApplicationConstSharedPtr application,
    const std::string& policy_app_id,
    const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery query(db());

  const int64_t time_stamp = static_cast<int64_t>(time(NULL));
  const mobile_apis::HMILevel::eType hmi_level = application->hmi_level();

  if (!query.Prepare(kUpdateApplicationData)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification query "
                 "for updating some application data");
    return false;
  }

  /* Positions of binding data for "query":
     field "hmiLevel" from table "application" = 0
     field "timeStamp" from table "application" = 1
     field "appID" from table "application" = 2
     field "deviceID" from table "application" = 3*/
  query.Bind(0, static_cast<int32_t>(hmi_level));
  query.Bind(1, time_stamp);
  query.Bind(2, policy_app_id);
  query.Bind(3, device_id);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution query");
    return false;
  }
  LOG4CXX_INFO(logger_, "Data were updated successfully in application table");
  return true;
}

void ResumptionDataDB::WriteDb() {
  LOG4CXX_AUTO_TRACE(logger_);
  db_->Backup();
}

bool ResumptionDataDB::UpdateGrammarID(const std::string& policy_app_id,
                                       const std::string& device_id,
                                       const uint32_t grammar_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(kUpdateGrammarID)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification query for updating grammar id.");
    return false;
  }

  //  Positions of binding data for "query":
  //  field "grammarID" from table "application" = 0
  //  field "appID" from table "application" = 1
  //  field "deviceID" from table "application" = 2
  query.Bind(0, static_cast<int32_t>(grammar_id));
  query.Bind(1, policy_app_id);
  query.Bind(2, device_id);

  if (!query.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution query");
    return false;
  }
  LOG4CXX_INFO(logger_, "Data were updated successfully in application table");
  return true;
}

utils::dbms::SQLDatabase* ResumptionDataDB::db() const {
#ifdef __QNX__
  utils::dbms::SQLDatabase* db = new utils::dbms::SQLDatabase(kDatabaseName);
  db->Open();
  return db;
#else
  return db_;
#endif
}

ApplicationParams::ApplicationParams(
    const smart_objects::SmartObject& application)
    : m_hash()
    , m_grammar_id(0)
    , m_connection_key(0)
    , m_hmi_app_id(0)
    , m_hmi_level(mobile_apis::HMILevel::INVALID_ENUM)
    , m_is_media_application(false)
    , m_is_valid(false) {
  using namespace app_mngr::strings;
  if (!application.keyExists(app_id) || !application.keyExists(hash_id) ||
      !application.keyExists(grammar_id) ||
      !application.keyExists(connection_key) ||
      !application.keyExists(hmi_app_id) || !application.keyExists(hmi_level) ||
      !application.keyExists(is_media_application)) {
    return;
  }
  m_is_valid = true;
  m_hash = application[hash_id].asString();
  m_grammar_id = application[grammar_id].asInt();
  m_connection_key = application[connection_key].asInt();
  m_hmi_app_id = application[hmi_app_id].asInt();
  m_hmi_level =
      static_cast<mobile_apis::HMILevel::eType>(application[hmi_level].asInt());
  m_is_media_application = application[is_media_application].asBool();
}

ApplicationParams::ApplicationParams(app_mngr::ApplicationSharedPtr application)
    : m_hash()
    , m_grammar_id(0)
    , m_connection_key(0)
    , m_hmi_app_id(0)
    , m_hmi_level(mobile_apis::HMILevel::INVALID_ENUM)
    , m_is_media_application(false)
    , m_is_valid(false) {
  if (application) {
    m_is_valid = true;
    m_hash = application->curHash();
    m_grammar_id = application->get_grammar_id();
    m_connection_key = application->app_id();
    m_hmi_app_id = application->hmi_app_id();
    m_hmi_level = application->hmi_level();
    m_is_media_application = application->IsAudioApplication();
    app_ptr = application;
  }
}

}  // namespace resumption

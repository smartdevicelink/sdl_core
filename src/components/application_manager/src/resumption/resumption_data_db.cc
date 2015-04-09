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

#include "application_manager/resumption/resumption_data_db.h"
#include "application_manager/resumption/resumption_sql_queries.h"
#include "application_manager/smart_object_keys.h"
#include "config_profile/profile.h"
#include "application_manager/message_helper.h"

namespace {
#ifdef CUSTOMER_PASA
const std::string kDatabaseName = "resumption.db";
#else  // CUSTOMER_PASA
const std::string kDatabaseName = "resumption";
#endif  // CUSTOMER_PASA
}

namespace resumption {
CREATE_LOGGERPTR_GLOBAL(logger_, "ResumptionDataDB")


ResumptionDataDB::ResumptionDataDB()
: db_(new utils::dbms::SQLDatabase(kDatabaseName)) {
#ifndef __QNX__
  std::string path = profile::Profile::instance()->app_storage_folder();
  if (!path.empty()) {
    db_->set_path(path + "/");
  }
#endif  // __QNX__
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
        profile::Profile::instance()->attempts_to_open_resumption_db();
    LOG4CXX_DEBUG(logger_, "Total attempts number is: " << attempts);
    bool is_opened = false;
    const uint16_t open_attempt_timeout_ms =
        profile::Profile::instance()->open_attempt_timeout_ms_resumption_db();
    const useconds_t sleep_interval_mcsec = open_attempt_timeout_ms * 1000;
    LOG4CXX_DEBUG(logger_, "Open attempt timeout(ms) is: "
                  << open_attempt_timeout_ms);
    for (int i = 0; i < attempts; ++i) {
      usleep(sleep_interval_mcsec);
      LOG4CXX_INFO(logger_, "Attempt: " << i+1);
      if (db_->Open()){
        LOG4CXX_INFO(logger_, "Database opened.");
        is_opened = true;
        break;
      }
    }
    if (!is_opened) {
      LOG4CXX_ERROR(logger_, "Open retry sequence failed. Tried "
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
  if (!query_checks_resumption.Prepare(kChecksResumptionData) || !query_checks_resumption.Exec()) {
    LOG4CXX_ERROR(logger_,
                  "Failed verification or execution query kChecksResumptionData"
                  << query_checks_resumption.LastError().text());
    return false;
  }
  if (0 == query_checks_resumption.GetInteger(0)) {
    utils::dbms::SQLQuery query_insert_resumption(db());
    if (!query_insert_resumption.Prepare(kInsertInitData) ||
        !query_insert_resumption.Exec()){
      LOG4CXX_ERROR(logger_,
                    "Failed insert init data to database: "
                    << query_insert_resumption.LastError().text());
      return false;
    }
  }
  return true;
}

void ResumptionDataDB::SaveApplication(
    app_mngr::ApplicationConstSharedPtr application) {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(application);
  bool application_exist = false;
  const std::string& m_app_id = application->mobile_app_id();
  const std::string device_id =
        MessageHelper::GetDeviceMacAddressForHandle(application->device());
  LOG4CXX_INFO(logger_, "app_id : " << application->app_id()
                <<" mobile app_id : " << m_app_id
                <<" device_id : " << device_id);


  if (!CheckExistenceApplication(m_app_id, device_id, application_exist)) {
    LOG4CXX_ERROR(logger_, "Problem with access to DB");
    return;
  }

  if (application_exist) {
    if (!DeleteSavedApplication(m_app_id, device_id)) {
      LOG4CXX_ERROR(logger_, "Deleting of application data is not finished");
      return;
    }
  }

  if (!SaveApplicationToDB(application, m_app_id, device_id, application_exist)) {
    LOG4CXX_ERROR(logger_, "Saving of application data is not finished");
    return;
  }

  LOG4CXX_INFO(logger_, "All data from application were saved successfully");
}

int ResumptionDataDB::GetStoredHMILevel(const std::string& m_app_id,
                                        const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  int hmi_level;
  if (SelectHMILevel(m_app_id, device_id, hmi_level)) {
    LOG4CXX_INFO(logger_, "Application with mobile application id  = "<<m_app_id
                 <<" and device id = "<<device_id<<"has hmi level = "<<hmi_level);
    return hmi_level;
  }
  LOG4CXX_FATAL(logger_, "HMI level doesn't exists in saved data");
  return -1;
}

bool ResumptionDataDB::IsHMIApplicationIdExist(uint32_t hmi_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  return CheckExistenceHMIId(hmi_app_id);
}

bool ResumptionDataDB::CheckSavedApplication(const std::string& mobile_app_id,
                                             const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool application_exist = false;
  if (!CheckExistenceApplication(mobile_app_id, device_id, application_exist) ||
      !application_exist) {
    LOG4CXX_WARN(logger_, "Problem with access to DB or application does not exist");
    return false;
  }
  LOG4CXX_INFO(logger_, "Application with mobile_app_id = "<<mobile_app_id
                   <<" and device_id = "<<device_id<<" does exist");
  return true;
}

uint32_t ResumptionDataDB::GetHMIApplicationID(const std::string& mobile_app_id,
                                               const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t hmi_app_id = 0;
  SelectHMIId(mobile_app_id, device_id, hmi_app_id);
  return hmi_app_id;
}

void ResumptionDataDB::OnSuspend() {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query_update_suspend_data(db());
  utils::dbms::SQLQuery query_update_last_ign_off_time(db());
  int application_lifes = 3;

  if(DeleteAppWithIgnCount(application_lifes)) {
    LOG4CXX_INFO(logger_, "Saved application with ign_off_count = "
                 << application_lifes<<" was deleted");
  } else {
    LOG4CXX_WARN(logger_, "Problem with removing applications");
  }

  if (query_update_suspend_data.Prepare(kUpdateSuspendData)) {
    if (query_update_suspend_data.Exec()) {
      LOG4CXX_INFO(logger_, "Data ign_off_count and suspend_count was updated");
    }
  }

  if (query_update_last_ign_off_time.Prepare(KUpdateLastIgnOffTime)) {
    query_update_last_ign_off_time.Bind(0, static_cast<int64_t>(time(NULL)));
    if (query_update_last_ign_off_time.Exec()) {
      LOG4CXX_INFO(logger_, "Data last_ign_off_time was updated");
    }
  }
}

bool ResumptionDataDB::DeleteAppWithIgnCount(int application_lifes) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery select_apps_for_removing(db());
  utils::dbms::SQLQuery count_app(db());

  if(!select_apps_for_removing.Prepare(kSelectApplicationsIgnOffCount) ||
      !count_app.Prepare(kCountApplicationsIgnOff)) {
    LOG4CXX_WARN(logger_, "Problem with verification query select_apps_for_removing or"
        " query count_app");
    return false;
  }
  count_app.Bind(0, application_lifes);
  if (!count_app.Exec() || !count_app.GetInteger(0)) {
    LOG4CXX_WARN(logger_, "Problem with execution or count app=0");
    return false;
  }
  std::string mobile_app_id;
  std::string device_id;
  select_apps_for_removing.Bind(0, application_lifes);
  while(select_apps_for_removing.Next()) {
    device_id = select_apps_for_removing.GetString(0);
    mobile_app_id = select_apps_for_removing.GetString(1);
    if (!DeleteSavedApplication(mobile_app_id, device_id)) {
      LOG4CXX_WARN(logger_, "Problem with removing application data");
      return false;
    }
  }
  LOG4CXX_WARN(logger_, "Applications data were removed successfully");
  return true;
}


bool ResumptionDataDB::GetHashId(const std::string& mobile_app_id,
                                 const std::string& device_id,
                                 std::string& hash_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  return SelectHashId(mobile_app_id, device_id, hash_id);
}

void ResumptionDataDB::OnAwake() {
  LOG4CXX_AUTO_TRACE(logger_);

  UpdateDataOnAwake();
}

bool ResumptionDataDB::GetSavedApplication(const std::string& mobile_app_id,
                                           const std::string& device_id,
                                           smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool application_exist = false;

  if (!CheckExistenceApplication(mobile_app_id, device_id, application_exist) ||
      !application_exist) {
    LOG4CXX_WARN(logger_, "Problem with access to DB or application does not exists");
    return false;
  }

  if (!SelectDataFromAppTable(mobile_app_id, device_id, saved_app)) {
    LOG4CXX_WARN(logger_, "Problem with restoring of data from application table");
    return false;
  }

  if (!SelectFilesData(mobile_app_id, device_id, saved_app)) {
    LOG4CXX_WARN(logger_, "Problem with restoring of files data");
    return false;
  }

  if (!SelectSubmenuData(mobile_app_id, device_id, saved_app)) {
    LOG4CXX_WARN(logger_, "Problem with restoring of submenu data");
    return false;
  }

  if (!SelectCommandData(mobile_app_id, device_id, saved_app)) {
    LOG4CXX_WARN(logger_, "Problem with restoring of command data");
    return false;
  }

  if (!SelectSubscriptionsData(mobile_app_id, device_id, saved_app)) {
    LOG4CXX_WARN(logger_, "Problem with restoring of subscriptions data");
    return false;
  }

  if (!SelectChoiceSetData(mobile_app_id, device_id, saved_app)) {
    LOG4CXX_WARN(logger_, "Problem with restoring of choice set data");
    return false;
  }

  if (!SelectGlobalPropertiesData(mobile_app_id, device_id, saved_app)) {
    LOG4CXX_WARN(logger_, "Problem with restoring of global properties data");
    return false;
  }
  LOG4CXX_INFO(logger_, "Application data were restored successfully");
  return true;
}

bool ResumptionDataDB::RemoveApplicationFromSaved(
    const std::string& mobile_app_id, const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool application_exist = false;
  if (!CheckExistenceApplication(mobile_app_id, device_id, application_exist) ||
      !application_exist) {
    LOG4CXX_ERROR(logger_, "Problem with access to DB or application does not"
        " exist");
    return false;
  }
  return DeleteSavedApplication(mobile_app_id, device_id);
}

uint32_t ResumptionDataDB::GetIgnOffTime() {
  LOG4CXX_AUTO_TRACE(logger_);

  return SelectIgnOffTime();
}

int ResumptionDataDB::IsApplicationSaved(const std::string& mobile_app_id,
                                         const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool application_exist = false;
  if (CheckExistenceApplication(mobile_app_id, device_id, application_exist) &&
      application_exist) {
    LOG4CXX_INFO(logger_, "Application exists in stored data");
    return 0;
  }
  return -1;
}

void ResumptionDataDB::GetDataForLoadResumeData(smart_objects::SmartObject& saved_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  SelectDataForLoadResumeData(saved_data);
}

bool ResumptionDataDB::SelectHMILevel(const std::string& m_app_id,
                                      const std::string& device_id,
                                      int& hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query_count(db());
  utils::dbms::SQLQuery query_select(db());
  if (query_count.Prepare(kSelectCountHMILevel) &&
      query_select.Prepare(kSelectHMILevel)) {
    query_count.Bind(0, device_id);
    query_count.Bind(1, m_app_id);
    query_select.Bind(0, device_id);
    query_select.Bind(1, m_app_id);
    if (query_count.Exec() && query_count.GetInteger(0) &&
        query_select.Exec()) {
      hmi_level = query_select.GetInteger(0);
      return true;
    }
  }
  return false;
}

bool ResumptionDataDB::CheckExistenceHMIId(uint32_t hmi_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kCheckHMIId)) {
    query.Bind(0, static_cast<int64_t>(hmi_app_id));
    if (query.Exec() && (query.GetInteger(0))) {
      LOG4CXX_INFO(logger_, "Saved data has HMI appID");
      return true;
    }
  }
  LOG4CXX_FATAL(logger_, "HMI appID = " <<hmi_app_id
                <<" doesn't exist in saved data");
  return false;
}

void ResumptionDataDB::SelectHMIId(const std::string& mobile_app_id,
                                   const std::string& device_id,
                                   uint32_t& hmi_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query_select(db());
  utils::dbms::SQLQuery query_check(db());

  if (query_select.Prepare(kSelectHMIId) &&
      query_check.Prepare(kSelectCountHMIId)) {
    query_select.Bind(0, device_id);
    query_select.Bind(1, mobile_app_id);
    query_check.Bind(0, device_id);
    query_check.Bind(1, mobile_app_id);
    if (query_check.Exec() && query_check.GetInteger(0)
        && query_select.Exec()) {
      hmi_id = query_select.GetUInteger(0);
      LOG4CXX_INFO(logger_, "HMI appID = "<<hmi_id);
      return;
    }
  }
  LOG4CXX_FATAL(logger_, "Saved data doesn't have application with "
                "device id = "<<device_id<<" and mobile appID = "<< mobile_app_id);
}

bool ResumptionDataDB::SelectHashId(const std::string& mobile_app_id,
                  const std::string& device_id,
                  std::string& hash_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery count(db());
  utils::dbms::SQLQuery select_hash(db());
  if (!select_hash.Prepare(kSelectHashId) ||
      !count.Prepare(kCountHashId)) {
    LOG4CXX_WARN(logger_, "Problem with verification count query or"
        " select_hash query");
    return false;
  }
  count.Bind(0, device_id);
  count.Bind(1, mobile_app_id);
  select_hash.Bind(0, device_id);
  select_hash.Bind(1, mobile_app_id);
  if (count.Exec() && count.GetInteger(0) && select_hash.Exec()) {
    hash_id = select_hash.GetString(0);
    LOG4CXX_INFO(logger_, "Saved hash ID = "<<hash_id);
    return true;
  }
  LOG4CXX_WARN(logger_, "Saved data doesn't have application with "
                "device id = "<<device_id<<" and mobile appID = "<<mobile_app_id
                << "or hashID");
  return false;
}

uint32_t ResumptionDataDB::SelectIgnOffTime() {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t ignOffTime = 0;
  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kSelectIgnOffTime)) {
    if (query.Exec()) {
      ignOffTime = query.GetUInteger(0);
      LOG4CXX_INFO(logger_, "Last ign off time = "<<ignOffTime);
      return ignOffTime;
    }
  }
  LOG4CXX_FATAL(logger_, "Problem with prepare query");
  return ignOffTime;
}

bool ResumptionDataDB::CheckExistenceApplication(const std::string& mobile_app_id,
                                                 const std::string& device_id,
                                                 bool& application_exist) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool result = false;
  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kCheckApplication)) {
    query.Bind(0, device_id);
    query.Bind(1, mobile_app_id);
    result = query.Exec();
  }
  if (result && query.GetInteger(0)) {
    LOG4CXX_INFO(logger_, "Saved data has application with mobile appID = "
                       <<mobile_app_id<<" and deviceID = "<<device_id);
    application_exist = true;
  } else if (result) {
    LOG4CXX_INFO(logger_, "Saved data does not contain application");
    application_exist = false;
  } else {
    LOG4CXX_WARN(logger_, "Problem with access DB");
  }
  return result;
}

void ResumptionDataDB::SelectDataForLoadResumeData(
    smart_objects::SmartObject& saved_data) {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery select_data(db());
  utils::dbms::SQLQuery count_application(db());
  if (!select_data.Prepare(kSelectDataForLoadResumeData) ||
      !count_application.Prepare(kCountApplications)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_data query"
        " or count application");
    return;
  }

  if (!count_application.Exec() || !count_application.GetInteger(0)) {
    LOG4CXX_WARN(logger_, "Problem with execution count_application query"
                 " or appliction table does not contain data");
    return;
  }
  smart_objects::SmartObject so_array_data(smart_objects::SmartType_Array);
  int i = 0;
  while(select_data.Next()) {
    smart_objects::SmartObject so_obj(smart_objects::SmartType_Map);
    so_obj[strings::hmi_level] = select_data.GetInteger(0);
    so_obj[strings::ign_off_count] = select_data.GetInteger(1);
    so_obj[strings::time_stamp] = select_data.GetUInteger(2);
    so_obj[strings::app_id] = select_data.GetString(3);
    so_obj[strings::device_id] = select_data.GetString(4);
    so_array_data[i++] = so_obj;
  }
  saved_data = so_array_data;
}

void ResumptionDataDB::UpdateHmiLevel(const std::string& mobile_app_id,
                                      const std::string& device_id,
                                      int32_t hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kUpdateHMILevel)) {
    query.Bind(0, hmi_level);
    query.Bind(1, device_id);
    query.Bind(2, mobile_app_id);
    if (query.Exec()) {
      LOG4CXX_INFO(logger_, "Saved data has application with mobile appID = "
                   <<mobile_app_id<<" and deviceID = "<<device_id
                   <<" has new HMI level = "<<hmi_level);

    }
  }
}

bool ResumptionDataDB::SelectFilesData(const std::string& mobile_app_id,
                                       const std::string& device_id,
                                       smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(count_item, kSelectCountFiles, mobile_app_id, device_id)) {
    return false;
  }
  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain files data");
    return true;
  }
  utils::dbms::SQLQuery select_files(db());
  if (!PrepareSelectQuery(select_files, mobile_app_id, device_id, kSelectFiles)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_files");
    return false;
  }
  saved_app[strings::application_files] = smart_objects::SmartObject(
      smart_objects::SmartType_Array);
  int i = 0;
  while(select_files.Next()) {
    smart_objects::SmartObject array_item(smart_objects::SmartType_Map);
    array_item[strings::file_type] = select_files.GetInteger(0);
    array_item[strings::is_download_complete] = select_files.GetBoolean(1);
    array_item[strings::persistent_file] = select_files.GetBoolean(2);
    array_item[strings::sync_file_name] = select_files.GetString(3);
    saved_app[strings::application_files][i++] = array_item;
  }
  LOG4CXX_INFO(logger_, "File data was restored successfully");
  return true;
}

bool ResumptionDataDB::SelectSubmenuData(const std::string& mobile_app_id,
                                         const std::string& device_id,
                                         smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(count_item, kSelectCountSubMenu, mobile_app_id, device_id)) {
    return false;
  }
  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain submenu data");
    return true;
  }
  utils::dbms::SQLQuery select_sub_menu(db());
  if (!PrepareSelectQuery(select_sub_menu, mobile_app_id, device_id, kSelectSubMenu)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_sub_menu");
    return false;
  }
  saved_app[strings::application_submenus] = smart_objects::SmartObject(
      smart_objects::SmartType_Array);
  int i = 0;
  while(select_sub_menu.Next()) {
    smart_objects::SmartObject array_item(smart_objects::SmartType_Map);
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

bool ResumptionDataDB::SelectCommandData(const std::string& mobile_app_id,
                                         const std::string& device_id,
                                         smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(count_item, kSelectCountCommands, mobile_app_id, device_id)) {
    return false;
  }
  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain commands data");
    return true;
  }

  utils::dbms::SQLQuery select_commands(db());
  if (!PrepareSelectQuery(select_commands, mobile_app_id, device_id, kSelectCommands)) {
    return false;
  }
  saved_app[strings::application_commands] = smart_objects::SmartObject(
        smart_objects::SmartType_Array);
  int64_t command_key = 0;
  int32_t command_idx = -1;
  size_t vr_cmd_idx = 0;
  bool vr_command_exist = false;

  while(select_commands.Next()) {
    if (command_key != select_commands.GetLongInt(0)) {
      ++ command_idx;
      saved_app[strings::application_commands][command_idx] =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      smart_objects::SmartObject& so_item =
          saved_app[strings::application_commands][command_idx];
      so_item[strings::cmd_id] = select_commands.GetInteger(1);
      smart_objects::SmartObject menu_params(smart_objects::SmartType_Map);
      smart_objects::SmartObject cmd_icon(smart_objects::SmartType_Map);
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
        so_item[strings::vr_commands] =
            smart_objects::SmartObject (smart_objects::SmartType_Array);
      } else {
        vr_command_exist = false;
      }
      vr_cmd_idx = 0;
      command_key = select_commands.GetLongInt(0);
    }
    if (vr_command_exist) {
      saved_app[strings::application_commands][command_idx]
               [strings::vr_commands][vr_cmd_idx++] = select_commands.GetString(7);
    }
  }
  LOG4CXX_INFO(logger_, "Commands were restored from DB successfully");
  return true;
}

bool ResumptionDataDB::SelectSubscriptionsData(const std::string& mobile_app_id,
                                           const std::string& device_id,
                                           smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(count_item, kSelectCountSubscriptions, mobile_app_id, device_id)) {
    return false;
  }
  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain subscriptions data");
    return true;
  }
  utils::dbms::SQLQuery select_subscriptions(db());
  if (!PrepareSelectQuery(select_subscriptions, mobile_app_id, device_id,
                          kSelectSubscriptions)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_subscriptions");
    return false;
  }
  saved_app[strings::application_subscribtions] = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  smart_objects::SmartObject application_buttons(smart_objects::SmartType_Array);
  smart_objects::SmartObject application_vehicle_info(smart_objects::SmartType_Array);
  size_t buttons_idx = 0;
  size_t vi_idx = 0;

  while (select_subscriptions.Next()) {
    if (!select_subscriptions.IsNull(0)) {
      application_vehicle_info[vi_idx++] = select_subscriptions.GetInteger(0);
    }
    if (!select_subscriptions.IsNull(1)) {
      application_buttons[buttons_idx++] = select_subscriptions.GetInteger(1);
    }
  }
  if (!application_buttons.empty()) {
    saved_app[strings::application_subscribtions][strings::application_buttons] =
        application_buttons;
  }

  if (!application_vehicle_info.empty()) {
    saved_app[strings::application_subscribtions][strings::application_vehicle_info] =
        application_vehicle_info;
  }
  LOG4CXX_INFO(logger_, "Subscriptions were restored from DB successfully");
  return true;
}

bool ResumptionDataDB::SelectChoiceSetData(const std::string& mobile_app_id,
                                           const std::string& device_id,
                                           smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(count_item, kSelectCountChoiceSet, mobile_app_id, device_id)) {
    return false;
  }
  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain choice set data");
    return true;
  }
  utils::dbms::SQLQuery select_choice_set(db());
  if (!PrepareSelectQuery(select_choice_set, mobile_app_id, device_id,
                          kSelectChoiceSets)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_choice_set");
    return false;
  }

  saved_app[strings::application_choice_sets] = smart_objects::SmartObject(
      smart_objects::SmartType_Array);
  int64_t application_choice_set_key = 0;
  int64_t choice_key = 0;
  int32_t choice_set_idx = -1;
  int32_t choice_idx = -1;
  size_t vr_cmd_idx = 0;

  while(select_choice_set.Next()) {
    if (application_choice_set_key != select_choice_set.GetLongInt(0)) {
      ++choice_set_idx;
      saved_app[strings::application_choice_sets][choice_set_idx] =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      smart_objects::SmartObject& choice_set_item =
          saved_app[strings::application_choice_sets][choice_set_idx];
      choice_set_item[strings::grammar_id] = select_choice_set.GetInteger(1);
      choice_set_item[strings::interaction_choice_set_id] = select_choice_set.GetInteger(2);
      saved_app[strings::application_choice_sets][choice_set_idx][strings::choice_set] =
          smart_objects::SmartObject(smart_objects::SmartType_Array);
      application_choice_set_key = select_choice_set.GetLongInt(0);
      choice_idx = -1;
    }
    if (choice_key != select_choice_set.GetLongInt(3)) {
      ++choice_idx;
      choice_key = select_choice_set.GetLongInt(3);

      saved_app[strings::application_choice_sets][choice_set_idx][strings::choice_set][choice_idx] =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      smart_objects::SmartObject& choice_item =
          saved_app[strings::application_choice_sets][choice_set_idx][strings::choice_set][choice_idx];
      choice_item[strings::choice_id] = select_choice_set.GetUInteger(4);
      choice_item[strings::menu_name] = select_choice_set.GetString(5);
      if (!(select_choice_set.IsNull(6))) {
        choice_item[strings::secondary_text] = select_choice_set.GetString(6);
      }
      if (!(select_choice_set.IsNull(7))) {
        choice_item[strings::tertiary_text] = select_choice_set.GetString(7);
      }
      if (!(select_choice_set.IsNull(8))) {
        smart_objects::SmartObject image (smart_objects::SmartType_Map);
        if (!SelectImageData(select_choice_set.GetLongInt(8), image)) {
          return false;
        }
        choice_item[strings::image] = image;
      }
      if (!(select_choice_set.IsNull(9))) {
        smart_objects::SmartObject secondary_image (smart_objects::SmartType_Map);
        if (!SelectImageData(select_choice_set.GetLongInt(9), secondary_image)) {
          return false;
        }
        choice_item[strings::secondary_image] = secondary_image;
      }
      choice_item[strings::vr_commands] =
          smart_objects::SmartObject (smart_objects::SmartType_Array);
      vr_cmd_idx = 0;
    }
    saved_app[strings::application_choice_sets][choice_set_idx]
             [strings::choice_set][choice_idx][strings::vr_commands][vr_cmd_idx++] =
                 select_choice_set.GetString(10);
  }

  LOG4CXX_INFO(logger_, "Choice sets were restored from DB successfully");
  return true;
}

bool ResumptionDataDB::SelectGlobalPropertiesData(const std::string& mobile_app_id,
                                                  const std::string& device_id,
                                                  smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  uint32_t count_item = 0;
  if (!SelectCountFromArray(count_item, kSelectCountGlobalProperties,
                            mobile_app_id, device_id)) {
    return false;
  }
  if (0 == count_item) {
    LOG4CXX_INFO(logger_, "Application does not contain global properties data");
    return true;
  }
  utils::dbms::SQLQuery select_globalproperties(db());
  if (!PrepareSelectQuery(select_globalproperties, mobile_app_id, device_id,
                          kSelectGlobalProperties)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_globalproperties");
    return false;
  }
  saved_app[strings::application_global_properties] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  smart_objects::SmartObject& global_properties =
      saved_app[strings::application_global_properties];
  smart_objects::SmartObject keyboard_properties(smart_objects::SmartType_Map);
  smart_objects::SmartObject help_prompt(smart_objects::SmartType_Array);
  smart_objects::SmartObject timeout_prompt(smart_objects::SmartType_Array);
  size_t help_prompt_idx = 0;
  size_t timeout_prompt_idx = 0;
  int64_t global_properties_key = 0;

  while (select_globalproperties.Next()) {
     if (global_properties_key != select_globalproperties.GetLongInt(0)) {
       global_properties_key = select_globalproperties.GetLongInt(0);
       if (!select_globalproperties.IsNull(1)) {
         global_properties[strings::vr_help_title] = select_globalproperties.GetString(1);
       }
       if (!select_globalproperties.IsNull(2)) {
         global_properties[strings::menu_title] = select_globalproperties.GetString(2);
       }
       if (!select_globalproperties.IsNull(3)) {
         smart_objects::SmartObject image (smart_objects::SmartType_Map);
         if (!SelectImageData(select_globalproperties.GetLongInt(3), image)) {
           return false;
         }
         global_properties[strings::menu_icon] = image;
       }
       if (!select_globalproperties.IsNull(4)) {
         keyboard_properties[strings::language] = select_globalproperties.GetInteger(4);
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
       smart_objects::SmartObject tts_chunk(smart_objects::SmartType_Map);
       if (!SelectTTSChunkData(select_globalproperties.GetLongInt(8), tts_chunk)) {
         return false;
       }
       help_prompt[help_prompt_idx++] = tts_chunk;
     }
     if (!select_globalproperties.IsNull(9)) {
       smart_objects::SmartObject tts_chunk(smart_objects::SmartType_Map);
       if (!SelectTTSChunkData(select_globalproperties.GetLongInt(9), tts_chunk)) {
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
    if (!SelectCharactersData(global_properties_key,
                         global_properties[strings::keyboard_properties])){
      return false;
    }
  }
  if (!SelectVrHelpItemsData(global_properties_key, global_properties)) {
    return false;
  }
  return true;
}

bool ResumptionDataDB::SelectVrHelpItemsData(int64_t global_properties_key,
                                    smart_objects::SmartObject& global_properties) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
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
  global_properties[strings::vr_help] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  smart_objects::SmartObject& vr_help_items = global_properties[strings::vr_help];
  select_vrhelp_item.Bind(0, global_properties_key);
  size_t vr_help_item_idx = 0;

  while (select_vrhelp_item.Next()) {
    smart_objects::SmartObject item(smart_objects::SmartType_Map);
    item[strings::text] = select_vrhelp_item.GetString(0);
    item[strings::position] = select_vrhelp_item.GetInteger(1);
    if (!select_vrhelp_item.IsNull(2) && !select_vrhelp_item.IsNull(3)) {
      smart_objects::SmartObject image(smart_objects::SmartType_Map);
      image[strings::image_type] = select_vrhelp_item.GetInteger(2);
      image[strings::value] = select_vrhelp_item.GetString(3);
      item[strings::image] = image;
    }
    vr_help_items[vr_help_item_idx++] = item;
  }
  LOG4CXX_INFO(logger_, "VR Help items were restored successfully");
  return true;
}

bool ResumptionDataDB::SelectCharactersData(int64_t global_properties_key,
                                            smart_objects::SmartObject& keyboard_properties) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
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
    LOG4CXX_INFO(logger_, "Keyboard properties doesn't contain table limited character list");
    return true;
  }
  utils::dbms::SQLQuery select_characters(db());
  if (!select_characters.Prepare(kSelectCharacter)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_characters query");
    return false;
  }

  keyboard_properties[strings::limited_character_list] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
  smart_objects::SmartObject& characters = keyboard_properties[strings::limited_character_list];
  select_characters.Bind(0, global_properties_key);
  size_t characters_idx = 0;
  while (select_characters.Next()) {
    characters[characters_idx++] = select_characters.GetString(0);
  }
  return true;
}

bool ResumptionDataDB::SelectImageData(int64_t image_key, smart_objects::SmartObject& image) {
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
  image[strings::image_type] = select_image.GetInteger(0);
  image[strings::value] = select_image.GetString(1);
  return true;
}

bool ResumptionDataDB::SelectTTSChunkData(int64_t tts_chunk_key, smart_objects::SmartObject& tts_chunk) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery select_tts_chunk(db());

  if(!select_tts_chunk.Prepare(kSelectTTSChunk)) {
    LOG4CXX_WARN(logger_, "Problem with verification select_tts_chunk query");
    return false;
  }
  select_tts_chunk.Bind(0, tts_chunk_key);
  if (!select_tts_chunk.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution select_tts_chunk query");
    return false;
  }
  tts_chunk[strings::text] = select_tts_chunk.GetString(0);
  tts_chunk[strings::type] = select_tts_chunk.GetInteger(1);
  return true;
}

bool ResumptionDataDB::SelectDataFromAppTable(const std::string& mobile_app_id,
                                              const std::string& device_id,
                                              smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery query(db());
  if(!query.Prepare(kSelectAppTable)) {
    LOG4CXX_WARN(logger_, "Problem with verification kSelectAppTable query");
    return false;
  }
  query.Bind(0, mobile_app_id);
  query.Bind(1, device_id);
  if(!query.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution kSelectAppTable query");
    return false;
  }
  saved_app[strings::app_id] = query.GetString(0);
  saved_app[strings::connection_key] = query.GetUInteger(1);
  saved_app[strings::grammar_id] = query.GetUInteger(2);
  saved_app[strings::hash_id] = query.GetString(3);
  saved_app[strings::hmi_app_id] = query.GetUInteger(4);
  saved_app[strings::hmi_level] = query.GetInteger(5);
  saved_app[strings::ign_off_count] = query.GetInteger(6);
  saved_app[strings::suspend_count] = query.GetInteger(7);
  saved_app[strings::time_stamp] = query.GetUInteger(8);
  saved_app[strings::device_id] = query.GetString(9);
  saved_app[strings::is_media_application] = query.GetBoolean(10);

  LOG4CXX_INFO(logger_, "Data from application table was restored successfully");
  return true;
}


bool ResumptionDataDB::SelectCountFromArray(uint32_t& count_item, const std::string& text_query,
                                            const std::string& mobile_app_id,
                                            const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  if(!query.Prepare(text_query)) {
    LOG4CXX_WARN(logger_, "Problem with verification query");
    return false;
  }
  query.Bind(0, mobile_app_id);
  query.Bind(1, device_id);
  if(!query.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution query");
    return false;
  }
  count_item = query.GetInteger(0);
  LOG4CXX_INFO(logger_, "count_item="<<count_item);
  return true;
}

bool ResumptionDataDB::DeleteSavedApplication(const std::string& mobile_app_id,
                                              const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  db_->BeginTransaction();
  if (!DeleteSavedFiles(mobile_app_id, device_id)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!DeleteSavedSubMenu(mobile_app_id, device_id)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!DeleteSavedSubscriptions(mobile_app_id, device_id)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!DeleteSavedCommands(mobile_app_id, device_id)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!DeleteSavedChoiceSet(mobile_app_id, device_id)) {
    db_->RollbackTransaction();
    return false;
  }
  if (!DeleteSavedGlobalProperties(mobile_app_id, device_id)) {
    db_->RollbackTransaction();
    return false;
  }
  db_->CommitTransaction();
  return true;
}



bool ResumptionDataDB::DeleteSavedFiles(const std::string& mobile_app_id,
                                        const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!ExecQueryToDeleteData(mobile_app_id, device_id, kDeleteFile)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from file.");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteApplicationFilesArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from applicationFilesArray.");
    return false;
  }
  return true;
}

bool ResumptionDataDB::DeleteSavedSubMenu(const std::string& mobile_app_id,
                                          const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecQueryToDeleteData(mobile_app_id, device_id, kDeleteSubMenu)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from subMenu.");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteApplicationSubMenuArray)) {
    LOG4CXX_WARN(logger_, "Incorrect delete from applicationSubMenuArray.");
    return false;
  }
  return true;
}

bool ResumptionDataDB::DeleteSavedSubscriptions(
    const std::string& mobile_app_id, const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteApplicationSubscribtionsArray)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete from applicationSubscribtionsArray.");
    return false;
  }
  return true;
}

bool ResumptionDataDB::DeleteSavedCommands(const std::string& mobile_app_id,
                                           const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteImageFromCommands)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete image from commands.");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteVrCommands)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete vrcommands from commands.");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteCommands)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete commands.");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteApplicationCommandsArray)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete applicationCommandsArray.");
    return false;
  }

  return true;
}

bool ResumptionDataDB::DeleteSavedChoiceSet(const std::string& mobile_app_id,
                                            const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecUnionQueryToDeleteData(
      mobile_app_id, device_id, kDeleteImageFromChoiceSet)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete image from choice set");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteVrCommandsFromChoiceSet)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete vrCommands from choice set");
    return false;
  }

  if (!ExecQueryToDeleteData(mobile_app_id, device_id, kDeleteChoice)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete choiceSet");
    return false;
  }

  if (!ExecQueryToDeleteData(mobile_app_id, device_id, kDeleteChoiceArray)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete from choiceArray");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteApplicationChoiceSet)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete applicationChoiceSet");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteApplicationChoiceSetArray)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete from ApplicationChoiceSetArray");
    return false;
  }

  return true;
}

bool ResumptionDataDB::DeleteSavedGlobalProperties(const std::string& mobile_app_id,
                                                   const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!ExecUnionQueryToDeleteData(
      mobile_app_id, device_id, kDeleteImageFromGlobalProperties)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete image from globalProperties");
    return false;
  }

  if (!ExecQueryToDeleteData(mobile_app_id, device_id, kDeletevrHelpItem)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete vrHelpItem");
    return false;
  }

  if (!ExecQueryToDeleteData(mobile_app_id, device_id, kDeletevrHelpItemArray)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete vrHelpItemArray");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteTableLimitedCharacterList)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete from tableLimitedCharacterList");
    return false;
  }

  if (!ExecQueryToDeleteData(mobile_app_id, device_id, kDeleteCharacterArray)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete from characterArray");
    return false;
  }

  if (!ExecUnionQueryToDeleteData(mobile_app_id, device_id, kDeleteTTSChunk)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete from TTSChunk");
    return false;
  }

  if (!ExecQueryToDeleteData(
      mobile_app_id, device_id, kDeleteHelpTimeoutPromptArray)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete from HelpTimeoutPromptArray");
    return false;
  }

  if (!ExecQueryToDeleteData(mobile_app_id, device_id, kDeleteGlobalProperties)) {
    LOG4CXX_WARN(logger_,
                 "Incorrect delete from GlobalProperties");
    return false;
  }

  return true;
}

bool ResumptionDataDB::ExecQueryToDeleteData(const std::string& mobile_app_id,
                                             const std::string& device_id,
                                             const std::string& text_query) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  bool result =  query.Prepare(text_query);
  if (result) {
    query.Bind(0, mobile_app_id);
    query.Bind(1, device_id);
    result = query.Exec();
  }
  return result;
}

bool ResumptionDataDB::ExecUnionQueryToDeleteData(const std::string& mobile_app_id,
                                                  const std::string& device_id,
                                                  const std::string& text_query) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  bool result =  query.Prepare(text_query);
  if (result) {
    query.Bind(0, mobile_app_id);
    query.Bind(1, device_id);
    query.Bind(2, mobile_app_id);
    query.Bind(3, device_id);
    result = query.Exec();
  }
  return result;
}

bool ResumptionDataDB::ExecSelectPrimaryKeyFromApplication(
    const std::string& mobile_app_id, const std::string& device_id,
    int64_t& primary_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery query(db());
  bool result =  query.Prepare(kSelectPrimaryKeyFromApplication);
  if (result) {
    query.Bind(0, mobile_app_id);
    query.Bind(1, device_id);
    result = query.Exec();
    if (result) {
      primary_key = query.GetUInteger(0);
    }
  }
  return result;
}

bool ResumptionDataDB::ExecInsertImage(int64_t& image_primary_key,
                     const smart_objects::SmartObject& image) {
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
      LOG4CXX_WARN(logger_, "Problem with preparing or execution "
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
      LOG4CXX_WARN(logger_, "Problem with preparing or execution "
                   "query for insert image to image table");
    }
  }
  return result;
}

bool ResumptionDataDB::ExecInsertChoice(int64_t choice_set_key,
                                        const smart_objects::SmartObject& choice_array) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery insert_choice(db());

  if (!insert_choice.Prepare(kInsertChoice)) {
    LOG4CXX_WARN(logger_, "Incorrect preparation insert_choice query");
    return false;
  }

  int64_t image_primary_key = 0;
  int64_t choice_primary_key = 0;

  for (size_t i = 0; i < choice_array.length(); ++i) {
    insert_choice.Bind(0, (choice_array[i][strings::choice_id]).asInt());
    insert_choice.Bind(1, (choice_array[i][strings::menu_name]).asString());

    CustomBind(strings::secondary_text, choice_array[i], insert_choice, 2);
    CustomBind(strings::tertiary_text, choice_array[i], insert_choice, 3);

    if (choice_array[i].keyExists(strings::image)) {
      if (!ExecInsertImage(image_primary_key, choice_array[i][strings::image])) {
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

    if ((!ExecInsertVrCommands(
        choice_primary_key, choice_array[i][strings::vr_commands],
        kVRCommandFromChoice)) || !insert_choice.Reset()) {

      LOG4CXX_WARN(logger_, "problemm with add vr commands to choice");
      return false;
    }

    if (!ExecInsertDataToArray(
        choice_set_key, choice_primary_key,  kInsertChoiceArray)) {
      LOG4CXX_INFO(logger_, "Problem with insertion data to choiceArray table");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Choice data were saved to DB successfully");
  return true;
}


bool ResumptionDataDB::ExecInsertVrCommands(
    const int64_t primary_key, const smart_objects::SmartObject& vr_commands_array,
    AccessoryVRCommand value) {
  LOG4CXX_AUTO_TRACE(logger_);
  utils::dbms::SQLQuery insert_vr_command(db());

  if(!insert_vr_command.Prepare(kInsertVrCommand)) {
    LOG4CXX_WARN(logger_, "Incorrect preparation insert_vr_command query");
    return false;
  }
  for (size_t i = 0; i < vr_commands_array.length(); ++i) {
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
    const std::string& text_query) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool result;
  utils::dbms::SQLQuery query_insert_array(db());
  result = query_insert_array.Prepare(text_query);
  if(result) {
    query_insert_array.Bind(0, first_primary_key);
    query_insert_array.Bind(1, second_primary_key);
    result = query_insert_array.Exec();
  }
  return result;
}

bool ResumptionDataDB::SaveApplicationToDB(
      app_mngr::ApplicationConstSharedPtr application,
      const std::string& mobile_app_id,
      const std::string& device_id,
      bool application_exist) {

  LOG4CXX_AUTO_TRACE(logger_);
  int64_t application_primary_key = 0;
  int64_t global_properties_key = 0;
  db_->BeginTransaction();
  if (!InsertGlobalPropertiesData(application, global_properties_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert globalProperties data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertApplicationData(application, mobile_app_id, device_id,
                             application_primary_key, global_properties_key,
                             application_exist)) {
    LOG4CXX_WARN(logger_, "Incorrect insert application data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertFilesData(application, application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert file data to DB.");
    db_->RollbackTransaction();
    return false;
  }

  if (!InsertSubMenuData(application, application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert submenu data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertCommandsData(application, application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert commands data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertSubscriptionsData(application, application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert subscribtions data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  if (!InsertChoiceSetData(application, application_primary_key)) {
    LOG4CXX_WARN(logger_, "Incorrect insert choiceset data to DB.");
    db_->RollbackTransaction();
    return false;
  }
  db_->CommitTransaction();
  return true;
}

bool ResumptionDataDB::InsertFilesData(
    app_mngr::ApplicationConstSharedPtr application,
    int64_t application_primary_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  smart_objects::SmartObject files_array(GetApplicationFiles(application));
  const size_t length_files_array = files_array.length();
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

  for (size_t i = 0; i < length_files_array; ++i) {

    query_insert_file.Bind(
        0, (files_array[i][strings::file_type]).asInt());
    query_insert_file.Bind(
        1, (files_array[i][strings::is_download_complete]).asBool());
    query_insert_file.Bind(
        2, (files_array[i][strings::persistent_file]).asBool());
    query_insert_file.Bind(
        3, (files_array[i][strings::sync_file_name]).asString());

    if (!query_insert_file.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion of files data");
      return false;
    }

    if ((!ExecInsertDataToArray(
        application_primary_key, query_insert_file.LastInsertId(),
        kInsertToApplicationFilesArray)) || !query_insert_file.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion to application files array");
      return false;
    }
  }

  LOG4CXX_INFO(logger_, "Files data were inserted successfully to DB");
  return true;
}

bool ResumptionDataDB::InsertSubMenuData(
    app_mngr::ApplicationConstSharedPtr application,
    int64_t application_primary_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  smart_objects::SmartObject submenu_array(GetApplicationSubMenus(application));
  const size_t length_submenu_array = submenu_array.length();
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

  for (size_t i = 0; i < length_submenu_array; ++i) {
    query_insert_submenu.Bind(
        0, (submenu_array[i][strings::menu_id]).asInt());
    query_insert_submenu.Bind(
        1, (submenu_array[i][strings::menu_name]).asString());
    CustomBind(strings::position, submenu_array[i], query_insert_submenu, 2);

    if (!query_insert_submenu.Exec()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion of submenu data");
      return false;
    }

    if ((!ExecInsertDataToArray(
        application_primary_key, query_insert_submenu.LastInsertId(),
        kInsertToApplicationSubMenuArray)) || !query_insert_submenu.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion to application submenu array");
      return false;
    }
  }

  LOG4CXX_INFO(logger_, "Data about submenu were inserted successfully to DB");
  return true;
}

bool ResumptionDataDB::InsertCommandsData(
    app_mngr::ApplicationConstSharedPtr application,
    int64_t application_primary_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  smart_objects::SmartObject command_array(GetApplicationCommands(application));
  const size_t length_command_array = command_array.length();
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
  for (size_t i = 0; i < length_command_array; ++i) {
    query_insert_command.Bind(0, command_array[i][strings::cmd_id].asInt());
    if (command_array[i].keyExists(strings::cmd_icon)) {
      if (!ExecInsertImage(image_primary_key,
                          command_array[i][strings::cmd_icon])) {
        LOG4CXX_WARN(logger_,
                     "Problem with insert command image to DB");
        return false;
      }
      query_insert_command.Bind(1, image_primary_key);
    } else {
      query_insert_command.Bind(1);
    }

    if (command_array[i].keyExists(strings::menu_params)) {
      smart_objects::SmartObject& menu_params =
          command_array[i][strings::menu_params];
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
    if (command_array[i].keyExists(strings::vr_commands)) {
      if (!ExecInsertVrCommands(command_primary_key,
                                command_array[i][strings::vr_commands],
                                kVRCommandFromCommand)) {
        return false;
      }
    }
    if ((!ExecInsertDataToArray(
        application_primary_key, command_primary_key,
        kInsertApplicationCommandArray)) || !query_insert_command.Reset()) {
      LOG4CXX_WARN(logger_, "Incorrect insertion to application commands array");
      return false;
    }
  }
  return true;
}

bool ResumptionDataDB::InsertSubscriptionsData(
    app_mngr::ApplicationConstSharedPtr application,
    int64_t application_primary_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  smart_objects::SmartObject subscriptions(
      GetApplicationSubscriptions(application));
  if(subscriptions.empty()) {
    LOG4CXX_INFO(logger_, "Application doesn't contain subscriptions");
    return true;
  }
  smart_objects::SmartObject& btn_sub = subscriptions[strings::application_buttons];
  smart_objects::SmartObject& vi_sub = subscriptions[strings::application_vehicle_info];
  size_t btn_sub_length  = btn_sub.length();
  size_t vi_sub_length = vi_sub.length();
  size_t max_length = (btn_sub_length > vi_sub_length)?btn_sub_length:vi_sub_length;

  utils::dbms::SQLQuery insert_subscriptions(db());
  if (!insert_subscriptions.Prepare(kInsertSubscriptions)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification queries for insertion subscriptions");
    return false;
  }

  for(size_t i = 0; i < max_length; ++i) {
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
      LOG4CXX_WARN(logger_,
                   "Incorrect insertion of buttons to subscriptions");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Subscriptions data were saved successfully");
  return true;
}

bool ResumptionDataDB::InsertChoiceSetData(
    app_mngr::ApplicationConstSharedPtr application,
    int64_t application_primary_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  smart_objects::SmartObject choiceset_array (
      GetApplicationInteractionChoiseSets(application));
  if (choiceset_array.empty()) {
    LOG4CXX_INFO(logger_, "Application doesn't contain choiceSet");
    return true;
  }
  int64_t choice_set_key = 0;

  for (size_t i = 0; i < choiceset_array.length(); ++i) {

    if (!ExecInsertApplicationChoiceSet(choice_set_key, choiceset_array[i])) {
      return false;
    }

    if (!ExecInsertChoice(choice_set_key,
                         choiceset_array[i][strings::choice_set])) {
      return false;
    }

    if(!ExecInsertDataToArray(choice_set_key, application_primary_key,
                              kInsertApplicationChoiceSetArray)) {
      LOG4CXX_WARN(logger_, "Problem with insertion data to"
                   " applicationChoiceSetArray table");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Choice set data were saved to DB successfully");
  return true;
}

bool ResumptionDataDB::ExecInsertApplicationChoiceSet(
    int64_t& choice_set_primary_key, const smart_objects::SmartObject& choiceset) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;

  utils::dbms::SQLQuery insert_application_choice_set(db());
  if (!insert_application_choice_set.Prepare(kInsertApplicationChoiceSet)) {
    LOG4CXX_WARN(logger_, "Problem with preparation insert "
                 "application choice set query");
    return false;
  }
  insert_application_choice_set.Bind(
      0, static_cast<int64_t>(choiceset[strings::grammar_id].asUInt()));
  insert_application_choice_set.Bind(
      1, choiceset[strings::interaction_choice_set_id].asInt());

  if (!insert_application_choice_set.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution insert application choice set query");
    return false;
  }
  choice_set_primary_key = insert_application_choice_set.LastInsertId();
  LOG4CXX_INFO(logger_, "Application choice data were saved successfully");
  return true;
}

bool ResumptionDataDB::InsertGlobalPropertiesData(
    app_mngr::ApplicationConstSharedPtr application,
    int64_t& global_properties_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  smart_objects::SmartObject global_properties(
      GetApplicationGlobalProperties(application));
  smart_objects::SmartMap::iterator it_begin = global_properties.map_begin();
  smart_objects::SmartMap::iterator it_end = global_properties.map_end();
  bool data_exists = false;
  while(it_begin != it_end) {
    if (smart_objects::SmartType::SmartType_Null
        != ((it_begin->second).getType())) {
      LOG4CXX_INFO(logger_, "Global properties contains - "<<it_begin->first);
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
    LOG4CXX_WARN(logger_, "Problem with preparation query "
                 "insert_global_properties");
    return false;
  }

  CustomBind(strings::vr_help_title, global_properties, insert_global_properties, 0);
  CustomBind(strings::menu_title, global_properties, insert_global_properties, 1);

  if (smart_objects::SmartType::SmartType_Null ==
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

  if (smart_objects::SmartType::SmartType_Null ==
      global_properties[strings::keyboard_properties].getType()) {
    insert_global_properties.Bind(3);
    insert_global_properties.Bind(4);
    insert_global_properties.Bind(5);
    insert_global_properties.Bind(6);
  } else {
    smart_objects::SmartObject& kb_prop = global_properties[strings::keyboard_properties];

    CustomBind(strings::language, kb_prop, insert_global_properties, 3);
    CustomBind(hmi_request::keyboard_layout, kb_prop, insert_global_properties, 4);
    CustomBind(strings::key_press_mode, kb_prop, insert_global_properties, 5);
    CustomBind(strings::auto_complete_text, kb_prop, insert_global_properties, 6);
  }
  if (!insert_global_properties.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with insert data to global properties table");
    return false;
  }

  global_properties_key = insert_global_properties.LastInsertId();
  if ((smart_objects::SmartType::SmartType_Null !=
      global_properties[strings::keyboard_properties].getType()) &&
      (global_properties[strings::keyboard_properties].keyExists(
          strings::limited_character_list))) {
    if(!ExecInsertLimitedCharacters(
        global_properties_key,
        global_properties[strings::keyboard_properties][strings::limited_character_list])) {
      LOG4CXX_WARN(logger_, "Problem with insert data to limited_character table");
      return false;
    }
  }

  if (smart_objects::SmartType::SmartType_Null !=
      global_properties[strings::vr_help].getType()) {
    if(!ExecInsertVRHelpItem(global_properties_key,
                             global_properties[strings::vr_help])) {
      LOG4CXX_WARN(logger_, "Problem with insert data to vrHelpItem table");
      return false;
    }
  }

  if (!ExecInsertHelpTimeoutArray(global_properties, global_properties_key)) {
    LOG4CXX_WARN(logger_, "Problem with insert data to HelpTimeoutPromptArray table");
    return false;
  }

  LOG4CXX_INFO(logger_, "Global properties data were saved successfully");
  return true;
}


bool ResumptionDataDB::ExecInsertHelpTimeoutArray(
    const smart_objects::SmartObject& global_properties, int64_t global_properties_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  size_t timeout_prompt_length = 0;
  size_t help_prompt_length = 0;

  if (smart_objects::SmartType::SmartType_Null !=
      global_properties[strings::help_prompt].getType()) {
    help_prompt_length = global_properties[strings::help_prompt].length();
  }

  if (smart_objects::SmartType::SmartType_Null !=
      global_properties[strings::timeout_prompt].getType()) {
    timeout_prompt_length = global_properties[strings::timeout_prompt].length();
  }
  if (0 == timeout_prompt_length && 0 == help_prompt_length) {
    LOG4CXX_INFO(logger_, "Application doesn't HelpPrompt and timoutPrompt data");
    return true;
  }

  utils::dbms::SQLQuery insert_help_prompt_array(db());

  if (!insert_help_prompt_array.Prepare(kInsertHelpTimeoutPromptArray)) {
    LOG4CXX_WARN(logger_, "Problem with verification query insert_help_prompt_array");
    return false;
  }
  int64_t tts_chunk_key = 0;

  size_t max_length = (timeout_prompt_length > help_prompt_length) ?
      timeout_prompt_length:help_prompt_length;

  for (size_t i = 0; i < max_length; ++i) {
    insert_help_prompt_array.Bind(0, global_properties_key);
    if (i < timeout_prompt_length) {
      if (!ExecInsertTTSChunks(global_properties[strings::timeout_prompt][i],
                               tts_chunk_key)) {
        LOG4CXX_WARN(logger_, "Problem with insertion timeoutPrompt's ttsChunk");
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
      LOG4CXX_WARN(logger_, "Problem with execution or resetting insert_help_prompt_array query");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Data were saved to helpTimeoutPromptArray table");
  return true;
}


bool ResumptionDataDB::ExecInsertTTSChunks(const smart_objects::SmartObject& tts_chunk,
                                           int64_t& tts_chunk_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery insert_tts_chunk(db());
  if (!insert_tts_chunk.Prepare(kInsertTTSChunk)) {
    LOG4CXX_WARN(logger_, "Problem with verification insert_tts_chunk query");
    return false;
  }
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
    int64_t global_properties_key, const smart_objects::SmartObject& characters_array) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery insert_characters(db());
  if (!insert_characters.Prepare(kInsertTableLimitedCharacter)) {
    LOG4CXX_WARN(logger_, "Problem with preparation query "
                 "insert_characters");
    return false;
  }
  for (size_t i = 0; i < characters_array.length(); ++i) {
    insert_characters.Bind(0, characters_array[i].asString());

    if (!insert_characters.Exec()) {
      LOG4CXX_WARN(logger_, "Problem with insert data to limited_character table");
      return false;
    }
    if ((!ExecInsertDataToArray(global_properties_key, insert_characters.LastInsertId(),
                               kInsertCharacterArray)) || (!insert_characters.Reset())) {
      LOG4CXX_WARN(logger_, "Problem with insert data to characterArray table");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Data were saved successfully to limited_character table");
  return true;
}

bool ResumptionDataDB::ExecInsertVRHelpItem(int64_t global_properties_key,
                                            const smart_objects::SmartObject& vrhelp_array) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery insert_vrhelp_item(db());
  if (!insert_vrhelp_item.Prepare(kInsertVRHelpItem)) {
    LOG4CXX_WARN(logger_, "Problem with preparation query insert_vrhelp_item");
    return false;
  }
  int64_t image_primary_key = 0;
  for (size_t i = 0; i < vrhelp_array.length(); ++i) {
    insert_vrhelp_item.Bind(0, vrhelp_array[i][strings::text].asString());
    insert_vrhelp_item.Bind(1, vrhelp_array[i][strings::position].asInt());
    if (vrhelp_array[i].keyExists(strings::image)) {
      if (!ExecInsertImage(image_primary_key, vrhelp_array[i][strings::image])) {
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

    if ((!ExecInsertDataToArray(global_properties_key, insert_vrhelp_item.LastInsertId(),
                                kInsertVRHelpItemArray)) || (!insert_vrhelp_item.Reset())) {
      LOG4CXX_WARN(logger_, "Problem with insert data to vrHelpItemArray table");
      return false;
    }
  }
  LOG4CXX_INFO(logger_, "Data were saved successfully to vrHelpItem array table");
  return true;
}

bool ResumptionDataDB::InsertApplicationData(app_mngr::ApplicationConstSharedPtr application,
                                             const std::string& mobile_app_id,
                                             const std::string& device_id,
                                             int64_t& application_primary_key,
                                             int64_t global_properties_key,
                                             bool application_exist) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;
  utils::dbms::SQLQuery query(db());

  const std::string hash = application->curHash();
  const int64_t grammar_id = application->get_grammar_id();
  const int64_t time_stamp = static_cast<int64_t>(time(NULL));
  const int64_t connection_key = application->app_id();
  const int64_t hmi_app_id = application->hmi_app_id();
  const mobile_apis::HMILevel::eType hmi_level = application->hmi_level();
  bool is_media_application = application->IsAudioApplication();

  if (application_exist) {
    if(!query.Prepare(kUpdateApplication)){
      LOG4CXX_WARN(logger_, "Problem with verification query "
                   "for update table application");
      return false;
    }
  } else {
    if(!query.Prepare(kInsertApplication)){
      LOG4CXX_WARN(logger_, "Problem with verification query "
                   "for insert to table application");
      return false;
    }
  }
  query.Bind(0, connection_key);
  query.Bind(1, grammar_id);
  query.Bind(2, hash);
  query.Bind(3, hmi_app_id);
  query.Bind(4, static_cast<int32_t>(hmi_level));
  query.Bind(5, 0);
  query.Bind(6, 0);
  query.Bind(7, time_stamp);
  query.Bind(8, global_properties_key);
  query.Bind(9, is_media_application);
  query.Bind(10, mobile_app_id);
  query.Bind(11, device_id);

  if(!query.Exec()) {
    LOG4CXX_WARN(logger_, "Problem with execution query");
    return false;
  }

  if (application_exist) {
    if(!ExecSelectPrimaryKeyFromApplication(mobile_app_id, device_id,
                                            application_primary_key)) {
      LOG4CXX_INFO(logger_, "Problem with receiving application primary key");
      return false;
    }
  } else {
    application_primary_key = query.LastInsertId();
  }

  LOG4CXX_INFO(logger_, "Application data were saved successfully");
  return true;
}

void ResumptionDataDB::CustomBind(const std::string& key,
                                  const smart_objects::SmartObject& so,
                                  utils::dbms::SQLQuery& query, const int pos) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (so.keyExists(key) && smart_objects::SmartType::SmartType_Null != so[key].getType()) {
    switch(so[key].getType()) {
      case smart_objects::SmartType::SmartType_Integer:{
        query.Bind(pos, so[key].asInt());
        break;
      }
      case smart_objects::SmartType::SmartType_String:{
        query.Bind(pos, so[key].asString());
        break;
      }
      default:{
        LOG4CXX_WARN(logger_, "Incorrect type");
        break;
      }
    }
  } else {
    query.Bind(pos);
  }
}

bool ResumptionDataDB::PrepareSelectQuery(utils::dbms::SQLQuery& query,
                                          const std::string& mobile_app_id,
                                          const std::string& device_id,
                                          const std::string& text_query) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!query.Prepare(text_query)) {
    LOG4CXX_WARN(logger_, "Problem with verification query");
    return false;
  }
  query.Bind(0, mobile_app_id);
  query.Bind(1, device_id);
  return true;
}

void ResumptionDataDB::UpdateDataOnAwake() {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kUpdateIgnOffCount)) {
    if (query.Exec()) {
      LOG4CXX_INFO(logger_, "Values of ignition off counts were updated successfully");
    }
  }
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



}  // namespace resumption

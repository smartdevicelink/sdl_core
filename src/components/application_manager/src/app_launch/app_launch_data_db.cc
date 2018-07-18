/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "application_manager/app_launch/app_launch_data_db.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/app_launch/app_launch_sql_queries.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/message_helper.h"


namespace app_launch {
CREATE_LOGGERPTR_GLOBAL(logger_, "AppLaunch")

AppLaunchDataDB::AppLaunchDataDB(const AppLaunchSettings& settings,
                                 DbStorage db_storage)
    : AppLaunchDataImpl(settings) {
  if (db_storage == In_File_Storage) {
    db_.reset(new utils::dbms::SQLDatabase(kDatabaseName));
#ifndef __QNX__
    std::string path = settings_.app_storage_folder();
    if (!path.empty()) {
      db()->set_path(path + "/");
    }
  } else if (db_storage == In_Memory_Storage) {
    db_.reset(new utils::dbms::SQLDatabase());
#endif  // __QNX__
    DCHECK(db_.get());
  } else {
    LOG4CXX_AUTO_TRACE(logger_);
    LOG4CXX_ERROR(logger_, "Get not existed type of database storage");
  }

  // Connect to resumption DB
  init_successeful_ = Init();
}

AppLaunchDataDB::~AppLaunchDataDB() {
  db()->Close();
}

bool AppLaunchDataDB::Init() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!db()->Open()) {
    LOG4CXX_ERROR(logger_, "Failed opening database.");
    LOG4CXX_INFO(logger_, "Starting opening retries.");
    const uint16_t attempts = settings_.app_launch_max_retry_attempt();
    LOG4CXX_DEBUG(logger_, "Total attempts number is: " << attempts);
    bool is_opened = false;
    const uint16_t open_attempt_timeout_ms =
        settings_.app_launch_retry_wait_time();
    const useconds_t sleep_interval_mcsec = open_attempt_timeout_ms * 1000u;
    LOG4CXX_DEBUG(logger_,
                  "Open attempt timeout(ms) is: " << open_attempt_timeout_ms);
    for (size_t i = 0u; i < attempts; ++i) {
      usleep(sleep_interval_mcsec);
      LOG4CXX_INFO(logger_, "Attempt: " << i + 1);
      if (db()->Open()) {
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
  if (!db()->IsReadWrite()) {
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

  return true;
}

bool AppLaunchDataDB::Persist() {
  LOG4CXX_AUTO_TRACE(logger_);
  bool retVal = false;

  if (!init_successeful_) {
    LOG4CXX_ERROR(logger_,
                  "AppLaunch data base was not successfully "
                  "initialize, AppLaunch won't work!");
    return retVal;
  }

  if ((retVal = WriteDb())) {
    LOG4CXX_DEBUG(logger_, "App_lauch had been successfully saved.");
  } else {
    LOG4CXX_WARN(logger_, "Fail to save app_launch data.");
  }

  return retVal;
}

bool AppLaunchDataDB::IsAppDataAlreadyExisted(
    const ApplicationData& app_data) const {
  LOG4CXX_AUTO_TRACE(logger_);
  bool retVal = false;

  if (!init_successeful_) {
    LOG4CXX_ERROR(logger_,
                  "AppLaunch data base was not successfully "
                  "initialize, AppLaunch won't work!");
    return retVal;
  }

  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(kFindApplicationData)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification queries 'kFindApplicationData'");
    return retVal;
  }

  query.Bind(device_mac_index, app_data.device_mac_);
  query.Bind(application_id_index, app_data.mobile_app_id_);
  query.Bind(bundle_id_index, app_data.bundle_id_);

  if (query.Exec()) {
    retVal = query.GetBoolean(result_query);
  } else {
    LOG4CXX_WARN(logger_,
                 "Failed execute query 'kGetNumberOfApplicationData'. Reson: "
                     << query.LastError().text());
  }

  return retVal;
}

bool AppLaunchDataDB::RefreshAppSessionTime(const ApplicationData& app_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool retVal = false;

  if (!init_successeful_) {
    LOG4CXX_ERROR(logger_,
                  "AppLaunch data base was not successfully "
                  "initialize, AppLaunch won't work!");
    return retVal;
  }

  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(kRefreshApplicationDataSessionTime)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification queries "
                 "'kRefreshApplicationDataSessionTime'");
    return retVal;
  }

  query.Bind(device_mac_index, app_data.device_mac_);
  query.Bind(application_id_index, app_data.mobile_app_id_);
  query.Bind(bundle_id_index, app_data.bundle_id_);

  if (query.Exec()) {
    LOG4CXX_DEBUG(logger_, "Dare&time last session were updated successfully");
    retVal = WriteDb();
  } else {
    LOG4CXX_WARN(logger_,
                 "Failed execute query 'kGetNumberOfApplicationData'. Reson: "
                     << query.LastError().text());
  }

  return retVal;
}

bool AppLaunchDataDB::AddNewAppData(const ApplicationData& app_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  bool retVal = false;

  if (!init_successeful_) {
    LOG4CXX_ERROR(logger_,
                  "AppLaunch data base was not successfully "
                  "initialize, AppLaunch won't work!");
    return retVal;
  }

  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(kAddApplicationData)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification queries 'kAddApplicationData'");
    return retVal;
  }

  query.Bind(device_mac_index, app_data.device_mac_);
  query.Bind(application_id_index, app_data.mobile_app_id_);
  query.Bind(bundle_id_index, app_data.bundle_id_);

  retVal = query.Exec();
  if (retVal) {
    LOG4CXX_DEBUG(logger_, "New application data was added successfully");
    retVal = WriteDb();
  } else {
    LOG4CXX_WARN(logger_,
                 "Failed execute query 'kGetNumberOfApplicationData'. Reson: "
                     << query.LastError().text());
  }

  return retVal;
}

std::vector<ApplicationDataPtr> AppLaunchDataDB::GetAppDataByDevMac(
    const std::string& dev_mac) const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<ApplicationDataPtr> dev_apps;

  if (!init_successeful_) {
    LOG4CXX_ERROR(logger_,
                  "AppLaunch data base was not successfully "
                  "initialize, AppLaunch won't work!");
    return dev_apps;
  }

  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(kGetApplicationDataByDevID)) {
    LOG4CXX_WARN(
        logger_,
        "Problem with verification queries 'kGetApplicationDataByDevID'");
    return dev_apps;
  }

  query.Bind(device_mac_index, dev_mac);
  const bool retVal = query.Exec();
  if (retVal) {
    LOG4CXX_INFO(logger_,
                 "Values of ignition off counts were updated successfully");
    do {
      const std::string device_mac = query.GetString(device_mac_index);
      const std::string mobile_app_id = query.GetString(application_id_index);
      const std::string bundle_id = query.GetString(bundle_id_index);
      dev_apps.push_back(std::make_shared<ApplicationData>(
          mobile_app_id, bundle_id, device_mac));
    } while (query.Next());
    LOG4CXX_DEBUG(logger_, "All application data has been successfully loaded");
  } else {
    LOG4CXX_WARN(logger_,
                 "Failed execute query 'kGetNumberOfApplicationData'. Reson: "
                     << query.LastError().text());
  }

  return dev_apps;
}

bool AppLaunchDataDB::Clear() {
  LOG4CXX_AUTO_TRACE(logger_);
  bool retVal = false;

  utils::dbms::SQLQuery query(db());
  retVal = query.Exec(kDropSchema);

  if (retVal) {
    LOG4CXX_INFO(logger_, "App_Launch table had been cleared successfully");
    retVal = WriteDb();
    init_successeful_ = false;
  } else {
    LOG4CXX_WARN(logger_,
                 "Failed dropping database: " << query.LastError().text());
  }

  return retVal;
}

uint32_t AppLaunchDataDB::GetCurentNumberOfAppData() const {
  LOG4CXX_AUTO_TRACE(logger_);
  uint32_t number_of_app_data = 0u;

  if (!init_successeful_) {
    LOG4CXX_ERROR(logger_,
                  "AppLaunch data base was not successfully "
                  "initialize, AppLaunch won't work!");
    return number_of_app_data;
  }

  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(kGetNumberOfApplicationData)) {
    LOG4CXX_WARN(
        logger_,
        "Problem with verification queries 'kGetNumberOfApplicationData'");
    return number_of_app_data;
  }

  if (query.Exec()) {
    LOG4CXX_INFO(logger_,
                 "Values of ignition off counts were updated successfully");

    number_of_app_data = query.GetInteger(result_query);
    LOG4CXX_DEBUG(logger_,
                  "Total cout saved mobile applications is "
                      << number_of_app_data);
  } else {
    LOG4CXX_WARN(logger_,
                 "Failed execute query 'kGetNumberOfApplicationData'. Reson: "
                     << query.LastError().text());
  }

  return number_of_app_data;
}

bool AppLaunchDataDB::DeleteOldestAppData() {
  LOG4CXX_AUTO_TRACE(logger_);
  bool retVal = false;

  if (!init_successeful_) {
    LOG4CXX_ERROR(logger_,
                  "AppLaunch data base was not successfully "
                  "initialize, AppLaunch won't work!");
    return retVal;
  }

  utils::dbms::SQLQuery query(db());

  if (!query.Prepare(kDeleteOldestAppData)) {
    LOG4CXX_WARN(logger_,
                 "Problem with verification queries 'kDeleteOldestAppData'");
    return retVal;
  }

  if ((retVal = query.Exec())) {
    LOG4CXX_INFO(logger_,
                 "Values of ignition off counts were updated successfully");
    retVal = WriteDb();
  } else {
    LOG4CXX_WARN(logger_,
                 "Failed execute query 'kGetNumberOfApplicationData'. Reson: "
                     << query.LastError().text());
  }

  return retVal;
}

bool AppLaunchDataDB::WriteDb() {
  LOG4CXX_AUTO_TRACE(logger_);
  return db_->Backup();
}

utils::dbms::SQLDatabase* AppLaunchDataDB::db() const {
#ifdef __QNX__
  std::unique_ptr<utils::dbms::SQLDatabase> db_qnx(
      new utils::dbms::SQLDatabase(kDatabaseName));
  db_qnx.get()->Open();
  return db_qnx.get();
#else
  return db_.get();
#endif  // __QNX__
}

}  // namespace resumption

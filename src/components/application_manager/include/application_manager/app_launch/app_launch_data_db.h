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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_DB_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_DB_H_
#include <memory>
#include "application_manager/app_launch/app_launch_data_impl.h"
#include "application_manager/app_launch_settings.h"
#include "utils/macro.h"
#ifdef __QNX__
#include "utils/qdb_wrapper/sql_database.h"
#include "utils/qdb_wrapper/sql_query.h"
#else  // linux
#include "utils/sqlite_wrapper/sql_database.h"
#include "utils/sqlite_wrapper/sql_query.h"
#endif

namespace app_launch {

const std::string kDatabaseName = "resumption";

/**
 * @brief Show should database be saved in a disk file or in memory
 */
enum DbStorage { In_Memory_Storage = 0, In_File_Storage };

/**
 * @brief class contains logic for representation application
 * launch_app data in data base
 */
class AppLaunchDataDB : public AppLaunchDataImpl {
 public:
  /**
   * @brief Constructor of AppLaunchDataDB
   * @param settings - setting of AppLaunch
   */
  AppLaunchDataDB(const AppLaunchSettings& settings,
                  DbStorage db_storage = In_File_Storage);

  /**
   * @brief allows to destroy AppLaunchDataDB object
   */
  ~AppLaunchDataDB();

  /**
   * @brief Creates or opens DB and initialize it
   * @return false if DB doesn't initialize
   * otherwise returns true
   */
  bool Init();

  /**
   * @brief delete App_launch table in DB, after calling this
   * one, it should again call init
   * @return true in success cases and false othrewise
   */
  bool Clear() OVERRIDE;

  /**
   * @brief Write database to file system
   */
  bool Persist() OVERRIDE;

  /**
   * @return current count of records
   * AppLaunch in DB
   */
  uint32_t GetCurentNumberOfAppData() const;

  /**
   * @param app_data - searching fields in object
   * @return  true in case application data already existed
   * and false othrewise
   */
  bool IsAppDataAlreadyExisted(const ApplicationData& app_data) const OVERRIDE;

  /**
  * @brief returns pointer to data base
  */
  utils::dbms::SQLDatabase* db() const;

  enum ApplicationDataDBIndexes {
    result_query = 0,
    device_mac_index = 0,
    application_id_index,
    bundle_id_index,
    timestamp_index
  };

 private:
  /**
   * @brief update time stamp
   * @param app_data - data to update
   * @return true in success cases and false othrewise
   */
  bool RefreshAppSessionTime(const ApplicationData& app_data) OVERRIDE;

  /**
   * @brief insert new data to DB
   * @param app_data - data to inserting
   * @return true in success cases and false othrewise
   */
  bool AddNewAppData(const ApplicationData& app_data) OVERRIDE;

  /**
   * @brief select from DB all records with this dev_mac
   * @param app_data - data to inserting
   * @return vector of ponter on founded records
   */
  std::vector<ApplicationDataPtr> GetAppDataByDevMac(
      const std::string& dev_mac) const OVERRIDE;
  /**
   * @brief delete record with oldest timestamp
   * @return true in success cases and false othrewise
   */
  bool DeleteOldestAppData() OVERRIDE;

  /**
   * @brief write DB to file
   * @return true in success cases and false othrewise
   */
  bool WriteDb();

  /**
  * @brief returns pointer to data base
  */
  std::unique_ptr<utils::dbms::SQLDatabase> db_;

  /**
  * @brief indicate initializing status of DB
  */
  bool init_successeful_;

  DISALLOW_COPY_AND_ASSIGN(AppLaunchDataDB);
};

}  // namespace app_launch

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_DB_H_

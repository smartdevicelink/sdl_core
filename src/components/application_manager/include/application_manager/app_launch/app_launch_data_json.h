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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_JSON_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_JSON_H_

#include "application_manager/app_launch/app_launch_data_impl.h"
#include "smart_objects/smart_object.h"
#include "resumption/last_state.h"
#include "utils/lock.h"
#include "utils/macro.h"

namespace app_launch {

enum { NotFound = -1 };
/**
 * @brief class contains logic for representation application launch_app data in
 * json file
 */
class AppLaunchDataJson : public AppLaunchDataImpl {
 public:
  /**
   * @brief Constructor of AppLaunchDataJson object
   */
  AppLaunchDataJson(const AppLaunchSettings& settings,
                    resumption::LastState& last_state);
  /**
   * @brief allows to destroy AppLaunchDataJson object
   */
  ~AppLaunchDataJson();

  /**
   * @brief Write Json to file system
   */
  bool Persist() OVERRIDE;

  /**
   * @brief delete App_launch table in DB, after calling this
   * one, it should again call init
   * @return true in success cases and false othrewise
   */
  virtual bool Clear();

  /**
   * @return current count of records
   * AppLaunch in DB
   */
  uint32_t GetCurentNumberOfAppData() const;

  /**
   * @param app_data - searching filled in object
   * @return  true in case application data already existed
   * and false othrewise
   */
  virtual bool IsAppDataAlreadyExisted(const ApplicationData& app_data) const;

  /**
   * @param app_data - searching filled in object
   * @param founded_index - referenceto index of founded record
   * in case it wasn't found it'll be -1
   * @return  pointer to json list object
   */
  Json::Value& GetApplicationListAndIndex(const ApplicationData& app_data,
                                          int32_t& founded_index) const;

 private:
  /**
   * @brief update time stamp
   * @param app_data - data to update
   * @return true in success cases and false othrewise
   */
  virtual bool RefreshAppSessionTime(const ApplicationData& app_data);

  /**
   * @brief insert new data to DB
   * @param app_data - data to inserting
   * @return true in success cases and false othrewise
   */
  virtual bool AddNewAppData(const ApplicationData& app_data);

  /**
   * @brief select from DB all records with this dev_mac
   * @param app_data - data to inserting
   * @return vector of pointer on founded records
   */
  std::vector<ApplicationDataPtr> GetAppDataByDevMac(
      const std::string& dev_mac) const OVERRIDE;

  /**
   * @return pointer to LastState functionality
   */
  resumption::LastState& last_state() const {
    return last_state_;
  }

  /**
   * @brief delete record with oldest timestamp
   * @return true in success cases and false othrewise
   */
  bool DeleteOldestAppData();

  /**
   * @return pointer to AppLaunch data block in Json file
   */
  Json::Value& GetSavedApplicationDataList() const;

  /**
   * @return pointer to AppLaunch records block in Json file
   */
  Json::Value& GetApplicationData() const;

  /**
   * @brief lock to protected common data
   */
  mutable sync_primitives::Lock app_launch_json_lock_;

  /**
   * @brief ponter to Last State object
   */
  resumption::LastState& last_state_;

  DISALLOW_COPY_AND_ASSIGN(AppLaunchDataJson);
};

}  // namespace app_launch

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_JSON_H_

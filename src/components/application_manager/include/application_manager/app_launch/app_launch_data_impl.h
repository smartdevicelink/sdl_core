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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_IMPL_H_

#include "application_manager/app_launch/app_launch_data.h"
#include "application_manager/app_launch_settings.h"
#include "utils/macro.h"
#include <memory>

namespace app_launch {

class AppLaunchDataImpl : public AppLaunchData {
 public:
  /**
   * @brief Constructor of AppLaunchDataImpl
   * @param settings - setting of AppLaunch
   */
  AppLaunchDataImpl(const AppLaunchSettings& settings);

  /**
   * @brief allows to destroy AppLaunchDataImpl object
   */
  virtual ~AppLaunchDataImpl();

  /**
   * @brief insert new data to DB
   * @param app_data - data to inserting
   * @return true in success cases and false othrewise
   */
  virtual bool AddApplicationData(const ApplicationData& app_data);

  /**
   * @brief select from DB all records with this dev_mac
   * @param app_data - data to inserting
   * @return return vector of pointers on founded records
   */
  std::vector<ApplicationDataPtr> GetApplicationDataByDevice(
      const std::string& dev_mac) OVERRIDE;

  /**
   * @brief Persist saves resumption data on file system
   */
  virtual bool Persist() = 0;

  /**
   * @return max count of iOS device that can be connected
   */
  virtual uint32_t get_max_number_iOS_devs() const {
    return kMaxNumberOfiOSdevice;
  }

  /**
   * @brief all AppLaunch settings
   */
  const AppLaunchSettings& settings_;

 private:
  /**
   * @param app_data - searching filled in object
   * @return  true in case application data already existed
   * and false othrewise
   */
  virtual bool IsAppDataAlreadyExisted(
      const ApplicationData& app_data) const = 0;

  /**
   * @brief update time stamp
   * @param app_data - data to update
   * @return true in success cases and false othrewise
   */
  virtual bool RefreshAppSessionTime(const ApplicationData& app_data) = 0;

  /**
   * @brief insert new data to DB
   * @param app_data - data to inserting
   * @return true in success cases and false othrewise
   */
  virtual bool AddNewAppData(const ApplicationData& app_data) = 0;

  /**
   * @brief select from DB all records with this dev_mac
   * @param app_data - data to inserting
   * @return vector of ponter on founded records
   */
  virtual std::vector<ApplicationDataPtr> GetAppDataByDevMac(
      const std::string& dev_mac) const = 0;

  /**
   * @brief delete record with oldest timestamp
   * @return true in success cases and false othrewise
   */
  virtual bool DeleteOldestAppData() = 0;

  /**
   * @return current count of records
   * AppLaunch in DB
   */
  virtual uint32_t GetCurentNumberOfAppData() const = 0;

  /**
   * @brief max count of iOS device that can be connected
   */
  const uint32_t kMaxNumberOfiOSdevice;
};
}  // namespace app_launch

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_IMPL_H_

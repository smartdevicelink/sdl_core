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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_H_

#include <stdint.h>
#include <vector>
#include <string>
#include <memory>

namespace app_launch {

/**
 * @brief struct holds AppLaunch data
 */
struct ApplicationData {
  ApplicationData(const std::string& mobile_app_id,
                  const std::string& bundle_id,
                  const std::string& device_id)
      : mobile_app_id_(mobile_app_id)
      , bundle_id_(bundle_id)
      , device_mac_(device_id) {}

  std::string mobile_app_id_;
  std::string bundle_id_;
  std::string device_mac_;
  bool operator==(const ApplicationData& app_data) const {
    return mobile_app_id_ == app_data.mobile_app_id_ &&
           bundle_id_ == app_data.bundle_id_ && device_mac_ == device_mac_;
  }
};
typedef std::shared_ptr<ApplicationData> ApplicationDataPtr;

/**
 * @brief class contains interfaces to AppLaunchDataDB and AppLaunchDataJson
 */
class AppLaunchData {
 public:
  /**
   * @brief allows correct delete heir object
   */
  virtual ~AppLaunchData() {}

  /**
   * @brief insert new data to DB
   * @param app_data - data to inserting
   * @return true in success cases and false othrewise
   */
  virtual bool AddApplicationData(const ApplicationData& app_data) = 0;

  /**
   * @brief select from DB all records with this dev_mac
   * @param app_data - data to inserting
   * @param dev_apps -
   * @return vector of pointers on results of select
   */
  virtual std::vector<ApplicationDataPtr> GetApplicationDataByDevice(
      const std::string& dev_mac) = 0;
  /**
   * @brief delete App_launch table in DB, after calling this
   * one, it should again call init
   * @return true in success cases and false othrewise
   */
  virtual bool Clear() = 0;

  /**
   * @brief Persist saves resumption data on file system
   */
  virtual bool Persist() = 0;
};
}  // namespace app_launch

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_APP_LAUNCH_APP_LAUNCH_DATA_H_

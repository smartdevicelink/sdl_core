/*
 * Copyright (c) 2017, Ford Motor Company
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
#include <algorithm>
#include "application_manager/app_launch/app_launch_data_json.h"
#include "application_manager/smart_object_keys.h"
#include "smart_objects/smart_object.h"
#include "utils/make_shared.h"
#include "utils/date_time.h"
#include "json/json.h"

namespace app_launch {

CREATE_LOGGERPTR_GLOBAL(logger_, "AppLaunch")

AppLaunchDataJson::AppLaunchDataJson(const AppLaunchSettings& settings,
                                     resumption::LastState& last_state)
    : AppLaunchDataImpl(settings)
    , app_launch_json_lock_(true)
    , last_state_(last_state) {}

AppLaunchDataJson::~AppLaunchDataJson() {}

Json::Value& AppLaunchDataJson::GetSavedApplicationDataList() const {
  using namespace application_manager;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(app_launch_json_lock_);
  Json::Value& app_launch = GetApplicationData();
  if (!app_launch.isMember(strings::app_launch_list)) {
    app_launch[strings::app_launch_list] = Json::Value(Json::arrayValue);
    LOG4CXX_WARN(logger_, "app_list section is missed");
  }
  Json::Value& app_launch_list = app_launch[strings::app_launch_list];
  if (!app_launch_list.isArray()) {
    LOG4CXX_ERROR(logger_, "app_launch_list type INVALID rewrite");
    app_launch_list = Json::Value(Json::arrayValue);
  }
  return app_launch_list;
}

Json::Value& AppLaunchDataJson::GetApplicationData() const {
  using namespace application_manager;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(app_launch_json_lock_);
  Json::Value& dictionary = last_state().get_dictionary();
  if (!dictionary.isMember(strings::app_launch)) {
    dictionary[strings::app_launch] = Json::Value(Json::objectValue);
    LOG4CXX_WARN(logger_, "app_launch section is missed");
  }
  Json::Value& app_launch = dictionary[strings::app_launch];
  if (!app_launch.isObject()) {
    LOG4CXX_ERROR(logger_, "resumption type INVALID rewrite");
    app_launch = Json::Value(Json::objectValue);
  }
  return app_launch;
}

Json::Value& AppLaunchDataJson::GetApplicationListAndIndex(
    const ApplicationData& app_data, int32_t& founded_index) const {
  using namespace application_manager;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(app_launch_json_lock_);

  Json::Value& apps_list = GetSavedApplicationDataList();
  const Json::ArrayIndex size = apps_list.size();

  for (Json::ArrayIndex idx = 0; idx != size; ++idx) {
    if (apps_list[idx].isMember(strings::device_id) &&
        apps_list[idx].isMember(strings::bundle_id) &&
        apps_list[idx].isMember(strings::app_id) &&
        apps_list[idx].isMember(strings::app_launch_last_session)) {
      const std::string deviceID =
          apps_list[idx][strings::device_id].asString();
      const std::string bundleID =
          apps_list[idx][strings::bundle_id].asString();
      const std::string appID = apps_list[idx][strings::app_id].asString();

      if (deviceID == app_data.device_mac_ && bundleID == app_data.bundle_id_ &&
          appID == app_data.mobile_app_id_) {
        founded_index = idx;
      }
    }
  }

  return apps_list;
}

bool AppLaunchDataJson::IsAppDataAlreadyExisted(
    const ApplicationData& app_data) const {
  LOG4CXX_AUTO_TRACE(logger_);

  int32_t index = NotFound;
  GetApplicationListAndIndex(app_data, index);
  return index == NotFound ? false : true;
}

bool AppLaunchDataJson::RefreshAppSessionTime(const ApplicationData& app_data) {
  using namespace application_manager;
  using namespace date_time;
  LOG4CXX_AUTO_TRACE(logger_);
  bool retVal = false;

  int32_t index = NotFound;
  Json::Value& json_data_list = GetApplicationListAndIndex(app_data, index);
  if (index != NotFound) {
    if (json_data_list.empty() == false) {
      json_data_list[index][strings::app_launch_last_session] =
          static_cast<Json::Value::UInt64>(DateTime::getCurrentTime().tv_sec);
      retVal = true;
    }
  }
  return retVal;
}

bool AppLaunchDataJson::AddNewAppData(const ApplicationData& app_data) {
  using namespace application_manager;
  using namespace date_time;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(app_launch_json_lock_);

  Json::Value& json_app_data =
      GetSavedApplicationDataList().append(Json::Value());
  json_app_data[strings::device_id] = app_data.device_mac_;
  json_app_data[strings::app_id] = app_data.mobile_app_id_;
  json_app_data[strings::bundle_id] = app_data.bundle_id_;
  json_app_data[strings::app_launch_last_session] =
      static_cast<Json::Value::UInt64>(DateTime::getCurrentTime().tv_sec);

  LOG4CXX_DEBUG(logger_,
                "New application data saved. Detatils device_id: "
                    << app_data.device_mac_
                    << ", app_id: " << app_data.mobile_app_id_
                    << ", bundle_id: " << app_data.bundle_id_ << ".");

  return true;
}

std::vector<ApplicationDataPtr> AppLaunchDataJson::GetAppDataByDevMac(
    const std::string& dev_mac) const {
  using namespace application_manager;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(app_launch_json_lock_);
  std::vector<ApplicationDataPtr> dev_apps;
  const Json::Value& apps_list = GetSavedApplicationDataList();
  const Json::ArrayIndex size = apps_list.size();

  for (Json::ArrayIndex idx = 0; idx != size; ++idx) {
    if (apps_list[idx].isMember(strings::device_id) &&
        apps_list[idx].isMember(strings::bundle_id) &&
        apps_list[idx].isMember(strings::app_id) &&
        apps_list[idx].isMember(strings::app_launch_last_session)) {
      const std::string deviceMac =
          apps_list[idx][strings::device_id].asString();
      const std::string bundleID =
          apps_list[idx][strings::bundle_id].asString();
      const std::string appID = apps_list[idx][strings::app_id].asString();

      if (deviceMac == dev_mac) {
        dev_apps.push_back(
            std::make_shared<ApplicationData>(appID, bundleID, deviceMac));
      }
    }
  }

  return dev_apps;
}

bool AppLaunchDataJson::Clear() {
  LOG4CXX_AUTO_TRACE(logger_);

  GetSavedApplicationDataList().clear();

  LOG4CXX_DEBUG(logger_,
                "Application launch JSON section successfully cleared.");

  return true;
}

uint32_t AppLaunchDataJson::GetCurentNumberOfAppData() const {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t list_size = GetSavedApplicationDataList().size();

  LOG4CXX_DEBUG(logger_,
                "Successfully was gotten app_launch list. Size: " << list_size);

  return list_size;
}

bool AppLaunchDataJson::DeleteOldestAppData() {
  using namespace application_manager;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(app_launch_json_lock_);
  std::vector<uint64_t> temp_array;
  std::vector<Json::Value> temp_json_list;
  Json::Value& apps_list = GetSavedApplicationDataList();
  const Json::ArrayIndex size = apps_list.size();

  // Search oldest record in Json
  // for it collect all timestaps in vector
  for (Json::ArrayIndex idx = 0; idx != size; ++idx) {
    if (apps_list[idx].isMember(strings::device_id) &&
        apps_list[idx].isMember(strings::bundle_id) &&
        apps_list[idx].isMember(strings::app_id) &&
        apps_list[idx].isMember(strings::app_launch_last_session)) {
      temp_array.push_back(
          apps_list[idx][strings::app_launch_last_session].asUInt64());
    }
  }

  // Calc oldest one and found index of it in Json
  const int32_t oldest_index =
      (std::min_element(temp_array.begin(), temp_array.end()) -
       temp_array.begin());

  // Copy in temporary vector Json list without oldest record
  int32_t i = 0;
  for (Json::Value::iterator it = GetSavedApplicationDataList().begin();
       it != GetSavedApplicationDataList().end();
       ++it, i++) {
    if ((*it).isMember(strings::device_id) &&
        (*it).isMember(strings::bundle_id) && (*it).isMember(strings::app_id) &&
        (*it).isMember(strings::app_launch_last_session)) {
      if (i == oldest_index) {
        continue;
      }
      temp_json_list.push_back((*it));
    }
  }

  // Clear Json list
  GetSavedApplicationDataList().clear();

  // Copy to Json new list without oldest one
  for (std::vector<Json::Value>::iterator it = temp_json_list.begin();
       it != temp_json_list.end();
       ++it) {
    GetSavedApplicationDataList().append((*it));
  }

  LOG4CXX_DEBUG(
      logger_, "Oldest application launch data had been successfully deleted.");

  return true;
}

bool AppLaunchDataJson::Persist() {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(app_launch_json_lock_);
  last_state().SaveStateToFileSystem();
  return true;
}

}  // app_launch

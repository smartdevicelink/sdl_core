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
#include "application_manager/app_launch/app_launch_data_json.h"
#include <algorithm>
#include "application_manager/smart_object_keys.h"
#include "json/json.h"
#include "smart_objects/smart_object.h"
#include "utils/date_time.h"

namespace app_launch {

SDL_CREATE_LOG_VARIABLE("AppLaunch")

AppLaunchDataJson::AppLaunchDataJson(
    const AppLaunchSettings& settings,
    resumption::LastStateWrapperPtr last_state_wrapper)
    : AppLaunchDataImpl(settings), last_state_wrapper_(last_state_wrapper) {}

AppLaunchDataJson::~AppLaunchDataJson() {}

Json::Value& AppLaunchDataJson::GetSavedApplicationDataList(
    Json::Value& dictionary) const {
  using namespace application_manager;
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock autolock(app_launch_json_lock_);
  Json::Value& app_launch = GetApplicationData(dictionary);
  if (!app_launch.isMember(strings::app_launch_list)) {
    app_launch[strings::app_launch_list] = Json::Value(Json::arrayValue);
    SDL_LOG_WARN("app_list section is missed");
  }
  Json::Value& app_launch_list = app_launch[strings::app_launch_list];
  if (!app_launch_list.isArray()) {
    SDL_LOG_ERROR("app_launch_list type INVALID rewrite");
    app_launch_list = Json::Value(Json::arrayValue);
  }
  return app_launch_list;
}

Json::Value& AppLaunchDataJson::GetApplicationData(
    Json::Value& dictionary) const {
  using namespace application_manager;
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock autolock(app_launch_json_lock_);
  if (!dictionary.isMember(strings::app_launch)) {
    dictionary[strings::app_launch] = Json::Value(Json::objectValue);
    SDL_LOG_WARN("app_launch section is missed");
  }
  Json::Value& app_launch = dictionary[strings::app_launch];
  if (!app_launch.isObject()) {
    SDL_LOG_ERROR("resumption type INVALID rewrite");
    app_launch = Json::Value(Json::objectValue);
  }
  return app_launch;
}

Json::Value& AppLaunchDataJson::GetApplicationListAndIndex(
    const ApplicationData& app_data,
    int32_t& found_index,
    Json::Value& dictionary) const {
  using namespace application_manager;
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock autolock(app_launch_json_lock_);

  Json::Value& apps_list = GetSavedApplicationDataList(dictionary);
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
        found_index = idx;
      }
    }
  }

  return apps_list;
}

bool AppLaunchDataJson::IsAppDataAlreadyExisted(
    const ApplicationData& app_data) const {
  SDL_LOG_AUTO_TRACE();

  int32_t index = NotFound;

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  GetApplicationListAndIndex(app_data, index, dictionary);
  accessor.GetMutableData().set_dictionary(dictionary);
  return index == NotFound ? false : true;
}

bool AppLaunchDataJson::RefreshAppSessionTime(const ApplicationData& app_data) {
  using namespace application_manager;
  using namespace date_time;
  SDL_LOG_AUTO_TRACE();
  bool retVal = false;

  int32_t index = NotFound;
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& json_data_list =
      GetApplicationListAndIndex(app_data, index, dictionary);
  if (index != NotFound) {
    if (json_data_list.empty() == false) {
      json_data_list[index][strings::app_launch_last_session] =
          static_cast<Json::Value::UInt64>(getSecs(getCurrentTime()));
      retVal = true;
    }
  }
  accessor.GetMutableData().set_dictionary(dictionary);
  return retVal;
}

bool AppLaunchDataJson::AddNewAppData(const ApplicationData& app_data) {
  using namespace application_manager;
  using namespace date_time;
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock autolock(app_launch_json_lock_);

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& json_app_data =
      GetSavedApplicationDataList(dictionary).append(Json::Value());
  json_app_data[strings::device_id] = app_data.device_mac_;
  json_app_data[strings::app_id] = app_data.mobile_app_id_;
  json_app_data[strings::bundle_id] = app_data.bundle_id_;
  json_app_data[strings::app_launch_last_session] =
      static_cast<Json::Value::UInt64>(getSecs(getCurrentTime()));
  accessor.GetMutableData().set_dictionary(dictionary);

  SDL_LOG_DEBUG("New application data saved. Detatils device_id: "
                << app_data.device_mac_
                << ", app_id: " << app_data.mobile_app_id_
                << ", bundle_id: " << app_data.bundle_id_ << ".");
  return true;
}

std::vector<ApplicationDataPtr> AppLaunchDataJson::GetAppDataByDevMac(
    const std::string& dev_mac) const {
  using namespace application_manager;
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock autolock(app_launch_json_lock_);
  std::vector<ApplicationDataPtr> dev_apps;
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  const Json::Value& apps_list = GetSavedApplicationDataList(dictionary);
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
  accessor.GetMutableData().set_dictionary(dictionary);
  return dev_apps;
}

bool AppLaunchDataJson::Clear() {
  SDL_LOG_AUTO_TRACE();
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  GetSavedApplicationDataList(dictionary).clear();
  accessor.GetMutableData().set_dictionary(dictionary);
  SDL_LOG_DEBUG("Application launch JSON section successfully cleared.");

  return true;
}

uint32_t AppLaunchDataJson::GetCurentNumberOfAppData() const {
  SDL_LOG_AUTO_TRACE();
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  const uint32_t list_size = GetSavedApplicationDataList(dictionary).size();
  accessor.GetMutableData().set_dictionary(dictionary);
  SDL_LOG_DEBUG("Successfully was gotten app_launch list. Size: " << list_size);

  return list_size;
}

bool AppLaunchDataJson::DeleteOldestAppData() {
  using namespace application_manager;
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock autolock(app_launch_json_lock_);
  std::vector<uint64_t> temp_array;
  std::vector<Json::Value> temp_json_list;
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& apps_list = GetSavedApplicationDataList(dictionary);
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
  for (auto it = apps_list.begin(); it != apps_list.end(); ++it, ++i) {
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
  GetSavedApplicationDataList(dictionary).clear();

  // Copy to Json new list without oldest one
  for (const auto& item : temp_json_list) {
    GetSavedApplicationDataList(dictionary).append(item);
  }
  accessor.GetMutableData().set_dictionary(dictionary);
  SDL_LOG_DEBUG(
      "Oldest application launch data had been successfully deleted.");

  return true;
}

bool AppLaunchDataJson::Persist() {
  SDL_LOG_AUTO_TRACE();
  last_state_wrapper_->get_accessor().GetMutableData().SaveToFileSystem();
  return true;
}

}  // namespace app_launch

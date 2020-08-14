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

#include "application_manager/resumption/resumption_data_json.h"
#include "application_manager/application_manager.h"
#include "application_manager/application_manager_settings.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "formatters/CFormatterJsonBase.h"
#include "json/json.h"
#include "smart_objects/smart_object.h"

namespace resumption {

namespace formatters = ns_smart_device_link::ns_json_handler::formatters;

CREATE_LOGGERPTR_GLOBAL(logger_, "Resumption")

ResumptionDataJson::ResumptionDataJson(
    resumption::LastStateWrapperPtr last_state_wrapper,
    const application_manager::ApplicationManager& application_manager)
    : ResumptionData(application_manager)
    , last_state_wrapper_(last_state_wrapper) {}

void ResumptionDataJson::SaveApplication(
    app_mngr::ApplicationSharedPtr application) {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(application);

  const std::string& policy_app_id = application->policy_app_id();
  LOG4CXX_DEBUG(logger_,
                "app_id : " << application->app_id()
                            << " policy_app_id : " << policy_app_id);
  const std::string hash = application->curHash();
  const uint32_t grammar_id = application->get_grammar_id();
  const uint32_t time_stamp = (uint32_t)time(NULL);
  const std::string device_mac = application->mac_address();
  const mobile_apis::HMILevel::eType hmi_level =
      application->hmi_level(mobile_apis::PredefinedWindows::DEFAULT_WINDOW);
  const bool is_subscribed_for_way_points =
      application_manager_.IsAppSubscribedForWayPoints(*application);

  Json::Value tmp;
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& json_app =
      GetFromSavedOrAppend(policy_app_id, device_mac, dictionary);

  json_app[strings::device_id] = device_mac;
  json_app[strings::app_id] = policy_app_id;
  json_app[strings::grammar_id] = grammar_id;
  json_app[strings::connection_key] = application->app_id();
  json_app[strings::hmi_app_id] = application->hmi_app_id();
  json_app[strings::is_media_application] = application->IsAudioApplication();
  json_app[strings::hmi_level] = static_cast<int32_t>(hmi_level);
  json_app[strings::ign_off_count] = 0;
  json_app[strings::hash_id] = hash;
  formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationCommands(application), tmp);
  json_app[strings::application_commands] = tmp;
  formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationSubMenus(application), tmp);
  json_app[strings::application_submenus] = tmp;
  formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationInteractionChoiseSets(application), tmp);
  json_app[strings::application_choice_sets] = tmp;
  formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationGlobalProperties(application), tmp);
  json_app[strings::application_global_properties] = tmp;
  formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationSubscriptions(application), tmp);
  json_app[strings::application_subscriptions] = tmp;
  formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationFiles(application), tmp);
  json_app[strings::application_files] = tmp;
  formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationWidgetsInfo(application), tmp);
  json_app[strings::windows_info] = tmp;
  json_app[strings::time_stamp] = time_stamp;
  json_app[strings::subscribed_for_way_points] = is_subscribed_for_way_points;

  accessor.GetMutableData().set_dictionary(dictionary);
  LOG4CXX_DEBUG(logger_, "SaveApplication : " << json_app.toStyledString());
}

bool ResumptionDataJson::IsHMIApplicationIdExist(uint32_t hmi_app_id) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  const Json::Value& saved_apps = GetSavedApplications(dictionary);

  for (const auto& saved_app : saved_apps) {
    if (saved_app.isMember(strings::hmi_app_id)) {
      if (saved_app[strings::hmi_app_id].asUInt() == hmi_app_id) {
        return true;
      }
    }
  }
  return false;
}

uint32_t ResumptionDataJson::GetHMIApplicationID(
    const std::string& policy_app_id, const std::string& device_id) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t hmi_app_id = 0;

  const int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return hmi_app_id;
  }
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  const Json::Value& json_app = GetSavedApplications(dictionary)[idx];
  if (json_app.isMember(strings::app_id) &&
      json_app.isMember(strings::device_id)) {
    hmi_app_id = json_app[strings::hmi_app_id].asUInt();
  }
  LOG4CXX_DEBUG(logger_, "hmi_app_id :" << hmi_app_id);
  return hmi_app_id;
}

void ResumptionDataJson::IncrementIgnOffCount() {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value to_save = Json::arrayValue;
  Json::Value dictionary;
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  dictionary = accessor.GetData().dictionary();
  Json::Value& saved_apps = GetSavedApplications(dictionary);
  for (auto& json_app : saved_apps) {
    if (json_app.isMember(strings::ign_off_count)) {
      Json::Value& ign_off_count = json_app[strings::ign_off_count];
      const uint32_t counter_value = ign_off_count.asUInt();
      ign_off_count = counter_value + 1;
    } else {
      LOG4CXX_WARN(logger_, "Unknown key among saved applications");
      Json::Value& ign_off_count = json_app[strings::ign_off_count];
      ign_off_count = 1;
    }
    to_save.append(json_app);
  }
  SetSavedApplication(to_save, dictionary);
  SetLastIgnOffTime(time(nullptr), dictionary);
  accessor.GetMutableData().set_dictionary(dictionary);
  LOG4CXX_DEBUG(logger_, GetResumptionData(dictionary).toStyledString());
}

void ResumptionDataJson::DecrementIgnOffCount() {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& saved_apps = GetSavedApplications(dictionary);
  for (auto& saved_app : saved_apps) {
    if (saved_app.isMember(strings::ign_off_count)) {
      const uint32_t ign_off_count = saved_app[strings::ign_off_count].asUInt();
      if (0 == ign_off_count) {
        LOG4CXX_WARN(logger_, "Application has not been suspended");
      } else {
        saved_app[strings::ign_off_count] = ign_off_count - 1;
      }
    } else {
      LOG4CXX_WARN(logger_, "Unknown key among saved applications");
      saved_app[strings::ign_off_count] = 0;
    }
  }
  accessor.GetMutableData().set_dictionary(dictionary);
}

bool ResumptionDataJson::GetHashId(const std::string& policy_app_id,
                                   const std::string& device_id,
                                   std::string& hash_id) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  const int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return false;
  }

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  const Json::Value& json_app = GetSavedApplications(dictionary)[idx];
  LOG4CXX_DEBUG(logger_,
                "Saved_application_data: " << json_app.toStyledString());
  if (json_app.isMember(strings::hash_id) &&
      json_app.isMember(strings::time_stamp)) {
    hash_id = json_app[strings::hash_id].asString();
    return true;
  }
  LOG4CXX_WARN(logger_, "There are some unknown keys in the dictionary.");
  return false;
}

bool ResumptionDataJson::GetSavedApplication(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  const int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    return false;
  }
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  const Json::Value& json_saved_app = GetSavedApplications(dictionary)[idx];
  formatters::CFormatterJsonBase::jsonValueToObj(json_saved_app, saved_app);

  return true;
}

bool ResumptionDataJson::RemoveApplicationFromSaved(
    const std::string& policy_app_id, const std::string& device_id) {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  bool result = false;
  std::vector<Json::Value> temp;
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& saved_apps = GetSavedApplications(dictionary);
  for (auto& app : saved_apps) {
    if (app.isMember(strings::app_id) && app.isMember(strings::device_id)) {
      const std::string& saved_policy_app_id = app[strings::app_id].asString();
      const std::string& saved_device_id = app[strings::device_id].asString();
      if (saved_policy_app_id == policy_app_id &&
          saved_device_id == device_id) {
        result = true;
      } else {
        temp.push_back(app);
      }
    }
  }

  if (false == result) {
    LOG4CXX_TRACE(logger_, "EXIT result: " << (result ? "true" : "false"));
    accessor.GetMutableData().set_dictionary(dictionary);
    return result;
  }

  GetSavedApplications(dictionary).clear();
  for (auto& app : temp) {
    GetSavedApplications(dictionary).append(app);
  }
  LOG4CXX_TRACE(logger_, "EXIT result: " << (result ? "true" : "false"));
  accessor.GetMutableData().set_dictionary(dictionary);
  return result;
}

uint32_t ResumptionDataJson::GetIgnOffTime() const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& resumption = GetResumptionData(dictionary);
  if (!resumption.isMember(strings::last_ign_off_time)) {
    resumption[strings::last_ign_off_time] = 0;
    accessor.GetMutableData().set_dictionary(dictionary);
    LOG4CXX_WARN(logger_, "last_save_time section is missed");
  }
  return resumption[strings::last_ign_off_time].asUInt();
}

uint32_t ResumptionDataJson::GetGlobalIgnOnCounter() const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& resumption = GetResumptionData(dictionary);
  if (resumption.isMember(strings::global_ign_on_counter)) {
    const uint32_t global_ign_on_counter =
        resumption[strings::global_ign_on_counter].asUInt();
    LOG4CXX_DEBUG(logger_, "Global Ign On Counter = " << global_ign_on_counter);
    return global_ign_on_counter;
  }
  return 1;
}

void ResumptionDataJson::IncrementGlobalIgnOnCounter() {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& resumption = GetResumptionData(dictionary);
  if (resumption.isMember(strings::global_ign_on_counter)) {
    const uint32_t global_ign_on_counter =
        resumption[strings::global_ign_on_counter].asUInt();
    LOG4CXX_DEBUG(
        logger_,
        "Global IGN ON counter in resumption data: " << global_ign_on_counter);
    resumption[strings::global_ign_on_counter] = global_ign_on_counter + 1;
    LOG4CXX_DEBUG(logger_,
                  "Global IGN ON counter new value: "
                      << resumption[strings::global_ign_on_counter].asUInt());
  } else {
    resumption[strings::global_ign_on_counter] = 1;
  }
  accessor.GetMutableData().set_dictionary(dictionary);
  accessor.GetMutableData().SaveToFileSystem();
}

void ResumptionDataJson::ResetGlobalIgnOnCount() {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& resumption = GetResumptionData(dictionary);

  resumption[strings::global_ign_on_counter] = 0;
  accessor.GetMutableData().set_dictionary(dictionary);
  LOG4CXX_DEBUG(logger_, "Global IGN ON counter resetting");
}

ssize_t ResumptionDataJson::IsApplicationSaved(
    const std::string& policy_app_id, const std::string& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);

  return GetObjectIndex(policy_app_id, device_id);
}

Json::Value& ResumptionDataJson::GetFromSavedOrAppend(
    const std::string& policy_app_id,
    const std::string& device_id,
    Json::Value& dictionary) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value& saved_apps = GetSavedApplications(dictionary);
  for (auto& saved_app : saved_apps) {
    if (device_id == saved_app[strings::device_id].asString() &&
        policy_app_id == saved_app[strings::app_id].asString()) {
      return saved_app;
    }
  }

  return GetSavedApplications(dictionary).append(Json::Value());
}

void ResumptionDataJson::GetDataForLoadResumeData(
    smart_objects::SmartObject& saved_data) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  smart_objects::SmartObject so_array_data(smart_objects::SmartType_Array);
  int i = 0;
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& saved_apps = GetSavedApplications(dictionary);

  for (auto& saved_app : saved_apps) {
    if ((saved_app.isMember(strings::hmi_level)) &&
        (saved_app.isMember(strings::ign_off_count)) &&
        (saved_app.isMember(strings::time_stamp)) &&
        (saved_app.isMember(strings::app_id)) &&
        (saved_app.isMember(strings::device_id))) {
      smart_objects::SmartObject so(smart_objects::SmartType_Map);
      so[strings::hmi_level] = saved_app[strings::hmi_level].asInt();
      so[strings::ign_off_count] = saved_app[strings::ign_off_count].asInt();
      so[strings::time_stamp] = saved_app[strings::time_stamp].asUInt();
      so[strings::app_id] = saved_app[strings::app_id].asString();
      so[strings::device_id] = saved_app[strings::device_id].asString();
      so_array_data[i++] = so;
    }
  }
  saved_data = so_array_data;
}

ResumptionDataJson::~ResumptionDataJson() {
  // TODO Probably Save Data on disk
}

void ResumptionDataJson::UpdateHmiLevel(
    const std::string& policy_app_id,
    const std::string& device_id,
    mobile_apis::HMILevel::eType hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;

  int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    LOG4CXX_WARN(logger_,
                 "Application isn't saved with mobile_app_id = "
                     << policy_app_id << " device_id = " << device_id);
    return;
  }
  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  GetSavedApplications(dictionary)[idx][strings::hmi_level] = hmi_level;
  accessor.GetMutableData().set_dictionary(dictionary);
}

Json::Value& ResumptionDataJson::GetSavedApplications(
    Json::Value& dictionary) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value& resumption = GetResumptionData(dictionary);
  if (!resumption.isMember(strings::resume_app_list)) {
    resumption[strings::resume_app_list] = Json::Value(Json::arrayValue);
    LOG4CXX_WARN(logger_, "app_list section is missed");
  }
  Json::Value& resume_app_list = resumption[strings::resume_app_list];
  if (!resume_app_list.isArray()) {
    LOG4CXX_ERROR(logger_, "resume_app_list type INVALID rewrite");
    resume_app_list = Json::Value(Json::arrayValue);
  }
  return resume_app_list;
}

Json::Value& ResumptionDataJson::GetResumptionData(
    Json::Value& dictionary) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  if (!dictionary.isMember(strings::resumption)) {
    dictionary[strings::resumption] = Json::Value(Json::objectValue);
    LOG4CXX_WARN(logger_, "resumption section is missed");
  }
  Json::Value& resumption = dictionary[strings::resumption];
  if (!resumption.isObject()) {
    LOG4CXX_ERROR(logger_, "resumption type INVALID rewrite");
    resumption = Json::Value(Json::objectValue);
  }
  return resumption;
}

ssize_t ResumptionDataJson::GetObjectIndex(const std::string& policy_app_id,
                                           const std::string& device_id) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  const Json::Value& apps = GetSavedApplications(dictionary);
  const Json::ArrayIndex size = apps.size();
  Json::ArrayIndex idx = 0;
  for (; idx != size; ++idx) {
    if (apps[idx].isMember(strings::app_id) &&
        apps[idx].isMember(strings::device_id)) {
      const std::string& saved_app_id = apps[idx][strings::app_id].asString();
      const std::string& saved_device_id =
          apps[idx][strings::device_id].asString();
      if (device_id == saved_device_id && policy_app_id == saved_app_id) {
        LOG4CXX_DEBUG(logger_, "Found " << idx);
        return idx;
      }
    }
  }
  return -1;
}

bool ResumptionDataJson::IsResumptionDataValid(uint32_t index) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  const Json::Value& json_app = GetSavedApplications(dictionary)[index];
  if (!json_app.isMember(strings::app_id) ||
      !json_app.isMember(strings::ign_off_count) ||
      !json_app.isMember(strings::hmi_level) ||
      !json_app.isMember(strings::hmi_app_id) ||
      !json_app.isMember(strings::time_stamp) ||
      !json_app.isMember(strings::device_id)) {
    LOG4CXX_ERROR(logger_, "Wrong resumption data");
    return false;
  }

  if (json_app.isMember(strings::hmi_app_id) &&
      0 >= json_app[strings::hmi_app_id].asUInt()) {
    LOG4CXX_ERROR(logger_, "Wrong resumption hmi app ID");
    return false;
  }

  return true;
}

void ResumptionDataJson::SetSavedApplication(Json::Value& apps_json,
                                             Json::Value& dictionary) {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value& app_list = GetSavedApplications(dictionary);
  app_list = apps_json;
}

void ResumptionDataJson::SetLastIgnOffTime(time_t ign_off_time,
                                           Json::Value& dictionary) {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);

  LOG4CXX_WARN(logger_, "ign_off_time = " << ign_off_time);
  Json::Value& resumption = GetResumptionData(dictionary);
  resumption[strings::last_ign_off_time] = static_cast<uint32_t>(ign_off_time);
}

bool ResumptionDataJson::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  return true;
}

bool ResumptionDataJson::DropAppDataResumption(const std::string& device_id,
                                               const std::string& app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace app_mngr;

  resumption::LastStateAccessor accessor = last_state_wrapper_->get_accessor();
  Json::Value dictionary = accessor.GetData().dictionary();
  Json::Value& application =
      GetFromSavedOrAppend(app_id, device_id, dictionary);
  if (application.isNull()) {
    LOG4CXX_DEBUG(logger_,
                  "Application " << app_id << " with device_id " << device_id
                                 << " hasn't been found in resumption data.");
    return false;
  }
  application[strings::application_commands].clear();
  application[strings::application_submenus].clear();
  application[strings::application_choice_sets].clear();
  application[strings::application_global_properties].clear();
  application[strings::application_subscriptions].clear();
  application[strings::application_files].clear();
  application.removeMember(strings::grammar_id);
  accessor.GetMutableData().set_dictionary(dictionary);
  LOG4CXX_DEBUG(logger_,
                "Resumption data for application "
                    << app_id << " with device_id " << device_id
                    << " has been dropped.");
  return true;
}

void ResumptionDataJson::Persist() {
  last_state_wrapper_->get_accessor().GetMutableData().SaveToFileSystem();
}

LastState& ResumptionDataJson::last_state() const {
  return last_state_wrapper_->get_accessor().GetMutableData();
}

}  // namespace resumption

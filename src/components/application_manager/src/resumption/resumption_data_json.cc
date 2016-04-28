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

#if defined(OS_WINDOWS)
#include <time.h>
#endif
#include "application_manager/application_manager_impl.h"
#include "application_manager/resumption/resumption_data_json.h"
#include "smart_objects/smart_object.h"

#include "formatters/CFormatterJsonBase.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/application_manager_settings.h"

namespace resumption {

namespace Formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

CREATE_LOGGERPTR_GLOBAL(logger_, "Resumption")

ResumptionDataJson::ResumptionDataJson(
    LastState& last_state,
    const application_manager::ApplicationManager& application_manager)
    : ResumptionData(application_manager), last_state_(last_state) {}

void ResumptionDataJson::SaveApplication(
    app_mngr::ApplicationSharedPtr application) {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(application);

  const std::string& policy_app_id = application->policy_app_id();
  LOGGER_DEBUG(logger_,
               "app_id : " << application->app_id()
                           << " policy_app_id : " << policy_app_id);
  const std::string hash = application->curHash();
  const uint32_t grammar_id = application->get_grammar_id();
  const utils::json::JsonValue::UInt time_stamp =
      static_cast<uint32_t>(time(NULL));
  const std::string device_mac = application->mac_address();
  const mobile_apis::HMILevel::eType hmi_level = application->hmi_level();
  const bool is_subscribed_for_way_points =
      application_manager_.IsAppSubscribedForWayPoints(application->app_id());

  sync_primitives::AutoLock autolock(resumption_lock_);
  JsonValue tmp;
  JsonValueRef json_app = GetFromSavedOrAppend(policy_app_id, device_mac);

  json_app[strings::device_id] = device_mac;
  json_app[strings::app_id] = policy_app_id;
  json_app[strings::grammar_id] = utils::json::JsonValue::UInt(grammar_id);
  json_app[strings::connection_key] =
      utils::json::JsonValue::UInt(application->app_id());
  json_app[strings::hmi_app_id] =
      utils::json::JsonValue::UInt(application->hmi_app_id());
  json_app[strings::is_media_application] = application->IsAudioApplication();
  json_app[strings::hmi_level] = utils::json::JsonValue::Int(hmi_level);
  json_app[strings::ign_off_count] = utils::json::JsonValue::UInt(0);
  json_app[strings::suspend_count] = utils::json::JsonValue::UInt(0);
  json_app[strings::hash_id] = hash;
  Formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationCommands(application), tmp);
  json_app[strings::application_commands] = tmp;
  Formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationSubMenus(application), tmp);
  json_app[strings::application_submenus] = tmp;
  Formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationInteractionChoiseSets(application), tmp);
  json_app[strings::application_choice_sets] = tmp;
  Formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationGlobalProperties(application), tmp);
  json_app[strings::application_global_properties] = tmp;
  Formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationSubscriptions(application), tmp);
  json_app[strings::application_subscribtions] = tmp;
  Formatters::CFormatterJsonBase::objToJsonValue(
      GetApplicationFiles(application), tmp);
  json_app[strings::application_files] = tmp;
  json_app[strings::time_stamp] = time_stamp;
  json_app[strings::subscribed_for_way_points] = is_subscribed_for_way_points;
  LOGGER_DEBUG(logger_, "SaveApplication : " << json_app.ToJson());
}

int32_t ResumptionDataJson::GetStoredHMILevel(
    const std::string& policy_app_id, const std::string& device_id) const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  int idx = GetObjectIndex(policy_app_id, device_id);
  if (idx != -1) {
    const JsonValueRef json_app = GetSavedApplications()[idx];
    if (json_app.HasMember(strings::hmi_level)) {
      return json_app[strings::hmi_level].AsInt();
    }
  }
  LOGGER_FATAL(logger_, "There are some unknown keys among the stored apps");
  return -1;
}

bool ResumptionDataJson::IsHMIApplicationIdExist(uint32_t hmi_app_id) const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  const JsonValueRef saved_applications = GetSavedApplications();
  for (JsonValue::const_iterator itr = saved_applications.begin(),
                                 end = saved_applications.end();
       itr != end;
       ++itr) {
    if ((*itr).HasMember(strings::hmi_app_id)) {
      if ((*itr)[strings::hmi_app_id].AsUInt() == hmi_app_id) {
        return true;
      }
    }
  }
  return false;
}

bool ResumptionDataJson::CheckSavedApplication(const std::string& policy_app_id,
                                               const std::string& device_id) {
  using namespace app_mngr;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  int index = IsApplicationSaved(policy_app_id, device_id);
  if (-1 == index) {
    return false;
  }

  if (!IsResumptionDataValid(index)) {
    LOGGER_INFO(
        logger_,
        "Resumption data for app_id "
            << policy_app_id << " device id " << device_id
            << " is corrupted. Remove application from resumption list");
    RemoveApplicationFromSaved(policy_app_id, device_id);
    return false;
  }
  return true;
}

uint32_t ResumptionDataJson::GetHMIApplicationID(
    const std::string& policy_app_id, const std::string& device_id) const {
  using namespace app_mngr;
  using namespace utils::json;

  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  uint32_t hmi_app_id = 0;

  const int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    LOGGER_WARN(logger_, "Application not saved");
    return hmi_app_id;
  }

  const JsonValueRef json_app = GetSavedApplications()[idx];
  if (json_app.HasMember(strings::app_id) &&
      json_app.HasMember(strings::device_id)) {
    hmi_app_id = json_app[strings::hmi_app_id].AsUInt();
  }
  LOGGER_DEBUG(logger_, "hmi_app_id :" << hmi_app_id);
  return hmi_app_id;
}

void ResumptionDataJson::OnSuspend() {
  using namespace app_mngr;
  using namespace utils::json;

  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  JsonValue to_save;
  JsonValueRef saved_applications = GetSavedApplications();
  for (JsonValue::iterator itr = saved_applications.begin(),
                           end = saved_applications.end();
       itr != end;
       ++itr) {
    if ((*itr).HasMember(strings::suspend_count)) {
      const uint32_t suspend_count = (*itr)[strings::suspend_count].AsUInt();
      (*itr)[strings::suspend_count] =
          utils::json::JsonValue::UInt(suspend_count + 1);
    } else {
      LOGGER_WARN(logger_, "Unknown key among saved applications");
      (*itr)[strings::suspend_count] = utils::json::JsonValue::UInt(1);
    }
    if ((*itr).HasMember(strings::ign_off_count)) {
      const uint32_t ign_off_count = (*itr)[strings::ign_off_count].AsUInt();
      const uint32_t application_lifes = 3;     // TODO make profile variable
      if (ign_off_count < application_lifes) {  // TODO read from profile
        (*itr)[strings::ign_off_count] =
            utils::json::JsonValue::UInt(ign_off_count + 1);
      }
    } else {
      LOGGER_WARN(logger_, "Unknown key among saved applications");
      (*itr)[strings::ign_off_count] = utils::json::JsonValue::UInt(1);
    }
    to_save.Append(*itr);
  }
  SetSavedApplication(to_save);
  SetLastIgnOffTime(time(NULL));
  LOGGER_DEBUG(logger_, GetResumptionData().ToJson());
}

void ResumptionDataJson::OnAwake() {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);

  JsonValueRef saved_applications = GetSavedApplications();
  for (JsonValue::iterator itr = saved_applications.begin(),
                           end = saved_applications.end();
       itr != end;
       ++itr) {
    if ((*itr).HasMember(strings::ign_off_count)) {
      const uint32_t ign_off_count = (*itr)[strings::ign_off_count].AsUInt();
      (*itr)[strings::ign_off_count] =
          utils::json::JsonValue::UInt(ign_off_count - 1);
    } else {
      LOGGER_WARN(logger_, "Unknown key among saved applications");
      (*itr)[strings::ign_off_count] = utils::json::JsonValue::UInt(0);
    }
  }
}

bool ResumptionDataJson::GetHashId(const std::string& policy_app_id,
                                   const std::string& device_id,
                                   std::string& hash_id) const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  const int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    LOGGER_WARN(logger_, "Application not saved");
    return false;
  }

  const JsonValueRef json_app = GetSavedApplications()[idx];
  LOGGER_DEBUG(logger_, "Saved_application_data: " << json_app.ToJson());
  if (json_app.HasMember(strings::hash_id) &&
      json_app.HasMember(strings::time_stamp)) {
    hash_id = json_app[strings::hash_id].AsString();
    return true;
  }
  LOGGER_WARN(logger_, "There are some unknown keys in the dictionary.");
  return false;
}

bool ResumptionDataJson::GetSavedApplication(
    const std::string& policy_app_id,
    const std::string& device_id,
    smart_objects::SmartObject& saved_app) const {
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  const int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    return false;
  }
  const JsonValueRef json_saved_app = GetSavedApplications()[idx];
  Formatters::CFormatterJsonBase::jsonValueToObj(json_saved_app, saved_app);
  return true;
}

bool ResumptionDataJson::RemoveApplicationFromSaved(
    const std::string& policy_app_id, const std::string& device_id) {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  bool result = false;
  std::vector<JsonValue> temp;

  JsonValueRef saved_applications = GetSavedApplications();
  for (JsonValue::iterator itr = saved_applications.begin(),
                           end = saved_applications.end();
       itr != end;
       ++itr) {
    if ((*itr).HasMember(strings::app_id) &&
        (*itr).HasMember(strings::device_id)) {
      const std::string& saved_policy_app_id =
          (*itr)[strings::app_id].AsString();
      const std::string& saved_device_id =
          (*itr)[strings::device_id].AsString();
      if (saved_policy_app_id != policy_app_id &&
          saved_device_id != device_id) {
        temp.push_back((*itr));
      } else {
        result = true;
      }
    }
  }

  if (false == result) {
    LOGGER_TRACE(logger_, "EXIT result: " << (result ? "true" : "false"));
    return result;
  }

  saved_applications.Clear();
  for (std::vector<JsonValue>::iterator it = temp.begin(); it != temp.end();
       ++it) {
    saved_applications.Append((*it));
  }
  LOGGER_TRACE(logger_, "EXIT result: " << (result ? "true" : "false"));
  return result;
}

uint32_t ResumptionDataJson::GetIgnOffTime() const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  JsonValueRef resumption = GetResumptionData();
  if (!resumption.HasMember(strings::last_ign_off_time)) {
    resumption[strings::last_ign_off_time] = utils::json::JsonValue::UInt(0);
    LOGGER_WARN(logger_, "last_save_time section is missed");
  }
  return resumption[strings::last_ign_off_time].AsUInt();
}

ssize_t ResumptionDataJson::IsApplicationSaved(
    const std::string& policy_app_id, const std::string& device_id) const {
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  return GetObjectIndex(policy_app_id, device_id);
}

utils::json::JsonValueRef ResumptionDataJson::GetFromSavedOrAppend(
    const std::string& policy_app_id, const std::string& device_id) const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  JsonValueRef saved_applications = GetSavedApplications();
  for (JsonValue::iterator itr = saved_applications.begin(),
                           end = saved_applications.end();
       itr != end;
       ++itr) {
    if (device_id == (*itr)[strings::device_id].AsString() &&
        policy_app_id == (*itr)[strings::app_id].AsString()) {
      return *itr;
    }
  }

  return GetSavedApplications().Append(JsonValue());
}

void ResumptionDataJson::GetDataForLoadResumeData(
    smart_objects::SmartObject& saved_data) const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  smart_objects::SmartObject so_array_data(smart_objects::SmartType_Array);
  int i = 0;
  JsonValueRef saved_applications = GetSavedApplications();
  for (JsonValue::iterator itr = saved_applications.begin(),
                           end = saved_applications.end();
       itr != end;
       ++itr) {
    if (((*itr).HasMember(strings::hmi_level)) &&
        ((*itr).HasMember(strings::ign_off_count)) &&
        ((*itr).HasMember(strings::time_stamp)) &&
        ((*itr).HasMember(strings::app_id)) &&
        ((*itr).HasMember(strings::device_id))) {
      smart_objects::SmartObject so(smart_objects::SmartType_Map);
      so[strings::hmi_level] =
          static_cast<int32_t>((*itr)[strings::hmi_level].AsInt());
      so[strings::ign_off_count] =
          static_cast<int32_t>((*itr)[strings::ign_off_count].AsInt());
      so[strings::time_stamp] =
          static_cast<uint32_t>((*itr)[strings::time_stamp].AsUInt());
      so[strings::app_id] = (*itr)[strings::app_id].AsString();
      so[strings::device_id] = (*itr)[strings::device_id].AsString();
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
  LOGGER_AUTO_TRACE(logger_);
  using namespace app_mngr;

  int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    LOGGER_WARN(logger_,
                "Application isn't saved with mobile_app_id = "
                    << policy_app_id << " device_id = " << device_id);
    return;
  }
  GetSavedApplications()[idx][strings::hmi_level] =
      utils::json::JsonValue::Int(hmi_level);
}

utils::json::JsonValueRef ResumptionDataJson::GetSavedApplications() const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  JsonValueRef resumption = GetResumptionData();
  if (!resumption.HasMember(strings::resume_app_list)) {
    resumption[strings::resume_app_list] = JsonValue(ValueType::ARRAY_VALUE);
    LOGGER_WARN(logger_, "app_list section is missed");
  }
  JsonValueRef resume_app_list = resumption[strings::resume_app_list];
  if (!resume_app_list.IsArray()) {
    LOGGER_ERROR(logger_, "resume_app_list type INVALID rewrite");
    resume_app_list = JsonValue(ValueType::ARRAY_VALUE);
  }
  return resume_app_list;
}

utils::json::JsonValueRef ResumptionDataJson::GetResumptionData() const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  JsonValue& last_state = last_state_.dictionary();
  if (!last_state.HasMember(strings::resumption)) {
    last_state[strings::resumption] = JsonValue(ValueType::OBJECT_VALUE);
    LOGGER_WARN(logger_, "resumption section is missed");
  }
  utils::json::JsonValueRef resumption = last_state[strings::resumption];
  if (!resumption.IsObject()) {
    LOGGER_ERROR(logger_, "resumption type INVALID rewrite");
    resumption = JsonValue(ValueType::OBJECT_VALUE);
  }
  return resumption;
}

ssize_t ResumptionDataJson::GetObjectIndex(const std::string& policy_app_id,
                                           const std::string& device_id) const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  const JsonValueRef apps = GetSavedApplications();
  const JsonValue::ArrayIndex size = apps.Size();
  JsonValue::ArrayIndex idx = 0;
  for (; idx != size; ++idx) {
    if (apps[idx].HasMember(strings::app_id) &&
        apps[idx].HasMember(strings::device_id)) {
      const std::string& saved_app_id = apps[idx][strings::app_id].AsString();
      const std::string& saved_device_id =
          apps[idx][strings::device_id].AsString();
      if (device_id == saved_device_id && policy_app_id == saved_app_id) {
        LOGGER_DEBUG(logger_, "Found " << idx);
        return idx;
      }
    }
  }
  return -1;
}

bool ResumptionDataJson::IsResumptionDataValid(uint32_t index) const {
  using namespace app_mngr;
  using namespace utils::json;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  const JsonValueRef json_app = GetSavedApplications()[index];
  if (!json_app.HasMember(strings::app_id) ||
      !json_app.HasMember(strings::ign_off_count) ||
      !json_app.HasMember(strings::hmi_level) ||
      !json_app.HasMember(strings::hmi_app_id) ||
      !json_app.HasMember(strings::time_stamp) ||
      !json_app.HasMember(strings::device_id)) {
    LOGGER_ERROR(logger_, "Wrong resumption data");
    return false;
  }

  if (json_app.HasMember(strings::hmi_app_id) &&
      0 >= json_app[strings::hmi_app_id].AsUInt()) {
    LOGGER_ERROR(logger_, "Wrong resumption hmi app ID");
    return false;
  }

  return true;
}

void ResumptionDataJson::SetSavedApplication(
    utils::json::JsonValueRef apps_json) {
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  utils::json::JsonValueRef app_list = GetSavedApplications();
  app_list = apps_json;
}

void ResumptionDataJson::SetLastIgnOffTime(time_t ign_off_time) {
  using namespace app_mngr;
  LOGGER_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  LOGGER_WARN(logger_, "ign_off_time = " << ign_off_time);
  utils::json::JsonValueRef resumption = GetResumptionData();
  resumption[strings::last_ign_off_time] =
      utils::json::JsonValue::UInt(ign_off_time);
}

bool ResumptionDataJson::Init() {
  LOGGER_AUTO_TRACE(logger_);
  return true;
}

bool ResumptionDataJson::DropAppDataResumption(const std::string& device_id,
                                               const std::string& app_id) {
  LOGGER_AUTO_TRACE(logger_);
  using namespace app_mngr;
  sync_primitives::AutoLock autolock(resumption_lock_);
  utils::json::JsonValueRef application =
      GetFromSavedOrAppend(app_id, device_id);
  if (application.IsNull()) {
    LOGGER_DEBUG(logger_,
                 "Application " << app_id << " with device_id " << device_id
                                << " hasn't been found in resumption data.");
    return false;
  }
  application[strings::application_commands].Clear();
  application[strings::application_submenus].Clear();
  application[strings::application_choice_sets].Clear();
  application[strings::application_global_properties].Clear();
  application[strings::application_subscribtions].Clear();
  application[strings::application_files].Clear();
  // Seems there is no interface for json wrapper - needs to be created
  // application.removeMember(strings::grammar_id);
  application[strings::grammar_id].Clear();
  LOGGER_DEBUG(logger_,
               "Resumption data for application "
                   << app_id << " with device_id " << device_id
                   << " has been dropped.");
  return true;
}

void ResumptionDataJson::Persist() {
    last_state().SaveToFileSystem();
}


}  // resumption

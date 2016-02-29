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

#include "application_manager/resumption/resumption_data_json.h"
#include "smart_objects/smart_object.h"
#include "json/json.h"
#include "formatters/CFormatterJsonBase.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "resumption/last_state.h"
#include "config_profile/profile.h"

namespace resumption {

namespace Formatters = NsSmartDeviceLink::NsJSONHandler::Formatters;

CREATE_LOGGERPTR_GLOBAL(logger_, "Resumption")

ResumptionDataJson::ResumptionDataJson() : ResumptionData() {}

void ResumptionDataJson::SaveApplication(
    app_mngr::ApplicationSharedPtr application) {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  DCHECK_OR_RETURN_VOID(application);

  const std::string& policy_app_id = application->mobile_app_id();
  LOG4CXX_DEBUG(logger_,
                "app_id : " << application->app_id() << " policy_app_id : "
                            << policy_app_id);
  const std::string hash = application->curHash();
  const uint32_t grammar_id = application->get_grammar_id();
  const uint32_t time_stamp = (uint32_t)time(NULL);
  const std::string device_mac = application->mac_address();
  const mobile_apis::HMILevel::eType hmi_level = application->hmi_level();

  sync_primitives::AutoLock autolock(resumption_lock_);
  Json::Value tmp;
  Json::Value& json_app = GetFromSavedOrAppend(policy_app_id, device_mac);

  json_app[strings::device_id] = device_mac;
  json_app[strings::app_id] = policy_app_id;
  json_app[strings::grammar_id] = grammar_id;
  json_app[strings::connection_key] = application->app_id();
  json_app[strings::hmi_app_id] = application->hmi_app_id();
  json_app[strings::is_media_application] = application->IsAudioApplication();
  json_app[strings::hmi_level] = static_cast<int32_t>(hmi_level);
  json_app[strings::ign_off_count] = 0;
  json_app[strings::suspend_count] = 0;
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
  LOG4CXX_DEBUG(logger_, "SaveApplication : " << json_app.toStyledString());
}

int32_t ResumptionDataJson::GetStoredHMILevel(
    const std::string& policy_app_id, const std::string& device_id) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  int idx = GetObjectIndex(policy_app_id, device_id);
  if (idx != -1) {
    const Json::Value& json_app = GetSavedApplications()[idx];
    if (json_app.isMember(strings::hmi_level)) {
      return json_app[strings::hmi_level].asInt();
    }
  }
  LOG4CXX_FATAL(logger_, "There are some unknown keys among the stored apps");
  return -1;
}

bool ResumptionDataJson::IsHMIApplicationIdExist(uint32_t hmi_app_id) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  for (Json::Value::iterator it = GetSavedApplications().begin();
       it != GetSavedApplications().end();
       ++it) {
    if ((*it).isMember(strings::hmi_app_id)) {
      if ((*it)[strings::hmi_app_id].asUInt() == hmi_app_id) {
        return true;
      }
    }
  }
  return false;
}

bool ResumptionDataJson::CheckSavedApplication(const std::string& policy_app_id,
                                               const std::string& device_id) {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  int index = IsApplicationSaved(policy_app_id, device_id);
  if (-1 == index) {
    return false;
  }

  if (!IsResumptionDataValid(index)) {
    LOG4CXX_INFO(
        logger_,
        "Resumption data for app_id "
            << policy_app_id
            << " device id "
            << device_id
            << " is corrupted. Remove application from resumption list");
    RemoveApplicationFromSaved(policy_app_id, device_id);
    return false;
  }
  return true;
}

uint32_t ResumptionDataJson::GetHMIApplicationID(
    const std::string& policy_app_id, const std::string& device_id) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  uint32_t hmi_app_id = 0;

  const int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return hmi_app_id;
  }

  const Json::Value& json_app = GetSavedApplications()[idx];
  if (json_app.isMember(strings::app_id) &&
      json_app.isMember(strings::device_id)) {
    hmi_app_id = json_app[strings::hmi_app_id].asUInt();
  }
  LOG4CXX_DEBUG(logger_, "hmi_app_id :" << hmi_app_id);
  return hmi_app_id;
}

void ResumptionDataJson::OnSuspend() {
  using namespace app_mngr;
  using namespace profile;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  Json::Value to_save;
  for (Json::Value::iterator it = GetSavedApplications().begin();
       it != GetSavedApplications().end();
       ++it) {
    if ((*it).isMember(strings::suspend_count)) {
      const uint32_t suspend_count = (*it)[strings::suspend_count].asUInt();
      (*it)[strings::suspend_count] = suspend_count + 1;
    } else {
      LOG4CXX_WARN(logger_, "Unknown key among saved applications");
      (*it)[strings::suspend_count] = 1;
    }
    if ((*it).isMember(strings::ign_off_count)) {
      Json::Value& ign_off_count = (*it)[strings::ign_off_count];
      const uint32_t counter_value = ign_off_count.asUInt();
      ign_off_count = counter_value + 1;
    } else {
      LOG4CXX_WARN(logger_, "Unknown key among saved applications");
      Json::Value& ign_off_count = (*it)[strings::ign_off_count];
      ign_off_count = 1;
    }
    to_save.append(*it);
  }
  SetSavedApplication(to_save);
  SetLastIgnOffTime(time(NULL));
  LOG4CXX_DEBUG(logger_, GetResumptionData().toStyledString());
  ::resumption::LastState::instance()->SaveToFileSystem();
}

void ResumptionDataJson::OnAwake() {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);

  for (Json::Value::iterator it = GetSavedApplications().begin();
       it != GetSavedApplications().end();
       ++it) {
    if ((*it).isMember(strings::ign_off_count)) {
      const uint32_t ign_off_count = (*it)[strings::ign_off_count].asUInt();
      (*it)[strings::ign_off_count] = ign_off_count - 1;
    } else {
      LOG4CXX_WARN(logger_, "Unknown key among saved applications");
      (*it)[strings::ign_off_count] = 0;
    }
  }
}

bool ResumptionDataJson::GetHashId(const std::string& policy_app_id,
                                   const std::string& device_id,
                                   std::string& hash_id) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  const int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    LOG4CXX_WARN(logger_, "Application not saved");
    return false;
  }

  const Json::Value& json_app = GetSavedApplications()[idx];
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
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  const int idx = GetObjectIndex(policy_app_id, device_id);
  if (-1 == idx) {
    return false;
  }
  const Json::Value& json_saved_app = GetSavedApplications()[idx];
  Formatters::CFormatterJsonBase::jsonValueToObj(json_saved_app, saved_app);
  return true;
}

bool ResumptionDataJson::RemoveApplicationFromSaved(
    const std::string& policy_app_id, const std::string& device_id) {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  bool result = false;
  std::vector<Json::Value> temp;
  for (Json::Value::iterator it = GetSavedApplications().begin();
       it != GetSavedApplications().end();
       ++it) {
    if ((*it).isMember(strings::app_id) && (*it).isMember(strings::device_id)) {
      const std::string& saved_policy_app_id =
          (*it)[strings::app_id].asString();
      const std::string& saved_device_id = (*it)[strings::device_id].asString();
      if (saved_policy_app_id != policy_app_id &&
          saved_device_id != device_id) {
        temp.push_back((*it));
      } else {
        result = true;
      }
    }
  }

  if (false == result) {
    LOG4CXX_TRACE(logger_, "EXIT result: " << (result ? "true" : "false"));
    return result;
  }

  GetSavedApplications().clear();
  for (std::vector<Json::Value>::iterator it = temp.begin(); it != temp.end();
       ++it) {
    GetSavedApplications().append((*it));
  }
  LOG4CXX_TRACE(logger_, "EXIT result: " << (result ? "true" : "false"));
  return result;
}

uint32_t ResumptionDataJson::GetIgnOffTime() const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  Json::Value& resumption = GetResumptionData();
  if (!resumption.isMember(strings::last_ign_off_time)) {
    resumption[strings::last_ign_off_time] = 0;
    LOG4CXX_WARN(logger_, "last_save_time section is missed");
  }
  return resumption[strings::last_ign_off_time].asUInt();
}

ssize_t ResumptionDataJson::IsApplicationSaved(
    const std::string& policy_app_id, const std::string& device_id) const {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  return GetObjectIndex(policy_app_id, device_id);
}

Json::Value& ResumptionDataJson::GetFromSavedOrAppend(
    const std::string& policy_app_id, const std::string& device_id) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  for (Json::Value::iterator it = GetSavedApplications().begin();
       it != GetSavedApplications().end();
       ++it) {
    if (device_id == (*it)[strings::device_id].asString() &&
        policy_app_id == (*it)[strings::app_id].asString()) {
      return *it;
    }
  }

  return GetSavedApplications().append(Json::Value());
}

void ResumptionDataJson::GetDataForLoadResumeData(
    smart_objects::SmartObject& saved_data) const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  smart_objects::SmartObject so_array_data(smart_objects::SmartType_Array);
  int i = 0;
  for (Json::Value::iterator it = GetSavedApplications().begin();
       it != GetSavedApplications().end();
       ++it) {
    if (((*it).isMember(strings::hmi_level)) &&
        ((*it).isMember(strings::ign_off_count)) &&
        ((*it).isMember(strings::time_stamp)) &&
        ((*it).isMember(strings::app_id)) &&
        ((*it).isMember(strings::device_id))) {
      smart_objects::SmartObject so(smart_objects::SmartType_Map);
      so[strings::hmi_level] = (*it)[strings::hmi_level].asInt();
      so[strings::ign_off_count] = (*it)[strings::ign_off_count].asInt();
      so[strings::time_stamp] = (*it)[strings::time_stamp].asUInt();
      so[strings::app_id] = (*it)[strings::app_id].asString();
      so[strings::device_id] = (*it)[strings::device_id].asString();
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
    LOG4CXX_WARN(
        logger_,
        "Application isn't saved with mobile_app_id = " << policy_app_id
                                                        << " device_id = "
                                                        << device_id);
    return;
  }
  GetSavedApplications()[idx][strings::hmi_level] = hmi_level;
}

Json::Value& ResumptionDataJson::GetSavedApplications() const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  Json::Value& resumption = GetResumptionData();
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

Json::Value& ResumptionDataJson::GetResumptionData() const {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  Json::Value& last_state = ::resumption::LastState::instance()->dictionary;
  if (!last_state.isMember(strings::resumption)) {
    last_state[strings::resumption] = Json::Value(Json::objectValue);
    LOG4CXX_WARN(logger_, "resumption section is missed");
  }
  Json::Value& resumption = last_state[strings::resumption];
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
  sync_primitives::AutoLock autolock(resumption_lock_);
  const Json::Value& apps = GetSavedApplications();
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
  sync_primitives::AutoLock autolock(resumption_lock_);
  const Json::Value& json_app = GetSavedApplications()[index];
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

void ResumptionDataJson::SetSavedApplication(Json::Value& apps_json) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  Json::Value& app_list = GetSavedApplications();
  app_list = apps_json;
}

void ResumptionDataJson::SetLastIgnOffTime(time_t ign_off_time) {
  using namespace app_mngr;
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock autolock(resumption_lock_);
  LOG4CXX_WARN(logger_, "ign_off_time = " << ign_off_time);
  Json::Value& resumption = GetResumptionData();
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
  sync_primitives::AutoLock autolock(resumption_lock_);
  Json::Value& application = GetFromSavedOrAppend(app_id, device_id);
  if (application.isNull()) {
    LOG4CXX_DEBUG(logger_, "Application " << app_id << " with device_id "
                  << device_id << " hasn't been found in resumption data.");
    return false;
  }
  application[strings::application_commands].clear();
  application[strings::application_submenus].clear();
  application[strings::application_choice_sets].clear();
  application[strings::application_global_properties].clear();
  application[strings::application_subscribtions].clear();
  application[strings::application_files].clear();
  application.removeMember(strings::grammar_id);
  LOG4CXX_DEBUG(logger_, "Resumption data for application " << app_id <<
                " with device_id " << device_id << " has been dropped.");
  return true;
}


} // resumption

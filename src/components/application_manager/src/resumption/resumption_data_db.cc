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
#include "application_manager/resumption/resumption_data_db.h"
#include "application_manager/resumption/resumption_sql_queries.h"
#if __QNX__
#  include "qdb_wrapper/sql_database.h"
#  include "qdb_wrapper/sql_query.h"
#else  // __QNX__
#  include "sqlite_wrapper/sql_database.h"
#  include "sqlite_wrapper/sql_query.h"
#endif  // __QNX__

namespace application_manager {
namespace resumption {
CREATE_LOGGERPTR_GLOBAL(logger_, "ResumptionDataDB")

int ResumptionDataDB::GetStoredHMILevel(const std::string& m_app_id,
                                        const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  int hmi_level;
  if (SelectHMILevel(m_app_id, device_id, hmi_level)) {
    LOG4CXX_INFO(logger_, "Application with mobile application id  = "<<m_app_id
                 <<" and device id = "<<device_id<<"has hmi level = "<<hmi_level);
    return hmi_level;
  }
  LOG4CXX_FATAL(logger_, "HMI level doesn't exists in saved data");
  return -1;
}

bool ResumptionDataDB::IsHMIApplicationIdExist(uint32_t hmi_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  return CheckExistenceHMIId(hmi_app_id);
}

bool ResumptionDataDB::CheckSavedApplication(const std::string& mobile_app_id,
                                             const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!CheckExistenceApplication(mobile_app_id, device_id)) {
    LOG4CXX_INFO(logger_, "Application with mobile_app_id = "<<mobile_app_id
                 <<" and device_id = "<<device_id<<" does not exist");
    return false;
  }
  return true;
}

uint32_t ResumptionDataDB::GetHMIApplicationID(const std::string& mobile_app_id,
                                               const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t hmi_app_id = 0;
  SelectHMIId(mobile_app_id, device_id, hmi_app_id);
  return hmi_app_id;
}

void ResumptionDataDB::Suspend() {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query_delete_applications(db());
  utils::dbms::SQLQuery query_update_suspend_data(db());
  utils::dbms::SQLQuery query_update_last_ign_off_time(db());

  if (query_delete_applications.Prepare(kDeleteApplicationsAccordingWithIgnOffCount)) {
    query_delete_applications.Bind(0, static_cast<int>(kApplicationLifes));
    if (query_delete_applications.Exec()) {
      LOG4CXX_INFO(logger_, "Saved application with ign_off_count = "<<kApplicationLifes
                   <<" was deleted");
    }
  }

  if (query_update_suspend_data.Prepare(kUpdateSuspendData)) {
    if (query_update_suspend_data.Exec()) {
      LOG4CXX_INFO(logger_, "Data ign_off_count and suspend_count was updated");
    }
  }

  if (query_update_last_ign_off_time.Prepare(KUpdateLastIgnOffTime)) {
    query_update_lals_ign_off_time.Bind(0, static_cast<uint32_t>(time(NULL)));
    if (query_update_last_ign_off_time.Exec()) {
      LOG4CXX_INFO(logger_, "Data last_ign_off_time was updated");
    }
  }
}


bool ResumptionDataDB::GetHashId(const std::string& mobile_app_id,
                                 const std::string& device_id,
                                 std::string& hash_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  return SelectHashId(mobile_app_id, device_id, hash_id);
}

void ResumptionDataDB::OnAwake() {
  LOG4CXX_AUTO_TRACE(logger_);

  UpdateDataOnAwake();
}

bool ResumptionDataDB::RemoveApplicationFromSaved(
    const std::string& mobile_app_id, const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  return DeleteSavedApplication(mobile_app_id, device_id);
}

uint32_t ResumptionDataDB::GetIgnOffTime() {
  LOG4CXX_AUTO_TRACE(logger_);

  return SelectIgnOffTime();
}

int ResumptionDataDB::IsApplicationSaved(const std::string& mobile_app_id,
                                         const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (CheckExistenceApplication(mobile_app_id, device_id)) {
    return 0;
  }
  return -1;
}

void ResumptionDataDB::GetDataForLoadResumeData(smart_objects::SmartObject& saved_data) {
  LOG4CXX_AUTO_TRACE(logger_);
  SelectDataForLoadResumeData(saved_data);
}

void ResumptionDataDB::SetHMILevelForSavedApplication(const std::string& mobile_app_id,
                                                      const std::string& device_id, int32_t hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);
  UpdateHmiLevel(mobile_app_id, device_id, hmi_level);
}

bool ResumptionDataDB::SelectHMILevel(const std::string& m_app_id,
                                      const std::string& device_id,
                                      int& hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kSelectHMILevel)) {
    query.Bind(0, device_id);
    query.Bind(1, m_app_id);
    if (query.Exec() && !(query.IsNull(0))) {
      hmi_level = query.GetInteger(0);
      return true;
    }
  }
  return false;
}

bool ResumptionDataDB::CheckExistenceHMIId(const uint32_t hmi_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kCheckHMIId)) {
    query.Bind(0, hmi_app_id);
    if (query.Exec() && !(query.IsNull(0))) {
      LOG4CXX_INFO(logger_, "Saved data has HMI appID");
      return true;
    }
  }
  LOG4CXX_FATAL(logger_, "HMI appID = " <<hmi_app_id
                <<" doesn't exist in saved data");
  return false;
}

void ResumptionDataDB::SelectHMIId(const std::string& mobile_app_id,
                                   const std::string& device_id,
                                   uint32_t& hmi_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kSelectHMIId)) {
    query.Bind(0, device_id);
    query.Bind(1, mobile_app_id);
    if (query.Exec() && !(query.IsNull(0))) {
      hmi_id = query.GetInteger(0);
      LOG4CXX_INFO(logger_, "Saved HMI appID = "<<hmi_id);
    }
  }
  LOG4CXX_FATAL(logger_, "Saved data doesn't have application with "
                "device id = "<<device_id<<" and mobile appID = "<<mobi_app_id
                "or hmi id"<<);
}

bool ResumptionDataDB::SelectHashId(const std::string& mobile_app_id,
                  const std::string& device_id,
                  std::string& hash_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kSelectHashId)) {
    query.Bind(0, device_id);
    query.Bind(1, mobile_app_id);
    if (query.Exec() && !(query.IsNull(0))) {
      hash_id = query.GetString(0);
      LOG4CXX_INFO(logger_, "Saved hash ID = "<<hash_id);
      return true;
    }
  }
  LOG4CXX_FATAL(logger_, "Saved data doesn't have application with "
                "device id = "<<device_id<<" and mobile appID = "<<mobi_app_id
                "or hashID");
  return false;
}

uint32_t ResumptionDataDB::SelectIgnOffTime() {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t ignOffTime = 0;
  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kSelectIgnOffTime)) {
    if (query.Exec()) {
      ignOffTime = query.GetUInteger(0);
      LOG4CXX_INFO(logger_, "Last ign off time = "<<ignOffTime);
    }
  }
  LOG4CXX_FATAL(logger_, "Problem with prepare query");
}

bool ResumptionDataDB::CheckExistenceApplication(const std::string& mobile_app_id,
                                                 const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kCheckApplication)) {
    query.Bind(0, device_id);
    query.Bind(1, mobile_app_id);
    if (query.Exec() && !(query.IsNull(0)) && query.GetInteger(0)) {
      LOG4CXX_INFO(logger_, "Saved data has application with mobile appID = "
                   <<mobile_app_id<<" and deviceID = "<<device_id);
      return true;
    }
  }
  LOG4CXX_FATAL(logger_, "Saved data does not contain application");
  return false;
}

void ResumptionDataDB::SelectDataForLoadResumeData(
    smart_objects::SmartObject& saved_data) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kSelectDataForLoadResumeData)) {
    smart_objects::SmartObject so_array_data(smart_objects::SmartType_Array);
    int i = 0;
    while(query.Next()) {
      smart_objects::SmartObject so_obj(smart_objects::SmartType_Map);
      so_obj[strings::hmi_level] = query.GetInteger(0);
      so_obj[strings::ign_off_count] = query.GetInteger(1);
      so_obj[strings::time_stamp] = query.GetUInteger(2);
      so_obj[strings::app_id] = query.GetString(3);
      so_obj[strings::device_id] = query.GetString(4);
      so_array_data[i++] = so_obj;
    }
  saved_data = so_array_data;
  }
}

void ResumptionDataDB::UpdateHmiLevel(const std::string& mobile_app_id,
                                      const std::string& device_id,
                                      int32_t hmi_level) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kUpdateHMILevel)) {
    query.Bind(0, hmi_level);
    query.Bind(1, device_id);
    query.Bind(2, mobile_app_id);
    if (query.Exec()) {
      LOG4CXX_INFO(logger_, "Saved data has application with mobile appID = "
                   <<mobile_app_id<<" and deviceID = "<<device_id
                   <<" has new HMI level = "<<hmi_level);

    }
  }
}

bool ResumptionDataDB::DeleteSavedApplication(const std::string& mobile_app_id,
                                              const std::string& device_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kDeleteApplication)) {
    query.Bind(0, device_id);
    query.Bind(1, mobile_app_id);
    if (query.Exec()) {
      LOG4CXX_INFO(logger_, "Data of application with mobile appID = "
                   <<mobile_app_id<<" and deviceID = "<<device_id
                   <<" was removed successfully");
      return true;
    }
  }
  LOG4CXX_INFO(logger_, "Data of application with mobile appID = "
               <<mobile_app_id<<" and deviceID = "<<device_id
               <<" was not removed");
  return false;
}

void ResumptionDataDB::UpdateDataOnAwake() {
  LOG4CXX_AUTO_TRACE(logger_);

  utils::dbms::SQLQuery query(db());
  if (query.Prepare(kUpdateIgnOffCount)) {
    if (query.Exec()) {
      LOG4CXX_INFO(logger_, "Values of ignition off counts were updated successfully");
    }
  }
}



}  // namespace resumption
}  // namespace application_manager

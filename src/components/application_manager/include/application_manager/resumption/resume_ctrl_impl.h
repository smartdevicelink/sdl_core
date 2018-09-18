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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUME_CTRL_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUME_CTRL_IMPL_H_

#include "application_manager/resumption/resume_ctrl.h"

#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"
#include "smart_objects/smart_object.h"
#include "application_manager/application.h"
#include "application_manager/resumption/resumption_data.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "utils/timer.h"

namespace resumption {

class LastState;

/**
 * @brief Contains logic for storage/restore data of applications.
 */

class ResumeCtrlImpl : public ResumeCtrl {
 public:
  /**
   * @brief allows to create ResumeCtrlImpl object
   */
  ResumeCtrlImpl(application_manager::ApplicationManager& application_manager);

  /**
   * @brief allows to destroy ResumeCtrlImpl object
   */
  ~ResumeCtrlImpl();

  /**
   * @brief Save all applications info to the file system
   */
  void SaveAllApplications() OVERRIDE;

  /**
   * @brief Save application persistent info for future resuming
   * @param application is application witch need to be saved
   */
  void SaveApplication(app_mngr::ApplicationSharedPtr application) OVERRIDE;

  /**
   * @brief Set application HMI Level and ausio_state as saved
   * @param application is application witch HMI Level is need to restore
   * @return true if success, otherwise return false
   */
  bool RestoreAppHMIState(app_mngr::ApplicationSharedPtr application) OVERRIDE;

  /**
   * @brief Set application HMI Level as stored in policy
   * @param application is application witch HMI Level is need to setup
   * @return true if success, otherwise return false
   */
  bool SetupDefaultHMILevel(
      app_mngr::ApplicationSharedPtr application) OVERRIDE;

  /**
   * @brief Setup HmiLevel for application
   * Do routine of setting up hmi_level
   * @param application is application witch HMI Level is need to setup
   * @param hmi_level HMI Level is needed to setup
   * @param hmi_level AudioStreamingState is needed to setup
   * @param check_policy indicate if policy data consent must be verified
   * @return true if success, otherwise return false
   */
  bool SetAppHMIState(app_mngr::ApplicationSharedPtr application,
                      const mobile_apis::HMILevel::eType hmi_level,
                      bool check_policy = true) OVERRIDE;

  /**
   * @brief Remove application from list of saved applications
   * @param application is application which need to be removed
   * @return return true, if success, otherwise return false
   */
  bool RemoveApplicationFromSaved(
      app_mngr::ApplicationConstSharedPtr application) OVERRIDE;

  /**
   * @brief Processes resumption data after receiving signal "Suspend"
   */
  void OnSuspend() OVERRIDE;

  /**
   * @brief Processes resumption data after receiving signal "Ignition Off"
   */
  void OnIgnitionOff() OVERRIDE;

  /**
   * @brief Processes resumption data after receiving signal "Awake"
   */
  void OnAwake() OVERRIDE;

  /**
   * @brief Checks if SDL has already received OnExitAllApplication notification
   * with "SUSPEND" reason
   *
   * @return Returns TRUE if SDL has received OnExitAllApplication notification
   * with reason "SUSPEND" otherwise returns FALSE
   */
  bool is_suspended() const OVERRIDE;

  /**
   * @brief Method stops timer "RsmCtrlPercist" when SDL
   * receives OnExitAllApplication notification
   * with reason "SUSPEND"
   */
  void StopSavePersistentDataTimer() OVERRIDE;

  /**
   * @brief Method stops restore_hmi_level_timer_ "RsmCtrlRstore" in OnSuspend()
   */
  void StopRestoreHmiLevelTimer();

  /**
   * @brief Start timer for resumption applications
   *        Restore D1-D5 data
   * @param application that is need to be restored
   * @return true if it was saved, otherwise return false
   */
  bool StartResumption(app_mngr::ApplicationSharedPtr application,
                       const std::string& hash,
                       ResumptionCallBack callback) OVERRIDE;

  void HandleOnTimeOut(const uint32_t correlation_id,
                       const hmi_apis::FunctionID::eType) OVERRIDE;

  /**
   * @brief Start timer for resumption applications
   *        Does not restore D1-D5 data
   * @param application that is need to be restored
   * @return true if it was saved, otherwise return false
   */
  bool StartResumptionOnlyHMILevel(
      app_mngr::ApplicationSharedPtr application) OVERRIDE;

  /**
   * @brief Retry resumption of an app if it has been disabled or limited
   *        due to absence of high-bandwidth transport.
   * @param app_id ID of the app to resume
   */
  void RetryResumption(const uint32_t app_id) OVERRIDE;

  /**
   * @brief Check if there are all files need for resumption
   * @param application that is need to be restored
   * @return true if it all files exist, otherwise return false
   */
  bool CheckPersistenceFilesForResumption(
      app_mngr::ApplicationSharedPtr application) OVERRIDE;

  /**
   * @brief Check application hash
   * @param application that is need to be restored
   * @return true if it was saved, otherwise return false
   */
  bool CheckApplicationHash(app_mngr::ApplicationSharedPtr application,
                            const std::string& hash) OVERRIDE;

  /**
   * @brief Checks if Resume controller have saved application with hmi app id
   * @param hmi_app_id - hmi application id
   * @return true if exist, false otherwise
   */
  bool IsHMIApplicationIdExist(uint32_t hmi_app_id) OVERRIDE;

  /**
   * @brief Check if Resume controller have saved instance of application
   * @param policy_app_id - mobile application id
   * @param device_id - id of device where application is run
   * @return true if exist, false otherwise
   */
  bool IsApplicationSaved(const std::string& policy_app_id,
                          const std::string& device_id) OVERRIDE;

  /**
   * @brief Function is used for application resume. HMI app ID must be
   * the same(PASA VCA module use it for stored app info).
   * Retrieves HMI app ID for the given policy app ID from stored information.
   * @param policy_app_id - mobile application id
   * @param device_id - id of device where application is run
   * @return HMI app ID
   */
  uint32_t GetHMIApplicationID(const std::string& policy_app_id,
                               const std::string& device_id) const OVERRIDE;

  /**
   * @brief Updates flag for saving application data
   */
  void ApplicationsDataUpdated() OVERRIDE {
    is_data_saved_ = false;
  }

  /**
   * @brief Resume HMI Level and audio streaming state if needed
   * @param application - application to restore hmi level
   * and audio streaming state
   */
  void StartAppHmiStateResumption(
      app_mngr::ApplicationSharedPtr application) OVERRIDE;

  /**
   * @brief Update launch_time_ to current
   */
  void ResetLaunchTime() OVERRIDE;

  /**
   * @brief Removes activated application from resumption list
   *
   * @param application application witch need to be removed from resumption
   */
  void OnAppActivated(app_mngr::ApplicationSharedPtr application) OVERRIDE;

  /**
   * @brief Removes app from resumption list
   *
   * app_id Application to remove
   */
  void RemoveFromResumption(uint32_t app_id) OVERRIDE;

  /**
   * @brief Initialization data for Resume controller
   * @return true if initialization is success otherwise
   * returns false
   */
  bool Init(LastState& last_state) OVERRIDE;

  /**
   * @brief Notify resume controller about new application
   * @param policy_app_id - mobile application id
   * @param device_id - id of device where application is run
   */
  void OnAppRegistrationStart(const std::string& policy_app_id,
                              const std::string& device_id) OVERRIDE;

  /**
   * @brief Notify resume controller about delete new application
   */
  void OnAppRegistrationEnd() OVERRIDE;

  int32_t GetSavedAppHmiLevel(const std::string& app_id,
                              const std::string& device_id) const OVERRIDE;

  /**
   * @brief geter for launch_time_
   * @return value of launch_time_
   */
  time_t LaunchTime() const OVERRIDE;

  /**
   * @brief Timer callback for  restoring HMI Level
   *
   */
  void ApplicationResumptiOnTimer();

  /**
   * @brief Method starts timer "RsmCtrlPercist" when
   * SDL receives onAwakeSDL notification
   */
  void StartSavePersistentDataTimer();

#ifdef BUILD_TESTS
  void set_resumption_storage(
      std::shared_ptr<ResumptionData> mock_storage) OVERRIDE;

  bool get_resumption_active() const OVERRIDE;
#endif  // BUILD_TESTS
 private:
  /**
   * @brief restores saved data of application
   * @param application contains application for which restores data
   * @return true if success, otherwise return false
   */
  bool RestoreApplicationData(app_mngr::ApplicationSharedPtr application,
                              ResumptionCallBack callback);

  /**
   * @brief SaveDataOnTimer :
   *  Timer callback for persisting ResumptionData each N seconds
   *  N gets from property
   */
  void SaveDataOnTimer();

  /**
   * @brief FinalPersistData persists ResumptionData last time and stops
   * persistent data timer to avoid further persisting
   */
  void FinalPersistData();

  bool CheckIgnCycleRestrictions(const smart_objects::SmartObject& saved_app);

  bool DisconnectedJustBeforeIgnOff(
      const smart_objects::SmartObject& saved_app);

  bool CheckAppRestrictions(app_mngr::ApplicationConstSharedPtr application,
                            const smart_objects::SmartObject& saved_app);

  /**
   * @brief CheckIcons allows to check application icons
   * @param application application under resumtion  application
   * @param json_object
   * @return true in case icons exists, false otherwise
   */
  bool CheckIcons(app_mngr::ApplicationSharedPtr application,
                  smart_objects::SmartObject& obj);

  /**
   * @brief CheckDelayAfterIgnOn should check if SDL was started less
   * then N seconds ago. N will be readed from profile.
   * @return true if SDL started N seconds ago, otherwise return false
   */
  bool CheckDelayAfterIgnOn();

  typedef std::pair<uint32_t, uint32_t> application_timestamp;

  std::set<app_mngr::ApplicationSharedPtr> retrieve_application();

  /**
   * @brief This struct need to map
   * timestamp and application from correlationID
   */
  struct ResumingApp {
    uint32_t old_session_key;  // session key is the same as app_id
    app_mngr::ApplicationSharedPtr app;
  };

  struct TimeStampComparator {
    bool operator()(const application_timestamp& lhs,
                    const application_timestamp& rhs) const {
      return lhs.second < rhs.second;
    }
  };

  /**
   * @brief Check device MAC address
   * @param application that is need to be restored
   * @param saved_device_mac Saved device MAC address
   * @return TRUE on success, otherwise FALSE
   */
  bool IsDeviceMacAddressEqual(app_mngr::ApplicationSharedPtr application,
                               const std::string& saved_device_mac);

  /**
   * @brief Get the last ignition off time from LastState
   * @return the last ignition off time from LastState
   */
  time_t GetIgnOffTime();

  /**
   * @brief Setup IgnOff time to LastState
   * @param ign_off_time - igition off time
   */
  void SetLastIgnOffTime(time_t ign_off_time);

  void InsertToTimerQueue(uint32_t app_id, uint32_t time_stamp);

  /**
   * @brief Add application to queue to restore HMI level
   * @param time_stamp contains time when application was stored to resumption
   * data
   * @param app_id contains id of application
   */
  void AddToResumptionTimerQueue(const uint32_t app_id);

  void LoadResumeData();

  /**
   * @brief Checks, if application data needs to be resumed
   * @param application Application data from storage
   * @return true, if data resumption must be skipped, otherwise - false
   */
  bool IsAppDataResumptionExpired(
      const smart_objects::SmartObject& application) const;

  /**
   * @brief Checks from resume data, if application has been disconnected
   * unexpectedly
   * @param app Application section from resume data
   * @return true, if it has been unexpectedly disconnected, otherwise - false
   */
  bool IsUnexpectedlyDisconnected(const smart_objects::SmartObject& app) const;

  /**
   * @brief Checks, if application can be resumed
   * @param application Application
   * @param time_stamp contain time when application was stored to resumption
   * data
   * @return true, if no restrictions currently, otherwise - false
   */
  bool IsResumeAllowed(
      const application_manager::ApplicationSharedPtr application) const;

  /**
   * @brief Retrieve the HMI level of the app when high-bandwidth transport
   *isn't available
   *
   * The value is configured through smartDeviceLink.ini file
   *
   * @param application an instance of the app
   * @return HMI level that the app is allowed when high-bandwidth transport
   *isn't available
   */
  mobile_apis::HMILevel::eType GetHmiLevelOnLowBandwidthTransport(
      app_mngr::ApplicationConstSharedPtr application) const;

  /**
   *@brief Mapping applications to time_stamps
   *       wait for timer to resume HMI Level
   *
   */
  mutable sync_primitives::Lock queue_lock_;
  timer::Timer restore_hmi_level_timer_;
  timer::Timer save_persistent_data_timer_;
  typedef std::list<uint32_t> WaitingForTimerList;
  WaitingForTimerList waiting_for_timer_;
  bool is_resumption_active_;
  bool is_data_saved_;
  bool is_suspended_;
  time_t launch_time_;
  std::shared_ptr<ResumptionData> resumption_storage_;
  application_manager::ApplicationManager& application_manager_;
  ResumptionDataProcessor resumption_data_processor_;
};

}  // namespace resumption
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUME_CTRL_IMPL_H_

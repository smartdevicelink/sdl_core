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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUME_CTRL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUME_CTRL_H_

#include <stdint.h>

#include "application_manager/resumption/resumption_data.h"

namespace application_manager {
class ApplicationManager;
class Application;
typedef std::shared_ptr<Application> ApplicationSharedPtr;
typedef std::shared_ptr<const Application> ApplicationConstSharedPtr;
}
namespace app_mngr = application_manager;

namespace resumption {

class LastState;

/**
 * @brief Contains logic for storage/restore data of applications.
 */

class ResumeCtrl {
 public:
  /**
  * @brief ResumptionCallBack Function signature to be called whe
  * DataResumptionFinished
  * @param result_code result code for sending to mobile
  * @param info additional info for sending to mobile
  */
  typedef std::function<void(mobile_apis::Result::eType result_code,
                             const std::string& info)> ResumptionCallBack;
  /**
   * @brief allows to destroy ResumeCtrl object
   */
  virtual ~ResumeCtrl() {}

  /**
   * @brief Save all applications info to the file system
   */
  virtual void SaveAllApplications() = 0;

  /**
   * @brief Save application persistent info for future resuming
   * @param application is application witch need to be saved
   */
  virtual void SaveApplication(
      application_manager::ApplicationSharedPtr application) = 0;

  /**
   * @brief Set application HMI Level and ausio_state as saved
   * @param application is application witch HMI Level is need to restore
   * @return true if success, otherwise return false
   */
  virtual bool RestoreAppHMIState(
      application_manager::ApplicationSharedPtr application) = 0;

  /**
   * @brief Set application HMI Level as stored in policy
   * @param application is application witch HMI Level is need to setup
   * @return true if success, otherwise return false
   */
  virtual bool SetupDefaultHMILevel(
      application_manager::ApplicationSharedPtr application) = 0;

  /**
   * @brief Setup HmiLevel for application
   * Do routine of setting up hmi_level
   * @param application is application witch HMI Level is need to setup
   * @param hmi_level HMI Level is needed to setup
   * @param hmi_level AudioStreamingState is needed to setup
   * @param check_policy indicate if policy data consent must be verified
   * @return true if success, otherwise return false
   */
  virtual bool SetAppHMIState(
      application_manager::ApplicationSharedPtr application,
      const mobile_apis::HMILevel::eType hmi_level,
      bool check_policy = true) = 0;

  /**
   * @brief Remove application from list of saved applications
   * @param application is application which need to be removed
   * @return return true, if success, otherwise return false
   */
  virtual bool RemoveApplicationFromSaved(
      app_mngr::ApplicationConstSharedPtr application) = 0;
  /**
   * @brief Processes resumption data after receiving signal "Suspend"
   */
  virtual void OnSuspend() = 0;

  /**
   * @brief Processes resumption data after receiving signal "Ignition Off"
   */
  virtual void OnIgnitionOff() = 0;

  /**
   * @brief Processes resumption data after receiving signal "Awake"
   */
  virtual void OnAwake() = 0;

  /**
   * @brief Checks if SDL has already received OnExitAllApplication notification
   * with "SUSPEND" reason
   *
   * @return Returns TRUE if SDL has received OnExitAllApplication notification
   * with reason "SUSPEND" otherwise returns FALSE
   */
  virtual bool is_suspended() const = 0;

  /**
   * @brief Method stops timer "RsmCtrlPercist" when SDL
   * receives OnExitAllApplication notification
   * with reason "SUSPEND"
   */
  virtual void StopSavePersistentDataTimer() = 0;

  /**
   * @brief Start timer for resumption applications
   *        Restore D1-D5 data
   * @param application that is need to be restored
   * @return true if it was saved, otherwise return false
   */
  virtual bool StartResumption(app_mngr::ApplicationSharedPtr application,
                               const std::string& hash,
                               ResumptionCallBack callback) = 0;

  /**
   * @brief Handle restored data when timeout appeared
   * @param application id - const int32_t
   */
  virtual void HandleOnTimeOut(const int32_t app_id) = 0;

  /**
   * @brief Start timer for resumption applications
   *        Does not restore D1-D5 data
   * @param application that is need to be restored
   * @return true if it was saved, otherwise return false
   */
  virtual bool StartResumptionOnlyHMILevel(
      app_mngr::ApplicationSharedPtr application) = 0;

  /**
   * @brief Retry resumption of an app if it has been disabled or limited
   *        due to absence of high-bandwidth transport.
   * @param app_id ID of the app to resume
   */
  virtual void RetryResumption(const uint32_t app_id) = 0;

  /**
   * @brief Check if there are all files need for resumption
   * @param application that is need to be restored
   * @return true if it all files exist, otherwise return false
   */
  virtual bool CheckPersistenceFilesForResumption(
      app_mngr::ApplicationSharedPtr application) = 0;

  /**
   * @brief Check application hash
   * @param application that is need to be restored
   * @return true if it was saved, otherwise return false
   */
  virtual bool CheckApplicationHash(app_mngr::ApplicationSharedPtr application,
                                    const std::string& hash) = 0;

  /**
   * @brief Checks if Resume controller have saved application with hmi app id
   * @param hmi_app_id - hmi application id
   * @return true if exist, false otherwise
   */
  virtual bool IsHMIApplicationIdExist(uint32_t hmi_app_id) = 0;

  /**
   * @brief Check if Resume controller have saved instance of application
   * @param policy_app_id - mobile application id
   * @param device_id - id of device where application is run
   * @return true if exist, false otherwise
   */
  virtual bool IsApplicationSaved(const std::string& policy_app_id,
                                  const std::string& device_id) = 0;

  /**
   * @brief Function is used for application resume. HMI app ID must be
   * the same(PASA VCA module use it for stored app info).
   * Retrieves HMI app ID for the given policy app ID from stored information.
   * @param policy_app_id - mobile application id
   * @param device_id - id of device where application is run
   * @return HMI app ID
   */
  virtual uint32_t GetHMIApplicationID(const std::string& policy_app_id,
                                       const std::string& device_id) const = 0;

  /**
   * @brief Updates flag for saving application data
   */
  virtual void ApplicationsDataUpdated() = 0;

  /**
   * @brief Resume HMI Level and audio streaming state if needed
   * @param application - application to restore hmi level
   * and audio streaming state
   */
  virtual void StartAppHmiStateResumption(
      application_manager::ApplicationSharedPtr application) = 0;

  /**
   * @brief Update launch_time_ to current
   */
  virtual void ResetLaunchTime() = 0;

  /**
   * @brief Removes activated application from resumption list
   *
   * @param application application witch need to be removed from resumption
   */
  virtual void OnAppActivated(app_mngr::ApplicationSharedPtr application) = 0;

  /**
   * @brief Removes app from resumption list
   *
   * app_id Application to remove
   */
  virtual void RemoveFromResumption(uint32_t app_id) = 0;

  /**
   * @brief Initialization data for Resume controller
   * @return true if initialization is success otherwise
   * returns false
   */
  virtual bool Init(LastState& last_state) = 0;

  /**
   * @brief Notify resume controller about new application
   * @param policy_app_id - mobile application id
   * @param device_id - id of device where application is run
   */
  virtual void OnAppRegistrationStart(const std::string& policy_app_id,
                                      const std::string& device_id) = 0;

  /**
   * @brief Notify resume controller about delete new application
   */
  virtual void OnAppRegistrationEnd() = 0;

  /**
   * @brief GetSavedHMILevels get saved apps hmi levels
   * @return mapping of mobile application id and saved hmi_level
   */
  virtual int32_t GetSavedAppHmiLevel(const std::string& app_id,
                                      const std::string& device_id) const = 0;

  virtual time_t LaunchTime() const = 0;

#ifdef BUILD_TESTS
  virtual void set_resumption_storage(
      std::shared_ptr<ResumptionData> mock_storage) = 0;

  virtual bool get_resumption_active() const = 0;
#endif  // BUILD_TESTS
};

}  // namespace resumption
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUME_CTRL_H_

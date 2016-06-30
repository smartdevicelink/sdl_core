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
#include "utils/shared_ptr.h"

namespace application_manager {
class ApplicationManager;
class Application;
typedef utils::SharedPtr<Application> ApplicationSharedPtr;
typedef utils::SharedPtr<const Application> ApplicationConstSharedPtr;
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
   * @brief Save application persistent info for future resuming
   * @param application is application witch need to be saved
   */
  virtual void SaveApplication(
      application_manager::ApplicationSharedPtr application) = 0;

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
   * @brief Processes resumption data after receiving signal "Awake"
   */
  virtual void OnAwake() = 0;

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
                               const std::string& hash) = 0;
  /**
   * @brief Start timer for resumption applications
   *        Does not restore D1-D5 data
   * @param application that is need to be restored
   * @return true if it was saved, otherwise return false
   */
  virtual bool StartResumptionOnlyHMILevel(
      app_mngr::ApplicationSharedPtr application) = 0;

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
};

}  // namespace resumption
#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUME_CTRL_H_

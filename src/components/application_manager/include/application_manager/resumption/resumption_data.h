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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_H_

#include "smart_objects/smart_object.h"
#include "application_manager/application.h"

namespace resumption {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;
namespace app_mngr = application_manager;

/**
 * @brief Abstract class which provides an interface for storage/restore
 * application data. Contains methods for receiving application data
 * from application.
 */

class ResumptionData {
 public:
  /**
   * @brief Constructor of ResumptionData
   */
  ResumptionData();

  /**
   * @brief Destructor of ResumptionData
   */
  virtual ~ResumptionData() {}

  /**
   * @brief Save application persistent info for future resuming
   * @param application is application witch need to be saved
   */
  virtual void SaveApplication(app_mngr::ApplicationSharedPtr application) = 0;

  /**
   * @brief Returns HMI level of application from saved data
   * @param policy_app_id contains policy application id of application
   * @param device_id contains id of device on which is running application
   * @return HMI level, if saved data does not contain HMI level method
   * returns -1
   */
  virtual int32_t GetStoredHMILevel(const std::string& policy_app_id,
                                    const std::string& device_id) const = 0;

  /**
   * @brief Checks if saved data of applications have hmi app id
   * @param hmi_app_id - hmi application id
   * @return true if exist, false otherwise
   */
  virtual bool IsHMIApplicationIdExist(uint32_t hmi_app_id) const = 0;

  /**
   * @brief Checks if saved data have application
   * and removes this data if it is not valid
   * @param policy_app_id - policy application id
   * @param device_id - contains id of device on which is running application
   * @return true if data exists and data is valid, false otherwise
   */
  virtual bool CheckSavedApplication(const std::string& policy_app_id,
                                     const std::string& device_id) = 0;

  /**
   * @brief Retrieves HMI app ID for the given mobile app ID
   * and device ID from stored information.
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return HMI app ID
   */
  virtual uint32_t GetHMIApplicationID(const std::string& policy_app_id,
                                       const std::string& device_id) const = 0;

  /**
   * @brief Increments ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void OnSuspend() = 0;

  /**
   * @brief Retrieves hash ID for the given mobile app ID
   * and device ID from stored information.
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hash_id - parameter which will contain HASH id from saved
   * application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetHashId(const std::string& policy_app_id,
                         const std::string& device_id,
                         std::string& hash_id) const = 0;

  /**
   * @brief Increments ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void OnAwake() = 0;

  /**
   * @brief Retrieves data of saved application for the given mobile app ID
   * and device ID
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param saved_app - parameter which will contain data of saved application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetSavedApplication(
      const std::string& policy_app_id,
      const std::string& device_id,
      smart_objects::SmartObject& saved_app) const = 0;

  /**
   * @brief Remove application from list of saved applications
   * @param policy_app_id application which need to be removed
   * @param device_id - contains id of device on which is running application
   * @return return true, if success, otherwise return false
   */
  virtual bool RemoveApplicationFromSaved(const std::string& policy_app_id,
                                          const std::string& device_id) = 0;

  /**
   * @brief Get the last ignition off time from LastState
   * @return the last ignition off time from LastState
   */
  virtual uint32_t GetIgnOffTime() const = 0;

  /**
   * @brief Checks if saved data have application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return index if data of application exists, otherwise returns -1
   */
  virtual ssize_t IsApplicationSaved(const std::string& policy_app_id,
                                     const std::string& device_id) const = 0;

  /**
   * @brief Retrieves data from saved application
   * @param saved_data - will be contain data for resume_ctrl
   */
  virtual void GetDataForLoadResumeData(
      smart_objects::SmartObject& saved_data) const = 0;

  /**
   * @brief Updates HMI level of saved application
   * @param policy_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_level - contains hmi level for saved application
   */
  virtual void UpdateHmiLevel(const std::string& policy_app_id,
                              const std::string& device_id,
                              mobile_apis::HMILevel::eType hmi_level) = 0;

  /**
   * @brief Init storage
   */
  virtual bool Init() = 0;

  /**
   * @brief Drops data related to applicaton data resumption
   * @param device_id Device ID
   * @param app_id Application ID
   * @return true, if dropped successfully, otherwise - false
   */
  virtual bool DropAppDataResumption(const std::string& device_id,
                                     const std::string& app_id) = 0;

  /**
   * @brief Persist saves resumption data on file system
   */
  virtual void Persist() = 0;
 protected:
  /**
   * @brief Retrieves of commands from application
   * @param application contains application of which selection commands
   * @return list of commands
   */
  smart_objects::SmartObject GetApplicationCommands(
      app_mngr::ApplicationConstSharedPtr application) const;

  /**
   * @brief Retrieves of submenues from application
   * @param application contains application of which selection submenues
   * @return list of submenues
   */
  smart_objects::SmartObject GetApplicationSubMenus(
      app_mngr::ApplicationConstSharedPtr application) const;

  /**
   * @brief Retrieves of interactionChoiceSet from application
   * @param application contains application of which selection
   * interactionChoiceSet
   * @return list of interaction choice set
   */
  smart_objects::SmartObject GetApplicationInteractionChoiseSets(
      app_mngr::ApplicationConstSharedPtr application) const;

  /**
   * @brief Retrieves of global properties from application
   * @param application contains application of which selection global
   * properties
   * @return global properties of application
   */
  smart_objects::SmartObject GetApplicationGlobalProperties(
      app_mngr::ApplicationConstSharedPtr application) const;

  /**
   * @brief Retrieves of subscriptions from application
   * @param application contains application of which selection subscriptions
   * @return subscriptions of application
   */
  smart_objects::SmartObject GetApplicationSubscriptions(
      app_mngr::ApplicationConstSharedPtr application) const;

  /**
   * @brief Retrieves of files from application
   * @param application contains application of which selection files
   * @return files of application
   */
  smart_objects::SmartObject GetApplicationFiles(
      app_mngr::ApplicationConstSharedPtr application) const;

  /**
   * @brief checks pointer that it is not equal NULL
   * @param ptr - contains pointer which need to check
   * @return smartObject from pointer
   */
  smart_objects::SmartObject PointerToSmartObj(
      const smart_objects::SmartObject* ptr) const;

  /**
   * @brief Creates smart array from received data
   * @param first - iterator points to beginning of the data
   * @param last - iterator points to ending of the data
   * @param key - contains smart array's name
   * @param result - will contain created array
   */
  template <typename Iterator>
  void Append(Iterator first,
              Iterator last,
              const std::string& key,
              smart_objects::SmartObject& result) const {
    int i = 0;
    result[key] = smart_objects::SmartObject(smart_objects::SmartType_Array);
    while (first != last) {
      result[key][i++] = *first;
      ++first;
    }
  }

  mutable sync_primitives::Lock resumption_lock_;
};
}  // namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_H_

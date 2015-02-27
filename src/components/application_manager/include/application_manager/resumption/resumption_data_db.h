/*
 * Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_DB_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_DB_H_

namespace application_manager {
namespace resumption {
class ResumptionDataDB : public ResumptionData {
  public:
  /**
   * @brief Save application persistent info for future resuming to db
   * @param application is application witch need to be saved
   */
  virtual void SaveApplication(ApplicationConstSharedPtr application);

  /**
   * @brief Returns HMI level of application from saved data
   * @param m_app_id contains mobile application id of application
   * @param device_id contains id of device on which is running application
   * @return HMI level if saved data does not contain HMI level method
   * returns -1
   */
  virtual int GetStoredHMILevel(const std::string& m_app_id,
                                const std::string& device_id);              ////////////////////////////////////////

  /**
   * @brief restores saved data of application
   * @param application contains application for which restores data
   * @return true if success, otherwise return false
   */
  virtual bool RestoreApplicationData(ApplicationSharedPtr application);

  /**
   * @brief Check if saved data of applications have hmi app id
   * @param hmi_app_id - hmi application id
   * @return true if exist, false otherwise
   */
  virtual bool IsHMIApplicationIdExist(uint32_t hmi_app_id);               ////////////////////////////////////////////

  /**
   * @brief Checks if saved data have application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if data exists, false otherwise
   */
  virtual bool CheckSavedApplication(const std::string& mobile_app_id,            ////////////////////////////////////////////
                                     const std::string& device_id);

  /**
   * @brief Retrieves HMI app ID for the given mobile app ID
   * and device ID from stored information.
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return HMI app ID
   */
  virtual uint32_t GetHMIApplicationID(const std::string& mobile_app_id,           ////////////////////////////////////////////
                                       const std::string& device_id);

  /**
   * @brief Increments ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void Suspend();                                                         ////////////////////////////////////////////

  /**
   * @brief Retrieves hash ID for the given mobile app ID
   * and device ID from stored information.
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hash_id - parameter which will contain HASH id from saved application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetHashId(const std::string& mobile_app_id,                        ////////////////////////////////////////////
                         const std::string& device_id,
                         std::string& hash_id);

  /**
   * @brief Increments ignition counter for all registered applications
   * and remember ign_off time stamp
   */
  virtual void OnAwake();                                                        ////////////////////////////////////////////

  /**
   * @brief Retrieves data of saved appliction for the given mobile app ID
   * and device ID
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param saved_app - parameter which will contain data of saved application
   * @return TRUE if application will be found in saved data otherwise
   * returns FALSE
   */
  virtual bool GetSavedApplication(const std::string& mobile_app_id,
                                   const std::string& device_id,
                                   smart_objects::SmartObject& saved_app);

  /**
   * @brief Remove application from list of saved applications
   * @param mobile_app_id application witch need to be removed
   * @param device_id - contains id of device on which is running application
   * @return return true, if success, otherwise return false
   */
  virtual bool RemoveApplicationFromSaved(const std::string& mobile_app_id,  ////////////////////////////////////////////
                                          const std::string& device_id);

  /**
   * @brief Get the last ignition off time from LastState
   * @return the last ignition off time from LastState
   */
  virtual uint32_t GetIgnOffTime();                                           ////////////////////////////////////////////

  /**
   * @brief Checks if saved data have application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return 0 if saved data contains application otherwise returns -1
   */
  virtual int IsApplicationSaved(const std::string& mobile_app_id,            ////////////////////////////////////////////
                                 const std::string& device_id);

  /**
   * @brief Retrieves data from saved application
   * @param  will contain data for resume_ctrl
   */
  virtual void GetDataForLoadResumeData(smart_objects::SmartObject& saved_data);     ////////////////////////////////////////////

  /**
   * @brief Changed HMI level for saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_level - contains new hmi level for saved application
   */
  virtual void SetHMILevelForSavedApplication(const std::string& mobile_app_id,      ////////////////////////////////////////////
                                              const std::string& device_id,
                                              int32_t hmi_level);

  virtual ~ResumptionDataDB();
  private:

  /**
   * @brief Retrieves hmi level from db
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_level - will contains hmi level for saved application
   * @return true if application with mobile id and device id has hmi level
   * otherwise returns false
   */
  bool SelectHMILevel(const std::string& m_app_id, const std::string& device_id,
                      int& hmi_level);
  /**
   * @brief Checks existence HMI id in DB
   * @param hmi_app_id - HMI id
   * return true if hmiID is same with saved hmiID otherwise returns false
   */
  bool CheckExistenceHMIId(const uint32_t hmi_app_id);

  /**
   * @brief Select HMI id from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_id - will contains hmi id from saved application
   */
  void SelectHMIId(const std::string& mobile_app_id, const std::string& device_id,
                   uint32_t& hmi_id);

  /**
   * @brief Select hash id from saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hash_id - will contains hash id from saved application
   */
  bool SelectHashId(const std::string& mobile_app_id,
                    const std::string& device_id,
                    std::string& hash_id);

  /**
   * @brief Select Ign off time
   * @return Ign off time from saved data
   */
  uint32_t SelectIgnOffTime();

  /**
   * @brief Checks existence application in DB
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if saved data contains application otherwise returns false
   */
  bool CheckExistenceApplication(const std::string& mobile_app_id,
                                 const std::string& device_id);

  /**
   * @brief Retrieves data from saved application
   * @param will contain data for resume_ctrl
   */
  void SelectDataForLoadResumeData(smart_objects::SmartObject& saved_data);

  /**
   * @brief Updates HMI level of saved application
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @param hmi_level - contains hmi level for saved appliction
   */
  void UpdateHmiLevel(const std::string& mobile_app_id,
                      const std::string& device_id,
                      int32_t hmi_level);

  /**
   * @brief Delete saved application from db
   * @param mobile_app_id - mobile application id
   * @param device_id - contains id of device on which is running application
   * @return true if application was found and deleted otherwise returns
   * false
   */
  bool DeleteSavedApplication(const std::string& mobile_app_id,
                              const std::string& device_id);
  /**
   * @brief Updates ignition of count on saved applications after onAwake
   * notification
   */
  void UpdateDataOnAwake();



};
}  // namespace resumption
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_DB_H_

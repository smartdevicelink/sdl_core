/**
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H

#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <list>

#include "json/json.h"
#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"
#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"
#include "application_manager/event_engine/event_observer.h"
#include "smart_objects/smart_object.h"
#include "application_manager/application.h"
#include "utils/timer_thread.h"

namespace application_manager {

namespace smart_objects = NsSmartDeviceLink::NsSmartObjects;

class ApplicationManagerImpl;
class Application;
class ResumeCtrl: public event_engine::EventObserver {

  public:

  /**
   * @brief Constructor
   * @param app_mngr ApplicationManager pointer
   */
    explicit ResumeCtrl(ApplicationManagerImpl* app_mngr);

    /**
     * @brief Event, that raised if application get resumption response from HMI
     * @param event : event object, that contains smart_object with HMI message
     */
    virtual void on_event(const event_engine::Event& event);

    /**
     * @brief Save all applications info to the file system
     */
    void SaveAllApplications();

    /**
     * @brief Save application persistent info for future resuming
     * @param application is application witch need to be saved
     */
    void SaveApplication(ApplicationConstSharedPtr application);

    /**
     * @brief Set application HMI Level as saved
     * @param application is application witch HMI Level is need to restore
     * @return true if success, otherwise return false
     */
    bool RestoreApplicationHMILevel(ApplicationSharedPtr application);

    /**
     * @brief Set application HMI Level as stored in policy
     * @param application is application witch HMI Level is need to setup
     * @return true if success, otherwise return false
     */
    bool SetupDefaultHMILevel(ApplicationSharedPtr application);

    /**
     * @brief Setup HmiLevel for application
     * Do routine of setting up hmi_level
     * @param application is application witch HMI Level is need to setup
     * @param hmi_level HMI Level is needed to setup
     * @param hmi_level AudioStreamingState is needed to setup
     * @param check_policy indicate if policy data consent must be verified
     * @return true if success, otherwise return false
     */
    bool SetupHMILevel(ApplicationSharedPtr application,
                       mobile_apis::HMILevel::eType hmi_level,
                       mobile_apis::AudioStreamingState::eType audio_streaming_state,
                       bool check_policy = true);

    /**
     * @brief Set application HMI Level as saved
     * @param application is application witch HMI Level is need to restore
     * @return true if success, otherwise return false
     */
    bool RestoreApplicationData(ApplicationSharedPtr application);

    /**
     * @brief Check if Resume controller have saved instance of application
     * @param application is application witch need to be checked
     * @return true if exist, false otherwise
     */
    bool ApplicationIsSaved(ApplicationConstSharedPtr application);

    /**
     * @brief Remove application from list of saved applications
     * @param application is application witch need to be removed
     * @return return true, if success, otherwise return false
     */
    bool RemoveApplicationFromSaved(ApplicationConstSharedPtr application);

    /**
     * @brief Increments ignition counter for all registered applications
     */
    void IgnitionOff();

    /**
     * @brief Start timer for resumption applications
     *        Restore D1-D5 data
     * @param application that is need to be restored
     * @return true if it was saved, otherwise return false
     */
    bool StartResumption(ApplicationSharedPtr application, uint32_t hash);

    /**
     * @brief Start timer for resumption applications
     *        Does not restore D1-D5 data
     * @param application that is need to be restored
     * @return true if it was saved, otherwise return false
     */
    bool StartResumptionOnlyHMILevel(ApplicationSharedPtr application);

    /**
     * @brief Check if there are all files need for resumption
     * @param application that is need to be restored
     * @return true if it all files exist, otherwise return false
     */
    bool CheckPersistenceFilesForResumption(ApplicationSharedPtr application);

    /**
     * @brief Check application hash
     * @param application that is need to be restored
     * @return true if it was saved, otherwise return false
     */
    bool CheckApplicationHash(ApplicationSharedPtr application, uint32_t hash);

    /**
     * @brief Check if Resume controller have saved application with hmi app id
     * @param hmi_app_id - hmi application id
     * @return true if exist, false otherwise
     */
    bool IsHMIApplicationIdExist(uint32_t hmi_app_id);

    /**
     * @brief Check if Resume controller have saved instance of application
     * @param mobile_app_id - mobile application id
     * @return true if exist, false otherwise
     */
    bool IsApplicationSaved(const std::string& mobile_app_id);

    /**
     * @brief Function is used for application resume. HMI app ID must be
     * the same(PASA VCA module use it for stored app info).
     * Retrieves HMI app ID for the given mobile app ID from stored information.
     *
     * @param mobile_app_id - mobile application id
     * @return HMI app ID
     */
    uint32_t GetHMIApplicationID(const std::string& mobile_app_id);

    /**
     * @brief Timer callback function
     *
     */
    void onTimer();

    void ClearResumptionInfo();

  private:

    typedef std::pair<uint32_t, uint32_t> application_timestamp;

    /**
     * @brief This struct need to map
     * timestamp and application from correlationID
     */
    struct ResumingApp {
      uint32_t old_session_key; // session key is the same as app_id
      ApplicationSharedPtr app;
    };

    struct TimeStampComparator {
        bool operator() (const application_timestamp& lhs,
                         const application_timestamp& rhs) const{
            return lhs.second < rhs.second;
        }
    };

    /**
     * @brief Check device MAC address
     *
     * @param application that is need to be restored
     * @param saved_device_mac Saved device MAC address
     *
     * @return TRUE on success, otherwise FALSE
     */
    bool IsDeviceMacAddressEqual(ApplicationSharedPtr application,
                                 const std::string& saved_device_mac);

    Json::Value& GetSavedApplications();

    void SetSavedApplication(Json::Value& apps_json);

    Json::Value GetApplicationCommands(
        ApplicationConstSharedPtr application);
    Json::Value GetApplicationSubMenus(
        ApplicationConstSharedPtr application);
    Json::Value GetApplicationInteractionChoiseSets(
        ApplicationConstSharedPtr application);
    Json::Value GetApplicationGlobalProperties(
        ApplicationConstSharedPtr application);
    Json::Value GetApplicationSubscriptions(
        ApplicationConstSharedPtr application);
    Json::Value GetApplicationFiles(
        ApplicationConstSharedPtr application);
    Json::Value GetApplicationShow(
        ApplicationConstSharedPtr application);

    Json::Value JsonFromSO(
        const NsSmartDeviceLink::NsSmartObjects::SmartObject *so);

    void SendHMIRequest(const hmi_apis::FunctionID::eType& function_id,
                        const smart_objects::SmartObject* msg_params = NULL,
                        bool use_events = false);

    bool ProcessHMIRequest(
        NsSmartDeviceLink::NsSmartObjects::SmartObject* request = NULL,
        bool use_events = false);

    /**
     * @brief Time step to check resumption TIME_OUT
     */
    static const uint32_t kTimeStep = 3;

    /**
     *  @brief times of IGN_OFF that zombie application have to be saved.
     */
    static const uint32_t kApplicationLifes = 3;

    /**
    *@brief Mapping applications to time_stamps
    *       wait for timer to resume HMI Level
    *
    */
    std::multiset<application_timestamp, TimeStampComparator> waiting_for_timer_;
    mutable sync_primitives::Lock   queue_lock_;
    ApplicationManagerImpl*         app_mngr_;
    timer::TimerThread<ResumeCtrl>  timer_;
};

}  // namespace application_manager
#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H

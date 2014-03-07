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

#include <cstdint>
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
     * In case of IGN_OFF or Ctl-C or MEATER_RESSET this info will save to to file system
     */
    void SaveApplication(ApplicationConstSharedPtr application);

    /**
     * @brief Load unregistered applications info from the file system
     *        You can use this function only if connection handler is ready
     */
    void LoadApplications();

    /**
     * @brief Set application HMI Level as saved
     * @param application is application witch HMI Level is need to restore
     * @return true if success, otherwise return false
     */
    bool RestoreApplicationHMILevel(ApplicationSharedPtr application);

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
     * @brief Save application info to FileSystem
     */
    void SavetoFileSystem();

    /**
     * @brief Start timer for resumption applications
     * @param application that is need to be restored
     * @return true if it was saved, otherwise return false
     */
    bool StartResumption(ApplicationSharedPtr application, uint32_t hash);

    /**
     * @brief Check application hash
     * @param application that is need to be restored
     * @return true if it was saved, otherwise return false
     */
    bool CheckApplicationHash(uint32_t app_id, uint32_t hash);

    /**
     * @brief Timer callback function
     *
     */
    void onTimer();

  private:

    mutable sync_primitives::Lock queue_lock_;

    timer::TimerThread<ResumeCtrl> timer_;
    /**
     * @brief This struct need to map
     * timestamp and application from correlationID
     */
    struct ResumingApp {
      uint32_t old_session_key; // session key is the same as app_id
      ApplicationSharedPtr app;
    };

    static log4cxx::LoggerPtr logger_;

    /**
     * @brief Time step to check resumption TIME_OUT
     */
    static const uint32_t kTimeStep = 3;

    /**
    *@brief Data of applications, that whait for resuming
    */
    std::vector<Json::Value> saved_applications_;

    typedef std::pair<uint32_t, uint32_t> application_timestamp;
    struct TimeStampComparator {
        bool operator() (const application_timestamp& lhs,
                         const application_timestamp& rhs) const{
            return lhs.second < rhs.second;
        }
    };

    /**
    *@brief Mapping applications to time_stamps
    *       wait for timer to resume HMI Level
    *
    */
    std::set<application_timestamp, TimeStampComparator> waiting_for_timer_;


    ApplicationManagerImpl* app_mngr_;

    /**
     *  @brief times of IGN_OFF that zombie application have to be saved.
     */
    static const uint32_t kApplicationLifes = 3;


    /**
     * @brief Check if Resume controller have saved instance of application
     * @param app_id - application id witch is the same as session_key or connection_key
     * @return true if exist, false otherwise
     */
    bool ApplicationIsSaved(const uint32_t app_id);

    Json::Value GetApplicationCommands(const uint32_t app_id);
    Json::Value GetApplicationInteractionChoiseSets(const uint32_t app_id);
    Json::Value GetApplicationGlobalProperties(const uint32_t app_id);
    Json::Value GetApplicationSubscriptions(const uint32_t app_id);
    Json::Value GetApplicationFiles(const uint32_t app_id);

    Json::Value JsonFromSO(const NsSmartDeviceLink::NsSmartObjects::SmartObject *so);

    void SendHMIRequest(const hmi_apis::FunctionID::eType& function_id,
                        const smart_objects::SmartObject* msg_params = NULL,
                        bool use_events = false);

    bool ProcessHMIRequest(
        NsSmartDeviceLink::NsSmartObjects::SmartObject* request = NULL,
        bool use_events = false);
};

}  // namespace application_manager
#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H

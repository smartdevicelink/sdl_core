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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H

#include <stdint.h>
#include <vector>
#include <map>
#include <set>
#include <list>


#include "interfaces/HMI_API.h"
#include "interfaces/HMI_API_schema.h"
#include "interfaces/MOBILE_API_schema.h"
#include "application_manager/event_engine/event_observer.h"
#include "smart_objects/smart_object.h"
#include "application_manager/application.h"
#include "utils/timer_thread.h"
#include "resumption_data.h"

namespace application_manager {
  class ApplicationManagerImpl;
  class Application;
}

namespace resumption {

class ResumeCtrl: public app_mngr::event_engine::EventObserver {

  public:

    ResumeCtrl();

    ~ResumeCtrl();
    /**
     * @brief Event, that raised if application get resumption response from HMI
     * @param event : event object, that contains smart_object with HMI message
     */
    virtual void on_event(const app_mngr::event_engine::Event& event);

    /**
     * @brief Save all applications info to the file system
     */
    void SaveAllApplications();

    /**
     * @brief Save application persistent info for future resuming
     * @param application is application witch need to be saved
     */
    void SaveApplication(app_mngr::ApplicationConstSharedPtr application);

    /**
     * @brief Set application HMI Level and ausio_state as saved
     * @param application is application witch HMI Level is need to restore
     * @return true if success, otherwise return false
     */
    bool RestoreAppHMIState(app_mngr::ApplicationSharedPtr application);

    /**
     * @brief Set application HMI Level as stored in policy
     * @param application is application witch HMI Level is need to setup
     * @return true if success, otherwise return false
     */
    bool SetupDefaultHMILevel(app_mngr::ApplicationSharedPtr application);

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
                       bool check_policy = true);

    /**
     * @brief Check if Resume controller have saved instance of application
     * @param application is application witch need to be checked
     * @return true if exist, false otherwise
     */
    bool ApplicationIsSaved(app_mngr::ApplicationConstSharedPtr application);

    /**
     * @brief Remove application from list of saved applications
     * @param application is application witch need to be removed
     * @return return true, if success, otherwise return false
     */
    bool RemoveApplicationFromSaved(
        app_mngr::ApplicationConstSharedPtr application);

    /**
     * @brief Increments ignition counter for all registered applications
     * and remember ign_off time stamp
     */
    void OnSuspend();

    /**
     * @brief Increments ignition counter for all registered applications
     * and remember ign_off time stamp
     */
    void OnAwake();

    /**
     * @brief Method starts timer "RsmCtrlPercist" when
     * SDL receives onAwakeSDL notification
     */
    void StartSavePersistentDataTimer();

    /**
     * @brief Method stops timer "RsmCtrlPercist" when SDL
     * receives OnExitAllApplication notification
     * with reason "SUSPEND"
     */
    void StopSavePersistentDataTimer();

    /**
     * @brief Start timer for resumption applications
     *        Restore D1-D5 data
     * @param application that is need to be restored
     * @return true if it was saved, otherwise return false
     */
    bool StartResumption(app_mngr::ApplicationSharedPtr application,
                         const std::string& hash);

    /**
     * @brief Start timer for resumption applications
     *        Does not restore D1-D5 data
     * @param application that is need to be restored
     * @return true if it was saved, otherwise return false
     */
    bool StartResumptionOnlyHMILevel(
        app_mngr::ApplicationSharedPtr application);

    /**
     * @brief Check if there are all files need for resumption
     * @param application that is need to be restored
     * @return true if it all files exist, otherwise return false
     */
    bool CheckPersistenceFilesForResumption(
        app_mngr::ApplicationSharedPtr application);

    /**
     * @brief Check application hash
     * @param application that is need to be restored
     * @return true if it was saved, otherwise return false
     */
    bool CheckApplicationHash(app_mngr::ApplicationSharedPtr application,
                              const std::string& hash);

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
    bool IsApplicationSaved(const std::string& mobile_app_id,
                            const std::string& device_id);

    /**
     * @brief Function is used for application resume. HMI app ID must be
     * the same(PASA VCA module use it for stored app info).
     * Retrieves HMI app ID for the given mobile app ID from stored information.
     *
     * @param mobile_app_id - mobile application id
     * @return HMI app ID
     */
    uint32_t GetHMIApplicationID(const std::string& mobile_app_id,
                                 const std::string& device_id);

    /**
     * @brief SaveDataOnTimer :
     *  Timer callback for persisting ResumptionData each N seconds
     *  N gets from property
     */
    void SaveDataOnTimer();

    void ApplicationsDataUpdated() {
      is_data_saved_ = false;
    }

    /**
     * @brief Resume HMI Level and audio streaming state if needed
     * @param application - application to restore hmi level
     * and audio streaming state
     */
    void StartAppHmiStateResumption(app_mngr::ApplicationSharedPtr application);

    /**
     * @brief Update launch_time_ to current
     */
    void ResetLaunchTime();

    /**
     * @brief IsLimmitedAllowed return true if it is allowed to setup
     * LIMITTED HmiLevel
     * (if there are no app with the same type in FULL ot LIMITED))
     * @param application to setup LIMITEd
     * @return true if allowed otherwise false
     */
    bool IsLimmitedAllowed(app_mngr::ApplicationConstSharedPtr application);

    /**
     * @brief ResolveHMILevelConflicts found maximum allowed HMILevel
     * @param application application to setup  hmi level
     * @param hmi_level requested to setup
     * @return maximum allowed HMILevel
     */
    mobile_apis::HMILevel::eType ResolveHMILevelConflicts(
        app_mngr::ApplicationSharedPtr application,
        const mobile_apis::HMILevel::eType hmi_level);

    /**
     * @brief Timer callback for  restoring HMI Level
     *
     */
    void ApplicationResumptiOnTimer();

    /**
     * @brief Removes activated application from resumption list
     *
     * @param application application witch need to be removed from resumption
     */
    void OnAppActivated(app_mngr::ApplicationSharedPtr application);

    /**
     * @brief Removes app from resumption list
     *
     * app_id Application to remove
     */
    void RemoveFromResumption(uint32_t app_id);


    /**
     * @brief Initialization data for Resume controller
     * @return true if initialization is success otherwise
     * returns false
     */
    bool Init();

  private:

    /**
     * @brief restores saved data of application
     * @param application contains application for which restores data
     * @return true if success, otherwise return false
     */
    bool RestoreApplicationData(app_mngr::ApplicationSharedPtr application);

    /**
     * @brief AddFiles allows to add files for the application
     * which should be resumed
     * @param application application which will be resumed
     * @param saved_app application specific section from backup file
     */
    void AddFiles(app_mngr::ApplicationSharedPtr application,
                  const smart_objects::SmartObject& saved_app);

    /**
     * @brief AddSubmenues allows to add sub menues for the application
     * which should be resumed
     * @param application application which will be resumed
     * @param saved_app application specific section from backup file
     */
    void AddSubmenues(app_mngr::ApplicationSharedPtr application,
                      const smart_objects::SmartObject& saved_app);

    /**
     * @brief AddCommands allows to add commands for the application
     * which should be resumed
     * @param application application which will be resumed
     * @param saved_app application specific section from backup file
     */
    void AddCommands(app_mngr::ApplicationSharedPtr application,
                     const smart_objects::SmartObject& saved_app);

    /**
     * @brief AddChoicesets allows to add choice sets for the application
     * which should be resumed
     * @param application application which will be resumed
     * @param saved_app application specific section from backup file
     */
    void AddChoicesets(app_mngr::ApplicationSharedPtr application,
                       const smart_objects::SmartObject& saved_app);

    /**
     * @brief SetGlobalProperties allows to restore global properties.
     * @param application application which will be resumed
     * @param saved_app application specific section from backup file
     */
    void SetGlobalProperties(app_mngr::ApplicationSharedPtr application,
                             const smart_objects::SmartObject& saved_app);

    /**
     * @brief AddSubscriptions allows to restore subscriptions
     * @param application application which will be resumed
     * @param saved_app application specific section from backup file
     */
    void AddSubscriptions(app_mngr::ApplicationSharedPtr application,
                          const smart_objects::SmartObject& saved_app);


    bool CheckIgnCycleRestrictions(const smart_objects::SmartObject& saved_app);

    bool DisconnectedJustBeforeIgnOff(const smart_objects::SmartObject& saved_app);

    bool CheckAppRestrictions(app_mngr::ApplicationConstSharedPtr application,
                              const smart_objects::SmartObject& saved_app);

    /**
     * @brief CheckIcons allows to check application icons
     *
     * @param application application under resumtion  application
     *
     * @param json_object
     *
     * @return true in case icons exists, false otherwise
     */
    bool CheckIcons(app_mngr::ApplicationSharedPtr application,
                    smart_objects::SmartObject& obj);

    /**
     * @brief CheckDelayAfterIgnOn should check if SDL was started less
     * then N secconds ago. N will be readed from profile.
     * @return true if SDL started N secconds ago, otherwise return false
     */
    bool CheckDelayAfterIgnOn();

    typedef std::pair<uint32_t, uint32_t> application_timestamp;

//    std::set<ApplicationSharedPtr> retrieve_application();

    /**
     * @brief This struct need to map
     * timestamp and application from correlationID
     */
    struct ResumingApp {
      uint32_t old_session_key; // session key is the same as app_id
      app_mngr::ApplicationSharedPtr app;
    };

    struct TimeStampComparator {
        bool operator() (const application_timestamp& lhs,
                         const application_timestamp& rhs) const{
            return lhs.second < rhs.second;
        }
    };

    /**
     * @brief geter for launch_time_
     * @return value of launch_time_
     */
    time_t launch_time() const;

    /**
     * @brief Check device MAC address
     *
     * @param application that is need to be restored
     * @param saved_device_mac Saved device MAC address
     *
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

    void SendHMIRequest(const hmi_apis::FunctionID::eType& function_id,
                        const smart_objects::SmartObject* msg_params = NULL,
                        bool use_events = false);

    /**
     * @brief Process specified HMI request
     *
     * @param request Request to process
     * @param use_events Process request events or not flag
     *
     * @return TRUE on success, otherwise FALSE
     */
    bool ProcessHMIRequest(
        smart_objects::SmartObjectSPtr request = NULL,
        bool use_events = false);

    /**
     * @brief Process list of HMI requests using ProcessHMIRequest method
     * @param requests List of requests to process
     */
    void ProcessHMIRequests(const smart_objects::SmartObjectList& requests);

    void InsertToTimerQueue(uint32_t app_id, uint32_t time_stamp);

    void AddToResumptionTimerQueue(uint32_t app_id);

    mobile_apis::HMILevel::eType IsHmiLevelFullAllowed(
        app_mngr::ApplicationConstSharedPtr app);

    void LoadResumeData();
    app_mngr::ApplicationManagerImpl* appMngr();

    /**
    *@brief Mapping applications to time_stamps
    *       wait for timer to resume HMI Level
    *
    */
    mutable sync_primitives::Lock   queue_lock_;
    timer::TimerThread<ResumeCtrl>  restore_hmi_level_timer_;
    bool                            is_resumption_active_;
    timer::TimerThread<ResumeCtrl>  save_persistent_data_timer_;
    typedef std::list<uint32_t>     WaitingForTimerList;
    WaitingForTimerList             waiting_for_timer_;
    bool is_data_saved_;
    time_t launch_time_;
    utils::SharedPtr<ResumptionData>    resumption_storage_;
};

}  // namespace resumption
#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H

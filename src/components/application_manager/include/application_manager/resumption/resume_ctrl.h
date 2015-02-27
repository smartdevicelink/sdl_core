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
#include "connection_handler/connection_handler_observer.h"
#include "connection_handler/device.h"
#include "application_manager/event_engine/event_observer.h"
#include "smart_objects/smart_object.h"
#include "application_manager/application.h"
#include "utils/timer_thread.h"

namespace application_manager {

namespace resumption {

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
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Save application persistent info for future resuming
     * @param application is application witch need to be saved
     */
    void SaveApplication(ApplicationConstSharedPtr application);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief Set application HMI Level as saved
     * @param application is application witch HMI Level is need to restore
     * @return true if success, otherwise return false
     */
    bool RestoreAppHMIState(ApplicationSharedPtr application);

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
    bool SetAppHMIState(ApplicationSharedPtr application,
                       const mobile_apis::HMILevel::eType hmi_level,
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
     * and remember ign_off time stamp
     */
    void Suspend();

    /**
     * @brief Increments ignition counter for all registered applications
     * and remember ign_off time stamp
     */
    void OnAwake();

    /**
     * @brief Method starts timer "RsmCtrlPercist" when SDL receives onAwakeSDL notification
     */
    void StartSavePersistentDataTimer();

    /**
     * @brief Method stops timer "RsmCtrlPercist" when SDL receives OnExitAllApplication notification
     * with reason "SUSPEND"
     */
    void StopSavePersistentDataTimer();

    /**
     * @brief Start timer for resumption applications
     *        Restore D1-D5 data
     * @param application that is need to be restored
     * @return true if it was saved, otherwise return false
     */
    bool StartResumption(ApplicationSharedPtr application, const std::string& hash);

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
    bool CheckApplicationHash(ApplicationSharedPtr application, const std::string& hash);

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
     * @brief SaveDataOnTimer :
     *  Timer callback for persisting ResumptionData each N seconds
     *  N gets from property
     */
    void SaveDataOnTimer();

    void ClearResumptionInfo();

    void ApplicationsDataUpdated() {
      is_data_saved = false;
    }

    /**
     * @brief Resume HMI Level and audio streaming state if needed
     * @param application - application to restore hmi level
     * and audio streaming state
     */
    void StartAppHmiStateResumption(ApplicationSharedPtr application);
    /**
     * @brief Update launch_time_ to current
     */
    void ResetLaunchTime();

  private:


    typedef std::pair<uint32_t, uint32_t> application_timestamp;

    std::set<ApplicationSharedPtr> retrieve_application();

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
    bool IsDeviceMacAddressEqual(ApplicationSharedPtr application,
                                 const std::string& saved_device_mac);

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
    * @brief Get Resumption section of LastState
    * @return Resumption section of LastState in Json
     */
    Json::Value& GetResumptionData();
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
    * @brief Get applications for resumption of LastState
    * @return applications for resumption of LastState
     */
    Json::Value& GetSavedApplications();
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    /**
    * @brief Set applications for resumption to LastState
    * @parems apps_json applications to write in LastState
     */
    void SetSavedApplication(Json::Value& apps_json);
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    Json::Value GetApplicationCommands(
        ApplicationConstSharedPtr application);
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    Json::Value JsonFromSO(const smart_objects::SmartObject *so);

    void SendHMIRequest(const hmi_apis::FunctionID::eType& function_id,
                        const smart_objects::SmartObject* msg_params = NULL,
                        bool use_events = false);

    bool ProcessHMIRequest(
        smart_objects::SmartObjectSPtr request = NULL,
        bool use_events = false);

    void InsertToTimerQueue(uint32_t app_id, uint32_t time_stamp);

    /**
     * @brief AddFiles allows to add files for the application
     * which should be resumed
     *
     * @param application application which will be resumed
     *
     * @param saved_app application specific section from backup file
     */
    void AddFiles(ApplicationSharedPtr application, const Json::Value& saved_app);

    /**
     * @brief AddSubmenues allows to add sub menues for the application
     * which should be resumed
     *
     * @param application application which will be resumed
     *
     * @param saved_app application specific section from backup file
     */
    void AddSubmenues(ApplicationSharedPtr application, const Json::Value& saved_app);

    /**
     * @brief AddCommands allows to add commands for the application
     * which should be resumed
     *
     * @param application application which will be resumed
     *
     * @param saved_app application specific section from backup file
     */
    void AddCommands(ApplicationSharedPtr application, const Json::Value& saved_app);

    /**
     * @brief AddChoicesets allows to add choice sets for the application
     * which should be resumed
     *
     * @param application application which will be resumed
     *
     * @param saved_app application specific section from backup file
     */
    void AddChoicesets(ApplicationSharedPtr application, const Json::Value& saved_app);

    /**
     * @brief SetGlobalProperties allows to restore global properties.
     *
     * @param application application which will be resumed
     *
     * @param saved_app application specific section from backup file
     */
    void SetGlobalProperties(ApplicationSharedPtr application, const Json::Value& saved_app);

    /**
     * @brief AddSubscriptions allows to restore subscriptions
     *
     * @param application application which will be resumed
     *
     * @param saved_app application specific section from backup file
     */
    void AddSubscriptions(ApplicationSharedPtr application, const Json::Value& saved_app);

    /**
     * @brief ProcessHMIRequests allows to process obtained requests.
     *
     * @param requests request that should be processed.
     */
    void ProcessHMIRequests(const smart_objects::SmartObjectList& requests);

    /**
     * @brief CheckIcons allows to check application icons
     *
     * @param application application under resumtion  application
     *
     * @param json_object
     *
     * @return true in case icons exists, false otherwise
     */
    bool CheckIcons(ApplicationSharedPtr application, const Json::Value& json_object);
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief GetFromSavedOrAppend allows to get existed record about application
     * or adds the new one.
     *
     * @param mobile_app_id application id.
     *
     * @return the reference to the record in applications array.
     */
    Json::Value& GetFromSavedOrAppend(const std::string& mobile_app_id);
    //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    /**
     * @brief CheckIgnCycleRestrictions checks if is needed to resume HMI state
     * by ign cycle restrictions
     * @param json_app - saved application
     * @return true if resumptions allowed, otherwise return false
     */
    bool CheckIgnCycleRestrictions(const Json::Value& json_app);

    /**
     * @brief DisconnectedInLastIgnCycle should check if was connected in prev ign cycle
     * @param json_app - saved applicationa
     * @return true if app connected in frep ign_cycle otherwise return false
     */
    bool DisconnectedInLastIgnCycle(const Json::Value& json_app);

    /**
     * @brief DisconnectedJustBeforeIgnOff should check if application
     * was dissconnected in N secconds delay before ign off.
     * N will be readed from profile
     * @param json_app - saved applicationa
     * @return was dissconnected in N secconds delay before ign off
     * otherwise return false
     */
    bool DisconnectedJustBeforeIgnOff(const Json::Value& json_app);

    /**
     * @brief CheckDelayAfterIgnOn should check if SDL was started less
     * then N secconds ago. N will be readed from profile.
     * @return true if SDL started N secconds ago, otherwise return false
     */
    bool CheckDelayAfterIgnOn();

    /**
     * @brief CheckAppRestrictions checks if is needed to resume HMI state
     * by application type and saved app_level
     * @param json_app - saved application
     * @return true if resumptions allowed, otherwise return false
     */
    bool CheckAppRestrictions(ApplicationSharedPtr application,
                              const Json::Value& json_app);
    /**
     * @brief GetObjectIndex allows to obtain specified obbject index from
     * applications arrays.
     *
     * @param mobile_app_id application id that should be found.
     *
     * @return application's index of or -1 if it doesn't exists
     */
    int GetObjectIndex(const std::string& mobile_app_id);

    /**
     * @brief Timer callback for  restoring HMI Level
     *
     */
    void ApplicationResumptiOnTimer();

    /*
     * @brief Loads data on start up
     */
    void LoadResumeData();

    template<typename Iterator>
    Json::Value Append(Iterator first,
                       Iterator last,
                       const std::string& key,
                       Json::Value& result) {
      while (first != last) {
        result[key].append(*first);
        ++first;
      }
      return result;
    }

    /**
     *  @brief times of IGN_OFF that zombie application have to be saved.
     */
    static const uint32_t kApplicationLifes = 3;

    /**
    *@brief Mapping applications to time_stamps
    *       wait for timer to resume HMI Level
    *
    */
    mutable sync_primitives::Lock   queue_lock_;
    sync_primitives::Lock           resumtion_lock_;
    ApplicationManagerImpl*         app_mngr_;
    timer::TimerThread<ResumeCtrl>  save_persistent_data_timer_;
    timer::TimerThread<ResumeCtrl>  restore_hmi_level_timer_;
    std::vector<uint32_t>           waiting_for_timer_;
    bool is_data_saved;
    time_t launch_time_;
    ResumptionData*                 resumption_storage_;
};

}  // namespace resumption
}  // namespace application_manager
#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H

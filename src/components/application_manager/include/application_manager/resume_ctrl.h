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

namespace application_manager {

class ApplicationManagerImpl;
class Application;
class ResumeCtrl: public event_engine::EventObserver {
  public:
    ResumeCtrl(const ApplicationManagerImpl* app_mngr);
    /**
     * @brief Save all applications info to the file system
     */
    void SaveAllApplications() ;

    /**
     * @brief Save application persistent info for future resuming
     * @param application is applicatint whitch need to be saved
     * In case of IGN_OFF or Ctl-C or MEATER_RESSET this info will saveto to file system
     */
    void SaveApplication(Application* application);

    /**
     * @brief Load unregistered applications info from the file system
     */
    void LoadApplications();

    /**
     * @brief Event, thar raised if application get resumption response from HMI
     * @param event : event object, that contains smart_object with HMI message
     */
    virtual void on_event(const event_engine::Event& event);

    /**
     * @brief Set application HMI Level as saved
     * @param application is applicatint whitch HMI Level is need to restore
     * @return true if succes, otherwise return false
     */
    bool RestoreApplicationHMILevel(Application* application);

    /**
     * @brief Check if Resume contriller have saved instance of application
     * @param application is applicatint whitch need to be checked
     * @return true if exist, false otherway
     */
    bool ApplicationIsSaved(const Application* application);

    /**
     * @brief Remove application from list of saved applications
     * @param application is applicatint whitch need to be removed
     * @return return true, if succes, otherwise return false
     */
    bool RemoveApplicationFromSaved(const Application* application);

    /**
     * @brief Save application info to FileSystem
     */
    void SavetoFS();

    /**
     * @brief Start timer for resumption applications
     * @param application that is need to be resaterted
     * @return true if it was saved, otherwise return false
     */
    bool StartResumption(Application* application);

    /**
     * @brief Timer callback function
     *
     */
    void onTimer();

  private:

    /**
     * @brief This struct need to map
     * timestamp and application from correlationID
     */
    struct ResumingApp {
      Application* app;
      uint32_t correlation_id;
      time_t start_resuming_time;
      bool is_waiting_for_timer;
    };

    static log4cxx::LoggerPtr logger_;

    /**
     * @brief Timer for resumption
     */
    timer::TimerThread<ResumeCtrl>  timer_;

    /**
     * @brief Time step to check resumption TIME_OUT
     */
    static const uint32_t kTimeStep = 1;

    /**
    *@brief Data of applications, that whait for resuming
    */
    std::vector<Json::Value> saved_applications_;

    /**
    *@brief Application, that wait for resuming timeout
    */
    std::list<ResumingApp*> resuming_applications_;

    const ApplicationManagerImpl* app_mngr_;

    /**
     *  @brief times of IGN_OFF that zombie application have to be saved.
     */
    static const uint32_t kApplicationLifes = 3;

    /**
     * @brief Get Parameters for connection as String
     * @param application is applicatint whose adress need to get
     * return string, that contains parameters for connection
     */
    std::string GetMacAddress(const Application* application);

    /**
     * @brief Get Send resumption requesto to mobile
     * response will come back in on_event()
     * @param application is application, that has to be resumed
     */
    void sendResumptionRequest(ResumingApp* application);
};

}  // namespace application_manager
#endif // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_RESUME_CTRL_H

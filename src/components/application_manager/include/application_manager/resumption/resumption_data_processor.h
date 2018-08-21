/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */
#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_PROCESS_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_PROCESS_

#include <map>
#include <vector>
#include <functional>

#include "smart_objects/smart_object.h"
#include "application_manager/application.h"
#include "application_manager/event_engine/event_observer.h"
namespace resumption {

namespace app_mngr = application_manager;

struct ResumptionRequest {
  hmi_apis::FunctionID::eType function_id;
  int32_t correlation_id;
  smart_objects::SmartObject message;
};

struct ApplicationResumptionStatus {
  std::vector<ResumptionRequest> list_of_sent_requests;
  std::vector<ResumptionRequest> error_requests;
  std::vector<ResumptionRequest> successful_requests;
};

/**
 * @brief Contains logic for the resumption and revert resumption data of
 *  applications.
 */
class ResumptionDataProcessor : public app_mngr::event_engine::EventObserver {
 public:
  /**
  * @brief allows to create ResumptionDataProcessor object
  */
  explicit ResumptionDataProcessor(
      application_manager::ApplicationManager& application_manager);

  /**
   * @brief allows to destroy ResumptionDataProcessor object
   */
  ~ResumptionDataProcessor();

  /**
  * @brief Running resumption data process from saved_app to application.
  * @param application application which will be resumed
  * @param saved_app application specific section from backup file
  */
  void Restore(app_mngr::ApplicationSharedPtr application,
               smart_objects::SmartObject& saved_app,
               std::function<void(mobile_apis::Result::eType,
                                  const std::string&)> callback);

  /**
   * @brief Event, that raised if application get resumption response from HMI
   * @param event : event object, that contains smart_object HMI response
   * message
   */
  void on_event(const app_mngr::event_engine::Event& event) OVERRIDE;

 private:
  /**
   * @brief Revert the data to the state before Resumption
   * @param app_id application id
   */
  void RevertRestoredData(const int32_t app_id);

  /**
   * @brief subscribe to events for the application and save request
   * @param app_id application id
   * @param request for saving
   */
  void WaitForResponse(const int32_t app_id, const ResumptionRequest& request);

  /**
   * @brief Process specified HMI request
   * @param request Request to process
   * @param use_events Process request events or not flag
   * @return TRUE on success, otherwise FALSE
   */
  void ProcessHMIRequest(smart_objects::SmartObjectSPtr request,
                         bool subscribe_on_response);

  /**
   * @brief Process list of HMI requests using ProcessHMIRequest method
   * @param requests List of requests to process
   */
  void ProcessHMIRequests(const smart_objects::SmartObjectList& requests);

  /**
   * @brief AddFiles allows to add files for the application
   * which should be resumed
   * @param application application which will be resumed
   * @param saved_app application specific section from backup file
   */
  void AddFiles(app_mngr::ApplicationSharedPtr application,
                const smart_objects::SmartObject& saved_app);

  /**
   * @brief Deleting files that have been resumed
   * @param app_id application id
   */
  void DeleteFiles(const int32_t app_id);

  /**
  * @brief AddSubmenues allows to add sub-menus for the application
  * which should be resumed
  * @param application application which will be resumed
  * @param saved_app application specific section from backup file
  */
  void AddSubmenues(app_mngr::ApplicationSharedPtr application,
                    const smart_objects::SmartObject& saved_app);

  /**
   * @brief Deleting sub-menus that have been resumed
   * @param app_id application id
   */
  void DeleteSubmenues(const int32_t app_id);

  /**
   * @brief AddCommands allows to add commands for the application
   * which should be resumed
   * @param application application which will be resumed
   * @param saved_app application specific section from backup file
   */
  void AddCommands(app_mngr::ApplicationSharedPtr application,
                   const smart_objects::SmartObject& saved_app);

  /**
   * @brief Deleting all commands that have been resumed
   * @param app_id application id
   */
  void DeleteCommands(const int32_t app_id);

  /**
   * @brief Deleting UI commands that have been resumed
   * @param app_id application id
   */
  void DeleteUICommands(const ResumptionRequest& request);

  /**
 * @brief Deleting VR commands that have been resumed
 * @param app_id application id
 */
  void DeleteVRCommands(const ResumptionRequest& request);

  /**
  * @brief AddChoicesets allows to add choice sets for the application
  * which should be resumed
  * @param application application which will be resumed
  * @param saved_app application specific section from backup file
  */
  void AddChoicesets(app_mngr::ApplicationSharedPtr application,
                     const smart_objects::SmartObject& saved_app);

  /**
   * @brief Deleting choice sets that have been resumed
   * @param app_id application id
   */
  void DeleteChoicesets(const int32_t app_id);

  /**
  * @brief SetGlobalProperties allows to restore global properties.
  * @param application application which will be resumed
  * @param saved_app application specific section from backup file
  */
  void SetGlobalProperties(app_mngr::ApplicationSharedPtr application,
                           const smart_objects::SmartObject& saved_app);

  /**
   * @brief Reset global properties that have been resumed
   * @param app_id application id
   */
  void DeleteGlobalProperties(const int32_t app_id);

  /**
  * @brief AddSubscriptions allows to restore subscriptions
  * @param application application which will be resumed
  * @param saved_app application specific section from backup file
  */
  void AddSubscriptions(app_mngr::ApplicationSharedPtr application,
                        const smart_objects::SmartObject& saved_app);

  /**
  * @brief AddSubscriptions allows to restore subscriptions
  * @param application application which will be resumed
  * @param saved_app application specific section from backup file
  */
  void AddButtonsSubscriptions(app_mngr::ApplicationSharedPtr application,
                               const smart_objects::SmartObject& saved_app);

  /**
  * @brief AddSubscriptions allows to restore subscriptions
  * @param application application which will be resumed
  * @param saved_app application specific section from backup file
  */
  void AddPluginsSubscriptions(app_mngr::ApplicationSharedPtr application,
                               const smart_objects::SmartObject& saved_app);

  /**
   * @brief Deleting subscriptions have been resumed
   * @param app_id application id
   */
  void DeleteSubscriptions(const int32_t app_id);

  /**
   * @brief Deleting buttons subscriptions have been resumed
   * @param app_id application id
   */
  void DeleteButtonsSubscriptions(app_mngr::ApplicationSharedPtr application);

  /**
   * @brief Deleting plugins subscriptions have been resumed
   * @param app_id application id
   */
  void DeletePluginsSubscriptions(app_mngr::ApplicationSharedPtr application);

  /**
  * @brief AddWayPointsSubscription allows to restore subscription
  * for WayPoints
  * @param application application which will be resumed
  * @param saved_app application specific section from backup file
  */
  void AddWayPointsSubscription(app_mngr::ApplicationSharedPtr application,
                                const smart_objects::SmartObject& saved_app);

  /**
   * @brief Deleting subscription for WayPoints have been resumed
   * @param app_id application id
   */
  void DeleteWayPointsSubscription(const int32_t app_id);

  /**
   * @brief A map of the IDs and Application Resumption Status for these ID
   **/
  app_mngr::ApplicationManager& application_manager_;
  std::map<std::int32_t, ApplicationResumptionStatus> resumption_status_;
  std::map<std::int32_t, std::function<void(mobile_apis::Result::eType,
                                            const std::string&)>> register_callbacks_;

};

}  // namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_PROCESS_

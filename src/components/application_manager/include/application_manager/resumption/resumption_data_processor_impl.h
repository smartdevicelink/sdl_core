/*
 Copyright (c) 2020, Ford Motor Company
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
#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_PROCESSOR_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_PROCESSOR_IMPL_H_

#include <functional>
#include <map>
#include <vector>

#include "application_manager/application.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/resumption/resume_ctrl.h"
#include "application_manager/resumption/resumption_data_processor.h"
#include "smart_objects/smart_object.h"
#include "utils/rwlock.h"

namespace resumption {

namespace app_mngr = application_manager;

/**
 * @brief ApplicationResumptionStatus contains information about all requests,
 * which was sent, and results of this operation
 */
struct ApplicationResumptionStatus {
  std::vector<ResumptionRequest> list_of_sent_requests;
  std::vector<ResumptionRequest> error_requests;
  std::vector<ResumptionRequest> successful_requests;
  std::vector<std::string> unsuccessful_vehicle_data_subscriptions_;
  std::vector<std::string> successful_vehicle_data_subscriptions_;
  std::vector<ModuleUid> successful_module_subscriptions_;
  std::vector<ModuleUid> unsuccessful_module_subscriptions_;
};

/**
 * @brief Contains logic for the resumption and revert resumption data of
 *  applications.
 */
class ResumptionDataProcessorImpl
    : public ResumptionDataProcessor,
      public app_mngr::event_engine::EventObserver {
 public:
  /**
   * @brief allows to create ResumptionDataProcessor object
   */
  explicit ResumptionDataProcessorImpl(
      application_manager::ApplicationManager& application_manager);

  /**
   * @brief allows to destroy ResumptionDataProcessor object
   */
  ~ResumptionDataProcessorImpl();

  void Restore(app_mngr::ApplicationSharedPtr application,
               smart_objects::SmartObject& saved_app,
               ResumeCtrl::ResumptionCallBack callback) override;

  void on_event(const app_mngr::event_engine::Event& event) override;

  void HandleOnTimeOut(const uint32_t correlation_id,
                       const hmi_apis::FunctionID::eType function_id) override;

  void SubscribeToResponse(const int32_t app_id,
                           const ResumptionRequest& request) override;

 private:
  /**
   * @brief GetAppIdWaitingForResponse returns application ID, which corresponds
   * to function ID and correlation ID from event
   * @param function_id Function ID
   * @param corr_id Correlation ID
   * @return optional object, which contains application ID, or empty optional,
   * if such ID wasn't found
   */
  utils::Optional<uint32_t> GetAppIdWaitingForResponse(
      const hmi_apis::FunctionID::eType function_id, const int32_t corr_id);

  /**
   * @brief GetRequest returns ResumptionRequest, which corresponds to
   * function ID and correlation ID from event
   * @param app_id ID of application, related to event
   * @param function_id Function ID
   * @param corr_id Correlation ID
   * @return optional object, which contains resumption request, or empty
   * optional, if such request wasn't found
   */
  utils::Optional<ResumptionRequest> GetRequest(
      const uint32_t app_id,
      const hmi_apis::FunctionID::eType function_id,
      const int32_t corr_id);

  /**
   * @brief ProcessResumptionStatus processes received response to determine
   * whether it is successful or not
   * @param app_id ID of application, related to event
   * @param response reference to response message
   * @param found_request reference to found request
   */
  void ProcessResumptionStatus(const uint32_t app_id,
                               const smart_objects::SmartObject& response,
                               const ResumptionRequest& found_request);

  /**
   * @brief EraseProcessedRequest erases processed request from list of pending
   * requests
   * @param app_id ID of application, related to event
   * @param found_request reference to found request
   */
  void EraseProcessedRequest(const uint32_t app_id,
                             const ResumptionRequest& found_request);

  /**
   * @brief IsResumptionFinished checks whether some responses are still
   * waiting
   * @param app_id ID of application, related to event
   * @return true, if resumption for this application is finished, or false, if
   * some requests aren't processed yet
   */
  bool IsResumptionFinished(const uint32_t app_id) const;

  /**
   * @brief IsResumptionSuccessful checks whether overall resumption status
   * is successful or not
   * @param app_id ID of application, related to event
   * @return true, if resumption is successful, false otherwise
   */
  bool IsResumptionSuccessful(const uint32_t app_id);

  /**
   * @brief GetResumptionCallback returns ResumptionCallBack, which was stored
   * for application, related to event
   * @param app_id ID of application, related to event
   * @return optional object, which contains callback, or empty optional,
   * if callback wasn't found for such application ID
   */
  utils::Optional<ResumeCtrl::ResumptionCallBack> GetResumptionCallback(
      const uint32_t app_id);

  /**
   * @brief EraseAppResumptionData erases data, needed for resumption, for
   * given application
   * @param app_id ID of application, related to event
   */
  void EraseAppResumptionData(const uint32_t app_id);

  /**
   * @brief Processes response message from HMI
   * @param response reference to response message
   * @param function_id function id of response
   * @param corr_id correlation id of response
   */
  void ProcessResponseFromHMI(const smart_objects::SmartObject& response,
                              const hmi_apis::FunctionID::eType function_id,
                              const int32_t corr_id);

  /**
   * @brief Checks whether resumption is successful and finalizes resumption
   * corresponding to result of this check
   * @param callback Function to be called when data resumption will be finished
   * @param app_id ID of application, related to event
   */
  void FinalizeResumption(const ResumeCtrl::ResumptionCallBack& callback,
                          const uint32_t app_id);
  /**
   * @brief Revert the data to the state before Resumption
   * @param shared ptr to application
   */
  void RevertRestoredData(app_mngr::ApplicationSharedPtr application);

  /**
   * @brief Process specified HMI message
   * @param request Message to process
   * @param subscribe_on_response flag to specify should message events be
   * processed or not
   * @return TRUE on success, otherwise FALSE
   */
  void ProcessMessageToHMI(smart_objects::SmartObjectSPtr request,
                           bool subscribe_on_response);

  /**
   * @brief Process list of HMI messages using ProcessHMIRequest method
   * @param messages List of messages to process
   */
  void ProcessMessagesToHMI(const smart_objects::SmartObjectList& messages);

  /**
   * @brief AddFiles allows to add files for the application
   * which should be resumed
   * @param application application which will be resumed
   * @param saved_app application specific section from backup file
   */
  void AddFiles(app_mngr::ApplicationSharedPtr application,
                const smart_objects::SmartObject& saved_app);

  /**
   * @brief AddWindows allows to add widget windows for the application
   * @param application application which will be resumed
   * @param saved_app application specific section from backup file
   */
  void AddWindows(app_mngr::ApplicationSharedPtr application,
                  const smart_objects::SmartObject& saved_app);

  /**
   * @brief AddSubmenus allows to add sub-menus for the application
   * which should be resumed
   * @param application application which will be resumed
   * @param saved_app application specific section from backup file
   */
  void AddSubmenus(app_mngr::ApplicationSharedPtr application,
                   const smart_objects::SmartObject& saved_app);

  /**
   * @brief Deleting sub-menus that have been resumed
   * @param shared ptr to application
   */
  void DeleteSubmenus(app_mngr::ApplicationSharedPtr application);

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
   * @param shared ptr to application
   */
  void DeleteCommands(app_mngr::ApplicationSharedPtr application);

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
   * @param shared ptr to application
   */
  void DeleteChoicesets(app_mngr::ApplicationSharedPtr application);

  /**
   * @brief SetGlobalProperties allows to restore global properties.
   * @param application application which will be resumed
   * @param saved_app application specific section from backup file
   */
  void SetGlobalProperties(app_mngr::ApplicationSharedPtr application,
                           const smart_objects::SmartObject& saved_app);

  /**
   * @brief Reset global properties that have been resumed
   * @param shared ptr to application
   */
  void DeleteGlobalProperties(app_mngr::ApplicationSharedPtr application);

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
   * @param shared ptr to application
   */
  void DeleteSubscriptions(app_mngr::ApplicationSharedPtr application);

  /**
   * @brief Deleting buttons subscriptions have been resumed
   * @param shared ptr to application
   */
  void DeleteButtonsSubscriptions(app_mngr::ApplicationSharedPtr application);

  /**
   * @brief Deleting plugins subscriptions have been resumed
   * @param app_id application id
   */
  void DeletePluginsSubscriptions(app_mngr::ApplicationSharedPtr application);

  /**
   * @brief Deletting subscription for CreateWindow have been resumed
   * @param shared ptr to application
   */
  void DeleteWindowsSubscriptions(app_mngr::ApplicationSharedPtr application);

  /**
   * @brief Get button subscriptions that need to be resumed.
   * Since some subscriptions can be set by default during
   * app registration, this function is needed to discard subscriptions
   * that do not need to be resumed
   * @param application which subscriptions to resume
   */
  app_mngr::ButtonSubscriptions GetButtonSubscriptionsToResume(
      app_mngr::ApplicationSharedPtr application) const;

  /**
   * @brief Checks whether SubscribeVehicleData response successful or not and
   * handles it
   * @param request reference to request SO
   * @param response reference to response SO
   * @param status reference to struct, which stores informaion about
   * successful and unsuccessful subscriptions to vehicle data
   */
  void CheckVehicleDataResponse(const smart_objects::SmartObject& request,
                                const smart_objects::SmartObject& response,
                                ApplicationResumptionStatus& status);

  void CheckModuleDataSubscription(const smart_objects::SmartObject& request,
                                   const smart_objects::SmartObject& response,
                                   ApplicationResumptionStatus& status);

  /**
   * @brief Checks whether CreateWindow response successful or not and handles
   * it
   * @param request reference to request SO
   * @param response reference to response SO
   */
  void CheckCreateWindowResponse(
      const smart_objects::SmartObject& request,
      const smart_objects::SmartObject& response) const;

  app_mngr::ApplicationManager& application_manager_;

  /**
   * @brief A map of the IDs and Application Resumption Status for these ID
   **/
  std::map<std::int32_t, ApplicationResumptionStatus> resumption_status_;
  mutable sync_primitives::RWLock resumption_status_lock_;

  /**
   * @brief A map of callbacks used when resumption is finished
   */
  std::map<std::int32_t, ResumeCtrl::ResumptionCallBack> register_callbacks_;
  mutable sync_primitives::RWLock register_callbacks_lock_;

  /**
   * @brief A map of sent requests and corresponding app_id
   */
  std::map<ResumptionRequestID, std::uint32_t> request_app_ids_;
  mutable sync_primitives::RWLock request_app_ids_lock_;
};

}  // namespace resumption

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_RESUMPTION_RESUMPTION_DATA_PROCESSOR_IMPL_H_

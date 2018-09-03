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

#include <algorithm>

#include "application_manager/resumption/resumption_data_processor.h"
#include "application_manager/application_manager.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/message_helper.h"
#include "application_manager/event_engine/event_observer.h"

namespace resumption {

using app_mngr::ApplicationSharedPtr;
using app_mngr::AppFile;
using app_mngr::MessageHelper;
using app_mngr::ChoiceSetMap;
using app_mngr::ButtonSubscriptions;
namespace strings = app_mngr::strings;
namespace event_engine = app_mngr::event_engine;

CREATE_LOGGERPTR_GLOBAL(logger_, "Resumption")

ResumptionDataProcessor::ResumptionDataProcessor(
    app_mngr::ApplicationManager& application_manager)
    : event_engine::EventObserver(application_manager.event_dispatcher())
    , application_manager_(application_manager) {}

ResumptionDataProcessor::~ResumptionDataProcessor() {}

void ResumptionDataProcessor::Restore(ApplicationSharedPtr application,
                                      smart_objects::SmartObject& saved_app,
                                      ResumeCtrl::ResumptionCallBack callback) {
  LOG4CXX_AUTO_TRACE(logger_);
  AddFiles(application, saved_app);
  AddSubmenues(application, saved_app);
  AddCommands(application, saved_app);
  AddChoicesets(application, saved_app);
  SetGlobalProperties(application, saved_app);
  AddSubscriptions(application, saved_app);
  AddWayPointsSubscription(application, saved_app);
  register_callbacks_[application->app_id()] = callback;
}

bool ResumptionRequestIDs::operator<(const ResumptionRequestIDs& other) const {
  LOG4CXX_AUTO_TRACE(logger_);
  return correlation_id < other.correlation_id ||
         function_id < other.function_id;
}

void ResumptionDataProcessor::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& response = event.smart_object();

  ResumptionRequestIDs request_ids;
  request_ids.function_id = event.id();
  request_ids.correlation_id = event.smart_object_correlation_id();
  //TODO i suppose it can be optimised with moving app id into
  //ResumptionRequest struct, so that we don't have to perform
  //basically the same search twice
  auto predicate =
      [&event](const std::pair<ResumptionRequestIDs, std::uint32_t>& item) {
        return item.first.function_id == event.id() &&
               item.first.correlation_id == event.smart_object_correlation_id();
      };

  auto app_id_ptr =
      std::find_if(request_app_ids_.begin(),
                   request_app_ids_.end(),
                   predicate);

  if (app_id_ptr == request_app_ids_.end()) {
    LOG4CXX_ERROR(logger_,
                  "application id for correlation id "
                      << event.smart_object_correlation_id()
                      << " and function id: " << event.id()
                      << " was not found.");
    return;
  }

  const uint32_t app_id = app_id_ptr->second;

  LOG4CXX_DEBUG(logger_, "app_id is: " << app_id);

  LOG4CXX_DEBUG(logger_,
                "Found function id: " << app_id_ptr->first.function_id
                                      << " correlation id: "
                                      << app_id_ptr->first.correlation_id);

  LOG4CXX_DEBUG(logger_,
                "Now processing event with function id: "
                    << request_ids.function_id
                    << " correlation id: " << request_ids.correlation_id);

  ApplicationResumptionStatus& status = resumption_status_[app_id];
  std::vector<ResumptionRequest>& list_of_sent_requests =
      status.list_of_sent_requests;

  auto request_ptr =
      std::find_if(list_of_sent_requests.begin(),
                   list_of_sent_requests.end(),
                   [&event](const ResumptionRequest& request) {
                     return request.request_ids.correlation_id ==
                                event.smart_object_correlation_id() &&
                            request.request_ids.function_id == event.id();
                   });

  if (list_of_sent_requests.end() == request_ptr) {
    LOG4CXX_ERROR(logger_, "Request not found");
    return;
  }

  const hmi_apis::Common_Result::eType result_code =
      static_cast<hmi_apis::Common_Result::eType>(
      response[strings::params][application_manager::hmi_response::code]
              .asInt());

  if (result_code == hmi_apis::Common_Result::SUCCESS) {
    status.successful_requests.push_back(*request_ptr);
  } else {
    status.error_requests.push_back(*request_ptr);
  }
  list_of_sent_requests.erase(request_ptr);

  if (!list_of_sent_requests.empty()) {
    LOG4CXX_DEBUG(logger_, "is not the last response for this application");
    return;
  }

  auto it = register_callbacks_.find(app_id);
  DCHECK_OR_RETURN_VOID(it != register_callbacks_.end());
  auto callback = it->second;
  if (status.error_requests.empty()) {
    LOG4CXX_DEBUG(logger_, "Resumption for app " << app_id << "successful");
    callback(mobile_apis::Result::SUCCESS, "Data resumption succesful");
  }
  if (!status.error_requests.empty()) {
    LOG4CXX_ERROR(logger_, "Resumption for app " << app_id << "failed");
    RevertRestoredData(app_id);
    callback(mobile_apis::Result::RESUME_FAILED, "Data resumption failed");
  }
  resumption_status_.erase(app_id);
  request_app_ids_.erase(app_id_ptr);
}

void ResumptionDataProcessor::RevertRestoredData(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  DeleteFiles(app_id);
  DeleteSubmenues(app_id);
  DeleteCommands(app_id);
  DeleteChoicesets(app_id);
  DeleteGlobalProperties(app_id);
  DeleteSubscriptions(app_id);
  DeleteWayPointsSubscription(app_id);
}

void ResumptionDataProcessor::WaitForResponse(
    const int32_t app_id, const ResumptionRequest& request) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_,
                "app " << app_id << "Subscribe on "
                       << request.request_ids.function_id << " "
                       << request.request_ids.correlation_id);
  subscribe_on_event(request.request_ids.function_id,
                     request.request_ids.correlation_id);
  resumption_status_[app_id].list_of_sent_requests.push_back(request);
  request_app_ids_.insert(std::make_pair(request.request_ids, app_id));
}

void ResumptionDataProcessor::ProcessHMIRequest(
    smart_objects::SmartObjectSPtr request, bool subscribe_on_response) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (subscribe_on_response) {
    auto function_id = static_cast<hmi_apis::FunctionID::eType>(
        (*request)[strings::params][strings::function_id].asInt());

    const int32_t hmi_correlation_id =
        (*request)[strings::params][strings::correlation_id].asInt();

    const int32_t app_id =
        (*request)[strings::msg_params][strings::app_id].asInt();

    ResumptionRequest wait_for_response;
    wait_for_response.request_ids.correlation_id = hmi_correlation_id;
    wait_for_response.request_ids.function_id = function_id;
    wait_for_response.message = *request;

    WaitForResponse(app_id, wait_for_response);
  }
  if (!application_manager_.GetRPCService().ManageHMICommand(request)) {
    LOG4CXX_ERROR(logger_, "Unable to send request");
  }
}

void ResumptionDataProcessor::ProcessHMIRequests(
    const smart_objects::SmartObjectList& requests) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (requests.empty()) {
    LOG4CXX_DEBUG(logger_, "requests list is empty");
    return;
  }

  for (const auto& it : requests) {
    ProcessHMIRequest(it, /*subscribe_on_response = */ true);
  }
}

void ResumptionDataProcessor::AddFiles(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!saved_app.keyExists(strings::application_files)) {
    LOG4CXX_ERROR(logger_, "application_files section is not exists");
    return;
  }

  const smart_objects::SmartObject& application_files =
      saved_app[strings::application_files];

  for (size_t i = 0; i < application_files.length(); ++i) {
    const smart_objects::SmartObject& file_data = application_files[i];
    const bool is_persistent = file_data.keyExists(strings::persistent_file) &&
                               file_data[strings::persistent_file].asBool();
    if (is_persistent) {
      AppFile file;
      file.is_persistent = is_persistent;
      file.is_download_complete =
          file_data[strings::is_download_complete].asBool();
      file.file_name = file_data[strings::sync_file_name].asString();
      file.file_type = static_cast<mobile_apis::FileType::eType>(
          file_data[strings::file_type].asInt());
      application->AddFile(file);
    }
  }
}

void ResumptionDataProcessor::DeleteFiles(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr application = application_manager_.application(app_id);
  while (!application->getAppFiles().empty()) {
    application->DeleteFile(application->getAppFiles().begin()->first);
  }
}

void ResumptionDataProcessor::AddSubmenues(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!saved_app.keyExists(strings::application_submenus)) {
    LOG4CXX_ERROR(logger_, "application_submenus section is not exists");
    return;
  }

  const smart_objects::SmartObject& app_submenus =
      saved_app[strings::application_submenus];

  for (size_t i = 0; i < app_submenus.length(); ++i) {
    const smart_objects::SmartObject& submenu = app_submenus[i];
    application->AddSubMenu(submenu[strings::menu_id].asUInt(), submenu);
  }

  ProcessHMIRequests(MessageHelper::CreateAddSubMenuRequestsToHMI(
      application, application_manager_.GetNextHMICorrelationID()));
}

void ResumptionDataProcessor::DeleteSubmenues(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationResumptionStatus& status = resumption_status_[app_id];
  ApplicationSharedPtr application = application_manager_.application(app_id);
  for (auto request : status.successful_requests) {
    if (hmi_apis::FunctionID::UI_AddSubMenu ==
        request.request_ids.function_id) {
      smart_objects::SmartObjectSPtr ui_sub_menu =
          MessageHelper::CreateMessageForHMI(
              hmi_apis::messageType::request,
              application_manager_.GetNextHMICorrelationID());

      (*ui_sub_menu)[strings::params][strings::function_id] =
          hmi_apis::FunctionID::UI_DeleteSubMenu;

      smart_objects::SmartObject msg_params =
          smart_objects::SmartObject(smart_objects::SmartType_Map);

      msg_params[strings::menu_id] =
          request.message[strings::msg_params][strings::menu_id];
      msg_params[strings::app_id] =
          request.message[strings::msg_params][strings::app_id];

      (*ui_sub_menu)[strings::msg_params] = msg_params;

      application->RemoveSubMenu(msg_params[strings::menu_id].asInt());
      ProcessHMIRequest(ui_sub_menu, /*subscribe_on_response = */ false);
    }
  }
}

void ResumptionDataProcessor::AddCommands(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!saved_app.keyExists(strings::application_commands)) {
    LOG4CXX_ERROR(logger_, "application_commands section is not exists");
  }

  const smart_objects::SmartObject& app_commands =
      saved_app[strings::application_commands];

  for (size_t i = 0; i < app_commands.length(); ++i) {
    const smart_objects::SmartObject& command = app_commands[i];
    const uint32_t cmd_id = command[strings::cmd_id].asUInt();

    application->AddCommand(cmd_id, command);
    application->help_prompt_manager().OnVrCommandAdded(
        cmd_id, command, /*is_resumption =*/true);
  }

  ProcessHMIRequests(MessageHelper::CreateAddCommandRequestToHMI(
      application, application_manager_));
}

void ResumptionDataProcessor::DeleteCommands(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr application = application_manager_.application(app_id);
  ApplicationResumptionStatus& status = resumption_status_[app_id];

  for (auto request : status.successful_requests) {
    const uint32_t cmd_id =
        request.message[strings::msg_params][strings::cmd_id].asUInt();

    if (hmi_apis::FunctionID::UI_AddCommand ==
        request.request_ids.function_id) {
      DeleteUICommands(request);
    }
    if (hmi_apis::FunctionID::VR_AddCommand ==
        request.request_ids.function_id) {
      DeleteVRCommands(request);
    }

    application->RemoveCommand(cmd_id);
    application->help_prompt_manager().OnVrCommandDeleted(
        cmd_id, /*is_resumption = */ true);
  }
}

void ResumptionDataProcessor::DeleteUICommands(
    const ResumptionRequest& request) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr ui_command =
      MessageHelper::CreateMessageForHMI(
          hmi_apis::messageType::request,
          application_manager_.GetNextHMICorrelationID());

  (*ui_command)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::UI_DeleteCommand;

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  const uint32_t cmd_id =
      request.message[strings::msg_params][strings::cmd_id].asUInt();

  msg_params[strings::cmd_id] = cmd_id;
  msg_params[strings::app_id] =
      request.message[strings::msg_params][strings::app_id];

  (*ui_command)[strings::msg_params] = msg_params;

  ProcessHMIRequest(ui_command, /*subscribe_on_response = */ false);
}

void ResumptionDataProcessor::DeleteVRCommands(
    const ResumptionRequest& request) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObjectSPtr vr_command =
      MessageHelper::CreateMessageForHMI(
          hmi_apis::messageType::request,
          application_manager_.GetNextHMICorrelationID());

  (*vr_command)[strings::params][strings::function_id] =
      hmi_apis::FunctionID::VR_DeleteCommand;

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  const uint32_t cmd_id =
      request.message[strings::msg_params][strings::cmd_id].asUInt();

  msg_params[strings::cmd_id] = cmd_id;
  msg_params[strings::app_id] =
      request.message[strings::msg_params][strings::app_id];
  msg_params[strings::type] =
      request.message[strings::msg_params][strings::type];
  msg_params[strings::grammar_id] =
      request.message[strings::msg_params][strings::grammar_id];

  (*vr_command)[strings::msg_params] = msg_params;

  ProcessHMIRequest(vr_command, /*subscribe_on_response = */ false);
}

void ResumptionDataProcessor::AddChoicesets(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!saved_app.keyExists(strings::application_choice_sets)) {
    LOG4CXX_ERROR(logger_, "There is no any choicesets");
  }

  const smart_objects::SmartObject& app_choice_sets =
      saved_app[strings::application_choice_sets];

  for (size_t i = 0; i < app_choice_sets.length(); ++i) {
    const smart_objects::SmartObject& choice_set = app_choice_sets[i];
    const int32_t choice_set_id =
        choice_set[strings::interaction_choice_set_id].asInt();
    application->AddChoiceSet(choice_set_id, choice_set);
  }

  ProcessHMIRequests(MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(
      application, application_manager_));
}

void ResumptionDataProcessor::DeleteChoicesets(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr application = application_manager_.application(app_id);

  const DataAccessor<ChoiceSetMap> accessor = application->choice_set_map();
  const ChoiceSetMap& choices = accessor.GetData();
  while (!choices.empty()) {
    application->RemoveChoiceSet(choices.begin()->first);
  }
}

void ResumptionDataProcessor::SetGlobalProperties(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!saved_app.keyExists(strings::application_global_properties)) {
    LOG4CXX_DEBUG(logger_,
                  "application_global_properties section is not exists");
    return;
  }

  const smart_objects::SmartObject& properties_so =
      saved_app[strings::application_global_properties];
  application->load_global_properties(properties_so);

  ProcessHMIRequests(MessageHelper::CreateGlobalPropertiesRequestsToHMI(
      application, application_manager_));
}

void ResumptionDataProcessor::DeleteGlobalProperties(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  const auto application = application_manager_.application(app_id);

  const auto result =
      application_manager_.ResetAllApplicationGlobalProperties(app_id);

  if (result.HasUIPropertiesReset()) {
    smart_objects::SmartObjectSPtr ui_gl_props_reset_req =
        MessageHelper::CreateUIResetGlobalPropertiesRequest(result,
                                                            application);

    ProcessHMIRequest(ui_gl_props_reset_req, false);
  }
  if (result.HasTTSPropertiesReset()) {
    smart_objects::SmartObjectSPtr tts_gl_props_reset_req =
        MessageHelper::CreateUIResetGlobalPropertiesRequest(result,
                                                            application);

    ProcessHMIRequest(tts_gl_props_reset_req, false);
  }
}

void ResumptionDataProcessor::AddWayPointsSubscription(
    app_mngr::ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (saved_app.keyExists(strings::subscribed_for_way_points)) {
    LOG4CXX_DEBUG(logger_, "subscribed_for_way_points section is not exists");
    return;
  }

  const smart_objects::SmartObject& subscribed_for_way_points_so =
      saved_app[strings::subscribed_for_way_points];
  if (true == subscribed_for_way_points_so.asBool()) {
    application_manager_.SubscribeAppForWayPoints(application);
  }
}

void ResumptionDataProcessor::DeleteWayPointsSubscription(
    const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (application_manager_.IsAppSubscribedForWayPoints(app_id)) {
    application_manager_.UnsubscribeAppFromWayPoints(app_id);
  }
}

void ResumptionDataProcessor::AddSubscriptions(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);

  AddButtonsSubscriptions(application, saved_app);
  AddPluginsSubscriptions(application, saved_app);
}

void ResumptionDataProcessor::AddButtonsSubscriptions(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!saved_app.keyExists(strings::application_subscriptions)) {
    LOG4CXX_DEBUG(logger_, "application_subscriptions section is not exists");
    return;
  }

  const smart_objects::SmartObject& subscriptions =
      saved_app[strings::application_subscriptions];

  if (subscriptions.keyExists(strings::application_buttons)) {
    const smart_objects::SmartObject& subscriptions_buttons =
        subscriptions[strings::application_buttons];
    mobile_apis::ButtonName::eType btn;
    for (size_t i = 0; i < subscriptions_buttons.length(); ++i) {
      btn = static_cast<mobile_apis::ButtonName::eType>(
          (subscriptions_buttons[i]).asInt());
      application->SubscribeToButton(btn);
    }
  }

  MessageHelper::SendAllOnButtonSubscriptionNotificationsForApp(
      application, application_manager_);
}

void ResumptionDataProcessor::AddPluginsSubscriptions(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  LOG4CXX_AUTO_TRACE(logger_);

  for (auto& extension : application->Extensions()) {
    extension->ProcessResumption(
        saved_app,
        [this](const int32_t app_id, const ResumptionRequest request) {
          this->WaitForResponse(app_id, request);
        });
  }
}

void ResumptionDataProcessor::DeleteSubscriptions(const int32_t app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  ApplicationSharedPtr application = application_manager_.application(app_id);
  DeleteButtonsSubscriptions(application);
  DeletePluginsSubscriptions(application);
}

void ResumptionDataProcessor::DeleteButtonsSubscriptions(
    ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  DataAccessor<ButtonSubscriptions> button_accessor =
      application->SubscribedButtons();
  ButtonSubscriptions button_subscriptions = button_accessor.GetData();
  while (!button_subscriptions.empty()) {
    auto btn = button_subscriptions.begin();
    MessageHelper::SendOnButtonSubscriptionNotification(
        application->hmi_app_id(),
        static_cast<hmi_apis::Common_ButtonName::eType>(*btn),
        /*is_subscribed = */ false,
        application_manager_);
    application->UnsubscribeFromButton(
        static_cast<mobile_apis::ButtonName::eType>(*btn));
  }

  MessageHelper::SendOnButtonSubscriptionNotification(
      application->hmi_app_id(),
      hmi_apis::Common_ButtonName::CUSTOM_BUTTON,
      /*is_subscribed = */ false,
      application_manager_);
  application->SubscribeToButton(mobile_apis::ButtonName::CUSTOM_BUTTON);
}

void ResumptionDataProcessor::DeletePluginsSubscriptions(
    application_manager::ApplicationSharedPtr application) {
  LOG4CXX_AUTO_TRACE(logger_);
  smart_objects::SmartObject extension_subscriptions;

  ApplicationResumptionStatus& status =
      resumption_status_[application->app_id()];
  for (auto request : status.successful_requests) {
    if (hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData ==
        request.request_ids.function_id) {
      extension_subscriptions[strings::application_vehicle_info] =
          request.message;
    }
  }

  for (auto& extension : application->Extensions()) {
    extension->RevertResumption(extension_subscriptions);
  }
}

}  // namespce resumption

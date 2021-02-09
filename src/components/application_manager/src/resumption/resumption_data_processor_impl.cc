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

#include <algorithm>

#include "application_manager/application_manager.h"
#include "application_manager/commands/command_impl.h"
#include "application_manager/display_capabilities_builder.h"
#include "application_manager/event_engine/event_observer.h"
#include "application_manager/message_helper.h"
#include "application_manager/resumption/resumption_data_processor_impl.h"
#include "application_manager/rpc_plugins/rc_rpc_plugin/include/rc_rpc_plugin/rc_module_constants.h"
#include "application_manager/smart_object_keys.h"

namespace resumption {

using app_mngr::AppFile;
using app_mngr::ApplicationSharedPtr;
using app_mngr::ButtonSubscriptions;
using app_mngr::ChoiceSetMap;
using app_mngr::MessageHelper;
namespace strings = app_mngr::strings;
namespace event_engine = app_mngr::event_engine;
namespace commands = app_mngr::commands;
namespace message_params = rc_rpc_plugin::message_params;

SDL_CREATE_LOG_VARIABLE("Resumption")

bool ResumptionRequestID::operator<(const ResumptionRequestID& other) const {
  return correlation_id < other.correlation_id ||
         function_id < other.function_id;
}

ResumptionDataProcessorImpl::ResumptionDataProcessorImpl(
    app_mngr::ApplicationManager& application_manager)
    : event_engine::EventObserver(application_manager.event_dispatcher())
    , application_manager_(application_manager)
    , resumption_status_lock_()
    , register_callbacks_lock_()
    , request_app_ids_lock_() {}

ResumptionDataProcessorImpl::~ResumptionDataProcessorImpl() {}

void ResumptionDataProcessorImpl::Restore(
    ApplicationSharedPtr application,
    smart_objects::SmartObject& saved_app,
    ResumeCtrl::ResumptionCallBack callback) {
  SDL_LOG_AUTO_TRACE();

  AddFiles(application, saved_app);
  AddSubmenus(application, saved_app);
  AddCommands(application, saved_app);
  AddChoicesets(application, saved_app);
  SetGlobalProperties(application, saved_app);
  AddSubscriptions(application, saved_app);
  AddWindows(application, saved_app);

  const auto app_id = application->app_id();
  if (!IsResumptionFinished(app_id)) {
    sync_primitives::AutoWriteLock lock(register_callbacks_lock_);
    register_callbacks_[app_id] = callback;
  } else {
    FinalizeResumption(callback, app_id);
  }
}

utils::Optional<uint32_t>
ResumptionDataProcessorImpl::GetAppIdWaitingForResponse(
    const hmi_apis::FunctionID::eType function_id, const int32_t corr_id) {
  SDL_LOG_AUTO_TRACE();

  auto predicate =
      [function_id,
       corr_id](const std::pair<ResumptionRequestID, std::uint32_t>& item) {
        return item.first.function_id == function_id &&
               item.first.correlation_id == corr_id;
      };

  sync_primitives::AutoReadLock lock(request_app_ids_lock_);
  auto app_id_ptr =
      std::find_if(request_app_ids_.begin(), request_app_ids_.end(), predicate);

  if (app_id_ptr == request_app_ids_.end()) {
    return utils::Optional<uint32_t>::OptionalEmpty::EMPTY;
  }
  return utils::Optional<uint32_t>(app_id_ptr->second);
}

utils::Optional<ResumptionRequest> ResumptionDataProcessorImpl::GetRequest(
    const uint32_t app_id,
    const hmi_apis::FunctionID::eType function_id,
    const int32_t corr_id) {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoReadLock lock(resumption_status_lock_);
  std::vector<ResumptionRequest>& list_of_sent_requests =
      resumption_status_[app_id].list_of_sent_requests;

  if (resumption_status_.find(app_id) == resumption_status_.end()) {
    SDL_LOG_ERROR("No resumption status info found for app_id: " << app_id);
    return utils::Optional<ResumptionRequest>::OptionalEmpty::EMPTY;
  }

  auto request_iter =
      std::find_if(list_of_sent_requests.begin(),
                   list_of_sent_requests.end(),
                   [function_id, corr_id](const ResumptionRequest& request) {
                     return request.request_id.correlation_id == corr_id &&
                            request.request_id.function_id == function_id;
                   });

  if (list_of_sent_requests.end() == request_iter) {
    return utils::Optional<ResumptionRequest>::OptionalEmpty::EMPTY;
  }
  return utils::Optional<ResumptionRequest>(*request_iter);
}

void ResumptionDataProcessorImpl::ProcessResumptionStatus(
    const uint32_t app_id,
    const smart_objects::SmartObject& response,
    const ResumptionRequest& found_request) {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoWriteLock lock(resumption_status_lock_);
  ApplicationResumptionStatus& status = resumption_status_[app_id];

  if (IsResponseSuccessful(response)) {
    status.successful_requests.push_back(found_request);
  } else {
    status.error_requests.push_back(found_request);
  }

  if (hmi_apis::FunctionID::VehicleInfo_SubscribeVehicleData ==
      found_request.request_id.function_id) {
    CheckVehicleDataResponse(found_request.message, response, status);
  }

  if (hmi_apis::FunctionID::UI_CreateWindow ==
      found_request.request_id.function_id) {
    CheckCreateWindowResponse(found_request.message, response);
  }

  if (hmi_apis::FunctionID::RC_GetInteriorVehicleData ==
      found_request.request_id.function_id) {
    CheckModuleDataSubscription(found_request.message, response, status);
  }
}

void ResumptionDataProcessorImpl::EraseProcessedRequest(
    const uint32_t app_id, const ResumptionRequest& found_request) {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoWriteLock lock(resumption_status_lock_);
  auto& list_of_sent_requests =
      resumption_status_[app_id].list_of_sent_requests;
  auto request_iter =
      std::find_if(list_of_sent_requests.begin(),
                   list_of_sent_requests.end(),
                   [found_request](const ResumptionRequest& request) {
                     return request.request_id.correlation_id ==
                                found_request.request_id.correlation_id &&
                            request.request_id.function_id ==
                                found_request.request_id.function_id;
                   });
  list_of_sent_requests.erase(request_iter);
}

bool ResumptionDataProcessorImpl::IsResumptionFinished(
    const uint32_t app_id) const {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoReadLock lock(resumption_status_lock_);
  bool is_requests_list_empty = true;
  const auto app_status = resumption_status_.find(app_id);
  if (app_status != resumption_status_.end()) {
    is_requests_list_empty = app_status->second.list_of_sent_requests.empty();
  }
  return is_requests_list_empty;
}

utils::Optional<ResumeCtrl::ResumptionCallBack>
ResumptionDataProcessorImpl::GetResumptionCallback(const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();

  sync_primitives::AutoReadLock lock(register_callbacks_lock_);
  auto it = register_callbacks_.find(app_id);
  if (it == register_callbacks_.end()) {
    return utils::Optional<
        ResumeCtrl::ResumptionCallBack>::OptionalEmpty::EMPTY;
  }
  return utils::Optional<ResumeCtrl::ResumptionCallBack>(it->second);
}

bool ResumptionDataProcessorImpl::IsResumptionSuccessful(
    const uint32_t app_id) {
  sync_primitives::AutoReadLock lock(resumption_status_lock_);
  auto it = resumption_status_.find(app_id);
  if (resumption_status_.end() == it) {
    return true;
  }

  const ApplicationResumptionStatus& status = it->second;
  return status.error_requests.empty() &&
         status.unsuccessful_vehicle_data_subscriptions_.empty() &&
         status.unsuccessful_module_subscriptions_.empty();
}

void ResumptionDataProcessorImpl::EraseAppResumptionData(
    const uint32_t app_id) {
  SDL_LOG_AUTO_TRACE();

  std::vector<ResumptionRequest> all_requests;

  resumption_status_lock_.AcquireForWriting();
  all_requests.insert(all_requests.end(),
                      resumption_status_[app_id].successful_requests.begin(),
                      resumption_status_[app_id].successful_requests.end());
  all_requests.insert(all_requests.end(),
                      resumption_status_[app_id].error_requests.begin(),
                      resumption_status_[app_id].error_requests.end());

  resumption_status_.erase(app_id);
  resumption_status_lock_.Release();

  request_app_ids_lock_.AcquireForWriting();
  for (auto request : all_requests) {
    request_app_ids_.erase(
        {request.request_id.function_id, request.request_id.correlation_id});
  }
  request_app_ids_lock_.Release();

  register_callbacks_lock_.AcquireForWriting();
  register_callbacks_.erase(app_id);
  register_callbacks_lock_.Release();
}

void ResumptionDataProcessorImpl::ProcessResponseFromHMI(
    const smart_objects::SmartObject& response,
    const hmi_apis::FunctionID::eType function_id,
    const int32_t corr_id) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_TRACE("Now processing event with function id: "
                << function_id << " correlation id: " << corr_id);

  auto found_app_id = GetAppIdWaitingForResponse(function_id, corr_id);
  if (!found_app_id) {
    SDL_LOG_ERROR("Application id for correlation id "
                  << corr_id << " and function id: " << function_id
                  << " was not found, such response is not expected.");
    return;
  }
  const uint32_t app_id = *found_app_id;
  SDL_LOG_DEBUG("app_id is: " << app_id);

  auto found_request = GetRequest(app_id, function_id, corr_id);
  if (!found_request) {
    SDL_LOG_ERROR("Request with function id " << function_id << " and corr id "
                                              << corr_id << " not found");
    return;
  }
  auto request = *found_request;

  ProcessResumptionStatus(app_id, response, request);
  EraseProcessedRequest(app_id, request);

  if (!IsResumptionFinished(app_id)) {
    SDL_LOG_DEBUG("Resumption app "
                  << app_id << " not finished. Some requests are still waited");
    return;
  }

  auto found_callback = GetResumptionCallback(app_id);
  if (!found_callback) {
    SDL_LOG_ERROR("Callback for app_id: " << app_id << " not found");
    return;
  }
  auto callback = *found_callback;
  FinalizeResumption(callback, app_id);
}

void ResumptionDataProcessorImpl::FinalizeResumption(
    const ResumeCtrl::ResumptionCallBack& callback, const uint32_t app_id) {
  if (IsResumptionSuccessful(app_id)) {
    SDL_LOG_DEBUG("Resumption for app " << app_id << " successful");
    callback(mobile_apis::Result::SUCCESS, "Data resumption successful");
    application_manager_.state_controller().ResumePostponedWindows(app_id);
  } else {
    SDL_LOG_ERROR("Resumption for app " << app_id << " failed");
    callback(mobile_apis::Result::RESUME_FAILED, "Data resumption failed");
    RevertRestoredData(application_manager_.application(app_id));
    application_manager_.state_controller().DropPostponedWindows(app_id);
  }
  EraseAppResumptionData(app_id);
}

void ResumptionDataProcessorImpl::HandleOnTimeOut(
    const uint32_t corr_id, const hmi_apis::FunctionID::eType function_id) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Handling timeout with corr id: "
                << corr_id << " and function_id: " << function_id);

  auto error_response = MessageHelper::CreateNegativeResponseFromHmi(
      function_id,
      corr_id,
      hmi_apis::Common_Result::GENERIC_ERROR,
      std::string());
  ProcessResponseFromHMI(*error_response, function_id, corr_id);
}

void ResumptionDataProcessorImpl::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG(
      "Handling response message from HMI "
      << event.id() << " "
      << event.smart_object()[strings::params][strings::correlation_id]
             .asInt());
  ProcessResponseFromHMI(
      event.smart_object(), event.id(), event.smart_object_correlation_id());
}

std::vector<ResumptionRequest> GetAllFailedRequests(
    uint32_t app_id,
    const std::map<std::int32_t, ApplicationResumptionStatus>&
        resumption_status,
    sync_primitives::RWLock& resumption_status_lock) {
  resumption_status_lock.AcquireForReading();
  std::vector<ResumptionRequest> failed_requests;
  std::vector<ResumptionRequest> missed_requests;
  auto it = resumption_status.find(app_id);
  if (it != resumption_status.end()) {
    failed_requests = it->second.error_requests;
    missed_requests = it->second.list_of_sent_requests;
  }
  resumption_status_lock.Release();

  failed_requests.insert(
      failed_requests.end(), missed_requests.begin(), missed_requests.end());
  return failed_requests;
}

void ResumptionDataProcessorImpl::RevertRestoredData(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Reverting for app: " << application->app_id());
  DeleteSubmenus(application);
  DeleteCommands(application);
  DeleteChoicesets(application);
  DeleteGlobalProperties(application);
  DeleteSubscriptions(application);
  DeleteWindowsSubscriptions(application);

  resumption_status_lock_.AcquireForWriting();
  resumption_status_.erase(application->app_id());
  resumption_status_lock_.Release();

  register_callbacks_lock_.AcquireForWriting();
  register_callbacks_.erase(application->app_id());
  register_callbacks_lock_.Release();
}

void ResumptionDataProcessorImpl::SubscribeToResponse(
    const int32_t app_id, const ResumptionRequest& request) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("App " << app_id << " subscribe on "
                       << request.request_id.function_id << " "
                       << request.request_id.correlation_id);
  subscribe_on_event(request.request_id.function_id,
                     request.request_id.correlation_id);

  resumption_status_lock_.AcquireForWriting();
  resumption_status_[app_id].list_of_sent_requests.push_back(request);
  resumption_status_lock_.Release();

  request_app_ids_lock_.AcquireForWriting();
  request_app_ids_.insert(std::make_pair(request.request_id, app_id));
  request_app_ids_lock_.Release();
}

void ResumptionDataProcessorImpl::ProcessMessageToHMI(
    smart_objects::SmartObjectSPtr message, bool subscribe_on_response) {
  SDL_LOG_AUTO_TRACE();
  if (subscribe_on_response) {
    auto function_id = static_cast<hmi_apis::FunctionID::eType>(
        (*message)[strings::params][strings::function_id].asInt());

    const int32_t hmi_correlation_id =
        (*message)[strings::params][strings::correlation_id].asInt();

    const int32_t app_id =
        (*message)[strings::msg_params][strings::app_id].asInt();

    ResumptionRequest wait_for_response;
    wait_for_response.request_id.correlation_id = hmi_correlation_id;
    wait_for_response.request_id.function_id = function_id;
    wait_for_response.message = *message;

    SubscribeToResponse(app_id, wait_for_response);
  }
  if (!application_manager_.GetRPCService().ManageHMICommand(message)) {
    SDL_LOG_ERROR("Unable to send request");
  }
}

void ResumptionDataProcessorImpl::ProcessMessagesToHMI(
    const smart_objects::SmartObjectList& messages) {
  SDL_LOG_AUTO_TRACE();
  for (const auto& message : messages) {
    const bool is_request_message =
        application_manager::MessageType::kRequest ==
        (*message)[strings::params][strings::message_type].asInt();

    ProcessMessageToHMI(message, is_request_message);
  }
}

void ResumptionDataProcessorImpl::AddFiles(
    app_mngr::ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();
  if (!saved_app.keyExists(strings::application_files)) {
    SDL_LOG_ERROR("application_files section is not exists");
    return;
  }

  const auto application_files =
      saved_app[strings::application_files].asArray();

  for (const auto file_data : *application_files) {
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

void ResumptionDataProcessorImpl::AddWindows(
    application_manager::ApplicationSharedPtr application,
    const ns_smart_device_link::ns_smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  if (!saved_app.keyExists(strings::windows_info)) {
    SDL_LOG_ERROR("windows_info section does not exist");
    return;
  }

  const auto& windows_info = saved_app[strings::windows_info];
  auto request_list = MessageHelper::CreateUICreateWindowRequestsToHMI(
      application, application_manager_, windows_info);

  ProcessMessagesToHMI(request_list);
}

void ResumptionDataProcessorImpl::AddSubmenus(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  if (!saved_app.keyExists(strings::application_submenus)) {
    SDL_LOG_ERROR("application_submenus section is not exists");
    return;
  }

  const smart_objects::SmartObject& app_submenus =
      saved_app[strings::application_submenus];

  for (size_t i = 0; i < app_submenus.length(); ++i) {
    const smart_objects::SmartObject& submenu = app_submenus[i];
    application->AddSubMenu(submenu[strings::menu_id].asUInt(), submenu);
  }

  ProcessMessagesToHMI(MessageHelper::CreateAddSubMenuRequestsToHMI(
      application, application_manager_));
}

utils::Optional<ResumptionRequest> FindResumptionSubmenuRequest(
    uint32_t menu_id, std::vector<ResumptionRequest>& requests) {
  using namespace utils;

  auto request_it = std::find_if(
      requests.begin(),
      requests.end(),
      [menu_id](const ResumptionRequest& request) {
        auto& msg_params = request.message[strings::msg_params];
        if (hmi_apis::FunctionID::UI_AddSubMenu ==
            request.request_id.function_id) {
          uint32_t failed_menu_id = msg_params[strings::menu_id].asUInt();
          return failed_menu_id == menu_id;
        }
        return false;
      });
  if (requests.end() != request_it) {
    return Optional<ResumptionRequest>(*request_it);
  }
  return Optional<ResumptionRequest>::OptionalEmpty::EMPTY;
}

void ResumptionDataProcessorImpl::DeleteSubmenus(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();

  auto failed_requests = GetAllFailedRequests(
      application->app_id(), resumption_status_, resumption_status_lock_);

  auto accessor = application->sub_menu_map();
  const auto sub_menu_map = accessor.GetData();

  for (const auto smenu : sub_menu_map) {
    auto failed_submenu_request =
        FindResumptionSubmenuRequest(smenu.first, failed_requests);
    if (!failed_submenu_request) {
      MessageHelper::SendDeleteSubmenuRequest(
          smenu.second, application, application_manager_);
    }
    application->RemoveSubMenu(smenu.first);
  }
}

void ResumptionDataProcessorImpl::AddCommands(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();
  if (!saved_app.keyExists(strings::application_commands)) {
    SDL_LOG_ERROR("application_commands section is not exists");
    return;
  }

  const smart_objects::SmartObject& app_commands =
      saved_app[strings::application_commands];

  for (size_t cmd_num = 0; cmd_num < app_commands.length(); ++cmd_num) {
    const smart_objects::SmartObject& command = app_commands[cmd_num];
    const uint32_t cmd_id = command[strings::cmd_id].asUInt();
    const uint32_t consecutive_num =
        commands::CommandImpl::CalcCommandInternalConsecutiveNumber(
            application);

    application->AddCommand(consecutive_num, command);
    application->help_prompt_manager().OnVrCommandAdded(
        cmd_id, command, true);  // is_resumption =true
  }

  ProcessMessagesToHMI(MessageHelper::CreateAddCommandRequestToHMI(
      application, application_manager_));
}

utils::Optional<ResumptionRequest> FindCommandResumptionRequest(
    uint32_t command_id, std::vector<ResumptionRequest>& requests) {
  using namespace utils;

  auto request_it = std::find_if(
      requests.begin(),
      requests.end(),
      [command_id](const ResumptionRequest& request) {
        auto& msg_params = request.message[strings::msg_params];
        const bool is_vr_command = hmi_apis::FunctionID::VR_AddCommand ==
                                       request.request_id.function_id &&
                                   hmi_apis::Common_VRCommandType::Command ==
                                       msg_params[strings::type].asInt();
        const bool is_ui_command = hmi_apis::FunctionID::UI_AddCommand ==
                                   request.request_id.function_id;

        if (is_vr_command || is_ui_command) {
          uint32_t cmd_id = msg_params[strings::cmd_id].asUInt();
          return cmd_id == command_id;
        }

        return false;
      });

  if (requests.end() != request_it) {
    return Optional<ResumptionRequest>(*request_it);
  }
  return Optional<ResumptionRequest>::OptionalEmpty::EMPTY;
}

void ResumptionDataProcessorImpl::DeleteCommands(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();

  auto failed_requests = GetAllFailedRequests(
      application->app_id(), resumption_status_, resumption_status_lock_);

  auto is_vr_command_failed = [](const ResumptionRequest& failed_command) {
    return failed_command.message[strings::msg_params].keyExists(
        strings::vr_commands);
  };

  auto extract_cmd_id =
      [](const smart_objects::SmartObject* const so_ptr) -> uint32_t {
    if (so_ptr->keyExists(strings::cmd_id)) {
      return so_ptr->getElement(strings::cmd_id).asUInt();
    }

    return 0;
  };

  auto accessor = application->commands_map();
  const auto commands_map = accessor.GetData();

  for (const auto cmd : commands_map) {
    const auto cmd_id = extract_cmd_id(cmd.second);
    if (0 == cmd_id) {
      SDL_LOG_ERROR("Can't extract cmd_id for command with internal number: "
                    << cmd.first);
      continue;
    }

    auto failed_command = FindCommandResumptionRequest(cmd_id, failed_requests);

    SDL_LOG_DEBUG(std::boolalpha << "Command with internal ID: " << cmd.first
                                 << " and cmdID: " << cmd_id << " was failed: "
                                 << static_cast<bool>(failed_command));
    if (!failed_command || (!is_vr_command_failed(*failed_command))) {
      auto delete_VR_command_msg = MessageHelper::CreateDeleteVRCommandRequest(
          cmd.second,
          application,
          application_manager_.GetNextHMICorrelationID());
      application_manager_.GetRPCService().ManageHMICommand(
          delete_VR_command_msg);
    }

    if (!failed_command || (is_vr_command_failed(*failed_command))) {
      auto delete_UI_command_msg = MessageHelper::CreateDeleteUICommandRequest(
          cmd.second,
          application->app_id(),
          application_manager_.GetNextHMICorrelationID());
      application_manager_.GetRPCService().ManageHMICommand(
          delete_UI_command_msg);
    }

    application->RemoveCommand(cmd_id);
    application->help_prompt_manager().OnVrCommandDeleted(cmd_id, true);
  }
}

void ResumptionDataProcessorImpl::AddChoicesets(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();
  if (!saved_app.keyExists(strings::application_choice_sets)) {
    SDL_LOG_ERROR("There is no any choicesets");
    return;
  }

  const smart_objects::SmartObject& app_choice_sets =
      saved_app[strings::application_choice_sets];

  for (size_t i = 0; i < app_choice_sets.length(); ++i) {
    const smart_objects::SmartObject& choice_set = app_choice_sets[i];
    const int32_t choice_set_id =
        choice_set[strings::interaction_choice_set_id].asInt();
    application->AddChoiceSet(choice_set_id, choice_set);
  }

  ProcessMessagesToHMI(MessageHelper::CreateAddVRCommandRequestFromChoiceToHMI(
      application, application_manager_));
}

utils::Optional<ResumptionRequest> FindResumptionChoiceSetRequest(
    uint32_t command_id, std::vector<ResumptionRequest>& requests) {
  using namespace utils;

  auto request_it =
      std::find_if(requests.begin(),
                   requests.end(),
                   [command_id](const ResumptionRequest& request) {
                     auto& msg_params = request.message[strings::msg_params];
                     if (msg_params.keyExists(strings::cmd_id) &&
                         (msg_params[strings::type] ==
                          hmi_apis::Common_VRCommandType::Choice)) {
                       uint32_t cmd_id = msg_params[strings::cmd_id].asUInt();
                       return cmd_id == command_id;
                     }
                     return false;
                   });
  if (requests.end() != request_it) {
    return Optional<ResumptionRequest>(*request_it);
  }
  return Optional<ResumptionRequest>::OptionalEmpty::EMPTY;
}

void ResumptionDataProcessorImpl::DeleteChoicesets(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();

  auto failed_requests = GetAllFailedRequests(
      application->app_id(), resumption_status_, resumption_status_lock_);

  auto accessor = application->choice_set_map();
  const auto choices = accessor.GetData();
  for (const auto choice : choices) {
    auto failed_choice_set =
        FindResumptionChoiceSetRequest(choice.first, failed_requests);
    if (!failed_choice_set) {
      MessageHelper::SendDeleteChoiceSetRequest(
          choice.second, application, application_manager_);
    }
    application->RemoveChoiceSet(choice.first);
  }
}

void ResumptionDataProcessorImpl::SetGlobalProperties(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();
  if (!saved_app.keyExists(strings::application_global_properties)) {
    SDL_LOG_DEBUG("application_global_properties section is not exists");
    return;
  }

  const smart_objects::SmartObject& properties_so =
      saved_app[strings::application_global_properties];
  application->load_global_properties(properties_so);

  if (saved_app.keyExists(strings::user_location)) {
    application->set_user_location(saved_app[strings::user_location]);
  }

  ProcessMessagesToHMI(MessageHelper::CreateGlobalPropertiesRequestsToHMI(
      application, application_manager_));
}

void ResumptionDataProcessorImpl::DeleteGlobalProperties(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  const uint32_t app_id = application->app_id();
  const auto result =
      application_manager_.ResetAllApplicationGlobalProperties(app_id);

  resumption_status_lock_.AcquireForReading();
  std::vector<ResumptionRequest> requests;
  if (resumption_status_.find(app_id) != resumption_status_.end()) {
    requests = resumption_status_[app_id].successful_requests;
  }
  resumption_status_lock_.Release();

  auto check_if_successful =
      [requests](hmi_apis::FunctionID::eType function_id) {
        for (auto& resumption_request : requests) {
          auto request_func =
              resumption_request.message[strings::params][strings::function_id]
                  .asInt();
          if (request_func == function_id) {
            return true;
          }
        }
        return false;
      };

  if (result.HasUIPropertiesReset() &&
      check_if_successful(hmi_apis::FunctionID::UI_SetGlobalProperties)) {
    smart_objects::SmartObjectSPtr msg_params =
        MessageHelper::CreateUIResetGlobalPropertiesRequest(result,
                                                            application);
    auto msg = MessageHelper::CreateMessageForHMI(
        hmi_apis::messageType::request,
        application_manager_.GetNextHMICorrelationID());
    (*msg)[strings::params][strings::function_id] =
        hmi_apis::FunctionID::UI_SetGlobalProperties;
    (*msg)[strings::msg_params] = *msg_params;
    ProcessMessageToHMI(msg, false);
  }

  if (result.HasTTSPropertiesReset() &&
      check_if_successful(hmi_apis::FunctionID::TTS_SetGlobalProperties)) {
    smart_objects::SmartObjectSPtr msg_params =
        MessageHelper::CreateTTSResetGlobalPropertiesRequest(result,
                                                             application);
    auto msg = MessageHelper::CreateMessageForHMI(
        hmi_apis::messageType::request,
        application_manager_.GetNextHMICorrelationID());
    (*msg)[strings::params][strings::function_id] =
        hmi_apis::FunctionID::TTS_SetGlobalProperties;

    (*msg)[strings::msg_params] = *msg_params;
    ProcessMessageToHMI(msg, false);
  }
}

void ResumptionDataProcessorImpl::AddSubscriptions(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  AddButtonsSubscriptions(application, saved_app);
  AddPluginsSubscriptions(application, saved_app);
}

void ResumptionDataProcessorImpl::AddButtonsSubscriptions(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  if (!saved_app.keyExists(strings::application_subscriptions)) {
    SDL_LOG_DEBUG("application_subscriptions section is not exists");
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

    ButtonSubscriptions button_subscriptions =
        GetButtonSubscriptionsToResume(application);

    ProcessMessagesToHMI(
        MessageHelper::CreateOnButtonSubscriptionNotificationsForApp(
            application, application_manager_, button_subscriptions));
  }
}

ButtonSubscriptions ResumptionDataProcessorImpl::GetButtonSubscriptionsToResume(
    ApplicationSharedPtr application) const {
  ButtonSubscriptions button_subscriptions =
      application->SubscribedButtons().GetData();
  auto it = button_subscriptions.find(mobile_apis::ButtonName::CUSTOM_BUTTON);

  if (it != button_subscriptions.end()) {
    button_subscriptions.erase(it);
  }

  return button_subscriptions;
}

void ResumptionDataProcessorImpl::AddPluginsSubscriptions(
    ApplicationSharedPtr application,
    const smart_objects::SmartObject& saved_app) {
  SDL_LOG_AUTO_TRACE();

  for (auto& extension : application->Extensions()) {
    extension->ProcessResumption(saved_app);
  }
}

void ResumptionDataProcessorImpl::DeleteSubscriptions(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  DeleteButtonsSubscriptions(application);
  DeletePluginsSubscriptions(application);
}

void ResumptionDataProcessorImpl::DeleteButtonsSubscriptions(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();
  const ButtonSubscriptions button_subscriptions =
      application->SubscribedButtons().GetData();
  for (auto btn : button_subscriptions) {
    const auto hmi_btn = static_cast<hmi_apis::Common_ButtonName::eType>(btn);
    if (hmi_apis::Common_ButtonName::CUSTOM_BUTTON == hmi_btn) {
      continue;
    }
    auto notification = MessageHelper::CreateOnButtonSubscriptionNotification(
        application->hmi_app_id(), hmi_btn, false);
    // is_subscribed = false
    ProcessMessageToHMI(notification, false);
    application->UnsubscribeFromButton(btn);
  }
}

void ResumptionDataProcessorImpl::DeleteWindowsSubscriptions(
    ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();

  const auto window_ids = application->GetWindowIds();
  for (const auto& window_id : window_ids) {
    const auto hmi_state = application->CurrentHmiState(window_id);
    if (mobile_apis::WindowType::WIDGET != hmi_state->window_type()) {
      continue;
    }

    SDL_LOG_DEBUG("Reverting CreateWindow for: " << window_id);

    auto delete_request = MessageHelper::CreateUIDeleteWindowRequestToHMI(
        application, application_manager_, window_id);
    const bool subscribe_on_request_events = false;
    ProcessMessageToHMI(delete_request, subscribe_on_request_events);

    application->RemoveWindowInfo(window_id);
    application->RemoveHMIState(window_id,
                                app_mngr::HmiState::StateID::STATE_ID_REGULAR);
    application->remove_window_capability(window_id);
  }
}

void ResumptionDataProcessorImpl::DeletePluginsSubscriptions(
    application_manager::ApplicationSharedPtr application) {
  SDL_LOG_AUTO_TRACE();

  resumption_status_lock_.AcquireForReading();
  auto it = resumption_status_.find(application->app_id());
  if (it == resumption_status_.end()) {
    resumption_status_lock_.Release();
    return;
  }

  const ApplicationResumptionStatus& status = it->second;
  smart_objects::SmartObject extension_vd_subscriptions;
  for (auto ivi : status.successful_vehicle_data_subscriptions_) {
    SDL_LOG_DEBUG("ivi " << ivi << " should be deleted");
    extension_vd_subscriptions[ivi] = true;
  }

  smart_objects::SmartObject extension_modules_subscriptions(
      smart_objects::SmartType_Map);

  if (!status.successful_module_subscriptions_.empty()) {
    extension_modules_subscriptions[message_params::kModuleData] =
        smart_objects::SmartObject(smart_objects::SmartType_Array);

    auto& module_data_so =
        extension_modules_subscriptions[message_params::kModuleData];

    uint32_t index = 0;
    for (const auto& module : status.successful_module_subscriptions_) {
      module_data_so[index] =
          smart_objects::SmartObject(smart_objects::SmartType_Map);
      module_data_so[index][message_params::kModuleType] = module.first;
      module_data_so[index][message_params::kModuleId] = module.second;
      index++;
    }
  }

  smart_objects::SmartObject resumption_data_to_revert(
      smart_objects::SmartType_Map);
  resumption_data_to_revert[application_manager::hmi_interface::vehicle_info] =
      extension_vd_subscriptions;
  resumption_data_to_revert[application_manager::hmi_interface::rc] =
      extension_modules_subscriptions;

  resumption_status_lock_.Release();

  auto extensions = application->Extensions();
  for (auto& extension : extensions) {
    extension->RevertResumption(resumption_data_to_revert);
  }
}

bool IsResponseSuccessful(const smart_objects::SmartObject& response) {
  return !response[strings::params].keyExists(strings::error_msg);
}

void ResumptionDataProcessorImpl::CheckVehicleDataResponse(
    const smart_objects::SmartObject& request,
    const smart_objects::SmartObject& response,
    ApplicationResumptionStatus& status) {
  SDL_LOG_AUTO_TRACE();
  const auto request_keys = request[strings::msg_params].enumerate();

  if (!IsResponseSuccessful(response)) {
    SDL_LOG_TRACE("Vehicle data request not successful");
    for (const auto key : request_keys) {
      status.unsuccessful_vehicle_data_subscriptions_.push_back(key);
    }
    return;
  }

  const auto& response_params = response[strings::msg_params];
  const auto response_keys = response_params.enumerate();
  for (auto& ivi : request_keys) {
    const auto it = response_keys.find(ivi);
    const auto kSuccess = hmi_apis::Common_VehicleDataResultCode::VDRC_SUCCESS;
    const auto vd_result_code =
        (response_keys.end() == it)
            ? kSuccess
            : response_params[ivi][strings::result_code].asInt();
    if (kSuccess != vd_result_code) {
      SDL_LOG_TRACE("ivi " << ivi << " was NOT successfuly subscribed");

      status.unsuccessful_vehicle_data_subscriptions_.push_back(ivi);
    } else {
      SDL_LOG_TRACE("ivi " << ivi << " was successfuly subscribed");
      status.successful_vehicle_data_subscriptions_.push_back(ivi);
    }
  }
}

void ResumptionDataProcessorImpl::CheckModuleDataSubscription(
    const ns_smart_device_link::ns_smart_objects::SmartObject& request,
    const ns_smart_device_link::ns_smart_objects::SmartObject& response,
    ApplicationResumptionStatus& status) {
  SDL_LOG_AUTO_TRACE();

  const auto& msg_params_so = request[strings::msg_params];
  const auto requested_module_type =
      msg_params_so[message_params::kModuleType].asString();
  const auto requested_module_id =
      msg_params_so[message_params::kModuleId].asString();
  const ModuleUid requested_module{requested_module_type, requested_module_id};

  if (!IsResponseSuccessful(response)) {
    SDL_LOG_TRACE("Module data subscription request NOT successful");
    status.unsuccessful_module_subscriptions_.push_back(requested_module);
    return;
  }

  const auto& response_module_data_so =
      response[strings::msg_params][message_params::kModuleData];

  if (0 == response_module_data_so.length()) {
    SDL_LOG_TRACE("Module data subscription request not successful");
    status.unsuccessful_module_subscriptions_.push_back(requested_module);
    return;
  }

  const auto responsed_module_type_int =
      static_cast<hmi_apis::Common_ModuleType::eType>(
          response_module_data_so[message_params::kModuleType].asUInt());

  const auto responsed_module_type_str =
      application_manager::EnumToString(responsed_module_type_int);

  const auto response_module_id =
      response_module_data_so[message_params::kModuleId].asString();
  const ModuleUid responsed_module{responsed_module_type_str,
                                   response_module_id};

  bool is_subscribe_success = false;
  if (response[application_manager::strings::msg_params].keyExists(
          rc_rpc_plugin::message_params::kIsSubscribed)) {
    is_subscribe_success =
        response[application_manager::strings::msg_params]
                [rc_rpc_plugin::message_params::kIsSubscribed]
                    .asBool();
  }

  const bool is_the_same_module = requested_module == responsed_module;

  if (is_the_same_module && is_subscribe_success) {
    SDL_LOG_TRACE("Module [" << requested_module.first << ":"
                             << requested_module.second
                             << "] was successfuly subscribed");
    status.successful_module_subscriptions_.push_back(requested_module);
  } else {
    SDL_LOG_TRACE("Module [" << requested_module.first << ":"
                             << requested_module.second
                             << "] was NOT successfuly subscribed");
    status.unsuccessful_module_subscriptions_.push_back(requested_module);
  }
}

void ResumptionDataProcessorImpl::CheckCreateWindowResponse(
    const smart_objects::SmartObject& request,
    const smart_objects::SmartObject& response) const {
  SDL_LOG_AUTO_TRACE();

  const auto& msg_params = request[strings::msg_params];
  const auto app_id = msg_params[strings::app_id].asInt();

  auto application = application_manager_.application(app_id);
  if (!application) {
    SDL_LOG_ERROR("Application is not registered by hmi id: " << app_id);
    return;
  }

  const auto window_id = msg_params[strings::window_id].asInt();
  if (!IsResponseSuccessful(response)) {
    SDL_LOG_ERROR("UI_CreateWindow for correlation id: "
                  << response[strings::params][strings::correlation_id].asInt()
                  << " has failed");
    auto& builder = application->display_capabilities_builder();
    builder.ResetDisplayCapabilities();
    return;
  }

  smart_objects::SmartObject window_info(smart_objects::SmartType_Map);
  auto fill_optional_param = [&window_info,
                              &msg_params](const std::string& key) {
    if (msg_params.keyExists(key)) {
      window_info[key] = msg_params[key].asString();
    }
  };
  fill_optional_param(strings::associated_service_type);
  fill_optional_param(strings::duplicate_updates_from_window_id);

  const auto window_name = msg_params[strings::window_name].asString();
  window_info[strings::window_name] = window_name;
  application->SetWindowInfo(window_id, window_info);

  const auto window_type = static_cast<mobile_apis::WindowType::eType>(
      msg_params[strings::window_type].asInt());

  // State should be initialized with INVALID_ENUM value to let state
  // controller trigger OnHmiStatus notification sending
  auto initial_state = application_manager_.CreateRegularState(
      application,
      window_type,
      mobile_apis::HMILevel::INVALID_ENUM,
      mobile_apis::AudioStreamingState::INVALID_ENUM,
      mobile_apis::VideoStreamingState::INVALID_ENUM,
      mobile_apis::SystemContext::INVALID_ENUM);
  application->SetInitialState(window_id, window_name, initial_state);

  // Default HMI level for all windows except the main one is always NONE
  application_manager_.state_controller().OnAppWindowAdded(
      application, window_id, window_type, mobile_apis::HMILevel::HMI_NONE);
}

}  // namespace resumption

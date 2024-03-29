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

#include "sdl_rpc_plugin/commands/mobile/delete_file_request.h"

#include "application_manager/application_impl.h"

#include "utils/file_system.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

DeleteFileRequest::DeleteFileRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromMobileImpl(message,
                            application_manager,
                            rpc_service,
                            hmi_capabilities,
                            policy_handler) {}

DeleteFileRequest::~DeleteFileRequest() {}

void DeleteFileRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!application) {
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    SDL_LOG_ERROR("Application is not registered");
    return;
  }

  if ((mobile_api::HMILevel::HMI_NONE ==
       application->hmi_level(
           mobile_apis::PredefinedWindows::DEFAULT_WINDOW)) &&
      (application_manager_.get_settings().delete_file_in_none() <=
       application->delete_file_in_none_count())) {
    // If application is in the HMI_NONE level the quantity of allowed
    // DeleteFile request is limited by the configuration profile
    SDL_LOG_ERROR("Too many requests from the app with HMILevel HMI_NONE");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  const std::string& sync_file_name =
      (*message_)[strings::msg_params][strings::sync_file_name].asString();

  if (!file_system::IsFileNameValid(sync_file_name)) {
    const std::string err_msg = "Sync file name contains forbidden symbols.";
    SDL_LOG_ERROR(err_msg);
    SendResponse(false, mobile_apis::Result::INVALID_DATA, err_msg.c_str());
    return;
  }

  std::string full_file_path =
      application_manager_.get_settings().app_storage_folder() + "/";
  full_file_path += application->folder_name();
  full_file_path += "/";
  full_file_path += sync_file_name;

  if (file_system::FileExists(full_file_path)) {
    if (file_system::DeleteFile(full_file_path)) {
      const application_manager::AppFile* file =
          application->GetFile(full_file_path);
      if (file) {
        SendFileRemovedNotification(file);
      }

      application->DeleteFile(full_file_path);
      application->increment_delete_file_in_none_count();
      SendResponse(true, mobile_apis::Result::SUCCESS);
    } else {
      SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
    }
  } else {
    SendResponse(false, mobile_apis::Result::REJECTED);
  }
}

void DeleteFileRequest::SendFileRemovedNotification(
    const application_manager::AppFile* file) const {
  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::app_id] = connection_key();
  msg_params[strings::file_name] = file->file_name;
  msg_params[strings::file_type] = file->file_type;

  CreateHMINotification(hmi_apis::FunctionID::BasicCommunication_OnFileRemoved,
                        msg_params);
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

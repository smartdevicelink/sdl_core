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

#include "sdl_rpc_plugin/commands/mobile/list_files_request.h"
#include <string>

#include "application_manager/application_impl.h"

#include "sdl_rpc_plugin/mobile_command_factory.h"
#include "utils/file_system.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

ListFilesRequest::ListFilesRequest(
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

ListFilesRequest::~ListFilesRequest() {}

void ListFilesRequest::Run() {
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
      (application_manager_.get_settings().list_files_in_none() <=
       application->list_files_in_none_count())) {
    // If application is in the HMI_NONE level the quantity of allowed
    // DeleteFile request is limited by the configuration profile
    SDL_LOG_ERROR("Too many requests from the app with HMILevel HMI_NONE ");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  application->increment_list_files_in_none_count();

  (*message_)[strings::msg_params][strings::space_available] =
      static_cast<int32_t>(application->GetAvailableDiskSpace());
  // Enumarating through File system
  uint32_t i = 0;
  std::string directory_name =
      application_manager_.get_settings().app_storage_folder();
  directory_name += "/" + application->folder_name();
  std::vector<std::string> persistent_files =
      file_system::ListFiles(directory_name);
  std::vector<std::string>::const_iterator it = persistent_files.begin();
  for (; it != persistent_files.end(); ++it) {
    if (i < application_manager_.get_settings().list_files_response_size()) {
      SDL_LOG_DEBUG("File " + *it + " added to ListFiles response");
      (*message_)[strings::msg_params][strings::filenames][i++] = *it;
    } else {
      SDL_LOG_DEBUG("File " + *it + " not added to ListFiles response");
    }
  }
  (*message_)[strings::params][strings::message_type] =
      application_manager::MessageType::kResponse;
  SendResponse(true,
               mobile_apis::Result::SUCCESS,
               NULL,
               &(*message_)[strings::msg_params]);
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

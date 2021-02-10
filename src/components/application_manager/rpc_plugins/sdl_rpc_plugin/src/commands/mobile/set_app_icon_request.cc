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

#include "sdl_rpc_plugin/commands/mobile/set_app_icon_request.h"
#include <algorithm>

#include "application_manager/application_impl.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "utils/helpers.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

SetAppIconRequest::SetAppIconRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : CommandRequestImpl(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler)
    , is_icons_saving_enabled_(false) {
  const std::string path =
      application_manager_.get_settings().app_icons_folder();

  if (!file_system::DirectoryExists(path)) {
    SDL_LOG_WARN("App icons folder doesn't exist.");
    if (!file_system::CreateDirectoryRecursively(path)) {
      SDL_LOG_ERROR("Unable to create app icons directory: " << path);
    }
  }

  is_icons_saving_enabled_ = file_system::IsWritingAllowed(path) &&
                             file_system::IsReadingAllowed(path);
}

SetAppIconRequest::~SetAppIconRequest() {}

void SetAppIconRequest::Run() {
  SDL_LOG_AUTO_TRACE();

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app) {
    SDL_LOG_ERROR("Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
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
  full_file_path += app->folder_name();
  full_file_path += "/";
  full_file_path += sync_file_name;

  if (!file_system::FileExists(full_file_path)) {
    SDL_LOG_ERROR("No such file " << full_file_path);
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  msg_params[strings::app_id] = app->app_id();
  msg_params[strings::sync_file_name] =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  // for further use in on_event function
  full_file_path_for_hmi_ = full_file_path;

  msg_params[strings::sync_file_name][strings::value] = full_file_path_for_hmi_;

  // TODO(VS): research why is image_type hardcoded
  msg_params[strings::sync_file_name][strings::image_type] =
      static_cast<int32_t>(SetAppIconRequest::ImageType::DYNAMIC);

  StartAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
  SendHMIRequest(hmi_apis::FunctionID::UI_SetAppIcon, &msg_params, true);
}

void SetAppIconRequest::CopyToIconStorage(
    const std::string& policy_app_id, const std::string& path_to_file) const {
  SDL_LOG_AUTO_TRACE();
  if (!(application_manager_.protocol_handler()
            .get_settings()
            .max_supported_protocol_version() >=
        protocol_handler::MajorProtocolVersion::PROTOCOL_VERSION_4)) {
    SDL_LOG_WARN("Icon copying skipped, since protocol ver. 4 is not enabled.");
    return;
  }

  std::vector<uint8_t> file_content;
  if (!file_system::ReadBinaryFile(path_to_file, file_content)) {
    SDL_LOG_ERROR("Can't read icon file: " << path_to_file);
    return;
  }

  const std::string icon_storage =
      application_manager_.get_settings().app_icons_folder();
  const uint64_t storage_max_size = static_cast<uint64_t>(
      application_manager_.get_settings().app_icons_folder_max_size());
  const uint64_t file_size = file_system::FileSize(path_to_file);

  if (0 == file_size) {
    SDL_LOG_ERROR("Can't get the icon file size: " << path_to_file);
    return;
  }

  if (storage_max_size < file_size) {
    SDL_LOG_ERROR("Icon size (" << file_size
                                << ") is bigger, than "
                                   " icons storage maximum size ("
                                << storage_max_size
                                << ")."
                                   "Copying skipped.");
    return;
  }

  const uint64_t storage_size =
      static_cast<uint64_t>(file_system::DirectorySize(icon_storage));

  if (storage_max_size < (file_size + storage_size)) {
    const uint32_t icons_amount =
        application_manager_.get_settings().app_icons_amount_to_remove();

    if (!icons_amount) {
      SDL_LOG_DEBUG(
          "No icons will be deleted, since amount icons to remove "
          "is zero. Icon saving skipped.");
      return;
    }

    while (!IsEnoughSpaceForIcon(file_size)) {
      RemoveOldestIcons(icon_storage, icons_amount);
    }
  }

  const std::string icon_path = icon_storage + "/" + policy_app_id;

  if (!file_system::CreateFile(icon_path)) {
    SDL_LOG_ERROR("Can't create icon: " << icon_path);
    return;
  }

  if (!file_system::WriteBinaryFile(icon_path, file_content)) {
    SDL_LOG_ERROR("Can't write icon: " << icon_path);
    return;
  }

  SDL_LOG_DEBUG("Icon was successfully copied from :" << path_to_file << " to "
                                                      << icon_path);

  return;
}

void SetAppIconRequest::RemoveOldestIcons(const std::string& storage,
                                          const uint32_t icons_amount) const {
  const std::vector<std::string> icons_list = file_system::ListFiles(storage);
  auto compareTime = [](const time_t t1, const time_t t2) -> bool {
    return difftime(t1, t2) > 0;
  };
  std::
      map<time_t, std::string, std::function<bool(const time_t, const time_t)> >
          icon_modification_time(compareTime);
  std::vector<std::string>::const_iterator it = icons_list.begin();
  for (; it != icons_list.end(); ++it) {
    const std::string file_name = *it;
    const std::string file_path = storage + "/" + file_name;
    if (!file_system::FileExists(file_path)) {
      continue;
    }
    const time_t time = file_system::GetFileModificationTime(file_path);
    icon_modification_time[time] = file_name;
  }

  for (size_t counter = 0; counter < icons_amount; ++counter) {
    if (!icon_modification_time.size()) {
      SDL_LOG_ERROR("No more icons left for deletion.");
      return;
    }
    const std::string file_name = icon_modification_time.begin()->second;
    const std::string file_path = storage + "/" + file_name;
    if (!file_system::DeleteFile(file_path)) {
      SDL_LOG_DEBUG("Error while deleting icon " << file_path);
    }
    icon_modification_time.erase(icon_modification_time.begin());
    SDL_LOG_DEBUG("Old icon " << file_path << " was deleted successfully.");
  }
}

bool SetAppIconRequest::IsEnoughSpaceForIcon(const uint64_t icon_size) const {
  const std::string icon_storage =
      application_manager_.get_settings().app_icons_folder();
  const uint64_t storage_max_size = static_cast<uint64_t>(
      application_manager_.get_settings().app_icons_folder_max_size());
  const uint64_t storage_size =
      static_cast<uint64_t>(file_system::DirectorySize(icon_storage));
  return storage_max_size >= (icon_size + storage_size);
}

void SetAppIconRequest::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::UI_SetAppIcon: {
      EndAwaitForInterface(HmiInterfaces::HMI_INTERFACE_UI);
      hmi_apis::Common_Result::eType result_code =
          static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asInt());
      const bool result = PrepareResultForMobileResponse(
          result_code, HmiInterfaces::HMI_INTERFACE_UI);
      std::string response_info;
      GetInfo(message, response_info);
      if (result) {
        ApplicationSharedPtr app =
            application_manager_.application(connection_key());

        if (!app) {
          SDL_LOG_ERROR(
              "Can't get application for connection key: " << connection_key());
          return;
        }

        if (is_icons_saving_enabled_ && !full_file_path_for_hmi_.empty()) {
          const auto policy_app_id = app->policy_app_id();
          CopyToIconStorage(policy_app_id, full_file_path_for_hmi_);
        }

        app->set_app_icon_path(full_file_path_for_hmi_);

        SDL_LOG_INFO("Icon path was set to '" << app->app_icon_path() << "'");
      }

      SendResponse(result,
                   MessageHelper::HMIToMobileResult(result_code),
                   response_info.empty() ? NULL : response_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    default: {
      SDL_LOG_ERROR("Received unknown event " << event.id());
      return;
    }
  }
}

}  // namespace commands

}  // namespace sdl_rpc_plugin

/*

 Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/mobile/put_file_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/application_impl.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"

namespace application_manager {

namespace commands {

PutFileRequest::PutFileRequest(const MessageSharedPtr& message)
  : CommandRequestImpl(message)
  , offset_(0)
  , sync_file_name_()
  , length_(0)
  , file_type_(mobile_apis::FileType::INVALID_ENUM)
  , is_persistent_file_(false) {
}

PutFileRequest::~PutFileRequest() {
}

void PutFileRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr application =
      ApplicationManagerImpl::instance()->application(connection_key());
  smart_objects::SmartObject response_params = smart_objects::SmartObject(
        smart_objects::SmartType_Map);

  if (!application) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (mobile_api::HMILevel::HMI_NONE == application->hmi_level() &&
      profile::Profile::instance()->put_file_in_none() <=
      application->put_file_in_none_count()) {
    // If application is in the HMI_NONE level the quantity of allowed
    // PutFile request is limited by the configuration profile
    LOG4CXX_ERROR(logger_,
                  "Too many requests from the app with HMILevel HMI_NONE ");
    SendResponse(false, mobile_apis::Result::REJECTED,
                 "Too many requests from the app with HMILevel HMI_NONE",
                 &response_params);
    return;
  }

  if (!(*message_)[strings::params].keyExists(strings::binary_data)) {
    LOG4CXX_ERROR(logger_, "Binary data empty");
    SendResponse(false, mobile_apis::Result::INVALID_DATA,
                 "Binary data empty",
                 &response_params);
    return;
  }

  if (!(*message_)[strings::msg_params].keyExists(strings::sync_file_name)) {
    LOG4CXX_ERROR(logger_, "No file name");
    SendResponse(false, mobile_apis::Result::INVALID_DATA,
                 "No file name",
                 &response_params);
    return;
  }

  if (!(*message_)[strings::msg_params].keyExists(strings::file_type)) {
    LOG4CXX_ERROR(logger_, "No file type");
    SendResponse(false, mobile_apis::Result::INVALID_DATA,
                 "No file type",
                 &response_params);
    return;
  }
  sync_file_name_ =
    (*message_)[strings::msg_params][strings::sync_file_name].asString();
  file_type_ =
    static_cast<mobile_apis::FileType::eType>(
      (*message_)[strings::msg_params][strings::file_type].asInt());
  const std::vector<uint8_t> binary_data =
    (*message_)[strings::params][strings::binary_data].asBinary();

  // Policy table update in json format is currently to be received via PutFile
  // TODO(PV): after latest discussion has to be changed
  if (mobile_apis::FileType::JSON == file_type_) {
    application_manager::ApplicationManagerImpl::instance()
        ->GetPolicyHandler()
        .ReceiveMessageFromSDK(sync_file_name_, binary_data);
  }

  offset_ = 0;
  is_persistent_file_ = false;
  bool is_system_file = false;
  length_ = binary_data.size();
  bool is_download_compleate = true;
  bool offset_exist =
      (*message_)[strings::msg_params].keyExists(strings::offset);

  if (offset_exist) {
    offset_ = (*message_)[strings::msg_params][strings::offset].asInt();
  }

  if ((*message_)[strings::msg_params].
      keyExists(strings::persistent_file)) {
    is_persistent_file_ =
      (*message_)[strings::msg_params][strings::persistent_file].asBool();
  }
  if ((*message_)[strings::msg_params].
      keyExists(strings::system_file)) {
    is_system_file =
      (*message_)[strings::msg_params][strings::system_file].asBool();
  }

  std::string file_path;

  if (is_system_file) {
    response_params[strings::space_available] = 0;
    file_path = profile::Profile::instance()->system_files_path();
  } else {
    file_path = profile::Profile::instance()->app_storage_folder();
    file_path += "/" + application->folder_name();

    uint32_t space_available = ApplicationManagerImpl::instance()->
        GetAvailableSpaceForApp(application->folder_name());

    if (binary_data.size() > space_available) {

      response_params[strings::space_available] =
          static_cast<uint32_t>(space_available);

      LOG4CXX_ERROR(logger_, "Out of memory");
      SendResponse(false, mobile_apis::Result::OUT_OF_MEMORY,
                   "Out of memory", &response_params);
      return;
    }
  }

  if (!file_system::CreateDirectoryRecursively(file_path)) {
    LOG4CXX_ERROR(logger_, "Cann't create folder");
    SendResponse(false, mobile_apis::Result::GENERIC_ERROR,
                 "Cann't create folder.", &response_params);
    return;
  }

  mobile_apis::Result::eType save_result =
      ApplicationManagerImpl::instance()->SaveBinary(binary_data, file_path,
                                                     sync_file_name_, offset_);

  if (!is_system_file) {
    response_params[strings::space_available] = static_cast<uint32_t>(
        ApplicationManagerImpl::instance()->GetAvailableSpaceForApp(
          application->folder_name()));
  }

  sync_file_name_ = file_path + "/" + sync_file_name_;
  switch (save_result) {
    case mobile_apis::Result::SUCCESS: {
      LOG4CXX_INFO(logger_, "PutFile is successful");
      if (!is_system_file) {
        AppFile file(sync_file_name_, is_persistent_file_,
                     is_download_compleate, file_type_);

        if (0 == offset_) {
          LOG4CXX_INFO(logger_, "New file downloading");
          if (!application->AddFile(file)) {

            LOG4CXX_INFO(logger_,
                         "Couldn't add file to application (File already Exist"
                         << " in application and was rewritten on FS)");
            /* It can be first part of new big file, so we need to update
               information about it's downloading status and persistence */
            if (!application->UpdateFile(file)) {
              LOG4CXX_ERROR(logger_, "Couldn't update file");
              /* If it is impossible to update file, application doesn't
              know about existing this file */
              SendResponse(false, mobile_apis::Result::INVALID_DATA,
                           "Couldn't update file",
                           &response_params);
              return;
            }
          } else {
            /* if file added - increment it's count
             ( may be application->AddFile have to incapsulate it? )
              Any way now this method evals not only in "none"*/
            application->increment_put_file_in_none_count();
          }
        }
      }

      SendResponse(true, save_result, "File was downloaded", &response_params);
      if (is_system_file) {
        SendOnPutFileNotification();
      }
      break;
    }
    default:
      LOG4CXX_WARN(logger_, "PutFile is unsuccessful. Result = " << save_result);
      SendResponse(false, save_result, "Can't save file", &response_params);
      break;
  }
}

void PutFileRequest::SendOnPutFileNotification() {
  LOG4CXX_INFO(logger_, "SendOnPutFileNotification" );
  smart_objects::SmartObjectSPtr notification = new smart_objects::SmartObject(
    smart_objects::SmartType_Map);

  smart_objects::SmartObject& message = *notification;
  message[strings::params][strings::function_id] =
    hmi_apis::FunctionID::BasicCommunication_OnPutFile;

  message[strings::params][strings::message_type] = MessageType::kNotification;
  message[strings::msg_params][strings::app_id] = connection_key();
  message[strings::msg_params][strings::sync_file_name] = sync_file_name_;
  message[strings::msg_params][strings::offset] = offset_;
  if (0 == offset_) {
    message[strings::msg_params][strings::file_size] =
        (*message_)[strings::msg_params][strings::length];
  }
  message[strings::msg_params][strings::length] = length_;
  message[strings::msg_params][strings::persistent_file] = is_persistent_file_;
  message[strings::msg_params][strings::file_type] = file_type_;
  ApplicationManagerImpl::instance()->ManageHMICommand(notification);
}

}  // namespace commands

}  // namespace application_manager

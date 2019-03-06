/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#include "sdl_rpc_plugin/commands/mobile/get_file_request.h"
#include "application_manager/application_impl.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/message_helper.h"
#include "application_manager/app_service_manager.h"
#include "utils/file_system.h"
#include <boost/crc.hpp>

namespace {
/**
* Calculates CRC32 checksum
* @param binary_data - input data for which CRC32 should be calculated
* @return calculated CRC32 checksum
*/
uint32_t GetCrc32CheckSum(const std::vector<uint8_t>& binary_data) {
  const std::size_t file_size = binary_data.size();
  boost::crc_32_type result;
  result.process_bytes(&binary_data[0], file_size);
  return result.checksum();
}

}  // namespace

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {

GetFileRequest::GetFileRequest(
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
    , file_name_("")
    , file_type_(mobile_apis::FileType::INVALID_ENUM)
    , length_(0)
    , offset_(0) {}

GetFileRequest::~GetFileRequest() {}

bool GetFileRequest::GetFilePath(std::string& file_path, bool& forward_to_hmi) {
  std::string path = application_manager_.get_settings().app_storage_folder();
  auto connect_key = connection_key();
  forward_to_hmi = false;

  if ((*message_)[strings::msg_params].keyExists(strings::app_service_id)) {
    std::string service_id =
        (*message_)[strings::msg_params][strings::app_service_id].asString();
    LOG4CXX_DEBUG(logger_,
                  "Finding storage directory for service id: " << service_id);

    AppService app_service_info;
    if (application_manager_.GetAppServiceManager().GetAppServiceInfo(
            service_id, app_service_info)) {
      if (app_service_info.mobile_service) {
        connect_key = app_service_info.connection_key;
      } else {
        forward_to_hmi = true;
        return true;
      }
    } else {
      return false;
    }
  } else {
    LOG4CXX_DEBUG(logger_, "Using current storage directory");
  }

  ApplicationSharedPtr app = application_manager_.application(connect_key);
  file_path = path + "/" + app->folder_name();
  return true;
}

void GetFileRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_INFO(logger_, "Received GetFile request");

  ApplicationSharedPtr app = application_manager_.application(connection_key());
  smart_objects::SmartObject response_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  if (!app) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  file_name_ = (*message_)[strings::msg_params][strings::file_name].asString();

  if (!file_system::IsFileNameValid(file_name_)) {
    LOG4CXX_ERROR(logger_,
                  "File name " << file_name_ << " contains forbidden symbols.");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "File name contains forbidden symbols",
                 &response_params);
    return;
  }

  // Initialize other params with default values. If exists overwrite the values
  LOG4CXX_DEBUG(logger_, "Intialize non manadatory params with default values");

  if ((*message_)[strings::msg_params].keyExists(strings::file_type)) {
    file_type_ = static_cast<mobile_apis::FileType::eType>(
        (*message_)[strings::msg_params][strings::file_type].asInt());
  }

  // Check if file exists on system (may have to use app service id to get the
  // correct app folder)
  LOG4CXX_DEBUG(logger_, "Check if file exists on system");
  std::string file_path;
  bool forward_to_hmi;

  if (GetFilePath(file_path, forward_to_hmi)) {
    if (forward_to_hmi) {
      LOG4CXX_DEBUG(logger_, "Forwarding GetFile request to HMI");
      SendHMIRequest(hmi_apis::FunctionID::BasicCommunication_GetFilePath,
                     &(*message_)[strings::msg_params],
                     true);
      return;
    }
  } else {
    LOG4CXX_ERROR(logger_, "Could not get file path");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "Could not get file path",
                 &response_params);
    return;
  }

  const std::string full_path = file_path + "/" + file_name_;
  if (!file_system::FileExists(full_path)) {
    LOG4CXX_ERROR(logger_, "File " << full_path << " does not exist");
    SendResponse(false,
                 mobile_apis::Result::FILE_NOT_FOUND,
                 "File does not exist",
                 &response_params);
    return;
  }

  // Handle offset
  LOG4CXX_DEBUG(logger_, "Handle offset and length parameters");
  const uint64_t file_size = file_system::FileSize(full_path);

  if ((*message_)[strings::msg_params].keyExists(strings::length)) {
    length_ = (*message_)[strings::msg_params][strings::length].asInt();
  } else {
    length_ = file_size;
  }
  if ((*message_)[strings::msg_params].keyExists(strings::offset)) {
    offset_ = (*message_)[strings::msg_params][strings::offset].asInt();
  }

  if (offset_ > file_size) {
    LOG4CXX_ERROR(logger_,
                  "Offset " << offset_ << " greater than file size "
                            << file_size);
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "Offset greater than file size",
                 &response_params);
    return;
  }
  if (length_ > file_size - offset_) {
    LOG4CXX_ERROR(logger_,
                  "Length " << length_ << " greater than file size - offset"
                            << file_size);
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "Length greater than file size - offset",
                 &response_params);
    return;
  }

  // Load data from file as binary data
  LOG4CXX_DEBUG(logger_, "Load binary data from file");
  std::vector<uint8_t> bin_data;
  if (!file_system::ReadBinaryFile(full_path, bin_data, offset_, length_)) {
    LOG4CXX_ERROR(logger_, "Failed to read from file: " << full_path);
    SendResponse(false,
                 mobile_apis::Result::GENERIC_ERROR,
                 "Unable to read from file",
                 &response_params);
    return;
  }

  // Construct response message
  if ((*message_)[strings::msg_params].keyExists(strings::offset)) {
    response_params[strings::offset] = offset_;
  }
  if ((*message_)[strings::msg_params].keyExists(strings::length)) {
    response_params[strings::length] = length_;
  }
  if ((*message_)[strings::msg_params].keyExists(strings::file_type)) {
    response_params[strings::file_type] = file_type_;
  }
  const uint32_t crc_calculated = GetCrc32CheckSum(bin_data);
  response_params[strings::crc32_check_sum] = crc_calculated;

  SendResponse(true,
               mobile_apis::Result::SUCCESS,
               "File uploaded",
               &response_params,
               bin_data);
}

void GetFileRequest::on_event(const app_mngr::event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (hmi_apis::FunctionID::BasicCommunication_GetFilePath != event.id()) {
    return;
  }
  const smart_objects::SmartObject& event_message = event.smart_object();

  hmi_apis::Common_Result::eType hmi_result =
      static_cast<hmi_apis::Common_Result::eType>(
          event_message[strings::params][hmi_response::code].asInt());

  mobile_apis::Result::eType result =
      MessageHelper::HMIToMobileResult(hmi_result);
  bool success = PrepareResultForMobileResponse(
      hmi_result, HmiInterfaces::HMI_INTERFACE_AppService);

  if (result != mobile_apis::Result::SUCCESS) {
    auto msg_params = event_message[strings::msg_params];
    const char* info = msg_params.keyExists(strings::info)
                           ? msg_params[strings::info].asCharArray()
                           : NULL;

    SendResponse(success, result, info, &msg_params);
    return;
  }
  smart_objects::SmartObject response_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  std::vector<uint8_t> bin_data;

  if (event_message[strings::msg_params].keyExists(strings::file_type)) {
    response_params[strings::file_type] =
        event_message[strings::msg_params][strings::file_type];
  }

  if (event_message[strings::msg_params].keyExists(strings::file_path)) {
    std::string full_path =
        event_message[strings::msg_params][strings::file_path].asString();

    if (!file_system::FileExists(full_path)) {
      LOG4CXX_ERROR(logger_, "File " << full_path << " does not exist");
      SendResponse(false,
                   mobile_apis::Result::FILE_NOT_FOUND,
                   "File does not exist",
                   &response_params);
      return;
    }

    const uint64_t file_size = file_system::FileSize(full_path);
    if ((*message_)[strings::msg_params].keyExists(strings::length)) {
      length_ = (*message_)[strings::msg_params][strings::length].asInt();
    } else {
      length_ = file_size;
    }
    if ((*message_)[strings::msg_params].keyExists(strings::offset)) {
      offset_ = (*message_)[strings::msg_params][strings::offset].asInt();
    }

    if (offset_ > file_size) {
      LOG4CXX_ERROR(logger_,
                    "Offset " << offset_ << " greater than file size "
                              << file_size);
      SendResponse(false,
                   mobile_apis::Result::INVALID_DATA,
                   "Offset greater than file size",
                   &response_params);
      return;
    }
    if (length_ > file_size - offset_) {
      LOG4CXX_ERROR(logger_,
                    "Length " << length_ << " greater than file size - offset"
                              << file_size);
      SendResponse(false,
                   mobile_apis::Result::INVALID_DATA,
                   "Length greater than file size - offset",
                   &response_params);
      return;
    }
    if (!file_system::ReadBinaryFile(full_path, bin_data, offset_, length_)) {
      LOG4CXX_ERROR(logger_, "Failed to read from file: " << full_path);
      SendResponse(false,
                   mobile_apis::Result::GENERIC_ERROR,
                   "Failed to read from file",
                   &response_params);
      return;
    }

    const uint32_t crc_calculated = GetCrc32CheckSum(bin_data);
    response_params[strings::crc32_check_sum] = crc_calculated;

  } else {
    LOG4CXX_ERROR(logger_, "HMI did not return a file path: ");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "HMI did not return a file path",
                 &response_params);
    return;
  }

  SendResponse(true,
               mobile_apis::Result::SUCCESS,
               "File uploaded",
               &response_params,
               bin_data);
}

}  // namespace commands
}  // namespace sdl_rpc_plugin

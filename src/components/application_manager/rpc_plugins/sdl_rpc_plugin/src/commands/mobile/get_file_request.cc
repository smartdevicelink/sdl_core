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
                         policy_handler) {}

GetFileRequest::~GetFileRequest() {}

std::string GetFileRequest::GetFilePath() {
  std::string file_path =
      application_manager_.get_settings().app_storage_folder();
  auto connect_key = connection_key();
  if ((*message_)[strings::msg_params].keyExists(strings::app_service_id)) {
    std::string service_id =
        (*message_)[strings::msg_params][strings::app_service_id].asString();
    LOG4CXX_DEBUG(logger_,
                  "Finding storage directory for service id: " << service_id);

    AppService app_service_info;
    if (application_manager_.GetAppServiceManager().GetAppServiceInfo(
            service_id, app_service_info)) {
      // TODO Figure how to handle hmi case (AppService mobile service = false)
      connect_key = app_service_info.connection_key;
    } else {
      return "";
    }
  } else {
    LOG4CXX_DEBUG(logger_, "Using current storage directory");
  }

  ApplicationSharedPtr app = application_manager_.application(connect_key);
  file_path += "/" + app->folder_name();
  return file_path;
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

  // Check that file name param exists else Return
  LOG4CXX_DEBUG(logger_, "Check if file name param exists");
  if (!(*message_)[strings::msg_params].keyExists(strings::file_name)) {
    LOG4CXX_ERROR(logger_, "File name parameter not specified");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "No file name",
                 &response_params);
    return;
  }

  // Initialize other params with default values. If exists overwrite the values
  LOG4CXX_DEBUG(logger_, "Intialize non manadatory params with default values");
  file_name_ = (*message_)[strings::msg_params][strings::file_name].asString();
  offset_ = 0;

  if ((*message_)[strings::msg_params].keyExists(strings::file_type)) {
    file_type_ = static_cast<mobile_apis::FileType::eType>(
        (*message_)[strings::msg_params][strings::file_type].asInt());
  }

  if ((*message_)[strings::msg_params].keyExists(strings::length)) {
    length_ = (*message_)[strings::msg_params][strings::length].asInt();
  }

  if ((*message_)[strings::msg_params].keyExists(strings::offset)) {
    offset_ = (*message_)[strings::msg_params][strings::offset].asInt();
  }

  // Check if file exists on system (may have to use app service id to get the
  // correct app folder)
  LOG4CXX_DEBUG(logger_, "Check if file exists on system");
  std::string file_path = GetFilePath();
  const std::string full_path = file_path + "/" + file_name_;
  if (!file_system::FileExists(full_path)) {
    LOG4CXX_ERROR(logger_, "File " << full_path << " does not exist");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "File does not exist",
                 &response_params);
    return;
  }

  // Handle offset
  LOG4CXX_DEBUG(logger_, "Handle offset parameter");
  const uint64_t file_size = file_system::FileSize(full_path);
  length_ = file_size;
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

  // Load data from file as binary data
  LOG4CXX_DEBUG(logger_, "Load binary data from file");
  std::vector<uint8_t> bin_data;
  if (!file_system::ReadBinaryFile(full_path, bin_data)) {
    LOG4CXX_ERROR(logger_, "Failed to read from file: " << full_path);
    SendResponse(false,
                 mobile_apis::Result::GENERIC_ERROR,
                 "Unable to read from file",
                 &response_params);
    return;
  }

  // Construct response message
  LOG4CXX_DEBUG(logger_, "Construct response");
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

  LOG4CXX_DEBUG(logger_, "file_name: " << file_name_);
  LOG4CXX_DEBUG(logger_, "file_type: " << file_type_);
  LOG4CXX_DEBUG(logger_, "offset: " << offset_);
  LOG4CXX_DEBUG(logger_, "length: " << length_);
  LOG4CXX_DEBUG(logger_, "file_path: " << file_path);
  LOG4CXX_DEBUG(logger_, "crc: " << crc_calculated);
  std::string st(bin_data.begin(), bin_data.end());
  LOG4CXX_DEBUG(logger_, "Binary data: " << st);

  SendResponse(true,
               mobile_apis::Result::SUCCESS,
               "File uploaded",
               &response_params,
               bin_data);
}

}  // namespace commands
}  // namespace sdl_rpc_plugin

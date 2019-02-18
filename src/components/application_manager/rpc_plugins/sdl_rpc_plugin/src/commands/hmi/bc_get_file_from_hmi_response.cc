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

#include "sdl_rpc_plugin/commands/hmi/bc_get_file_from_hmi_response.h"
#include "application_manager/application_impl.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/message_helper.h"
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

BCGetFileFromHMIResponse::BCGetFileFromHMIResponse(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : ResponseFromHMI(message,
                      application_manager,
                      rpc_service,
                      hmi_capabilities,
                      policy_handler)
    , length_(0)
    , offset_(0) {}

BCGetFileFromHMIResponse::~BCGetFileFromHMIResponse() {}

void BCGetFileFromHMIResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "GETFILE_FROMHMI_RESPONSE");

  MessageHelper::PrintSmartObject(*message_);
  smart_objects::SmartObjectSPtr result =
      std::make_shared<smart_objects::SmartObject>();

  smart_objects::SmartObject& response = *result;

  response[strings::params][strings::message_type] = MessageType::kResponse;
  response[strings::params][strings::correlation_id] = correlation_id();
  response[strings::params][strings::protocol_type] =
      CommandImpl::mobile_protocol_type_;
  response[strings::params][strings::protocol_version] =
      CommandImpl::protocol_version_;
  response[strings::params][strings::connection_key] =
      (*message_)[strings::msg_params][strings::app_id];
  response[strings::params][strings::function_id] = function_id();

  if ((*message_)[strings::msg_params].keyExists(strings::file_type)) {
    response[strings::msg_params][strings::file_type] =
        (*message_)[strings::msg_params][strings::file_type];
  }
  if ((*message_)[strings::msg_params].keyExists(strings::offset)) {
    response[strings::msg_params][strings::offset] =
        (*message_)[strings::msg_params][strings::offset];
    offset_ = (*message_)[strings::msg_params][strings::offset].asInt();
  }
  if ((*message_)[strings::msg_params].keyExists(strings::length)) {
    response[strings::msg_params][strings::length] =
        (*message_)[strings::msg_params][strings::length];
    length_ = (*message_)[strings::msg_params][strings::length].asInt();
  }

  if ((*message_)[strings::msg_params].keyExists(strings::file_path)) {
    std::vector<uint8_t> bin_data;
    std::string full_path =
        (*message_)[strings::msg_params][strings::file_path].asString();
    if (!file_system::ReadBinaryFile(full_path, bin_data, offset_)) {
      LOG4CXX_ERROR(logger_, "Failed to read from file: " << full_path);
      response[strings::msg_params][strings::success] = false;
      response[strings::msg_params][strings::result_code] =
          mobile_apis::Result::GENERIC_ERROR;
      SendResponseToMobile(result, application_manager_);
      return;
    }
    response[strings::params][strings::binary_data] = bin_data;

    const uint32_t crc_calculated = GetCrc32CheckSum(bin_data);
    response[strings::msg_params][strings::crc32_check_sum] = crc_calculated;
  } else {
    LOG4CXX_ERROR(logger_, "HMI did not return a file path: ");
    response[strings::msg_params][strings::success] = false;
    response[strings::msg_params][strings::result_code] =
        mobile_apis::Result::INVALID_DATA;
    SendResponseToMobile(result, application_manager_);
    return;
  }

  response[strings::msg_params][strings::success] = true;
  response[strings::msg_params][strings::result_code] =
      mobile_apis::Result::SUCCESS;

  SendResponseToMobile(result, application_manager_);
}

}  // namespace commands
}  // namespace sdl_rpc_plugin

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

#include <algorithm>
#include "application_manager/commands/mobile/put_file_request.h"

#include "application_manager/policies/policy_handler.h"
#include "application_manager/application_impl.h"

#include "utils/file_system.h"

namespace application_manager {

namespace commands {

PutFileRequest::PutFileRequest(const MessageSharedPtr& message,
                               ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager)
    , offset_(0)
    , sync_file_name_()
    , length_(0)
    , file_type_(mobile_apis::FileType::INVALID_ENUM)
    , is_persistent_file_(false) {}

PutFileRequest::~PutFileRequest() {}

void PutFileRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());
  smart_objects::SmartObject response_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);

  if (!application) {
    LOG4CXX_ERROR(logger_, "Application is not registered");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (mobile_api::HMILevel::HMI_NONE == application->hmi_level() &&
      application_manager_.get_settings().put_file_in_none() <=
          application->put_file_in_none_count()) {
    // If application is in the HMI_NONE level the quantity of allowed
    // PutFile request is limited by the configuration profile
    LOG4CXX_ERROR(logger_,
                  "Too many requests from the app with HMILevel HMI_NONE ");
    SendResponse(false,
                 mobile_apis::Result::REJECTED,
                 "Too many requests from the app with HMILevel HMI_NONE",
                 &response_params);
    return;
  }

  if (!(*message_)[strings::params].keyExists(strings::binary_data)) {
    LOG4CXX_ERROR(logger_, "Binary data empty");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "Binary data empty",
                 &response_params);
    return;
  }

  if (!(*message_)[strings::msg_params].keyExists(strings::sync_file_name)) {
    LOG4CXX_ERROR(logger_, "No file name");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "No file name",
                 &response_params);
    return;
  }

  if (!(*message_)[strings::msg_params].keyExists(strings::file_type)) {
    LOG4CXX_ERROR(logger_, "No file type");
    SendResponse(false,
                 mobile_apis::Result::INVALID_DATA,
                 "No file type",
                 &response_params);
    return;
  }
  sync_file_name_ =
      (*message_)[strings::msg_params][strings::sync_file_name].asString();
  file_type_ = static_cast<mobile_apis::FileType::eType>(
      (*message_)[strings::msg_params][strings::file_type].asInt());
  const std::vector<uint8_t> binary_data =
      (*message_)[strings::params][strings::binary_data].asBinary();

  if ((*message_)[strings::msg_params].keyExists(strings::crc)) {
    uint32_t expected_crc = (uint32_t)(
        (*message_)[strings::msg_params][strings::crc].asInt() & 0xFFFFFFFF);
    uint32_t actual_crc = CalculateCRC(binary_data);
    if (actual_crc != expected_crc) {
      LOG4CXX_ERROR(logger_,
                    "CRC failed, PutFile data is corrupted. Expected: 0x"
                        << std::hex << expected_crc << ", got: 0x"
                        << actual_crc);
      SendResponse(false,
                   mobile_apis::Result::CORRUPTED_DATA,
                   "CRC failed, PutFile data is corrupted.",
                   &response_params);
      return;
    }
  }

  // Policy table update in json format is currently to be received via PutFile
  // TODO(PV): after latest discussion has to be changed
  if (mobile_apis::FileType::JSON == file_type_) {
    application_manager_.GetPolicyHandler().ReceiveMessageFromSDK(
        sync_file_name_, binary_data);
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

  if ((*message_)[strings::msg_params].keyExists(strings::persistent_file)) {
    is_persistent_file_ =
        (*message_)[strings::msg_params][strings::persistent_file].asBool();
  }
  if ((*message_)[strings::msg_params].keyExists(strings::system_file)) {
    is_system_file =
        (*message_)[strings::msg_params][strings::system_file].asBool();
  }

  std::string file_path;

  if (is_system_file) {
    response_params[strings::space_available] = 0;
    file_path = application_manager_.get_settings().system_files_path();
  } else {
    file_path = application_manager_.get_settings().app_storage_folder();
    file_path += "/" + application->folder_name();

    uint32_t space_available = application->GetAvailableDiskSpace();

    if (binary_data.size() > space_available) {
      response_params[strings::space_available] =
          static_cast<uint32_t>(space_available);

      LOG4CXX_ERROR(logger_, "Out of memory");
      SendResponse(false,
                   mobile_apis::Result::OUT_OF_MEMORY,
                   "Out of memory",
                   &response_params);
      return;
    }
  }

  if (!file_system::CreateDirectoryRecursively(file_path)) {
    LOG4CXX_ERROR(logger_, "Can't create folder");
    SendResponse(false,
                 mobile_apis::Result::GENERIC_ERROR,
                 "Can't create folder.",
                 &response_params);
    return;
  }
  const std::string full_path = file_path + "/" + sync_file_name_;
  UNUSED(full_path);
  LOG4CXX_DEBUG(logger_,
                "Wrtiting " << binary_data.size() << "bytes to " << full_path
                            << " (current size is"
                            << file_system::FileSize(full_path) << ")");

  mobile_apis::Result::eType save_result = application_manager_.SaveBinary(
      binary_data, file_path, sync_file_name_, offset_);

  LOG4CXX_DEBUG(logger_,
                "New size of " << full_path << " is "
                               << file_system::FileSize(full_path) << " bytes");
  if (!is_system_file) {
    response_params[strings::space_available] =
        static_cast<uint32_t>(application->GetAvailableDiskSpace());
  }

  sync_file_name_ = file_path + "/" + sync_file_name_;
  switch (save_result) {
    case mobile_apis::Result::SUCCESS: {
      LOG4CXX_INFO(logger_, "PutFile is successful");
      if (!is_system_file) {
        AppFile file(sync_file_name_,
                     is_persistent_file_,
                     is_download_compleate,
                     file_type_);

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
              SendResponse(false,
                           mobile_apis::Result::INVALID_DATA,
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
      LOG4CXX_WARN(logger_,
                   "PutFile is unsuccessful. Result = " << save_result);
      SendResponse(false, save_result, "Can't save file", &response_params);
      break;
  }
}

void PutFileRequest::SendOnPutFileNotification() {
  LOG4CXX_INFO(logger_, "SendOnPutFileNotification");
  smart_objects::SmartObjectSPtr notification =
      new smart_objects::SmartObject(smart_objects::SmartType_Map);

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
  application_manager_.ManageHMICommand(notification);
}

uint32_t PutFileRequest::CalculateCRC(std::vector<uint8_t> data) {
  uint32_t crc_table[] = {
      0x00000000, 0x77073096, 0xEE0E612C, 0x990951BA, 0x076DC419, 0x706AF48F,
      0xE963A535, 0x9E6495A3, 0x0EDB8832, 0x79DCB8A4, 0xE0D5E91E, 0x97D2D988,
      0x09B64C2B, 0x7EB17CBD, 0xE7B82D07, 0x90BF1D91, 0x1DB71064, 0x6AB020F2,
      0xF3B97148, 0x84BE41DE, 0x1ADAD47D, 0x6DDDE4EB, 0xF4D4B551, 0x83D385C7,
      0x136C9856, 0x646BA8C0, 0xFD62F97A, 0x8A65C9EC, 0x14015C4F, 0x63066CD9,
      0xFA0F3D63, 0x8D080DF5, 0x3B6E20C8, 0x4C69105E, 0xD56041E4, 0xA2677172,
      0x3C03E4D1, 0x4B04D447, 0xD20D85FD, 0xA50AB56B, 0x35B5A8FA, 0x42B2986C,
      0xDBBBC9D6, 0xACBCF940, 0x32D86CE3, 0x45DF5C75, 0xDCD60DCF, 0xABD13D59,
      0x26D930AC, 0x51DE003A, 0xC8D75180, 0xBFD06116, 0x21B4F4B5, 0x56B3C423,
      0xCFBA9599, 0xB8BDA50F, 0x2802B89E, 0x5F058808, 0xC60CD9B2, 0xB10BE924,
      0x2F6F7C87, 0x58684C11, 0xC1611DAB, 0xB6662D3D, 0x76DC4190, 0x01DB7106,
      0x98D220BC, 0xEFD5102A, 0x71B18589, 0x06B6B51F, 0x9FBFE4A5, 0xE8B8D433,
      0x7807C9A2, 0x0F00F934, 0x9609A88E, 0xE10E9818, 0x7F6A0DBB, 0x086D3D2D,
      0x91646C97, 0xE6635C01, 0x6B6B51F4, 0x1C6C6162, 0x856530D8, 0xF262004E,
      0x6C0695ED, 0x1B01A57B, 0x8208F4C1, 0xF50FC457, 0x65B0D9C6, 0x12B7E950,
      0x8BBEB8EA, 0xFCB9887C, 0x62DD1DDF, 0x15DA2D49, 0x8CD37CF3, 0xFBD44C65,
      0x4DB26158, 0x3AB551CE, 0xA3BC0074, 0xD4BB30E2, 0x4ADFA541, 0x3DD895D7,
      0xA4D1C46D, 0xD3D6F4FB, 0x4369E96A, 0x346ED9FC, 0xAD678846, 0xDA60B8D0,
      0x44042D73, 0x33031DE5, 0xAA0A4C5F, 0xDD0D7CC9, 0x5005713C, 0x270241AA,
      0xBE0B1010, 0xC90C2086, 0x5768B525, 0x206F85B3, 0xB966D409, 0xCE61E49F,
      0x5EDEF90E, 0x29D9C998, 0xB0D09822, 0xC7D7A8B4, 0x59B33D17, 0x2EB40D81,
      0xB7BD5C3B, 0xC0BA6CAD, 0xEDB88320, 0x9ABFB3B6, 0x03B6E20C, 0x74B1D29A,
      0xEAD54739, 0x9DD277AF, 0x04DB2615, 0x73DC1683, 0xE3630B12, 0x94643B84,
      0x0D6D6A3E, 0x7A6A5AA8, 0xE40ECF0B, 0x9309FF9D, 0x0A00AE27, 0x7D079EB1,
      0xF00F9344, 0x8708A3D2, 0x1E01F268, 0x6906C2FE, 0xF762575D, 0x806567CB,
      0x196C3671, 0x6E6B06E7, 0xFED41B76, 0x89D32BE0, 0x10DA7A5A, 0x67DD4ACC,
      0xF9B9DF6F, 0x8EBEEFF9, 0x17B7BE43, 0x60B08ED5, 0xD6D6A3E8, 0xA1D1937E,
      0x38D8C2C4, 0x4FDFF252, 0xD1BB67F1, 0xA6BC5767, 0x3FB506DD, 0x48B2364B,
      0xD80D2BDA, 0xAF0A1B4C, 0x36034AF6, 0x41047A60, 0xDF60EFC3, 0xA867DF55,
      0x316E8EEF, 0x4669BE79, 0xCB61B38C, 0xBC66831A, 0x256FD2A0, 0x5268E236,
      0xCC0C7795, 0xBB0B4703, 0x220216B9, 0x5505262F, 0xC5BA3BBE, 0xB2BD0B28,
      0x2BB45A92, 0x5CB36A04, 0xC2D7FFA7, 0xB5D0CF31, 0x2CD99E8B, 0x5BDEAE1D,
      0x9B64C2B0, 0xEC63F226, 0x756AA39C, 0x026D930A, 0x9C0906A9, 0xEB0E363F,
      0x72076785, 0x05005713, 0x95BF4A82, 0xE2B87A14, 0x7BB12BAE, 0x0CB61B38,
      0x92D28E9B, 0xE5D5BE0D, 0x7CDCEFB7, 0x0BDBDF21, 0x86D3D2D4, 0xF1D4E242,
      0x68DDB3F8, 0x1FDA836E, 0x81BE16CD, 0xF6B9265B, 0x6FB077E1, 0x18B74777,
      0x88085AE6, 0xFF0F6A70, 0x66063BCA, 0x11010B5C, 0x8F659EFF, 0xF862AE69,
      0x616BFFD3, 0x166CCF45, 0xA00AE278, 0xD70DD2EE, 0x4E048354, 0x3903B3C2,
      0xA7672661, 0xD06016F7, 0x4969474D, 0x3E6E77DB, 0xAED16A4A, 0xD9D65ADC,
      0x40DF0B66, 0x37D83BF0, 0xA9BCAE53, 0xDEBB9EC5, 0x47B2CF7F, 0x30B5FFE9,
      0xBDBDF21C, 0xCABAC28A, 0x53B39330, 0x24B4A3A6, 0xBAD03605, 0xCDD70693,
      0x54DE5729, 0x23D967BF, 0xB3667A2E, 0xC4614AB8, 0x5D681B02, 0x2A6F2B94,
      0xB40BBE37, 0xC30C8EA1, 0x5A05DF1B, 0x2D02EF8D};

  uint32_t crc_value = 0xFFFFFFFFul;
  for (uint32_t i = 0; i < data.size(); i++) {
    crc_value = crc_table[data[i] ^ (crc_value & 0xFF)] ^ (crc_value >> 8);
  }
  return ~crc_value;
}

}  // namespace commands

}  // namespace application_manager

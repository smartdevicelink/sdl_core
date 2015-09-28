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

#include <vector>
#include <string>
#include <stdio.h>
#include "application_manager/commands/mobile/system_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/policies/policy_handler.h"
#include "interfaces/MOBILE_API.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"
#include "formatters/CFormatterJsonBase.h"
#include "json/json.h"
#include "utils/helpers.h"

namespace application_manager {

namespace commands {

uint32_t SystemRequest::index = 0;

const std::string kSYNC = "SYNC";
const std::string kIVSU = "IVSU";

SystemRequest::SystemRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {}

SystemRequest::~SystemRequest() {}

void SystemRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  ApplicationSharedPtr application =
      ApplicationManagerImpl::instance()->application(connection_key());

  if (!(application.valid())) {
    LOG4CXX_ERROR(logger_, "NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const mobile_apis::RequestType::eType request_type =
      static_cast<mobile_apis::RequestType::eType>(
          (*message_)[strings::msg_params][strings::request_type].asInt());

  if (!policy::PolicyHandler::instance()->IsRequestTypeAllowed(
          application->mobile_app_id(), request_type)) {
    SendResponse(false, mobile_apis::Result::DISALLOWED);
    return;
  }

  std::string file_name;
  if ((*message_)[strings::msg_params].keyExists(strings::file_name)) {
    file_name = (*message_)[strings::msg_params][strings::file_name].asString();
  } else {
    file_name = kSYNC;
  }

  bool is_system_file = std::string::npos != file_name.find(kSYNC) ||
                        std::string::npos != file_name.find(kIVSU);

  // to avoid override existing file
  if (is_system_file) {
    const uint8_t max_size = 255;
    char buf[max_size] = {'\0'};
    snprintf(buf, max_size - 1, "%d%s", index++, file_name.c_str());
    file_name = buf;
  }

  std::vector<uint8_t> binary_data;
  std::string binary_data_folder;
  if ((*message_)[strings::params].keyExists(strings::binary_data)) {
    binary_data = (*message_)[strings::params][strings::binary_data].asBinary();
    binary_data_folder = profile::Profile::instance()->system_files_path();
  } else {
    binary_data_folder = profile::Profile::instance()->app_storage_folder();
    binary_data_folder += "/";
    binary_data_folder += application->folder_name();
    binary_data_folder += "/";
  }

  std::string file_dst_path = profile::Profile::instance()->system_files_path();
  file_dst_path += "/";
  file_dst_path += file_name;

  if ((*message_)[strings::params].keyExists(strings::binary_data)) {
    LOG4CXX_DEBUG(
        logger_,
        "Binary data is present. Trying to save it to: " << binary_data_folder);
    if (mobile_apis::Result::SUCCESS !=
        (ApplicationManagerImpl::instance()->SaveBinary(
            binary_data, binary_data_folder, file_name, 0))) {
      LOG4CXX_DEBUG(logger_, "Binary data can't be saved.");
      SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
      return;
    }
  } else {
    std::string app_full_file_path = binary_data_folder;
    app_full_file_path += file_name;

    LOG4CXX_DEBUG(logger_,
                  "Binary data is not present. Trying to find file "
                      << file_name
                      << " within previously saved app file in "
                      << binary_data_folder);

    const AppFile* file = application->GetFile(app_full_file_path);
    if (!file || !file->is_download_complete ||
        !file_system::MoveFile(app_full_file_path, file_dst_path)) {
      LOG4CXX_DEBUG(logger_, "Binary data not found.");

      std::string origin_file_name;
      if ((*message_)[strings::msg_params].keyExists(strings::file_name)) {
        origin_file_name =
            (*message_)[strings::msg_params][strings::file_name].asString();
      }
      if (!(mobile_apis::RequestType::HTTP == request_type &&
            0 == origin_file_name.compare(kIVSU))) {
        LOG4CXX_DEBUG(logger_, "Binary data required. Reject");
        SendResponse(false, mobile_apis::Result::REJECTED);
        return;
      }
      LOG4CXX_DEBUG(logger_, "IVSU does not require binary data. Continue");
    }
    processing_file_ = file_dst_path;
  }

  LOG4CXX_DEBUG(logger_, "Binary data ok.");

  if (mobile_apis::RequestType::QUERY_APPS == request_type) {
    using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;

    smart_objects::SmartObject sm_object;
    Json::Reader reader;
    std::string json(binary_data.begin(), binary_data.end());
    Json::Value root;
    if (!reader.parse(json.c_str(), root)) {
      LOG4CXX_DEBUG(logger_, "Unable to parse query_app json file.");
      return;
    }

    CFormatterJsonBase::jsonValueToObj(root, sm_object);

    if (!ValidateQueryAppData(sm_object)) {
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }

    ApplicationManagerImpl::instance()->ProcessQueryApp(sm_object,
                                                        connection_key());
    SendResponse(true, mobile_apis::Result::SUCCESS);
    return;
  }

  smart_objects::SmartObject msg_params =
      smart_objects::SmartObject(smart_objects::SmartType_Map);
  if (std::string::npos != file_name.find(kIVSU)) {
    msg_params[strings::file_name] = file_name;
  } else {
    msg_params[strings::file_name] = file_dst_path;
  }

  if (mobile_apis::RequestType::PROPRIETARY != request_type) {
    msg_params[strings::app_id] = (application->mobile_app_id());
  }
  msg_params[strings::request_type] =
      (*message_)[strings::msg_params][strings::request_type];
  SendHMIRequest(hmi_apis::FunctionID::BasicCommunication_SystemRequest,
                 &msg_params,
                 true);
}

void SystemRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace helpers;

  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::BasicCommunication_SystemRequest: {
      mobile_apis::Result::eType result_code =
          GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asUInt()));

      const bool result = Compare<mobile_api::Result::eType, EQ, ONE>(
          result_code,
          mobile_api::Result::SUCCESS,
          mobile_api::Result::WARNINGS);

      ApplicationSharedPtr application =
          ApplicationManagerImpl::instance()->application(connection_key());

      if (!(application.valid())) {
        LOG4CXX_ERROR(logger_, "NULL pointer");
        return;
      }

      if (!processing_file_.empty()) {
        file_system::DeleteFile(processing_file_);
        processing_file_.clear();
      }
      SendResponse(result, result_code, NULL, &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}

bool SystemRequest::ValidateQueryAppData(
    const smart_objects::SmartObject& data) const {
  if (!data.isValid()) {
    LOG4CXX_ERROR(logger_, "QueryApps response is not valid.");
    return false;
  }
  if (!data.keyExists(json::response)) {
    LOG4CXX_ERROR(logger_,
                  "QueryApps response does not contain '" << json::response
                                                          << "' parameter.");
    return false;
  }
  smart_objects::SmartArray* obj_array = data[json::response].asArray();
  if (NULL == obj_array) {
    return false;
  }

  const std::size_t arr_size(obj_array->size());
  for (std::size_t idx = 0; idx < arr_size; ++idx) {
    const smart_objects::SmartObject& app_data = (*obj_array)[idx];
    if (!app_data.isValid()) {
      LOG4CXX_ERROR(logger_, "Wrong application data in json file.");
      continue;
    }
    std::string os_type;
    if (app_data.keyExists(json::ios)) {
      os_type = json::ios;
      if (!app_data[os_type].keyExists(json::urlScheme)) {
        LOG4CXX_ERROR(logger_, "Can't find URL scheme in json file.");
        continue;
      }
    } else if (app_data.keyExists(json::android)) {
      os_type = json::android;
      if (!app_data[os_type].keyExists(json::packageName)) {
        LOG4CXX_ERROR(logger_, "Can't find package name in json file.");
        continue;
      }
    }

    if (os_type.empty()) {
      LOG4CXX_ERROR(logger_, "Can't find mobile OS type in json file.");
      continue;
    }

    if (!app_data.keyExists(json::appId)) {
      LOG4CXX_ERROR(logger_, "Can't find app ID in json file.");
      continue;
    }

    if (!app_data.keyExists(json::name)) {
      LOG4CXX_ERROR(logger_, "Can't find app name in json file.");
      continue;
    }

    return true;
  }

  return false;
}

}  // namespace commands

}  // namespace application_manager

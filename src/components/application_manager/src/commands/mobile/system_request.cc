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
#include "formatters/CFormatterJsonBase.hpp"
#include "json/json.h"

namespace application_manager {

namespace commands {

uint32_t SystemRequest::index = 0;

SystemRequest::SystemRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

SystemRequest::~SystemRequest() {
}

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

  if (!(*message_)[strings::params].keyExists(strings::binary_data) &&
      (mobile_apis::RequestType::PROPRIETARY == request_type ||
       mobile_apis::RequestType::QUERY_APPS == request_type)) {

      LOG4CXX_ERROR(logger_, "Binary data empty");

      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
  }

  std::vector<uint8_t> binary_data;
  if ((*message_)[strings::params].keyExists(strings::binary_data)) {
    binary_data = (*message_)[strings::params][strings::binary_data].asBinary();
  }

  if (mobile_apis::RequestType::QUERY_APPS == request_type) {
    using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;

    const std::string json(binary_data.begin(), binary_data.end());
    Json::Value value;
    Json::Reader reader;
    if (!reader.parse(json.c_str(), value)) {
      LOG4CXX_ERROR(logger_, "Can't parse json received from QueryApps.");
      return;
    }

    smart_objects::SmartObject sm_object;
    CFormatterJsonBase::jsonValueToObj(value, sm_object);

    if (!ValidateQueryAppData(sm_object)) {
      SendResponse(false, mobile_apis::Result::INVALID_DATA);
      return;
    }

    ApplicationManagerImpl::instance()->ProcessQueryApp(sm_object,
                                                        connection_key());
    SendResponse(true, mobile_apis::Result::SUCCESS);
    return;
  }

  std::string file_path = profile::Profile::instance()->system_files_path();
  if (!file_system::CreateDirectoryRecursively(file_path)) {
    LOG4CXX_ERROR(logger_, "Cann't create folder.");
    SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
    return;
  }

  std::string file_name = "SYNC";
  if ((*message_)[strings::msg_params].keyExists(strings::file_name)) {
    file_name = (*message_)[strings::msg_params][strings::file_name].asString();
  }

  // to avoid override existing file
  const uint8_t max_size = 255;
  char buf[max_size] = {'\0'};
  snprintf(buf, sizeof(buf)/sizeof(buf[0]), "%d%s", index++, file_name.c_str());
  file_name = buf;

  std::string full_file_path = file_path + "/" + file_name;
  if (binary_data.size()) {
    if (mobile_apis::Result::SUCCESS  !=
        (ApplicationManagerImpl::instance()->SaveBinary(
            binary_data, file_path, file_name, 0))) {
      SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
      return;
    }
  } else {
    if (!policy::PolicyHandler::instance()->IsRequestTypeAllowed(
             application->mobile_app_id(), request_type)) {
      SendResponse(false, mobile_apis::Result::DISALLOWED);
      return;
    }
    std::string app_file_path =
        profile::Profile::instance()->app_storage_folder();
    std::string app_full_file_path = app_file_path + "/" + file_name;

    const AppFile* file = application->GetFile(app_full_file_path);
    if (!file || !file->is_download_complete ||
        !file_system::MoveFile(app_full_file_path, full_file_path)) {
      SendResponse(false, mobile_apis::Result::REJECTED);
      return;
    }
    processing_file_ = full_file_path;
  }

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  if (std::string::npos != file_name.find("IVSU")) {
    msg_params[strings::file_name] = file_name.c_str();
  } else {
    msg_params[strings::file_name] = full_file_path;
  }

  if (mobile_apis::RequestType::PROPRIETARY != request_type) {
    msg_params[strings::app_id] = (application->mobile_app_id());
  }
  msg_params[strings::request_type] =
      (*message_)[strings::msg_params][strings::request_type];
  SendHMIRequest(hmi_apis::FunctionID::BasicCommunication_SystemRequest,
                 &msg_params, true);

}

void SystemRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_INFO(logger_, "AddSubMenuRequest::on_event");
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::BasicCommunication_SystemRequest: {
      mobile_apis::Result::eType result_code =
          GetMobileResultCode(static_cast<hmi_apis::Common_Result::eType>(
              message[strings::params][hmi_response::code].asUInt()));
      bool result = mobile_apis::Result::SUCCESS == result_code;

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
    const smart_objects::SmartObject& data) const  {
  if (!data.isValid()) {
    LOG4CXX_ERROR(logger_, "QueryApps response is not valid.");
    return false;
  }
  if (!data.keyExists(json::response)) {
    LOG4CXX_ERROR(logger_,
                  "QueryApps response does not contain '"
                  << json::response << "' parameter.");
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

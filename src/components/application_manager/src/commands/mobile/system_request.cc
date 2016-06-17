/*

Copyright (c) 2016, Ford Motor Company
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

#include "application_manager/commands/mobile/system_request.h"

#include <vector>
#include <string>
#include <stdio.h>
#include <algorithm>
#include <sstream>
#include "application_manager/policies/policy_handler_interface.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "formatters/CFormatterJsonBase.h"

#include "utils/helpers.h"
#include "utils/custom_string.h"
#include "utils/json_utils.h"

#if defined(_MSC_VER)
#define snprintf _snprintf_s
#endif

namespace application_manager {
SDL_CREATE_LOGGER("ApplicationManager")
namespace {

#ifdef ENABLE_LOG
const char* kQueryAppsValidationFailedPrefix =
    ":QUERY_APPS_VALIDATION_FAILED: ";
#endif

const unsigned int kVrSynonymLengthMax = 40U;
const unsigned int kVrSynonymLengthMin = 1U;
const unsigned int kVrArraySizeMax = 100U;
const unsigned int kVrArraySizeMin = 1U;

class QueryAppsDataValidator {
 public:
  typedef std::set<std::string> SynonymsSet;
  typedef std::map<std::string, SynonymsSet> SynonymsMap;

  QueryAppsDataValidator(const smart_objects::SmartObject& object,
                         const ApplicationManager& manager)
      : data_(object), manager_(manager) {}

  bool Validate() const {
    SDL_AUTO_TRACE();
    if (!data_.isValid()) {
      SDL_ERROR(kQueryAppsValidationFailedPrefix
                << "QueryApps response is not valid.");
      return false;
    }
    if (!HasResponseKey()) {
      return false;
    }
    return true;
  }

 private:
  bool HasResponseKey() const {
    if (!data_.keyExists(json::response)) {
      SDL_WARN(kQueryAppsValidationFailedPrefix
               << "QueryApps response does not contain '" << json::response
               << "' parameter.");
      return false;
    }
    return true;
  }

  bool ValidateSynonymsAtLanguage(const smart_objects::SmartObject& language,
                                  const std::string& language_name,
                                  SynonymsMap& synonyms_map) const {
    if (!language[language_name].keyExists(json::vrSynonyms)) {
      SDL_WARN(kQueryAppsValidationFailedPrefix
               << "'languages.vrSynonyms' doesn't exist");
      return false;
    }
    const smart_objects::SmartArray* synonyms_array =
        language[language_name][json::vrSynonyms].asArray();
    if (!synonyms_array) {
      SDL_WARN(kQueryAppsValidationFailedPrefix << "vrSynonyms is not array.");
      return false;
    }
    const size_t synonyms_array_size = synonyms_array->size();
    if (synonyms_array_size < kVrArraySizeMin) {
      SDL_WARN(kQueryAppsValidationFailedPrefix
               << "vrSynomyms array has [" << synonyms_array_size
               << "] size < allowed min size [" << kVrArraySizeMin << "]");
      return false;
    }
    if (synonyms_array_size > kVrArraySizeMax) {
      SDL_WARN(kQueryAppsValidationFailedPrefix
               << "vrSynomyms array size [" << synonyms_array_size
               << "] exceeds maximum allowed size [" << kVrArraySizeMax << "]");
      return false;
    }

    for (std::size_t idx = 0; idx < synonyms_array_size; ++idx) {
      const smart_objects::SmartObject& synonym = (*synonyms_array)[idx];
      const std::string vrSynonym = synonym.asString();
      if (vrSynonym.length() > kVrSynonymLengthMax) {
        SDL_WARN(kQueryAppsValidationFailedPrefix
                 << "vrSYnomym item [" << idx << "] exceeds max length ["
                 << vrSynonym.length() << "]>[" << kVrSynonymLengthMax << "]");
        return false;
      }
      if (vrSynonym.length() < kVrSynonymLengthMin) {
        SDL_WARN(kQueryAppsValidationFailedPrefix
                 << "vrSYnomym item [" << idx << "] length ["
                 << vrSynonym.length() << "] is less then min length ["
                 << kVrSynonymLengthMin << "] allowed.");
        return false;
      }
      // Verify duplicates
      SynonymsMap::iterator synonyms_map_iter =
          synonyms_map.find(language_name);
      if (synonyms_map_iter != synonyms_map.end()) {
        if (!(*synonyms_map_iter).second.insert(vrSynonym).second) {
          SDL_WARN(kQueryAppsValidationFailedPrefix
                   << "vrSYnomym item already defined [" << vrSynonym.c_str()
                   << "] for language [" << language_name << "]");
          return false;
        }
      }
    }
    return true;
  }

  const smart_objects::SmartObject& data_;
  const ApplicationManager& manager_;

  DISALLOW_COPY_AND_ASSIGN(QueryAppsDataValidator);
};
}

namespace commands {
namespace custom_str = utils::custom_string;

uint32_t SystemRequest::index = 0;

const std::string kSYNC = "SYNC";
const std::string kIVSU = "IVSU";

SystemRequest::SystemRequest(const MessageSharedPtr& message,
                             ApplicationManager& application_manager)
    : CommandRequestImpl(message, application_manager) {}

SystemRequest::~SystemRequest() {}

void SystemRequest::Run() {
  SDL_AUTO_TRACE();

  ApplicationSharedPtr application =
      application_manager_.application(connection_key());

  if (!(application.valid())) {
    SDL_ERROR("NULL pointer");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  const mobile_apis::RequestType::eType request_type =
      static_cast<mobile_apis::RequestType::eType>(
          (*message_)[strings::msg_params][strings::request_type].asInt());

  const policy::PolicyHandlerInterface& policy_handler =
      application_manager_.GetPolicyHandler();
  if (!policy_handler.IsRequestTypeAllowed(application->policy_app_id(),
                                           request_type)) {
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
    binary_data_folder =
        application_manager_.get_settings().system_files_path();
  } else {
    binary_data_folder = file_system::ConcatPath(
        application_manager_.get_settings().app_storage_folder(),
        application->folder_name());
    binary_data_folder += file_system::GetPathDelimiter();
  }

  std::string file_dst_path = file_system::ConcatPath(
      application_manager_.get_settings().system_files_path(), file_name);

  if ((*message_)[strings::params].keyExists(strings::binary_data)) {
    SDL_DEBUG(
        "Binary data is present. Trying to save it to: " << binary_data_folder);
    if (mobile_apis::Result::SUCCESS !=
        (application_manager_.SaveBinary(
            binary_data, binary_data_folder, file_name, 0))) {
      SDL_DEBUG("Binary data can't be saved.");
      SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
      return;
    }
  } else {
    std::string app_full_file_path = binary_data_folder;
    app_full_file_path += file_name;

    SDL_DEBUG("Binary data is not present. Trying to find file "
              << file_name << " within previously saved app file in "
              << binary_data_folder);

    const AppFile* file = application->GetFile(app_full_file_path);
    if (!file || !file->is_download_complete ||
        !file_system::MoveFile(app_full_file_path, file_dst_path)) {
      SDL_DEBUG("Binary data not found.");

      std::string origin_file_name;
      if ((*message_)[strings::msg_params].keyExists(strings::file_name)) {
        origin_file_name =
            (*message_)[strings::msg_params][strings::file_name].asString();
      }
      if (!(mobile_apis::RequestType::HTTP == request_type &&
            0 == origin_file_name.compare(kIVSU))) {
        SDL_DEBUG("Binary data required. Reject");
        SendResponse(false, mobile_apis::Result::REJECTED);
        return;
      }
      SDL_DEBUG("IVSU does not require binary data. Continue");
    }
    processing_file_ = file_dst_path;
  }

  SDL_DEBUG("Binary data ok.");

  if (mobile_apis::RequestType::QUERY_APPS == request_type) {
    using namespace NsSmartDeviceLink::NsJSONHandler::Formatters;
    using namespace utils::json;

    smart_objects::SmartObject sm_object;
    std::string json_string(binary_data.begin(), binary_data.end());

    JsonValue::ParseResult parse_result = JsonValue::Parse(json_string);
    if (!parse_result.second) {
      SDL_DEBUG("Unable to parse query_app json file.");
      return;
    }
    JsonValue& root_json = parse_result.first;

    CFormatterJsonBase::jsonValueToObj(root_json, sm_object);

    if (!ValidateQueryAppData(sm_object)) {
      SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
      return;
    }

    application_manager_.ProcessQueryApp(sm_object, connection_key());
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
    msg_params[strings::app_id] = (application->policy_app_id());
  }
  msg_params[strings::request_type] =
      (*message_)[strings::msg_params][strings::request_type];
  SendHMIRequest(hmi_apis::FunctionID::BasicCommunication_SystemRequest,
                 &msg_params,
                 true);
}

void SystemRequest::on_event(const event_engine::Event& event) {
  SDL_AUTO_TRACE();
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
          application_manager_.application(connection_key());

      if (!(application.valid())) {
        SDL_ERROR("NULL pointer");
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
      SDL_ERROR("Received unknown event" << event.id());
      return;
    }
  }
}

bool SystemRequest::ValidateQueryAppData(
    const smart_objects::SmartObject& data) const {
  if (!data.isValid()) {
    SDL_ERROR(kQueryAppsValidationFailedPrefix
              << "QueryApps response is not valid.");
    return false;
  }
  if (!data.keyExists(json::response)) {
    SDL_ERROR(kQueryAppsValidationFailedPrefix
              << "QueryApps response does not contain '" << json::response
              << "' parameter.");
    return false;
  }

  QueryAppsDataValidator validator(data, application_manager_);
  return validator.Validate();
}

}  // namespace commands

}  // namespace application_manager

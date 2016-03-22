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
#include <map>
#include <set>
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
#include "utils/custom_string.h"

namespace application_manager {

namespace commands {

uint32_t SystemRequest::index = 0;

const std::string kSYNC = "SYNC";
const std::string kIVSU = "IVSU";

#define QUERY_APPS_VALIDATION_FAILED ":QUERY_APPS_VALIDATION_FAILED:"

struct comparer {
 public:
  bool operator()(const utils::custom_string::CustomString x,
                  const utils::custom_string::CustomString y) {
    return x.AsMBString().compare(y.AsMBString()) < 0;
  }
};

template <typename ObjectType>
class QueryAppsDataValidatorTemplate {
 public:
  typedef std::set<utils::custom_string::CustomString, comparer>
      synonyms_set_type;
  typedef std::map<std::string, synonyms_set_type> synonyms_map_type;

  QueryAppsDataValidatorTemplate(const ObjectType& o,
                                 const char* error_message = "warning")
      : data_(o)
      , error_message_(error_message)
      , max_vr_synonym_length_(40)
      , min_vr_synonym_length_(1)
      , max_tts_name_length_(500)
      , max_vr_array_size_(100) {}

  bool Validate() const {
    if (!data_.isValid()) {
      LOG4CXX_ERROR(logger_,
                    QUERY_APPS_VALIDATION_FAILED
                        << "QueryApps response is not valid.");
      return false;
    }
    if (!hasResponseKey())
      return false;

    if (app_ids_map_.size())
      app_ids_map_.clear();
    if (synonyms_map_.size())
      synonyms_map_.clear();

    return validateAppDataAndOsAndLanguagesData();
  }

 private:
  bool hasResponseKey() const {
    if (!data_.keyExists(json::response)) {
      LOG4CXX_ERROR(logger_,
                    QUERY_APPS_VALIDATION_FAILED
                        << "QueryApps response does not contain '"
                        << json::response << "' parameter.");
      return false;
    }
    return true;
  }

  bool validateAppDataAndOsAndLanguagesData() const {
    smart_objects::SmartArray* obj_array = data_[json::response].asArray();
    if (NULL == obj_array) {
      LOG4CXX_ERROR(logger_,
                    QUERY_APPS_VALIDATION_FAILED
                        << "QueryApps response is not array.");
      return false;
    }
    bool result = true;
    const std::size_t arr_size(obj_array->size());
    for (std::size_t idx = 0; idx < arr_size; ++idx) {
      const smart_objects::SmartObject& app_data = (*obj_array)[idx];

      if (!app_data.isValid()) {
        LOG4CXX_WARN(logger_,
                     QUERY_APPS_VALIDATION_FAILED
                         << "Wrong application data in json file.");
        result = false;
        continue;
      }

      if (!validateAppIdAndAppName(app_data))
        result = false;

      // verify os and dependent languages data
      std::string os_type;
      if (app_data.keyExists(json::ios)) {
        os_type = json::ios;
        if (!app_data[os_type].keyExists(json::urlScheme)) {
          LOG4CXX_WARN(logger_,
                       QUERY_APPS_VALIDATION_FAILED
                           << "Can't find URL scheme in json file.");
          result = false;
        } else {
          if (app_data[os_type][json::urlScheme].asString().length() > 255) {
            LOG4CXX_WARN(
                logger_,
                QUERY_APPS_VALIDATION_FAILED
                    << "An urlscheme length exceeds maximum allowed ["
                    << app_data[os_type][json::urlScheme].asString().length()
                    << "]>[255]");
          }
        }
      } else if (app_data.keyExists(json::android)) {
        os_type = json::android;
        if (!app_data[os_type].keyExists(json::packageName)) {
          LOG4CXX_WARN(logger_,
                       QUERY_APPS_VALIDATION_FAILED
                           << "Can't find package name in json file.");
          result = false;
        } else {
          if (app_data[json::android][json::packageName].asString().length() >
              255) {
            LOG4CXX_WARN(logger_,
                         QUERY_APPS_VALIDATION_FAILED
                             << "Package name length ["
                             << app_data[json::android][json::packageName]
                                    .asString()
                                    .length()
                             << "] exceeds max lengthCan't find package name "
                                "in json file.");
            result = false;
          }
        }
      }

      if (os_type.empty()) {
        LOG4CXX_WARN(logger_,
                     QUERY_APPS_VALIDATION_FAILED
                         << "Can't find mobile OS type in json file.");
        result = false;
        continue;
      }

      // languages verification
      if (!app_data[os_type].keyExists(json::languages)) {
        LOG4CXX_WARN(logger_, "\"languages\" not exists");
        result = false;
        continue;
      }
      if (!validateLanduages(app_data[os_type][json::languages]))
        result = false;
    }
    return result;
  }

  bool validateAppIdAndAppName(
      const smart_objects::SmartObject& app_data) const {
    // verify appid
    if (!app_data.keyExists(json::appId)) {
      LOG4CXX_WARN(logger_,
                   QUERY_APPS_VALIDATION_FAILED
                       << "Can't find app ID in json file.");
      return false;
    }

    // verify appid lenght
    const std::string app_id(app_data[json::appId].asString());
    if (app_id.length() > 100) {
      LOG4CXX_WARN(logger_,
                   QUERY_APPS_VALIDATION_FAILED
                       << "An Object ID length exceeds maximum allowed ["
                       << app_id.length() << "]>[100]");
      return false;
    }

    // verify that appid is unique
    if (app_ids_map_.find(app_id) != app_ids_map_.end()) {
      LOG4CXX_WARN(logger_,
                   QUERY_APPS_VALIDATION_FAILED
                       << "An Object ID is not unigue [" << app_id << "]");
      return false;
    } else {
      app_ids_map_[app_id] = 1;
    }

    // verify that app is not registered yet
    ApplicationSharedPtr registered_app =
        ApplicationManagerImpl::instance()->application_by_policy_id(app_id);
    if (registered_app) {
      LOG4CXX_DEBUG(logger_,
                    QUERY_APPS_VALIDATION_FAILED
                        << "Application with the same id: " << app_id
                        << " is registered already.");
      return false;
    }
    // verify app name exist
    if (!app_data.keyExists(json::name)) {
      LOG4CXX_WARN(logger_,
                   QUERY_APPS_VALIDATION_FAILED
                       << "Can't find app name in json file.");
      return false;
    }
    // and app name length
    const custom_str::CustomString appName(
        app_data[json::name].asCustomString());
    if (appName.length() > 100) {
      LOG4CXX_WARN(logger_,
                   QUERY_APPS_VALIDATION_FAILED
                       << "Name of application exceeds maximum allowed ["
                       << appName.length() << "]>[100].");
      return false;
    }

    return true;
  }

  bool validateLanduages(const smart_objects::SmartObject& languages) const {
    bool result = true;
    bool default_language_found = false;
    const size_t size = languages.length();
    // verify languages array size
    if (size > 100) {
      LOG4CXX_WARN(logger_,
                   QUERY_APPS_VALIDATION_FAILED
                       << "\"languages\" array exceeds max size [" << size
                       << "]>[100]");
      result = false;
    }
    // every language has ttsname string
    // and has vrsynonyms array
    for (size_t idx = 0; idx < size; ++idx) {
      const smart_objects::SmartObject& language = languages.getElement(idx);
      std::set<std::string> keys = language.enumerate();
      for (std::set<std::string>::const_iterator iter = keys.begin();
           iter != keys.end();
           ++iter) {
        // verify default exists
        if (!(*iter).compare(json::default_)) {
          default_language_found = true;
        }
        // add set for synonyms' duplicates validation
        if (synonyms_map_.find(*iter) == synonyms_map_.end())
          synonyms_map_[*iter] = synonyms_set_type();
        // ttsName verification
        if (!language[*iter].keyExists(json::ttsName)) {
          LOG4CXX_WARN(logger_,
                       QUERY_APPS_VALIDATION_FAILED
                           << "\"languages\".\"\ttsName\" doesn't exist");
          result = false;
        } else {
          const smart_objects::SmartObject& ttsNameObject =
              language[*iter][json::ttsName];
          // ttsName is string
          if (smart_objects::SmartType_String == ttsNameObject.getType()) {
            utils::custom_string::CustomString ttsName =
                language[*iter][json::ttsName].asCustomString();
            if (ttsName.length() > max_tts_name_length_) {
              LOG4CXX_WARN(logger_,
                           QUERY_APPS_VALIDATION_FAILED
                               << "ttsName string exceeds max length ["
                               << ttsName.length() << "]>["
                               << max_tts_name_length_ << "]");
              result = false;
            }
          } else {
            LOG4CXX_WARN(logger_,
                         QUERY_APPS_VALIDATION_FAILED
                             << "ttsName is not the string type.");
            result = false;
          }
        }
        if (!language[*iter].keyExists(json::vrSynonyms)) {
          LOG4CXX_WARN(logger_,
                       QUERY_APPS_VALIDATION_FAILED
                           << "\"languages\".\"\vrSynonyms\" doesn't exist");
          result = false;
        } else {
          smart_objects::SmartArray* syn_array =
              language[*iter][json::vrSynonyms].asArray();
          if (NULL == syn_array) {
            LOG4CXX_WARN(logger_,
                         QUERY_APPS_VALIDATION_FAILED
                             << "vrSynonyms is not array.");
            result = false;
          } else {
            const size_t syn_size = syn_array->size();
            if (0 == syn_size) {
              LOG4CXX_WARN(logger_,
                           QUERY_APPS_VALIDATION_FAILED
                               << "vrSynomyms array has [" << syn_size
                               << "] size <[1]");
              result = false;
            }
            if (syn_size > max_vr_array_size_) {
              LOG4CXX_WARN(logger_,
                           QUERY_APPS_VALIDATION_FAILED
                               << "vrSynomyms array size [" << syn_size
                               << "] exceeds maximum allowed size ["
                               << max_vr_array_size_ << "]");
              result = false;
            }
            for (std::size_t idx = 0; idx < syn_size; ++idx) {
              const smart_objects::SmartObject& synonym = (*syn_array)[idx];
              utils::custom_string::CustomString vrSynonym =
                  synonym.asCustomString();
              if (vrSynonym.length() > max_vr_synonym_length_) {
                LOG4CXX_WARN(logger_,
                             QUERY_APPS_VALIDATION_FAILED
                                 << "vrSYnomym item [" << idx
                                 << "] exceeds max length ["
                                 << vrSynonym.length() << "]>["
                                 << max_vr_synonym_length_ << "]");
                result = false;
              }
              if (vrSynonym.length() < min_vr_synonym_length_) {
                LOG4CXX_WARN(logger_,
                             QUERY_APPS_VALIDATION_FAILED
                                 << "vrSYnomym item [" << idx << "] length ["
                                 << vrSynonym.length()
                                 << "] is less then min length ["
                                 << min_vr_synonym_length_ << "] allowed.");
                result = false;
              }
              // verify duplicates
              synonyms_map_type::iterator synonyms_map_iter =
                  synonyms_map_.find(*iter);
              DCHECK(synonyms_map_iter != synonyms_map_.end());
              synonyms_set_type* synonyms_set = &(synonyms_map_iter->second);
              if (synonyms_map_iter != synonyms_map_.end()) {
                if ((*synonyms_map_iter).second.find(vrSynonym) ==
                    synonyms_set->end()) {
                  synonyms_set->insert(vrSynonym);
                } else {
                  LOG4CXX_WARN(logger_,
                               QUERY_APPS_VALIDATION_FAILED
                                   << "vrSYnomym item already defined ["
                                   << vrSynonym.c_str() << "] for language ["
                                   << *iter << "]");
                  result = false;
                }
              }
            }
          }
        }
      }
      // verify default exists
      if (!default_language_found) {
        LOG4CXX_WARN(logger_,
                     QUERY_APPS_VALIDATION_FAILED
                         << " \"languages\".\"default\" doesn't exist");
        result = false;
      }
    }
    return result;
  }

  mutable std::map<std::string, int> app_ids_map_;
  mutable synonyms_map_type synonyms_map_;

  const ObjectType& data_;
  const char* error_message_;
  const unsigned int max_vr_synonym_length_;
  const unsigned int min_vr_synonym_length_;
  const unsigned int max_tts_name_length_;
  const unsigned int max_vr_array_size_;

  DISALLOW_COPY_AND_ASSIGN(QueryAppsDataValidatorTemplate<ObjectType>);
};

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
                      << file_name << " within previously saved app file in "
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
      SendResponse(false, mobile_apis::Result::GENERIC_ERROR);
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
    LOG4CXX_ERROR(logger_,
                  QUERY_APPS_VALIDATION_FAILED
                      << "QueryApps response is not valid.");
    return false;
  }
  if (!data.keyExists(json::response)) {
    LOG4CXX_ERROR(logger_,
                  QUERY_APPS_VALIDATION_FAILED
                      << "QueryApps response does not contain '"
                      << json::response << "' parameter.");
    return false;
  }

  QueryAppsDataValidatorTemplate<smart_objects::SmartObject> validator(data);
  return validator.Validate();
}

}  // namespace commands

}  // namespace application_manager

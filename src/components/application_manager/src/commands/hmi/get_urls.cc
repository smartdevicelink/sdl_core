/*
 * Copyright (c) 2016, Ford Motor Company
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * Redistributions of source code must retain the above copyright notice, this
 * list of conditions and the following disclaimer.
 *
 * Redistributions in binary form must reproduce the above copyright notice,
 * this list of conditions and the following
 * disclaimer in the documentation and/or other materials provided with the
 * distribution.
 *
 * Neither the name of the Ford Motor Company nor the names of its contributors
 * may be used to endorse or promote products derived from this software
 * without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#include "application_manager/commands/hmi/get_urls.h"
#include "application_manager/message.h"
#include "application_manager/application_manager.h"
#include "application_manager/policies/policy_handler.h"

namespace application_manager {
namespace commands {

GetUrls::GetUrls(const MessageSharedPtr& message,
                 ApplicationManager& application_manager)
    : RequestFromHMI(message, application_manager) {}

GetUrls::~GetUrls() {}

void GetUrls::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  namespace Common_Result = hmi_apis::Common_Result;
  using policy::EndpointUrls;

  if (!application_manager_.GetPolicyHandler().PolicyEnabled()) {
    SendResponseToHMI(Common_Result::DATA_NOT_AVAILABLE);
    return;
  }

  const uint32_t service_to_check =
      (*message_)[strings::msg_params][hmi_request::service].asUInt();

  EndpointUrls endpoints;
  application_manager_.GetPolicyHandler().GetUpdateUrls(service_to_check,
                                                        endpoints);

  if (endpoints.empty()) {
    LOG4CXX_ERROR(logger_, "No URLs for service " << service_to_check);
    SendResponseToHMI(Common_Result::DATA_NOT_AVAILABLE);
    return;
  }

#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
  const uint32_t policy_service = 7u;

  if (policy_service == service_to_check) {
    ProcessPolicyServiceURLs(endpoints);
    return;
  }
#endif  // PROPRIETARY_MODE || EXTERNAL_PROPRIETARY_MODE

  ProcessServiceURLs(endpoints);
}

void GetUrls::ProcessServiceURLs(const policy::EndpointUrls& endpoints) {
  namespace Common_Result = hmi_apis::Common_Result;
  using smart_objects::SmartObject;

  (*message_)[strings::msg_params].erase(hmi_request::service);
  SmartObject& urls = (*message_)[strings::msg_params][hmi_response::urls];

  size_t index = 0;
  for (size_t e = 0; e < endpoints.size(); ++e) {
    for (size_t u = 0; u < endpoints[e].url.size(); ++u, ++index) {
      const std::string app_url = endpoints[e].url[u];

      SmartObject& service_info = urls[index];

      service_info[strings::url] = app_url;
      if (policy::kDefaultId != endpoints[e].app_id) {
#ifndef EXTERNAL_PROPRIETARY_MODE
        service_info[hmi_response::policy_app_id] = endpoints[e].app_id;
#else   // EXTERNAL_PROPRIETARY_MODE
        ApplicationSharedPtr app =
            application_manager_.application_by_policy_id(endpoints[e].app_id);

        if (!app) {
          LOG4CXX_ERROR(logger_,
                        "Can't find application with policy id "
                            << endpoints[e].app_id
                            << " URLs adding for this appliation is skipped.");
          continue;
        }

        service_info[strings::app_id] = app->hmi_app_id();
#endif  // EXTERNAL_PROPRIETARY_MODE
      }
    }
  }
  SendResponseToHMI(Common_Result::SUCCESS);
}

void GetUrls::SendResponseToHMI(hmi_apis::Common_Result::eType result) {
  (*message_)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message_)[strings::params][hmi_response::code] = result;
  application_manager_.ManageHMICommand(message_);
}

#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
struct PolicyAppIdComparator {
  PolicyAppIdComparator(const std::string& policy_app_id)
      : policy_app_id_(policy_app_id) {}

  bool operator()(const policy::EndpointData& data) {
    return data.app_id == policy_app_id_;
  }
  std::string policy_app_id_;
};

void FillSODefaultUrls(smart_objects::SmartObject& urls,
                       const policy::EndpointUrls& endpoints) {
  using smart_objects::SmartObject;
  PolicyAppIdComparator comparator(policy::kDefaultId);
  policy::EndpointUrls::const_iterator it =
      std::find_if(endpoints.begin(), endpoints.end(), comparator);
  if (it == endpoints.end()) {
    return;
  }
  SmartObject service_info = SmartObject(smart_objects::SmartType_Map);
  for (size_t i = 0; i < (*it).url.size(); ++i) {
    service_info[strings::url] = (*it).url[i];
    urls[i] = service_info;
  }
}

void GetUrls::ProcessPolicyServiceURLs(const policy::EndpointUrls& endpoints) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace smart_objects;
  using namespace application_manager;
  using namespace strings;
  using namespace hmi_apis;

  const uint32_t app_id_to_send_to =
      application_manager_.GetPolicyHandler().GetAppIdForSending();

  if (!app_id_to_send_to) {
    LOG4CXX_ERROR(logger_,
                  "There are no available applications for processing.");
    SmartObject urls(SmartType_Array);
    FillSODefaultUrls(urls, endpoints);
    if (!urls.empty()) {
      (*message_)[msg_params][hmi_response::urls] = urls;
    }
    (*message_).erase(hmi_request::service);
    SendResponseToHMI(Common_Result::SUCCESS);
    return;
  }

  ApplicationSharedPtr app =
      application_manager_.application(app_id_to_send_to);

  if (!app.valid()) {
    LOG4CXX_WARN(logger_,
                 "There is no registered application with "
                 "connection key '"
                     << app_id_to_send_to << "'");
    SendResponseToHMI(Common_Result::DATA_NOT_AVAILABLE);
    return;
  }

  SmartObject& object = *message_;
  object[msg_params].erase(hmi_request::service);
  object[msg_params][hmi_response::urls] = SmartObject(SmartType_Array);
  SmartObject& urls = object[msg_params][hmi_response::urls];

  FillSOUrls(urls, endpoints, app);
  // TODO(AOleynik): Issue with absent policy_app_id. Need to fix later on.
  // Possibly related to smart schema
  SendResponseToHMI(Common_Result::SUCCESS);
  return;
}

void GetUrls::FillSOUrls(smart_objects::SmartObject& urls,
                         const policy::EndpointUrls& endpoints,
                         const ApplicationSharedPtr app) {
  using namespace smart_objects;
  const std::string mobile_app_id = app->policy_app_id();
  size_t count_of_services = 0;

  // Will use URLs for particular application if they will be found
  // Otherwise URLs from default section will be used
  for (size_t i = 0; i < endpoints.size(); ++i) {
    SmartObject service_info = SmartObject(SmartType_Map);
    if (mobile_app_id == endpoints[i].app_id) {
      if (endpoints[i].url.size()) {
        for (size_t it_url = 0; it_url != endpoints[i].url.size(); ++it_url) {
          service_info[strings::url] = endpoints[i].url[it_url];
          urls[count_of_services++] = service_info;
        }
      }
    }
    if (policy::kDefaultId == endpoints[i].app_id) {
      for (size_t it_url = 0; it_url != endpoints[i].url.size(); ++it_url) {
        service_info[strings::app_id] = app->app_id();
        service_info[strings::url] = endpoints[i].url[it_url];
        urls[count_of_services++] = service_info;
      }
    }
  }
}

#endif  // PROPRIETARY_MODE || EXTERNAL_PROPRIETARY

}  // namespace commands
}  // namespace application_manager

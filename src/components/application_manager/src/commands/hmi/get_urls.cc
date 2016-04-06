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
#include "application_manager/application_manager_impl.h"
#include "application_manager/policies/policy_handler.h"

namespace application_manager {
namespace commands {

GetUrls::GetUrls(const MessageSharedPtr& message) : RequestFromHMI(message) {}

GetUrls::~GetUrls() {}

void GetUrls::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace smart_objects;
  using namespace application_manager;
  using namespace strings;
  using namespace hmi_apis;

  if (!policy::PolicyHandler::instance()->PolicyEnabled()) {
    SendResponseToHMI(Common_Result::DATA_NOT_AVAILABLE);
    return;
  }

  SmartObject& object = *message_;
  const std::string service_to_check =
      object[msg_params][hmi_request::service].asString();

  policy::EndpointUrls endpoints;
  policy::PolicyHandler::instance()->GetServiceUrls(
      object[strings::msg_params][hmi_request::service].asString(),
      endpoints);
  if (endpoints.empty()) {
    LOG4CXX_ERROR(logger_, "No URLs for service " << service_to_check);
    SendResponseToHMI(Common_Result::DATA_NOT_AVAILABLE);
    return;
  }

#ifdef EXTENDED_POLICY
  const std::string policy_service = "7";

  if (policy_service == service_to_check) {
    ProcessPolicyServiceURLs(endpoints);
    return;
  }
#endif
  ProcessServiceURLs(endpoints);
}

#ifdef EXTENDED_POLICY
void GetUrls::ProcessPolicyServiceURLs(const policy::EndpointUrls& endpoints) {
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace smart_objects;
  using namespace application_manager;
  using namespace strings;
  using namespace hmi_apis;

  const uint32_t app_id_to_send_to =
      policy::PolicyHandler::instance()->GetAppIdForSending();

  if (!app_id_to_send_to) {
    LOG4CXX_ERROR(logger_,
                  "There are no available applications for processing.");
    ApplicationManagerImpl::instance()->ManageHMICommand(message_);
    return;
  }

  ApplicationSharedPtr app =
      ApplicationManagerImpl::instance()->application(app_id_to_send_to);

  if (!app.valid()) {
    LOG4CXX_WARN(logger_,
                 "There is no registered application with "
                 "connection key '" << app_id_to_send_to << "'");
    SendResponseToHMI(Common_Result::DATA_NOT_AVAILABLE);
    return;
  }

  SmartObject& object = *message_;
  object[msg_params].erase(hmi_request::service);
  object[msg_params][hmi_response::urls] = SmartObject(SmartType_Array);

  SmartObject& urls = object[msg_params][hmi_response::urls];

  const std::string mobile_app_id = app->mobile_app_id();
  std::string default_url = "URL is not found";

  // Will use only one URL for particular application if it will be found
  // Otherwise URL from default section will used
  SmartObject service_info = SmartObject(SmartType_Map);

  for (size_t e = 0; e < endpoints.size(); ++e) {

    if (mobile_app_id == endpoints[e].app_id) {
      if (endpoints[e].url.size()) {
        service_info[url] = endpoints[e].url[0];
        SendResponseToHMI(Common_Result::SUCCESS);
        return;
      }
    }
    if (policy::kDefaultId == endpoints[e].app_id) {
      if (endpoints[e].url.size()) {
        default_url = endpoints[e].url[0];
      }
    }
  }

  service_info[hmi_response::policy_app_id] = mobile_app_id;
  service_info[strings::url] = default_url;
  urls[0] = service_info;
  // TODO(AOleynik): Issue with absent policy_app_id. Need to fix later on.
  // Possibly related to smart schema
  SendResponseToHMI(Common_Result::SUCCESS);
  return;
}
#endif

void GetUrls::ProcessServiceURLs(const policy::EndpointUrls& endpoints) {
  using namespace smart_objects;
  using namespace strings;
  using namespace hmi_apis;

  SmartObject& object = *message_;
  object[msg_params].erase(hmi_request::service);
  object[msg_params][hmi_response::urls] = SmartObject(SmartType_Array);

  SmartObject& urls = object[msg_params][hmi_response::urls];

  size_t index = 0;
  for (size_t e = 0; e < endpoints.size(); ++e) {
    for (size_t u = 0; u < endpoints[e].url.size(); ++u, ++index) {
      const std::string app_url = endpoints[e].url[u];

      urls[index] = SmartObject(SmartType_Map);
      SmartObject& service_info = urls[index];

      service_info[url] = app_url;
      if (policy::kDefaultId != endpoints[e].app_id) {
        service_info[hmi_response::policy_app_id] = endpoints[e].app_id;
      }
    }
  }
  SendResponseToHMI(Common_Result::SUCCESS);
}

void GetUrls::SendResponseToHMI(hmi_apis::Common_Result::eType result) {
  (*message_)[strings::params][strings::message_type] = MessageType::kResponse;
  (*message_)[strings::params][hmi_response::code] = result;
  ApplicationManagerImpl::instance()->ManageHMICommand(message_);
}

}  // namespace commands
}  // namespace application_manager

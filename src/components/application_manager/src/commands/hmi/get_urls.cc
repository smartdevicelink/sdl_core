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

  SmartObject& object = *message_;
  object[params][message_type] = MessageType::kResponse;
  if (!policy::PolicyHandler::instance()->PolicyEnabled()) {
    object[params][hmi_response::code] = Common_Result::DATA_NOT_AVAILABLE;
    ApplicationManagerImpl::instance()->ManageHMICommand(message_);
    return;
  }

  policy::EndpointUrls endpoints;
  policy::PolicyHandler::instance()->GetServiceUrls(
      object[strings::msg_params][hmi_request::service].asString(),
      endpoints);
  if (endpoints.empty()) {
    object[params][hmi_response::code] = Common_Result::DATA_NOT_AVAILABLE;
    ApplicationManagerImpl::instance()->ManageHMICommand(message_);
    return;
  }

  object[msg_params].erase(hmi_request::service);
  object[msg_params][hmi_response::urls] = SmartObject(SmartType_Array);

  SmartObject& urls = object[msg_params][hmi_response::urls];

  size_t index = 0;

  for (size_t service = 0; service < endpoints.size(); ++service) {
    for (size_t app = 0; app < endpoints[service].url.size(); ++app, ++index) {
      const std::string app_url = endpoints[service].url[app];

      urls[index] = SmartObject(SmartType_Map);
      SmartObject& service_info = urls[index];

      // TODO(AOleynik): Currently sends default for service 7
      // Must be changed to send choosen app_id, but logic should be clarified
      // first
      service_info[url] = app_url;
      if (policy::kDefaultId != endpoints[service].app_id) {
        service_info[hmi_response::policy_app_id] = endpoints[service].app_id;
      }
    }
  }

  object[params][hmi_response::code] = Common_Result::SUCCESS;
  ApplicationManagerImpl::instance()->ManageHMICommand(message_);
}

}  // namespace commands
}  // namespace application_manager

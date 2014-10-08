/**
 * Copyright (c) 2013, Ford Motor Company
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

GetUrls::GetUrls(const MessageSharedPtr& message)
  : RequestFromHMI(message) {
}

GetUrls::~GetUrls() {
}

void GetUrls::Run() {
  LOG4CXX_INFO(logger_, "GetUrls::Run");
  smart_objects::SmartObject& object = *message_;
  object[strings::params][strings::message_type] = MessageType::kResponse;
  if (policy::PolicyHandler::instance()->PolicyEnabled()) {
    policy::EndpointUrls endpoints =
      policy::PolicyHandler::instance()->GetUpdateUrls(
        object[strings::msg_params][hmi_request::service].asInt());
    object[strings::msg_params].erase(hmi_request::service);
    object[strings::msg_params][hmi_response::urls] =
      smart_objects::SmartObject(smart_objects::SmartType_Array);
    for (size_t i = 0; i < endpoints.size(); ++i) {
      std::string url = endpoints[i].url.empty() ? "" : endpoints[i].url[0];
      object[strings::msg_params][hmi_response::urls][i][strings::url] = url;
      if (policy::kDefaultId != endpoints[i].app_id) {
        object[strings::msg_params][hmi_response::urls][i][hmi_response::policy_app_id] =
          endpoints[i].app_id;
      }
    }
    object[strings::params][hmi_response::code] = hmi_apis::Common_Result::SUCCESS;
  } else {
    object[strings::params][hmi_response::code] = hmi_apis::Common_Result::DATA_NOT_AVAILABLE;
  }
  ApplicationManagerImpl::instance()->ManageHMICommand(message_);
}

}  // namespace commands
}  // namespace application_manager

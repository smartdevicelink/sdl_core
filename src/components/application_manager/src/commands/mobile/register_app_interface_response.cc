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

#include "application_manager/commands/mobile/register_app_interface_response.h"
#include "interfaces/MOBILE_API.h"
#include "application_manager/application_manager_impl.h"
#include "connection_handler/connection_handler.h"
#include "application_manager/policies/policy_handler_interface.h"

namespace application_manager {

namespace commands {

void RegisterAppInterfaceResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  mobile_apis::Result::eType result_code = mobile_apis::Result::SUCCESS;
  bool success = (*message_)[strings::msg_params][strings::success].asBool();
  bool last_message = !success;
  // Do not close connection in case of APPLICATION_NOT_REGISTERED despite it is an error
  if (!success && (*message_)[strings::msg_params].keyExists(strings::result_code)) {
    result_code = static_cast<mobile_apis::Result::eType>(
        (*message_)[strings::msg_params][strings::result_code].asInt());
    if (result_code ==  mobile_apis::Result::APPLICATION_REGISTERED_ALREADY) {
      last_message = false;
    }
  }

  SendResponse(success, result_code, last_message);

  if (mobile_apis::Result::SUCCESS != result_code) { return; }

  // Add registered application to the policy db right after response sent to
  // mobile to be able to check all other API according to app permissions
  application_manager::ApplicationSharedPtr application =
      application_manager::ApplicationManagerImpl::instance()->application(
          connection_key());
  if (!application) {
    LOG4CXX_ERROR(logger_, "Application with connection key "
                  << connection_key() << " is not registered.");
    return;
  }

  SetHeartBeatTimeout(connection_key(), application->mobile_app_id());

  // Default HMI level should be set before any permissions validation, since it
  // relies on HMI level.
  ApplicationManagerImpl::instance()->OnApplicationRegistered(application);

  // Sends OnPermissionChange notification to mobile right after RAI response
  // and HMI level set-up
  application_manager::ApplicationManagerImpl::instance()
      ->GetPolicyHandler().OnAppRegisteredOnMobile(application->mobile_app_id());
}

void RegisterAppInterfaceResponse::SetHeartBeatTimeout(
    uint32_t connection_key, const std::string& mobile_app_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  const policy::PolicyHandlerInterface& policy_handler =
      application_manager::ApplicationManagerImpl::instance()->GetPolicyHandler();
  if (policy_handler.PolicyEnabled()) {
    const uint32_t timeout = policy_handler.HeartBeatTimeout(mobile_app_id);
    if (timeout > 0) {
      application_manager::ApplicationManagerImpl::instance()->
          connection_handler().SetHeartBeatTimeout(connection_key, timeout);
    }
  } else {
    LOG4CXX_INFO(logger_, "Policy is turn off");
  }
}

}  // namespace commands
}  // namespace application_manager

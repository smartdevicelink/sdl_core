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
#include "application_manager/commands/hmi/get_system_info_response.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/message_helper.h"

namespace application_manager {

namespace commands {

GetSystemInfoResponse::GetSystemInfoResponse(
  const MessageSharedPtr& message): ResponseFromHMI(message) {
}

GetSystemInfoResponse::~GetSystemInfoResponse() {
}

void GetSystemInfoResponse::Run() {
  LOG4CXX_INFO(logger_, "GetSystemInfoResponse::Run");
  const hmi_apis::Common_Result::eType code =
      static_cast<hmi_apis::Common_Result::eType>(
          (*message_)[strings::params][hmi_response::code].asInt());

  if (hmi_apis::Common_Result::SUCCESS != code) {
    LOG4CXX_WARN(logger_, "GetSystemError returns an error code " << code);
    return;
  }
  const std::string ccpu_version =
      (*message_)[strings::msg_params]["ccpu_version"].asString();
  const std::string wers_country_code =
      (*message_)[strings::msg_params]["wersCountryCode"].asString();
  uint32_t lang_code = (*message_)[strings::msg_params]["language"].asUInt();
  const std::string language =
      application_manager::MessageHelper::CommonLanguageToString(
        static_cast<hmi_apis::Common_Language::eType>(lang_code));

  policy::PolicyHandler::instance()->OnGetSystemInfo(ccpu_version,
                                                     wers_country_code,
                                                     language);
}

}  // namespace commands

}  // namespace application_manager

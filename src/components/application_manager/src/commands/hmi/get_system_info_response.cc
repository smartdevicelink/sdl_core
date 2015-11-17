/*
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
#include "application_manager/application_manager_impl.h"
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
  LOG4CXX_AUTO_TRACE(logger_);
  const hmi_apis::Common_Result::eType code =
      static_cast<hmi_apis::Common_Result::eType>(
          (*message_)[strings::params][hmi_response::code].asInt());

  std::string ccpu_version;
  std::string wers_country_code;
  std::string language;

  if (hmi_apis::Common_Result::SUCCESS == code) {
    ccpu_version =
        (*message_)[strings::msg_params]["ccpu_version"].asString();
    wers_country_code =
        (*message_)[strings::msg_params]["wersCountryCode"].asString();
    uint32_t lang_code = (*message_)[strings::msg_params]["language"].asUInt();
    language = application_manager::MessageHelper::CommonLanguageToString(
        static_cast<hmi_apis::Common_Language::eType>(lang_code));

    HMICapabilities& hmi_capabilities =
      ApplicationManagerImpl::instance()->hmi_capabilities();
    hmi_capabilities.set_ccpu_version(ccpu_version);
  } else {
    LOG4CXX_WARN(logger_, "GetSystemError returns an error code " << code);

    // We have to set preloaded flag as false in policy table on any response
    // of GetSystemInfo (SDLAQ-CRS-2365)
    const std::string empty_value;
    policy::PolicyHandler::instance()->OnGetSystemInfo(empty_value,
                                                       empty_value,
                                                       empty_value);
    return;
  }

  // We have to set preloaded flag as false in policy table on any response
  // of GetSystemInfo (SDLAQ-CRS-2365)
  policy::PolicyHandler::instance()->OnGetSystemInfo(ccpu_version,
                                                     wers_country_code,
                                                     language);
}

}  // namespace commands

}  // namespace application_manager

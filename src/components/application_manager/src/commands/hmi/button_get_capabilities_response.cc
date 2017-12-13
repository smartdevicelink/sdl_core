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
#include "application_manager/commands/hmi/button_get_capabilities_response.h"

#include "utils/logger.h"

namespace application_manager {

namespace commands {

ButtonGetCapabilitiesResponse::ButtonGetCapabilitiesResponse(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : ResponseFromHMI(message, application_manager) {}

ButtonGetCapabilitiesResponse::~ButtonGetCapabilitiesResponse() {}

void ButtonGetCapabilitiesResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  const hmi_apis::Common_Result::eType code =
      static_cast<hmi_apis::Common_Result::eType>(
          (*message_)[strings::params][hmi_response::code].asInt());

  if (hmi_apis::Common_Result::SUCCESS != code) {
    LOG4CXX_ERROR(logger_, "Error is returned. Capabilities won't be updated.");
    return;
  }

  NonConstDataAccessor<HMICapabilities> hmi_capabilities_accessor = application_manager_.hmi_capabilities();
  HMICapabilities& hmi_capabilities = hmi_capabilities_accessor.GetData();

  hmi_capabilities.set_button_capabilities(
      (*message_)[strings::msg_params][hmi_response::capabilities]);

  hmi_capabilities.set_preset_bank_capabilities(
      (*message_)[strings::msg_params][hmi_response::preset_bank_capabilities]);
}

}  // namespace commands

}  // namespace application_manager

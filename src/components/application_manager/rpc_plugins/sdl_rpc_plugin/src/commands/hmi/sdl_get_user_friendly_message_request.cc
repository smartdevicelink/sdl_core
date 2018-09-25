/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "sdl_rpc_plugin/commands/hmi/sdl_get_user_friendly_message_request.h"
#include "application_manager/message_helper.h"

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

SDLGetUserFriendlyMessageRequest::SDLGetUserFriendlyMessageRequest(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : RequestFromHMI(message,
                     application_manager,
                     rpc_service,
                     hmi_capabilities,
                     policy_handle) {}

SDLGetUserFriendlyMessageRequest::~SDLGetUserFriendlyMessageRequest() {}

void SDLGetUserFriendlyMessageRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  const std::string messageCodes = "messageCodes";
  if (!(*message_)[strings::msg_params].keyExists(messageCodes)) {
    LOG4CXX_WARN(logger_,
                 "Mandatory parameter '" + messageCodes + "'' is missing");
    return;
  }
  smart_objects::SmartArray* msg =
      (*message_)[strings::msg_params][messageCodes].asArray();

  std::vector<std::string> msg_codes;

  smart_objects::SmartArray::const_iterator it = msg->begin();
  smart_objects::SmartArray::const_iterator it_end = msg->end();
  for (; it != it_end; ++it) {
    msg_codes.push_back((*it).asString());
  }

  std::string required_language;
  if ((*message_)[strings::msg_params].keyExists(strings::language)) {
    uint32_t lang_code =
        (*message_)[strings::msg_params][strings::language].asUInt();
    required_language =
        application_manager::MessageHelper::CommonLanguageToString(
            static_cast<hmi_apis::Common_Language::eType>(lang_code));
  } else {
    hmi_apis::Common_Language::eType ui_language =
        hmi_capabilities_.active_ui_language();

    required_language =
        application_manager::MessageHelper::CommonLanguageToString(ui_language);
  }

  policy_handler_.OnGetUserFriendlyMessage(
      msg_codes,
      required_language,
      (*message_)[strings::params][strings::correlation_id].asInt());
}

}  // namespace commands
}  // namespace application_manager

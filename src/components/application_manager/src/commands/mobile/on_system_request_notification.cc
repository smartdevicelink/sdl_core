/*
 Copyright (c) 2016, Ford Motor Company
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

#include <cstring>
#include <cstdio>
#include <string>
#include "application_manager/commands/mobile/on_system_request_notification.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "application_manager/application_manager.h"
#include "application_manager/policies/policy_handler_interface.h"

namespace application_manager {

namespace commands {

namespace mobile {

OnSystemRequestNotification::OnSystemRequestNotification(
    const MessageSharedPtr& message, ApplicationManager& application_manager)
    : CommandNotificationImpl(message, application_manager) {}

OnSystemRequestNotification::~OnSystemRequestNotification() {}

void OnSystemRequestNotification::Run() {
  SDL_AUTO_TRACE();
  using namespace application_manager;
  using namespace mobile_apis;

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app.valid()) {
    SDL_ERROR("Application with connection key " << connection_key()
                                                 << " is not registered.");
    return;
  }

  RequestType::eType request_type = static_cast<RequestType::eType>(
      (*message_)[strings::msg_params][strings::request_type].asInt());
  const policy::PolicyHandlerInterface& policy_handler =
      application_manager_.GetPolicyHandler();
  if (!policy_handler.IsRequestTypeAllowed(app->policy_app_id(),
                                           request_type)) {
    SDL_WARN("Request type " << request_type << " is not allowed by policies");
    return;
  }

  if (RequestType::PROPRIETARY == request_type) {
/* According to requirements:
   "If the requestType = PROPRIETARY, add to mobile API fileType = JSON
    If the requestType = HTTP, add to mobile API fileType = BINARY"
   Also in Genivi SDL we don't save the PT to file - we put it directly in
   binary_data */

#ifdef EXTENDED_POLICY
    const std::string filename =
        (*message_)[strings::msg_params][strings::file_name].asString();

    BinaryMessage binary_data;
    file_system::ReadBinaryFile(filename, binary_data);
    AddHeader(binary_data);
    (*message_)[strings::params][strings::binary_data] = binary_data;
#endif
    (*message_)[strings::msg_params][strings::file_type] = FileType::JSON;
  } else if (RequestType::HTTP == request_type) {
    (*message_)[strings::msg_params][strings::file_type] = FileType::BINARY;
  }

  SendNotification();
}

#ifdef EXTENDED_POLICY
void OnSystemRequestNotification::AddHeader(BinaryMessage& message) const {
  SDL_AUTO_TRACE();
  const int timeout = application_manager_.GetPolicyHandler().TimeoutExchange();

  char size_str[24];

  if (0 > sprintf(size_str, "%zu", static_cast<size_t>(message.size()))) {
    memset(size_str, 0, sizeof(size_str));
  }

  char timeout_str[24];
  if (0 > sprintf(timeout_str, "%d", timeout)) {
    memset(timeout_str, 0, sizeof(timeout_str));
  }

  const std::string header =

      "{"
      " \"HTTPRequest\": {"
      "\"headers\": {"
      "\"ContentType\": \"application/json\","
      "\"ConnectTimeout\": " +
      std::string(timeout_str) +
      ","
      "\"DoOutput\": true,"
      "\"DoInput\": true,"
      "\"UseCaches\": false,"
      "\"RequestMethod\": \"POST\","
      "\"ReadTimeout\":" +
      std::string(timeout_str) +
      ","
      "\"InstanceFollowRedirects\": false,"
      "\"charset\": \"utf-8\","
      "\"Content_Length\": " +
      std::string(size_str) +
      "},"
      "\"body\": \"" +
      std::string(message.begin(), message.end()) +
      "\""
      "}"
      "}";

  message.clear();
  message.assign(header.begin(), header.end());
  message.push_back(0u);

  SDL_DEBUG("Header added: " << std::string(message.begin(), message.end()));
}
#endif

}  // namespace mobile

}  // namespace commands

}  // namespace application_manager

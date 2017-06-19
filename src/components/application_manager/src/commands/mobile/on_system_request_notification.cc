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
  LOG4CXX_AUTO_TRACE(logger_);
  using namespace application_manager;
  using namespace mobile_apis;

  ApplicationSharedPtr app = application_manager_.application(connection_key());

  if (!app.valid()) {
    LOG4CXX_ERROR(logger_,
                  "Application with connection key " << connection_key()
                                                     << " is not registered.");
    return;
  }

  RequestType::eType request_type = static_cast<RequestType::eType>(
      (*message_)[strings::msg_params][strings::request_type].asInt());
  const policy::PolicyHandlerInterface& policy_handler =
      application_manager_.GetPolicyHandler();
  if (!policy_handler.IsRequestTypeAllowed(app->policy_app_id(),
                                           request_type)) {
    LOG4CXX_WARN(logger_,
                 "Request type " << request_type
                                 << " is not allowed by policies");
    return;
  }

  if (RequestType::PROPRIETARY == request_type) {
    /* According to requirements:
       "If the requestType = PROPRIETARY, add to mobile API fileType = JSON
        If the requestType = HTTP, add to mobile API fileType = BINARY"
       Also in Genivi SDL we don't save the PT to file - we put it directly in
       binary_data */

    const std::string filename =
        (*message_)[strings::msg_params][strings::file_name].asString();
    BinaryMessage binary_data;
    file_system::ReadBinaryFile(filename, binary_data);
#if defined(PROPRIETARY_MODE)
    AddHeader(binary_data);
#endif  // PROPRIETARY_MODE

#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
    (*message_)[strings::params][strings::binary_data] = binary_data;
#endif  // PROPRIETARY_MODE

    (*message_)[strings::msg_params][strings::file_type] = FileType::JSON;
  } else if (RequestType::HTTP == request_type) {
    (*message_)[strings::msg_params][strings::file_type] = FileType::BINARY;
  }

  SendNotification();
}

#ifdef PROPRIETARY_MODE
void OnSystemRequestNotification::AddHeader(BinaryMessage& message) const {
  LOG4CXX_AUTO_TRACE(logger_);
  const uint32_t timeout =
      application_manager_.GetPolicyHandler().TimeoutExchangeSec();

  size_t content_length;
  char size_str[24];

  if (0 > sprintf(size_str, "%zu", static_cast<size_t>(message.size()))) {
    memset(size_str, 0, sizeof(size_str));
  }

  char timeout_str[24];
  if (0 > sprintf(timeout_str, "%d", timeout)) {
    memset(timeout_str, 0, sizeof(timeout_str));
  }

  std::string policy_table_string = std::string(message.begin(), message.end());

  /* The Content-Length to be sent in the HTTP Request header should be
  calculated before additional escape characters are added to the
  policy table string. The mobile proxy will remove the escape
  characters after receiving this request. */

  content_length = ParsePTString(policy_table_string);

  if (0 > sprintf(size_str, "%zu", content_length)) {
    memset(size_str, 0, sizeof(size_str));
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
      "\"Content-Length\": " +
      std::string(size_str) +
      "},"
      "\"body\": \"" +
      policy_table_string +
      "\""
      "}"
      "}";

  message.clear();
  message.assign(header.begin(), header.end());

  LOG4CXX_DEBUG(
      logger_, "Header added: " << std::string(message.begin(), message.end()));
}

size_t OnSystemRequestNotification::ParsePTString(
    std::string& pt_string) const {
  std::string result;
  size_t length = pt_string.length();
  size_t result_length = length;
  result.reserve(length * 2);
  for (size_t i = 0; i < length; ++i) {
    if (pt_string[i] == '\"' || pt_string[i] == '\\') {
      result += '\\';
    } else if (pt_string[i] == '\n') {
      result_length--;  // contentLength is adjusted when this character is not
                        // copied to result.
      continue;
    }
    result += pt_string[i];
  }
  pt_string = result;
  return result_length;
}
#endif  // PROPRIETARY_MODE

}  // namespace mobile

}  // namespace commands

}  // namespace application_manager

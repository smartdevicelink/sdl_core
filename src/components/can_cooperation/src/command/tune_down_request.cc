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

#include "can_cooperation/commands/tune_down_request.h"
#include "functional_module/function_ids.h"
#include "json/json.h"

namespace can_cooperation {

namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "TuneDownRequest");

TuneDownRequest::TuneDownRequest(
    const application_manager::MessagePtr& message)
  : BaseCommandRequest(message) {
}

TuneDownRequest::~TuneDownRequest() {
}

void TuneDownRequest::Run() {
  LOG4CXX_INFO(logger_, "TuneDownRequest::Run");

  application_manager::ApplicationSharedPtr app =
      service_->GetApplication(message_->connection_key());
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Application doesn't registered!");
    SendResponse(false, "APPLICATION_NOT_REGISTERED",  "");
    return;
  }

  CANAppExtensionPtr extension = GetAppExtension(app);
  if (!extension->IsControlGiven()) {
    LOG4CXX_ERROR(logger_, "Application doesn't have access!");
    SendResponse(false, "REJECTED",  "");
  }

  SendRequest(functional_modules::can_api::tune_down, "");
}

void TuneDownRequest::on_event(const event_engine::Event<application_manager::MessagePtr,
              std::string>& event) {
  LOG4CXX_INFO(logger_, "TuneDownRequest::on_event");

  application_manager::ApplicationSharedPtr app =
      service_->GetApplication(message_->connection_key());
  if (!app.valid()) {
    LOG4CXX_ERROR(logger_, "Application doesn't registered!");
    SendResponse(false, "APPLICATION_NOT_REGISTERED", "");
    return;
  }

  if (functional_modules::can_api::tune_down == event.id()) {
    // TODO(VS): create function for result code parsing an use it in all command
    std::string result_code = "INVALID_DATA";
    std::string info = "";
    bool success = false;
    application_manager::MessagePtr message = event.event_message();

    Json::Value value;
     Json::Reader reader;
     reader.parse(message->json_message(), value);

    if (application_manager::MessageType::kResponse == message->type()) {
      if (value["result"].isMember("code")) {
        result_code = GetMobileResultCode(
            static_cast<hmi_apis::Common_Result::eType>(
                value["result"]["code"].asInt()));
      }
    } else if (application_manager::MessageType::kErrorResponse ==
               message->type()) {
      if (value["error"].isMember("code")) {
        result_code = GetMobileResultCode(
            static_cast<hmi_apis::Common_Result::eType>(
                value["error"]["code"].asInt()));
      }

      if (value["error"].isMember("message")) {
        info = value["result"]["message"].asCString();
      }
    }

    if (("SUCCESS" == result_code) || ("WARNINGS" == result_code)) {
      success = true;
    }

    SendResponse(success, result_code.c_str(), info);
  } else {
    LOG4CXX_ERROR(logger_,"Received unknown event: " << event.id());
    return;
  }
}

}  // namespace commands

}  // namespace can_cooperation

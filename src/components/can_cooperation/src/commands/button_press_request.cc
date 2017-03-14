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

#include "can_cooperation/commands/button_press_request.h"
#include "can_cooperation/message_helper.h"
#include "can_cooperation/validators/button_press_request_validator.h"
#include "can_cooperation/can_module_constants.h"
#include "functional_module/function_ids.h"
#include "json/json.h"
#include "can_cooperation/can_module_constants.h"

namespace can_cooperation {

namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "ButtonPressRequest")

ButtonPressRequest::ButtonPressRequest(
    const application_manager::MessagePtr& message,
    CANModuleInterface& can_module)
    : BaseCommandRequest(message, can_module) {}

ButtonPressRequest::~ButtonPressRequest() {}

void ButtonPressRequest::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value params;

  Json::Reader reader;
  reader.parse(message_->json_message(), params);

  SendRequest(functional_modules::hmi_api::button_press, params, true);
}

void ButtonPressRequest::OnEvent(
    const can_event_engine::Event<application_manager::MessagePtr, std::string>&
        event) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (functional_modules::hmi_api::button_press == event.id()) {
    std::string result_code;
    std::string info;

    Json::Value value;
    Json::Reader reader;
    reader.parse(event.event_message()->json_message(), value);

    bool success = ParseResultCode(value, result_code, info);

    SendResponse(success, result_code.c_str(), info);
  } else {
    LOG4CXX_ERROR(logger_, "Received unknown event: " << event.id());
  }
}

bool ButtonPressRequest::Validate() {
  LOG4CXX_AUTO_TRACE(logger_);

  Json::Value json;
  if (!this->ParseJsonString(&json)) {
    return false;
  }

  Json::Value outgoing_json;

  validators::ButtonPressRequestValidator button_press_request_validator;

  if (validators::ValidationResult::SUCCESS !=
      button_press_request_validator.Validate(json, outgoing_json)) {
    LOG4CXX_INFO(logger_, "ButtonPressRequest validation failed!");
    SendResponse(
        false, result_codes::kInvalidData, "Mobile request validation failed!");
    return false;
  } else {
    message_->set_json_message(MessageHelper::ValueToString(outgoing_json));
  }

  return true;
}

std::string ButtonPressRequest::ModuleType(const Json::Value& message) {
  return message.get(message_params::kModuleType, Json::Value("")).asString();
}

Json::Value ButtonPressRequest::GetInteriorZone(const Json::Value& message) {
  return message.get(message_params::kZone, Json::Value(Json::objectValue));
}

SeatLocation ButtonPressRequest::InteriorZone(const Json::Value& message) {
  Json::Value zone =
      message.get(message_params::kZone, Json::Value(Json::objectValue));
  return CreateInteriorZone(zone);
}

}  // namespace commands

}  // namespace can_cooperation

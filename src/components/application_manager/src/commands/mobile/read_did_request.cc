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

#include <string>
#include "application_manager/commands/mobile/read_did_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "interfaces/MOBILE_API.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

ReadDIDRequest::ReadDIDRequest(const MessageSharedPtr& message)
    : CommandRequestImpl(message) {
}

ReadDIDRequest::~ReadDIDRequest() {
}

void ReadDIDRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  uint32_t app_id = (*message_)[strings::params][strings::connection_key]
      .asUInt();

  ApplicationSharedPtr app = ApplicationManagerImpl::instance()->application(app_id);
  LOG4CXX_INFO(logger_, "Correlation_id :" << (*message_)[strings::params][strings::correlation_id]
               .asUInt());

  if (!app) {
    LOG4CXX_ERROR(logger_, "An application is not registered.");
    SendResponse(false, mobile_apis::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (app->IsCommandLimitsExceeded(
        static_cast<mobile_apis::FunctionID::eType>(function_id()),
        application_manager::TLimitSource::CONFIG_FILE)) {
    LOG4CXX_ERROR(logger_, "ReadDID frequency is too high.");
    SendResponse(false, mobile_apis::Result::REJECTED);
    return;
  }

  if ((*message_)[strings::msg_params][strings::did_location].empty()) {
    LOG4CXX_ERROR(logger_, "INVALID_DATA");
    SendResponse(false, mobile_apis::Result::INVALID_DATA);
    return;
  }

  smart_objects::SmartObject msg_params = smart_objects::SmartObject(
      smart_objects::SmartType_Map);
  msg_params[strings::app_id] = app->app_id();
  msg_params[strings::ecu_name] =
      (*message_)[strings::msg_params][strings::ecu_name];
  msg_params[strings::did_location] =
      (*message_)[strings::msg_params][strings::did_location];

  SendHMIRequest(hmi_apis::FunctionID::VehicleInfo_ReadDID, &msg_params, true);
}

void ReadDIDRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
    case hmi_apis::FunctionID::VehicleInfo_ReadDID: {
      mobile_apis::Result::eType result_code =
          static_cast<mobile_apis::Result::eType>(
              message[strings::params][hmi_response::code].asInt());

      bool result = mobile_apis::Result::SUCCESS == result_code;

      const std::string return_info =
          message[strings::msg_params][hmi_response::message].asString();

      SendResponse(result, result_code, return_info.c_str(),
                   &(message[strings::msg_params]));
      break;
    }
    default: {
      LOG4CXX_ERROR(logger_, "Received unknown event" << event.id());
      return;
    }
  }
}

}  // namespace commands

}  // namespace application_manager

/*
 * Copyright (c) 2016, Ford Motor Company
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

#include "vr_module/commands/process_data.h"

#include "utils/logger.h"
#include "vr_module/convertor.h"
#include "vr_module/event_engine/event_dispatcher.h"
#include "vr_module/mobile_event.h"
#include "vr_module/service_module.h"

namespace vr_module {
namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "VRModule")

using event_engine::EventDispatcher;

ProcessData::ProcessData(const vr_hmi_api::ServiceMessage& message,
                         ServiceModule* module)
    : module_(module),
      message_(message) {
  request_.set_rpc(vr_mobile_api::PROCESS_DATA);
  request_.set_rpc_type(vr_mobile_api::REQUEST);
  request_.set_correlation_id(module_->GetNextCorrelationID());
  module_->RegisterRequest(request_.correlation_id(), this);
}

ProcessData::~ProcessData() {
  EventDispatcher<vr_mobile_api::ServiceMessage, vr_mobile_api::RPCName>::instance()
      ->remove_observer(this);
}

bool ProcessData::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!module_->HasActivatedService()) {
    LOG4CXX_ERROR(logger_, "No active service");
    SendResponse(vr_hmi_api::REJECTED);
    module_->UnregisterRequest(request_.correlation_id());
    return false;
  }
  vr_hmi_api::ProcessDataRequest params;
  if (message_.has_params() && params.ParseFromString(message_.params())) {
    vr_mobile_api::ProcessDataRequest mobile_request;
    mobile_request.set_raw(params.raw());
    std::string raw;
    mobile_request.SerializeToString(&raw);
    request_.set_params(raw);
    EventDispatcher<vr_mobile_api::ServiceMessage, vr_mobile_api::RPCName>::instance()
        ->add_observer(request_.rpc(), request_.correlation_id(), this);
    return module_->SendToMobile(request_);
  } else {
    LOG4CXX_ERROR(logger_, "Could not get raw data from message");
    SendResponse(vr_hmi_api::INVALID_DATA);
    module_->UnregisterRequest(request_.correlation_id());
  }
  return false;
}

void ProcessData::OnTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  SendResponse(vr_hmi_api::TIMEOUT);
}

void ProcessData::on_event(
    const event_engine::Event<vr_mobile_api::ServiceMessage,
        vr_mobile_api::RPCName>& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const vr_mobile_api::ServiceMessage& message = event.event_message();
  vr_mobile_api::ProcessDataResponse mobile_response;
  if (message.has_params()
      && mobile_response.ParseFromString(message.params())) {
    vr_hmi_api::ResultCode code = Convertor(mobile_response.result());
    const std::string* text =
        mobile_response.has_text() ? &mobile_response.text() : NULL;
    const std::string* info =
        mobile_response.has_info() ? &mobile_response.info() : NULL;
    SendResponse(code, text, info);
  } else {
    LOG4CXX_ERROR(logger_, "Could not get raw data from message");
    SendResponse(vr_hmi_api::GENERIC_ERROR);
  }
  module_->UnregisterRequest(request_.correlation_id());
}

bool ProcessData::SendResponse(vr_hmi_api::ResultCode code,
                               const std::string* text,
                               const std::string* info) {
  LOG4CXX_AUTO_TRACE(logger_);
  message_.set_rpc_type(vr_hmi_api::RESPONSE);
  vr_hmi_api::ProcessDataResponse response;
  std::string params;
  response.set_result(code);
  if (text) {
    response.set_text(*text);
  }
  if (info) {
    response.set_info(*info);
  }
  response.SerializeToString(&params);
  message_.set_params(params);
  return module_->SendToHmi(message_);
}

}  // namespace commands
}  // namespace vr_module

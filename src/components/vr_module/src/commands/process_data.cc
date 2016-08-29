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
  }
  return false;
}

void ProcessData::OnTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  message_.set_rpc_type(vr_hmi_api::RESPONSE);
  vr_hmi_api::ProcessDataResponse hmi_response;
  hmi_response.set_result(vr_hmi_api::TIMEOUT);
  std::string params;
  hmi_response.SerializeToString(&params);
  message_.set_params(params);
  module_->SendToHmi(message_);
}

void ProcessData::on_event(
    const event_engine::Event<vr_mobile_api::ServiceMessage,
        vr_mobile_api::RPCName>& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const vr_mobile_api::ServiceMessage& message = event.event_message();
  vr_mobile_api::ProcessDataResponse mobile_response;
  if (message.has_params()
      && mobile_response.ParseFromString(message.params())) {
    message_.set_rpc_type(vr_hmi_api::RESPONSE);
    vr_hmi_api::ProcessDataResponse hmi_response;
    std::string params;
    hmi_response.set_result(Convertor(mobile_response.result()));
    if (mobile_response.has_text()) {
      hmi_response.set_text(mobile_response.text());
    }
    if (mobile_response.has_info()) {
      hmi_response.set_info(mobile_response.info());
    }
    hmi_response.SerializeToString(&params);
    message_.set_params(params);
    module_->SendToHmi(message_);
  } else {
    LOG4CXX_ERROR(logger_, "Could not get raw data from message");
  }
  module_->UnregisterRequest(request_.correlation_id());
}

}  // namespace commands
}  // namespace vr_module

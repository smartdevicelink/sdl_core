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

#include "vr_module/commands/activate_service.h"

#include "utils/logger.h"
#include "vr_module/convertor.h"
#include "vr_module/event_engine/event_dispatcher.h"
#include "vr_module/mobile_event.h"
#include "vr_module/service_module.h"

namespace vr_module {
namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "VRModule")

using event_engine::EventDispatcher;

ActivateService::ActivateService(const vr_hmi_api::ServiceMessage& message,
                                 ServiceModule* module)
    : module_(module),
      message_(message) {
  request_.set_rpc(vr_mobile_api::ACTIVATE);
  request_.set_rpc_type(vr_mobile_api::REQUEST);
  request_.set_correlation_id(module_->GetNextCorrelationID());
  module_->RegisterRequest(request_.correlation_id(), this);
}

ActivateService::~ActivateService() {
  EventDispatcher<vr_mobile_api::ServiceMessage, vr_mobile_api::RPCName>::instance()
      ->remove_observer(this);
}

bool ActivateService::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);
  vr_hmi_api::ActivateServiceRequest params;
  if (message_.has_params() && params.ParseFromString(message_.params())) {
    int32_t app_id = params.appid();
    module_->ActivateService(app_id);
    EventDispatcher<vr_mobile_api::ServiceMessage, vr_mobile_api::RPCName>::instance()
        ->add_observer(request_.rpc(), request_.correlation_id(), this);
    return module_->SendToMobile(request_);
  } else {
    LOG4CXX_ERROR(logger_, "Could not get app_id from message");
  }
  return false;
}

void ActivateService::OnTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  message_.set_rpc_type(vr_hmi_api::RESPONSE);
  vr_hmi_api::ActivateServiceResponse hmi_response;
  hmi_response.set_result(vr_hmi_api::TIMEOUT);
  std::string params;
  hmi_response.SerializeToString(&params);
  message_.set_params(params);
  module_->SendToHmi(message_);
}

void ActivateService::on_event(
    const event_engine::Event<vr_mobile_api::ServiceMessage,
        vr_mobile_api::RPCName>& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const vr_mobile_api::ServiceMessage& message = event.event_message();
  vr_mobile_api::ActivateServiceResponse mobile_response;
  if (message.has_params()
      && mobile_response.ParseFromString(message.params())) {
    bool activated = mobile_response.result() == vr_mobile_api::SUCCESS;
    if (!activated) {
      module_->DeactivateService();
    }
    message_.set_rpc_type(vr_hmi_api::RESPONSE);
    vr_hmi_api::ActivateServiceResponse hmi_response;
    hmi_response.set_result(Convertor(mobile_response.result()));
    std::string params;
    hmi_response.SerializeToString(&params);
    message_.set_params(params);
    module_->SendToHmi(message_);
  } else {
    LOG4CXX_ERROR(logger_, "Could not get result from message");
  }
  module_->UnregisterRequest(request_.correlation_id());
}

}  // namespace commands
}  // namespace vr_module

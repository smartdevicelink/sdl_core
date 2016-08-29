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

#include "vr_module/commands/support_service.h"

#include "utils/logger.h"
#include "vr_module/event_engine/event_dispatcher.h"
#include "vr_module/hmi_event.h"
#include "vr_module/service_module.h"

namespace vr_module {
namespace commands {

CREATE_LOGGERPTR_GLOBAL(logger_, "VRModule")

using event_engine::EventDispatcher;

SupportService::SupportService(const vr_hmi_api::ServiceMessage& message,
                               ServiceModule* module)
    : module_(module),
      message_(message) {
  message_.set_rpc_type(vr_hmi_api::REQUEST);
  message_.set_correlation_id(module_->GetNextCorrelationID());
  module_->RegisterRequest(message_.correlation_id(), this);
}

vr_module::commands::SupportService::~SupportService() {
  EventDispatcher<vr_hmi_api::ServiceMessage, vr_hmi_api::RPCName>::instance()
      ->remove_observer(this);
}

bool SupportService::Execute() {
  LOG4CXX_AUTO_TRACE(logger_);
  EventDispatcher<vr_hmi_api::ServiceMessage, vr_hmi_api::RPCName>::instance()
      ->add_observer(message_.rpc(), message_.correlation_id(), this);
  return module_->SendToHmi(message_);
}

void SupportService::OnTimeout() {
  LOG4CXX_AUTO_TRACE(logger_);
  module_->DisableSupport();
}

void SupportService::on_event(
    const event_engine::Event<vr_hmi_api::ServiceMessage, vr_hmi_api::RPCName>& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const vr_hmi_api::ServiceMessage& message = event.event_message();
  vr_hmi_api::SupportServiceResponse response;
  if (message.has_params() && response.ParseFromString(message.params())) {
    bool supported = response.result() == vr_hmi_api::SUCCESS;
    if (supported) {
      module_->EnableSupport();
    } else {
      module_->DisableSupport();
    }
  } else {
    LOG4CXX_ERROR(logger_, "Could not get result from message");
  }
  module_->UnregisterRequest(message.correlation_id());
}

}  // namespace commands
}  // namespace vr_module

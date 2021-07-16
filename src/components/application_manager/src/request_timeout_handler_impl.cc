/*
 * Copyright (c) 2020, Ford Motor Company
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

#include "application_manager/request_timeout_handler_impl.h"
#include "application_manager/message_helper.h"
#include "application_manager/request_info.h"
#include "utils/logger.h"

SDL_CREATE_LOG_VARIABLE("RequestTimeoutHandler")

namespace application_manager {
namespace request_controller {

RequestTimeoutHandlerImpl::RequestTimeoutHandlerImpl(
    ApplicationManager& application_manager)
    : EventObserver(application_manager.event_dispatcher())
    , application_manager_(application_manager) {
  subscribe_on_event(hmi_apis::FunctionID::BasicCommunication_OnResetTimeout);
}

void RequestTimeoutHandlerImpl::AddRequest(const uint32_t hmi_correlation_id,
                                           const Request& request) {
  requests_.insert(std::make_pair(hmi_correlation_id, request));
}

void RequestTimeoutHandlerImpl::RemoveRequest(
    const uint32_t hmi_correlation_id) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(requests_lock_);
  auto it = requests_.find(hmi_correlation_id);
  if (it != requests_.end()) {
    requests_.erase(it);
  };
}

bool RequestTimeoutHandlerImpl::IsTimeoutUpdateRequired(
    const Request& request,
    const uint32_t timeout,
    const hmi_apis::FunctionID::eType method_name) {
  if (0 == timeout) {
    SDL_LOG_WARN("Zero timeout ignored");
    return false;
  }

  if (static_cast<hmi_apis::FunctionID::eType>(request.hmi_function_id_) !=
      method_name) {
    SDL_LOG_WARN("Method name does not match the hmi function id");
    return false;
  }

  return true;
}

void RequestTimeoutHandlerImpl::on_event(const event_engine::Event& event) {
  SDL_LOG_AUTO_TRACE();
  const auto event_id = event.id();
  if (hmi_apis::FunctionID::BasicCommunication_OnResetTimeout == event_id) {
    const smart_objects::SmartObject& message = event.smart_object();
    const auto method_name = StringToEnum<hmi_apis::FunctionID::eType>(
        message[strings::msg_params][strings::method_name].asString());

    if (hmi_apis::FunctionID::INVALID_ENUM == method_name) {
      SDL_LOG_WARN(
          "Wrong method name received: "
          << message[strings::msg_params][strings::method_name].asString());
      return;
    }
    uint32_t timeout = application_manager_.get_settings().default_timeout();
    if (message[strings::msg_params].keyExists(strings::reset_period)) {
      timeout = message[strings::msg_params][strings::reset_period].asUInt();
    }
    const auto hmi_corr_id =
        message[strings::msg_params][strings::request_id].asUInt();
    auto it = requests_.find(hmi_corr_id);
    if (it != requests_.end()) {
      const auto& request = it->second;
      if (IsTimeoutUpdateRequired(request, timeout, method_name)) {
        application_manager_.UpdateRequestTimeout(
            request.connection_key_, request.mob_correlation_id_, timeout);
        application_manager_.UpdateRequestTimeout(
            RequestInfo::kHmiConnectionKey, hmi_corr_id, timeout);
      }
    } else {
      SDL_LOG_WARN("Timeout reset failed by " << hmi_corr_id
                                              << ", no such mobile command");
    }
  }
}
}  // namespace request_controller
}  // namespace application_manager

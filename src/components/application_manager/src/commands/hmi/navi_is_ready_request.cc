/*
 * Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/hmi/navi_is_ready_request.h"

namespace application_manager {

namespace commands {

NaviIsReadyRequest::NaviIsReadyRequest(const MessageSharedPtr& message,
                                       ApplicationManager& application_manager)
    : RequestToHMI(message, application_manager)
    , EventObserver(application_manager.event_dispatcher()) {}

NaviIsReadyRequest::~NaviIsReadyRequest() {}

void NaviIsReadyRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  subscribe_on_event(hmi_apis::FunctionID::Navigation_IsReady,
                     correlation_id());
  SendRequest();
}

void NaviIsReadyRequest::on_event(const event_engine::Event& event) {
  LOG4CXX_AUTO_TRACE(logger_);
  const smart_objects::SmartObject& message = event.smart_object();
  switch (event.id()) {
    case hmi_apis::FunctionID::Navigation_IsReady: {
      LOG4CXX_DEBUG(logger_, "Received Navigation_IsReady event");
      unsubscribe_from_event(hmi_apis::FunctionID::Navigation_IsReady);
      const bool is_available =
          ChangeInterfaceState(application_manager_,
                               message,
                               HmiInterfaces::HMI_INTERFACE_Navigation);

      HMICapabilities& hmi_capabilities =
          application_manager_.hmi_capabilities();
      hmi_capabilities.set_is_navi_cooperating(is_available);
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

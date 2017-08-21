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

#include "application_manager/policies/policy_event_observer.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/policies/policy_handler.h"
#include "utils/date_time.h"
#include "smart_objects/smart_object.h"

namespace policy {
using namespace application_manager;
class PolicyHandler;

CREATE_LOGGERPTR_GLOBAL(logger_, "PolicyHandler")

PolicyEventObserver::PolicyEventObserver(
    policy::PolicyHandlerInterface* const policy_handler,
    application_manager::event_engine::EventDispatcher& event_dispatcher)
    : EventObserver(event_dispatcher), policy_handler_(policy_handler) {}

void PolicyEventObserver::set_policy_handler(
    PolicyHandlerInterface* const policy_handler) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock auto_lock(policy_handler_lock_);
  LOG4CXX_DEBUG(logger_, "Set policy handler " << policy_handler);
  policy_handler_ = policy_handler;
}

void PolicyEventObserver::on_event(const event_engine::Event& event) {
  sync_primitives::AutoLock auto_lock(policy_handler_lock_);
  if (!policy_handler_) {
    return;
  }
  const smart_objects::SmartObject& message = event.smart_object();

  switch (event.id()) {
#ifdef HMI_DBUS_API
    case hmi_apis::FunctionID::VehicleInfo_GetOdometer: {
      ProcessOdometerEvent(message);
      break;
    }
    default: { break; }
      unsubscribe_from_event(hmi_apis::FunctionID::VehicleInfo_GetOdometer);
#else
    case hmi_apis::FunctionID::VehicleInfo_GetVehicleData: {
      ProcessOdometerEvent(message);
      unsubscribe_from_event(hmi_apis::FunctionID::VehicleInfo_GetVehicleData);
      break;
    }
    case hmi_apis::FunctionID::BasicCommunication_OnReady: {
      policy_handler_->OnSystemReady();
      unsubscribe_from_event(hmi_apis::FunctionID::BasicCommunication_OnReady);
      break;
    }
    default: { break; }
#endif
  }
}

void PolicyEventObserver::ProcessOdometerEvent(
    const smart_objects::SmartObject& message) {
  if (hmi_apis::Common_Result::SUCCESS ==
      static_cast<hmi_apis::Common_Result::eType>(
          message[strings::params][hmi_response::code].asInt())) {
    if (message[strings::msg_params].keyExists(strings::odometer)) {
      if (policy_handler_) {
        policy_handler_->PTUpdatedAt(
            Counters::KILOMETERS,
            message[strings::msg_params][strings::odometer].asInt());
      }
    }
  }
}

void PolicyEventObserver::subscribe_on_event(
    const event_engine::Event::EventID& event_id, int32_t hmi_correlation_id) {
  event_engine::EventObserver::subscribe_on_event(event_id, hmi_correlation_id);
}
}  //  namespace policy

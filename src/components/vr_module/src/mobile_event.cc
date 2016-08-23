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


#include "vr_module/mobile_event.h"

namespace vr_module {

MobileEvent::MobileEvent(const vr_mobile_api::ServiceMessage& message)
  : event_engine::Event<vr_mobile_api::ServiceMessage,
    vr_mobile_api::RPCName>(message, message.rpc()) {
}

MobileEvent::~MobileEvent() {
}

int32_t MobileEvent::event_message_function_id() const {
  return int32_t(event_message_.rpc());
}

int32_t MobileEvent::event_message_correlation_id() const {
  return event_message_.correlation_id();
}

event_engine::MessageType MobileEvent::event_message_type() const {
  switch (event_message_.rpc_type()) {
    case vr_mobile_api::NOTIFICATION: return event_engine::kNotification;
    case vr_mobile_api::REQUEST: return event_engine::kRequest;
    case vr_mobile_api::RESPONSE: return event_engine::kResponse;
    default: return event_engine::kRequest;
  }
}

}  //  namespace vr_module

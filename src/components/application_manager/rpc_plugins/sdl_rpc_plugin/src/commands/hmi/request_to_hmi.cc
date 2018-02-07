/*
 Copyright (c) 2014, Ford Motor Company
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

#include "application_manager/commands/hmi/request_to_hmi.h"
#include "application_manager/rpc_service.h"

namespace application_manager {

namespace commands {

bool CheckAvailabilityHMIInterfaces(ApplicationManager& application_manager,
                                    HmiInterfaces::InterfaceID interface) {
  const HmiInterfaces::InterfaceState state =
      application_manager.hmi_interfaces().GetInterfaceState(interface);
  return HmiInterfaces::STATE_NOT_AVAILABLE != state;
}

bool ChangeInterfaceState(ApplicationManager& application_manager,
                          const smart_objects::SmartObject& response_from_hmi,
                          HmiInterfaces::InterfaceID interface) {
  if (response_from_hmi[strings::msg_params].keyExists(strings::available)) {
    const bool is_available =
        response_from_hmi[strings::msg_params][strings::available].asBool();
    const HmiInterfaces::InterfaceState interface_state =
        is_available ? HmiInterfaces::STATE_AVAILABLE
                     : HmiInterfaces::STATE_NOT_AVAILABLE;
    application_manager.hmi_interfaces().SetInterfaceState(interface,
                                                           interface_state);
    return is_available;
  }
  return false;
}

RequestToHMI::RequestToHMI(const MessageSharedPtr& message,
                           ApplicationManager& application_manager)
    : CommandImpl(message, application_manager) {}

RequestToHMI::~RequestToHMI() {}

bool RequestToHMI::Init() {
  // Replace Mobile connection id with HMI app id
  return ReplaceMobileWithHMIAppId(*message_);
}

bool RequestToHMI::CleanUp() {
  return true;
}

void RequestToHMI::Run() {}

void RequestToHMI::SendRequest() {
  (*message_)[strings::params][strings::protocol_type] = hmi_protocol_type_;
  (*message_)[strings::params][strings::protocol_version] = protocol_version_;
  application_manager_.GetRPCService().SendMessageToHMI(message_);
}

}  // namespace commands

}  // namespace application_manager

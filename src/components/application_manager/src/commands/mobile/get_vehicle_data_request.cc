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

#include <string>
#include "application_manager/commands/mobile/get_vehicle_data_request.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "application_manager/message_chaining.h"
#include "interfaces/v4_protocol_v2_0_revT.h"
#include "utils/logger.h"

namespace application_manager {

namespace commands {

namespace str = strings;

log4cxx::LoggerPtr logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Commands"));

GetVehicleDataRequest::GetVehicleDataRequest(
    const MessageSharedPtr& message): CommandRequestImpl(message) {
}

GetVehicleDataRequest::~GetVehicleDataRequest() {
}

void GetVehicleDataRequest::Run() {
  LOG4CXX_INFO(logger_, "GetVehicleDataRequest::Run ");

  ApplicationImpl* app = static_cast<ApplicationImpl*>(
      ApplicationManagerImpl::instance()->
      application((*message_)[str::params][str::connection_key]));

  if (!app) {
    SendResponse(false,
                 NsSmartDeviceLinkRPC::V2::Result::APPLICATION_NOT_REGISTERED);
    return;
  }

  if (mobile_api::HMILevel::HMI_NONE == app->hmi_level()) {
    SendResponse(false,
                     NsSmartDeviceLinkRPC::V2::Result::REJECTED);
    return;
  }

  smart_objects::CSmartObject* p_smrt_vr  =
      new smart_objects::CSmartObject();

  if (!p_smrt_vr) {
    SendResponse(false,
                     NsSmartDeviceLinkRPC::V2::Result::OUT_OF_MEMORY);
    return;
  }

  // copy entirely smart object
  (*p_smrt_vr) = (*message_);

  const int correlation_id =
        (*p_smrt_vr)[strings::params][strings::correlation_id];
  const int connection_key =
        (*p_smrt_vr)[strings::params][strings::connection_key];

  // TODO(DK) HMI Request Id
  const int vr_cmd_id = 92;
  (*p_smrt_vr)[strings::params][strings::function_id] = vr_cmd_id;

  (*p_smrt_vr)[strings::params][strings::message_type] =
      MessageType::kRequest;


  MessageChaining * chain = NULL;
  chain = ApplicationManagerImpl::instance()->AddMessageChain(chain,
      connection_key, correlation_id, vr_cmd_id, p_smrt_vr);

  ApplicationManagerImpl::instance()->SendMessageToHMI(p_smrt_vr);
}

}  // namespace commands

}  // namespace application_manager

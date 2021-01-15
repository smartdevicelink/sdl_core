/*
 * Copyright (c) 2018, Ford Motor Company
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

#include "rc_rpc_plugin/commands/hmi/rc_get_interior_vehicle_data_request.h"
#include "application_manager/message_helper.h"
#include "application_manager/resumption/resume_ctrl.h"

#include "utils/macro.h"

namespace rc_rpc_plugin {
namespace commands {

SDL_CREATE_LOG_VARIABLE("Commands")

RCGetInteriorVehicleDataRequest::RCGetInteriorVehicleDataRequest(
    const app_mngr::commands::MessageSharedPtr& message,
    const RCCommandParams& params)
    : application_manager::commands::RequestToHMI(message,
                                                  params.application_manager_,
                                                  params.rpc_service_,
                                                  params.hmi_capabilities_,
                                                  params.policy_handler_) {}

RCGetInteriorVehicleDataRequest::~RCGetInteriorVehicleDataRequest() {}

void RCGetInteriorVehicleDataRequest::Run() {
  SDL_LOG_AUTO_TRACE();
  SendRequest();
}

void RCGetInteriorVehicleDataRequest::OnTimeOut() {
  SDL_LOG_TRACE("function_id: " << function_id()
                                << " correlation_id: " << correlation_id());
  using namespace application_manager;
  event_engine::Event timeout_event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);

  auto error_response = MessageHelper::CreateNegativeResponseFromHmi(
      function_id(),
      correlation_id(),
      hmi_apis::Common_Result::GENERIC_ERROR,
      std::string("Timed out"));
  timeout_event.set_smart_object(*error_response);
  timeout_event.raise(application_manager_.event_dispatcher());
}

}  // namespace commands
}  // namespace rc_rpc_plugin

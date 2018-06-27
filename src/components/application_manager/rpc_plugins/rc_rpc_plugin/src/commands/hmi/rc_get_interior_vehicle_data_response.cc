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

#include "rc_rpc_plugin/commands/hmi/rc_get_interior_vehicle_data_response.h"
#include "rc_rpc_plugin/rc_module_constants.h"
#include "application_manager/event_engine/event.h"

namespace rc_rpc_plugin {
namespace commands {

RCGetInteriorVehicleDataResponse::RCGetInteriorVehicleDataResponse(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle,
    ResourceAllocationManager& resource_allocation_manager,
    InteriorDataCache& interior_data_cache)
    : application_manager::commands::ResponseFromHMI(message,
                                                     application_manager,
                                                     rpc_service,
                                                     hmi_capabilities,
                                                     policy_handle) {
  UNUSED(resource_allocation_manager);
  UNUSED(interior_data_cache);
}

void RCGetInteriorVehicleDataResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  app_mngr::event_engine::Event event(
      hmi_apis::FunctionID::RC_GetInteriorVehicleData);

  smart_objects::SmartObject& module_data = (*message_)
      [application_manager::strings::msg_params][message_params::kModuleData];
  if (module_data.keyExists(message_params::kAudioControlData)) {
    smart_objects::SmartObject& audio_control_data =
        module_data[message_params::kAudioControlData];
    if (audio_control_data.keyExists(message_params::kKeepContext)) {
      audio_control_data.erase(message_params::kKeepContext);
    }
  }

  event.set_smart_object(*message_);
  event.raise(application_manager_.event_dispatcher());
}

RCGetInteriorVehicleDataResponse::~RCGetInteriorVehicleDataResponse() {}

}  // namespace commands
}  // namespace rc_rpc_plugin

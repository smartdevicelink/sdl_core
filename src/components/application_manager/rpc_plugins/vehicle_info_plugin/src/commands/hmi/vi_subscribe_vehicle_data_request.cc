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

#include "vehicle_info_plugin/commands/hmi/vi_subscribe_vehicle_data_request.h"
#include "application_manager/message_helper.h"
#include "application_manager/resumption/resume_ctrl.h"

namespace vehicle_info_plugin {
using namespace application_manager;

namespace commands {

VISubscribeVehicleDataRequest::VISubscribeVehicleDataRequest(
    const application_manager::commands::MessageSharedPtr& message,
    const VehicleInfoCommandParams& params)
    : RequestToHMI(message,
                   params.application_manager_,
                   params.rpc_service_,
                   params.hmi_capabilities_,
                   params.policy_handler_)
    , custom_vehicle_data_manager_(params.custom_vehicle_data_manager_) {}

VISubscribeVehicleDataRequest::~VISubscribeVehicleDataRequest() {}

void VISubscribeVehicleDataRequest::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  const auto& rpc_spec_vehicle_data = MessageHelper::vehicle_data();
  auto& msg_params = (*message_)[strings::msg_params];

  smart_objects::SmartObject custom_data;
  for (const auto& name : msg_params.enumerate()) {
    const auto& found_it = rpc_spec_vehicle_data.find(name);
    if (rpc_spec_vehicle_data.end() == found_it) {
      custom_data[name] = msg_params[name];
      msg_params.erase(name);
    }
  }

  auto hmi_custom_msg_params =
      custom_vehicle_data_manager_.CreateHMIMessageParams(
          custom_data.enumerate());
  for (const auto& item : hmi_custom_msg_params.enumerate()) {
    msg_params[item] = hmi_custom_msg_params[item];
  }

  SendRequest();
}

void VISubscribeVehicleDataRequest::onTimeOut() {
  auto& resume_ctrl = application_manager_.resume_controller();

  resume_ctrl.HandleOnTimeOut(
      correlation_id(),
      static_cast<hmi_apis::FunctionID::eType>(function_id()));
}

}  // namespace commands
}  // namespace vehicle_info_plugin

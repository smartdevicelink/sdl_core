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
#include "vehicle_info_plugin/commands/hmi/vi_get_vehicle_type_response.h"

namespace vehicle_info_plugin {
using namespace application_manager;

namespace commands {

VIGetVehicleTypeResponse::VIGetVehicleTypeResponse(
    const application_manager::commands::MessageSharedPtr& message,
    const VehicleInfoCommandParams& params)
    : ResponseFromHMI(message,
                      params.application_manager_,
                      params.rpc_service_,
                      params.hmi_capabilities_,
                      params.policy_handler_) {}

VIGetVehicleTypeResponse::~VIGetVehicleTypeResponse() {}

void VIGetVehicleTypeResponse::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  const auto result_code = static_cast<hmi_apis::Common_Result::eType>(
      (*message_)[strings::params][hmi_response::code].asInt());

  if (hmi_apis::Common_Result::SUCCESS != result_code) {
    LOG4CXX_DEBUG(logger_,
                  "Request was not successful. Don't change HMI capabilities");
    return;
  }

  std::vector<std::string> sections_to_update;
  hmi_capabilities_.set_vehicle_type(
      (*message_)[strings::msg_params][hmi_response::vehicle_type]);
  sections_to_update.push_back(hmi_response::vehicle_type);

  if (!hmi_capabilities_.SaveCachedCapabilitiesToFile(
          hmi_interface::vehicle_info,
          sections_to_update,
          message_->getSchema())) {
    LOG4CXX_ERROR(
        logger_, "Failed to save VehicleInfo.GetVehicleType response to cache");
  }
}

}  // namespace commands

}  // namespace vehicle_info_plugin

/*
 Copyright (c) 2019, Ford Motor Company, Livio
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the the copyright holders nor the names of their
 contributors may be used to endorse or promote products derived from this
 software without specific prior written permission.

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

#include "sdl_rpc_plugin/commands/hmi/bc_join_network_request_from_hmi.h"
#include "application_manager/application_impl.h"
#include "application_manager/rpc_service.h"
#include "interfaces/MOBILE_API.h"

#include "application_manager/message_helper.h"

namespace sdl_rpc_plugin {
using namespace application_manager;
namespace commands {

BCJoinNetworkRequestFromHMI::BCJoinNetworkRequestFromHMI(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : RequestFromHMI(message,
                         application_manager,
                         rpc_service,
                         hmi_capabilities,
                         policy_handler) {}

BCJoinNetworkRequestFromHMI::~BCJoinNetworkRequestFromHMI() {}

void BCJoinNetworkRequestFromHMI::Run() {
  LOG4CXX_AUTO_TRACE(logger_);
  std::string policy_app_id = (*message_)[strings::msg_params][strings::policy_app_id].asString();
  ApplicationSharedPtr app = application_manager_.application_by_policy_id(policy_app_id);
  smart_objects::SmartObject mobile_msg_params(smart_objects::SmartType_Map);
  mobile_msg_params[strings::wifi_state] = (*message_)[strings::msg_params][strings::wifi_state];
  mobile_msg_params[strings::ssid] = (*message_)[strings::msg_params][strings::ssid];
  mobile_msg_params[strings::password] = (*message_)[strings::msg_params][strings::password];
  mobile_msg_params[strings::security_type] = (*message_)[strings::msg_params][strings::security_type];
  SendMobileRequest(mobile_apis::FunctionID::JoinNetworkID, app, &mobile_msg_params, true);

}

void BCJoinNetworkRequestFromHMI::on_event(
    const event_engine::MobileEvent& event) {
  smart_objects::SmartObject event_message(event.smart_object());

  auto& msg_params = event_message[strings::msg_params];

  mobile_apis::Result::eType mobile_result =
      static_cast<mobile_apis::Result::eType>(
          msg_params[strings::result_code].asInt());
  hmi_apis::Common_Result::eType result =
      MessageHelper::MobileToHMIResult(mobile_result);
  bool success = IsMobileResultSuccess(mobile_result);


  SendResponse(success,
                correlation_id(),
                hmi_apis::FunctionID::BasicCommunication_JoinNetwork,
                result,
                &msg_params,
                application_manager::commands::Command::SOURCE_SDL_TO_HMI);

}

}  // namespace commands
}  // namespace app_service_rpc_plugin

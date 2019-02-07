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

#include "app_service_rpc_plugin/app_service_command_factory.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "AppServiceRpcPlugin")

namespace app_service_rpc_plugin {

AppServiceCommandFactory::AppServiceCommandFactory(
    app_mngr::ApplicationManager& application_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : hmi_command_factory_(new AppServiceHmiCommandFactory(
          application_manager, rpc_service, hmi_capabilities, policy_handler))
    , mobile_command_factory_(new AppServiceMobileCommandFactory(
          application_manager, rpc_service, hmi_capabilities, policy_handler)) {
  LOG4CXX_AUTO_TRACE(logger_);
}

AppServiceCommandFactory::~AppServiceCommandFactory() {
  LOG4CXX_AUTO_TRACE(logger_);
}

app_mngr::CommandSharedPtr AppServiceCommandFactory::CreateCommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  if (app_mngr::commands::Command::SOURCE_HMI == source) {
    return hmi_command_factory_->CreateCommand(message, source);
  } else {
    return mobile_command_factory_->CreateCommand(message, source);
  }
}

bool AppServiceCommandFactory::IsAbleToProcess(
    const int32_t function_id,
    const commands::Command::CommandSource source) const {
  LOG4CXX_DEBUG(logger_,
                "AppServiceCommandFactory::IsAbleToProcess" << function_id
                                                            << " " << source);
  return commands::Command::SOURCE_HMI == source
             ? hmi_command_factory_->IsAbleToProcess(function_id, source)
             : mobile_command_factory_->IsAbleToProcess(function_id, source);
}
}  // namespace service_plugin

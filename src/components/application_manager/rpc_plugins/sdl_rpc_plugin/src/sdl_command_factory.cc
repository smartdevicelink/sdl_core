/*
 Copyright (c) 2018, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the copyright holders nor the names of their contributors
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

#include "sdl_rpc_plugin/sdl_command_factory.h"
#include "sdl_rpc_plugin/hmi_command_factory.h"
#include "sdl_rpc_plugin/mobile_command_factory.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

SDLCommandFactory::SDLCommandFactory(
    app_mngr::ApplicationManager& app_manager,
    app_mngr::rpc_service::RPCService& rpc_service,
    app_mngr::HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handler)
    : app_manager_(app_manager)
    , rpc_service_(rpc_service)
    , hmi_capabilities_(hmi_capabilities)
    , policy_handler_(policy_handler) {
  hmi_command_factory_.reset(new HMICommandFactory(
      app_manager, rpc_service, hmi_capabilities, policy_handler));
  mobile_command_factory_.reset(new MobileCommandFactory(
      app_manager, rpc_service, hmi_capabilities, policy_handler));
}

app_mngr::CommandSharedPtr SDLCommandFactory::CreateCommand(
    const app_mngr::commands::MessageSharedPtr& message,
    app_mngr::commands::Command::CommandSource source) {
  if (app_mngr::commands::Command::SOURCE_HMI == source ||
      app_mngr::commands::Command::SOURCE_SDL_TO_HMI == source) {
    return hmi_command_factory_->CreateCommand(message, source);
  } else {
    return mobile_command_factory_->CreateCommand(message, source);
  }
}

bool SDLCommandFactory::IsAbleToProcess(
    const int32_t FunctionID,
    const application_manager::commands::Command::CommandSource source) const {
  bool is_hmi_command_factory_able_to_process =
      hmi_command_factory_->IsAbleToProcess(FunctionID, source);
  bool is_mobile_command_factory_able_to_process =
      mobile_command_factory_->IsAbleToProcess(FunctionID, source);

  return (app_mngr::commands::Command::SOURCE_HMI == source ||
          app_mngr::commands::Command::SOURCE_SDL_TO_HMI == source)
             ? is_hmi_command_factory_able_to_process
             : is_mobile_command_factory_able_to_process;
}

}  // namespace application_manager

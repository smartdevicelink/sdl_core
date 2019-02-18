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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_GET_FILE_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_GET_FILE_REQUEST_H_

#include "application_manager/commands/command_request_impl.h"
#include "application_manager/event_engine/event.h"

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

/**
 * @brief GetFileRequest command class
 **/
class GetFileRequest : public app_mngr::commands::CommandRequestImpl {
 public:
  /**
   * @brief GetFileRequest class constructor
   *
   * @param message Incoming SmartObject message
   **/
  GetFileRequest(const app_mngr::commands::MessageSharedPtr& message,
                 app_mngr::ApplicationManager& application_manager,
                 app_mngr::rpc_service::RPCService& rpc_service,
                 app_mngr::HMICapabilities& hmi_capabilities,
                 policy::PolicyHandlerInterface& policy_handle);

  /**
   * @brief GetFileRequest class destructor
   **/
  virtual ~GetFileRequest();

  /**
   * @brief Execute command
   **/
  virtual void Run();

  bool GetFilePath(std::string& file_path, bool& forward_to_hmi);

  void on_event(const app_mngr::event_engine::Event& event) FINAL;

 private:
  std::string file_name_;
  mobile_apis::FileType::eType file_type_;
  uint32_t length_;
  uint32_t offset_;

  DISALLOW_COPY_AND_ASSIGN(GetFileRequest);
};

}  // namespace commands

}  // namespace sdl_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_GET_FILE_REQUEST_H_

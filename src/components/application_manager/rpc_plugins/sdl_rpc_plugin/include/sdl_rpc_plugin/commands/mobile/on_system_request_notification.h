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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_ON_SYSTEM_REQUEST_NOTIFICATION_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_SDL_RPC_PLUGIN_INCLUDE_SDL_RPC_PLUGIN_COMMANDS_MOBILE_ON_SYSTEM_REQUEST_NOTIFICATION_H_

#include "application_manager/commands/command_notification_impl.h"
#include "policy/policy_table/enums.h"
#include <vector>

namespace sdl_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {

namespace mobile {

/**
 * @brief OnSystemRequestNotification class
 **/
class OnSystemRequestNotification
    : public app_mngr::commands::CommandNotificationImpl {
 public:
  typedef std::vector<uint8_t> BinaryMessage;
  /**
   * @brief OnSystemRequestNotification class constructor
   *
   * @param message Incoming SmartObject message
   **/
  OnSystemRequestNotification(
      const app_mngr::commands::MessageSharedPtr& message,
      app_mngr::ApplicationManager& application_manager,
      app_mngr::rpc_service::RPCService& rpc_service,
      app_mngr::HMICapabilities& hmi_capabilities,
      policy::PolicyHandlerInterface& policy_handler);

  /**
   * @brief OnSystemRequestNotification class destructor
   **/
  ~OnSystemRequestNotification() OVERRIDE;

  /**
   * @brief Execute command
   **/
  void Run() OVERRIDE;

 private:
#if defined(PROPRIETARY_MODE) || defined(EXTERNAL_PROPRIETARY_MODE)
  /**
   * @brief Adds the full_app_id_supported to a string, allows attaching this
  flag to a PT snapshot
   * @param policy_table_string PT snapshot string
   * @param useFullAppID what to set full_app_id_supported to in the snapshot
  \ */
  void AttachFullAppIdSupport(std::string& policy_table_string,
                              bool useFullAppID) const;
#endif

#ifdef PROPRIETARY_MODE

/**
   * @brief Adds HTTP header to message
   * @param message Message
   * @param useFullAppID 
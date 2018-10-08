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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_BUTTON_PRESS_REQUEST_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_BUTTON_PRESS_REQUEST_H_

#include "rc_rpc_plugin/commands/rc_command_request.h"

namespace rc_rpc_plugin {
namespace app_mngr = application_manager;

namespace commands {
class ButtonPressRequest : public RCCommandRequest {
 public:
  ButtonPressRequest(
      const application_manager::commands::MessageSharedPtr& message,
      const RCCommandParams& params);

  /**
   * @brief Execute command
   * send HMI request if message contains appropriate
   * button name and module type
   * otherwise sends negative sesponse to mobile
   */
  void Execute() FINAL;

  /**
   * @brief AcquireResource Tries to acquire specific resource
   * @param message Incoming message containg the resource name
   * @return Acquire result
   */
  AcquireResult::eType AcquireResource(
      const app_mngr::commands::MessageSharedPtr& message) FINAL;

  /**
   * @brief IsResourceFree check resource state
   * @param module_type Resource name
   * @return True if free, otherwise - false
   */
  bool IsResourceFree(const std::string& module_type) const FINAL;

  /**
   * @brief SetResourceState changes state of resource
   * @param state State to set for resource
   */
  void SetResourceState(const std::string& module_type,
                        const ResourceState::eType state) FINAL;

  /**
   * @brief Interface method that is called whenever new event received
   *
   * @param event The received event
   */
  void on_event(const app_mngr::event_engine::Event& event) FINAL;

  std::string ModuleType() FINAL;

  /**
   * @brief ButtonPressRequest class destructor
   */
  ~ButtonPressRequest();
};

}  // namespace commands
}  // namespace rc_rpc_plugin

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_RPC_PLUGINS_RC_RPC_PLUGIN_INCLUDE_RC_RPC_PLUGIN_COMMANDS_MOBILE_BUTTON_PRESS_REQUEST_H_

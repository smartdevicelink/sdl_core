/*
 Copyright (c) 2017, Ford Motor Company
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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_COMMANDS_COMMAND_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_COMMANDS_COMMAND_H_

#include "utils/shared_ptr.h"
#include "remote_control/remote_plugin_interface.h"
#include "remote_control/event_engine/event.h"

namespace remote_control {

class RemotePluginInterface;

namespace commands {

/**
 * @brief Command interface
 **/
class Command {
 public:
  /**
   * @brief Execute command
   */
  virtual void Run() = 0;

  /**
   * \brief Command class destructor
   */
  virtual ~Command() {}

  /**
   * \brief Command on timeout reaction
   */
  virtual void OnTimeout() = 0;

  /**
   * @brief Interface method that is called whenever new event received
   * @param event The received event
   */
  virtual void on_event(
      const rc_event_engine::Event<application_manager::MessagePtr,
                                   std::string>& event) {}

 protected:
  Command(RemotePluginInterface& rc_module) : rc_module_(rc_module) {}

  RemotePluginInterface& rc_module_;
};

}  // namespace commands

}  // namespace remote_control

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_COMMANDS_COMMAND_H_

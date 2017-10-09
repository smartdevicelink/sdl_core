/*
 Copyright (c) 2013, Ford Motor Company
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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_NOTIFICATION_IMPL_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_NOTIFICATION_IMPL_H_

#include "application_manager/commands/command_impl.h"
#include "smart_objects/smart_object.h"

namespace application_manager {

namespace commands {

class CommandNotificationImpl : public CommandImpl {
 public:
  CommandNotificationImpl(const MessageSharedPtr& message,
                          ApplicationManager& application_manager);
  /**
   * @brief CommandNotificationImpl class destructor
   **/
  virtual ~CommandNotificationImpl();

  /**
   * @brief Init required by command resources
   **/
  bool Init() OVERRIDE;

  /**
   * @brief Cleanup all resources used by command
   **/
  bool CleanUp() OVERRIDE;

  /**
   * @brief Execute corresponding command by calling the action on reciever
   **/
  void Run() OVERRIDE;

  /**
   * @brief Sends notification message to Mobile
   */
  void SendNotification();

 private:
  /**
   * @brief Checks message permissions and parameters according to policy table
   * permissions
   */
  bool CheckAllowedParameters(const MessageSharedPtr message) const;

  /**
   * @brief Remove from current message parameters disallowed by policy table
   * @param param_permissions Current message permissions structure
   */
  void RemoveDisallowedParameters(
      const MessageSharedPtr message,
      const CommandParametersPermissions& param_permissions) const;

  DISALLOW_COPY_AND_ASSIGN(CommandNotificationImpl);
};

}  // namespace commands

}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_COMMAND_NOTIFICATION_IMPL_H_

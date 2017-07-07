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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_COMMANDS_BASE_COMMAND_NOTIFICATION_H_
#define SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_COMMANDS_BASE_COMMAND_NOTIFICATION_H_

#include "can_cooperation/commands/command.h"
#include "application_manager/message.h"
#include "application_manager/service.h"
#include "can_cooperation/can_app_extension.h"
#include "can_cooperation/can_module_interface.h"
#include "utils/logger.h"

namespace Json {
class Value;
}

namespace can_cooperation {

namespace commands {

/**
 * @brief Base command class for notifications
 */
class BaseCommandNotification : public Command {
 public:
  /**
   * @brief BaseCommandNotification class constructor
   *
   * @param message Message from mobile
   **/
  BaseCommandNotification(const application_manager::MessagePtr& message,
                          CANModuleInterface& can_module);

  /**
   * @brief BaseCommandNotification class destructor
   */
  virtual ~BaseCommandNotification();

  /**
   * \brief BaseCommandNotification on timeout reaction
   */
  virtual void OnTimeout() {}

  // TODO(KKolodiy): need rename to Execute
  void Run();

 protected:
  application_manager::MessagePtr message() {
    return message_;
  }
  application_manager::ServicePtr service_;

  CANAppExtensionPtr GetAppExtension(
      application_manager::ApplicationSharedPtr app) const;

  /**
   * @brief executes specific logic of children classes
   */
  // TODO(KKolodiy): need rename to Run
  virtual void Execute() = 0;

  /**
   * @brief executes specific message validation logic of children classes
   */
  virtual bool Validate() = 0;

  virtual std::string ModuleType(const Json::Value& message);
  virtual std::vector<std::string> ControlData(const Json::Value& message);

  void NotifyOneApplication(application_manager::MessagePtr message);

 private:
  void NotifyApplications();
  bool CheckPolicy(application_manager::MessagePtr message);
  application_manager::MessagePtr message_;
};

}  // namespace commands

}  // namespace can_cooperation

#endif  // SRC_COMPONENTS_CAN_COOPERATION_INCLUDE_CAN_COOPERATION_COMMANDS_BASE_COMMAND_NOTIFICATION_H_

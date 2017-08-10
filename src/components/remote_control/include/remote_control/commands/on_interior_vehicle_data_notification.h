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

#ifndef SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_COMMANDS_ON_INTERIOR_VEHICLE_DATA_NOTIFICATION_H_
#define SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_COMMANDS_ON_INTERIOR_VEHICLE_DATA_NOTIFICATION_H_

#include "utils/macro.h"
#include "remote_control/commands/base_command_notification.h"

namespace remote_control {

namespace commands {

/**
 * @brief OnInteriorVehicleDataNotification command class
 */
class OnInteriorVehicleDataNotification : public BaseCommandNotification {
 public:
  /**
   * @brief OnInteriorVehicleDataNotification class constructor
   *
   * @param message Message with notification
   **/
  OnInteriorVehicleDataNotification(
      const application_manager::MessagePtr& message,
      RemotePluginInterface& rc_module);

  /**
   * @brief Execute command
   */
  void Execute() FINAL;

  /**
   * @brief OnInteriorVehicleDataNotification class destructor
   */
  virtual ~OnInteriorVehicleDataNotification();

 protected:
  std::string ModuleType(const Json::Value& message) FINAL;
};

}  // namespace commands

}  // namespace remote_control

#endif  // SRC_COMPONENTS_REMOTE_CONTROL_INCLUDE_REMOTE_CONTROL_COMMANDS_ON_INTERIOR_VEHICLE_DATA_NOTIFICATION_H_

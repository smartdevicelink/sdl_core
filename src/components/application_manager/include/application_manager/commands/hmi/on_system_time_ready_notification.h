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

#ifndef SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_ON_SYSTEM_TIME_READY_NOTIFICATION_H_
#define SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_ON_SYSTEM_TIME_READY_NOTIFICATION_H_

#include "application_manager/commands/hmi/notification_from_hmi.h"
#include "application_manager/application_manager_impl.h"

namespace application_manager {

namespace commands {

/**
 * @brief OnSystemTimeReadyNotification command class.
 * Notifies SDL whenever system time module is ready.
 * It could be GPS or any other module which is allows
 * to obtain system time. Once SDL receive this notification
 * it is allowed to use GetSystemTimeRequest to rerieve system time.
 */
class OnSystemTimeReadyNotification : public NotificationFromHMI {
 public:
  /**
   * @brief OnSystemTimeReadyNotification create the command.
   * @param message content of the command. Passed directy to base class.
   */
  OnSystemTimeReadyNotification(const MessageSharedPtr& message,
                                ApplicationManager& application_manager);

  /**
   * @brief ~OnSystemTimeReadyNotification destroys the command object.
   */
  ~OnSystemTimeReadyNotification();

  /**
   * @brief Run creates SystemTimeReady event
   * and notifies all the subscribers.
   */
  void Run() FINAL;
};

}  // namespace commands
}  // namespace application_manager

#endif  // SRC_COMPONENTS_APPLICATION_MANAGER_INCLUDE_APPLICATION_MANAGER_COMMANDS_HMI_ON_SYSTEM_TIME_READY_NOTIFICATION_H_

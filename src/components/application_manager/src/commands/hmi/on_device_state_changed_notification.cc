/**
 * Copyright (c) 2013, Ford Motor Company
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

#include "application_manager/commands/hmi/on_device_state_changed_notification.h"
#include "application_manager/policies/policy_handler.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"

namespace application_manager {

namespace commands {

OnDeviceStateChangedNotification::OnDeviceStateChangedNotification(
  const MessageSharedPtr& message)
  : NotificationFromHMI(message) {
}

OnDeviceStateChangedNotification::~OnDeviceStateChangedNotification() {
}

void OnDeviceStateChangedNotification::Run() {
  LOG4CXX_INFO(logger_, "OnDeviceStateChangedNotification::Run");

  if ((*message_)[strings::msg_params]["deviceState"]
      == hmi_apis::Common_DeviceState::UNPAIRED) {
    std::string device_id = (*message_)[strings::msg_params]["deviceInternalId"]
                            .asString();
    if (device_id.empty()) {
      if ((*message_)[strings::msg_params].keyExists("deviceId")) {
        device_id = MessageHelper::GetDeviceMacAddressForHandle(
                      (*message_)[strings::msg_params]["deviceId"]["id"].asInt());
      }
    }
    policy::PolicyHandler::instance()->RemoveDevice(device_id);
  }
}

}  // namespace commands

}  // namespace application_manager


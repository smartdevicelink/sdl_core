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

#include <algorithm>
#include "sdl_rpc_plugin/commands/hmi/on_device_state_changed_notification.h"
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "interfaces/HMI_API.h"
#include "encryption/hashing.h"

namespace {

/**
 * @brief Converts internal device id in format XXXXXXXXXXXX into
 * xx:xx:xx:xx:xx:xx as such format of mac address is required for calculating
 * device hash ID used in the policy table
 * @param internal device id in format XXXXXXXXXXXX
 * @return converted device mac address in format xx:xx:xx:xx:xx:xx
 */
std::string convert_to_bt_mac(const std::string& device_internal_id) {
  std::stringstream bt_mac;
  const size_t octet_hex_length = 2;
  for (size_t i = 0; i < device_internal_id.length(); i += octet_hex_length) {
    if (i > 0) {
      bt_mac << ':';
    }
    if ('0' != device_internal_id[i]) {
      bt_mac << static_cast<char>(::tolower(device_internal_id[i]));
    }
    bt_mac << static_cast<char>(::tolower(device_internal_id[i + 1]));
  }
  return bt_mac.str();
}
}  // namespace

namespace sdl_rpc_plugin {
using namespace application_manager;

namespace commands {

OnDeviceStateChangedNotification::OnDeviceStateChangedNotification(
    const application_manager::commands::MessageSharedPtr& message,
    ApplicationManager& application_manager,
    rpc_service::RPCService& rpc_service,
    HMICapabilities& hmi_capabilities,
    policy::PolicyHandlerInterface& policy_handle)
    : NotificationFromHMI(message,
                          application_manager,
                          rpc_service,
                          hmi_capabilities,
                          policy_handle) {}

OnDeviceStateChangedNotification::~OnDeviceStateChangedNotification() {}

void OnDeviceStateChangedNotification::Run() {
  LOG4CXX_AUTO_TRACE(logger_);

  if ((*message_)[strings::msg_params]["deviceState"] ==
      hmi_apis::Common_DeviceState::UNPAIRED) {
    // It is expected, that "deviceInternalId" is the device MAC address in
    // form XXXXXXXXXX
    std::string device_id =
        (*message_)[strings::msg_params]["deviceInternalId"].asString();
    if (device_id.empty()) {
      if ((*message_)[strings::msg_params].keyExists("deviceId")) {
        device_id =
            (*message_)[strings::msg_params]["deviceId"]["id"].asString();
      }
    } else {
      // Policy uses hashed MAC address as device_id
      LOG4CXX_DEBUG(logger_, "Device_id from HMI: " << device_id);
      std::string bt_mac = convert_to_bt_mac(device_id);
      LOG4CXX_DEBUG(logger_, "Device_id as BT MAC: " << bt_mac);
      device_id = encryption::MakeHash(bt_mac);
      LOG4CXX_DEBUG(logger_, "Device_id hashed as BT MAC : " << device_id);
    }
    policy_handler_.RemoveDevice(device_id);
  }
}

}  // namespace commands

}  // namespace application_manager

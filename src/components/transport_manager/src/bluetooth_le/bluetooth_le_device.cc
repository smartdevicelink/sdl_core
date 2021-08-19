/*
 *
 * Copyright (c) 2021, Ford Motor Company
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

#include "transport_manager/bluetooth_le/bluetooth_le_device.h"
#include "transport_manager/common.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {
SDL_CREATE_LOG_VARIABLE("TransportManager")

const ApplicationHandle kDefaultAppHandle = 1u;

  /**
   * @brief Constructor.
   *
   * @param device_address Bluetooth address.
   * @param device_name Human-readable device name.
   **/
  BluetoothLeDevice::BluetoothLeDevice(const std::string& device_address,
                  const char* device_name)
    : Device(device_name, device_address)
    , address_(device_address)
    , applications_list_({kDefaultAppHandle}) { }

  bool BluetoothLeDevice::IsSameAs(const Device* other) const {
      SDL_LOG_AUTO_TRACE();
      const BluetoothLeDevice* other_bluetooth_device =
      dynamic_cast<const BluetoothLeDevice*>(other);
      if(other_bluetooth_device) {
        return other_bluetooth_device->address_ == address_;
      }

      SDL_LOG_TRACE("Compare with no BLE device !");

      return false;
  }

  ApplicationList BluetoothLeDevice::GetApplicationList() const {
      return applications_list_;
  }

}
}

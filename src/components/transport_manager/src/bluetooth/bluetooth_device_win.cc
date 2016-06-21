/*
 *
 * Copyright (c) 2016, Ford Motor Company
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

#include "transport_manager/bluetooth/bluetooth_device.h"
#include "utils/bluetooth/bluetooth_utils_win.h"

#include <errno.h>
#include <sys/types.h>
#include <algorithm>
#include <limits>

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {
SDL_CREATE_LOGGER("TransportManager")

bool BluetoothDevice::GetRfcommChannel(const ApplicationHandle app_handle,
                                       uint8_t* channel_out) {
  SDL_TRACE("enter. app_handle: " << app_handle
                                  << ", channel_out: " << channel_out);
  if (app_handle < 0 || app_handle > std::numeric_limits<uint8_t>::max()) {
    SDL_TRACE(
        "exit with FALSE. Condition: app_handle < 0 || app_handle > "
        "numeric_limits::max()");
    return false;
  }
  const uint8_t channel = static_cast<uint8_t>(app_handle);
  RfcommChannelVector::const_iterator it =
      std::find(rfcomm_channels_.begin(), rfcomm_channels_.end(), channel);
  if (it == rfcomm_channels_.end()) {
    SDL_TRACE(
        "exit with FALSE. Condition: channel not found in RfcommChannelVector");
    return false;
  }
  *channel_out = channel;
  SDL_TRACE("exit with TRUE");
  return true;
}

std::string BluetoothDevice::GetUniqueDeviceId(
    const BLUETOOTH_ADDR_INFO& device_address) {
  SDL_TRACE(
      "enter. device_adress: " << utils::BthDeviceAddrToStr(device_address));
  char device_address_string[32];
  sprintf(device_address_string, "%ws", device_address.szName);
  SDL_TRACE("exit with BT-" << device_address_string);
  return std::string("BT-") + device_address_string;
}

BluetoothDevice::BluetoothDevice(const BLUETOOTH_ADDR_INFO& device_address,
                                 const char* device_name,
                                 const RfcommChannelVector& rfcomm_channels,
                                 const SOCKADDR_BTH& sock_addr_bth_server)
    : Device(device_name, GetUniqueDeviceId(device_address))
    , address_(device_address)
    , rfcomm_channels_(rfcomm_channels)
    , sock_addr_bth_server_(sock_addr_bth_server) {}

bool BluetoothDevice::IsSameAs(const Device* other) const {
  SDL_TRACE("enter. device: " << other);
  bool result = false;

  const BluetoothDevice* other_bluetooth_device =
      dynamic_cast<const BluetoothDevice*>(other);

  if (0 != other_bluetooth_device) {
    if (0 == memcmp(&address_,
                    &other_bluetooth_device->address_,
                    sizeof(BTH_ADDR))) {
      result = true;
    }
  }
  if (result) {
    SDL_TRACE("exit with TRUE");
  } else {
    SDL_TRACE("exit with FALSE");
  }
  return result;
}

SOCKADDR_BTH BluetoothDevice::getSocketBthAddr() {
  return sock_addr_bth_server_;
}

ApplicationList BluetoothDevice::GetApplicationList() const {
  return ApplicationList(rfcomm_channels_.begin(), rfcomm_channels_.end());
}

}  // namespace transport_adapter
}  // namespace transport_manager

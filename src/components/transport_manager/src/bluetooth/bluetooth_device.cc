/**
 * \file BluetoothAdapter.cpp
 * \brief BluetoothAdapter class source file.
 *
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

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>

#include <iomanip>
#include <set>
#include <bluetooth/bluetooth.h>

#include "transport_manager/bluetooth/bluetooth_device.h"

namespace transport_manager {
namespace device_adapter {

bool BluetoothDevice::GetRfcommChannel(const ApplicationHandle app_handle,
                                       uint8_t* channel_out) {
  RfcommChannels::const_iterator it = rfcomm_channels_.find(app_handle);
  if (it != rfcomm_channels_.end()) {
    *channel_out = it->second;
    return true;
  } else {
    return false;
  }
}

std::string BluetoothDevice::GetUniqueDeviceId(const bdaddr_t& device_address) {
  char device_address_string[32];
  ba2str(&device_address, device_address_string);
  return std::string("BT-") + device_address_string;
}

BluetoothDevice::BluetoothDevice(const bdaddr_t& address, const char* name,
                                 const RfcommChannelVector& rfcomm_channels)
    : Device(name, GetUniqueDeviceId(address)),
      address_(address),
      next_application_handle_(1) {
  for (RfcommChannelVector::const_iterator it = rfcomm_channels.begin();
      it != rfcomm_channels.end(); ++it) {
    rfcomm_channels_[next_application_handle_++] = *it;
  }
}

bool BluetoothDevice::IsSameAs(const Device* other) const {
  bool result = false;

  const BluetoothDevice* other_bluetooth_device =
      dynamic_cast<const BluetoothDevice*>(other);

  if (0 != other_bluetooth_device) {
    if (0
        == memcmp(&address_, &other_bluetooth_device->address_,
                  sizeof(bdaddr_t))) {
      result = true;
    }
  }

  return result;
}

ApplicationList BluetoothDevice::GetApplicationList() const {
  ApplicationList result;
  for (RfcommChannels::const_iterator it = rfcomm_channels_.begin();
      it != rfcomm_channels_.end(); ++it)
    result.push_back(it->first);
  return result;
}

}  // namespace device_adapter
}  // namespace transport_manager


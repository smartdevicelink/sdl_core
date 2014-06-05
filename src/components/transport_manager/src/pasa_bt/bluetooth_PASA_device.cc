/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "transport_manager/pasa_bt/bluetooth_PASA_device.h"

#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <limits>
#include <sstream>

namespace transport_manager {
namespace transport_adapter {

BluetoothPASADevice::SCOMMChannel::SCOMMChannel():mSppQueueName("") {
}

BluetoothPASADevice::SCOMMChannel::SCOMMChannel(const char *sppQue):
  mSppQueueName(sppQue) {
}

BluetoothPASADevice::BluetoothPASADevice(const char* name, const UI_8 (&mac)[6])
  : Device(name, MacToString(mac)),
    applications_(),
    last_app_handle_(0) {
  memcpy(mac_, mac, sizeof(mac));
}

bool BluetoothPASADevice::IsSameAs(const Device* other) const {
  const BluetoothPASADevice* other_bluetooth_device =
      dynamic_cast<const BluetoothPASADevice*>(other);
  if (other_bluetooth_device) {
    if (unique_device_id() != other_bluetooth_device->unique_device_id()) {
      return true;
    }
  }
  return false;
}

// Todd: BT Support
std::string BluetoothPASADevice::GetSppQName(ApplicationHandle app_handle) const {
  sync_primitives::AutoLock lock(applications_lock_);
  for (Applications::const_iterator i = applications_.begin(); i != applications_.end(); ++i) {
    if (i->first == app_handle) {
      return i->second.mSppQueueName;
    }
  }
  return std::string();
}

void BluetoothPASADevice::AddChannel(const SCOMMChannel& channel) {
  sync_primitives::AutoLock lock(applications_lock_);
  for (Applications::const_iterator i = applications_.begin(); i != applications_.end(); ++i) {
    if (i->second == channel) {
      applications_.push_back(std::make_pair(++last_app_handle_, channel));
      break;
    }
  }
}

bool BluetoothPASADevice::RemoveChannel(const SCOMMChannel& channel, ApplicationHandle* app_handle) {
  sync_primitives::AutoLock lock(applications_lock_);
  for (Applications::iterator i = applications_.begin(); i != applications_.end(); ++i) {
    if (i->second == channel) {
      if (app_handle) {
        *app_handle = i->first;
      }
      applications_.erase(i);
      return true;
    }
  }
  return false;
}

ApplicationList BluetoothPASADevice::GetApplicationList() const {
  ApplicationList result;
  sync_primitives::AutoLock lock(applications_lock_);
  for (Applications::const_iterator i = applications_.begin(); i != applications_.end(); ++i) {
    result.push_back(i->first);
  }
  return result;
}

std::string MacToString(const UI_8 (&mac)[6]) {
  std::ostringstream oss;
  oss << std::hex;
  for (int i = 0; ; ++i) {
    oss << static_cast<int>(mac[i]);
    if (i == (sizeof(mac) / sizeof(mac[0]) - 1)) break;
    oss << ":";
  }
  return oss.str();
}
}  // namespace transport_adapter
}  // namespace transport_manager


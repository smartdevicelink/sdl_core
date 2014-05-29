/**
 * \file bluetooth_device.cc
 * \brief BluetoothDevice class source file.
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

#include "transport_manager/pasa_bt/bluetooth_device.h"

#include <errno.h>
#include <string.h>
#include <sys/types.h>
//#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <limits>
#include <sstream>

namespace transport_manager {
namespace transport_adapter {

// Todd: BT Support
#if 0
bool BluetoothDevice::GetRfcommChannel(const ApplicationHandle app_handle,
                                       uint8_t* channel_out) {
  if (app_handle < 0 || app_handle > std::numeric_limits<uint8_t>::max())
    return false;
  const uint8_t channel = static_cast<uint8_t>(app_handle);
  RfcommChannelVector::const_iterator it = std::find(rfcomm_channels_.begin(),
                                                     rfcomm_channels_.end(),
                                                     channel);
  if (it == rfcomm_channels_.end())
    return false;
  *channel_out = channel;
  return true;
}
#endif

// Todd: BT Support
#if 0
std::string BluetoothDevice::GetUniqueDeviceId(const bdaddr_t& device_address) {
  char device_address_string[32];
  ba2str(&device_address, device_address_string);
  return std::string("BT-") + device_address_string;
}
#endif

BluetoothDevice::SCOMMChannel::SCOMMChannel():
mSppQueueName("")
{
}

BluetoothDevice::SCOMMChannel::SCOMMChannel(const char *sppQue):
mSppQueueName(sppQue)
{
}

BluetoothDevice::BluetoothDevice(const char* name, const UI_8 (&mac)[6])
    : Device(name, MacToString(mac)),
      applications_(),
      last_app_handle_(0)
{
    memcpy(mac_, mac, sizeof(mac));
    pthread_mutex_init(&applications_mutex_, 0);
}


BluetoothDevice::~BluetoothDevice() {
    pthread_mutex_destroy(&applications_mutex_);
}

#if 0
BluetoothDevice::BluetoothDevice(const bdaddr_t& device_address, const char* device_name,
                                 const RfcommChannelVector& rfcomm_channels)
    : Device(device_name, GetUniqueDeviceId(device_address)),
      address_(device_address),
      rfcomm_channels_(rfcomm_channels) {
}
#endif

bool BluetoothDevice::IsSameAs(const Device* other) const {
  bool result = false;

  const BluetoothDevice* other_bluetooth_device =
      dynamic_cast<const BluetoothDevice*>(other);

  if (0 != other_bluetooth_device) {
    if (0
        == (unique_device_id() != other_bluetooth_device->unique_device_id())) {
      result = true;
    }
  }

  return result;
}

// Todd: BT Support
std::string BluetoothDevice::GetSppQName(ApplicationHandle app_handle) const {
	std::string result;
	pthread_mutex_lock(&applications_mutex_);
    for (Applications::const_iterator i = applications_.begin(); i != applications_.end(); ++i) {
    	if (i->first == app_handle) {
    		result = i->second.mSppQueueName;
    		break;
    	}
    }
	pthread_mutex_unlock(&applications_mutex_);
    return result;
}

void BluetoothDevice::AddChannel(const SCOMMChannel& channel) {
	bool exists = false;
	pthread_mutex_lock(&applications_mutex_);
    for (Applications::const_iterator i = applications_.begin(); i != applications_.end(); ++i) {
    	if (i->second == channel) {
    		exists = true;
    		break;
    	}
    }
    if (!exists) {
    	applications_.push_back(std::make_pair(++last_app_handle_, channel));
    }
	pthread_mutex_unlock(&applications_mutex_);
}

bool BluetoothDevice::RemoveChannel(const SCOMMChannel& channel, ApplicationHandle* app_handle) {
	bool found = false;
	pthread_mutex_lock(&applications_mutex_);
    for (Applications::iterator i = applications_.begin(); i != applications_.end(); ++i) {
    	if (i->second == channel) {
    		if (app_handle) *app_handle = i->first;
    		applications_.erase(i);
    		found = true;
    		break;
    	}
    }
	pthread_mutex_unlock(&applications_mutex_);
	return found;
}

ApplicationList BluetoothDevice::GetApplicationList() const {
	ApplicationList result;
	pthread_mutex_lock(&applications_mutex_);
    for (Applications::const_iterator i = applications_.begin(); i != applications_.end(); ++i) {
    	result.push_back(i->first);
    }
	pthread_mutex_unlock(&applications_mutex_);
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


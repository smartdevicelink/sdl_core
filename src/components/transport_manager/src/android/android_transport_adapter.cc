/*
 * \file android_transport_adapter.cc
 * \brief AndroidTransportAdapter class source file.
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

#include <errno.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <iomanip>
#include <set>

#include "transport_manager/android/android_connection_factory.h"
#include "transport_manager/android/android_device_scanner.h"
#include "transport_manager/android/android_transport_adapter.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

namespace {
std::string getTransportName(AndroidTransportType transport_type) {
  switch (transport_type) {
    case AndroidTransportType::BLE:
      return "Bluetooth Low Energy";
      break;

    case AndroidTransportType::BT:
      return "Bluetooth";
      break;

    default:
      SDL_LOG_ERROR("Unknown android transport type");
      break;
  }

  return "Unknown";
}
}  // namespace

AndroidTransportAdapter::AndroidTransportAdapter(
    resumption::LastStateWrapperPtr last_state_wrapper,
    const TransportManagerSettings& settings,
    AndroidTransportType transport_type)
    : TransportAdapterImpl(new AndroidDeviceScanner(this),
                           new AndroidConnectionFactory(this),
                           NULL,
                           last_state_wrapper,
                           settings)
    , active_device_uid_()
    , app_handle_(0)
    , transport_type_(transport_type) {
  SDL_LOG_DEBUG("AndroidTransportAdapter created for transport type : "
                << getTransportName(transport_type));
}

DeviceType AndroidTransportAdapter::GetDeviceType() const {
  switch (transport_type_) {
    case AndroidTransportType::BLE:
      return DeviceType::BLUETOOTH_LE;
      break;

    case AndroidTransportType::BT:
      return DeviceType::BLUETOOTH;
      break;
  }

  SDL_LOG_ERROR("Unknown android transport type");
  return DeviceType::UNKNOWN;
}

void AndroidTransportAdapter::Store() const {}

bool AndroidTransportAdapter::Restore() {
  return true;
}

AndroidTransportAdapter::~AndroidTransportAdapter() {
  SDL_LOG_DEBUG("Destroying Android transport adapter");
}

void AndroidTransportAdapter::SearchDeviceDone(const DeviceVector& devices) {
  for (const DeviceSptr& device : devices) {
    if (dynamic_cast<AndroidIpcDevice*>(device.get()) != nullptr) {
      active_device_uid_ = device->unique_device_id();
      SDL_LOG_DEBUG(
          "New active Android Ipc device found: " << active_device_uid_);

      const auto apps_list = device->GetApplicationList();
      if (!apps_list.empty()) {
        app_handle_ = apps_list.front();
        SDL_LOG_DEBUG(
            "New active Android ipc device app handle: " << app_handle_);
      }

      break;
    }
  }

  TransportAdapterImpl::SearchDeviceDone(devices);
}

void AndroidTransportAdapter::DisconnectDone(
    const DeviceUID& device_handle, const ApplicationHandle& app_handle) {
  if (active_device_uid_ == device_handle) {
    const auto disconnect_result =
        TransportAdapterImpl::Disconnect(active_device_uid_, app_handle_);
    if (TransportAdapter::OK == disconnect_result) {
      TransportAdapterImpl::DisconnectDone(active_device_uid_, app_handle_);
      active_device_uid_.clear();
      app_handle_ = 0;
    }

    return;
  }

  TransportAdapterImpl::DisconnectDone(device_handle, app_handle);
}

bool AndroidTransportAdapter::ToBeAutoConnected(DeviceSptr device) const {
  if (!active_device_uid_.empty()) {
    // Android Ipc device connection is established on the Java side
    return device->unique_device_id() == active_device_uid_;
  }

  return false;
}

std::string AndroidTransportAdapter::GetSenderSocketName() const {
  switch (transport_type_) {
    case AndroidTransportType::BLE:
      return "./localBleReader";
      break;

    case AndroidTransportType::BT:
      return "./localBtReader";
      break;
  }

  return "";
}

std::string AndroidTransportAdapter::GetReceiverSocketName() const {
  switch (transport_type_) {
    case AndroidTransportType::BLE:
      return "./localBleWriter";
      break;

    case AndroidTransportType::BT:
      return "./localBtWriter";
      break;
  }

  return "";
}

std::string AndroidTransportAdapter::GetControlReceiverSocketName() const {
  switch (transport_type_) {
    case AndroidTransportType::BLE:
      return "./localBleControl";
      break;

    case AndroidTransportType::BT:
      return "./localBtControl";
      break;
  }
  return "";
}

}  // namespace transport_adapter
}  // namespace transport_manager

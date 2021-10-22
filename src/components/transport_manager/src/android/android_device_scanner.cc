/*
 * \file android_device_scanner.cc
 * \brief AndroidDeviceScanner class header file.
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

#include "transport_manager/android/android_device_scanner.h"

#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include <vector>

#include "transport_manager/android/android_ipc_control_protocol.h"
#include "transport_manager/android/android_ipc_device.h"
#include "transport_manager/android/android_transport_adapter.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

AndroidDeviceScanner::AndroidDeviceScanner(AndroidTransportAdapter* controller)
    : controller_(controller)
    , ipc_control_receiver_(new LocalSocketReceiver(
          std::bind(&AndroidDeviceScanner::ProcessMessage,
                    this,
                    std::placeholders::_1),
          LocalSocketReceiver::ChannelNameGetter(
              [this]() { return controller_->GetTransportName(); }))) {
  ipc_control_receiver_thread_ = std::thread([&]() {
    ipc_control_receiver_->Init(
        static_cast<AndroidTransportAdapter*>(controller_)
            ->GetControlReceiverSocketName());
    ipc_control_receiver_->Run();
  });
}

AndroidDeviceScanner::~AndroidDeviceScanner() {
  Terminate();
}

void AndroidDeviceScanner::UpdateTotalDeviceList() {
  SDL_LOG_AUTO_TRACE();
  DeviceVector devices;
  devices.insert(devices.end(),
                 found_devices_with_sdl_.begin(),
                 found_devices_with_sdl_.end());
  controller_->SearchDeviceDone(devices);
}

void AndroidDeviceScanner::Terminate() {
  SDL_LOG_AUTO_TRACE();
  ipc_control_receiver_->Stop();
  if (ipc_control_receiver_thread_.joinable()) {
    ipc_control_receiver_thread_.join();
  }
}

TransportAdapter::Error AndroidDeviceScanner::Init() {
  return TransportAdapter::OK;
}

TransportAdapter::Error AndroidDeviceScanner::Scan() {
  return ipc_control_receiver_thread_.joinable() ? TransportAdapter::OK
                                                 : TransportAdapter::FAIL;
}

bool AndroidDeviceScanner::IsInitialised() const {
  return ipc_control_receiver_thread_.joinable();
}

void AndroidDeviceScanner::ProcessMessage(const std::vector<uint8_t>& data) {
  if (data.size() > 0) {
    SDL_LOG_DEBUG("Control message:  " << data.data() << " "
                                       << controller_->GetTransportName());

    const auto action = AndroidIpcControlProtocol::GetMessageActionType(data);

    switch (action) {
      case AndroidIpcProtocolActions::ON_DEVICE_CONNECTED: {
        DeviceSptr device_ptr(new AndroidIpcDevice(
            AndroidIpcControlProtocol::GetAddress(data),
            AndroidIpcControlProtocol::GetName(data).c_str()));

        found_devices_with_sdl_.push_back(device_ptr);

        UpdateTotalDeviceList();
      } break;

      case AndroidIpcProtocolActions::ON_DEVICE_DISCONNECTED: {
        const auto addr = AndroidIpcControlProtocol::GetAddress(data);
        auto it_device =
            std::find_if(found_devices_with_sdl_.begin(),
                         found_devices_with_sdl_.end(),
                         [&addr](DeviceSptr d) {
                           AndroidIpcDevice tDevice(addr, "");
                           return d->IsSameAs(static_cast<Device*>(&tDevice));
                         });

        if (it_device != found_devices_with_sdl_.end()) {
          DeviceSptr device = *it_device;
          found_devices_with_sdl_.erase(it_device);
          controller_->DisconnectDone(device->unique_device_id(), 0);
        }
      } break;

      default:
        SDL_LOG_ERROR("Undefined BLE action " << static_cast<uint32_t>(action));
        break;
    }

  } else {
    SDL_LOG_DEBUG("Control message fail " << controller_->GetTransportName());
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager

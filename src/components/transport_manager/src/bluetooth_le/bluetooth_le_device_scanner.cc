/*
 * \file bluetooth_le_device_scanner.cc
 * \brief BluetoothLeDeviceScanner class header file.
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

#include "transport_manager/bluetooth_le/bluetooth_le_device_scanner.h"

#include <errno.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sstream>
#include <vector>

#include "transport_manager/bluetooth_le/bluetooth_le_device.h"
#include "transport_manager/bluetooth_le/bluetooth_le_transport_adapter.h"
#include "transport_manager/bluetooth_le/ble_control_protocol.h"

#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

BluetoothLeDeviceScanner::BluetoothLeDeviceScanner(
    TransportAdapterController* controller)
    : controller_(controller)
    , ble_control_server_( BleServer::ControlSocketName,
     std::bind(&BluetoothLeDeviceScanner::ProcessMessage, this, std::placeholders::_1)) {

  ble_control_server_thread_ = std::thread([&]() {
    ble_control_server_.Init();
    ble_control_server_.Run();
    });
}

BluetoothLeDeviceScanner::~BluetoothLeDeviceScanner() {
  Terminate();
}

void BluetoothLeDeviceScanner::UpdateTotalDeviceList() {
  SDL_LOG_AUTO_TRACE();
  DeviceVector devices;
  devices.insert(devices.end(),
                 found_devices_with_sdl_.begin(),
                 found_devices_with_sdl_.end());
  controller_->SearchDeviceDone(devices);
}

void BluetoothLeDeviceScanner::Terminate() {
  SDL_LOG_AUTO_TRACE();
  ble_control_server_.Stop();
  if(ble_control_server_thread_.joinable()) {
    ble_control_server_thread_.join();
  }
}

TransportAdapter::Error BluetoothLeDeviceScanner::Init() {
  return TransportAdapter::OK;
}

TransportAdapter::Error BluetoothLeDeviceScanner::Scan() {
  return ble_control_server_thread_.joinable() ? TransportAdapter::OK : TransportAdapter::FAIL;
}

bool BluetoothLeDeviceScanner::IsInitialised() const {
  return ble_control_server_thread_.joinable();
}

void BluetoothLeDeviceScanner::ProcessMessage(const std::vector<uint8_t>& data) {
  if (data.size() > 0) {
    SDL_LOG_DEBUG("Control message:  " << data.data());

    const auto action = BleControlProtocol::GetMessageActionType(data);

    switch(action){
      case BleProtocolActions::ON_DEVICE_CONNECTED:
      {
        DeviceSptr device_ptr(new BluetoothLeDevice(
            BleControlProtocol::GetAddress(data),
            BleControlProtocol::GetName(data).c_str() ));

        found_devices_with_sdl_.push_back(device_ptr);

        UpdateTotalDeviceList();
      }
      break;

      case BleProtocolActions::ON_DEVICE_DISCONNECTED:
      {
        const auto addr = BleControlProtocol::GetAddress(data);
        auto it_device = std::find_if(found_devices_with_sdl_.begin(),
                                      found_devices_with_sdl_.end(), [&addr](DeviceSptr d){
          BluetoothLeDevice tDevice(addr, "");
          return d->IsSameAs(static_cast<Device*>(&tDevice));
        });

        if (it_device != found_devices_with_sdl_.end()) {
          DeviceSptr device = *it_device;
          found_devices_with_sdl_.erase(it_device);
          controller_->DisconnectDone(device->unique_device_id(), 0);
        }
      }
      break;

      default:
        SDL_LOG_ERROR("Undefined BLE action " << static_cast<uint32_t>(action));
      break;
    }

  } else {
    SDL_LOG_DEBUG("Control message fail");
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager

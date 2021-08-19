/*
 * \file bluetooth_le_device_scanner.h
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

#ifndef SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_LE_BLUETOOTH_LE_DEVICE_SCANNER_H_
#define SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_LE_BLUETOOTH_LE_DEVICE_SCANNER_H_

#include "transport_manager/bluetooth_le/bluetooth_le_device.h"
#include "transport_manager/transport_adapter/device_scanner.h"
#include "transport_manager/bluetooth_le/ble_server.h"

#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/threads/thread_delegate.h"

class Thread;

namespace transport_manager {
namespace transport_adapter {
class TransportAdapterController;

/**
 * @brief Scan for devices using bluetooth LE.
 */
class BluetoothLeDeviceScanner : public DeviceScanner {
 public:
  /**
   * @brief Constructor.
   * @param controller Transport adapter controller
   */
  BluetoothLeDeviceScanner(TransportAdapterController* controller);
  /**
   * @brief Destructor.
   */
  ~BluetoothLeDeviceScanner();

 protected:

  void Terminate() override;

  TransportAdapter::Error Init() override;

  TransportAdapter::Error Scan() override;

  bool IsInitialised() const override;

 private:
  /**
   * @brief Summarizes the total list of devices (paired and scanned) and
   * notifies controller
   */
  void UpdateTotalDeviceList();

  /**
   * @brief Processing messages received from ble_control_server_
   * @param data Message data in json format
   */
  void ProcessMessage(const std::vector<uint8_t>& data);

  TransportAdapterController* controller_;

  DeviceVector found_devices_with_sdl_;

  BleServer ble_control_server_;
  std::thread ble_control_server_thread_;
};

}  // namespace transport_adapter
}  // namespace transport_manager
#endif  // SRC_COMPONENTS_TRANSPORT_MANAGER_INCLUDE_TRANSPORT_MANAGER_BLUETOOTH_BLUETOOTH_DEVICE_SCANNER_H_

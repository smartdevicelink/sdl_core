/*
 * Copyright (c) 2017, Ford Motor Company
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

#pragma once

#include "transport_manager/tcp/tcp_transport_adapter.h"
#include "transport_manager/transport_manager_settings.h"
#include "resumption/last_state.h"
#include "utils/macro.h"

namespace transport_manager {
namespace transport_adapter {

/**
 * @brief The IAP2BluetoothEmulationTransportAdapter class implements iAP2
 * Bluetooth transport over TCP
 */
class IAP2BluetoothEmulationTransportAdapter : public TcpTransportAdapter {
 public:
  /**
   * @brief IAP2BluetoothEmulationTransportAdapter constructor
   * @param port TCP port to listen on
   * @param last_state LastState instance reference
   * @param settings Settings reference
   */
  IAP2BluetoothEmulationTransportAdapter(
      const uint16_t port,
      resumption::LastState& last_state,
      const TransportManagerSettings& settings);

  /**
   * @brief DeviceSwitched is called during switching from iAP2 Bluetooth to
   * iAP2 USB transport.
   * Currently there is no logic to switch back from iAP2 USB to iAP2 Bluetooth
   * due to Bluetooth reconnection takes much time and this violates some Apple
   * restrictions for iAP2 protocol, so if this method will be called - assert
   * will be called
   * @param device_handle Device handle of switched device
   */
  void DeviceSwitched(const DeviceUID& device_handle) OVERRIDE;

 protected:
  /**
   * @brief GetDeviceType Provides SDL device type for transport adapter
   * @return Device type
   */
  DeviceType GetDeviceType() const OVERRIDE;
};

/**
 * @brief The IAP2USBEmulationTransportAdapter class implements emulation of
 * iAP2 USB transport over TCP
 */
class IAP2USBEmulationTransportAdapter : public TcpTransportAdapter {
 public:
  /**
   * @brief IAP2USBEmulationTransportAdapter constructor
   * @param port TCP port to listen on
   * @param last_state LastState instance reference
   * @param settings Settings reference
   */
  IAP2USBEmulationTransportAdapter(const uint16_t port,
                                   resumption::LastState& last_state,
                                   const TransportManagerSettings& settings);

  /**
   * @brief DeviceSwitched is called during switching from iAP2 Bluetooth to
   * iAP2 USB transport
   * @param device_handle Device handle of switched device
   */
  void DeviceSwitched(const DeviceUID& device_handle) OVERRIDE;

 protected:
  /**
   * @brief GetDeviceType Provides SDL device type for transport adapter
   * @return Device type
   */
  DeviceType GetDeviceType() const OVERRIDE;
};
}
}  // namespace transport_manager::transport_adapter

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

#include "transport_manager/iap2_emulation/iap2_transport_adapter.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "IAP2Emulation");

IAP2BluetoothEmulationTransportAdapter::IAP2BluetoothEmulationTransportAdapter(
    const uint16_t port,
    resumption::LastState& last_state,
    const TransportManagerSettings& settings)
    : TcpTransportAdapter(port, last_state, settings) {}

void IAP2BluetoothEmulationTransportAdapter::DeviceSwitched(
    const DeviceUID& device_handle) {
  LOG4CXX_AUTO_TRACE(logger_);
  UNUSED(device_handle);
  DCHECK(!"Switching for iAP2 Bluetooth is not supported.");
}

DeviceType IAP2BluetoothEmulationTransportAdapter::GetDeviceType() const {
  return IOS_BT;
}

IAP2USBEmulationTransportAdapter::IAP2USBEmulationTransportAdapter(
    const uint16_t port,
    resumption::LastState& last_state,
    const TransportManagerSettings& settings)
    : TcpTransportAdapter(port, last_state, settings) {}

void IAP2USBEmulationTransportAdapter::DeviceSwitched(
    const DeviceUID& device_handle) {
  LOG4CXX_AUTO_TRACE(logger_);
  UNUSED(device_handle);
  LOG4CXX_DEBUG(logger_, "iAP2 USB device is switched with iAP2 Bluetooth");
}

DeviceType IAP2USBEmulationTransportAdapter::GetDeviceType() const {
  return IOS_USB;
}
}
}  // namespace transport_manager::transport_adapter

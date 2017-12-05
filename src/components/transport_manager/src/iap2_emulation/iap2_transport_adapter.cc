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

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>

#include "utils/threads/thread.h"
#include "utils/file_system.h"

namespace  {
static const mode_t mode = 0666;
static const auto in_signals_channel = "iap_signals_in";
static const auto out_signals_channel = "iap_signals_out";
} // namespace

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
    : TcpTransportAdapter(port, last_state, settings), out_(0) {
  auto delegate = new IAPSignalHandlerDelegate(*this);
  signal_handler_ =
      threads::CreateThread("iAP signal handler", delegate);
  signal_handler_->start();
  const auto result = mkfifo(out_signals_channel, mode);
  LOG4CXX_DEBUG(logger_, "Out signals channel creation result: " << result);
}

IAP2USBEmulationTransportAdapter::~IAP2USBEmulationTransportAdapter() {
  signal_handler_->join();
  threads::DeleteThread(signal_handler_);  
  LOG4CXX_DEBUG(logger_, "Out close result: " << close(out_));
  LOG4CXX_DEBUG(logger_, "Out unlink result: " << unlink(out_signals_channel));
}

void IAP2USBEmulationTransportAdapter::DeviceSwitched(
    const DeviceUID& device_handle) {
  LOG4CXX_AUTO_TRACE(logger_);
  UNUSED(device_handle);
  const auto switch_signal_ack = std::string("SDL_TRANSPORT_SWITCH_ACK\n");

  auto out_ = open(out_signals_channel, O_WRONLY);
  LOG4CXX_DEBUG(logger_, "Out channel descriptor: " << out_);

  const auto bytes =
      write(out_, switch_signal_ack.c_str(), switch_signal_ack.size());
  LOG4CXX_DEBUG(logger_, "Written bytes to out: " << bytes);

  LOG4CXX_DEBUG(logger_, "Switching signal ACK is sent");
  LOG4CXX_DEBUG(logger_, "iAP2 USB device is switched with iAP2 Bluetooth");
}

DeviceType IAP2USBEmulationTransportAdapter::GetDeviceType() const {
  return IOS_USB;
}

IAP2USBEmulationTransportAdapter::
IAPSignalHandlerDelegate::IAPSignalHandlerDelegate(
    IAP2USBEmulationTransportAdapter& adapter)
  : adapter_(adapter),
    run_flag_(true),
    in_(0) {
  const auto result = mkfifo(in_signals_channel, mode);
  LOG4CXX_DEBUG(logger_, "In signals channel creation result: " << result);
}

void IAP2USBEmulationTransportAdapter::IAPSignalHandlerDelegate::threadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Signal handling is started");
  const auto switch_signal = "SDL_TRANSPORT_SWITCH";
  LOG4CXX_DEBUG(logger_, "Waiting for signal: " << switch_signal);

  in_ = open(in_signals_channel, O_RDONLY);
  LOG4CXX_DEBUG(logger_, "In channel descriptor: " << in_);

  const auto size = 32;
  while (run_flag_) {
    char buffer[size];
    auto bytes = read(in_, &buffer, size);
    if (!bytes) {
      continue;
    }
    LOG4CXX_DEBUG(logger_, "Read in bytes: " << bytes);
    std::string str(buffer);
    if (std::string::npos != str.find(switch_signal)) {
      LOG4CXX_DEBUG(logger_, "Switch signal received.");
      adapter_.DoTransportSwitch();
    }
  }
}

void IAP2USBEmulationTransportAdapter::
IAPSignalHandlerDelegate::exitThreadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "Stopping signal handling.");
  run_flag_ = false;
  LOG4CXX_DEBUG(logger_, "In close result: " << close(in_));
  LOG4CXX_DEBUG(logger_, "In unlink result: " << unlink(in_signals_channel));
}

}
}  // namespace transport_manager::transport_adapter

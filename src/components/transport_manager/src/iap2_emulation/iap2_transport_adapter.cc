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

#include <fcntl.h>
#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "utils/file_system.h"
#include "utils/threads/thread.h"

namespace {
static const mode_t mode = 0666;
static const auto in_signals_channel = "iap_signals_in";
static const auto out_signals_channel = "iap_signals_out";
}  // namespace

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("IAP2Emulation");

IAP2BluetoothEmulationTransportAdapter::IAP2BluetoothEmulationTransportAdapter(
    const uint16_t port,
    resumption::LastStateWrapperPtr last_state_wrapper,
    const TransportManagerSettings& settings)
    : TcpTransportAdapter(port, last_state_wrapper, settings) {}

void IAP2BluetoothEmulationTransportAdapter::DeviceSwitched(
    const DeviceUID& device_handle) {
  SDL_LOG_AUTO_TRACE();
  UNUSED(device_handle);
  DCHECK(!"Switching for iAP2 Bluetooth is not supported.");
}

DeviceType IAP2BluetoothEmulationTransportAdapter::GetDeviceType() const {
  return IOS_BT;
}

void IAP2BluetoothEmulationTransportAdapter::TransportConfigUpdated(
    const TransportConfig& new_config) {
  return;
}

IAP2USBEmulationTransportAdapter::IAP2USBEmulationTransportAdapter(
    const uint16_t port,
    resumption::LastStateWrapperPtr last_state_wrapper,
    const TransportManagerSettings& settings)
    : TcpTransportAdapter(port, last_state_wrapper, settings), out_(0) {
  auto delegate = new IAPSignalHandlerDelegate(*this);
  signal_handler_ = threads::CreateThread("iAP signal handler", delegate);
  signal_handler_->Start();
  const auto result = mkfifo(out_signals_channel, mode);
  UNUSED(result);
  SDL_LOG_DEBUG("Out signals channel creation result: " << result);
}

IAP2USBEmulationTransportAdapter::~IAP2USBEmulationTransportAdapter() {
  signal_handler_->Stop(threads::Thread::kThreadSoftStop);
  auto delegate = signal_handler_->GetDelegate();
  signal_handler_->SetDelegate(NULL);
  delete delegate;
  threads::DeleteThread(signal_handler_);
  SDL_LOG_DEBUG("Out close result: " << close(out_));
  SDL_LOG_DEBUG("Out unlink result: " << unlink(out_signals_channel));
}

void IAP2USBEmulationTransportAdapter::DeviceSwitched(
    const DeviceUID& device_handle) {
  SDL_LOG_AUTO_TRACE();
  UNUSED(device_handle);
  const auto switch_signal_ack = std::string("SDL_TRANSPORT_SWITCH_ACK\n");

  auto out_ = open(out_signals_channel, O_WRONLY);
  SDL_LOG_DEBUG("Out channel descriptor: " << out_);

  if (out_ < 0) {
    SDL_LOG_ERROR("Failed to open out signals channel");
    return;
  }

  const auto bytes =
      write(out_, switch_signal_ack.c_str(), switch_signal_ack.size());
  UNUSED(bytes);
  SDL_LOG_DEBUG("Written bytes to out: " << bytes);

  SDL_LOG_DEBUG("Switching signal ACK is sent");
  SDL_LOG_DEBUG("iAP2 USB device is switched with iAP2 Bluetooth");
  close(out_);
}

DeviceType IAP2USBEmulationTransportAdapter::GetDeviceType() const {
  return IOS_USB;
}

void IAP2USBEmulationTransportAdapter::TransportConfigUpdated(
    const TransportConfig& new_config) {
  return;
}

IAP2USBEmulationTransportAdapter::IAPSignalHandlerDelegate::
    IAPSignalHandlerDelegate(IAP2USBEmulationTransportAdapter& adapter)
    : adapter_(adapter), run_flag_(true), in_(0) {
  const auto result = mkfifo(in_signals_channel, mode);
  UNUSED(result);
  SDL_LOG_DEBUG("In signals channel creation result: " << result);
}

void IAP2USBEmulationTransportAdapter::IAPSignalHandlerDelegate::threadMain() {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Signal handling is started");
  const auto switch_signal = "SDL_TRANSPORT_SWITCH";
  SDL_LOG_DEBUG("Waiting for signal: " << switch_signal);

  in_ = open(in_signals_channel, O_RDONLY);
  SDL_LOG_DEBUG("In channel descriptor: " << in_);
  if (in_ < 0) {
    SDL_LOG_ERROR("Failed to open in signals channel");
    return;
  }

  const auto size = 32;
  while (run_flag_) {
    char buffer[size];
    auto bytes = read(in_, &buffer, size - 1);
    if (0 == bytes) {
      continue;
    }
    if (-1 == bytes) {
      SDL_LOG_DEBUG("Error during input pipe read");
      break;
    }
    SDL_LOG_DEBUG("Read in bytes: " << bytes);
    buffer[bytes] = '\0';
    std::string str(buffer);
    if (std::string::npos != str.find(switch_signal)) {
      SDL_LOG_DEBUG("Switch signal received.");
      adapter_.DoTransportSwitch();
    }
  }

  SDL_LOG_DEBUG("In close result: " << close(in_));
  SDL_LOG_DEBUG("In unlink result: " << unlink(in_signals_channel));
}

void IAP2USBEmulationTransportAdapter::IAPSignalHandlerDelegate::
    exitThreadMain() {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("Stopping signal handling.");
  run_flag_ = false;
  ThreadDelegate::exitThreadMain();
}
}  // namespace transport_adapter
}  // namespace transport_manager

/*
 * \file transport_manager_default.cc
 * \brief TransportManagerDefault class source file.
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
#include "transport_manager/transport_manager_default.h"
#include "transport_manager/tcp/tcp_transport_adapter.h"
#include "utils/logger.h"

#ifdef BLUETOOTH_SUPPORT
#include "transport_manager/bluetooth/bluetooth_transport_adapter.h"
#endif

#if defined(USB_SUPPORT)
#include "transport_manager/usb/usb_aoa_adapter.h"
#endif  // USB_SUPPORT

#if defined(CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT)
#include "transport_manager/cloud/cloud_websocket_transport_adapter.h"
#endif

#if defined(BUILD_TESTS)
#include "transport_manager/iap2_emulation/iap2_transport_adapter.h"
#endif  // BUILD_TEST

namespace transport_manager {
CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

TransportManagerDefault::TransportManagerDefault(
    const TransportManagerSettings& settings)
    : TransportManagerImpl(settings) {}

int TransportManagerDefault::Init(resumption::LastState& last_state) {
  LOG4CXX_TRACE(logger_, "enter");
  if (E_SUCCESS != TransportManagerImpl::Init(last_state)) {
    LOG4CXX_TRACE(logger_,
                  "exit with E_TM_IS_NOT_INITIALIZED. Condition: E_SUCCESS != "
                  "TransportManagerImpl::Init()");
    return E_TM_IS_NOT_INITIALIZED;
  }

#ifdef BLUETOOTH_SUPPORT
  transport_adapter::TransportAdapterImpl* ta_bluetooth =
      new transport_adapter::BluetoothTransportAdapter(last_state,
                                                       get_settings());
#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    ta_bluetooth->SetTelemetryObserver(metric_observer_);
  }
#endif  // TELEMETRY_MONITOR
  AddTransportAdapter(ta_bluetooth);
  ta_bluetooth = NULL;
#endif

  const uint16_t port = get_settings().transport_manager_tcp_adapter_port();
  transport_adapter::TransportAdapterImpl* ta_tcp =
      new transport_adapter::TcpTransportAdapter(
          port, last_state, get_settings());
#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    ta_tcp->SetTelemetryObserver(metric_observer_);
  }
#endif  // TELEMETRY_MONITOR
  AddTransportAdapter(ta_tcp);
  ta_tcp = NULL;

#if defined(USB_SUPPORT)
  transport_adapter::TransportAdapterImpl* ta_usb =
      new transport_adapter::UsbAoaAdapter(last_state, get_settings());
#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    ta_usb->SetTelemetryObserver(metric_observer_);
  }
#endif  // TELEMETRY_MONITOR
  AddTransportAdapter(ta_usb);
  ta_usb = NULL;
#endif  // USB_SUPPORT

#if defined CLOUD_APP_WEBSOCKET_TRANSPORT_SUPPORT
  printf("Creating cloud transport\n");
  transport_adapter::TransportAdapterImpl* ta_cloud =
      new transport_adapter::CloudWebsocketTransportAdapter(
          last_state, get_settings());  // Todo add retry connection logic from
                                        // ini to initializer.
#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    ta_cloud->SetTelemetryObserver(metric_observer_);
  }
#endif  // TELEMETRY_MONITOR
  AddTransportAdapter(ta_cloud);
  ta_cloud = NULL;
#endif

#if defined BUILD_TESTS
  const uint16_t iap2_bt_emu_port = 23456;
  transport_adapter::IAP2BluetoothEmulationTransportAdapter*
      iap2_bt_emu_adapter =
          new transport_adapter::IAP2BluetoothEmulationTransportAdapter(
              iap2_bt_emu_port, last_state, get_settings());

  AddTransportAdapter(iap2_bt_emu_adapter);

  const uint16_t iap2_usb_emu_port = 34567;
  transport_adapter::IAP2USBEmulationTransportAdapter* iap2_usb_emu_adapter =
      new transport_adapter::IAP2USBEmulationTransportAdapter(
          iap2_usb_emu_port, last_state, get_settings());

  AddTransportAdapter(iap2_usb_emu_adapter);
#endif  // BUILD_TEST

  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

TransportManagerDefault::~TransportManagerDefault() {}

}  //  namespace transport_manager

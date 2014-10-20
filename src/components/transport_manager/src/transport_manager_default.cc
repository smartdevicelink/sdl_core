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

#include "config_profile/profile.h"

#include "transport_manager/transport_manager_default.h"
#include "transport_manager/tcp/tcp_transport_adapter.h"
#include "utils/logger.h"

#ifdef BLUETOOTH_SUPPORT
#include "transport_manager/bluetooth/bluetooth_transport_adapter.h"
#endif

// CUSTOMER_PASA

#if defined(USB_SUPPORT)
#include "transport_manager/usb/usb_aoa_adapter.h"
#endif  // USB_SUPPORT



namespace transport_manager {
CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

int TransportManagerDefault::Init() {
  LOG4CXX_TRACE(logger_, "enter");
  if (E_SUCCESS != TransportManagerImpl::Init()) {
    LOG4CXX_TRACE(logger_, "exit with E_TM_IS_NOT_INITIALIZED. Condition: E_SUCCESS != TransportManagerImpl::Init()");
    return E_TM_IS_NOT_INITIALIZED;
  }
  transport_adapter::TransportAdapterImpl* ta;
#ifdef BLUETOOTH_SUPPORT

  ta = new transport_adapter::BluetoothTransportAdapter;

#ifdef TIME_TESTER
  if (metric_observer_) {
    ta->SetTimeMetricObserver(metric_observer_);
  }
#endif  // TIME_TESTER
  AddTransportAdapter(ta);
#endif
  uint16_t port = profile::Profile::instance()->transport_manager_tcp_adapter_port();
  ta = new transport_adapter::TcpTransportAdapter(port);
#ifdef TIME_TESTER
  if (metric_observer_) {
    ta->SetTimeMetricObserver(metric_observer_);
  }
#endif  // TIME_TESTER
  AddTransportAdapter(ta);

// CUSTOMER_PASA

#if defined(USB_SUPPORT)
  ta = new transport_adapter::UsbAoaAdapter();
#ifdef TIME_TESTER
  if (metric_observer_) {
    ta->SetTimeMetricObserver(metric_observer_);
  }
#endif  // TIME_TESTER
  AddTransportAdapter(ta);
#endif  // USB_SUPPORT



  LOG4CXX_TRACE(logger_, "exit with E_SUCCESS");
  return E_SUCCESS;
}

TransportManagerDefault::~TransportManagerDefault() {}

TransportManagerDefault::TransportManagerDefault()
    : TransportManagerImpl() {}

}  //  namespace transport_manager

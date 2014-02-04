/**
 * \file tcp_transport_adapter.cc
 * \brief TcpTransportAdapter class source file.
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

#include <memory.h>
#include <signal.h>
#include <errno.h>

#include "resumption/last_state.h"

#include "transport_manager/tcp/tcp_transport_adapter.h"
#include "transport_manager/tcp/tcp_client_listener.h"
#include "transport_manager/tcp/tcp_connection_factory.h"
#include "transport_manager/tcp/tcp_device.h"

#ifdef AVAHI_SUPPORT
#include "transport_manager/tcp/dnssd_service_browser.h"
#endif

namespace transport_manager {
namespace transport_adapter {

TcpTransportAdapter::TcpTransportAdapter()
    : TransportAdapterImpl(
#ifdef AVAHI_SUPPORT
    new DnssdServiceBrowser(this)
#else
    NULL
#endif
    , new TcpConnectionFactory(this)
    , new TcpClientListener(this, default_port)) {
}

TcpTransportAdapter::~TcpTransportAdapter() {
}

DeviceType TcpTransportAdapter::GetDeviceType() const {
  return "sdl-tcp";
}

void TcpTransportAdapter::Store(void) const {
  LOG4CXX_TRACE_ENTER(logger_);
  DeviceList device_ids = GetDeviceList();
  resumption::LastState::TcpDeviceStateContainer states;
  for (DeviceList::const_iterator i = device_ids.begin(); i != device_ids.end(); ++i) {
    DeviceUID device_id = *i;
    DeviceSptr device = FindDevice(device_id);
    utils::SharedPtr<TcpDevice> tcp_device =
      DeviceSptr::static_pointer_cast<TcpDevice>(device);
    resumption::LastState::TcpDeviceState state;
    state.in_addr = tcp_device->in_addr();
    state.name = tcp_device->name();
    ApplicationList app_ids = tcp_device->GetApplicationList();
    for (ApplicationList::const_iterator j = app_ids.begin(); j != app_ids.end(); ++j) {
      ApplicationHandle app_handle = *j;
      uint16_t port = tcp_device->GetApplicationPort(app_handle);
      resumption::LastState::TcpDeviceApplicationState app_state;
      app_state.port = port;
      state.applications.push_back(app_state);
    }
    states.push_back(state);
  }
  resumption::LastState::instance()->StoreTcpDeviceStates(states);
  LOG4CXX_TRACE_EXIT(logger_);
}

bool TcpTransportAdapter::Restore(void) {
  LOG4CXX_TRACE_ENTER(logger_);
  bool errors_occured = false;
  resumption::LastState::TcpDeviceStateContainer tcp_device_states =
    resumption::LastState::instance()->tcp_device_states();
  for (resumption::LastState::TcpDeviceStateContainer::const_iterator i =
    tcp_device_states.begin(); i != tcp_device_states.end(); ++i) {
    TcpDevice* tcp_device = new TcpDevice(i->in_addr, i->name);
    DeviceSptr device(tcp_device);
    AddDevice(device);
    resumption::LastState::TcpDeviceApplicationStateContainer applications =
      i->applications;
    for (resumption::LastState::TcpDeviceApplicationStateContainer::const_iterator j =
      applications.begin(); j != applications.end(); ++j) {
      ApplicationHandle app_handle = tcp_device->AddDiscoveredApplication(j->port);
      if (Error::OK != Connect(device->unique_device_id(), app_handle)) {
        errors_occured = true;
      }
    }
  }
  LOG4CXX_TRACE_EXIT(logger_);
  return !errors_occured;
}

}  // namespace transport_adapter
}  // namespace transport_manager


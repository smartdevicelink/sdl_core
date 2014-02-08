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

void TcpTransportAdapter::Store() const {
  LOG4CXX_TRACE_ENTER(logger_);
  resumption::LastState::Dictionary tcp_adapter_dictionary;
  DeviceList device_ids = GetDeviceList();
  for (DeviceList::const_iterator i = device_ids.begin(); i != device_ids.end(); ++i) {
    DeviceUID device_id = *i;
    DeviceSptr device = FindDevice(device_id);
    if (!device) { // device could have been disconnected
      continue;
    }
    utils::SharedPtr<TcpDevice> tcp_device =
      DeviceSptr::static_pointer_cast<TcpDevice>(device);
    resumption::LastState::Dictionary device_dictionary;
    struct in_addr address;
    address.s_addr = tcp_device->in_addr();
    device_dictionary.AddItem("address", std::string(inet_ntoa(address)));
    resumption::LastState::Dictionary applications_dictionary;
    ApplicationList app_ids = tcp_device->GetApplicationList();
    for (ApplicationList::const_iterator j = app_ids.begin(); j != app_ids.end(); ++j) {
      ApplicationHandle app_handle = *j;
      int port = tcp_device->GetApplicationPort(app_handle);
      if (port != -1) { // don't want to store incoming applications
        resumption::LastState::Dictionary application_dictionary;
        application_dictionary.AddItem("port", std::to_string(port));
        applications_dictionary.AddSubitem(std::to_string(port), application_dictionary);
      }
    }
    device_dictionary.AddSubitem("applications", applications_dictionary);
    tcp_adapter_dictionary.AddSubitem(tcp_device->name(), device_dictionary);
  }
  resumption::LastState::instance()->dictionary.AddSubitem(
    "TcpAdapter", tcp_adapter_dictionary
  );
  LOG4CXX_TRACE_EXIT(logger_);
}

bool TcpTransportAdapter::Restore() {
  LOG4CXX_TRACE_ENTER(logger_);
  bool errors_occured = false;
  resumption::LastState::Dictionary tcp_adapter_dictionary =
    resumption::LastState::instance()->dictionary.SubitemAt("TcpAdapter");
  for (resumption::LastState::Dictionary::const_iterator i =
    tcp_adapter_dictionary.begin(); i != tcp_adapter_dictionary.end(); ++i) {
    std::string name = i->first;
    resumption::LastState::Dictionary device_dictionary = i->second;
    std::string address_record = device_dictionary.ItemAt("address");
    in_addr_t address = inet_addr(address_record.c_str());
    TcpDevice* tcp_device = new TcpDevice(address, name);
    DeviceSptr device(tcp_device);
    AddDevice(device);
    resumption::LastState::Dictionary applications_dictionary =
      device_dictionary.SubitemAt("applications");
    for (resumption::LastState::Dictionary::const_iterator j =
      applications_dictionary.begin(); j != applications_dictionary.end(); ++j) {
      resumption::LastState::Dictionary application_dictionary = j->second;
      std::string port_record = application_dictionary.ItemAt("port");
      int port = atoi(port_record.c_str());
      ApplicationHandle app_handle = tcp_device->AddDiscoveredApplication(port);
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


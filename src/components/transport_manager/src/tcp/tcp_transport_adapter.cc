/**
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
#include <sstream>
#include <cstdlib>

#include "resumption/last_state.h"

#include "utils/logger.h"
#include "utils/threads/thread_delegate.h"

#include "transport_manager/tcp/tcp_transport_adapter.h"
#include "transport_manager/tcp/tcp_client_listener.h"
#include "transport_manager/tcp/tcp_connection_factory.h"
#include "transport_manager/tcp/tcp_device.h"

#ifdef AVAHI_SUPPORT
#include "transport_manager/tcp/dnssd_service_browser.h"
#endif

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportAdapterImpl")

TcpTransportAdapter::TcpTransportAdapter(const uint16_t port)
  : TransportAdapterImpl(
#ifdef AVAHI_SUPPORT
    new DnssdServiceBrowser(this),
#else
    NULL,
#endif
    new TcpConnectionFactory(this),
    new TcpClientListener(this, port, false)) {
}

TcpTransportAdapter::~TcpTransportAdapter() {
}

DeviceType TcpTransportAdapter::GetDeviceType() const {
  return "sdl-tcp";
}

void TcpTransportAdapter::Store() const {
  LOG4CXX_TRACE(logger_, "enter");
  Json::Value tcp_adapter_dictionary;
  Json::Value devices_dictionary;
  DeviceList device_ids = GetDeviceList();
  for (DeviceList::const_iterator i = device_ids.begin(); i != device_ids.end(); ++i) {
    DeviceUID device_id = *i;
    DeviceSptr device = FindDevice(device_id);
    if (!device) { // device could have been disconnected
      continue;
    }
    utils::SharedPtr<TcpDevice> tcp_device =
      DeviceSptr::static_pointer_cast<TcpDevice>(device);
    Json::Value device_dictionary;
    device_dictionary["name"] = tcp_device->name();
    struct in_addr address;
    address.s_addr = tcp_device->in_addr();
    device_dictionary["address"] = std::string(inet_ntoa(address));
    Json::Value applications_dictionary;
    ApplicationList app_ids = tcp_device->GetApplicationList();
    for (ApplicationList::const_iterator j = app_ids.begin(); j != app_ids.end(); ++j) {
      ApplicationHandle app_handle = *j;
      if (FindEstablishedConnection(tcp_device->unique_device_id(), app_handle)) {
        int port = tcp_device->GetApplicationPort(app_handle);
        if (port != -1) { // don't want to store incoming applications
          Json::Value application_dictionary;
          char port_record[12];
          sprintf(port_record, "%d", port);
          application_dictionary["port"] = std::string(port_record);
          applications_dictionary.append(application_dictionary);
        }
      }
    }
    if (!applications_dictionary.empty()) {
      device_dictionary["applications"] = applications_dictionary;
      devices_dictionary.append(device_dictionary);
    }
  }
  tcp_adapter_dictionary["devices"] = devices_dictionary;
  resumption::LastState::instance()->dictionary["TransportManager"]["TcpAdapter"] =
    tcp_adapter_dictionary;
  LOG4CXX_TRACE(logger_, "exit");
}

bool TcpTransportAdapter::Restore() {
  LOG4CXX_TRACE(logger_, "enter");
  bool errors_occurred = false;
  const Json::Value tcp_adapter_dictionary =
    resumption::LastState::instance()->dictionary["TransportManager"]["TcpAdapter"];
  const Json::Value devices_dictionary = tcp_adapter_dictionary["devices"];
  for (Json::Value::const_iterator i = devices_dictionary.begin();
       i != devices_dictionary.end(); ++i) {
    const Json::Value device_dictionary = *i;
    std::string name = device_dictionary["name"].asString();
    std::string address_record = device_dictionary["address"].asString();
    in_addr_t address = inet_addr(address_record.c_str());
    TcpDevice* tcp_device = new TcpDevice(address, name);
    DeviceSptr device(tcp_device);
    AddDevice(device);
    const Json::Value applications_dictionary = device_dictionary["applications"];
    for (Json::Value::const_iterator j = applications_dictionary.begin();
         j != applications_dictionary.end(); ++j) {
      const Json::Value application_dictionary = *j;
      std::string port_record = application_dictionary["port"].asString();
      int port = atoi(port_record.c_str());
      ApplicationHandle app_handle = tcp_device->AddDiscoveredApplication(port);
      if (Error::OK != Connect(device->unique_device_id(), app_handle)) {
        errors_occurred = true;
      }
    }
  }
  bool result = !errors_occurred;
  if (result) {
    LOG4CXX_TRACE(logger_, "exit with TRUE");
  } else {
    LOG4CXX_TRACE(logger_, "exit with FALSE");
  }
  return result;
}

}  // namespace transport_adapter
}  // namespace transport_manager


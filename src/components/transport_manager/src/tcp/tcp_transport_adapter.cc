/*
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

#include "transport_manager/tcp/tcp_transport_adapter.h"

#include <errno.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>

#include <cstdlib>
#include <sstream>

#if defined(OS_WINDOWS)
#include "utils/winhdr.h"
#endif

#include "resumption/last_state.h"
#include "transport_manager/tcp/tcp_client_listener.h"
#include "transport_manager/tcp/tcp_connection_factory.h"
#include "transport_manager/tcp/tcp_device.h"
#include "utils/json_utils.h"
#include "utils/convert_utils.h"
#include "utils/logger.h"
#include "utils/threads/thread_delegate.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOGGER("TransportManager")

TcpTransportAdapter::TcpTransportAdapter(
    const uint16_t port,
    resumption::LastState& last_state,
    const TransportManagerSettings& settings)
    : TransportAdapterImpl(NULL,
                           new TcpConnectionFactory(this),
                           new TcpClientListener(this, port, true),
                           last_state,
                           settings) {}

TcpTransportAdapter::~TcpTransportAdapter() {}

DeviceType TcpTransportAdapter::GetDeviceType() const {
  return TCP;
}

void TcpTransportAdapter::Store() const {
  SDL_AUTO_TRACE();
  using namespace utils::json;

  JsonValue tcp_adapter_dictionary;
  JsonValue devices_dictionary;
  DeviceList device_ids = GetDeviceList();
  for (DeviceList::const_iterator i = device_ids.begin(); i != device_ids.end();
       ++i) {
    DeviceUID device_id = *i;
    DeviceSptr device = FindDevice(device_id);
    if (!device) {  // device could have been disconnected
      continue;
    }
    utils::SharedPtr<TcpDevice> tcp_device =
        DeviceSptr::static_pointer_cast<TcpDevice>(device);
    JsonValue device_dictionary;
    device_dictionary["name"] = tcp_device->name();
    device_dictionary["address"] = tcp_device->Address().ToString();
    JsonValue applications_dictionary;
    ApplicationList app_ids = tcp_device->GetApplicationList();
    for (ApplicationList::const_iterator j = app_ids.begin();
         j != app_ids.end();
         ++j) {
      ApplicationHandle app_handle = *j;
      if (FindEstablishedConnection(tcp_device->unique_device_id(),
                                    app_handle)) {
        int port = tcp_device->GetApplicationPort(app_handle);
        if (port != -1) {  // don't want to store incoming applications
          JsonValue application_dictionary;
          application_dictionary["port"] =
              utils::ConvertInt64ToLongLongInt(port);
          applications_dictionary.Append(application_dictionary);
        }
      }
    }
    if (!applications_dictionary.IsEmpty()) {
      device_dictionary["applications"] = applications_dictionary;
      devices_dictionary.Append(device_dictionary);
    }
  }
  tcp_adapter_dictionary["devices"] = devices_dictionary;
  JsonValue& dictionary = last_state().dictionary();
  dictionary["TransportManager"]["TcpAdapter"] = tcp_adapter_dictionary;
}

bool TcpTransportAdapter::Restore() {
  SDL_AUTO_TRACE();
  using namespace utils::json;
  bool errors_occurred = false;
  const JsonValue& dictionary = last_state().dictionary();
  const JsonValueRef tcp_adapter_dictionary =
      dictionary["TransportManager"]["TcpAdapter"];
  const JsonValueRef devices_dictionary = tcp_adapter_dictionary["devices"];
  for (JsonValue::const_iterator devices_itr = devices_dictionary.begin(),
                                 devices_end = devices_dictionary.end();
       devices_itr != devices_end;
       ++devices_itr) {
    const JsonValueRef device_dictionary = *devices_itr;
    std::string name = device_dictionary["name"].AsString();
    std::string address = device_dictionary["address"].AsString();
    TcpDevice* tcp_device = new TcpDevice(utils::HostAddress(address), name);
    DeviceSptr device(tcp_device);
    AddDevice(device);
    const JsonValueRef applications_dictionary =
        device_dictionary["applications"];
    for (JsonValue::const_iterator
             applications_itr = applications_dictionary.begin(),
             applications_end = applications_dictionary.end();
         applications_itr != applications_end;
         ++applications_itr) {
      const JsonValueRef application_dictionary = *applications_itr;
      int port = application_dictionary["port"].AsInt();
      ApplicationHandle app_handle = tcp_device->AddApplication(port, false);
      if (Error::OK != Connect(device->unique_device_id(), app_handle)) {
        errors_occurred = true;
      }
    }
  }
  bool result = !errors_occurred;
  SDL_DEBUG("result " << std::boolalpha << result);
  return result;
}

}  // namespace transport_adapter
}  // namespace transport_manager

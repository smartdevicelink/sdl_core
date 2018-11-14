/*
 * Copyright (c) 2018, Livio
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

#include "transport_manager/cloud/cloud_websocket_transport_adapter.h"
#include "transport_manager/cloud/cloud_websocket_connection_factory.h"

#include "transport_manager/cloud/cloud_device.h"
#include "transport_manager/cloud/websocket_client_connection.h"

#include <boost/regex.hpp>

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

CloudWebsocketTransportAdapter::CloudWebsocketTransportAdapter(
    resumption::LastState& last_state, const TransportManagerSettings& settings)
    : TransportAdapterImpl(NULL,
                           new CloudWebsocketConnectionFactory(this),
                           NULL,
                           last_state,
                           settings) {
  /*DeviceUID device_id  = "Cloud";
  ApplicationHandle app_handle = 100;
  printf("Calling create connection\n");
  const TransportAdapter::Error err =
      server_connection_factory_->CreateConnection(device_id, app_handle);
    LOG4CXX_DEBUG(logger_, err);*/
}

CloudWebsocketTransportAdapter::~CloudWebsocketTransportAdapter() {}

void CloudWebsocketTransportAdapter::CloudTransportConfigUpdated(
    const CloudAppTransportConfig& new_config) {}

CloudAppTransportConfig
CloudWebsocketTransportAdapter::GetCloudTransportConfiguration() const {
  return transport_config_;
}

DeviceType CloudWebsocketTransportAdapter::GetDeviceType() const {
  return CLOUD_WEBSOCKET;
}

void CloudWebsocketTransportAdapter::Store() const {
}  // todo decide if this is needed

bool CloudWebsocketTransportAdapter::Restore() {  // todo decide if resumption
                                                  // is needed
  return true;
}

void CloudWebsocketTransportAdapter::CreateDevice(const std::string& uid) {
  boost::regex pattern(
      "(wss?):\\/\\/([A-Z\\d\\.-]{2,})\\.?([A-Z]{2,})?(:\\d{2,4})\\/",
      boost::regex::icase);
  std::string str = uid;
  if (!boost::regex_match(str, pattern)) {
    LOG4CXX_DEBUG(logger_, "Invalid Endpoint: " << uid);
    return;
  }

  LOG4CXX_DEBUG(logger_, "Valid Endpoint: " << uid);
  std::size_t pos = uid.find(":");
  pos = uid.find(":", pos + 1);
  // std::size_t size = uid.length();
  std::string host = uid.substr(0, pos);
  std::string port = uid.substr(pos + 1);
  std::string device_id = uid;

  LOG4CXX_DEBUG(logger_,
                "Creating Cloud Device For Host: " << host
                                                   << " and Port: " << port);

  // todo get nickname from policies to name device
  auto cloud_device = std::make_shared<CloudDevice>(host, port, device_id);

  DeviceVector devices{cloud_device};

  SearchDeviceDone(devices);

  // Create connection object, do not start until app is activated
  std::shared_ptr<WebsocketClientConnection> connection =
      std::make_shared<WebsocketClientConnection>(uid, 0, this);

  ConnectionCreated(connection, uid, 0);
  ConnectPending(uid, 0);

  return;
}
}
}
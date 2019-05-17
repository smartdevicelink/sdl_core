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
                           settings) {}

CloudWebsocketTransportAdapter::~CloudWebsocketTransportAdapter() {}

void CloudWebsocketTransportAdapter::SetAppCloudTransportConfig(
    std::string app_id, CloudAppProperties properties) {
  transport_config_[app_id] = properties;
}

const CloudAppProperties&
CloudWebsocketTransportAdapter::GetAppCloudTransportConfig(std::string app_id) {
  return transport_config_[app_id];
}

DeviceType CloudWebsocketTransportAdapter::GetDeviceType() const {
  return CLOUD_WEBSOCKET;
}

void CloudWebsocketTransportAdapter::Store() const {}

bool CloudWebsocketTransportAdapter::Restore() {
  return true;
}

void CloudWebsocketTransportAdapter::CreateDevice(const std::string& uid) {
  // If the device has already been created, just ignore the request
  DeviceSptr device = FindDevice(uid);
  if (device.use_count() != 0) {
    return;
  }

  std::string protocol_pattern = "(wss?)";
  std::string host_pattern = "([a-zA-Z\\d\\.-]{2,}\\@?([a-zA-Z]{2,})?)";
  std::string port_pattern = "(\\d{2,5})";
  // Optional parameters
  std::string path_pattern = "((\\/[^\\n\\t\\/#? ]+)*)?";
  std::string query_pattern = "\\/?(\\?[^=&#\\n\\t ]*=?[^&#\\n\\t ]*&?)?";
  std::string position_pattern = "(#[^\\n\\t ]*)?";

  // Extract host and port from endpoint string
  boost::regex group_pattern(protocol_pattern + ":\\/\\/" + host_pattern + ":" +
                                 port_pattern + path_pattern + query_pattern +
                                 position_pattern,
                             boost::regex::icase);
  boost::smatch results;
  std::string str = uid;

  if (!boost::regex_search(str, results, group_pattern)) {
    LOG4CXX_DEBUG(logger_, "Invalid Pattern: " << uid);
    return;
  }

  LOG4CXX_DEBUG(logger_, "#Results: " << results.size());
  std::string results_str;
  for (size_t i = 0; i < results.size(); i++) {
    results_str += " R[" + std::to_string(i) + "]:";
    results_str +=
        (results[i].length() != 0) ? results[i] : std::string("<EMPTY>");
  }
  LOG4CXX_DEBUG(logger_, "Results: " << results_str);

  std::string device_id = uid;

  CloudAppEndpoint endpoint{.host = results[2],
                            .port = results[4],
                            .path = results[5] + "/",
                            .query = results[7],
                            .position = results[8]};

  LOG4CXX_DEBUG(logger_,
                "Creating Cloud Device For Host: "
                    << endpoint.host << " at Port: " << endpoint.port
                    << " with Target: "
                    << (endpoint.path + endpoint.query + endpoint.position));

  auto cloud_device = std::make_shared<CloudDevice>(endpoint, device_id);

  DeviceVector devices{cloud_device};

  SearchDeviceDone(devices);

  // Create connection object, do not start until app is activated
  std::shared_ptr<WebsocketClientConnection> connection =
      std::make_shared<WebsocketClientConnection>(uid, 0, this);

  ConnectionCreated(connection, uid, 0);
  ConnectPending(uid, 0);

  return;
}
}  // namespace transport_adapter
}  // namespace transport_manager

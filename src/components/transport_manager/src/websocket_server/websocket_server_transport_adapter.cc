/*
 * Copyright (c) 2020, Ford Motor Company
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

#include "transport_manager/websocket_server/websocket_server_transport_adapter.h"

#include <errno.h>
#include <memory.h>
#include <signal.h>
#include <stdio.h>

#include <cstdlib>
#include <sstream>

#include "transport_manager/websocket_server/websocket_listener.h"
#include "utils/logger.h"
#include "utils/threads/thread_delegate.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "WebSocketTransportAdapter")

WebSocketServerTransportAdapter::WebSocketServerTransportAdapter(
    resumption::LastState& last_state, const TransportManagerSettings& settings)
    : TransportAdapterImpl(nullptr,
                           nullptr,
                           new WebSocketListener(this, settings),
                           last_state,
                           settings) {}

WebSocketServerTransportAdapter::~WebSocketServerTransportAdapter() {}

void WebSocketServerTransportAdapter::TransportConfigUpdated(
    const TransportConfig& new_config) {
  LOG4CXX_AUTO_TRACE(logger_);

  transport_config_ = new_config;

  // call the method of parent class to trigger OnTransportConfigUpdated() for
  // the listeners
  TransportAdapterImpl::TransportConfigUpdated(new_config);
}

TransportConfig WebSocketServerTransportAdapter::GetTransportConfiguration()
    const {
  LOG4CXX_AUTO_TRACE(logger_);
  return transport_config_;
}

DeviceType WebSocketServerTransportAdapter::GetDeviceType() const {
  return WEBENGINE_WEBSOCKET;
}

DeviceSptr WebSocketServerTransportAdapter::AddDevice(DeviceSptr device) {
  LOG4CXX_AUTO_TRACE(logger_);
  webengine_device_ = device;
  return TransportAdapterImpl::AddDevice(webengine_device_);
}

TransportAdapter::Error WebSocketServerTransportAdapter::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (webengine_device_) {
    AddDevice(webengine_device_);
  }
  return TransportAdapterImpl::Init();
}

}  // namespace transport_adapter
}  // namespace transport_manager

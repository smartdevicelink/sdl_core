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
#include "utils/gen_hash.h"
#include "utils/logger.h"
#include "utils/threads/thread_delegate.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("WebSocketTransportAdapter")

WebSocketServerTransportAdapter::WebSocketServerTransportAdapter(
    resumption::LastStateWrapperPtr last_state_wrapper,
    const TransportManagerSettings& settings)
    : TransportAdapterImpl(nullptr,
                           nullptr,
                           new WebSocketListener(this, settings),
                           last_state_wrapper,
                           settings) {}

WebSocketServerTransportAdapter::~WebSocketServerTransportAdapter() {}

void WebSocketServerTransportAdapter::TransportConfigUpdated(
    const TransportConfig& new_config) {
  SDL_LOG_AUTO_TRACE();

  transport_config_ = new_config;

  // call the method of parent class to trigger OnTransportConfigUpdated() for
  // the listeners
  TransportAdapterImpl::TransportConfigUpdated(new_config);
}

TransportConfig WebSocketServerTransportAdapter::GetTransportConfiguration()
    const {
  SDL_LOG_AUTO_TRACE();
  return transport_config_;
}

DeviceType WebSocketServerTransportAdapter::GetDeviceType() const {
  return WEBENGINE_WEBSOCKET;
}

DeviceSptr WebSocketServerTransportAdapter::AddDevice(DeviceSptr device) {
  SDL_LOG_AUTO_TRACE();
  webengine_device_ = device;
  Store();
  return TransportAdapterImpl::AddDevice(webengine_device_);
}

TransportAdapter::Error WebSocketServerTransportAdapter::Init() {
  SDL_LOG_AUTO_TRACE();
  if (webengine_device_) {
    AddDevice(webengine_device_);
  }
  return TransportAdapterImpl::Init();
}

void WebSocketServerTransportAdapter::Store() const {
  SDL_LOG_AUTO_TRACE();
  if (webengine_device_) {
    Json::Value& dictionary = last_state().get_dictionary();
    if (dictionary["TransportManager"].isMember("WebsocketServerAdapter")) {
      SDL_LOG_DEBUG(
          "WebsocketServerAdapter already exists. Storing is skipped");
      return;
    }

    Json::Value device_dictionary;
    device_dictionary["unique_id"] = webengine_device_->unique_device_id();

    Json::Value ws_adapter_dictionary;
    ws_adapter_dictionary["device"] = device_dictionary;
    dictionary["TransportManager"]["WebsocketServerAdapter"] =
        ws_adapter_dictionary;
  }
}

bool WebSocketServerTransportAdapter::Restore() {
  SDL_LOG_AUTO_TRACE();
  const Json::Value& dictionary = last_state().get_dictionary();
  const Json::Value ws_adapter_dictionary =
      dictionary["TransportManager"]["WebsocketServerAdapter"];
  webengine_device_id_ =
      ws_adapter_dictionary["device"]["unique_id"].asString();
  if (webengine_device_id_.empty()) {
    srand(time(0));
    const size_t device_id_length = 64u;
    webengine_device_id_ = utils::gen_hash(device_id_length);
  }
  return true;
}

std::string WebSocketServerTransportAdapter::GetStoredDeviceID() const {
  SDL_LOG_AUTO_TRACE();
  return webengine_device_id_;
}

}  // namespace transport_adapter
}  // namespace transport_manager

/*
 * \file cloud_websocket_connection_factory.cc
 * \brief CloudWebsocketConnectionFactory class source file.
 *
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

#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/cloud/cloud_websocket_connection_factory.h"
#include "transport_manager/cloud/websocket_client_connection.h"
#include "utils/logger.h"

#include "transport_manager/cloud/cloud_device.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

CloudWebsocketConnectionFactory::CloudWebsocketConnectionFactory(
    TransportAdapterController* controller)
    : controller_(controller) {}

TransportAdapter::Error CloudWebsocketConnectionFactory::Init() {
  return TransportAdapter::OK;
}

TransportAdapter::Error CloudWebsocketConnectionFactory::CreateConnection(
    const DeviceUID& device_uid, const ApplicationHandle& app_handle) {
  LOG4CXX_AUTO_TRACE(logger_);
  std::shared_ptr<WebsocketClientConnection> connection =
      std::make_shared<WebsocketClientConnection>(
          device_uid, app_handle, controller_);
  controller_->ConnectionCreated(connection, device_uid, app_handle);
  TransportAdapter::Error error = connection->Start();
  if (TransportAdapter::OK != error) {
    LOG4CXX_ERROR(
        logger_,
        "Cloud Websocket connection::Start() failed with error: " << error);
  }
  return error;
}

void CloudWebsocketConnectionFactory::Terminate() {}

bool CloudWebsocketConnectionFactory::IsInitialised() const {
  return true;
}

CloudWebsocketConnectionFactory::~CloudWebsocketConnectionFactory() {}

}  // namespace transport_adapter
}  // namespace transport_manager

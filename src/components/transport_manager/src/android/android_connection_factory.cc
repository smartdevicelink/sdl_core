/*
 * \file android_connection_factory.cc
 * \brief AndroidConnectionFactory class source file.
 *
 * Copyright (c) 2021, Ford Motor Company
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

#include "transport_manager/android/android_connection_factory.h"
#include "transport_manager/android/android_socket_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "utils/logger.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

AndroidConnectionFactory::AndroidConnectionFactory(
    AndroidTransportAdapter* controller)
    : controller_(controller) {}

TransportAdapter::Error AndroidConnectionFactory::Init() {
  return TransportAdapter::OK;
}

TransportAdapter::Error AndroidConnectionFactory::CreateConnection(
    const DeviceUID& device_uid, const ApplicationHandle& app_handle) {
  SDL_LOG_AUTO_TRACE();
  std::shared_ptr<AndroidSocketConnection> connection =
      std::make_shared<AndroidSocketConnection>(
          device_uid, app_handle, controller_);
  controller_->ConnectionCreated(connection, device_uid, app_handle);
  TransportAdapter::Error error = connection->Start();
  if (TransportAdapter::OK != error) {
    SDL_LOG_ERROR(
        "Android Ipc connection::Start() failed with error: " << error);
  }
  return error;
}

void AndroidConnectionFactory::Terminate() {}

bool AndroidConnectionFactory::IsInitialised() const {
  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager

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

#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/tcp/tcp_socket_connection.h"
#include "transport_manager/tcp/tcp_device.h"
#include "utils/logger.h"

#include <memory.h>
#include <signal.h>
#include <errno.h>

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")


TcpSocketConnection::TcpSocketConnection(const DeviceUID& device_uid,
    const ApplicationHandle& app_handle,
    TransportAdapterController* controller)
  : ThreadedSocketConnection(device_uid, app_handle, controller) {
}

TcpSocketConnection::~TcpSocketConnection() {
}

bool TcpSocketConnection::Establish(ConnectError** error) {
  return true;
}

TcpServerOiginatedSocketConnection::TcpServerOiginatedSocketConnection(
  const DeviceUID& device_uid, const ApplicationHandle& app_handle,
  TransportAdapterController* controller)
  : ThreadedSocketConnection(device_uid, app_handle, controller) {
}

TcpServerOiginatedSocketConnection::~TcpServerOiginatedSocketConnection() {
}

bool TcpServerOiginatedSocketConnection::Establish(ConnectError** error) {
  LOG4CXX_TRACE(logger_, "enter. error " << error);
  DeviceSptr device = controller()->FindDevice(device_handle());
  if (!device.valid()) {
    LOG4CXX_ERROR(logger_, "Device " << device_handle() << " not found");
    *error = new ConnectError();
    LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: !device.valid()");
    return false;
  }
  TcpDevice* tcp_device = static_cast<TcpDevice*>(device.get());

  int port;
  if (-1 == (port = tcp_device->GetApplicationPort(application_handle()))) {
    LOG4CXX_ERROR(logger_, "Application port for " << application_handle() << " not found");
    *error = new ConnectError();
    LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: port not found");
    return false;
  }

  const int socket = ::socket(AF_INET, SOCK_STREAM, 0);
  if (socket < 0) {
    LOG4CXX_ERROR(logger_, "Failed to create socket");
    *error = new ConnectError();
    LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: failed to create socket");
    return false;
  }

  struct sockaddr_in addr = { 0 };
  addr.sin_family = AF_INET;
  addr.sin_addr.s_addr = tcp_device->in_addr();
  addr.sin_port = htons(port);

  LOG4CXX_INFO(logger_, "Connecting " << inet_ntoa(addr.sin_addr) << ":"
               << port);
  if (::connect(socket, (struct sockaddr*) &addr, sizeof(addr)) < 0) {
    LOG4CXX_ERROR(logger_, "Failed to connect for application "
                  << application_handle() << ", error " << errno);
    *error = new ConnectError();
    LOG4CXX_TRACE(logger_, "exit with FALSE. Condition: failed to connect to application");
    return false;
  }

  set_socket(socket);
  LOG4CXX_TRACE(logger_, "exit with TRUE");
  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager


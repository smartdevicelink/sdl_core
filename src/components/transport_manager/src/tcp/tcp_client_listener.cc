/*
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

#include "transport_manager/tcp/tcp_client_listener.h"

#include "utils/logger.h"
#include "utils/threads/thread.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/tcp/tcp_device.h"
#include "transport_manager/tcp/tcp_socket_connection.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

TcpClientListener::TcpClientListener(TransportAdapterController* controller,
                                     const uint16_t port,
                                     const bool enable_keepalive)
    : port_(port)
    , enable_keepalive_(enable_keepalive)
    , controller_(controller)
    , thread_(0)
    , thread_stop_requested_(false) {
  thread_ = threads::CreateThread("TcpClientListener",
                                  new ListeningThreadDelegate(this));
}

TransportAdapter::Error TcpClientListener::Init() {
  LOGGER_AUTO_TRACE(logger_);
  thread_stop_requested_ = false;

  return TransportAdapter::OK;
}

void TcpClientListener::Terminate() {
  LOGGER_AUTO_TRACE(logger_);
}

bool TcpClientListener::IsInitialised() const {
  return thread_;
}

TcpClientListener::~TcpClientListener() {
  LOGGER_AUTO_TRACE(logger_);
  StopListening();
  delete thread_->delegate();
  threads::DeleteThread(thread_);
  Terminate();
}

void TcpClientListener::Loop() {
  LOGGER_AUTO_TRACE(logger_);
  utils::TcpServerSocket server_socket;
  // Moved from init for compatibility with the Qt
  const int kBacklog = 128;
  const utils::HostAddress address(utils::SpecialAddress::Any);
  if (!server_socket.Listen(address, port_, kBacklog)) {
    LOGGER_ERROR(logger_,
                 "Failed to listen on " << address.ToString() << ":" << port_);
    return;
  }
  while (!thread_stop_requested_) {
    // Wait for the new connection
    utils::TcpSocketConnection client_connection = server_socket.Accept();

    if (thread_stop_requested_) {
      LOGGER_DEBUG(logger_, "thread_stop_requested_");
      client_connection.Close();
      break;
    }

    if (!client_connection.IsValid()) {
      LOGGER_ERROR(logger_, "Failed to accept new client connection");
      continue;
    }

    const utils::HostAddress client_address = client_connection.GetAddress();
    LOGGER_INFO(logger_,
                "Connected client " << client_address.ToString() << ":"
                                    << client_connection.GetPort());

    if (enable_keepalive_) {
      client_connection.EnableKeepalive();
    }

    TcpDevice* tcp_device =
        new TcpDevice(client_address, client_address.ToString());
    DeviceSptr device = controller_->AddDevice(tcp_device);
    tcp_device = static_cast<TcpDevice*>(device.get());
    const ApplicationHandle app_handle =
        tcp_device->AddApplication(client_connection.GetPort(), true);

    TcpSocketConnection* connection(new TcpSocketConnection(
        device->unique_device_id(), app_handle, controller_));
    // Ownership on socket is transfered to connection
    connection->SetSocket(client_connection);
    const TransportAdapter::Error error = connection->Start();
    if (error != TransportAdapter::OK) {
      delete connection;
    }
  }
  if (server_socket.Close()) {
    LOGGER_DEBUG(logger_, "Server socket successfully closed");
  } else {
    LOGGER_ERROR(logger_, "Failed to close server socket");
  }
}

void TcpClientListener::StopLoop() {
  LOGGER_AUTO_TRACE(logger_);
  thread_stop_requested_ = true;
  // We need to connect to the listening socket to unblock accept() call
  // "0.0.0.0" is not valid address to connect to.
  utils::TcpSocketConnection byesocket;
  utils::HostAddress address(utils::SpecialAddress::LoopBack);
  if (!byesocket.Connect(address, port_)) {
    LOGGER_ERROR(logger_,
                 "Bye socket has failed to connect to the server "
                     << address.ToString() << ":" << port_);
  }
}

TransportAdapter::Error TcpClientListener::StartListening() {
  LOGGER_AUTO_TRACE(logger_);
  if (thread_->is_running()) {
    LOGGER_WARN(
        logger_,
        "TransportAdapter::BAD_STATE. Listener has already been started");
    return TransportAdapter::BAD_STATE;
  }

  if (!thread_->start()) {
    LOGGER_ERROR(logger_, "Tcp client listener thread start failed");
    return TransportAdapter::FAIL;
  }
  LOGGER_INFO(logger_, "Tcp client listener has started successfully");
  return TransportAdapter::OK;
}

void TcpClientListener::ListeningThreadDelegate::exitThreadMain() {
  parent_->StopLoop();
}

void TcpClientListener::ListeningThreadDelegate::threadMain() {
  parent_->Loop();
}

TcpClientListener::ListeningThreadDelegate::ListeningThreadDelegate(
    TcpClientListener* parent)
    : parent_(parent) {}

TransportAdapter::Error TcpClientListener::StopListening() {
  LOGGER_AUTO_TRACE(logger_);
  if (!thread_->is_running()) {
    LOGGER_DEBUG(logger_, "TcpClientListener is not running now");
    return TransportAdapter::BAD_STATE;
  }

  thread_->join();

  LOGGER_INFO(logger_, "Tcp client listener has stopped successfully");
  return TransportAdapter::OK;
}

}  // namespace transport_adapter
}  // namespace transport_manager

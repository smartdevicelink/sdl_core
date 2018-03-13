/*
 *
 * Copyright (c) 2017, Ford Motor Company
 * All rights reserved.
 *
 * Copyright (c) 2018 Xevo Inc.
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
 * Neither the name of the copyright holders nor the names of its contributors
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

#include <memory.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#ifdef __linux__
#include <linux/tcp.h>
#else  // __linux__
#include <sys/time.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/tcp_var.h>
#endif  // __linux__

#include <sstream>

#include "utils/logger.h"
#include "utils/make_shared.h"
#include "utils/threads/thread.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "transport_manager/tcp/network_interface_listener.h"
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
    , socket_(-1)
    , thread_stop_requested_(false) {
  thread_ = threads::CreateThread("TcpClientListener",
                                  new ListeningThreadDelegate(this));
  interface_listener_ = new NetworkInterfaceListener(this);
}

TransportAdapter::Error TcpClientListener::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  thread_stop_requested_ = false;

  socket_ = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == socket_) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create socket");
    return TransportAdapter::FAIL;
  }

  sockaddr_in server_address = {0};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int optval = 1;
  if (0 !=
      setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "setsockopt SO_REUSEADDR failed");
  }

  if (bind(socket_,
           reinterpret_cast<sockaddr*>(&server_address),
           sizeof(server_address)) != 0) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "bind() failed");
    return TransportAdapter::FAIL;
  }

  const int kBacklog = 128;
  if (0 != listen(socket_, kBacklog)) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "listen() failed");
    return TransportAdapter::FAIL;
  }

  if (!interface_listener_->Init()) {
    close(socket_);
    socket_ = -1;
    return TransportAdapter::FAIL;
  }
  return TransportAdapter::OK;
}

void TcpClientListener::Terminate() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (socket_ == -1) {
    LOG4CXX_WARN(logger_, "Socket has been closed");
    return;
  }
  if (shutdown(socket_, SHUT_RDWR) != 0) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to shutdown socket");
  }
  if (close(socket_) != 0) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to close socket");
  }
  socket_ = -1;

  interface_listener_->Deinit();
}

bool TcpClientListener::IsInitialised() const {
  return thread_;
}

TcpClientListener::~TcpClientListener() {
  LOG4CXX_AUTO_TRACE(logger_);
  StopListening();
  delete thread_->delegate();
  threads::DeleteThread(thread_);
  Terminate();
}

void SetKeepaliveOptions(const int fd) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(logger_, "fd: " << fd);
  int yes = 1;
  int keepidle = 3;  // 3 seconds to disconnection detecting
  int keepcnt = 5;
  int keepintvl = 1;
#ifdef __linux__
  int user_timeout = 7000;  // milliseconds
  if (0 != setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes))) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "setsockopt SO_KEEPALIVE failed");
  }
  if (0 !=
      setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle))) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "setsockopt TCP_KEEPIDLE failed");
  }
  if (0 !=
      setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt))) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "setsockopt TCP_KEEPCNT failed");
  }
  if (0 != setsockopt(
               fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl))) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "setsockopt TCP_KEEPINTVL failed");
  }
  if (0 != setsockopt(fd,
                      IPPROTO_TCP,
                      TCP_USER_TIMEOUT,
                      &user_timeout,
                      sizeof(user_timeout))) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "setsockopt TCP_USER_TIMEOUT failed");
  }
#elif defined(__QNX__)  // __linux__
  // TODO(KKolodiy): Out of order!
  const int kMidLength = 4;
  int mib[kMidLength];

  mib[0] = CTL_NET;
  mib[1] = AF_INET;
  mib[2] = IPPROTO_TCP;
  mib[3] = TCPCTL_KEEPIDLE;
  sysctl(mib, kMidLength, NULL, NULL, &keepidle, sizeof(keepidle));

  mib[0] = CTL_NET;
  mib[1] = AF_INET;
  mib[2] = IPPROTO_TCP;
  mib[3] = TCPCTL_KEEPCNT;
  sysctl(mib, kMidLength, NULL, NULL, &keepcnt, sizeof(keepcnt));

  mib[0] = CTL_NET;
  mib[1] = AF_INET;
  mib[2] = IPPROTO_TCP;
  mib[3] = TCPCTL_KEEPINTVL;
  sysctl(mib, kMidLength, NULL, NULL, &keepintvl, sizeof(keepintvl));

  struct timeval tval = {0};
  tval.tv_sec = keepidle;
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPALIVE, &tval, sizeof(tval));
#endif                  // __QNX__
}

void TcpClientListener::Loop() {
  LOG4CXX_AUTO_TRACE(logger_);
  while (!thread_stop_requested_) {
    sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    const int connection_fd = accept(
        socket_, (struct sockaddr*)&client_address, &client_address_size);
    if (thread_stop_requested_) {
      LOG4CXX_DEBUG(logger_, "thread_stop_requested_");
      close(connection_fd);
      break;
    }

    if (connection_fd < 0) {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "accept() failed");
      continue;
    }

    if (AF_INET != client_address.sin_family) {
      LOG4CXX_DEBUG(logger_, "Address of connected client is invalid");
      close(connection_fd);
      continue;
    }

    char device_name[32];
    strncpy(device_name,
            inet_ntoa(client_address.sin_addr),
            sizeof(device_name) / sizeof(device_name[0]));
    LOG4CXX_INFO(logger_, "Connected client " << device_name);
    LOG4CXX_INFO(logger_, "Port is: " << port_);

    if (enable_keepalive_) {
      SetKeepaliveOptions(connection_fd);
    }

    const auto device_uid =
        device_name + std::string(":") + std::to_string(port_);

#if defined(BUILD_TESTS)
    TcpDevice* tcp_device =
        new TcpDevice(client_address.sin_addr.s_addr, device_uid, device_name);
#else
    TcpDevice* tcp_device =
        new TcpDevice(client_address.sin_addr.s_addr, device_uid);
#endif  // BUILD_TESTS

    DeviceSptr device = controller_->AddDevice(tcp_device);
    tcp_device = static_cast<TcpDevice*>(device.get());
    const ApplicationHandle app_handle =
        tcp_device->AddIncomingApplication(connection_fd);

    utils::SharedPtr<TcpSocketConnection> connection =
        utils::MakeShared<TcpSocketConnection>(
            device->unique_device_id(), app_handle, controller_);
    controller_->ConnectionCreated(
        connection, device->unique_device_id(), app_handle);
    connection->set_socket(connection_fd);
    const TransportAdapter::Error error = connection->Start();
    if (TransportAdapter::OK != error) {
      LOG4CXX_ERROR(logger_,
                    "TCP connection::Start() failed with error: " << error);
    }
  }
}

void TcpClientListener::StopLoop() {
  LOG4CXX_AUTO_TRACE(logger_);
  thread_stop_requested_ = true;
  // We need to connect to the listening socket to unblock accept() call
  int byesocket = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server_address = {0};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_);
  server_address.sin_addr.s_addr = INADDR_ANY;
  if (0 != connect(byesocket,
                   reinterpret_cast<sockaddr*>(&server_address),
                   sizeof(server_address))) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "Failed to connect byesocket");
  } else {
    // Can only shutdown socket if connected
    if (0 != shutdown(byesocket, SHUT_RDWR)) {
      LOG4CXX_WARN_WITH_ERRNO(logger_, "Failed to shutdown byesocket");
    }
  }
  close(byesocket);
}

TransportAdapter::Error TcpClientListener::StartListening() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (thread_->is_running()) {
    LOG4CXX_WARN(
        logger_,
        "TransportAdapter::BAD_STATE. Listener has already been started");
    return TransportAdapter::BAD_STATE;
  }

  if (!interface_listener_->Start()) {
    return TransportAdapter::FAIL;
  }

  if (!thread_->start()) {
    LOG4CXX_ERROR(logger_, "Tcp client listener thread start failed");
    interface_listener_->Stop();
    return TransportAdapter::FAIL;
  }
  LOG4CXX_INFO(logger_, "Tcp client listener has started successfully");
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
  LOG4CXX_AUTO_TRACE(logger_);
  if (!thread_->is_running()) {
    LOG4CXX_DEBUG(logger_, "TcpClientListener is not running now");
    return TransportAdapter::BAD_STATE;
  }

  interface_listener_->Stop();

  thread_->join();

  LOG4CXX_INFO(logger_, "Tcp client listener has stopped successfully");
  return TransportAdapter::OK;
}

void TcpClientListener::OnIPAddressUpdated(const std::string ipv4_addr,
                                           const std::string ipv6_addr) {
  LOG4CXX_AUTO_TRACE(logger_);

  // Since we only create a TCP socket with IPv4 option (AF_INET), currently we
  // do not use IPv6 address.
  if (ipv4_addr != current_ip_address_) {
    current_ip_address_ = ipv4_addr;

    std::string enabled = !current_ip_address_.empty() ? "true" : "false";
    std::ostringstream oss;
    oss << port_;

    TransportConfig config;
    config.insert(std::make_pair(std::string("enabled"), enabled));
    config.insert(
        std::make_pair(std::string("tcp_ip_address"), current_ip_address_));
    config.insert(std::make_pair(std::string("tcp_port"), oss.str()));

    controller_->TransportConfigUpdated(config);
  }
}

}  // namespace transport_adapter
}  // namespace transport_manager

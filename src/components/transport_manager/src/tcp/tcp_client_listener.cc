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

#include <memory.h>
#include <signal.h>
#include <errno.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#ifdef __linux__
#  include <linux/tcp.h>
#else  // __linux__
#  include <sys/time.h>
#  include <netinet/in.h>
#  include <netinet/tcp.h>
#  include <netinet/tcp_var.h>
#endif  // __linux__

#include <sstream>

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
    : port_(port),
      enable_keepalive_(enable_keepalive),
      controller_(controller),
      thread_(0),
      socket_(-1),
      thread_stop_requested_(false) {
  thread_ = threads::CreateThread("TcpClientListener",
                                  new ListeningThreadDelegate(this));
}

TransportAdapter::Error TcpClientListener::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  thread_stop_requested_ = false;
  return TransportAdapter::OK;
}

void TcpClientListener::Terminate() {
    thread_->stop();
}

bool TcpClientListener::IsInitialised() const {
  return thread_;
}

TcpClientListener::~TcpClientListener() {
  LOG4CXX_AUTO_TRACE(logger_);
  thread_->join();
  delete thread_->delegate();
  threads::DeleteThread(thread_);
}

void SetKeepaliveOptions(const int fd) {
  LOG4CXX_TRACE(logger_, "enter. fd: " << fd);
  int yes = 1;
  int keepidle = 3;  // 3 seconds to disconnection detecting
  int keepcnt = 5;
  int keepintvl = 1;
#ifdef __linux__
  int user_timeout = 7000;  // milliseconds
  setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt));
  setsockopt(fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl));
  setsockopt(fd, IPPROTO_TCP, TCP_USER_TIMEOUT, &user_timeout,
             sizeof(user_timeout));
#elif defined(__QNX__)  // __linux__
  // TODO (KKolodiy): Out of order!
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
#endif  // __QNX__
  LOG4CXX_TRACE(logger_, "exit");
}

void TcpClientListener::Loop() {
  LOG4CXX_TRACE(logger_, "enter");
  while (!thread_stop_requested_) {
    sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    const int connection_fd = accept(socket_,
                                     (struct sockaddr*) &client_address,
                                     &client_address_size);
    if (thread_stop_requested_) {
      LOG4CXX_DEBUG(logger_, "thread_stop_requested_");
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
    strncpy(device_name, inet_ntoa(client_address.sin_addr),
            sizeof(device_name) / sizeof(device_name[0]));
    LOG4CXX_INFO(logger_, "Connected client " << device_name);

    if (enable_keepalive_) {
      SetKeepaliveOptions(connection_fd);
    }

    TcpDevice* tcp_device = new TcpDevice(client_address.sin_addr.s_addr,
                                          device_name);
    DeviceSptr device = controller_->AddDevice(tcp_device);
    tcp_device = static_cast<TcpDevice*>(device.get());
    const ApplicationHandle app_handle = tcp_device->AddIncomingApplication(
        connection_fd);

    TcpSocketConnection* connection(
        new TcpSocketConnection(device->unique_device_id(), app_handle,
                                controller_));
    connection->set_socket(connection_fd);
    const TransportAdapter::Error error = connection->Start();
    if (error != TransportAdapter::OK) {
      delete connection;
    }
  }
  LOG4CXX_TRACE(logger_, "exit");
}

void TcpClientListener::StopLoop() {
  LOG4CXX_AUTO_TRACE(logger_);
  thread_stop_requested_ = true;
  // We need to connect to the listening socket to unblock accept() call
  int byesocket = socket(AF_INET, SOCK_STREAM, 0);
  sockaddr_in server_address = { 0 };
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_);
  server_address.sin_addr.s_addr = INADDR_ANY;
  connect(byesocket, (sockaddr*) &server_address, sizeof(server_address));
  shutdown(byesocket, SHUT_RDWR);
  close(byesocket);
}

TransportAdapter::Error TcpClientListener::StartListening() {
  LOG4CXX_TRACE(logger_, "enter");
  if (!thread_ || thread_->is_running()) {
    LOG4CXX_TRACE(
        logger_,
        "exit with TransportAdapter::BAD_STATE. Condition: thread_started_");
    return TransportAdapter::BAD_STATE;
  }

  socket_ = socket(AF_INET, SOCK_STREAM, 0);

  if (-1 == socket_) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create socket");
    LOG4CXX_TRACE(logger_,
                  "exit with TransportAdapter::FAIL. Condition: -1 == socket_");
    return TransportAdapter::FAIL;
  }

  sockaddr_in server_address = { 0 };
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int optval = 1;
  setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  if (0 != bind(socket_, (sockaddr*) &server_address, sizeof(server_address))) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "bind() failed");
    LOG4CXX_TRACE(
        logger_,
        "exit with TransportAdapter::FAIL. Condition: 0 != bind(socket_, (sockaddr*) &server_address, sizeof(server_address))");
    return TransportAdapter::FAIL;
  }

  if (0 != listen(socket_, 128)) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "listen() failed");
    LOG4CXX_TRACE(
        logger_,
        "exit with TransportAdapter::FAIL. Condition: 0 != listen(socket_, 128)");
    return TransportAdapter::FAIL;
  }

  if (thread_->start()) {
    LOG4CXX_DEBUG(logger_, "Tcp client listener thread started");
  } else {
    LOG4CXX_ERROR(logger_, "Tcp client listener thread start failed");
    return TransportAdapter::FAIL;
  }
  LOG4CXX_TRACE(logger_, "exit with TransportAdapter::OK");
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
    : parent_(parent) {
}

TransportAdapter::Error TcpClientListener::StopListening() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!thread_ || !thread_->is_running()) {
    LOG4CXX_TRACE(logger_, "TcpClientListener is not running now");
    return TransportAdapter::BAD_STATE;
  }

  thread_->stop();
  close(socket_);
  socket_ = -1;
  return TransportAdapter::OK;
}

}  // namespace transport_adapter
}  // namespace transport_manager

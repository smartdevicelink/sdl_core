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
#include <fcntl.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/select.h>
#include <sys/sysctl.h>
#include <sys/socket.h>
#include <ifaddrs.h>
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
#include "transport_manager/tcp/network_interface_listener_impl.h"
#include "transport_manager/tcp/tcp_device.h"
#include "transport_manager/tcp/tcp_socket_connection.h"

namespace transport_manager {
namespace transport_adapter {

CREATE_LOGGERPTR_GLOBAL(logger_, "TransportManager")

static bool SetNonblocking(int s);

TcpClientListener::TcpClientListener(TransportAdapterController* controller,
                                     const uint16_t port,
                                     const bool enable_keepalive,
                                     const std::string designated_interface)
    : port_(port)
    , enable_keepalive_(enable_keepalive)
    , controller_(controller)
    , initialized_(false)
    , started_(false)
    , thread_(0)
    , socket_(-1)
    , thread_stop_requested_(false)
    , designated_interface_(designated_interface) {
  pipe_fds_[0] = pipe_fds_[1] = -1;
  thread_ = threads::CreateThread("TcpClientListener",
                                  new ListeningThreadDelegate(this));
  interface_listener_ =
      new NetworkInterfaceListenerImpl(this, designated_interface);
}

TransportAdapter::Error TcpClientListener::Init() {
  LOG4CXX_AUTO_TRACE(logger_);
  thread_stop_requested_ = false;

  if (!IsListeningOnSpecificInterface()) {
    // Network interface is not specified. We will listen on all interfaces
    // using INADDR_ANY. If socket creation fails, we will treat it an error.
    socket_ = CreateIPv4ServerSocket(port_);
    if (-1 == socket_) {
      LOG4CXX_ERROR(logger_, "Failed to create TCP socket");
      return TransportAdapter::FAIL;
    }
  } else {
    // Network interface is specified and we wiill listen only on the interface.
    // In this case, the server socket will be created once
    // NetworkInterfaceListener notifies the interface's IP address.
    LOG4CXX_INFO(logger_,
                 "TCP server socket will listen on "
                     << designated_interface_
                     << " once it has an IPv4 address.");
  }

  if (!interface_listener_->Init()) {
    if (socket_ >= 0) {
      close(socket_);
      socket_ = -1;
    }
    return TransportAdapter::FAIL;
  }

  initialized_ = true;
  return TransportAdapter::OK;
}

void TcpClientListener::Terminate() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!initialized_) {
    return;
  }

  if (!IsListeningOnSpecificInterface()) {
    DestroyServerSocket(socket_);
    socket_ = -1;
  } else {
    sync_primitives::AutoLock auto_lock(start_stop_lock_);
    DestroyServerSocket(socket_);
    socket_ = -1;
  }

  interface_listener_->Deinit();
  initialized_ = false;
}

bool TcpClientListener::IsInitialised() const {
  return initialized_;
}

TcpClientListener::~TcpClientListener() {
  LOG4CXX_AUTO_TRACE(logger_);
  StopListening();
  delete thread_->delegate();
  threads::DeleteThread(thread_);
  Terminate();
  delete interface_listener_;
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
  fd_set rfds;
  char dummy[16];

  while (!thread_stop_requested_) {
    FD_ZERO(&rfds);
    FD_SET(socket_, &rfds);
    FD_SET(pipe_fds_[0], &rfds);
    int nfds = socket_ > pipe_fds_[0] ? socket_ : pipe_fds_[0];

    int ret = select(nfds + 1, &rfds, NULL, NULL, NULL);
    if (ret < 0) {
      if (errno == EINTR) {
        continue;
      } else {
        LOG4CXX_WARN(logger_, "select failed for TCP server socket");
        break;
      }
    }

    if (FD_ISSET(pipe_fds_[0], &rfds)) {
      ret = read(pipe_fds_[0], dummy, sizeof(dummy));
      if (ret < 0) {
        if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK) {
          LOG4CXX_WARN(
              logger_,
              "Failed to read from pipe, aborting TCP server socket loop.");
          break;
        }
      } else if (ret == 0) {
        LOG4CXX_WARN(logger_,
                     "Pipe disconnected, aborting TCP server socket loop.");
        break;
      } else {
        LOG4CXX_DEBUG(logger_,
                      "received stop command of TCP server socket loop");
        break;
      }
    }

    if (FD_ISSET(socket_, &rfds)) {
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
      TcpDevice* tcp_device = new TcpDevice(
          client_address.sin_addr.s_addr, device_uid, device_name);
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

  LOG4CXX_INFO(logger_, "TCP server socket loop is terminated.");
}

void TcpClientListener::StopLoop() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (pipe_fds_[1] < 0) {
    LOG4CXX_WARN(logger_, "StopLoop called in invalid state");
    return;
  }

  thread_stop_requested_ = true;

  char dummy[1] = {0};
  int ret = write(pipe_fds_[1], dummy, sizeof(dummy));
  if (ret <= 0) {
    LOG4CXX_WARN_WITH_ERRNO(
        logger_, "Failed to send stop message to TCP server socket loop");
  }
}

TransportAdapter::Error TcpClientListener::StartListening() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (started_) {
    LOG4CXX_WARN(
        logger_,
        "TransportAdapter::BAD_STATE. Listener has already been started");
    return TransportAdapter::BAD_STATE;
  }

  if (!interface_listener_->Start()) {
    return TransportAdapter::FAIL;
  }

  if (!IsListeningOnSpecificInterface()) {
    TransportAdapter::Error ret = StartListeningThread();
    if (TransportAdapter::OK != ret) {
      LOG4CXX_ERROR(logger_, "Tcp client listener thread start failed");
      interface_listener_->Stop();
      return ret;
    }
  }

  started_ = true;
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
  if (!started_) {
    LOG4CXX_DEBUG(logger_, "TcpClientListener is not running now");
    return TransportAdapter::BAD_STATE;
  }

  interface_listener_->Stop();

  StopListeningThread();

  started_ = false;
  LOG4CXX_INFO(logger_, "Tcp client listener has stopped successfully");
  return TransportAdapter::OK;
}

TransportAdapter::Error TcpClientListener::StartListeningThread() {
  LOG4CXX_AUTO_TRACE(logger_);

  // StartListening() can be called from multiple threads
  sync_primitives::AutoLock auto_lock(start_stop_lock_);

  if (pipe_fds_[0] < 0 || pipe_fds_[1] < 0) {
    // recreate the pipe every time, so that the thread loop will not get
    // leftover
    // data inside pipe after it is started
    if (pipe(pipe_fds_) != 0) {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create internal pipe");
      return TransportAdapter::FAIL;
    }
    if (!SetNonblocking(pipe_fds_[0])) {
      LOG4CXX_WARN(logger_, "Failed to configure pipe to non-blocking");
    }
  }

  thread_stop_requested_ = false;

  if (!thread_->start()) {
    return TransportAdapter::FAIL;
  }
  return TransportAdapter::OK;
}

TransportAdapter::Error TcpClientListener::StopListeningThread() {
  LOG4CXX_AUTO_TRACE(logger_);

  // StopListening() can be called from multiple threads
  sync_primitives::AutoLock auto_lock(start_stop_lock_);

  thread_->join();

  close(pipe_fds_[1]);
  pipe_fds_[1] = -1;
  close(pipe_fds_[0]);
  pipe_fds_[0] = -1;

  return TransportAdapter::OK;
}

void TcpClientListener::OnIPAddressUpdated(const std::string ipv4_addr,
                                           const std::string ipv6_addr) {
  LOG4CXX_AUTO_TRACE(logger_);

  // Since we only create a TCP socket with IPv4 option (AF_INET), currently we
  // do not use IPv6 address.
  if (ipv4_addr != current_ip_address_) {
    if (IsListeningOnSpecificInterface()) {
      if (!current_ip_address_.empty()) {
        // the server socket is running, terminate it
        LOG4CXX_DEBUG(logger_,
                      "Stopping current TCP server socket on "
                          << designated_interface_);
        StopOnNetworkInterface();
      }
      if (!ipv4_addr.empty()) {
        // start (or restart) server socket with the new IP address
        LOG4CXX_DEBUG(
            logger_, "Starting TCP server socket on " << designated_interface_);
        StartOnNetworkInterface();
      }
    }

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

bool TcpClientListener::StartOnNetworkInterface() {
  LOG4CXX_AUTO_TRACE(logger_);

  // this method is only for the case that network interface is specified
  if (IsListeningOnSpecificInterface()) {
    {
      // make sure that two threads will not update socket_ at the same time
      sync_primitives::AutoLock auto_lock(start_stop_lock_);
      if (socket_ < 0) {
        socket_ = CreateIPv4ServerSocket(port_, designated_interface_);
        if (-1 == socket_) {
          LOG4CXX_WARN(logger_, "Failed to create TCP socket");
          return false;
        }
      }
    }

    if (TransportAdapter::OK != StartListeningThread()) {
      LOG4CXX_WARN(logger_, "Failed to start TCP client listener");
      return false;
    }
    LOG4CXX_INFO(logger_,
                 "TCP server socket started on " << designated_interface_);
  }
  return true;
}

bool TcpClientListener::StopOnNetworkInterface() {
  LOG4CXX_AUTO_TRACE(logger_);

  if (IsListeningOnSpecificInterface()) {
    if (TransportAdapter::OK != StopListeningThread()) {
      LOG4CXX_WARN(logger_, "Failed to stop TCP client listener");
      return false;
    }

    {
      sync_primitives::AutoLock auto_lock(start_stop_lock_);
      DestroyServerSocket(socket_);
      socket_ = -1;
    }

    LOG4CXX_INFO(logger_,
                 "TCP server socket on " << designated_interface_
                                         << " stopped");
  }
  return true;
}

bool TcpClientListener::IsListeningOnSpecificInterface() const {
  return !designated_interface_.empty();
}

int TcpClientListener::CreateIPv4ServerSocket(
    uint16_t port, const std::string interface_name) {
  LOG4CXX_AUTO_TRACE(logger_);

  struct in_addr ipv4_address;
  memset(&ipv4_address, 0, sizeof(ipv4_address));
  if (interface_name.empty()) {
    ipv4_address.s_addr = htonl(INADDR_ANY);
  } else if (!GetIPv4Address(interface_name, &ipv4_address)) {
    return -1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sock) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create socket");
    return -1;
  }

  sockaddr_in server_address = {0};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr = ipv4_address;

  int optval = 1;
  if (0 !=
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) {
    LOG4CXX_WARN_WITH_ERRNO(logger_, "setsockopt SO_REUSEADDR failed");
  }

  if (bind(sock,
           reinterpret_cast<sockaddr*>(&server_address),
           sizeof(server_address)) != 0) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "bind() failed");
    close(sock);
    return -1;
  }

  const int kBacklog = 128;
  if (0 != listen(sock, kBacklog)) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "listen() failed");
    close(sock);
    return -1;
  }

  return sock;
}

void TcpClientListener::DestroyServerSocket(int sock) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (sock >= 0) {
    if (shutdown(sock, SHUT_RDWR) != 0) {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to shutdown socket");
    }
    if (close(sock) != 0) {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to close socket");
    }
  }
}

bool TcpClientListener::GetIPv4Address(const std::string interface_name,
                                       struct in_addr* ip_address) {
  LOG4CXX_AUTO_TRACE(logger_);

#ifdef BUILD_TESTS
  // return a dummy address of INADDR_LOOPBACK
  struct in_addr dummy_addr;
  dummy_addr.s_addr = htonl(INADDR_LOOPBACK);

  if (ip_address != NULL) {
    *ip_address = dummy_addr;
  }
  return true;
#endif

  struct ifaddrs* if_list;
  if (getifaddrs(&if_list) != 0) {
    LOG4CXX_WARN(logger_, "getifaddrs failed");
    return false;
  }

  struct ifaddrs* interface;
  bool found = false;

  for (interface = if_list; interface != NULL;
       interface = interface->ifa_next) {
    if (interface->ifa_name == NULL) {
      continue;
    }
    if (interface_name == interface->ifa_name) {
      switch (interface->ifa_addr->sa_family) {
        case AF_INET: {
          struct sockaddr_in* addr =
              reinterpret_cast<struct sockaddr_in*>(interface->ifa_addr);
          if (ip_address != NULL) {
            *ip_address = addr->sin_addr;
          }
          found = true;
          break;
        }
        default:
          break;
      }
    }
  }

  freeifaddrs(if_list);

  return found;
}

static bool SetNonblocking(int s) {
  int prev_flag = fcntl(s, F_GETFL, 0);
  if (prev_flag == -1) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to acquire socket flag");
    return false;
  }

  int ret = fcntl(s, F_SETFL, prev_flag | O_NONBLOCK);
  if (ret == -1) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_,
                             "Failed to configure socket to non-blocking");
    return false;
  }

  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager

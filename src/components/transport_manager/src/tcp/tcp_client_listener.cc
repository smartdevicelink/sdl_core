/*
 *
 * Copyright (c) 2017, Ford Motor Company
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

#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <ifaddrs.h>
#include <memory.h>
#include <signal.h>
#include <sys/select.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef __linux__
#include <linux/tcp.h>
#else  // __linux__
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <netinet/tcp_var.h>
#include <sys/sysctl.h>
#include <sys/time.h>
#endif  // __linux__

#include <sstream>

#include "utils/logger.h"

#include "transport_manager/tcp/network_interface_listener_impl.h"
#include "transport_manager/tcp/tcp_device.h"
#include "transport_manager/tcp/tcp_socket_connection.h"
#include "transport_manager/transport_adapter/transport_adapter_controller.h"
#include "utils/threads/thread.h"

namespace transport_manager {
namespace transport_adapter {

SDL_CREATE_LOG_VARIABLE("TransportManager")

static bool SetNonblocking(int s);

#ifdef BUILD_TESTS
bool TcpClientListener::testing_ = false;
#endif  // BUILD_TESTS

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
    , remove_devices_on_terminate_(false)
    , designated_interface_(designated_interface) {
  pipe_fds_[0] = pipe_fds_[1] = -1;
  thread_ = threads::CreateThread("TcpClientListener",
                                  new ListeningThreadDelegate(this));
  interface_listener_ =
      new NetworkInterfaceListenerImpl(this, designated_interface);
}

TransportAdapter::Error TcpClientListener::Init() {
  SDL_LOG_AUTO_TRACE();
  thread_stop_requested_.store(false);

  if (!IsListeningOnSpecificInterface()) {
    // Network interface is not specified. We will listen on all interfaces
    // using INADDR_ANY. If socket creation fails, we will treat it an error.
    socket_ = CreateIPv4ServerSocket(port_);
    if (-1 == socket_) {
      SDL_LOG_ERROR("Failed to create TCP socket");
      return TransportAdapter::FAIL;
    }
  } else {
    // Network interface is specified and we will listen only on the interface.
    // In this case, the server socket will be created once
    // NetworkInterfaceListener notifies the interface's IP address.
    SDL_LOG_INFO("TCP server socket will listen on "
                 << designated_interface_ << " once it has an IPv4 address.");
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
  SDL_LOG_AUTO_TRACE();

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
  SDL_LOG_AUTO_TRACE();
  StopListening();
  delete thread_->GetDelegate();
  threads::DeleteThread(thread_);
  Terminate();
  delete interface_listener_;
}

void SetKeepaliveOptions(const int fd) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG("fd: " << fd);
  // cppcheck-suppress unreadVariable
  int yes = 1;
  // cppcheck-suppress unreadVariable
  int keepidle = 3;  // 3 seconds to disconnection detecting
  // cppcheck-suppress unreadVariable
  int keepcnt = 5;
  // cppcheck-suppress unreadVariable
  int keepintvl = 1;
#ifdef __linux__
  int user_timeout = 7000;  // milliseconds
  if (0 != setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &yes, sizeof(yes))) {
    SDL_LOG_WARN_WITH_ERRNO("setsockopt SO_KEEPALIVE failed");
  }
  if (0 !=
      setsockopt(fd, IPPROTO_TCP, TCP_KEEPIDLE, &keepidle, sizeof(keepidle))) {
    SDL_LOG_WARN_WITH_ERRNO("setsockopt TCP_KEEPIDLE failed");
  }
  if (0 !=
      setsockopt(fd, IPPROTO_TCP, TCP_KEEPCNT, &keepcnt, sizeof(keepcnt))) {
    SDL_LOG_WARN_WITH_ERRNO("setsockopt TCP_KEEPCNT failed");
  }
  if (0 != setsockopt(
               fd, IPPROTO_TCP, TCP_KEEPINTVL, &keepintvl, sizeof(keepintvl))) {
    SDL_LOG_WARN_WITH_ERRNO("setsockopt TCP_KEEPINTVL failed");
  }
  if (0 != setsockopt(fd,
                      IPPROTO_TCP,
                      TCP_USER_TIMEOUT,
                      &user_timeout,
                      sizeof(user_timeout))) {
    SDL_LOG_WARN_WITH_ERRNO("setsockopt TCP_USER_TIMEOUT failed");
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
  SDL_LOG_AUTO_TRACE();
  fd_set rfds;
  char dummy[16];
  std::vector<DeviceUID> device_uid_list;

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
        SDL_LOG_WARN("select failed for TCP server socket");
        break;
      }
    }

    if (FD_ISSET(pipe_fds_[0], &rfds)) {
      ret = read(pipe_fds_[0], dummy, sizeof(dummy));
      if (ret < 0) {
        if (errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK) {
          SDL_LOG_WARN(
              "Failed to read from pipe, aborting TCP server socket loop.");
          break;
        }
      } else if (ret == 0) {
        SDL_LOG_WARN("Pipe disconnected, aborting TCP server socket loop.");
        break;
      } else {
        SDL_LOG_DEBUG("received stop command of TCP server socket loop");
        break;
      }
    }

    if (FD_ISSET(socket_, &rfds)) {
      sockaddr_in client_address;
      socklen_t client_address_size = sizeof(client_address);
      const int connection_fd = accept(
          socket_, (struct sockaddr*)&client_address, &client_address_size);
      if (thread_stop_requested_) {
        SDL_LOG_DEBUG("thread_stop_requested_");
        close(connection_fd);
        break;
      }

      if (connection_fd < 0) {
        SDL_LOG_ERROR_WITH_ERRNO("accept() failed");
        continue;
      }

      if (AF_INET != client_address.sin_family) {
        SDL_LOG_DEBUG("Address of connected client is invalid");
        close(connection_fd);
        continue;
      }

      char device_name[32];
      size_t size = sizeof(device_name) / sizeof(device_name[0]);
      strncpy(device_name, inet_ntoa(client_address.sin_addr), size);

      device_name[size - 1] = '\0';
      SDL_LOG_INFO("Connected client " << device_name);
      SDL_LOG_INFO("Port is: " << port_);

      if (enable_keepalive_) {
        SetKeepaliveOptions(connection_fd);
      }

      const auto device_uid =
          device_name + std::string(":") + std::to_string(port_);

#if defined(ENABLE_IAP2EMULATION)
      auto tcp_device = std::make_shared<TcpDevice>(
          client_address.sin_addr.s_addr, device_uid, device_name);
#else
      auto tcp_device = std::make_shared<TcpDevice>(
          client_address.sin_addr.s_addr, device_uid);
#endif  // ENABLE_IAP2EMULATION

      DeviceSptr device = controller_->AddDevice(tcp_device);
      auto tcp_device_raw = static_cast<TcpDevice*>(device.get());
      const ApplicationHandle app_handle =
          tcp_device_raw->AddIncomingApplication(connection_fd);

      std::shared_ptr<TcpSocketConnection> connection =
          std::make_shared<TcpSocketConnection>(
              device->unique_device_id(), app_handle, controller_);
      controller_->ConnectionCreated(
          connection, device->unique_device_id(), app_handle);
      connection->set_socket(connection_fd);
      const TransportAdapter::Error error = connection->Start();
      if (TransportAdapter::OK != error) {
        SDL_LOG_ERROR("TCP connection::Start() failed with error: " << error);
      } else {
        device_uid_list.push_back(device->unique_device_id());
      }
    }
  }

  if (remove_devices_on_terminate_) {
    for (std::vector<DeviceUID>::iterator it = device_uid_list.begin();
         it != device_uid_list.end();
         ++it) {
      controller_->DeviceDisconnected(*it, DisconnectDeviceError());
    }
  }
  SDL_LOG_INFO("TCP server socket loop is terminated.");
}

void TcpClientListener::StopLoop() {
  SDL_LOG_AUTO_TRACE();
  if (pipe_fds_[1] < 0) {
    SDL_LOG_WARN("StopLoop called in invalid state");
    return;
  }

  thread_stop_requested_.store(true);

  char dummy[1] = {0};
  int ret = write(pipe_fds_[1], dummy, sizeof(dummy));
  if (ret <= 0) {
    SDL_LOG_WARN_WITH_ERRNO(
        "Failed to send stop message to TCP server socket loop");
  }
}

TransportAdapter::Error TcpClientListener::StartListening() {
  SDL_LOG_AUTO_TRACE();
  if (started_) {
    SDL_LOG_WARN(
        "TransportAdapter::BAD_STATE. Listener has already been started");
    return TransportAdapter::BAD_STATE;
  }

  if (!interface_listener_->Start()) {
    return TransportAdapter::FAIL;
  }

  if (!IsListeningOnSpecificInterface()) {
    TransportAdapter::Error ret = StartListeningThread();
    if (TransportAdapter::OK != ret) {
      SDL_LOG_ERROR("Tcp client listener thread start failed");
      interface_listener_->Stop();
      return ret;
    }
  }

  started_ = true;
  SDL_LOG_INFO("Tcp client listener has started successfully");
  return TransportAdapter::OK;
}

TransportAdapter::Error TcpClientListener::ResumeListening() {
  SDL_LOG_AUTO_TRACE();

  interface_listener_->Init();
  StartListeningThread();
  started_ = true;

  SDL_LOG_INFO("Tcp client listener was resumed successfully");
  return TransportAdapter::OK;
}

TransportAdapter::Error TcpClientListener::StopListening() {
  SDL_LOG_AUTO_TRACE();
  if (!started_) {
    SDL_LOG_DEBUG("TcpClientListener is not running now");
    return TransportAdapter::BAD_STATE;
  }

  interface_listener_->Stop();

  StopListeningThread();

  started_ = false;
  SDL_LOG_INFO("Tcp client listener was stopped successfully");
  return TransportAdapter::OK;
}

TransportAdapter::Error TcpClientListener::SuspendListening() {
  SDL_LOG_AUTO_TRACE();
  if (!started_) {
    SDL_LOG_DEBUG("TcpClientListener is not running now");
    return TransportAdapter::BAD_STATE;
  }

  if (shutdown(socket_, SHUT_RDWR) != 0) {
    SDL_LOG_WARN("Socket was unable to be shutdowned");
  }

  if (close(socket_) != 0) {
    SDL_LOG_ERROR_WITH_ERRNO("Failed to close socket");
  }

  interface_listener_->Deinit();
  StopListeningThread();
  started_ = false;

  SDL_LOG_INFO("Tcp client listener was suspended");
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

TransportAdapter::Error TcpClientListener::StartListeningThread() {
  SDL_LOG_AUTO_TRACE();

  // StartListening() can be called from multiple threads
  sync_primitives::AutoLock auto_lock(start_stop_lock_);

  if (pipe_fds_[0] < 0 || pipe_fds_[1] < 0) {
    // recreate the pipe every time, so that the thread loop will not get
    // leftover data inside pipe after it is started
    if (pipe(pipe_fds_) != 0) {
      SDL_LOG_ERROR_WITH_ERRNO("Failed to create internal pipe");
      return TransportAdapter::FAIL;
    }
    if (!SetNonblocking(pipe_fds_[0])) {
      SDL_LOG_WARN("Failed to configure pipe to non-blocking");
    }
  }

  thread_stop_requested_.store(false);

  if (!thread_->Start()) {
    return TransportAdapter::FAIL;
  }
  return TransportAdapter::OK;
}

TransportAdapter::Error TcpClientListener::StopListeningThread() {
  SDL_LOG_AUTO_TRACE();

  // StopListening() can be called from multiple threads
  sync_primitives::AutoLock auto_lock(start_stop_lock_);

  thread_->Stop(threads::Thread::kThreadStopDelegate);

  close(pipe_fds_[1]);
  pipe_fds_[1] = -1;
  close(pipe_fds_[0]);
  pipe_fds_[0] = -1;

  return TransportAdapter::OK;
}

void TcpClientListener::OnIPAddressUpdated(const std::string ipv4_addr,
                                           const std::string ipv6_addr) {
  SDL_LOG_AUTO_TRACE();

  // Since we only create a TCP socket with IPv4 option (AF_INET), currently we
  // do not use IPv6 address.
  if (ipv4_addr != current_ip_address_) {
    if (IsListeningOnSpecificInterface()) {
      if (!current_ip_address_.empty()) {
        // the server socket is running, terminate it
        SDL_LOG_DEBUG("Stopping current TCP server socket on "
                      << designated_interface_);
        StopOnNetworkInterface();
      }
      if (!ipv4_addr.empty()) {
        // start (or restart) server socket with the new IP address
        SDL_LOG_DEBUG("Starting TCP server socket on "
                      << designated_interface_);
        StartOnNetworkInterface();
      }
    }

    current_ip_address_ = ipv4_addr;

    std::string enabled = !current_ip_address_.empty() ? "true" : "false";
    std::ostringstream oss;
    oss << port_;

    TransportConfig config;
    config.insert(std::make_pair(tc_enabled, enabled));
    config.insert(std::make_pair(tc_tcp_ip_address, current_ip_address_));
    config.insert(std::make_pair(tc_tcp_port, oss.str()));

    controller_->TransportConfigUpdated(config);
  }
}

bool TcpClientListener::StartOnNetworkInterface() {
  SDL_LOG_AUTO_TRACE();

  // this method is only for the case that network interface is specified
  if (IsListeningOnSpecificInterface()) {
    {
      // make sure that two threads will not update socket_ at the same time
      sync_primitives::AutoLock auto_lock(start_stop_lock_);
      if (socket_ < 0) {
        socket_ = CreateIPv4ServerSocket(port_, designated_interface_);
        if (-1 == socket_) {
          SDL_LOG_WARN("Failed to create TCP socket");
          return false;
        }
      }
    }

    remove_devices_on_terminate_.store(true);

    if (TransportAdapter::OK != StartListeningThread()) {
      SDL_LOG_WARN("Failed to start TCP client listener");
      return false;
    }
    SDL_LOG_INFO("TCP server socket started on " << designated_interface_);
  }
  return true;
}

bool TcpClientListener::StopOnNetworkInterface() {
  SDL_LOG_AUTO_TRACE();

  if (IsListeningOnSpecificInterface()) {
    if (TransportAdapter::OK != StopListeningThread()) {
      SDL_LOG_WARN("Failed to stop TCP client listener");
      return false;
    }

    {
      sync_primitives::AutoLock auto_lock(start_stop_lock_);
      DestroyServerSocket(socket_);
      socket_ = -1;
    }

    remove_devices_on_terminate_.store(false);

    SDL_LOG_INFO("TCP server socket on " << designated_interface_
                                         << " stopped");
  }
  return true;
}

bool TcpClientListener::IsListeningOnSpecificInterface() const {
  return !designated_interface_.empty();
}

int TcpClientListener::CreateIPv4ServerSocket(
    uint16_t port, const std::string interface_name) {
  SDL_LOG_AUTO_TRACE();

  struct in_addr ipv4_address;
  memset(&ipv4_address, 0, sizeof(ipv4_address));
  if (interface_name.empty()) {
    ipv4_address.s_addr = htonl(INADDR_ANY);
  } else if (!GetIPv4Address(interface_name, &ipv4_address)) {
    return -1;
  }

  int sock = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == sock) {
    SDL_LOG_ERROR_WITH_ERRNO("Failed to create socket");
    return -1;
  }

  sockaddr_in server_address = {0};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr = ipv4_address;

  int optval = 1;
  if (0 !=
      setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) {
    SDL_LOG_WARN_WITH_ERRNO("setsockopt SO_REUSEADDR failed");
  }

  if (bind(sock,
           reinterpret_cast<sockaddr*>(&server_address),
           sizeof(server_address)) != 0) {
    SDL_LOG_ERROR_WITH_ERRNO("bind() failed");
    close(sock);
    return -1;
  }

  const int kBacklog = 128;
  if (0 != listen(sock, kBacklog)) {
    SDL_LOG_ERROR_WITH_ERRNO("listen() failed");
    close(sock);
    return -1;
  }

  return sock;
}

void TcpClientListener::DestroyServerSocket(int sock) {
  SDL_LOG_AUTO_TRACE();
  if (sock >= 0) {
    if (shutdown(sock, SHUT_RDWR) != 0) {
      SDL_LOG_ERROR_WITH_ERRNO("Failed to shutdown socket");
    }
    if (close(sock) != 0) {
      SDL_LOG_ERROR_WITH_ERRNO("Failed to close socket");
    }
  }
}

bool TcpClientListener::GetIPv4Address(const std::string interface_name,
                                       struct in_addr* ip_address) {
  SDL_LOG_AUTO_TRACE();

#ifdef BUILD_TESTS
  if (testing_) {
    // don't actually call getifaddrs(), instead return a dummy address of
    // INADDR_LOOPBACK
    struct in_addr dummy_addr;
    dummy_addr.s_addr = htonl(INADDR_LOOPBACK);

    if (ip_address != NULL) {
      *ip_address = dummy_addr;
    }
    return true;
  }
#endif  // BUILD_TESTS

  struct ifaddrs* if_list;
  if (getifaddrs(&if_list) != 0) {
    SDL_LOG_WARN("getifaddrs failed");
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
      if (interface->ifa_addr == NULL) {
        continue;
      }
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
    SDL_LOG_ERROR_WITH_ERRNO("Failed to acquire socket flag");
    return false;
  }

  int ret = fcntl(s, F_SETFL, prev_flag | O_NONBLOCK);
  if (ret == -1) {
    SDL_LOG_ERROR_WITH_ERRNO("Failed to configure socket to non-blocking");
    return false;
  }

  return true;
}

}  // namespace transport_adapter
}  // namespace transport_manager

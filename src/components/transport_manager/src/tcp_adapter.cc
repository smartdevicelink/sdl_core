/**
 * \file tcp_adapter.cpp
 * \brief TcpDeviceAdapter class source file.
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

#include <memory.h>
#include <errno.h>

#include "transport_manager/tcp_adapter.h"
#include "transport_manager/device_adapter_impl.h"

namespace transport_manager {

namespace device_adapter {

TcpClientListener::TcpClientListener(DeviceAdapterController* controller,
                                     const uint16_t port)
    : port_(port),
      controller_(controller),
      thread_(),
      socket_(-1),
      thread_started_(false),
      shutdown_requested_(false),
      ready_(false) {
}

void* tcpClientListenerThread(void* data) {
  TcpClientListener* tcpClientListener = static_cast<TcpClientListener*>(data);
  assert(tcpClientListener != 0);
  tcpClientListener->thread();
  return 0;
}

DeviceAdapter::Error TcpClientListener::init() {
  socket_ = socket(AF_INET, SOCK_STREAM, 0);

  if (-1 == socket_) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "Failed to create socket");
    return DeviceAdapter::FAIL;
  }

  sockaddr_in server_address;
  memset(&server_address, 0, sizeof(server_address));
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port_);
  server_address.sin_addr.s_addr = INADDR_ANY;

  int optval = 1;
  setsockopt(socket_, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval));

  if (0 != bind(socket_, (sockaddr*) &server_address, sizeof(server_address))) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "bind() failed");
    return DeviceAdapter::FAIL;
  }

  if (0 != listen(socket_, 128)) {
    LOG4CXX_ERROR_WITH_ERRNO(logger_, "listen() failed");
    return DeviceAdapter::FAIL;
  }

  const int thread_start_error = pthread_create(&thread_, 0,
                                                &tcpClientListenerThread, this);
  if (0 == thread_start_error) {
    thread_started_ = true;
    LOG4CXX_INFO(logger_, "Tcp client listener thread started");
  } else {
    LOG4CXX_ERROR(
        logger_,
        "Tcp client listener thread start failed, error code " << thread_start_error);
    return DeviceAdapter::FAIL;
  }
  return DeviceAdapter::OK;
}

DeviceAdapter::Error TcpClientListener::acceptConnect(
    const DeviceHandle device_handle, const ApplicationHandle app_handle,
    const ConnectionId session_id) {
  DeviceSptr device = controller_->findDevice(device_handle);
  if (device.get() == 0)
    return DeviceAdapter::BAD_PARAM;
  TcpDevice* tcp_device = static_cast<TcpDevice*>(device.get());
  const int socket = tcp_device->getApplicationSocket(app_handle);
  if (socket == -1)
    return DeviceAdapter::BAD_PARAM;

  TcpSocketConnection* connection(
      new TcpSocketConnection(device_handle, app_handle, session_id,
                              controller_));
  connection->set_socket(socket);
  const DeviceAdapter::Error error = connection->start();
  if (error != DeviceAdapter::OK)
    delete connection;
  return error;
}

DeviceAdapter::Error TcpClientListener::declineConnect(
    const DeviceHandle device_handle, const ApplicationHandle app_handle) {
  DeviceSptr device = controller_->findDevice(device_handle);
  if (device.get() == 0)
    return DeviceAdapter::BAD_PARAM;
  TcpDevice* tcp_device = static_cast<TcpDevice*>(device.get());
  const int socket = tcp_device->getApplicationSocket(app_handle);
  if (socket == -1)
    return DeviceAdapter::BAD_PARAM;

  close(socket);
  return DeviceAdapter::OK;
}

void TcpClientListener::terminate() {
  shutdown_requested_ = true;
  if (socket_ != -1)
    shutdown(socket_, SHUT_RDWR);

  if (true == thread_started_) {
    pthread_join(thread_, 0);
    LOG4CXX_INFO(logger_, "Tcp client listener thread terminated");
  }
}

bool TcpClientListener::isInitialised() const {
  return ready_;
}

TcpClientListener::~TcpClientListener() {
}

void TcpClientListener::thread() {
  LOG4CXX_INFO(logger_, "Tcp client listener thread started");

  ready_ = true;
  while (false == shutdown_requested_) {
    sockaddr_in client_address;
    socklen_t client_address_size = sizeof(client_address);
    const int connection_fd = accept(socket_,
                                     (struct sockaddr*) &client_address,
                                     &client_address_size);

    if (connection_fd < 0) {
      LOG4CXX_ERROR_WITH_ERRNO(logger_, "accept() failed");
      continue;
    }

    if (AF_INET != client_address.sin_family) {
      LOG4CXX_ERROR(logger_, "Address of connected client is invalid");
      continue;
    }

    char device_name[32];
    strncpy(device_name, inet_ntoa(client_address.sin_addr),
            sizeof(device_name) / sizeof(device_name[0]));
    LOG4CXX_INFO(logger_, "Connected client " << device_name);

    TcpDevice* tcp_device = new TcpDevice(client_address.sin_addr, device_name);
    std::pair<DeviceHandle, DeviceSptr> device_pair = controller_->addDevice(
        tcp_device);
    DeviceHandle device_handle = device_pair.first;
    tcp_device = static_cast<TcpDevice*>(device_pair.second.get());
    const ApplicationHandle app_handle = tcp_device->addApplication(
        connection_fd);
    controller_->connectRequested(device_handle, app_handle);
  }

  LOG4CXX_INFO(logger_, "Tcp client listener thread finished");
}

TcpDevice::TcpDevice(const in_addr& in_addr, const char* name)
    : Device(name),
      in_addr_(in_addr) {
  pthread_mutex_init(&applications_mutex_, 0);
}

bool TcpDevice::isSameAs(const Device* other) const {
  const TcpDevice* other_tcp_device = static_cast<const TcpDevice*>(other);
  return other_tcp_device->in_addr_.s_addr == in_addr_.s_addr;
}

ApplicationList TcpDevice::getApplicationList() const {
  pthread_mutex_lock(&applications_mutex_);
  ApplicationList app_list(applications_.begin(), applications_.end());
  pthread_mutex_unlock(&applications_mutex_);
  return app_list;
}

ApplicationHandle TcpDevice::addApplication(const int socket) {
  pthread_mutex_lock(&applications_mutex_);
  applications_.insert(socket);
  pthread_mutex_unlock(&applications_mutex_);
  return socket;
}

void TcpDevice::removeApplication(const ApplicationHandle app_handle) {
  pthread_mutex_lock(&applications_mutex_);
  applications_.erase(app_handle);
  pthread_mutex_unlock(&applications_mutex_);
}

TcpDevice::~TcpDevice() {
  pthread_mutex_destroy(&applications_mutex_);
}

int TcpDevice::getApplicationSocket(const ApplicationHandle app_handle) const {
  return app_handle;
}

TcpSocketConnection::TcpSocketConnection(const DeviceHandle device_handle,
                                         const ApplicationHandle app_handle,
                                         const ConnectionId session_id,
                                         DeviceAdapterController* controller)
    : ThreadedSocketConnection(device_handle, app_handle, session_id,
                               controller) {
}

TcpSocketConnection::~TcpSocketConnection() {
}

bool TcpSocketConnection::establish(ConnectError** error) {
  return true;
}

TcpDeviceAdapter::TcpDeviceAdapter()
    : DeviceAdapterImpl(0, 0, new TcpClientListener(this, default_port)) {
}

TcpDeviceAdapter::~TcpDeviceAdapter() {
}

DeviceType TcpDeviceAdapter::getDeviceType() const {
  return "sdl-tcp";
}

}  // namespace device_adapter

}  // namespace transport_manager


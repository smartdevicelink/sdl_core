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
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
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
      shutdown_requested_(false) {
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

void TcpClientListener::terminate() {
  shutdown_requested_ = true;

  if (true == thread_started_) {
    /*FIXME
     pthread_mutex_lock(&device_scan_requested_mutex_);
     device_scan_requested_ = false;
     pthread_cond_signal(&device_scan_requested_cond_);
     pthread_mutex_unlock(&device_scan_requested_mutex_);
     LOG4CXX_INFO(logger_,
     "Waiting for bluetooth device scanner thread termination");
     */
    pthread_join(thread_, 0);
    LOG4CXX_INFO(logger_, "Tcp client listener thread terminated");
  }
}

TcpClientListener::~TcpClientListener() {
}

void TcpClientListener::thread() {
  LOG4CXX_INFO(logger_, "Tcp client listener thread started");

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

    bool isConnectionThreadStarted = false;

    char deviceName[32];

    strncpy(deviceName, inet_ntoa(client_address.sin_addr),
            sizeof(deviceName) / sizeof(deviceName[0]));

    LOG4CXX_INFO(logger_, "Connected client " << deviceName);

    tDeviceHandle deviceHandle = InvalidDeviceHandle;

    pthread_mutex_lock (&mDevicesMutex);

    for (tDeviceMap::const_iterator deviceIterator = mDevices.begin();
        deviceIterator != mDevices.end(); ++deviceIterator) {
      const STCPDevice * device =
          dynamic_cast<const STCPDevice *>(deviceIterator->second);

      if (0 != device) {
        if (clientAddress.sin_addr.s_addr == device->mAddress) {
          deviceHandle = deviceIterator->first;
          break;
        }
      }
    }

    pthread_mutex_unlock(&mDevicesMutex);

    if (InvalidDeviceHandle == deviceHandle) {
      deviceHandle = mHandleGenerator.generateNewDeviceHandle();
      STCPDevice * device = new STCPDevice(deviceName,
                                           clientAddress.sin_addr.s_addr);

      pthread_mutex_lock(&mDevicesMutex);

      if (true
          == mDevices.insert(std::make_pair(deviceHandle, device)).second) {
        device->mUniqueDeviceId = std::string("TCP-") + deviceName;

        LOG4CXX_INFO(
            logger_,
            "Added new device " << deviceHandle << ": " << device->mName << " (" << device->mUniqueDeviceId << ")");
      } else {
        LOG4CXX_ERROR(
            logger_,
            "Device handle " << deviceHandle << " is already present in devices map");

        deviceHandle = InvalidDeviceHandle;
        delete device;
        device = 0;
      }

      pthread_mutex_unlock(&mDevicesMutex);

      if (0 != device) {
        updateClientDeviceList();
      }
    }

    if (InvalidDeviceHandle != deviceHandle) {
      isConnectionThreadStarted = startConnection(
          new STCPConnection(deviceHandle, connectionFd,
                             clientAddress.sin_port));
    } else {
      LOG4CXX_ERROR(logger_, "Failed to insert new device into devices map");
    }
  }

  if (false == isConnectionThreadStarted) {
    close (connectionFd);
  }

  LOG4CXX_INFO(logger_, "Tcp client listener thread finished");
}

TcpDevice::TcpDevice(const int port, const char* name) {
}

bool TcpDevice::isSameAs(const Device* other) const {
}

ApplicationList TcpDevice::getApplicationList() const {
}

TcpSocketConnection::TcpSocketConnection(const DeviceHandle device_handle,
                                         const ApplicationHandle app_handle,
                                         const SessionID session_id,
                                         DeviceAdapterController* controller) {
}

TcpSocketConnection::~TcpSocketConnection() {
}

bool TcpSocketConnection::establish(ConnectError** error) {
}

TcpDeviceAdapter::TcpDeviceAdapter()
    : client_connection_listener_(new TcpClientListener) {
  setClientConnectionListener(client_connection_listener_.get());
}

TcpDeviceAdapter::~TcpDeviceAdapter() {
}

DeviceType TcpDeviceAdapter::getDeviceType() const {
  return "sdl-tcp";
}

}
// namespace device_adapter

}// namespace transport_manager


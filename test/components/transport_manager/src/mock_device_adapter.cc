/*
 * \file mock_device_adapter.cc
 * \brief 
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
#include <sys/socket.h>
#include <sys/un.h>

#include <list>

#include "transport_manager/mock_device_adapter.h"
#include "transport_manager/device_adapter_impl.h"

namespace test {
namespace components {
namespace transport_manager {

static void *mockDeviceWorker(void* p) {
  int socket_fd = *static_cast<int*>(p);
  delete static_cast<int*>(p);

  unsigned char *buf = new unsigned char[4096];

  size_t len = recv(socket_fd, buf, 4096, 0);
  send(socket_fd, buf, len, 0);

  return NULL;
}

void MockDeviceAdapter::MockDevice::start(int socket_fd) {
  pthread_create(&workerThread, NULL, mockDeviceWorker, new int(socket_fd));
}

DeviceType MockDeviceAdapter::getDeviceType() const {
  return "mock-adapter";
}

bool MockDeviceAdapter::isSearchDevicesSupported() const {
  return true;
}

bool MockDeviceAdapter::isServerOriginatedConnectSupported() const {
  return true;
}

bool MockDeviceAdapter::isClientOriginatedConnectSupported() const {
  return true;
}

ApplicationList MockDeviceAdapter::getApplicationList(const DeviceHandle device_handle) const {
  ApplicationList rc;
  rc.push_back(100);
  return rc;
}

void MockDeviceAdapter::connectionThread(Connection *connection) {
  assert(connection != 0);
  LOG4CXX_INFO(
      logger_,
      "Connection thread started for session " << connection->session_id());

  int fd[2];

  const DeviceHandle device_handle = connection->device_handle();
  const ApplicationHandle app_handle = connection->application_handle();
  const int session_id = connection->session_id();

  socketpair(AF_UNIX, SOCK_STREAM, 0, fd);

  MockDevice *device = dynamic_cast<MockDevice*>(devices_[device_handle]);
  device->start(fd[0]);
  connection->set_connection_socket(fd[1]);
  handleCommunication(connection);
}

void MockDeviceAdapter::mainThread() {
  while (!shutdown_flag_) {
      DeviceMap new_devices;
      DeviceVector discovered_devices;

      bool device_scan_requested = waitForDeviceScanRequest(0);

      if(device_scan_requested) {
        if (devices_.empty()) {
          devices_[1] = new MockDevice("my_device");
        }
        for(DeviceAdapterListenerList::iterator it = listeners_.begin(); it != listeners_.end(); ++it){
          (*it)->onSearchDeviceDone(this);
        }
        device_scan_requested_ = false;
      }
  }
}

MockDeviceAdapter::~MockDeviceAdapter() { }

}
}
}

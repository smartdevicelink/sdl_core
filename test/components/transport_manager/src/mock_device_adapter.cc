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

namespace {

  struct workerData_t{
    pthread_t tid;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    int sockfd;
    bool active;
  };

  workerData_t workerData[5];

  static void *mockDeviceWorker(void* p) {
    workerData_t *data = static_cast<workerData_t*>(p);

    unsigned char *buf = new unsigned char[4096];
    pthread_mutex_lock(&data->mutex);
    while(data->active) {
      pthread_cond_wait(&data->cond, &data->mutex);
      int len = recv(data->sockfd, buf, 4096, 0);
      if (len > 0) {
        send(data->sockfd, buf, len, 0);
      }
    }

    pthread_mutex_unlock(&data->mutex);
    delete[] buf;
    return NULL;
  }

  static void *mockDeviceListenerThreadRoutine(void *p) {
    test::components::transport_manager::MockDeviceAdapter::listenerData_t *data = static_cast<test::components::transport_manager::MockDeviceAdapter::listenerData_t*>(p);
    data->sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (data->sockfd == -1) {
      return NULL;
    }
    unlink("./mockDevice");
    sockaddr_un my_addr;
    memset(&my_addr, 0, sizeof(my_addr));
    strcpy(my_addr.sun_path, "./mockDevice");
    my_addr.sun_family = AF_UNIX;
    int res = bind(data->sockfd, reinterpret_cast<sockaddr*>(&my_addr), sizeof(my_addr));
    if (res == -1) {
      return NULL;
    }

    res = listen(data->sockfd, 5);

    for (int i = 0; i < 5; ++i) {
      workerData[i].active = true;
      workerData[i].sockfd = 0;
      pthread_mutex_init(&workerData[i].mutex, NULL);
      pthread_create(&workerData[i].tid, NULL, mockDeviceWorker, &workerData[i]);
    }

    pthread_barrier_wait(&data->barrier);
    while(data->active) {
      socklen_t addr_size;
      sockaddr peer_addr;

      int peer_socket = accept(data->sockfd, &peer_addr, &addr_size);

      if (peer_socket != 0) {
        for (int i = 0; i < 5; ++i) {
          if (workerData[i].sockfd == 0) {
            workerData[i].sockfd = peer_socket;
            pthread_cond_signal(&workerData[i].cond);
            break;
          }
        }
      }
    }

    return NULL;
  }
}

namespace test {
namespace components {
namespace transport_manager {

void MockDeviceAdapter::MockDevice::start() {
  listener.active = true;
  pthread_mutex_init(&listener.mutex, NULL);
  pthread_barrier_init(&listener.barrier, NULL, 2);
  pthread_mutex_lock(&listener.mutex);
  pthread_create(&workerThread, NULL, mockDeviceListenerThreadRoutine, &listener);
  pthread_barrier_wait(&listener.barrier);
}

void MockDeviceAdapter::MockDevice::stop() {
  listener.active = false;
  close(listener.sockfd);
  for (int i = 0; i < 5; ++i) {
    close(workerData[i].sockfd);
    workerData[i].active = false;
    pthread_cond_signal(&workerData[i].cond);
  }
  for (int i = 0; i < 5; ++i) {
    pthread_join(workerData[i].tid, NULL);
  }

  unlink("./mockDevice");
}

::transport_manager::device_adapter::DeviceType MockDeviceAdapter::getDeviceType() const {
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

void MockDeviceAdapter::connectionThread(::transport_manager::device_adapter::Connection *connection) {
  assert(connection != 0);
///*todo: uncomment afer test fix
//  const DeviceHandle device_handle = connection->device_handle();
//  const ApplicationHandle app_handle = connection->application_handle();
//  const int session_id = connection->session_id();

  int peer_sock = socket(AF_UNIX, SOCK_STREAM, 0);
  sockaddr_un my_addr;
  memset(&my_addr, 0, sizeof(my_addr));
  strcpy(my_addr.sun_path, "./mockDevice");
  my_addr.sun_family = AF_UNIX;
  int res = ::connect(peer_sock, reinterpret_cast<sockaddr*>(&my_addr), sizeof(my_addr));

  if (res == -1) {
    return;
  }
//  connection->set_connection_socket(peer_sock);

//  handleCommunication(connection);
}

void MockDeviceAdapter::mainThread() {
/*todo: uncomment after test fix
  while (!shutdown_flag_) {
      DeviceMap new_devices;
      DeviceVector discovered_devices;

      bool device_scan_requested = waitForDeviceScanRequest(0);

      if(device_scan_requested) {
        for(DeviceAdapterListenerList::iterator it = listeners_.begin(); it != listeners_.end(); ++it){
          (*it)->onSearchDeviceDone(this);
        }
        device_scan_requested_ = false;
      }
  }
*/
}

void MockDeviceAdapter::addDevice(const char *name) {
  static int devid = 100;
///*todo: uncomment afer test fix
  //  MockDevice* dev = new MockDevice(name);
//  dev->start();
//  devices_[dev->unique_device_id()] = dev;
}

MockDeviceAdapter::~MockDeviceAdapter() {
///*todo: uncomment afer test fix
  //  for (auto d : devices_) {
//    static_cast<MockDevice*>(d.second)->stop();
//  };
}
}
}
}

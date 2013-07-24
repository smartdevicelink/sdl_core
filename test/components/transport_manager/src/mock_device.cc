/*
 * \file mock_device.cc
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

#include <pthread.h>
#include <sys/socket.h>
#include <sys/un.h>

#include <transport_manager/mock_device.h>

namespace test {
namespace components {
namespace transport_manager {

namespace {

struct workerData_t {
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
  while (data->active) {
    pthread_cond_wait(&data->cond, &data->mutex);
    ssize_t len = recv(data->sockfd, buf, 4096, 0);
    if (len > 0) {
      send(data->sockfd, buf, len, 0);
    }
  }

  pthread_mutex_unlock(&data->mutex);
  delete[] buf;
  return NULL;
}

void *mockDeviceListenerThreadRoutine(void *p) {
  test::components::transport_manager::listenerData_t *data =
      static_cast<test::components::transport_manager::listenerData_t*>(p);
  data->sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (data->sockfd == -1) {
    return NULL;
  }
  unlink("./mockDevice");
  sockaddr_un my_addr;
  memset(&my_addr, 0, sizeof(my_addr));
  strcpy(my_addr.sun_path, "./mockDevice");
  my_addr.sun_family = AF_UNIX;
  int res = bind(data->sockfd, reinterpret_cast<sockaddr*>(&my_addr),
                 sizeof(my_addr));
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
  while (data->active) {
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

void MockDevice::start() {
  listener.active = true;
  pthread_mutex_init(&listener.mutex, NULL);
  pthread_barrier_init(&listener.barrier, NULL, 2);
  pthread_mutex_lock(&listener.mutex);
  pthread_create(&workerThread, NULL, mockDeviceListenerThreadRoutine,
                 &listener);
  pthread_barrier_wait(&listener.barrier);
}

void MockDevice::stop() {
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

bool MockDevice::isSameAs(const Device* other) const {
  return unique_device_id() == other->unique_device_id();
}

ApplicationList MockDevice::getApplicationList() const {
  return applications_;
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test


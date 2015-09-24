/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "include/mock_application.h"
#include "include/mock_device.h"

namespace {

struct workerData {
  int sockfd;
};

void *applicationWorker(void *p) {
  workerData *data = static_cast<workerData*>(p);
  char *buf = new char[2 * 1024 * 1024];
  ssize_t len;

  while (true) {
  len = recv(data->sockfd, buf, 2 * 1024 * 1024, 0);
  if (len == 0)
    break;
  send(data->sockfd, buf, len, 0);
  }
  delete[] buf;
  delete data;
  return NULL;
}

void * applicationListener(void *p) {
  using test::components::transport_manager::MockApplication;
  MockApplication *app = static_cast<MockApplication*>(p);

  unlink(app->socket_name().c_str());

  app->sockfd = socket(AF_UNIX, SOCK_STREAM, 0);
  if (app->sockfd == -1) {
    // TODO: indicate error
    return NULL;
  }
  sockaddr_un my_addr;
  memset(&my_addr, 0, sizeof(my_addr));
  strcpy(my_addr.sun_path, app->socket_name().c_str());
  my_addr.sun_family = AF_UNIX;
  int res = bind(app->sockfd, reinterpret_cast<sockaddr*>(&my_addr),
                 sizeof(my_addr));
  if (res == -1) {
    return NULL;
  }

  res = listen(app->sockfd, 5);
  if (res == -1) {
    return NULL;
  }

  pthread_mutex_lock(&app->ready_mutex);
  app->active = true;
  pthread_cond_signal(&app->ready_cond);
  pthread_mutex_unlock(&app->ready_mutex);

  while (app->active) {
    socklen_t addr_size;
    sockaddr peer_addr;

    int peer_socket = accept(app->sockfd, &peer_addr, &addr_size);
    if (peer_socket != -1) {
      pthread_t t;
      workerData* data = new workerData();
      data->sockfd = peer_socket;
      pthread_create(&t, NULL, &applicationWorker, data);
    }
  }

  unlink(app->socket_name().c_str());

  return NULL;
}
}

namespace test {
namespace components {
namespace transport_manager {

MockApplication::MockApplication(const MockDevice *device, ApplicationHandle id)
    : device(device),
      handle(id),
      workerThread(0),
      sockfd(-1),
      active(false) {
  std::ostringstream oss;
  oss << "mockDevice" << device->unique_device_id() << "-" << id;
  socket_name_ = oss.str();
}

void MockApplication::Start() {

  pthread_cond_init(&ready_cond, NULL);
  pthread_mutex_init(&ready_mutex, NULL);

  pthread_mutex_lock(&ready_mutex);
  pthread_create(&workerThread, NULL, &applicationListener, this);
  pthread_cond_wait(&ready_cond, &ready_mutex);
  pthread_mutex_unlock(&ready_mutex);
}

void MockApplication::Stop() {
  active = false;
  shutdown(sockfd, SHUT_RDWR);
  close(sockfd);
  pthread_join(workerThread, NULL);
}

}  // namespace transport_manager
}  // namespace components
}  // namespace test

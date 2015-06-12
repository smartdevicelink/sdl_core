/*
 * mock_application.h
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

#ifndef MOCK_APPLICATION_H_
#define MOCK_APPLICATION_H_

#include <pthread.h>

#include <sstream>
#include <string>

#include <transport_manager/common.h>
#include <transport_manager/transport_manager.h>
#include <transport_manager/transport_adapter/transport_adapter_impl.h>
#include <transport_manager/transport_manager_impl.h>
#include "mock_device_scanner.h"

namespace test {
namespace components {
namespace transport_manager {

class MockDevice;
using ::transport_manager::ApplicationHandle;

class MockApplication {
 public:
  const MockDevice *device;
  ApplicationHandle handle;
  pthread_t workerThread;
  pthread_cond_t ready_cond;
  pthread_mutex_t ready_mutex;
  int sockfd;
  bool active;
 private:
  std::string socket_name_;
 public:
  MockApplication(const MockDevice* device, ApplicationHandle id);
  void Start();
  void Stop();
  const std::string &socket_name() const {
    return socket_name_;
  }
};

}  // namespace transport_manager
}  // namespace components
}  // namespace test

#endif /* MOCK_APPLICATION_H_ */

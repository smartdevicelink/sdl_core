/*
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

#include <arpa/inet.h>
#include <net/if.h>
#include <time.h>

#include "gtest/gtest.h"
#include "transport_manager/tcp/network_interface_listener_impl.h"
#include "transport_manager/tcp/mock_tcp_client_listener.h"
#include "utils/test_async_waiter.h"
#include "utils/threads/thread.h"

namespace test {
namespace components {
namespace transport_manager_test {

namespace {
const long kThreadStartWaitMsec = 10;
const uint32_t kStartNotificationTimeoutMsec = 500;
}

using ::testing::_;
using ::testing::AtLeast;
using ::testing::SaveArg;

class NetworkInterfaceListenerTest : public ::testing::Test {
 public:
  NetworkInterfaceListenerTest()
      : interface_listener_impl_(NULL)
      , mock_tcp_client_listener_(NULL, 0, false, "") {}

  virtual ~NetworkInterfaceListenerTest() {}

 protected:
  struct InterfaceEntry {
    const char* name;
    const char* ipv4_address;
    const char* ipv6_address;
    unsigned int flags;
  };

  void Deinit() {
    delete interface_listener_impl_;
  }

  void SleepFor(long msec) const {
    if (msec > 0) {
      struct timespec ts = {0, msec * 1000 * 1000};
      nanosleep(&ts, NULL);
    }
  }

  NetworkInterfaceListenerImpl* interface_listener_impl_;
  MockTcpClientListener mock_tcp_client_listener_;
};

}  // namespace transport_manager_test
}  // namespace components
}  // namespace test

/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_VR_MODULE_TEST_INCLUDE_MOCK_CONNECTED_SOCKET_H_
#define SRC_COMPONENTS_VR_MODULE_TEST_INCLUDE_MOCK_CONNECTED_SOCKET_H_

#include "gmock/gmock.h"
#include "net/connected_socket.h"

namespace net {

class MockConnectedSocket : public ConnectedSocket {
 public:
  MockConnectedSocket() : ConnectedSocket(NET_AF_INET, "127.0.0.1", 8080) {}
  MOCK_METHOD4(set_opt,
      bool(Int32 level, Int32 optname, const void* optval, socklen_t optlen));
  MOCK_METHOD0(close,
      void());
  MOCK_METHOD0(shutdown,
      void());
  MOCK_METHOD1(set_blocking_mode,
      void(bool is_blocking));
  MOCK_METHOD3(send,
      ssize_t(const UInt8* buffer, size_t size, Int32 flags));
  MOCK_METHOD2(send,
      ssize_t(Int32 file_fd, Int32 flags));
  MOCK_METHOD3(recv,
      ssize_t(UInt8* buffer, size_t size, Int32 flags));
  MOCK_METHOD2(recv,
      ssize_t(struct msghdr* msg, Int32 flags));
  MOCK_METHOD0(connect,
      bool());
};

}  // namespace net

#endif  // SRC_COMPONENTS_VR_MODULE_TEST_INCLUDE_MOCK_CONNECTED_SOCKET_H_

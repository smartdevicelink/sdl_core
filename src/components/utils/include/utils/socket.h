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
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SOCKET_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SOCKET_H_

#include <cstdint>
#include <string>

#include "utils/host_address.h"
#include "utils/pimpl.h"
#include "utils/macro.h"

namespace utils {

class TcpServerSocket;

class TcpConnectionEventHandler {
 public:
  virtual ~TcpConnectionEventHandler() {}

  virtual void OnError(int error) = 0;

  virtual void OnData(const uint8_t* const buffer, std::size_t size) = 0;

  virtual void OnCanWrite() = 0;

  virtual void OnClose() = 0;
};

class TcpSocketConnection {
 public:
  TcpSocketConnection();

  ~TcpSocketConnection();

  /*
   * We should implement copy constructor and assignment operator
   * with const argument for TcpSocketConnection class because
   * Accept() method of TcpServerSocket requires them to be able
   * to return copy of TcpServerSocket instance
   */
  TcpSocketConnection(const TcpSocketConnection& rhs);

  TcpSocketConnection& operator=(const TcpSocketConnection& rhs);

  bool Send(const char* const buffer,
            const std::size_t size,
            std::size_t& bytes_written);

  bool Send(const uint8_t* const buffer,
            const std::size_t size,
            std::size_t& bytes_written);

  bool Close();

  bool IsValid() const;

  void EnableKeepalive();

  int GetNativeHandle();

  HostAddress GetAddress() const;

  uint16_t GetPort() const;

  bool Connect(const HostAddress& address, const uint16_t port);

  bool Attach(const int tcp_socket,
              const HostAddress& address,
              const uint16_t port);

  bool Notify();

  void Wait();

  void SetEventHandler(TcpConnectionEventHandler* event_handler);

 private:
  class Impl;

  explicit TcpSocketConnection(Impl* impl);

  friend class TcpServerSocket;

  Pimpl<Impl> impl_;

  static const int kKeepAliveTimeSec =
      3;  // 3 seconds to disconnection detecting

  static const int kKeepAliveIntervalSec = 1;
};

class TcpServerSocket {
 public:
  TcpServerSocket();

  ~TcpServerSocket();

  bool IsListening() const;

  bool Close();

  bool Listen(const HostAddress& address,
              const uint16_t port,
              const int backlog);

  TcpSocketConnection Accept();

 private:
  class Impl;
  Pimpl<Impl> impl_;
};

////////////////////////////////////////////////////////////////////////////////
/// Implementation
////////////////////////////////////////////////////////////////////////////////

inline bool TcpSocketConnection::Send(const uint8_t* const buffer,
                                      const std::size_t size,
                                      std::size_t& bytes_written) {
  return Send(reinterpret_cast<const char*>(buffer), size, bytes_written);
}

}  // namespace utils

#endif  // SRC_COMPONENTS_UTILS_INCLUDE_UTILS_SOCKET_H_

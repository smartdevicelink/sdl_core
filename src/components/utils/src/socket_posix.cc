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
#include "utils/socket.h"

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <poll.h>
#include <errno.h>
#include <fcntl.h>
#include <unistd.h>

#include "utils/macro.h"
#include "utils/pimpl_impl.h"
#include "utils/socket_utils.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

namespace {

bool CloseSocket(int& socket) {
  LOGGER_AUTO_TRACE(logger_);
  if (0 == socket) {
    LOGGER_DEBUG(logger_,
                 "Socket " << socket << " is not valid. Skip closing.");
    return true;
  }
  if (-1 != close(socket)) {
    LOGGER_WARN(logger_, "Failed to close socket " << socket << ": " << errno);
    return false;
  }
  socket = 0;
  return true;
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
/// utils::TcpSocketConnection::Impl
////////////////////////////////////////////////////////////////////////////////

class utils::TcpSocketConnection::Impl {
 public:
  Impl();

  explicit Impl(const int tcp_socket,
                const HostAddress& address,
                const uint16_t port);

  ~Impl();

  bool Send(const char* buffer,
            const std::size_t size,
            std::size_t& bytes_written);

  bool Close();

  bool IsValid() const;

  void EnableKeepalive();

  int GetNativeHandle();

  utils::HostAddress GetAddress() const;

  uint16_t GetPort() const;

  bool Connect(const HostAddress& address, const uint16_t port);

  bool Notify();

  void Wait();

  void SetEventHandler(TcpConnectionEventHandler* event_handler);

 private:
  void OnError(int error);

  void OnRead();

  void OnWrite();

  void OnClose();

  bool CreateNotifictionPipes();

  int tcp_socket_;

  int read_fd_;

  int write_fd_;

  HostAddress address_;

  uint16_t port_;

  TcpConnectionEventHandler* event_handler_;
};

utils::TcpSocketConnection::Impl::Impl()
    : tcp_socket_(0)
    , read_fd_(-1)
    , write_fd_(-1)
    , address_()
    , port_(0u)
    , event_handler_(NULL) {}

utils::TcpSocketConnection::Impl::Impl(const int tcp_socket,
                                       const HostAddress& address,
                                       const uint16_t port)
    : tcp_socket_(tcp_socket)
    , read_fd_(-1)
    , write_fd_(-1)
    , address_(address)
    , port_(port)
    , event_handler_(NULL) {
  if (!CreateNotifictionPipes()) {
    Close();
  }
}

utils::TcpSocketConnection::Impl::~Impl() {
  Close();
}

bool utils::TcpSocketConnection::Impl::CreateNotifictionPipes() {
  int fds[2];
  const int result = pipe(fds);

  if (0 != result) {
    LOGGER_ERROR(logger_, "pipe creation failed: " << errno);
    return false;
  }

  LOGGER_DEBUG(logger_, "pipe created");
  read_fd_ = fds[0];
  write_fd_ = fds[1];

  const int fcntl_ret =
      fcntl(read_fd_, F_SETFL, fcntl(read_fd_, F_GETFL) | O_NONBLOCK);
  if (0 != fcntl_ret) {
    LOGGER_ERROR(logger_, "fcntl failed: " << errno);
    return false;
  }
  return true;
}

bool utils::TcpSocketConnection::Impl::Send(const char* buffer,
                                            const std::size_t size,
                                            std::size_t& bytes_written) {
  bytes_written = 0u;
  if (!IsValid()) {
    LOGGER_ERROR(logger_, "Failed to send data socket is not valid");
    return false;
  }
  const int flags = MSG_NOSIGNAL;
  int written = send(tcp_socket_, buffer, size, flags);
  int socket_error = errno;
  if (-1 == written) {
    if (EAGAIN != socket_error && EWOULDBLOCK != socket_error) {
      LOGGER_ERROR(logger_, "Failed to send data: " << socket_error);
      return false;
    } else {
      return true;
    }
  }
  // Lets double chek written because we have signed to unsigned conversion
  DCHECK(written >= 0);
  bytes_written = static_cast<std::size_t>(written);
  LOGGER_DEBUG(logger_,
               "Sent " << written << " bytes to socket " << tcp_socket_);
  return true;
}

bool utils::TcpSocketConnection::Impl::Close() {
  if (!IsValid()) {
    LOGGER_DEBUG(logger_, "Connection is not valid. Nothing to close.");
    return true;
  }
  LOGGER_DEBUG(logger_,
               "Closing connection " << address_.ToString() << ":" << port_);

  // Possibly we're waiting on Wait. We have to interrupt this.
  Notify();

  if (-1 != read_fd_) {
    close(read_fd_);
  }
  if (-1 != write_fd_) {
    close(write_fd_);
  }

  return CloseSocket(tcp_socket_);
}

bool utils::TcpSocketConnection::Impl::IsValid() const {
  return tcp_socket_ != 0;
}

void utils::TcpSocketConnection::Impl::EnableKeepalive() {
  utils::EnableKeepalive(
      GetNativeHandle(), kKeepAliveTimeSec, kKeepAliveIntervalSec);
}

int utils::TcpSocketConnection::Impl::GetNativeHandle() {
  return tcp_socket_;
}

utils::HostAddress utils::TcpSocketConnection::Impl::GetAddress() const {
  return address_;
}

uint16_t utils::TcpSocketConnection::Impl::GetPort() const {
  return port_;
}

bool utils::TcpSocketConnection::Impl::Connect(const HostAddress& address,
                                               const uint16_t port) {
  if (IsValid()) {
    LOGGER_ERROR(logger_, "Already connected. Closing existing connection.");
    Close();
    return false;
  }
  int client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == client_socket) {
    LOGGER_ERROR(logger_, "Failed to create client socket. Error: " << errno);
    return false;
  }
  sockaddr_in server_address = {0};
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);
  server_address.sin_addr.s_addr = address.ToIp4Address(false);
  if (!connect(client_socket,
               reinterpret_cast<sockaddr*>(&server_address),
               sizeof(server_address)) == 0) {
    LOGGER_ERROR(logger_,
                 "Failed to connect to the server " << address.ToString() << ":"
                                                    << port
                                                    << ". Error: " << errno);
    CloseSocket(client_socket);
    return false;
  }
  if (!CreateNotifictionPipes()) {
    Close();
    return false;
  }
  tcp_socket_ = client_socket;
  address_ = address;
  port_ = port;
  return true;
}

bool utils::TcpSocketConnection::Impl::Notify() {
  if (-1 == write_fd_) {
    LOGGER_ERROR(logger_,
                 "Failed to wake up connection thread for connection "
                     << this << ". Error: " << errno);
    return false;
  }
  uint8_t buffer = 0;
  if (1 != write(write_fd_, &buffer, 1)) {
    LOGGER_ERROR(logger_,
                 "Failed to wake up connection thread for connection "
                     << this << ". Error: " << errno);
    return false;
  }
  return true;
}

void utils::TcpSocketConnection::Impl::SetEventHandler(
    TcpConnectionEventHandler* event_handler) {
  LOGGER_DEBUG(logger_, "Setting event handle to " << event_handler);
  event_handler_ = event_handler;
}

void utils::TcpSocketConnection::Impl::OnError(int error) {
  if (!event_handler_) {
    return;
  }
  event_handler_->OnError(error);
}

void utils::TcpSocketConnection::Impl::OnRead() {
  LOGGER_AUTO_TRACE(logger_);
  if (!event_handler_) {
    return;
  }
  const std::size_t buffer_size = 4096u;
  char buffer[buffer_size];
  int bytes_read = -1;

  do {
    bytes_read = recv(tcp_socket_, buffer, sizeof(buffer), MSG_DONTWAIT);
    if (bytes_read > 0) {
      LOGGER_DEBUG(logger_,
                   "Received " << bytes_read << " bytes from socket "
                               << tcp_socket_);
      uint8_t* casted_buffer = reinterpret_cast<uint8_t*>(buffer);
      event_handler_->OnData(casted_buffer, bytes_read);
    } else if (bytes_read < 0) {
      int socket_error = errno;
      if (EAGAIN != socket_error && EWOULDBLOCK != socket_error) {
        LOGGER_ERROR(logger_,
                     "recv() failed for connection "
                         << tcp_socket_ << ". Error: " << socket_error);
        OnError(socket_error);
        return;
      }
    } else {
      LOGGER_WARN(logger_,
                  "Socket " << tcp_socket_ << " closed by remote peer");
      OnError(errno);
      return;
    }
  } while (bytes_read > 0);
}

void utils::TcpSocketConnection::Impl::OnWrite() {
  if (!event_handler_) {
    return;
  }
  event_handler_->OnCanWrite();
}

void utils::TcpSocketConnection::Impl::OnClose() {
  if (!event_handler_) {
    return;
  }
  event_handler_->OnClose();
}

void utils::TcpSocketConnection::Impl::Wait() {
  if (!IsValid()) {
    LOGGER_ERROR(logger_, "Cannot wait. Not connected.");
    Close();
    return;
  }

  const nfds_t kPollFdsSize = 2;
  pollfd poll_fds[kPollFdsSize];
  poll_fds[0].fd = tcp_socket_;
  // TODO: Fix data race. frames_to_send_ should be protected
  poll_fds[0].events = POLLIN | POLLPRI;
  poll_fds[1].fd = read_fd_;
  poll_fds[1].events = POLLIN | POLLPRI;
  if (-1 == poll(poll_fds, kPollFdsSize, -1)) {
    LOGGER_ERROR(logger_,
                 "poll failed for the socket " << tcp_socket_
                                               << ". Error: " << errno);
    OnError(errno);
    return;
  }
  LOGGER_DEBUG(logger_,
               "poll is ok for the socket "
                   << tcp_socket_ << " revents0: " << std::hex
                   << poll_fds[0].revents << " revents1:" << std::hex
                   << poll_fds[1].revents);
  // error check
  if (0 != (poll_fds[1].revents & (POLLERR | POLLHUP | POLLNVAL))) {
    LOGGER_ERROR(logger_,
                 "Notification pipe for socket "
                     << tcp_socket_ << " terminated. Error: " << errno);
    OnError(errno);
    return;
  }
  if (poll_fds[0].revents & (POLLERR | POLLHUP | POLLNVAL)) {
    LOGGER_DEBUG(logger_, "Socket " << tcp_socket_ << " has terminated");
    OnClose();
    return;
  }

  // clear notifications in the notification pipe
  char buffer[256];
  ssize_t bytes_read = -1;
  do {
    bytes_read = read(read_fd_, buffer, sizeof(buffer));
  } while (bytes_read > 0);
  if ((bytes_read < 0) && (EAGAIN != errno)) {
    LOGGER_ERROR(logger_,
                 "Failed to clear notification pipe. Poll failed for socket "
                     << tcp_socket_ << ". Error: " << errno);
    OnError(errno);
    return;
  }

  // send data if possible
  if (poll_fds[1].revents & (POLLIN | POLLPRI)) {
    OnWrite();
    return;
  }

  // receive data
  if (poll_fds[0].revents & (POLLIN | POLLPRI)) {
    OnRead();
  }
}

////////////////////////////////////////////////////////////////////////////////
/// utils::TcpSocketConnection
////////////////////////////////////////////////////////////////////////////////

// Ctor&Dtor should be in the cc file
// to prevent inlining.
// Otherwise the compiler will try to inline
// and fail to find ctor of the Pimpl.
utils::TcpSocketConnection::TcpSocketConnection() {}

utils::TcpSocketConnection::~TcpSocketConnection() {}

utils::TcpSocketConnection::TcpSocketConnection(
    const TcpSocketConnection& rhs) {
  impl_ = const_cast<TcpSocketConnection&>(rhs).impl_;
}

utils::TcpSocketConnection& utils::TcpSocketConnection::operator=(
    const TcpSocketConnection& rhs) {
  impl_ = const_cast<TcpSocketConnection&>(rhs).impl_;
  return *this;
}

utils::TcpSocketConnection::TcpSocketConnection(Impl* impl) : impl_(impl) {
  DCHECK(impl);
}

bool utils::TcpSocketConnection::Send(const char* buffer,
                                      const std::size_t size,
                                      std::size_t& bytes_written) {
  return impl_->Send(buffer, size, bytes_written);
}

bool utils::TcpSocketConnection::Close() {
  return impl_->Close();
}

bool utils::TcpSocketConnection::IsValid() const {
  return impl_->IsValid();
}

void utils::TcpSocketConnection::EnableKeepalive() {
  impl_->EnableKeepalive();
}

int utils::TcpSocketConnection::GetNativeHandle() {
  return impl_->GetNativeHandle();
}

utils::HostAddress utils::TcpSocketConnection::GetAddress() const {
  return impl_->GetAddress();
}

uint16_t utils::TcpSocketConnection::GetPort() const {
  return impl_->GetPort();
}

bool utils::TcpSocketConnection::Connect(const HostAddress& address,
                                         const uint16_t port) {
  return impl_->Connect(address, port);
}

bool utils::TcpSocketConnection::Notify() {
  return impl_->Notify();
}

void utils::TcpSocketConnection::Wait() {
  impl_->Wait();
}

void utils::TcpSocketConnection::SetEventHandler(
    TcpConnectionEventHandler* event_handler) {
  impl_->SetEventHandler(event_handler);
}

////////////////////////////////////////////////////////////////////////////////
/// utils::ServerTcpSocket::Impl
////////////////////////////////////////////////////////////////////////////////

class utils::TcpServerSocket::Impl {
 public:
  explicit Impl();

  ~Impl();

  bool IsListening() const;

  bool Close();

  bool Listen(const HostAddress& address,
              const uint16_t port,
              const int backlog);

  TcpSocketConnection Accept();

 private:
  int server_socket_;

  bool is_listening_;
};

utils::TcpServerSocket::Impl::Impl()
    : server_socket_(0), is_listening_(false) {}

utils::TcpServerSocket::Impl::~Impl() {
  Close();
}

bool utils::TcpServerSocket::Impl::IsListening() const {
  return server_socket_ && is_listening_;
}

bool utils::TcpServerSocket::Impl::Close() {
  return CloseSocket(server_socket_);
}

bool utils::TcpServerSocket::Impl::Listen(const HostAddress& address,
                                          const uint16_t port,
                                          const int backlog) {
  LOGGER_AUTO_TRACE(logger_);
  if (IsListening()) {
    LOGGER_ERROR(logger_, "Cannot listen. Already listeneing.");
    return false;
  }

  int server_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (-1 == server_socket) {
    LOGGER_ERROR(logger_, "Failed to create server socket: " << errno);
    return false;
  }

  int optval = 1;
  if (-1 ==
      setsockopt(
          server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) {
    LOGGER_ERROR(logger_, "Unable to set sockopt: " << errno);
    return false;
  }

  struct sockaddr_in server_address = {0};
  server_address.sin_addr.s_addr = address.ToIp4Address(false);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);

  if (-1 == bind(server_socket,
                 reinterpret_cast<struct sockaddr*>(&server_address),
                 sizeof(server_address))) {
    LOGGER_ERROR(logger_, "Unable to bind: " << errno);
    return false;
  }

  LOGGER_DEBUG(logger_,
               "Start listening on " << address.ToString() << ":" << port);

  if (-1 == listen(server_socket, backlog)) {
    LOGGER_ERROR(logger_,
                 "Failed to listen on " << address.ToString() << ":" << port
                                        << ". Error: " << errno);
    return false;
  }

  server_socket_ = server_socket;
  is_listening_ = true;
  return true;
}

utils::TcpSocketConnection utils::TcpServerSocket::Impl::Accept() {
  LOGGER_AUTO_TRACE(logger_);

  struct sockaddr_in client_address = {0};
  int client_address_length = sizeof(client_address);
  int client_socket =
      accept(server_socket_,
             reinterpret_cast<sockaddr*>(&client_address),
             reinterpret_cast<socklen_t*>(&client_address_length));
  if (-1 == client_socket) {
    LOGGER_ERROR(logger_, "Failed to accept client socket: " << errno);
    return utils::TcpSocketConnection();
  }
  if (AF_INET != client_address.sin_family) {
    LOGGER_DEBUG(logger_,
                 "Address of the connected client is invalid. Not AF_INET.");
    CloseSocket(client_socket);
    return utils::TcpSocketConnection();
  }
  const HostAddress accepted_client_address(inet_ntoa(client_address.sin_addr));
  LOGGER_DEBUG(logger_,
               "Accepted new client connection "
                   << accepted_client_address.ToString() << ":"
                   << client_address.sin_port);
  return TcpSocketConnection(new TcpSocketConnection::Impl(
      client_socket, accepted_client_address, client_address.sin_port));
}

////////////////////////////////////////////////////////////////////////////////
/// utils::TcpServerSocket
////////////////////////////////////////////////////////////////////////////////

// Ctor&Dtor should be in the cc file
// to prevent inlining.
// Otherwise the compiler will try to inline
// and fail to find ctor of the Pimpl.
utils::TcpServerSocket::TcpServerSocket() {}

utils::TcpServerSocket::~TcpServerSocket() {}

bool utils::TcpServerSocket::IsListening() const {
  return impl_->IsListening();
}

bool utils::TcpServerSocket::Close() {
  return impl_->Close();
}

bool utils::TcpServerSocket::Listen(const HostAddress& address,
                                    const uint16_t port,
                                    const int backlog) {
  return impl_->Listen(address, port, backlog);
}

utils::TcpSocketConnection utils::TcpServerSocket::Accept() {
  return impl_->Accept();
}

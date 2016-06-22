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
#include "utils/winhdr.h"
#include "utils/macro.h"
#include "utils/pimpl_impl.h"
#include "utils/socket_utils.h"

SDL_CREATE_LOGGER("Utils")

namespace {

bool CloseSocket(SOCKET& socket) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("Closing socket " << socket);
  if (NULL == socket) {
    SDL_DEBUG("Socket " << socket << " is not valid. Skip closing.");
    return true;
  }
  if (SOCKET_ERROR == closesocket(socket)) {
    SDL_WARN("Failed to close socket " << socket << ": " << WSAGetLastError());
    return false;
  }
  socket = NULL;
  return true;
}

HANDLE CreateNotifyEvent() {
  SDL_AUTO_TRACE();
  HANDLE result = CreateEvent(NULL,   // no security attribute
                              true,   // is manual-reset event
                              false,  // initial state = non-signaled
                              NULL);  // unnamed event object
  DCHECK(result);
  return result;
}

}  // namespace

////////////////////////////////////////////////////////////////////////////////
/// utils::TcpSocketConnection::Impl
////////////////////////////////////////////////////////////////////////////////

class utils::TcpSocketConnection::Impl {
 public:
  Impl();

  Impl(const SOCKET tcp_socket,
       const HostAddress& address,
       const uint16_t port);

  ~Impl();

  bool Send(const char* buffer, std::size_t size, std::size_t& bytes_written);

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

  SOCKET tcp_socket_;

  HANDLE notify_event_;

  HostAddress address_;

  uint16_t port_;

  TcpConnectionEventHandler* event_handler_;
};

utils::TcpSocketConnection::Impl::Impl()
    : tcp_socket_(NULL)
    , notify_event_(NULL)
    , address_()
    , port_(0u)
    , event_handler_(NULL) {}

utils::TcpSocketConnection::Impl::Impl(const SOCKET tcp_socket,
                                       const HostAddress& address,
                                       const uint16_t port)
    : tcp_socket_(tcp_socket)
    , notify_event_(CreateNotifyEvent())
    , address_(address)
    , port_(port)
    , event_handler_(NULL) {
  // Set socket to non-block mode
  unsigned long socket_mode = 1;
  if (!ioctlsocket(tcp_socket_, FIONBIO, &socket_mode) == 0) {
    SDL_ERROR("Failed to set socket to non blocking mode. Error: "
              << WSAGetLastError());
    CloseSocket(tcp_socket_);
  }
}

utils::TcpSocketConnection::Impl::~Impl() {
  Close();
}

bool utils::TcpSocketConnection::Impl::Send(const char* const buffer,
                                            std::size_t size,
                                            std::size_t& bytes_written) {
  bytes_written = 0u;
  if (!IsValid()) {
    SDL_ERROR("Failed to send data socket is not valid");
    return false;
  }
  const int flags = 0;
  int written = send(tcp_socket_, buffer, size, flags);
  int socket_error = WSAGetLastError();
  if (SOCKET_ERROR == written) {
    if (WSAEWOULDBLOCK != socket_error) {
      SDL_ERROR("Failed to send data: " << socket_error);
      return false;
    } else {
      return true;
    }
  }
  // Lets double chek written because we have signed to unsigned conversion
  DCHECK(written >= 0);
  bytes_written = static_cast<size_t>(written);
  SDL_DEBUG("Sent " << written << " bytes to socket " << tcp_socket_);
  return true;
}

bool utils::TcpSocketConnection::Impl::Close() {
  if (!IsValid()) {
    SDL_DEBUG("Connection is not valid. Nothing to close.");
    return true;
  }
  SDL_DEBUG("Closing connection " << address_.ToString() << ":" << port_);

  // Possibly we're waiting on Wait. We have to interrupt this.
  Notify();

  const BOOL event_closed = CloseHandle(notify_event_);
  if (!event_closed) {
    SDL_WARN("Failed to close event handler");
  }

  const bool socket_closed = CloseSocket(tcp_socket_);
  if (!socket_closed) {
    SDL_WARN("Failed to close socket handler");
  }

  return event_closed && socket_closed;
}

bool utils::TcpSocketConnection::Impl::IsValid() const {
  return tcp_socket_ != NULL;
}

void utils::TcpSocketConnection::Impl::EnableKeepalive() {
  utils::EnableKeepalive(
      GetNativeHandle(), kKeepAliveTimeSec, kKeepAliveIntervalSec);
}

int utils::TcpSocketConnection::Impl::GetNativeHandle() {
  return static_cast<int>(tcp_socket_);
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
    SDL_ERROR("Already connected. Closing existing connection.");
    Close();
    return false;
  }
  SOCKET client_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (INVALID_SOCKET == client_socket) {
    SDL_ERROR("Failed to create client socket. Error: " << WSAGetLastError());
    return false;
  }
  sockaddr_in server_address = {0};
  server_address.sin_family = AF_INET;
  server_address.sin_port = static_cast<USHORT>(htons(port));
  server_address.sin_addr.s_addr = address.ToIp4Address(false);
  if (!connect(client_socket,
               reinterpret_cast<sockaddr*>(&server_address),
               sizeof(server_address)) == 0) {
    SDL_ERROR("Failed to connect to the server "
              << address.ToString() << ":" << port
              << ". Error: " << WSAGetLastError());
    CloseSocket(client_socket);
    return false;
  }
  notify_event_ = CreateNotifyEvent();
  tcp_socket_ = client_socket;
  address_ = address;
  port_ = port;
  return true;
}

void utils::TcpSocketConnection::Impl::OnError(int error) {
  if (!event_handler_) {
    return;
  }
  event_handler_->OnError(error);
}

void utils::TcpSocketConnection::Impl::OnRead() {
  SDL_AUTO_TRACE();
  if (!event_handler_) {
    return;
  }
  const std::size_t buffer_size = 4096u;
  char buffer[buffer_size];
  int bytes_read = -1;

  do {
    bytes_read = recv(tcp_socket_, buffer, sizeof(buffer), 0);
    if (bytes_read > 0) {
      SDL_DEBUG("Received " << bytes_read << " bytes from socket "
                            << tcp_socket_);
      uint8_t* casted_buffer = reinterpret_cast<uint8_t*>(buffer);
      event_handler_->OnData(casted_buffer, bytes_read);
    } else if (bytes_read < 0) {
      int socket_error = WSAGetLastError();
      if (bytes_read == SOCKET_ERROR && WSAEWOULDBLOCK != socket_error) {
        SDL_ERROR("recv() failed for connection "
                  << tcp_socket_ << ". Error: " << socket_error);
        OnError(socket_error);
        return;
      }
    } else {
      SDL_WARN("Socket " << tcp_socket_ << " closed by remote peer");
      OnError(WSAGetLastError());
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
    SDL_ERROR("Cannot wait. Not connected.");
    Close();
    return;
  }

  WSANETWORKEVENTS net_events;
  HANDLE socketEvent = WSACreateEvent();
  WSAEventSelect(tcp_socket_, socketEvent, FD_WRITE | FD_READ | FD_CLOSE);

  const int events_to_wait_count = 2;
  HANDLE events_to_wait[events_to_wait_count] = {socketEvent, notify_event_};

  DWORD waited_index = WSAWaitForMultipleEvents(
      events_to_wait_count, events_to_wait, false, WSA_INFINITE, false);

  // We've waited for events. We should reset the
  // notify_event_. So on the next enter to the Wait it will
  // be bkocked
  ResetEvent(notify_event_);

  const bool is_socket_event = (waited_index == WAIT_OBJECT_0);
  const bool is_notify_event = (waited_index == WAIT_OBJECT_0 + 1);

  if (is_socket_event || is_notify_event) {
    SDL_DEBUG("Waited event for the connection "
              << tcp_socket_ << ". Socket event: " << is_socket_event
              << ". Notify event: " << is_notify_event);
  } else {
    SDL_ERROR("Wait for socket or notification has failed with error: "
              << WSAGetLastError());
    OnError(WSAGetLastError());
    return;
  }
  bool is_can_write = false;
  if (is_socket_event) {
    if (WSAEnumNetworkEvents(tcp_socket_,
                             events_to_wait[waited_index - WAIT_OBJECT_0],
                             &net_events) == SOCKET_ERROR) {
      SDL_ERROR("Failed to enum socket events: " << WSAGetLastError());
      OnError(WSAGetLastError());
      return;
    }
    if (net_events.lNetworkEvents & FD_READ) {
      SDL_DEBUG("Network event: FD_READ");
      OnRead();
      return;
    }
    if (net_events.lNetworkEvents & FD_WRITE) {
      SDL_DEBUG("Network event: FD_WRITE");
      is_can_write = true;
    }
    if (net_events.lNetworkEvents & FD_CLOSE) {
      SDL_DEBUG("Network event: FD_CLOSE. "
                << "Connection " << this << " terminated");
      OnClose();
      return;
    }
  }

  // Means that we received notify, thus
  // caller have something to send.
  // Or we received FD_WRITE(can write)
  // event from the socket
  if (is_notify_event || is_can_write) {
    OnWrite();
  }
}

bool utils::TcpSocketConnection::Impl::Notify() {
  return SetEvent(notify_event_);
}

void utils::TcpSocketConnection::Impl::SetEventHandler(
    TcpConnectionEventHandler* event_handler) {
  SDL_DEBUG("Setting event handle to " << event_handler);
  event_handler_ = event_handler;
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

bool utils::TcpSocketConnection::Send(const char* const buffer,
                                      std::size_t size,
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

void utils::TcpSocketConnection::Wait() {
  impl_->Wait();
}

bool utils::TcpSocketConnection::Notify() {
  return impl_->Notify();
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
  Impl();

  ~Impl();

  bool IsListening() const;

  bool Close();

  bool Listen(const HostAddress& address,
              const uint16_t port,
              const int backlog);

  TcpSocketConnection Accept();

 private:
  SOCKET server_socket_;

  bool is_listening_;
};

utils::TcpServerSocket::Impl::Impl()
    : server_socket_(NULL), is_listening_(false) {}

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
  SDL_AUTO_TRACE();
  SDL_DEBUG("Start listening on " << address.ToString() << ":" << port);

  if (IsListening()) {
    SDL_ERROR("Cannot listen " << address.ToString() << ":" << port
                               << ". Already listeneing.");
    return false;
  }

  SOCKET server_socket = socket(AF_INET, SOCK_STREAM, 0);

  if (INVALID_SOCKET == server_socket) {
    SDL_ERROR("Failed to create server socket: " << WSAGetLastError());
    return false;
  }
  SDL_DEBUG("Created server socket " << socket);

  char optval = 1;
  if (SOCKET_ERROR ==
      setsockopt(
          server_socket, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) {
    SDL_ERROR(
        "Failed to to set sockopt SO_REUSEADDR. Error: " << WSAGetLastError());
    return false;
  }

  struct sockaddr_in server_address = {0};
  server_address.sin_addr.s_addr = address.ToIp4Address(false);
  server_address.sin_family = AF_INET;
  server_address.sin_port = htons(port);

  if (SOCKET_ERROR == bind(server_socket,
                           reinterpret_cast<struct sockaddr*>(&server_address),
                           sizeof(server_address))) {
    SDL_ERROR("Failed to bind to " << address.ToString() << ":" << port
                                   << ". Error: " << WSAGetLastError());
    return false;
  }

  if (SOCKET_ERROR == listen(server_socket, backlog)) {
    SDL_WARN("Failed to listen on " << address.ToString() << ":" << port
                                    << ". Error: " << WSAGetLastError());
    return false;
  }

  SDL_DEBUG("Listening on " << address.ToString() << ":" << port);

  server_socket_ = server_socket;
  is_listening_ = true;
  return true;
}

utils::TcpSocketConnection utils::TcpServerSocket::Impl::Accept() {
  SDL_AUTO_TRACE();

  struct sockaddr_in client_address = {0};
  int client_address_length = sizeof(client_address);
  SOCKET client_socket = accept(server_socket_,
                                reinterpret_cast<sockaddr*>(&client_address),
                                &client_address_length);
  if (SOCKET_ERROR == client_socket) {
    SDL_ERROR("Failed to accept client socket: " << WSAGetLastError());
    return utils::TcpSocketConnection();
  }
  if (AF_INET != client_address.sin_family) {
    SDL_ERROR("Address of the connected client is invalid. Not AF_INET.");
    CloseSocket(client_socket);
    return utils::TcpSocketConnection();
  }
  const HostAddress accepted_client_address(client_address.sin_addr.s_addr,
                                            false);
  SDL_DEBUG("Accepted new client connection "
            << client_socket << " " << accepted_client_address.ToString() << ":"
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

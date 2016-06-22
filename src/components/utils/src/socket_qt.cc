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

#include <QtNetwork>

#include "utils/macro.h"
#include "utils/pimpl_impl.h"
#include "utils/socket_utils.h"

SDL_CREATE_LOGGER("Utils")

namespace {

inline QHostAddress FromHostAddress(const utils::HostAddress& address) {
  return QHostAddress(address.ToIp4Address(true));
}

inline utils::HostAddress ToHostAddress(const QHostAddress& address) {
  return utils::HostAddress(address.toIPv4Address(), true);
}
}  // namespace

////////////////////////////////////////////////////////////////////////////////
/// utils::TcpSocketConnection::Impl
////////////////////////////////////////////////////////////////////////////////

class utils::TcpSocketConnection::Impl : public QObject {
  Q_OBJECT
 public:
  Impl();

  Impl(qintptr socketDescriptor,
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

  void SetEventHandler(TcpConnectionEventHandler* event_handler);

  void Copy(TcpSocketConnection& rhs);

  bool Notify();

  void Wait();

  Q_SIGNAL void DataReceivedSignal(QByteArray buffer, int bytes_read);

  Q_SIGNAL void NotifySignal();

 private:
  void OnError(int error);

  void OnRead();

  void OnWrite();

  void OnClose();

  void InitSocketSignals();

  Q_SLOT void OnCloseSlot();

  Q_SLOT void OnDataReceivedSlot(QByteArray buffer, int bytes_read);

  Q_SLOT void OnErrorSlot();

  HostAddress address_;

  int socket_descriptor_;

  QAtomicInt is_initiated_;

  QScopedPointer<QTcpSocket> tcp_socket_;

  QScopedPointer<QEventLoop> loop_;

  uint16_t port_;

  utils::TcpConnectionEventHandler* event_handler_;

  // Classes inherited of QObject must non copyable. See
  // http://doc.qt.io/qt-5/qobject.html#no-copy-constructor-or-assignment-operator
  Q_DISABLE_COPY(Impl)
};

utils::TcpSocketConnection::Impl::Impl()
    : QObject(NULL)
    , address_()
    , socket_descriptor_(NULL)
    , is_initiated_(0)
    , tcp_socket_(NULL)
    , event_handler_(NULL)
    , port_(0u) {}

utils::TcpSocketConnection::Impl::Impl(qintptr socket_descriptor,
                                       const HostAddress& address,
                                       const uint16_t port)
    : QObject(NULL)
    , address_(address)
    , socket_descriptor_(socket_descriptor)
    , is_initiated_(0)
    , tcp_socket_(NULL)
    , event_handler_(NULL)
    , port_(port) {}

utils::TcpSocketConnection::Impl::~Impl() {
  Close();
}

void utils::TcpSocketConnection::Impl::Copy(TcpSocketConnection& rhs) {
  socket_descriptor_ = rhs.GetNativeHandle();
  address_ = rhs.GetAddress();
  port_ = rhs.GetPort();
}

bool utils::TcpSocketConnection::Impl::Send(const char* buffer,
                                            const std::size_t size,
                                            std::size_t& bytes_written) {
  SDL_AUTO_TRACE();
  bytes_written = 0;
  if (!IsValid()) {
    SDL_WARN("Cannot send. Socket is not valid.");
    return false;
  }
  if (!tcp_socket_) {
    tcp_socket_.reset(new QTcpSocket());
    tcp_socket_->setSocketDescriptor(socket_descriptor_);
  }

  QByteArray data(buffer, size);
  qint64 written = tcp_socket_->write(data);
  if (written != -1) {
    tcp_socket_->flush();
    tcp_socket_->waitForBytesWritten();
    DCHECK(written >= 0)
    bytes_written = static_cast<std::size_t>(written);
  } else {
    SDL_WARN("Failed to send: " << tcp_socket_->errorString().toStdString());
    return false;
  }
  return true;
}

bool utils::TcpSocketConnection::Impl::Close() {
  SDL_AUTO_TRACE();
  if (!IsValid()) {
    SDL_DEBUG("Not valid. Exit Close");
    return true;
  }
  if (tcp_socket_) {
    socket_descriptor_ = NULL;
    tcp_socket_->close();
    tcp_socket_.reset();
  }
  return true;
}

bool utils::TcpSocketConnection::Impl::IsValid() const {
  return socket_descriptor_ != NULL;
}

void utils::TcpSocketConnection::Impl::EnableKeepalive() {
  utils::EnableKeepalive(
      GetNativeHandle(), kKeepAliveTimeSec, kKeepAliveIntervalSec);
}

int utils::TcpSocketConnection::Impl::GetNativeHandle() {
  return socket_descriptor_;
}

utils::HostAddress utils::TcpSocketConnection::Impl::GetAddress() const {
  if (!IsValid()) {
    return HostAddress(SpecialAddress::Any);
  }
  return address_;
}

uint16_t utils::TcpSocketConnection::Impl::GetPort() const {
  return port_;
}

bool utils::TcpSocketConnection::Impl::Connect(const HostAddress& address,
                                               const uint16_t port) {
  if (IsValid()) {
    Close();
  }

  tcp_socket_.reset(new QTcpSocket());
  tcp_socket_->connectToHost(FromHostAddress(address), port);
  return tcp_socket_->waitForConnected();
}

void utils::TcpSocketConnection::Impl::OnError(int error) {
  if (!event_handler_) {
    return;
  }
  event_handler_->OnError(error);
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

void utils::TcpSocketConnection::Impl::OnRead() {
  if (!event_handler_) {
    return;
  }
  int bytes_read = -1;
  QByteArray buffer;
  while (tcp_socket_->bytesAvailable() > 0) {
    buffer = tcp_socket_->readAll();
    bytes_read = buffer.size();
    if (bytes_read > 0) {
      SDL_DEBUG("Received " << bytes_read << " bytes from socket "
                            << socket_descriptor_);
      emit DataReceivedSignal(buffer, bytes_read);
    }
  }
}

void utils::TcpSocketConnection::Impl::InitSocketSignals() {
  connect(tcp_socket_.data(),
          SIGNAL(disconnected()),
          this,
          SLOT(OnCloseSlot()),
          Qt::DirectConnection);
  connect(tcp_socket_.data(),
          SIGNAL(error(QAbstractSocket::SocketError)),
          this,
          SLOT(OnErrorSlot()),
          Qt::DirectConnection);
  connect(this,
          SIGNAL(DataReceivedSignal(QByteArray, int)),
          this,
          SLOT(OnDataReceivedSlot(QByteArray, int)),
          Qt::DirectConnection);
  connect(tcp_socket_.data(),
          SIGNAL(readyRead()),
          loop_.data(),
          SLOT(quit()),
          Qt::DirectConnection);
  connect(this,
          SIGNAL(NotifySignal()),
          loop_.data(),
          SLOT(quit()),
          Qt::QueuedConnection);
}

void utils::TcpSocketConnection::Impl::Wait() {
  SDL_AUTO_TRACE();
  if (!IsValid()) {
    SDL_ERROR("Cannot wait. Not connected.");
    return;
  }
  if (!is_initiated_) {
    is_initiated_ = 1;
    tcp_socket_.reset(new QTcpSocket());
    tcp_socket_->setSocketDescriptor(socket_descriptor_);
    loop_.reset(new QEventLoop);
    InitSocketSignals();
  }

  loop_->exec();
  OnRead();
  OnWrite();
}

void utils::TcpSocketConnection::Impl::OnErrorSlot() {
  SDL_DEBUG("Socket error code:#["
            << tcp_socket_->error() << "]. "
            << tcp_socket_->errorString().toStdString().c_str());
  if (tcp_socket_->error() != QAbstractSocket::RemoteHostClosedError) {
    OnError(tcp_socket_->error());
  }
  loop_->exit();
}

void utils::TcpSocketConnection::Impl::OnCloseSlot() {
  OnClose();
  loop_->exit();
}

void utils::TcpSocketConnection::Impl::OnDataReceivedSlot(QByteArray buffer,
                                                          int bytes_read) {
  uint8_t* casted_buffer = reinterpret_cast<uint8_t*>(buffer.data());
  event_handler_->OnData(casted_buffer, bytes_read);
}

bool utils::TcpSocketConnection::Impl::Notify() {
  SDL_AUTO_TRACE();
  emit NotifySignal();
  return true;
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
  impl_->Copy(const_cast<TcpSocketConnection&>(rhs));
}

utils::TcpSocketConnection& utils::TcpSocketConnection::operator=(
    const TcpSocketConnection& rhs) {
  impl_->Copy(const_cast<TcpSocketConnection&>(rhs));
  return *this;
}

utils::TcpSocketConnection::TcpSocketConnection(Impl* impl) : impl_(impl) {
  DCHECK(impl);
}

bool utils::TcpSocketConnection::Send(const char* buffer,
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
  QTcpServer* server_socket_;
};

utils::TcpServerSocket::Impl::Impl() : server_socket_(NULL) {}

utils::TcpServerSocket::Impl::~Impl() {
  SDL_AUTO_TRACE();
  Close();
}

bool utils::TcpServerSocket::Impl::IsListening() const {
  return server_socket_ && server_socket_->isListening();
}

bool utils::TcpServerSocket::Impl::Close() {
  if (server_socket_) {
    server_socket_->close();
    delete server_socket_;
    server_socket_ = NULL;
  }
  return true;
}

bool utils::TcpServerSocket::Impl::Listen(const HostAddress& address,
                                          const uint16_t port,
                                          const int backlog) {
  SDL_AUTO_TRACE();
  SDL_DEBUG("Start listening on " << address.ToString() << ":" << port);

  if (server_socket_) {
    SDL_WARN("Cannot listen. server_socket_ is null");
    return false;
  }

  server_socket_ = new QTcpServer();

  server_socket_->setMaxPendingConnections(backlog);

  SDL_DEBUG("Start listening on " << address.ToString() << ":" << port);

  if (!server_socket_->listen(FromHostAddress(address), port)) {
    SDL_WARN("Failed to listen on "
             << address.ToString() << ":" << port
             << ". Error: " << server_socket_->errorString().toStdString());
    return false;
  }

  SDL_DEBUG("Listening on " << address.ToString() << ":" << port);
  return true;
}

utils::TcpSocketConnection utils::TcpServerSocket::Impl::Accept() {
  SDL_AUTO_TRACE();
  bool waited = server_socket_->waitForNewConnection(-1);
  if (!waited) {
    SDL_WARN("Failed to wait for the new connection: "
             << server_socket_->errorString().toStdString());
    return utils::TcpSocketConnection();
  }

  QTcpSocket* client_connection = server_socket_->nextPendingConnection();
  if (!client_connection) {
    SDL_WARN("Failed to get new connection: "
             << server_socket_->errorString().toStdString());
    return utils::TcpSocketConnection();
  }
  SDL_DEBUG("Accepted new client connection "
            << client_connection->peerAddress().toString().toStdString() << ":"
            << client_connection->peerPort());
  const HostAddress host_address =
      ToHostAddress(client_connection->peerAddress());
  const uint16_t port = static_cast<uint16_t>(client_connection->peerPort());
  return TcpSocketConnection(new TcpSocketConnection::Impl(
      client_connection->socketDescriptor(), host_address, port));
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

#include "socket_qt.moc"

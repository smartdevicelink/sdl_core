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
#include <string>
#include <cstdint>
#include <cstddef>
#include <QtNetwork>

#include "utils/winhdr.h"
#include "utils/pipe.h"
#include "utils/pimpl_impl.h"
#include "utils/logger.h"
#include "utils/file_system.h"

SDL_CREATE_LOGGER("Utils")

namespace {
const std::string kPlatformPipePrefix = "\\\\.\\pipe\\";
}  // namespace

namespace utils {

class Pipe::Impl {
 public:
  friend Pipe;

  Impl();
  ~Impl();

  bool Open();
  void Close();
  bool IsOpen() const;

  bool Write(const uint8_t* buffer,
             size_t bytes_to_write,
             size_t& bytes_written);

 private:
  QString name_;
  QLocalServer* server_socket_;
  QLocalSocket* client_socket_;
};

}  // namespace utils

////////////////////////////////////////////////////////////////////////////////
/// utils::Pipe::Impl
////////////////////////////////////////////////////////////////////////////////

utils::Pipe::Impl::Impl()
    : name_(), server_socket_(NULL), client_socket_(NULL) {}

utils::Pipe::Impl::~Impl() {
  Close();
}

bool utils::Pipe::Impl::Open() {
  if (IsOpen()) {
    SDL_WARN("Named pipe: " << name_.toStdString() << " is already opened");
    return true;
  }
  server_socket_ = new QLocalServer();
  server_socket_->setSocketOptions(QLocalServer::WorldAccessOption);
  if (!server_socket_->listen(name_)) {
    delete server_socket_;
    server_socket_ = NULL;
    SDL_ERROR("Cannot create named pipe: " << name_.toStdString());
    return false;
  }
  if (server_socket_->waitForNewConnection(-1)) {
    client_socket_ = server_socket_->nextPendingConnection();
  }
  if (!client_socket_) {
    delete server_socket_;
    server_socket_ = NULL;
    SDL_ERROR("Cannot connect to named pipe: " << name_.toStdString());
    return false;
  }
  return true;
}

void utils::Pipe::Impl::Close() {
  if (!IsOpen()) {
    SDL_WARN("Named pipe: " << name_.toStdString() << " is not opened");
    return;
  }
  if (server_socket_) {
    server_socket_->close();
  }
  delete client_socket_;
  client_socket_ = NULL;
  delete server_socket_;
  server_socket_ = NULL;
}

bool utils::Pipe::Impl::IsOpen() const {
  return NULL != server_socket_ && NULL != client_socket_;
}

bool utils::Pipe::Impl::Write(const uint8_t* buffer,
                              size_t bytes_to_write,
                              size_t& bytes_written) {
  bytes_written = 0;
  if (!IsOpen()) {
    SDL_ERROR("Named pipe: " << name_.toStdString() << " is not opened");
    return false;
  }
  if (bytes_to_write == 0) {
    SDL_WARN("Trying to write 0 bytes");
    return true;
  }
  qint64 written = client_socket_->write(reinterpret_cast<const char*>(buffer),
                                         static_cast<qint64>(bytes_to_write));
  if (-1 == written) {
    SDL_ERROR("Cannot write to named pipe: " << name_.toStdString());
    return false;
  }
  client_socket_->waitForBytesWritten();
  client_socket_->flush();
  bytes_written = static_cast<size_t>(written);
  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// utils::Pipe
////////////////////////////////////////////////////////////////////////////////

utils::Pipe::Pipe(const std::string& name) {
  impl_->name_ = (kPlatformPipePrefix +
                  file_system::RetrieveFileNameFromPath(name)).c_str();
}

bool utils::Pipe::Open() {
  return impl_->Open();
}

void utils::Pipe::Close() {
  impl_->Close();
}

bool utils::Pipe::IsOpen() const {
  return impl_->IsOpen();
}

bool utils::Pipe::Write(const uint8_t* buffer,
                        size_t bytes_to_write,
                        size_t& bytes_written) {
  return impl_->Write(buffer, bytes_to_write, bytes_written);
}

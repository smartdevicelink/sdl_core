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

#include "utils/winhdr.h"
#include "utils/pipe.h"
#include "utils/pimpl_impl.h"
#include "utils/logger.h"
#include "utils/file_system.h"

SDL_CREATE_LOGGER("Utils")

namespace {
const std::string kPlatformPipePrefix = "\\\\.\\pipe\\";
DWORD kInBufferSize = 1024;
DWORD kOutBufferSize = 1024;
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
  std::string name_;
  HANDLE handle_;
};

}  // namespace utils

////////////////////////////////////////////////////////////////////////////////
/// utils::Pipe::Impl
////////////////////////////////////////////////////////////////////////////////

utils::Pipe::Impl::Impl() : name_(), handle_(NULL) {}

utils::Pipe::Impl::~Impl() {
  Close();
}

bool utils::Pipe::Impl::Open() {
  if (IsOpen()) {
    SDL_WARN("Named pipe: " << name_ << " is already opened");
    return true;
  }
  handle_ = CreateNamedPipe(TEXT(name_.c_str()),
                            PIPE_ACCESS_DUPLEX,
                            PIPE_TYPE_BYTE | PIPE_READMODE_BYTE,
                            PIPE_UNLIMITED_INSTANCES,
                            kOutBufferSize,
                            kInBufferSize,
                            0,
                            NULL);
  if (INVALID_HANDLE_VALUE == handle_) {
    handle_ = NULL;
    SDL_ERROR("Cannot create named pipe: " << name_);
    return false;
  }
  if (0 == ConnectNamedPipe(handle_, NULL)) {
    CloseHandle(handle_);
    handle_ = NULL;
    SDL_ERROR("Cannot connect to named pipe: " << name_);
    return false;
  }
  return true;
}

void utils::Pipe::Impl::Close() {
  if (!IsOpen()) {
    SDL_WARN("Named pipe: " << name_ << " is not opened");
    return;
  }
  if (0 == DisconnectNamedPipe(handle_)) {
    SDL_WARN("Cannot disconnect from named pipe: " << name_);
  }
  if (0 == CloseHandle(handle_)) {
    SDL_WARN("Cannot delete named pipe: " << name_);
  }
  handle_ = NULL;
}

bool utils::Pipe::Impl::IsOpen() const {
  return NULL != handle_;
}

bool utils::Pipe::Impl::Write(const uint8_t* buffer,
                              size_t bytes_to_write,
                              size_t& bytes_written) {
  bytes_written = 0;
  if (!IsOpen()) {
    SDL_ERROR("Named pipe: " << name_ << " is not opened");
    return false;
  }
  if (bytes_to_write == 0) {
    SDL_WARN("Trying to write 0 bytes");
    return true;
  }
  DWORD written = 0;
  const BOOL result = WriteFile(handle_,
                                static_cast<LPCVOID>(buffer),
                                static_cast<DWORD>(bytes_to_write),
                                &written,
                                NULL);
  if (0 == result) {
    SDL_ERROR("Cannot write to named pipe: " << name_);
    return false;
  }
  bytes_written = static_cast<size_t>(written);
  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// utils::Pipe
////////////////////////////////////////////////////////////////////////////////

utils::Pipe::Pipe(const std::string& name) {
  impl_->name_ =
      kPlatformPipePrefix + file_system::RetrieveFileNameFromPath(name);
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

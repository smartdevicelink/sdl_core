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
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "utils/pipe.h"
#include "utils/pimpl_impl.h"
#include "utils/logger.h"

CREATE_LOGGERPTR_GLOBAL(logger_ptr, "Utils")

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
  int handle_;
};

}  // namespace utils

////////////////////////////////////////////////////////////////////////////////
/// utils::Pipe::Impl
////////////////////////////////////////////////////////////////////////////////

utils::Pipe::Impl::Impl() : name_(), handle_(0) {}

utils::Pipe::Impl::~Impl() {
  Close();
}

bool utils::Pipe::Impl::Open() {
  if (IsOpen()) {
    LOGGER_WARN(logger_ptr, "Named pipe: " << name_ << " is already opened");
    return true;
  }
  handle_ = mkfifo(name_.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
  if (-1 == handle_) {
    handle_ = 0;
    LOGGER_ERROR(logger_ptr, "Cannot create named pipe: " << name_);
    return false;
  }
  if (-1 == open(name_.c_str(), O_RDWR, 0)) {
    unlink(name_.c_str());
    handle_ = 0;
    LOGGER_ERROR(logger_ptr, "Cannot connect to named pipe: " << name_);
    return false;
  }
  return true;
}

void utils::Pipe::Impl::Close() {
  if (!IsOpen()) {
    LOGGER_WARN(logger_ptr, "Named pipe: " << name_ << " is not opened");
    return;
  }
  if (-1 == close(handle_)) {
    LOGGER_WARN(logger_ptr, "Cannot disconnect from named pipe: " << name_);
  }
  if (-1 == unlink(name_.c_str())) {
    LOGGER_WARN(logger_ptr, "Cannot delete named pipe: " << name_);
  }
  handle_ = 0;
}

bool utils::Pipe::Impl::IsOpen() const {
  return 0 != handle_;
}

bool utils::Pipe::Impl::Write(const uint8_t* buffer,
                              size_t bytes_to_write,
                              size_t& bytes_written) {
  bytes_written = 0;
  if (!IsOpen()) {
    LOGGER_ERROR(logger_ptr, "Named pipe: " << name_ << " is not opened");
    return false;
  }
  if (bytes_to_write == 0) {
    LOGGER_WARN(logger_ptr, "Trying to write 0 bytes");
    return true;
  }
  const ssize_t written =
      write(handle_, static_cast<const void*>(buffer), bytes_to_write);
  if (-1 == written) {
    LOGGER_ERROR(logger_ptr, "Cannot write to named pipe: " << name_);
    return false;
  }
  bytes_written = static_cast<size_t>(written);
  return true;
}

////////////////////////////////////////////////////////////////////////////////
/// utils::Pipe
////////////////////////////////////////////////////////////////////////////////

utils::Pipe::Pipe(const std::string& name) {
  impl_->name_ = name;
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

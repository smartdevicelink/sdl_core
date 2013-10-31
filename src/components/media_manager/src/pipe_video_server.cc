/**
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

#include <unistd.h>

#include "media_manager/pipe_video_server.h"
#include "config_profile/profile.h"

namespace media_manager {

namespace video_stream_producer_consumer {

log4cxx::LoggerPtr PipeVideoServer::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("PipeVideoServer"));

PipeVideoServer::PipeVideoServer() : pipe_fd_(0) {
  LOG4CXX_TRACE_ENTER(logger_);

  named_pipe_path_ = profile::Profile::instance()->named_pipe_path();

  start();
}

PipeVideoServer::~PipeVideoServer() {
  close(pipe_fd_);
  unlink(named_pipe_path_.c_str());
}

bool PipeVideoServer::start() {
  LOG4CXX_TRACE_ENTER(logger_);

  if ((mkfifo(named_pipe_path_.c_str(), S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
      && (errno != EEXIST)) {
    LOG4CXX_ERROR(logger_, "Cannot create pipe " << named_pipe_path_);
    return false;
  }

  LOG4CXX_TRACE(logger_, "Pipe was successfully created");

  if (pipe_fd_ = open(named_pipe_path_.c_str(), O_RDWR, 0)) {
    LOG4CXX_ERROR(logger_, "Cannot open pipe for writing " << named_pipe_path_);
    return false;
  }

  LOG4CXX_TRACE(logger_, "Pipe was opened for writing " << named_pipe_path_);

  return true;
}

bool PipeVideoServer::stop() {
  LOG4CXX_TRACE_ENTER(logger_);

  return true;
}


void PipeVideoServer::sendMsg(const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_TRACE_ENTER(logger_);

  ssize_t ret = write(pipe_fd_, message.get()->data(), message.get()->data_size());

  if (ret == -1) {
    LOG4CXX_ERROR(logger_, "Failed writing data to pipe " << named_pipe_path_);
  } else if (ret != message.get()->data_size()) {
    LOG4CXX_WARN(logger_, "Couldn't write all the data to pipe " << named_pipe_path_);
  }

  LOG4CXX_WARN(logger_, "The data was successfully written to a pipe " << named_pipe_path_);
}

}  // namespace video_stream_producer_consumer
}  // namespace media_manager

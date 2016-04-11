/*
 * Copyright (c) 2014, Ford Motor Company
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

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils/logger.h"
#include "utils/file_system.h"
#include "media_manager/pipe_streamer_adapter.h"

namespace media_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "PipeStreamerAdapter")

PipeStreamerAdapter::PipeStreamerAdapter(
    const std::string& named_pipe_path, const std::string& app_storage_folder)
  : StreamerAdapter(new PipeStreamer(this, named_pipe_path, app_storage_folder)) {
}

PipeStreamerAdapter::~PipeStreamerAdapter() {
}

PipeStreamerAdapter::PipeStreamer::PipeStreamer(
    PipeStreamerAdapter* const adapter,
    const std::string& named_pipe_path,
    const std::string& app_storage_folder)
  : Streamer(adapter),
    named_pipe_path_(named_pipe_path),
    app_storage_folder_(app_storage_folder),
    pipe_fd_(0) {
    if (!file_system::CreateDirectoryRecursively(app_storage_folder_)) {
      LOG4CXX_ERROR(logger_, "Cannot create app storage folder "
                    << app_storage_folder_ );
      return;
    }
    if ((mkfifo(named_pipe_path_.c_str(),
                S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
        && (errno != EEXIST)) {
      LOG4CXX_ERROR(logger_, "Cannot create pipe " << named_pipe_path_);
    } else {
      LOG4CXX_INFO(logger_, "Pipe " << named_pipe_path_
                   << " was successfully created");
    }
}
PipeStreamerAdapter::PipeStreamer::~PipeStreamer() {
  if (0 == unlink(named_pipe_path_.c_str()) ) {
    LOG4CXX_INFO(logger_, "Pipe " << named_pipe_path_ << " was removed");
  } else {
    LOG4CXX_ERROR(logger_, "Error removing pipe " << named_pipe_path_);
  }
}


bool PipeStreamerAdapter::PipeStreamer::Connect() {
  LOG4CXX_AUTO_TRACE(logger_);

  pipe_fd_ = open(named_pipe_path_.c_str(), O_RDWR, 0);
  if (-1 == pipe_fd_) {
    LOG4CXX_ERROR(logger_, "Cannot open pipe for writing "
                  << named_pipe_path_);
    return false;
  }

  LOG4CXX_INFO(logger_, "Pipe " << named_pipe_path_
                << " was successfuly opened for writing");
  return true;
}

void PipeStreamerAdapter::PipeStreamer::Disconnect() {
  LOG4CXX_AUTO_TRACE(logger_);
  if (0 == close(pipe_fd_)) {
    LOG4CXX_INFO(logger_, "Pipe " << named_pipe_path_ << " was closed");
  } else {
    LOG4CXX_ERROR(logger_, "Error closing pipe " << named_pipe_path_);
  }
}

bool PipeStreamerAdapter::PipeStreamer::Send(
    protocol_handler::RawMessagePtr msg) {
  LOG4CXX_AUTO_TRACE(logger_);
  ssize_t ret = write(pipe_fd_, msg->data(), msg->data_size());
  if (-1 == ret) {
    LOG4CXX_ERROR(logger_, "Failed writing data to pipe "
                  << named_pipe_path_);
    return false;
  }

  if (static_cast<uint32_t>(ret) != msg->data_size()) {
    LOG4CXX_WARN(logger_, "Couldn't write all the data to pipe "
                 << named_pipe_path_);
  }

  LOG4CXX_INFO(logger_, "Streamer::sent " << msg->data_size());
  return true;
}

}  // namespace media_manager

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

#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils/logger.h"
#include "config_profile/profile.h"
#include "media_manager/pipe_streamer_adapter.h"

namespace media_manager {

namespace {
log4cxx::LoggerPtr logger =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("PipeStreamerAdapter"));
}

PipeStreamerAdapter::PipeStreamerAdapter()
  : pipe_fd_(0) {
  LOG4CXX_INFO(logger, "PipeStreamerAdapter::PipeStreamerAdapter");
  named_pipe_path_ = profile::Profile::instance()->named_video_pipe_path();
}

PipeStreamerAdapter::~PipeStreamerAdapter() {
  LOG4CXX_INFO(logger, "PipeStreamerAdapter::~PipeStreamerAdapter");
  if (0 != current_application_) {
    StopActivity(current_application_);
  }
}

void PipeStreamerAdapter::SendData(
  int32_t application_key,
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_INFO(logger, "PipeStreamerAdapter::SendData");

  if (application_key != current_application_) {
    LOG4CXX_WARN(logger, "Wrong application " << application_key);
    return;
  }

  ssize_t ret = write(pipe_fd_, message.get()->data(),
                      message.get()->data_size());

  if (ret == -1) {
    LOG4CXX_ERROR(logger, "Failed writing data to pipe " << named_pipe_path_);
    for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
         media_listeners_.end() != it;
         ++it) {
      (*it)->OnErrorReceived(application_key, -1);
    }
  } else if (ret != message.get()->data_size()) {
    LOG4CXX_WARN(logger, "Couldn't write all the data to pipe "
                 << named_pipe_path_);
  }

  LOG4CXX_INFO(logger, "The data was successfully written to a pipe "
               << named_pipe_path_);

  static int32_t messsages_for_session = 0;
  ++messsages_for_session;

  LOG4CXX_INFO(logger, "Handling map streaming message. This is "
              << messsages_for_session << "th message for " << application_key);
  for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
       media_listeners_.end() != it;
       ++it) {
    (*it)->OnDataReceived(application_key, messsages_for_session);
  }
}

void PipeStreamerAdapter::StartActivity(int32_t application_key) {
  LOG4CXX_INFO(logger, "PipeStreamerAdapter::StartActivity");

  if (application_key == current_application_) {
    LOG4CXX_WARN(logger, "Already started activity for " << application_key);
    return;
  }

  if ((mkfifo(named_pipe_path_.c_str(),
              S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
      && (errno != EEXIST)) {
    LOG4CXX_ERROR(logger, "Cannot create pipe " << named_pipe_path_);
    return;
  }

  LOG4CXX_TRACE(logger, "Pipe was successfully created");

  pipe_fd_ = open(named_pipe_path_.c_str(), O_RDWR, 0);
  if (-1 == pipe_fd_) {
    LOG4CXX_ERROR(logger, "Cannot open pipe for writing " << named_pipe_path_);
    return;
  }

  current_application_ = application_key;

  for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
       media_listeners_.end() != it;
       ++it) {
    (*it)->OnActivityStarted(application_key);
  }

  LOG4CXX_TRACE(logger, "Pipe was opened for writing " << named_pipe_path_);
}

void PipeStreamerAdapter::StopActivity(int32_t application_key) {
  LOG4CXX_INFO(logger, "PipeStreamerAdapter::StopActivity");
  if (application_key != current_application_) {
    LOG4CXX_WARN(logger, "Not performing activity for " << application_key);
    return;
  }
  close(pipe_fd_);
  unlink(named_pipe_path_.c_str());
  for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
       media_listeners_.end() != it;
       ++it) {
    (*it)->OnActivityEnded(application_key);
  }
}

bool PipeStreamerAdapter::is_app_performing_activity(
  int32_t application_key) {
  return (application_key == current_application_);
}

}  // namespace media_manager

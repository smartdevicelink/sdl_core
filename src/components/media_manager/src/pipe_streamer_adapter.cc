/*
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

#include <errno.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include "utils/logger.h"
#include "utils/file_system.h"
#include "config_profile/profile.h"
#include "media_manager/pipe_streamer_adapter.h"

namespace media_manager {

CREATE_LOGGERPTR_GLOBAL(logger, "PipeStreamerAdapter")

PipeStreamerAdapter::PipeStreamerAdapter()
  : is_ready_(false),
    thread_(NULL),
    messages_() {
  LOG4CXX_INFO(logger, "PipeStreamerAdapter::PipeStreamerAdapter");
}

PipeStreamerAdapter::~PipeStreamerAdapter() {
  LOG4CXX_INFO(logger, "PipeStreamerAdapter::~PipeStreamerAdapter");

  if ((0 != current_application_ ) && (is_ready_)) {
    StopActivity(current_application_);
  }

  thread_->stop();
}

void PipeStreamerAdapter::SendData(
  int32_t application_key,
  const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_INFO(logger, "PipeStreamerAdapter::SendData");

  if (application_key != current_application_) {
    LOG4CXX_WARN(logger, "Wrong application " << application_key);
    return;
  }

  if (is_ready_) {
    messages_.push(message);
  }
}

void PipeStreamerAdapter::StartActivity(int32_t application_key) {
  LOG4CXX_INFO(logger, "PipeStreamerAdapter::StartActivity");

  if (application_key == current_application_) {
    LOG4CXX_WARN(logger, "Already started activity for " << application_key);
    return;
  }

  current_application_ = application_key;
  is_ready_ = true;

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

  is_ready_ = false;
  current_application_ = 0;

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

void PipeStreamerAdapter::Init() {
  if (!thread_) {
    LOG4CXX_INFO(logger, "Create and start sending thread");
    thread_ = threads::CreateThread("PipeStreamer", new Streamer(this));
    const size_t kStackSize = 16384;
    thread_->startWithOptions(threads::ThreadOptions(kStackSize));
  }
}

PipeStreamerAdapter::Streamer::Streamer(
  PipeStreamerAdapter* server)
  : server_(server),
    pipe_fd_(0),
    stop_flag_(false) {
}

PipeStreamerAdapter::Streamer::~Streamer() {
  server_ = NULL;
}

void PipeStreamerAdapter::Streamer::threadMain() {
  LOG4CXX_INFO(logger, "Streamer::threadMain");

  open();

  while (!stop_flag_) {
    while (!server_->messages_.empty()) {
      ::protocol_handler::RawMessagePtr msg = server_->messages_.pop();
      if (!msg) {
        LOG4CXX_ERROR(logger, "Null pointer message");
        continue;
      }

      ssize_t ret = write(pipe_fd_, msg.get()->data(),
                          msg.get()->data_size());

      if (ret == -1) {
        LOG4CXX_ERROR(logger, "Failed writing data to pipe "
                      << server_->named_pipe_path_);

        std::set<MediaListenerPtr>::iterator it =
            server_->media_listeners_.begin();
        for (;server_->media_listeners_.end() != it; ++it) {
          (*it)->OnErrorReceived(server_->current_application_, -1);
        }
      } else if (static_cast<uint32_t>(ret) != msg.get()->data_size()) {
        LOG4CXX_WARN(logger, "Couldn't write all the data to pipe "
                     << server_->named_pipe_path_);
      }

      static int32_t messsages_for_session = 0;
      ++messsages_for_session;

      LOG4CXX_INFO(logger, "Handling map streaming message. This is "
                   << messsages_for_session << " the message for "
                   << server_->current_application_);
      std::set<MediaListenerPtr>::iterator it =
          server_->media_listeners_.begin();
      for (; server_->media_listeners_.end() != it; ++it) {
        (*it)->OnDataReceived(server_->current_application_,
                              messsages_for_session);
      }
    }
    server_->messages_.wait();
  }
  close();
}

bool PipeStreamerAdapter::Streamer::exitThreadMain() {
  LOG4CXX_INFO(logger, "Streamer::exitThreadMain");
  stop_flag_ = true;
  server_->messages_.Shutdown();
  return false;
}

void PipeStreamerAdapter::Streamer::open() {

  LOG4CXX_INFO(logger, "Streamer::open() " << server_->named_pipe_path_.c_str());

  DCHECK(file_system::CreateDirectoryRecursively(
      profile::Profile::instance()->app_storage_folder()));

  if ((mkfifo(server_->named_pipe_path_.c_str(),
              S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH) < 0)
      && (errno != EEXIST)) {
    LOG4CXX_ERROR(logger, "Cannot create pipe " << server_->named_pipe_path_);
    return;
  }

  pipe_fd_ = ::open(server_->named_pipe_path_.c_str(), O_RDWR, 0);
  if (-1 == pipe_fd_) {
    LOG4CXX_ERROR(logger, "Cannot open pipe for writing "
                  << server_->named_pipe_path_);
    return;
  }

  LOG4CXX_TRACE(logger, "Pipe " << server_->named_pipe_path_
                << " was successfully created");
}

void PipeStreamerAdapter::Streamer::close() {
  ::close(pipe_fd_);
  unlink(server_->named_pipe_path_.c_str());
}

}  // namespace media_manager

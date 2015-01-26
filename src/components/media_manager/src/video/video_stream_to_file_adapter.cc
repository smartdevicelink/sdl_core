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

#include "utils/logger.h"
#include "utils/file_system.h"
#include "config_profile/profile.h"
#include "media_manager/video/video_stream_to_file_adapter.h"

namespace media_manager {

CREATE_LOGGERPTR_GLOBAL(logger, "VideoStreamToFileAdapter")

VideoStreamToFileAdapter::VideoStreamToFileAdapter(const std::string& file_name)
  : file_name_(file_name),
    is_ready_(false),
    thread_(NULL) {
  Init();
}

VideoStreamToFileAdapter::~VideoStreamToFileAdapter() {
  if ((0 != current_application_ ) && (is_ready_)) {
    StopActivity(current_application_);
  }

  thread_->stop();
}

void VideoStreamToFileAdapter::Init() {
  if (!thread_) {
    LOG4CXX_INFO(logger, "Create and start sending thread");
    thread_ = threads::CreateThread("VideoStreamer",
                                  new Streamer(this));
    const size_t kStackSize = 16384;
    thread_->startWithOptions(threads::ThreadOptions(kStackSize));
  }
}

void VideoStreamToFileAdapter::SendData(
  int32_t application_key,
  const ::protocol_handler::RawMessagePtr message) {
  LOG4CXX_INFO(logger, "VideoStreamToFileAdapter::SendData "
               << application_key);

  if (application_key != current_application_) {
    LOG4CXX_WARN(logger, "Wrong application " << application_key);
    return;
  }

  if (is_ready_) {
    messages_.push(message);
  }
}

void VideoStreamToFileAdapter::StartActivity(int32_t application_key) {
  LOG4CXX_INFO(logger, "VideoStreamToFileAdapter::StartActivity "
               << application_key);
  if (application_key == current_application_) {
    LOG4CXX_WARN(logger, "Already running video stream to file for "
                 << application_key);
    return;
  }

  current_application_ = application_key;
  is_ready_ = true;

  for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
       media_listeners_.end() != it;
       ++it) {
    (*it)->OnActivityStarted(application_key);
  }
}

void VideoStreamToFileAdapter::StopActivity(int32_t application_key) {
  LOG4CXX_INFO(logger, "VideoStreamToFileAdapter::StopActivity "
               << application_key);
  if (application_key != current_application_) {
    LOG4CXX_WARN(logger, "Performing activity for another key "
                 << current_application_);
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

bool VideoStreamToFileAdapter::is_app_performing_activity(int32_t
                                                          application_key) {
  return (application_key == current_application_ && is_ready_);
}

VideoStreamToFileAdapter::Streamer::Streamer(
  VideoStreamToFileAdapter* server)
  : server_(server),
    stop_flag_(false),
    file_stream_(NULL) {
}

VideoStreamToFileAdapter::Streamer::~Streamer() {
  server_ = NULL;
  file_stream_ = NULL;
}

void VideoStreamToFileAdapter::Streamer::threadMain() {
  LOG4CXX_INFO(logger, "Streamer::threadMain");

  open();

  while (!stop_flag_) {
    while (!server_->messages_.empty()) {
      ::protocol_handler::RawMessagePtr msg = server_->messages_.pop();
      if (!msg) {
        LOG4CXX_ERROR(logger, "Null pointer message");
        continue;
      }

      if (file_stream_) {
        file_system::Write(file_stream_, msg->data(), msg->data_size());

        static int32_t messsages_for_session = 0;
        ++messsages_for_session;
        std::set<MediaListenerPtr>::iterator it =
            server_->media_listeners_.begin();
        for (; server_->media_listeners_.end() != it; ++it) {
          (*it)->OnDataReceived(server_->current_application_,
                                messsages_for_session);
        }
      } else {
        LOG4CXX_WARN(logger, "Can't open File stream! " << server_->file_name_);
      }
    }
    server_->messages_.wait();
  }

  close();
}

bool VideoStreamToFileAdapter::Streamer::exitThreadMain() {
  LOG4CXX_INFO(logger, "Streamer::exitThreadMain");
  stop_flag_ = true;
  server_->messages_.Shutdown();
  return false;
}

void VideoStreamToFileAdapter::Streamer::open() {
  LOG4CXX_INFO(logger, "Streamer::open()" << server_->file_name_.c_str());

  DCHECK(file_system::CreateDirectoryRecursively(
      profile::Profile::instance()->app_storage_folder()));

  file_stream_ = file_system::Open(server_->file_name_);
  if (!file_stream_) {
      LOG4CXX_WARN(logger, "Can't open file stream! " << server_->file_name_);
  } else {
    LOG4CXX_INFO(logger, "file_stream_ opened :" << file_stream_);
  }
}

void VideoStreamToFileAdapter::Streamer::close() {
  if (file_stream_) {
    file_system::Close(file_stream_);
    delete file_stream_;
    file_stream_ = NULL;
  }
  file_system::DeleteFile(server_->file_name_);
}

}  //  namespace media_manager

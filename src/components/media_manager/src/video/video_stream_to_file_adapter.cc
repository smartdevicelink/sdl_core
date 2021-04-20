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

#include "media_manager/video/video_stream_to_file_adapter.h"
#include "config_profile/profile.h"
#include "utils/file_system.h"
#include "utils/logger.h"

namespace media_manager {

SDL_CREATE_LOG_VARIABLE("VideoStreamToFileAdapter")

VideoStreamToFileAdapter::VideoStreamToFileAdapter(const std::string& file_name)
    : file_name_(file_name)
    , is_ready_(false)
    , thread_(threads::CreateThread("VideoStreamer", new Streamer(this))) {
  Init();
}

VideoStreamToFileAdapter::~VideoStreamToFileAdapter() {
  SDL_LOG_AUTO_TRACE();
  if ((0 != current_application_) && (is_ready_)) {
    StopActivity(current_application_);
  }
  thread_->Stop(threads::Thread::kThreadSoftStop);
  delete thread_->GetDelegate();
  threads::DeleteThread(thread_);
}

void VideoStreamToFileAdapter::Init() {
  if (thread_->IsRunning()) {
    SDL_LOG_DEBUG("Start sending thread");
    const size_t kStackSize = 16384;
    thread_->Start(threads::ThreadOptions(kStackSize));
  } else {
    SDL_LOG_WARN("thread is already running");
  }
}

void VideoStreamToFileAdapter::SendData(
    int32_t application_key, const ::protocol_handler::RawMessagePtr message) {
  SDL_LOG_INFO("VideoStreamToFileAdapter::SendData " << application_key);

  if (application_key != current_application_) {
    SDL_LOG_WARN("Wrong application " << application_key);
    return;
  }

  if (is_ready_) {
    messages_.push(message);
  }
}

void VideoStreamToFileAdapter::StartActivity(int32_t application_key) {
  SDL_LOG_INFO("VideoStreamToFileAdapter::StartActivity " << application_key);
  if (application_key == current_application_) {
    SDL_LOG_WARN("Already running video stream to file for "
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
  SDL_LOG_INFO("VideoStreamToFileAdapter::StopActivity " << application_key);
  if (application_key != current_application_) {
    SDL_LOG_WARN("Performing activity for another key "
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

bool VideoStreamToFileAdapter::is_app_performing_activity(
    int32_t application_key) {
  return (application_key == current_application_ && is_ready_);
}

VideoStreamToFileAdapter::Streamer::Streamer(VideoStreamToFileAdapter* server)
    : server_(server), stop_flag_(false), file_stream_(NULL) {}

VideoStreamToFileAdapter::Streamer::~Streamer() {
  server_ = NULL;
  file_stream_ = NULL;
}

void VideoStreamToFileAdapter::Streamer::threadMain() {
  SDL_LOG_AUTO_TRACE();

  open();

  while (!stop_flag_) {
    while (!server_->messages_.empty()) {
      ::protocol_handler::RawMessagePtr msg = server_->messages_.pop();
      if (!msg) {
        SDL_LOG_ERROR("Null pointer message");
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
        SDL_LOG_WARN("Can't open File stream! " << server_->file_name_);
      }
    }
    server_->messages_.wait();
  }

  close();
}

void VideoStreamToFileAdapter::Streamer::exitThreadMain() {
  SDL_LOG_AUTO_TRACE();
  stop_flag_ = true;
  server_->messages_.Shutdown();
}

void VideoStreamToFileAdapter::Streamer::open() {
  SDL_LOG_INFO("Streamer::open()" << server_->file_name_.c_str());

  DCHECK(file_system::CreateDirectoryRecursively(
      profile::Profile::instance()->app_storage_folder()));

  file_stream_ = file_system::Open(server_->file_name_);
  if (!file_stream_) {
    SDL_LOG_WARN("Can't open file stream! " << server_->file_name_);
  } else {
    SDL_LOG_INFO("file_stream_ opened :" << file_stream_);
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

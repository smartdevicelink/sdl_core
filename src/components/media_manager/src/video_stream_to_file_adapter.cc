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

#include "./video_stream_to_file_adapter.h"
#include "utils/file_system.h"

namespace media_manager {

namespace {
log4cxx::LoggerPtr logger =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("VideoStreamToFileAdapter"));
}

VideoStreamToFileAdapter::VideoStreamToFileAdapter(const std::string& file_name)
  : file_name_(file_name)
  , file_stream_(NULL) {
}

VideoStreamToFileAdapter::~VideoStreamToFileAdapter() {
  StopActivity(current_application_);
}

void VideoStreamToFileAdapter::SendData(
  int32_t application_key,
  const protocol_handler::RawMessagePtr& message) {
  LOG4CXX_INFO(logger, "VideoStreamToFileAdapter::SendData"
               << application_key);
  if (application_key == current_application_ &&
      file_stream_) {
    file_system::Write(file_stream_, message->data(), message->data_size());

    static int32_t messsages_for_session = 0;
    ++messsages_for_session;

    LOG4CXX_INFO(logger, "Handling map streaming message. This is "
                 << messsages_for_session << "th message for "
                 << application_key);
    for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
         media_listeners_.end() != it;
         ++it) {
      (*it)->OnDataReceived(application_key, messsages_for_session);
    }
  } else {
    LOG4CXX_WARN(logger, "Running for another application "
                 << current_application_);
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
  CloseCurrent();

  file_stream_ = file_system::Open(file_name_);
  if (file_stream_ == NULL){
      LOG4CXX_WARN(logger, "Cant open File stream! "<<file_name_);
    }
  else{
      LOG4CXX_INFO(logger, "file_stream_ opened :"
                   << file_stream_);
    }

  current_application_ = application_key;

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

  for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
       media_listeners_.end() != it;
       ++it) {
    (*it)->OnActivityEnded(application_key);
  }

  CloseCurrent();
  current_application_ = 0;
}

bool VideoStreamToFileAdapter::is_app_performing_activity(int32_t application_key) {
  return (application_key == current_application_ && file_stream_);
}

void VideoStreamToFileAdapter::CloseCurrent() {
  if (file_stream_) {
    file_system::Close(file_stream_);
    file_system::DeleteFile(file_name_);
    delete file_stream_;
    file_stream_ = NULL;
  }
}

}  //  namespace media_manager

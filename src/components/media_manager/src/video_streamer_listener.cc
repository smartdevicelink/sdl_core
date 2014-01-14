/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "media_manager/video_streamer_listener.h"
#include "media_manager/media_manager_impl.h"

namespace media_manager {

log4cxx::LoggerPtr VideoStreamerListener::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("VideoStreamerListener"));

VideoStreamerListener::VideoStreamerListener()
  : current_application_(0) {
}

VideoStreamerListener::~VideoStreamerListener() {
  OnActivityEnded(current_application_);
}

void VideoStreamerListener::OnDataReceived(
  int32_t application_key,
  const DataForListener& data) {
  MediaManagerImpl::instance()->FramesProcessed(application_key, data);
}

void VideoStreamerListener::OnErrorReceived(
  int32_t application_key,
  const DataForListener& data) {
  LOG4CXX_ERROR(logger_, "VideoStreamerListener::OnErrorReceived");
}

void VideoStreamerListener::OnActivityStarted(int32_t application_key) {
  LOG4CXX_INFO(logger_, "VideoStreamerListener::OnActivityStarted");
  if (current_application_ == application_key) {
    LOG4CXX_WARN(logger_, "Already performing activity for "
                 << application_key);
    return;
  }
  current_application_ = application_key;
}

void VideoStreamerListener::OnActivityEnded(int32_t application_key) {
  LOG4CXX_INFO(logger_, "VideoStreamerListener::OnActivityEnded");
  if (current_application_ != application_key) {
    LOG4CXX_WARN(logger_, "Already not performing activity for "
                 << application_key);
    return;
  }
  current_application_ = 0;
}
}  //  namespace media_manager

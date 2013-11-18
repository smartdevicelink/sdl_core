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

#include "media_manager/media_manager_impl.h"
#include "media_manager/a2dp_source_player_adapter.h"
#include "media_manager/from_mic_recorder_adapter.h"
#include "media_manager/from_mic_recorder_listener.h"

namespace media_manager {

log4cxx::LoggerPtr MediaManagerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("MediaManagerImpl"));

MediaManagerImpl* MediaManagerImpl::instance() {
  static MediaManagerImpl instance;
  return &instance;
}

MediaManagerImpl::MediaManagerImpl()
  : a2dp_player_(NULL)
  , from_mic_recorder_(NULL)
  , from_mic_listener_(NULL) {
  Init();
}

MediaManagerImpl::~MediaManagerImpl() {
  if (a2dp_player_) {
    delete a2dp_player_;
    a2dp_player_ = NULL;
  }

  if (from_mic_listener_) {
    delete from_mic_listener_;
    from_mic_listener_ = NULL;
  }

  if (from_mic_recorder_) {
    delete from_mic_recorder_;
    from_mic_recorder_ = NULL;
  }
}

void MediaManagerImpl::Init() {
#if defined(DEFAULT_MEDIA)
  a2dp_player_ = new A2DPSourcePlayerAdapter();
  from_mic_recorder_ = new FromMicRecorderAdapter();
#endif
}

void MediaManagerImpl::PlayA2DPSource(int application_key) {
  a2dp_player_->StartActivity(application_key);
}

void MediaManagerImpl::StopA2DPSource(int application_key) {
  a2dp_player_->StopActivity(application_key);
}

void MediaManagerImpl::StartMicrophoneRecording(
  int application_key,
  const std::string& output_file,
  int duration) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StartMicrophoneRecording")  ;
  from_mic_listener_ = new FromMicRecorderListener(output_file);
  from_mic_recorder_->AddListener(from_mic_listener_);
#if defined(DEFAULT_MEDIA)
  (static_cast<FromMicRecorderAdapter*>(from_mic_recorder_))
  ->set_output_file(output_file);
  (static_cast<FromMicRecorderAdapter*>(from_mic_recorder_))
  ->set_duration(duration);
  from_mic_recorder_->StartActivity(application_key);
#endif
  from_mic_listener_->OnActivityStarted(application_key);
}

}  //  namespace media_manager

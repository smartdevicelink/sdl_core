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

#include "config_profile/profile.h"
#include "media_manager/media_manager_impl.h"
#include "media_manager/from_mic_recorder_listener.h"
#include "media_manager/video_streamer_listener.h"
#if defined(DEFAULT_MEDIA)
#include "media_manager/a2dp_source_player_adapter.h"
#include "media_manager/from_mic_recorder_adapter.h"
#include "./socket_video_streamer_adapter.h"
#include "./pipe_video_streamer_adapter.h"
#else
#include "./video_stream_to_file_adapter.h"
#endif

namespace media_manager {

log4cxx::LoggerPtr MediaManagerImpl::logger_ = log4cxx::LoggerPtr(
      log4cxx::Logger::getLogger("MediaManagerImpl"));

MediaManagerImpl* MediaManagerImpl::instance() {
  static MediaManagerImpl instance;
  return &instance;
}

MediaManagerImpl::MediaManagerImpl()
  : protocol_handler_(NULL)
  , a2dp_player_(NULL)
  , from_mic_recorder_(NULL)
  , video_streamer_(NULL) {
  Init();
}

MediaManagerImpl::~MediaManagerImpl() {
  if (a2dp_player_) {
    delete a2dp_player_;
    a2dp_player_ = NULL;
  }

  if (from_mic_recorder_) {
    delete from_mic_recorder_;
    from_mic_recorder_ = NULL;
  }

  if (video_streamer_) {
    delete video_streamer_;
    video_streamer_ = NULL;
  }
}

void MediaManagerImpl::SetProtocolHandler(
  protocol_handler::ProtocolHandler* protocol_handler) {
  protocol_handler_ = protocol_handler;
}

void MediaManagerImpl::Init() {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::Init()");
#if defined(DEFAULT_MEDIA)
  LOG4CXX_INFO(logger_, "Called Init with default configuration.");
  a2dp_player_ = new A2DPSourcePlayerAdapter();
  from_mic_recorder_ = new FromMicRecorderAdapter();
  if ("socket" == profile::Profile::instance()->video_server_type()) {
    video_streamer_ = new SocketVideoStreamerAdapter();
  } else if ("pipe" == profile::Profile::instance()->video_server_type()) {
    video_streamer_ = new PipeVideoStreamerAdapter();
  }
#else
  video_streamer_ = new VideoStreamToFileAdapter(
    profile::Profile::instance()->video_stream_file());
#endif
  video_streamer_listener_ = new VideoStreamerListener();

  if (NULL != video_streamer_) {
    video_streamer_->AddListener(video_streamer_listener_);
  }
}

void MediaManagerImpl::PlayA2DPSource(int application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::PlayA2DPSource");
  if (a2dp_player_) {
    a2dp_player_->StartActivity(application_key);
  }
}

void MediaManagerImpl::StopA2DPSource(int application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StopA2DPSource");
  if (a2dp_player_) {
    a2dp_player_->StopActivity(application_key);
  }
}

void MediaManagerImpl::StartMicrophoneRecording(
  int application_key,
  const std::string& output_file,
  int duration) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StartMicrophoneRecording to "
               << output_file);
#if defined(DEFAULT_MEDIA)
  from_mic_listener_ = new FromMicRecorderListener(output_file);
  if (from_mic_recorder_) {
    from_mic_recorder_->AddListener(from_mic_listener_);
    (static_cast<FromMicRecorderAdapter*>(from_mic_recorder_))
    ->set_output_file(output_file);
    (static_cast<FromMicRecorderAdapter*>(from_mic_recorder_))
    ->set_duration(duration);
    from_mic_recorder_->StartActivity(application_key);
  }
#else
  const char* predefined_rec_file = "audio.8bit.wav";
  from_mic_listener_ = new FromMicRecorderListener(predefined_rec_file);
#endif
  from_mic_listener_->OnActivityStarted(application_key);
}

void MediaManagerImpl::StopMicrophoneRecording(int application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StopMicrophoneRecording");
#if defined(DEFAULT_MEDIA)
  if (from_mic_recorder_) {
    from_mic_recorder_->StopActivity(application_key);
  }
#endif
  if (from_mic_listener_) {
    from_mic_listener_->OnActivityEnded(application_key);
  }
}

void MediaManagerImpl::StartVideoStreaming(int application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StartVideoStreaming");

  if (video_streamer_) {
    video_streamer_->StartActivity(application_key);
  }
}

void MediaManagerImpl::StopVideoStreaming(int application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StopVideoStreaming");
  if (video_streamer_) {
    video_streamer_->StopActivity(application_key);
  }
}

void MediaManagerImpl::OnMessageReceived(
  const protocol_handler::RawMessagePtr& message) {
  if (message->is_fully_binary()) {
    if (video_streamer_) {
      video_streamer_->SendData(message->connection_key(), message);
    }
  }
}

void MediaManagerImpl::OnMobileMessageSent(
  const protocol_handler::RawMessagePtr& message) {
}

void MediaManagerImpl::FramesProcessed(int application_key,
                                       int frame_number) {
  if (protocol_handler_) {
    protocol_handler_->SendFramesNumber(application_key,
                                        frame_number);
  }
}

}  //  namespace media_manager

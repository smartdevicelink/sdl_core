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
#include "media_manager/audio/from_mic_recorder_listener.h"
#include "media_manager/streamer_listener.h"
#include "application_manager/message_helper.h"
#include "application_manager/application.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "utils/file_system.h"
#include "utils/logger.h"
#if defined(EXTENDED_MEDIA_MODE)
#include "media_manager/audio/a2dp_source_player_adapter.h"
#include "media_manager/audio/from_mic_recorder_adapter.h"
#endif
#include "media_manager/video/socket_video_streamer_adapter.h"
#include "media_manager/audio/socket_audio_streamer_adapter.h"
#include "media_manager/video/pipe_video_streamer_adapter.h"
#include "media_manager/audio/pipe_audio_streamer_adapter.h"
#include "media_manager/video/video_stream_to_file_adapter.h"


namespace media_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "MediaManagerImpl")

MediaManagerImpl::MediaManagerImpl()
  : protocol_handler_(NULL)
  , a2dp_player_(NULL)
  , from_mic_recorder_(NULL)
  , video_streamer_(NULL)
  , audio_streamer_(NULL)
  , video_stream_active_(false)
  , audio_stream_active_(false) {
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

  if (audio_streamer_) {
    delete audio_streamer_;
    audio_streamer_ = NULL;
  }
}

void MediaManagerImpl::SetProtocolHandler(
  protocol_handler::ProtocolHandler* protocol_handler) {
  protocol_handler_ = protocol_handler;
}

void MediaManagerImpl::Init() {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::Init()");

#if defined(EXTENDED_MEDIA_MODE)
  LOG4CXX_INFO(logger_, "Called Init with default configuration.");
  a2dp_player_ = new A2DPSourcePlayerAdapter();
  from_mic_recorder_ = new FromMicRecorderAdapter();
#endif

  if ("socket" == profile::Profile::instance()->video_server_type()) {
    video_streamer_ = new SocketVideoStreamerAdapter();
  } else if ("pipe" == profile::Profile::instance()->video_server_type()) {
    video_streamer_ = new PipeVideoStreamerAdapter();
  } else if ("file" == profile::Profile::instance()->video_server_type()) {
    video_streamer_ = new VideoStreamToFileAdapter(
        profile::Profile::instance()->video_stream_file());
  }

  if ("socket" == profile::Profile::instance()->audio_server_type()) {
    audio_streamer_ = new SocketAudioStreamerAdapter();
  } else if ("pipe" == profile::Profile::instance()->audio_server_type()) {
    audio_streamer_ = new PipeAudioStreamerAdapter();
  } else if ("file" == profile::Profile::instance()->audio_server_type()) {
    audio_streamer_ = new VideoStreamToFileAdapter(
        profile::Profile::instance()->audio_stream_file());
  }

  video_streamer_listener_ = new StreamerListener();
  audio_streamer_listener_ = new StreamerListener();

  if (NULL != video_streamer_) {
    video_streamer_->AddListener(video_streamer_listener_);
  }

  if (NULL != audio_streamer_) {
    audio_streamer_->AddListener(audio_streamer_listener_);
  }
}

void MediaManagerImpl::PlayA2DPSource(int32_t application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::PlayA2DPSource");
  if (a2dp_player_) {
    a2dp_player_->StartActivity(application_key);
  }
}

void MediaManagerImpl::StopA2DPSource(int32_t application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StopA2DPSource");
  if (a2dp_player_) {
    a2dp_player_->StopActivity(application_key);
  }
}

void MediaManagerImpl::StartMicrophoneRecording(
  int32_t application_key,
  const std::string& output_file,
  int32_t duration) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StartMicrophoneRecording to "
               << output_file);
  application_manager::ApplicationSharedPtr app =
    application_manager::ApplicationManagerImpl::instance()->
      application(application_key);
  std::string file_path =
  profile::Profile::instance()->app_storage_folder();
  file_path += "/";
  file_path += output_file;
  from_mic_listener_ = new FromMicRecorderListener(file_path);
#if defined(EXTENDED_MEDIA_MODE)
  if (from_mic_recorder_) {
    from_mic_recorder_->AddListener(from_mic_listener_);
    (static_cast<FromMicRecorderAdapter*>(from_mic_recorder_))
    ->set_output_file(file_path);
    (static_cast<FromMicRecorderAdapter*>(from_mic_recorder_))
    ->set_duration(duration);
    from_mic_recorder_->StartActivity(application_key);
  }
#else
  if (file_system::FileExists(file_path)) {
    LOG4CXX_INFO(logger_, "File " << output_file << " exists, removing");
    if (file_system::DeleteFile(file_path)) {
      LOG4CXX_INFO(logger_, "File " << output_file << " removed");
    }
    else {
      LOG4CXX_WARN(logger_, "Could not remove file " << output_file);
    }
  }
  const std::string record_file_source =
      profile::Profile::instance()->app_resourse_folder() + "/" +
      profile::Profile::instance()->recording_file_source();
  std::vector<uint8_t> buf;
  if (file_system::ReadBinaryFile(record_file_source, buf)) {
    if (file_system::Write(file_path, buf)) {
      LOG4CXX_INFO(logger_,
        "File " << record_file_source << " copied to " << output_file);
    }
    else {
      LOG4CXX_WARN(logger_, "Could not write to file " << output_file);
    }
  }
  else {
    LOG4CXX_WARN(logger_, "Could not read file " << record_file_source);
  }
#endif
  from_mic_listener_->OnActivityStarted(application_key);
}

void MediaManagerImpl::StopMicrophoneRecording(int32_t application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StopMicrophoneRecording");
#if defined(EXTENDED_MEDIA_MODE)
  if (from_mic_recorder_) {
    from_mic_recorder_->StopActivity(application_key);
  }
#endif
  if (from_mic_listener_) {
    from_mic_listener_->OnActivityEnded(application_key);
  }
}

void MediaManagerImpl::StartVideoStreaming(int32_t application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StartVideoStreaming");

  if (video_streamer_) {
    if (!video_stream_active_) {
      video_stream_active_ = true;
      video_streamer_->StartActivity(application_key);
      application_manager::MessageHelper::SendNaviStartStream(application_key);
    }
  }
}

void MediaManagerImpl::StopVideoStreaming(int32_t application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StopVideoStreaming");
  if (video_streamer_) {
    video_stream_active_ = false;
    application_manager::MessageHelper::SendNaviStopStream(application_key);
    video_streamer_->StopActivity(application_key);
  }
}

void MediaManagerImpl::StartAudioStreaming(int32_t application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StartAudioStreaming");

  if (audio_streamer_) {
    if (!audio_stream_active_) {
      audio_stream_active_ = true;
      audio_streamer_->StartActivity(application_key);
      application_manager::MessageHelper::SendAudioStartStream(application_key);
    }
  }
}

void MediaManagerImpl::StopAudioStreaming(int32_t application_key) {
  LOG4CXX_INFO(logger_, "MediaManagerImpl::StopAudioStreaming");
  if (audio_streamer_) {
    audio_stream_active_ = false;
    application_manager::MessageHelper::SendAudioStopStream(application_key);
    audio_streamer_->StopActivity(application_key);
  }
}

void MediaManagerImpl::OnMessageReceived(
  const ::protocol_handler::RawMessagePtr message) {
  if (message->service_type()
      == protocol_handler::kMobileNav) {
    if (!(application_manager::ApplicationManagerImpl::instance()->
         IsVideoStreamingAllowed(message->connection_key()))) {
       return;
     }
    if (video_streamer_) {
      video_streamer_->SendData(message->connection_key(), message);
    }
  } else if (message->service_type()
          == protocol_handler::kAudio) {
    if (!(application_manager::ApplicationManagerImpl::instance()->
         IsAudioStreamingAllowed(message->connection_key()))) {
       return;
     }
    if (audio_streamer_) {
      audio_streamer_->SendData(message->connection_key(), message);
    }
  }
}

void MediaManagerImpl::OnMobileMessageSent(
  const ::protocol_handler::RawMessagePtr message) {
}

void MediaManagerImpl::FramesProcessed(int32_t application_key,
                                       int32_t frame_number) {
  if (protocol_handler_) {
    protocol_handler_->SendFramesNumber(application_key,
                                        frame_number);
  }
}

}  //  namespace media_manager

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

#include "config_profile/profile.h"
#include "media_manager/media_manager_impl.h"
#include "media_manager/audio/from_mic_recorder_listener.h"
#include "media_manager/streamer_listener.h"
#include "application_manager/message_helper.h"
#include "application_manager/application.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/application_impl.h"
#include "protocol_handler/protocol_handler.h"
#include "utils/file_system.h"
#include "utils/logger.h"
#include "utils/helpers.h"
#if defined(EXTENDED_MEDIA_MODE)
#include "media_manager/audio/a2dp_source_player_adapter.h"
#include "media_manager/audio/from_mic_recorder_adapter.h"
#endif
#include "media_manager/video/socket_video_streamer_adapter.h"
#include "media_manager/audio/socket_audio_streamer_adapter.h"
#include "media_manager/video/pipe_video_streamer_adapter.h"
#include "media_manager/audio/pipe_audio_streamer_adapter.h"
#include "media_manager/video/file_video_streamer_adapter.h"
#include "media_manager/audio/file_audio_streamer_adapter.h"

namespace media_manager {

using profile::Profile;
using timer::TimerThread;

CREATE_LOGGERPTR_GLOBAL(logger_, "MediaManagerImpl")

MediaManagerImpl::MediaManagerImpl()
  : protocol_handler_(NULL)
  , a2dp_player_(NULL)
  , from_mic_recorder_(NULL) {
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
}

#ifdef BUILD_TESTS
  void MediaManagerImpl::set_mock_a2dp_player(MediaAdapter* media_adapter) {
    a2dp_player_= media_adapter;
  }

  void MediaManagerImpl::set_mock_mic_listener(MediaListenerPtr media_listener) {
    from_mic_listener_ = media_listener;
  }

#ifdef EXTENDED_MEDIA_MODE
  void MediaManagerImpl::set_mock_mic_recorder(MediaAdapterImpl* media_adapter) {
    from_mic_recorder_ = media_adapter;
  }

#endif // EXTENDED_MEDIA_MODE

  void MediaManagerImpl::set_mock_streamer(protocol_handler::ServiceType stype,
                                           MediaAdapterImpl* mock_stream) {
    streamer_[stype]=  mock_stream;
  }

  void MediaManagerImpl::set_mock_streamer_listener(protocol_handler::ServiceType stype,
                                                    MediaAdapterListener* mock_stream) {
    streamer_listener_[stype]=  mock_stream;
  }

#endif // BUILD_TESTS

void MediaManagerImpl::Init() {
  using namespace protocol_handler;
  LOG4CXX_INFO(logger_, "MediaManagerImpl::Init()");

#if defined(EXTENDED_MEDIA_MODE)
  LOG4CXX_INFO(logger_, "Called Init with default configuration.");
  a2dp_player_ = new A2DPSourcePlayerAdapter();
  from_mic_recorder_ = new FromMicRecorderAdapter();
#endif

  if ("socket" == profile::Profile::instance()->video_server_type()) {
    streamer_[ServiceType::kMobileNav] = new SocketVideoStreamerAdapter();
  } else if ("pipe" == profile::Profile::instance()->video_server_type()) {
    streamer_[ServiceType::kMobileNav] = new PipeVideoStreamerAdapter();
  } else if ("file" == profile::Profile::instance()->video_server_type()) {
    streamer_[ServiceType::kMobileNav] = new FileVideoStreamerAdapter();
  }

  if ("socket" == profile::Profile::instance()->audio_server_type()) {
    streamer_[ServiceType::kAudio] = new SocketAudioStreamerAdapter();
  } else if ("pipe" == profile::Profile::instance()->audio_server_type()) {
    streamer_[ServiceType::kAudio] = new PipeAudioStreamerAdapter();
  } else if ("file" == profile::Profile::instance()->audio_server_type()) {
    streamer_[ServiceType::kAudio] = new FileAudioStreamerAdapter();
  }

  streamer_listener_[ServiceType::kMobileNav] = new StreamerListener();
  streamer_listener_[ServiceType::kAudio] = new StreamerListener();

  if (streamer_[ServiceType::kMobileNav]) {
    streamer_[ServiceType::kMobileNav]->AddListener(
        streamer_listener_[ServiceType::kMobileNav]);
  }

  if (streamer_[ServiceType::kAudio]) {
    streamer_[ServiceType::kAudio]->AddListener(
        streamer_listener_[ServiceType::kAudio]);
  }
}

void MediaManagerImpl::PlayA2DPSource(int32_t application_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (a2dp_player_) {
    a2dp_player_->StartActivity(application_key);
  }
}

void MediaManagerImpl::StopA2DPSource(int32_t application_key) {
  LOG4CXX_AUTO_TRACE(logger_);
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
  LOG4CXX_AUTO_TRACE(logger_);
#if defined(EXTENDED_MEDIA_MODE)
  if (from_mic_recorder_) {
    from_mic_recorder_->StopActivity(application_key);
  }
#endif
  if (from_mic_listener_) {
    from_mic_listener_->OnActivityEnded(application_key);
  }
#if defined(EXTENDED_MEDIA_MODE)
  if (from_mic_recorder_) {
    from_mic_recorder_->RemoveListener(from_mic_listener_);
  }
#endif
}

void MediaManagerImpl::StartStreaming(
    int32_t application_key, protocol_handler::ServiceType service_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (streamer_[service_type]) {
    streamer_[service_type]->StartActivity(application_key);
  }
}

void MediaManagerImpl::StopStreaming(
    int32_t application_key, protocol_handler::ServiceType service_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (streamer_[service_type]) {
    streamer_[service_type]->StopActivity(application_key);
  }
}

void MediaManagerImpl::SetProtocolHandler(
  protocol_handler::ProtocolHandler* protocol_handler) {
  protocol_handler_ = protocol_handler;
}

void MediaManagerImpl::OnMessageReceived(
    const ::protocol_handler::RawMessagePtr message) {
  using namespace protocol_handler;
  using namespace application_manager;
  using namespace helpers;
  LOG4CXX_AUTO_TRACE(logger_);

  const uint32_t streaming_app_id = message->connection_key();
  const ServiceType service_type = message->service_type();

  if (Compare<ServiceType, NEQ, ALL>(
        service_type, ServiceType::kMobileNav, ServiceType::kAudio)) {
    LOG4CXX_DEBUG(logger_, "Unsupported service type in MediaManager");
    return;
  }

  ApplicationManagerImpl* app_mgr = ApplicationManagerImpl::instance();
  DCHECK_OR_RETURN_VOID(app_mgr);

  if (!app_mgr->CanAppStream(streaming_app_id, service_type)) {
    app_mgr->ForbidStreaming(streaming_app_id);
    LOG4CXX_ERROR(logger_, "The application trying to stream when it should not.");
    return;
  }

  ApplicationSharedPtr app = app_mgr->application(streaming_app_id);
  if (app) {
    app->WakeUpStreaming(service_type);
    streamer_[service_type]->SendData(streaming_app_id, message);
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

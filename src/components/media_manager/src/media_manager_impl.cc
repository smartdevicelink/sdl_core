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

#include "media_manager/media_manager_impl.h"
#include "application_manager/application.h"
#include "application_manager/application_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/message_helper.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/MOBILE_API.h"
#include "media_manager/audio/from_mic_recorder_listener.h"
#include "media_manager/streamer_listener.h"
#include "protocol_handler/protocol_handler.h"
#include "utils/file_system.h"
#include "utils/helpers.h"
#include "utils/logger.h"
#if defined(EXTENDED_MEDIA_MODE)
#include "media_manager/audio/a2dp_source_player_adapter.h"
#include "media_manager/audio/from_mic_recorder_adapter.h"
#endif
#include "media_manager/audio/file_audio_streamer_adapter.h"
#include "media_manager/audio/pipe_audio_streamer_adapter.h"
#include "media_manager/audio/socket_audio_streamer_adapter.h"
#include "media_manager/media_manager_settings.h"
#include "media_manager/video/file_video_streamer_adapter.h"
#include "media_manager/video/pipe_video_streamer_adapter.h"
#include "media_manager/video/socket_video_streamer_adapter.h"

namespace media_manager {

SDL_CREATE_LOG_VARIABLE("MediaManager")

MediaManagerImpl::MediaManagerImpl(
    application_manager::ApplicationManager& application_manager,
    const MediaManagerSettings& settings)
    : settings_(settings)
    , protocol_handler_(NULL)
    , a2dp_player_(NULL)
    , from_mic_recorder_(NULL)
    , stream_data_size_(0ull)
    , application_manager_(application_manager) {
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
  a2dp_player_ = media_adapter;
}

void MediaManagerImpl::set_mock_mic_listener(MediaListenerPtr media_listener) {
  from_mic_listener_ = media_listener;
}

#ifdef EXTENDED_MEDIA_MODE
void MediaManagerImpl::set_mock_mic_recorder(MediaAdapterImpl* media_adapter) {
  from_mic_recorder_ = media_adapter;
}

#endif  // EXTENDED_MEDIA_MODE

void MediaManagerImpl::set_mock_streamer(
    protocol_handler::ServiceType stype,
    std::shared_ptr<MediaAdapterImpl> mock_stream) {
  streamer_[stype] = mock_stream;
}

void MediaManagerImpl::set_mock_streamer_listener(
    protocol_handler::ServiceType stype,
    std::shared_ptr<MediaAdapterListener> mock_stream) {
  streamer_listener_[stype] = mock_stream;
}

#endif  // BUILD_TESTS

void MediaManagerImpl::Init() {
  using namespace protocol_handler;
  SDL_LOG_INFO("MediaManagerImpl::Init()");

#if defined(EXTENDED_MEDIA_MODE)
  SDL_LOG_INFO("Called Init with default configuration.");
  from_mic_recorder_ = new FromMicRecorderAdapter();
#endif

  if ("socket" == settings().video_server_type()) {
    streamer_[ServiceType::kMobileNav] =
        std::make_shared<SocketVideoStreamerAdapter>(
            settings().server_address(), settings().video_streaming_port());
  } else if ("pipe" == settings().video_server_type()) {
    streamer_[ServiceType::kMobileNav] =
        std::make_shared<PipeVideoStreamerAdapter>(
            settings().named_video_pipe_path(),
            settings().app_storage_folder());
  } else if ("file" == settings().video_server_type()) {
    streamer_[ServiceType::kMobileNav] =
        std::make_shared<FileVideoStreamerAdapter>(
            settings().video_stream_file(), settings().app_storage_folder());
  }

  if ("socket" == settings().audio_server_type()) {
    streamer_[ServiceType::kAudio] =
        std::make_shared<SocketAudioStreamerAdapter>(
            settings().server_address(), settings().audio_streaming_port());
  } else if ("pipe" == settings().audio_server_type()) {
    streamer_[ServiceType::kAudio] = std::make_shared<PipeAudioStreamerAdapter>(
        settings().named_audio_pipe_path(), settings().app_storage_folder());
  } else if ("file" == settings().audio_server_type()) {
    streamer_[ServiceType::kAudio] = std::make_shared<FileAudioStreamerAdapter>(
        settings().audio_stream_file(), settings().app_storage_folder());
  }

  streamer_listener_[ServiceType::kMobileNav] =
      std::make_shared<StreamerListener>(*this);
  streamer_listener_[ServiceType::kAudio] =
      std::make_shared<StreamerListener>(*this);

  if (streamer_[ServiceType::kMobileNav]) {
    streamer_[ServiceType::kMobileNav]->AddListener(
        streamer_listener_[ServiceType::kMobileNav]);
  }

  if (streamer_[ServiceType::kAudio]) {
    streamer_[ServiceType::kAudio]->AddListener(
        streamer_listener_[ServiceType::kAudio]);
  }
}

void MediaManagerImpl::StartMicrophoneRecording(
    int32_t application_key,
    const std::string& output_file,
    int32_t duration,
    mobile_apis::SamplingRate::eType sampling_rate,
    mobile_apis::BitsPerSample::eType bits_per_sample,
    mobile_apis::AudioType::eType audio_type) {
  SDL_LOG_INFO("MediaManagerImpl::StartMicrophoneRecording to " << output_file);
  application_manager::ApplicationSharedPtr app =
      application_manager_.application(application_key);
  std::string file_path = settings().app_storage_folder();
  file_path += "/";
  file_path += output_file;
  from_mic_listener_ = std::make_shared<FromMicRecorderListener>(
      file_path, application_manager_);
#ifdef EXTENDED_MEDIA_MODE
  if (from_mic_recorder_) {
    from_mic_recorder_->AddListener(from_mic_listener_);
    (static_cast<FromMicRecorderAdapter*>(from_mic_recorder_))
        ->set_output_file(file_path);
    (static_cast<FromMicRecorderAdapter*>(from_mic_recorder_))
        ->set_config(sampling_rate, bits_per_sample, audio_type, duration);
    from_mic_recorder_->StartActivity(application_key);
  }
#else
  if (file_system::FileExists(file_path)) {
    SDL_LOG_INFO("File " << output_file << " exists, removing");
    if (file_system::DeleteFile(file_path)) {
      SDL_LOG_INFO("File " << output_file << " removed");
    } else {
      SDL_LOG_WARN("Could not remove file " << output_file);
    }
  }
  const std::string record_file_source = settings().app_resource_folder() +
                                         "/" +
                                         settings().recording_file_source();
  std::vector<uint8_t> buf;
  if (file_system::ReadBinaryFile(record_file_source, buf)) {
    if (file_system::Write(file_path, buf)) {
      SDL_LOG_INFO("File " << record_file_source << " copied to "
                           << output_file);
    } else {
      SDL_LOG_WARN("Could not write to file " << output_file);
    }
  } else {
    SDL_LOG_WARN("Could not read file " << record_file_source);
  }
#endif
  from_mic_listener_->OnActivityStarted(application_key);
}

void MediaManagerImpl::StopMicrophoneRecording(int32_t application_key) {
  SDL_LOG_AUTO_TRACE();
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
  SDL_LOG_AUTO_TRACE();

  if (streamer_[service_type]) {
    streamer_[service_type]->StartActivity(application_key);
  }
}

void MediaManagerImpl::StopStreaming(
    int32_t application_key, protocol_handler::ServiceType service_type) {
  SDL_LOG_AUTO_TRACE();

  stream_data_size_ = 0ull;

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
  SDL_LOG_AUTO_TRACE();

  const uint32_t streaming_app_id = message->connection_key();
  const ServiceType service_type = message->service_type();

  if (Compare<ServiceType, NEQ, ALL>(
          service_type, ServiceType::kMobileNav, ServiceType::kAudio)) {
    SDL_LOG_DEBUG("Unsupported service type in MediaManager");
    return;
  }

  if (!application_manager_.CanAppStream(streaming_app_id, service_type)) {
    application_manager_.ForbidStreaming(streaming_app_id, service_type);
    SDL_LOG_ERROR(
        "The application is trying to stream when it should not."
        " service type: "
        << service_type);
    return;
  }

  ApplicationSharedPtr app = application_manager_.application(streaming_app_id);
  if (app) {
    if (ServiceType::kAudio == service_type) {
      if (stream_data_size_ == 0) {
        socket_audio_stream_start_time_ = std::chrono::system_clock::now();
      }

      stream_data_size_ += message->data_size();
      uint32_t ms_for_all_data = DataSizeToMilliseconds(stream_data_size_);
      uint32_t ms_since_stream_start =
          std::chrono::duration_cast<std::chrono::milliseconds>(
              std::chrono::system_clock::now() -
              socket_audio_stream_start_time_)
              .count();
      uint32_t ms_stream_remaining = ms_for_all_data - ms_since_stream_start;
      SDL_LOG_DEBUG("stream_data_size_: "
                    << stream_data_size_
                    << " ms_for_all_data: " << ms_for_all_data
                    << " ms_since_stream_start: " << ms_since_stream_start
                    << " ms_stream_remaining: " << ms_stream_remaining);

      app->WakeUpStreaming(service_type, ms_stream_remaining);
    } else {
      app->WakeUpStreaming(service_type);
    }
    streamer_[service_type]->SendData(streaming_app_id, message);
  }
}

void MediaManagerImpl::OnMobileMessageSent(
    const ::protocol_handler::RawMessagePtr message) {}

void MediaManagerImpl::FramesProcessed(int32_t application_key,
                                       int32_t frame_number) {
  if (protocol_handler_) {
    protocol_handler_->SendFramesNumber(application_key, frame_number);
  }
}

const MediaManagerSettings& MediaManagerImpl::settings() const {
  return settings_;
}

uint32_t MediaManagerImpl::DataSizeToMilliseconds(uint64_t data_size) const {
  uint32_t bits_per_sample = 16;
  uint32_t sampling_rate = 16000;

  if (application_manager_.hmi_capabilities().pcm_stream_capabilities()) {
    const auto pcm_caps =
        application_manager_.hmi_capabilities().pcm_stream_capabilities();

    if (pcm_caps->keyExists(application_manager::strings::bits_per_sample)) {
      auto type = static_cast<hmi_apis::Common_BitsPerSample::eType>(
          pcm_caps->getElement(application_manager::strings::bits_per_sample)
              .asUInt());
      switch (type) {
        case hmi_apis::Common_BitsPerSample::RATE_8_BIT: {
          bits_per_sample = 8;
          break;
        }
        case hmi_apis::Common_BitsPerSample::RATE_16_BIT: {
          bits_per_sample = 16;
          break;
        }
        default:
          break;
      }
    }

    if (pcm_caps->keyExists(application_manager::strings::sampling_rate)) {
      auto type = static_cast<hmi_apis::Common_SamplingRate::eType>(
          pcm_caps->getElement(application_manager::strings::sampling_rate)
              .asUInt());
      switch (type) {
        case hmi_apis::Common_SamplingRate::RATE_8KHZ: {
          sampling_rate = 8000;
          break;
        }
        case hmi_apis::Common_SamplingRate::RATE_16KHZ: {
          sampling_rate = 16000;
          break;
        }
        case hmi_apis::Common_SamplingRate::RATE_22KHZ: {
          sampling_rate = 22000;
          break;
        }
        case hmi_apis::Common_SamplingRate::RATE_44KHZ: {
          sampling_rate = 44000;
          break;
        }
        default:
          break;
      }
    }
  }

  SDL_LOG_DEBUG("Params of pcm_stream_capabilities: bits_per_sample: "
                << bits_per_sample << " sampling_rate: " << sampling_rate);

  constexpr uint16_t latency_compensation = 500;
  return 1000 * data_size / (sampling_rate * bits_per_sample / 8) +
         latency_compensation;
}

}  //  namespace media_manager

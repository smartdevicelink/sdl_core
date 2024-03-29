/*
 * Copyright (c) 2016, Ford Motor Company
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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_IMPL_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_IMPL_H_

#include <chrono>
#include <map>
#include <string>
#include "interfaces/MOBILE_API.h"
#include "media_manager/media_adapter_impl.h"
#include "media_manager/media_adapter_listener.h"
#include "media_manager/media_manager.h"
#include "protocol_handler/protocol_handler.h"
#include "protocol_handler/protocol_observer.h"

namespace application_manager {
class ApplicationManager;
}

namespace connection_handler {
class ConnectionHandlerImpl;
}

namespace media_manager {

class MediaManagerImpl : public MediaManager,
                         public protocol_handler::ProtocolObserver {
 public:
  MediaManagerImpl(application_manager::ApplicationManager& application_manager,
                   const MediaManagerSettings& settings);
  virtual ~MediaManagerImpl();

  virtual void StartMicrophoneRecording(
      int32_t application_key,
      const std::string& outputFileName,
      int32_t duration,
      mobile_apis::SamplingRate::eType sampling_rate,
      mobile_apis::BitsPerSample::eType bits_per_sample,
      mobile_apis::AudioType::eType audio_type);
  virtual void StopMicrophoneRecording(int32_t application_key);

  virtual void StartStreaming(int32_t application_key,
                              protocol_handler::ServiceType service_type);
  virtual void StopStreaming(int32_t application_key,
                             protocol_handler::ServiceType service_type);

  virtual void SetProtocolHandler(
      protocol_handler::ProtocolHandler* protocol_handler);
  virtual void OnMessageReceived(
      const ::protocol_handler::RawMessagePtr message);
  virtual void OnMobileMessageSent(
      const ::protocol_handler::RawMessagePtr message);
  virtual void FramesProcessed(int32_t application_key, int32_t frame_number);

  virtual const MediaManagerSettings& settings() const OVERRIDE;

  virtual uint32_t DataSizeToMilliseconds(uint64_t data_size) const OVERRIDE;

#ifdef BUILD_TESTS
  void set_mock_a2dp_player(MediaAdapter* media_adapter);
  void set_mock_mic_listener(MediaListenerPtr media_listener);
  void set_mock_mic_recorder(MediaAdapterImpl* media_adapter);
  void set_mock_streamer(protocol_handler::ServiceType stype,
                         std::shared_ptr<MediaAdapterImpl> mock_stream);
  void set_mock_streamer_listener(
      protocol_handler::ServiceType stype,
      std::shared_ptr<MediaAdapterListener> mock_stream);
#endif  // BUILD_TESTS

 protected:
  virtual void Init();

  const MediaManagerSettings& settings_;

  protocol_handler::ProtocolHandler* protocol_handler_;
  MediaAdapter* a2dp_player_;

  MediaAdapterImpl* from_mic_recorder_;
  MediaListenerPtr from_mic_listener_;

  std::map<protocol_handler::ServiceType, MediaAdapterImplPtr> streamer_;
  std::map<protocol_handler::ServiceType, MediaListenerPtr> streamer_listener_;

  uint64_t stream_data_size_;
  std::chrono::time_point<std::chrono::system_clock>
      socket_audio_stream_start_time_;

  application_manager::ApplicationManager& application_manager_;

 private:
  DISALLOW_COPY_AND_ASSIGN(MediaManagerImpl);
};

}  //  namespace media_manager

#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_IMPL_H_

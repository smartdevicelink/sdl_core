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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_IMPL_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_IMPL_H_

#include <string>
#include "utils/singleton.h"
#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/protocol_handler.h"
#include "protocol/service_type.h"
#include "media_manager/media_manager.h"
#include "media_manager/media_adapter_impl.h"
#include "media_manager/media_adapter_listener.h"

namespace media_manager {

class MediaManagerImpl : public MediaManager,
  public protocol_handler::ProtocolObserver,
  public utils::Singleton<MediaManagerImpl> {
  public:
    virtual ~MediaManagerImpl();
    virtual void SetProtocolHandler(
      protocol_handler::ProtocolHandler* protocol_handler);
    virtual void PlayA2DPSource(int32_t application_key);
    virtual void StopA2DPSource(int32_t application_key);
    virtual void StartMicrophoneRecording(int32_t application_key,
                                          const std::string& outputFileName,
                                          int32_t duration);
    virtual void StopMicrophoneRecording(int32_t application_key);
    virtual void StartVideoStreaming(int32_t application_key);
    virtual void StopVideoStreaming(int32_t application_key);
    virtual void StartAudioStreaming(int32_t application_key);
    virtual void StopAudioStreaming(int32_t application_key);
    virtual void OnMessageReceived(
      const ::protocol_handler::RawMessagePtr message);
    virtual void OnMobileMessageSent(
      const ::protocol_handler::RawMessagePtr message);
    virtual void FramesProcessed(int32_t application_key, int32_t frame_number);

  protected:
    MediaManagerImpl();
    virtual void Init();
    protocol_handler::ProtocolHandler* protocol_handler_;
    MediaAdapter*                      a2dp_player_;
    MediaAdapterImpl*                  from_mic_recorder_;
    MediaListenerPtr                   from_mic_listener_;
    MediaAdapterImpl*                  video_streamer_;
    MediaAdapterImpl*                  audio_streamer_;
    MediaListenerPtr                   video_streamer_listener_;
    MediaListenerPtr                   audio_streamer_listener_;
    bool                               video_stream_active_;
    bool                               audio_stream_active_;

  private:
    DISALLOW_COPY_AND_ASSIGN(MediaManagerImpl);
    FRIEND_BASE_SINGLETON_CLASS(MediaManagerImpl);
};

}  //  namespace media_manager
#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_IMPL_H_

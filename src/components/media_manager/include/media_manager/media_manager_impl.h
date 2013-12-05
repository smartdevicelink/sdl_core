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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_IMPL_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_IMPL_H_

#include <string>
#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/protocol_handler.h"
#include "media_manager/media_manager.h"
#include "media_manager/media_adapter_impl.h"
#include "media_manager/media_adapter_listener.h"

namespace media_manager {

class MediaManagerImpl : public MediaManager,
  public protocol_handler::ProtocolObserver {
  public:
    static MediaManagerImpl* instance();
    virtual ~MediaManagerImpl();
    virtual void SetProtocolHandler(
      protocol_handler::ProtocolHandler* protocol_handler);
    virtual void PlayA2DPSource(int application_key);
    virtual void StopA2DPSource(int application_key);
    virtual void StartMicrophoneRecording(int application_key,
                                          const std::string& outputFileName,
                                          int duration);
    virtual void StopMicrophoneRecording(int application_key);
    virtual void StartVideoStreaming(int application_key);
    virtual void StopVideoStreaming(int application_key);
    virtual void OnMessageReceived(
      const protocol_handler::RawMessagePtr& message);
    virtual void OnMobileMessageSent(
      const protocol_handler::RawMessagePtr& message);
    virtual void FramesProcessed(int application_key, int frame_number);

  protected:
    MediaManagerImpl();
    virtual void Init();
    protocol_handler::ProtocolHandler* protocol_handler_;
    MediaAdapter* a2dp_player_;
    MediaAdapterImpl* from_mic_recorder_;
    MediaListenerPtr from_mic_listener_;
    MediaAdapterImpl* video_streamer_;
    MediaListenerPtr video_streamer_listener_;

  private:
    static log4cxx::LoggerPtr logger_;
    DISALLOW_COPY_AND_ASSIGN(MediaManagerImpl);
};

}  //  namespace media_manager
#endif  //  SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_IMPL_H_

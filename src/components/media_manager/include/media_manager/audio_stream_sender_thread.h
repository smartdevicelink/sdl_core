//
// Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_AUDIO_STREAM_SENDER_THREAD_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_AUDIO_STREAM_SENDER_THREAD_H_

#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/threads/thread_delegate.h"
#include "utils/synchronisation_primitives.h"
#include "utils/timer.h"

namespace NsSmartDeviceLink {
namespace NsSmartObjects {
class SmartObject;
}
}

namespace media_manager {

typedef enum {
  SR_INVALID = -1,
  SR_8KHZ = 0,
  SR_16KHZ = 1,
  SR_22KHZ = 2,
  SR_44KHZ = 3
} SamplingRate;

typedef enum {
  ACQ_INVALID = -1,
  ACQ_8_BIT = 0,
  ACQ_16_BIT = 1
} AudioCaptureQuality;

typedef enum {
  AT_INVALID = -1,
  AT_PCM = 0
} AudioType;

/*
 * @brief AudioStreamSenderThread class used to read binary data written from microphone
 * and send it every 1 second to mobile device.
 */
class AudioStreamSenderThread : public threads::ThreadDelegate {
 public:
  /*
   * @brief AudioStreamSenderThread class constructor
   *
   * @param session_key     Session key of connection for Mobile side
   * @param correlation_id  Correlation id for response for Mobile side
   */
  AudioStreamSenderThread(const std::string fileName, unsigned int session_key,
                          unsigned int correlation_id);

  /*
   * @brief AudioStreamSenderThread class destructor
   */
  ~AudioStreamSenderThread();

  /**
   * @brief Thread procedure.
   */
  void threadMain();

  /*
   * @brief Retrieve session key
   *
   * @return Stored session key
   */
  unsigned int session_key() const;

  /*
   * @brief Retrieve correlation id
   *
   * @return Stored correlation id
   */
  unsigned int correlation_id() const;

  void exitThreadMain();

 private:
  /*
   * @brief Sends AudioPassThru request
   */
  bool SendEndAudioPassThru();

  void sendAudioChunkToMobile();

  /*
   * @brief Creates command for corresponding smart object
   *
   * @param cmd Smart object representing command
   */
  void FactoryCreateCommand(
      NsSmartDeviceLink::NsSmartObjects::SmartObject* cmd);

  unsigned int session_key_;
  unsigned int correlation_id_;
  const std::string fileName_;
    int offset_;
  bool shouldBeStoped_;
  sync_primitives::SynchronisationPrimitives stopFlagMutex_;

  static const int kAudioPassThruTimeout;
  static log4cxx::LoggerPtr logger_;

  DISALLOW_COPY_AND_ASSIGN(AudioStreamSenderThread);
};
}  // namespace media_manager

#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_AUDIO_STREAM_SENDER_THREAD_H_

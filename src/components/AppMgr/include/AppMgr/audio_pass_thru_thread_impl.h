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

#ifndef SRC_COMPONENTS_APPMGR_INCLUDE_APPMGR_AUDIO_PASS_THRU_H_
#define SRC_COMPONENTS_APPMGR_INCLUDE_APPMGR_AUDIO_PASS_THRU_H_

#include "LoggerHelper.hpp"

#include "JSONHandler/SDLRPCObjects/V2/SamplingRate.h"
#include "JSONHandler/SDLRPCObjects/V2/AudioType.h"
#include "JSONHandler/SDLRPCObjects/V2/AudioCaptureQuality.h"

#include "utils/macro.h"
#include "utils/threads/thread_delegate.h"
#include "utils/synchronisation_primitives.h"
#include "utils/timer.h"

namespace rpc_commands {
class AudioPassThruThreadImpl : public threads::IThreadDelegate {
 public:
  AudioPassThruThreadImpl(
      unsigned int session_key, unsigned int id, unsigned int max_duration,
      const NsSmartDeviceLinkRPCV2::SamplingRate& sampling_rate,
      const NsSmartDeviceLinkRPCV2::AudioCaptureQuality& bits_per_sample,
      const NsSmartDeviceLinkRPCV2::AudioType& audio_type);
  ~AudioPassThruThreadImpl();

  void threadMain();

  unsigned int session_key() const;
  unsigned int id() const;
  unsigned int max_duration() const;

  const NsSmartDeviceLinkRPCV2::SamplingRate& sampling_rate() const;
  const NsSmartDeviceLinkRPCV2::AudioCaptureQuality& bits_per_sample() const;
  const NsSmartDeviceLinkRPCV2::AudioType& audio_type() const;

  static const int kAudioPassThruTimeout;

 private:
  bool sendEndAudioPassThruToHMI();
  static void audioPassThruTimerProc(int i);

  unsigned int session_key_;
  unsigned int id_;
  unsigned int max_duration_;

  threads::SynchronisationPrimitives synchronisation_;
  threads::Timer * timer_;

  NsSmartDeviceLinkRPCV2::SamplingRate sampling_rate_;
  NsSmartDeviceLinkRPCV2::AudioCaptureQuality bits_per_sample_;
  NsSmartDeviceLinkRPCV2::AudioType audio_type_;

  static log4cplus::Logger logger_;

  DISALLOW_COPY_AND_ASSIGN(AudioPassThruThreadImpl);
};
}  // namespace rpc_commands

#endif  // SRC_COMPONENTS_APPMGR_INCLUDE_APPMGR_AUDIO_PASS_THRU_H_

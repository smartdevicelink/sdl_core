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

#ifndef SRC_COMPONENTS_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_H_

#include <string>
#include "protocol/service_type.h"
#include "media_manager/media_manager_settings.h"
#include "utils/macro.h"  // for "DEPRECATED"
namespace media_manager {

/**
 * \brief Describes different sampling options for PerformAudioPassThru.
 */
typedef enum {
  /// Invalid sampling rate.
  SR_INVALID = -1,
  /// Sampling rate of 8000 Hz.
  SR_8KHZ = 0,
  /// Sampling rate of 16000 Hz.
  SR_16KHZ = 1,
  /// Sampling rate of 22050 Hz.
  SR_22KHZ = 2,
  /// Sampling rate of 44100 Hz.
  SR_44KHZ = 3
} SamplingRate;

/**
 * \brief Describes different quality options for PerformAudioPassThru.
 */
typedef enum {
  /// Invalid audio quality.
  ACQ_INVALID = -1,
  /// Audio sample is 8 bits wide, unsigned.
  ACQ_8_BIT = 0,
  /// Audio sample is 16 bits wide, signed, and in little endian.
  ACQ_16_BIT = 1
} AudioCaptureQuality;

/**
 * \brief Describes different audio type options for PerformAudioPassThru.
 */
typedef enum {
  /// Invalid audio type.
  AT_INVALID = -1,
  /// Linear PCM.
  AT_PCM = 0
} AudioType;

class MediaManager {
 public:
  virtual void PlayA2DPSource(int32_t application_key) = 0;
  virtual void StopA2DPSource(int32_t application_key) = 0;

  DEPRECATED virtual void StartMicrophoneRecording(
      int32_t application_key,
      const std::string& outputFileName,
      int32_t duration) = 0;
  virtual void StartMicrophoneRecording(int32_t application_key,
                                        const std::string& outputFileName,
                                        int32_t duration,
                                        SamplingRate sampling_rate,
                                        AudioCaptureQuality bits_per_sample,
                                        AudioType audio_type) = 0;
  virtual void StopMicrophoneRecording(int32_t application_key) = 0;

  virtual void StartStreaming(int32_t application_key,
                              protocol_handler::ServiceType service_type) = 0;
  virtual void StopStreaming(int32_t application_key,
                             protocol_handler::ServiceType service_type) = 0;
  virtual void FramesProcessed(int32_t application_key,
                               int32_t frame_number) = 0;
  /**
   * \brief Media manager settings getter
   * \return pointer to media manager settings class
   */
  virtual const MediaManagerSettings& settings() const = 0;

  virtual ~MediaManager() {}
};

}  // namespace media_manager

#endif  // SRC_COMPONENTS_INCLUDE_MEDIA_MANAGER_MEDIA_MANAGER_H_

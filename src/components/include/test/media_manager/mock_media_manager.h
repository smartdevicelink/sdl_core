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
#ifndef SRC_COMPONENTS_INCLUDE_TEST_MEDIA_MANAGER_MOCK_MEDIA_MANAGER_H_
#define SRC_COMPONENTS_INCLUDE_TEST_MEDIA_MANAGER_MOCK_MEDIA_MANAGER_H_

#include "gmock/gmock.h"
#include "media_manager/media_manager.h"

namespace test {
namespace components {
namespace media_manager_test {

class MockMediaManager : public media_manager::MediaManager {
 public:
  MOCK_METHOD1(PlayA2DPSource, void(int32_t application_key));
  MOCK_METHOD1(StopA2DPSource, void(int32_t application_key));
  MOCK_METHOD3(StartMicrophoneRecording,
               void(int32_t application_key,
                    const std::string& outputFileName,
                    int32_t duration));
  MOCK_METHOD6(StartMicrophoneRecording,
               void(int32_t application_key,
                    const std::string& outputFileName,
                    int32_t duration,
                    media_manager::SamplingRate sampling_rate,
                    media_manager::AudioCaptureQuality bits_per_sample,
                    media_manager::AudioType audio_type));
  MOCK_METHOD1(StopMicrophoneRecording, void(int32_t application_key));
  MOCK_METHOD2(StartStreaming,
               void(int32_t application_key,
                    protocol_handler::ServiceType service_type));
  MOCK_METHOD2(StopStreaming,
               void(int32_t application_key,
                    protocol_handler::ServiceType service_type));
  MOCK_METHOD2(FramesProcessed,
               void(int32_t application_key, int32_t frame_number));
  MOCK_CONST_METHOD0(settings, const media_manager::MediaManagerSettings&());
};

}  // namespace media_manager_test
}  // namespace components
}  // namespace test

#endif  // SRC_COMPONENTS_INCLUDE_TEST_MEDIA_MANAGER_MOCK_MEDIA_MANAGER_H_

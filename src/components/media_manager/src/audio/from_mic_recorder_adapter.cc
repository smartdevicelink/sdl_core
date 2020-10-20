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

#include "media_manager/audio/from_mic_recorder_adapter.h"
#include <string>
#include "interfaces/MOBILE_API.h"
#include "media_manager/audio/from_mic_to_file_recorder_thread.h"
#include "utils/logger.h"
#include "utils/threads/thread.h"

namespace media_manager {

SDL_CREATE_LOG_VARIABLE("MediaManager")

FromMicRecorderAdapter::FromMicRecorderAdapter()
    : recorder_thread_(NULL)
    , output_file_("default_recorded_audio.wav")
    , kDefaultDuration(1000)
    , sampling_rate_(mobile_apis::SamplingRate::INVALID_ENUM)
    , bits_per_sample_(mobile_apis::BitsPerSample::INVALID_ENUM)
    , audio_type_(mobile_apis::AudioType::INVALID_ENUM)
    , duration_(kDefaultDuration) {}

FromMicRecorderAdapter::~FromMicRecorderAdapter() {
  SDL_LOG_AUTO_TRACE();
  if (recorder_thread_) {
    recorder_thread_->Stop(threads::Thread::kThreadSoftStop);
    delete recorder_thread_->GetDelegate();
    threads::DeleteThread(recorder_thread_);
  }
}

void FromMicRecorderAdapter::StartActivity(int32_t application_key) {
  SDL_LOG_DEBUG("Start with app " << application_key);
  if (application_key == current_application_) {
    SDL_LOG_WARN("Running recording from mic for " << current_application_);
    return;
  }

  // Todd: No gstreamer recorder thread
  if (!recorder_thread_) {
    FromMicToFileRecorderThread* thread_delegate =
        new FromMicToFileRecorderThread(output_file_,
                                        duration_,
                                        sampling_rate_,
                                        bits_per_sample_,
                                        audio_type_);
    recorder_thread_ = threads::CreateThread("MicrophoneRec", thread_delegate);
  }

  if (NULL != recorder_thread_) {
    recorder_thread_->Start();
    current_application_ = application_key;
  }
}

void FromMicRecorderAdapter::StopActivity(int32_t application_key) {
  SDL_LOG_INFO("FromMicRecorderAdapter::StopActivity " << application_key);
  if (application_key != current_application_) {
    SDL_LOG_WARN("Running activity on other app key " << current_application_);
    return;
  }

  if (recorder_thread_) {
    recorder_thread_->Stop(threads::Thread::kThreadSoftStop);
    delete recorder_thread_->GetDelegate();
    threads::DeleteThread(recorder_thread_);
    recorder_thread_ = NULL;
  }
  current_application_ = 0;
}

bool FromMicRecorderAdapter::is_app_performing_activity(
    int32_t application_key) const {
  return (application_key == current_application_);
}

void FromMicRecorderAdapter::set_output_file(const std::string& output_file) {
  output_file_ = output_file;
}

void FromMicRecorderAdapter::set_duration(int32_t duration) {
  duration_ = duration;
}

void FromMicRecorderAdapter::set_config(
    mobile_apis::SamplingRate::eType sampling_rate,
    mobile_apis::BitsPerSample::eType bits_per_sample,
    mobile_apis::AudioType::eType audio_type,
    int32_t duration) {
  sampling_rate_ = sampling_rate;
  bits_per_sample_ = bits_per_sample;
  audio_type_ = audio_type;
  duration_ = duration;
}

}  // namespace media_manager

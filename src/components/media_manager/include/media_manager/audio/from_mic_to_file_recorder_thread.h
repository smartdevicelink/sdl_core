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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_FROM_MIC_TO_FILE_RECORDER_THREAD_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_FROM_MIC_TO_FILE_RECORDER_THREAD_H_

#include <gst/gst.h>
#include <net/if.h>
#include <string>

#include "interfaces/MOBILE_API.h"
#include "utils/lock.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace media_manager {

class FromMicToFileRecorderThread : public threads::ThreadDelegate {
 public:
  FromMicToFileRecorderThread(
      const std::string& output_file,
      int32_t duration,
      mobile_apis::SamplingRate::eType sampling_rate =
          mobile_apis::SamplingRate::INVALID_ENUM,
      mobile_apis::BitsPerSample::eType bits_per_sample =
          mobile_apis::BitsPerSample::INVALID_ENUM,
      mobile_apis::AudioType::eType audio_type =
          mobile_apis::AudioType::INVALID_ENUM);
  ~FromMicToFileRecorderThread();
  void threadMain();

  void exitThreadMain();

  void set_output_file(const std::string& output_file);
  void set_record_duration(int32_t duration);

 private:
  int32_t argc_;
  gchar** argv_;

  const std::string oKey_;
  const std::string tKey_;

  static GMainLoop* loop;
  threads::Thread* sleepThread_;
  bool shouldBeStoped_;
  sync_primitives::Lock stopFlagLock_;

  std::string outputFileName_, durationString_;
  mobile_apis::SamplingRate::eType samplingRate_;
  mobile_apis::BitsPerSample::eType bitsPerSample_;

  typedef struct {
    GstElement* pipeline;
    gint duration;
  } GstTimeout;

  void initArgs();
  void deinitArgs();

  void psleep(void* timeout);
  // create_caps_string() creates a string which is fed to capsfilter's "caps"
  // property. The string specifies audio format. example:
  // "audio/x-raw,format=(string)S16LE,rate=44100,channels=1"
  std::string create_caps_string();

  class SleepThreadDelegate : public threads::ThreadDelegate {
   public:
    explicit SleepThreadDelegate(GstTimeout timeout);

    void threadMain();

   private:
    GstTimeout timeout_;

    DISALLOW_COPY_AND_ASSIGN(SleepThreadDelegate);
  };

  DISALLOW_COPY_AND_ASSIGN(FromMicToFileRecorderThread);
};

}  // namespace media_manager

#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_FROM_MIC_TO_FILE_RECORDER_THREAD_H_

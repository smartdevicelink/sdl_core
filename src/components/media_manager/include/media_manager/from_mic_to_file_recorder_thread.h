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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_AUDIO_MANAGER_FROM_MIC_TO_FILE_RECORDER_THREAD_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_AUDIO_MANAGER_FROM_MIC_TO_FILE_RECORDER_THREAD_H_

#include <net/if.h>
#include <gst/gst.h>
#include <string>
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/synchronisation_primitives.h"

namespace media_manager {

class FromMicToFileRecorderThread : public threads::ThreadDelegate {
 public:
  FromMicToFileRecorderThread();

  void threadMain();

  void exitThreadMain();

  void setOutputFileName(const std::string& outputFileName);
  void setRecordDuration(int duration);

 private:
  static log4cxx::LoggerPtr logger_;

  int argc_;
  gchar** argv_;

  const std::string oKey_;
  const std::string tKey_;

  static GMainLoop *loop;
  threads::Thread* sleepThread_;
  bool shouldBeStoped_;
  sync_primitives::SynchronisationPrimitives stopFlagMutex_;

  std::string outputFileName_, durationString_;

  typedef struct {
    GstElement *pipeline;
    gint duration;
  } GstTimeout;

  void initArgs();

  void psleep(void *timeout);

  class SleepThreadDelegate : public threads::ThreadDelegate {
   public:
    SleepThreadDelegate(GstTimeout timeout);

    void threadMain();

   private:
    GstTimeout timeout_;

    DISALLOW_COPY_AND_ASSIGN(SleepThreadDelegate);
  };

  DISALLOW_COPY_AND_ASSIGN(FromMicToFileRecorderThread);
};

}  // namespace media_manager

#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_AUDIO_MANAGER_FROM_MIC_TO_FILE_RECORDER_THREAD_H_

/*
 Copyright (c) 2013, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#include "utils/threads/thread.h"
#include "utils/logger.h"
#include "media_manager/audio/from_mic_recorder_listener.h"
#include "media_manager/audio/audio_stream_sender_thread.h"

namespace media_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "FromMicRecorderListener")

FromMicRecorderListener::FromMicRecorderListener(
  const std::string& file_name)
  : reader_(NULL)
  , file_name_(file_name) {
}

FromMicRecorderListener::~FromMicRecorderListener() {
  if (reader_) {
    reader_->stop();
    reader_ = NULL;
  }
}

void FromMicRecorderListener::OnDataReceived(
  int32_t application_key,
  const DataForListener& data) {
}

void FromMicRecorderListener::OnErrorReceived(
  int32_t application_key,
  const DataForListener& data) {
}

void FromMicRecorderListener::OnActivityStarted(int32_t application_key) {
  LOG4CXX_INFO(logger_, "FromMicRecorderListener::OnActivityStarted "
               << application_key);
  if (application_key == current_application_) {
    return;
  }
  if (!reader_) {
    AudioStreamSenderThread* thread_delegate =
      new AudioStreamSenderThread(file_name_, application_key);
    reader_ = threads::CreateThread("RecorderSender", thread_delegate);
  }
  if (reader_) {
    reader_->start();
    current_application_ = application_key;
  }
}

void FromMicRecorderListener::OnActivityEnded(int32_t application_key) {
  LOG4CXX_INFO(logger_, "FromMicRecorderListener::OnActivityEnded "
               << application_key);
  if (application_key != current_application_) {
    LOG4CXX_WARN(logger_, "Not performing activity on " << application_key
                 << " but on " << current_application_);
    return;
  }
  if (reader_) {
    reader_->stop();
    reader_ = NULL;
  }
  current_application_ = 0;
}

}  //  namespace media_manager

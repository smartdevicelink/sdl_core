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

#if defined(OS_POSIX) && defined(OS_LINUX)
#include <pthread.h>  // TODO(DK): Need to remove
#endif

#include <string>
#include "audio_manager/audio_stream_sender_thread.h"
#include "application_manager/application_manager_impl.h"
#include "application_manager/mobile_command_factory.h"
#include "application_manager/application_impl.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "utils/timer.h"

#include "application_manager/smart_object_keys.h"
#include "application_manager/message.h"

namespace audio_manager {

const int AudioStreamSenderThread::kAudioPassThruTimeout = 1;
log4cxx::LoggerPtr AudioStreamSenderThread::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("AudioPassThruThread"));

AudioStreamSenderThread::AudioStreamSenderThread(
    const std::string fileName, unsigned int session_key,
    unsigned int correlation_id)
    : session_key_(session_key),
      correlation_id_(correlation_id),
      fileName_(fileName) {
  LOG4CXX_TRACE_ENTER(logger_);
  stopFlagMutex_.init();
}

AudioStreamSenderThread::~AudioStreamSenderThread() {
}

void AudioStreamSenderThread::threadMain() {
  LOG4CXX_TRACE_ENTER(logger_);

  offset_ = 0;

  stopFlagMutex_.lock();
  shouldBeStoped_ = false;
  stopFlagMutex_.unlock();

  while (true) {

    stopFlagMutex_.lock();
    bool shouldBeStoped = shouldBeStoped_;
    stopFlagMutex_.unlock();

    if (shouldBeStoped) {
       break;
    }

    sendAudioChunkToMobile();

    stopFlagMutex_.lock();
    shouldBeStoped = shouldBeStoped_;
    stopFlagMutex_.unlock();

    if (shouldBeStoped) {
       break;
    }

  }
}

void AudioStreamSenderThread::sendAudioChunkToMobile() {
  LOG4CXX_TRACE_ENTER(logger_);

  std::vector<unsigned char> binaryData;
  std::vector<unsigned char>::iterator from;
  std::vector<unsigned char>::iterator to;

  usleep(kAudioPassThruTimeout * 1000000);

  if (!file_system::ReadBinaryFile(fileName_, binaryData)) {
    LOG4CXX_ERROR_EXT(logger_, "Unable to read file." << fileName_);

    return;
  }

  if (binaryData.empty()) {
    LOG4CXX_ERROR_EXT(logger_, "Binary data is empty.");
    return;
  }

  LOG4CXX_INFO_EXT(logger_, "offset = " << offset_);

  from = binaryData.begin() + offset_;
  to = binaryData.end();

  if (from < binaryData.end() /*from != binaryData.end()*/) {
    LOG4CXX_INFO_EXT(logger_, "from != binaryData.end()");

    offset_ = offset_ + to - from;

    application_manager::ApplicationManagerImpl::instance()->
        sendAudioPassThroughNotification(session_key_, correlation_id_,
                                         std::vector<unsigned char>(from, to));
     binaryData.clear();
   }
}

void AudioStreamSenderThread::exitThreadMain() {
  LOG4CXX_INFO(logger_, "AudioStreamSenderThread::exitThreadMain");

  stopFlagMutex_.lock();
  shouldBeStoped_ = true;
  stopFlagMutex_.unlock();
}

unsigned int AudioStreamSenderThread::session_key() const {
  return session_key_;
}

unsigned int AudioStreamSenderThread::correlation_id() const {
  return correlation_id_;
}

}  // namespace application_manager

//
// Copyright (c) 2014, Ford Motor Company
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
#include <unistd.h>
#endif


#include <string>
#include <string.h>
#include "application_manager/application_manager_impl.h"
#include "application_manager/mobile_command_factory.h"
#include "application_manager/application_impl.h"
#include "smart_objects/smart_object.h"
#include "interfaces/MOBILE_API.h"
#include "utils/file_system.h"
#include "utils/logger.h"

#include "media_manager/audio/audio_stream_sender_thread.h"
#include "application_manager/smart_object_keys.h"
#include "application_manager/message.h"

namespace media_manager {
using sync_primitives::AutoLock;

const int32_t AudioStreamSenderThread::kAudioPassThruTimeout = 1;
const uint32_t kMqueueMessageSize = 4095;

CREATE_LOGGERPTR_GLOBAL(logger_, "MediaManager")

AudioStreamSenderThread::AudioStreamSenderThread(
  const std::string fileName, uint32_t session_key)
  : session_key_(session_key),
    fileName_(fileName),
    shouldBeStoped_(false),
    shouldBeStoped_lock_(),
    shouldBeStoped_cv_() {
  LOG4CXX_AUTO_TRACE(logger_);
}

AudioStreamSenderThread::~AudioStreamSenderThread() {
}

void AudioStreamSenderThread::threadMain() {
  LOG4CXX_AUTO_TRACE(logger_);

  offset_ = 0;

  while (false == getShouldBeStopped()) {
    AutoLock auto_lock(shouldBeStoped_lock_);
    shouldBeStoped_cv_.WaitFor(auto_lock, kAudioPassThruTimeout * 1000);
    sendAudioChunkToMobile();
  }

}

void AudioStreamSenderThread::sendAudioChunkToMobile() {
  LOG4CXX_AUTO_TRACE(logger_);

  std::vector<uint8_t> binaryData;
  std::vector<uint8_t>::iterator from;
  std::vector<uint8_t>::iterator to;

  if (!file_system::ReadBinaryFile(fileName_, binaryData)) {
    LOG4CXX_ERROR(logger_, "Unable to read file." << fileName_);

    return;
  }

  if (binaryData.empty()) {
    LOG4CXX_ERROR(logger_, "Binary data is empty.");
    return;
  }

  LOG4CXX_INFO(logger_, "offset = " << offset_);

  from = binaryData.begin() + offset_;
  to = binaryData.end();

  if (from < binaryData.end() /*from != binaryData.end()*/) {
    LOG4CXX_INFO(logger_, "from != binaryData.end()");

    offset_ = offset_ + to - from;
    std::vector<uint8_t> data(from, to);

    application_manager::ApplicationManagerImpl::instance()->
    SendAudioPassThroughNotification(session_key_, data);
    binaryData.clear();
  }
#if !defined(EXTENDED_MEDIA_MODE)
  // without recording stream restart reading 1-sec file
  offset_ = 0;
#endif
}

bool AudioStreamSenderThread::getShouldBeStopped() {
  AutoLock auto_lock(shouldBeStoped_lock_);

  return shouldBeStoped_;
}

void AudioStreamSenderThread::setShouldBeStopped(bool should_stop) {
  AutoLock auto_lock(shouldBeStoped_lock_);
  shouldBeStoped_ = should_stop;
  shouldBeStoped_cv_.NotifyOne();
}

void AudioStreamSenderThread::exitThreadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  setShouldBeStopped(true);
}

uint32_t AudioStreamSenderThread::session_key() const {
  return session_key_;
}

}  // namespace media_manager

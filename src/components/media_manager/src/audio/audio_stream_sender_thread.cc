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

#include <string.h>
#include <string>
#include "application_manager/application_impl.h"
#include "application_manager/application_manager.h"
#include "application_manager/commands/command.h"
#include "application_manager/message.h"
#include "application_manager/rpc_service.h"
#include "application_manager/smart_object_keys.h"
#include "interfaces/MOBILE_API.h"
#include "media_manager/audio/audio_stream_sender_thread.h"
#include "media_manager/media_manager_settings.h"
#include "smart_objects/smart_object.h"
#include "utils/file_system.h"
#include "utils/logger.h"

namespace media_manager {
using sync_primitives::AutoLock;
namespace strings = application_manager::strings;

#ifdef EXTENDED_MEDIA_MODE
const int32_t AudioStreamSenderThread::kAudioPassThruTimeout = 50;
#else
const int32_t AudioStreamSenderThread::kAudioPassThruTimeout = 1000;
#endif
const uint32_t kMqueueMessageSize = 4095;

// Size of RIFF header contained in a .wav file: 12 bytes for 'RIFF' chunk
// descriptor, 24 bytes for 'fmt ' sub-chunk and 8 bytes for 'data' sub-chunk
// header.
// The correct format of audio stream for AudioPassThru feature is to include
// only audio sample data. Since both pre-recorded file (audio.8bit.wav) and
// GStreamer-generated file contain RIFF header, we will skip it when reading
// the files.
static const uint32_t kRIFFHeaderSize = 44;

SDL_CREATE_LOG_VARIABLE("MediaManager")

AudioStreamSenderThread::AudioStreamSenderThread(
    const std::string& fileName,
    uint32_t session_key,
    application_manager::ApplicationManager& app_mngr)
    : session_key_(session_key)
    , fileName_(fileName)
    , offset_(kRIFFHeaderSize)
    , shouldBeStoped_(false)
    , shouldBeStoped_lock_()
    , shouldBeStoped_cv_()
    , application_manager_(app_mngr) {
  SDL_LOG_AUTO_TRACE();
}

AudioStreamSenderThread::~AudioStreamSenderThread() {}

void AudioStreamSenderThread::threadMain() {
  SDL_LOG_AUTO_TRACE();

  offset_ = kRIFFHeaderSize;

  while (false == getShouldBeStopped()) {
    AutoLock auto_lock(shouldBeStoped_lock_);
    shouldBeStoped_cv_.WaitFor(auto_lock, kAudioPassThruTimeout);
    sendAudioChunkToMobile();
  }
}

void AudioStreamSenderThread::sendAudioChunkToMobile() {
  SDL_LOG_AUTO_TRACE();

  std::vector<uint8_t> binaryData;
  std::vector<uint8_t>::iterator from;
  std::vector<uint8_t>::iterator to;

  if (!file_system::ReadBinaryFile(fileName_, binaryData)) {
    SDL_LOG_ERROR("Unable to read file." << fileName_);

    return;
  }

  if (binaryData.empty()) {
    SDL_LOG_ERROR("Binary data is empty.");
    return;
  }

  SDL_LOG_INFO("offset = " << offset_);

  from = binaryData.begin() + offset_;
  to = binaryData.end();

  if (from < binaryData.end() /*from != binaryData.end()*/) {
    SDL_LOG_INFO("from != binaryData.end()");

    offset_ = offset_ + to - from;
    std::vector<uint8_t> data(from, to);

    SendAudioPassThroughNotification(session_key_, data);
    binaryData.clear();
  }
#if !defined(EXTENDED_MEDIA_MODE)
  // without recording stream restart reading 1-sec file
  offset_ = kRIFFHeaderSize;
#endif
}

void AudioStreamSenderThread::SendAudioPassThroughNotification(
    uint32_t session_key, std::vector<uint8_t>& binary_data) {
  SDL_LOG_AUTO_TRACE();

  if (!application_manager_.is_audio_pass_thru_active()) {
    SDL_LOG_ERROR(
        "Trying to send PassThroughNotification"
        " when PassThrough is not active");
    return;
  }

  AudioData data;
  data.session_key = session_key;
  data.binary_data = binary_data;

  smart_objects::SmartObjectSPtr on_audio_pass =
      std::make_shared<smart_objects::SmartObject>();

  if (!on_audio_pass) {
    SDL_LOG_ERROR("OnAudioPassThru NULL pointer");
    return;
  }

  SDL_LOG_DEBUG("Fill smart object");

  (*on_audio_pass)[strings::params][strings::message_type] =
      application_manager::MessageType::kNotification;

  (*on_audio_pass)[strings::params][strings::connection_key] =
      static_cast<int32_t>(data.session_key);
  (*on_audio_pass)[strings::params][strings::function_id] =
      mobile_apis::FunctionID::OnAudioPassThruID;

  SDL_LOG_DEBUG("Fill binary data");
  // binary data
  (*on_audio_pass)[strings::params][strings::binary_data] =
      smart_objects::SmartObject(data.binary_data);

  SDL_LOG_DEBUG("After fill binary data");
  SDL_LOG_DEBUG("Send data");
  application_manager_.GetRPCService().ManageMobileCommand(
      on_audio_pass, application_manager::commands::Command::SOURCE_SDL);
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
  SDL_LOG_AUTO_TRACE();
  setShouldBeStopped(true);
}

uint32_t AudioStreamSenderThread::session_key() const {
  return session_key_;
}

}  // namespace media_manager

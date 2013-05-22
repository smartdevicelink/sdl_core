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
#include <sys/socket.h>
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>
#endif

#include "AppMgr/audio_pass_thru_thread_impl.h"
#include "AppMgr/rpc_helper.h"

#include "AppMgr/AppMgrCore.h"
#include "AppMgr/HMIHandler.h"
#include "AppMgr/MobileHandler.h"

#include "JSONHandler/SDLRPCObjects/V2/Result.h"
#include "JSONHandler/SDLRPCObjects/V2/FunctionID.h"
#include "JSONHandler/SDLRPCObjects/V2/PerformAudioPassThru_response.h"
#include "JSONHandler/SDLRPCObjects/V2/OnAudioPassThru.h"

#include "JSONHandler/RPC2Objects/Marshaller.h"
#include "JSONHandler/RPC2Objects/NsRPC2Communication/UI/EndAudioPassThru.h"

#include "JSONHandler/JSONRPC2Handler.h"

#include "utils/file_system.h"
#include "utils/timer.h"

namespace {
#if defined(OS_POSIX) && defined(OS_LINUX)
pthread_cond_t cv;
pthread_mutex_t audioPassThruMutex = PTHREAD_MUTEX_INITIALIZER;
#endif
}

namespace rpc_commands {

const int AudioPassThruThreadImpl::kAudioPassThruTimeout = 1;

log4cxx::LoggerPtr AudioPassThruThreadImpl::logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("AppMgrCore"));

AudioPassThruThreadImpl::AudioPassThruThreadImpl(
  unsigned int session_key, unsigned int id, unsigned int max_duration,
  const NsSmartDeviceLinkRPCV2::SamplingRate& sampling_rate,
  const NsSmartDeviceLinkRPCV2::AudioCaptureQuality& bits_per_sample,
  const NsSmartDeviceLinkRPCV2::AudioType& audio_type)
  : session_key_(session_key),
    id_(id),
    max_duration_(max_duration),
    sampling_rate_(sampling_rate),
    bits_per_sample_(bits_per_sample),
    audio_type_(audio_type),
    timer_(NULL) {
}

AudioPassThruThreadImpl::~AudioPassThruThreadImpl() {
}

void AudioPassThruThreadImpl::Init() {
  synchronisation_.init();
  timer_ = new sync_primitives::Timer(&synchronisation_);
}

void AudioPassThruThreadImpl::audioPassThruTimerProc(int i) {
#if defined(OS_POSIX) && defined(OS_LINUX)
  pthread_cond_signal(&cv);
#endif
}

bool AudioPassThruThreadImpl::sendEndAudioPassThruToHMI() {
  NsRPC2Communication::UI::EndAudioPassThru* endAudioPassThru =
    new NsRPC2Communication::UI::EndAudioPassThru;
  if (!endAudioPassThru) {
    LOG4CXX_ERROR_EXT(
      logger_,
      "OUT_OF_MEMORY: new NsRPC2Communication::UI::EndAudioPassThru.");

    rpc_commands::helpers::sendResponse <
    NsSmartDeviceLinkRPCV2::PerformAudioPassThru_response,
                           NsSmartDeviceLinkRPCV2::Result::ResultInternal > (
                             NsSmartDeviceLinkRPCV2::FunctionID::PerformAudioPassThruID,
                             NsSmartDeviceLinkRPCV2::Result::OUT_OF_MEMORY, id_, false,
                             session_key_);
    return false;
  }

  NsAppManager::Application* app = NsAppManager::AppMgrCore::getInstance()
                                   .getItem(session_key_);
  if (!app) {
    LOG4CXX_ERROR_EXT(logger_,
                      "No application associated with this registry item!");
    return false;
  }

  endAudioPassThru->setId(
    NsAppManager::HMIHandler::getInstance().getJsonRPC2Handler()
    ->getNextMessageId());
  endAudioPassThru->set_appId(app->getAppID());
  NsAppManager::HMIHandler::getInstance().sendRequest(endAudioPassThru);
  return true;
}

void AudioPassThruThreadImpl::threadMain() {
  unsigned int audioLength = 0;
  std::string filename;
  if (bits_per_sample_.get()
      == NsSmartDeviceLinkRPCV2::AudioCaptureQuality::FIX_8_BIT) {
    filename = "audio.8bit.wav";
    audioLength = 5000;
  } else if (bits_per_sample_.get()
             == NsSmartDeviceLinkRPCV2::AudioCaptureQuality::FIX_16_BIT) {
    filename = "";  // TODO(AK): Add file name here.
    // 3 minute audio.
    audioLength = static_cast<unsigned int>(1000 * 60 * 2.7);
  } else {
#if defined(OS_POSIX) && defined(OS_LINUX)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
#endif

    if (sendEndAudioPassThruToHMI()) {
      rpc_commands::helpers::sendResponse <
      NsSmartDeviceLinkRPCV2::PerformAudioPassThru_response,
                             NsSmartDeviceLinkRPCV2::Result::ResultInternal > (
                               NsSmartDeviceLinkRPCV2::FunctionID::PerformAudioPassThruID,
                               NsSmartDeviceLinkRPCV2::Result::GENERIC_ERROR, id_, false,
                               session_key_);
    }

    NsAppManager::AppMgrCore::getInstance().setAudioPassThruFlag(false);

#if defined(OS_POSIX) && defined(OS_LINUX)
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_exit(NULL);
#endif
  }

  std::vector<unsigned char> binaryData;
  if (!file_system::read_binary_file(filename, binaryData)) {
#if defined(OS_POSIX) && defined(OS_LINUX)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
#endif

    LOG4CXX_ERROR_EXT(logger_, "Can't read from file.");

    if (sendEndAudioPassThruToHMI()) {
      rpc_commands::helpers::sendResponse <
      NsSmartDeviceLinkRPCV2::PerformAudioPassThru_response,
                             NsSmartDeviceLinkRPCV2::Result::ResultInternal > (
                               NsSmartDeviceLinkRPCV2::FunctionID::PerformAudioPassThruID,
                               NsSmartDeviceLinkRPCV2::Result::GENERIC_ERROR, id_, false,
                               session_key_);
    }

    NsAppManager::AppMgrCore::getInstance().setAudioPassThruFlag(false);

#if defined(OS_POSIX) && defined(OS_LINUX)
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_exit(NULL);
#endif
  }

  if (binaryData.empty()) {
#if defined(OS_POSIX) && defined(OS_LINUX)
    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
#endif

    LOG4CXX_ERROR_EXT(logger_, "Binary data empty.");

    if (sendEndAudioPassThruToHMI()) {
      rpc_commands::helpers::sendResponse <
      NsSmartDeviceLinkRPCV2::PerformAudioPassThru_response,
                             NsSmartDeviceLinkRPCV2::Result::ResultInternal > (
                               NsSmartDeviceLinkRPCV2::FunctionID::PerformAudioPassThruID,
                               NsSmartDeviceLinkRPCV2::Result::GENERIC_ERROR, id_, false,
                               session_key_);
    }

    NsAppManager::AppMgrCore::getInstance().setAudioPassThruFlag(false);

#if defined(OS_POSIX) && defined(OS_LINUX)
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
    pthread_exit(NULL);
#endif
  }

  unsigned int percentOfAudioLength = 0;  // % of audio file.
  unsigned int dataLength = 0;  // part of file data.

  // Send only part of file
  if (max_duration_ != 0 && max_duration_ < audioLength) {
    percentOfAudioLength = (max_duration_ * 100) / audioLength;
    dataLength = (binaryData.size() * percentOfAudioLength) / 100;
  } else {
    percentOfAudioLength = 100;
    dataLength = binaryData.size();
  }

  // Part of file in seconds =
  // audio length in seconds * (%) of audio length / 100%
  unsigned int seconds = ((audioLength / 1000) * percentOfAudioLength) / 100;
  // Step is data length * AUDIO_PASS_THRU_TIMEOUT / seconds
  unsigned int step = dataLength * kAudioPassThruTimeout / seconds;

  std::vector<unsigned char>::iterator from = binaryData.begin();
  std::vector<unsigned char>::iterator to = from + step;

  // Minimal timeout is 1 second now.
  for (int i = 0; i != seconds; i += kAudioPassThruTimeout) {
#if defined(OS_POSIX) && defined(OS_LINUX)

    LOG4CXX_INFO(logger_, "\n\t\t\t\t\tBefore timer; kAudioPassThruTimeout "
                 << kAudioPassThruTimeout << "; seconds " << seconds);
    timer_->StartWait(kAudioPassThruTimeout);

    pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
#endif

    NsSmartDeviceLinkRPCV2::OnAudioPassThru* onAudioPassThru =
      new NsSmartDeviceLinkRPCV2::OnAudioPassThru;
    if (!onAudioPassThru) {
      LOG4CXX_ERROR_EXT(
        logger_,
        "OUT_OF_MEMORY: new NsSmartDeviceLinkRPCV2::OnAudioPassThru.");

      if (sendEndAudioPassThruToHMI()) {
        rpc_commands::helpers::sendResponse <
        NsSmartDeviceLinkRPCV2::PerformAudioPassThru_response,
                               NsSmartDeviceLinkRPCV2::Result::ResultInternal > (
                                 NsSmartDeviceLinkRPCV2::FunctionID::PerformAudioPassThruID,
                                 NsSmartDeviceLinkRPCV2::Result::OUT_OF_MEMORY, id_, false,
                                 session_key_);
      }

      delete onAudioPassThru;
      NsAppManager::AppMgrCore::getInstance().setAudioPassThruFlag(false);

#if defined(OS_POSIX) && defined(OS_LINUX)
      pthread_exit(NULL);
#endif
    }

    onAudioPassThru->setBinaryData(std::vector<unsigned char>(from, to));
    onAudioPassThru->setMethodId(
      NsSmartDeviceLinkRPCV2::FunctionID::OnAudioPassThruID);
    onAudioPassThru->setMessageType(
      NsSmartDeviceLinkRPC::SDLRPCMessage::NOTIFICATION);

    NsAppManager::MobileHandler::getInstance().sendRPCMessage(onAudioPassThru,
        session_key_);

#if defined(OS_POSIX) && defined(OS_LINUX)
    pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
#endif

    from = to;
    to = to + step;
  }

#if defined(OS_POSIX) && defined(OS_LINUX)
  pthread_setcancelstate(PTHREAD_CANCEL_DISABLE, NULL);
#endif

  NsAppManager::AppMgrCore::getInstance().setAudioPassThruFlag(false);

#if defined(OS_POSIX) && defined(OS_LINUX)
  pthread_setcancelstate(PTHREAD_CANCEL_ENABLE, NULL);
  pthread_exit(NULL);
#endif
}

unsigned int AudioPassThruThreadImpl::session_key() const {
  return session_key_;
}

unsigned int AudioPassThruThreadImpl::id() const {
  return id_;
}

unsigned int AudioPassThruThreadImpl::max_duration() const {
  return max_duration_;
}

const NsSmartDeviceLinkRPCV2::SamplingRate&
AudioPassThruThreadImpl::sampling_rate() const {
  return sampling_rate_;
}

const NsSmartDeviceLinkRPCV2::AudioCaptureQuality&
AudioPassThruThreadImpl::bits_per_sample() const {
  return bits_per_sample_;
}

const NsSmartDeviceLinkRPCV2::AudioType&
AudioPassThruThreadImpl::audio_type() const {
  return audio_type_;
}
}  // namespace rpc_commands

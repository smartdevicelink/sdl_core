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

#include <algorithm>
#include <iterator>
#include "audio_manager/audio_manager_impl.h"

namespace audio_manager {

log4cxx::LoggerPtr AudioManagerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("AudioManagerImpl"));

AudioManagerImpl* AudioManagerImpl::sInstance_ = 0;

const std::string AudioManagerImpl::sA2DPSourcePrefix_ = "bluez_source.";

AudioManager* AudioManagerImpl::getAudioManager() {
  if (0 == sInstance_) {
    sInstance_ = new AudioManagerImpl;
  }

  return sInstance_;
}

AudioManagerImpl::AudioManagerImpl()
// Six hex-pairs + five underscores + '\n'
  : MAC_ADDRESS_LENGTH_(12 + 5 + 1) {
}

AudioManagerImpl::~AudioManagerImpl() {
}

void AudioManagerImpl::addA2DPSource(const std::string& device) {
  if (sources_.find(device) == sources_.end()) {
    sources_.insert(std::pair<std::string, threads::Thread*>(
        device, NULL));
  }
}

void AudioManagerImpl::removeA2DPSource(const std::string& device) {
  std::map<std::string, threads::Thread*>::iterator it =
      sources_.find(device);

  if (it != sources_.end()) {
    // Source exists
    LOG4CXX_DEBUG(logger_, "Source exists");
    if (NULL != (*it).second) {
      // Sources thread was allocated
      LOG4CXX_DEBUG(logger_, "Sources thread was allocated");
      if ((*it).second->is_running()) {
        // Sources thread was started - stop it
        LOG4CXX_DEBUG(logger_, "Sources thread was started - stop it");
        (*it).second->stop();
      }
      // Delete allocated thread
      LOG4CXX_DEBUG(logger_, "Delete allocated thread");
      delete (*it).second;
    }

    sources_.erase(it);
  }
}

void AudioManagerImpl::playA2DPSource(const std::string& device) {
  std::map<std::string, threads::Thread*>::iterator it =
      sources_.find(device);

  if (it != sources_.end()) {
    // Source exists - allocate thread for the source
    (*it).second = new threads::Thread((*it).first.c_str(),
        new A2DPSourcePlayerThread((*it).first));

    if (NULL != (*it).second) {
      // Thread was successfully allocated - start thread
      (*it).second->start();
    }
  }
}

void AudioManagerImpl::stopA2DPSource(const std::string& device) {
  std::map<std::string, threads::Thread*>::iterator it =
      sources_.find(device);

  if (it != sources_.end()) {
    // Source exists
    if (NULL != (*it).second) {
      // Sources thread was allocated
      if ((*it).second->is_running()) {
        // Sources thread was started - stop it
        (*it).second->stop();
      }
    }
  }
}

std::string AudioManagerImpl::sockAddr2SourceAddr(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  char mac[MAC_ADDRESS_LENGTH_];

  snprintf(mac, MAC_ADDRESS_LENGTH_, "%02x_%02x_%02x_%02x_%02x_%02x",
           (unsigned char)device.sa_data[0], (unsigned char)device.sa_data[1],
           (unsigned char)device.sa_data[2], (unsigned char)device.sa_data[3],
           (unsigned char)device.sa_data[4], (unsigned char)device.sa_data[5]);

  std::string ret = std::string(mac);

  std::transform(ret.begin(), ret.end(), ret.begin(), toupper);

  ret = sA2DPSourcePrefix_ + ret;

  LOG4CXX_TRACE(logger_, "Mac : " << ret);

  return ret;
}

void AudioManagerImpl::addA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  addA2DPSource(sockAddr2SourceAddr(device));
}

void AudioManagerImpl::removeA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  removeA2DPSource(sockAddr2SourceAddr(device));
}

void AudioManagerImpl::playA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  playA2DPSource(sockAddr2SourceAddr(device));
}

void AudioManagerImpl::stopA2DPSource(const sockaddr& device) {
  LOG4CXX_TRACE_ENTER(logger_);

  stopA2DPSource(sockAddr2SourceAddr(device));
}

void AudioManagerImpl::startMicrophoneRecording(
    const std::string& outputFileName, int duration) {
  LOG4CXX_TRACE_ENTER(logger_);

  FromMicToFileRecorderThread* recordThreadDelegate =
      new FromMicToFileRecorderThread();

  if(NULL != recordThreadDelegate)
  {
    recordThreadDelegate->setOutputFileName(outputFileName);
    recordThreadDelegate->setRecordDuration(duration);

    recorderThread_ = new threads::Thread("MicrophoneRecorder"
        , recordThreadDelegate);

    if(NULL != recorderThread_) {
      recorderThread_->start();
    }
  }
}

void AudioManagerImpl::stopMicrophoneRecording() {
  LOG4CXX_TRACE_ENTER(logger_);

  if(NULL != recorderThread_) {
    recorderThread_->stop();
    delete recorderThread_;
  }
}

}  // namespace audio_manager

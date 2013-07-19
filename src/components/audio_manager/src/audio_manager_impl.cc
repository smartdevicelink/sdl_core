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

#include <pulse/simple.h>
#include <pulse/error.h>
#include <string.h>
#include <net/if.h>

#include <iterator>
#include "audio_manager/audio_manager_impl.h"

namespace audio_manager {

log4cxx::LoggerPtr AudioManagerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("AudioManagerImpl"));

AudioManagerImpl* AudioManagerImpl::sInstance_ = 0;

AudioManager* AudioManagerImpl::getAudioManager() {
  if (0 == sInstance_) {
    sInstance_ = new AudioManagerImpl;
  }

  return sInstance_;
}

AudioManagerImpl::AudioManagerImpl() {
}

AudioManagerImpl::~AudioManagerImpl() {
}

void AudioManagerImpl::addA2DPSource(const std::string& device) {
  LOG4CXX_TRACE_ENTER(logger_);
  if (sources_.find(device) == sources_.end()) {
    sources_.insert(std::pair<std::string, threads::Thread*>(
        device, NULL));
  }
}

void AudioManagerImpl::removeA2DPSource(const std::string& device) {
}

void AudioManagerImpl::playA2DPSource(const std::string& device) {
  LOG4CXX_TRACE_ENTER(logger_);
  std::map<std::string, threads::Thread*>::iterator it =
      sources_.find(device);

  if (it != sources_.end()) {
    (*it).second = new threads::Thread((*it).first.c_str(),
        new A2DPSourcePlayerThread((*it).first));

    (*it).second->start();
  }
}

void AudioManagerImpl::stopA2DPSource(const std::string& device) {
}

const pa_sample_spec AudioManagerImpl::A2DPSourcePlayerThread::
      sSampleFormat_ = {
      /*format*/    PA_SAMPLE_S16LE,
      /*rate*/      44100,
      /*channels*/  2 };


AudioManagerImpl::A2DPSourcePlayerThread::A2DPSourcePlayerThread(
    const std::string& device)
    : threads::ThreadDelegate(),
      device_(device),
      BUFSIZE_(32) {
}

void AudioManagerImpl::A2DPSourcePlayerThread::freeStreams() {
  LOG4CXX_TRACE_ENTER(logger_);
  if (s_in) {
    pa_simple_free(s_in);
  }

  if (s_out) {
    pa_simple_free(s_out);
  }
}

void AudioManagerImpl::A2DPSourcePlayerThread::threadMain() {
  LOG4CXX_TRACE_ENTER(logger_);

  int error;

  const char * a2dpSource = device_.c_str();

  LOG4CXX_TRACE(logger_, device_);

  LOG4CXX_INFO(logger_, "Creating streams");

  /* Create a new playback stream */
  if (!(s_out = pa_simple_new(NULL, "AudioManager", PA_STREAM_PLAYBACK, NULL,
                          "playback", &sSampleFormat_, NULL, NULL, &error))) {
    LOG4CXX_ERROR(logger_, "pa_simple_new() failed: " << pa_strerror(error));
    freeStreams();
    return;
  }

  if (!(s_in = pa_simple_new(NULL, "AudioManager", PA_STREAM_RECORD, a2dpSource,
                             "record", &sSampleFormat_, NULL, NULL, &error))) {
    LOG4CXX_ERROR(logger_, "pa_simple_new() failed: " << pa_strerror(error));
    freeStreams();
    return;
  }

  LOG4CXX_INFO(logger_, "Entering main loop");

  for (;;) {
    uint8_t buf[BUFSIZE_];
    ssize_t r;

    pa_usec_t latency;

    if ((latency = pa_simple_get_latency(s_in, &error)) == (pa_usec_t) -1) {
      LOG4CXX_ERROR(logger_, "pa_simple_get_latency() failed: "
                   << pa_strerror(error));
      break;
    }

    LOG4CXX_DEBUG(logger_, "In: " << static_cast<float>(latency));

    if ((latency = pa_simple_get_latency(s_out, &error)) == (pa_usec_t) -1) {
      LOG4CXX_ERROR(logger_, "pa_simple_get_latency() failed: "
                    << pa_strerror(error));
      break;
    }

    LOG4CXX_DEBUG(logger_, "Out: " << static_cast<float>(latency));

    if (pa_simple_read(s_in, buf, sizeof(buf), &error) < 0) {
      LOG4CXX_ERROR(logger_, "read() failed: " << strerror(error));
      break;
    }

    /* ... and play it */
    if (pa_simple_write(s_out, buf, sizeof(buf), &error) < 0) {
      LOG4CXX_ERROR(logger_, "pa_simple_write() failed: "
                    << pa_strerror(error));
      break;
    }
  }

  /* Make sure that every single sample was played */
  if (pa_simple_drain(s_out, &error) < 0) {
    LOG4CXX_ERROR(logger_, "pa_simple_drain() failed: " << pa_strerror(error));
    freeStreams();
    return;
  }

  freeStreams();
}

}  // namespace audio_manager

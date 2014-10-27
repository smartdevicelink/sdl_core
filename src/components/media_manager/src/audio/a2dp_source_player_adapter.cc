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
#include <net/if.h>
#include <pulse/simple.h>
#include <pulse/error.h>
#include <string.h>
#include <utility>
#include "utils/threads/thread.h"
#include "media_manager/audio/a2dp_source_player_adapter.h"
#include "utils/lock.h"
#include "utils/threads/thread_delegate.h"
#include "utils/logger.h"
#include "connection_handler/connection_handler_impl.h"

namespace media_manager {

CREATE_LOGGERPTR_GLOBAL(logger_, "A2DPSourcePlayerAdapter");

const static size_t BUFSIZE = 32;

class A2DPSourcePlayerAdapter::A2DPSourcePlayerThread
    : public threads::ThreadDelegate {
  public:
    explicit A2DPSourcePlayerThread(const std::string& device);

    void threadMain();

    bool exitThreadMain();

  private:
    // The Sample format to use
    static const pa_sample_spec sSampleFormat_;

    pa_simple* s_in, *s_out;
    std::string device_;
    bool should_be_stopped_;
    sync_primitives::Lock should_be_stopped_lock_;

    void freeStreams();

    DISALLOW_COPY_AND_ASSIGN(A2DPSourcePlayerThread);
};

A2DPSourcePlayerAdapter::A2DPSourcePlayerAdapter() {
}

A2DPSourcePlayerAdapter::~A2DPSourcePlayerAdapter() {
  for (std::map<int32_t, threads::Thread*>::iterator it = sources_.begin();
       sources_.end() != it;
       ++it) {
    if (NULL != it->second) {
      it->second->stop();
      threads::DeleteThread(it->second);
    }
  }
  sources_.clear();
}

void A2DPSourcePlayerAdapter::StartActivity(int32_t application_key) {
  LOG4CXX_INFO(logger_, "Starting a2dp playing music for "
               << application_key << " application.");
  if (application_key != current_application_) {
    current_application_ = application_key;

    uint32_t device_id = 0;
    connection_handler::ConnectionHandlerImpl::instance()->
        GetDataOnSessionKey(application_key, 0, NULL, &device_id);
    std::string mac_adddress;
    connection_handler::ConnectionHandlerImpl::instance()->GetDataOnDeviceID(
      device_id,
      NULL,
      NULL,
      &mac_adddress);

    // TODO(PK): Convert mac_adddress to the
    // following format : "bluez_source.XX_XX_XX_XX_XX_XX" if needed
    // before passing to the A2DPSourcePlayerThread constructor

    threads::Thread* new_activity = threads::CreateThread(
      mac_adddress.c_str(),
      new A2DPSourcePlayerAdapter::A2DPSourcePlayerThread(mac_adddress));
    sources_[application_key] = new_activity;
    new_activity->start();
  }
}

void A2DPSourcePlayerAdapter::StopActivity(int32_t application_key) {
  LOG4CXX_INFO(logger_, "Stopping 2dp playing for "
               << application_key << " application.");
  if (application_key != current_application_) {
    return;
  }
  std::map<int32_t, threads::Thread*>::iterator it =
    sources_.find(application_key);
  if (sources_.end() != it) {
    LOG4CXX_DEBUG(logger_, "Source exists.");
    if (NULL != it->second) {
      LOG4CXX_DEBUG(logger_, "Sources thread was allocated");
      if ((*it).second->is_running()) {
        // Sources thread was started - stop it
        LOG4CXX_DEBUG(logger_, "Sources thread was started - stop it");
        (*it).second->stop();
        threads::DeleteThread(it->second);
      }
    }
    current_application_ = 0;
  }
}

bool A2DPSourcePlayerAdapter::is_app_performing_activity(int32_t
                                                         application_key) {
  return (application_key == current_application_);
}

const pa_sample_spec A2DPSourcePlayerAdapter::A2DPSourcePlayerThread::
sSampleFormat_ = {
  /*format*/    PA_SAMPLE_S16LE,
  /*rate*/      44100,
  /*channels*/  2
};

A2DPSourcePlayerAdapter::A2DPSourcePlayerThread::A2DPSourcePlayerThread(
  const std::string& device)
  : threads::ThreadDelegate(),
    device_(device) {
}

void A2DPSourcePlayerAdapter::A2DPSourcePlayerThread::freeStreams() {
  LOG4CXX_INFO(logger_, "Free streams in A2DPSourcePlayerThread.");
  if (s_in) {
    pa_simple_free(s_in);
  }

  if (s_out) {
    pa_simple_free(s_out);
  }
}

bool A2DPSourcePlayerAdapter::A2DPSourcePlayerThread::exitThreadMain() {
  sync_primitives::AutoLock auto_lock(should_be_stopped_lock_);
  should_be_stopped_ = true;
  return true;
}

void A2DPSourcePlayerAdapter::A2DPSourcePlayerThread::threadMain() {
  LOG4CXX_INFO(logger_, "Main thread of A2DPSourcePlayerThread.");

  {
    sync_primitives::AutoLock auto_lock(should_be_stopped_lock_);
    should_be_stopped_ = false;
  }

  int32_t error;

  const char* a2dpSource = device_.c_str();

  LOG4CXX_DEBUG(logger_, device_);

  LOG4CXX_DEBUG(logger_, "Creating streams");

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

  LOG4CXX_DEBUG(logger_, "Entering main loop");

  for (;;) {
    uint8_t buf[BUFSIZE];

    pa_usec_t latency;

    if ((latency = pa_simple_get_latency(s_in, &error)) == (pa_usec_t) - 1) {
      LOG4CXX_ERROR(logger_, "pa_simple_get_latency() failed: "
                    << pa_strerror(error));
      break;
    }

    // LOG4CXX_INFO(logger_, "In: " << static_cast<float>(latency));

    if ((latency = pa_simple_get_latency(s_out, &error)) == (pa_usec_t) - 1) {
      LOG4CXX_ERROR(logger_, "pa_simple_get_latency() failed: "
                    << pa_strerror(error));
      break;
    }

    // LOG4CXX_INFO(logger_, "Out: " << static_cast<float>(latency));

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

    bool should_be_stopped;
    {
      sync_primitives::AutoLock auto_lock(should_be_stopped_lock_);
      should_be_stopped = should_be_stopped_;
    }

    if (should_be_stopped) {
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

}  // namespace media_manager

/*
 * Copyright (c) 2015, Ford Motor Company
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

#include "media_manager/streamer_adapter.h"
#include "utils/logger.h"

namespace media_manager {

SDL_CREATE_LOG_VARIABLE("StreamerAdapter")

StreamerAdapter::StreamerAdapter(Streamer* const streamer)
    : current_application_(0), messages_(), streamer_(streamer), thread_(NULL) {
  DCHECK(streamer_);
  thread_ = threads::CreateThread("StreamerAdapter", streamer_);
}

StreamerAdapter::~StreamerAdapter() {
  if (streamer_) {
    streamer_->Close();
  }
  thread_->Stop(threads::Thread::kThreadSoftStop);
  delete streamer_;
  threads::DeleteThread(thread_);
}

void StreamerAdapter::StartActivity(int32_t application_key) {
  SDL_LOG_AUTO_TRACE();
  if (is_app_performing_activity(application_key)) {
    SDL_LOG_WARN("Activity for application: " << application_key
                                              << " has been already started");
    return;
  }
  messages_.Reset();

  DCHECK(thread_);
  const size_t kStackSize = 16384;
  thread_->Start(threads::ThreadOptions(kStackSize));

  for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
       media_listeners_.end() != it;
       ++it) {
    (*it)->OnActivityStarted(application_key);
  }
  current_application_ = application_key;
}

size_t StreamerAdapter::GetMsgQueueSize() {
  return messages_.size();
}

void StreamerAdapter::StopActivity(int32_t application_key) {
  SDL_LOG_AUTO_TRACE();
  if (!is_app_performing_activity(application_key)) {
    SDL_LOG_WARN("Activity for application: " << application_key
                                              << " has not been started");
    return;
  }

  DCHECK(streamer_);
  streamer_->exitThreadMain();

  for (std::set<MediaListenerPtr>::iterator it = media_listeners_.begin();
       media_listeners_.end() != it;
       ++it) {
    (*it)->OnActivityEnded(application_key);
  }
  current_application_ = 0;
}

void StreamerAdapter::SendData(int32_t application_key,
                               const ::protocol_handler::RawMessagePtr msg) {
  SDL_LOG_AUTO_TRACE();
  if (!is_app_performing_activity(application_key)) {
    SDL_LOG_ERROR("Activity for application: " << application_key
                                               << " has not been started");
    return;
  }
  messages_.push(msg);
}

bool StreamerAdapter::is_app_performing_activity(
    int32_t application_key) const {
  return application_key == current_application_;
}

StreamerAdapter::Streamer::Streamer(StreamerAdapter* const adapter)
    : stop_flag_(false), adapter_(adapter) {
  DCHECK(adapter_);
}

StreamerAdapter::Streamer::~Streamer() {}

void StreamerAdapter::Streamer::threadMain() {
  SDL_LOG_AUTO_TRACE();
  if (!adapter_) {
    SDL_LOG_ERROR("Null pointer to adapter");
    return;
  }
  if (!Connect()) {
    SDL_LOG_ERROR("Unable to connect");
    return;
  }
  stop_flag_ = false;
  while (!stop_flag_) {
    adapter_->messages_.wait();
    while (!adapter_->messages_.empty()) {
      protocol_handler::RawMessagePtr msg;
      if (!adapter_->messages_.pop(msg)) {
        SDL_LOG_ERROR("Empty message queue");
        continue;
      }
      if (!msg) {
        SDL_LOG_ERROR("Null pointer message");
        continue;
      }
      if (!Send(msg)) {
        SDL_LOG_ERROR("Unable to send. Disconnecting");
        Disconnect();
        return;
      }
      static int32_t messages_for_session = 0;
      ++messages_for_session;

      SDL_LOG_TRACE("Handling map streaming message. This is "
                    << messages_for_session << " message for "
                    << adapter_->current_application_);
      std::set<MediaListenerPtr>::iterator it =
          adapter_->media_listeners_.begin();
      for (; adapter_->media_listeners_.end() != it; ++it) {
        (*it)->OnDataReceived(adapter_->current_application_,
                              messages_for_session);
      }
    }
  }
  Disconnect();
}

void StreamerAdapter::Streamer::exitThreadMain() {
  SDL_LOG_AUTO_TRACE();
  stop_flag_ = true;
  if (adapter_) {
    adapter_->messages_.Shutdown();
  }
}

}  // namespace media_manager

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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_STREAMER_ADAPTER_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_STREAMER_ADAPTER_H_

#include "media_manager/media_adapter_impl.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "utils/atomic_object.h"
#include "utils/shared_ptr.h"
#include "protocol/raw_message.h"

namespace media_manager {

/**
 * Class StreamerAdapter represents media adapter
 * for streaming data to some destination point
 * (pipe, socket, file)
 */
class StreamerAdapter : public MediaAdapterImpl {
 protected:
  class Streamer;

 public:
  explicit StreamerAdapter(Streamer* const streamer);
  virtual ~StreamerAdapter();

  virtual void StartActivity(int32_t application_key);
  virtual void StopActivity(int32_t application_key);
  virtual void SendData(int32_t application_key,
                        const ::protocol_handler::RawMessagePtr msg);
  virtual bool is_app_performing_activity(
      int32_t application_key) const;

 protected:
  // TODO(AN): APPLINK-15203 Use MessageLoopThread
  class Streamer : public threads::ThreadDelegate {
   public:
    explicit Streamer(StreamerAdapter* const adapter);
    virtual ~Streamer();

    virtual void threadMain();
    virtual void exitThreadMain();

   protected:
    virtual bool Connect() = 0;
    virtual void Disconnect() = 0;
    virtual bool Send(protocol_handler::RawMessagePtr msg) = 0;

   private:
    sync_primitives::atomic_bool stop_flag_;
    StreamerAdapter*             adapter_;

    DISALLOW_COPY_AND_ASSIGN(Streamer);
  };

 private:
  int32_t current_application_;
  utils::MessageQueue<protocol_handler::RawMessagePtr> messages_;

  Streamer*        streamer_;
  threads::Thread* thread_;

  DISALLOW_COPY_AND_ASSIGN(StreamerAdapter);
};

typedef utils::SharedPtr<StreamerAdapter> StreamerAdapterPtr;

}  // namespace media_manager

#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_STREAMER_ADAPTER_H_

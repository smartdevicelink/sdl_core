/*
 * Copyright (c) 2014-2015, Ford Motor Company
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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_SOCKET_STREAMER_ADAPTER_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_SOCKET_STREAMER_ADAPTER_H_

#include <string>
#include "media_manager/streamer_adapter.h"
#include "utils/threads/thread_delegate.h"

namespace media_manager {

class SocketStreamerAdapter : public StreamerAdapter {
 public:
  SocketStreamerAdapter(const std::string& ip,
                        uint16_t port,
                        const std::string& header);
  virtual ~SocketStreamerAdapter();

 protected:
  class SocketStreamer : public StreamerAdapter::Streamer {
   public:
    SocketStreamer(SocketStreamerAdapter* const adapter,
                   const std::string& ip,
                   uint16_t port,
                   const std::string& header);
    virtual ~SocketStreamer();

   protected:
    virtual bool Connect();
    virtual void Disconnect();
    virtual bool Send(protocol_handler::RawMessagePtr msg);

   private:
    std::string ip_;
    uint16_t    port_;
    std::string header_;

    int32_t     socket_fd_;
    int32_t     send_socket_fd_;
    bool        is_first_frame_;
  };
};

}  //  namespace media_manager

#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_SOCKET_STREAMER_ADAPTER_H_

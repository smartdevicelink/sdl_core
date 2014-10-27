/*
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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_SOCKET_STREAMER_ADAPTER_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_SOCKET_STREAMER_ADAPTER_H_

#include <string>
#include "media_manager/media_adapter_impl.h"
#include "utils/logger.h"
#include "utils/shared_ptr.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace media_manager {
class SocketStreamerAdapter : public MediaAdapterImpl {
  public:
    SocketStreamerAdapter();
    virtual ~SocketStreamerAdapter();
    virtual void SendData(int32_t application_key,
                          const ::protocol_handler::RawMessagePtr message);
    virtual void StartActivity(int32_t application_key);
    virtual void StopActivity(int32_t application_key);
    virtual bool is_app_performing_activity(int32_t application_key);

  protected:

    /*
     * @brief Start streamer thread
     */
    virtual void Init();

    int32_t port_;
    std::string ip_;

  private:
    class Streamer : public threads::ThreadDelegate {
      public:
        /*
         * Default constructor
         *
         * @param server  Server pointer
         */
        explicit Streamer(SocketStreamerAdapter* const server);

        /*
         * Destructor
         */
        ~Streamer();

        /*
         * Function called by thread on start
         */
        void threadMain();

        /*
         * Function called by thread on exit
         */
        bool exitThreadMain();

        /*
         * Checks if server is ready
         *
         * @return TRUE if socket is ready otherwise FALSE
         */
        bool is_ready() const;

        /*
         * Starts server
         *
         */
        void start();

        /*
         * Stops server
         *
         */
        void stop();

        /*
         * Sends data to connected client
         *
         * @param block Pointer to the data
         */
        bool send(const ::protocol_handler::RawMessagePtr msg);

      private:
        SocketStreamerAdapter* const server_;
        int32_t new_socket_fd_;
        bool is_first_loop_;
        volatile bool is_client_connected_;
        volatile bool stop_flag_;
        sync_primitives::Lock thread_lock;
        DISALLOW_COPY_AND_ASSIGN(Streamer);
    };

    int32_t                                       socket_fd_;
    bool                                          is_ready_;
    threads::Thread*                              thread_;
    Streamer*                                     streamer_;
    MessageQueue<protocol_handler::RawMessagePtr>                   messages_;
    DISALLOW_COPY_AND_ASSIGN(SocketStreamerAdapter);
};
}  //  namespace media_manager


#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_SOCKET_STREAMER_ADAPTER_H_

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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_VIDEO_VIDEO_STREAM_TO_FILE_ADAPTER_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_VIDEO_VIDEO_STREAM_TO_FILE_ADAPTER_H_

#include <string>
#include <fstream>
#include "media_manager/media_adapter_impl.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"

namespace media_manager {

class VideoStreamToFileAdapter : public MediaAdapterImpl {
  public:
    explicit VideoStreamToFileAdapter(const std::string& file_name);
    virtual ~VideoStreamToFileAdapter();
    virtual void SendData(int32_t application_key,
                          const ::protocol_handler::RawMessagePtr message);
    virtual void StartActivity(int32_t application_key);
    virtual void StopActivity(int32_t application_key);
    virtual bool is_app_performing_activity(int32_t application_key);

    /*
     * @brief Start streamer thread
     */
    virtual void Init();

  private:
    class Streamer : public threads::ThreadDelegate {
      public:
        /*
         * Default constructor
         *
         * @param server  Server pointer
         */
        explicit Streamer(VideoStreamToFileAdapter* server);

        /*
         * Destructor
         */
        ~Streamer();

        /*
         * @brief Function called by thread on start
         */
        void threadMain();

        /*
         * @brief Function called by thread on exit
         */
        bool exitThreadMain();

        /*
         * @brief Opens file
         */
        void open();

        /*
         * @brief Closes file
         */
        void close();

      private:
        VideoStreamToFileAdapter*   server_;
        volatile bool               stop_flag_;
        std::ofstream*              file_stream_;

        DISALLOW_COPY_AND_ASSIGN(Streamer);
    };

  private:
    std::string                                   file_name_;
    bool                                          is_ready_;
    threads::Thread*                              thread_;
    MessageQueue<protocol_handler::RawMessagePtr> messages_;
};
}  //  namespace media_manager

#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_VIDEO_VIDEO_STREAM_TO_FILE_ADAPTER_H_

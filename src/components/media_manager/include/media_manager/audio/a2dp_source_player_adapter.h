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

#ifndef SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_A2DP_SOURCE_PLAYER_ADAPTER_H_
#define SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_A2DP_SOURCE_PLAYER_ADAPTER_H_

#include <map>
#include "protocol/common.h"
#include "media_manager/media_adapter_impl.h"

namespace threads {
class Thread;
}

namespace media_manager {

class A2DPSourcePlayerAdapter : public MediaAdapterImpl {
  public:
    A2DPSourcePlayerAdapter();
    ~A2DPSourcePlayerAdapter();
    void SendData(int32_t application_key,
                  const ::protocol_handler::RawMessagePtr message) {}
    void StartActivity(int32_t application_key);
    void StopActivity(int32_t application_key);
    bool is_app_performing_activity(int32_t application_key);

  private:
    class A2DPSourcePlayerThread;

    std::map<int32_t, threads::Thread*> sources_;
    DISALLOW_COPY_AND_ASSIGN(A2DPSourcePlayerAdapter);
};

}  // namespace media_manager

#endif  // SRC_COMPONENTS_MEDIA_MANAGER_INCLUDE_MEDIA_MANAGER_AUDIO_A2DP_SOURCE_PLAYER_ADAPTER_H_

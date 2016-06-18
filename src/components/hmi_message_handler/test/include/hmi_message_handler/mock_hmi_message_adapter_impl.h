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

#include "gmock/gmock.h"
#include "hmi_message_handler/hmi_message_adapter_impl.h"
#include "hmi_message_handler/hmi_message_handler.h"

namespace test {
namespace components {
namespace hmi_message_handler_test {

using hmi_message_handler::HMIMessageAdapterImpl;
using hmi_message_handler::HMIMessageHandler;
using hmi_message_handler::MessageSharedPointer;

class MockHMIMessageAdapterImpl : public HMIMessageAdapterImpl {
 public:
  MockHMIMessageAdapterImpl(HMIMessageHandler* handler)
      : HMIMessageAdapterImpl(handler) {}

  MOCK_METHOD0(SubscribeTo, void());
  MOCK_METHOD1(SendMessageToHMI, void(MessageSharedPointer SPtr));

  HMIMessageHandler* get_handler() {
    return HMIMessageAdapterImpl::handler();
  }
};

}  // namespace hmi_message_handler_test
}  // namespace components
}  // namespace test

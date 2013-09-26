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

#ifndef SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_HANDLER_IMPL_H_
#define SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_HANDLER_IMPL_H_

#include <set>
#include "hmi_message_handler/hmi_message_adapter.h"
#include "hmi_message_handler/hmi_message_handler.h"
#include "utils/macro.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"

namespace hmi_message_handler {

typedef utils::SharedPtr<application_manager::Message> MessageSharedPointer;

class ToHMIThreadImpl;
class FromHMIThreadImpl;

class HMIMessageHandlerImpl : public HMIMessageHandler {
 public:
  static HMIMessageHandlerImpl* instance();
  ~HMIMessageHandlerImpl();
  void OnMessageReceived(MessageSharedPointer message);
  void SendMessageToHMI(MessageSharedPointer message);
  void set_message_observer(HMIMessageObserver* observer);
  void OnErrorSending(MessageSharedPointer message);
  void AddHMIMessageAdapter(HMIMessageAdapter* adapter);
  void RemoveHMIMessageAdapter(HMIMessageAdapter* adapter);

 private:
  HMIMessageHandlerImpl();

  HMIMessageObserver* observer_;
  std::set<HMIMessageAdapter*> message_adapters_;

  threads::Thread* to_hmi_thread_;
  friend class ToHMIThreadImpl;

  threads::Thread* from_hmi_thread_;
  friend class FromHMIThreadImpl;

  MessageQueue<MessageSharedPointer> messages_to_hmi_;
  MessageQueue<MessageSharedPointer> messages_from_hmi_;

  static log4cxx::LoggerPtr logger_;

  DISALLOW_COPY_AND_ASSIGN(HMIMessageHandlerImpl);
};
}  // namespace hmi_message_handler

#endif  // SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_HANDLER_IMPL_H_

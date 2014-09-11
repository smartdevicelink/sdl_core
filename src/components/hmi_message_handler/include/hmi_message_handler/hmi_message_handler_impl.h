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
#include "utils/prioritized_queue.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/threads/thread.h"
#include "utils/singleton.h"

namespace hmi_message_handler {


namespace impl {
/*
* These dummy classes are here to locally impose strong typing on different
* kinds of messages
* Currently there is no type difference between incoming and outgoing messages
* TODO(ik): replace these with globally defined message types
* when we have them.
*/
struct MessageFromHmi: public MessageSharedPointer {
  MessageFromHmi(const MessageSharedPointer& message)
      : MessageSharedPointer(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const { return (*this)->Priority().OrderingValue(); }
};

struct MessageToHmi: public MessageSharedPointer {
  MessageToHmi(const MessageSharedPointer& message)
      : MessageSharedPointer(message) {}
  // PrioritizedQueue requres this method to decide which priority to assign
  size_t PriorityOrder() const { return (*this)->Priority().OrderingValue(); }
};

typedef threads::MessageLoopThread<
    utils::PrioritizedQueue<MessageFromHmi> > FromHmiQueue;
typedef threads::MessageLoopThread<
    utils::PrioritizedQueue<MessageToHmi> > ToHmiQueue;
}

class ToHMIThreadImpl;
class FromHMIThreadImpl;

class HMIMessageHandlerImpl
    : public HMIMessageHandler,
      public impl::FromHmiQueue::Handler,
      public impl::ToHmiQueue::Handler,
      public utils::Singleton<HMIMessageHandlerImpl> {
 public:
  ~HMIMessageHandlerImpl();
  void OnMessageReceived(MessageSharedPointer message);
  void SendMessageToHMI(MessageSharedPointer message);
  void set_message_observer(HMIMessageObserver* observer);
  void OnErrorSending(MessageSharedPointer message);
  void AddHMIMessageAdapter(HMIMessageAdapter* adapter);
  void RemoveHMIMessageAdapter(HMIMessageAdapter* adapter);

 private:
  HMIMessageHandlerImpl();


  // threads::MessageLoopThread<*>::Handler implementations

  // CALLED ON messages_from_hmi_ THREAD!
  virtual void Handle(const impl::MessageFromHmi message) OVERRIDE;
  // CALLED ON messages_to_hmi_ THREAD!
  virtual void Handle(const impl::MessageToHmi message) OVERRIDE;
 private:

  HMIMessageObserver* observer_;
  mutable sync_primitives::Lock observer_locker_;
  std::set<HMIMessageAdapter*> message_adapters_;

  // Construct message threads when everything is already created

  // Thread that pumps messages being passed to hmi.
  impl::ToHmiQueue messages_to_hmi_;
  // Thread that pumps messages coming from hmi.
  impl::FromHmiQueue messages_from_hmi_;

  DISALLOW_COPY_AND_ASSIGN(HMIMessageHandlerImpl);
  FRIEND_BASE_SINGLETON_CLASS(HMIMessageHandlerImpl);
};
}  // namespace hmi_message_handler

#endif  // SRC_COMPONENTS_HMI_MESSAGE_HANDLER_INCLUDE_HMI_MESSAGE_HANDLER_HMI_MESSAGE_HANDLER_IMPL_H_

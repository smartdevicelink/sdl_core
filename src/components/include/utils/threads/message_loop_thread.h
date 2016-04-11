/*
 * Copyright (c) 2014, Ford Motor Company
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

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_THREADS_MESSAGE_LOOP_THREAD_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_THREADS_MESSAGE_LOOP_THREAD_H_

#include <string>
#include <queue>

#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"
#include "utils/shared_ptr.h"
#include "utils/lock.h"

namespace threads {

using ::utils::MessageQueue;

/**
 * \class MessageLoopThread
 * \brief Handles a thread which sole purpose is to pump messages pushed
 * to it's queue. To handle messages someone, Handler must be implemented and
 * passed to MessageLoopThread constructor.
 */
template < class Q >
class MessageLoopThread {
 public:
  typedef Q Queue;
  typedef typename Queue::value_type Message;
  /*
   * Handler interface. It is called from a thread that is
   * owned by MessageLoopThread so make sure is only accesses
   * thread-safe data
   */
  struct Handler {
    /*
     * Method called by MessageLoopThread to process single message
     * from it's queue. After calling this method message is discarded.
     */
    // TODO (AKozoriz) : change to const reference (APPLINK-20235)
    virtual void Handle(const Message message) = 0;

    virtual ~Handler() {}
  };

  /*
   * Constructs new MessageLoopThread. Must be named to aid debugging.
   */
  MessageLoopThread(const std::string& name,
                    Handler* handler,
                    const ThreadOptions& thread_opts = ThreadOptions());
  ~MessageLoopThread();

  // Places a message to the therad's queue. Thread-safe.
  void PostMessage(const Message& message);

  // Process already posted messages and stop thread processing. Thread-safe.
  void Shutdown();

  // Added for utils/test/auto_trace_test.cc
  size_t GetMessageQueueSize() const;

  /*
   * Wait until message queue will be empty
   */
  void WaitDumpQueue();

 private:
  /*
   * Implementation of ThreadDelegate that actually pumps the queue and is
   * able to correctly shut it down
   */
  class LoopThreadDelegate : public threads::ThreadDelegate {

   public:
    LoopThreadDelegate(MessageQueue<Message, Queue>* message_queue,
                       Handler* handler);

    // threads::ThreadDelegate overrides
    virtual void threadMain() OVERRIDE;
    virtual void exitThreadMain() OVERRIDE;

   private:
    // Handle all messages that are in the queue until it is empty
    void DrainQue();
    // Handler that processes messages
    Handler& handler_;
    // Message queue that is actually owned by MessageLoopThread
    MessageQueue<Message, Queue>& message_queue_;
  };

 private:
  MessageQueue<Message, Queue> message_queue_;
  LoopThreadDelegate* thread_delegate_;
  threads::Thread* thread_;
};

///////// Implementation

template<class Q>
size_t MessageLoopThread<Q>::GetMessageQueueSize() const {
  return message_queue_.size();
}

template<class Q>
MessageLoopThread<Q>::MessageLoopThread(const std::string&   name,
                                        Handler*             handler,
                                        const ThreadOptions& thread_opts)
    : thread_delegate_(new LoopThreadDelegate(&message_queue_, handler)),
      thread_(threads::CreateThread(name.c_str(),
                                    thread_delegate_)) {
  const bool started = thread_->start(thread_opts);
  if (!started) {
    CREATE_LOGGERPTR_LOCAL(logger_, "Utils")
    LOG4CXX_ERROR(logger_, "Failed to start thread " << name);
  }
}

template<class Q>
MessageLoopThread<Q>::~MessageLoopThread() {
  Shutdown();
  delete thread_delegate_;
  threads::DeleteThread(thread_);
}

template <class Q>
void MessageLoopThread<Q>::PostMessage(const Message& message) {
  message_queue_.push(message);
}

template <class Q>
void MessageLoopThread<Q>::Shutdown() {
  thread_->join();
}

template<class Q>
void MessageLoopThread<Q>::WaitDumpQueue() {
  message_queue_.WaitUntilEmpty();
}

//////////
template<class Q>
MessageLoopThread<Q>::LoopThreadDelegate::LoopThreadDelegate(
    MessageQueue<Message, Queue>* message_queue, Handler* handler)
    : handler_(*handler),
      message_queue_(*message_queue) {
  DCHECK(handler != NULL);
  DCHECK(message_queue != NULL);
}

template<class Q>
void MessageLoopThread<Q>::LoopThreadDelegate::threadMain() {
  CREATE_LOGGERPTR_LOCAL(logger_, "Utils")
  LOG4CXX_AUTO_TRACE(logger_);
  while (!message_queue_.IsShuttingDown()) {
    DrainQue();
    message_queue_.wait();
  }
  // Process leftover messages
  DrainQue();
}

template<class Q>
void MessageLoopThread<Q>::LoopThreadDelegate::exitThreadMain() {
  message_queue_.Shutdown();
}

template<class Q>
void MessageLoopThread<Q>::LoopThreadDelegate::DrainQue() {
  while (!message_queue_.empty()) {
    Message msg;
    if (message_queue_.pop(msg)) {
      handler_.Handle(msg);
    }
  }
}

}  // namespace threads
#endif  // SRC_COMPONENTS_INCLUDE_UTILS_THREADS_MESSAGE_LOOP_THREAD_H_

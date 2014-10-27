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

#ifndef MESSAGE_QUEUE_CLASS
#define MESSAGE_QUEUE_CLASS

#include <queue>

#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/logger.h"
#include "utils/prioritized_queue.h"

/**
 * \class MessageQueue
 * \brief Wrapper for multithreading queue.
 */
 //TODO(Ezamakhov): move to utils namespace
template<typename T, class Q = std::queue<T> > class MessageQueue {
  public:
    typedef Q Queue;
    /**
     * \brief Default constructor
     */
    MessageQueue();

    /**
     * \brief Destructor
     */
    ~MessageQueue();

    /**
     * \brief Returns size of the queue.
     * \return Size of the queue.
     */
    size_t size() const;

    /**
     * \brief If queue is empty.
     * \return Is queue empty.
     */
    bool empty() const;

    /**
     * \brief Tells if queue is being shut down
     */
    bool IsShuttingDown() const;

    /**
     * \brief Adds element to the queue.
     * \param element Element to be added to the queue.n
     */
    void push(const T& element);

    /**
     * \brief Removes element from the queue and returns it.
     * \return To element of the queue.
     */
    T pop();

    /**
     * \brief Conditional wait.
     */
    void wait();

    /**
     * \brief Shutdown the queue.
     * This leads to waking up everyone waiting on the queue
     * Queue being shut down can be drained ( with pop() )
     * But nothing must be added to the queue after it began
     * shutting down
     */
    void Shutdown();

    /**
      * \brief Clears queue.
      */
    void Reset();

  private:

    /**
     *\brief Queue
     */
    Queue queue_;
    volatile bool shutting_down_;

    /**
     *\brief Platform specific syncronisation variable
     */
    mutable sync_primitives::Lock queue_lock_;
    sync_primitives::ConditionalVariable queue_new_items_;
};

template<typename T, class Q> MessageQueue<T, Q>::MessageQueue()
    : shutting_down_(false) {
}

template<typename T, class Q> MessageQueue<T, Q>::~MessageQueue() {
  if (!queue_.empty()) {
    CREATE_LOGGERPTR_LOCAL(logger_, "Utils")
    LOG4CXX_ERROR(logger_, "Destruction of non-drained queue");
  }
}

template<typename T, class Q> void MessageQueue<T, Q>::wait() {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  while ((!shutting_down_) && queue_.empty()) {
    queue_new_items_.Wait(auto_lock);
  }
}

template<typename T, class Q> size_t MessageQueue<T, Q>::size() const {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  return queue_.size();
}

template<typename T, class Q> bool MessageQueue<T, Q>::empty() const {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  return queue_.empty();
}

template<typename T, class Q> bool MessageQueue<T, Q>::IsShuttingDown() const {
  return shutting_down_;
}

template<typename T, class Q> void MessageQueue<T, Q>::push(const T& element) {
  {
    sync_primitives::AutoLock auto_lock(queue_lock_);
    if (shutting_down_) {
      CREATE_LOGGERPTR_LOCAL(logger_, "Utils")
      LOG4CXX_ERROR(logger_, "Runtime error, pushing into queue"
                           " that is being shut down");
    }
    queue_.push(element);
  }
  queue_new_items_.Broadcast();
}

template<typename T, class Q> T MessageQueue<T, Q>::pop() {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  if (queue_.empty()) {
    CREATE_LOGGERPTR_LOCAL(logger_, "Utils")
    LOG4CXX_ERROR(logger_, "Runtime error, popping out of empty queue");
    NOTREACHED();
  }
  T result = queue_.front();
  queue_.pop();
  return result;
}

template<typename T, class Q> void MessageQueue<T, Q>::Shutdown() {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  shutting_down_ = true;
  queue_new_items_.Broadcast();
}

template<typename T, class Q> void MessageQueue<T, Q>::Reset() {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  shutting_down_ = false;
  if (!queue_.empty()) {
    Queue empty_queue;
    queue_.swap(empty_queue);
  }
}

#endif  //  MESSAGE_QUEUE_CLASS

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

#ifndef MESSAGE_QUEUE_CLASS
#define MESSAGE_QUEUE_CLASS

#include <queue>

#include "utils/conditional_variable.h"
#include "utils/lock.h"
#include "utils/logger.h"

/**
 * \class MessageQueue
 * \brief Wrapper for multithreading queue.
 */

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
    int size() const;

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
  private:

    /*
     * Different ways of taking next element from queue for
     * two supported queue types
     */
    static T TakeFromQueue(std::queue<T>& queue);
    static T TakeFromQueue(std::priority_queue<T>& queue);

    /**
     *\brief Queue
     */
    Queue queue_;
    volatile bool shutting_down_;
    /**
     *\brief Platform specific syncronisation variable
     */
    mutable sync_primitives::Lock queue_lock_;
    mutable sync_primitives::ConditionalVariable queue_new_items_;
};

template<typename T, class Q> MessageQueue<T, Q>::MessageQueue()
    : shutting_down_(false) {
}

template<typename T, class Q> MessageQueue<T, Q>::~MessageQueue() {
  if (!queue_.empty()) {
    log4cxx::LoggerPtr logger =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Utils"));
    LOG4CXX_ERROR(logger, "Destruction of non-drained queue");
  }
}

template<typename T, class Q> void MessageQueue<T, Q>::wait() {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  while (!shutting_down_ && queue_.empty()) {
    queue_new_items_.Wait(auto_lock);
  }
}

template<typename T, class Q> int MessageQueue<T, Q>::size() const {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  return queue_.size();
}

template<typename T, class Q> bool MessageQueue<T, Q>::empty() const {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  return queue_.empty();
}

template<typename T, class Q> bool MessageQueue<T, Q>::IsShuttingDown() const {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  return shutting_down_;
}

template<typename T, class Q> void MessageQueue<T, Q>::push(const T& element) {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  if (shutting_down_) {
    log4cxx::LoggerPtr logger =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Utils"));
    LOG4CXX_ERROR(logger, "Runtime error, pushing into queue"
                         " that is being shut down");
  }
  queue_.push(element);
  queue_new_items_.Broadcast();
}

template<typename T, class Q> T MessageQueue<T, Q>::pop() {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  if (queue_.empty()) {
    log4cxx::LoggerPtr logger =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Utils"));
    LOG4CXX_ERROR(logger, "Runtime error, popping out of empty que");
  }
  return TakeFromQueue(queue_);
}

template<typename T, class Q> void MessageQueue<T, Q>::Shutdown() {
  sync_primitives::AutoLock auto_lock(queue_lock_);
  shutting_down_ = true;
  queue_new_items_.Broadcast();
}


template<typename T, class Q>
T MessageQueue<T, Q>::TakeFromQueue(std::queue<T>& queue) {
  T result = queue.front();
  queue.pop();
  return result;
}

template<typename T, class Q>
T MessageQueue<T, Q>::TakeFromQueue(std::priority_queue<T>& queue) {
  T result = queue.top();
  queue.pop();
  return result;
}

#endif  //  MESSAGE_QUEUE_CLASS

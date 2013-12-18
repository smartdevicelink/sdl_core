#ifndef SRC_COMPONENTS_UTILS_INCLUDE_MESSAGE_LOOP_THREAD_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_MESSAGE_LOOP_THREAD_H_

#include <string>
#include <queue>

#include "utils/logger.h"
#include "utils/macro.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"

namespace threads {

/*
 * Class that handles a thread which sole purpose is to pump messages pushed
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
    virtual void Handle(const Message& message) = 0;
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
 private:

  /*
   * Implementation of ThreadDelegate that actually pumps the queue and is
   * able to correctly shut it down
   */
  struct LoopThreadDelegate : public threads::ThreadDelegate {
    LoopThreadDelegate(MessageQueue<Message, Queue>* message_queue,
                       Handler* handler);

    // threads::ThreadDelegate overrides
    virtual void threadMain() OVERRIDE;
    virtual bool exitThreadMain() OVERRIDE;
   private:
    // Handle all messages that are in the queue until it is empty
    void DrainQue();
   private:
    // Handler that processes messages
    Handler& handler_;
    // Message queue that is actually owned by MessageLoopThread
    MessageQueue<Message, Queue>& message_queue_;
  };
 private:
  MessageQueue<Message, Queue> message_queue_;
  threads::Thread thread_;
};

///////// Implementation

template<class Q>
MessageLoopThread<Q>::MessageLoopThread(const std::string& name,
                                              Handler* handler,
                                              const ThreadOptions& thread_opts)
    : thread_(name.c_str(), new LoopThreadDelegate(&message_queue_, handler)) {
  bool started = thread_.startWithOptions(thread_opts);
  if (!started) {
    log4cxx::LoggerPtr logger =
        log4cxx::LoggerPtr(log4cxx::Logger::getLogger("Utils"));
    LOG4CXX_ERROR(logger, "Failed to start thread " << name);
  }
}

template<class Q>
MessageLoopThread<Q>::~MessageLoopThread() {
  // this will join us with the thread while it drains message queue
  thread_.stop();
}

template <class Q>
void MessageLoopThread<Q>::PostMessage(const Message& message) {
  message_queue_.push(message);
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
  while(!message_queue_.IsShuttingDown()){
    DrainQue();
    message_queue_.wait();
  }
  // Process leftover messages
  DrainQue();
}

template<class Q>
bool MessageLoopThread<Q>::LoopThreadDelegate::exitThreadMain() {
  message_queue_.Shutdown();
  // Prevent canceling thread until queue is drained
  return true;
}

template<class Q>
void MessageLoopThread<Q>::LoopThreadDelegate::DrainQue() {
  while(!message_queue_.empty()) {
    handler_.Handle(message_queue_.pop());
  }
}

} // namespace utils

#endif // SRC_COMPONENTS_UTILS_INCLUDE_MESSAGE_LOOP_THREAD_H_

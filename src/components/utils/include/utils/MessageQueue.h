/**
 * \file MessageQueue.hpp
 * \brief Template MessageQueue class header.
 */

#ifndef MESSAGE_QUEUE_CLASS
#define MESSAGE_QUEUE_CLASS

#include <queue>
#include "utils/synchronisation_primitives.h"

/**
 * \class MessageQueue
 * \brief Wrapper for multithreading queue.
 */
template<typename T> class MessageQueue {
 public:
  /**
   * \brief Default constructor
   */
  MessageQueue();

  /**
   * \brief Constructor
   * \param queue Existing queue.
   */
  explicit MessageQueue(std::queue<T> queue);

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
   * \brief Adds element to the queue.
   * \param element Element to be added to the queue.n
   */
  void push(const T & element);

  /**
   * \brief Removes element from the queue and returns it.
   * \return To element of the queue.
   */
  T pop();

  /**
   * \brief Conditional wait.
   */
  void wait();

 private:
  /**
   *\brief Queue
   */
  std::queue<T> mQueue;
  /**
   *\brief Platform specific syncronisation variable
   */
  mutable threads::SynchronisationPrimitives sync_variable_;
  /**
   *\brief Bool condition for waiting.
   */
  bool mIsUp;
};

template<typename T> MessageQueue<T>::MessageQueue()
    : mIsUp(false) {
}

template<typename T> MessageQueue<T>::MessageQueue(std::queue<T> queue) {
  sync_variable_.lock();
  mQueue = std::queue<T>(queue);
  sync_variable_.unlock();
}

template<typename T> MessageQueue<T>::~MessageQueue() {
}

template<typename T> void MessageQueue<T>::wait() {
  sync_variable_.lock();
  while (!mIsUp) {
    sync_variable_.wait();
  }
  mIsUp = false;
  sync_variable_.unlock();
}

template<typename T> int MessageQueue<T>::size() const {
  int result = 0;
  sync_variable_.lock();
  result = mQueue.size();
  sync_variable_.unlock();
  return result;
}

template<typename T> bool MessageQueue<T>::empty() const {
  bool result = true;
  sync_variable_.lock();
  result = mQueue.empty();
  sync_variable_.unlock();
  return result;
}

template<typename T> void MessageQueue<T>::push(const T & element) {
  sync_variable_.lock();
  mQueue.push(element);

  sync_variable_.signal();
  mIsUp = true;

  sync_variable_.unlock();
}

template<typename T> T MessageQueue<T>::pop() {
  sync_variable_.lock();
  if (mQueue.empty()) {
    //error, TRACE
  }

  T result = mQueue.front();
  mQueue.pop();

  sync_variable_.unlock();
  return result;
}

#endif  //  MESSAGE_QUEUE_CLASS

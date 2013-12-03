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
#ifndef SRC_COMPONENTS_UTILS_INCLUDE_THREADS_THREAD_VALIDATOR_H_
#define SRC_COMPONENTS_UTILS_INCLUDE_THREADS_THREAD_VALIDATOR_H_

#include "utils/threads/thread.h"

namespace threads {
/*
 * Use objects of this class to validate (in run-time) access
 * to single-threaded objects. It remembers the thread it was
 * created on and then allows to easily check whether an object is
 * being accessed from different thread and log an error in that case
 *
 * Put a member of type SingleThreadValidator in your class and call
 * AssertRunningOnCreationThread in it's every public method
 *
 * class MySingleThreadedThing {
 * .....
 * public:
 *   void DoSomeWork();
 * .....
 * private:
 *   SingleThreadValidator thread_validator_;
 * .....
 * };
 *
 * void MySingleThreadedThing::DoSomeWork() {
 *   thread_validator_.AssertRunningOnCreationThread();
 *   .....
 * }
 */
class SingleThreadSimpleValidator {
 public:
  SingleThreadSimpleValidator();
  ~SingleThreadSimpleValidator();

  // This method should be called in every public method
  // of classes being checked for absence of concurrent access
  void AssertRunningOnCreationThread() const;
 private:
  const Thread::Id creation_thread_id_;
};


/*
 * This is bit more sophisticated debug helper which allows
 * objects being checked to be transferred between threads.
 * Make sure to pass ownership calling PassToThread before
 * object is accessed from that thread!
 *
 * It's better to virtually inherit it to make PassToThread publicly
 * available to code that uses objects of your classes and make sure
 * that if your object consists of different thread-validated
 * parts you'll need not to call PassToThread proxy method for
 * every part of your composite object
 */
class SingleThreadValidator {
 public:
  SingleThreadValidator();
  ~SingleThreadValidator();

  // Must be called prior to transferring object being validated to
  // another thread or when passing it back
  void PassToThread(Thread::Id thread_id) const;
  // This method should be called in every public method
  // of classes being checked for absence of unintended concurrent
  // access
  void AssertRunningOnValidThread() const;
 private:
  mutable Thread::Id owning_thread_id_;
};

} // namespace threads

#endif // SRC_COMPONENTS_UTILS_INCLUDE_THREADS_THREAD_VALIDATOR_H_

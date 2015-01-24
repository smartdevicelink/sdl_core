/*
 Copyright (c) 2014, Ford Motor Company
 All rights reserved.

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:

 Redistributions of source code must retain the above copyright notice, this
 list of conditions and the following disclaimer.

 Redistributions in binary form must reproduce the above copyright notice,
 this list of conditions and the following
 disclaimer in the documentation and/or other materials provided with the
 distribution.

 Neither the name of the Ford Motor Company nor the names of its contributors
 may be used to endorse or promote products derived from this software
 without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
 */

#ifndef SRC_COMPONENTS_INCLUDE_UTILS_ASYNC_RUNNER_H_
#define SRC_COMPONENTS_INCLUDE_UTILS_ASYNC_RUNNER_H_

#include <string>
#include <queue>

#include "thread_delegate.h"
#include "thread.h"

#include "utils/lock.h"
#include "utils/conditional_variable.h"

namespace threads {

/**
 * @brief The AsyncRunner class allows to run passed delegate asynchronously
 * The class contains thread which will handle passed delegates and put it
 * into special queue. In case this queue is not empty AsyncRunner will
 * create separate thread for delegate processing. So actualy this AsyncRunner
 * is kind of manager for async functions.
 */
class AsyncRunner {
  public:
    /**
     * @brief AsyncRunner constructor, allows to create and run new thread.
     * The thread will be removed in destructor and appropriate delegate will
     * be removed some time latter after pthred_join.
     *
     * @param thread_name thread's name.
     *
     * @param delegate delegate to run within thread.
     */
    explicit AsyncRunner(const std::string& thread_name);

    /**
     * @brief AsyncRun pass obtained delegate into internal queue
     *
     * @param delegate the objet which has to be concuremtly run
     */
    void AsyncRun(threads::ThreadDelegate* delegate);
    /**
     * @brief Stop delegates activity
     */
    void Stop();

    ~AsyncRunner();

  private:

    class AsyncRunnerDelegate: public threads::ThreadDelegate {
      public:
        AsyncRunnerDelegate();

        /**
         * @brief threadMain runs delegates queue handling.
         */
        virtual void threadMain();

        /**
         * @brief exitThreadMain doing stuff before exit from thread.
         */
        virtual void exitThreadMain();

        /**
         * @brief runDelegate push obtained delegate into specific queue
         *
         * @param delegate object that has to be executed.
         */
        void runDelegate(threads::ThreadDelegate* delegate);

      private:
        /**
         * @brief processDelegate allows to pop delegate
         * from queue and process it.
         */
        void processDelegate();

        /**
         * @brief waitForDelegate wait while delegates queue is empty.
         */
        void waitForDelegate();

        std::queue<threads::ThreadDelegate*> delegates_queue_;
        sync_primitives::ConditionalVariable delegate_notifier_;
        sync_primitives::Lock delegates_queue_lock_;
        volatile bool stop_flag_;
    };

    threads::Thread* thread_;
    AsyncRunnerDelegate* executor_;
};

} // namespace threads

#endif // SRC_COMPONENTS_INCLUDE_UTILS_ASYNC_RUNNER_H_

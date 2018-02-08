/*
 * Copyright (c) 2018, Ford Motor Company
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

#ifndef SRC_APPMAIN_MQ_SIGNALS_HANDLER_H_
#define SRC_APPMAIN_MQ_SIGNALS_HANDLER_H_

#include <fcntl.h>    /* For O_* constants */
#include <sys/stat.h> /* For mode constants */
#include <semaphore.h>
#include <mqueue.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include <string>
#include "utils/threads/thread_delegate.h"
#include "application_manager/application_manager_impl.h"
#include "utils/macro.h"
#define MAX_QUEUE_NAME_SIZE 48
#define MAX_QUEUE_MSG_SIZE 4096
#define MSGQ_MAX_MESSAGES 10
#define MSGQ_MESSAGE_SIZE MAX_QUEUE_MSG_SIZE

namespace main_namespace {

class LifeCycle;

class MQSignalsHandler {
 public:
  /**
   * @brief Constructor
   */
  MQSignalsHandler(LifeCycle& life_cycle, const std::string& mq_name);
  /**
   * @brief Creates all parts of MQSignalHandler
   */
  bool Init();
  void Process(const std::string& msg);
  bool IsActive() const;
  ~MQSignalsHandler();

 private:
  void Destroy();

  class MQueue {
   public:
    explicit MQueue(const std::string& mq_name);
    ~MQueue();
    bool Init();
    std::string Receive();
    void FinishActivity();

   private:
    mqd_t mq_;
    const std::string& sdl_mq_name_;
  };

  class NotificationThreadDelegate : public threads::ThreadDelegate {
   public:
    NotificationThreadDelegate(MQSignalsHandler& mq_signals_handler,
                               MQueue& mqueue)
        : mq_signals_handler_(mq_signals_handler), mqueue_(mqueue) {}

    ~NotificationThreadDelegate() {}

    void threadMain() OVERRIDE;
    void exitThreadMain() OVERRIDE;

   private:
    MQSignalsHandler& mq_signals_handler_;
    MQueue mqueue_;
  };

  enum State { kRun, kSleep, kStop };
  State state_;
  NotificationThreadDelegate* notifications_delegate_;
  LifeCycle& life_cycle_;
  threads::Thread* mq_handler_thread_;
  MQueue mqueue_;
};

}  // namespace main_namespace

#endif  // SRC_APPMAIN_MQ_SIGNALS_HANDLER_H_

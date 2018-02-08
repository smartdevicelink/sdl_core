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

#include "mq_signals_handler.h"
#include "life_cycle.h"
#include "utils/logger.h"
#include "config_profile/profile.h"

namespace main_namespace {

namespace {
const std::string kSdlLowVoltage = "SDL_LOW_VOLTAGE";
const std::string kSdlWakeUp = "WAKE_UP";
const std::string kSdlShutDown = "SHUT_DOWN";
}  // namespace

CREATE_LOGGERPTR_GLOBAL(logger_, "MQHandler")

MQSignalsHandler::MQSignalsHandler(LifeCycle& life_cycle,
                                   const std::string& mq_name)
    : state_(kRun)
    , notifications_delegate_(NULL)
    , life_cycle_(life_cycle)
    , mq_handler_thread_(NULL)
    , mqueue_(mq_name) {}

bool MQSignalsHandler::Init() {
  if (!mqueue_.Init()) {
    LOG4CXX_FATAL(logger_, "Stopping SDL...");
    return false;
  }
  notifications_delegate_ = new NotificationThreadDelegate(*this, mqueue_);
  mq_handler_thread_ =
      threads::CreateThread("MQ_HANDLER_THREAD", notifications_delegate_);
  mq_handler_thread_->start();
  return true;
}

void MQSignalsHandler::Destroy() {
  state_ = kStop;
  mq_handler_thread_->join();
  delete notifications_delegate_;
  threads::DeleteThread(mq_handler_thread_);
}

MQSignalsHandler::~MQSignalsHandler() {
  Destroy();
}

void MQSignalsHandler::Process(const std::string& msg) {
  if (msg.empty()) {
    return;
  }

  LOG4CXX_DEBUG(logger_, "Received MQ Signal: " << msg);
  LOG4CXX_DEBUG(logger_, "Current state is : " << state_);

  switch (state_) {
    case kRun:
      if (kSdlLowVoltage == msg) {
        LOG4CXX_DEBUG(logger_, "Received LOW VOLTAGE signal");
        life_cycle_.LowVoltage();
        state_ = kSleep;
      } else if (kSdlShutDown == msg) {
        LOG4CXX_DEBUG(logger_, "Received SDL SHUT DOWN signal");
        state_ = kStop;
        kill(getpid(), SIGINT);
      } else if (kSdlWakeUp == msg) {
        // Do nothing
      } else {
        // SDL resumes its regular work after receiving "WAKE_UP"
      }
      break;
    case kSleep:
      if (kSdlWakeUp == msg) {
        LOG4CXX_DEBUG(logger_, "Received SDL WAKE UP signal");
        life_cycle_.WakeUp();
        state_ = kRun;
      } else if (kSdlShutDown == msg) {
        LOG4CXX_DEBUG(logger_, "Received SDL SHUT DOWN signal");
        state_ = kStop;
        kill(getpid(), SIGINT);
      }
      break;
    case kStop: /* nothing do here */
      break;
  }
}

bool MQSignalsHandler::IsActive() const {
  return state_ != kStop;
}

MQSignalsHandler::MQueue::MQueue(const std::string& mq_name)
    : mq_(-1), sdl_mq_name_(mq_name) {}

bool MQSignalsHandler::MQueue::Init() {
  using namespace ::profile;
  LOG4CXX_AUTO_TRACE(logger_);
  struct mq_attr attributes;
  attributes.mq_maxmsg = MSGQ_MAX_MESSAGES;
  attributes.mq_msgsize = MAX_QUEUE_MSG_SIZE;

  mq_ = mq_open(
      sdl_mq_name_.c_str(), O_RDWR | O_CREAT, S_IRUSR | S_IWUSR, &attributes);
  if (-1 == mq_) {
    LOG4CXX_DEBUG(logger_, "MQUEUE NAME :" << sdl_mq_name_);
    LOG4CXX_FATAL(logger_,
                  "Error opening mqueue! Error code : " << strerror(errno));
    return false;
  }
  return true;
}

MQSignalsHandler::MQueue::~MQueue() {
  LOG4CXX_AUTO_TRACE(logger_);
  FinishActivity();
}

void MQSignalsHandler::MQueue::FinishActivity() {
  LOG4CXX_AUTO_TRACE(logger_);
  // 31 is the highest message priority in mqueue
  const unsigned int msg_priority = 31u;
  mq_send(mq_, kSdlShutDown.c_str(), sizeof(kSdlShutDown), msg_priority);
  mq_close(mq_);
  mq_unlink(sdl_mq_name_.c_str());
}

std::string MQSignalsHandler::MQueue::Receive() {
  LOG4CXX_AUTO_TRACE(logger_);
  char mq_buffer[MAX_QUEUE_MSG_SIZE] = {0};
  ssize_t length = mq_receive(mq_, mq_buffer, sizeof(mq_buffer), 0);
  if (length == -1) {
    LOG4CXX_DEBUG(logger_,
                  "Error receiving message! Error code : " << strerror(errno));
    return "";
  }

  LOG4CXX_DEBUG(logger_, "MQUEUE CONTENTS :" << mq_buffer);
  return std::string(mq_buffer);
}

void MQSignalsHandler::NotificationThreadDelegate::threadMain() {
  while (mq_signals_handler_.IsActive()) {
    const std::string msg = mqueue_.Receive();
    mq_signals_handler_.Process(msg);
  }
}

void MQSignalsHandler::NotificationThreadDelegate::exitThreadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  mqueue_.FinishActivity();
}

}  // namespace main_namespace

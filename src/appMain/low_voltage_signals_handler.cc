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

#include "appMain/low_voltage_signals_handler.h"

#include <signal.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <iostream>
#include "appMain/life_cycle.h"
#include "utils/signals.h"
#include "utils/logger.h"
#include "utils/typed_enum_print.h"
#include "config_profile/profile.h"

namespace main_namespace {

CREATE_LOGGERPTR_GLOBAL(logger_, "LowVoltageSignalsHandler")

LowVoltageSignalsHandler::LowVoltageSignalsHandler(
    LifeCycle& life_cycle, const LowVoltageSignalsOffset& offset_data)
    : notifications_delegate_(new NotificationThreadDelegate(*this))
    , signals_handler_thread_(threads::CreateThread(
          "LV_SIGNALS_HANDLER_THREAD", notifications_delegate_.get()))
    , life_cycle_(life_cycle)
    , SIGLOWVOLTAGE_(offset_data.low_voltage_signal_offset + SIGRTMIN)
    , SIGWAKEUP_(offset_data.wake_up_signal_offset + SIGRTMIN)
    , SIGIGNOFF_(offset_data.ignition_off_signal_offset + SIGRTMIN)
    , cpid_(-1) {
  sigemptyset(&lv_mask_);
  sigaddset(&lv_mask_, SIGLOWVOLTAGE_);
  signals_handler_thread_->start();
}

sigset_t LowVoltageSignalsHandler::LowVoltageSignalsMask() const {
  return lv_mask_;
}

int LowVoltageSignalsHandler::low_voltage_signo() const {
  return SIGLOWVOLTAGE_;
}

int LowVoltageSignalsHandler::wake_up_signo() const {
  return SIGWAKEUP_;
}

int LowVoltageSignalsHandler::ignition_off_signo() const {
  return SIGIGNOFF_;
}

void LowVoltageSignalsHandler::Destroy() {
  if (signals_handler_thread_) {
    signals_handler_thread_->join();
  }
  notifications_delegate_.reset();
  threads::DeleteThread(signals_handler_thread_);
}

LowVoltageSignalsHandler::~LowVoltageSignalsHandler() {
  Destroy();
}

void LowVoltageSignalsHandler::HandleSignal(const int signo) {
  if (SIGLOWVOLTAGE_ == signo) {
    LOG4CXX_DEBUG(logger_, "Received LOW_VOLTAGE signal");

    life_cycle_.LowVoltage();
    cpid_ = utils::Signals::Fork();

    if (0 > cpid_) {
      LOG4CXX_FATAL(logger_,
                    "Error due fork() call. Error: " << strerror(errno));
      utils::Signals::ExitProcess(EXIT_FAILURE);
    }

    if (0 != cpid_) {
      // In Parent process
      LOG4CXX_DEBUG(logger_, "Child PID: " << cpid_);
      utils::Signals::WaitPid(cpid_, nullptr, 0);
      LOG4CXX_DEBUG(logger_, "Child process: " << cpid_ << " is stopped");
      life_cycle_.WakeUp();
    } else {
      // In Child process
      sigset_t signal_set;
      sigfillset(&signal_set);
      pthread_sigmask(SIG_BLOCK, &signal_set, nullptr);
      sigemptyset(&lv_mask_);
      sigaddset(&lv_mask_, SIGWAKEUP_);
      sigaddset(&lv_mask_, SIGIGNOFF_);
      std::cout << "Stopping parent process: " << getppid() << std::endl;
      utils::Signals::SendSignal(SIGSTOP, getppid());
      std::cout << "SIGSTOP signal sent to " << getppid() << std::endl;
    }
    return;
  }

  if (SIGWAKEUP_ == signo) {
    std::cout << "Received WAKE UP signal" << std::endl;
    std::cout << "Waking Up parent process: " << getppid() << std::endl;
    utils::Signals::SendSignal(SIGCONT, getppid());
    std::cout << "Stopping child process: " << getpid() << std::endl;
    utils::Signals::ExitProcess(0);
  }

  if (SIGIGNOFF_ == signo) {
    std::cout << "Received IGNITION_OFF signal" << std::endl;
    std::cout << "Stopping all SDL processes..." << std::endl;
    utils::Signals::SendSignal(SIGKILL, getppid());
    utils::Signals::ExitProcess(0);
  }
}

void NotificationThreadDelegate::threadMain() {
  while (true) {
    int signo = 0;
    const sigset_t lv_mask =
        low_voltage_signals_handler_.LowVoltageSignalsMask();
    const int err = sigwait(&lv_mask, &signo);
    if (0 != err) {
      LOG4CXX_ERROR(
          logger_,
          "Sigwait() error! Signals set contains an invalid signal number!");
      continue;
    }
    low_voltage_signals_handler_.HandleSignal(signo);
  }
}

void NotificationThreadDelegate::exitThreadMain() {
  LOG4CXX_AUTO_TRACE(logger_);
  ThreadDelegate::exitThreadMain();
}

}  // namespace main_namespace

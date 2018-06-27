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
#include "appMain/life_cycle.h"
#include "utils/logger.h"
#include "utils/typed_enum_print.h"
#include "config_profile/profile.h"

namespace main_namespace {

CREATE_LOGGERPTR_GLOBAL(logger_, "LowVoltageSignalsHandler")

LowVoltageSignalsHandler::LowVoltageSignalsHandler(
    LifeCycle& life_cycle, const LowVoltageSignalsOffset& offset_data)
    : state_(SDLState::kRun)
    , notifications_delegate_(new NotificationThreadDelegate(*this))
    , signals_handler_thread_(threads::CreateThread(
          "LV_SIGNALS_HANDLER_THREAD", notifications_delegate_.get()))
    , life_cycle_(life_cycle)
    , SIGLOWVOLTAGE_(offset_data.low_voltage_signal_offset + SIGRTMIN)
    , SIGWAKEUP_(offset_data.wake_up_signal_offset + SIGRTMIN)
    , SIGIGNOFF_(offset_data.ignition_off_signal_offset + SIGRTMIN) {
  signals_handler_thread_->start();
}

SDLState LowVoltageSignalsHandler::get_current_sdl_state() const {
  return state_;
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
  state_ = SDLState::kStop;
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
  using utils::operator<<;
  LOG4CXX_DEBUG(logger_, "Received Signal: " << signo);
  LOG4CXX_DEBUG(logger_, "Current state is : " << get_current_sdl_state());

  auto handle_run_state = [this, signo]() {

    if (SIGLOWVOLTAGE_ == signo) {
      LOG4CXX_DEBUG(logger_, "Received LOW_VOLTAGE signal");
      state_ = SDLState::kSleep;
      life_cycle_.LowVoltage();
      return;
    }

    if (SIGIGNOFF_ == signo) {
      LOG4CXX_DEBUG(logger_,
                    "Received IGNITION_OFF signal. But SDL is in active state");
      // Do nothing
      return;
    }

    if (SIGWAKEUP_ == signo) {
      LOG4CXX_DEBUG(logger_,
                    "Received WAKE_UP signal. But SDL is in active state");
      // Do nothing
      return;
    }
    LOG4CXX_DEBUG(logger_, "Received UNKNOWN signal");
  };

  auto handle_sleep_state = [this, signo]() {

    if (SIGWAKEUP_ == signo) {
      LOG4CXX_DEBUG(logger_, "Received WAKE UP signal");
      state_ = SDLState::kRun;
      life_cycle_.WakeUp();
      return;
    }

    if (SIGIGNOFF_ == signo) {
      LOG4CXX_DEBUG(logger_, "Received IGNITION_OFF signal");
      state_ = SDLState::kStop;
      life_cycle_.IgnitionOff();
      return;
    }

    if (SIGLOWVOLTAGE_ == signo) {
      LOG4CXX_DEBUG(
          logger_,
          "Received LOW VOLTAGE signal. But SDL is already in sleep state");
      // Do nothing
      return;
    }
    LOG4CXX_DEBUG(logger_, "Received UNKNOWN signal");
  };

  switch (state_) {
    case SDLState::kRun:
      handle_run_state();
      break;
    case SDLState::kSleep:
      handle_sleep_state();
      break;
    case SDLState::kStop: /* nothing to do here */
      LOG4CXX_DEBUG(logger_, "SDL is in stopping state");
      break;
  }
}

bool LowVoltageSignalsHandler::IsActive() const {
  return SDLState::kStop != state_;
}

void NotificationThreadDelegate::threadMain() {
  sigset_t lv_mask;
  sigemptyset(&lv_mask);
  sigaddset(&lv_mask, low_voltage_signals_handler_.low_voltage_signo());
  sigaddset(&lv_mask, low_voltage_signals_handler_.wake_up_signo());
  sigaddset(&lv_mask, low_voltage_signals_handler_.ignition_off_signo());

  while (low_voltage_signals_handler_.IsActive()) {
    int signo = 0;
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

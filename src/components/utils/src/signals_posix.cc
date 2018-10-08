/*
 * Copyright (c) 2016, Ford Motor Company
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
#include <sys/wait.h>
#include <csignal>
#include <cstdlib>
#include <stdint.h>
#include <iostream>

#include "utils/signals.h"

namespace utils {
bool Signals::UnsubscribeFromTermination() {
  // Disable some system signals receiving in thread
  // by blocking those signals
  // (system signals processes only in the main thread)
  // Mustn't block all signals!
  // See "Advanced Programming in the UNIX Environment, 3rd Edition"
  // (http://poincare.matf.bg.ac.rs/~ivana//courses/ps/sistemi_knjige/pomocno/apue.pdf,
  // "12.8. Threads and Signals".
  sigset_t signal_set;
  sigemptyset(&signal_set);
  sigaddset(&signal_set, SIGINT);
  sigaddset(&signal_set, SIGTERM);

  return !pthread_sigmask(SIG_BLOCK, &signal_set, NULL);
}

bool Signals::UnsubscribeFromLowVoltageSignals(
    const main_namespace::LowVoltageSignalsOffset& offset_data) {
  // Disable Low Voltage signals in main thread
  // due to all further threads will inherit signals mask
  sigset_t signal_set;
  sigemptyset(&signal_set);
  const int SIGLOWVOLTAGE = SIGRTMIN + offset_data.low_voltage_signal_offset;
  const int SIGWAKEUP = SIGRTMIN + offset_data.wake_up_signal_offset;
  const int SIGIGNOFF = SIGRTMIN + offset_data.ignition_off_signal_offset;
  sigaddset(&signal_set, SIGLOWVOLTAGE);
  sigaddset(&signal_set, SIGWAKEUP);
  sigaddset(&signal_set, SIGIGNOFF);

  // Set signals mask to be blocked by thread
  return !pthread_sigmask(SIG_BLOCK, &signal_set, nullptr);
}

void Signals::SendSignal(const int signo, const pid_t pid) {
  if (kill(pid, signo) == -1) {
    std::cerr << "Error sending signal: " << strsignal(signo) << " to " << pid
              << " .Error: " << strerror(errno) << std::endl;
  }
}

pid_t Signals::Fork() {
  return fork();
}

void Signals::ExitProcess(const int status) {
  exit(status);
}

void Signals::WaitPid(pid_t cpid, int* status, int options) {
  waitpid(cpid, status, options);
}

namespace {
bool CatchSIGSEGV(sighandler_t handler) {
  struct sigaction act;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  return !sigaction(SIGSEGV, &act, NULL);
}
}  // namespace

bool Signals::WaitTerminationSignals(sighandler_t sig_handler) {
  sigset_t signal_set;
  int sig = -1;

  sigemptyset(&signal_set);
  sigaddset(&signal_set, SIGINT);
  sigaddset(&signal_set, SIGTERM);

  if (!CatchSIGSEGV(sig_handler)) {
    return false;
  }

  if (!sigwait(&signal_set, &sig)) {
    sig_handler(sig);
    return true;
  }
  return false;
}
}  // namespace utils

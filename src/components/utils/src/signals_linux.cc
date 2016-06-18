/*
 * Copyright (c) 2014, Ford Motor Company
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
#include <csignal>
#include <cstdlib>
#include <stdint.h>
#include <unistd.h>

#include "utils/signals.h"
#include "utils/logger.h"

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

namespace {

void SigHandler(int sig) {
  switch (sig) {
    case SIGINT:
      LOGGER_INFO(logger_, "SIGINT signal has been caught");
      break;
    case SIGTERM:
      LOGGER_INFO(logger_, "SIGTERM signal has been caught");
      break;
    case SIGSEGV:
      LOGGER_INFO(logger_, "SIGSEGV signal has been caught");
      break;
    default:
      LOGGER_INFO(logger_, "Unexpected signal has been caught");
      break;
  }
}

bool CatchSIGSEGV(sighandler_t handler) {
  struct sigaction act;

  act.sa_handler = handler;
  sigemptyset(&act.sa_mask);
  act.sa_flags = 0;

  return !sigaction(SIGSEGV, &act, NULL);
}

}  //  namespace

namespace utils {

void WaitForSdlExecute() {
  sigset_t signal_set;
  int sig = -1;

  sigemptyset(&signal_set);
  sigaddset(&signal_set, SIGINT);
  sigaddset(&signal_set, SIGTERM);

  if (!CatchSIGSEGV(&SigHandler) || 0 != sigwait(&signal_set, &sig)) {
    LOGGER_FATAL(logger_, "Subscribe to system signals error");
  }

  SigHandler(sig);
}

void CreateSdlEvent() {}

void SubscribeToTerminationSignals() {}

}  //  namespace utils

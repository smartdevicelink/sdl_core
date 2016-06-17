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
#include "utils/winhdr.h"
#include "utils/signals.h"
#include "utils/logger.h"

SDL_CREATE_LOGGER("Utils")

namespace {
HANDLE signal_handle = NULL;

void HandleSignals(HANDLE& signal_handle, const char* log_name) {
  SDL_INFO(log_name);
  SetEvent(signal_handle);
}

void SigHandler(int sig) {
  switch (sig) {
    case SIGINT:
      HandleSignals(signal_handle, "SIGINT signal has been caught");
      break;
    case SIGTERM:
      HandleSignals(signal_handle, "SIGTERM signal has been caught");
      break;
    case SIGSEGV:
      HandleSignals(signal_handle, "SIGSEGV signal has been caught");
      break;
    default:
      HandleSignals(signal_handle, "Unexpected signal has been caught");
      break;
  }
}
}  //  namespace

namespace utils {

void WaitForSdlExecute() {
  if (signal_handle) {
    WaitForSingleObject(signal_handle, INFINITE);
  } else {
    SDL_FATAL("SDL is not subscribed to signal events");
  }
}

void CreateSdlEvent() {
  signal_handle = CreateEvent(NULL, true, false, "SignalEvent");
}

void SubscribeToTerminationSignals() {
  if ((signal(SIGINT, &SigHandler) == SIG_ERR) ||
      (signal(SIGTERM, &SigHandler) == SIG_ERR) ||
      (signal(SIGSEGV, &SigHandler) == SIG_ERR)) {
    SDL_FATAL("Subscribe to system signals error");
  }
}
}  //  namespace utils

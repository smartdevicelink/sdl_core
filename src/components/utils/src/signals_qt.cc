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
#include "utils/signals.h"
#include "utils/logger.h"
#include <QCoreApplication>

CREATE_LOGGERPTR_GLOBAL(logger_, "Utils")

namespace {

void HandleSignals(const char* log_name) {
  LOGGER_INFO(logger_, log_name);
  QCoreApplication* const app = QCoreApplication::instance();
  if (!app) {
    LOGGER_FATAL(logger_, "No QCoreApplication instance already");
  }
  app->quit();
}

void SigHandler(int sig) {
  switch (sig) {
    case SIGINT:
      HandleSignals("SIGINT signal has been caught");
      break;
    case SIGTERM:
      HandleSignals("SIGTERM signal has been caught");
      break;
    case SIGSEGV:
      HandleSignals("SIGSEGV signal has been caught");
      break;
    default:
      HandleSignals("Unexpected signal has been caught");
      break;
  }
}
}  //  namespace

namespace utils {

void WaitForSdlExecute() {
  QCoreApplication::instance()->processEvents();
  QCoreApplication::instance()->exec();
}

void CreateSdlEvent() {}

void SubscribeToTerminationSignals() {
  if ((signal(SIGINT, &SigHandler) == SIG_ERR) ||
      (signal(SIGTERM, &SigHandler) == SIG_ERR) ||
      (signal(SIGSEGV, &SigHandler) == SIG_ERR)) {
    LOGGER_FATAL(logger_, "SDL is not subscribed to signal events");
  }
}
}  //  namespace utils

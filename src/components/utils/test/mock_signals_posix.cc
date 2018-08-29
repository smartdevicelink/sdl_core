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

#include "gtest/gtest.h"
#include "gmock/gmock.h"
#include "utils/mock_signals_posix.h"
#include "utils/signals.h"

namespace utils {

bool Signals::UnsubscribeFromTermination() {
  return MockSignalsPosix::signals_posix_mock()->UnsubscribeFromTermination();
}

bool Signals::WaitTerminationSignals(sighandler_t sig_handler) {
  return MockSignalsPosix::signals_posix_mock()->WaitTerminationSignals(
      sig_handler);
}

bool Signals::UnsubscribeFromLowVoltageSignals(
    const main_namespace::LowVoltageSignalsOffset& offset_data) {
  return MockSignalsPosix::signals_posix_mock()
      ->UnsubscribeFromLowVoltageSignals(offset_data);
}

void Signals::SendSignal(const int signo, const pid_t pid) {
  MockSignalsPosix::signals_posix_mock()->SendSignal(signo, pid);
}

void Signals::ExitProcess(const int status) {
  MockSignalsPosix::signals_posix_mock()->ExitProcess(status);
}

void Signals::WaitPid(pid_t cpid, int* status, int options) {
  MockSignalsPosix::signals_posix_mock()->WaitPid(cpid, status, options);
}

pid_t Signals::Fork() {
  return MockSignalsPosix::signals_posix_mock()->Fork();
}

MockSignalsPosix* MockSignalsPosix::signals_posix_mock() {
  static ::testing::NiceMock<MockSignalsPosix> signals_posix_mock;
  return &signals_posix_mock;
}

}  // namespace utils

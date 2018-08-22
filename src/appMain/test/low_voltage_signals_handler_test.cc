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

#include <memory>
#include "gtest/gtest.h"
#include "appMain/test/mock_life_cycle.h"
#include "utils/mock_signals_posix.h"
#include "config_profile/profile.h"
#include "utils/macro.h"

namespace test {

using ::testing::_;
using ::testing::Return;
using ::testing::InSequence;

class LowVoltageSignalsHandlerTest : public ::testing::Test {
 protected:
  LowVoltageSignalsHandlerTest()
      : mock_life_cycle_(std::make_shared<main_namespace::MockLifeCycle>())
      , mock_signals_posix_(*utils::MockSignalsPosix::signals_posix_mock()) {}

  void SetUp() OVERRIDE {
    profile_.set_config_file_name("smartDeviceLink.ini");
    signals_offset_ = {profile_.low_voltage_signal_offset(),
                       profile_.wake_up_signal_offset(),
                       profile_.ignition_off_signal_offset()};

    low_voltage_signals_handler_ =
        std::unique_ptr<main_namespace::LowVoltageSignalsHandler>(
            new main_namespace::LowVoltageSignalsHandler(
                *mock_life_cycle_.get(), signals_offset_));
  }

  profile::Profile profile_;
  main_namespace::LowVoltageSignalsOffset signals_offset_;
  std::unique_ptr<main_namespace::LowVoltageSignalsHandler>
      low_voltage_signals_handler_;
  std::shared_ptr<main_namespace::MockLifeCycle> mock_life_cycle_;
  utils::MockSignalsPosix& mock_signals_posix_;
};

TEST_F(
    LowVoltageSignalsHandlerTest,
    LowVoltageSignalReceived_CheckParentProcessBehavior_ExpectChildCreationAndtLowVoltageCall) {
  // To guarantee strict call orders
  InSequence guarantees_calls_sequence;
  // Set expectation after LOW VOLTAGE signal
  EXPECT_CALL(*mock_life_cycle_, LowVoltage());
  // Expect child process creation
  const pid_t cpid = 111;
  EXPECT_CALL(mock_signals_posix_, Fork()).WillOnce(Return(cpid));
  // Expect parent process sleep
  EXPECT_CALL(mock_signals_posix_, WaitPid(_, nullptr, 0));
  // Expect parent process wakes up
  EXPECT_CALL(*mock_life_cycle_, WakeUp());
  const int low_voltage_signo =
      low_voltage_signals_handler_->low_voltage_signo();
  // Emulate LOW VOLTAGE signal receipt and handling
  low_voltage_signals_handler_->HandleSignal(low_voltage_signo);
}

TEST_F(
    LowVoltageSignalsHandlerTest,
    LowVoltageSignalReceived_CheckChildProcessBehavior_ExpectChildSendsStopToParentProcess) {
  // To guarantee strict call orders
  InSequence guarantees_calls_sequence;
  // Set expectation after LOW VOLTAGE signal
  EXPECT_CALL(*mock_life_cycle_, LowVoltage());
  // Expect child process creation
  const pid_t cpid = 0;
  EXPECT_CALL(mock_signals_posix_, Fork()).WillOnce(Return(cpid));
  // Expect SIGCONT signal to be sent to parent process
  EXPECT_CALL(mock_signals_posix_, SendSignal(SIGSTOP, _));
  const int low_voltage_signo =
      low_voltage_signals_handler_->low_voltage_signo();
  // Emulate LOW VOLTAGE signal receipt and handling
  low_voltage_signals_handler_->HandleSignal(low_voltage_signo);
}

TEST_F(LowVoltageSignalsHandlerTest,
       WakeUpSignalReceived_ExpectParentProcessWakeUpAndChildProcessExit) {
  // To guarantee strict call orders
  InSequence guarantees_calls_sequence;
  // Expect SIGCONT signal to be sent to parent process
  EXPECT_CALL(mock_signals_posix_, SendSignal(SIGCONT, _));
  // Expect child process exit
  EXPECT_CALL(mock_signals_posix_, ExitProcess(0));
  const int wake_up_signo = low_voltage_signals_handler_->wake_up_signo();
  // Emulate WAKE UP signal receipt and handling
  low_voltage_signals_handler_->HandleSignal(wake_up_signo);
}

TEST_F(LowVoltageSignalsHandlerTest,
       IgnitionOffSignalReceived_ExpectAllProcessesStopped) {
  // To guarantee strict call orders
  InSequence guarantees_calls_sequence;
  // Expect SIGKILL signal to be sent to parent process
  EXPECT_CALL(mock_signals_posix_, SendSignal(SIGKILL, _));
  // Expect child process exit
  EXPECT_CALL(mock_signals_posix_, ExitProcess(0));
  const int ign_off_signo = low_voltage_signals_handler_->ignition_off_signo();
  // Emulate IGN OFF signal receipt and handling
  low_voltage_signals_handler_->HandleSignal(ign_off_signo);
}

}  //  namespace test

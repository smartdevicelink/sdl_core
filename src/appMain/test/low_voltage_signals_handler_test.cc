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
#include "config_profile/profile.h"
#include "utils/macro.h"

namespace test {

class LowVoltageSignalsHandlerTest : public ::testing::Test {
 protected:
  void SetUp() OVERRIDE {
    profile_.set_config_file_name("smartDeviceLink.ini");
    signals_offset_ = {profile_.low_voltage_signal_offset(),
                       profile_.wake_up_signal_offset(),
                       profile_.ignition_off_signal_offset()};

    low_voltage_signals_handler_ =
        std::unique_ptr<main_namespace::LowVoltageSignalsHandler>(
            new main_namespace::LowVoltageSignalsHandler(mock_life_cycle_,
                                                         signals_offset_));
  }

  profile::Profile profile_;
  main_namespace::LowVoltageSignalsOffset signals_offset_;
  std::unique_ptr<main_namespace::LowVoltageSignalsHandler>
      low_voltage_signals_handler_;
  main_namespace::MockLifeCycle mock_life_cycle_;
};

TEST_F(LowVoltageSignalsHandlerTest,
       LowVoltageSignalReceived_ExpectLifeCycleLowVoltageCall) {
  // Check that initial SDL state is running
  EXPECT_EQ(main_namespace::SDLState::kRun,
            low_voltage_signals_handler_->get_current_sdl_state());
  // Set expectation after LOW VOLTAGE signal
  EXPECT_CALL(mock_life_cycle_, LowVoltage());
  const int low_voltage_signo =
      low_voltage_signals_handler_->low_voltage_signo();
  // Emulate LOW VOLTAGE signal receipt and handling
  low_voltage_signals_handler_->HandleSignal(low_voltage_signo);
  // Check that SDL is in sleep state after LOW VOLTAGE
  EXPECT_EQ(main_namespace::SDLState::kSleep,
            low_voltage_signals_handler_->get_current_sdl_state());
}

TEST_F(LowVoltageSignalsHandlerTest,
       WakeUpSignalReceived_ExpectLifeCycleWakeUpCall) {
  // Check that initial SDL state is running
  EXPECT_EQ(main_namespace::SDLState::kRun,
            low_voltage_signals_handler_->get_current_sdl_state());
  EXPECT_CALL(mock_life_cycle_, LowVoltage());
  const int low_voltage_signo =
      low_voltage_signals_handler_->low_voltage_signo();
  // Emulate LOW VOLTAGE signals receipt and handling
  low_voltage_signals_handler_->HandleSignal(low_voltage_signo);
  // Check that SDL is in sleep state after LOW VOLTAGE
  EXPECT_EQ(main_namespace::SDLState::kSleep,
            low_voltage_signals_handler_->get_current_sdl_state());
  EXPECT_CALL(mock_life_cycle_, WakeUp());
  const int wake_up_signo = low_voltage_signals_handler_->wake_up_signo();
  // Emulate WAKE UP signal receipt and handling
  low_voltage_signals_handler_->HandleSignal(wake_up_signo);
  // Check that SDL is in running state after WAKE UP
  EXPECT_EQ(main_namespace::SDLState::kRun,
            low_voltage_signals_handler_->get_current_sdl_state());
}

TEST_F(LowVoltageSignalsHandlerTest,
       IgnitionOffSignalReceived_ExpectLifeCycleIgnitionOffCall) {
  // Check that initial SDL state is running
  EXPECT_EQ(main_namespace::SDLState::kRun,
            low_voltage_signals_handler_->get_current_sdl_state());
  EXPECT_CALL(mock_life_cycle_, LowVoltage());
  const int low_voltage_signo =
      low_voltage_signals_handler_->low_voltage_signo();
  // Emulate LOW VOLTAGE signals receipt and handling
  low_voltage_signals_handler_->HandleSignal(low_voltage_signo);
  // Check that SDL is in sleep state after LOW VOLTAGE
  EXPECT_EQ(main_namespace::SDLState::kSleep,
            low_voltage_signals_handler_->get_current_sdl_state());
  EXPECT_CALL(mock_life_cycle_, IgnitionOff());
  const int ignition_off_signo =
      low_voltage_signals_handler_->ignition_off_signo();

  // Emulate IGNITION OFF signal receipt and handling
  low_voltage_signals_handler_->HandleSignal(ignition_off_signo);
  // Check that SDL is in stopped state after IGNITION OFF
  EXPECT_EQ(main_namespace::SDLState::kStop,
            low_voltage_signals_handler_->get_current_sdl_state());
}
}  //  namespace test

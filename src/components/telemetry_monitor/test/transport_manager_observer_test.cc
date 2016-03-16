/*
 * Copyright (c) 2015, Ford Motor Company
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
#include "telemetry_monitor/mock_telemetry_monitor.h"
#include "protocol_handler/telemetry_observer.h"
#include "telemetry_monitor/transport_manager_metric_wrapper.h"
#include "telemetry_monitor/transport_manager_observer.h"

namespace test {
namespace components {
namespace telemetry_monitor_test {

using namespace telemetry_monitor;
using ::testing::_;

TEST(TransportManagerObserverTest, MessageProcess) {
  MockTelemetryMonitor mock_telemetry_monitor;
  TransportManagerObserver tr_observer(&mock_telemetry_monitor);
  protocol_handler::RawMessage* ptr = new ::protocol_handler::RawMessage(0, 0, NULL, 0);
  tr_observer.StartRawMsg(ptr);
  EXPECT_CALL(mock_telemetry_monitor, SendMetric(_));
  tr_observer.StopRawMsg(ptr);
  delete ptr;
}

}  // namespace telemetry_monitor
}  // namespace components
}  // namespace test

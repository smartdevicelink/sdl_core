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

#include "telemetry_monitor/application_manager_observer.h"
#include "application_manager/telemetry_observer.h"
#include "gtest/gtest.h"
#include "telemetry_monitor/application_manager_metric_wrapper.h"
#include "telemetry_monitor/mock_telemetry_monitor.h"

#include "telemetry_monitor/telemetry_monitor.h"

namespace test {
namespace components {
namespace telemetry_monitor_test {

using namespace telemetry_monitor;
using ::testing::_;

TEST(ApplicationManagerObserver, CallOnMessage) {
  MockTelemetryMonitor mock_telemetry_monitor;
  ApplicationManagerObserver app_observer(&mock_telemetry_monitor);
  typedef application_manager::AMTelemetryObserver::MessageMetric AMMetric;
  std::shared_ptr<AMMetric> ptr =
      application_manager::AMTelemetryObserver::MessageMetricSharedPtr();
  EXPECT_CALL(mock_telemetry_monitor, SendMetric(_));
  app_observer.OnMessage(ptr);
}

}  // namespace telemetry_monitor_test
}  // namespace components
}  // namespace test

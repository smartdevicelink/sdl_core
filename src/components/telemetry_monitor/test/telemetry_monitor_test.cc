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
#include "telemetry_monitor/telemetry_monitor.h"
#include "protocol_handler/telemetry_observer.h"
#include "protocol_handler/protocol_handler.h"
#include "protocol_handler//mock_protocol_handler.h"
#include "protocol_handler/mock_session_observer.h"
#include "protocol_handler/mock_protocol_handler_settings.h"
#include "connection_handler/mock_connection_handler.h"
#include "transport_manager/mock_transport_manager.h"
#include "telemetry_monitor/mock_telemetry_observable.h"


using testing::Return;
using testing::_;

namespace test {
namespace components {
namespace telemetry_monitor_test {

using namespace telemetry_monitor;

class StreamerMock : public Streamer {
 public:
  StreamerMock(TelemetryMonitor* const server)
      : Streamer(server) {
    is_client_connected_ = true;
  }
  MOCK_METHOD1(PushMessage,void(utils::SharedPtr<MetricWrapper> metric));
};

TEST(TelemetryMonitorTest, MessageProcess) {
  const std::string& server_address = "server_address";
  const uint16_t port = 12345;
  MockTelemetryObservable am_observeble;

   transport_manager_test::MockTransportManager transport_manager_mock;
   testing::NiceMock<connection_handler_test::MockConnectionHandler>
       connection_handler_mock;
  testing::NiceMock<
      ::test::components::protocol_handler_test::MockProtocolHandlerSettings>
       protocol_handler_settings_mock;
   ::test::components::protocol_handler_test::MockSessionObserver
       session_observer_mock;

  const size_t init_value = 1000u;
  ON_CALL(protocol_handler_settings_mock, maximum_payload_size())
      .WillByDefault(Return(init_value));
  ON_CALL(protocol_handler_settings_mock, message_frequency_time())
      .WillByDefault(Return(init_value));
  ON_CALL(protocol_handler_settings_mock, malformed_message_filtering())
      .WillByDefault(Return(init_value));
  ON_CALL(protocol_handler_settings_mock, multiframe_waiting_timeout())
      .WillByDefault(Return(init_value));
   protocol_handler::ProtocolHandlerImpl protocol_handler_mock(
      protocol_handler_settings_mock, session_observer_mock,
      connection_handler_mock, transport_manager_mock);

  EXPECT_CALL(am_observeble, SetTelemetryObserver(_));
  EXPECT_CALL(transport_manager_mock, SetTelemetryObserver(_));
  telemetry_monitor::TelemetryMonitor telemetry_monitor(server_address, port);
  StreamerMock streamer_mock(&telemetry_monitor);
  telemetry_monitor.Start();

  telemetry_monitor.set_streamer(&streamer_mock);
  telemetry_monitor.Init(&protocol_handler_mock, &am_observeble,
                           &transport_manager_mock);
  utils::SharedPtr<telemetry_monitor::MetricWrapper> test_metric;
  EXPECT_CALL(streamer_mock, PushMessage(test_metric));
  telemetry_monitor.SendMetric(test_metric);
}

}  // namespace telemetry_monitor
}  // namespace components
}  // namespace test

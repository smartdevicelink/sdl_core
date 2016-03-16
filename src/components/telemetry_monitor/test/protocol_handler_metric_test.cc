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
#include "telemetry_monitor/json_keys.h"
#include "json/json.h"
#include "utils/resource_usage.h"
#include "telemetry_monitor/protocol_handler_metric_wrapper.h"
#include "protocol_handler/telemetry_observer.h"

namespace test {
namespace components {
namespace telemetry_monitor_test {

using namespace ::telemetry_monitor;

TEST(ProtocolHandlerMetricTest, grabResources) {
  ProtocolHandlerMecticWrapper metric_test;
  EXPECT_TRUE(metric_test.grabResources());
}

TEST(ProtocolHandlerMetricTest, GetJsonMetric) {
  ProtocolHandlerMecticWrapper metric_test;

  TimevalStruct start_time;
  start_time.tv_sec = 1;
  start_time.tv_usec = 0;

  TimevalStruct end_time;
  end_time.tv_sec = 10;
  end_time.tv_usec = 0;
  metric_test.message_metric = new protocol_handler::PHTelemetryObserver::MessageMetric();
  metric_test.message_metric->begin = start_time;
  metric_test.message_metric->end = end_time;
  metric_test.message_metric->message_id = 5;
  metric_test.message_metric->connection_key = 2;
  Json::Value jvalue = metric_test.GetJsonMetric();

  EXPECT_EQ("\"ProtocolHandler\"\n", jvalue[strings::logger].toStyledString());
  EXPECT_EQ("null\n", jvalue[strings::stime].toStyledString());
  EXPECT_EQ("null\n", jvalue[strings::utime].toStyledString());
  EXPECT_EQ("null\n", jvalue[strings::memory].toStyledString());

  EXPECT_EQ(date_time::DateTime::getuSecs(start_time), jvalue[strings::begin].asInt64());
  EXPECT_EQ(date_time::DateTime::getuSecs(end_time), jvalue[strings::end].asInt64());
  EXPECT_EQ(5, jvalue[strings::message_id].asInt64());
  EXPECT_EQ(2, jvalue[strings::connection_key].asInt());
}

TEST(ProtocolHandlerMetricTest, GetJsonMetricWithGrabResources) {
  ProtocolHandlerMecticWrapper metric_test;
  utils::ResourseUsage* resources = utils::Resources::getCurrentResourseUsage();
  EXPECT_TRUE(resources != NULL);
  EXPECT_TRUE(metric_test.grabResources());

  TimevalStruct start_time;
  start_time.tv_sec = 1;
  start_time.tv_usec = 0;

  TimevalStruct end_time;
  end_time.tv_sec = 10;
  end_time.tv_usec = 0;
  metric_test.message_metric = new protocol_handler::PHTelemetryObserver::MessageMetric();
  metric_test.message_metric->begin = start_time;
  metric_test.message_metric->end = end_time;
  metric_test.message_metric->message_id = 5;
  metric_test.message_metric->connection_key = 2;
  Json::Value jvalue = metric_test.GetJsonMetric();

  EXPECT_TRUE(jvalue[strings::stime].isInt());
  EXPECT_TRUE(jvalue[strings::utime].isInt());
  EXPECT_TRUE(jvalue[strings::memory].isInt());
  EXPECT_NE("null/n", jvalue[strings::stime].toStyledString());
  EXPECT_NE("null/n", jvalue[strings::utime].toStyledString());
  EXPECT_NE("null/n", jvalue[strings::memory].toStyledString());

  EXPECT_EQ(date_time::DateTime::getuSecs(start_time), jvalue[strings::begin].asInt64());
  EXPECT_EQ(date_time::DateTime::getuSecs(end_time), jvalue[strings::end].asInt64());
  EXPECT_EQ(5, jvalue[strings::message_id].asInt64());
  EXPECT_EQ(2, jvalue[strings::connection_key].asInt());

  EXPECT_NEAR(resources->stime, jvalue[strings::stime].asInt(),1);
  EXPECT_NEAR(resources->utime, jvalue[strings::utime].asInt(),1);
  EXPECT_EQ(resources->memory, jvalue[strings::memory].asInt());
  delete resources;
}

}  // namespace telemetry_monitor
}  // namespace components
}  // namespace test

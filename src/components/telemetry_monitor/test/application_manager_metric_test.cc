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
#include "utils/resource_usage.h"
#include "application_manager/smart_object_keys.h"
#include "telemetry_monitor/application_manager_metric_wrapper.h"

namespace test {
namespace components {
namespace telemetry_monitor_test {

using namespace ::telemetry_monitor;

TEST(ApplicationManagerMetricWrapper, grabResources) {
  ApplicationManagerMetricWrapper metric_test;
  EXPECT_TRUE(metric_test.grabResources());
}

TEST(ApplicationManagerMetricWrapper, GetJsonMetric) {
  ApplicationManagerMetricWrapper metric_test;

  TimevalStruct start_time;
  start_time.tv_sec = 1;
  start_time.tv_usec = 0;

  TimevalStruct end_time;
  end_time.tv_sec = 10;
  end_time.tv_usec = 0;
  metric_test.message_metric = new application_manager::AMTelemetryObserver::MessageMetric();
  metric_test.message_metric->begin = start_time;
  metric_test.message_metric->end = end_time;
  NsSmartDeviceLink::NsSmartObjects::SmartObject obj;
  obj["params"][application_manager::strings::correlation_id] = 11;
  obj["params"][application_manager::strings::connection_key] = 12;
  metric_test.message_metric->message = new NsSmartDeviceLink::NsSmartObjects::SmartObject(obj);
  Json::Value jvalue = metric_test.GetJsonMetric();

  EXPECT_EQ("null\n", jvalue[telemetry_monitor::strings::stime].toStyledString());
  EXPECT_EQ("null\n", jvalue[telemetry_monitor::strings::utime].toStyledString());
  EXPECT_EQ("null\n", jvalue[telemetry_monitor::strings::memory].toStyledString());

  EXPECT_EQ(date_time::DateTime::getuSecs(start_time), jvalue[telemetry_monitor::strings::begin].asInt64());
  EXPECT_EQ(date_time::DateTime::getuSecs(end_time), jvalue[telemetry_monitor::strings::end].asInt64());
  EXPECT_EQ(obj["params"][application_manager::strings::correlation_id].asInt(), jvalue[telemetry_monitor::strings::correlation_id].asInt64());
  EXPECT_EQ(obj["params"][application_manager::strings::connection_key].asInt(), jvalue[telemetry_monitor::strings::connection_key].asInt());
}

TEST(ApplicationManagerMetricWrapper, GetJsonMetricWithGrabResources) {
  ApplicationManagerMetricWrapper metric_test;
  utils::ResourseUsage* resources = utils::Resources::getCurrentResourseUsage();
  EXPECT_TRUE(metric_test.grabResources());

  TimevalStruct start_time;
  start_time.tv_sec = 1;
  start_time.tv_usec = 0;

  TimevalStruct end_time;
  end_time.tv_sec = 10;
  end_time.tv_usec = 0;

  metric_test.message_metric = new application_manager::AMTelemetryObserver::MessageMetric();
  metric_test.message_metric->begin = start_time;
  metric_test.message_metric->end = end_time;
  NsSmartDeviceLink::NsSmartObjects::SmartObject obj;
  obj["params"][application_manager::strings::correlation_id] = 11;
  obj["params"][application_manager::strings::connection_key] = 12;
  metric_test.message_metric->message = new NsSmartDeviceLink::NsSmartObjects::SmartObject(obj);
  Json::Value jvalue = metric_test.GetJsonMetric();

  EXPECT_EQ(date_time::DateTime::getuSecs(start_time), jvalue[telemetry_monitor::strings::begin].asInt64());
  EXPECT_EQ(date_time::DateTime::getuSecs(end_time), jvalue[telemetry_monitor::strings::end].asInt64());
  EXPECT_EQ(obj["params"][application_manager::strings::correlation_id].asInt(), jvalue[telemetry_monitor::strings::correlation_id].asInt64());
  EXPECT_EQ(obj["params"][application_manager::strings::connection_key].asInt(), jvalue[telemetry_monitor::strings::connection_key].asInt());

  EXPECT_EQ(date_time::DateTime::getuSecs(start_time), jvalue[telemetry_monitor::strings::begin].asInt64());
  EXPECT_EQ(date_time::DateTime::getuSecs(end_time), jvalue[telemetry_monitor::strings::end].asInt64());

  EXPECT_NEAR(resources->stime, jvalue[telemetry_monitor::strings::stime].asInt(),1);
  EXPECT_NEAR(resources->utime, jvalue[telemetry_monitor::strings::utime].asInt(),1);
  EXPECT_EQ(resources->memory, jvalue[telemetry_monitor::strings::memory].asInt());

  delete resources;
}

}  // namespace telemetry_monitor
}  // namespace components
}  // namespace test

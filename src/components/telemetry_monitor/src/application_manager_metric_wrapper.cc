/*
 * Copyright (c) 2014, Ford Motor Company
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

#include "telemetry_monitor/application_manager_metric_wrapper.h"
#include "telemetry_monitor/json_keys.h"
#include "application_manager/smart_object_keys.h"
#include "utils/convert_utils.h"

namespace telemetry_monitor {

Json::Value ApplicationManagerMetricWrapper::GetJsonMetric() {
  Json::Value result = MetricWrapper::GetJsonMetric();
  result[strings::logger] = "ApplicationManager";
  result[strings::begin] =
      Json::Int64(date_time::DateTime::getuSecs(message_metric->begin));
  result[strings::end] =
      Json::Int64(date_time::DateTime::getuSecs(message_metric->end));
  const NsSmartDeviceLink::NsSmartObjects::SmartObject& params =
      message_metric->message->getElement(application_manager::strings::params);
  result[strings::correlation_id] = utils::ConvertInt64ToLongLongInt(
      params[application_manager::strings::correlation_id].asInt());
  result[strings::connection_key] = utils::ConvertInt64ToLongLongInt(
      params[application_manager::strings::connection_key].asInt());
  return result;
}
}  // namespace telemetry_monitor

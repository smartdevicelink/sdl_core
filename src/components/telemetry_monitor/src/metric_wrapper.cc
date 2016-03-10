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

#include "telemetry_monitor/metric_wrapper.h"
#include "telemetry_monitor/json_keys.h"

namespace telemetry_monitor {

  MetricWrapper::MetricWrapper():
    resources(NULL) {
  }

bool MetricWrapper::grabResources() {
  Clear();
  resources = utils::Resources::getCurrentResourseUsage();
  if (NULL != resources) {
    return true;
  } else {
    return false;
  }
  flush(std::cout);
}

std::string MetricWrapper::GetStyledString() {
  return GetJsonMetric().toStyledString();
}

Json::Value MetricWrapper::GetJsonMetric() {
  Json::Value result;
  if (resources) {
    result[strings::stime] = resources->stime;
    result[strings::utime] = resources->utime;
    result[strings::memory] = resources->memory;
  }
  return result;
}

void MetricWrapper::Clear() {
  if (NULL != resources) {
    delete resources;
    resources = NULL;
  }
}

MetricWrapper::~MetricWrapper() {
  Clear();
}

}

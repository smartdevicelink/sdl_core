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
#include "telemetry_monitor/transport_manager_observer.h"

#include <time.h>

#include "telemetry_monitor/transport_manager_metric_wrapper.h"
#include "telemetry_monitor/telemetry_monitor.h"

namespace telemetry_monitor {

TransportManagerObserver::TransportManagerObserver(TelemetryMonitor* telemetry_monitor):
  telemetry_monitor_ (telemetry_monitor) {
}

void TransportManagerObserver::StartRawMsg(const protocol_handler::RawMessage* ptr) {
  time_starts[ptr] = date_time::DateTime::getCurrentTime();
}

void TransportManagerObserver::StopRawMsg(const protocol_handler::RawMessage* ptr) {
    std::map<const protocol_handler::RawMessage*, TimevalStruct>::const_iterator it;
    it = time_starts.find(ptr);
    if (it != time_starts.end()) {
      TransportManagerMecticWrapper* m = new TransportManagerMecticWrapper();
      m->message_metric = new transport_manager::TMTelemetryObserver::MessageMetric();
      m->message_metric->begin = it->second;
      m->message_metric->end = date_time::DateTime::getCurrentTime();
      m->message_metric->data_size = ptr->data_size();
      m->grabResources();
      telemetry_monitor_->SendMetric(m);
    }
}

} //namespace telemetry_monitor

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

#ifndef SRC_COMPONENTS_TELEMETRY_MONITOR_INCLUDE_TELEMETRY_MONITOR_H_
#define SRC_COMPONENTS_TELEMETRY_MONITOR_INCLUDE_TELEMETRY_MONITOR_H_

#include <string>

#include "utils/shared_ptr.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"
#include "utils/threads/thread_delegate.h"
#include "telemetry_monitor/metric_wrapper.h"
#include "telemetry_monitor/application_manager_observer.h"
#include "application_manager/application_manager_impl.h"
#include "telemetry_monitor/transport_manager_observer.h"
#include "transport_manager/transport_manager_impl.h"
#include "protocol_handler_observer.h"
#include "protocol_handler/protocol_handler_impl.h"

namespace telemetry_monitor {

using ::utils::MessageQueue;

class Streamer : public threads::ThreadDelegate {
 public:
  explicit Streamer(TelemetryMonitor* const server);
  ~Streamer();
  void threadMain() OVERRIDE;
  void exitThreadMain() OVERRIDE;

  virtual void PushMessage(utils::SharedPtr<MetricWrapper> metric);
  volatile bool is_client_connected_;
 private:
  void Start();
  void Stop();
  bool IsReady() const;
  bool Send(const std::string &msg);
  void ShutDownAndCloseSocket(int32_t socket_fd);
  TelemetryMonitor* const kserver_;
  int32_t server_socket_fd_;
  int32_t client_socket_fd_;
  volatile bool stop_flag_;
  MessageQueue<utils::SharedPtr<MetricWrapper> > messages_;
  DISALLOW_COPY_AND_ASSIGN(Streamer);
};

class TelemetryMonitor {
 public:
  TelemetryMonitor(const std::string& server_address, uint16_t port);
  virtual ~TelemetryMonitor();
  virtual void Init(
      TelemetryObservable<protocol_handler::PHTelemetryObserver>* protocol_handler,
      TelemetryObservable<application_manager::AMTelemetryObserver>* app_manager,
      TelemetryObservable<transport_manager::TMTelemetryObserver>* transport_manager);
  virtual void Stop();
  virtual void Start();
  virtual void SendMetric(utils::SharedPtr<MetricWrapper> metric);
  void set_streamer(Streamer* streamer);
  const std::string& ip() const;
  int16_t port() const;

 private:
  std::string server_address_;
  int16_t port_;
  bool is_ready_;
  threads::Thread* thread_;
  Streamer* streamer_;
  ApplicationManagerObserver app_observer;
  TransportManagerObserver tm_observer;
  ProtocolHandlerObserver ph_observer;

  DISALLOW_COPY_AND_ASSIGN(TelemetryMonitor);
};
}  // namespace telemetry_monitor
#endif  // SRC_COMPONENTS_TELEMETRY_MONITOR_INCLUDE_TELEMETRY_MONITOR_H_

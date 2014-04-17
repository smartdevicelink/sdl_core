/**
 * Copyright (c) 2013, Ford Motor Company
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
#include "connection_handler/heartbeat_monitor.h"
#include "connection_handler/connection.h"
#include "utils/logger.h"

namespace connection_handler {

namespace {
  GETLOGGER(logger_, "ConnectionHandler")
}

HeartBeatMonitor::HeartBeatMonitor(int32_t heartbeat_timeout_seconds,
                                   Connection* connection)
    : heartbeat_timeout_seconds_(heartbeat_timeout_seconds),
      connection_(connection),
      timer_(this, &HeartBeatMonitor::TimeOut) {
}

HeartBeatMonitor::~HeartBeatMonitor() {
  AssertRunningOnCreationThread();
}

void HeartBeatMonitor::BeginMonitoring() {
  AssertRunningOnCreationThread();
  if (heartbeat_timeout_seconds_ != 0) {
    LOG4CXX_INFO(logger_, "Heart beat monitor: monitoring connection "
                 << connection_->connection_handle() << ", timeout "
                 << heartbeat_timeout_seconds_ << " seconds");
    timer_.start(heartbeat_timeout_seconds_);
  } else {
    LOG4CXX_INFO(logger_, "Heart beat monitor: disabled");
  }
}

void HeartBeatMonitor::TimeOut() {
  LOG4CXX_INFO(logger_, "Heart beat monitor: connection "
               << connection_->connection_handle() << " timed out, closing");
  connection_->Close();
}

void HeartBeatMonitor::KeepAlive() {
  AssertRunningOnCreationThread();
  if (heartbeat_timeout_seconds_ != 0) {
    LOG4CXX_INFO(
        logger_,
        "Resetting heart beat timer for connection "
          << connection_->connection_handle());
    timer_.stop();
    timer_.start(heartbeat_timeout_seconds_);
  }
}


} // namespace connection_handler

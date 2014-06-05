/**
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

#include <algorithm>

#include "connection_handler/connection.h"
#include "connection_handler/connection_handler.h"
#include "protocol_handler/protocol_packet.h"
#include "utils/logger.h"
#include "utils/macro.h"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ConnectionHandler")

Connection::Connection(ConnectionHandle connection_handle,
                       DeviceHandle connection_device_handle,
                       ConnectionHandler* connection_handler,
                       int32_t heartbeat_timeout)
    : connection_handler_(connection_handler),
      connection_handle_(connection_handle),
      connection_device_handle_(connection_device_handle) {
  DCHECK(connection_handler_);

  heartbeat_monitor_ = new HeartBeatMonitor(heartbeat_timeout, this);
  heart_beat_monitor_thread_ = new threads::Thread("HeartBeatMonitorThread",
                                                   heartbeat_monitor_);
}

Connection::~Connection() {
  session_map_.clear();
  heart_beat_monitor_thread_->stop();
  delete heart_beat_monitor_thread_;
}

int32_t Connection::AddNewSession() {
  sync_primitives::AutoLock lock(session_map_lock_);

  int32_t result = -1;

  const uint8_t max_connections = 255;
  int32_t size = session_map_.size();
  if (max_connections > size) {

   for (uint8_t session_id = 1; session_id <= 255; ++session_id) {
     if (session_map_.end() == session_map_.find(session_id)) {
       /* whenever new session created RPC and Bulk services are
       established automatically */
       session_map_[session_id].push_back(protocol_handler::kRpc);
       session_map_[session_id].push_back(protocol_handler::kBulk);
       result = session_id;
       break;
     }
   }
  }

  return result;
}

int32_t Connection::RemoveSession(uint8_t session) {
  sync_primitives::AutoLock lock(session_map_lock_);
  int32_t result = -1;
  SessionMapIterator it = session_map_.find(session);
  if (session_map_.end() == it) {
    LOG4CXX_ERROR(logger_, "Session not found in this connection!");
  } else {
    heartbeat_monitor_->RemoveSession(session);
    session_map_.erase(session);
    result = session;
  }

  return result;
}

bool Connection::AddNewService(uint8_t session, uint8_t service) {
  sync_primitives::AutoLock lock(session_map_lock_);
  bool result = false;

  SessionMapIterator session_it = session_map_.find(session);
  if (session_it == session_map_.end()) {
    LOG4CXX_ERROR(logger_, "Session not found in this connection!");
    return result;
  }

  ServiceListIterator service_it = find(session_it->second.begin(),
                                        session_it->second.end(), service);
  if (service_it != session_it->second.end()) {
    LOG4CXX_ERROR(logger_, "Session " << static_cast<int32_t>(session) <<
                  " already established"
                  " service " << service);
  } else {
    session_it->second.push_back(service);
    result = true;
  }

  return result;
}

bool Connection::RemoveService(uint8_t session, uint8_t service) {
  sync_primitives::AutoLock lock(session_map_lock_);
  bool result = false;

  SessionMapIterator session_it = session_map_.find(session);
  if (session_it == session_map_.end()) {
    LOG4CXX_ERROR(logger_, "Session not found in this connection!");
    return result;
  }

  ServiceListIterator service_it = find(session_it->second.begin(),
                                        session_it->second.end(), service);
  if (service_it != session_it->second.end()) {
    session_it->second.erase(service_it);
    result = true;
  } else {
    LOG4CXX_ERROR(logger_, "Session " << static_cast<int32_t>(session) <<
                  " didn't established"
                  " service " << service);
  }

  return result;
}

ConnectionHandle Connection::connection_handle() const {
  return connection_handle_;
}

DeviceHandle Connection::connection_device_handle() {
  return connection_device_handle_;
}

const SessionMap Connection::session_map() const {
  sync_primitives::AutoLock lock(session_map_lock_);
  return session_map_;
}

void Connection::CloseSession(uint8_t session_id) {
  size_t size;

  {
    sync_primitives::AutoLock lock(session_map_lock_);

    SessionMapIterator session_it = session_map_.find(session_id);
    if (session_it == session_map_.end()) {
      return;
    }

    size = session_map_.size();
  }

  //Close connection if it is last session
  if (1 == size) {
    heartbeat_monitor_->RemoveSession(session_id);
    connection_handler_->CloseConnection(connection_handle_);
  } else {
    connection_handler_->CloseSession(connection_handle_, session_id);
  }
}

void Connection::StartHeartBeat(uint8_t session_id) {
    bool is_first_session = heartbeat_monitor_->AddSession(session_id);

    // start monitoring thread when first session with heartbeat added
    if (is_first_session) {
      heart_beat_monitor_thread_->start();
    }
}

void Connection::SendHeartBeat(uint8_t session_id) {
  connection_handler_->SendHeartBeat(connection_handle_, session_id);
}

void Connection::KeepAlive(uint8_t session_id) {
  heartbeat_monitor_->KeepAlive(session_id);
}

}/* namespace connection_handler */

/**
 * \file Connection.cpp
 * \brief Connection class implementation.
 *
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

#include <algorithm>

#include "connection_handler/connection.h"
#include "connection_handler/connection_handler.h"
#include "utils/macro.h"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

GETLOGGER(Connection::logger_, "ConnectionHandler")

Connection::Connection(ConnectionHandle connection_handle,
                       DeviceHandle connection_device_handle,
                       ConnectionHandler* connection_handler,
                       int32_t heartbeat_timeout)
    : connection_handler_(connection_handler),
      connection_handle_(connection_handle),
      connection_device_handle_(connection_device_handle),
      heartbeat_monitor_(heartbeat_timeout, this) {
  DCHECK(connection_handler_);
}

Connection::~Connection() {
  session_map_.clear();
}

int32_t Connection::AddNewSession() {
  sync_primitives::AutoLock lock(session_map_lock_);

  int32_t result = -1;

  if (session_map_.empty()) {
    heartbeat_monitor_.BeginMonitoring();
  }

  const uint8_t max_connections = 255;
  int32_t size = session_map_.size();
  if (max_connections > size) {

    ++size;
    /* whenever new session created RPC and Bulk services are
    established automatically */
    session_map_[size].push_back(
        static_cast<uint8_t>(protocol_handler::kRpc));
    session_map_[size].push_back(
        static_cast<uint8_t>(protocol_handler::kBulk));

    result = size;
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
    LOG4CXX_ERROR(logger_, "Session " << session << " already established"
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
    LOG4CXX_ERROR(logger_, "Session " << session << " didn't established"
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

void Connection::Close() {
  connection_handler_->CloseConnection(connection_handle_);
}

void Connection::KeepAlive() {
  heartbeat_monitor_.KeepAlive();
}

}/* namespace connection_handler */

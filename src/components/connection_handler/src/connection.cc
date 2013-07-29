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

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

log4cxx::LoggerPtr Connection::logger_ =
  log4cxx::LoggerPtr(log4cxx::Logger::getLogger("ConnectionHandler"));

Connection::Connection(ConnectionHandle connection_handle,
                       DeviceHandle connection_device_handle)
  : connection_handle_(connection_handle),
    connection_device_handle_(connection_device_handle),
    session_id_counter_(1) {
}

Connection::~Connection() {
}

int Connection::AddNewSession() {
  int result = -1;
  if (255 > session_id_counter_) {
    session_list_.push_back(session_id_counter_);
    result = session_id_counter_++;
  }
  return result;
}

int Connection::RemoveSession(unsigned char session) {
  int result = -1;
  SessionListIterator it = std::find(session_list_.begin(), session_list_.end(),
                                     session);
  if (session_list_.end() == it) {
    LOG4CXX_ERROR(logger_, "Session not found in this connection!");
  } else {
    session_list_.erase(it);
    result = session;
  }
  return result;
}

int Connection::GetFirstSessionID() {
  int result = -1;
  SessionListIterator it = session_list_.begin();
  if (session_list_.end() != it) {
    result = *it;
  }
  return result;
}

ConnectionHandle Connection::connection_handle() {
  return connection_handle_;
}

DeviceHandle Connection::connection_device_handle() {
  return connection_device_handle_;
}

void Connection::GetSessionList(SessionList& session_list) {
  session_list = session_list_;
}
}/* namespace connection_handler */

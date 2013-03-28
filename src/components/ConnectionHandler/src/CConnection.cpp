/**
 * \file CConnection.cpp
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

#include "Logger.hpp"

#include "ConnectionHandler/CConnection.hpp"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

log4cplus::Logger CConnection::logger_ = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("ConnectionHandler"));

CConnection::CConnection(ConnectionHandle aConnectionHandle,
                         DeviceHandle aConnectionDeviceHandle)
    : connection_handle_(aConnectionHandle),
      connection_device_handle_(aConnectionDeviceHandle),
      session_id_counter_(1) {
}

CConnection::~CConnection() {
}

int CConnection::AddNewSession() {
  int result = -1;
  if (255 > session_id_counter_) {
    session_list_.push_back(session_id_counter_);
    result = session_id_counter_++;
  }
  return result;
}

int CConnection::RemoveSession(unsigned char aSession) {
  int result = -1;
  SessionListIterator it = std::find(session_list_.begin(), session_list_.end(),
                                      aSession);
  if (session_list_.end() == it) {
    LOG4CPLUS_ERROR(logger_, "Session not found in this connection!");
  } else {
    session_list_.erase(it);
    result = aSession;
  }
  return result;
}

int CConnection::GetFirstSessionID() {
  int result = -1;
  SessionListIterator it = session_list_.begin();
  if (session_list_.end() != it) {
    result = *it;
  }
  return result;
}

ConnectionHandle CConnection::connection_handle() {
  return connection_handle_;
}

DeviceHandle CConnection::connection_device_handle() {
  return connection_device_handle_;
}

void CConnection::GetSessionList(SessionList & session_list) {
  session_list = session_list_;
}
}/* namespace connection_handler */

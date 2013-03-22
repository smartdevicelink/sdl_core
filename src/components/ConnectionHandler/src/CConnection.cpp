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
 * \namespace NsConnectionHandler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace NsConnectionHandler {

log4cplus::Logger CConnection::mLogger = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("ConnectionHandler"));

CConnection::CConnection(tConnectionHandle aConnectionHandle,
                         tDeviceHandle aConnectionDeviceHandle)
    : mConnectionHandle(aConnectionHandle),
      mConnectionDeviceHandle(aConnectionDeviceHandle),
      mSessionIDCounter(1) {
}

CConnection::~CConnection() {
}

int CConnection::addNewSession() {
  int result = -1;
  if (255 > mSessionIDCounter) {
    mSessionList.push_back(mSessionIDCounter);
    result = mSessionIDCounter++;
  }
  return result;
}

int CConnection::removeSession(unsigned char aSession) {
  int result = -1;
  tSessionListIterator it = std::find(mSessionList.begin(), mSessionList.end(),
                                      aSession);
  if (mSessionList.end() == it) {
    LOG4CPLUS_ERROR(mLogger, "Session not found in this connection!");
  } else {
    mSessionList.erase(it);
    result = aSession;
  }
  return result;
}

int CConnection::getFirstSessionID() {
  int result = -1;
  tSessionListIterator it = mSessionList.begin();
  if (mSessionList.end() != it) {
    result = *it;
  }
  return result;
}

tConnectionHandle CConnection::getConnectionHandle() {
  return mConnectionHandle;
}

tDeviceHandle CConnection::getConnectionDeviceHandle() {
  return mConnectionDeviceHandle;
}

void CConnection::GetSessionList(tSessionList & session_list) {
  session_list = mSessionList;
}
}/* namespace NsConnectionHandler */

/**
 * \file Connection.hpp
 * \brief Connection class.
 * Stores connection information
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

#ifndef SRC_COMPONENTS_CONNECTIONHANDLER_INCLUDE_CONNECTIONHANDLER_CONNECTION_H_
#define SRC_COMPONENTS_CONNECTIONHANDLER_INCLUDE_CONNECTIONHANDLER_CONNECTION_H_

#include <map>
#include <vector>

#include "Logger.hpp"
#include "ConnectionHandler/device.h"

/**
 * \namespace connection_handler
 * \brief SmartDeviceLink ConnectionHandler namespace.
 */
namespace connection_handler {

/**
 * \brief Type for ConnectionHandle
 */
typedef int ConnectionHandle;

/**
 * \brief Type for Sessions vector
 */
typedef std::vector<unsigned char> SessionList;

/**
 * \brief Type for Sessions vector iterator
 */
typedef std::vector<unsigned char>::iterator SessionListIterator;

/**
 * \class Connection
 * \brief Connection class
 */
class Connection {
 public:
  /**
   * \brief Class constructor
   */
  Connection(ConnectionHandle connection_handle,
              DeviceHandle connection_device_handle);

  /**
   * \brief Destructor
   */
  ~Connection();

  /**
   * \brief Returns device handle
   * \return DeviceHandle
   */
  ConnectionHandle connection_handle();

  /**
   * \brief Returns connection device handle
   * \return ConnectionDeviceHandle
   */
  DeviceHandle connection_device_handle();

  /**
   * \brief Adds session to connection
   * \return sessionID or -1 in case of issues
   */
  int AddNewSession();

  /**
   * \brief Removes session from connection
   * \param aSession session ID
   * \return sessionID or -1 in case of issues
   */
  int RemoveSession(unsigned char session);

  /**
   * \brief Returns ID of first session from connection
   * \return first sessionID or -1 in case of issues
   */
  int GetFirstSessionID();

  /**
   * \brief Returns list of sessions which have been opened in
   *  current connection.
   * \param session_list list of sessions
   */
  void GetSessionList(SessionList & session_list);

 private:
  /**
   * \brief Current connection handle.
   */
  ConnectionHandle connection_handle_;

  /**
   * \brief DeviceHandle of this connection.
   */
  DeviceHandle connection_device_handle_;

  /**
   * \brief Counter to generate session id's.
   */
  unsigned char session_id_counter_;

  /**
   * \brief Counter to generate session id's.
   */
  SessionList session_list_;

  /**
   * \brief For logging.
   */
  static log4cplus::Logger logger_;
};

/**
 * \brief Type for Connections map
 * Key is ConnectionHandle which is uniq
 */
typedef std::map<int, Connection> ConnectionList;

/**
 * \brief Type for Connections map iterator
 */
typedef std::map<int, Connection>::iterator ConnectionListIterator;

}/* namespace connection_handler */

#endif  // SRC_COMPONENTS_CONNECTIONHANDLER_INCLUDE_CONNECTIONHANDLER_CONNECTION_H_

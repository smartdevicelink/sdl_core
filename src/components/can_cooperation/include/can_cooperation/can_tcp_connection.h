/*
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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_SRC_CAN_TCP_CONNECTION_H_
#define SRC_COMPONENTS_CAN_COOPERATION_SRC_CAN_TCP_CONNECTION_H_

#include <string>
#include <deque>
#include "can_cooperation/can_connection.h"

namespace threads {
class Thread;
}

namespace can_cooperation {

/**
  * @class CANTCPConnection
  * @brief Implementation of CANConnection using TCP communication
  * between itself and CAN integrator software.
  * Configuration of connection is done in "can_config.json" file.
  * By default port is 8092, address is "127.0.0.1".
  * CAN integrator software acts as server, CANTCPConnection connects as client.
  */
class CANTCPConnection : public CANConnection {
 public:
  CANTCPConnection();
  ~CANTCPConnection();
  ConnectionState SendMessage(const CANMessage& message);
  ConnectionState ReadMessage(CANMessage* message);

 private:
  ConnectionState OpenConnection();
  ConnectionState CloseConnection();
  ConnectionState Flash();
  std::deque<CANMessage> to_send_;
  std::string address_;
  int port_;
  int socket_;
  ConnectionState current_state_;
  threads::Thread* thread_;

  friend class TCPClientDelegate;
  friend class TcpConnectionTest;
};
}  //  namespace can_cooperation

#endif  //  SRC_COMPONENTS_CAN_COOPERATION_SRC_CAN_TCP_CONNECTION_H_

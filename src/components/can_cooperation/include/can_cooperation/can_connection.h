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

#ifndef SRC_COMPONENTS_CAN_COOPERATION_SRC_CAN_CONNECTION_H_
#define SRC_COMPONENTS_CAN_COOPERATION_SRC_CAN_CONNECTION_H_

#include "json/json.h"
#include "utils/shared_ptr.h"

namespace can_cooperation {

/**
 * @brief Type for CAN<->RSDL messages
 */
typedef Json::Value CANMessage;

/**
 * @brief Enum describing possible CAN<->RSDL connection states
 */
enum ConnectionState { NONE = -1, OPENED, CLOSED, INVALID };

/**
  * @class CANConnectionObserver
  * @brief Interface class describing notifications of receiving message/
  * error occured during communication with CAN.
  * CANModule implements this interfaces.
  */
class CANConnectionObserver {
 public:
  virtual void OnCANMessageReceived(const CANMessage& message) = 0;
  virtual void OnCANConnectionError(ConnectionState state,
                                    const std::string& info) = 0;
};

/**
  * @class
  * @brief Abstract class defining methods for communication between
  * CAN-integration software and RSDL.
  * Class implementing CANConnection has to have subscriber (see
 * @CANConnectionObserver).
  * Class implementing CANConnection has to define SendMessage and ReadMessage
 * methods.
  */
class CANConnection {
 public:
  /**
   * @brief Destructor
   */
  virtual ~CANConnection() {
    observer_ = NULL;
  }
  /**
    * @brief Implementation of SendMessage pure virtual function should
    * send message from RSDL to CAN-integration software.
    * @param message Message to be sent to CAN-integration software
    * @return current state of connection with CAN-integration software
    */
  virtual ConnectionState SendMessage(const CANMessage& message) = 0;

  /**
    * @brief Sets observer of events occuring during communication
    * between RSDL and CAN-integration software
    * @param observer Pointer to implementation of CANConnectionObserver
   * interface
    */
  virtual void set_observer(CANConnectionObserver* observer) {
    DCHECK(observer);
    if (observer) {
      observer_ = observer;
    }
  }

 protected:
  /**
   * \brief Class constructor
   */
  CANConnection() : observer_(NULL) {}
  /**
    * @brief Implementation of pure virtual function ReadMessage should
    * receive message from CAN-integration software and forward it to RSDL
    * via CANConnectionObserver implementation member.
    * @param message Message received from CAN-integration software
    * @return current state of connection with CAN-integration software
    */
  virtual ConnectionState ReadMessage(CANMessage* message) = 0;
  CANConnectionObserver* observer_;
};

/**
 * @brief Type for smart pointer to CANConnection.
 */
typedef utils::SharedPtr<CANConnection> CANConnectionSPtr;

}  //  namespace can_cooperation

#endif  //  SRC_COMPONENTS_CAN_COOPERATION_SRC_CAN_CONNECTION_H_

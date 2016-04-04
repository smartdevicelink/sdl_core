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
#ifndef SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_H_
#define SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_H_

#include "protocol/common.h"
#include "protocol_handler/protocol_handler_settings.h"

/**
 *\namespace protocol_handlerHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {

class ProtocolObserver;
class SessionObserver;
/**
 * \class ProtocolHandler
 * \brief Interface for component parsing protocol header
 * on the messages between SDL and mobile application.
 */
class ProtocolHandler {
 public:
  /**
   * \brief Adds pointer to higher layer handler for message exchange
   * \param observer Pointer to object of the class implementing
   * IProtocolObserver
   */
  virtual void AddProtocolObserver(ProtocolObserver *observer) = 0;

  /**
   * \brief Removes pointer to higher layer handler for message exchange
   * \param observer Pointer to object of the class implementing
   * IProtocolObserver.
   */
  virtual void RemoveProtocolObserver(ProtocolObserver *observer) = 0;

  /**
   * \brief Method for sending message to Mobile Application.
   * \param message RawMessage with params to be sent to Mobile App.
   * \param final_message tells whether message's
   * connection must be closed when message is processed
   */
  virtual void SendMessageToMobileApp(const RawMessagePtr message,
                                      bool final_message) = 0;

  /**
   * \brief Sends number of processed frames in case of binary nav streaming
   * \param connection_key Id of connection over which message is to be sent
   * \param number_of_frames Number of frames processed by
   * streaming server and displayed to user.
   */
  virtual void SendFramesNumber(uint32_t connection_key,
                                int32_t number_of_frames) = 0;

  /**
   * \brief Prepare and send heartbeat message to mobile app
    * \param connection_id Identifier of connection within which
    * session exists
    * \param session_id ID of session to be ended
   */
  virtual void SendHeartBeat(int32_t connection_id, uint8_t session_id) = 0;

  /**
    * \brief Sends ending session to mobile application
    * \param connection_id Identifier of connection within which
    * session exists
    * \param session_id ID of session to be ended
    */
  virtual void SendEndSession(int32_t connection_id, uint8_t session_id) = 0;

  virtual void SendEndService(int32_t connection_id,
                              uint8_t session_id,
                              uint8_t service_type) = 0;
  /**
   * \brief Protocol handler settings getter
   * \return pointer to protocol handler settings class
   */
  virtual const ProtocolHandlerSettings& get_settings() const = 0;
  virtual SessionObserver& get_session_observer() = 0;

 protected:
  /**
   * \brief Destructor
   */
  virtual ~ProtocolHandler() {
  }
};
}  //  namespace protocol_handler
#endif  // SRC_COMPONENTS_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_H_

/**
 * \file protocol_handler.h
 * \brief ProtocolHandlerImpl class header file.
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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_IMPL_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_IMPL_H_

#include <map>

#include "utils/logger.h"
#include "utils/message_queue.h"
#include "utils/threads/thread.h"

#include "protocol_handler/protocol_handler.h"
#include "protocol_handler/protocol_packet.h"
#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/session_observer.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_listener_impl.h"

/**
 *\namespace NsProtocolHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {
class MessagesFromMobileAppHandler;
class MessagesToMobileAppHandler;

using transport_manager::TransportManagerListenerImpl;

/**
 * \class ProtocolHandlerImpl
 * \brief Class for handling message exchange between Transport and higher
 * layers. Receives message in form of array of bytes, parses its protocol,
 * handles according to parsing results (version number, start/end session etc
 * and if needed passes message to JSON Handler or notifies Connection Handler
 * about activities around sessions.
 */
class ProtocolHandlerImpl : public TransportManagerListenerImpl,
  public ProtocolHandler {
  public:
    /**
     * \brief Constructor
     * \param transportManager Pointer to Transport layer handler for
     * message exchange.
     */
    explicit ProtocolHandlerImpl(
      transport_manager::TransportManager* transport_manager);

    /**
     * \brief Destructor
     */
    ~ProtocolHandlerImpl();

    /**
     * \brief Sets pointer for higher layer handler for message exchange
     * \param observer Pointer to object of the class implementing
     * IProtocolObserver
     */
    void set_protocol_observer(ProtocolObserver* observer);

    /**
     * \brief Sets pointer for Connection Handler layer for managing sessions
     * \param observer Pointer to object of the class implementing
     * ISessionObserver
     */
    void set_session_observer(SessionObserver* observer);

    /**
     * \brief Method for sending message to Mobile Application.
     * \param message Message with params to be sent to Mobile App.
     */
    void SendMessageToMobileApp(const RawMessageSptr& message);

  protected:
    /**
     * \brief Sends fail of ending session to mobile application.
     * \param connection_handle Identifier of connection whithin which
     * session exists
     * \param session_id ID of session ment to be ended
     * \param service_type Type of session: RPC or BULK Data. RPC by default.
     */
    void SendEndSessionNAck(
      const RawMessageSptr& original_message,
      unsigned int session_id,
      unsigned char service_type = SERVICE_TYPE_RPC);

    /**
     * \brief Sends acknowledgement of starting session to mobile application
     * with session number and hash code for second version of protocol.
     * \param connection_handle Identifier of connection whithin which session
     * was started
     * \param session_id ID of session to be sent to mobile applicatin.
     * \param protocol_version Version of protocol used for communication
     * \param hash_code For second version of protocol: identifier of session
     * to be sent to
     * mobile app for using when ending session.
     * \param service_type Type of session: RPC or BULK Data. RPC by default.
     */
    void SendStartSessionAck(
      const RawMessageSptr& original_message,
      unsigned char session_id,
      unsigned char protocol_version,
      unsigned int hash_code = 0,
      unsigned char service_type = SERVICE_TYPE_RPC);

    /**
     * \brief Sends fail of starting session to mobile application.
     * \param connection_handle Identifier of connection whithin which session
     * ment to be started
     * \param service_type Type of session: RPC or BULK Data. RPC by default.
     */
    void SendStartSessionNAck(
      const RawMessageSptr& original_message,
      unsigned char service_type = SERVICE_TYPE_RPC);

  private:
    /**
     * @brief Notifies about recieving message from TM.
     *
     * @param message Recieved message
     **/
    virtual void OnTMMessageReceived(
      const RawMessageSptr& message);

    /**
     * @brief Notifies about error on receiving message from TM.
     *
     * @param error Occured error
     **/
    virtual void OnTMMessageReceiveFailed(
      const transport_manager::DataReceiveError& error);

    /**
     * @brief Notifies about successfully sending message.
     *
     **/
    virtual void OnTMMessageSend();

    /**
     * @brief Notifies about error occured during
     * sending message.
     *
     * @param error Describes occured error.
     * @param message Message during sending which error occured.
     **/
    virtual void OnTMMessageSendFailed(
      const transport_manager::DataSendError& error,
      const RawMessageSptr& message);

    /**
     * \brief Sends message which size permits to send it in one frame.
     * \param connection_handle Identifier of connection through which message
     * is to be sent.
     * \param session_id ID of session through which message is to be sent.
     * \param protocol_version Version of Protocol used in message.
     * \param service_type Type of session, RPC or BULK Data
     * \param data_size Size of message excluding protocol header
     * \param data Message string
     * \param compress Compression flag
     * \return \saRESULT_CODE Status of operation
     */
    RESULT_CODE SendSingleFrameMessage(
      const RawMessageSptr& original_message,
      const unsigned char session_id,
      unsigned int protocol_version,
      const unsigned char service_type,
      const unsigned int data_size,
      const unsigned char* data,
      const bool compress);

    /**
     * \brief Sends message which size doesn't permit to send it in one frame.
     * \param connection_handle Identifier of connection through which message
     * is to be sent.
     * \param session_id ID of session through which message is to be sent.
     * \param protocol_version Version of Protocol used in message.
     * \param service_type Type of session, RPC or BULK Data
     * \param data_size Size of message excluding protocol header
     * \param data Message string
     * \param compress Compression flag
     * \param max_data_size Maximum allowed size of single frame.
     * \return \saRESULT_CODE Status of operation
     */
    RESULT_CODE SendMultiFrameMessage(
      const RawMessageSptr& original_message,
      const unsigned char session_id,
      unsigned int protocol_version,
      const unsigned char service_type,
      const unsigned int data_size,
      const unsigned char* data,
      const bool compress,
      const unsigned int max_data_size);

    /**
     * \brief Sends message already containing protocol header.
     * \param connection_handle Identifier of connection through which message
     * is to be sent.
     * \param packet Message with protocol header.
     * \return \saRESULT_CODE Status of operation
     */
    RESULT_CODE SendFrame(
      const RawMessageSptr& original_message,
      const ProtocolPacket& packet);

    /**
     * \brief Handles received message.
     * \param connection_handle Identifier of connection through which message
     * is received.
     * \param packet Received message with protocol header.
     * \return \saRESULT_CODE Status of operation
     */
    RESULT_CODE HandleMessage(
      const RawMessageSptr& original_message,
      ProtocolPacket* packet);

    /**
     * \brief Handles message received in multiple frames. Collects all frames
     * of message.
     * \param connection_handle Identifier of connection through which message
     * is received.
     * \param packet Current frame of message with protocol header.
     * \return \saRESULT_CODE Status of operation
     */
    RESULT_CODE HandleMultiFrameMessage(
      const RawMessageSptr& original_message,
      ProtocolPacket* packet);

    /**
     * \brief Handles message received in single frame.
     * \param connection_handle Identifier of connection through which message
     * is received.
     * \param packet Received message with protocol header.
     * \return \saRESULT_CODE Status of operation
     */
    RESULT_CODE HandleControlMessage(
      const RawMessageSptr& original_message,
      const ProtocolPacket* packet);

    /**
     * \brief For logging.
     */
    static log4cxx::LoggerPtr logger_;

    /**
     *\brief Pointer on instance of class implementing IProtocolObserver
     *\brief (JSON Handler)
     */
    ProtocolObserver* protocol_observer_;

    /**
     *\brief Pointer on instance of class implementing ISessionObserver
     *\brief (Connection Handler)
     */
    SessionObserver* session_observer_;

    /**
     *\brief Pointer on instance of Transport layer handler for message exchange.
     */
    transport_manager::TransportManager* transport_manager_;

    /**
     *\brief Queue for message from Mobile side.
     */
    MessageQueue<RawMessageSptr> messages_from_mobile_app_;

    /**
     *\brief Queue for message to Mobile side.
     */
    MessageQueue<RawMessageSptr> messages_to_mobile_app_;

    /**
     *\brief Map of frames for messages received in multiple frames.
     */
    std::map<int, ProtocolPacket*> incomplete_multi_frame_messages_;

    /**
     *\brief Counter of messages sent in each session.
     */
    std::map<unsigned char, unsigned int> message_counters_;

    // Thread for handling messages from Mobile side.
    threads::Thread* handle_messages_from_mobile_app_;
    friend class MessagesFromMobileAppHandler;

    // Thread for handling message to Mobile side.
    threads::Thread* handle_messages_to_mobile_app_;
    friend class MessagesToMobileAppHandler;
};
}  // namespace protocol_handler

#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_IMPL_H_

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

#ifndef SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_IMPL_H_
#define SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_IMPL_H_

#include <map>
#include <memory>
#include <set>
#include <cstdint>
#include <utility>  // std::make_pair
#include <vector>
#include "utils/prioritized_queue.h"
#include "utils/message_queue.h"
#include "utils/threads/message_loop_thread.h"
#include "utils/shared_ptr.h"
#include "utils/messagemeter.h"

#include "protocol_handler/protocol_handler.h"
#include "protocol_handler/protocol_packet.h"
#include "protocol_handler/protocol_handler_settings.h"
#include "protocol_handler/session_observer.h"
#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/incoming_data_handler.h"
#include "protocol_handler/multiframe_builder.h"
#include "transport_manager/common.h"
#include "transport_manager/transport_manager.h"
#include "transport_manager/transport_manager_listener_empty.h"
#include "connection_handler/connection_handler.h"

#ifdef TELEMETRY_MONITOR
#include "protocol_handler/telemetry_observer.h"
#include "telemetry_monitor/telemetry_observable.h"
#endif  // TELEMETRY_MONITOR

#ifdef ENABLE_SECURITY
#include "security_manager/security_manager.h"
#endif  // ENABLE_SECURITY

namespace connection_handler {
class ConnectionHandlerImpl;
}  // namespace connection_handler

/**
 *\namespace protocol_handlerHandler
 *\brief Namespace for SmartDeviceLink ProtocolHandler related functionality.
 */
namespace protocol_handler {
class ProtocolObserver;
class SessionObserver;

class MessagesFromMobileAppHandler;
class MessagesToMobileAppHandler;

using transport_manager::TransportManagerListenerEmpty;

typedef std::multimap<int32_t, RawMessagePtr> MessagesOverNaviMap;
typedef std::set<ProtocolObserver*> ProtocolObservers;
typedef transport_manager::ConnectionUID ConnectionID;

namespace impl {
/*
 * These dummy classes are here to locally impose strong typing on different
 * kinds of messages
 * Currently there is no type difference between incoming and outgoing messages
 * TODO(ik): replace these with globally defined message types
 * when we have them.
 */
struct RawFordMessageFromMobile: public ProtocolFramePtr {
  RawFordMessageFromMobile() {}
  explicit RawFordMessageFromMobile(const ProtocolFramePtr message)
    : ProtocolFramePtr(message) {}
  // PrioritizedQueue requires this method to decide which priority to assign
  size_t PriorityOrder() const {
    return MessagePriority::FromServiceType(
             ServiceTypeFromByte(get()->service_type())).OrderingValue();
  }
};

struct RawFordMessageToMobile: public ProtocolFramePtr {
  RawFordMessageToMobile() : is_final(false) {}
  explicit RawFordMessageToMobile(const ProtocolFramePtr message,
                                  bool final_message)
    : ProtocolFramePtr(message), is_final(final_message) {}
  // PrioritizedQueue requires this method to decide which priority to assign
  size_t PriorityOrder() const {
    return MessagePriority::FromServiceType(
             ServiceTypeFromByte(get()->service_type())).OrderingValue();
  }
  // Signals whether connection to mobile must be closed after processing this message
  bool is_final;
};

// Short type names for prioritized message queues
typedef threads::MessageLoopThread <
  utils::PrioritizedQueue<RawFordMessageFromMobile> > FromMobileQueue;
typedef threads::MessageLoopThread <
  utils::PrioritizedQueue<RawFordMessageToMobile> > ToMobileQueue;
}  // namespace impl

/**
 * \class ProtocolHandlerImpl
 * \brief Class for handling message exchange between Transport and higher
 * layers. Receives message in form of array of bytes, parses its protocol,
 * handles according to parsing results (version number, start/end session etc
 * and if needed passes message to JSON Handler or notifies Connection Handler
 * about activities around sessions.
 */
class ProtocolHandlerImpl
  : public ProtocolHandler,
    public TransportManagerListenerEmpty,
    public impl::FromMobileQueue::Handler,
    public impl::ToMobileQueue::Handler
#ifdef TELEMETRY_MONITOR
    ,
    public telemetry_monitor::TelemetryObservable<PHTelemetryObserver>
#endif  // TELEMETRY_MONITOR
    {
 public:
  /**
   * @brief Constructor
   * @param settings reference to ProtocolHandlerSettingsImpl object
   * @param session_observer reference to SessionObserver to access session
   * information and controll session life cycle
   * @param transportManager Pointer to Transport layer handler for
   */
    ProtocolHandlerImpl(const ProtocolHandlerSettings& settings,
                        protocol_handler::SessionObserver& session_observer,
                        connection_handler::ConnectionHandler& connection_handler,
                        transport_manager::TransportManager& transport_manager);
  /**
   * \brief Destructor
   */
  ~ProtocolHandlerImpl();

  /**
   * \brief Adds pointer to higher layer handler for message exchange
   * \param observer Pointer to object of the class implementing
   * IProtocolObserver
   */
  void AddProtocolObserver(ProtocolObserver *observer);

  void RemoveProtocolObserver(
      ProtocolObserver* observer) OVERRIDE;

#ifdef ENABLE_SECURITY
  /**
   * \brief Sets pointer for SecurityManager layer for managing protection routine
   * \param security_manager Pointer to SecurityManager object
   */
  void set_security_manager(security_manager::SecurityManager *security_manager);
#endif  // ENABLE_SECURITY

  /**
   * \brief Stop all handling activity
   */
  void Stop();

  /**
   * \brief Method for sending message to Mobile Application
   * \param message Message with params to be sent to Mobile App
   */
  void SendMessageToMobileApp(const RawMessagePtr message,
                              bool final_message) OVERRIDE;

  /**
   * \brief Sends number of processed frames in case of binary nav streaming
   * \param connection_key Unique key used by other components as session identifier
   * \param number_of_frames Number of frames processed by
   * streaming server and displayed to user.
   */
  void SendFramesNumber(uint32_t connection_key, int32_t number_of_frames);

#ifdef TELEMETRY_MONITOR
  /**
   * @brief Setup observer for time metric.
   *
   * @param observer - pointer to observer
   */
  void SetTelemetryObserver(PHTelemetryObserver *observer);
#endif  // TELEMETRY_MONITOR

  /*
   * Prepare and send heartbeat message to mobile
   */
  void SendHeartBeat(int32_t connection_id, uint8_t session_id);

  /**
    * \brief Sends ending session to mobile application
    * \param connection_id Identifier of connection within which
    * session exists
    * \param session_id ID of session to be ended
    */
  void SendEndSession(int32_t connection_id, uint8_t session_id);

  void SendEndService(int32_t connection_id,
                      uint8_t session_id,
                      uint8_t service_type);

  // TODO(Ezamakhov): move Ack/Nack as interface for StartSessionHandler
  /**
   * \brief Sends acknowledgement of starting session to mobile application
   * with session number and hash code for second version of protocol
   * was started
   * \param connection_id Identifier of connection within which session
   * \param session_id ID of session to be sent to mobile application
   * \param protocol_version Version of protocol used for communication
   * \param hash_code For second version of protocol: identifier of session
   * to be sent to
   * mobile app for using when ending session
   * \param service_type Type of session: RPC or BULK Data. RPC by default
   * \param protection Protection flag
   */
  void SendStartSessionAck(ConnectionID connection_id,
                           uint8_t session_id,
                           uint8_t protocol_version,
                           uint32_t hash_code,
                           uint8_t service_type,
                           bool protection);

  const ProtocolHandlerSettings& get_settings() const OVERRIDE {
    return settings_;
  }

  /**
   * \brief Sends fail of starting session to mobile application
   * \param connection_id Identifier of connection within which session
   * \param session_id ID of session to be sent to mobile application
   * \param protocol_version Version of protocol used for communication
   * \param service_type Type of session: RPC or BULK Data. RPC by default
   */
  void SendStartSessionNAck(ConnectionID connection_id,
                            uint8_t session_id,
                            uint8_t protocol_version,
                            uint8_t service_type);

  /**
   * \brief Sends acknowledgement of end session/service to mobile application
   * with session number for second version of protocol.
   * \param connection_id Identifier of connection
   * \param connection_handle Identifier of connection within which session
   * was started
   * \param session_id ID of session to be sent to mobile application
   * \param protocol_version Version of protocol used for communication
   * mobile app for using when ending session.
   * \param service_type Type of session: RPC or BULK Data. RPC by default
   */
  void SendEndSessionAck(ConnectionID connection_id,
                         uint8_t session_id,
                         uint8_t protocol_version,
                         uint8_t service_type);

  /**
   * \brief Sends fail of ending session to mobile application
   * \param connection_id Identifier of connection within which
   * session exists
   * \param session_id ID of session ment to be ended
   * \param protocol_version Version of protocol used for communication
   * \param service_type Type of session: RPC or BULK Data. RPC by default
   */
  void SendEndSessionNAck(ConnectionID connection_id,
                          uint32_t session_id,
                          uint8_t protocol_version,
                          uint8_t service_type);

 private:
  void SendEndServicePrivate(int32_t connection_id, uint8_t session_id,
                             uint8_t service_type);

  /*
   * Prepare and send heartbeat acknowledge message
   */
  RESULT_CODE SendHeartBeatAck(ConnectionID connection_id,
                               uint8_t session_id,
                               uint32_t message_id);

  /**
   * @brief Notifies about receiving message from TM.
   *
   * @param message Received message
   **/
  void OnTMMessageReceived(
    const RawMessagePtr message) OVERRIDE;

  /**
   * @brief Notifies about error on receiving message from TM.
   *
   * @param error Occurred error
   **/
  void OnTMMessageReceiveFailed(
    const transport_manager::DataReceiveError &error) OVERRIDE;

  /**
   * @brief Notifies about successfully sending message.
   *
   **/
  void OnTMMessageSend(const RawMessagePtr message) OVERRIDE;

  /**
   * @brief Notifies about error occurred during
   * sending message.
   *
   * @param error Describes occurred error.
   * @param message Message during sending which error occurred.
   **/
  void OnTMMessageSendFailed(
    const transport_manager::DataSendError &error,
    const RawMessagePtr message) OVERRIDE;

  void OnConnectionEstablished(const transport_manager::DeviceInfo &device_info,
    const transport_manager::ConnectionUID connection_id) OVERRIDE;

  void OnConnectionClosed(
    const transport_manager::ConnectionUID connection_id) OVERRIDE;

  /**
   * @brief Notifies subscribers about message
   * received from mobile device.
   * @param message Message with already parsed header.
   */
  void NotifySubscribers(const RawMessagePtr message);

  /**
   * \brief Sends message which size permits to send it in one frame.
   * \param connection_handle Identifier of connection through which message
   * is to be sent.
   * \param session_id ID of session through which message is to be sent.
   * \param protocol_version Version of Protocol used in message.
   * \param service_type Type of session, RPC or BULK Data
   * \param data_size Size of message excluding protocol header
   * \param data Message string
   * \param is_final_message if is_final_message = true - it is last message
   * \return \saRESULT_CODE Status of operation
   */
  RESULT_CODE SendSingleFrameMessage(const ConnectionID connection_id,
                                     const uint8_t session_id,
                                     const uint32_t protocol_version,
                                     const uint8_t service_type,
                                     const size_t data_size,
                                     const uint8_t *data,
                                     const bool is_final_message);

  /**
   * \brief Sends message which size doesn't permit to send it in one frame.
   * \param connection_handle Identifier of connection through which message
   * is to be sent.
   * \param session_id ID of session through which message is to be sent.
   * \param protocol_version Version of Protocol used in message.
   * \param service_type Type of session, RPC or BULK Data
   * \param data_size Size of message excluding protocol header
   * \param data Message string
   * \param max_data_size Maximum allowed size of single frame.
   * \param is_final_message if is_final_message = true - it is last message
   * \return \saRESULT_CODE Status of operation
   */
  RESULT_CODE SendMultiFrameMessage(const ConnectionID connection_id,
                                    const uint8_t session_id,
                                    const uint8_t protocol_version,
                                    const uint8_t service_type,
                                    const size_t data_size,
                                    const uint8_t *data,
                                    const size_t max_frame_size,
                                    const bool is_final_message);

  /**
   * \brief Sends message already containing protocol header.
   * \param packet Message with protocol header
   * \return \saRESULT_CODE Status of operation
   */
  RESULT_CODE SendFrame(const ProtocolFramePtr packet);

  /**
   * \brief Handles received message.
   * \param connection_handle Identifier of connection through which message
   * is received.
   * \param packet Received message with protocol header.
   * \return \saRESULT_CODE Status of operation
   */
  RESULT_CODE HandleMessage(const ProtocolFramePtr packet);

  /**
   * \brief Handles message received in single frame.
   * \param connection_handle Identifier of connection through which message
   * is received.
   * \param packet Frame of message with protocol header.
   * \return \saRESULT_CODE Status of operation
   */
  RESULT_CODE HandleSingleFrameMessage(const ProtocolFramePtr packet);
  /**
   * \brief Handles message received in multiple frames. Collects all frames
   * of message.
   * \param connection_handle Identifier of connection through which message
   * is received.
   * \param packet Current frame of message with protocol header.
   * \return \saRESULT_CODE Status of operation
   */
  RESULT_CODE HandleMultiFrameMessage(const ProtocolFramePtr packet);

  /**
   * \brief Handles message received in single frame.
   * \param connection_handle Identifier of connection through which message
   * is received.
   * \param packet Received message with protocol header.
   * \return \saRESULT_CODE Status of operation
   */
  RESULT_CODE HandleControlMessage(const ProtocolFramePtr packet);

  RESULT_CODE HandleControlMessageEndSession(const ProtocolPacket& packet);

  RESULT_CODE HandleControlMessageEndServiceACK(const ProtocolPacket& packet);

  RESULT_CODE HandleControlMessageStartSession(const ProtocolPacket& packet);

  RESULT_CODE HandleControlMessageHeartBeat(const ProtocolPacket& packet);

  void PopValideAndExpirateMultiframes();

  // threads::MessageLoopThread<*>::Handler implementations
  // CALLED ON raw_ford_messages_from_mobile_ thread!
  void Handle(const impl::RawFordMessageFromMobile message);
  // CALLED ON raw_ford_messages_to_mobile_ thread!
  void Handle(const impl::RawFordMessageToMobile message);

#ifdef ENABLE_SECURITY
  /**
   * \brief Encryption/Decryption methodes for SecureSecvice check
   * \param packet frame of message to encrypted/decrypted
   */
  RESULT_CODE EncryptFrame(ProtocolFramePtr packet);
  RESULT_CODE DecryptFrame(ProtocolFramePtr packet);
#endif  // ENABLE_SECURITY

  bool TrackMessage(const uint32_t &connection_key);

  bool TrackMalformedMessage(const uint32_t &connection_key,
                             const size_t count);
  /**
   * @brief Function returns supported SDL Protocol Version,
   */
  uint8_t SupportedSDLProtocolVersion() const;

  const ProtocolHandlerSettings& settings_;

  /**
   *\brief Pointer on instance of class implementing IProtocolObserver
   *\brief (JSON Handler)
   */
  ProtocolObservers protocol_observers_;

  /**
   *\brief Pointer on instance of class implementing ISessionObserver
   *\brief (Connection Handler).
   * According to usage it can't be null
   */
  SessionObserver& session_observer_;

  /**
   *\brief Pointer on instance of Transport layer handler for message exchange.
   *\brief Instance of class implementing Connection  Handler t keep connection
   *alive
   */
  connection_handler::ConnectionHandler& connection_handler_;

  /**
   *\brief Pointer on instance of Transport layer handler for message exchange.
   */
  transport_manager::TransportManager& transport_manager_;

  /**
   *\brief Assembling support class.
   */
  MultiFrameBuilder multiframe_builder_;

  /**
   * \brief Map of messages (frames) received over mobile nave session
   * for map streaming.
   */
  MessagesOverNaviMap message_over_navi_session_;

  /**
   * \brief Untill specified otherwise, amount of message recievied
   * over streaming session to send Ack
   */
  const uint32_t kPeriodForNaviAck;

  /**
   *\brief Counter of messages sent in each session.
   * Used ad unique message identifier
   */
  std::map<uint8_t, uint32_t> message_counters_;

  /**
   *\brief Counter of messages sent in each session.
   */
  std::map<ConnectionID, uint32_t> malformed_message_counters_;

  /**
   *\brief map for session last message.
   */
  std::map<uint8_t, uint32_t> sessions_last_message_id_;

  /**
   *\brief Connections that must be closed after their last messages were sent
   */
  std::vector<uint32_t> ready_to_close_connections_;

  ProtocolPacket::ProtocolHeaderValidator protocol_header_validator_;
  IncomingDataHandler incoming_data_handler_;
  // Use uint32_t as application identifier
  utils::MessageMeter<uint32_t> message_meter_;
  // Use uint32_t as connection identifier
  utils::MessageMeter<uint32_t> malformed_message_meter_;

#ifdef ENABLE_SECURITY
  security_manager::SecurityManager *security_manager_;
#endif  // ENABLE_SECURITY

  // Thread that pumps non-parsed messages coming from mobile side.
  impl::FromMobileQueue raw_ford_messages_from_mobile_;
  // Thread that pumps messages prepared to being sent to mobile side.
  impl::ToMobileQueue raw_ford_messages_to_mobile_;

  sync_primitives::Lock protocol_observers_lock_;

#ifdef TELEMETRY_MONITOR
  PHTelemetryObserver *metric_observer_;
#endif  // TELEMETRY_MONITOR
};
}  // namespace protocol_handler
#endif  // SRC_COMPONENTS_PROTOCOL_HANDLER_INCLUDE_PROTOCOL_HANDLER_PROTOCOL_HANDLER_IMPL_H_

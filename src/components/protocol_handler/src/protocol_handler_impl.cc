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

#include "protocol_handler/protocol_handler_impl.h"
#include <memory.h>
#include <algorithm>  // std::find

#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/session_observer.h"
#include "utils/byte_order.h"
#include "protocol/common.h"

#ifdef ENABLE_SECURITY
#include "security_manager/ssl_context.h"
#include "security_manager/security_manager.h"
#endif  // ENABLE_SECURITY

namespace protocol_handler {

CREATE_LOGGERPTR_GLOBAL(logger_, "ProtocolHandler")

/**
 * Function return packet data as std::string.
 * If packet data is not printable return error message
 */
std::string ConvertPacketDataToString(const uint8_t* data,
                                      const size_t data_size);

const size_t kStackSize = 32768;

ProtocolHandlerImpl::ProtocolHandlerImpl(
    const ProtocolHandlerSettings& settings,
    protocol_handler::SessionObserver& session_observer,
    connection_handler::ConnectionHandler& connection_handler,
    transport_manager::TransportManager& transport_manager)
    : settings_(settings)
    , protocol_observers_()
    , session_observer_(session_observer)
    , connection_handler_(connection_handler)
    , transport_manager_(transport_manager)
    , kPeriodForNaviAck(5)
    ,
#ifdef ENABLE_SECURITY
    security_manager_(NULL)
    ,
#endif  // ENABLE_SECURITY
    raw_ford_messages_from_mobile_(
        "PH FromMobile", this, threads::ThreadOptions(kStackSize))
    , raw_ford_messages_to_mobile_(
          "PH ToMobile", this, threads::ThreadOptions(kStackSize))
#ifdef TELEMETRY_MONITOR
    , metric_observer_(NULL)
    ,
#endif  // TELEMETRY_MONITOR
    ptu_state_(kNotTriggered) {
  LOG4CXX_AUTO_TRACE(logger_);
  protocol_header_validator_.set_max_payload_size(
      get_settings().maximum_payload_size());
  incoming_data_handler_.set_validator(&protocol_header_validator_);

  const size_t& message_frequency_count =
      get_settings().message_frequency_count();
  const size_t& message_frequency_time =
      get_settings().message_frequency_time();

  if (message_frequency_time > 0u && message_frequency_count > 0u) {
    message_meter_.set_time_range(message_frequency_time);
    LOG4CXX_DEBUG(logger_,
                  "Frequency meter is enabled ( "
                      << message_frequency_count << " per "
                      << message_frequency_time << " mSecond)");
  } else {
    LOG4CXX_WARN(logger_, "Frequency meter is disabled");
  }

  const size_t& malformed_frequency_time =
      get_settings().malformed_frequency_time();
  const size_t& malformed_frequency_count =
      get_settings().message_frequency_time();

  if (get_settings().malformed_message_filtering()) {
    if (malformed_frequency_time > 0u && malformed_frequency_count > 0u) {
      malformed_message_meter_.set_time_range(malformed_frequency_time);
      LOG4CXX_DEBUG(logger_,
                    "Malformed frequency meter is enabled ( "
                        << malformed_frequency_count << " per "
                        << malformed_frequency_time << " mSecond)");
    } else {
      LOG4CXX_WARN(logger_, "Malformed frequency meter is disabled");
    }
  } else {
    LOG4CXX_WARN(
        logger_,
        "Malformed message filtering is disabled."
            << "Connection will be close on first malformed message detection");
  }
  multiframe_builder_.set_waiting_timeout(
      get_settings().multiframe_waiting_timeout());
}

ProtocolHandlerImpl::~ProtocolHandlerImpl() {
  sync_primitives::AutoLock lock(protocol_observers_lock_);
  if (!protocol_observers_.empty()) {
    LOG4CXX_WARN(logger_,
                 "Not all observers have unsubscribed"
                 " from ProtocolHandlerImpl");
  }
}

void ProtocolHandlerImpl::AddProtocolObserver(ProtocolObserver* observer) {
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to IProtocolObserver.");
    return;
  }
  sync_primitives::AutoLock lock(protocol_observers_lock_);
  protocol_observers_.insert(observer);
}

void ProtocolHandlerImpl::RemoveProtocolObserver(ProtocolObserver* observer) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to IProtocolObserver.");
    return;
  }
  sync_primitives::AutoLock lock(protocol_observers_lock_);
  protocol_observers_.erase(observer);
}

void set_hash_id(uint32_t hash_id, protocol_handler::ProtocolPacket& packet) {
  if (HASH_ID_NOT_SUPPORTED == hash_id || HASH_ID_WRONG == hash_id) {
    return;
  }
  if (packet.protocol_version() < PROTOCOL_VERSION_2) {
    LOG4CXX_DEBUG(logger_,
                  "Packet needs no hash data (protocol version less 2)");
    return;
  }
  LOG4CXX_DEBUG(logger_,
                "Set hash_id 0x" << std::hex << hash_id << " to the packet 0x"
                                 << &packet);
  // Hash id shall be 4 bytes according Ford Protocol v8
  DCHECK(sizeof(hash_id) == 4);
  const uint32_t hash_id_be = LE_TO_BE32(hash_id);
  packet.set_data(reinterpret_cast<const uint8_t*>(&hash_id_be),
                  sizeof(hash_id_be));
}

void ProtocolHandlerImpl::SendStartSessionAck(ConnectionID connection_id,
                                              uint8_t session_id,
                                              uint8_t,
                                              uint32_t hash_id,
                                              uint8_t service_type,
                                              bool protection) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint8_t protocolVersion = SupportedSDLProtocolVersion();

  ProtocolFramePtr ptr(
      new protocol_handler::ProtocolPacket(connection_id,
                                           protocolVersion,
                                           protection,
                                           FRAME_TYPE_CONTROL,
                                           service_type,
                                           FRAME_DATA_START_SERVICE_ACK,
                                           session_id,
                                           0u,
                                           message_counters_[session_id]++));

  set_hash_id(hash_id, *ptr);

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_DEBUG(logger_,
                "SendStartSessionAck() for connection "
                    << connection_id << " for service_type "
                    << static_cast<int32_t>(service_type) << " session_id "
                    << static_cast<int32_t>(session_id) << " protection "
                    << (protection ? "ON" : "OFF"));
}

void ProtocolHandlerImpl::SendStartSessionNAck(ConnectionID connection_id,
                                               uint8_t session_id,
                                               uint8_t protocol_version,
                                               uint8_t service_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  ProtocolFramePtr ptr(
      new protocol_handler::ProtocolPacket(connection_id,
                                           protocol_version,
                                           PROTECTION_OFF,
                                           FRAME_TYPE_CONTROL,
                                           service_type,
                                           FRAME_DATA_START_SERVICE_NACK,
                                           session_id,
                                           0u,
                                           message_counters_[session_id]++));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_DEBUG(logger_,
                "SendStartSessionNAck() for connection "
                    << connection_id << " for service_type "
                    << static_cast<int32_t>(service_type) << " session_id "
                    << static_cast<int32_t>(session_id));
}

void ProtocolHandlerImpl::SendEndSessionNAck(ConnectionID connection_id,
                                             uint32_t session_id,
                                             uint8_t protocol_version,
                                             uint8_t service_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  ProtocolFramePtr ptr(
      new protocol_handler::ProtocolPacket(connection_id,
                                           protocol_version,
                                           PROTECTION_OFF,
                                           FRAME_TYPE_CONTROL,
                                           service_type,
                                           FRAME_DATA_END_SERVICE_NACK,
                                           session_id,
                                           0u,
                                           message_counters_[session_id]++));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_DEBUG(logger_,
                "SendEndSessionNAck() for connection "
                    << connection_id << " for service_type "
                    << static_cast<int32_t>(service_type) << " session_id "
                    << static_cast<int32_t>(session_id));
}

SessionObserver& ProtocolHandlerImpl::get_session_observer() {
  return session_observer_;
}

void ProtocolHandlerImpl::SendEndSessionAck(ConnectionID connection_id,
                                            uint8_t session_id,
                                            uint8_t protocol_version,
                                            uint8_t service_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  ProtocolFramePtr ptr(
      new protocol_handler::ProtocolPacket(connection_id,
                                           protocol_version,
                                           PROTECTION_OFF,
                                           FRAME_TYPE_CONTROL,
                                           service_type,
                                           FRAME_DATA_END_SERVICE_ACK,
                                           session_id,
                                           0u,
                                           message_counters_[session_id]++));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_DEBUG(logger_,
                "SendEndSessionAck() for connection "
                    << connection_id << " for service_type "
                    << static_cast<int32_t>(service_type) << " session_id "
                    << static_cast<int32_t>(session_id));
}

void ProtocolHandlerImpl::SendEndServicePrivate(int32_t connection_id,
                                                uint8_t session_id,
                                                uint8_t service_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          connection_id, session_id, protocol_version)) {
    ProtocolFramePtr ptr(
        new protocol_handler::ProtocolPacket(connection_id,
                                             protocol_version,
                                             PROTECTION_OFF,
                                             FRAME_TYPE_CONTROL,
                                             service_type,
                                             FRAME_DATA_END_SERVICE,
                                             session_id,
                                             0,
                                             message_counters_[session_id]++));

    raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, false));
    LOG4CXX_DEBUG(logger_,
                  "SendEndSession() for connection "
                      << connection_id << " for service_type " << service_type
                      << " session_id " << static_cast<int32_t>(session_id));
  } else {
    LOG4CXX_WARN(
        logger_,
        "SendEndSession is failed connection or session does not exist");
  }
}

void ProtocolHandlerImpl::SendEndSession(int32_t connection_id,
                                         uint8_t session_id) {
  SendEndServicePrivate(connection_id, session_id, SERVICE_TYPE_RPC);
}

void ProtocolHandlerImpl::SendEndService(int32_t connection_id,
                                         uint8_t session_id,
                                         uint8_t service_type) {
  SendEndServicePrivate(connection_id, session_id, service_type);
}

RESULT_CODE ProtocolHandlerImpl::SendHeartBeatAck(ConnectionID connection_id,
                                                  uint8_t session_id,
                                                  uint32_t message_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          connection_id, session_id, protocol_version)) {
    ProtocolFramePtr ptr(
        new protocol_handler::ProtocolPacket(connection_id,
                                             protocol_version,
                                             PROTECTION_OFF,
                                             FRAME_TYPE_CONTROL,
                                             SERVICE_TYPE_CONTROL,
                                             FRAME_DATA_HEART_BEAT_ACK,
                                             session_id,
                                             0u,
                                             message_id));

    raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, false));
    return RESULT_OK;
  }
  LOG4CXX_WARN(
      logger_,
      "SendHeartBeatAck is failed connection or session does not exist");
  return RESULT_FAIL;
}

void ProtocolHandlerImpl::SendHeartBeat(int32_t connection_id,
                                        uint8_t session_id) {
  LOG4CXX_AUTO_TRACE(logger_);
  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          connection_id, session_id, protocol_version)) {
    ProtocolFramePtr ptr(
        new protocol_handler::ProtocolPacket(connection_id,
                                             protocol_version,
                                             PROTECTION_OFF,
                                             FRAME_TYPE_CONTROL,
                                             SERVICE_TYPE_CONTROL,
                                             FRAME_DATA_HEART_BEAT,
                                             session_id,
                                             0u,
                                             message_counters_[session_id]++));
    raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, false));
    LOG4CXX_DEBUG(logger_, "SendHeartBeat finished successfully");
  } else {
    LOG4CXX_WARN(
        logger_,
        "SendHeartBeat is failed connection or session does not exist");
  }
}

void ProtocolHandlerImpl::SendMessageToMobileApp(const RawMessagePtr message,
                                                 bool final_message) {
#ifdef TELEMETRY_MONITOR
  const TimevalStruct start_time = date_time::DateTime::getCurrentTime();
#endif  // TELEMETRY_MONITOR
  LOG4CXX_AUTO_TRACE(logger_);
  if (!message) {
    LOG4CXX_ERROR(logger_,
                  "Invalid message for sending to mobile app is received.");
    return;
  }

  uint32_t connection_handle = 0;
  uint8_t sessionID = 0;
  session_observer_.PairFromKey(
      message->connection_key(), &connection_handle, &sessionID);
#ifdef TELEMETRY_MONITOR
  uint32_t message_id = message_counters_[sessionID];
  if (metric_observer_) {
    metric_observer_->StartMessageProcess(message_id, start_time);
  }
#endif  // TELEMETRY_MONITOR
  const size_t max_frame_size = get_settings().maximum_payload_size();
  size_t frame_size = MAXIMUM_FRAME_DATA_V2_SIZE;
  switch (message->protocol_version()) {
    case PROTOCOL_VERSION_3:
    case PROTOCOL_VERSION_4:
      frame_size = max_frame_size > MAXIMUM_FRAME_DATA_V2_SIZE
                       ? max_frame_size
                       : MAXIMUM_FRAME_DATA_V2_SIZE;
      break;
    default:
      break;
  }
#ifdef ENABLE_SECURITY
  const security_manager::SSLContext* ssl_context =
      session_observer_.GetSSLContext(message->connection_key(),
                                      message->service_type());
  if (ssl_context && ssl_context->IsInitCompleted()) {
    const size_t max_block_size = ssl_context->get_max_block_size(frame_size);
    DCHECK(max_block_size > 0);
    if (max_block_size > 0) {
      frame_size = max_block_size;
      LOG4CXX_DEBUG(logger_,
                    "Security set new optimal packet size " << frame_size);
    } else {
      LOG4CXX_ERROR(
          logger_,
          "Security could not return max block size, use the origin one");
    }
  }
  LOG4CXX_DEBUG(logger_, "Optimal packet size is " << frame_size);
#endif  // ENABLE_SECURITY

  if (message->data_size() <= frame_size) {
    RESULT_CODE result = SendSingleFrameMessage(connection_handle,
                                                sessionID,
                                                message->protocol_version(),
                                                message->service_type(),
                                                message->data_size(),
                                                message->data(),
                                                final_message);
    if (result != RESULT_OK) {
      LOG4CXX_ERROR(logger_,
                    "ProtocolHandler failed to send single frame message.");
    }
  } else {
    LOG4CXX_DEBUG(logger_,
                  "Message will be sent in multiple frames; max frame size is "
                      << frame_size);

    RESULT_CODE result = SendMultiFrameMessage(connection_handle,
                                               sessionID,
                                               message->protocol_version(),
                                               message->service_type(),
                                               message->data_size(),
                                               message->data(),
                                               frame_size,
                                               final_message);
    if (result != RESULT_OK) {
      LOG4CXX_ERROR(logger_,
                    "ProtocolHandler failed to send multiframe messages.");
    }
  }
#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    PHTelemetryObserver::MessageMetric* metric =
        new PHTelemetryObserver::MessageMetric();
    metric->message_id = message_id;
    metric->connection_key = message->connection_key();
    metric->raw_msg = message;
    metric_observer_->EndMessageProcess(metric);
  }
#endif
}

void ProtocolHandlerImpl::OnTMMessageReceived(const RawMessagePtr tm_message) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (!tm_message) {
    LOG4CXX_ERROR(logger_,
                  "Invalid incoming message received in"
                      << " ProtocolHandler from Transport Manager.");
    return;
  }

  const uint32_t connection_key = tm_message->connection_key();
  LOG4CXX_DEBUG(logger_,
                "Received data from TM  with connection id "
                    << connection_key << " msg data_size "
                    << tm_message->data_size());

  RESULT_CODE result;
  size_t malformed_occurs = 0u;
  const ProtocolFramePtrList protocol_frames =
      incoming_data_handler_.ProcessData(
          *tm_message, &result, &malformed_occurs);
  LOG4CXX_DEBUG(logger_, "Proccessed " << protocol_frames.size() << " frames");
  if (result != RESULT_OK) {
    if (result == RESULT_MALFORMED_OCCURS) {
      LOG4CXX_WARN(logger_,
                   "Malformed message occurs, connection id "
                       << connection_key);
      if (!get_settings().malformed_message_filtering()) {
        LOG4CXX_DEBUG(logger_, "Malformed message filterign disabled");
        session_observer_.OnMalformedMessageCallback(connection_key);
        // For tracking only malformed occurrence check outpute
      } else {
        if (malformed_occurs > 0) {
          TrackMalformedMessage(connection_key, malformed_occurs);
        }
      }
    } else {
      LOG4CXX_ERROR(logger_, "Incoming data processing failed.");
      transport_manager_.DisconnectForce(connection_key);
    }
  }

  for (ProtocolFramePtrList::const_iterator it = protocol_frames.begin();
       it != protocol_frames.end();
       ++it) {
#ifdef TELEMETRY_MONITOR
    const TimevalStruct start_time = date_time::DateTime::getCurrentTime();
#endif  // TELEMETRY_MONITOR
    ProtocolFramePtr frame = *it;
#ifdef ENABLE_SECURITY
    const RESULT_CODE result = DecryptFrame(frame);
    if (result != RESULT_OK) {
      LOG4CXX_WARN(logger_, "Error frame decryption. Frame skipped.");
      continue;
    }
#endif  // ENABLE_SECURITY
    impl::RawFordMessageFromMobile msg(frame);
#ifdef TELEMETRY_MONITOR
    if (metric_observer_) {
      metric_observer_->StartMessageProcess(msg->message_id(), start_time);
    }
#endif  // TELEMETRY_MONITOR

    raw_ford_messages_from_mobile_.PostMessage(msg);
  }
}

void ProtocolHandlerImpl::OnTMMessageReceiveFailed(
    const transport_manager::DataReceiveError& error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Received error on attemping to recieve message.");
}

void ProtocolHandlerImpl::NotifySubscribers(const RawMessagePtr message) {
  LOG4CXX_AUTO_TRACE(logger_);
  sync_primitives::AutoLock lock(protocol_observers_lock_);
  if (protocol_observers_.empty()) {
    LOG4CXX_ERROR(
        logger_,
        "Cannot handle multiframe message: no IProtocolObserver is set.");
  }
  for (ProtocolObservers::iterator it = protocol_observers_.begin();
       protocol_observers_.end() != it;
       ++it) {
    ProtocolObserver* observe = *it;
    observe->OnMessageReceived(message);
  }
}

void ProtocolHandlerImpl::OnTMMessageSend(const RawMessagePtr message) {
  LOG4CXX_DEBUG(logger_, "Sending message finished successfully.");

  uint32_t connection_handle = 0;
  uint8_t sessionID = 0;

  session_observer_.PairFromKey(
      message->connection_key(), &connection_handle, &sessionID);

  std::vector<uint32_t>::iterator connection_it =
      std::find(ready_to_close_connections_.begin(),
                ready_to_close_connections_.end(),
                connection_handle);

  if (ready_to_close_connections_.end() != connection_it) {
    ready_to_close_connections_.erase(connection_it);
    transport_manager_.Disconnect(connection_handle);
    return;
  }

  ProtocolPacket sent_message(message->connection_key());
  const RESULT_CODE result =
      sent_message.deserializePacket(message->data(), message->data_size());
  if (result != RESULT_OK) {
    LOG4CXX_ERROR(logger_, "Error while message deserialization.");
    return;
  }
  std::map<uint8_t, uint32_t>::iterator it =
      sessions_last_message_id_.find(sent_message.session_id());

  if (sessions_last_message_id_.end() != it) {
    uint32_t last_message_id = it->second;
    sessions_last_message_id_.erase(it);
    if ((sent_message.message_id() == last_message_id) &&
        ((FRAME_TYPE_SINGLE == sent_message.frame_type()) ||
         ((FRAME_TYPE_CONSECUTIVE == sent_message.frame_type()) &&
          (0 == sent_message.frame_data())))) {
      ready_to_close_connections_.push_back(connection_handle);
      SendEndSession(connection_handle, sent_message.session_id());
    }
  }
  sync_primitives::AutoLock lock(protocol_observers_lock_);
  for (ProtocolObservers::iterator it = protocol_observers_.begin();
       protocol_observers_.end() != it;
       ++it) {
    (*it)->OnMobileMessageSent(message);
  }
}

void ProtocolHandlerImpl::OnTMMessageSendFailed(
    const transport_manager::DataSendError& error,
    const RawMessagePtr message) {
  DCHECK_OR_RETURN_VOID(message);
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_,
                "Sending message " << message->data_size()
                                   << "bytes failed, connection_key "
                                   << message->connection_key()
                                   << "Error_text: " << error.text());
}

void ProtocolHandlerImpl::OnConnectionEstablished(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectionUID connection_id) {
  incoming_data_handler_.AddConnection(connection_id);
  multiframe_builder_.AddConnection(connection_id);
}

void ProtocolHandlerImpl::OnConnectionClosed(
    const transport_manager::ConnectionUID connection_id) {
  incoming_data_handler_.RemoveConnection(connection_id);
  message_meter_.ClearIdentifiers();
  malformed_message_meter_.ClearIdentifiers();
  multiframe_builder_.RemoveConnection(connection_id);
}

RESULT_CODE ProtocolHandlerImpl::SendFrame(const ProtocolFramePtr packet) {
  LOG4CXX_AUTO_TRACE(logger_);
  if (!packet) {
    LOG4CXX_ERROR(logger_, "Failed to send empty packet.");
    return RESULT_FAIL;
  }
#ifdef ENABLE_SECURITY
  // and return protect flag to Packet constructor for makeing design by Policy
  const RESULT_CODE result = EncryptFrame(packet);
  if (result != RESULT_OK) {
    LOG4CXX_WARN(logger_, "Error frame encryption. Frame droped.");
    return RESULT_FAIL;
  }
#endif  // ENABLE_SECURITY

  LOG4CXX_DEBUG(
      logger_,
      "Packet to be sent: "
          << ConvertPacketDataToString(packet->data(), packet->data_size())
          << " of size: " << packet->data_size());
  const RawMessagePtr message_to_send = packet->serializePacket();
  if (!message_to_send) {
    LOG4CXX_ERROR(logger_, "Serialization error");
    return RESULT_FAIL;
  }
  LOG4CXX_DEBUG(logger_,
                "Message to send with connection id "
                    << static_cast<int>(packet->connection_id()));

  if (transport_manager::E_SUCCESS !=
      transport_manager_.SendMessageToDevice(message_to_send)) {
    LOG4CXX_WARN(logger_, "Can't send message to device");
    return RESULT_FAIL;
  }
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::SendSingleFrameMessage(
    const ConnectionID connection_id,
    const uint8_t session_id,
    const uint32_t protocol_version,
    const uint8_t service_type,
    const size_t data_size,
    const uint8_t* data,
    const bool is_final_message) {
  LOG4CXX_AUTO_TRACE(logger_);

  ProtocolFramePtr ptr(
      new protocol_handler::ProtocolPacket(connection_id,
                                           protocol_version,
                                           PROTECTION_OFF,
                                           FRAME_TYPE_SINGLE,
                                           service_type,
                                           FRAME_DATA_SINGLE,
                                           session_id,
                                           data_size,
                                           message_counters_[session_id]++,
                                           data));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, is_final_message));
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::SendMultiFrameMessage(
    const ConnectionID connection_id,
    const uint8_t session_id,
    const uint8_t protocol_version,
    const uint8_t service_type,
    const size_t data_size,
    const uint8_t* data,
    const size_t max_frame_size,
    const bool is_final_message) {
  LOG4CXX_AUTO_TRACE(logger_);

  LOG4CXX_DEBUG(logger_,
                " data size " << data_size << " max_frame_size "
                              << max_frame_size);

  // remainder of last frame
  const size_t lastframe_remainder = data_size % max_frame_size;
  // size of last frame (full fill or not)
  const size_t lastframe_size =
      lastframe_remainder > 0 ? lastframe_remainder : max_frame_size;

  const size_t frames_count = data_size / max_frame_size +
                              // add last frame if not empty
                              (lastframe_remainder > 0 ? 1 : 0);

  LOG4CXX_DEBUG(logger_,
                "Data " << data_size << " bytes in " << frames_count
                        << " frames with last frame size " << lastframe_size);

  DCHECK(max_frame_size >= FIRST_FRAME_DATA_SIZE);
  DCHECK(FIRST_FRAME_DATA_SIZE >= 8);
  uint8_t out_data[FIRST_FRAME_DATA_SIZE];
  out_data[0] = data_size >> 24;
  out_data[1] = data_size >> 16;
  out_data[2] = data_size >> 8;
  out_data[3] = data_size;

  out_data[4] = frames_count >> 24;
  out_data[5] = frames_count >> 16;
  out_data[6] = frames_count >> 8;
  out_data[7] = frames_count;

  // TODO(EZamakhov): investigate message_id for CONSECUTIVE frames -
  // APPLINK-9531
  const uint8_t message_id = message_counters_[session_id]++;
  const ProtocolFramePtr firstPacket(
      new protocol_handler::ProtocolPacket(connection_id,
                                           protocol_version,
                                           PROTECTION_OFF,
                                           FRAME_TYPE_FIRST,
                                           service_type,
                                           FRAME_DATA_FIRST,
                                           session_id,
                                           FIRST_FRAME_DATA_SIZE,
                                           message_id,
                                           out_data));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(firstPacket, false));
  LOG4CXX_DEBUG(logger_, "First frame is sent.");

  for (uint32_t i = 0; i < frames_count; ++i) {
    const bool is_last_frame = (i == (frames_count - 1));
    const size_t frame_size = is_last_frame ? lastframe_size : max_frame_size;
    const uint8_t data_type = is_last_frame
                                  ? FRAME_DATA_LAST_CONSECUTIVE
                                  : (i % FRAME_DATA_MAX_CONSECUTIVE + 1);
    const bool is_final_packet = is_last_frame ? is_final_message : false;

    const ProtocolFramePtr ptr(
        new protocol_handler::ProtocolPacket(connection_id,
                                             protocol_version,
                                             PROTECTION_OFF,
                                             FRAME_TYPE_CONSECUTIVE,
                                             service_type,
                                             data_type,
                                             session_id,
                                             frame_size,
                                             message_id,
                                             data + max_frame_size * i));

    raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, is_final_packet));
    LOG4CXX_DEBUG(logger_, '#' << i << " frame is sent.");
  }
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleMessage(const ProtocolFramePtr packet) {
  DCHECK_OR_RETURN(packet, RESULT_UNKNOWN);
  LOG4CXX_DEBUG(logger_, "Handling message " << packet);
  switch (packet->frame_type()) {
    case FRAME_TYPE_CONTROL:
      LOG4CXX_TRACE(logger_, "FRAME_TYPE_CONTROL");
      return HandleControlMessage(packet);
    case FRAME_TYPE_SINGLE:
      LOG4CXX_TRACE(logger_, "FRAME_TYPE_SINGLE");
      return HandleSingleFrameMessage(packet);
    case FRAME_TYPE_FIRST:
    case FRAME_TYPE_CONSECUTIVE:
      LOG4CXX_TRACE(logger_, "FRAME_TYPE_FIRST or FRAME_TYPE_CONSECUTIVE");
      return HandleMultiFrameMessage(packet);
    default: {
      LOG4CXX_WARN(logger_, "Unknown frame type" << packet->frame_type());
      return RESULT_FAIL;
    }
  }
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleSingleFrameMessage(
    const ProtocolFramePtr packet) {
  LOG4CXX_AUTO_TRACE(logger_);

  LOG4CXX_DEBUG(
      logger_,
      "FRAME_TYPE_SINGLE message of size "
          << packet->data_size() << "; message "
          << ConvertPacketDataToString(packet->data(), packet->data_size()));

  const uint32_t connection_key = session_observer_.KeyFromPair(
      packet->connection_id(), packet->session_id());

  const RawMessagePtr rawMessage(new RawMessage(connection_key,
                                                packet->protocol_version(),
                                                packet->data(),
                                                packet->total_data_bytes(),
                                                packet->service_type(),
                                                packet->payload_size()));
  if (!rawMessage) {
    return RESULT_FAIL;
  }
#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    PHTelemetryObserver::MessageMetric* metric =
        new PHTelemetryObserver::MessageMetric();
    metric->message_id = packet->message_id();
    metric->connection_key = connection_key;
    metric->raw_msg = rawMessage;
    metric_observer_->EndMessageProcess(metric);
  }
#endif

  // TODO(EZamakhov): check service in session
  NotifySubscribers(rawMessage);
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleMultiFrameMessage(
    const ProtocolFramePtr packet) {
  LOG4CXX_AUTO_TRACE(logger_);

  if (multiframe_builder_.AddFrame(packet) != RESULT_OK) {
    LOG4CXX_WARN(logger_, "Frame assembling issue");
  }

  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessage(
    const ProtocolFramePtr packet) {
  LOG4CXX_AUTO_TRACE(logger_);

  // TODO{ALeshin}: Rename "Session" to "Service" on PH, CH, AM levels
  switch (packet->frame_data()) {
    case FRAME_DATA_START_SERVICE: {
      LOG4CXX_TRACE(logger_, "FrameData: StartService");
      return HandleControlMessageStartSession(*packet);
    }
    case FRAME_DATA_END_SERVICE: {
      LOG4CXX_TRACE(logger_, "FrameData: StopService");
      return HandleControlMessageEndSession(*packet);
    }
    case FRAME_DATA_END_SERVICE_ACK: {
      LOG4CXX_TRACE(logger_, "FrameData: StopService ACK");
      return HandleControlMessageEndServiceACK(*packet);
    }
    case FRAME_DATA_HEART_BEAT: {
      LOG4CXX_TRACE(logger_, "FrameData: Heartbeat");
      return HandleControlMessageHeartBeat(*packet);
    }
    case FRAME_DATA_HEART_BEAT_ACK: {
      LOG4CXX_TRACE(logger_, "FrameData Heartbeat ACK");
      LOG4CXX_DEBUG(logger_,
                    "Received Heartbeat ACK from mobile,"
                    " connection: "
                        << packet->connection_id());
      return RESULT_OK;
    }
    default:
      LOG4CXX_WARN(logger_,
                   "Control message of type "
                       << static_cast<int>(packet->frame_data()) << " ignored");
      return RESULT_OK;
  }
  return RESULT_OK;
}

uint32_t get_hash_id(const ProtocolPacket& packet) {
  if (packet.protocol_version() < PROTOCOL_VERSION_2) {
    LOG4CXX_DEBUG(logger_,
                  "Packet without hash data (protocol version less 2)");
    return HASH_ID_NOT_SUPPORTED;
  }
  if (packet.data_size() < 4) {
    LOG4CXX_WARN(logger_, "Packet without hash data (data size less 4)");
    return HASH_ID_WRONG;
  }
  const uint32_t hash_be = *(reinterpret_cast<uint32_t*>(packet.data()));
  const uint32_t hash_le = BE_TO_LE32(hash_be);
  // null hash is wrong hash value
  return hash_le == HASH_ID_NOT_SUPPORTED ? HASH_ID_WRONG : hash_le;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageEndSession(
    const ProtocolPacket& packet) {
  LOG4CXX_AUTO_TRACE(logger_);

  const uint8_t current_session_id = packet.session_id();
  const uint32_t hash_id = get_hash_id(packet);
  const ServiceType service_type = ServiceTypeFromByte(packet.service_type());

  const ConnectionID connection_id = packet.connection_id();
  const uint32_t session_key = session_observer_.OnSessionEndedCallback(
      connection_id, current_session_id, hash_id, service_type);

  // TODO(EZamakhov): add clean up output queue (for removed service)
  if (session_key != 0) {
    SendEndSessionAck(connection_id,
                      current_session_id,
                      packet.protocol_version(),
                      service_type);
    message_counters_.erase(current_session_id);
  } else {
    LOG4CXX_WARN(logger_,
                 "Refused to end session " << static_cast<int>(service_type)
                                           << " type.");
    SendEndSessionNAck(connection_id,
                       current_session_id,
                       packet.protocol_version(),
                       service_type);
  }
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageEndServiceACK(
    const ProtocolPacket& packet) {
  LOG4CXX_AUTO_TRACE(logger_);

  const uint8_t current_session_id = packet.session_id();
  const uint32_t hash_id = get_hash_id(packet);
  const ServiceType service_type = ServiceTypeFromByte(packet.service_type());
  const ConnectionID connection_id = packet.connection_id();

  const uint32_t session_key = session_observer_.OnSessionEndedCallback(
      connection_id, current_session_id, hash_id, service_type);

  if (0 == session_key) {
    LOG4CXX_WARN(logger_, "Refused to end service");
    return RESULT_FAIL;
  }

  return RESULT_OK;
}

#ifdef ENABLE_SECURITY
namespace {
/**
 * \brief SecurityManagerListener for send Ack/NAck on success or fail
 * SSL handshake
 */
class StartSessionHandler : public security_manager::SecurityManagerListener {
 public:
  StartSessionHandler(uint32_t connection_key,
                      ProtocolHandlerImpl* protocol_handler,
                      SessionObserver& session_observer,
                      ConnectionID connection_id,
                      int32_t session_id,
                      uint8_t protocol_version,
                      uint32_t hash_id,
                      ServiceType service_type,
                      const std::vector<int>& force_protected_service)
      : connection_key_(connection_key)
      , protocol_handler_(protocol_handler)
      , session_observer_(session_observer)
      , connection_id_(connection_id)
      , session_id_(session_id)
      , protocol_version_(protocol_version)
      , hash_id_(hash_id)
      , service_type_(service_type)
      , force_protected_service_(force_protected_service) {}

  bool OnHandshakeDone(
      const uint32_t connection_key,
      security_manager::SSLContext::HandshakeResult result) OVERRIDE {
    if (connection_key != connection_key_) {
      return false;
    }
    const bool success =
        result == security_manager::SSLContext::Handshake_Result_Success;
    // check current service protection
    const bool was_service_protection_enabled =
        session_observer_.GetSSLContext(connection_key_, service_type_) != NULL;
    if (was_service_protection_enabled) {
      if (!success) {
        protocol_handler_->SendStartSessionNAck(
            connection_id_, session_id_, protocol_version_, service_type_);
      } else {
        protocol_handler_->SendStartSessionAck(connection_id_,
                                               session_id_,
                                               protocol_version_,
                                               hash_id_,
                                               service_type_,
                                               success);
      }
    } else {
      if (success) {
        session_observer_.SetProtectionFlag(connection_key_, service_type_);
      }
      protocol_handler_->SendStartSessionAck(connection_id_,
                                             session_id_,
                                             protocol_version_,
                                             hash_id_,
                                             service_type_,
                                             success);
    }
    delete this;
    return true;
  }

  void OnCertificateUpdateRequired() OVERRIDE {}

  virtual const std::vector<int>& force_protected_service() const {
    return force_protected_service_;
  }

 private:
  const uint32_t connection_key_;
  ProtocolHandlerImpl* protocol_handler_;
  SessionObserver& session_observer_;

  const ConnectionID connection_id_;
  const int32_t session_id_;
  const uint8_t protocol_version_;
  const uint32_t hash_id_;
  const ServiceType service_type_;
  const std::vector<int> force_protected_service_;
};
}  // namespace
#endif  // ENABLE_SECURITY

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageStartSession(
    const ProtocolPacket& packet) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(
      logger_,
      "Protocol version:" << static_cast<int>(packet.protocol_version()));
  const ServiceType service_type = ServiceTypeFromByte(packet.service_type());
  const uint8_t protocol_version = packet.protocol_version();

#ifdef ENABLE_SECURITY
  const bool protection =
      // Protocol version 1 is not support protection
      (protocol_version > PROTOCOL_VERSION_1) ? packet.protection_flag()
                                              : false;
#else
  const bool protection = false;
#endif  // ENABLE_SECURITY

  struct SessionObserver::ExistingSessionInfo si = {0};
  const ConnectionID connection_id = packet.connection_id();
  const bool first_try = false;
  const uint32_t session_id =
      session_observer_.OnSessionStartedCallback(connection_id,
                                                 packet.session_id(),
                                                 service_type,
                                                 protection,
                                                 first_try,
                                                 &si);

  if (0 == session_id) {
    LOG4CXX_WARN(logger_,
                 "Refused by session_observer to create service "
                     << static_cast<int32_t>(service_type) << " type.");
    SendStartSessionNAck(connection_id,
                         packet.session_id(),
                         protocol_version,
                         packet.service_type());
    return RESULT_OK;
  }

#ifdef ENABLE_SECURITY
  // if encryption of the service is requested and security plug-in is enable
  if (si.start_protected_ && security_manager_) {
    const uint32_t connection_key =
        session_observer_.KeyFromPair(connection_id, session_id);
    struct SessionInfo session_info = SessionInfo(connection_id,
                                                  packet.session_id(),
                                                  protocol_version,
                                                  packet.service_type(),
                                                  si.hash_id_,
                                                  si.service_exists_,
                                                  connection_key);
    security_manager::SSLContext* ssl_context =
        GetSSLContextBySession(session_info);
    if (!ssl_context) {
      ssl_context = security_manager_->CreateSSLContext(connection_key);
    }
    if (!ssl_context || security_manager_->IsCertificateUpdateRequired()) {
      const std::string error("CreateSSLContext failed");
      LOG4CXX_ERROR(logger_, error);
      security_manager_->SendInternalError(
          connection_key,
          security_manager::SecurityManager::ERROR_INTERNAL,
          error);
      // PTU for navi application has already been triggered by RAI
      // so that there is no need of new PTU
      if (si.is_navi_) {
        LOG4CXX_INFO(logger_, "application is navi ");
        // if service has already been started as non encrypted
        // or it has to be protected
        // and there is no valid certificate
        // Nack should be sent
        if (si.service_exists_ ||
            (si.start_protected_ && !si.can_be_unprotected_)) {
          LOG4CXX_INFO(logger_,
                       "application is " << si.start_protected_ << ", "
                                         << si.can_be_unprotected_
                                         << ", service exist"
                                         << si.service_exists_);
          //              if (!si.start_protected_) {
          SendStartSessionNAck(connection_id,
                               packet.session_id(),
                               protocol_version,
                               packet.service_type());
        } else {
          const bool second_try = true;
          const uint32_t session_id =
              session_observer_.OnSessionStartedCallback(connection_id,
                                                         packet.session_id(),
                                                         service_type,
                                                         protection,
                                                         second_try,
                                                         &si);

          // service has not been started yet
          // so that start it as non encrypted
          // Ack encryption=false should be sent
          // Service was started, so session_id should not be = 0
          if (session_id != 0) {
            SendStartSessionAck(connection_id,
                                session_id,
                                packet.protocol_version(),
                                si.hash_id_,
                                packet.service_type(),
                                PROTECTION_OFF);
          } else {
            SendStartSessionNAck(connection_id,
                                 packet.session_id(),
                                 protocol_version,
                                 packet.service_type());
          }
        }
        return RESULT_OK;
      }
      LOG4CXX_INFO(logger_, "application is not navi ");
      // This is non navi application,
      // so that a PTU must be triggered if no certificate is provided
      // and PTU has not been triggered by this reason
      sync_primitives::AutoLock lock(ptu_state_lock_);
      // PTU has not been triggered
      // trigger PTU

      // TODO ADD LOGS HERE
      if (ptu_state_ == kNotTriggered) {
        LOG4CXX_INFO(logger_, "application should trigger PTU now");
        ptu_state_ = kTriggered;
        pending_session_ = session_info;
        security_manager_->NotifyOnCertififcateUpdateRequired();
      } else if (ptu_state_ != kTriggered) {
        // PTU has already been triggered
        // and had brought no valid certificate
        // Nack should be sent

        if (si.can_be_unprotected_) {
          // service has not been started yet
          // so that start can be started both as non encrypted and encrypted
          // And TLS handshake failed
          // We should start session as not encrypted
          LOG4CXX_INFO(logger_, "application can start unprotected");
          const bool second_try = true;
          const uint32_t session_id =
              session_observer_.OnSessionStartedCallback(connection_id,
                                                         packet.session_id(),
                                                         service_type,
                                                         protection,
                                                         second_try,
                                                         &si);

          // Ack encryption=false should be sent
          SendStartSessionAck(connection_id,
                              session_id,
                              packet.protocol_version(),
                              si.hash_id_,
                              packet.service_type(),
                              PROTECTION_OFF);
        }
        LOG4CXX_INFO(logger_, "application cannot start unprotected");
        SendStartSessionNAck(connection_id,
                             packet.session_id(),
                             protocol_version,
                             packet.service_type());
      }

      return RESULT_OK;
    }
    // Certificate is existing and doesn't rquire update,
    // try to start encrypted service
    if (ssl_context->IsInitCompleted()) {
      // mark service as protected
      session_observer_.SetProtectionFlag(connection_key, service_type);
      // Start service as protected with current SSLContext
      SendStartSessionAck(connection_id,
                          session_id,
                          packet.protocol_version(),
                          si.hash_id_,
                          packet.service_type(),
                          PROTECTION_ON);
    } else {
      security_manager_->AddListener(
          new StartSessionHandler(connection_key,
                                  this,
                                  session_observer_,
                                  connection_id,
                                  session_id,
                                  packet.protocol_version(),
                                  si.hash_id_,
                                  service_type,
                                  get_settings().force_protected_service()));
      if (!ssl_context->IsHandshakePending()) {
        // Start handshake process
        security_manager_->StartHandshake(connection_key);
      }
      LOG4CXX_DEBUG(logger_,
                    "Protection establishing for connection "
                        << connection_key << " is in progress");
    }
    return RESULT_OK;
  }
#endif  // ENABLE_SECURITY
  // Start service without protection
  SendStartSessionAck(connection_id,
                      session_id,
                      packet.protocol_version(),
                      si.hash_id_,
                      packet.service_type(),
                      PROTECTION_OFF);
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageHeartBeat(
    const ProtocolPacket& packet) {
  const ConnectionID connection_id = packet.connection_id();
  LOG4CXX_DEBUG(logger_,
                "Sending heart beat acknowledgment for connection "
                    << connection_id);
  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          connection_id, packet.session_id(), protocol_version)) {
    // TODO(EZamakhov): investigate message_id for HeartBeatAck
    if (PROTOCOL_VERSION_3 == protocol_version ||
        PROTOCOL_VERSION_4 == protocol_version) {
      return SendHeartBeatAck(
          connection_id, packet.session_id(), packet.message_id());
    } else {
      LOG4CXX_WARN(logger_, "HeartBeat is not supported");
      return RESULT_HEARTBEAT_IS_NOT_SUPPORTED;
    }

  } else {
    LOG4CXX_WARN(
        logger_,
        "SendHeartBeatAck is failed connection or session does not exist");
    return RESULT_FAIL;
  }
}

void ProtocolHandlerImpl::PopValideAndExpirateMultiframes() {
  const ProtocolFramePtrList& frame_list = multiframe_builder_.PopMultiframes();
  for (ProtocolFramePtrList::const_iterator it = frame_list.begin();
       it != frame_list.end();
       ++it) {
    const ProtocolFramePtr frame = *it;
    DCHECK(frame);
    if (!frame) {
      continue;
    }

    const uint32_t connection_key = session_observer_.KeyFromPair(
        frame->connection_id(), frame->session_id());
    LOG4CXX_DEBUG(logger_,
                  "Result frame" << frame << "for connection "
                                 << connection_key);
    const RawMessagePtr rawMessage(new RawMessage(connection_key,
                                                  frame->protocol_version(),
                                                  frame->data(),
                                                  frame->total_data_bytes(),
                                                  frame->service_type(),
                                                  frame->payload_size()));
    DCHECK(rawMessage);

#ifdef TELEMETRY_MONITOR
    if (metric_observer_) {
      PHTelemetryObserver::MessageMetric* metric =
          new PHTelemetryObserver::MessageMetric();
      metric->raw_msg = rawMessage;
      metric_observer_->EndMessageProcess(metric);
    }
#endif  // TELEMETRY_MONITOR
    NotifySubscribers(rawMessage);
  }
}

bool ProtocolHandlerImpl::TrackMessage(const uint32_t& connection_key) {
  LOG4CXX_AUTO_TRACE(logger_);
  const size_t& frequency_time = get_settings().message_frequency_time();
  const size_t& frequency_count = get_settings().message_frequency_count();
  if (frequency_time > 0u && frequency_count > 0u) {
    const size_t message_frequency =
        message_meter_.TrackMessage(connection_key);
    LOG4CXX_DEBUG(logger_,
                  "Frequency of " << connection_key << " is "
                                  << message_frequency);
    if (message_frequency > frequency_count) {
      LOG4CXX_WARN(logger_,
                   "Frequency of " << connection_key << " is marked as high.");
      session_observer_.OnApplicationFloodCallBack(connection_key);
      message_meter_.RemoveIdentifier(connection_key);
      return true;
    }
  }
  return false;
}

bool ProtocolHandlerImpl::TrackMalformedMessage(const uint32_t& connection_key,
                                                const size_t count) {
  const size_t& malformed_frequency_count =
      get_settings().malformed_frequency_count();
  LOG4CXX_AUTO_TRACE(logger_);
  if (get_settings().malformed_frequency_time() > 0u &&
      malformed_frequency_count > 0u) {
    const size_t malformed_message_frequency =
        malformed_message_meter_.TrackMessages(connection_key, count);
    LOG4CXX_DEBUG(logger_,
                  "Malformed frequency of " << connection_key << " is "
                                            << malformed_message_frequency);
    if (!get_settings().malformed_message_filtering() ||
        malformed_message_frequency > malformed_frequency_count) {
      LOG4CXX_WARN(logger_,
                   "Malformed frequency of " << connection_key
                                             << " is marked as high.");
      session_observer_.OnMalformedMessageCallback(connection_key);
      malformed_message_meter_.RemoveIdentifier(connection_key);
      return true;
    }
  }
  return false;
}

void ProtocolHandlerImpl::Handle(const impl::RawFordMessageFromMobile message) {
  LOG4CXX_AUTO_TRACE(logger_);

  switch (message->service_type()) {
    case kMobileNav:
    case kAudio:
      break;
    default: {
      const uint32_t connection_key = session_observer_.KeyFromPair(
          message->connection_id(), message->session_id());
      if (TrackMessage(connection_key)) {
        return;
      }
    } break;
  }
  LOG4CXX_DEBUG(logger_, "Message : " << message.get());
  const uint8_t c_id = message->connection_id();
  const uint32_t m_id = message->session_id();

  if (session_observer_.IsHeartBeatSupported(c_id, m_id)) {
    connection_handler_.KeepConnectionAlive(c_id, m_id);
  }

  // TODO(EZamakhov): remove dublication of IncomingDataHandler logic
  if (((0 != message->data()) && (0 != message->data_size())) ||
      FRAME_TYPE_CONTROL == message->frame_type() ||
      FRAME_TYPE_FIRST == message->frame_type()) {
    LOG4CXX_DEBUG(logger_, "Packet: dataSize " << message->data_size());
    HandleMessage(message);
    PopValideAndExpirateMultiframes();
  } else {
    LOG4CXX_WARN(logger_,
                 "handleMessagesFromMobileApp() - incorrect or NULL data");
  }
}

void ProtocolHandlerImpl::Handle(const impl::RawFordMessageToMobile message) {
  LOG4CXX_DEBUG(logger_,
                "Message to mobile app: connection id "
                    << static_cast<int>(message->connection_id())
                    << ";"
                       " dataSize: " << message->data_size()
                    << " ;"
                       " protocolVersion "
                    << static_cast<int>(message->protocol_version()));

  if (message.is_final) {
    sessions_last_message_id_.insert(std::pair<uint8_t, uint32_t>(
        message->session_id(), message->message_id()));
  }

  SendFrame(message);
}

void ProtocolHandlerImpl::Stop() {
  raw_ford_messages_from_mobile_.Shutdown();
  raw_ford_messages_to_mobile_.Shutdown();
}

#ifdef ENABLE_SECURITY
void ProtocolHandlerImpl::set_security_manager(
    security_manager::SecurityManager* security_manager) {
  if (!security_manager) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to SecurityManager.");
    return;
  }
  security_manager_ = security_manager;
}

RESULT_CODE ProtocolHandlerImpl::EncryptFrame(ProtocolFramePtr packet) {
  DCHECK(packet);
  // Control frames and data over control service shall be unprotected
  if (packet->service_type() == kControl ||
      packet->frame_type() == FRAME_TYPE_CONTROL) {
    return RESULT_OK;
  }
  if (!security_manager_) {
    LOG4CXX_WARN(logger_, "No security_manager_ set.");
    return RESULT_FAIL;
  }
  const uint32_t connection_key = session_observer_.KeyFromPair(
      packet->connection_id(), packet->session_id());
  security_manager::SSLContext* context = session_observer_.GetSSLContext(
      connection_key, ServiceTypeFromByte(packet->service_type()));
  if (!context || !context->IsInitCompleted()) {
    return RESULT_OK;
  }
  const uint8_t* out_data;
  size_t out_data_size;
  if (!context->Encrypt(
          packet->data(), packet->data_size(), &out_data, &out_data_size)) {
    const std::string error_text(context->LastError());
    LOG4CXX_ERROR(logger_, "Enryption failed: " << error_text);
    security_manager_->SendInternalError(
        connection_key,
        security_manager::SecurityManager::ERROR_ENCRYPTION_FAILED,
        error_text);
    // Close session to prevent usage unprotected service/session
    session_observer_.OnSessionEndedCallback(packet->connection_id(),
                                             packet->session_id(),
                                             packet->message_id(),
                                             kRpc);
    return RESULT_OK;
  }
  LOG4CXX_DEBUG(logger_,
                "Encrypted " << packet->data_size() << " bytes to "
                             << out_data_size << " bytes");
  DCHECK(out_data);
  DCHECK(out_data_size);
  packet->set_protection_flag(true);
  packet->set_data(out_data, out_data_size);
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::DecryptFrame(ProtocolFramePtr packet) {
  DCHECK(packet);
  if (!packet->protection_flag() ||
      // Control frames and data over control service shall be unprotected
      packet->service_type() == kControl ||
      packet->frame_type() == FRAME_TYPE_CONTROL) {
    return RESULT_OK;
  }
  if (!security_manager_) {
    LOG4CXX_WARN(logger_, "No security_manager_ set.");
    return RESULT_FAIL;
  }
  const uint32_t connection_key = session_observer_.KeyFromPair(
      packet->connection_id(), packet->session_id());
  security_manager::SSLContext* context = session_observer_.GetSSLContext(
      connection_key, ServiceTypeFromByte(packet->service_type()));
  if (!context || !context->IsInitCompleted()) {
    const std::string error_text("Fail decryption for unprotected service ");
    LOG4CXX_ERROR(logger_,
                  error_text << static_cast<int>(packet->service_type()));
    security_manager_->SendInternalError(
        connection_key,
        security_manager::SecurityManager::ERROR_SERVICE_NOT_PROTECTED,
        error_text);
    return RESULT_ENCRYPTION_FAILED;
  }
  const uint8_t* out_data;
  size_t out_data_size;
  if (!context->Decrypt(
          packet->data(), packet->data_size(), &out_data, &out_data_size)) {
    const std::string error_text(context->LastError());
    LOG4CXX_ERROR(logger_, "Decryption failed: " << error_text);
    security_manager_->SendInternalError(
        connection_key,
        security_manager::SecurityManager::ERROR_DECRYPTION_FAILED,
        error_text);
    // Close session to prevent usage unprotected service/session
    session_observer_.OnSessionEndedCallback(packet->connection_id(),
                                             packet->session_id(),
                                             packet->message_id(),
                                             kRpc);
    return RESULT_ENCRYPTION_FAILED;
  }
  LOG4CXX_DEBUG(logger_,
                "Decrypted " << packet->data_size() << " bytes to "
                             << out_data_size << " bytes");
  DCHECK(out_data);
  DCHECK(out_data_size);
  packet->set_data(out_data, out_data_size);
  return RESULT_OK;
}
#endif  // ENABLE_SECURITY

void ProtocolHandlerImpl::SendFramesNumber(uint32_t connection_key,
                                           int32_t number_of_frames) {
  LOG4CXX_DEBUG(
      logger_, "SendFramesNumber MobileNaviAck for session " << connection_key);

  // TODO(EZamakhov): add protocol version check - to avoid send for
  // PROTOCOL_VERSION_1
  transport_manager::ConnectionUID connection_id = 0;
  uint8_t session_id = 0;
  session_observer_.PairFromKey(connection_key, &connection_id, &session_id);
  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          connection_id, session_id, protocol_version)) {
    ProtocolFramePtr ptr(
        new protocol_handler::ProtocolPacket(connection_id,
                                             protocol_version,
                                             PROTECTION_OFF,
                                             FRAME_TYPE_CONTROL,
                                             SERVICE_TYPE_NAVI,
                                             FRAME_DATA_SERVICE_DATA_ACK,
                                             session_id,
                                             0,
                                             message_counters_[session_id]++));

    // Flow control data shall be 4 bytes according Ford Protocol
    DCHECK(sizeof(number_of_frames) == 4);
    number_of_frames = LE_TO_BE32(number_of_frames);
    ptr->set_data(reinterpret_cast<const uint8_t*>(&number_of_frames),
                  sizeof(number_of_frames));
    raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, false));
    LOG4CXX_DEBUG(logger_, "SendFramesNumber finished successfully");
  } else {
    LOG4CXX_WARN(
        logger_,
        "SendFramesNumber is failed connection or session does not exist");
  }
}

#ifdef TELEMETRY_MONITOR
void ProtocolHandlerImpl::SetTelemetryObserver(PHTelemetryObserver* observer) {
  metric_observer_ = observer;
}
#endif  // TELEMETRY_MONITOR

std::string ConvertPacketDataToString(const uint8_t* data,
                                      const size_t data_size) {
  if (0 == data_size)
    return std::string();
  bool is_printable_array = true;
  std::locale loc;
  const char* text = reinterpret_cast<const char*>(data);
  // Check data for printability
  for (size_t i = 0; i < data_size; ++i) {
    if (!std::isprint(text[i], loc)) {
      is_printable_array = false;
      break;
    }
  }
  return is_printable_array ? std::string(text, data_size)
                            : std::string("is raw data");
}

uint8_t ProtocolHandlerImpl::SupportedSDLProtocolVersion() const {
  LOG4CXX_AUTO_TRACE(logger_);

  bool heart_beat_support = (0 != get_settings().heart_beat_timeout());

  bool sdl4_support = get_settings().enable_protocol_4();

  if (sdl4_support) {
    return PROTOCOL_VERSION_4;
  }
  if (heart_beat_support) {
    return PROTOCOL_VERSION_3;
  }
  return PROTOCOL_VERSION_2;
}

void ProtocolHandlerImpl::OnPTUFinished(const bool ptu_result) {
  LOG4CXX_AUTO_TRACE(logger_);
#ifdef ENABLE_SECURITY
  sync_primitives::AutoLock lock(ptu_state_lock_);
  if (ptu_state_ != kTriggered) {
    return;
  }
  if (ptu_result) {
    ptu_state_ = kFinSuccess;
    security_manager::SSLContext* ssl_context =
        GetSSLContextBySession(pending_session_);
    if ((!ssl_context) || security_manager_->IsCertificateUpdateRequired()) {
      SendStartSessionNAck(pending_session_.connection_id_,
                           pending_session_.session_id_,
                           pending_session_.protocol_version_,
                           pending_session_.service_type_);
    } else {
      StartEncryptedService(pending_session_);
    }
    return;
  }
  ptu_state_ = kFinNotSuccess;
  SendStartSessionNAck(pending_session_.connection_id_,
                       pending_session_.session_id_,
                       pending_session_.protocol_version_,
                       pending_session_.service_type_);
#endif
}

bool ProtocolHandlerImpl::OnCertificateUpdated(
    const std::string& certificate_data) {
  LOG4CXX_AUTO_TRACE(logger_);
#ifdef ENABLE_SECURITY
  sync_primitives::AutoLock lock(ptu_state_lock_);
  if (ptu_state_ != kTriggered) {
    ptu_state_ = kNotTriggered;
  }
  return false;
#else
  return false;
#endif
}

void ProtocolHandlerImpl::OnUpdateHMIAppType(
    std::map<std::string, std::vector<std::string> > app_hmi_types) {}

#ifdef ENABLE_SECURITY
security_manager::SSLContext* ProtocolHandlerImpl::GetSSLContextBySession(
    const SessionInfo& session_info) {
  const uint32_t connection_key = session_observer_.KeyFromPair(
      session_info.connection_id_, session_info.session_id_);
  session_observer_.SetProtectionFlag(
      connection_key, ServiceTypeFromByte(session_info.service_type_));
  return session_observer_.GetSSLContext(connection_key,
                                         protocol_handler::kControl);
}

void ProtocolHandlerImpl::StartEncryptedService(const SessionInfo& si) {
  security_manager::SSLContext* ssl_context = GetSSLContextBySession(si);

  struct SessionObserver::ExistingSessionInfo dump = {0};
  const ServiceType service_type = ServiceTypeFromByte(si.service_type_);
  const bool try_start = true;
  session_observer_.OnSessionStartedCallback(si.connection_id_,
                                             si.session_id_,
                                             service_type,
                                             PROTECTION_ON,
                                             try_start,
                                             &dump);

  // Start service as protected with current SSLContext
  if (ssl_context && ssl_context->IsInitCompleted()) {
    SendStartSessionAck(si.connection_id_,
                        si.session_id_,
                        si.protocol_version_,
                        si.hash_id_,
                        si.service_type_,
                        PROTECTION_ON);
  } else {
    security_manager_->AddListener(
        new StartSessionHandler(si.connection_key_,
                                this,
                                session_observer_,
                                si.connection_id_,
                                si.session_id_,
                                si.protocol_version_,
                                si.hash_id_,
                                ServiceTypeFromByte(si.service_type_),
                                get_settings().force_protected_service()));
    if (!ssl_context->IsHandshakePending()) {
      // Start handshake process
      security_manager_->StartHandshake(si.connection_key_);
    }
  }
  LOG4CXX_DEBUG(logger_,
                "Protection establishing for connection " << si.connection_key_
                                                          << " is in progress");
}
#endif

}  // namespace protocol_handler

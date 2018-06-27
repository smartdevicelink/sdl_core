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
#include <arpa/inet.h>  // for INET6_ADDRSTRLEN
#include <memory.h>
#include <algorithm>  // std::find
#include <bson_object.h>
#include <protocol/bson_object_keys.h>

#include "connection_handler/connection_handler_impl.h"
#include "protocol_handler/session_observer.h"
#include "utils/byte_order.h"
#include "utils/helpers.h"
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

const size_t kStackSize = 65536;

ProtocolPacket::ProtocolVersion defaultProtocolVersion(5, 1, 0);
ProtocolPacket::ProtocolVersion minMultipleTransportsVersion(5, 1, 0);

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
    , start_session_frame_map_lock_()
    , start_session_frame_map_()
    , tcp_enabled_(false)
#ifdef TELEMETRY_MONITOR
    , metric_observer_(NULL)
#endif  // TELEMETRY_MONITOR

{
  LOG4CXX_AUTO_TRACE(logger_);
  protocol_header_validator_.set_max_payload_size(
      get_settings().maximum_payload_size());
  protocol_header_validator_.set_max_control_payload_size(
      get_settings().maximum_control_payload_size());
  protocol_header_validator_.set_max_rpc_payload_size(
      get_settings().maximum_rpc_payload_size());
  protocol_header_validator_.set_max_audio_payload_size(
      get_settings().maximum_audio_payload_size());
  protocol_header_validator_.set_max_video_payload_size(
      get_settings().maximum_video_payload_size());
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
                                              uint8_t input_protocol_version,
                                              uint32_t hash_id,
                                              uint8_t service_type,
                                              bool protection) {
  LOG4CXX_AUTO_TRACE(logger_);
  ProtocolPacket::ProtocolVersion fullVersion;
  SendStartSessionAck(connection_id,
                      session_id,
                      input_protocol_version,
                      hash_id,
                      service_type,
                      protection,
                      fullVersion);
}

void ProtocolHandlerImpl::SendStartSessionAck(
    ConnectionID connection_id,
    uint8_t session_id,
    uint8_t input_protocol_version,
    uint32_t hash_id,
    uint8_t service_type,
    bool protection,
    ProtocolPacket::ProtocolVersion& full_version) {
  LOG4CXX_AUTO_TRACE(logger_);

  BsonObject empty_param;
  bson_object_initialize_default(&empty_param);

  SendStartSessionAck(connection_id,
                      session_id,
                      input_protocol_version,
                      hash_id,
                      service_type,
                      protection,
                      full_version,
                      empty_param);

  bson_object_deinitialize(&empty_param);
}

void ProtocolHandlerImpl::SendStartSessionAck(
    ConnectionID connection_id,
    uint8_t session_id,
    uint8_t input_protocol_version,
    uint32_t hash_id,
    uint8_t service_type,
    bool protection,
    ProtocolPacket::ProtocolVersion& full_version,
    BsonObject& params) {
  LOG4CXX_AUTO_TRACE(logger_);

  bool send_transport_update_event = false;

  uint8_t ack_protocol_version = SupportedSDLProtocolVersion();

  const bool proxy_supports_v5_protocol =
      input_protocol_version >= PROTOCOL_VERSION_5 ||
      (ServiceTypeFromByte(service_type) == kRpc &&
       full_version.majorVersion >= PROTOCOL_VERSION_5);

  if (kRpc != service_type) {
    // In case if input protocol version os bigger then supported, SDL should
    // respond with maximum supported protocol version
    ack_protocol_version = input_protocol_version < ack_protocol_version
                               ? input_protocol_version
                               : ack_protocol_version;
  }

  // We can't send a V5+ packet if the proxy doesn't support it,
  // so we manually set a maximum of V4 in that case
  if (!proxy_supports_v5_protocol &&
      ack_protocol_version >= PROTOCOL_VERSION_5) {
    ack_protocol_version = PROTOCOL_VERSION_4;
  }

  ProtocolFramePtr ptr(
      new protocol_handler::ProtocolPacket(connection_id,
                                           ack_protocol_version,
                                           protection,
                                           FRAME_TYPE_CONTROL,
                                           service_type,
                                           FRAME_DATA_START_SERVICE_ACK,
                                           session_id,
                                           0u,
                                           message_counters_[session_id]++));

  // Cannot include a constructed payload if either side doesn't support it
  if (ack_protocol_version >= PROTOCOL_VERSION_5) {
    ServiceType serviceTypeValue = ServiceTypeFromByte(service_type);

    const bool mtu_written = bson_object_put_int64(
        &params,
        strings::mtu,
        static_cast<int64_t>(
            protocol_header_validator_.max_payload_size_by_service_type(
                serviceTypeValue)));
    LOG4CXX_DEBUG(logger_,
                  "MTU parameter was written to bson params: "
                      << mtu_written << "; Value: "
                      << static_cast<int32_t>(
                             bson_object_get_int64(&params, strings::mtu)));

    if (serviceTypeValue == kRpc) {
      // Hash ID is only used in RPC case
      const bool hash_written = bson_object_put_int32(
          &params, strings::hash_id, static_cast<int32_t>(hash_id));
      LOG4CXX_DEBUG(logger_,
                    "Hash parameter was written to bson params: "
                        << hash_written << "; Value: "
                        << static_cast<int32_t>(bson_object_get_int32(
                               &params, strings::hash_id)));

      // Minimum protocol version supported by both
      ProtocolPacket::ProtocolVersion* minVersion =
          (full_version.majorVersion < PROTOCOL_VERSION_5)
              ? &defaultProtocolVersion
              : ProtocolPacket::ProtocolVersion::min(full_version,
                                                     defaultProtocolVersion);
      char protocolVersionString[256];
      strncpy(protocolVersionString, (*minVersion).to_string().c_str(), 255);

      const bool protocol_ver_written = bson_object_put_string(
          &params, strings::protocol_version, protocolVersionString);
      LOG4CXX_DEBUG(
          logger_,
          "Protocol version parameter was written to bson params: "
              << protocol_ver_written << "; Value: "
              << bson_object_get_string(&params, strings::protocol_version));

      LOG4CXX_INFO(logger_,
                   "Protocol Version String " << protocolVersionString);

      std::vector<std::string> secondaryTransports;
      std::vector<int32_t> audioServiceTransports;
      std::vector<int32_t> videoServiceTransports;
      if (*minVersion >= minMultipleTransportsVersion) {
        if (ParseSecondaryTransportConfiguration(connection_id,
                                                 secondaryTransports,
                                                 audioServiceTransports,
                                                 videoServiceTransports)) {
          LOG4CXX_TRACE(logger_, "Multiple transports are enabled.");
          BsonArray secondaryTransportsArr;
          bson_array_initialize(&secondaryTransportsArr,
                                secondaryTransports.size());
          for (unsigned int i = 0; i < secondaryTransports.size(); i++) {
            char secondaryTransport[255];
            strncpy(secondaryTransport,
                    secondaryTransports[i].c_str(),
                    sizeof(secondaryTransport));
            secondaryTransport[sizeof(secondaryTransport) - 1] = '\0';
            LOG4CXX_DEBUG(
                logger_,
                "Adding "
                    << secondaryTransport
                    << " to secondaryTransports parameter of StartSessionAck");
            bson_array_add_string(&secondaryTransportsArr, secondaryTransport);
          }
          bson_object_put_array(
              &params, strings::secondary_transports, &secondaryTransportsArr);

          BsonArray audioServiceTransportsArr;
          bson_array_initialize(&audioServiceTransportsArr,
                                audioServiceTransports.size());
          for (unsigned int i = 0; i < audioServiceTransports.size(); i++) {
            LOG4CXX_TRACE(logger_,
                          "Adding " << audioServiceTransports[i]
                                    << " to audioServiceTransports parameter "
                                       "of StartSessionAck");
            bson_array_add_int32(&audioServiceTransportsArr,
                                 audioServiceTransports[i]);
          }
          bson_object_put_array(&params,
                                strings::audio_service_transports,
                                &audioServiceTransportsArr);

          BsonArray videoServiceTransportsArr;
          bson_array_initialize(&videoServiceTransportsArr,
                                videoServiceTransports.size());
          for (unsigned int i = 0; i < videoServiceTransports.size(); i++) {
            LOG4CXX_TRACE(logger_,
                          "Adding " << videoServiceTransports[i]
                                    << " to videoServiceTransports parameter "
                                       "of StartSessionAck");
            bson_array_add_int32(&videoServiceTransportsArr,
                                 videoServiceTransports[i]);
          }
          bson_object_put_array(&params,
                                strings::video_service_transports,
                                &videoServiceTransportsArr);

          if (settings_.multiple_transports_enabled()) {
            send_transport_update_event = true;
          } else {
            LOG4CXX_TRACE(
                logger_,
                "Multiple transports feature is disabled by configuration");
            // In this case, we must remember that this session will never have
            // a secondary transport.
            connection_handler_.SetSecondaryTransportID(session_id,
                                                        DISABLED_SECONDARY);
          }
        } else {
          LOG4CXX_WARN(
              logger_,
              "Failed to set up secondary transport and service type params");
          connection_handler_.SetSecondaryTransportID(session_id,
                                                      DISABLED_SECONDARY);
        }
      } else {
        LOG4CXX_INFO(logger_, "Older protocol version. No multiple transports");
        connection_handler_.SetSecondaryTransportID(session_id,
                                                    DISABLED_SECONDARY);
      }
    }
    uint8_t* payloadBytes = bson_object_to_bytes(&params);
    ptr->set_data(payloadBytes, bson_object_size(&params));
    free(payloadBytes);
  } else {
    set_hash_id(hash_id, *ptr);
  }

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_DEBUG(logger_,
                "SendStartSessionAck() for connection "
                    << connection_id << " for service_type "
                    << static_cast<int32_t>(service_type) << " session_id "
                    << static_cast<int32_t>(session_id) << " protection "
                    << (protection ? "ON" : "OFF"));

  if (send_transport_update_event) {
    // Wait until the StartService ACK has been processed for sending.
    // The TransportUpdateEvent has a higher priority, being that it's
    // a SERVICE_TYPE_CONTROL message. (The ACK is SERVICE_TYPE_RPC.)
    LOG4CXX_DEBUG(logger_, "Waiting for the MessageToMobile queue to be empty");
    raw_ford_messages_to_mobile_.WaitDumpQueue();
    LOG4CXX_DEBUG(logger_, "Sending the TransportUpdate event");
    SendTransportUpdateEvent(connection_id, session_id);
  }
}

void ProtocolHandlerImpl::SendStartSessionNAck(ConnectionID connection_id,
                                               uint8_t session_id,
                                               uint8_t protocol_version,
                                               uint8_t service_type) {
  std::vector<std::string> rejectedParams;
  SendStartSessionNAck(connection_id,
                       session_id,
                       protocol_version,
                       service_type,
                       rejectedParams);
}

void ProtocolHandlerImpl::SendStartSessionNAck(
    ConnectionID connection_id,
    uint8_t session_id,
    uint8_t protocol_version,
    uint8_t service_type,
    std::vector<std::string>& rejectedParams) {
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

  uint8_t maxProtocolVersion = SupportedSDLProtocolVersion();

  if (protocol_version >= PROTOCOL_VERSION_5 &&
      maxProtocolVersion >= PROTOCOL_VERSION_5 && rejectedParams.size() > 0) {
    BsonObject payloadObj;
    bson_object_initialize_default(&payloadObj);
    BsonArray rejectedParamsArr;
    bson_array_initialize(&rejectedParamsArr, rejectedParams.size());
    for (std::string param : rejectedParams) {
      char paramPtr[255];
      strncpy(paramPtr, param.c_str(), 255);
      bson_array_add_string(&rejectedParamsArr, paramPtr);
    }
    bson_object_put_array(
        &payloadObj, strings::rejected_params, &rejectedParamsArr);
    uint8_t* payloadBytes = bson_object_to_bytes(&payloadObj);
    ptr->set_data(payloadBytes, bson_object_size(&payloadObj));
    free(payloadBytes);
    bson_object_deinitialize(&payloadObj);
  }

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
  std::vector<std::string> rejectedParams;
  SendEndSessionNAck(connection_id,
                     session_id,
                     protocol_version,
                     service_type,
                     rejectedParams);
}

void ProtocolHandlerImpl::SendEndSessionNAck(
    ConnectionID connection_id,
    uint32_t session_id,
    uint8_t protocol_version,
    uint8_t service_type,
    std::vector<std::string>& rejectedParams) {
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

  uint8_t maxProtocolVersion = SupportedSDLProtocolVersion();

  if (protocol_version >= PROTOCOL_VERSION_5 &&
      maxProtocolVersion >= PROTOCOL_VERSION_5 && rejectedParams.size() > 0) {
    BsonObject payloadObj;
    bson_object_initialize_default(&payloadObj);
    BsonArray rejectedParamsArr;
    bson_array_initialize(&rejectedParamsArr, rejectedParams.size());
    for (std::string param : rejectedParams) {
      char paramPtr[255];
      strncpy(paramPtr, param.c_str(), 255);
      bson_array_add_string(&rejectedParamsArr, paramPtr);
    }
    bson_object_put_array(
        &payloadObj, strings::rejected_params, &rejectedParamsArr);
    uint8_t* payloadBytes = bson_object_to_bytes(&payloadObj);
    ptr->set_data(payloadBytes, bson_object_size(&payloadObj));
    free(payloadBytes);
    bson_object_deinitialize(&payloadObj);
  }

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

void ProtocolHandlerImpl::SendEndServicePrivate(int32_t primary_connection_id,
                                                int32_t connection_id,
                                                uint8_t session_id,
                                                uint8_t service_type) {
  LOG4CXX_AUTO_TRACE(logger_);

  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          primary_connection_id, session_id, protocol_version)) {
    LOG4CXX_TRACE(logger_,
                  "SendEndServicePrivate using protocol version "
                      << static_cast<int32_t>(protocol_version));
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
                  "SendEndServicePrivate() for connection "
                      << primary_connection_id << " for service_type "
                      << static_cast<int>(service_type)
                      << " service connection " << connection_id
                      << " session_id " << static_cast<int32_t>(session_id));
  } else {
    LOG4CXX_WARN(
        logger_,
        "SendEndServicePrivate is failed connection or session does not exist");
  }
}

void ProtocolHandlerImpl::SendEndSession(int32_t connection_id,
                                         uint8_t session_id) {
  // A session is always associated with a primary connection ID
  SendEndServicePrivate(
      connection_id, connection_id, session_id, SERVICE_TYPE_RPC);
}

void ProtocolHandlerImpl::SendEndService(int32_t primary_connection_id,
                                         int32_t connection_id,
                                         uint8_t session_id,
                                         uint8_t service_type) {
  SendEndServicePrivate(
      primary_connection_id, connection_id, session_id, service_type);
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

void ProtocolHandlerImpl::SendTransportUpdateEvent(ConnectionID connection_id,
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
                                             FRAME_DATA_TRANSPORT_EVENT_UPDATE,
                                             session_id,
                                             0,
                                             message_counters_[session_id]++));

    BsonObject payload_obj;
    bson_object_initialize_default(&payload_obj);

    int32_t tcp_port = atoi(tcp_port_.c_str());
    char tcp_ip_address[INET6_ADDRSTRLEN + 1];
    if (tcp_enabled_ && (tcp_port != 0)) {
      strncpy(tcp_ip_address, tcp_ip_address_.c_str(), INET6_ADDRSTRLEN);
      tcp_ip_address[INET6_ADDRSTRLEN] = '\0';
      bson_object_put_string(
          &payload_obj, strings::tcp_ip_address, tcp_ip_address);
      bson_object_put_int32(&payload_obj, strings::tcp_port, tcp_port);
    } else {
      tcp_ip_address[0] = '\0';
      bson_object_put_string(
          &payload_obj, strings::tcp_ip_address, tcp_ip_address);
      // omit TCP port number
    }
    LOG4CXX_INFO(logger_,
                 "SendTransportUpdateEvent IP address: "
                     << tcp_ip_address << " Port: " << tcp_port);

    uint8_t* payloadBytes = bson_object_to_bytes(&payload_obj);
    ptr->set_data(payloadBytes, bson_object_size(&payload_obj));
    free(payloadBytes);
    bson_object_deinitialize(&payload_obj);

    raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, false));

    LOG4CXX_DEBUG(logger_,
                  "SendTransportUpdateEvent() for connection "
                      << connection_id << " for session "
                      << static_cast<int32_t>(session_id));
  } else {
    LOG4CXX_WARN(logger_,
                 "SendTransportUpdateEvent is failed connection or session "
                 "does not exist");
  }
}

RESULT_CODE ProtocolHandlerImpl::SendRegisterSecondaryTransportAck(
    ConnectionID connection_id,
    ConnectionID primary_transport_connection_id,
    uint8_t session_id) {
  LOG4CXX_AUTO_TRACE(logger_);

  // acquire the protocol version from primary transport
  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          primary_transport_connection_id, session_id, protocol_version)) {
    ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(
        connection_id,
        protocol_version,
        PROTECTION_OFF,
        FRAME_TYPE_CONTROL,
        SERVICE_TYPE_CONTROL,
        FRAME_DATA_REGISTER_SECONDARY_TRANSPORT_ACK,
        session_id,
        0u,
        2));

    raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, false));
    return RESULT_OK;
  }
  LOG4CXX_WARN(logger_,
               "RegisterSecondaryTransportAck is failed connection or session "
               "does not exist");
  return RESULT_FAIL;
}

RESULT_CODE ProtocolHandlerImpl::SendRegisterSecondaryTransportNAck(
    ConnectionID connection_id,
    ConnectionID primary_transport_connection_id,
    uint8_t session_id,
    BsonObject* reason) {
  LOG4CXX_AUTO_TRACE(logger_);

  // If mobile sends an invalid session ID and we cannot find out the Connection
  // ID of primary transport, then we use version 5. (The multiple-transports
  // feature is added in 5.1.0.)
  uint8_t protocol_version = PROTOCOL_VERSION_5;
  if (primary_transport_connection_id > 0) {
    // acquire the protocol version from primary transport
    if (!session_observer_.ProtocolVersionUsed(
            primary_transport_connection_id, session_id, protocol_version)) {
      LOG4CXX_WARN(logger_,
                   "Failed to acquire protocol version for "
                   "RegisterSecondaryTransportNAck");
      return RESULT_FAIL;
    }
  }

  ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(
      connection_id,
      protocol_version,
      PROTECTION_OFF,
      FRAME_TYPE_CONTROL,
      SERVICE_TYPE_CONTROL,
      FRAME_DATA_REGISTER_SECONDARY_TRANSPORT_NACK,
      session_id,
      0u,
      2));

  if (reason) {
    uint8_t* payloadBytes = bson_object_to_bytes(reason);
    ptr->set_data(payloadBytes, bson_object_size(reason));
    free(payloadBytes);
  }

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));
  return RESULT_OK;
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
  size_t max_frame_size = get_settings().maximum_payload_size();
  size_t frame_size = MAXIMUM_FRAME_DATA_V2_SIZE;
  switch (message->protocol_version()) {
    case PROTOCOL_VERSION_3:
    case PROTOCOL_VERSION_4:
      frame_size = max_frame_size > MAXIMUM_FRAME_DATA_V2_SIZE
                       ? max_frame_size
                       : MAXIMUM_FRAME_DATA_V2_SIZE;
      break;
    case PROTOCOL_VERSION_5:
      max_frame_size =
          protocol_header_validator_.max_payload_size_by_service_type(
              ServiceTypeFromByte(message->service_type()));
      frame_size = max_frame_size > MAXIMUM_FRAME_DATA_V2_SIZE
                       ? max_frame_size
                       : MAXIMUM_FRAME_DATA_V2_SIZE;
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

void ProtocolHandlerImpl::OnUnexpectedDisconnect(
    const transport_manager::ConnectionUID connection_id,
    const transport_manager::CommunicationError& error) {
  OnConnectionClosed(connection_id);
}

void ProtocolHandlerImpl::NotifyOnFailedHandshake() {
  LOG4CXX_AUTO_TRACE(logger_);
  security_manager_->NotifyListenersOnHandshakeFailed();
}

void ProtocolHandlerImpl::OnTransportConfigUpdated(
    const transport_manager::transport_adapter::TransportConfig& configs) {
  LOG4CXX_AUTO_TRACE(logger_);

  transport_manager::transport_adapter::TransportConfig::const_iterator it =
      configs.find(TC_ENABLED);
  if (configs.end() == it) {
    LOG4CXX_WARN(logger_, "No enabled field in OnTransportConfigUpdated");
    return;
  }

  bool tcp_enabled = (0 == strcmp("true", it->second.c_str()));
  std::string tcp_port;

  if (tcp_enabled) {
    it = configs.find(TC_TCP_PORT);
    if (configs.end() == it) {
      LOG4CXX_WARN(logger_, "No port field in OnTransportConfigUpdated");
      return;
    }
    tcp_port = it->second;

    it = configs.find(TC_TCP_IP_ADDRESS);
    if (configs.end() == it) {
      LOG4CXX_WARN(logger_, "No IP address field in OnTransportConfigUpdated");
      return;
    }
    tcp_enabled_ = true;
    tcp_port_ = tcp_port;
    tcp_ip_address_ = it->second;
  } else {
    tcp_enabled_ = false;
    tcp_port_.clear();
    tcp_ip_address_.clear();
  }

  LOG4CXX_INFO(logger_,
               "OnTransportConfigUpdated: new config enabled is "
                   << tcp_enabled_ << ". Port is " << tcp_port_
                   << ". IP Address is " << tcp_ip_address_);

  // Walk the SessionConnection map and find all sessions that need a
  // TransportUpdate Event. Sessions flagged with DISABLED_SECONDARY in their
  // secondary transport are ineligible for secondary transport, and
  // therefore don't get this event.
  NonConstDataAccessor<connection_handler::SessionConnectionMap>
      session_connection_map_accessor =
          connection_handler_.session_connection_map();
  connection_handler::SessionConnectionMap& session_connection_map =
      session_connection_map_accessor.GetData();
  connection_handler::SessionConnectionMap::iterator itr =
      session_connection_map.begin();
  while (itr != session_connection_map.end()) {
    const connection_handler::SessionTransports st = itr->second;
    LOG4CXX_INFO(logger_,
                 "OnTransportConfigUpdated found session "
                     << itr->first << " with primary connection  "
                     << st.primary_transport << " and secondary connection "
                     << st.secondary_transport);
    if (st.secondary_transport != DISABLED_SECONDARY) {
      SendTransportUpdateEvent(st.primary_transport, itr->first);
    }
    itr++;
  }
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

  // Replace a potential secondary transport ID in the packet with the primary
  // transport ID
  const connection_handler::SessionTransports st =
      connection_handler_.GetSessionTransports(packet->session_id());
  if (st.primary_transport != 0) {
    packet->set_connection_id(st.primary_transport);
  }
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

  // Replace a potential secondary transport ID in the packet with the primary
  // transport ID
  const connection_handler::SessionTransports st =
      connection_handler_.GetSessionTransports(packet->session_id());
  if (st.primary_transport != 0) {
    packet->set_connection_id(st.primary_transport);
  }

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
      return HandleControlMessageStartSession(packet);
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
    case FRAME_DATA_REGISTER_SECONDARY_TRANSPORT: {
      LOG4CXX_TRACE(logger_, "FrameData: RegisterSecondaryTransport");
      return HandleControlMessageRegisterSecondaryTransport(packet);
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
  if (packet.protocol_version() >= PROTOCOL_VERSION_5) {
    BsonObject obj = bson_object_from_bytes(packet.data());
    const uint32_t hash_id =
        (uint32_t)bson_object_get_int32(&obj, strings::hash_id);
    bson_object_deinitialize(&obj);
    return hash_id;
  } else {
    const uint32_t hash_be = *(reinterpret_cast<uint32_t*>(packet.data()));
    const uint32_t hash_le = BE_TO_LE32(hash_be);

    // null hash is wrong hash value
    return hash_le == HASH_ID_NOT_SUPPORTED ? HASH_ID_WRONG : hash_le;
  }
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageEndSession(
    const ProtocolPacket& packet) {
  LOG4CXX_AUTO_TRACE(logger_);

  const uint8_t current_session_id = packet.session_id();
  uint32_t hash_id = get_hash_id(packet);
  const ServiceType service_type = ServiceTypeFromByte(packet.service_type());

  const ConnectionID connection_id = packet.connection_id();
  const uint32_t session_key = session_observer_.OnSessionEndedCallback(
      connection_id, current_session_id, &hash_id, service_type);

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
    if (packet.protocol_version() >= PROTOCOL_VERSION_5) {
      std::vector<std::string> rejectedParams;
      if (hash_id == protocol_handler::HASH_ID_WRONG) {
        rejectedParams.push_back(std::string(strings::hash_id));
      }
      SendEndSessionNAck(connection_id,
                         current_session_id,
                         packet.protocol_version(),
                         service_type,
                         rejectedParams);
    } else {
      SendEndSessionNAck(connection_id,
                         current_session_id,
                         packet.protocol_version(),
                         service_type);
    }
  }
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageEndServiceACK(
    const ProtocolPacket& packet) {
  LOG4CXX_AUTO_TRACE(logger_);

  const uint8_t current_session_id = packet.session_id();
  uint32_t hash_id = get_hash_id(packet);
  const ServiceType service_type = ServiceTypeFromByte(packet.service_type());
  const ConnectionID connection_id = packet.connection_id();

  const uint32_t session_key = session_observer_.OnSessionEndedCallback(
      connection_id, current_session_id, &hash_id, service_type);

  if (0 == session_key) {
    LOG4CXX_WARN(logger_, "Refused to end service");
    return RESULT_FAIL;
  }

  return RESULT_OK;
}

// Suppress warning for deprecated method used within another deprecated method
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wdeprecated-declarations"
RESULT_CODE ProtocolHandlerImpl::HandleControlMessageStartSession(
    const ProtocolPacket& packet) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(
      logger_,
      "Protocol version:" << static_cast<int>(packet.protocol_version()));
  const ServiceType service_type = ServiceTypeFromByte(packet.service_type());

#ifdef ENABLE_SECURITY
  const bool protection =
      // Protocol version 1 is not support protection
      (packet.protocol_version() > PROTOCOL_VERSION_1)
          ? packet.protection_flag()
          : false;
#else
  const bool protection = false;
#endif  // ENABLE_SECURITY

  uint32_t hash_id;
  const ConnectionID connection_id = packet.connection_id();
  const uint32_t session_id = session_observer_.OnSessionStartedCallback(
      connection_id, packet.session_id(), service_type, protection, &hash_id);

  if (0 == session_id) {
    LOG4CXX_WARN(logger_,
                 "Refused by session_observer to create service "
                     << static_cast<int32_t>(service_type) << " type.");
    SendStartSessionNAck(connection_id,
                         packet.session_id(),
                         packet.protocol_version(),
                         packet.service_type());
    return RESULT_OK;
  }

#ifdef ENABLE_SECURITY
  // for packet is encrypted and security plugin is enable
  if (protection && security_manager_) {
    const uint32_t connection_key =
        session_observer_.KeyFromPair(connection_id, session_id);

    security_manager::SSLContext* ssl_context =
        security_manager_->CreateSSLContext(
            connection_key, security_manager::SecurityManager::kUseExisting);
    if (!ssl_context) {
      const std::string error("CreateSSLContext failed");
      LOG4CXX_ERROR(logger_, error);
      security_manager_->SendInternalError(
          connection_key,
          security_manager::SecurityManager::ERROR_INTERNAL,
          error);
      // Start service without protection
      SendStartSessionAck(connection_id,
                          session_id,
                          packet.protocol_version(),
                          hash_id,
                          packet.service_type(),
                          PROTECTION_OFF);
      return RESULT_OK;
    }
    ProtocolPacket::ProtocolVersion* fullVersion;
    std::vector<std::string> rejectedParams(0, std::string(""));
    // Can't check protocol_version because the first packet is v1, but there
    // could still be a payload, in which case we can get the real protocol
    // version
    if (packet.service_type() == kRpc && packet.data_size() != 0) {
      BsonObject obj = bson_object_from_bytes(packet.data());
      fullVersion = new ProtocolPacket::ProtocolVersion(
          std::string(bson_object_get_string(&obj, "protocolVersion")));
      bson_object_deinitialize(&obj);
      // Constructed payloads added in Protocol v5
      if (fullVersion->majorVersion < PROTOCOL_VERSION_5) {
        rejectedParams.push_back(std::string("protocolVersion"));
      }
    } else {
      fullVersion = new ProtocolPacket::ProtocolVersion();
    }
    if (!rejectedParams.empty()) {
      SendStartSessionNAck(connection_id,
                           packet.session_id(),
                           packet.protocol_version(),
                           packet.service_type(),
                           rejectedParams);
    } else if (ssl_context->IsInitCompleted()) {
      // mark service as protected
      session_observer_.SetProtectionFlag(connection_key, service_type);
      // Start service as protected with current SSLContext
      SendStartSessionAck(connection_id,
                          session_id,
                          packet.protocol_version(),
                          hash_id,
                          packet.service_type(),
                          PROTECTION_ON,
                          *fullVersion);
    } else {
      security_manager_->AddListener(
          new HandshakeHandler(*this,
                               session_observer_,
                               connection_key,
                               connection_id,
                               session_id,
                               packet.protocol_version(),
                               hash_id,
                               service_type,
                               get_settings().force_protected_service(),
                               false,
                               *fullVersion,
                               NULL));
      if (!ssl_context->IsHandshakePending()) {
        // Start handshake process
        security_manager_->StartHandshake(connection_key);
      }
    }
    delete fullVersion;
    LOG4CXX_DEBUG(logger_,
                  "Protection establishing for connection "
                      << connection_key << " is in progress");
    return RESULT_OK;
  }
#endif  // ENABLE_SECURITY
  if (packet.service_type() == kRpc && packet.data_size() != 0) {
    BsonObject obj = bson_object_from_bytes(packet.data());
    ProtocolPacket::ProtocolVersion fullVersion(
        bson_object_get_string(&obj, "protocolVersion"));
    bson_object_deinitialize(&obj);

    if (fullVersion.majorVersion >= PROTOCOL_VERSION_5) {
      // Start service without protection
      SendStartSessionAck(connection_id,
                          session_id,
                          packet.protocol_version(),
                          hash_id,
                          packet.service_type(),
                          PROTECTION_OFF,
                          fullVersion);
    } else {
      std::vector<std::string> rejectedParams(1,
                                              std::string("protocolVersion"));
      SendStartSessionNAck(connection_id,
                           packet.session_id(),
                           packet.protocol_version(),
                           packet.service_type(),
                           rejectedParams);
    }

  } else {
    // Start service without protection
    SendStartSessionAck(connection_id,
                        session_id,
                        packet.protocol_version(),
                        hash_id,
                        packet.service_type(),
                        PROTECTION_OFF);
  }
  return RESULT_OK;
}
#pragma GCC diagnostic pop

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageStartSession(
    const ProtocolFramePtr packet) {
  LOG4CXX_AUTO_TRACE(logger_);
  LOG4CXX_DEBUG(
      logger_,
      "Protocol version:" << static_cast<int>(packet->protocol_version()));
  const ServiceType service_type = ServiceTypeFromByte(packet->service_type());
  BsonObject bson_obj;
  if (packet->data() != NULL) {
    bson_obj = bson_object_from_bytes(packet->data());
  } else {
    bson_object_initialize_default(&bson_obj);
  }

#ifdef ENABLE_SECURITY
  const uint8_t protocol_version = packet->protocol_version();
  const bool protection =
      // Protocol version 1 does not support protection
      (protocol_version > PROTOCOL_VERSION_1) ? packet->protection_flag()
                                              : false;
#else
  const bool protection = false;
#endif  // ENABLE_SECURITY

  const ConnectionID connection_id = packet->connection_id();
  const uint8_t session_id = packet->session_id();

  LOG4CXX_INFO(logger_,
               "StartSession ID " << static_cast<int>(session_id)
                                  << " and Connection ID "
                                  << static_cast<int>(connection_id));

  {
    sync_primitives::AutoLock auto_lock(start_session_frame_map_lock_);
    start_session_frame_map_[std::make_pair(connection_id, session_id)] =
        packet;
  }

  session_observer_.OnSessionStartedCallback(
      connection_id, packet->session_id(), service_type, protection, &bson_obj);
  bson_object_deinitialize(&bson_obj);

  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageRegisterSecondaryTransport(
    const ProtocolFramePtr packet) {
  LOG4CXX_AUTO_TRACE(logger_);
  const uint8_t session_id = packet->session_id();
  const ConnectionID connection_id = packet->connection_id();
  ConnectionID primary_connection_id = 0;

  LOG4CXX_INFO(logger_,
               "RegisterSecondaryTransport ID "
                   << static_cast<int>(session_id) << " and Connection ID "
                   << static_cast<int>(connection_id));

  if (connection_handler_.OnSecondaryTransportStarted(
          primary_connection_id, connection_id, session_id)) {
    SendRegisterSecondaryTransportAck(
        connection_id, primary_connection_id, session_id);
  } else {
    char reason[256];
    BsonObject registerSecondaryTransportNackObj;
    bson_object_initialize_default(&registerSecondaryTransportNackObj);
    if (0 == session_id) {
      strncpy(reason,
              "RegisterSecondaryTransport MUST include a non-zero session ID",
              255);
    } else if (primary_connection_id == 0) {
      strncpy(reason, "RegisterSecondaryTransport session ID not found", 255);
    } else {
      strncpy(
          reason,
          "RegisterSecondaryTransport session ID has already been registered",
          255);
    }
    bson_object_put_string(
        &registerSecondaryTransportNackObj, strings::reason, reason);
    SendRegisterSecondaryTransportNAck(connection_id,
                                       primary_connection_id,
                                       session_id,
                                       &registerSecondaryTransportNackObj);
    bson_object_deinitialize(&registerSecondaryTransportNackObj);
  }

  return RESULT_OK;
}

void ProtocolHandlerImpl::NotifySessionStartedResult(
    int32_t connection_id,
    uint8_t session_id,
    uint8_t generated_session_id,
    uint32_t hash_id,
    bool protection,
    std::vector<std::string>& rejected_params) {
  LOG4CXX_AUTO_TRACE(logger_);
  protocol_handler::SessionContext context(connection_id,
                                           connection_id,
                                           session_id,
                                           generated_session_id,
                                           ServiceType::kInvalidServiceType,
                                           hash_id,
                                           protection);
  NotifySessionStarted(context, rejected_params);
}

void ProtocolHandlerImpl::NotifySessionStarted(
    const SessionContext& context, std::vector<std::string>& rejected_params) {
  LOG4CXX_AUTO_TRACE(logger_);

  ProtocolFramePtr packet;
  {
    sync_primitives::AutoLock auto_lock(start_session_frame_map_lock_);
    StartSessionFrameMap::iterator it = start_session_frame_map_.find(
        std::make_pair(context.connection_id_, context.initial_session_id_));
    if (it == start_session_frame_map_.end()) {
      LOG4CXX_ERROR(logger_, "Cannot find Session Started packet");
      return;
    }
    packet = it->second;
    start_session_frame_map_.erase(it);
  }

  const ServiceType service_type = ServiceTypeFromByte(packet->service_type());
  const uint8_t protocol_version = packet->protocol_version();

  if (0 == context.new_session_id_) {
    LOG4CXX_WARN(logger_,
                 "Refused by session_observer to create service "
                     << static_cast<int32_t>(service_type) << " type.");
    SendStartSessionNAck(context.connection_id_,
                         packet->session_id(),
                         protocol_version,
                         packet->service_type(),
                         rejected_params);
    return;
  }

  std::shared_ptr<BsonObject> start_session_ack_params(
      new BsonObject(), [](BsonObject* obj) { bson_object_deinitialize(obj); });
  bson_object_initialize_default(start_session_ack_params.get());
  // when video service is successfully started, copy input parameters
  // ("width", "height", "videoProtocol", "videoCodec") to the ACK packet
  if (packet->service_type() == kMobileNav && packet->data() != NULL) {
    BsonObject req_param = bson_object_from_bytes(packet->data());
    BsonElement* element = NULL;

    if ((element = bson_object_get(&req_param, strings::height)) != NULL &&
        element->type == TYPE_INT32) {
      bson_object_put_int32(start_session_ack_params.get(),
                            strings::height,
                            bson_object_get_int32(&req_param, strings::height));
    }
    if ((element = bson_object_get(&req_param, strings::width)) != NULL &&
        element->type == TYPE_INT32) {
      bson_object_put_int32(start_session_ack_params.get(),
                            strings::width,
                            bson_object_get_int32(&req_param, strings::width));
    }
    char* protocol =
        bson_object_get_string(&req_param, strings::video_protocol);
    if (protocol != NULL) {
      bson_object_put_string(
          start_session_ack_params.get(), strings::video_protocol, protocol);
    }
    char* codec = bson_object_get_string(&req_param, strings::video_codec);
    if (codec != NULL) {
      bson_object_put_string(
          start_session_ack_params.get(), strings::video_codec, codec);
    }
    bson_object_deinitialize(&req_param);
  }

  std::shared_ptr<ProtocolPacket::ProtocolVersion> fullVersion;

  // Can't check protocol_version because the first packet is v1, but there
  // could still be a payload, in which case we can get the real protocol
  // version
  if (packet->service_type() == kRpc && packet->data() != NULL) {
    BsonObject request_params = bson_object_from_bytes(packet->data());
    char* version_param =
        bson_object_get_string(&request_params, strings::protocol_version);
    std::string version_string(version_param == NULL ? "" : version_param);
    fullVersion =
        std::make_shared<ProtocolPacket::ProtocolVersion>(version_string);
    // Constructed payloads added in Protocol v5
    if (fullVersion->majorVersion < PROTOCOL_VERSION_5) {
      rejected_params.push_back(std::string(strings::protocol_version));
    }
    bson_object_deinitialize(&request_params);
  } else {
    fullVersion = std::make_shared<ProtocolPacket::ProtocolVersion>();
  }

#ifdef ENABLE_SECURITY
  // for packet is encrypted and security plugin is enable
  if (context.is_protected_ && security_manager_) {
    const uint32_t connection_key = session_observer_.KeyFromPair(
        context.connection_id_, context.new_session_id_);

    std::shared_ptr<HandshakeHandler> handler =
        std::make_shared<HandshakeHandler>(*this,
                                           session_observer_,
                                           *fullVersion,
                                           context,
                                           packet->protocol_version(),
                                           start_session_ack_params);

    security_manager::SSLContext* ssl_context =
        security_manager_->CreateSSLContext(
            connection_key,
            security_manager::SecurityManager::ContextCreationStrategy::
                kUseExisting);
    if (!ssl_context) {
      const std::string error("CreateSSLContext failed");
      LOG4CXX_ERROR(logger_, error);
      security_manager_->SendInternalError(
          connection_key,
          security_manager::SecurityManager::ERROR_INTERNAL,
          error);

      handler->OnHandshakeDone(
          connection_key, security_manager::SSLContext::Handshake_Result_Fail);

      return;
    }

    if (!rejected_params.empty()) {
      SendStartSessionNAck(context.connection_id_,
                           packet->session_id(),
                           protocol_version,
                           packet->service_type(),
                           rejected_params);
    } else if (ssl_context->IsInitCompleted()) {
      // mark service as protected
      session_observer_.SetProtectionFlag(connection_key, service_type);
      // Start service as protected with current SSLContext
      SendStartSessionAck(context.connection_id_,
                          context.new_session_id_,
                          packet->protocol_version(),
                          context.hash_id_,
                          packet->service_type(),
                          PROTECTION_ON,
                          *fullVersion,
                          *start_session_ack_params);
    } else {
      LOG4CXX_DEBUG(logger_,
                    "Adding Handshake handler to listeners: " << handler.get());
      security_manager::SecurityManagerListener* listener =
          new HandshakeHandler(*handler);
      security_manager_->AddListener(listener);

      if (!ssl_context->IsHandshakePending()) {
        // Start handshake process
        security_manager_->StartHandshake(connection_key);

        if (!security_manager_->IsSystemTimeProviderReady()) {
          security_manager_->RemoveListener(listener);
          SendStartSessionNAck(context.connection_id_,
                               packet->session_id(),
                               protocol_version,
                               packet->service_type(),
                               rejected_params);
        }
      }
    }

    LOG4CXX_DEBUG(logger_,
                  "Protection establishing for connection "
                      << connection_key << " is in progress");
    return;
  }
#endif  // ENABLE_SECURITY
  if (rejected_params.empty()) {
    SendStartSessionAck(context.connection_id_,
                        context.new_session_id_,
                        packet->protocol_version(),
                        context.hash_id_,
                        packet->service_type(),
                        PROTECTION_OFF,
                        *fullVersion,
                        *start_session_ack_params);
  } else {
    SendStartSessionNAck(context.connection_id_,
                         packet->session_id(),
                         protocol_version,
                         packet->service_type(),
                         rejected_params);
  }
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
    if (protocol_version >= PROTOCOL_VERSION_3 &&
        protocol_version <= PROTOCOL_VERSION_5) {
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
  LOG4CXX_AUTO_TRACE(logger_);
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

  sync_primitives::AutoLock auto_lock(start_session_frame_map_lock_);
  start_session_frame_map_.clear();
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
      // For protocol v5 control frames could be protected
      (packet->frame_type() == FRAME_TYPE_CONTROL &&
       packet->protocol_version() < PROTOCOL_VERSION_5)) {
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

    uint32_t hash_id = packet->message_id();
    // Close session to prevent usage unprotected service/session
    session_observer_.OnSessionEndedCallback(
        packet->connection_id(), packet->session_id(), &hash_id, kRpc);
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

  bool shoud_not_decrypt;
  if (packet->protocol_version() >= PROTOCOL_VERSION_5) {
    // For v5 protocol control frames except StartService could be encrypted
    shoud_not_decrypt =
        !packet->protection_flag() || packet->service_type() == kControl ||
        (FRAME_TYPE_CONTROL == packet->frame_type() &&
         helpers::Compare<ServiceType, helpers::EQ, helpers::ONE>(
             static_cast<ServiceType>(packet->service_type()),
             kMobileNav,
             kAudio,
             kRpc));
  } else {
    // Control frames and data over control service shall be unprotected
    shoud_not_decrypt = !packet->protection_flag() ||
                        packet->service_type() == kControl ||
                        packet->frame_type() == FRAME_TYPE_CONTROL;
  }

  if (shoud_not_decrypt) {
    LOG4CXX_DEBUG(logger_, "Frame will not be decrypted");
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

    uint32_t hash_id = packet->message_id();
    // Close session to prevent usage unprotected service/session
    session_observer_.OnSessionEndedCallback(
        packet->connection_id(), packet->session_id(), &hash_id, kRpc);
    return RESULT_ENCRYPTION_FAILED;
  }
  LOG4CXX_DEBUG(logger_,
                "Decrypted " << packet->data_size() << " bytes to "
                             << out_data_size << " bytes");
  DCHECK(out_data);
  DCHECK(out_data_size);
  // Special handling for decrypted FIRST_FRAME
  if (packet->frame_type() == FRAME_TYPE_FIRST && packet->protection_flag()) {
    packet->HandleRawFirstFrameData(out_data);
    return RESULT_OK;
  }
  packet->set_data(out_data, out_data_size);
  return RESULT_OK;
}
#endif  // ENABLE_SECURITY

void ProtocolHandlerImpl::SendFramesNumber(uint32_t connection_key,
                                           int32_t number_of_frames) {
  LOG4CXX_DEBUG(
      logger_, "SendFramesNumber MobileNaviAck for session " << connection_key);

  transport_manager::ConnectionUID connection_id = 0;
  uint8_t session_id = 0;
  session_observer_.PairFromKey(connection_key, &connection_id, &session_id);
  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          connection_id, session_id, protocol_version)) {
    if (protocol_version > PROTOCOL_VERSION_1 &&
        protocol_version < PROTOCOL_VERSION_5) {
      ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(
          connection_id,
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
    }
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
  return get_settings().max_supported_protocol_version();
}

const impl::TransportTypes transportTypes = {
    std::make_pair(std::string("AOA_USB"),
                   impl::TransportDescription("USB", false, true)),
    std::make_pair(std::string("SPP_BLUETOOTH"),
                   impl::TransportDescription("Bluetooth", false, true)),
    std::make_pair(std::string("IAP_BLUETOOTH"),
                   impl::TransportDescription("Bluetooth", true, false)),
    std::make_pair(std::string("IAP_USB"),
                   impl::TransportDescription("USB", true, false)),
    std::make_pair(std::string("TCP_WIFI"),
                   impl::TransportDescription("WiFi", true, true)),
    std::make_pair(std::string("IAP_USB_HOST_MODE"),
                   impl::TransportDescription("USB", true, false)),
    std::make_pair(std::string("IAP_USB_DEVICE_MODE"),
                   impl::TransportDescription("USB", true, false)),
    std::make_pair(std::string("IAP_CARPLAY"),
                   impl::TransportDescription("WiFi", true, false))};

const impl::TransportDescription
ProtocolHandlerImpl::GetTransportTypeFromConnectionType(
    const std::string& connection_type) const {
  impl::TransportDescription result =
      impl::TransportDescription("", false, false);
  impl::TransportTypes::const_iterator it =
      transportTypes.find(connection_type);
  if (it != transportTypes.end()) {
    result = it->second;
  } else {
    LOG4CXX_ERROR(logger_, "Unknown connection type " << connection_type);
  }

  return result;
}

const bool ProtocolHandlerImpl::ParseSecondaryTransportConfiguration(
    const ConnectionID connection_id,
    std::vector<std::string>& secondaryTransports,
    std::vector<int32_t>& audioServiceTransports,
    std::vector<int32_t>& videoServiceTransports) const {
  LOG4CXX_AUTO_TRACE(logger_);
  std::vector<std::string> secondary_transport_types;

  // First discover what the connection type of the primary transport is
  // and look up the allowed secondary transports for that primary transport
  const std::string connection_type =
      session_observer_.TransportTypeProfileStringFromConnHandle(connection_id);
  const impl::TransportDescription td =
      GetTransportTypeFromConnectionType(connection_type);
  if (settings_.multiple_transports_enabled()) {
    if (td.transport_type_ == "USB") {
      secondary_transport_types = settings_.secondary_transports_for_usb();
    } else if (td.transport_type_ == "Bluetooth") {
      secondary_transport_types =
          settings_.secondary_transports_for_bluetooth();
    } else if (td.transport_type_ == "WiFi") {
      secondary_transport_types = settings_.secondary_transports_for_wifi();
    } else {
      LOG4CXX_ERROR(
          logger_,
          "Bad or unknown device type in ParseSecondaryTransportConfiguration");
      return false;
    }
  }

  // Then, generate the "secondaryTransports" array for the StartSession ACK
  GenerateSecondaryTransportsForStartSessionAck(secondary_transport_types,
                                                td.ios_transport_,
                                                td.android_transport_,
                                                secondaryTransports);

  // Next, figure out which connections audio or video services are allowed on
  GenerateServiceTransportsForStartSessionAck(
      settings_.audio_service_transports(),
      connection_type,
      td.transport_type_,
      secondary_transport_types,
      audioServiceTransports);

  GenerateServiceTransportsForStartSessionAck(
      settings_.video_service_transports(),
      connection_type,
      td.transport_type_,
      secondary_transport_types,
      videoServiceTransports);

  return true;
}

void ProtocolHandlerImpl::GenerateSecondaryTransportsForStartSessionAck(
    const std::vector<std::string>& secondary_transport_types,
    bool device_is_ios,
    bool device_is_android,
    std::vector<std::string>& secondaryTransports) const {
  LOG4CXX_AUTO_TRACE(logger_);

  // Parse the "secondary_transport_types" vector (which comes from
  // smartDeviceLink.ini). For each entry in the vector, add an
  // appropriate string to the secondaryTransports
  std::vector<std::string>::const_iterator it =
      secondary_transport_types.begin();
  while (it != secondary_transport_types.end()) {
    const utils::custom_string::CustomString transport_type(*it);
    if (transport_type.CompareIgnoreCase("USB")) {
      if (device_is_ios) {
        LOG4CXX_TRACE(
            logger_,
            "Adding IAP_USB to secondaryTransports for StartSessionAck");
        secondaryTransports.push_back("IAP_USB");
      }
      if (device_is_android) {
        LOG4CXX_TRACE(
            logger_,
            "Adding AOA_USB to secondaryTransports for StartSessionAck");
        secondaryTransports.push_back("AOA_USB");
      }
    } else if (transport_type.CompareIgnoreCase("Bluetooth")) {
      if (device_is_ios) {
        LOG4CXX_TRACE(
            logger_,
            "Adding IAP_BLUETOOTH to secondaryTransports for StartSessionAck");
        secondaryTransports.push_back("IAP_BLUETOOTH");
      }
      if (device_is_android) {
        LOG4CXX_TRACE(
            logger_,
            "Adding SPP_BLUETOOTH to secondaryTransports for StartSessionAck");
        secondaryTransports.push_back("SPP_BLUETOOTH");
      }
    }
    if (transport_type.CompareIgnoreCase("WiFi")) {
      LOG4CXX_TRACE(
          logger_,
          "Adding TCP_WIFI to secondaryTransports for StartSessionAck");
      secondaryTransports.push_back("TCP_WIFI");
    }

    it++;
  }
}

void ProtocolHandlerImpl::GenerateServiceTransportsForStartSessionAck(
    const std::vector<std::string>& service_transports,
    const std::string& primary_connection_type,
    const std::string& primary_transport_type,
    const std::vector<std::string>& secondary_transport_types,
    std::vector<int32_t>& serviceTransports) const {
  LOG4CXX_AUTO_TRACE(logger_);

  if (service_transports.size() == 0) {
    LOG4CXX_TRACE(logger_,
                  "Empty Service Transports. Allowing service to run on both "
                  "connections");
    serviceTransports.push_back(1);
    serviceTransports.push_back(2);
  } else {
    bool fPrimaryAdded = false;
    bool fSecondaryAdded = false;
    std::vector<std::string>::const_iterator it = service_transports.begin();
    for (; it != service_transports.end(); it++) {
      const utils::custom_string::CustomString transport(*it);
      LOG4CXX_TRACE(logger_,
                    "Service Allowed to run on " << transport.c_str()
                                                 << " transport");

      if (!fPrimaryAdded &&
          (transport.CompareIgnoreCase(primary_connection_type.c_str()) ||
           (transport.CompareIgnoreCase("IAP_USB") &&
            primary_transport_type.compare("USB") == 0))) {
        LOG4CXX_TRACE(logger_, "Service allowed on primary transport");
        serviceTransports.push_back(1);
        fPrimaryAdded = true;
      }

      if (!fSecondaryAdded) {
        const utils::custom_string::CustomString transport_type(
            TransportTypeFromTransport(transport));
        std::vector<std::string>::const_iterator found =
            std::find_if(secondary_transport_types.begin(),
                         secondary_transport_types.end(),
                         [&](const std::string& secondary_transport_type) {
                           return transport_type.CompareIgnoreCase(
                               secondary_transport_type.c_str());
                         });
        if (found != secondary_transport_types.end()) {
          LOG4CXX_TRACE(logger_, "Service allowed on secondary transport");
          serviceTransports.push_back(2);
          fSecondaryAdded = true;
        }
      }

      if (fPrimaryAdded && fSecondaryAdded) {
        break;
      }
    }
  }
}

const std::string ProtocolHandlerImpl::TransportTypeFromTransport(
    const utils::custom_string::CustomString& transport) const {
  std::string transport_type;

  if (transport.CompareIgnoreCase("IAP_BLUETOOTH") ||
      transport.CompareIgnoreCase("SPP_BLUETOOTH")) {
    transport_type = "Bluetooth";
  } else if (transport.CompareIgnoreCase("IAP_USB") ||
             transport.CompareIgnoreCase("AOA_USB") ||
             transport.CompareIgnoreCase("IAP_USB_HOST_MODE") ||
             transport.CompareIgnoreCase("IAP_USB_DEVICE_MODE")) {
    transport_type = "USB";
  } else if (transport.CompareIgnoreCase("TCP_WIFI") ||
             transport.CompareIgnoreCase("IAP_CARPLAY")) {
    transport_type = "WiFi";
  }

  return transport_type;
}

}  // namespace protocol_handler

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
#include <bson_object.h>
#include <memory.h>
#include <protocol/bson_object_keys.h>
#include <algorithm>  // std::find

#include "connection_handler/connection_handler_impl.h"
#include "protocol/common.h"
#include "protocol_handler/session_observer.h"
#include "utils/byte_order.h"
#include "utils/helpers.h"

#ifdef ENABLE_SECURITY
#include "security_manager/security_manager.h"
#include "security_manager/ssl_context.h"
#endif  // ENABLE_SECURITY

namespace protocol_handler {

SDL_CREATE_LOG_VARIABLE("ProtocolHandler")

const size_t kStackSize = 131072;

const utils::SemanticVersion default_protocol_version(5, 4, 0);
const utils::SemanticVersion min_multiple_transports_version(5, 1, 0);
const utils::SemanticVersion min_cloud_app_version(5, 2, 0);
const utils::SemanticVersion min_reason_param_version(5, 3, 0);
const utils::SemanticVersion min_vehicle_data_version(5, 4, 0);

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
  SDL_LOG_AUTO_TRACE();
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
  protocol_header_validator_.set_max_protocol_version_supported(
      get_settings().max_supported_protocol_version());
  incoming_data_handler_.set_validator(&protocol_header_validator_);

  const size_t& message_frequency_count =
      get_settings().message_frequency_count();
  const size_t& message_frequency_time =
      get_settings().message_frequency_time();

  if (message_frequency_time > 0u && message_frequency_count > 0u) {
    message_meter_.set_time_range(message_frequency_time);
    SDL_LOG_DEBUG("Frequency meter is enabled ( "
                  << message_frequency_count << " per "
                  << message_frequency_time << " mSecond)");
  } else {
    SDL_LOG_WARN("Frequency meter is disabled");
  }

  const size_t& malformed_frequency_time =
      get_settings().malformed_frequency_time();
  const size_t& malformed_frequency_count =
      get_settings().message_frequency_time();

  if (get_settings().malformed_message_filtering()) {
    if (malformed_frequency_time > 0u && malformed_frequency_count > 0u) {
      malformed_message_meter_.set_time_range(malformed_frequency_time);
      SDL_LOG_DEBUG("Malformed frequency meter is enabled ( "
                    << malformed_frequency_count << " per "
                    << malformed_frequency_time << " mSecond)");
    } else {
      SDL_LOG_WARN("Malformed frequency meter is disabled");
    }
  } else {
    SDL_LOG_WARN(
        "Malformed message filtering is disabled."
        << "Connection will be close on first malformed message detection");
  }
  multiframe_builder_.set_waiting_timeout(
      get_settings().multiframe_waiting_timeout());
}

ProtocolHandlerImpl::~ProtocolHandlerImpl() {
  sync_primitives::AutoLock lock(protocol_observers_lock_);
  if (!protocol_observers_.empty()) {
    SDL_LOG_WARN(
        "Not all observers have unsubscribed"
        " from ProtocolHandlerImpl");
  }
}

void ProtocolHandlerImpl::AddProtocolObserver(ProtocolObserver* observer) {
  if (!observer) {
    SDL_LOG_ERROR("Invalid (NULL) pointer to IProtocolObserver.");
    return;
  }
  sync_primitives::AutoLock lock(protocol_observers_lock_);
  protocol_observers_.insert(observer);
}

void ProtocolHandlerImpl::RemoveProtocolObserver(ProtocolObserver* observer) {
  SDL_LOG_AUTO_TRACE();
  if (!observer) {
    SDL_LOG_ERROR("Invalid (NULL) pointer to IProtocolObserver.");
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
    SDL_LOG_DEBUG("Packet needs no hash data (protocol version less 2)");
    return;
  }
  SDL_LOG_DEBUG("Set hash_id 0x" << std::hex << hash_id << " to the packet 0x"
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
  SDL_LOG_AUTO_TRACE();
  utils::SemanticVersion fullVersion;
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
    utils::SemanticVersion& full_version) {
  SDL_LOG_AUTO_TRACE();

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

void ProtocolHandlerImpl::WriteProtocolVehicleData(
    BsonObject& params, const connection_handler::ProtocolVehicleData& data) {
  auto write_string_to_bson = [&params](const std::string& param_name,
                                        const std::string& param_value) {
    if (param_value.empty()) {
      return;
    }

    const uint16_t max_string_length = 500;
    char value_buffer[max_string_length + 1];  // extra byte for NULL symbol
    strncpy(value_buffer, param_value.c_str(), sizeof(value_buffer));
    value_buffer[max_string_length] = 0;

    if (bson_object_put_string(&params, param_name.c_str(), value_buffer)) {
      SDL_LOG_DEBUG("Parameter "
                    << param_name << " has been written to bson with value: "
                    << bson_object_get_string(&params, param_name.c_str()));
    } else {
      SDL_LOG_DEBUG("Failed to write parameter " << param_name
                                                 << " into bson structure");
    }
  };

  write_string_to_bson(strings::vehicle_make, data.vehicle_make);
  write_string_to_bson(strings::vehicle_model, data.vehicle_model);
  write_string_to_bson(strings::vehicle_model_year, data.vehicle_year);
  write_string_to_bson(strings::vehicle_trim, data.vehicle_trim);
  write_string_to_bson(strings::vehicle_system_software_version,
                       data.vehicle_system_software_version);
  write_string_to_bson(strings::vehicle_system_hardware_version,
                       data.vehicle_system_hardware_version);
}

void ProtocolHandlerImpl::SendStartSessionAck(
    ConnectionID connection_id,
    uint8_t session_id,
    uint8_t input_protocol_version,
    uint32_t hash_id,
    uint8_t service_type,
    bool protection,
    utils::SemanticVersion& full_version,
    BsonObject& params) {
  SDL_LOG_AUTO_TRACE();

  bool send_transport_update_event = false;

  uint8_t ack_protocol_version = SupportedSDLProtocolVersion();

  const bool proxy_supports_v5_protocol =
      input_protocol_version >= PROTOCOL_VERSION_5 ||
      (ServiceTypeFromByte(service_type) == kRpc &&
       full_version.major_version_ >= PROTOCOL_VERSION_5);

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
    UNUSED(mtu_written)
    SDL_LOG_DEBUG(
        "MTU parameter was written to bson params: "
        << mtu_written << "; Value: "
        << static_cast<int32_t>(bson_object_get_int64(&params, strings::mtu)));

    if (serviceTypeValue == kRpc) {
      SDL_LOG_DEBUG("Collecting protocol vehicle data");
      connection_handler::ProtocolVehicleData data;
      if (full_version >= min_vehicle_data_version &&
          connection_handler_.GetProtocolVehicleData(data)) {
        WriteProtocolVehicleData(params, data);
      }

      // Hash ID is only used in RPC case
      const bool hash_written = bson_object_put_int32(
          &params, strings::hash_id, static_cast<int32_t>(hash_id));
      UNUSED(hash_written);
      SDL_LOG_DEBUG("Hash parameter was written to bson params: "
                    << hash_written << "; Value: "
                    << static_cast<int32_t>(
                           bson_object_get_int32(&params, strings::hash_id)));

      // Minimum protocol version supported by both
      const utils::SemanticVersion& min_version =
          (full_version.major_version_ < PROTOCOL_VERSION_5)
              ? default_protocol_version
              : ((full_version < default_protocol_version)
                     ? full_version
                     : default_protocol_version);
      char protocol_version_string[256];
      strncpy(protocol_version_string, (min_version).toString().c_str(), 255);

      const bool protocol_ver_written = bson_object_put_string(
          &params, strings::protocol_version, protocol_version_string);
      UNUSED(protocol_ver_written);
      SDL_LOG_DEBUG(
          "Protocol version parameter was written to bson params: "
          << protocol_ver_written << "; Value: "
          << bson_object_get_string(&params, strings::protocol_version));

      SDL_LOG_INFO("Protocol Version String " << protocol_version_string);

      std::vector<std::string> secondaryTransports;
      std::vector<int32_t> audioServiceTransports;
      std::vector<int32_t> videoServiceTransports;
      if (min_version >= min_multiple_transports_version) {
        if (ParseSecondaryTransportConfiguration(connection_id,
                                                 secondaryTransports,
                                                 audioServiceTransports,
                                                 videoServiceTransports)) {
          SDL_LOG_DEBUG("Multiple transports are enabled.");
          BsonArray secondaryTransportsArr;
          bson_array_initialize(&secondaryTransportsArr,
                                secondaryTransports.size());
          for (unsigned int i = 0; i < secondaryTransports.size(); ++i) {
            char secondaryTransport[255];
            strncpy(secondaryTransport,
                    secondaryTransports[i].c_str(),
                    sizeof(secondaryTransport));
            secondaryTransport[sizeof(secondaryTransport) - 1] = '\0';
            SDL_LOG_DEBUG(
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
          for (unsigned int i = 0; i < audioServiceTransports.size(); ++i) {
            SDL_LOG_DEBUG("Adding " << audioServiceTransports[i]
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
          for (unsigned int i = 0; i < videoServiceTransports.size(); ++i) {
            SDL_LOG_DEBUG("Adding " << videoServiceTransports[i]
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
            SDL_LOG_DEBUG(
                "Multiple transports feature is disabled by configuration");
            // In this case, we must remember that this session will never have
            // a secondary transport.
            connection_handler_.SetSecondaryTransportID(session_id,
                                                        kDisabledSecondary);
          }
        } else {
          SDL_LOG_WARN(
              "Failed to set up secondary transport and service type params");
          connection_handler_.SetSecondaryTransportID(session_id,
                                                      kDisabledSecondary);
        }
      } else {
        SDL_LOG_INFO("Older protocol version. No multiple transports");
        connection_handler_.SetSecondaryTransportID(session_id,
                                                    kDisabledSecondary);
      }

      std::string policy_app_id =
          connection_handler_.GetCloudAppID(connection_id);
      if (min_version >= min_cloud_app_version && !policy_app_id.empty()) {
        sync_primitives::AutoLock lock(auth_token_map_lock_);
        auto it = auth_token_map_.find(policy_app_id);
        if (it != auth_token_map_.end()) {
          char auth_token[65536];
          strncpy(auth_token, it->second.c_str(), 65535);
          auth_token[sizeof(auth_token) - 1] = '\0';
          bson_object_put_string(&params, strings::auth_token, auth_token);
        }
      }
    }
    uint8_t* payload_bytes = bson_object_to_bytes(&params);
    ptr->set_data(payload_bytes, bson_object_size(&params));
    free(payload_bytes);
  } else {
    set_hash_id(hash_id, *ptr);
  }

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  const uint32_t connection_key =
      session_observer_.KeyFromPair(connection_id, session_id);
  connection_handler_.BindProtocolVersionWithSession(connection_key,
                                                     ack_protocol_version);

  SDL_LOG_DEBUG("SendStartSessionAck() for connection "
                << connection_id << " for service_type "
                << static_cast<int32_t>(service_type) << " session_id "
                << static_cast<int32_t>(session_id) << " protection "
                << (protection ? "ON" : "OFF"));

  if (send_transport_update_event) {
    // Wait until the StartService ACK has been processed for sending.
    // The TransportUpdateEvent has a higher priority, being that it's
    // a SERVICE_TYPE_CONTROL message. (The ACK is SERVICE_TYPE_RPC.)
    SDL_LOG_DEBUG("Waiting for the MessageToMobile queue to be empty");
    raw_ford_messages_to_mobile_.WaitDumpQueue();
    SDL_LOG_DEBUG("Sending the TransportUpdate event");
    SendTransportUpdateEvent(connection_id, session_id);
  }
}

void ProtocolHandlerImpl::SendStartSessionNAck(
    ConnectionID connection_id,
    uint8_t session_id,
    uint8_t protocol_version,
    uint8_t service_type,
    const std::string& reason,
    utils::SemanticVersion& full_version) {
  std::vector<std::string> rejectedParams;
  SendStartSessionNAck(connection_id,
                       session_id,
                       protocol_version,
                       service_type,
                       rejectedParams,
                       reason,
                       full_version);
}

void ProtocolHandlerImpl::SendStartSessionNAck(
    ConnectionID connection_id,
    uint8_t session_id,
    uint8_t protocol_version,
    uint8_t service_type,
    std::vector<std::string>& rejectedParams,
    const std::string& reason,
    utils::SemanticVersion& full_version) {
  SDL_LOG_AUTO_TRACE();

  if (!full_version.isValid()) {
    if (!session_observer_.ProtocolVersionUsed(
            connection_id, session_id, full_version)) {
      SDL_LOG_WARN("Connection: " << connection_id << " and/or session: "
                                  << session_id << "no longer exist(s).");
    }
  }

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
      maxProtocolVersion >= PROTOCOL_VERSION_5) {
    BsonObject payloadObj;
    bson_object_initialize_default(&payloadObj);
    if (rejectedParams.size() > 0) {
      BsonArray rejectedParamsArr;
      bson_array_initialize(&rejectedParamsArr, rejectedParams.size());
      for (std::string param : rejectedParams) {
        char paramPtr[256];
        strncpy(paramPtr, param.c_str(), sizeof(paramPtr));
        paramPtr[sizeof(paramPtr) - 1] = '\0';
        bson_array_add_string(&rejectedParamsArr, paramPtr);
      }
      bson_object_put_array(
          &payloadObj, strings::rejected_params, &rejectedParamsArr);
    }
    if (!reason.empty() && full_version >= min_reason_param_version) {
      bson_object_put_string(
          &payloadObj, strings::reason, const_cast<char*>(reason.c_str()));
    }
    uint8_t* payloadBytes = bson_object_to_bytes(&payloadObj);
    ptr->set_data(payloadBytes, bson_object_size(&payloadObj));
    free(payloadBytes);
    bson_object_deinitialize(&payloadObj);
  }

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  SDL_LOG_DEBUG("SendStartSessionNAck() for connection "
                << connection_id << " for service_type "
                << static_cast<int32_t>(service_type) << " session_id "
                << static_cast<int32_t>(session_id)
                << (reason.empty() ? "" : " reason \"" + reason + "\""));
}

void ProtocolHandlerImpl::SendEndSessionNAck(ConnectionID connection_id,
                                             uint32_t session_id,
                                             uint8_t protocol_version,
                                             uint8_t service_type,
                                             const std::string reason) {
  std::vector<std::string> rejectedParams;
  SendEndSessionNAck(connection_id,
                     session_id,
                     protocol_version,
                     service_type,
                     rejectedParams,
                     reason);
}

void ProtocolHandlerImpl::SendEndSessionNAck(
    ConnectionID connection_id,
    uint32_t session_id,
    uint8_t protocol_version,
    uint8_t service_type,
    std::vector<std::string>& rejectedParams,
    const std::string reason) {
  SDL_LOG_AUTO_TRACE();

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

  utils::SemanticVersion full_version;
  if (!session_observer_.ProtocolVersionUsed(
          connection_id, session_id, full_version)) {
    SDL_LOG_WARN("Connection: " << connection_id << " and/or session: "
                                << session_id << "no longer exist(s).");
    return;
  }

  if (protocol_version >= PROTOCOL_VERSION_5 &&
      maxProtocolVersion >= PROTOCOL_VERSION_5) {
    BsonObject payloadObj;
    bson_object_initialize_default(&payloadObj);

    if (rejectedParams.size() > 0) {
      BsonArray rejectedParamsArr;
      bson_array_initialize(&rejectedParamsArr, rejectedParams.size());
      for (std::string param : rejectedParams) {
        char paramPtr[256];
        strncpy(paramPtr, param.c_str(), sizeof(paramPtr));
        paramPtr[sizeof(paramPtr) - 1] = '\0';
        bson_array_add_string(&rejectedParamsArr, paramPtr);
      }
      bson_object_put_array(
          &payloadObj, strings::rejected_params, &rejectedParamsArr);
    }
    if (!reason.empty() && full_version >= min_reason_param_version) {
      bson_object_put_string(
          &payloadObj, strings::reason, const_cast<char*>(reason.c_str()));
    }
    uint8_t* payloadBytes = bson_object_to_bytes(&payloadObj);
    ptr->set_data(payloadBytes, bson_object_size(&payloadObj));
    free(payloadBytes);
    bson_object_deinitialize(&payloadObj);
  }

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  SDL_LOG_DEBUG("SendEndSessionNAck() for connection "
                << connection_id << " for service_type "
                << static_cast<int32_t>(service_type) << " session_id "
                << static_cast<int32_t>(session_id)
                << (reason.empty() ? "" : " reason \"" + reason + "\""));
}

SessionObserver& ProtocolHandlerImpl::get_session_observer() {
  return session_observer_;
}

void ProtocolHandlerImpl::SendEndSessionAck(ConnectionID connection_id,
                                            uint8_t session_id,
                                            uint8_t protocol_version,
                                            uint8_t service_type) {
  SDL_LOG_AUTO_TRACE();

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

  SDL_LOG_DEBUG("SendEndSessionAck() for connection "
                << connection_id << " for service_type "
                << static_cast<int32_t>(service_type) << " session_id "
                << static_cast<int32_t>(session_id));
}

void ProtocolHandlerImpl::SendEndServicePrivate(int32_t primary_connection_id,
                                                int32_t connection_id,
                                                uint8_t session_id,
                                                uint8_t service_type) {
  SDL_LOG_AUTO_TRACE();

  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          primary_connection_id, session_id, protocol_version)) {
    SDL_LOG_TRACE("SendEndServicePrivate using protocol version "
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
    SDL_LOG_DEBUG("SendEndServicePrivate() for connection "
                  << connection_id << " for service_type "
                  << static_cast<int>(service_type) << " primary connection "
                  << primary_connection_id << " session_id "
                  << static_cast<int32_t>(session_id));
  } else {
    SDL_LOG_WARN(
        "SendEndServicePrivate is failed connection or session does not exist");
  }
}

void ProtocolHandlerImpl::SendEndSession(int32_t connection_id,
                                         uint8_t session_id) {
  SDL_LOG_AUTO_TRACE();
  // A session is always associated with a primary connection ID
  SendEndServicePrivate(
      connection_id, connection_id, session_id, SERVICE_TYPE_RPC);
}

void ProtocolHandlerImpl::SendEndService(int32_t primary_connection_id,
                                         int32_t connection_id,
                                         uint8_t session_id,
                                         uint8_t service_type) {
  SDL_LOG_AUTO_TRACE();
  SendEndServicePrivate(
      primary_connection_id, connection_id, session_id, service_type);
}

RESULT_CODE ProtocolHandlerImpl::SendHeartBeatAck(ConnectionID connection_id,
                                                  uint8_t session_id,
                                                  uint32_t message_id) {
  SDL_LOG_AUTO_TRACE();

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
  SDL_LOG_WARN(
      "SendHeartBeatAck is failed connection or session does not exist");
  return RESULT_FAIL;
}

void ProtocolHandlerImpl::SendTransportUpdateEvent(ConnectionID connection_id,
                                                   uint8_t session_id) {
  SDL_LOG_AUTO_TRACE();

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
    SDL_LOG_INFO("SendTransportUpdateEvent IP address: "
                 << tcp_ip_address << " Port: " << tcp_port);

    uint8_t* payloadBytes = bson_object_to_bytes(&payload_obj);
    ptr->set_data(payloadBytes, bson_object_size(&payload_obj));
    free(payloadBytes);
    bson_object_deinitialize(&payload_obj);

    raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, false));

    SDL_LOG_DEBUG("SendTransportUpdateEvent() for connection "
                  << connection_id << " for session "
                  << static_cast<int32_t>(session_id));
  } else {
    SDL_LOG_WARN(
        "SendTransportUpdateEvent is failed connection or session "
        "does not exist");
  }
}

RESULT_CODE ProtocolHandlerImpl::SendRegisterSecondaryTransportAck(
    ConnectionID connection_id,
    ConnectionID primary_transport_connection_id,
    uint8_t session_id) {
  SDL_LOG_AUTO_TRACE();

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
  SDL_LOG_WARN(
      "RegisterSecondaryTransportAck is failed connection or session "
      "does not exist");
  return RESULT_FAIL;
}

RESULT_CODE ProtocolHandlerImpl::SendRegisterSecondaryTransportNAck(
    ConnectionID connection_id,
    ConnectionID primary_transport_connection_id,
    uint8_t session_id,
    BsonObject* reason) {
  SDL_LOG_AUTO_TRACE();

  // If mobile sends an invalid session ID and we cannot find out the Connection
  // ID of primary transport, then we use version 5. (The multiple-transports
  // feature is added in 5.1.0.)
  uint8_t protocol_version = PROTOCOL_VERSION_5;
  if (primary_transport_connection_id > 0) {
    // acquire the protocol version from primary transport
    if (!session_observer_.ProtocolVersionUsed(
            primary_transport_connection_id, session_id, protocol_version)) {
      SDL_LOG_WARN(
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
  SDL_LOG_AUTO_TRACE();
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
    SDL_LOG_DEBUG("SendHeartBeat finished successfully");
  } else {
    SDL_LOG_WARN(
        "SendHeartBeat is failed connection or session does not exist");
  }
}

void ProtocolHandlerImpl::SendMessageToMobileApp(const RawMessagePtr message,
                                                 bool needs_encryption,
                                                 bool final_message) {
#ifdef TELEMETRY_MONITOR
  const date_time::TimeDuration start_time = date_time::getCurrentTime();
#endif  // TELEMETRY_MONITOR
  SDL_LOG_AUTO_TRACE();
  if (!message) {
    SDL_LOG_ERROR("Invalid message for sending to mobile app is received.");
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
      SDL_LOG_DEBUG("Security set new optimal packet size " << frame_size);
    } else {
      SDL_LOG_ERROR(
          "Security could not return max block size, use the origin one");
    }
  }
  SDL_LOG_DEBUG("Optimal packet size is " << frame_size);
#endif  // ENABLE_SECURITY

  if (message->data_size() <= frame_size) {
    RESULT_CODE result = SendSingleFrameMessage(connection_handle,
                                                sessionID,
                                                message->protocol_version(),
                                                message->service_type(),
                                                message->data_size(),
                                                message->data(),
                                                needs_encryption,
                                                final_message);
    if (result != RESULT_OK) {
      SDL_LOG_ERROR("ProtocolHandler failed to send single frame message.");
    }
  } else {
    SDL_LOG_DEBUG("Message will be sent in multiple frames; max frame size is "
                  << frame_size);

    RESULT_CODE result = SendMultiFrameMessage(connection_handle,
                                               sessionID,
                                               message->protocol_version(),
                                               message->service_type(),
                                               message->data_size(),
                                               message->data(),
                                               frame_size,
                                               needs_encryption,
                                               final_message);
    if (result != RESULT_OK) {
      SDL_LOG_ERROR("ProtocolHandler failed to send multiframe messages.");
    }
  }
#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    auto metric = std::make_shared<PHTelemetryObserver::MessageMetric>();
    metric->message_id = message_id;
    metric->connection_key = message->connection_key();
    metric->raw_msg = message;
    metric_observer_->EndMessageProcess(metric);
  }
#endif
}

void ProtocolHandlerImpl::OnTMMessageReceived(const RawMessagePtr tm_message) {
  SDL_LOG_AUTO_TRACE();

  if (!tm_message) {
    SDL_LOG_ERROR("Invalid incoming message received in"
                  << " ProtocolHandler from Transport Manager.");
    return;
  }

  const uint32_t connection_key = tm_message->connection_key();
  SDL_LOG_TRACE("Received data from TM  with connection id "
                << connection_key << " msg data_size "
                << tm_message->data_size());

  RESULT_CODE result;
  size_t malformed_occurs = 0u;
  const ProtocolFramePtrList protocol_frames =
      incoming_data_handler_.ProcessData(
          *tm_message, result, &malformed_occurs);
  SDL_LOG_TRACE("Processed " << protocol_frames.size() << " frames");
  if (result != RESULT_OK) {
    if (result == RESULT_MALFORMED_OCCURS) {
      SDL_LOG_WARN("Malformed message occurs, connection id "
                   << connection_key);
      if (!get_settings().malformed_message_filtering()) {
        SDL_LOG_DEBUG("Malformed message filtering disabled");
        session_observer_.OnMalformedMessageCallback(connection_key);
        // For tracking only malformed occurrence check output
      } else {
        if (malformed_occurs > 0) {
          TrackMalformedMessage(connection_key, malformed_occurs);
        }
      }
    } else {
      SDL_LOG_ERROR("Incoming data processing failed.");
      transport_manager_.DisconnectForce(connection_key);
    }
  }

  for (ProtocolFramePtrList::const_iterator it = protocol_frames.begin();
       it != protocol_frames.end();
       ++it) {
#ifdef TELEMETRY_MONITOR
    const date_time::TimeDuration start_time = date_time::getCurrentTime();
#endif  // TELEMETRY_MONITOR
    ProtocolFramePtr frame = *it;
#ifdef ENABLE_SECURITY
    const RESULT_CODE result = DecryptFrame(frame);
    if (result != RESULT_OK) {
      SDL_LOG_WARN("Error frame decryption. Frame skipped.");
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
  SDL_LOG_ERROR("Received error on attemping to recieve message.");
}

void ProtocolHandlerImpl::NotifySubscribers(const RawMessagePtr message) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(protocol_observers_lock_);
  if (protocol_observers_.empty()) {
    SDL_LOG_ERROR(
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
  SDL_LOG_DEBUG("Sending message finished successfully.");

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
    session_observer_.OnFinalMessageCallback(connection_handle);
    transport_manager_.Disconnect(connection_handle);
    return;
  }

  ProtocolPacket sent_message(message->connection_key());
  const RESULT_CODE result =
      sent_message.deserializePacket(message->data(), message->data_size());
  if (result != RESULT_OK) {
    SDL_LOG_ERROR("Error while message deserialization.");
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
  SDL_LOG_ERROR("Sending message " << message->data_size()
                                   << " bytes failed, connection_key "
                                   << message->connection_key()
                                   << " Error_text: " << error.text());

  uint32_t connection_handle = 0;
  uint8_t session_id = 0;

  session_observer_.PairFromKey(
      message->connection_key(), &connection_handle, &session_id);

  const auto connection_it = std::find(ready_to_close_connections_.begin(),
                                       ready_to_close_connections_.end(),
                                       connection_handle);
  if (ready_to_close_connections_.end() != connection_it) {
    ready_to_close_connections_.erase(connection_it);
  }

  const auto last_message_it = sessions_last_message_id_.find(session_id);
  if (sessions_last_message_id_.end() != last_message_it) {
    sessions_last_message_id_.erase(last_message_it);
  }
}

void ProtocolHandlerImpl::OnConnectionPending(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectionUID connection_id) {}

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

void ProtocolHandlerImpl::NotifyOnGetSystemTimeFailed() {
  SDL_LOG_AUTO_TRACE();
#ifdef ENABLE_SECURITY
  security_manager_->ResetPendingSystemTimeRequests();
  security_manager_->NotifyListenersOnGetSystemTimeFailed();
#endif  // ENABLE_SECURITY
}

void ProtocolHandlerImpl::ProcessFailedPTU() {
#ifdef ENABLE_SECURITY
  security_manager_->ProcessFailedPTU();
#endif  // ENABLE_SECURITY
}

#if defined(EXTERNAL_PROPRIETARY_MODE) && defined(ENABLE_SECURITY)
void ProtocolHandlerImpl::ProcessFailedCertDecrypt() {
  SDL_LOG_AUTO_TRACE();
  security_manager_->ProcessFailedCertDecrypt();
}
#endif

void ProtocolHandlerImpl::OnTransportConfigUpdated(
    const transport_manager::transport_adapter::TransportConfig& configs) {
  SDL_LOG_AUTO_TRACE();

  transport_manager::transport_adapter::TransportConfig::const_iterator it =
      configs.find(transport_manager::transport_adapter::tc_enabled);
  if (configs.end() == it) {
    SDL_LOG_WARN("No enabled field in OnTransportConfigUpdated");
    return;
  }

  bool tcp_enabled = (0 == strcmp("true", it->second.c_str()));
  std::string tcp_port;

  if (tcp_enabled) {
    it = configs.find(transport_manager::transport_adapter::tc_tcp_port);
    if (configs.end() == it) {
      SDL_LOG_WARN("No port field in OnTransportConfigUpdated");
      return;
    }
    tcp_port = it->second;

    it = configs.find(transport_manager::transport_adapter::tc_tcp_ip_address);
    if (configs.end() == it) {
      SDL_LOG_WARN("No IP address field in OnTransportConfigUpdated");
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

  SDL_LOG_INFO("OnTransportConfigUpdated: new config enabled is "
               << tcp_enabled_ << ". Port is " << tcp_port_
               << ". IP Address is " << tcp_ip_address_);

  // Walk the SessionConnection map and find all sessions that need a
  // TransportUpdate Event. Sessions flagged with kDisabledSecondary in their
  // secondary transport are ineligible for secondary transport, and
  // therefore don't get this event.
  DataAccessor<connection_handler::SessionConnectionMap>
      session_connection_map_accessor =
          connection_handler_.session_connection_map();
  const connection_handler::SessionConnectionMap& session_connection_map =
      session_connection_map_accessor.GetData();
  connection_handler::SessionConnectionMap::const_iterator itr =
      session_connection_map.begin();
  while (itr != session_connection_map.end()) {
    const connection_handler::SessionTransports st = itr->second;
    SDL_LOG_INFO("OnTransportConfigUpdated found session "
                 << itr->first << " with primary connection  "
                 << st.primary_transport << " and secondary connection "
                 << st.secondary_transport);
    if (st.secondary_transport != kDisabledSecondary) {
      SendTransportUpdateEvent(st.primary_transport, itr->first);
    }
    ++itr;
  }
}

void ProtocolHandlerImpl::OnAuthTokenUpdated(const std::string& policy_app_id,
                                             const std::string& auth_token) {
  SDL_LOG_AUTO_TRACE();
  sync_primitives::AutoLock lock(auth_token_map_lock_);
  if (auth_token.empty()) {
    auth_token_map_.erase(policy_app_id);
  } else {
    auth_token_map_[policy_app_id] = auth_token;
  }
}

bool ProtocolHandlerImpl::IsRPCServiceSecure(
    const uint32_t connection_key) const {
  SDL_LOG_AUTO_TRACE();
#ifdef ENABLE_SECURITY

  security_manager::SSLContext* context =
      session_observer_.GetSSLContext(connection_key, ServiceType::kRpc);
  return (context && context->IsInitCompleted());
#else
  return false;
#endif  // ENABLE_SECURITY
}

RESULT_CODE ProtocolHandlerImpl::SendFrame(const ProtocolFramePtr packet) {
  SDL_LOG_AUTO_TRACE();
  if (!packet) {
    SDL_LOG_ERROR("Failed to send empty packet.");
    return RESULT_FAIL;
  }
#ifdef ENABLE_SECURITY
  // and return protect flag to Packet constructor for makeing design by Policy
  const RESULT_CODE result = EncryptFrame(packet);
  if (result != RESULT_OK) {
    SDL_LOG_WARN("Error frame encryption. Frame droped.");
    return RESULT_FAIL;
  }
#endif  // ENABLE_SECURITY

  SDL_LOG_DEBUG(
      "Packet to be sent: "
      << utils::ConvertBinaryDataToString(packet->data(), packet->data_size())
      << " of size: " << packet->data_size());
  const RawMessagePtr message_to_send = packet->serializePacket();
  if (!message_to_send) {
    SDL_LOG_ERROR("Serialization error");
    return RESULT_FAIL;
  }
  SDL_LOG_DEBUG("Message to send with connection id "
                << static_cast<int>(packet->connection_id()));

  if (transport_manager::E_SUCCESS !=
      transport_manager_.SendMessageToDevice(message_to_send)) {
    SDL_LOG_WARN("Can't send message to device");
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
    const bool needs_encryption,
    const bool is_final_message) {
  SDL_LOG_AUTO_TRACE();

  SDL_LOG_DEBUG("Packet needs encryption: " << std::boolalpha
                                            << needs_encryption);

  ProtocolFramePtr ptr(
      new protocol_handler::ProtocolPacket(connection_id,
                                           protocol_version,
                                           needs_encryption,
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
    const bool needs_encryption,
    const bool is_final_message) {
  SDL_LOG_AUTO_TRACE();

  SDL_LOG_DEBUG(" data size " << data_size << " max_frame_size "
                              << max_frame_size);

  // remainder of last frame
  const size_t lastframe_remainder = data_size % max_frame_size;
  // size of last frame (full fill or not)
  const size_t lastframe_size =
      lastframe_remainder > 0 ? lastframe_remainder : max_frame_size;

  const size_t frames_count = data_size / max_frame_size +
                              // add last frame if not empty
                              (lastframe_remainder > 0 ? 1 : 0);

  SDL_LOG_DEBUG("Data " << data_size << " bytes in " << frames_count
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
                                           false,
                                           FRAME_TYPE_FIRST,
                                           service_type,
                                           FRAME_DATA_FIRST,
                                           session_id,
                                           FIRST_FRAME_DATA_SIZE,
                                           message_id,
                                           out_data));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(firstPacket, false));
  SDL_LOG_DEBUG("First frame is sent.");

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
                                             needs_encryption,
                                             FRAME_TYPE_CONSECUTIVE,
                                             service_type,
                                             data_type,
                                             session_id,
                                             frame_size,
                                             message_id,
                                             data + max_frame_size * i));

    raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, is_final_packet));
    SDL_LOG_DEBUG('#' << i << " frame is sent.");
  }
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleMessage(const ProtocolFramePtr packet) {
  DCHECK_OR_RETURN(packet, RESULT_UNKNOWN);
  SDL_LOG_TRACE("Handling message " << packet);
  switch (packet->frame_type()) {
    case FRAME_TYPE_CONTROL:
      SDL_LOG_TRACE("FRAME_TYPE_CONTROL");
      return HandleControlMessage(packet);
    case FRAME_TYPE_SINGLE:
      SDL_LOG_TRACE("FRAME_TYPE_SINGLE");
      return HandleSingleFrameMessage(packet);
    case FRAME_TYPE_FIRST:
    case FRAME_TYPE_CONSECUTIVE:
      SDL_LOG_TRACE("FRAME_TYPE_FIRST or FRAME_TYPE_CONSECUTIVE");
      return HandleMultiFrameMessage(packet);
    default: {
      SDL_LOG_WARN("Unknown frame type" << packet->frame_type());
      return RESULT_FAIL;
    }
  }
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleSingleFrameMessage(
    const ProtocolFramePtr packet) {
  SDL_LOG_AUTO_TRACE();

  SDL_LOG_TRACE(
      "FRAME_TYPE_SINGLE message of size "
      << packet->data_size() << "; message "
      << utils::ConvertBinaryDataToString(packet->data(), packet->data_size()));

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
                                                packet->protection_flag(),
                                                packet->service_type(),
                                                packet->payload_size()));
  if (!rawMessage) {
    return RESULT_FAIL;
  }
#ifdef TELEMETRY_MONITOR
  if (metric_observer_) {
    auto metric = std::make_shared<PHTelemetryObserver::MessageMetric>();
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
  SDL_LOG_AUTO_TRACE();

  // Replace a potential secondary transport ID in the packet with the primary
  // transport ID
  const connection_handler::SessionTransports st =
      connection_handler_.GetSessionTransports(packet->session_id());
  if (st.primary_transport != 0) {
    packet->set_connection_id(st.primary_transport);
  }

  if (multiframe_builder_.AddFrame(packet) != RESULT_OK) {
    SDL_LOG_WARN("Frame assembling issue");
  }

  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessage(
    const ProtocolFramePtr packet) {
  SDL_LOG_AUTO_TRACE();

  // TODO{ALeshin}: Rename "Session" to "Service" on PH, CH, AM levels
  switch (packet->frame_data()) {
    case FRAME_DATA_START_SERVICE: {
      SDL_LOG_TRACE("FrameData: StartService");
      return HandleControlMessageStartSession(packet);
    }
    case FRAME_DATA_END_SERVICE: {
      SDL_LOG_TRACE("FrameData: StopService");
      return HandleControlMessageEndSession(*packet);
    }
    case FRAME_DATA_END_SERVICE_ACK: {
      SDL_LOG_TRACE("FrameData: StopService ACK");
      return HandleControlMessageEndServiceACK(*packet);
    }
    case FRAME_DATA_HEART_BEAT: {
      SDL_LOG_TRACE("FrameData: Heartbeat");
      return HandleControlMessageHeartBeat(*packet);
    }
    case FRAME_DATA_HEART_BEAT_ACK: {
      SDL_LOG_TRACE("FrameData Heartbeat ACK");
      SDL_LOG_DEBUG(
          "Received Heartbeat ACK from mobile,"
          " connection: "
          << packet->connection_id());
      return RESULT_OK;
    }
    case FRAME_DATA_REGISTER_SECONDARY_TRANSPORT: {
      SDL_LOG_TRACE("FrameData: RegisterSecondaryTransport");
      return HandleControlMessageRegisterSecondaryTransport(packet);
    }
    default:
      SDL_LOG_WARN("Control message of type "
                   << static_cast<int>(packet->frame_data()) << " ignored");
      return RESULT_OK;
  }
  return RESULT_OK;
}

uint32_t get_hash_id(const ProtocolPacket& packet) {
  if (packet.protocol_version() < PROTOCOL_VERSION_2) {
    SDL_LOG_DEBUG("Packet without hash data (protocol version less 2)");
    return HASH_ID_NOT_SUPPORTED;
  }
  if (packet.data_size() < 4) {
    SDL_LOG_WARN("Packet without hash data (data size less 4)");
    return HASH_ID_WRONG;
  }
  if (packet.protocol_version() >= PROTOCOL_VERSION_5) {
    BsonObject obj;
    size_t obj_size = bson_object_from_bytes_len(
        &obj, packet.data(), packet.total_data_bytes());
    if (obj_size > 0) {
      const uint32_t hash_id =
          (uint32_t)bson_object_get_int32(&obj, strings::hash_id);
      bson_object_deinitialize(&obj);
      return hash_id;
    } else {
      SDL_LOG_WARN("Failed to parse BSON field for hash ID");
      return HASH_ID_WRONG;
    }
  } else {
    const uint32_t hash_be = *(reinterpret_cast<uint32_t*>(packet.data()));
    const uint32_t hash_le = BE_TO_LE32(hash_be);

    // null hash is wrong hash value
    return hash_le == HASH_ID_NOT_SUPPORTED ? HASH_ID_WRONG : hash_le;
  }
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageEndSession(
    const ProtocolPacket& packet) {
  SDL_LOG_AUTO_TRACE();

  const uint8_t current_session_id = packet.session_id();
  uint32_t hash_id = get_hash_id(packet);
  const ServiceType service_type = ServiceTypeFromByte(packet.service_type());

  const ConnectionID connection_id = packet.connection_id();
  std::string err_reason;
  const uint32_t session_key = session_observer_.OnSessionEndedCallback(
      connection_id, current_session_id, &hash_id, service_type, &err_reason);

  // TODO(EZamakhov): add clean up output queue (for removed service)
  if (session_key != 0) {
    SendEndSessionAck(connection_id,
                      current_session_id,
                      packet.protocol_version(),
                      service_type);
    message_counters_.erase(current_session_id);
  } else {
    SDL_LOG_WARN("Refused to end session " << static_cast<int>(service_type)
                                           << " type.");
    if (packet.protocol_version() >= PROTOCOL_VERSION_5) {
      std::vector<std::string> rejectedParams;
      if (hash_id == protocol_handler::HASH_ID_WRONG &&
          packet.service_type() == kRpc) {
        rejectedParams.push_back(std::string(strings::hash_id));
      }
      SendEndSessionNAck(connection_id,
                         current_session_id,
                         packet.protocol_version(),
                         service_type,
                         rejectedParams,
                         err_reason);
    } else {
      SendEndSessionNAck(connection_id,
                         current_session_id,
                         packet.protocol_version(),
                         service_type,
                         err_reason);
    }
  }
  return RESULT_OK;
}

const ServiceStatus ProtocolHandlerImpl::ServiceDisallowedBySettings(
    const ServiceType service_type,
    const ConnectionID connection_id,
    const uint8_t session_id,
    const bool protection) const {
  SDL_LOG_AUTO_TRACE();
  const std::string& transport =
      session_observer_.TransportTypeProfileStringFromConnHandle(connection_id);

  const auto video_transports = settings_.video_service_transports();
  const bool is_video_allowed =
      video_transports.empty() ||
      std::find(video_transports.begin(), video_transports.end(), transport) !=
          video_transports.end();

  const auto audio_transports = settings_.audio_service_transports();
  const bool is_audio_allowed =
      audio_transports.empty() ||
      std::find(audio_transports.begin(), audio_transports.end(), transport) !=
          audio_transports.end();

#ifdef ENABLE_SECURITY
  const auto& force_protected = get_settings().force_protected_service();

  const auto& force_unprotected = get_settings().force_unprotected_service();

  const bool is_force_protected =
      (helpers::in_range(force_protected, service_type));

  const bool is_force_unprotected =
      (helpers::in_range(force_unprotected, service_type));

  const bool can_start_protected = is_force_protected && protection;

  const bool can_start_unprotected = is_force_unprotected && !protection;

  if (is_force_protected && !can_start_protected) {
    return ServiceStatus::PROTECTION_ENFORCED;
  }

  if (is_force_unprotected && !can_start_unprotected) {
    return ServiceStatus::UNSECURE_START_FAILED;
  }
#endif  // ENABLE_SECURITY

  if ((ServiceType::kMobileNav == service_type && !is_video_allowed) ||
      (ServiceType::kAudio == service_type && !is_audio_allowed)) {
    return ServiceStatus::SERVICE_START_FAILED;
  }

  return ServiceStatus::INVALID_ENUM;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageEndServiceACK(
    const ProtocolPacket& packet) {
  SDL_LOG_AUTO_TRACE();

  const uint8_t current_session_id = packet.session_id();
  uint32_t hash_id = get_hash_id(packet);
  const ServiceType service_type = ServiceTypeFromByte(packet.service_type());
  const ConnectionID connection_id = packet.connection_id();

  const uint32_t session_key = session_observer_.OnSessionEndedCallback(
      connection_id, current_session_id, &hash_id, service_type);

  if (0 == session_key) {
    SDL_LOG_WARN("Refused to end service");
    return RESULT_FAIL;
  }

  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageStartSession(
    const ProtocolFramePtr packet) {
  SDL_LOG_AUTO_TRACE();
  SDL_LOG_DEBUG(
      "Protocol version:" << static_cast<int>(packet->protocol_version()));
  const ServiceType service_type = ServiceTypeFromByte(packet->service_type());

  const uint8_t protocol_version = packet->protocol_version();
#ifdef ENABLE_SECURITY
  const bool protection =
      // Protocol version 1 does not support protection
      (protocol_version > PROTOCOL_VERSION_1) ? packet->protection_flag()
                                              : false;
#else
  const bool protection = false;
#endif  // ENABLE_SECURITY

  const ConnectionID connection_id = packet->connection_id();
  const uint8_t session_id = packet->session_id();
  const uint32_t connection_key =
      session_observer_.KeyFromPair(connection_id, session_id);

  service_status_update_handler_->OnServiceUpdate(
      connection_key, service_type, ServiceStatus::SERVICE_RECEIVED);

  const auto settings_check = ServiceDisallowedBySettings(
      service_type, connection_id, session_id, protection);

  if (ServiceStatus::INVALID_ENUM != settings_check) {
    SDL_LOG_DEBUG("Rejecting StartService for service:"
                  << service_type << ", disallowed by settings.");
    service_status_update_handler_->OnServiceUpdate(
        connection_key, service_type, settings_check);

    std::string reason("Service type: " + std::to_string(service_type) +
                       " disallowed by settings.");
    if (ServiceStatus::PROTECTION_ENFORCED == settings_check) {
      reason += " Allowed only in protected mode";
    }
    if (ServiceStatus::UNSECURE_START_FAILED == settings_check) {
      reason += " Allowed only in unprotected mode";
    }

    utils::SemanticVersion version;
    if (packet->service_type() == kRpc && packet->data() != NULL) {
      ParseFullVersion(version, packet);
    }

    SendStartSessionNAck(connection_id,
                         session_id,
                         protocol_version,
                         service_type,
                         reason,
                         version);
    return RESULT_OK;
  }

  SDL_LOG_INFO("StartSession ID " << static_cast<int>(session_id)
                                  << " and Connection ID "
                                  << static_cast<int>(connection_id));

  {
    sync_primitives::AutoLock auto_lock(start_session_frame_map_lock_);
    start_session_frame_map_[std::make_pair(connection_id, session_id)] =
        packet;
  }

  BsonObject bson_obj;
  if (packet->data() != NULL) {
    size_t bson_obj_size = bson_object_from_bytes_len(
        &bson_obj, packet->data(), packet->total_data_bytes());
    if (bson_obj_size == 0) {
      SDL_LOG_WARN("Failed to parse BSON field of start service packet");
      bson_object_initialize_default(&bson_obj);
    }
  } else {
    bson_object_initialize_default(&bson_obj);
  }

  session_observer_.OnSessionStartedCallback(
      connection_id, packet->session_id(), service_type, protection, &bson_obj);
  bson_object_deinitialize(&bson_obj);

  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageRegisterSecondaryTransport(
    const ProtocolFramePtr packet) {
  SDL_LOG_AUTO_TRACE();
  const uint8_t session_id = packet->session_id();
  const ConnectionID connection_id = packet->connection_id();
  ConnectionID primary_connection_id = 0;

  SDL_LOG_INFO("RegisterSecondaryTransport ID "
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

bool ProtocolHandlerImpl::ParseFullVersion(
    utils::SemanticVersion& full_version,
    const ProtocolFramePtr& packet) const {
  SDL_LOG_AUTO_TRACE();

  BsonObject request_params;
  size_t request_params_size = bson_object_from_bytes_len(
      &request_params, packet->data(), packet->total_data_bytes());
  if (request_params_size > 0) {
    char* version_param =
        bson_object_get_string(&request_params, strings::protocol_version);
    std::string version_string(version_param == NULL ? "" : version_param);
    full_version = version_string;

    // Constructed payloads added in Protocol v5
    if (full_version.major_version_ < PROTOCOL_VERSION_5) {
      return false;
    }
    bson_object_deinitialize(&request_params);
  } else {
    SDL_LOG_WARN("Failed to parse start service packet for version string");
  }

  return true;
}

void ProtocolHandlerImpl::NotifySessionStarted(
    SessionContext& context,
    std::vector<std::string>& rejected_params,
    const std::string err_reason) {
  SDL_LOG_AUTO_TRACE();

  ProtocolFramePtr packet;
  {
    sync_primitives::AutoLock auto_lock(start_session_frame_map_lock_);
    StartSessionFrameMap::iterator it = start_session_frame_map_.find(
        std::make_pair(context.connection_id_, context.initial_session_id_));
    if (it == start_session_frame_map_.end()) {
      SDL_LOG_ERROR("Cannot find Session Started packet");
      return;
    }
    packet = it->second;
    start_session_frame_map_.erase(it);
  }

  const ServiceType service_type = ServiceTypeFromByte(packet->service_type());
  const uint8_t protocol_version = packet->protocol_version();
  utils::SemanticVersion full_version;

  // Can't check protocol_version because the first packet is v1, but there
  // could still be a payload, in which case we can get the real protocol
  // version
  if (packet->service_type() == kRpc && packet->data() != NULL) {
    if (ParseFullVersion(full_version, packet)) {
      const auto connection_key = session_observer_.KeyFromPair(
          packet->connection_id(), context.new_session_id_);
      connection_handler_.BindProtocolVersionWithSession(connection_key,
                                                         full_version);
    } else {
      rejected_params.push_back(std::string(strings::protocol_version));
    }
  }

  if (context.is_start_session_failed_ || !context.new_session_id_) {
    SDL_LOG_WARN("Refused by session_observer to create service "
                 << static_cast<int32_t>(service_type) << " type.");
    const auto session_id = packet->session_id();
    const auto connection_key =
        session_observer_.KeyFromPair(context.connection_id_, session_id);
    service_status_update_handler_->OnServiceUpdate(
        connection_key,
        context.service_type_,
        ServiceStatus::SERVICE_START_FAILED);
    SendStartSessionNAck(context.connection_id_,
                         session_id,
                         protocol_version,
                         packet->service_type(),
                         rejected_params,
                         err_reason,
                         full_version);
    return;
  }

  std::shared_ptr<BsonObject> start_session_ack_params(
      new BsonObject(), [](BsonObject* obj) {
        bson_object_deinitialize(obj);
        delete obj;
      });
  bson_object_initialize_default(start_session_ack_params.get());
  // when video service is successfully started, copy input parameters
  // ("width", "height", "videoProtocol", "videoCodec") to the ACK packet
  if (packet->service_type() == kMobileNav && packet->data() != NULL) {
    BsonObject req_param;
    size_t req_param_size = bson_object_from_bytes_len(
        &req_param, packet->data(), packet->total_data_bytes());
    if (req_param_size > 0) {
      BsonElement* element = NULL;

      if ((element = bson_object_get(&req_param, strings::height)) != NULL &&
          element->type == TYPE_INT32) {
        bson_object_put_int32(
            start_session_ack_params.get(),
            strings::height,
            bson_object_get_int32(&req_param, strings::height));
      }
      if ((element = bson_object_get(&req_param, strings::width)) != NULL &&
          element->type == TYPE_INT32) {
        bson_object_put_int32(
            start_session_ack_params.get(),
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
    } else {
      SDL_LOG_WARN("Failed to parse BSON field of start service (video)");
    }
  }

#ifdef ENABLE_SECURITY
  // for packet is encrypted and security plugin is enable
  if (context.is_protected_ && security_manager_) {
    const uint32_t connection_key = session_observer_.KeyFromPair(
        context.connection_id_, context.new_session_id_);

    std::shared_ptr<HandshakeHandler> handler =
        std::make_shared<HandshakeHandler>(
            *this,
            session_observer_,
            full_version,
            context,
            packet->protocol_version(),
            start_session_ack_params,
            *(service_status_update_handler_.get()));

    security_manager::SSLContext* ssl_context =
        security_manager_->CreateSSLContext(
            connection_key,
            security_manager::SecurityManager::ContextCreationStrategy::
                kUseExisting);
    if (!ssl_context) {
      const std::string error("CreateSSLContext failed");
      SDL_LOG_ERROR(error);
      security_manager_->SendInternalError(
          connection_key,
          security_manager::SecurityManager::ERROR_INTERNAL,
          error);

      handler->OnHandshakeDone(
          connection_key, security_manager::SSLContext::Handshake_Result_Fail);

      return;
    }

    if (!rejected_params.empty()) {
      service_status_update_handler_->OnServiceUpdate(
          connection_key,
          context.service_type_,
          ServiceStatus::SERVICE_START_FAILED);
      SendStartSessionNAck(context.connection_id_,
                           packet->session_id(),
                           protocol_version,
                           packet->service_type(),
                           rejected_params,
                           "SSL Handshake failed due to rejected parameters",
                           full_version);
      if (packet->service_type() != kRpc) {
        context.is_start_session_failed_ = true;
      }
    } else if (ssl_context->IsInitCompleted()) {
      // mark service as protected
      session_observer_.SetProtectionFlag(connection_key, service_type);
      // Start service as protected with current SSLContext
      service_status_update_handler_->OnServiceUpdate(
          connection_key,
          context.service_type_,
          ServiceStatus::SERVICE_ACCEPTED);
      SendStartSessionAck(context.connection_id_,
                          context.new_session_id_,
                          packet->protocol_version(),
                          context.hash_id_,
                          packet->service_type(),
                          PROTECTION_ON,
                          full_version,
                          *start_session_ack_params);
    } else {
      SDL_LOG_DEBUG("Adding Handshake handler to listeners: " << handler.get());
      security_manager::SecurityManagerListener* listener =
          new HandshakeHandler(*handler);
      security_manager_->AddListener(listener);

      if (!ssl_context->IsHandshakePending()) {
        // Start handshake process
        security_manager_->StartHandshake(connection_key);

        if (!security_manager_->IsSystemTimeProviderReady()) {
          security_manager_->RemoveListener(listener);
          service_status_update_handler_->OnServiceUpdate(
              connection_key,
              context.service_type_,
              ServiceStatus::SERVICE_START_FAILED);
          SendStartSessionNAck(context.connection_id_,
                               packet->session_id(),
                               protocol_version,
                               packet->service_type(),
                               rejected_params,
                               "System time provider is not ready",
                               full_version);
          if (packet->service_type() != kRpc) {
            context.is_start_session_failed_ = true;
          }
        }
      }
    }

    SDL_LOG_DEBUG("Protection establishing for connection "
                  << connection_key << " is in progress");
    return;
  }
#endif  // ENABLE_SECURITY
  const uint32_t connection_key = session_observer_.KeyFromPair(
      context.connection_id_, context.new_session_id_);
  if (rejected_params.empty()) {
    service_status_update_handler_->OnServiceUpdate(
        connection_key, context.service_type_, ServiceStatus::SERVICE_ACCEPTED);
    SendStartSessionAck(context.connection_id_,
                        context.new_session_id_,
                        packet->protocol_version(),
                        context.hash_id_,
                        packet->service_type(),
                        PROTECTION_OFF,
                        full_version,
                        *start_session_ack_params);
  } else {
    service_status_update_handler_->OnServiceUpdate(
        connection_key,
        context.service_type_,
        ServiceStatus::SERVICE_START_FAILED);
    SendStartSessionNAck(
        context.connection_id_,
        packet->session_id(),
        protocol_version,
        packet->service_type(),
        rejected_params,
        "Certain parameters in the StartService request were rejected",
        full_version);
    context.is_start_session_failed_ = true;
  }
}

void ProtocolHandlerImpl::NotifySessionStarted(
    const SessionContext& context,
    std::vector<std::string>& rejected_params,
    const std::string err_reason) {
  SessionContext context_copy = context;
  NotifySessionStarted(context_copy, rejected_params, err_reason);
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageHeartBeat(
    const ProtocolPacket& packet) {
  const ConnectionID connection_id = packet.connection_id();
  const uint32_t session_id = packet.session_id();
  SDL_LOG_DEBUG("Sending heart beat acknowledgment for connection "
                << connection_id << " session " << session_id);
  uint8_t protocol_version;
  if (session_observer_.ProtocolVersionUsed(
          connection_id, session_id, protocol_version)) {
    // TODO(EZamakhov): investigate message_id for HeartBeatAck
    if (protocol_version >= PROTOCOL_VERSION_3 &&
        protocol_version <= PROTOCOL_VERSION_5) {
      const uint32_t connection_key =
          session_observer_.KeyFromPair(connection_id, session_id);
      if (!connection_handler_.IsSessionHeartbeatTracked(connection_key)) {
        SDL_LOG_DEBUG("Session heartbeat tracking is not started. "
                      << "Starting it for session " << session_id);
        connection_handler_.StartSessionHeartBeat(connection_key);
      }
      return SendHeartBeatAck(connection_id, session_id, packet.message_id());
    } else {
      SDL_LOG_WARN("HeartBeat is not supported");
      return RESULT_HEARTBEAT_IS_NOT_SUPPORTED;
    }

  } else {
    SDL_LOG_WARN(
        "SendHeartBeatAck is failed connection or session does not exist");
    return RESULT_FAIL;
  }
}

void ProtocolHandlerImpl::PopValidAndExpiredMultiframes() {
  SDL_LOG_AUTO_TRACE();
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
    SDL_LOG_TRACE("Result frame" << frame << "for connection "
                                 << connection_key);
    const RawMessagePtr rawMessage(new RawMessage(connection_key,
                                                  frame->protocol_version(),
                                                  frame->data(),
                                                  frame->total_data_bytes(),
                                                  frame->protection_flag(),
                                                  frame->service_type(),
                                                  frame->payload_size()));
    DCHECK(rawMessage);

#ifdef TELEMETRY_MONITOR
    if (metric_observer_) {
      auto metric = std::make_shared<PHTelemetryObserver::MessageMetric>();
      metric->raw_msg = rawMessage;
      metric_observer_->EndMessageProcess(metric);
    }
#endif  // TELEMETRY_MONITOR
    NotifySubscribers(rawMessage);
  }
}

bool ProtocolHandlerImpl::TrackMessage(const uint32_t& connection_key) {
  SDL_LOG_AUTO_TRACE();
  const size_t& frequency_time = get_settings().message_frequency_time();
  const size_t& frequency_count = get_settings().message_frequency_count();
  if (frequency_time > 0u && frequency_count > 0u) {
    const size_t message_frequency =
        message_meter_.TrackMessage(connection_key);
    SDL_LOG_DEBUG("Frequency of " << connection_key << " is "
                                  << message_frequency);
    if (message_frequency > frequency_count) {
      SDL_LOG_WARN("Frequency of " << connection_key << " is marked as high.");
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
  SDL_LOG_AUTO_TRACE();
  if (get_settings().malformed_frequency_time() > 0u &&
      malformed_frequency_count > 0u) {
    const size_t malformed_message_frequency =
        malformed_message_meter_.TrackMessages(connection_key, count);
    SDL_LOG_DEBUG("Malformed frequency of " << connection_key << " is "
                                            << malformed_message_frequency);
    if (!get_settings().malformed_message_filtering() ||
        malformed_message_frequency > malformed_frequency_count) {
      SDL_LOG_WARN("Malformed frequency of " << connection_key
                                             << " is marked as high.");
      session_observer_.OnMalformedMessageCallback(connection_key);
      malformed_message_meter_.RemoveIdentifier(connection_key);
      return true;
    }
  }
  return false;
}

void ProtocolHandlerImpl::Handle(const impl::RawFordMessageFromMobile message) {
  SDL_LOG_AUTO_TRACE();

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
  SDL_LOG_TRACE("Message : " << message.get());
  const uint8_t c_id = message->connection_id();
  const uint32_t m_id = message->session_id();

  if (session_observer_.IsHeartBeatSupported(c_id, m_id)) {
    connection_handler_.KeepConnectionAlive(c_id, m_id);
  }

  // TODO(EZamakhov): remove dublication of IncomingDataHandler logic
  if (((0 != message->data()) && (0 != message->data_size())) ||
      FRAME_TYPE_CONTROL == message->frame_type() ||
      FRAME_TYPE_FIRST == message->frame_type()) {
    SDL_LOG_TRACE("Packet: dataSize " << message->data_size());
    HandleMessage(message);
    PopValidAndExpiredMultiframes();
  } else {
    SDL_LOG_WARN("handleMessagesFromMobileApp() - incorrect or NULL data");
  }
}

void ProtocolHandlerImpl::Handle(const impl::RawFordMessageToMobile message) {
  SDL_LOG_DEBUG("Message to mobile app: connection id "
                << static_cast<int>(message->connection_id())
                << ";"
                   " dataSize: "
                << message->data_size()
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

void ProtocolHandlerImpl::set_service_status_update_handler(
    std::unique_ptr<ServiceStatusUpdateHandler> handler) {
  service_status_update_handler_ = std::move(handler);
}

#ifdef ENABLE_SECURITY
void ProtocolHandlerImpl::set_security_manager(
    security_manager::SecurityManager* security_manager) {
  if (!security_manager) {
    SDL_LOG_ERROR("Invalid (NULL) pointer to SecurityManager.");
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
    SDL_LOG_WARN("No security_manager_ set.");
    return RESULT_FAIL;
  }
  const uint32_t connection_key = session_observer_.KeyFromPair(
      packet->connection_id(), packet->session_id());
  security_manager::SSLContext* context = session_observer_.GetSSLContext(
      connection_key, ServiceTypeFromByte(packet->service_type()));

  SDL_LOG_DEBUG("Protection flag is: " << packet->protection_flag()
                                       << std::boolalpha);
  if ((!context || !context->IsInitCompleted()) || !packet->protection_flag()) {
    SDL_LOG_DEBUG("Ecryption is skipped!");
    return RESULT_OK;
  }
  const uint8_t* out_data;
  size_t out_data_size;
  if (!context->Encrypt(
          packet->data(), packet->data_size(), &out_data, &out_data_size)) {
    const std::string error_text(context->LastError());
    SDL_LOG_ERROR("Enryption failed: " << error_text);
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
  SDL_LOG_DEBUG("Encrypted " << packet->data_size() << " bytes to "
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
    SDL_LOG_DEBUG("Frame will not be decrypted");
    return RESULT_OK;
  }

  if (!security_manager_) {
    SDL_LOG_WARN("No security_manager_ set.");
    return RESULT_FAIL;
  }
  const uint32_t connection_key = session_observer_.KeyFromPair(
      packet->connection_id(), packet->session_id());
  security_manager::SSLContext* context = session_observer_.GetSSLContext(
      connection_key, ServiceTypeFromByte(packet->service_type()));
  if (!context || !context->IsInitCompleted()) {
    const std::string error_text("Fail decryption for unprotected service ");
    SDL_LOG_ERROR(error_text << static_cast<int>(packet->service_type()));
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
    SDL_LOG_ERROR("Decryption failed: " << error_text);
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
  SDL_LOG_DEBUG("Decrypted " << packet->data_size() << " bytes to "
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
  SDL_LOG_TRACE("SendFramesNumber MobileNaviAck for session "
                << connection_key);

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
      SDL_LOG_DEBUG("SendFramesNumber finished successfully");
    }
  } else {
    SDL_LOG_WARN(
        "SendFramesNumber is failed connection or session does not exist");
  }
}

#ifdef TELEMETRY_MONITOR
void ProtocolHandlerImpl::SetTelemetryObserver(PHTelemetryObserver* observer) {
  metric_observer_ = observer;
}
#endif  // TELEMETRY_MONITOR

uint8_t ProtocolHandlerImpl::SupportedSDLProtocolVersion() const {
  SDL_LOG_AUTO_TRACE();
  return get_settings().max_supported_protocol_version();
}

const impl::TransportTypes transportTypes = {
    std::make_pair(
        std::string("AOA_USB"),
        impl::TransportDescription(impl::TransportType::TT_USB, false, true)),
    std::make_pair(std::string("SPP_BLUETOOTH"),
                   impl::TransportDescription(
                       impl::TransportType::TT_BLUETOOTH, false, true)),
    std::make_pair(std::string("IAP_BLUETOOTH"),
                   impl::TransportDescription(
                       impl::TransportType::TT_BLUETOOTH, true, false)),
    std::make_pair(
        std::string("IAP_USB"),
        impl::TransportDescription(impl::TransportType::TT_USB, true, false)),
    std::make_pair(
        std::string("TCP_WIFI"),
        impl::TransportDescription(impl::TransportType::TT_WIFI, true, true)),
    std::make_pair(
        std::string("IAP_USB_HOST_MODE"),
        impl::TransportDescription(impl::TransportType::TT_USB, true, false)),
    std::make_pair(
        std::string("IAP_USB_DEVICE_MODE"),
        impl::TransportDescription(impl::TransportType::TT_USB, true, false)),
    std::make_pair(
        std::string("IAP_CARPLAY"),
        impl::TransportDescription(impl::TransportType::TT_WIFI, true, false)),
    std::make_pair(std::string("WEBSOCKET"),
                   impl::TransportDescription(
                       impl::TransportType::TT_WEBSOCKET, false, false))};

const impl::TransportDescription
ProtocolHandlerImpl::GetTransportTypeFromConnectionType(
    const std::string& connection_type) const {
  impl::TransportDescription result =
      impl::TransportDescription(impl::TransportType::TT_NONE, false, false);
  impl::TransportTypes::const_iterator it =
      transportTypes.find(connection_type);
  if (it != transportTypes.end()) {
    result = it->second;
  } else {
    SDL_LOG_ERROR("Unknown connection type " << connection_type);
  }

  return result;
}

const bool ProtocolHandlerImpl::ParseSecondaryTransportConfiguration(
    const ConnectionID connection_id,
    std::vector<std::string>& secondaryTransports,
    std::vector<int32_t>& audioServiceTransports,
    std::vector<int32_t>& videoServiceTransports) const {
  SDL_LOG_AUTO_TRACE();
  std::vector<std::string> secondary_transport_types;

  // First discover what the connection type of the primary transport is
  // and look up the allowed secondary transports for that primary transport
  const std::string connection_type =
      session_observer_.TransportTypeProfileStringFromConnHandle(connection_id);
  const impl::TransportDescription td =
      GetTransportTypeFromConnectionType(connection_type);
  if (settings_.multiple_transports_enabled()) {
    if (td.transport_type_ == impl::TransportType::TT_USB) {
      secondary_transport_types = settings_.secondary_transports_for_usb();
    } else if (td.transport_type_ == impl::TransportType::TT_BLUETOOTH) {
      secondary_transport_types =
          settings_.secondary_transports_for_bluetooth();
    } else if (td.transport_type_ == impl::TransportType::TT_WIFI) {
      secondary_transport_types = settings_.secondary_transports_for_wifi();
    } else {
      SDL_LOG_ERROR(
          "Bad or unknown device type in ParseSecondaryTransportConfiguration");
      return false;
    }
  }
  // note: even if settings_.multiple_transports_enabled() is false, we still
  // send out an empty "secondaryTransports" parameter, along with
  // "videoServiceTransports" and "audioServiceTransports" params which are
  // useful without secondary transport.

  // Then, generate the "secondaryTransports" array for the StartSession ACK
  GenerateSecondaryTransportsForStartSessionAck(secondary_transport_types,
                                                td.ios_transport_,
                                                td.android_transport_,
                                                secondaryTransports);

  // Next, figure out which connections audio or video services are allowed on
  GenerateServiceTransportsForStartSessionAck(
      settings_.multiple_transports_enabled(),
      settings_.audio_service_transports(),
      connection_type,
      td.transport_type_,
      secondary_transport_types,
      audioServiceTransports);

  GenerateServiceTransportsForStartSessionAck(
      settings_.multiple_transports_enabled(),
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
  SDL_LOG_AUTO_TRACE();

  // Parse the "secondary_transport_types" vector (which comes from
  // smartDeviceLink.ini). For each entry in the vector, add an
  // appropriate string to the secondaryTransports
  std::vector<std::string>::const_iterator it =
      secondary_transport_types.begin();
  while (it != secondary_transport_types.end()) {
    const utils::custom_string::CustomString transport_type(*it);
    if (transport_type.CompareIgnoreCase("USB")) {
      if (device_is_ios) {
        SDL_LOG_TRACE(
            "Adding IAP_USB to secondaryTransports for StartSessionAck");
        secondaryTransports.push_back("IAP_USB");
      }
      if (device_is_android) {
        SDL_LOG_TRACE(
            "Adding AOA_USB to secondaryTransports for StartSessionAck");
        secondaryTransports.push_back("AOA_USB");
      }
    } else if (transport_type.CompareIgnoreCase("Bluetooth")) {
      if (device_is_ios) {
        SDL_LOG_TRACE(
            "Adding IAP_BLUETOOTH to secondaryTransports for StartSessionAck");
        secondaryTransports.push_back("IAP_BLUETOOTH");
      }
      if (device_is_android) {
        SDL_LOG_TRACE(
            "Adding SPP_BLUETOOTH to secondaryTransports for StartSessionAck");
        secondaryTransports.push_back("SPP_BLUETOOTH");
      }
    }
    if (transport_type.CompareIgnoreCase("WiFi")) {
      SDL_LOG_TRACE(
          "Adding TCP_WIFI to secondaryTransports for StartSessionAck");
      secondaryTransports.push_back("TCP_WIFI");
    }

    ++it;
  }
}

void ProtocolHandlerImpl::GenerateServiceTransportsForStartSessionAck(
    bool secondary_enabled,
    const std::vector<std::string>& service_transports,
    const std::string& primary_connection_type,
    const impl::TransportType primary_transport_type,
    const std::vector<std::string>& secondary_transport_types,
    std::vector<int32_t>& serviceTransports) const {
  SDL_LOG_AUTO_TRACE();

  if (service_transports.size() == 0) {
    if (secondary_enabled && !secondary_transport_types.empty()) {
      SDL_LOG_TRACE(
          "Empty Service Transports. Allowing service to run on both "
          "connections");
      serviceTransports.push_back(1);
      serviceTransports.push_back(2);
    } else {
      serviceTransports.push_back(1);
    }
  } else {
    bool fPrimaryAdded = false;
    bool fSecondaryAdded = false;
    std::vector<std::string>::const_iterator it = service_transports.begin();
    for (; it != service_transports.end(); ++it) {
      const utils::custom_string::CustomString transport(*it);
      SDL_LOG_TRACE("Service Allowed to run on " << transport.c_str()
                                                 << " transport");

      if (!fPrimaryAdded &&
          (transport.CompareIgnoreCase(primary_connection_type.c_str()) ||
           (transport.CompareIgnoreCase("IAP_USB") &&
            primary_transport_type == impl::TransportType::TT_USB))) {
        SDL_LOG_TRACE("Service allowed on primary transport");
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
          SDL_LOG_TRACE("Service allowed on secondary transport");
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

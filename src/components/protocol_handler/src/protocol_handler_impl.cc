/**
 * \file ProtocolHandler.cpp
 * \brief ProtocolHandler class source file.
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

#include "protocol_handler/protocol_handler_impl.h"

#include <memory.h>

#include "connection_handler/connection_handler_impl.h"
#include "config_profile/profile.h"

namespace protocol_handler {

#ifdef ENABLE_LOG
log4cxx::LoggerPtr ProtocolHandlerImpl::logger_ = log4cxx::LoggerPtr(
    log4cxx::Logger::getLogger("ProtocolHandler"));
#endif // ENABLE_LOG


/**
 * Function return packet data as std::string.
 * If packet data is not printable return error message
 */
std::string ConvertPacketDataToString(const uint8_t *data,
                                      const std::size_t data_size);

const size_t kStackSize = 32768;

class ProtocolHandlerImpl::IncomingDataHandler {
 public:
  IncomingDataHandler() : connections_data_() {}

  bool ProcessData(const RawMessagePtr tm_message,
                   std::vector<ProtocolFramePtr>* out_frames) {
    const ConnectionID connection_id = tm_message->connection_key();
    const uint8_t* data = tm_message->data();
    const std::size_t size = tm_message->data_size();
    LOG4CXX_TRACE(logger_, "Start of processing incoming data of size "
                               << size << " for connection " << connection_id);
    const uint32_t kBytesForSizeDetection = 8;
    ConnectionsData::iterator it = connections_data_.find(connection_id);
    if (connections_data_.end() == it) {
      LOG4CXX_ERROR(logger_, "ProcessData requested for unknown connection");
      return false;
    }
    std::vector<uint8_t>& connection_data = it->second;
    connection_data.insert(connection_data.end(), data, data + size);

    LOG4CXX_TRACE(logger_, "Total data size for connection "
                               << connection_id << " is "
                               << connection_data.size());
    while (connection_data.size() >= kBytesForSizeDetection) {
      const uint32_t packet_size = GetPacketSize(&connection_data[0]);
      if (0 == packet_size) {
        LOG4CXX_ERROR(logger_, "Failed to get packet size");
        return false;
      }
      LOG4CXX_TRACE(logger_, "Packet size " << packet_size);
      if (connection_data.size() >= packet_size) {
        ProtocolFramePtr frame(new protocol_handler::ProtocolPacket(
            connection_id, &connection_data[0], packet_size));
        out_frames->push_back(frame);
        connection_data.erase(connection_data.begin(),
                              connection_data.begin() + packet_size);
        LOG4CXX_TRACE(logger_,
                      "Packet created and passed, new data size for connection "
                          << connection_id << " is " << connection_data.size());
      } else {
        LOG4CXX_TRACE(logger_, "Packet data is not available yet");
        return true;
      }
    }
    return true;
  }

  void AddConnection(ConnectionID connection_id) {
    connections_data_[connection_id];
  }

  void RemoveConnection(ConnectionID connection_id) {
    connections_data_.erase(connection_id);
  }

 private:
  /**
   * @brief Returns size of frame to be formed from raw bytes.
   * expects first bytes of message which will be treated as frame header.
   */
  uint32_t GetPacketSize(unsigned char* received_bytes) {
    DCHECK(received_bytes);
    unsigned char offset = sizeof(uint32_t);
    unsigned char version = received_bytes[0] >> 4u;
    uint32_t frame_body_size = received_bytes[offset++] << 24u;
    frame_body_size |= received_bytes[offset++] << 16u;
    frame_body_size |= received_bytes[offset++] << 8u;
    frame_body_size |= received_bytes[offset++];

    uint32_t required_size = frame_body_size;
    switch (version) {
      case PROTOCOL_VERSION_1:
        required_size += PROTOCOL_HEADER_V1_SIZE;
        break;
      case PROTOCOL_VERSION_3:
      case PROTOCOL_VERSION_2:
        required_size += PROTOCOL_HEADER_V2_SIZE;
        break;
      default:
        LOG4CXX_ERROR(logger_, "Unknown protocol version.");
        return 0;
    }
    return required_size;
  }

  typedef std::map<ConnectionID, std::vector<uint8_t> > ConnectionsData;
  ConnectionsData connections_data_;
};

ProtocolHandlerImpl::ProtocolHandlerImpl(
    transport_manager::TransportManager* transport_manager_param)
    : protocol_observers_(),
      session_observer_(0),
      transport_manager_(transport_manager_param),
      kPeriodForNaviAck(5),
      incoming_data_handler_(new IncomingDataHandler),
      raw_ford_messages_from_mobile_("MessagesFromMobileAppHandler", this,
                                     threads::ThreadOptions(kStackSize)),
      raw_ford_messages_to_mobile_("MessagesToMobileAppHandler", this,
                                   threads::ThreadOptions(kStackSize)),
      metric_observer_(NULL) {
  LOG4CXX_TRACE_ENTER(logger_);

  LOG4CXX_TRACE_EXIT(logger_);
}

ProtocolHandlerImpl::~ProtocolHandlerImpl() {
  if (!protocol_observers_.empty()) {
    LOG4CXX_WARN(logger_, "Not all observers have unsubscribed"
                 " from ProtocolHandlerImpl");
  }
}

void ProtocolHandlerImpl::AddProtocolObserver(ProtocolObserver* observer) {
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to IProtocolObserver.");
    return;
  }

  protocol_observers_.insert(observer);
}

void ProtocolHandlerImpl::RemoveProtocolObserver(ProtocolObserver* observer) {
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to IProtocolObserver.");
    return;
  }

  protocol_observers_.erase(observer);
}

void ProtocolHandlerImpl::set_session_observer(SessionObserver* observer) {
  if (!observer) {
    LOG4CXX_ERROR(logger_, "Invalid (NULL) pointer to ISessionObserver.");
    return;
  }

  session_observer_ = observer;
}

void ProtocolHandlerImpl::SetMetricObserver(PHMetricObserver* observer) {
  metric_observer_ = observer;
}

void ProtocolHandlerImpl::SendStartSessionAck(ConnectionID connection_id,
                                              uint8_t session_id,
                                              uint8_t protocol_version,
                                              uint32_t hash_code,
                                              uint8_t service_type) {
  LOG4CXX_TRACE_ENTER(logger_);

  ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(connection_id,
      PROTOCOL_VERSION_3, COMPRESS_OFF, FRAME_TYPE_CONTROL,
      service_type, FRAME_DATA_START_SERVICE_ACK, session_id,
      0, hash_code));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_INFO(logger_,
               "sendStartSessionAck() for connection " << connection_id
               << " for service_type " << static_cast<int32_t>(service_type)
               << " session_id " << static_cast<int32_t>(session_id));

  LOG4CXX_TRACE_EXIT(logger_);
}

void ProtocolHandlerImpl::SendStartSessionNAck(ConnectionID connection_id,
                                               uint8_t session_id,
                                               uint8_t protocol_version,
                                               uint8_t service_type) {
  LOG4CXX_TRACE_ENTER(logger_);

  ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(connection_id,
      protocol_version, COMPRESS_OFF, FRAME_TYPE_CONTROL,
      service_type, FRAME_DATA_START_SERVICE_NACK,
      session_id, 0, 0));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_INFO(logger_,
               "sendStartSessionNAck() for connection " << connection_id
               << " for service_type " << static_cast<int32_t>(service_type)
               << " session_id " << static_cast<int32_t>(session_id));

  LOG4CXX_TRACE_EXIT(logger_);
}

void ProtocolHandlerImpl::SendEndSessionNAck(ConnectionID connection_id,
                                             uint32_t session_id,
                                             uint8_t protocol_version,
                                             uint8_t service_type) {
  LOG4CXX_TRACE_ENTER(logger_);

  ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(connection_id,
      protocol_version, COMPRESS_OFF, FRAME_TYPE_CONTROL,
      service_type, FRAME_DATA_END_SERVICE_NACK,
      session_id, 0, 0));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_INFO(logger_, "SendEndSessionNAck() for connection " << connection_id
               << " for service_type " << static_cast<int32_t>(service_type)
               << " session_id " << static_cast<int32_t>(session_id));

  LOG4CXX_TRACE_EXIT(logger_);
}

void ProtocolHandlerImpl::SendEndSessionAck(ConnectionID connection_id,
                                            uint8_t session_id,
                                            uint8_t protocol_version,
                                            uint32_t hash_code,
                                            uint8_t service_type) {
  LOG4CXX_TRACE_ENTER(logger_);

  ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(connection_id,
      protocol_version, COMPRESS_OFF, FRAME_TYPE_CONTROL,
      service_type, FRAME_DATA_END_SERVICE_ACK, session_id, 0,
      hash_code));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_INFO(logger_,
               "SendEndSessionAck() for connection " << connection_id
               << " for service_type " << static_cast<int32_t>(service_type)
               << " session_id " << static_cast<int32_t>(session_id));

  LOG4CXX_TRACE_EXIT(logger_);
}

RESULT_CODE ProtocolHandlerImpl::SendHeartBeatAck(ConnectionID connection_id,
                                                  uint8_t session_id,
                                                  uint32_t message_id) {
  LOG4CXX_TRACE_ENTER(logger_);

  ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(connection_id,
      PROTOCOL_VERSION_3, COMPRESS_OFF, FRAME_TYPE_CONTROL,
      SERVICE_TYPE_ZERO, FRAME_DATA_HEART_BEAT_ACK, session_id,
      0, message_id));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, false));

  LOG4CXX_TRACE_EXIT(logger_);
  return RESULT_OK;
}

void ProtocolHandlerImpl::SendMessageToMobileApp(const RawMessagePtr& message,
                                                 bool final_message) {
  LOG4CXX_TRACE_ENTER(logger_);
  if (!message) {
    LOG4CXX_ERROR(logger_,
        "Invalid message for sending to mobile app is received.");
    LOG4CXX_TRACE_EXIT(logger_);
    return;
  }

  uint32_t maxDataSize = 0;
  if (PROTOCOL_VERSION_1 == message->protocol_version()) {
    maxDataSize = MAXIMUM_FRAME_DATA_SIZE - PROTOCOL_HEADER_V1_SIZE;
  } else {
    maxDataSize = MAXIMUM_FRAME_DATA_SIZE - PROTOCOL_HEADER_V2_SIZE;
  }

  if (!session_observer_) {
    LOG4CXX_ERROR(
        logger_,
        "Cannot handle message to mobile app:" <<
        " ISessionObserver doesn't exist.");
    return;
  }
  uint32_t connection_handle = 0;
  uint8_t sessionID = 0;
  session_observer_->PairFromKey(message->connection_key(), &connection_handle,
                                 &sessionID);

  if (message->data_size() <= maxDataSize) {
    RESULT_CODE result = SendSingleFrameMessage(connection_handle, sessionID,
                                                message->protocol_version(),
                                                SERVICE_TYPE_RPC,
                                                message->data_size(),
                                                message->data(), false,
                                                final_message);
    if (result != RESULT_OK) {
      LOG4CXX_ERROR(logger_,
          "ProtocolHandler failed to send single frame message.");
    }
  } else {
    LOG4CXX_INFO_EXT(
        logger_,
        "Message will be sent in multiple frames; max size is " << maxDataSize);

    RESULT_CODE result = SendMultiFrameMessage(connection_handle, sessionID,
                                               message->protocol_version(),
                                               SERVICE_TYPE_RPC,
                                               message->data_size(),
                                               message->data(), false,
                                               maxDataSize, final_message);
    if (result != RESULT_OK) {
      LOG4CXX_ERROR(logger_,
          "ProtocolHandler failed to send multiframe messages.");
    }
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

void ProtocolHandlerImpl::OnTMMessageReceived(const RawMessagePtr tm_message) {
  LOG4CXX_TRACE_ENTER(logger_);
  connection_handler::ConnectionHandlerImpl* connection_handler =
      connection_handler::ConnectionHandlerImpl::instance();
  // Connection handler should be accessed from TM thread only
  connection_handler->KeepConnectionAlive(tm_message->connection_key());

  if (tm_message) {
    LOG4CXX_INFO_EXT(
        logger_,
        "Received from TM " << tm_message->data() << " with connection id "
                            << tm_message->connection_key() << " msg data_size "
                            << tm_message->data_size());
  } else {
    LOG4CXX_ERROR(
        logger_,
        "Invalid incoming message received in"
        << " ProtocolHandler from Transport Manager.");
    return;
  }

  std::vector<ProtocolFramePtr> protocol_frames;
  const bool ok =
      incoming_data_handler_->ProcessData(tm_message, &protocol_frames);
  if (!ok) {
    LOG4CXX_ERROR(logger_,
                  "Incoming data processing failed. Terminating connection.");
    transport_manager_->DisconnectForce(tm_message->connection_key());
  }

  for (std::vector<ProtocolFramePtr>::const_iterator it =
           protocol_frames.begin();
       it != protocol_frames.end(); ++it) {
    impl::RawFordMessageFromMobile msg(*it);
    if (metric_observer_) {
      metric_observer_->StartMessageProcess(msg->message_id());
    }
    printf("MYLOG OnTMMessageReceived %d \n", msg->message_id());
    raw_ford_messages_from_mobile_.PostMessage(msg);
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

void ProtocolHandlerImpl::OnTMMessageReceiveFailed(
    const transport_manager::DataReceiveError& error) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Received error on attemping to recieve message.");
}

void ProtocolHandlerImpl::NotifySubscribers(const RawMessagePtr& message) {
  for (ProtocolObservers::iterator it = protocol_observers_.begin();
      protocol_observers_.end() != it; ++it) {
    (*it)->OnMessageReceived(message);
  }
}

void ProtocolHandlerImpl::OnTMMessageSend(const RawMessagePtr message) {
  LOG4CXX_INFO(logger_, "Sending message finished successfully.");

  uint32_t connection_handle = 0;
  uint8_t sessionID = 0;
  const ProtocolPacket sent_message(message->connection_key(),
                                    message->data(),
                                    message->data_size());

  std::map<uint8_t, uint32_t>::iterator it =
      sessions_last_message_id_.find(sent_message.session_id());

  if (sessions_last_message_id_.end() != it) {
    uint32_t last_message_id = it->second;
    sessions_last_message_id_.erase(it);
    if ((sent_message.message_id() ==  last_message_id) &&
        ((FRAME_TYPE_SINGLE == sent_message.frame_type()) ||
        ((FRAME_TYPE_CONSECUTIVE == sent_message.frame_type()) &&
         (0 == sent_message.frame_data())))) {

      session_observer_->PairFromKey(message->connection_key(),
                                       &connection_handle,
                                       &sessionID);
      transport_manager_->Disconnect(connection_handle);
    }
  }

  for (ProtocolObservers::iterator it = protocol_observers_.begin();
      protocol_observers_.end() != it; ++it) {
    (*it)->OnMobileMessageSent(message);
  }
}

void ProtocolHandlerImpl::OnTMMessageSendFailed(
    const transport_manager::DataSendError& error,
    const RawMessagePtr& message) {
  // TODO(PV): implement
  LOG4CXX_ERROR(logger_, "Sending message " <<
      message-> data() << " failed.");
}

void ProtocolHandlerImpl::OnConnectionEstablished(
    const transport_manager::DeviceInfo& device_info,
    const transport_manager::ConnectionUID& connection_id) {
  incoming_data_handler_->AddConnection(connection_id);
}

void ProtocolHandlerImpl::OnConnectionClosed(
    const transport_manager::ConnectionUID& connection_id) {
  incoming_data_handler_->RemoveConnection(connection_id);
}

RESULT_CODE ProtocolHandlerImpl::SendFrame(ConnectionID connection_id,
                                           const ProtocolPacket& packet) {
  LOG4CXX_TRACE_ENTER(logger_);

  if (!packet.packet()) {
    LOG4CXX_ERROR(logger_, "Failed to create packet.");

    LOG4CXX_TRACE_EXIT(logger_);
    return RESULT_FAIL;
  }

  LOG4CXX_INFO_EXT(
      logger_,
      "Packet to be sent: " << packet.packet() <<
      " of size: " << packet.packet_size());

  if (!session_observer_) {
    LOG4CXX_WARN(logger_, "No session_observer_ set.");
    return RESULT_FAIL;
  }

  RawMessagePtr message_to_send(
      new RawMessage(connection_id, packet.protocol_version(), packet.packet(),
                     packet.packet_size()));

  LOG4CXX_INFO(logger_,
               "Message to send with connection id " << connection_id);

  if (transport_manager_) {
    if (transport_manager::E_SUCCESS !=
            transport_manager_->SendMessageToDevice(message_to_send)) {
        LOG4CXX_WARN(logger_, "Can't send message to device");
        return RESULT_FAIL;
    };
  } else {
    LOG4CXX_WARN(logger_, "No Transport Manager found.");
    LOG4CXX_TRACE_EXIT(logger_);
    return RESULT_FAIL;
  }

  LOG4CXX_TRACE_EXIT(logger_);
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::SendSingleFrameMessage(
    ConnectionID connection_id, const uint8_t session_id,
    uint32_t protocol_version, const uint8_t service_type,
    const uint32_t data_size, const uint8_t* data, const bool compress,
    const bool is_final_message) {
  LOG4CXX_TRACE_ENTER(logger_);

  ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(connection_id,
      protocol_version, compress, FRAME_TYPE_SINGLE, service_type, 0,
      session_id, data_size, message_counters_[session_id]++, data));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(ptr, is_final_message));

  LOG4CXX_TRACE_EXIT(logger_);
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::SendMultiFrameMessage(
    ConnectionID connection_id, const uint8_t session_id,
    uint32_t protocol_version, const uint8_t service_type,
    const uint32_t data_size, const uint8_t* data, const bool compress,
    const uint32_t maxdata_size, const bool is_final_message) {
  LOG4CXX_TRACE_ENTER(logger_);
  RESULT_CODE retVal = RESULT_OK;

  LOG4CXX_INFO_EXT(
      logger_, " data size " << data_size << " maxdata_size " << maxdata_size);

  int32_t numOfFrames = 0;
  int32_t lastdata_size = 0;

  if (data_size % maxdata_size) {
    numOfFrames = (data_size / maxdata_size) + 1;
    lastdata_size = data_size % maxdata_size;
  } else {
    numOfFrames = data_size / maxdata_size;
    lastdata_size = maxdata_size;
  }

  LOG4CXX_INFO_EXT(
      logger_,
      "Data size " << data_size << " of " << numOfFrames <<
      " frames with last frame " << lastdata_size);

  uint8_t* outDataFirstFrame = new uint8_t[FIRST_FRAME_DATA_SIZE];
  outDataFirstFrame[0] = data_size >> 24;
  outDataFirstFrame[1] = data_size >> 16;
  outDataFirstFrame[2] = data_size >> 8;
  outDataFirstFrame[3] = data_size;

  outDataFirstFrame[4] = numOfFrames >> 24;
  outDataFirstFrame[5] = numOfFrames >> 16;
  outDataFirstFrame[6] = numOfFrames >> 8;
  outDataFirstFrame[7] = numOfFrames;

  ProtocolFramePtr firstPacket(new protocol_handler::ProtocolPacket(connection_id,
      protocol_version, compress, FRAME_TYPE_FIRST, service_type, 0,
      session_id, FIRST_FRAME_DATA_SIZE, ++message_counters_[session_id],
      outDataFirstFrame));

  raw_ford_messages_to_mobile_.PostMessage(
      impl::RawFordMessageToMobile(firstPacket, false));
  LOG4CXX_INFO_EXT(logger_, "First frame is sent.");

  uint8_t* outDataFrame = new uint8_t[maxdata_size];
  for (uint32_t i = 0; i < numOfFrames; i++) {
    if (i != (numOfFrames - 1)) {
      memcpy(outDataFrame, data + (maxdata_size * i), maxdata_size);

      ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(connection_id,
          protocol_version, compress, FRAME_TYPE_CONSECUTIVE,
          service_type, ((i % FRAME_DATA_MAX_VALUE) + 1), session_id,
          maxdata_size, message_counters_[session_id], outDataFrame));

      raw_ford_messages_to_mobile_.PostMessage(
          impl::RawFordMessageToMobile(ptr, false));

    } else {
      memcpy(outDataFrame, data + (maxdata_size * i), lastdata_size);

      ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(connection_id,
          protocol_version, compress, FRAME_TYPE_CONSECUTIVE,
          service_type, 0x0, session_id, lastdata_size,
          message_counters_[session_id], outDataFrame));

      raw_ford_messages_to_mobile_.PostMessage(
          impl::RawFordMessageToMobile(ptr, is_final_message));
    }
  }

  LOG4CXX_TRACE_EXIT(logger_);
  return retVal;
}

RESULT_CODE ProtocolHandlerImpl::HandleMessage(ConnectionID connection_id,
                                               const ProtocolFramePtr& packet) {
  LOG4CXX_TRACE_ENTER(logger_);
  printf("\t\tMYLOG HandleMessage %d \n", packet->message_id());
  switch (packet->frame_type()) {
    case FRAME_TYPE_CONTROL: {
      LOG4CXX_INFO(logger_, "handleMessage(1) - case FRAME_TYPE_CONTROL");
      LOG4CXX_TRACE_EXIT(logger_);
      return HandleControlMessage(connection_id, packet);
    }
    case FRAME_TYPE_SINGLE: {
      LOG4CXX_INFO(
          logger_,
            "FRAME_TYPE_SINGLE message of size " << packet->data_size() << "; message "
            << ConvertPacketDataToString(packet->data(), packet->data_size()));

      if (!session_observer_) {
        LOG4CXX_ERROR(
            logger_,
            "Cannot handle message from Transport"
            << " Manager: ISessionObserver doesn't exist.");

        LOG4CXX_TRACE_EXIT(logger_);
        return RESULT_FAIL;
      }

      int32_t connection_key = session_observer_->KeyFromPair(
          connection_id, packet->session_id());

      RawMessagePtr raw_message(
          new RawMessage(connection_key, packet->protocol_version(), packet->data(),
                         packet->data_size(), packet->service_type()));
      if (metric_observer_) {
        PHMetricObserver::MessageMetric* metric = new PHMetricObserver::MessageMetric();
        metric->message_id = packet->message_id();
        metric->connection_key = connection_key;
        metric->raw_msg = raw_message;
        metric_observer_->EndMessageProcess(metric);
      }
      printf("\t\t\tMYLOG BEFORE NOTIFY %d \n", packet->message_id());
      NotifySubscribers(raw_message);
      break;
    }
    case FRAME_TYPE_FIRST:
    case FRAME_TYPE_CONSECUTIVE: {
      LOG4CXX_INFO(logger_, "handleMessage() - case FRAME_TYPE_CONSECUTIVE");

      LOG4CXX_TRACE_EXIT(logger_);
      return HandleMultiFrameMessage(connection_id, packet);
    }
    default: {
      LOG4CXX_WARN(logger_, "handleMessage() - case default!!!");
      return RESULT_FAIL;
    }
  }

  LOG4CXX_TRACE_EXIT(logger_);
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleMultiFrameMessage(
    ConnectionID connection_id, const ProtocolFramePtr& packet) {
  LOG4CXX_TRACE_ENTER(logger_);
  printf("\t\t\t HandleMultiFrameMessage %d \n", packet.get());
  if (!session_observer_) {
    LOG4CXX_ERROR(logger_, "No ISessionObserver set.");

    LOG4CXX_TRACE_EXIT(logger_);
    return RESULT_FAIL;
  }

  int32_t key = session_observer_->KeyFromPair(connection_id,
                                               packet->session_id());
  LOG4CXX_INFO_EXT(
      logger_,
      "Packet " << packet << "; session id " << static_cast<int32_t>(key));

  if (packet->frame_type() == FRAME_TYPE_FIRST) {
    LOG4CXX_INFO(logger_, "handleMultiFrameMessage() - FRAME_TYPE_FIRST "
                 << packet->data_size());
    incomplete_multi_frame_messages_[key] = packet;
  } else {
    LOG4CXX_INFO(logger_, "handleMultiFrameMessage() - Consecutive frame");

    std::map<int32_t, ProtocolFramePtr>::iterator it =
        incomplete_multi_frame_messages_.find(key);

    if (it == incomplete_multi_frame_messages_.end()) {
      LOG4CXX_ERROR(
          logger_, "Frame of multiframe message for non-existing session id");

      LOG4CXX_TRACE_EXIT(logger_);
      return RESULT_FAIL;
    }

    if (it->second->appendData(packet->data(), packet->data_size())
        != RESULT_OK) {
      LOG4CXX_ERROR(logger_,
          "Failed to append frame for multiframe message.");

      LOG4CXX_TRACE_EXIT(logger_);
      return RESULT_FAIL;
    }

    if (packet->frame_data() == FRAME_DATA_LAST_FRAME) {

      LOG4CXX_INFO(
          logger_,
          "Last frame of multiframe message size " << packet->data_size()
              << "; connection key " << key);

      if (protocol_observers_.empty()) {
        LOG4CXX_ERROR(
            logger_,
            "Cannot handle multiframe message: no IProtocolObserver is set.");

        LOG4CXX_TRACE_EXIT(logger_);
        return RESULT_FAIL;
      }

      ProtocolPacket* completePacket = it->second.get();
      RawMessagePtr rawMessage (new RawMessage(
          key, completePacket->protocol_version(), completePacket->data(),
          completePacket->total_data_bytes(), completePacket->service_type()));
      if (metric_observer_) {
        PHMetricObserver::MessageMetric* metric = new PHMetricObserver::MessageMetric();
        metric->raw_msg = rawMessage;
        metric_observer_->EndMessageProcess(metric);
      }
      printf("\t\t\tMYLOG BEFORE NOTIFY MULTIFRAME %d \n", completePacket->message_id());
      NotifySubscribers(rawMessage);

      incomplete_multi_frame_messages_.erase(it);
    }
  }

  LOG4CXX_TRACE_EXIT(logger_);
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessage(
    ConnectionID connection_id, const ProtocolFramePtr& packet) {
  printf("\t\t\tMYLOG HandleControlMessage %d \n", packet->message_id());
  if (!session_observer_) {
    LOG4CXX_ERROR(logger_, "ISessionObserver is not set.");

    LOG4CXX_TRACE_EXIT(logger_);
    return RESULT_FAIL;
  }

  switch (packet->frame_data()) {
    case FRAME_DATA_START_SERVICE:
      return HandleControlMessageStartSession(connection_id, *(packet.get()));
    case FRAME_DATA_END_SERVICE:
      return HandleControlMessageEndSession(connection_id, *(packet.get()));
    case FRAME_DATA_HEART_BEAT: {
      LOG4CXX_INFO(logger_,
                   "Received heart beat for connection " << connection_id);
      return HandleControlMessageHeartBeat(connection_id, *(packet.get()));
    }
    default:
      LOG4CXX_WARN(
          logger_,
          "Control message of type " << int32_t(packet->frame_data())
              << " ignored");
      return RESULT_OK;
  }
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageEndSession(
    ConnectionID connection_id, const ProtocolPacket& packet) {
  LOG4CXX_INFO(logger_,
               "ProtocolHandlerImpl::HandleControlMessageEndSession()");

  uint8_t current_session_id = packet.session_id();

  uint32_t hash_code = 0;
  if (1 != packet.protocol_version()) {
    hash_code = packet.message_id();
  }

  bool success = true;
  int32_t session_hash_code = session_observer_->OnSessionEndedCallback(
      connection_id, current_session_id, hash_code,
      ServiceTypeFromByte(packet.service_type()));

  if (-1 != session_hash_code) {
    if (1 != packet.protocol_version()) {
      if (packet.message_id() != session_hash_code) {
        success = false;
      }
    }
  } else {
    success = false;
  }

  if (success) {
    SendEndSessionAck(
        connection_id, current_session_id, packet.protocol_version(),
        session_observer_->KeyFromPair(connection_id, current_session_id),
        packet.service_type());
    message_counters_.erase(current_session_id);
  } else {
    LOG4CXX_INFO_EXT(
        logger_,
        "Refused to end session " << packet.service_type() << " type.");
    SendEndSessionNAck(connection_id, current_session_id, packet.protocol_version(),
                       packet.service_type());
  }
  printf("\t\t\t\tMYLOG HandleControlMessageEndSession %d \n", packet.message_id());
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageStartSession(
    ConnectionID connection_id, const ProtocolPacket& packet) {
  LOG4CXX_INFO_EXT(logger_,
                   "Protocol version: " <<
                   static_cast<int>(packet.protocol_version()));

  int32_t session_id = session_observer_->OnSessionStartedCallback(
      connection_id, packet.session_id(),
      ServiceTypeFromByte(packet.service_type()));

  if (-1 != session_id) {
    SendStartSessionAck(
        connection_id, session_id, packet.protocol_version(),
        session_observer_->KeyFromPair(connection_id, session_id),
        packet.service_type());
  } else {
    LOG4CXX_INFO_EXT(
        logger_,
        "Refused to create service " <<
        static_cast<int32_t>(packet.service_type()) << " type.");

    SendStartSessionNAck(connection_id, packet.session_id(), packet.protocol_version(),
                         packet.service_type());
  }
  printf("\t\t\t\tMYLOG HandleControlMessageStartSession %d \n", packet.message_id());
  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::HandleControlMessageHeartBeat(
    ConnectionID connection_id, const ProtocolPacket& packet) {
  LOG4CXX_INFO(
      logger_,
      "Sending heart beat acknowledgment for connection " << connection_id);
  printf("\t\t\t\tMYLOG HandleControlMessageHeartBeat %d \n", packet.message_id());
  return SendHeartBeatAck(connection_id, packet.session_id(),
                          packet.message_id());
}

void ProtocolHandlerImpl::Handle(
    const impl::RawFordMessageFromMobile& message) {
  LOG4CXX_TRACE_ENTER(logger_);

  if (((0 != message->data()) && (0 != message->data_size())) ||
      FRAME_TYPE_CONTROL == message->frame_type() ||
      FRAME_TYPE_FIRST == message->frame_type()) {

      LOG4CXX_INFO_EXT(logger_, "Packet: dataSize " << message->data_size());
      printf("\tMYLOG Handle %d \n", message->message_id());
      HandleMessage(message->connection_key(), message);
  } else {
    LOG4CXX_WARN(logger_,
                 "handleMessagesFromMobileApp() - incorrect or NULL data");
  }
  LOG4CXX_TRACE_EXIT(logger_);
}

void ProtocolHandlerImpl::Handle(const impl::RawFordMessageToMobile& message) {
  LOG4CXX_INFO_EXT(
      logger_,
      "Message to mobile app: connection " << message->connection_key() << ";"
      " dataSize: " << message->data_size() << " ;"
      " protocolVersion " << message->protocol_version());

  if (message.is_final) {
    sessions_last_message_id_.insert(
        std::pair<uint8_t, uint32_t>(message->session_id(),
                                     message->message_id()));
  }

  SendFrame(message->connection_key(), (*message.get()));
}

void ProtocolHandlerImpl::SendFramesNumber(int32_t connection_key,
                                           int32_t number_of_frames) {
  LOG4CXX_INFO(logger_,
               "SendFramesNumber MobileNaviAck for session " << connection_key);

  transport_manager::ConnectionUID   connection_id = 0;
  uint8_t session_id = 0;
  session_observer_->PairFromKey(connection_key, &connection_id, &session_id);
  ProtocolFramePtr ptr(new protocol_handler::ProtocolPacket(connection_id,
      PROTOCOL_VERSION_3, COMPRESS_OFF, FRAME_TYPE_CONTROL,
      SERVICE_TYPE_NAVI, FRAME_DATA_SERVICE_DATA_ACK,
      session_id, 0, number_of_frames));

  raw_ford_messages_to_mobile_.PostMessage(
        impl::RawFordMessageToMobile(ptr, false));
}

void ProtocolHandlerImpl::SetTimeMetricObserver(PHMetricObserver* observer) {
  metric_observer_ = observer;
}

std::string ConvertPacketDataToString(const uint8_t* data,
                                      const std::size_t data_size) {
  if (0 == data_size)
    return std::string();
  bool is_printable_array = true;
  std::locale loc;
  const char* text = reinterpret_cast<const char*>(data);
  // Check data for printability
  for (int i = 0; i < data_size; ++i) {
    if (!std::isprint(text[i], loc)) {
      is_printable_array = false;
      break;
    }
  }
  return is_printable_array ? std::string(text) : std::string("is raw data");
}

}  // namespace protocol_handler

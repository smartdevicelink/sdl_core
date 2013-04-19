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

#include <memory.h>

#include "TransportManager/ITransportManager.hpp"
#include "protocol_handler/session_observer.h"
#include "protocol_handler/protocol_observer.h"
#include "protocol_handler/protocol_handler_impl.h"
#include "protocol_handler/message_from_mobile_app_handler.h"
#include "protocol_handler/messages_to_mobile_app_handler.h"

#include "Utils/macro.h"

namespace protocol_handler {

log4cplus::Logger ProtocolHandlerImpl::logger_ = log4cplus::Logger::getInstance(
    LOG4CPLUS_TEXT("ProtocolHandler"));

ProtocolHandlerImpl::ProtocolHandlerImpl(
    NsSmartDeviceLink::NsTransportManager::ITransportManager* transportManager)
    : protocol_observer_(0),
      session_observer_(0),
      transport_manager_(transportManager),
      handle_messages_from_mobile_app_(NULL),
      handle_messages_to_mobile_app_(NULL) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);

  handle_messages_from_mobile_app_ = new threads::Thread(
      "MessagesFromMobileAppHandler", new MessagesFromMobileAppHandler(this));
  handle_messages_from_mobile_app_->startWithOptions(
      threads::ThreadOptions(threads::Thread::kMinStackSize));

  handle_messages_to_mobile_app_ = new threads::Thread(
      "MessagesToMobileAppHandler", new MessagesToMobileAppHandler(this));
  handle_messages_to_mobile_app_->startWithOptions(
      threads::ThreadOptions(threads::Thread::kMinStackSize));
}

ProtocolHandlerImpl::~ProtocolHandlerImpl() {
  handle_messages_from_mobile_app_->stop();
  delete handle_messages_from_mobile_app_;
  handle_messages_from_mobile_app_ = NULL;

  handle_messages_to_mobile_app_->stop();
  delete handle_messages_to_mobile_app_;
  handle_messages_to_mobile_app_ = NULL;

  protocol_observer_ = 0;
  session_observer_ = 0;
  transport_manager_ = 0;
}

void ProtocolHandlerImpl::set_protocol_observer(ProtocolObserver* observer) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);
  if (!observer) {
    LOG4CPLUS_ERROR(logger_, "Invalid (NULL) pointer to IProtocolObserver.");
    return;
  }

  protocol_observer_ = observer;
}

void ProtocolHandlerImpl::set_session_observer(SessionObserver* observer) {
  if (!observer) {
    LOG4CPLUS_ERROR(logger_, "Invalid (NULL) pointer to ISessionObserver.");
    return;
  }

  session_observer_ = observer;
}

void ProtocolHandlerImpl::sendEndSessionNAck(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    unsigned int sessionID, unsigned char serviceType) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);

  ProtocolPacket packet(PROTOCOL_VERSION_2, COMPRESS_OFF, FRAME_TYPE_CONTROL,
                        0x0, FRAME_DATA_END_SESSION_NACK, sessionID, 0, 0);

  if (RESULT_OK == sendFrame(connectionHandle, packet)) {
    LOG4CPLUS_INFO(logger_, "sendStartSessionAck() - BT write OK");
  } else {
    LOG4CPLUS_ERROR(logger_, "sendStartSessionAck() - BT write FAIL");
  }
}

void ProtocolHandlerImpl::sendStartSessionAck(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    unsigned char sessionID, unsigned char protocolVersion,
    unsigned int hashCode, unsigned char serviceType) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);

  ProtocolPacket packet(protocolVersion, COMPRESS_OFF, FRAME_TYPE_CONTROL,
                        serviceType, FRAME_DATA_START_SESSION_ACK, sessionID, 0,
                        hashCode);

  if (RESULT_OK == sendFrame(connectionHandle, packet)) {
    LOG4CPLUS_INFO(
        logger_,
        "sendStartSessionAck() for connection " << connectionHandle
          << " for serviceType " << serviceType << " sessionID " << sessionID);
  } else {
    LOG4CPLUS_ERROR(logger_, "sendStartSessionAck() - BT write FAIL");
  }
}

void ProtocolHandlerImpl::sendStartSessionNAck(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    unsigned char serviceType) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);

  unsigned char versionFlag = PROTOCOL_VERSION_1;

  ProtocolPacket packet(versionFlag, COMPRESS_OFF, FRAME_TYPE_CONTROL,
                        serviceType, FRAME_DATA_START_SESSION_NACK, 0x0, 0, 0);

  if (RESULT_OK == sendFrame(connectionHandle, packet)) {
    LOG4CPLUS_INFO(logger_, "sendStartSessionAck() - BT write OK");
  } else {
    LOG4CPLUS_ERROR(logger_, "sendStartSessionAck() - BT write FAIL");
  }
}

void ProtocolHandlerImpl::sendMessageToMobileApp(const RawMessage* message) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);
  if (!message) {
    LOG4CPLUS_ERROR(logger_,
                    "Invalid message for sending to mobile app is received.");
    return;
  }
  messages_to_mobile_app_.push(message);
}

void ProtocolHandlerImpl::onFrameReceived(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    const uint8_t* data, size_t dataSize) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);
  if (connectionHandle && dataSize > 0 && data) {
    LOG4CPLUS_INFO_EXT(logger_,
                       "Received from TM " << data << "; size " << dataSize);
    IncomingMessage * message = new IncomingMessage;
    message->data = new unsigned char[dataSize];
    memcpy(message->data, data, dataSize);
    message->data_size = dataSize;
    message->connection_handle = connectionHandle;
    messages_from_mobile_app_.push(message);
  } else {
    LOG4CPLUS_ERROR(
        logger_,
        "Invalid incoming message received in"
          << " ProtocolHandler from Transport Manager.");
  }
}

void ProtocolHandlerImpl::onFrameSendCompleted(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    int userData,
    NsSmartDeviceLink::NsTransportManager::ESendStatus sendStatus) {
  if (NsSmartDeviceLink::NsTransportManager::SendStatusOK != sendStatus) {
    LOG4CPLUS_ERROR(logger_, "Failed to send frame with number " << userData);
  }
}

RESULT_CODE ProtocolHandlerImpl::sendFrame(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    const ProtocolPacket& packet) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);
  if (!packet.packet()) {
    LOG4CPLUS_ERROR(logger_, "Failed to create packet.");
    return RESULT_FAIL;
  }

  LOG4CPLUS_INFO_EXT(
      logger_,
      "Packet to be sent: " << packet.packet()
        << " of size: " << packet.packet_size());

  if (transport_manager_) {
    transport_manager_->sendFrame(connectionHandle, packet.packet(),
                                  packet.packet_size(),
                                  static_cast<int>(packet.frame_data()));
  } else {
    LOG4CPLUS_WARN(logger_, "No Transport Manager found.");
    return RESULT_FAIL;
  }

  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::sendSingleFrameMessage(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    const unsigned char sessionID, unsigned int protocolVersion,
    const unsigned char servType, const unsigned int dataSize,
    const unsigned char* data, const bool compress) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);

  unsigned char versionF = PROTOCOL_VERSION_1;
  if (2 == protocolVersion) {
    versionF = PROTOCOL_VERSION_2;
  }

  ProtocolPacket packet(versionF, compress, FRAME_TYPE_SINGLE, servType, 0,
                        sessionID, dataSize, message_counters_[sessionID]++,
                        data);

  return sendFrame(connectionHandle, packet);
}

RESULT_CODE ProtocolHandlerImpl::sendMultiFrameMessage(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    const unsigned char sessionID, unsigned int protocolVersion,
    const unsigned char servType, const unsigned int dataSize,
    const unsigned char* data, const bool compress,
    const unsigned int maxDataSize) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);
  RESULT_CODE retVal = RESULT_OK;

  LOG4CPLUS_INFO_EXT(
      logger_, " data size " << dataSize << " maxDataSize " << maxDataSize);

  unsigned char versionF = PROTOCOL_VERSION_1;
  if (2 == protocolVersion) {
    versionF = PROTOCOL_VERSION_2;
  }

  int numOfFrames = 0;
  int lastDataSize = 0;

  if (dataSize % maxDataSize) {
    numOfFrames = (dataSize / maxDataSize) + 1;
    lastDataSize = dataSize % maxDataSize;
  } else {
    numOfFrames = dataSize / maxDataSize;
    lastDataSize = maxDataSize;
  }

  LOG4CPLUS_INFO_EXT(
      logger_,
      "Data size " << dataSize << " of " << numOfFrames
        << " frames with last frame " << lastDataSize);

  unsigned char *outDataFirstFrame = new unsigned char[FIRST_FRAME_DATA_SIZE];
  outDataFirstFrame[0] = dataSize >> 24;
  outDataFirstFrame[1] = dataSize >> 16;
  outDataFirstFrame[2] = dataSize >> 8;
  outDataFirstFrame[3] = dataSize;

  outDataFirstFrame[4] = numOfFrames >> 24;
  outDataFirstFrame[5] = numOfFrames >> 16;
  outDataFirstFrame[6] = numOfFrames >> 8;
  outDataFirstFrame[7] = numOfFrames;

  ProtocolPacket firstPacket(versionF, compress, FRAME_TYPE_FIRST, servType, 0,
                             sessionID, FIRST_FRAME_DATA_SIZE,
                             ++message_counters_[sessionID], outDataFirstFrame);

  retVal = sendFrame(connectionHandle, firstPacket);

  LOG4CPLUS_INFO_EXT(logger_, "First frame is sent.");

  delete[] outDataFirstFrame;

  unsigned char* outDataFrame = new unsigned char[maxDataSize];

  for (unsigned int i = 0; i < numOfFrames; i++) {
    if (i != (numOfFrames - 1)) {
      memcpy(outDataFrame, data + (maxDataSize * i), maxDataSize);

      ProtocolPacket packet(versionF, compress, FRAME_TYPE_CONSECUTIVE,
                            servType, ((i % FRAME_DATA_MAX_VALUE) + 1),
                            sessionID, maxDataSize,
                            message_counters_[sessionID], outDataFrame);

      retVal = sendFrame(connectionHandle, packet);
      if (RESULT_FAIL == retVal) {
        break;
      }
    } else {
      memcpy(outDataFrame, data + (maxDataSize * i), lastDataSize);

      ProtocolPacket packet(versionF, compress, FRAME_TYPE_CONSECUTIVE,
                            servType, 0x0, sessionID, lastDataSize,
                            message_counters_[sessionID], outDataFrame);

      retVal = sendFrame(connectionHandle, packet);
    }
  }

  delete[] outDataFrame;

  return retVal;
}

RESULT_CODE ProtocolHandlerImpl::handleMessage(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    ProtocolPacket* packet) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);

  switch (packet->frame_type()) {
    case FRAME_TYPE_CONTROL: {
      LOG4CPLUS_INFO(logger_, "handleMessage() - case FRAME_TYPE_CONTROL");

      return handleControlMessage(connectionHandle, packet);
    }
    case FRAME_TYPE_SINGLE: {
      LOG4CPLUS_INFO(
          logger_,
          "FRAME_TYPE_SINGLE: of size " << packet->data_size()
            << ";message " << packet -> data());

      if (!session_observer_) {
        LOG4CPLUS_ERROR(
            logger_,
            "Cannot handle message from Transport"
              << " Manager: ISessionObserver doesn't exist.");
        return RESULT_FAIL;
      }

      int connectionKey = session_observer_->keyFromPair(connectionHandle,
                                                         packet->session_id());

      RawMessage* rawMessage = new RawMessage(connectionKey, packet->version(),
                                              packet->data(),
                                              packet->data_size());

      if (protocol_observer_)
        protocol_observer_->onMessageReceived(rawMessage);

      break;
    }
    case FRAME_TYPE_FIRST:
    case FRAME_TYPE_CONSECUTIVE: {
      LOG4CPLUS_INFO(logger_, "handleMessage() - case FRAME_TYPE_CONSECUTIVE");

      return handleMultiFrameMessage(connectionHandle, packet);
    }
    default: {
      LOG4CPLUS_WARN(logger_, "handleMessage() - case default!!!");
    }
  }

  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::handleMultiFrameMessage(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    ProtocolPacket* packet) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);

  if (!session_observer_) {
    LOG4CPLUS_ERROR(logger_, "No ISessionObserver set.");
    return RESULT_FAIL;
  }

  LOG4CPLUS_INFO_EXT(
      logger_, "Packet " << packet << "; sessionID " << packet -> session_id());

  int key = session_observer_->keyFromPair(connectionHandle,
                                           packet->session_id());

  if (packet->frame_type() == FRAME_TYPE_FIRST) {
    LOG4CPLUS_INFO(logger_, "handleMultiFrameMessage() - FRAME_TYPE_FIRST");

    // const unsigned char* data = packet -> getData();
    unsigned int totalDataBytes = packet->data()[0] << 24;
    totalDataBytes |= packet->data()[1] << 16;
    totalDataBytes |= packet->data()[2] << 8;
    totalDataBytes |= packet->data()[3];

    packet->set_total_data_bytes(totalDataBytes);

    incomplete_multi_frame_messages_[key] = packet;
  } else {
    LOG4CPLUS_INFO(logger_, "handleMultiFrameMessage() - Consecutive frame");

    std::map<int, ProtocolPacket*>::iterator it =
        incomplete_multi_frame_messages_.find(key);

    if (it == incomplete_multi_frame_messages_.end()) {
      LOG4CPLUS_ERROR(
          logger_, "Frame of multiframe message for non-existing session id");
      return RESULT_FAIL;
    }

    // LOG4CPLUS_INFO(mLogger, "Found message " << it->second->)

    if (it->second->appendData(packet->data(), packet->data_size())
        != RESULT_OK) {
      LOG4CPLUS_ERROR(logger_,
                      "Failed to append frame for multiframe message.");
      return RESULT_FAIL;
    }

    if (packet->frame_data() == FRAME_DATA_LAST_FRAME) {
      if (!protocol_observer_) {
        LOG4CPLUS_ERROR(
            logger_,
            "Cannot handle multiframe message: no IProtocolObserver is set.");
        return RESULT_FAIL;
      }

      ProtocolPacket* completePacket = it->second;
      RawMessage* rawMessage = new RawMessage(
          key, completePacket->version(), completePacket->data(),
          completePacket->total_data_bytes());

      protocol_observer_->onMessageReceived(rawMessage);

      incomplete_multi_frame_messages_.erase(it);
    }
  }

  return RESULT_OK;
}

RESULT_CODE ProtocolHandlerImpl::handleControlMessage(
    NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    const ProtocolPacket* packet) {
  LOG4CPLUS_TRACE_METHOD(logger_, __PRETTY_FUNCTION__);

  if (!session_observer_) {
    LOG4CPLUS_ERROR(logger_, "ISessionObserver is not set.");
    return RESULT_FAIL;
  }

  if (packet->frame_data() == FRAME_DATA_END_SESSION) {
    LOG4CPLUS_INFO(logger_, "handleControlMessage() - FRAME_DATA_END_SESSION");

    unsigned char currentSessionID = packet->session_id();

    unsigned int hashCode = 0;
    if (packet->version() == 2) {
      hashCode = packet->message_id();
    }

    bool success = true;
    int sessionHashCode = session_observer_->onSessionEndedCallback(
        connectionHandle, currentSessionID, hashCode);

    if (-1 != sessionHashCode) {
      // TODO(PV): check hash code only for second version of protocol.
      if (2 == packet->version()) {
        if (packet->message_id() != sessionHashCode) {
          success = false;
        }
      }
    } else {
      success = false;
    }

    if (success) {
      message_counters_.erase(currentSessionID);
    } else {
      LOG4CPLUS_INFO_EXT(
          logger_,
          "Refused to end session " << packet -> service_type() << " type.");
      sendEndSessionNAck(connectionHandle, currentSessionID,
                         packet->service_type());
    }
  }

  if (packet->frame_data() == FRAME_DATA_START_SESSION) {
    LOG4CPLUS_INFO(logger_,
                   "handleControlMessage() - FRAME_DATA_START_SESSION");
    LOG4CPLUS_INFO_EXT(
        logger_, "Version 2 " << (packet -> version() == PROTOCOL_VERSION_2));

    int sessionId = session_observer_->onSessionStartedCallback(
        connectionHandle);
    if (-1 != sessionId) {
      sendStartSessionAck(
          connectionHandle, sessionId, packet->version(),
          session_observer_->keyFromPair(connectionHandle, sessionId),
          packet->service_type());
    } else {
      LOG4CPLUS_INFO_EXT(
          logger_,
          "Refused to create session " << packet -> service_type() << " type.");
      sendStartSessionNAck(connectionHandle, packet->service_type());
    }
  }

  return RESULT_OK;
}

}  // namespace protocol_handler

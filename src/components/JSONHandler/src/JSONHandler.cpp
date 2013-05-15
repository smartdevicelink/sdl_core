/**
* \file JSONHandler.cpp
* \brief JSONHandler class source file.
* Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
*/

#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <json/reader.h>
#include <json/writer.h>

#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/SDLRPCObjects/V1/Marshaller.h"
#include "JSONHandler/SDLRPCObjects/V2/Marshaller.h"
#include "JSONHandler/incoming_thread_impl.h"
#include "JSONHandler/outgoing_thread_impl.h"

namespace
{
    enum ProtocolVersion
    {
        INVALID_PROTOCOL = 0,
        V1 = 1,
        V2 = 2
    };

    // TODO(AK): Remove this after new code generation.
    class InvalidResponseFactory
    {
    public:
        typedef std::map<std::string, NsSmartDeviceLinkRPC::Marshaller::Methods> Responses;

        typedef NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal FunctionID;
        typedef NsSmartDeviceLinkRPC::Marshaller::Methods Methods;

        static NsSmartDeviceLinkRPC::SDLRPCResponse* getResponse(FunctionID functionId,
                                                        int correlationId)
        {
            NsSmartDeviceLinkRPC::SDLRPCResponse* response = NULL;

            switch (functionId)
            {
            case FunctionID::AlertID:
                response = new NsSmartDeviceLinkRPCV2::Alert_response();
                break;
            case FunctionID::DeleteFileID:
                response = new NsSmartDeviceLinkRPCV2::DeleteFile_response();
                break;
            case FunctionID::GetDTCsID:
                response = new NsSmartDeviceLinkRPCV2::GetDTCs_response();
                break;
            case FunctionID::GetVehicleDataID:
                response = new NsSmartDeviceLinkRPCV2::GetVehicleData_response();
                break;
            case FunctionID::ListFilesID:
                response = new NsSmartDeviceLinkRPCV2::ListFiles_response();
                break;
            case FunctionID::PerformInteractionID:
                response = new NsSmartDeviceLinkRPCV2::PerformInteraction_response();
                break;
            case FunctionID::PutFileID:
                response = new NsSmartDeviceLinkRPCV2::PutFile_response();
                break;
            case FunctionID::ReadDIDID:
                response = new NsSmartDeviceLinkRPCV2::ReadDID_response();
                break;
            case FunctionID::RegisterAppInterfaceID:
                response = new NsSmartDeviceLinkRPCV2::RegisterAppInterface_response();
                break;
            case FunctionID::SliderID:
                response = new NsSmartDeviceLinkRPCV2::Slider_response();
                break;
            case FunctionID::SubscribeVehicleDataID:
                response = new NsSmartDeviceLinkRPCV2::SubscribeVehicleData_response();
                break;
            case FunctionID::UnsubscribeVehicleDataID:
                response = new NsSmartDeviceLinkRPCV2::UnsubscribeVehicleData_response();
                break;
            default:
                response = new NsSmartDeviceLinkRPC::SDLRPCResponse(V2, functionId);
                break;
            }

            response->setMethodId(static_cast<FunctionID>(functionId));
            response->setCorrelationID(correlationId);
            response->set_success(false);
            response->set_resultCode(NsSmartDeviceLinkRPCV2::Result::INVALID_DATA);
            return response;
        }

        static NsSmartDeviceLinkRPC::SDLRPCResponse* getResponse(const std::string& jsonString)
        {
            Responses responses;
            responses.insert(std::make_pair("PerformInteraction", Methods::METHOD_PERFORMINTERACTION_RESPONSE));
            responses.insert(std::make_pair("RegisterAppInterface", Methods::METHOD_REGISTERAPPINTERFACE_RESPONSE));

            std::string methodName;
            int correlationID = -1;

            try
            {
                Json::Reader reader;
                Json::Value json;
                if (!reader.parse(jsonString, json, false) ||
                    !json.isObject() ||
                    !json.isMember("request") ||
                    !json["request"].isObject())
                {
                    return NULL;
                }

                const Json::Value& jsonResponse = json["request"];

                if (!jsonResponse.isMember("name") || !jsonResponse["name"].isString())
                {
                    return NULL;
                }
                methodName = jsonResponse["name"].asString();

                if (!jsonResponse.isMember("correlationID") || !jsonResponse["correlationID"].isInt())
                {
                    return NULL;
                }
                correlationID = jsonResponse["correlationID"].asInt();
            }
            catch (...)
            {
                return NULL;
            }

            Methods methodId = Methods::METHOD_INVALID;
            Responses::const_iterator item = responses.find(methodName);
            if (item != responses.end())
            {
                methodId = item->second;
            }

            NsSmartDeviceLinkRPC::SDLRPCResponse* response = NULL;
            switch (methodId)
            {
            case Methods::METHOD_PERFORMINTERACTION_RESPONSE:
                response = new NsSmartDeviceLinkRPC::PerformInteraction_response();
                break;
            case Methods::METHOD_REGISTERAPPINTERFACE_RESPONSE:
                response = new NsSmartDeviceLinkRPC::RegisterAppInterface_response();
                break;
            default:
                response = new NsSmartDeviceLinkRPC::SDLRPCResponse(V1, methodId);
                break;
            }

            response->set_success(false);
            response->set_resultCode(NsSmartDeviceLinkRPCV2::Result::INVALID_DATA);
            response->setCorrelationID(correlationID);
            return response;
        }

    private:
        InvalidResponseFactory();
        ~InvalidResponseFactory();
    };
}


log4cxx::LoggerPtr JSONHandler::logger_ =
    log4cxx::LoggerPtr(log4cxx::Logger::getLogger("JSONHandler"));

JSONHandler::JSONHandler(protocol_handler::ProtocolHandler* protocolHandler)
    : mProtocolHandler(protocolHandler) {

  incoming_thread_ = new threads::Thread(
      "json_handler::IncomingThreadImpl",
      new json_handler::IncomingThreadImpl(this));
  incoming_thread_->startWithOptions(
      threads::ThreadOptions(threads::Thread::kMinStackSize));

  outgoing_thread_ = new threads::Thread(
      "json_handler::OutgoingThreadImpl",
      new json_handler::OutgoingThreadImpl(this));
  outgoing_thread_->startWithOptions(
      threads::ThreadOptions(threads::Thread::kMinStackSize));
}

JSONHandler::~JSONHandler()
{
    incoming_thread_->stop();
    delete incoming_thread_;
    incoming_thread_ = NULL;

    outgoing_thread_->stop();
    delete outgoing_thread_;
    outgoing_thread_ = NULL;

    mProtocolHandler = 0;
    mMessagesObserver = 0;
}

/*Methods for IRPCMessagesObserver*/
void JSONHandler::setRPCMessagesObserver(IRPCMessagesObserver* messagesObserver)
{
    if (!messagesObserver)
    {
        LOG4CXX_ERROR(logger_, "Invalid (null) pointer to IRPCMessagesObserver.");
    }
    mMessagesObserver = messagesObserver;
}

void JSONHandler::sendRPCMessage(const NsSmartDeviceLinkRPC::SDLRPCMessage* message, int connectionKey)
{
    LOG4CXX_INFO(logger_, "An outgoing message has been received");
    if (message)
    {
        mOutgoingMessages.push(std::make_pair(connectionKey, message));
    }
}
/*End of methods for IRPCMessagesObserver*/
void JSONHandler::setProtocolHandler(protocol_handler::ProtocolHandler* protocolHandler)
{
    if (!protocolHandler)
    {
        LOG4CXX_ERROR(logger_, "Invalid (null) pointer to ProtocolHandler.");
        return;
    }
    mProtocolHandler = protocolHandler;
}

void JSONHandler::onMessageReceived(const protocol_handler::RawMessage* message)
{
    if (!message)
    {
        LOG4CXX_ERROR(logger_, "Received invalid message from ProtocolHandler.");
        return;
    }

    LOG4CXX_INFO(logger_, "Received message from mobile App: " << message->data()
                   << " of size " << message->data_size());

    mIncomingMessages.push(message);
}
/*end of methods from IProtocolObserver*/

std::string JSONHandler::clearEmptySpaces(const std::string& input)
{
    std::string str = input;
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    return str;
}

NsSmartDeviceLinkRPC::SDLRPCMessage* JSONHandler::handleIncomingMessageProtocolV1(
    const protocol_handler::RawMessage* message)
{
    std::string jsonMessage = std::string((const char*)message->data(), message->data_size());

    if (jsonMessage.length() == 0)
    {
        LOG4CXX_ERROR(logger_, "Received invalid json packet.");
        return 0;
    }

    std::string jsonCleanMessage = clearEmptySpaces(jsonMessage);

    NsSmartDeviceLinkRPC::SDLRPCMessage* messageObject = NsSmartDeviceLinkRPC::Marshaller::fromString(jsonCleanMessage);
    LOG4CXX_INFO_EXT(logger_, "Received a message from mobile side: "
                       << "\n" << NsSmartDeviceLinkRPC::Marshaller::toJSON(messageObject));

    if (!messageObject)
    {
        LOG4CXX_WARN(logger_, "Invalid mobile message received.");

        NsSmartDeviceLinkRPC::SDLRPCResponse* response = InvalidResponseFactory::getResponse(jsonMessage);
        if (!response)
        {
            LOG4CXX_ERROR(logger_, "new NsSmartDeviceLinkRPC::ALRPCMessage failed...");
            return NULL;
        }

        sendRPCMessage(response, message->connection_key());
        return NULL;
    }

    return messageObject;
}

NsSmartDeviceLinkRPC::SDLRPCMessage* JSONHandler::handleIncomingMessageProtocolV2(
    const protocol_handler::RawMessage* message)
{
    unsigned char* receivedData = message->data();
    unsigned char offset = 0;
    unsigned char firstByte = receivedData[offset++];

    int rpcType = -1;
    unsigned char rpcTypeFlag = firstByte >> 4u;
    switch (rpcTypeFlag)
    {
    case RPC_REQUEST:
        rpcType = 0;
        break;
    case RPC_RESPONSE:
        rpcType = 1;
        break;
    case RPC_NOTIFICATION:
        rpcType = 2;
        break;
    }
    LOG4CXX_INFO_EXT(logger_, "RPC Type of the message is " << rpcType << " from flag " << rpcTypeFlag);

    unsigned int functionId = firstByte >> 8u;

    functionId <<= 24u;
    functionId |= receivedData[offset++] << 16u;
    functionId |= receivedData[offset++] << 8u;
    functionId |= receivedData[offset++];

    LOG4CXX_INFO_EXT(logger_, "FunctionId is " << functionId);

    unsigned int correlationId = receivedData[offset++] << 24u;
    correlationId |= receivedData[offset++] << 16u;
    correlationId |= receivedData[offset++] << 8u;
    correlationId |= receivedData[offset++];

    LOG4CXX_INFO_EXT(logger_, "\t\t\tCorrelation id " << correlationId);

    unsigned int jsonSize = receivedData[offset++] << 24u;
    jsonSize |= receivedData[offset++] << 16u;
    jsonSize |= receivedData[offset++] << 8u;
    jsonSize |= receivedData[offset++];

    LOG4CXX_INFO_EXT(logger_, "Json size is " << jsonSize);

    if (jsonSize > message->data_size())
    {
        LOG4CXX_ERROR(logger_, "Received invalid json packet header.");
        return 0;
    }

    std::string jsonMessage = std::string((const char*)receivedData + offset, jsonSize);
    if (jsonMessage.length() == 0)
    {
        LOG4CXX_ERROR(logger_, "Received invalid json packet.");
        return 0;
    }

    std::string jsonCleanMessage = clearEmptySpaces(jsonMessage);

    Json::Reader reader;
    Json::Value json;

    if (!reader.parse(jsonCleanMessage, json, false))
    {
        LOG4CXX_ERROR(logger_, "Received invalid json string.");
        return 0;
    }
    //TODO (PV): temporary solution, will be fixed after changes to codegeneration
    LOG4CXX_INFO_EXT(logger_, "Received from mobile side: " << "\n" << json);

    Json::Value tempSolution;
    tempSolution["parameters"] = json;

    LOG4CXX_INFO_EXT(logger_, "Added params: " << "\n" << tempSolution);

    /*NsSmartDeviceLinkRPC::SDLRPCMessage * messageObject = NsSmartDeviceLinkRPCV2::Marshaller::fromString(
        jsonCleanMessage,
        static_cast<NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal>(functionId),
        static_cast<NsSmartDeviceLinkRPCV2::messageType::messageTypeInternal>(rpcType) );*/

    NsSmartDeviceLinkRPC::SDLRPCMessage* messageObject = NsSmartDeviceLinkRPCV2::Marshaller::fromJSON(
                tempSolution,
                static_cast<NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal>(functionId),
                static_cast<NsSmartDeviceLinkRPCV2::messageType::messageTypeInternal>(rpcType));

    //LOG4CXX_INFO_EXT(logger_, "Received a message from mobile side: " <<
    // std::endl << NsSmartDeviceLinkRPCV2::Marshaller::toJSON(
    //   messageObject, static_cast<NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal>(functionId),
    //   static_cast<NsSmartDeviceLinkRPCV2::messageType::messageTypeInternal>(rpcType)));

    if (message -> data_size() > offset + jsonSize)
    {
        // unsigned int binarySize = message->getDataSize() - offset - jsonSize;
        std::vector<unsigned char> binaryData(receivedData + offset + jsonSize,
                                              receivedData + message->data_size());
        LOG4CXX_INFO_EXT(logger_, "Binary data is present in message.");
        messageObject -> setBinaryData(binaryData);
    }

    if (!messageObject)
    {
        LOG4CXX_WARN(logger_, "Invalid mobile message received.");
        NsSmartDeviceLinkRPC::SDLRPCResponse* response = InvalidResponseFactory::getResponse(
                static_cast<NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal>(functionId), correlationId);
        if (!response)
        {
            LOG4CXX_ERROR(logger_, "new NsSmartDeviceLinkRPC::ALRPCMessage failed...");
            return NULL;
        }

        sendRPCMessage(response, message->connection_key());
        return NULL;
    }
    else
    {
        messageObject->setMethodId(functionId);
    }

    if (RPC_REQUEST == rpcTypeFlag || RPC_RESPONSE == rpcTypeFlag)
    {
        messageObject->setCorrelationID(correlationId);
    }

    return messageObject;
}

protocol_handler::RawMessage* JSONHandler::handleOutgoingMessageProtocolV1(int connectionKey,
        const NsSmartDeviceLinkRPC::SDLRPCMessage*   message)
{
    LOG4CXX_INFO_EXT(logger_, "handling a message " << message->getMethodId() << " protocol 1");
    LOG4CXX_INFO_EXT(logger_, "message text: " << "\n" << NsSmartDeviceLinkRPC::Marshaller::toJSON(message));
    std::string messageString = NsSmartDeviceLinkRPC::Marshaller::toString(message);

    if (messageString.length() == 0)
    {
        LOG4CXX_ERROR(logger_, "Failed to serialize ALRPCMessage object version 1.");
        return 0;
    }

    unsigned char* rawMessage = new unsigned char[messageString.length() + 1];
    memcpy(rawMessage, messageString.c_str(), messageString.length() + 1);

    protocol_handler::RawMessage* msgToProtocolHandler = new protocol_handler::RawMessage(
        connectionKey,
        1,
        rawMessage,
        messageString.length() + 1);

    return msgToProtocolHandler;
}

protocol_handler::RawMessage* JSONHandler::handleOutgoingMessageProtocolV2(int connectionKey,
        const NsSmartDeviceLinkRPC::SDLRPCMessage*   message)
{
    LOG4CXX_INFO_EXT(logger_, "handling a message " << message->getMethodId() << " protocol 2");
    Json::Value json = NsSmartDeviceLinkRPCV2::Marshaller::toJSON(message,
                       static_cast<NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal>(message -> getMethodId()),
                       static_cast<NsSmartDeviceLinkRPCV2::messageType::messageTypeInternal>(message -> getMessageType()));

    if (json.isNull())
    {
        if (NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal::OnAudioPassThruID == message->getMethodId())
        {
            LOG4CXX_INFO_EXT(logger_, "Handling OnAudioPassThru message with 0 length!");
            // akara
            // Workaround to have no JSON string in OnAudioPassThru notification
            // This notification contains audio data only.
            const uint MAX_HEADER_SIZE = 12;
            unsigned int jsonSize = 0;
            unsigned int binarySize = 0;
            if (message->getBinaryData())
            {
                binarySize = message->getBinaryData()->size();
            }
            unsigned char* dataForSending = new unsigned char[MAX_HEADER_SIZE + jsonSize + binarySize];
            unsigned char offset = 0;

            unsigned char rpcTypeFlag = RPC_NOTIFICATION;

            unsigned int functionId = message->getMethodId();
            dataForSending[offset++] = ((rpcTypeFlag << 4) & 0xF0) | (functionId >> 24);
            dataForSending[offset++] = functionId >> 16;
            dataForSending[offset++] = functionId >> 8;
            dataForSending[offset++] = functionId;

            unsigned int correlationId = message->getCorrelationID();
            LOG4CXX_INFO_EXT(logger_, "\t\t\tCorrelation ID is " << correlationId);
            dataForSending[offset++] = correlationId >> 24;
            dataForSending[offset++] = correlationId >> 16;
            dataForSending[offset++] = correlationId >> 8;
            dataForSending[offset++] = correlationId;

            dataForSending[offset++] = jsonSize >> 24;
            dataForSending[offset++] = jsonSize >> 16;
            dataForSending[offset++] = jsonSize >> 8;
            dataForSending[offset++] = jsonSize;

            if (message->getBinaryData())
            {
                const std::vector<unsigned char>& binaryData = *(message->getBinaryData());
                unsigned char* currentPointer = dataForSending + offset + jsonSize;
                for (unsigned int i = 0; i < binarySize; ++i)
                {
                    currentPointer[i] = binaryData[i];
                }
            }

            protocol_handler::RawMessage* msgToProtocolHandler = new protocol_handler::RawMessage(
                connectionKey,
                2,
                dataForSending,
                MAX_HEADER_SIZE + jsonSize + binarySize);

            return msgToProtocolHandler;
        }
        else
        {
            LOG4CXX_ERROR(logger_, "Failed to serialize ALRPCMessage object version 2.");
            return 0;
        }
    }

    LOG4CXX_INFO_EXT(logger_, "Message to be sent to mobile app \n" << json["parameters"]);

    //TODO (PV): temporary solution, will be fixed after changes to codegeneration
    Json::FastWriter writer;
    std::string messageString = writer.write(json["parameters"]);/*NsSmartDeviceLinkRPCV2::Marshaller::toString( message,
                        static_cast<NsSmartDeviceLinkRPCV2::FunctionID::FunctionIDInternal>(message -> getMethodId()),
                        static_cast<NsSmartDeviceLinkRPCV2::messageType::messageTypeInternal>(message -> getMessageType()) );*/

    //LOG4CXX_INFO_EXT(logger_, "message text: " << std::endl << json );
    if (messageString.length() == 0)
    {
        LOG4CXX_ERROR(logger_, "Failed to serialize ALRPCMessage object version 2.");
        return 0;
    }

    const uint MAX_HEADER_SIZE = 12;
    unsigned int jsonSize = messageString.length() + 1;
    unsigned int binarySize = 0;
    if (message->getBinaryData())
    {
        binarySize = message->getBinaryData()->size();
    }
    unsigned char* dataForSending = new unsigned char[MAX_HEADER_SIZE + jsonSize + binarySize];
    unsigned char offset = 0;

    unsigned char rpcTypeFlag = 0;
    switch (message->getMessageType())
    {
    case 0:
        rpcTypeFlag = RPC_REQUEST;
        break;
    case 1:
        rpcTypeFlag = RPC_RESPONSE;
        break;
    case 2:
        rpcTypeFlag = RPC_NOTIFICATION;
        break;
    }

    unsigned int functionId = message->getMethodId();
    dataForSending[offset++] = ((rpcTypeFlag << 4) & 0xF0) | (functionId >> 24);
    dataForSending[offset++] = functionId >> 16;
    dataForSending[offset++] = functionId >> 8;
    dataForSending[offset++] = functionId;

    unsigned int correlationId = message->getCorrelationID();
    LOG4CXX_INFO_EXT(logger_, "\t\t\tCorrelation ID is " << correlationId);
    dataForSending[offset++] = correlationId >> 24;
    dataForSending[offset++] = correlationId >> 16;
    dataForSending[offset++] = correlationId >> 8;
    dataForSending[offset++] = correlationId;

    dataForSending[offset++] = jsonSize >> 24;
    dataForSending[offset++] = jsonSize >> 16;
    dataForSending[offset++] = jsonSize >> 8;
    dataForSending[offset++] = jsonSize;

    memcpy(dataForSending + offset, messageString.c_str(), jsonSize);

    if (message->getBinaryData())
    {
        const std::vector<unsigned char>& binaryData = *(message->getBinaryData());
        unsigned char* currentPointer = dataForSending + offset + jsonSize;
        for (unsigned int i = 0; i < binarySize; ++i)
        {
            currentPointer[i] = binaryData[i];
        }
    }

    protocol_handler::RawMessage* msgToProtocolHandler = new protocol_handler::RawMessage(
        connectionKey,
        2,
        dataForSending,
        MAX_HEADER_SIZE + jsonSize + binarySize);

    return msgToProtocolHandler;
}

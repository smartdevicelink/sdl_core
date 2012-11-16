#include <memory.h>
#include <iostream>
#include <stdio.h>
#include <time.h>

#include "TransportManager/ITransportManager.hpp"

#include "ProtocolHandler.hpp"
#include "ProtocolPacketHeader.hpp"
#include "IProtocolObserver.hpp"

namespace AxisCore
{

Logger ProtocolHandler::mLogger = Logger::getInstance(LOG4CPLUS_TEXT("AxisCore.ProtocolHandler") );

ProtocolHandler::ProtocolHandler(IProtocolObserver *observer,
                                 NsAppLink::NsTransportManager::ITransportManager * transportManager,
                                 const UInt8 protocolVersion) :
                NsAppLink::NsTransportManager::ITransportManagerDataListener(),
                mConnectionHandle(NsAppLink::NsTransportManager::InvalidConnectionHandle),
                mProtocolObserver(observer),
                mTransportManager(transportManager),
                mProtocolVersion(protocolVersion),
                mSessionIdCounter(1)
{    
    //PropertyConfigurator::doConfigure(LOG4CPLUS_TEXT("log4cplus.properties") );
    if (0 != mTransportManager)
    {
        mTransportManager->addDataListener(this);
    }
    
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    LOG4CPLUS_INFO(mLogger, "   !!!!   ****    !!!!   ****    !!!!   >>>>FUCKYEA!!!<<<<< ProtocolHandler consturcted");
    srand(time(0) );
}

ProtocolHandler::~ProtocolHandler()
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    std::map<UInt8, std::queue<Message *> >::iterator it;
    for (it = mToUpperLevelMessagesQueues.begin() ; it != mToUpperLevelMessagesQueues.end() ; it++)
    {
        for (unsigned int j = 0 ; j < it->second.size() ; j++)
        {
            delete it->second.front();
            it->second.front() = NULL;
            it->second.pop();
        }
    }

    mToUpperLevelMessagesQueues.clear();

    std::map<UInt8, Message *>::iterator k;
    for (k = mIncompleteMultiFrameMessages.begin(); k != mIncompleteMultiFrameMessages.end() ; k++)
    {
        delete k->second;
        k->second = NULL;
    }
    mIncompleteMultiFrameMessages.clear();
}

ERROR_CODE ProtocolHandler::startSession(const UInt8 servType)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    /**
      * Protocol Version always equals 1 in the start session header
      */
    ProtocolPacketHeader header(PROTOCOL_VERSION_1,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                servType,
                                FRAME_DATA_START_SESSION,
                                0,
                                0,
                                0);

    if (ERR_OK == sendFrame(mConnectionHandle, header, 0))
        LOG4CPLUS_INFO(mLogger, "startSession() - OK");
    else
    {
        LOG4CPLUS_WARN(mLogger, "startSession() - FAIL");
        retVal = ERR_FAIL;
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::endSession(const UInt8 sessionID, const UInt32 hashCode)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    bool correctEndSession = false;
    UInt32 _hashCode = 0;
    if (mProtocolVersion == PROTOCOL_VERSION_2)
    {
        if (mHashCodes.count(sessionID) )
        {
            if (hashCode == mHashCodes[sessionID])
            {
                _hashCode = hashCode;
                correctEndSession = true;
            }
            else
                LOG4CPLUS_WARN(mLogger, "endSession() - incorrect hashCode");
        }
    }
    else if (mProtocolVersion == PROTOCOL_VERSION_1)
        correctEndSession = true;

    if (correctEndSession)
    {
        ProtocolPacketHeader header(mProtocolVersion,
                                    COMPRESS_OFF,
                                    FRAME_TYPE_CONTROL,
                                    SERVICE_TYPE_RPC,
                                    FRAME_DATA_END_SESSION,
                                    sessionID,
                                    0,
                                    _hashCode);

        if (ERR_OK == sendFrame(mConnectionHandle, header, 0))
        {
            LOG4CPLUS_INFO(mLogger, "endSession() - OK\n");
            mHashCodes.erase(sessionID);
        }
        else
        {
            LOG4CPLUS_WARN(mLogger, "endSession() - FAIL");
            retVal = ERR_FAIL;
        }
    }
    else
        retVal = ERR_FAIL;

    return retVal;
}

ERROR_CODE ProtocolHandler::sendSingleFrameMessage(const UInt8 sessionID,
                                                   const UInt8 servType,
                                                   const UInt32 dataSize,
                                                   const UInt8 *data,
                                                   const bool compress)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    ProtocolPacketHeader header(mProtocolVersion,
                                compress,
                                FRAME_TYPE_SINGLE,
                                servType,
                                0,
                                sessionID,
                                dataSize,
                                mMessageCounters[sessionID]++);

    if (ERR_OK != sendFrame(mConnectionHandle, header, data))
    {
        LOG4CPLUS_WARN(mLogger, "sendSingleFrame() - write failed");
        retVal = ERR_FAIL;
    }
    else
        LOG4CPLUS_INFO(mLogger, "sendSingleFrame() - write OK");

    return retVal;
}

ERROR_CODE ProtocolHandler::sendMultiFrameMessage(const UInt8 sessionID,
                                                  const UInt8 servType,
                                                  const UInt32 dataSize,
                                                  const UInt8 *data,
                                                  const bool compress,
                                                  const UInt32 maxDataSize)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    int numOfFrames = 0;
    int lastDataSize = 0;

    if (dataSize % maxDataSize)
    {
        numOfFrames = (dataSize / maxDataSize) + 1;
        lastDataSize = dataSize % maxDataSize;
    }
    else
        numOfFrames = dataSize / maxDataSize;

    ProtocolPacketHeader firstHeader(mProtocolVersion,
                                     compress,
                                     FRAME_TYPE_FIRST,
                                     servType,
                                     0,
                                     sessionID,
                                     FIRST_FRAME_DATA_SIZE,
                                     mMessageCounters[sessionID]);

    UInt8 *outDataFirstFrame = new UInt8[FIRST_FRAME_DATA_SIZE];
    outDataFirstFrame[0] = dataSize >> 24;
    outDataFirstFrame[1] = dataSize >> 16;
    outDataFirstFrame[2] = dataSize >> 8;
    outDataFirstFrame[3] = dataSize;

    outDataFirstFrame[4] = numOfFrames >> 24;
    outDataFirstFrame[5] = numOfFrames >> 16;
    outDataFirstFrame[6] = numOfFrames >> 8;
    outDataFirstFrame[7] = numOfFrames;

    if (ERR_OK != sendFrame(mConnectionHandle, firstHeader, outDataFirstFrame))
    {
        LOG4CPLUS_WARN(mLogger, "sendData() - write first frame FAIL");
        delete [] outDataFirstFrame;
        return ERR_FAIL;
    }
    else
        LOG4CPLUS_INFO(mLogger, "sendData() - write first frame OK");

    delete [] outDataFirstFrame;


    UInt8 *outDataFrame = new UInt8[maxDataSize];

    for (UInt8 i = 0 ; i < numOfFrames ; i++)
    {
        if (i != (numOfFrames - 1) )
        {
            ProtocolPacketHeader header(mProtocolVersion,
                                        compress,
                                        FRAME_TYPE_CONSECUTIVE,
                                        servType,
                                        ( (i % FRAME_DATA_MAX_VALUE) + 1),
                                        sessionID,
                                        maxDataSize,
                                        mMessageCounters[sessionID]);

            memcpy(outDataFrame, data + (maxDataSize * i), maxDataSize);

            if (ERR_OK != sendFrame(mConnectionHandle, header, outDataFrame))
            {
                LOG4CPLUS_WARN(mLogger, "sendData() - write consecutive frame FAIL");
                retVal = ERR_FAIL;
                break;
            }
            else
                LOG4CPLUS_INFO(mLogger, "sendData() - write consecutive frame OK");
        }
        else
        {
            ProtocolPacketHeader header(mProtocolVersion,
                                        compress,
                                        FRAME_TYPE_CONSECUTIVE,
                                        servType,
                                        0x0,
                                        sessionID,
                                        lastDataSize,
                                        mMessageCounters[sessionID]++);

            memcpy(outDataFrame, data + (maxDataSize * i), lastDataSize);

            if (ERR_OK != sendFrame(mConnectionHandle, header, outDataFrame))
            {
                LOG4CPLUS_WARN(mLogger, "sendData() - write last frame FAIL");
                retVal = ERR_FAIL;
                break;
            }
            else
                LOG4CPLUS_INFO(mLogger, "sendData() - write last frame OK");
        }
    }

    delete [] outDataFrame;

    return retVal;
}

ERROR_CODE ProtocolHandler::sendData(const UInt8 sessionID,
                                     const UInt8 servType,
                                     const UInt32 dataSize,
                                     const UInt8 *data,
                                     const bool compress)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    if (mSessionStates.count(sessionID) )
    {
        if (mSessionStates[sessionID] != HANDSHAKE_DONE)
            retVal = ERR_FAIL;
    }
    else
        retVal = ERR_FAIL;

    if (retVal != ERR_FAIL)
    {
        UInt32 maxDataSize = 0;
        if (mProtocolVersion == PROTOCOL_VERSION_1)
            maxDataSize = MAXIMUM_FRAME_SIZE - PROTOCOL_HEADER_V1_SIZE;
        else if (mProtocolVersion == PROTOCOL_VERSION_2)
            maxDataSize = MAXIMUM_FRAME_SIZE - PROTOCOL_HEADER_V2_SIZE;

        if (dataSize <= maxDataSize)
        {
            if (sendSingleFrameMessage(sessionID, servType, dataSize, data, compress) != ERR_OK)
                retVal = ERR_FAIL;
        }
        else
        {
            if (sendMultiFrameMessage(sessionID, servType, dataSize, data, compress, maxDataSize)
                    != ERR_OK)
            {
                retVal = ERR_FAIL;
            }
        }
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::receiveData(const UInt8 sessionID,
                                        const UInt32 messageID,
                                        const UInt8 servType,
                                        const UInt32 receivedDataSize,
                                        UInt8 *data)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    if (mToUpperLevelMessagesQueues.count(sessionID) )
    {
        if (!mToUpperLevelMessagesQueues[sessionID].empty() )
        {
            Message *currentMessage = mToUpperLevelMessagesQueues[sessionID].front();
            if (currentMessage)
            {
                if (receivedDataSize == currentMessage->getTotalDataBytes() )
                {
                    if ( ( (mProtocolVersion == PROTOCOL_VERSION_2)
                           && (messageID == currentMessage->getMessageID() ) )
                         || (mProtocolVersion == PROTOCOL_VERSION_1) )
                    {
                        memcpy(data, currentMessage->getMessageData(), receivedDataSize);

                        delete currentMessage;
                        currentMessage = NULL;
                        mToUpperLevelMessagesQueues[sessionID].pop();
                    }
                    else
                        retVal = ERR_FAIL;
                }
                else
                {
                    LOG4CPLUS_WARN(mLogger, "receiveData() - requested msg size != real Msg size");
                    retVal = ERR_FAIL;
                }
            }
            else
                retVal = ERR_FAIL;
        }
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::sendStartSessionAck(const UInt8 sessionID)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    UInt32 hashCode = 0;
    if (mProtocolVersion == PROTOCOL_VERSION_2)
        hashCode = rand() % 0xFFFFFFFF;

    mHashCodes[sessionID] = hashCode;

    ProtocolPacketHeader header(mProtocolVersion,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                SERVICE_TYPE_RPC,
                                FRAME_DATA_START_SESSION_ACK,
                                sessionID,
                                0,
                                hashCode);

    if (ERR_OK == sendFrame(mConnectionHandle, header, 0))
        LOG4CPLUS_INFO(mLogger, "sendStartSessionAck() - BT write OK");
    else
    {
        LOG4CPLUS_ERROR(mLogger, "sendStartSessionAck() - BT write FAIL");
        retVal = ERR_FAIL;
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::sendEndSessionNAck(const UInt8 sessionID)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    ProtocolPacketHeader header(PROTOCOL_VERSION_2,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                SERVICE_TYPE_RPC,
                                FRAME_DATA_END_SESSION_NACK,
                                sessionID,
                                0,
                                0);

    if (ERR_OK == sendFrame(mConnectionHandle, header, 0))
        LOG4CPLUS_INFO(mLogger, "sendEndSessionNAck() - BT write OK");
    else
    {
        LOG4CPLUS_ERROR(mLogger, "sendEndSessionNAck() - BT write FAIL");
        retVal = ERR_FAIL;
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::handleMessage(const ProtocolPacketHeader &header, UInt8 *data)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    switch (header.frameType)
    {
    case FRAME_TYPE_CONTROL:
    {
        LOG4CPLUS_INFO(mLogger, "handleMessage() - case FRAME_TYPE_CONTROL");

        if (handleControlMessage(header, data) != ERR_OK)
        {
            LOG4CPLUS_WARN(mLogger, "handleMessage() - handleControlMessage FAIL");
            retVal = ERR_FAIL;
        }
        break;
    }
    case FRAME_TYPE_SINGLE:
    {
        LOG4CPLUS_INFO(mLogger, "handleMessage() - case FRAME_TYPE_SINGLE");
        if (mSessionStates.count(header.sessionID) )
        {
            if (mSessionStates[header.sessionID] == HANDSHAKE_DONE)
            {
                Message *message = new Message(header, data, false);
                mToUpperLevelMessagesQueues[header.sessionID].push(message);

                if (mProtocolObserver)
                    mProtocolObserver->dataReceivedCallback(header.sessionID,
                                                            header.messageID,
                                                            header.dataSize);
                else
                    retVal = ERR_FAIL;
            }
            else
            {
                LOG4CPLUS_WARN(mLogger
                         , "handleMessage() - case FRAME_TYPE_SINGLE but HANDSHAKE_NOT_DONE");
                retVal = ERR_FAIL;
            }
        }
        else
        {
            LOG4CPLUS_WARN(mLogger, "handleMessage() - case FRAME_TYPE_SINGLE. unknown sessionID");
            retVal = ERR_FAIL;
        }

        break;
    }
    case FRAME_TYPE_FIRST:
    {
        LOG4CPLUS_INFO(mLogger, "handleMessage() - case FRAME_TYPE_FIRST");
        if (mSessionStates.count(header.sessionID) )
        {
            if (mSessionStates[header.sessionID] == HANDSHAKE_DONE)
                handleMultiFrameMessage(header, data);
            else
            {
                LOG4CPLUS_WARN(mLogger
                      , "handleMessage() - case FRAME_TYPE_FIRST but HANDSHAKE_NOT_DONE");
                retVal = ERR_FAIL;
            }
        }
        else
        {
            LOG4CPLUS_WARN(mLogger, "handleMessage() - case FRAME_TYPE_FIRST. unknown sessionID");
            retVal = ERR_FAIL;
        }

        break;
    }
    case FRAME_TYPE_CONSECUTIVE:
    {
        LOG4CPLUS_INFO(mLogger, "handleMessage() - case FRAME_TYPE_CONSECUTIVE");
        if (mSessionStates.count(header.sessionID) )
        {
            if (mSessionStates[header.sessionID] == HANDSHAKE_DONE)
                handleMultiFrameMessage(header, data);
            else
            {
                LOG4CPLUS_WARN(mLogger
                      , "handleMessage() - case FRAME_TYPE_CONSECUTIVE but HANDSHAKE_NOT_DONE");
                retVal = ERR_FAIL;
            }
        }
        else
        {
            LOG4CPLUS_WARN(mLogger, "handleMessage() - case FRAME_TYPE_CONSECUTIVE. unknown sessionID");
            retVal = ERR_FAIL;
        }

        break;
    }
    default:
    {
        LOG4CPLUS_WARN(mLogger, "handleMessage() - case default!!!");
    }
    }

    delete [] data;
    data = NULL;

    return retVal;
}

ERROR_CODE ProtocolHandler::handleControlMessage(const ProtocolPacketHeader &header, UInt8 *data)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    UInt8 currentSessionID = header.sessionID;
    if (mSessionStates.count(currentSessionID) && (header.frameData != FRAME_DATA_START_SESSION) )
    {
        UInt8 currentState = mSessionStates[currentSessionID];
        switch (currentState)
        {
        case HANDSHAKE_NOT_DONE:
        {
            LOG4CPLUS_INFO(mLogger, "handleControlMessage() - case HANDSHAKE_NOT_DONE");
            if (header.frameData == FRAME_DATA_START_SESSION_ACK)
            {
                LOG4CPLUS_INFO(mLogger, "handleControlMessage() - case FRAME_DATA_START_SESSION_ACK");
                mSessionStates[currentSessionID] = HANDSHAKE_DONE;
                if (header.version == PROTOCOL_VERSION_2)
                    mHashCodes[currentSessionID] = header.messageID;
                if (mProtocolObserver)
                    mProtocolObserver->sessionStartedCallback(currentSessionID, header.messageID);
                else
                    retVal = ERR_FAIL;
            }
            else if (header.frameData == FRAME_DATA_START_SESSION_NACK)
                LOG4CPLUS_INFO(mLogger, "handleControlMessage() - session rejected");
            else
            {
                LOG4CPLUS_WARN(mLogger
                        , "handleControlMessage() - case HANDSHAKE_NOT_DONE invalid frameData");
            }

            break;
        }
        case HANDSHAKE_DONE:
        {
            LOG4CPLUS_INFO(mLogger, "handleControlMessage() - case HANDSHAKE_DONE");
            if (header.frameData == FRAME_DATA_END_SESSION)
            {
                LOG4CPLUS_INFO(mLogger, "handleControlMessage() - FRAME_DATA_END_SESSION");
                bool correctEndSession = false;
                if (header.version == PROTOCOL_VERSION_2)
                {
                    if (header.messageID == mHashCodes[header.sessionID])
                        correctEndSession = true;
                    else
                    {
                        LOG4CPLUS_WARN(mLogger, "handleControlMessage() - incorrect hashCode");
                        if (sendEndSessionNAck(header.sessionID) != ERR_OK)
                            retVal = ERR_FAIL;
                    }
                }
                else if ( (header.version == PROTOCOL_VERSION_1)
                          && (header.version == mProtocolVersion) )
                {
                    correctEndSession = true;
                }

                if (correctEndSession)
                {
                    mSessionStates.erase(currentSessionID);
                    mHashCodes.erase(currentSessionID);
                    if (mProtocolObserver)
                        mProtocolObserver->sessionEndedCallback(currentSessionID);
                    else
                        retVal = ERR_FAIL;
                }
            }
            else
            {
                LOG4CPLUS_WARN(mLogger
                         , "handleControlMessage() - case HANDSHAKE_DONE invalid frameData");
            }

            break;
        }
        default:
        { }
        }
    }
    else
    {
        LOG4CPLUS_INFO(mLogger, "handleControlMessage() - new sessionID");
        if (header.frameData == FRAME_DATA_START_SESSION)
        {
            LOG4CPLUS_INFO(mLogger, "handleControlMessage() - FRAME_DATA_START_SESSION");
            if (sendStartSessionAck(mSessionIdCounter) == ERR_OK)
            {
                mSessionStates.insert(std::pair<UInt8, UInt8>(mSessionIdCounter, HANDSHAKE_DONE) );

                if (mProtocolObserver)
                {
                    mProtocolObserver->sessionStartedCallback(mSessionIdCounter
                                                              , mHashCodes[mSessionIdCounter]);
                }
                else
                    retVal = ERR_FAIL;

                mSessionIdCounter++;
            }
        }
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::handleMultiFrameMessage(const ProtocolPacketHeader &header
                                                  , UInt8 *data)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    ERROR_CODE retVal = ERR_OK;

    if (header.frameType == FRAME_TYPE_FIRST)
    {
        LOG4CPLUS_INFO(mLogger, "handleMultiFrameMessage() - FRAME_TYPE_FIRST");
        Message *multiFrameMessage = new Message(header, data, true);
        mIncompleteMultiFrameMessages[header.sessionID] = multiFrameMessage;
    }
    else
    {
        LOG4CPLUS_INFO(mLogger, "handleMultiFrameMessage() - Consecutive frame");

        if (mIncompleteMultiFrameMessages.count(header.sessionID) )
        {
            if (mIncompleteMultiFrameMessages[header.sessionID]->addConsecutiveMessage(header, data)
                    == ERR_OK)
            {
                LOG4CPLUS_INFO(mLogger, "handleMultiFrameMessage() - addConsecutiveMessage OK");
                if (header.frameData == FRAME_DATA_LAST_FRAME)
                {
                    mToUpperLevelMessagesQueues[header.sessionID]
                            .push(mIncompleteMultiFrameMessages[header.sessionID] );
                    mIncompleteMultiFrameMessages.erase(header.sessionID);

                    if (mProtocolObserver)
                        mProtocolObserver->dataReceivedCallback(header.sessionID
                                  , header.messageID
                                  , mToUpperLevelMessagesQueues[header.sessionID].front()
                                                                ->getTotalDataBytes() );
                    else
                        retVal = ERR_FAIL;
                }
            }
            else
            {
                LOG4CPLUS_WARN(mLogger, "handleMultiFrameMessage() - addConsecutiveMessage FAIL");
                retVal = ERR_FAIL;
            }
        }
        else
        {
            LOG4CPLUS_WARN(mLogger, "handleMultiFrameMessage() - no sessionID in incomplete messages");
            retVal = ERR_FAIL;
        }
    }

    return retVal;
}

void ProtocolHandler::onFrameReceived(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const uint8_t * Data, size_t DataSize)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    
    LOG4CPLUS_INFO(mLogger, "   !!!!   ****    !!!!   ****    !!!!   >>>>FUCKYEA!!!<<<<< onFrameReceived(): DataSize: " << DataSize);
    
    //Temp solution for single connection 
    mConnectionHandle = ConnectionHandle;
    
    //@TODO check for ConnectionHandle.
    //@TODO check for data size - crash is possible.
    if ((0 != Data) && (0 != DataSize) && (MAXIMUM_FRAME_SIZE >= DataSize))
    {        
        ProtocolPacketHeader header;
        
        UInt8 offset = 0;
        UInt8 firstByte = Data[offset];
        offset++;
        
        header.version = firstByte >> 4u;
        if (firstByte & 0x08u)
        {
            header.compress = true;
        }
        else
        {
            header.compress = false;
        }
        
        header.frameType = firstByte & 0x07u;

        header.serviceType = Data[offset++];        
        header.frameData = Data[offset++];        
        header.sessionID = Data[offset++];
        
        header.dataSize  = Data[offset++] << 24u;
        header.dataSize |= Data[offset++] << 16u;
        header.dataSize |= Data[offset++] << 8u;
        header.dataSize |= Data[offset++];
        
        if (header.version == PROTOCOL_VERSION_2)
        {
            header.messageID  = Data[offset++] << 24u;
            header.messageID |= Data[offset++] << 16u;
            header.messageID |= Data[offset++] << 8u;
            header.messageID |= Data[offset++];
        }
        else
        {
            header.messageID = 0u;
        }
        
        const UInt32 dataPayloadSize = DataSize - offset;
        
        if (dataPayloadSize != header.dataSize)
        {
            LOG4CPLUS_ERROR(mLogger, "onFrameReceived() - EPIC FAIL: dataPayloadSize != header.dataSize");
            return;
        }
        
        UInt8 * data = 0;
        if (dataPayloadSize != 0u)
        {
            data = new UInt8[DataSize - offset];
            memcpy(data, Data + offset, dataPayloadSize);
        }
                
        handleMessage(header, data);
    }
    else
    {
        LOG4CPLUS_WARN(mLogger, "onFrameReceived() - incorrect or NULL data");
    }   
}

ERROR_CODE ProtocolHandler::sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const ProtocolPacketHeader &header, const UInt8 *data)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    
    if (NsAppLink::NsTransportManager::InvalidConnectionHandle == mConnectionHandle)
    {
        LOG4CPLUS_ERROR(mLogger, "sendFrame() - InvalidConnectionHandle - unable to send data");
        return ERR_FAIL;
    }
    
    UInt8 offset = 0;
    UInt8 compress = 0x0;
    UInt8 rawData[MAXIMUM_FRAME_SIZE];
    if (header.compress)
    {
        compress = 0x1;
    }
    UInt8 firstByte = ( (header.version << 4) & 0xF0 )
                      | ( (compress << 3) & 0x08)
                      | (header.frameType & 0x07);
         
    rawData[offset++] = firstByte;
    rawData[offset++] = header.serviceType;    
    rawData[offset++] = header.frameData;    
    rawData[offset++] = header.sessionID;

    rawData[offset++] = header.dataSize >> 24;
    rawData[offset++] = header.dataSize >> 16;
    rawData[offset++] = header.dataSize >> 8;
    rawData[offset++] = header.dataSize;

    if (header.version == PROTOCOL_VERSION_2)
    {
        rawData[offset++] = header.messageID >> 24;
        rawData[offset++] = header.messageID >> 16;
        rawData[offset++] = header.messageID >> 8;
        rawData[offset++] = header.messageID;
    }

    if (data)
    {
        if ( (offset + header.dataSize) <= MAXIMUM_FRAME_SIZE)
            memcpy(rawData + offset, data, header.dataSize);
        else
        {
            LOG4CPLUS_WARN(mLogger, "sendFrame() - buffer is too small for writing");
            return ERR_FAIL;
        }
    }

    if (mTransportManager)
    {
        mTransportManager->sendFrame(ConnectionHandle, rawData, header.dataSize + offset);
    }
    else
    {
        return ERR_FAIL;
    }
    
    return ERR_OK;
}

} //namespace AxisCore

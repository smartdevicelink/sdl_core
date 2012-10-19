#include <memory.h>
#include <iostream>
#include <stdio.h>
#include <time.h>

#include "ProtocolHandler.hpp"
#include "ProtocolPacketHeader.hpp"
#include "IProtocolObserver.hpp"
#include "../../../appMain/CBTAdapter.hpp"
#include "transport/bt/BluetoothAPI.hpp"

namespace AxisCore
{

ProtocolHandler::ProtocolHandler(IProtocolObserver *observer,
                                 Bluetooth::IBluetoothAPI *btAdapter,
                                 const UInt8 protocolVersion) :
                mProtocolObserver(observer),
                mBTAdapter(btAdapter),
                mProtocolVersion(protocolVersion),
                mSessionIdCounter(1)
{    
    printf("%s:%d enter ProtocolHandler::ProtocolHandler()\n", __FILE__, __LINE__);
    srand(time(0) );
    if (btAdapter)
    {
        mBTReader.setBTAdapter(btAdapter);
        mBTWriter.setBTAdapter(btAdapter);
        btAdapter->initBluetooth(this);
    }
}

ProtocolHandler::~ProtocolHandler()
{
    printf("%s:%d enter ProtocolHandler::~ProtocolHandler()\n", __FILE__, __LINE__);

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

    if (mBTWriter.write(header, 0) == ERR_OK)
        printf("%s:%d PH::startSession() return OK\n", __FILE__, __LINE__);
    else
    {
        printf("%s:%d PH::startSession() return FAIL\n", __FILE__, __LINE__);
        retVal = ERR_FAIL;
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::endSession(const UInt8 sessionID, const UInt32 hashCode)
{
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
                printf("%s:%d PH::endSession() incorrect hashCode\n", __FILE__, __LINE__);
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

        if (mBTWriter.write(header, 0) == ERR_OK)
        {
            printf("%s:%d PH::endSession() return OK\n", __FILE__, __LINE__);
            mHashCodes.erase(sessionID);
        }
        else
        {
            printf("%s:%d PH::endSession() return FAIL\n", __FILE__, __LINE__);
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
    ERROR_CODE retVal = ERR_OK;

    ProtocolPacketHeader header(mProtocolVersion,
                                compress,
                                FRAME_TYPE_SINGLE,
                                servType,
                                0,
                                sessionID,
                                dataSize,
                                mMessageCounters[sessionID]++);

    if (mBTWriter.write(header, data) != ERR_OK)
    {
        printf("%s:%d PH::sendSingleFrame() write single frame FAIL\n", __FILE__, __LINE__);
        retVal = ERR_FAIL;
    }
    else
        printf("%s:%d PH::sendSingleFrame() write single frame OK\n", __FILE__, __LINE__);

    return retVal;
}

ERROR_CODE ProtocolHandler::sendMultiFrameMessage(const UInt8 sessionID,
                                                  const UInt8 servType,
                                                  const UInt32 dataSize,
                                                  const UInt8 *data,
                                                  const bool compress,
                                                  const UInt32 maxDataSize)
{
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

    if (mBTWriter.write(firstHeader, outDataFirstFrame) != ERR_OK)
    {
        printf("%s:%d PH::sendData() write first frame FAIL\n", __FILE__, __LINE__);
        delete [] outDataFirstFrame;
        return ERR_FAIL;
    }
    else
        printf("%s:%d PH::sendData() write first frame OK\n", __FILE__, __LINE__);

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

            if (mBTWriter.write(header, outDataFrame) != ERR_OK)
            {
                printf("%s:%d PH::sendData() write consecutive frame FAIL\n", __FILE__, __LINE__);
                retVal = ERR_FAIL;
                break;
            }
            else
                printf("%s:%d PH::sendData() write consecutive frame OK\n", __FILE__, __LINE__);
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

            if (mBTWriter.write(header, outDataFrame) != ERR_OK)
            {
                printf("%s:%d PH::sendData() write last frame FAIL\n", __FILE__, __LINE__);
                retVal = ERR_FAIL;
                break;
            }
            else
                printf("%s:%d PH::sendData() write last frame OK\n", __FILE__, __LINE__);
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
    printf("%s:%d enter PH::sendData()\n", __FILE__, __LINE__);
    ERROR_CODE retVal = ERR_OK;

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

    return retVal;
}

ERROR_CODE ProtocolHandler::receiveData(const UInt8 sessionID,
                                        const UInt32 messageID,
                                        const UInt8 servType,
                                        const UInt32 receivedDataSize,
                                        UInt8 *data)
{
    printf("%s:%d enter PH::receiveData()\n", __FILE__, __LINE__);
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
                    printf("%s:%d PH::receiveData() requested msg size != real Msg size\n"
                           , __FILE__, __LINE__);
                    retVal = ERR_FAIL;
                }
            }
            else
            {
                printf("%s:%d PH::receiveData() !currentMessage ptr\n", __FILE__, __LINE__);
                retVal = ERR_FAIL;
            }
        }
    }

    if (retVal)
        printf("%s:%d PH::receiveData() returns OK\n", __FILE__, __LINE__);
    else
        printf("%s:%d PH::receiveData() returns FAIL\n", __FILE__, __LINE__);

    return retVal;
}

ERROR_CODE ProtocolHandler::sendStartSessionAck(const UInt8 sessionID)
{
    printf("%s:%d enter PH::sendStartSessionAck()\n", __FILE__, __LINE__);
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

    if (mBTWriter.write(header, 0) == ERR_OK)
        printf("%s:%d PH::sendStartSessionAck() BT write OK\n", __FILE__, __LINE__);
    else
    {
        printf("%s:%d PH::sendStartSessionAck() BT write FAIL\n", __FILE__, __LINE__);
        retVal = ERR_FAIL;
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::sendEndSessionNAck(const UInt8 sessionID)
{
    printf("%s:%d enter PH::sendEndSessionNAck()\n", __FILE__, __LINE__);
    ERROR_CODE retVal = ERR_OK;

    ProtocolPacketHeader header(PROTOCOL_VERSION_2,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                SERVICE_TYPE_RPC,
                                FRAME_DATA_END_SESSION_NACK,
                                sessionID,
                                0,
                                0);

    if (mBTWriter.write(header, 0) == ERR_OK)
        printf("%s:%d PH::sendEndSessionNAck() BT write OK\n", __FILE__, __LINE__);
    else
    {
        printf("%s:%d PH::sendEndSessionNAck() BT write FAIL\n", __FILE__, __LINE__);
        retVal = ERR_FAIL;
    }

    return retVal;
}

ERROR_CODE ProtocolHandler::handleMessage(const ProtocolPacketHeader &header, UInt8 *data)
{
    printf("%s:%d enter PH::handleMessage()\n", __FILE__, __LINE__);
    ERROR_CODE retVal = ERR_OK;

    switch (header.frameType)
    {
    case FRAME_TYPE_CONTROL:
    {
        printf("%s:%d PH::handleMessage() case FRAME_TYPE_CONTROL\n"
               , __FILE__, __LINE__);
        if (handleControlMessage(header, data) != ERR_OK)
        {
            printf("%s:%d PH::handleMessage() handleControlMessage FAIL\n", __FILE__, __LINE__);
            retVal = ERR_FAIL;
        }
        break;
    }
    case FRAME_TYPE_SINGLE:
    {
        printf("%s:%d PH::handleMessage() case FRAME_TYPE_SINGLE\n", __FILE__, __LINE__);
        if (mSessionStates.count(header.sessionID) )
        {
            if (mSessionStates[header.sessionID] == HANDSHAKE_DONE)
            {
                Message *message = new Message(header, data, false);
                mToUpperLevelMessagesQueues[header.sessionID].push(message);

                if (mProtocolObserver)
                    mProtocolObserver->dataReceivedCallback(header.sessionID, 0, header.dataSize);
                else
                    retVal = ERR_FAIL;
            }
            else
            {
                printf("%s:%d PH::handleMessage() case FRAME_TYPE_SINGLE but HANDSHAKE_NOT_DONE\n"
                       , __FILE__, __LINE__);
                retVal = ERR_FAIL;
            }
        }
        else
        {
            printf("%s:%d PH::handleMessage() case FRAME_TYPE_SINGLE. unknown sessionID\n"
                   , __FILE__, __LINE__);
            retVal = ERR_FAIL;
        }

        break;
    }
    case FRAME_TYPE_FIRST:
    {
        printf("%s:%d PH::handleMessage() case FRAME_TYPE_FIRST\n"
               , __FILE__, __LINE__);

        if (mSessionStates.count(header.sessionID) )
        {
            if (mSessionStates[header.sessionID] == HANDSHAKE_DONE)
                handleMultiFrameMessage(header, data);
            else
            {
                printf("%s:%d PH::handleMessage() case FRAME_TYPE_FIRST but HANDSHAKE_NOT_DONE\n"
                       , __FILE__, __LINE__);
                retVal = ERR_FAIL;
            }
        }
        else
        {
            printf("%s:%d PH::handleMessage() case FRAME_TYPE_FIRST. unknown sessionID\n"
                   , __FILE__, __LINE__);
            retVal = ERR_FAIL;
        }

        break;
    }
    case FRAME_TYPE_CONSECUTIVE:
    {
        printf("%s:%d PH::handleMessage() case FRAME_TYPE_CONSECUTIVE\n", __FILE__, __LINE__);

        if (mSessionStates.count(header.sessionID) )
        {
            if (mSessionStates[header.sessionID] == HANDSHAKE_DONE)
                handleMultiFrameMessage(header, data);
            else
            {
                printf("%s:%d PH::handleMessage() case FRAME_TYPE_CONSECUTIVE but HANDSHAKE_NOT_DONE\n"
                       , __FILE__, __LINE__);
                retVal = ERR_FAIL;
            }
        }
        else
        {
            printf("%s:%d PH::handleMessage() case FRAME_TYPE_CONSECUTIVE. unknown sessionID\n"
                   , __FILE__, __LINE__);
            retVal = ERR_FAIL;
        }

        break;
    }
    default:
    {
        printf("%s:%d PH::handleMessage() case default!!!\n", __FILE__, __LINE__);
    }
    }

    delete [] data;
    data = NULL;

    return retVal;
}

ERROR_CODE ProtocolHandler::handleControlMessage(const ProtocolPacketHeader &header, UInt8 *data)
{
    ERROR_CODE retVal = ERR_OK;

    UInt8 currentSessionID = header.sessionID;
    if (mSessionStates.count(currentSessionID) && (header.frameData != FRAME_DATA_START_SESSION) )
    {
        UInt8 currentState = mSessionStates[currentSessionID];
        switch (currentState)
        {
        case HANDSHAKE_NOT_DONE:
        {
            printf("%s:%d PH::handleControlMessage() case HANDSHAKE_NOT_DONE\n", __FILE__, __LINE__);
            if (header.frameData == FRAME_DATA_START_SESSION_ACK)
            {
                printf("%s:%d PH::handleControlMessage() frameData == FRAME_DATA_START_SESSION_ACK\n"
                       , __FILE__, __LINE__);
                mSessionStates[currentSessionID] = HANDSHAKE_DONE;
                if (header.version == PROTOCOL_VERSION_2)
                    mHashCodes[currentSessionID] = header.messageID;
                if (mProtocolObserver)
                    mProtocolObserver->sessionStartedCallback(currentSessionID, header.messageID);
                else
                    retVal = ERR_FAIL;
            }
            else if (header.frameData == FRAME_DATA_START_SESSION_NACK)
            {
                printf("%s:%d PH::handleControlMessage() session rejected\n", __FILE__, __LINE__);
            }
            else
            {
                printf("%s:%d PH::handleControlMessage() case HANDSHAKE_NOT_DONE invalid frameData\n"
                       , __FILE__, __LINE__);
            }

            break;
        }
        case HANDSHAKE_DONE:
        {
            printf("%s:%d PH::handleControlMessage() case HANDSHAKE_DONE\n", __FILE__, __LINE__);
            if (header.frameData == FRAME_DATA_END_SESSION)
            {
                printf("%s:%d PH::handleControlMessage() end session message\n"
                       , __FILE__, __LINE__);
                bool correctEndSession = false;
                if (header.version == PROTOCOL_VERSION_2)
                {
                    if (header.messageID == mHashCodes[header.sessionID])
                        correctEndSession = true;
                    else
                    {
                        printf("%s:%d PH::handleControlMessage() end session: incorrect hashCode\n"
                               , __FILE__, __LINE__);
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
                printf("%s:%d PH::handleControlMessage() case HANDSHAKE_DONE invalid frameData\n"
                       , __FILE__, __LINE__);
            }

            break;
        }
        default:
        { }
        }
    }
    else
    {
        printf("%s:%d PH::handleControlMessage() new sessionID\n", __FILE__, __LINE__);
        if (header.frameData == FRAME_DATA_START_SESSION)
        {
            printf("%s:%d PH::handleControlMessage() frameData == FRAME_DATA_START_SESSION\n"
                   , __FILE__, __LINE__);

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
    ERROR_CODE retVal = ERR_OK;
    printf("%s:%d enter PH::handleMultiFrameMessage()\n", __FILE__, __LINE__);

    if (header.frameType == FRAME_TYPE_FIRST)
    {
        printf("%s:%d PH::handleMultiFrameMessage() : FRAME_TYPE_FIRST\n", __FILE__, __LINE__);
        Message *multiFrameMessage = new Message(header, data, true);
        mIncompleteMultiFrameMessages[header.sessionID] = multiFrameMessage;
    }
    else
    {
        printf("%s:%d PH::handleMultiFrameMessage() : Consecutive frame\n"
               , __FILE__, __LINE__);

        if (mIncompleteMultiFrameMessages.count(header.sessionID) )
        {
            if (mIncompleteMultiFrameMessages[header.sessionID]->addConsecutiveMessage(header, data)
                    == ERR_OK)
            {
                printf("%s:%d PH::handleMultiFrameMessage() addConsecutiveMessage OK\n"
                       , __FILE__, __LINE__);
                if (header.frameData == FRAME_DATA_LAST_FRAME)
                {
                    mToUpperLevelMessagesQueues[header.sessionID]
                            .push(mIncompleteMultiFrameMessages[header.sessionID] );
                    mIncompleteMultiFrameMessages.erase(header.sessionID);

                    if (mProtocolObserver)
                        mProtocolObserver->dataReceivedCallback(header.sessionID
                                  , 0
                                  , mToUpperLevelMessagesQueues[header.sessionID].front()
                                                                ->getTotalDataBytes() );
                    else
                        retVal = ERR_FAIL;
                }
            }
            else
            {
                printf("%s:%d PH::handleMultiFrameMessage() addConsecutiveMessage FAIL\n"
                       , __FILE__, __LINE__);
                retVal = ERR_FAIL;
            }
        }
        else
        {
            printf("%s:%d PH::handleMultiFrameMessage() no sessionID in mIncompleteMultiFrameMessages\n"
                   , __FILE__, __LINE__);
            retVal = ERR_FAIL;
        }
    }

    return retVal;
}

/**
  * Bluetooth Callbacks
  */

void ProtocolHandler::onError(BLUETOOTH_ERROR errCode)
{
    printf("%s:%d PH::onError() bluetooth error %d \n", __FILE__, __LINE__, errCode);
}

void ProtocolHandler::dataReceived()
{
    printf("%s:%d enter PH::dataReceived()\n", __FILE__, __LINE__);
    UInt32 dataSize = 0;

    if (mBTAdapter)
        dataSize = mBTAdapter->getBuffer().size();
    else
        return;

    UInt8 *data = new UInt8[dataSize];
    ProtocolPacketHeader header;

    if (mBTReader.read(header, data, dataSize) == ERR_OK)
        handleMessage(header, data);
    else
        printf("%s:%d PH::dataReceived() error reading\n", __FILE__, __LINE__);
}

} //namespace AxisCore

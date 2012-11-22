#include "ProtocolHandler/ProtocolHandler.h"

log4cplus::Logger ProtocolHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ProtocolHandler"));

ProtocolHandler::ProtocolHandler(IProtocolObserver *observer,
                    NsAppLink::NsTransportManager::ITransportManager * transportManager) :
 mProtocolObserver( observer )
,mTransportManager( transportManager )
{
    pthread_create( &mHandleMessagesFromMobileApp, NULL, &ProtocolHandler::handleMessagesFromMobileApp, (void *)this );
    pthread_create( &mHandleMessagesToMobileApp, NULL, &ProtocolHandler::handleMessagesToMobileApp, (void *)this );
}


ProtocolHandler::~ProtocolHandler()
{
    pthread_kill( mHandleMessagesFromMobileApp, 1 );
    pthread_kill( mHandleMessagesToMobileApp, 1 );
    mProtocolObserver = 0;
    mTransportManager = 0;
}

RESULT_CODE ProtocolHandler::endSession(unsigned int sessionID, unsigned int hashCode)
{

}

void ProtocolHandler::sendData(const AppLinkRawMessage * message)
{
    if ( !message )
    {
        LOG4CPLUS_ERROR(mLogger, "Invalid message for sending to mobile app is received.");
        return;
    }
    mMessagesToMobileApp.push(message);    
}


virtual void ProtocolHandler::onFrameReceived(NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
    const uint8_t * data, size_t dataSize)
{
    if (connectionHandle && dataSize > 0 && data )
    {
        IncomingMessage message;
        message.data = data;
        message.dataSize = dataSize;
        message.connectionHandle = connectionHandle;
        mIncomingMessages.push( message );
    }
    else 
    {
        LOG4CPLUS_ERROR( mLogger, "Invalid incoming message received in ProtocolHandler." );
    }
}

void ProtocolHandler::sendStartSessionAck( NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle,
              unsigned int protocolVersion,
              unsigned char sessionID )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    unsigned char versionF = PROTOCOL_VERSION_1;
    if (2 == protocolVersion)
    {
        versionF = PROTOCOL_VERSION_2;
    }

    UInt32 hashCode = 0;
    if (mProtocolVersion == PROTOCOL_VERSION_2)
        hashCode = rand() % 0xFFFFFFFF;

    mHashCodes[sessionID] = hashCode;

    ProtocolPacket packet(versionF,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                SERVICE_TYPE_RPC,
                                FRAME_DATA_START_SESSION_ACK,
                                sessionID,
                                0,
                                hashCode);

    if (RESULT_OK == sendFrame(mConnectionHandle, packet))
    {
        LOG4CPLUS_INFO(mLogger, "sendStartSessionAck() - BT write OK");
    }
    else
    {
        LOG4CPLUS_ERROR(mLogger, "sendStartSessionAck() - BT write FAIL");
    }
}

RESULT_CODE ProtocolHandler::sendFrame( NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle,
        const ProtocolPacket & packet )
{
    if ( !packet.getPacket() )
    {
        LOG4CPLUS_ERROR(mLogger, "Failed to create packet.");
        return RESULT_FAIL;
    }

    if (mTransportManager)
    {
        mTransportManager->sendFrame(connectionHandle, packet.getPacket(), packet.getPacketSize() );
    }
    else
    {
        LOG4CPLUS_WARNING(mLogger, "No Transport Manager found.");
        return RESULT_FAIL;
    }

    return RESULT_OK;
}

RESULT_CODE ProtocolHandler::sendSingleFrameMessage(unsigned int connectionHandle,
                                      unsigned char sessionID,
                                      unsigned int protocolVersion,
                                      unsigned char servType,
                                      unsigned int dataSize,
                                      const unsigned char *data,
                                      bool compress)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    unsigned char versionF = PROTOCOL_VERSION_1;
    if (2 == protocolVersion)
    {
        versionF = PROTOCOL_VERSION_2;
    }

    ProtocolPacket packet(versionF,
                        compress,
                        FRAME_TYPE_SINGLE,
                        servType,
                        0,
                        sessionID,
                        dataSize,
                        mMessageCounters[sessionID]++,
                        data);

    return sendFrame( connectionHandle, packet );   
}
        
RESULT_CODE ProtocolHandler::sendMultiFrameMessage(unsigned int connectionHandle,
                                         const unsigned char sessionID,
                                         unsigned int protocolVersion,
                                         const unsigned char servType,
                                         const unsigned int dataSize,
                                         const unsigned char *data,
                                         const bool compress,
                                         const unsigned int maxDataSize)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    RESULT_CODE retVal = RESULT_OK;

    unsigned char versionF = PROTOCOL_VERSION_1;
    if (2 == protocolVersion)
    {
        versionF = PROTOCOL_VERSION_2;
    }

    int numOfFrames = 0;
    int lastDataSize = 0;

    if (dataSize % maxDataSize)
    {
        numOfFrames = (dataSize / maxDataSize) + 1;
        lastDataSize = dataSize % maxDataSize;
    }
    else
        numOfFrames = dataSize / maxDataSize;

    unsigned char *outDataFirstFrame = new unsigned char[FIRST_FRAME_DATA_SIZE];
    outDataFirstFrame[0] = dataSize >> 24;
    outDataFirstFrame[1] = dataSize >> 16;
    outDataFirstFrame[2] = dataSize >> 8;
    outDataFirstFrame[3] = dataSize;

    outDataFirstFrame[4] = numOfFrames >> 24;
    outDataFirstFrame[5] = numOfFrames >> 16;
    outDataFirstFrame[6] = numOfFrames >> 8;
    outDataFirstFrame[7] = numOfFrames;

    ProtocolPacket firstPacket(versionF,
                                     compress,
                                     FRAME_TYPE_FIRST,
                                     servType,
                                     0,
                                     sessionID,
                                     FIRST_FRAME_DATA_SIZE,
                                     mMessageCounters[sessionID],
                                     outDataFirstFrame);  

    retVal = sendFrame( connectionHandle, firstPacket );

    delete [] outDataFirstFrame;

    unsigned char *outDataFrame = new unsigned char[maxDataSize];

    for (unsigned int i = 0 ; i < numOfFrames ; i++)
    {
        if (i != (numOfFrames - 1) )
        {
            memcpy(outDataFrame, data + (maxDataSize * i), maxDataSize);

            ProtocolPacket packet(versionF,
                                        compress,
                                        FRAME_TYPE_CONSECUTIVE,
                                        servType,
                                        ( (i % FRAME_DATA_MAX_VALUE) + 1),
                                        sessionID,
                                        maxDataSize,
                                        mMessageCounters[sessionID],
                                        outDataFrame);            

            retVal = sendFrame( connectionHandle, packet );
            if ( RESULT_FAIL == retVal)
            {
                break;
            }
        }
        else
        {
            memcpy(outDataFrame, data + (maxDataSize * i), lastDataSize);

            ProtocolPacket packet(versionF,
                                        compress,
                                        FRAME_TYPE_CONSECUTIVE,
                                        servType,
                                        0x0,
                                        sessionID,
                                        lastDataSize,
                                        mMessageCounters[sessionID]++,
                                        outDataFrame);            

            retVal = sendFrame( connectionHandle, packet );
        }
    }

    delete [] outDataFrame;

    return retVal;
}

void ProtocolHandler::handleMessage( const ProtocolPacket& packet )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    switch (header.frameType)
    {
    case FRAME_TYPE_CONTROL:
    {
        LOG4CPLUS_INFO(mLogger, "handleMessage() - case FRAME_TYPE_CONTROL");

        handleControlMessage( packet );
        break;
    }
    case FRAME_TYPE_SINGLE:
    {
        LOG4CPLUS_INFO(mLogger, "handleMessage() - case FRAME_TYPE_SINGLE");
        
        AppLinkRawMessage * rawMessage = new AppLinkRawMessage( connectionHandle,
                                    packet.sessionID,
                                    packet.version,
                                    packet.data,
                                    packet.dataSize);

        if (mProtocolObserver)
            mProtocolObserver->dataReceivedCallback(rawMessage);

        break;
    }
    case FRAME_TYPE_FIRST:
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

RESULT_CODE ProtocolHandler::handleMultiFrameMessage( NsAppLink::NsTransportManager::tConnectionHandle connectionHandle,
              ProtocolPacket * packet )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    if (header.frameType == FRAME_TYPE_FIRST)
    {
        LOG4CPLUS_INFO(mLogger, "handleMultiFrameMessage() - FRAME_TYPE_FIRST");
        
        packet -> totalDataBytes = packet -> data[0] << 24;
        packet -> totalDataBytes |= packet -> data[1] << 16;
        packet -> totalDataBytes |= packet -> data[2] << 8;
        packet -> totalDataBytes |= packet -> data[3];

        mIncompleteMultiFrameMessages[connectionHandle] = packet;
    }
    else
    {
        LOG4CPLUS_INFO(mLogger, "handleMultiFrameMessage() - Consecutive frame");

        pair<PacketsMultimap::iterator,PacketsMultimap::iterator> messagesFromConnection;

        messagesFromConnection = mIncompleteMultiFrameMessages.equal_range( connectionHandle );

        for ( PacketsMultimap::iterator it = messagesFromConnection.first;
                it != messagesFromConnection.second;
                ++it )
        {
            if ( it->second->sessionID == packet->sessionID )
            {
                it->second->appendData( packet -> data, packet -> dataSize );
                
                if ( packet -> frameData == FRAME_DATA_LAST_FRAME )
                {
                    if ( mProtocolObserver )
                    {
                        mProtocolObserver -> dataReceivedCallback(
                            it->second->sessionID,
                            it->second->messageID,
                            packet->data,
                            packet->totalDataBytes);
                    }
                    mIncompleteMultiFrameMessages.erase( it );
                    break;
                }
            }
        }

    }
}

void ProtocolHandler::handleControlMessage( NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle,
              const ProtocolPacket & packet )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    unsigned char currentSessionID = packet.sessionID;
    
    if (header.frameData == FRAME_DATA_END_SESSION)
    {
        LOG4CPLUS_INFO(mLogger, "handleControlMessage() - FRAME_DATA_END_SESSION");
        bool correctEndSession = false;
        if (header.version == PROTOCOL_VERSION_2)
        {
            /*if (header.messageID == mHashCodes[header.sessionID])
                correctEndSession = true;
            else
            {
                LOG4CPLUS_WARN(mLogger, "handleControlMessage() - incorrect hashCode");
                if (sendEndSessionNAck(header.sessionID) != ERR_OK)
                    retVal = ERR_FAIL;
            }*/
        }
        else if ( (header.version == PROTOCOL_VERSION_1) )
        {
            correctEndSession = true;
        }

        if (correctEndSession)
        {
            mHashCodes.erase(currentSessionID);
            if (mProtocolObserver)
                mProtocolObserver->sessionEndedCallback(currentSessionID);
        }
    }
    if (header.frameData == FRAME_DATA_START_SESSION)
    {
        LOG4CPLUS_INFO(mLogger, "handleControlMessage() - FRAME_DATA_START_SESSION");
        if (sendStartSessionAck(connectionHandle, 1, mSessionIdCounter) == RESULT_OK)
        {
            if (mProtocolObserver)
            {
                mProtocolObserver->sessionStartedCallback(mSessionIdCounter
                                                          , mHashCodes[mSessionIdCounter]);
            }

            mSessionIdCounter++;
        }
    }
}

void * ProtocolHandler::handleMessagesFromMobileApp( void * params )
{
    ProtocolHandler * handler = static_cast<ProtocolHandler*> (params);
    if ( !handler )
    {
        pthread_exit( 0 );
    }

    while( 1 )
    {
        while( ! handler -> mMessagesFromMobileApp.empty() )
        {
            IncomingMessage message = handler -> mMessagesFromMobileApp.pop();
            LOG4CPLUS_INFO_EXT(mLogger, "Message from mobile app received of size " << message.mDataSize );

            //@TODO check for ConnectionHandle.
            //@TODO check for data size - crash is possible.
            if ((0 != message.mData) && (0 != message.mDataSize) && (MAXIMUM_FRAME_SIZE >= message.mDataSize))
            {        
                ProtocolPacket packet;
                
                UInt8 offset = 0;
                UInt8 firstByte = message.mData[offset];
                offset++;
                
                packet.version = firstByte >> 4u;
                if (firstByte & 0x08u)
                {
                    packet.compress = true;
                }
                else
                {
                    packet.compress = false;
                }
                
                packet.frameType = firstByte & 0x07u;

                packet.serviceType = message.mData[offset++];        
                packet.frameData = message.mData[offset++];        
                packet.sessionID = message.mData[offset++];
                
                packet.dataSize  = message.mData[offset++] << 24u;
                packet.dataSize |= message.mData[offset++] << 16u;
                packet.dataSize |= message.mData[offset++] << 8u;
                packet.dataSize |= message.mData[offset++];
                
                if (packet.version == PROTOCOL_VERSION_2)
                {
                    packet.messageID  = message.mData[offset++] << 24u;
                    packet.messageID |= message.mData[offset++] << 16u;
                    packet.messageID |= message.mData[offset++] << 8u;
                    packet.messageID |= message.mData[offset++];
                }
                else
                {
                    packet.messageID = 0u;
                }
                
                const UInt32 dataPayloadSize = message.mDataSize - offset;
                
                if (dataPayloadSize != packet.dataSize)
                {
                    LOG4CPLUS_ERROR(mLogger, "handleMessagesFromMobileApp() - EPIC FAIL: dataPayloadSize != packet.dataSize");
                    return;
                }
                
                UInt8 * data = 0;
                if (dataPayloadSize != 0u)
                {
                    data = new UInt8[message.mDataSize - offset];
                    memcpy(data, message.mData + offset, dataPayloadSize);
                }
                packet.data = data;
                        
                handleMessage(packet);
            }
            else
            {
                LOG4CPLUS_WARN(mLogger, "handleMessagesFromMobileApp() - incorrect or NULL data");
            }
        }
        handler -> mMessagesFromMobileApp.wait();
    }

    pthread_exit( 0 );
}

void * ProtocolHandler::handleMessagesToMobileApp( void * params )
{
    ProtocolHandler * handler = static_cast<ProtocolHandler*> (params);
    if ( !handler )
    {
        pthread_exit( 0 );
    }

    //TODO: check if continue running condition.
    while( 1 )
    {
        while ( ! handler -> mMessagesToMobileApp.empty() )
        {
            const AppLinkRawMessage * message = handler -> mMessagesToMobileApp.pop();
            LOG4CPLUS_INFO_EXT(mLogger, "Message to mobile app: connection " << message->getConnectionID()
                    << "; sessionID: " << message->getSessionID() 
                    << "; dataSize: " << message->getDataSize() );

            unsigned int maxDataSize = 0;
            if ( PROTOCOL_VERSION_1 == message -> getProtocolVersion() )
                maxDataSize = MAXIMUM_FRAME_SIZE - PROTOCOL_HEADER_V1_SIZE;
            else if ( PROTOCOL_VERSION_2 == message -> getProtocolVersion() )
                maxDataSize = MAXIMUM_FRAME_SIZE - PROTOCOL_HEADER_V2_SIZE;

            if ( message -> getDataSize() <= maxDataSize )
            {
                if (sendSingleFrameMessage(message -> getConnectionID(),
                                            message -> getSessionID(), 
                                            SERVICE_TYPE_RPC, 
                                            message -> getDataSize(), 
                                            message -> getData(), 
                                            false) != RESULT_OK)
                {
                    LOG4CPLUS_ERROR(mLogger, "ProtocolHandler failed to send single frame message.");
                }                    
            }
            else
            {
                if (sendMultiFrameMessage(message -> getConnectionID(),
                                            message -> getSessionID(), 
                                            SERVICE_TYPE_RPC, 
                                            message -> getDataSize(), 
                                            message -> getData(), 
                                            false,
                                            maxDataSize) != RESULT_OK)
                {
                    LOG4CPLUS_ERROR(mLogger, "ProtocolHandler failed to send multi frame messages.");
                }
            }
        }
        handler -> mMessagesToMobileApp.wait();
    }

    pthread_exit( 0 );
}
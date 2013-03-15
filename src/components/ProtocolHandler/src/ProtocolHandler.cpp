/**
* \file ProtocolHandler.cpp
* \brief ProtocolHandler class source file.
* 
* Copyright (c) 2013 Ford Motor Company
*/

#include <pthread.h>
#include <signal.h>
#include <memory.h>
#include "TransportManager/ITransportManager.hpp"
#include "ProtocolHandler/ISessionObserver.h"
#include "ProtocolHandler/IProtocolObserver.h"
#include "ProtocolHandler/ProtocolHandler.h"

using namespace NsProtocolHandler;

log4cplus::Logger ProtocolHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("ProtocolHandler"));

ProtocolHandler::ProtocolHandler( NsSmartDeviceLink::NsTransportManager::ITransportManager * transportManager ) :
 mProtocolObserver( 0 )
,mSessionObserver( 0 )
,mTransportManager( transportManager )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    pthread_create( &mHandleMessagesFromMobileApp, NULL, &ProtocolHandler::handleMessagesFromMobileApp, (void *)this );
    pthread_create( &mHandleMessagesToMobileApp, NULL, &ProtocolHandler::handleMessagesToMobileApp, (void *)this );
}

ProtocolHandler::~ProtocolHandler()
{
    pthread_kill( mHandleMessagesFromMobileApp, 1 );
    pthread_kill( mHandleMessagesToMobileApp, 1 );
    mProtocolObserver = 0;
    mSessionObserver = 0;
    mTransportManager = 0;
}

void ProtocolHandler::setProtocolObserver( IProtocolObserver * observer )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    if ( !observer )
    {
        LOG4CPLUS_ERROR(mLogger, "Invalid (NULL) pointer to IProtocolObserver.");
        return;
    }

    mProtocolObserver = observer;
}

void ProtocolHandler::setSessionObserver( ISessionObserver * observer )
{
    if ( !observer )
    {
        LOG4CPLUS_ERROR(mLogger, "Invalid (NULL) pointer to ISessionObserver.");
        return;
    }

    mSessionObserver = observer;
}

void ProtocolHandler::sendEndSessionNAck( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned int sessionID,
              unsigned char serviceType )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    ProtocolPacket packet(PROTOCOL_VERSION_2,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                0x0,
                                FRAME_DATA_END_SESSION_NACK,
                                sessionID,
                                0,
                                0);

    if (RESULT_OK == sendFrame(connectionHandle, packet))
    {
        LOG4CPLUS_INFO(mLogger, "sendStartSessionAck() - BT write OK");
    }
    else
    {
        LOG4CPLUS_ERROR(mLogger, "sendStartSessionAck() - BT write FAIL");
    }
}

void ProtocolHandler::sendStartSessionAck( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned char sessionID,
              unsigned char protocolVersion,
              unsigned int hashCode,
              unsigned char serviceType )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    ProtocolPacket packet(protocolVersion,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                serviceType,
                                FRAME_DATA_START_SESSION_ACK,
                                sessionID,
                                0,
                                hashCode);

    if (RESULT_OK == sendFrame(connectionHandle, packet))
    {
        LOG4CPLUS_INFO(mLogger, "sendStartSessionAck() for connection " << connectionHandle
                            << " for serviceType " << serviceType << " sessionID " << sessionID);
    }
    else
    {
        LOG4CPLUS_ERROR(mLogger, "sendStartSessionAck() - BT write FAIL");
    }
}

void ProtocolHandler::sendStartSessionNAck( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              unsigned char serviceType )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    unsigned char versionFlag = PROTOCOL_VERSION_1;

    ProtocolPacket packet(versionFlag,
                                COMPRESS_OFF,
                                FRAME_TYPE_CONTROL,
                                serviceType,
                                FRAME_DATA_START_SESSION_NACK,
                                0x0,
                                0,
                                0);

    if (RESULT_OK == sendFrame(connectionHandle, packet))
    {
        LOG4CPLUS_INFO(mLogger, "sendStartSessionAck() - BT write OK");
    }
    else
    {
        LOG4CPLUS_ERROR(mLogger, "sendStartSessionAck() - BT write FAIL");
    }

}

void ProtocolHandler::sendData(const SmartDeviceLinkRawMessage * message)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    if ( !message )
    {
        LOG4CPLUS_ERROR(mLogger, "Invalid message for sending to mobile app is received.");
        return;
    }
    mMessagesToMobileApp.push(message);
}

void ProtocolHandler::onFrameReceived(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
    const uint8_t * data, size_t dataSize)
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    if (connectionHandle && dataSize > 0 && data )
    {
        LOG4CPLUS_INFO_EXT(mLogger, "Received from TM " << data << "; size " << dataSize);
        IncomingMessage  * message = new IncomingMessage;
        message -> mData = new unsigned char[dataSize];
        memcpy(message -> mData, data, dataSize);
        message -> mDataSize = dataSize;
        message -> mConnectionHandle = connectionHandle;
        mMessagesFromMobileApp.push( message );
    }
    else
    {
        LOG4CPLUS_ERROR( mLogger, "Invalid incoming message received in ProtocolHandler from Transport Manager." );
    }
}

void ProtocolHandler::onFrameSendCompleted(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
            int userData, NsSmartDeviceLink::NsTransportManager::ESendStatus sendStatus)
{
    if ( NsSmartDeviceLink::NsTransportManager::SendStatusOK != sendStatus )
    {
        LOG4CPLUS_ERROR(mLogger, "Failed to send frame with number " << userData);
    }
}

RESULT_CODE ProtocolHandler::sendFrame( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
        const ProtocolPacket & packet )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);
    if ( !packet.getPacket() )
    {
        LOG4CPLUS_ERROR(mLogger, "Failed to create packet.");
        return RESULT_FAIL;
    }

    LOG4CPLUS_INFO_EXT( mLogger, "Packet to be sent: " << packet.getPacket() << " of size: " << packet.getPacketSize());

    if (mTransportManager)
    {
        mTransportManager->sendFrame(connectionHandle, packet.getPacket(), packet.getPacketSize(), static_cast<int>(packet.getFrameData()) );
    }
    else
    {
        LOG4CPLUS_WARN(mLogger, "No Transport Manager found.");
        return RESULT_FAIL;
    }

    return RESULT_OK;
}

RESULT_CODE ProtocolHandler::sendSingleFrameMessage(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
                                      const unsigned char sessionID,
                                      unsigned int protocolVersion,
                                      const unsigned char servType,
                                      const unsigned int dataSize,
                                      const unsigned char *data,
                                      const bool compress)
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

RESULT_CODE ProtocolHandler::sendMultiFrameMessage(NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
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

    LOG4CPLUS_INFO_EXT(mLogger, " data size " << dataSize << " maxDataSize " << maxDataSize);

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
    {
        numOfFrames = dataSize / maxDataSize;
        lastDataSize = maxDataSize;
    }

    LOG4CPLUS_INFO_EXT(mLogger, "Data size " << dataSize << " of " << numOfFrames << " frames with last frame " << lastDataSize);

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
                                     ++mMessageCounters[sessionID],
                                     outDataFirstFrame);

    retVal = sendFrame( connectionHandle, firstPacket );

    LOG4CPLUS_INFO_EXT(mLogger, "First frame is sent.");

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
                                        mMessageCounters[sessionID],
                                        outDataFrame);

            retVal = sendFrame( connectionHandle, packet );
        }
    }

    delete [] outDataFrame;

    return retVal;
}

RESULT_CODE ProtocolHandler::handleMessage( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
                    ProtocolPacket * packet )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    switch (packet -> getFrameType())
    {
        case FRAME_TYPE_CONTROL:
        {
            LOG4CPLUS_INFO(mLogger, "handleMessage() - case FRAME_TYPE_CONTROL");

            return handleControlMessage( connectionHandle, packet );
        }
        case FRAME_TYPE_SINGLE:
        {
            LOG4CPLUS_INFO(mLogger, "FRAME_TYPE_SINGLE: of size " << packet->getDataSize() << ";message " << packet -> getData());

            if ( !mSessionObserver )
            {
                LOG4CPLUS_ERROR(mLogger, "Cannot handle message from Transport Manager: ISessionObserver doesn't exist.");
                return RESULT_FAIL;
            }

            int connectionKey = mSessionObserver -> keyFromPair( connectionHandle,
                                        packet -> getSessionId() );

            SmartDeviceLinkRawMessage * rawMessage = new SmartDeviceLinkRawMessage( connectionKey,
                                        packet -> getVersion(),
                                        packet -> getData(),
                                        packet -> getDataSize() );

            if (mProtocolObserver)
                mProtocolObserver->onDataReceivedCallback(rawMessage);

            break;
        }
        case FRAME_TYPE_FIRST:
        case FRAME_TYPE_CONSECUTIVE:
        {
            LOG4CPLUS_INFO(mLogger, "handleMessage() - case FRAME_TYPE_CONSECUTIVE");

            return handleMultiFrameMessage( connectionHandle, packet );
        }
        default:
        {
            LOG4CPLUS_WARN(mLogger, "handleMessage() - case default!!!");
        }
    }

    return RESULT_OK;
}

RESULT_CODE ProtocolHandler::handleMultiFrameMessage( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              ProtocolPacket * packet )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    if ( !mSessionObserver )
    {
        LOG4CPLUS_ERROR(mLogger, "No ISessionObserver set.");
        return RESULT_FAIL;
    }

    LOG4CPLUS_INFO_EXT(mLogger, "Packet " << packet << "; sessionID " << packet -> getSessionId());

    int key = mSessionObserver->keyFromPair(connectionHandle, packet -> getSessionId());

    if (packet -> getFrameType() == FRAME_TYPE_FIRST)
    {
        LOG4CPLUS_INFO(mLogger, "handleMultiFrameMessage() - FRAME_TYPE_FIRST");

        //const unsigned char * data = packet -> getData();
        unsigned int totalDataBytes = packet -> getData()[0] << 24;
        totalDataBytes |= packet -> getData()[1] << 16;
        totalDataBytes |= packet -> getData()[2] << 8;
        totalDataBytes |= packet -> getData()[3];

        packet -> setTotalDataBytes( totalDataBytes );

        mIncompleteMultiFrameMessages[key] = packet;
    }
    else
    {
        LOG4CPLUS_INFO(mLogger, "handleMultiFrameMessage() - Consecutive frame");

        std::map<int, ProtocolPacket*>::iterator it = mIncompleteMultiFrameMessages.find(key);

        if ( it == mIncompleteMultiFrameMessages.end() )
        {
            LOG4CPLUS_ERROR(mLogger, "Frame of multiframe message for non-existing session id");
            return RESULT_FAIL;
        }

        //LOG4CPLUS_INFO(mLogger, "Found message " << it->second->)

        if ( it->second->appendData( packet -> getData(), packet -> getDataSize() ) != RESULT_OK )
        {
            LOG4CPLUS_ERROR(mLogger, "Failed to append frame for multiframe message.");
            return RESULT_FAIL;
        }

        if ( packet -> getFrameData() == FRAME_DATA_LAST_FRAME )
        {
            if ( !mProtocolObserver )
            {
                LOG4CPLUS_ERROR(mLogger, "Cannot handle multiframe message: no IProtocolObserver is set.");
                return RESULT_FAIL;
            }

            ProtocolPacket * completePacket = it->second;
            SmartDeviceLinkRawMessage * rawMessage = new SmartDeviceLinkRawMessage( key,
                                        completePacket -> getVersion(),
                                        completePacket -> getData(),
                                        completePacket -> getTotalDataBytes() );

            mProtocolObserver -> onDataReceivedCallback( rawMessage );

            mIncompleteMultiFrameMessages.erase( it );
        }

    }

    return RESULT_OK;
}

RESULT_CODE ProtocolHandler::handleControlMessage( NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle,
              const ProtocolPacket * packet )
{
    LOG4CPLUS_TRACE_METHOD(mLogger, __PRETTY_FUNCTION__);

    if ( !mSessionObserver )
    {
        LOG4CPLUS_ERROR(mLogger, "ISessionObserver is not set.");
        return RESULT_FAIL;
    }

    if (packet -> getFrameData() == FRAME_DATA_END_SESSION)
    {
        LOG4CPLUS_INFO(mLogger, "handleControlMessage() - FRAME_DATA_END_SESSION");

        unsigned char currentSessionID = packet -> getSessionId();

        unsigned int hashCode = 0;
        if ( packet -> getVersion() == 2 )
        {
            hashCode = packet -> getMessageId();
        }

        bool success = true;
        int sessionHashCode = mSessionObserver -> onSessionEndedCallback( connectionHandle, currentSessionID, hashCode );

        if ( -1 != sessionHashCode )
        {
            if ( 2 == packet -> getVersion() ) // check hash code only for second version of protocol.
            {
                if ( packet -> getMessageId() != sessionHashCode )
                {
                    success = false;
                }
            }
        }
        else
        {
            success = false;
        }

        if  ( success )
        {
            mMessageCounters.erase( currentSessionID );
        }
        else
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Refused to end session " << packet -> getServiceType() << " type.");
            sendEndSessionNAck(connectionHandle, currentSessionID, packet -> getServiceType());
        }
    }

    if (packet -> getFrameData() == FRAME_DATA_START_SESSION)
    {
        LOG4CPLUS_INFO(mLogger, "handleControlMessage() - FRAME_DATA_START_SESSION");
        LOG4CPLUS_INFO_EXT(mLogger, "Version 2 " << (packet -> getVersion() == PROTOCOL_VERSION_2));

        int sessionId = mSessionObserver -> onSessionStartedCallback( connectionHandle );
        if ( -1 != sessionId )
        {
            sendStartSessionAck(connectionHandle, sessionId,
                    packet -> getVersion(),
                    mSessionObserver -> keyFromPair(connectionHandle, sessionId),
                    packet -> getServiceType());
        }
        else
        {
            LOG4CPLUS_INFO_EXT(mLogger, "Refused to create session " << packet -> getServiceType() << " type.");
            sendStartSessionNAck(connectionHandle, packet -> getServiceType());
        }
    }

    return RESULT_OK;
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
            IncomingMessage * message = handler -> mMessagesFromMobileApp.pop();
            LOG4CPLUS_INFO_EXT(mLogger, "Message " << message -> mData << " from mobile app received of size " << message -> mDataSize );

            //@TODO check for ConnectionHandle.
            //@TODO check for data size - crash is possible.
            if ((0 != message -> mData) && (0 != message -> mDataSize) && (MAXIMUM_FRAME_DATA_SIZE + PROTOCOL_HEADER_V2_SIZE >= message -> mDataSize))
            {
                ProtocolPacket * packet = new ProtocolPacket;
                LOG4CPLUS_INFO_EXT(mLogger ,"Data: " << packet -> getData());
                if ( packet -> deserializePacket( message -> mData, message -> mDataSize ) == RESULT_FAIL )
                {
                    LOG4CPLUS_ERROR(mLogger, "Failed to parse received message.");
                    delete packet;
                }
                else
                {
                    LOG4CPLUS_INFO_EXT(mLogger, "Packet: dataSize " << packet -> getDataSize());
                    handler -> handleMessage( message -> mConnectionHandle, packet );
                }
            }
            else
            {
                LOG4CPLUS_WARN(mLogger, "handleMessagesFromMobileApp() - incorrect or NULL data");
            }

            delete message;
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
            const SmartDeviceLinkRawMessage * message = handler -> mMessagesToMobileApp.pop();
            LOG4CPLUS_INFO_EXT(mLogger, "Message to mobile app: connection " << message->getConnectionKey()
                    << "; dataSize: " << message->getDataSize() << " ; protocolVersion " << message -> getProtocolVersion());

            unsigned int maxDataSize = 0;
            if ( PROTOCOL_VERSION_1 == message -> getProtocolVersion() )
                maxDataSize = MAXIMUM_FRAME_DATA_SIZE - PROTOCOL_HEADER_V1_SIZE;
            else if ( PROTOCOL_VERSION_2 == message -> getProtocolVersion() )
                maxDataSize = MAXIMUM_FRAME_DATA_SIZE - PROTOCOL_HEADER_V2_SIZE;

            NsSmartDeviceLink::NsTransportManager::tConnectionHandle connectionHandle = 0;
            unsigned char sessionID = 0;

            if ( !handler -> mSessionObserver )
            {
                LOG4CPLUS_ERROR(mLogger, "Cannot handle message to mobile app: ISessionObserver doesn't exist.");
                pthread_exit(0);
            }
            handler -> mSessionObserver -> pairFromKey( message->getConnectionKey(), connectionHandle, sessionID );

            if ( message -> getDataSize() <= maxDataSize )
            {
                if (handler -> sendSingleFrameMessage(connectionHandle,
                                            sessionID,
                                            message -> getProtocolVersion(),
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
                LOG4CPLUS_INFO_EXT(mLogger, "Message will be sent in multiple frames; max size is " << maxDataSize);
                if (handler -> sendMultiFrameMessage(connectionHandle,
                                            sessionID,
                                            message -> getProtocolVersion(),
                                            SERVICE_TYPE_RPC, // TODO : check if this is correct assumption; and remove this hot fix because it is not supposed to be so.
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

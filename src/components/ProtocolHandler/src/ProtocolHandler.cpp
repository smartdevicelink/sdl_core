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
        mIncomingMessages.push( message );
    }
    else 
    {
        LOG4CPLUS_ERROR( mLogger, "Invalid incoming message received in ProtocolHandler." );
    }
}

RESULT_CODE ProtocolHandler::sendFrame(NsAppLink::NsTransportManager::tConnectionHandle ConnectionHandle, const ProtocolPacketHeader &header, const UInt8 *data)
{
    
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
                if (sendSingleFrameMessage(sessionID, servType, dataSize, data, compress) != ERR_OK)
                    retVal = RESULT_FAIL;
            }
            else
            {
                if (sendMultiFrameMessage(sessionID, servType, dataSize, data, compress, maxDataSize)
                        != RESULT_OK)
                {
                    retVal = RESULT_FAIL;
                }
            }
        }
        handler -> mMessagesToMobileApp.wait();
    }

    pthread_exit( 0 );
}
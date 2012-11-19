/**
* \file JSONHandler.cpp
* \brief JSONHandler class source file.
* \author PVyshnevska
*/


#include <stdio.h>
#include <algorithm>
#include <string.h>
#include <pthread.h>
#include <signal.h>
#include <json/reader.h>
#include <json/writer.h>
#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/ALRPCObjects/Marshaller.h"


log4cplus::Logger JSONHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("JSONHandler"));

JSONHandler::JSONHandler( AxisCore::ProtocolHandler * protocolHandler ) :
mProtocolHandler( protocolHandler )
{
    pthread_create( &mWaitForIncomingMessagesThread, NULL, &JSONHandler::waitForIncomingMessages, (void *)this );
    pthread_create( &mWaitForOutgoingMessagesThread, NULL, &JSONHandler::waitForOutgoingMessages, (void *)this );
}
    
JSONHandler::~JSONHandler()
{
    pthread_kill( mWaitForIncomingMessagesThread, 1 );
    pthread_kill( mWaitForOutgoingMessagesThread, 1 );
    mProtocolHandler = 0;
    mMessagesObserver = 0;
}

/*Methods for IRPCMessagesObserver*/
void JSONHandler::setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver )
{
    if ( !messagesObserver )
    {
        LOG4CPLUS_ERROR( mLogger, "Invalid (null) pointer to IRPCMessagesObserver." );
    }
    mMessagesObserver = messagesObserver;
}

void JSONHandler::sendRPCMessage( const NsAppLinkRPC::ALRPCMessage * message, unsigned char sessionId )
{
    LOG4CPLUS_INFO(mLogger, "An outgoing message has been received" );
    if ( message )
    {
        mOutgoingMessages.push( message );
    } 
}
/*End of methods for IRPCMessagesObserver*/

/*Methods from IProtocolObserver*/
void JSONHandler::setProtocolHandler( AxisCore::ProtocolHandler * protocolHandler )
{
    mProtocolHandler = protocolHandler;
}

void JSONHandler::sessionStartedCallback(const UInt8 sessionID, const UInt32 hashCode)
{
    mSessionID = sessionID;
}

void JSONHandler::sessionEndedCallback(const UInt8 sessionID)
{}

void JSONHandler::dataReceivedCallback(const UInt8 sessionID, const UInt32 messageID, const UInt32 dataSize)
{
    LOG4CPLUS_INFO( mLogger, "Received message from mobile App." );
    UInt8 *data = new UInt8[dataSize+1];
    memset(data, 0, dataSize+1);

    if ( !mProtocolHandler )
    {
        LOG4CPLUS_ERROR( mLogger, "Cannot receive message: ProtocolHandler doesn't exist." );
        return; 
    }
    mProtocolHandler -> receiveData(sessionID, messageID, AxisCore::SERVICE_TYPE_RPC, dataSize, data);

    LOG4CPLUS_INFO( mLogger, "Mobile message size is " << dataSize << "; sessionID is " << sessionID );

    std::string str = std::string( (const char*)data, dataSize);
    std::string receivedString = clearEmptySpaces( str );

    delete[] data;

    mIncomingMessages.push( receivedString );
}
/*end of methods from IProtocolObserver*/

std::string JSONHandler::clearEmptySpaces( const std::string & input )
{
    LOG4CPLUS_INFO_EXT(mLogger, "Input string: " << input);
    std::string str = input;
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    LOG4CPLUS_INFO_EXT(mLogger, "After clearing new lines: " << str);
    return str;
}

void * JSONHandler::waitForIncomingMessages( void * params )
{
    JSONHandler * handler = static_cast<JSONHandler*>( params );
    if ( !handler )
    {
        pthread_exit( 0 );
    }

    while( 1 )
    {        
        while ( ! handler -> mIncomingMessages.empty() )
        {
            LOG4CPLUS_INFO( mLogger, "Incoming mobile message received." );
            std::string jsonMessage = handler -> mIncomingMessages.pop();

            NsAppLinkRPC::ALRPCMessage * currentMessage = NsAppLinkRPC::Marshaller::fromString( jsonMessage );
            LOG4CPLUS_EINFO mLogger, "Json string: "<< jsonMessage );


            if ( !currentMessage )
            {
                LOG4CPLUS_ERROR( mLogger, "Invalid mobile message received." );
                continue;
            }

            if ( !handler -> mMessagesObserver )
            {
                LOG4CPLUS_ERROR( mLogger, "Cannot handle mobile message: MessageObserver doesn't exist." );
                pthread_exit( 0 );
            }
            handler -> mMessagesObserver -> onMessageReceivedCallback( currentMessage, handler -> mSessionID );
            LOG4CPLUS_INFO( mLogger, "Incoming mobile message handled." );
        }
        handler -> mIncomingMessages.wait();
    }
}

void * JSONHandler::waitForOutgoingMessages( void * params )
{
    JSONHandler * handler = static_cast<JSONHandler*>( params );
    if ( !handler )
    {
        pthread_exit( 0 );
    }

    while( 1 )
    {
        while ( ! handler -> mOutgoingMessages.empty() )
        {
            const NsAppLinkRPC::ALRPCMessage * message = handler -> mOutgoingMessages.pop();
            LOG4CPLUS_INFO( mLogger, "Outgoing mobile message " << message->getMethodId() << " received." );

            std::string messageString = NsAppLinkRPC::Marshaller::toString( message );

            UInt8* pData;
            pData = new UInt8[messageString.length() + 1];
            memcpy (pData, messageString.c_str(), messageString.length() + 1);

            if ( !handler -> mProtocolHandler )
            {
                LOG4CPLUS_ERROR( mLogger, "Cannot handle mobile message: ProtocolHandler doesn't exist." );
                pthread_exit( 0 );
            }
            handler -> mProtocolHandler -> sendData( handler -> mSessionID,  AxisCore::SERVICE_TYPE_RPC, 
                    messageString.size() + 1, pData, false );

            delete message;
            LOG4CPLUS_INFO( mLogger, "Outgoing mobile message handled." );
        }
        handler -> mOutgoingMessages.wait();
    }
}

#include "JSONHandler/JSONHandler.h"
#include "JSONHandler/ALRPCObjects/Marshaller.h"
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <json/reader.h>
#include <json/writer.h>
#include <algorithm>
#include <pthread.h>
#include <signal.h>

log4cplus::Logger JSONHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("JSONRPC2Handler"));

JSONHandler::JSONHandler( AxisCore::ProtocolHandler * protocolHandler )
:mProtocolHandler( protocolHandler )
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
        LOG4CPLUS_ERROR( mLogger, "Invalid incoming message." );
    }
    mMessagesObserver = messagesObserver;
}

void JSONHandler::sendRPCMessage( const ALRPCMessage * message, unsigned char sessionId )
{
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
    UInt8 *data = new UInt8[dataSize+1];
    memset(data, 0, dataSize+1);

    if ( !mProtocolHandler )
    {
        LOG4CPLUS_ERROR( mLogger, "Cannot receive message: ProtocolHandler doesn't exist." );
        return; 
    }
    mProtocolHandler -> receiveData(sessionID, messageID, AxisCore::SERVICE_TYPE_RPC, dataSize, data);

    std::string str = std::string( (const char*)data, dataSize);
    std::string receivedString = clearEmptySpaces( str );

    mIncomingMessages.push( receivedString );
}
/*end of methods from IProtocolObserver*/

std::string JSONHandler::clearEmptySpaces( const std::string & input )
{
    std::string result = "";
    for ( int i = 0; i < input.size(); i++ )
    {
        if ( (input[i] != 0x09) && (input[i] != 0x20) && (input[i] != 0x0A) )
        {
            result += input[i];
        }
    }
    return result;
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
            LOG4CPLUS_INFO( mLogger, "Incoming message received." );
            std::string jsonMessage = handler -> mIncomingMessages.pop();

            ALRPCMessage * currentMessage = Marshaller::fromString( jsonMessage );

            if ( !handler -> mMessagesObserver )
            {
                LOG4CPLUS_ERROR( mLogger, "Cannot handle message: MessageObserver doesn't exist." );
                pthread_exit( 0 );
            }
            handler -> mMessagesObserver -> onMessageReceivedCallback( currentMessage, handler -> mSessionID );
            LOG4CPLUS_INFO( mLogger, "Incoming message handled." );
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
            LOG4CPLUS_INFO( mLogger, "Outgoing message received." );
            const ALRPCMessage * message = handler -> mOutgoingMessages.pop();

            std::string messageString = Marshaller::toString( message );

            UInt8* pData;
            pData = new UInt8[messageString.length() + 1];
            memcpy (pData, messageString.c_str(), messageString.length() + 1);

            if ( !handler -> mProtocolHandler )
            {
                LOG4CPLUS_ERROR( mLogger, "Cannot handle message: ProtocolHandler doesn't exist." );
                pthread_exit( 0 );
            }
            handler -> mProtocolHandler -> sendData( handler -> mSessionID,  AxisCore::SERVICE_TYPE_RPC, 
                    messageString.size() + 1, pData, false );

            delete message;
            LOG4CPLUS_INFO( mLogger, "Outgoing message handled." );
        }
        handler -> mOutgoingMessages.wait();
    }
}
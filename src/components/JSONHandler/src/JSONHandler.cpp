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
#include "JSONHandler/ALRPCObjects/V1/Marshaller.h"
#include "JSONHandler/ALRPCObjects/V2/Marshaller.h"


log4cplus::Logger JSONHandler::mLogger = log4cplus::Logger::getInstance(LOG4CPLUS_TEXT("JSONHandler"));

JSONHandler::JSONHandler( NsProtocolHandler::ProtocolHandler * protocolHandler ) :
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

void JSONHandler::sendRPCMessage( const NsAppLinkRPC::ALRPCMessage * message, int connectionKey )
{
    LOG4CPLUS_INFO(mLogger, "An outgoing message has been received" );
    if ( message )
    {
        mOutgoingMessages.push( std::make_pair( connectionKey, message ));
    }
}
/*End of methods for IRPCMessagesObserver*/
void JSONHandler::setProtocolHandler( NsProtocolHandler::ProtocolHandler * protocolHandler )
{
    if ( !protocolHandler )
    {
        LOG4CPLUS_ERROR(mLogger, "Invalid (null) pointer to ProtocolHandler.");
        return;
    }
    mProtocolHandler = protocolHandler;
}

void JSONHandler::onDataReceivedCallback( const NsProtocolHandler::AppLinkRawMessage * message )
{
    if ( !message )
    {
        LOG4CPLUS_ERROR(mLogger, "Received invalid message from ProtocolHandler.");
        return;
    }

    LOG4CPLUS_INFO( mLogger, "Received message from mobile App: " << message->getData() << " of size " << message->getDataSize());

    mIncomingMessages.push( message );
}
/*end of methods from IProtocolObserver*/

std::string JSONHandler::clearEmptySpaces( const std::string & input )
{
    std::string str = input;
    str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
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
            const NsProtocolHandler::AppLinkRawMessage * message = handler -> mIncomingMessages.pop();

            NsAppLinkRPC::ALRPCMessage * currentMessage = 0;

            LOG4CPLUS_INFO_EXT(mLogger, "Message of protocol version " << message -> getProtocolVersion());

            if ( message -> getProtocolVersion() == 1 )
            {
                currentMessage = handler -> handleIncomingMessageProtocolV1( message );
            }
            else if ( message -> getProtocolVersion() == 2 )
            {
                currentMessage = handler -> handleIncomingMessageProtocolV2( message );
            }
            else
            {
                LOG4CPLUS_WARN(mLogger, "Message of wrong protocol version received.");
                continue;
            }

            if ( !currentMessage )
            {
                LOG4CPLUS_ERROR( mLogger, "Invalid mobile message received." );
                continue;
            }

            currentMessage -> setProtocolVersion( message -> getProtocolVersion() );

            if ( !handler -> mMessagesObserver )
            {
                LOG4CPLUS_ERROR( mLogger, "Cannot handle mobile message: MessageObserver doesn't exist." );
                pthread_exit( 0 );
            }

            handler -> mMessagesObserver -> onMessageReceivedCallback( currentMessage, message -> getConnectionKey() );

            LOG4CPLUS_INFO( mLogger, "Incoming mobile message handled." );
        }
        handler -> mIncomingMessages.wait();
    }
}

NsAppLinkRPC::ALRPCMessage * JSONHandler::handleIncomingMessageProtocolV1( const NsProtocolHandler::AppLinkRawMessage * message )
{
    std::string jsonMessage = std::string( (const char*)message->getData(), message->getDataSize());

    if ( jsonMessage.length() == 0 )
    {
        LOG4CPLUS_ERROR(mLogger, "Received invalid json packet.");
        return 0;
    }

    std::string jsonCleanMessage = clearEmptySpaces( jsonMessage );

    NsAppLinkRPC::ALRPCMessage * messageObject = NsAppLinkRPC::Marshaller::fromString( jsonCleanMessage );
    LOG4CPLUS_INFO_EXT(mLogger, "Received a message from mobile side: " << std::endl << NsAppLinkRPC::Marshaller::toJSON(messageObject));
    return messageObject;
}

NsAppLinkRPC::ALRPCMessage * JSONHandler::handleIncomingMessageProtocolV2( const NsProtocolHandler::AppLinkRawMessage * message )
{
    unsigned char * receivedData = message->getData();
    unsigned char offset = 0;
    unsigned char firstByte = receivedData[offset++];

    int rpcType = -1;
    unsigned char rpcTypeFlag = firstByte >> 4u;
    switch( rpcTypeFlag )
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
    LOG4CPLUS_INFO_EXT(mLogger, "RPC Type of the message is " << rpcType << " from flag " << rpcTypeFlag);

    unsigned int functionId = firstByte >> 8u;

    functionId <<= 24u;
    functionId |= receivedData[offset++] << 16u;
    functionId |= receivedData[offset++] << 8u;
    functionId |= receivedData[offset++];

    LOG4CPLUS_INFO_EXT(mLogger, "FunctionId is " << functionId);

    unsigned int correlationId = receivedData[offset++] << 24u;
    correlationId |= receivedData[offset++] << 16u;
    correlationId |= receivedData[offset++] << 8u;
    correlationId |= receivedData[offset++];

    LOG4CPLUS_INFO_EXT(mLogger, "Correlation id " << correlationId);

    unsigned int jsonSize = receivedData[offset++] << 24u;
    jsonSize |= receivedData[offset++] << 16u;
    jsonSize |= receivedData[offset++] << 8u;
    jsonSize |= receivedData[offset++];

    LOG4CPLUS_INFO_EXT(mLogger, "Json size is " << jsonSize);

    if ( jsonSize > message->getDataSize() )
    {
        LOG4CPLUS_ERROR(mLogger, "Received invalid json packet header.");
        return 0;
    }

    std::string jsonMessage = std::string( (const char*)receivedData + offset, jsonSize );
    if ( jsonMessage.length() == 0 )
    {
        LOG4CPLUS_ERROR(mLogger, "Received invalid json packet.");
        return 0;
    }

    std::string jsonCleanMessage = clearEmptySpaces( jsonMessage );

    NsAppLinkRPC::ALRPCMessage * messageObject = NsAppLinkRPCV2::Marshaller::fromString( jsonCleanMessage, static_cast<NsAppLinkRPCV2::FunctionID::FunctionIDInternal>(functionId), static_cast<NsAppLinkRPCV2::messageType::messageTypeInternal>(rpcType) );
    LOG4CPLUS_INFO_EXT(mLogger, "Received a message from mobile side: " << std::endl << NsAppLinkRPCV2::Marshaller::toJSON(messageObject, static_cast<NsAppLinkRPCV2::FunctionID::FunctionIDInternal>(functionId), static_cast<NsAppLinkRPCV2::messageType::messageTypeInternal>(rpcType)));

    if ( message -> getDataSize() > offset + jsonSize )
    {
        unsigned int binarySize = message->getDataSize() - offset - jsonSize;
        std::vector<unsigned char> binaryData( receivedData+offset+jsonSize, 
            receivedData + message->getDataSize() );
        LOG4CPLUS_INFO_EXT(mLogger, "Binary data is present in message.");
        messageObject -> setBinaryData( binaryData );
    }

    if ( messageObject )
    {
        messageObject->setMethodId( functionId );
        /*if ( RPC_REQUEST == rpcTypeFlag || RPC_RESPONSE == rpcTypeFlag )
        {
            messageObject->setCorrelationID( correlationId );
        } */
    }

    return messageObject;
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
            std::pair<int,const NsAppLinkRPC::ALRPCMessage *> messagePair = handler -> mOutgoingMessages.pop();
            const NsAppLinkRPC::ALRPCMessage *  message = messagePair.second;
            LOG4CPLUS_INFO( mLogger, "Outgoing mobile message " << message->getMethodId() << " received." );

            NsProtocolHandler::AppLinkRawMessage * msgToProtocolHandler = 0;

            if ( message -> getProtocolVersion() == 1 )
            {
                msgToProtocolHandler = handler -> handleOutgoingMessageProtocolV1(messagePair.first, message);
            }
            else if ( message -> getProtocolVersion() == 2 )
            {
                LOG4CPLUS_INFO_EXT(mLogger, "method id " << static_cast<NsAppLinkRPCV2::FunctionID::FunctionIDInternal>(message -> getMethodId())
                    << "; message type " << static_cast<NsAppLinkRPCV2::messageType::messageTypeInternal>(message -> getMessageType()));

                msgToProtocolHandler = handler -> handleOutgoingMessageProtocolV2(messagePair.first, message);
            }

            if ( !msgToProtocolHandler )
            {
                LOG4CPLUS_ERROR( mLogger, "Faile to create message string.");
                continue;
            }

            if ( !handler -> mProtocolHandler )
            {
                LOG4CPLUS_ERROR( mLogger, "Cannot handle mobile message: ProtocolHandler doesn't exist." );
                pthread_exit( 0 );
            }

            LOG4CPLUS_INFO_EXT(mLogger, "Sending to ProtocolHandler: " << msgToProtocolHandler->getData() << " of size " << msgToProtocolHandler->getDataSize());
            handler -> mProtocolHandler -> sendData( msgToProtocolHandler );

            delete message;
            LOG4CPLUS_INFO( mLogger, "Outgoing mobile message handled." );
        }
        handler -> mOutgoingMessages.wait();
    }
}

NsProtocolHandler::AppLinkRawMessage * JSONHandler::handleOutgoingMessageProtocolV1( int connectionKey, const NsAppLinkRPC::ALRPCMessage *  message )
{
    LOG4CPLUS_INFO_EXT(mLogger, "handling a message " << message->getMethodId() << " protocol 1");
    LOG4CPLUS_INFO_EXT(mLogger, "message text: " << std::endl << NsAppLinkRPC::Marshaller::toJSON( message ));
    std::string messageString = NsAppLinkRPC::Marshaller::toString( message );

    if ( messageString.length() == 0 )
    {
        LOG4CPLUS_ERROR(mLogger, "Failed to serialize ALRPCMessage object version 1.");
        return 0;
    }

    unsigned char * rawMessage = new unsigned char[messageString.length() + 1];
    memcpy( rawMessage, messageString.c_str(), messageString.length() +1 );

    NsProtocolHandler::AppLinkRawMessage * msgToProtocolHandler = new NsProtocolHandler::AppLinkRawMessage(
                connectionKey,
                1,
                rawMessage,
                messageString.length() + 1);

    return msgToProtocolHandler;
}

NsProtocolHandler::AppLinkRawMessage * JSONHandler::handleOutgoingMessageProtocolV2( int connectionKey, const NsAppLinkRPC::ALRPCMessage *  message )
{
    LOG4CPLUS_INFO_EXT(mLogger, "handling a message " << message->getMethodId() << " protocol 2");
    LOG4CPLUS_INFO_EXT(mLogger, "message text: " << std::endl << NsAppLinkRPCV2::Marshaller::toJSON( message,
                                                                     static_cast<NsAppLinkRPCV2::FunctionID::FunctionIDInternal>(message -> getMethodId()),
                                                                     static_cast<NsAppLinkRPCV2::messageType::messageTypeInternal>(message -> getMessageType())) );
    std::string messageString = NsAppLinkRPCV2::Marshaller::toString( message,
                        static_cast<NsAppLinkRPCV2::FunctionID::FunctionIDInternal>(message -> getMethodId()),
                        static_cast<NsAppLinkRPCV2::messageType::messageTypeInternal>(message -> getMessageType()) );
    if ( messageString.length() == 0 )
    {
        LOG4CPLUS_ERROR(mLogger, "Failed to serialize ALRPCMessage object version 2.");
        return 0;
    }

    const uint MAX_HEADER_SIZE = 12;
    unsigned int jsonSize = messageString.length() +1;
    unsigned int binarySize = 0;
    if ( message->getBinaryData() )
    {
        binarySize = message->getBinaryData()->size();
    }
    unsigned char * dataForSending = new unsigned char[MAX_HEADER_SIZE + jsonSize + binarySize];
    unsigned char offset = 0;

    unsigned char rpcTypeFlag = 0;
    switch( message->getMessageType() )
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
    dataForSending[offset++] = ( (rpcTypeFlag << 4) & 0xF0 ) | (functionId >> 24);
    dataForSending[offset++] = functionId >> 16;
    dataForSending[offset++] = functionId >> 8;
    dataForSending[offset++] = functionId;

    unsigned int correlationId = 4567;//message->getCorrelationId();
    dataForSending[offset++] = correlationId >> 24;
    dataForSending[offset++] = correlationId >> 16;
    dataForSending[offset++] = correlationId >> 8;
    dataForSending[offset++] = correlationId;

    dataForSending[offset++] = jsonSize >> 24;
    dataForSending[offset++] = jsonSize >> 16;
    dataForSending[offset++] = jsonSize >> 8;
    dataForSending[offset++] = jsonSize;

    memcpy( dataForSending + offset, messageString.c_str(), jsonSize );

    if ( message->getBinaryData() )
    {
        const std::vector<unsigned char> & binaryData = *(message->getBinaryData());
        unsigned char * currentPointer = dataForSending + offset + jsonSize;
        for( unsigned int i = 0; i < binarySize; ++i )
        {
            currentPointer[i] = binaryData[i];
        }
    }

    NsProtocolHandler::AppLinkRawMessage * msgToProtocolHandler = new NsProtocolHandler::AppLinkRawMessage(
                        connectionKey,
                        2,
                        dataForSending,
                        MAX_HEADER_SIZE + jsonSize + binarySize);

    return msgToProtocolHandler;
}

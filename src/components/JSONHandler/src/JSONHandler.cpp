
#include "JSONHandler/JSONHandler.h"
#include <algorithm>
#include <string.h>
#include <stdio.h>
#include <json/reader.h>
#include <json/writer.h>
#include "JSONHandler/RegisterAppInterfaceResponse.h"
#include <algorithm>
#include <pthread.h>



JSONHandler::JSONHandler( AxisCore::ProtocolHandler * protocolHandler )
:mProtocolHandler( protocolHandler )
{
    mFactory = new MobileRPCFactory();
    pthread_create( &mWaitForIncomingMessagesThread, NULL, &JSONHandler::waitForIncomingMessages, (void *)this );
}
    
JSONHandler::~JSONHandler()
{
    pthread_join( mWaitForIncomingMessagesThread, NULL );
    mProtocolHandler = 0;
    mMessagesObserver = 0;
    delete mFactory;
    mFactory = 0;
}

/*Methods for IRPCMessagesObserver*/
void JSONHandler::setRPCMessagesObserver( IRPCMessagesObserver * messagesObserver )
{
    if ( !messagesObserver )
    {
        //TRACE
    }
    mMessagesObserver = messagesObserver;
}

void JSONHandler::sendRPCMessage( const MobileRPCMessage * message )
{
    if ( mProtocolHandler && message )
    {
        //mProtocolHandler -> sendData( mSessionID, AxisCore::SERVICE_TYPE_RPC, UInt32 dataSize, UInt8 *data, bool compress);
    } 
}
/*End of methods for IRPCMessagesObserver*/

/*Methods from IProtocolObserver*/
void JSONHandler::setProtocolHandler( AxisCore::ProtocolHandler * protocolHandler )
{
    mProtocolHandler = protocolHandler;
}

void JSONHandler::sessionStartedCallback(const UInt8 sessionID)
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
        return; //TRACE
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

MobileRPCMessage * JSONHandler::createObjectFromJSON( const std::string & jsonString )
{
    Json::Value root;   
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( jsonString, root );
    if ( !parsingSuccessful ) 
    {
        return 0;
    }

    MobileRPCMessage * result = 0;    
    unsigned int protocolVersion = 0;

    result = checkMessageTypeForProtocol1( root );
    if ( result )
    {        
        return result;
    } 
    else 
    {
        return 0; //version 2 or higher
    }
}

std::string JSONHandler::serializeObjectToJSON( const MobileRPCMessage & mobileRPCObject )
{
    if ( mobileRPCObject.getProtocolVersion() == 1 )
    {
        return serializeObjectToJSONProtocol1( mobileRPCObject );
    }
}

std::string JSONHandler::serializeObjectToJSONProtocol1 ( const MobileRPCMessage & mobileRPCObject )
{
    Json::Value root = createJSONFromObject( mobileRPCObject );
    return jsonToString( root );
}

Json::Value JSONHandler::createJSONFromObject ( const MobileRPCMessage & mobileRPCObject )
{
    Json::Value root;
    std::string messageType;
    switch( mobileRPCObject.getMessageType() ) {
        case 0:
            messageType = "request";
            break;
        case 1:
            messageType = "response";
            break;
        case 2:
            messageType = "notification";
            break;
        default:
            messageType = "";
    }
    
    Json::Value typeValue;
    
    if ( mobileRPCObject.getCorrelationID() != 0 )
    {
        typeValue["correlationID"] = mobileRPCObject.getCorrelationID();
    }
    if ( !mobileRPCObject.getFunctionName().empty() )
    {
        typeValue["name"] = mobileRPCObject.getFunctionName();
    }
    root[messageType] = typeValue;
    return root;
}

std::string JSONHandler::jsonToString( const Json::Value & jsonObject )
{
    Json::FastWriter writer;
    std::string root_to_print = writer.write( jsonObject );
    return root_to_print;
}

MobileRPCMessage * JSONHandler::checkMessageTypeForProtocol1 ( const Json::Value & root )
{
    if ( !root["request"].isNull() )
    {
        return generateRequestVersion1( root["request"] );
    }
    if ( !root["response"].isNull() )
    {
        return generateResponseVersion1( root["response"] );
    }
    if ( !root["notification"].isNull() )
    {
        return generateNotificationVersion1( root["notification"] );
    }

    return 0;
}

MobileRPCRequest * JSONHandler::generateRequestVersion1( const Json::Value & root ) 
{
    MobileRPCRequest * request = new MobileRPCRequest( 1 );
    return (MobileRPCRequest *)fillMessageWithData( root, request );
}

MobileRPCResponse * JSONHandler::generateResponseVersion1( const Json::Value & root ) 
{
    MobileRPCResponse * response = new MobileRPCResponse( 1 );
    return (MobileRPCResponse *)fillMessageWithData( root, response );
}

MobileRPCNotification * JSONHandler::generateNotificationVersion1( const Json::Value & root )
{
    MobileRPCNotification * notification = new MobileRPCNotification( 1 );
    return (MobileRPCNotification *) fillMessageWithData( root, notification );
}

MobileRPCMessage * JSONHandler::fillMessageWithData ( const Json::Value & jsonMessage, 
    MobileRPCMessage * message )
{
    unsigned int correlationID = 0;
    Json::Value value;
    value = jsonMessage["correlationID"];
    if ( !value.isNull() ) {
        correlationID = value.asInt();
        message->setCorrelationID( correlationID );
    }    

    value = jsonMessage["name"];
    if ( !value.isNull() )
    {
        message->setFunctionName( value.asString() );
    }

    value = jsonMessage["parameters"];
    if ( !value.isNull() )
    {
        Json::Value paramValue;
        paramValue["parameters"] = value;
        Json::FastWriter writer;
        std::string paramsToString = writer.write( paramValue );
        message -> setParametersString( paramsToString );
    }

    return message;
}

Json::Value JSONHandler::getParametersFromJSON( const std::string & jsonString )
{
    Json::Value root;   
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( jsonString, root );
    if ( !parsingSuccessful ) 
    {
        return Json::Value::null;
    }

    return root["parameters"];
}

const MobileRPCFactory * JSONHandler::getFactory() const
{
    return mFactory;
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
            std::string jsonMessage = handler -> mIncomingMessages.pop();


            MobileRPCMessage * mCurrentMessage = handler -> createObjectFromJSON( jsonMessage );

            RegisterAppInterfaceResponse response = handler -> mFactory -> createRegisterAppInterfaceResponse( *mCurrentMessage );

            Json::Value parameters = handler -> mFactory -> serializeRegisterAppInterfaceResponse( response );
            Json::Value root = handler -> createJSONFromObject( response );
            if ( root.isNull() )
            {
                pthread_exit( 0 );
            }
            root["parameters"] = parameters;
            std::string responseString = handler -> jsonToString( root );
            printf("Response string:\n%s\n", responseString.c_str());
            UInt8* pData;
            pData = new UInt8[responseString.length() + 1];
            memcpy (pData, responseString.c_str(), responseString.length() + 1);
            handler -> mProtocolHandler -> sendData( handler -> mSessionID,  AxisCore::SERVICE_TYPE_RPC, 
                responseString.size() + 1, pData, false );

            // OnHMIStatus
            OnHMIStatus notification    = handler -> mFactory -> createOnHMIStatus();
            Json::Value parameters1     = handler -> mFactory -> serializeOnHMIStatus( notification );

            Json::Value root1 = handler -> createJSONFromObject( notification );
            if ( root1.isNull() )           
            {
                pthread_exit( 0 );;
            }

            root1["parameters"] = parameters1;
            std::string notificationString = handler -> jsonToString( root1 );
            printf("Notification string:\n%s\n", notificationString.c_str());
            UInt8* pData1;
            pData1 = new UInt8[notificationString.length() + 1];
            memcpy (pData1, notificationString.c_str(), notificationString.length() + 1);
            handler -> mProtocolHandler -> sendData( handler -> mSessionID,  AxisCore::SERVICE_TYPE_RPC, 
                notificationString.size() + 1, pData1, false );

        }
        sleep(10);
    }
    pthread_exit( 0 );
}

#include "JSONHandler/JSONHandler.h"
#include <algorithm>


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
        result -> setOriginalString( jsonString );
        return result;
    } 
    else 
    {
        return 0; //version 2 or higher
    }
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

    return message;
}
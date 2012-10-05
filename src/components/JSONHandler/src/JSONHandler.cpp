

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

    return 0;
}

MobileRPCRequest * JSONHandler::generateRequestVersion1 ( const Json::Value & root ) 
{
    MobileRPCRequest * request = new MobileRPCRequest( 1 );
    return (MobileRPCRequest *)fillMessageWithData( root, request );
}

/*MobileRPCResponse * generateResponseVersion1 ( const Json::Value & root ) 
{
    unsigned int correlationID = 0;
    Json::Value value;
    value = root["correlationID"];
    if ( !value.isNull() ) {
        correlationID = value.asInt();
    }
    return new MobileRPCResponse( 1, correlationID );
}*/

MobileRPCMessage * JSONHandler::fillMessageWithData ( const Json::Value & jsonMessage, 
    MobileRPCMessage * message )
{
    unsigned int correlationID = 0;
    Json::Value value;
    value = jsonMessage["correlationID"];
    if ( !value.isNull() ) {
        correlationID = value.asInt();
    }
    message->setCorrelationID( correlationID );
    return message;
}

#include "JSONHandler/JSONHandler.h"
#include <algorithm>
#include <json/reader.h>
#include <json/writer.h>

MobileRPCFactory * JSONHandler::mFactory = 0;

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
    
    Json::StyledWriter writer;
    std::string root_to_print = writer.write( root );
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

MobileRPCFactory * JSONHandler::getFactory()
{
    if ( mFactory == 0 )
    {
        mFactory = new MobileRPCFactory();
    }
    return mFactory;
}
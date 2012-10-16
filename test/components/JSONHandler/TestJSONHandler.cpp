#include "TestJSONHandler.h"
#include <iostream>
#include <json/writer.h>
#include <cstdio>
#include <cstdlib>
#include <string.h>

void TestJSONHandler::secondRelease( const std::string & jsonString )
{    
    jsonHandler -> mIncomingMessages.push( jsonString );
}

void TestJSONHandler::firstRelease( const std::string & jsonString )
{
    MobileRPCMessage * message = jsonHandler -> createObjectFromJSON( jsonString );
    std::cout << "type: " << message->getMessageType() << std::endl;
    std::cout << "protocol version: " << message->getProtocolVersion() << std::endl;
    std::cout << "correation id: " << message -> getCorrelationID() << std::endl;
    std::cout << "function name: " << message -> getFunctionName() << std::endl;
    std::cout << "original string: " << message -> getParametersString() << std::endl;

    std::string serializedString = jsonHandler -> serializeObjectToJSON( *message );
    std::cout << serializedString << std::endl;

    RegisterAppInterface requestMessage = jsonHandler -> getFactory() -> createRegisterAppInterface( *message );
    Json::Value params = jsonHandler -> getFactory() -> serializeRegisterAppInterface( requestMessage );
    Json::StyledWriter writer;
    std::string params_to_print = writer.write( params );
    std::cout << "serialized params for RegisterAppInterface: \n" << params_to_print << std::endl;

    RegisterAppInterfaceResponse response = jsonHandler -> getFactory() -> createRegisterAppInterfaceResponse( *message );
    Json::Value parameters = jsonHandler -> getFactory() -> serializeRegisterAppInterfaceResponse( response );

    Json::Value root = jsonHandler -> createJSONFromObject( response );
    if ( root.isNull() )
    {
        return;
    }

    root["response"]["parameters"] = parameters;
    std::string responseString = jsonHandler -> jsonToString( root );
    UInt8* pData;
    pData = new UInt8[responseString.length() + 1];
    memcpy (pData, responseString.c_str(), responseString.length() + 1);
    std::cout << "Register response: " << responseString << std::endl;
    std::cout << "UInt8: " << pData << std::endl;

    delete message;
}
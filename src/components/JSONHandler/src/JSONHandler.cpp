#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/JSONHandler.h"
#include <json/value.h>
#include <json/reader.h>


MobileRPCMessage JSONHandler::createObjectFromJSON( const std::string & jsonString )
{
    Json::Value root;   
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( jsonString, root );
    if ( !parsingSuccessful ) 
    {
        //return -1;
    }

   /* unsigned int protocolVersion;
    MobileRPCMessage::MessageType messageType;
    unsigned int mCorrelationID;
    Parameters mParameters;
    std::string mFunctionName;
    unsigned int mFunctionID;
    unsigned int mJSONMessageSize;*/

    Json::Value::Members members = root.getMemberNames();

    for ( int i = 0; i < members.size(); ++i ) 
    {
        
    }

    if ( val.isNull() ) {
        std::cout<<"null"<<std::endl;
    } else if ( val.isArray() ) {
        //for ( int i = 0; i < val.size(); ++i ) {
            Json::Value x = val["correlationID"];
            if ( x.isNull() ) {
                std::cout << "no correlationID" << std::endl;
            }
        //}
    } else {
        Json::Value x = val["correlationID"];
        if ( x.isNull() ) {
            std::cout << "no correlationID" << std::endl;
        } else {
            std::cout << x.asInt() << std::endl;
        }
        std::cout << "smth" << std::endl;
    }

    Json::StyledWriter writer;
    std::string root_to_print = writer.write( root );
    std::cout << root_to_print;
    return 0;
}
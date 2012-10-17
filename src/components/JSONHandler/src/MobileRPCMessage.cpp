#include "JSONHandler/MobileRPCMessage.h"
#include "JSONHandler/Constants.h"
#include <json/reader.h>
#include <json/writer.h>

MobileRPCMessage::MobileRPCMessage( 
    unsigned int protocolVersion, MessageType messageType )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( 0 )
,mFunctionName( "" )
,mFunctionID( 0 )
,mJSONMessageSize( 0 )
{}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, 
    MessageType messageType, std::string functionName )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( 0 )
,mFunctionName( functionName )
,mFunctionID( 0 )
,mJSONMessageSize( 0 )
{}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, 
    MessageType messageType, unsigned int functionID )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( 0 )
,mFunctionName( "" )
,mFunctionID( functionID )
,mJSONMessageSize( 0 )
{}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, std::string functionName,
        unsigned int functionID, unsigned int messageSize )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( correlationID )
,mFunctionName( functionName )
,mFunctionID( functionID )
,mJSONMessageSize( messageSize )
{ }

MobileRPCMessage::MobileRPCMessage( const MobileRPCMessage& message )
:mProtocolVersion( message.getProtocolVersion() )
,mMessageType( message.getMessageType() )
,mCorrelationID( message.getCorrelationID() )
,mFunctionName( message.getFunctionName() )
,mFunctionID( message.getFunctionID() )
,mJSONMessageSize( message.getJSONMessageSize() )
{}

MobileRPCMessage::~MobileRPCMessage() 
{}

std::string MobileRPCMessage::serialize() const
{ 
    Json::Value result;
    Json::Value value;

    if ( getProtocolVersion() == 1 )
    {
        std::string messageType;
        switch( getMessageType() ) {
            case 0:
                messageType = Constants::REQUEST;
                break;
            case 1:
                messageType = Constants::RESPONSE;
                break;
            case 2:
                messageType = Constants::NOTIFICATION;
                break;
            default:
                messageType = "";
        }
            
        if ( getCorrelationID() != 0 )
        {
            value[Constants::CORRELATIONID] = getCorrelationID();
        }
        if ( !getFunctionName().empty() )
        {
            value[Constants::FUNCTIONNAME] = getFunctionName();
        }
        result[messageType] = value;
    }

    if ( !result.isNull() )
    {
        return jsonToString(result);
    }
    else 
    {
        return "";
    }
}
    
int MobileRPCMessage::deserialize( const std::string & jsonString )
{
    Json::Value root;   
    Json::Reader reader;
    bool parsingSuccessful = reader.parse( jsonString, root );
    if ( !parsingSuccessful ) 
    {
        return -1;
    }
   
    if ( getProtocolVersion() == 1 )
    {
        Json::Value value;
        value = root["correlationID"];
        if ( !value.isNull() ) {
            setCorrelationID( value.asInt() );
        }    

        value = root["name"];
        if ( !value.isNull() )
        {
            setFunctionName( value.asString() );
        }
    }
    else
    {
        return 0; //version 2 or higher
    }   

    return 0; 
}

std::string MobileRPCMessage::jsonToString( const Json::Value & jsonObject ) const
{
    Json::FastWriter writer;
    std::string root_to_print = writer.write( jsonObject );
    return root_to_print;
}

unsigned int MobileRPCMessage::getProtocolVersion() const
{
    return mProtocolVersion;
}
    
MobileRPCMessage::MessageType MobileRPCMessage::getMessageType() const
{
    return mMessageType;
}
    
unsigned int MobileRPCMessage::getCorrelationID() const
{
    return mCorrelationID;
}
    
const std::string & MobileRPCMessage::getFunctionName() const
{
    return mFunctionName;
}
    
unsigned int MobileRPCMessage::getFunctionID() const
{
    return mFunctionID;
}
    
unsigned int MobileRPCMessage::getJSONMessageSize() const
{
    return mJSONMessageSize;
}

void MobileRPCMessage::setFunctionName( const std::string & functionName )
{
    mFunctionName = functionName;
}
    
void MobileRPCMessage::setFunctionID( unsigned int functionID )
{
    mFunctionID = functionID;
}
    
void MobileRPCMessage::setJSONMessageSize( unsigned int messageSize )
{
    mJSONMessageSize = messageSize;
}

void MobileRPCMessage::setCorrelationID( unsigned int correlationID )
{
    mCorrelationID = correlationID;
}


void MobileRPCMessage::setParametersString( std::string parametersString )
{
    mParametersString = parametersString;
}
    
std::string MobileRPCMessage::getParametersString() const
{
    return mParametersString;
}

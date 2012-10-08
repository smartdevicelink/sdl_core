#include "JSONHandler/MobileRPCMessage.h"


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
{
    MobileRPCMessage(protocolVersion, messageType, 
        0, functionName, 0, 0);
}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, 
    MessageType messageType, unsigned int functionID )
{
    
    MobileRPCMessage(protocolVersion, messageType, 
        0, "", functionID, 0);
}

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

MobileRPCMessage::~MobileRPCMessage() 
{}

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


void MobileRPCMessage::setOriginalString( std::string originalString )
{
    mOriginalString = originalString;
}
    
std::string MobileRPCMessage::getOriginalString() const
{
    return mOriginalString;
}
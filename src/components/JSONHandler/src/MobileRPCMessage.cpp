#include "JSONHandler/MobileRPCMessage.h"

MobileRPCMessage::MobileRPCMessage( 
    unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( correlationID )
,mFunctionName( "" )
,mFunctionID( 0 )
,mJSONMessageSize( 0 )
{}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, Parameters params, std::string functionName )
{
    MobileRPCMessage(protocolVersion, messageType, 
        correlationID, params, functionName, 0, 0);
}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, Parameters params,
        unsigned int functionID, unsigned int messageSize )
{
    MobileRPCMessage(protocolVersion, messageType, 
        correlationID, params, "", functionID, messageSize);
}

MobileRPCMessage::MobileRPCMessage( unsigned int protocolVersion, MessageType messageType, 
        unsigned int correlationID, Parameters params, std::string functionName,
        unsigned int functionID, unsigned int messageSize )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mCorrelationID( correlationID )
,mParameters( params )
,mFunctionName( functionName )
,mFunctionID( functionID )
,mJSONMessageSize( messageSize )
{ }

MobileRPCMessage::~MobileRPCMessage() {}

MobileRPCMessage::MessageType MobileRPCMessage::getMessageType() const
{
    return mMessageType;
}

void MobileRPCMessage::setCorrelationID( unsigned int correlationID )
{
    mCorrelationID = correlationID;
}
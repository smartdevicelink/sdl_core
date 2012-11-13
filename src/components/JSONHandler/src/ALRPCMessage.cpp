#include "JSONHandler/ALRPCMessage.h"

using namespace AppLinkRPC;

ALRPCMessage::ALRPCMessage( unsigned int protocolVersion, MessageType messageType )
:mProtocolVersion( protocolVersion)
,mMessageType( messageType )
,mMethodId(-1)
{
}

ALRPCMessage::ALRPCMessage( unsigned int protocolVersion, MessageType messageType, int  methodId )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mMethodId( methodId )
{}

ALRPCMessage::~ALRPCMessage()
{}

unsigned int ALRPCMessage::getProtocolVersion() const
{
    return mProtocolVersion;
}

ALRPCMessage::MessageType ALRPCMessage::getMessageType() const
{
    return mMessageType;
}
    
int ALRPCMessage::getMethodId() const
{
    return mMethodId;
}

void ALRPCMessage::setProtocolVersion( unsigned int protocolVersion )
{
    mProtocolVersion = protocolVersion;
}

void ALRPCMessage::setMessageType( ALRPCMessage::MessageType messageType )
{
    mMessageType = messageType;
}

void ALRPCMessage::setMethodId( int methodId )
{
    mMethodId = methodId;
}

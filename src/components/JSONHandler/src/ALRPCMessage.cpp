/**
* \file ALRPCMessage.cpp
* \brief ALRPCMessage class source file.
* \author PVyshnevska
*/


#include "JSONHandler/ALRPCMessage.h"

using namespace NsAppLinkRPC;

ALRPCMessage::ALRPCMessage( unsigned int protocolVersion, MessageType messageType )
:mProtocolVersion( protocolVersion)
,mMessageType( messageType )
,mMethodId(-1)
,mBinaryData( 0 )
,mCorrelationID( 0 )
{
}

ALRPCMessage::ALRPCMessage( unsigned int protocolVersion, MessageType messageType, int  methodId )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mMethodId( methodId )
,mBinaryData( 0 )
,mCorrelationID( 0 )
{}

ALRPCMessage::~ALRPCMessage()
{
    if ( mBinaryData )
    {
        delete mBinaryData;
        mBinaryData = 0;
    }
}

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

void ALRPCMessage::setBinaryData( const std::vector<unsigned char> & binaryData )
{
    if ( mBinaryData )
    {
        delete mBinaryData;
    }
    mBinaryData = new std::vector<unsigned char> (binaryData);
}

const std::vector<unsigned char> * ALRPCMessage::getBinaryData() const
{
    return mBinaryData;
}

unsigned int ALRPCMessage::getCorrelationID() const 
{ 
    return mCorrelationID; 
}

void ALRPCMessage::setCorrelationID( unsigned int correlationID ) 
{ 
    mCorrelationID = correlationID; 
}
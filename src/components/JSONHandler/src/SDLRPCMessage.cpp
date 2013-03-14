/**
* \file SDLRPCMessage.cpp
* \brief SDLRPCMessage class source file.
* \author PVyshnevska
*/


#include "JSONHandler/SDLRPCMessage.h"

using namespace NsSmartDeviceLinkRPC;

SDLRPCMessage::SDLRPCMessage( unsigned int protocolVersion, MessageType messageType )
:mProtocolVersion( protocolVersion)
,mMessageType( messageType )
,mMethodId(1)
,mBinaryData( 0 )
,mCorrelationID( 0 )
{
}

SDLRPCMessage::SDLRPCMessage( unsigned int protocolVersion, MessageType messageType, int  methodId )
:mProtocolVersion( protocolVersion )
,mMessageType( messageType )
,mMethodId( methodId )
,mBinaryData( 0 )
,mCorrelationID( 0 )
{}

SDLRPCMessage::~SDLRPCMessage()
{
    if ( mBinaryData )
    {
        delete mBinaryData;
        mBinaryData = 0;
    }
}

unsigned int SDLRPCMessage::getProtocolVersion() const
{
    return mProtocolVersion;
}

SDLRPCMessage::MessageType SDLRPCMessage::getMessageType() const
{
    return mMessageType;
}

int SDLRPCMessage::getMethodId() const
{
    return mMethodId;
}

void SDLRPCMessage::setProtocolVersion( unsigned int protocolVersion )
{
    mProtocolVersion = protocolVersion;
}

void SDLRPCMessage::setMessageType( SDLRPCMessage::MessageType messageType )
{
    mMessageType = messageType;
}

void SDLRPCMessage::setMethodId( int methodId )
{
    mMethodId = methodId;
}

void SDLRPCMessage::setBinaryData( const std::vector<unsigned char> & binaryData )
{
    if ( mBinaryData )
    {
        delete mBinaryData;
    }
    mBinaryData = new std::vector<unsigned char> (binaryData);
}

const std::vector<unsigned char> * SDLRPCMessage::getBinaryData() const
{
    return mBinaryData;
}

unsigned int SDLRPCMessage::getCorrelationID() const
{ 
    return mCorrelationID;
}

void SDLRPCMessage::setCorrelationID( unsigned int correlationID ) 
{ 
    mCorrelationID = correlationID; 
}

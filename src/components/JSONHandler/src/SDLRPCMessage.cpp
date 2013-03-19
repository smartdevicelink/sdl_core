/**
* \file SDLRPCMessage.cpp
* \brief SDLRPCMessage class source file.
* Copyright (c) 2013, Ford Motor Company
// All rights reserved.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met:
//
// Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
//
// Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following
// disclaimer in the documentation and/or other materials provided with the
// distribution.
//
// Neither the name of the Ford Motor Company nor the names of its contributors
// may be used to endorse or promote products derived from this software
// without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
// ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
// LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
// SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
// INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
// CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
// ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
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

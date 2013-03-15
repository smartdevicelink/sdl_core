/**
* \file SmartDeviceLinkRawMessage.cpp
* \brief SmartDeviceLinkRawMessage class source file.
* 
* Copyright (c) 2013 Ford Motor Company
*/

#include "ProtocolHandler/SmartDeviceLinkRawMessage.h"

using namespace NsProtocolHandler;

SmartDeviceLinkRawMessage::SmartDeviceLinkRawMessage(int connectionKey,
                            unsigned int protocolVersion,
                            unsigned char * data,
                            unsigned int dataSize) : 
 mConnectionKey( connectionKey )
,mProtocolVersion( protocolVersion )
,mData( data )
,mDataSize( dataSize )
{}
        
SmartDeviceLinkRawMessage::~SmartDeviceLinkRawMessage()
{
    if ( mData )
    {
        delete [] mData;
        mData = 0;
    }
}

int SmartDeviceLinkRawMessage::getConnectionKey() const
{
    return mConnectionKey;
}
        
unsigned char * SmartDeviceLinkRawMessage::getData() const
{
    return mData;
}
        
unsigned int SmartDeviceLinkRawMessage::getDataSize() const
{
    return mDataSize;
}

unsigned int SmartDeviceLinkRawMessage::getProtocolVersion() const
{
    return mProtocolVersion;
}

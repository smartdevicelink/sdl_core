#include "ProtocolHandler/AppLinkRawMessage.h"

using namespace NsProtocolHandler;

AppLinkRawMessage::AppLinkRawMessage(int connectionKey,
                            unsigned int protocolVersion,
                            unsigned char * data,
                            unsigned int dataSize) : 
 mConnectionKey( connectionKey )
,mProtocolVersion( protocolVersion )
,mData( data )
,mDataSize( dataSize )
{}
        
AppLinkRawMessage::~AppLinkRawMessage()
{
    if ( mData )
    {
        delete [] mData;
        mData = 0;
    }
}

unsigned int AppLinkRawMessage::getConnectionKey() const
{
    return mConnectionKey;
}
        
unsigned char * AppLinkRawMessage::getData() const
{
    return mData;
}
        
unsigned int AppLinkRawMessage::getDataSize() const
{
    return mDataSize;
}

unsigned int AppLinkRawMessage::getProtocolVersion() const
{
    return mProtocolVersion;
}

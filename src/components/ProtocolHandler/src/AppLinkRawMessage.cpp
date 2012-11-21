#include "ProtocolHandler/AppLinkRawMessage.h"

AppLinkRawMessage::AppLinkRawMessage(unsigned int connectionID,
                            unsigned int sessionID,
                            unsigned int protocolVersion,
                            unsigned int * data,
                            unsigned int dataSize) : 
 mConnectionID( connectionID )
,mSessionID( sessionID )
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

unsigned int AppLinkRawMessage::getConnectionID() const
{
    return mConnectionID;
}
        
unsigned int AppLinkRawMessage::getSessionID() const
{
    return mSessionID;
}
        
unsigned int * AppLinkRawMessage::getData() const
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

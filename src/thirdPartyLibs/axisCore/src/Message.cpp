#include <memory.h>

#include "Message.hpp"

Message::Message(const ProtocolPacketHeader &header
                                   , UInt8 *data
                                   , bool isMultiFrame) :
            mMessageID(header.messageID),
            mTotalDataBytes(0),
            mNumberOfConsFrames(0),
            mData(0),
            mDataOffset(0),
            mIsMultiFrame(isMultiFrame)
{
    if (mIsMultiFrame)
    {
        mTotalDataBytes = ( (UInt32 *)data)[0];
        mNumberOfConsFrames = ( (UInt32 *)data)[1];
    }
    else
        mTotalDataBytes = header.dataSize;

    mData = new UInt8[mTotalDataBytes];
    if (!mIsMultiFrame)
        memcpy(mData, data, mTotalDataBytes);
}

Message::~Message()
{
    delete [] mData;
}

ERROR_CODE Message::addConsecutiveMessage(const ProtocolPacketHeader &header, UInt8 *data)
{
    ERROR_CODE retVal = ERR_OK;

    if (mIsMultiFrame)
    {
        if ( (mDataOffset + header.dataSize) <= mTotalDataBytes)
        {
            memcpy(mData + mDataOffset, data, header.dataSize);
            mDataOffset += header.dataSize;
        }
        else
            retVal = ERR_FAIL;
    }

    return retVal;
}

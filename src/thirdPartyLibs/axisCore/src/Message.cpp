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
    if (mIsMultiFrame)
    {
        if ( (mDataOffset + header.dataSize) <= mTotalDataBytes)
        {
            memcpy(mData + mDataOffset, data, header.dataSize);
            mDataOffset += header.dataSize;

            return ERR_OK;
        }
        else
            return ERR_FAIL;
    }


    return ERR_FAIL;
}

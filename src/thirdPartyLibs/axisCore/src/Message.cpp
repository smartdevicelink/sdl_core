#include <memory.h>

#include "Message.hpp"

namespace AxisCore
{

Message::Message(const ProtocolPacketHeader &header
                                   , UInt8 *data
                                   , bool isMultiFrame) :
            mMessageID(header.messageID),
            mTotalDataBytes(0),
            mNumberOfConsFrames(0),
            mData(0),
            mDataOffset(0),
            mIsMultiFrame(isMultiFrame),
            mLastSubMessageNumber(0),
            mSubMessagesConnected(0)
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
        // TODO TMP OFF
        if (true /*checkMultiFrameSubMessageOrder(header)*/ )
        {
            if ( (mDataOffset + header.dataSize) <= mTotalDataBytes)
            {
                memcpy(mData + mDataOffset, data, header.dataSize);
                mDataOffset += header.dataSize;
            }
            else
                retVal = ERR_FAIL;
        }
        else
           retVal = ERR_FAIL;
    }
    else
        retVal = ERR_FAIL;

    return retVal;
}

bool Message::checkMultiFrameSubMessageOrder(const ProtocolPacketHeader &header)
{
    bool retVal = true;

    if (mSubMessagesConnected == (mNumberOfConsFrames - 1) )
    {
        if (header.frameData != FRAME_DATA_LAST_FRAME)
            retVal = false;
    }
    else
    {
        if (mLastSubMessageNumber < FRAME_DATA_MAX_VALUE)
        {
            if ( (mLastSubMessageNumber + 1) != header.frameData)
                retVal = false;
            else
            {
                mLastSubMessageNumber++;
                mSubMessagesConnected++;
            }
        }
        else if (mLastSubMessageNumber == FRAME_DATA_MAX_VALUE)
        {
            if (header.frameData != 0x01)
                retVal = false;
            else
            {
                mLastSubMessageNumber = 0x01;
                mSubMessagesConnected++;
            }
        }
        else
            retVal = false;
    }

    return retVal;
}

} //namespace AxisCore

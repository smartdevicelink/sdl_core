#include <memory.h>

#include "Message.hpp"
#include "Logger.hpp"

namespace AxisCore
{

Logger Message::mLogger = Logger::getInstance(LOG4CPLUS_TEXT("AxisCore.ProtocolHandler") );

Message::Message(const ProtocolPacketHeader &header,
                 UInt8 *data,
                 bool isMultiFrame) :
            mTotalDataBytes(0),            
            mData(0),
            mIsMultiFrame(isMultiFrame),
            mNumberOfConsFrames(0),
            mDataOffset(0),
            mLastSubMessageNumber(0),
            mSubMessagesConnected(0),
            mMessageID(header.messageID)
{
    if (mIsMultiFrame)
    {
        mTotalDataBytes  = data[0] << 24;
        mTotalDataBytes |= data[1] << 16;
        mTotalDataBytes |= data[2] << 8;
        mTotalDataBytes |= data[3];

        mNumberOfConsFrames  = data[4] << 24;
        mNumberOfConsFrames |= data[5] << 16;
        mNumberOfConsFrames |= data[6] << 8;
        mNumberOfConsFrames |= data[7];
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

RESULT_CODE Message::addConsecutiveMessage(const ProtocolPacketHeader &header, UInt8 *data)
{
    RESULT_CODE retVal = RESULT_OK;

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
                retVal = RESULT_FAIL;
        }
        else
           retVal = RESULT_FAIL;
    }
    else
        retVal = RESULT_FAIL;

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

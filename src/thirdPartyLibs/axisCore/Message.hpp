#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ProtocolPacketHeader.hpp"

class Message
{
public:
    Message(const ProtocolPacketHeader &header, UInt8 *data, bool isMultiFrame);

    ~Message();

    UInt8 *getMessageData()     { return mData; }

    UInt32 getTotalDataBytes()  { return mTotalDataBytes; }

    UInt32 getMessageID()       { return mMessageID; }

    ERROR_CODE addConsecutiveMessage(const ProtocolPacketHeader &header, UInt8 *data);

private:
    UInt32 mMessageID;
    UInt32 mTotalDataBytes;
    UInt32 mNumberOfConsFrames;
    UInt8 *mData;
    UInt32 mDataOffset;
    bool mIsMultiFrame;
};

#endif //MESSAGE_HPP

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ProtocolPacketHeader.hpp"

namespace AxisCore
{

class Message
{
public:
    Message(const ProtocolPacketHeader &header, UInt8 *data, bool isMultiFrame);

    ~Message();

    /**
      * Get pointer to the data array
      */
    UInt8 *getMessageData()     { return mData; }

    /**
      * Get size of mData array
      */
    UInt32 getTotalDataBytes()  { return mTotalDataBytes; }

    /**
      * Get message ID
      */
    UInt32 getMessageID()       { return mMessageID; }

    /**
      * Add consecutive message to multiframe message
      * @param header Message header
      * @param data Data array
      */
    ERROR_CODE addConsecutiveMessage(const ProtocolPacketHeader &header, UInt8 *data);

private:
    UInt32 mMessageID;
    UInt32 mTotalDataBytes;
    UInt32 mNumberOfConsFrames;
    UInt8 *mData;
    UInt32 mDataOffset;
    bool mIsMultiFrame;
};

} //namespace AxisCore

#endif //MESSAGE_HPP

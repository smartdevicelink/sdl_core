#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "ProtocolPacketHeader.hpp"
#include "Logger.hpp"

namespace AxisCore
{

/**
 * \class Message
 * \brief Single or multiframe message (protocol level)
 * \author amarkosov
 */
class Message
{
public:
    Message(const ProtocolPacketHeader &header, UInt8 *data, bool isMultiFrame);

    ~Message();

    /**
      * Get message ID
      */
    UInt32 getMessageID()       { return mMessageID; }

    /**
      * Get pointer to the data array
      */
    UInt8 *getMessageData()     { return mData; }

    /**
      * Get size of mData array
      */
    UInt32 getTotalDataBytes()  { return mTotalDataBytes; }

    /**
      * Add consecutive message to multiframe message
      * @param header Message header
      * @param data Data array
      */
    RESULT_CODE addConsecutiveMessage(const ProtocolPacketHeader &header, UInt8 *data);

private:
    bool checkMultiFrameSubMessageOrder(const ProtocolPacketHeader &header);

    UInt32 mTotalDataBytes;    
    UInt8 *mData;    
    bool mIsMultiFrame;
    /**
      * For multiframe messages
      */
    UInt32 mNumberOfConsFrames;
    UInt32 mDataOffset;
    UInt8 mLastSubMessageNumber;
    UInt32 mSubMessagesConnected;
    UInt32 mMessageID;
    static Logger mLogger;
};

} //namespace AxisCore

#endif //MESSAGE_HPP

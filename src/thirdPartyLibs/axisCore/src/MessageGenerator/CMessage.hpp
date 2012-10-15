#ifndef CMESSAGE_HPP_
#define CMESSAGE_HPP_

#include <queue>
#include <string>

#include "../../utils/misc/Types.hpp"
#include "../../transport/bt/Blob.hpp"

#include "../../include/ProtocolPacketHeader.hpp"

namespace AxisCore
{

struct ProtocolPacketHeader;

class CMessage
{
public:
   static Blob getNextBlob();
   static void releaseCurrentBlob(const Blob &blob);

   static void generateInitialMessage(UInt8 serviceType, UInt8 sessionID);
   static void generateFinalMessage(UInt8 serviceType, UInt8 sessionID);
   static void generateSingleMessage(UInt8 serviceType, UInt8 sessionID, std::string payload);
   static void generateMultipleMessages(UInt8 serviceType, UInt8 sessionID, std::string payload, int messagesQuantity);

   static UInt32 verify(ProtocolPacketHeader& header, UInt32 fieldsToValidate);

private:

   static std::queue<Blob> blobQueue;

   //static void write();
   //static void generate();

   static void saveSentHeader();
   static void dispayField();


   static ProtocolPacketHeader sentHeader;

   static UInt8 sVersion;
   static UInt8 sCompressedFlag;
   static UInt8 sFrameType;
   static UInt8 sServiceType;
   static UInt8 sFrameData;
   static UInt8 sSessionID;
   static UInt32 sDataSize;
   static UInt32 sMessageID;

   static void* sPacketData;

   static int getBit(const UInt32 value, const UInt32 position);
   static void setBit(UInt32 value, const UInt32 position);
   static void clearBit(UInt32 value, const UInt32 position);
};

} //namespace AxisCore

namespace Bluetooth
{
   static const Blob getBuffer()
   {
      return AxisCore::CMessage::getNextBlob();
   }

   static void releaseBuffer(const Blob& blob)
   {
      AxisCore::CMessage::releaseCurrentBlob(blob);
   }
}

#endif /* СMESSAGE_HPP_ */



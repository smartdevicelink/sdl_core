#ifndef CMESSAGE_HPP_
#define CMESSAGE_HPP_

#include <queue>
#include <string>

#include "../utils/misc/Types.hpp"
#include "../transport/bt/Blob.hpp"

namespace AxisCore
{

struct ProtocolPacketHeader;

class CMessage
{
public:
   CMessage();

   virtual const Blob getBuffer();
   virtual void releaseBuffer(const Blob&);

   virtual void sendBuffer(UInt8 * pBuffer, size_t size);

   virtual ~CMessage();

public:
   void generateInitialMessage(UInt8 serviceType);
   void generateFinalMessage(UInt8 protocolVersion, UInt8 serviceType, UInt8 sessionID, UInt32 hashCode);
   void generateSingleMessage(UInt8 protocolVersion,
                              UInt8 serviceType,
                              UInt8 sessionID,
                              std::string payload);
   void generateMultipleMessages(UInt8 protocolVersion,
                                 UInt8 serviceType,
                                 UInt8 sessionID,
                                 std::string payload,
                                 int messagesQuantity);

   UInt32 verify(ProtocolPacketHeader& header, UInt32 fieldsToValidate);

private:

   std::queue<Blob> blobQueue;
   Blob getNextBlob();
   void releaseCurrentBlob(const Blob &blob);

   void saveSentHeader();
   void dispayField();

   ProtocolPacketHeader* sentHeader;

   UInt8 sVersion;
   UInt8 sCompressedFlag;
   UInt8 sFrameType;
   UInt8 sServiceType;
   UInt8 sFrameData;
   UInt8 sSessionID;
   UInt32 sDataSize;
   UInt32 sMessageID;

   void* sPacketData;

   int getBit(const UInt32 value, const UInt32 position);
   void setBit(UInt32 value, const UInt32 position);
   void clearBit(UInt32 value, const UInt32 position);
};

} //namespace AxisCore

#endif /* СMESSAGE_HPP_ */



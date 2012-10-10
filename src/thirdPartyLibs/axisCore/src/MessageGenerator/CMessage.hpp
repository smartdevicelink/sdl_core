#ifndef CMESSAGE_HPP_
#define CMESSAGE_HPP_

#include <queue>
#include <string>

#include "../../utils/misc/Types.hpp"
#include "../../transport/bt/Blob.hpp"

class CMessage
{
public:
   static Blob getNextBlob();
   static void releaseCurrentBlob(const Blob &blob);

   static void generateInitialMessage();
   static void generateFinalMessage();
   static void generateSingleMessage(std::string payload);
   static void generateMultipleMessages(std::string payload, int messagesQuantity);

private:

   static std::queue<Blob> blobQueue;

   //static void write();
   //static void generate();

   static void dispayField();

   static UInt8 sVersion;
   static UInt8 sCompressedFlag;
   static UInt8 sFrameType;
   static UInt8 sServiceType;
   static UInt8 sFrameData;
   static UInt8 sSessionID;
   static UInt32 sDataSize;
   static UInt32 sMessageID;

   static void* sPacketData;


};

namespace Bluetooth
{
   static const Blob getBuffer()
   {
      return CMessage::getNextBlob();
   }

   static void releaseBuffer(const Blob& blob)
   {
      CMessage::releaseCurrentBlob(blob);
   }
}

#endif /* СMESSAGE_HPP_ */


